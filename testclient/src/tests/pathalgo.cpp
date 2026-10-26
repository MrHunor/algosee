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
#include "../../src/defs/general.h"
#include "../../src/defs/testcases.h"
#include "../../src/logger/logger.h"
#include "../utils/utils.h"
#include <cmath>
#include <string>
#include <utility>

void runPathalgoTest(std::vector<run> &exitMessage, StateClass &state)
{
    for (const auto &testcase : testCasesPathfinding)
    {

        run currentrun;
        currentrun.failure = false;
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
        std::vector<std::vector<int>> map(
            testcase.second.second,
            std::vector<int>(testcase.second.first, mapInitValue));
        try
        {
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
            std::string command =
                "curl -X POST \"http://127.0.0.1:8080/pathalgo?algo=" +
                currentrun.algoname + "\"" +
                " -H \"Content-Type: application/json\""
                " -d '{\"MAP\": " +
                mapString + ",\"START\": [" + std::to_string(start.first) + "," +
                std::to_string(start.second) + "],\"GOAL\": [" +
                std::to_string(goal.first) + "," + std::to_string(goal.second) + "]}'";
            std::string output = executeCommand(command);
            state.out("Recived", 0);
            json response = parseNDJSON(output);
            state.out("Parsed.", 0);
            checkForAllResponseTypes(response);
            checkForCertainResponseType(
                response, implementedAlgoExpectedResponseType.at(currentrun.algoname));
            std::vector<std::pair<int, int>> path = response["PATH"];

            if(path.empty()||path.front()!=start||path.back()!=goal)throw std::runtime_error("Path is either empty, does not start at start or does not end at end.");
            for (int i = 0; i < path.size(); i++)
            {
                if (i == 0)
                {
                    if (path[i] != start)
                    {
                        currentrun.failure = true;
                        i = path.size();
                    }
                    continue;
                }
                if (i == path.size() - 1)
                {
                    if (path[i] != goal)
                    {
                        currentrun.failure = true;
                        i = path.size();
                        continue;
                    }
                }
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
            exitMessage.push_back(currentrun);
        }
    }
}