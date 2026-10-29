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

//--------------------------MAP BASED--------------------------------------

/**
 * @brief Run the ASTAR algorithm on a Map
 *
 * @param map A 2D vector with integers in the format of [x][y]=weight
 *
 * @param start A std::pair of int in the format of x,y containing the start tile
 *
 * @param goal A std::pair of in in the format of x,y specifiying the goal tile
 *
 * @throws hopefully never
 *
 * @returns A vector of std::pair of integers containing a step by step instruction for
 * the path
 *
 */
std::vector<std::pair<int, int>> ASTAR(const MapInt &map, std::pair<int, int> start,
                                       std::pair<int, int> goal, json &response);

/**
 * @brief Run a Bi Directional Breadth first search on A Map
 *
 * @param map A MapBool object in the format map[x][y]=true/false, with true meaning
 * blocked and false meaning passable
 *
 * @param start A std::pair of integers containing the cooridnates of the start in format
 * {x,y}
 *
 * @param goal A std::pair of integers containing the cooridnates of the goal in format
 * {x,y}
 *
 * @throws Hopefully never
 *
 * @returns The Path in format of a step to step by x,y
 */
std::vector<std::pair<int, int>> BIBFS(const MapBool &map, std::pair<int, int> start,
                                       std::pair<int, int> goal, json &response);

/**
 * @brief Run a BreadthFirst search on A Map
 *
 * @param map A MapBool object in the format map[x][y]=true/false, with true meaning
 * blocked and false meaning passable
 *
 * @param start A std::pair of integers containing the cooridnates of the start in format
 * {x,y}
 *
 * @param goal A std::pair of integers containing the cooridnates of the goal in format
 * {x,y}
 *
 * @throws Hopefully never
 *
 * @returns The Path in format of a step to step by x,y
 */
std::vector<std::pair<int, int>> BreadthFirstSearch(const MapBool &map,
                                                    std::pair<int, int> start,
                                                    std::pair<int, int> goal,
                                                    json &response);

/**
 * @brief Run a Depth First search on A Map
 *
 * @param map A MapBool object in the format map[x][y]=true/false, with true meaning
 * blocked and false meaning passable
 *
 * @param start A std::pair of integers containing the cooridnates of the start in format
 * {x,y}
 *
 * @param goal A std::pair of integers containing the cooridnates of the goal in format
 * {x,y}
 *
 * @throws Hopefully never
 *
 * @returns The Path in format of a step to step by x,y
 */
std::vector<std::pair<int, int>> DepthFirstSearch(const MapBool &map,
                                                  std::pair<int, int> start,
                                                  std::pair<int, int> goal,
                                                  json &response);

/**
 * @brief Run the Djikstra algorithm on a Map
 *
 * @param map A 2D vector with integers in the format of [x][y]=weight
 *
 * @param start A std::pair of int in the format of x,y containing the start tile
 *
 * @param goal A std::pair of in in the format of x,y specifiying the goal tile
 *
 * @throws hopefully never
 *
 * @returns A vector of std::pair of integers containing a step by step instruction for
 * the path
 *
 */
std::vector<std::pair<int, int>> Dijkstra(const MapInt &map, std::pair<int, int> start,
                                          std::pair<int, int> goal, json &response);

/**
 * @brief Run the Greedy Best First Search algorithm on a Map
 *
 * @param map A 2D vector with integers in the format of [x][y]=weight
 *
 * @param start A std::pair of int in the format of x,y containing the start tile
 *
 * @param goal A std::pair of in in the format of x,y specifiying the goal tile
 *
 * @throws hopefully never
 *
 * @returns A vector of std::pair of integers containing a step by step instruction for
 * the path
 *
 */
std::vector<std::pair<int, int>> GBFS(const MapInt &map, std::pair<int, int> start,
                                      std::pair<int, int> goal, json &response);

//--------------------GRAPH BASED---------------------------------------------

/**
 * @brief Execute a BreadFirst Search on a given graph
 *
 * @param graph A Graph object with the layout graph[ID]={connections}
 *
 * @param start A integer specifiying which graph ID is the start
 *
 * @param end A integer specifiying which graph ID is the goal
 *
 * @param response A json object used for setting the response
 *
 * @throws Hopefully never
 *
 * @note This was written after the map based version, so comments used there arent used
 * here, so if your struggling to first try to understand the map based version (even tho
 * thats harder but that ones one me)
 *
 * @note Yeah this version is WAY easier then the other one
 *
 * @returns The Path in steps from NodeID to NodeID
 * */
std::vector<int> BreadthFirstSearch(const GraphUnweight &graph, int start, int goal,
                                    json &response);

std::vector<int> ASTAR(const GraphWeighted &graph, int start, int goal, json &response);

std::vector<int> BIBFS(const GraphUnweight &graph, const int start, const int goal,
                       json &response);

std::vector<int> DepthFirstSearch(const GraphUnweight &graph, int start, int goal,
                                  json &response);

std::vector<int> Dijkstra(const GraphWeighted &graph, int start, int goal,
                          json &response);

std::vector<int> GBFS(const GraphWeighted &graph, int start, int goal, json &response);
#endif