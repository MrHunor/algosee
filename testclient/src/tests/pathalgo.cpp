/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
#include "../../src/defs/general.h"
#include "../../src/defs/implemented.h"
#include "../../src/defs/testcases.h"
#include "../../src/logger/logger.h"
#include "../utils/utils.h"
#include <cmath>
#include <httplib.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <utility>

void runPathalgoTest(std::vector<run> &exitMessage, httplib::Client &client,
                     StateClass &state)
{
    for (const auto &testcase : testCasesPathfindingMap)
    {

        run currentrun;
        currentrun.failure = false;
        currentrun.algotype = "Pathfinding Map";
        currentrun.algoname = testcase.first;
        currentrun.size = -1;
        currentrun.mapSize = testcase.second;
        unsigned short mapInitValue = 0;
        state.out("Testing path validness for:" + currentrun.algoname +
                      " on Mapsize:" + std::to_string(currentrun.mapSize.first) + "x" +
                      std::to_string(currentrun.mapSize.second),
                  0);
        if (currentrun.algoname == "ASTAR")
        { // A* needs non zero map values so init a diffrent vector
            mapInitValue = 1;
        }
        std::vector<std::vector<int>> map(testcase.second.second,
                                          std::vector<int>(testcase.second.first, 0));

        try
        {
            for (auto &row : map)
            {
                for (auto &element : row)
                {
                    if (implementedPathAlgoCatagory.at(currentrun.algoname) == 'W')
                    {
                        element =
                            randomInt(mapInitValue, std::numeric_limits<u_int8_t>::max());
                    }
                    else
                    {
                        element = mapInitValue;
                    }
                }
            }
            std::string mapString;
            mapString += "[";
            for (const auto &row : map)
            {
                mapString += "[";
                for (const auto &element : row)
                {
                    mapString += std::to_string(element);
                    mapString += ",";
                }
                mapString.erase(mapString.size() - 1, 1);
                mapString += "]";
                mapString += ",";
            }
            mapString.erase(mapString.size() - 1, 1);
            mapString += "]";
            std::pair<int, int> start = {0, 0};
            std::pair<int, int> goal = {testcase.second.first - 1,
                                        testcase.second.second - 1};

            json reqBody = {{"MAP", map}, {"START", start}, {"GOAL", goal}};

            auto res = client.Post("/pathalgoMap?algo=" + currentrun.algoname,
                                   reqBody.dump(), "application/json");

            if (!res)
                throw std::runtime_error("Request failed.");
            if (res->status != 200)
                throw std::runtime_error(
                    "Non 200 Code returned:\nerrorCode:" + std::to_string(res->status) +
                    "\nserver details:" + res->body);

            state.out("Recived", 0);
            json response = json::parse(res->body);
            state.out("Parsed.", 0);
            checkForAllResponseTypes(response);
            checkForCertainResponseType(
                response, implementedAlgoExpectedResponseType.at(currentrun.algoname));
            std::vector<std::pair<int, int>> path = response["PATH"];

            if (path.empty() || path.front() != start || path.back() != goal)
                throw std::runtime_error("Path is either empty, does not start at start "
                                         "or does not end at end.");
            for (int i = 1; i < path.size(); i++)
            {

                if (std::abs(path[i].first - path[i - 1].first) > 1 ||
                    std::abs(path[i].second - path[i - 1].second) > 1)
                {
                    currentrun.failure = true;
                    i = path.size();
                    continue;
                }
            }
            exitMessage.push_back(currentrun);
        }
        catch (const std::exception &e)
        {
            state.out("Path test failed for " + currentrun.algoname + ": " + e.what(), 0);
            currentrun.failure = true;
            currentrun.failureReason = e.what();
            exitMessage.push_back(currentrun);
        }
    }
    for (const auto &testcase : testCasesPathfindingGraph)
    {
        run currentrun;
        currentrun.failure = false;
        currentrun.algotype = "Pathfinding Graph";
        currentrun.algoname = testcase.first;
        currentrun.size = testcase.second;
        currentrun.mapSize = {-1, -1};
        try
        {
            // i know a lot of this branch is copy paste but it serves for a
            // cleaner split and better readability
            if (implementedPathAlgoCatagory.at(currentrun.algoname) == 'U')
            {
                GraphUnweighted graph;
                state.out("Testing path validness for:" + currentrun.algoname +
                              "\nSize:" + std::to_string(currentrun.size),
                          0);

                graph = generateGU(currentrun.size, randomInt(0, currentrun.size - 1),
                                   randomInt(0, currentrun.size - 1));

                int start = randomInt(0, currentrun.size - 1);
                int goal = randomInt(0, currentrun.size - 1);
                json reqBody = {{"GRAPH", graph},
                                {
                                    "START",
                                    start,
                                },
                                {"GOAL", goal}};
                auto res = client.Post("/pathalgoGraph?algo=" + currentrun.algoname,
                                       reqBody.dump(), "application/json");

                if (!res)
                    throw std::runtime_error("Request failed.");
                if (res->status != 200)
                    throw std::runtime_error("Non 200 Code returned:\nerrorCode:" +
                                             std::to_string(res->status) +
                                             "\nserver details:" + res->body);

                state.out("Recived", 0);
                json response = json::parse(res->body);
                state.out("Parsed.", 0);
                checkForAllResponseTypes(response);
                checkForCertainResponseType(
                    response,
                    implementedAlgoExpectedResponseType.at(currentrun.algoname));
                std::vector<int> path = response["PATH"];

                if (path.empty() || path.front() != start || path.back() != goal)
                    throw std::runtime_error(
                        "Path is either empty, does not start at start "
                        "or does not end at end.");
                for (int i = 0; i < path.size() - 1; i++)
                {
                    bool foundNext = false;
                    for (const auto &element : graph[path[i]])
                    {
                        if (element == path[i + 1])
                            foundNext = true;
                    }
                    if (!foundNext)
                        throw std::runtime_error(
                            "While investigating path element:" + std::to_string(i) +
                            ", No connection was found thats the same as the next path "
                            "element");
                }
                exitMessage.push_back(currentrun);
            }
            if (implementedPathAlgoCatagory.at(currentrun.algoname) == 'W')
            {
                      GraphWeighted graph;
                state.out("Testing path validness for:" + currentrun.algoname +
                              "\nSize:" + std::to_string(currentrun.size),
                          0);
                int weightLowerBound=0;
                if(currentrun.algoname == "ASTAR")weightLowerBound=1;
                graph = generateGW(currentrun.size, randomInt(0, currentrun.size - 1),
                                   randomInt(0, currentrun.size - 1),weightLowerBound);

                int start = randomInt(0, currentrun.size - 1);
                int goal = randomInt(0, currentrun.size - 1);
                json reqBody = {{"GRAPH", graph},
                                {
                                    "START",
                                    start,
                                },
                                {"GOAL", goal}};
                auto res = client.Post("/pathalgoGraph?algo=" + currentrun.algoname,
                                       reqBody.dump(), "application/json");

                if (!res)
                    throw std::runtime_error("Request failed.");
                if (res->status != 200)
                    throw std::runtime_error("Non 200 Code returned:\nerrorCode:" +
                                             std::to_string(res->status) +
                                             "\nserver details:" + res->body);

                state.out("Recived", 0);
                json response = json::parse(res->body);
                state.out("Parsed.", 0);
                checkForAllResponseTypes(response);
                checkForCertainResponseType(
                    response,
                    implementedAlgoExpectedResponseType.at(currentrun.algoname));
                std::vector<int> path = response["PATH"];

                if (path.empty() || path.front() != start || path.back() != goal)
                    throw std::runtime_error(
                        "Path is either empty, does not start at start "
                        "or does not end at end.");
                for (int i = 0; i < path.size() - 1; i++)
                {
                    bool foundNext = false;
                    for (const auto &element : graph[path[i]])
                    {
                        if (element.first == path[i + 1])
                            foundNext = true;
                    }
                    if (!foundNext)
                        throw std::runtime_error(
                            "While investigating path element:" + std::to_string(i) +
                            ", No connection was found thats the same as the next path "
                            "element");
                }
                exitMessage.push_back(currentrun);
            }
        }
        catch (const std::exception &e)
        {
            state.out("Path test failed for " + currentrun.algoname + ": " + e.what(), 0);
            currentrun.failure = true;
            currentrun.failureReason = e.what();
            exitMessage.push_back(currentrun);
        }
    }
}