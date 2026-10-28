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

/**
 * @file algo.h
 *
 * @brief Header containing pathfinding algorithm defintions
 *
 */
#ifndef PALGOS_H
#define PALGOS_H
#include "../defs/general.h"
#include "../defs/types.h"
#include <nlohmann/json.hpp>
#include <vector>

std::vector<std::pair<int, int>> BreadthFirstSearch(const MapBool &map,
                                                    std::pair<int, int> start,
                                                    std::pair<int, int> goal,
                                                    json &response);
std::vector<std::pair<int, int>> Dijkstra(const MapInt &map,
                                          std::pair<int, int> start,
                                          std::pair<int, int> goal, json &response);
std::vector<std::pair<int, int>> DepthFirstSearch(const MapBool &map,
                                                  std::pair<int, int> start,
                                                  std::pair<int, int> goal,
                                                  json &response);
std::vector<std::pair<int, int>> BIBFS(const MapBool &map,
                                       std::pair<int, int> start,
                                       std::pair<int, int> goal, json &response);
std::vector<std::pair<int, int>> ASTAR(const MapInt &map,
                                       std::pair<int, int> start,
                                       std::pair<int, int> goal, json &response);
std::vector<std::pair<int, int>> GBFS(const MapInt &map,
                                      std::pair<int, int> start, std::pair<int, int> goal,
                                      json &response);
#endif