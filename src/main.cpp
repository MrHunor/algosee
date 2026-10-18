/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano
 * Zapata.
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

#include "build_info.h"
#include "defs/general.h"
#include "defs/types.h"
#include "logger/logger.h"
#include "routing/routing.h"

#include "utils/general/utils.h"
#include <httplib.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>

int main(int argc, char *argv[])
{
    remove("log.txt");
    stateClass state;
    state.verbose = 0;
    state.out("algosee Copyright (C) 2026  MrHunor, siryanni (as equals)\n"
              "This program comes with ABSOLUTELY NO WARRANTY; for details "
              "visit:'https://www.gnu.org/licenses/gpl-3.0.en.html'.\n"
              "This is free software, and you are welcome to redistribute it\n"
              "under certain conditions; visit "
              "'https://www.gnu.org/licenses/gpl-3.0.en.html' for details.\n",
              0, RED);
    state.out(QUOTE, 0, BRIGHT_BLUE);

    state.out("Starting...", 0);
    state.out("VERSION:" + std::string(ALGOSEE_VERSION), 0);
    state.out("Build on:" + std::string(ALGOSEE_BUILD_TIME), 0);
    httplib::Server server;
    clientListType clientList;
    std::mutex clientListMutex;
    //--------------------------PREROUTING HANDLING-------------------
    server.set_pre_routing_handler(
        [&state, &clientList, &clientListMutex](const httplib::Request &req, httplib::Response &res) {
            return RunPreflightHandler(req, res, state, clientList, clientListMutex);
        });

    //-------------------------POSTROUTING HANDLING-------------------------
    // this runs before sending any response
    server.set_post_routing_handler([&state](const httplib::Request &req, httplib::Response &res) {
        state.out("Target:" + req.target + "\nBody:" + req.body, 0);

        res.headers.erase("Access-Control-Allow-Origin");
        res.set_header("Access-Control-Allow-Origin", "*");
    });

    //--------------------------OPTIONS-----------------------------------------
    // handle a preflight request from a diffrent origin; firefox & co. considers
    // diffrent ports on the same computer to be diffrent origins so this fix is
    // needed to work
    const auto serverOptionHandler =
        [](const httplib::Request &req,
           httplib::Response &res) { // first time using callbacks like this kinda nervous >_<
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.status = 200;
        };

    server.Options("(/sortalgo)", serverOptionHandler);
    server.Options("(/pathalgo)", serverOptionHandler);
    server.Options("(/status)", serverOptionHandler);
    server.Options("(/image)", serverOptionHandler);
    server.Options("(/limits)", serverOptionHandler);

    server.set_payload_max_length(10000000);

    //--------------------------LIMITS----------------------------------
    server.Get("/limits", [&](const httplib::Request &req, httplib::Response &res) {
        RunLimits(res, state);
        return;
    });

    //--------------------------IMAGE-----------------------------------------------------------
    server.Post("/image", [&](const httplib::Request &req, httplib::Response &res) {
        StreamClass stream;
        res.set_chunked_content_provider("application/json", [&state, &stream](size_t, httplib::DataSink &sink) {
            while (true)
            {
                json chunk;

                std::unique_lock<std::mutex> lock(
                    stream.writeMutex); // has to be a unique_locl because the cv_notify.wait() operations needs one

                stream.cv_notify.wait(lock, [&] {
                    return !stream.chunks.empty() ||
                           stream.finished; // if either evaluate to true then the wait is finished, so if chunks is
                                            // not empty or if the stream is finished
                });

                if (stream.chunks.empty() && stream.finished)
                    break; // queue is empty and stream is finished, exit out of loop and exit out of handler

                chunk = std::move(stream.chunks.front());
                stream.chunks.pop(); // move the first chunk to chunk and delete from queue
                if (!sink.write(chunk.dump().c_str(),
                                chunk.dump().size())) //.write should evaluate to true is the sink is still
                                                      // viable, if not something bad happend
                {
                    stream.finish();
                    return false; // bad happend, return false (this case is weird)
                }
            }
            sink.done();
            return true; // everything fine, normal exit return true
        });

        RunImageSort(req, res, state);
        return;
    });

    //---------------------------STATUS---------------------------------------------------------------------
    server.Get("/status", [&](const httplib::Request &req, httplib::Response &res) {
        RunStatus(req, res, state);
        return;
    });

    //------------------------------SORTALGO---------------------------------------------------------------
    server.Post("/sortalgo", [&](const httplib::Request &req, httplib::Response &res) {
        StreamClass stream;
        res.set_chunked_content_provider("application/json", [&state, &stream](size_t, httplib::DataSink &sink) {
            while (true)
            {
                json chunk;

                std::unique_lock<std::mutex> lock(
                    stream.writeMutex); // has to be a unique_locl because the cv_notify.wait() operations needs one

                stream.cv_notify.wait(lock, [&] {
                    return !stream.chunks.empty() ||
                           stream.finished; // if either evaluate to true then the wait is finished, so if chunks is
                                            // not empty or if the stream is finished
                });

                if (stream.chunks.empty() && stream.finished)
                    break; // queue is empty and stream is finished, exit out of loop and exit out of handler

                chunk = std::move(stream.chunks.front());
                stream.chunks.pop(); // move the first chunk to chunk and delete from queue
                if (!sink.write(chunk.dump().c_str(),
                                chunk.dump().size())) //.write should evaluate to true is the sink is still
                                                      // viable, if not something bad happend
                {
                    stream.finish();
                    return false; // bad happend, return false (this case is weird)
                }
            }
            sink.done();
            return true; // everything fine, normal exit return true
        });
        RunSortalgo(req, res, state,stream);
        return;
    });

    //------------------------------PATHALGO----------------------------------------------------------------
    server.Post("/pathalgo", [&](const httplib::Request &req, httplib::Response &res) {
        RunPathalgo(req, res, state);
        StreamClass stream;
        res.set_chunked_content_provider("application/json", [&state, &stream](size_t, httplib::DataSink &sink) {
            while (true)
            {
                json chunk;

                std::unique_lock<std::mutex> lock(
                    stream.writeMutex); // has to be a unique_locl because the cv_notify.wait() operations needs one

                stream.cv_notify.wait(lock, [&] {
                    return !stream.chunks.empty() ||
                           stream.finished; // if either evaluate to true then the wait is finished, so if chunks is
                                            // not empty or if the stream is finished
                });

                if (stream.chunks.empty() && stream.finished)
                    break; // queue is empty and stream is finished, exit out of loop and exit out of handler

                chunk = std::move(stream.chunks.front());
                stream.chunks.pop(); // move the first chunk to chunk and delete from queue
                if (!sink.write(chunk.dump().c_str(),
                                chunk.dump().size())) //.write should evaluate to true is the sink is still
                                                      // viable, if not something bad happend
                {
                    stream.finish();
                    return false; // bad happend, return false (this case is weird)
                }
            }
            sink.done();
            return true; // everything fine, normal exit return true
        });
        return;
    });

    // render port detection magic
    const char *port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 8080;

    state.out("Listining on:0.0.0.0:" + std::to_string(port), 0);
    if (!server.listen("0.0.0.0", port))
    {
        InvalidInputMessage("Failed to listen on 0.0.0.0:" + std::to_string(port));
    }

    return 0;
}
