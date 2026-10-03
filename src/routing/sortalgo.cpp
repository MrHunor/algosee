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
#include "../validate/sort/validate.h"
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

  state.out("Processing values....", 0);
  SortValidReturn info;
  info = validateSort(req, state);
  if (info.failure) {
    returnFailedAnswer(res, info.failureString);
    return;
  }

  state.out("Finished.", 0);

  json response;
  response["VERSION"] = VERSION;

  state.out("Starting time mesurement...", 0);
  auto startTime = std::chrono::steady_clock::now();

  if (info.algo == "selection")
    selectionSort(info.values, response);
  else if (info.algo == "cycle")
    cycleSort(info.values, response);
  else if (info.algo == "bubble")
    bubbleSort(info.values, response);
  else if (info.algo == "quick")
    quickSort(info.values, 0, info.values.size() - 1, response);
  else if (info.algo == "merge")
    mergeSort(info.values, response);
  else if (info.algo == "counting") {
    countingSort(info.values, response);
  } else if (info.algo == "bogo") {
    bogoSort(info.values, response);
  }

  // Default for all algorithms exit
  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
  state.out("Ended Time mesurement.", 0);
  response["TIME"] = elapsed.count();

  response["SORTED"] = info.values;

  res.status = 200;
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
  state.out("Sending response...", 0);
  return;
}