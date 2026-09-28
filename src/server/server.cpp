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
#include "../utils/utils.h"
#include <chrono>
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

std::string checkMapValidness(json parsed, std::pair<int, int> start,
                              std::pair<int, int> goal) {
  int collums = parsed["MAP"][0].size();
  bool sameRowLength = true;
  for (int i = 1; i < parsed["MAP"].size(); i++) {
    if (parsed["MAP"][i].size() != collums) {
      sameRowLength = false;
      break;
    }
  }
  if (!sameRowLength) {
    return "Rows have diffrent lengths.";
  }

  if (start.first < 0 || start.first >= parsed["MAP"][0].size() ||
      start.second < 0 || start.second >= parsed["MAP"].size() ||
      goal.first < 0 || goal.first >= parsed["MAP"][0].size() ||
      goal.second < 0 || goal.second >= parsed["MAP"].size()) {
    return "Start or goal coordinates invalid (out of map).";
  }
  return "";
}

std::vector<std::vector<bool>> castIntMapToBoolIfNeeded(const json &input) {
  bool isBool = true;
  // check if input array is already boolean
  try {
    for (const auto &row : input["MAP"]) {
      for (const auto &element : row) {
        if (!element.is_boolean())
          isBool = false;
      }
    }

  } catch (const std::exception &e) {
    return {};
  }

  if (isBool == true)
    return input["MAP"];

  std::vector<std::vector<bool>> retval;
  std::vector<bool> retvalRow;
  for (const auto& row : input["MAP"]) {
    retvalRow.clear();
    for (const auto& element : row) {
      if (element == 0)
        retvalRow.push_back(false);
      else if (element == 1)
        retvalRow.push_back(true);
      else
        return {};
    }
    retval.push_back(retvalRow);
  }
  return retval;
}


void afterSelfTestRun(std::string algorithmName,
                      std::vector<int> &paramPassValues,
                      std::vector<int> &testvalues,
                      std::vector<int> &sortedValues, json &response,
                      json &parampassTest,
                      const std::chrono::steady_clock::time_point &startTime) {
  // end time measurement
  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
  // cleanup
  parampassTest.clear();
  // check if sort worked
  if (paramPassValues == sortedValues) {
    response[algorithmName + " STATUS"] = "SUCESS";
    response[algorithmName + " TIME"] = elapsed.count();
  } else {
    response[algorithmName + " STATUS"] = "FAILED";
    response[algorithmName + " TIME"] = elapsed.count();
  }

  // reset passvalues to testvalues
  paramPassValues = testvalues;
}

void RunStatus(const httplib::Request &req, httplib::Response &res,
               stateClass &state) {
  json response;
  response["STATUS"] = "ONLINE";
  response["VERION"] = "VERSION";
  if (req.has_header("X-Forwarded-For")) {
    state.out("Status request from:" + req.get_header_value("X-Forwarded-For"),
              0);
  } else {
    state.out(req.remote_addr, 0);
  }
  state.out("Send status signal.", 0);
  res.status = 200;
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
}

