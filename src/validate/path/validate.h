/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
#ifndef PATHVALIDATE_H
#define PATHVALIDATE_H
#include "../../defs/types.h"
#include "../../logger/logger.h"
#include <httplib/httplib.h>
/**
 * @file validate.h
 *
 * @brief Contains defintions for Validation functions and their custom return structs
 *
 */

struct PathValidReturnMap
{
    std::string algo;

    MapInt mapInt;
    MapBool mapBool;

    std::pair<int, int> start;
    std::pair<int, int> goal;
};

struct PathValidReturnGraph
{
    std::string algo;

    GraphUnweighted graphUnweight;
    GraphWeighted graphWeighted;

    unsigned int start;
    unsigned int goal;
};

/**
 * @brief Check if a request matches the requirements
 *
 * @details Checks for: the algo URL parameter being present, the algo URL parameter being
 *          equal to an implemented algorithm, the body containing an MAP array and a
 *          START and GOAL parameter, the number of map elements not exceeding the
 *          implemented max, the map not being empty, the map rows not being the same
 *          length, the start or goal being out of the map bounds, an element being below
 *          -1 for dijkstra/ASTAR/GBFS, an element not being higher then the implemented
 *          maximum for dijkstra/ASTAR/GBFS and the map not containing any zero values for
 *          ASTAR
 *
 * @param req A httplib::Request
 *
 * @param state The global StateClass logger
 *
 * @throws On a case of the request not meeting a requirement
 *
 * @returns A custom return struct containing all needed variables for the algorithm to
 * execute
 * */
PathValidReturnMap ValidatePathMap(const httplib::Request &req, StateClass &state);

/**
 * @brief Check if a request matches the requirements
 *
 * @details Checks for: The URL algo parameter being present, the url algo parameter being
 *          equal to one of the implemented pathfinding algorithms, the body containing
 *          the
 *          parameters GRAPH (as an array), START and GOAL, checking if the total element
 *          count is
 *          higher then the implemeted max, the min/max not being lower/higher then
 *          constraints for
 *          specific algos, not containing any 0 values for ASTAR
 *
 * @param req A httplib::Request
 *
 * @param state The global StateClass logger
 *
 * @throws On a case of the request not meeting a requirement
 *
 * @returns A custom return struct containing all needed variables for the algorithm to
 execute
 *
 */
PathValidReturnGraph ValdiatePathGraph(const httplib::Request &req, StateClass &state);
#endif