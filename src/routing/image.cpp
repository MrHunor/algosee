/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
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
#include "../utils/image/image.h"
#include "../logger/logger.h"
#include "../sortalgo/algos.h"
#include "../utils/general/utils.h"
#include "../validate/image/validate.h"
#include "build_info.h"
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <stb_image.h>

void runImageSort(const httplib::Request &req, httplib::Response &res,
                  stateClass &state) {
  try {
    json response;
    response["VERSION"] = ALGOSEE_VERSION;
    response["BUILDTIME"] = ALGOSEE_BUILD_TIME;
    ImageValidReturn info = validateImage(req, state);
    shuffleImage(info.img);
    response["SHUFFLED (INDEXES)"]=indexPixelArrToJson(info.img.data);
    state.out("Starting time mesurement...", 0);
    auto startTime = std::chrono::steady_clock::now();

    if (info.algo == "selection")
      selectionSort(info.img.data, response);
    else if (info.algo == "cycle")
      cycleSort(info.img.data, response);
    else if (info.algo == "bubble")
      bubbleSort(info.img.data, response);
    else if (info.algo == "quick")
      quickSort(info.img.data, 0, info.img.data.size() - 1, response);
    else if (info.algo == "merge")
      mergeSort(info.img.data, response);
    else if (info.algo == "counting") {
      countingSort(info.img.data, response);
    } else if (info.algo == "bogo") {
      bogoSort(info.img.data, response);
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        endTime - startTime);
    state.out("Ended Time mesurement.", 0);
    response["TIME"] = elapsed.count();
    response["SORTED"]=indexPixelArrToJson(info.img.data);

    res.status = 200;
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
    state.out("Sending response...", 0);
  } catch (const std::exception &e) {
    state.out("Cought an exception:" + std::string(e.what()), 0);
    returnFailedAnswer(res, std::string(e.what()));
  }
  return;
}
