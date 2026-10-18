/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#ifndef STREAM_H
#define STREAM_H
#include "../defs/general.h"
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class StreamClass
{
  public:
    std::mutex writeMutex; // locks the thread to prevent data race
    std::condition_variable
        cv_notify; // notifys a thread waiting for new chunks to be written (so the stream callback in httplib)
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

    void finish()
    {
        std::lock_guard<std::mutex> lock(writeMutex);
        finished = true;
        cv_notify.notify_all(); // notify all waiting threads to finish operations
    }
};

#endif