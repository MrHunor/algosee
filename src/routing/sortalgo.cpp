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
#include "../exception/exception.h"
#include "../logger/logger.h"
#include "../sortalgo/algos.h"
#include "../utils/general/utils.h"
#include "../validate/sort/validate.h"
#include <chrono>
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

void RunSortalgo(SortValidReturn info, StateClass &state, StreamClass &stream)
{
    try
    {
        json response;
        generateJsonRetrunArras(response);
        fillWithMetaData(response);
        stream.sendChunk(std::move(response));
        response.clear();

        state.out("Starting time mesurement...", 0);
        auto startTime = std::chrono::steady_clock::now();

        if (info.algo == "selection")
            SelectionSort(info.values, response, stream);
        else if (info.algo == "cycle")
            CycleSort(info.values, response, stream);
        else if (info.algo == "bubble")
            BubbleSort(info.values, response, stream);
        else if (info.algo == "quick")
            QuickSort(info.values, 0, info.values.size() - 1, response, stream);
        else if (info.algo == "merge")
            MergeSort(info.values, response, stream);
        else if (info.algo == "counting")
        {
            CountingSort(info.values, response, stream);
        }
        else if (info.algo == "bogo")
        {
            BogoSort(info.values, response, stream);
        }

        // Default for all algorithms exit
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
        state.out("Ended Time mesurement.", 0);
        response["TIME"] = elapsed.count();
        response["SORTED"] = info.values;
        response["ERROR"] = "";
        response["ERRORCODE"] = 0;

        stream.sendChunk(std::move(response));
        response.clear();
        stream.finish();
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
        state.out("Cought an exception:" + std::string(e.what()), 0);
        ReturnFailedAnswerStreamOnly(std::string(e.what()), 400, &stream);
    }
    return;
}