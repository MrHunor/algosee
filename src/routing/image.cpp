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
#include <algorithm>
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <stb_image.h>

void RunImageSort(ImageValidReturn info, stateClass &state, StreamClass &stream)
{
    try
    {
        json response;
        response["VERSION"] = ALGOSEE_VERSION;
        response["BUILDTIME"] = ALGOSEE_BUILD_TIME;
        ShuffleImage(info.img);
        response["SHUFFLED (INDEXES)"] = IndexPixelArrToJson(info.img.data);
        stream.sendChunk(std::move(response));
        response.clear();
        state.out("Starting time mesurement...", 0);
        auto startTime = std::chrono::steady_clock::now();

        if (info.algo == "selection")
            SelectionSort(info.img.data, response, stream);
        else if (info.algo == "cycle")
            CycleSort(info.img.data, response, stream);
        else if (info.algo == "bubble")
            BubbleSort(info.img.data, response, stream);
        else if (info.algo == "quick")
            QuickSort(info.img.data, 0, info.img.data.size() - 1, response, stream);
        else if (info.algo == "merge")
            MergeSort(info.img.data, response, stream);
        else if (info.algo == "counting")
        {
            CountingSort(info.img.data, response, stream);
        }
        else if (info.algo == "bogo")
        {
            BogoSort(info.img.data, response, stream);
        }
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        state.out("Ended Time mesurement.", 0);
        response["TIME"] = elapsed.count();
        response["SORTED"] = IndexPixelArrToJson(info.img.data);

        state.out("Sending response...", 0);
        stream.sendChunk(std::move(response));
        stream.finish();
        response.clear();
    }
    catch (const excep &e)
    {
        state.out("Cought a excep, Details:" + std::string(e.what()) + "\n" +
                      "Status Code:" + std::to_string(e.exitCode),
                  0);
        ReturnFailedAnswerStreamOnly(std::string(e.what()), e.exitCode, &stream);
    }
    catch (const std::exception &e)
    {
        state.out("Cought an std::exception, Details:" + std::string(e.what()), 0);
        ReturnFailedAnswerStreamOnly(std::string(e.what()), 400, &stream);
    }
    return;
}
