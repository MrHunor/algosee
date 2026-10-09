/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#include "../utils/pathalgo/pathalgo.h"
#include "../logger/logger.h"
#include "../pathalgo/algos.h"
#include "../utils/general/utils.h"
#include "../validate/path/validate.h"
#include "build_info.h"
#include <chrono>
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

void RunPathalgo(const httplib::Request &req, httplib::Response &res,
                 stateClass &state) {
  try {
    // variables
    std::vector<std::pair<int, int>> path;
    json response;
    response["VERSION"] = ALGOSEE_VERSION;
    response["BUILDTIME"] = ALGOSEE_BUILD_TIME;
    bool sameRowLength = true;
    int collums;
    // log ip

    // parsing
    state.out("Processing values....", 0);
    PathValidReturn info = validatePath(req, state);

    state.out("Starting time mesurement...", 0);
    auto startTime = std::chrono::steady_clock::now();

    if (info.algo == "BFS" || info.algo == "DFS" || info.algo == "BIBFS") {

      // this could very likely throw
      if (info.algo == "BFS")
        path =
            BreadthFirstSearch(info.mapBool, info.start, info.goal, response);
      else if (info.algo == "DFS")
        path = DepthFirstSearch(info.mapBool, info.start, info.goal, response);
      else if (info.algo == "BIBFS")
        path = BIBFS(info.mapBool, info.start, info.goal, response);
    }

    if (info.algo == "dijkstra")
      path = Dijkstra(info.mapInt, info.start, info.goal, response);

    auto endTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        endTime - startTime);
    state.out("Ended time measurement.", 0);
    response["TIME"] = elapsed.count();

    if (path.empty()) {
      throw excep("No valid path found from start to goal.");
    }
    res.status = 200;
    response["PATH"] = path;
    state.out("Finished. Replying;\nresponse:" + response.dump(), 0);
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
  } catch (const std::exception &e) {
    state.out("Cought an exception:" + std::string(e.what()), 0);
    returnFailedAnswer(res, std::string(e.what()));
  }
  return;
}
