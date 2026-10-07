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
#include "../sortalgo/algos.h"
#include "../utils/general/utils.h"
#include <chrono>
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

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
    if(!parsed.contains("values"))
    {
      returnFailedAnswer(res,"No value parameter. (request.contains(values)==false)",413);
      return;
    }
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