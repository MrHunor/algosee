/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include "sortalgo/algos.h"
#include "utils/defs.h"
#include "utils/utils.h"
#include <chrono>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "server/server.h"

using json = nlohmann::json;
int main(int argc, char *argv[]) {
  stateClass state;

  state.out("algosee @ https://github.com/MrHunor/algosee\n"
            "GNU General Public License v3 (GPLv3) © 2026 MrHunor,siryanni "
            "(as equals)\n"
            "This Program is provided \"AS IS\" without warranty of any kind.",
            0, RED);

  state.out("Starting...", 0);

  httplib::Server server;

  // handle a preflight request from a diffrent origin; firefox considers
  // diffrent ports on the same computer to be diffrent origins so this fix is
  // needed to work
  server.Options(
      R"(/sortalgo)", [](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 200;
      });

  // Server status endpoint
  server.Get("/status",
             [&](const httplib::Request &req, httplib::Response &res) {
               json status = {{"status", "online"}};
               state.out("Send status signal.", 0);
               res.status = 200;
               res.set_header("Access-Control-Allow-Origin", "*");
               res.set_content(status.dump(4), "application/json");
             });

  server.Get("/selftest",[&](const httplib::Request &req, httplib::Response &res)     {      
    state.out("Initating selftest...",0);
    json response;
    response["VERSION"]= VERSION;
    std::vector<int> testvalues = {5,3,1,2,6,4};
    std::vector<int> passParamValues = testvalues;
    std::vector<int> sortedvalues = {1,2,3,4,5,6};
    std::vector<std::vector<int>> tries; 
    std::vector<std::pair<int,int>> moves;
    json passParamTest;

    auto startTime = std::chrono::steady_clock::now();
    selectionSort(passParamValues,moves);
    afterSelfTestRun("Selection Sort", passParamValues, testvalues, sortedvalues, moves,tries, response, passParamTest, startTime);

    startTime = std::chrono::steady_clock::now();
    bogoSort(passParamValues, tries);
    afterSelfTestRun("Bogo Sort", passParamValues, testvalues, sortedvalues, moves,tries, response, passParamTest, startTime);

    startTime = std::chrono::steady_clock::now();
    bubbleSort(passParamValues, moves);
    afterSelfTestRun("Bubble Sort", passParamValues, testvalues, sortedvalues, moves,tries, response, passParamTest, startTime);
   
    startTime = std::chrono::steady_clock::now();
    quickSort(passParamValues, 0, passParamValues.size()-1, moves);
    afterSelfTestRun("Quick Sort", passParamValues, testvalues, sortedvalues, moves,tries, response, passParamTest, startTime);
    
    startTime = std::chrono::steady_clock::now();
    mergeSort(passParamValues, passParamTest);
    afterSelfTestRun("Merge Sort", passParamValues, testvalues, sortedvalues, moves,tries, response, passParamTest, startTime);
    
        startTime = std::chrono::steady_clock::now();
    passParamValues = countingSort(passParamValues,passParamTest);
    afterSelfTestRun("Counting Sort", passParamValues, testvalues, sortedvalues, moves,tries, response, passParamTest, startTime);
    
    res.status=400;
    res.set_content(response.dump(),"application/json");
    return;  
  });
  server.Post("/sortalgo", [&](const httplib::Request &req,
                               httplib::Response &res) {
    state.out("Recived Request:\nTarget:" + req.target + "\nBody:" + req.body,
              0);

    auto parsed = json::parse(req.body);
    std::vector<int> values = parsed["values"];
    if (values.empty()) {
      returnFailedAnswer(res, "No values specified. (values.empty()==true)");
      return;
    }

    if (!req.has_param("algo")) {
      returnFailedAnswer(res, "Request URL does not contain a algo parameter.");
      return;
    }

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
      state.out("Finished sorting, sending reply...", 0);
      res.set_header("Access-Control-Allow-Origin", "*");
      response["TIME"] = elapsed.count();
      response["MOVES"] = moves;
      res.set_content(response.dump(), "application/json");
      return;
    }

    if (algo == "bogo") {

      std::vector<std::vector<int>> tries;
      if (values.size() > 12) {
        returnFailedAnswer(
            res, "Aborted early:Too many elements specified, request would be "
                 "too big. (>12 Elements > 479.001.600 Possibility; which is "
                 "rougly >26GB in RAM for recording tries)");
        return;
      }

      bogoSort(values, tries);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Finished sorting, sending reply...", 0);
      res.set_header("Access-Control-Allow-Origin", "*");
      response["TIME"] = elapsed.count();
      response["TRIES"] = tries;
      response["SORTED"] = values;
      res.set_content(response.dump(), "application/json");
      return;
    }

    if (algo == "bubble") {
      bubbleSort(values, moves);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      state.out("Finished sorting, sending reply...", 0);
      res.set_header("Access-Control-Allow-Origin", "*");
      response["TIME"] = elapsed.count();
      response["MOVES"] = moves;
      response["SORTED"] = values;
      res.set_content(response.dump(), "application/json");
      return;
    }

    if (algo == "merge") {
      mergeSort(values, response);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      response["TIME"] = elapsed.count();
      response["SORTED"] = values;
      state.out("Finished sorting, sending reply...", 0);
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      return;
    }

    if (algo == "counting") {
      countingSort(values, response);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      response["TIME"] = elapsed.count();
      response["SORTED"] = values;
      state.out("Finished sorting, sending reply...", 0);
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      return;
    }

    if (algo == "quick") {
      quickSort(values, 0, values.size() - 1, moves);
      auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
      response["TIME"] = elapsed.count();
      response["MOVES"] = moves;
      response["SORTED"]=values;
      state.out("Finished sorting, sending reply...", 0);
      res.set_header("Access-Control-Allow-Origin", "*");
      res.set_content(response.dump(), "application/json");
      return;
    }

    returnFailedAnswer(res, "Unknown Algorithm");
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
