/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include "server/server.h"
#include "sortalgo/algos.h"
#include "pathalgo/algos.h"
#include "utils/defs.h"
#include "utils/utils.h"
#include <chrono>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <numeric>
#include <vector>

using json = nlohmann::json;
int main(int argc, char *argv[]) {
  stateClass state;
  state.verbose=0;
  state.out("algosee @ https://github.com/MrHunor/algosee\n"
            "GNU General Public License v3 (GPLv3) © 2026 MrHunor,siryanni "
            "(as equals)\n"
            "This Program is provided \"AS IS\" without warranty of any kind.",
            0, RED);

  state.out("Starting...", 0);

  httplib::Server server;

  // handle a preflight request from a diffrent origin; firefox & co. considers
  // diffrent ports on the same computer to be diffrent origins so this fix is
  // needed to work
  server.Options(
      R"(/sortalgo)", [](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 200;
      });
 server.Options(
      R"(/pathalgo)", [](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 200;
      });


  //---------------------------STATUS---------------------------------------------------------------------
  server.Get("/status", [&](const httplib::Request &req,
                            httplib::Response &res) {
    json response = {{"status", "online"}};
    if (req.has_header("X-Forwarded-For")) {
      state.out(
          "Status request from:" + req.get_header_value("X-Forwarded-For"), 0);
    } else {
      state.out(req.remote_addr, 0);
    }
    state.out("Send status signal.", 0);
    res.status = 200;
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
  });

  //------------------------------SELFTEST---------------------------------------------------------------

  server.Get("/selftest", [&](const httplib::Request &req,
                              httplib::Response &res) {
    state.out("Initating selftest...", 0);
    json response;
    if (req.has_header("X-Forwarded-For")) {
      state.out("recived selftest request from:" +
                    req.get_header_value("X-Forwarded-For"),
                0);
    } else {
      state.out("recived selftest request from:" + req.remote_addr, 0);
    }

    if(!req.has_param("n"))
    {
      returnFailedAnswer(res, "No amount parameter n provided.",400);
      return;
    }
    
    int n = std::stoi(req.get_param_value("n"));

    if(n> MAX_ELEMENT_COUNT)
    {
      returnFailedAnswer(res, "Provided n value exceeds the max limit set for algorithms ("+std::to_string(MAX_ELEMENT_COUNT)+")",413);
      return;
    }

    response["VERSION"] = VERSION;
    std::vector<int> testvalues(n);
    std::iota(testvalues.begin(),testvalues.end(),0);//fill with testvalues
    std::vector<int> passParamValues = testvalues;
    std::vector<int> sortedvalues=testvalues;
    std::sort(sortedvalues.begin(),sortedvalues.end());
    std::vector<std::vector<int>> tries;
    std::vector<std::pair<int, int>> moves;
    json passParamTest;

    auto startTime = std::chrono::steady_clock::now();
    selectionSort(passParamValues, moves);
    afterSelfTestRun("Selection Sort", passParamValues, testvalues,
                     sortedvalues, moves, tries, response, passParamTest,
                     startTime);

    if(n<MAX_ELEMENT_COUNT_BOGO)
    {
    startTime = std::chrono::steady_clock::now();
    bogoSort(passParamValues, tries);
    afterSelfTestRun("Bogo Sort", passParamValues, testvalues, sortedvalues,
                     moves, tries, response, passParamTest, startTime);
    }else response["Bogo Sort"]="N too large to execute bogo sort without likely running out of memory, skipped this test.";

    startTime = std::chrono::steady_clock::now();
    bubbleSort(passParamValues, moves);
    afterSelfTestRun("Bubble Sort", passParamValues, testvalues, sortedvalues,
                     moves, tries, response, passParamTest, startTime);

    startTime = std::chrono::steady_clock::now();
    quickSort(passParamValues, 0, passParamValues.size() - 1, moves);
    afterSelfTestRun("Quick Sort", passParamValues, testvalues, sortedvalues,
                     moves, tries, response, passParamTest, startTime);

    startTime = std::chrono::steady_clock::now();
    mergeSort(passParamValues, passParamTest);
    afterSelfTestRun("Merge Sort", passParamValues, testvalues, sortedvalues,
                     moves, tries, response, passParamTest, startTime);

    startTime = std::chrono::steady_clock::now();
    passParamValues = countingSort(passParamValues, passParamTest);
    afterSelfTestRun("Counting Sort", passParamValues, testvalues, sortedvalues,
                     moves, tries, response, passParamTest, startTime);

    res.status = 200;
    state.out("Sending reply...", 0);
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
    return;
  });

  //------------------------------SORTALGO---------------------------------------------------------------
  server.Post("/sortalgo", [&](const httplib::Request &req,
                               httplib::Response &res) {
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
      returnFailedAnswer(res, "No values specified. (values.empty()==true)",
                         400);
      return;
    }
    if(values.size()>MAX_ELEMENT_COUNT)
    {
      returnFailedAnswer(res, "Too many elements specified. Max element count:"+std::to_string(MAX_ELEMENT_COUNT),413);
    }

    if (!req.has_param("algo")) {
      returnFailedAnswer(res, "Request URL does not contain a algo parameter.",
                         400);
      return;
    }

    state.out("Finished.", 0);

    json response;
    response["VERSION"] = VERSION;
    std::vector<std::pair<int, int>> moves;
    std::string algo = req.get_param_value("algo");
    state.out("Parsed algo:" + algo, 0);
    state.out("Starting time mesurement...", 0);
    auto startTime = std::chrono::steady_clock::now();

    if (algo == "selection") {
      selectionSort(values, moves);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Ended Time mesurement.", 0);
      state.out("Crafting response...", 0);
      response["TIME"] = elapsed.count();
      response["MOVES"] = moves;
      res.set_header("Access-Control-Allow-Origin", "*");
      res.status = 200;
      res.set_content(response.dump(), "application/json");
      state.out("Finished.", 0);
      state.out("Sending response...", 0);
      return;
    }

    if (algo == "bogo") {

      std::vector<std::vector<int>> tries;
      if (values.size() > MAX_ELEMENT_COUNT_BOGO) {
        returnFailedAnswer(
            res, "Aborted early:Too many elements specified, answer would "
                 "likely exceed the memory limit of the server");
        return;
      }

      bogoSort(values, tries);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Ended Time mesurement.", 0);
      state.out("Crafting response...", 0);
      response["TIME"] = elapsed.count();
      response["TRIES"] = tries;
      response["SORTED"] = values;
      res.set_header("Access-Control-Allow-Origin", "*");
      res.status = 200;
      res.set_content(response.dump(), "application/json");
      state.out("Finished.", 0);
      state.out("Sending response...", 0);
      return;
    }

    if (algo == "bubble") {
      bubbleSort(values, moves);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Ended Time mesurement.", 0);
      state.out("Crafting response...", 0);
      response["TIME"] = elapsed.count();
      response["MOVES"] = moves;
      response["SORTED"] = values;
      res.status = 200;
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      state.out("Finished.", 0);
      state.out("Sending response...", 0);
      return;
    }

    if (algo == "merge") {
      mergeSort(values, response);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Ended Time mesurement.", 0);
      state.out("Crafting response...", 0);
      response["TIME"] = elapsed.count();
      response["SORTED"] = values;
      res.status = 200;
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      state.out("Finished.", 0);
      state.out("Sending response...", 0);
      return;
    }

    if (algo == "counting") {
    bool hasNegNumbers   = std::any_of(values.begin(), values.end(), [](int x) {//some weird callback shit
    return x < 0;
    });
    if(hasNegNumbers)
    {
      returnFailedAnswer(res,"Counting sort does not allow negative numbers. See Issue #39 on github.com/mrhunor/algosee/issues for more info.",501);
      return;
    }
      values = countingSort(values, response);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Ended Time mesurement.", 0);
      state.out("Crafting response...", 0);
      response["TIME"] = elapsed.count();
      response["SORTED"] = values;
      res.status = 200;
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      state.out("Finished.", 0);
      state.out("Sending response...", 0);
      return;
    }

    if (algo == "quick") {
      quickSort(values, 0, values.size() - 1, moves);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Ended Time mesurement.", 0);
      state.out("Crafting response...", 0);
      response["TIME"] = elapsed.count();
      response["MOVES"] = moves;
      response["SORTED"] = values;
      state.out("Finished sorting, sending reply...", 0);
      res.status = 200;
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      state.out("Finished.", 0);
      state.out("Sending response...", 0);
      return;
    }

    returnFailedAnswer(res, "Unknown Algorithm", 501);
  });

  //------------------------------PATHALGO----------------------------------------------------------------
  server.Post("/pathalgo",[&](const httplib::Request &req,
                               httplib::Response &res) {

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
    json response;
    response["VERSION"]=VERSION;
    auto parsed = json::parse(req.body);
    if(!parsed.contains("MAP")||!parsed.contains("START")||!parsed.contains("GOAL"))
    {
      returnFailedAnswer(res, "Request does not include needed values. Either MAP,START or GOAL is missing",400);
      return;
    }

    std::vector<std::vector<bool>> map = parsed["MAP"];
    std::pair<int,int> start = parsed["START"];
    std::pair<int,int> end = parsed["GOAL"];
    state.out("Finished.", 0);
   

    if (map.empty()) {
      returnFailedAnswer(res, "No values specified. (values.empty()==true)",
                         400);
      return;
    }

    if (!req.has_param("algo")) {
      returnFailedAnswer(res, "Request URL does not contain a algo parameter.",
                         400);
      return;
    }

    std::string algo = req.get_param_value("algo");
    state.out("Parsed algo:" + algo, 0);
    state.out("Starting time mesurement...", 0);
    auto startTime = std::chrono::steady_clock::now();

    if(algo=="BFS")
    {
      auto retval = BreadthFirstSearch(map, start, end);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
      endTime - startTime);
      state.out("Ended time measurement.",0);
      response["TIME"]=elapsed.count();
      if(retval.empty()){
      returnFailedAnswer(res,"No Valid path from start to goal could be found.",500);
      return;
      }
      res.status=200;
      response["PATH"]=retval;
            res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(),"application/json");
      return;
    }



  });
  // renderer port detection magic
  const char *port_env = std::getenv("PORT");
  int port = port_env ? std::stoi(port_env) : 8080;

  state.out("Listining on:0.0.0.0:" + std::to_string(port), 0);
  if (!server.listen("0.0.0.0", port)) {
    InvalidInputMessage("Failed to listen on 0.0.0.0:" + std::to_string(port));
  }

  return 0;
}