void RunSelftest(const httplib::Request &req, httplib::Response &res,
                 stateClass &state) {

  state.out("Initating selftest...", 0);
  json response;
  response["VERSION"] = VERSION;
  int n = 1;
  if (req.has_header("X-Forwarded-For")) {
    state.out("recived selftest request from:" +
                  req.get_header_value("X-Forwarded-For"),
              0);
  } else {
    state.out("recived selftest request from:" + req.remote_addr, 0);
  }

  if (!req.has_param("n")) {
    returnFailedAnswer(res, "No amount parameter n provided.", 400);
    return;
  }

  try {
    n = std::stoi(req.get_param_value("n"));

  } catch (const std::invalid_argument &) {
    returnFailedAnswer(res, "Provided n is not a valid integer", 422);
    return;
  } catch (const std::out_of_range &) {
    returnFailedAnswer(res, "Provided n is too big", 422);
    return;
  }

  if (n > MAX_ELEMENT_COUNT) {
    returnFailedAnswer(
        res,
        "Provided n value exceeds the max limit set for algorithms (" +
            std::to_string(MAX_ELEMENT_COUNT) + ")",
        413);
    return;
  }

  response["VERSION"] = VERSION;
  std::vector<int> testvalues(n);
  std::iota(testvalues.begin(), testvalues.end(), 0); // fill with testvalues
  std::vector<int> passParamValues = testvalues;
  std::vector<int> sortedvalues = testvalues;
  std::sort(sortedvalues.begin(), sortedvalues.end());
  std::vector<std::vector<int>> tries;
  std::vector<std::pair<int, int>> moves;
  json passParamResponse;

  auto startTime = std::chrono::steady_clock::now();
  selectionSort(passParamValues, passParamResponse);
  afterSelfTestRun("Selection Sort", passParamValues, testvalues, sortedvalues,
                   response, passParamResponse, startTime);

  if (n < MAX_ELEMENT_COUNT_BOGO) {
    startTime = std::chrono::steady_clock::now();
    bogoSort(passParamValues, passParamResponse);
    afterSelfTestRun("Bogo Sort", passParamValues, testvalues, sortedvalues,
                     response, passParamResponse, startTime);
  } else
    response["Bogo Sort"] = "N too large to execute bogo sort without likely "
                            "running out of memory, skipped this test.";

  startTime = std::chrono::steady_clock::now();
  bubbleSort(passParamValues, passParamResponse);
  afterSelfTestRun("Bubble Sort", passParamValues, testvalues, sortedvalues,
                   response, passParamResponse, startTime);

  startTime = std::chrono::steady_clock::now();
  quickSort(passParamValues, 0, passParamValues.size() - 1, passParamResponse);
  afterSelfTestRun("Quick Sort", passParamValues, testvalues, sortedvalues,
                   response, passParamResponse, startTime);

  startTime = std::chrono::steady_clock::now();
  mergeSort(passParamValues, passParamResponse);
  afterSelfTestRun("Merge Sort", passParamValues, testvalues, sortedvalues,
                   response, passParamResponse, startTime);

  startTime = std::chrono::steady_clock::now();
  countingSort(passParamValues, passParamResponse);
  afterSelfTestRun("Counting Sort", passParamValues, testvalues, sortedvalues,
                   response, passParamResponse, startTime);

  res.status = 200;
  state.out("Sending reply...", 0);
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
}

void runSortalgo(const httplib::Request &req, httplib::Response &res,
                 stateClass &state) {
  std::string ip;
  if (req.has_header("X-Forwarded-For")) {
    ip = req.get_header_value("X-Forwarded-For");
  } else {
    ip = req.remote_addr;
  }

  state.out("Recived Request:\nClientIP:" + ip + "\nTarget:" + req.target +
                "\nBody:" + req.body,
            0);

  state.out("Parasing values....", 0);

  std::vector<int> values;
  try {
    auto parsed = json::parse(req.body);

    values = parsed["values"].get<std::vector<int>>();
    if (values.empty()) {
      returnFailedAnswer(res, "No values specified. (values.empty()==true)",
                         400);
      return;
    }
    if (values.size() > MAX_ELEMENT_COUNT) {
      returnFailedAnswer(res,
                         "Too many elements specified. Max element count:" +
                             std::to_string(MAX_ELEMENT_COUNT),
                         413);
      return;
    }

  } catch (const std::exception &e) {
    returnFailedAnswer(res,
                       "Failed to parse request body. Exception details:" +
                           std::string(e.what()),
                       400);
    return;
  }

  state.out("Finished.", 0);

  json response;
  response["VERSION"] = VERSION;

  std::string algo = req.get_param_value("algo");
  state.out("Parsed algo:" + algo, 0);

  if (!implementedSortAlgos.contains(algo)) {
    returnFailedAnswer(res, "The provided algo could no be found.", 501);
    return;
  }

  state.out("Starting time mesurement...", 0);
  auto startTime = std::chrono::steady_clock::now();

  if (algo == "selection")
    selectionSort(values, response);
  else if (algo == "cycle")
    cycleSort(values, response);
  else if (algo == "bubble")
    bubbleSort(values, response);
  else if (algo == "quick")
    quickSort(values, 0, values.size() - 1, response);
  else if (algo == "merge")
    mergeSort(values, response);
  else if (algo == "counting") {
    bool hasNegNumbers = std::any_of(values.begin(), values.end(),
                                     [](int x) { // some weird callback shit
                                       return x < 0;
                                     });
    if (hasNegNumbers) {
      returnFailedAnswer(
          res,
          "Counting sort does not allow negative numbers. See Issue #39 on "
          "github.com/mrhunor/algosee/issues for more info.",
          501);
      return;
    }
    countingSort(values, response);
  } else if (algo == "bogo") {
    if (values.size() > MAX_ELEMENT_COUNT_BOGO) {
      returnFailedAnswer(
          res, "Aborted early:Too many elements specified, answer would "
               "likely exceed the memory limit of the server");
      return;
    }

    bogoSort(values, response);
  }

  // Default for all algorithms exit
  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
  state.out("Ended Time mesurement.", 0);
  response["TIME"] = elapsed.count();

  response["SORTED"] = values;

  res.status = 200;
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
  state.out("Sending response...", 0);
  return;
}

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
