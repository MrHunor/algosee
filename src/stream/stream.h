/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.(root/LICENSE)  If not, see
 * <https://www.gnu.org/licenses/>.
 */
/**
 * @file
 *
 * @brief Delares StreamClass and streaming related functions
 *
 */
#ifndef STREAM_H
#define STREAM_H
#include "../defs/general.h"

#include <httplib.h>
#include <mutex>
#include <queue>

/**
 * @brief Class that handles streaming to a client
 *
 * @param None, no constructor here
 *
 * @throws Never
 *
 * @returns Nothing, no constructor
 *
 * */
class StreamClass
{
  public:
    std::mutex writeMutex; // locks the thread to prevent data race
    std::condition_variable
        cv_notify; // notifys a thread waiting for new chunks to be
                   // written (so the stream callback in httplib, see MakeChunkProvider )
    std::queue<json> chunks;
    bool finished = false;

    void sendChunk(json chunk) // adds to the class queue of chunks
    {
        std::lock_guard<std::mutex> lock(writeMutex);
        if (finished)
            return;
        chunks.push(std::move(chunk));
        cv_notify.notify_one(); // notify a thread that new data is avialable
    }

    void finish() // finish the stream
    {
        std::unique_lock<std::mutex> lock(writeMutex);
        finished = true;
        cv_notify.notify_all(); // notify all waiting threads to finish operations
    }
};

/**
 * @brief Create a Lamda to pass to httplib::cpp set contentprovider
 *
 * @param stream A shared_ptr to a StreamClass to create the Chunk Provider for (lifetime
 * dynamically managed because of shared_ptr)
 *
 * @throws The lamda creator function never, the lamda itself should also never throw
 *
 * @returns A lamda that takes a size_t and a httplib::Datasink as arguments
 */
httplib::ContentProviderWithoutLength MakeChunkProvider(
    std::shared_ptr<StreamClass> stream);
#endif