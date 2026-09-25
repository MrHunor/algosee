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
#include "../pathalgo/algos.h"
#include "../sortalgo/algos.h"
#include "../utils/defs.h"
#include "../utils/utils.h"
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <thread>

using json = nlohmann::json;

void afterSelfTestRun(std::string algorithmName,
                      std::vector<int> &paramPassValues,
                      std::vector<int> &testvalues,
                      std::vector<int> &sortedValues,
                      json &response,
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
  response["STATUS"]="ONLINE";
  response["VERION"]="VERSION";
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
  response["VERSION"]=VERSION;
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

  } catch (const std::invalid_argument&) {
    returnFailedAnswer(res, "Provided n is not a valid integer",422);
    return;
   } catch (const std::out_of_range&) {
    returnFailedAnswer(res, "Provided n is too big",422);
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
  selectionSort(passParamValues,passParamResponse );
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
  quickSort(passParamValues, 0, passParamValues.size() - 1,passParamResponse);
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

  state.out("parasing values....", 0);
  auto parsed = json::parse(req.body);
  std::vector<int> values = parsed["values"];
  if (values.empty()) {
    returnFailedAnswer(res, "No values specified. (values.empty()==true)", 400);
    return;
  }
  if (values.size() > MAX_ELEMENT_COUNT) {
    returnFailedAnswer(res,
                       "Too many elements specified. Max element count:" +
                           std::to_string(MAX_ELEMENT_COUNT),
                       413);
    return;
  }

  if (!req.has_param("algo")) {
    returnFailedAnswer(res, "Request URL does not contain a algo parameter.",
                       400);
    return;
  }

  state.out("Finished.", 0);

  json response;
  response["VERSION"] = VERSION;

  std::string algo = req.get_param_value("algo");
  state.out("Parsed algo:" + algo, 0);

  if (!implementedSortAlgos.contains(algo)) {
     returnFailedAnswer(res, "The provided algo could no be found.",501);
  return;
  }

  state.out("Starting time mesurement...", 0);
  auto startTime = std::chrono::steady_clock::now();

  

    if (algo == "selection")
      selectionSort(values, response);
    else if (algo =="cycle")
       cycleSort(values,response);
    else if (algo == "bubble")
      bubbleSort(values, response);
    else if (algo == "quick")
      quickSort(values, 0, values.size() - 1, response);
    else if (algo == "merge")
      mergeSort(values, response);
    else if (algo == "counting")
    {
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
    countingSort(values,response);
    } 
    else if (algo == "bogo")
    {
       if (values.size() > MAX_ELEMENT_COUNT_BOGO) {
      returnFailedAnswer(
          res, "Aborted early:Too many elements specified, answer would "
               "likely exceed the memory limit of the server");
      return;
    }

    bogoSort(values, response);
    }

  
  

  //Default for all algorithms exit
  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
  std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
  state.out("Ended Time mesurement.", 0);
  response["TIME"] = elapsed.count();
  
  response["SORTED"]=values;

  res.status = 200;
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
  state.out("Sending response...", 0);
  return;

}

void RunPathalgo(const httplib::Request &req, httplib::Response &res,
                 stateClass &state) {
  
  //variables
  std::string ip;
  std::vector<std::pair<int,int>> path;
  json response;
  std::string algo;
  bool sameRowLength=true;
  int collums;
  //log ip 
  if (req.has_header("X-Forwarded-For")) {
    ip = req.get_header_value("X-Forwarded-For");
  } else {
    ip = req.remote_addr;
  }

  state.out("Recived Request:\nClientIP:" + ip + "\nTarget:" + req.target +
                "\nBody:" + req.body,
            0);

  //parsing            
  state.out("parsing values....", 0);
  response["VERSION"] = VERSION;
  auto parsed = json::parse(req.body);
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

  std::pair<int, int> start = parsed["START"];
  std::pair<int, int> goal = parsed["GOAL"];
  state.out("Finished.", 0);

  //input validation

collums= parsed["MAP"][0].size();
  for(int i = 1; i<parsed["MAP"].size();i++)
  {
   if(parsed["MAP"][i].size()!=collums)
   {
    sameRowLength=false;
    break;
   }
  }
  if(!sameRowLength)
  {
    returnFailedAnswer(res, "Map contains rows of diffrent lengths.",442);
    return;
  }

  if(start.first<0||start.first>=parsed["MAP"][0].size()
   ||start.second<0 || start.second>=parsed["MAP"].size()
   ||goal.first<0||goal.first>=parsed["MAP"][0].size()
   ||goal.second<0 || goal.second>=parsed["MAP"].size()
  )
  {
   returnFailedAnswer(res, "Start or goal coordinates invalid (out of map).",422);
   return;
  }

  

   algo = req.get_param_value("algo");
  state.out("Parsed algo:" + algo, 0);

  if (!implementedPathAlgos.contains(algo)) {
     returnFailedAnswer(res, "The provided algo could no be found.",442);
  return;
  }
  
  state.out("Starting time mesurement...", 0);
  auto startTime = std::chrono::steady_clock::now();

  



  if (algo == "BFS") {
    std::vector<std::vector<bool>> map = parsed["MAP"];
    if (map.empty()) {
      returnFailedAnswer(res, "No values specified. (map.empty()==true)", 400);
      return;
    }
    path = BreadthFirstSearch(map, start, goal, response);

  }
  if(algo == "dijkstra")
  {
    std::vector<std::vector<int>> map = parsed["MAP"];
    if (map.empty()) {
      returnFailedAnswer(res, "No values specified. (map.empty()==true)", 400);
      return;
    }
    path = Dijkstra(map, start, goal, response);
  }


    auto endTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        endTime - startTime);
    state.out("Ended time measurement.", 0);
    response["TIME"] = elapsed.count();
    if (path.empty()) {
      returnFailedAnswer(
          res, "No Valid path from start to goal could be found.", 500);
      return;
    }
    res.status = 200;
    response["PATH"] = path;
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
    return;
}