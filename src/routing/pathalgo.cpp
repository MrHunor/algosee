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
#include "../logger/logger.h"
#include "../pathalgo/algos.h"
#include "../sortalgo/algos.h"
#include "../utils/pathalgo/pathalgo.h"
#include "../utils/general/utils.h"
#include <chrono>
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

void RunPathalgo(const httplib::Request &req, httplib::Response &res,
                 stateClass &state) {

  // variables
  std::string ip;
  std::vector<std::pair<int, int>> path;
  json response;
  response["VERSION"] = VERSION;
  std::string algo;
  bool sameRowLength = true;
  int collums;
  // log ip
  if (req.has_header("X-Forwarded-For")) {
    ip = req.get_header_value("X-Forwarded-For");
  } else {
    ip = req.remote_addr;
  }

  state.out("Recived Request:\nClientIP:" + ip + "\nTarget:" + req.target +
                "\nBody:" + req.body,
            0);

  // parsing
  state.out("Parsing values....", 0);

  json parsed;
  std::pair<int, int> start;
  std::pair<int, int> goal;
  try {
    parsed = json::parse(req.body);
    if (!parsed.contains("MAP") || !parsed.contains("START") ||
        !parsed.contains("GOAL")) {
      returnFailedAnswer(res,
                         "Request does not include needed values. Either "
                         "MAP,START or GOAL is missing",
                         400);
      return;
    }

    if (!req.has_param("algo")) {
      returnFailedAnswer(res, "Request URL does not contain a algo parameter.",
                         400);
      return;
    }

    start = parsed["START"];
    goal = parsed["GOAL"];
    state.out("Finished.", 0);

  } catch (const std::exception &e) {
    returnFailedAnswer(res,
                       "Failed to parse request body. Exception details:" +
                           std::string(e.what()),
                       400);
    return;
  }

  // input validation
  checkMapValidness(parsed, start, goal);

  algo = req.get_param_value("algo");
  state.out("Parsed algo:" + algo, 0);

  if (!implementedPathAlgos.contains(algo)) {
    returnFailedAnswer(res, "The provided algo could no be found.", 442);
    return;
  }

  state.out("Starting time mesurement...", 0);
  auto startTime = std::chrono::steady_clock::now();

  if (algo == "BFS" || algo == "DFS" || algo == "BIBFS") {

    // this could very likely throw
    std::vector<std::vector<bool>> map;
    try {
      map = castIntMapToBoolIfNeeded(parsed);
    } catch (const std::exception &e) {
      returnFailedAnswer(
          res,
          "Failed to read map from req into vector. Exception details:" +
              std::string(e.what()),
          500);
    }

    if (map.empty()) {
      returnFailedAnswer(res, "No values specified. (map.empty()==true)", 400);
      return;
    }
    state.out("Casted input map:\n" + parsed["MAP"].dump() + "\n to:\n", 0);
    for (const auto row : map) {
      for (const auto value : row) {
        std::cout << value;
      }
      std::cout << std::endl;
    }

    if (algo == "BFS")
      path = BreadthFirstSearch(map, start, goal, response);
    else if (algo == "DFS")
      path = DepthFirstSearch(map, start, goal, response);
    else if (algo == "BIBFS")
      path = BIBFS(map, start, goal, response);
  }

  if (algo == "dijkstra") {

    std::vector<std::vector<int>> map;
    try {
       map = parsed["MAP"];
    } catch (const std::exception e) {
      returnFailedAnswer(
          res,
          "Failed to copy map from req to vector. Exception details:" +
              std::string(e.what()),
          500);
    }
    if (map.empty()) {
      returnFailedAnswer(res, "No values specified. (map.empty()==true)", 400);
      return;
    }
    path = Dijkstra(map, start, goal, response);
  }

  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
  state.out("Ended time measurement.", 0);
  response["TIME"] = elapsed.count();
  if (path.empty()) {
    returnFailedAnswer(res, "No Valid path from start to goal could be found.",
                       500);
    return;
  }
  res.status = 200;
  response["PATH"] = path;
  state.out("Finished. Replying;\nresponse:" + response.dump(), 0);
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
  return;
}
