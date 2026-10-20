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
#include "stream.h"
#include "../defs/general.h"
#include <httplib.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <thread>
#include <vector>

httplib::ContentProviderWithoutLength MakeChunkProvider(std::shared_ptr<StreamClass> stream)//this returns a lamda, you cant have auto here because you want to use the lamda somewhere else so you have to explicitly state that this is a contentprovider
{
    return [stream](size_t, httplib::DataSink &sink) {
        while (true)
        {
            json chunk;

            std::unique_lock<std::mutex> lock(
                stream->writeMutex); // has to be a unique_locl because the cv_notify.wait() operations needs one

            stream->cv_notify.wait(lock, [&] {
                return !stream->chunks.empty() ||
                       stream->finished; // if either evaluate to true then the wait is finished, so if chunks is
                                         // not empty or if the stream is finished
            });

            if (stream->chunks.empty() && stream->finished)
                break; // queue is empty and stream is finished, exit out of loop and exit out of handler

            chunk = std::move(stream->chunks.front());
            stream->chunks.pop(); // move the first chunk to chunk and delete from queue
            lock.unlock();        // you have to unlock the lock because stream->finish locks the same mutex again
            const std::string chunkPlusEnd = chunk.dump()+"\n";
            if (!sink.write(chunkPlusEnd.data(),
                            chunkPlusEnd.size())) //.write should evaluate to true is the sink is still
                                                  // viable, if not something bad happend
            {
                stream->finish();
                return false; // bad happend, return false (this case is weird)
            }
        }
        sink.done();
        return true; // everything fine, normal exit return true
    };
}