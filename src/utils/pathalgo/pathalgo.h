/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, Vilmos45 (as equals)
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
/**
 * @file pathalgo.h
 *
 * @brief Contains function defintions for utilitiy functions for pathfinding algorithms
 *
 */

#ifndef PATHALGO_UTILS_H
#define PATHALGO_UTILS_H
#include "../../defs/general.h"
#include "../../defs/types.h"
#include <cstdint>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

/**
 * @brief Swap the 2 Dimensions in a 2D MapBool Array
 *
 * @param visited The MapBool array to swap
 *
 * @throws Never
 *
 * @returns The swapped input array as a json object
 * */
json SwapVisitedArray(const MapBool &visited);

json SwapVisitedArray(const std::vector<std::vector<uint64_t>> &visited);

/**
 * @brief Check maps internal validness
 *
 * @details Checks if the Map is empty, if all rows are the same length and if the start
 * or goal is out of bounds
 *
 * @param mapInt A Map Int to analyse
 *
 * @param start A std::pair of int desciribing the start
 *
 * @param goal A std::pair of int descirbing the goal
 *
 * @throws If a map is invalid
 *
 * @returns Nothig
 *
 * */
void CheckMapValidness(const MapInt &mapInt, const std::pair<int, int> &start,
                       const std::pair<int, int> &goal);
void CheckMapValidness(const MapBool &map, const std::pair<int, int> &start,
                       const std::pair<int, int> &goal);
GraphUnweighted buildReverseGraph(const GraphUnweighted &arr);
std::pair<int64_t, uint64_t> CheckRange(const MapInt &map);
std::pair<int64_t, uint64_t> CheckRange(const GraphWeighted &graph);
std::vector<std::vector<bool>> CastIntMapToBoolIfNeeded(const json &input);
bool CheckMapForZero(const MapInt &map);
bool CheckMapForZero(const GraphWeighted &graph);
#endif