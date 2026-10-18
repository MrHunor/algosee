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
#include "../utils/image/image.h"
#include "../exception/exception.h"
#include "../logger/logger.h"
#include "../sortalgo/algos.h"
#include "../utils/general/utils.h"
#include "../validate/image/validate.h"
#include "build_info.h"
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <stb_image.h>

void RunImageSort(const httplib::Request &req, httplib::Response &res, stateClass &state)
{
    try
    {
        json response;
        response["VERSION"] = ALGOSEE_VERSION;
        response["BUILDTIME"] = ALGOSEE_BUILD_TIME;
        ImageValidReturn info = ValidateImage(req, state);
        ShuffleImage(info.img);
        response["SHUFFLED (INDEXES)"] = IndexPixelArrToJson(info.img.data);
        state.out("Starting time mesurement...", 0);
        auto startTime = std::chrono::steady_clock::now();

        if (info.algo == "selection")
            SelectionSort(info.img.data, response);
        else if (info.algo == "cycle")
            CycleSort(info.img.data, response);
        else if (info.algo == "bubble")
            BubbleSort(info.img.data, response);
        else if (info.algo == "quick")
            QuickSort(info.img.data, 0, info.img.data.size() - 1, response);
        else if (info.algo == "merge")
            MergeSort(info.img.data, response);
        else if (info.algo == "counting")
        {
            CountingSort(info.img.data, response);
        }
        else if (info.algo == "bogo")
        {
            BogoSort(info.img.data, response);
        }
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        state.out("Ended Time mesurement.", 0);
        response["TIME"] = elapsed.count();
        response["SORTED"] = IndexPixelArrToJson(info.img.data);

        res.status = 200;
        res.set_header("Access-Control-Allow-Origin", "*");
        std::string responseString = std::move(response.dump());
        state.out("Sending response:" + responseString, 0);
        res.set_content(responseString, "application/json");
    }
    catch (const excep &e)
    {
        state.out(
            "Cought a excep, Details:" + std::string(e.what()) + "\n" + "Status Code:" + std::to_string(e.exitCode), 0);
        ReturnFailedAnswer(res, std::string(e.what()), e.exitCode);
    }
    catch (const std::exception &e)
    {
        state.out("Cought an std::exception, Details:" + std::string(e.what()), 0);
        ReturnFailedAnswer(res, std::string(e.what()));
    }
    return;
}
