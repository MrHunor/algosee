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
#include "exception/exception.h"
#include "logger/logger.h"
#include "routing/routing.h"

#include "stream/stream.h"
#include "utils/general/utils.h"
#include "validate/image/validate.h"
#include "validate/sort/validate.h"
#include <exception>
#include <httplib.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

int main(int argc, char *argv[])
{
    remove("log.txt");
    StateClass state;
    state.verbose = 0;
    state.out("algosee Copyright (C) 2026  MrHunor, siryanni (as equals)\n"
              "This program comes with ABSOLUTELY NO WARRANTY; for details "
              "visit:'https://www.gnu.org/licenses/gpl-3.0.en.html'.\n"
              "This is free software, and you are welcome to redistribute it\n"
              "under certain conditions; visit "
              "'https://www.gnu.org/licenses/gpl-3.0.en.html' for details.",
              0, RED);
    state.out(QUOTE, 0, BRIGHT_BLUE);

    try
    {
        std::string asciiArt = readFile("../assets/WideTransparentLogoAsciiArt.txt");
        state.out("\n" + asciiArt, 0);
    }
    catch (const std::exception &e)
    {
        state.out("Non fatal exception:" + std::string(e.what()), 0);
    }

    state.out("Starting...", 0);
    state.out("VERSION:" + std::string(ALGOSEE_VERSION), 0);
    state.out("Build on:" + std::string(ALGOSEE_BUILD_TIME), 0);
    httplib::Server server;
    clientListType clientList;
    std::mutex clientListMutex;
    //--------------------------PREROUTING HANDLING-------------------
    server.set_pre_routing_handler(
        [&state, &clientList, &clientListMutex](const httplib::Request &req,
                                                httplib::Response &res) {
            return RunPreflightHandler(req, res, state, clientList, clientListMutex);
        });

    //-------------------------POSTROUTING HANDLING-------------------------
    // this runs before sending any response
    server.set_post_routing_handler(
        [&state](const httplib::Request &req, httplib::Response &res) {
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
           httplib::Response
               &res) { // first time using callbacks like this kinda nervous >_<
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
        auto stream = std::make_shared<StreamClass>(); // see sortalgo
        // extract information from req and res before starting the thread because as soon
        // as the handler returns they will become undefined

        ImageValidReturn info;
        try
        {
            info = ValidateImage(
                req, state); // this could likely throw in the process of validating
        }
        catch (const excep &e)
        {
            state.out("Cought a excep, Details:" + std::string(e.what()) + "\n" +
                          "Status Code:" + std::to_string(e.exitCode),
                      0);
            ReturnFailedAnswerStreamOnly(std::string(e.what()), e.exitCode, &*stream);
            return;
        }
        catch (const std::exception &e)
        {
            state.out("Cought an std::exception, Details:" + std::string(e.what()), 0);
            ReturnFailedAnswerStreamOnly(std::string(e.what()), 400, &*stream);
            return;
        }

        res.set_chunked_content_provider("application/x-ndjson",
                                         MakeChunkProvider(stream));

        std::thread IMGsorter(
            [stream, &state, info] { RunImageSort(info, state, *stream); });
        IMGsorter.detach(); // runs the sorting until finished in a seperate thread,
                            // writing to the sink in the process, allowing the handler to
                            // return and the chunkprovider to stream the chunks to the
                            // client (or atleast thats the thought)

        res.set_header("Access-Control-Allow-Origin",
                       "*"); // must be set here as far as i know
        return;
    });

    //---------------------------STATUS---------------------------------------------------------------------
    server.Get("/status", [&](const httplib::Request &req, httplib::Response &res) {
        RunStatus(req, res, state);
        return;
    });

    //------------------------------SORTALGO---------------------------------------------------------------
    server.Post("/sortalgo", [&](const httplib::Request &req, httplib::Response &res) {
        auto stream = std::make_shared<StreamClass>(); // kinda complicated why this is
                                                       // needed, the problem is that
        // this handler /sortalgo might return and run streams
        // destuctor if defined like: StreamClass stream and the
        // set_chunk_content_provided might call it after that because
        // it still has to return a chunk and because the object has
        // been destroyed the lamda acesses invalid memory. Another
        // specialty is that this should only be needed in this
        // specific callback scenario as these are the two exit points
        // of a valid destructor, so for any other usecase you can
        // dereference the pointer and pass the object Another point
        // you must look out for is that you have to pass stream by
        // value and not by reference because only this way it is
        // shared (so the lifetime is bound to either destructors)

        SortValidReturn info;
        try
        {
            info = ValidateSort(req, state);
        }

        catch (const excep &e)
        {
            state.out("Cought a excep, Details:" + std::string(e.what()) + "\n" +
                          "Status Code:" + std::to_string(e.exitCode),
                      0);
            ReturnFailedAnswerStreamOnly(std::string(e.what()), e.exitCode, &*stream);
            return;
        }
        catch (const std::exception &e)
        {
            state.out("Cought an std::exception, Details:" + std::string(e.what()), 0);
            ReturnFailedAnswerStreamOnly(std::string(e.what()), 400, &*stream);
            return;
        }

        res.set_chunked_content_provider("application/x-ndjson",
                                         MakeChunkProvider(stream));

        std::thread NUMsorter(
            [info, stream, &state] { RunSortalgo(info, state, *stream); });
        NUMsorter.detach();

        res.set_header("Access-Control-Allow-Origin", "*");
        return;
    });

    //------------------------------PATHALGO----------------------------------------------------------------
    server.Post("/pathalgo", [&](const httplib::Request &req, httplib::Response &res) {
        RunPathalgo(req, res, state);
        return;
    });

    try
    {
        // render port detection magic
        const char *port_env = std::getenv("PORT");
        int port = port_env ? std::stoi(port_env) : 8080;

        state.out("Listining on:0.0.0.0:" + std::to_string(port), 0);
        if (!server.listen("0.0.0.0", port))
        {
            throw std::runtime_error("Failed to listen on 0.0.0.0:" +
                                     std::to_string(port));
        }
    }
    catch (const std::exception &e)
    {
        InvalidInputMessage("Server failed to startup with an exception:" +
                            std::string(e.what()));
    }
    return 0;
}
