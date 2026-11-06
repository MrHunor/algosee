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
#ifndef DEF_TESTCASES_H
#define DEF_TESTCASES_H
#include "max_sizes.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
const std::vector<std::pair<std::string, std::pair<int, int>>> testCasesImage{

    {"selection", {5, 5}},
    {"selection", {15, 12}},
    {"selection", {25, 25}},
    {"selection", {25, 12}},
    {"selection",
     {sqrt(MAX_PIXEL_COUNT_IMAGE) - 1,
      sqrt(MAX_PIXEL_COUNT_IMAGE) -
          1}}, // the -1 is there is there in case of weird float to int casting

    {"bubble", {5, 5}},
    {"bubble", {15, 12}},
    {"bubble", {25, 25}},
    {"bubble", {25, 12}},
    {"bubble", {sqrt(MAX_PIXEL_COUNT_IMAGE) - 1, sqrt(MAX_PIXEL_COUNT_IMAGE) - 1}},

    {"quick", {5, 5}},
    {"quick", {15, 12}},
    {"quick", {25, 25}},
    {"quick", {25, 12}},
    {"quick", {sqrt(MAX_PIXEL_COUNT_IMAGE) - 1, sqrt(MAX_PIXEL_COUNT_IMAGE) - 1}},

    {"merge", {5, 5}},
    {"merge", {15, 12}},
    {"merge", {25, 25}},
    {"merge", {25, 12}},
    {"merge", {sqrt(MAX_PIXEL_COUNT_IMAGE) - 1, sqrt(MAX_PIXEL_COUNT_IMAGE) - 1}},

    {"counting", {5, 5}},
    {"counting", {15, 12}},
    {"counting", {25, 25}},
    {"counting", {25, 12}},
    {"counting", {sqrt(MAX_PIXEL_COUNT_IMAGE) - 1, sqrt(MAX_PIXEL_COUNT_IMAGE) - 1}},

    {"cycle", {5, 5}},
    {"cycle", {15, 12}},
    {"cycle", {25, 25}},
    {"cycle", {25, 12}},
    {"cycle", {sqrt(MAX_PIXEL_COUNT_IMAGE) - 1, sqrt(MAX_PIXEL_COUNT_IMAGE) - 1}},

    // bogo is factorial time: keep sizes tiny
    {"bogo", {1, 2}},
    {"bogo", {2, 3}},
    {"bogo", {sqrt(MAX_ELEMENT_COUNT_BOGO) - 1, sqrt(MAX_ELEMENT_COUNT_BOGO) - 1}}};

const std::vector<std::pair<std::string, std::pair<uint8_t, uint8_t>>>
    testCasesPathfindingMap{
        {"BFS", {10, 5}},         {"BFS", {20, 5}},         {"BFS", {100, 45}},
        {"BFS", {250, 150}},      {"BFS", {249, 249}},

        {"dijkstra", {10, 5}},    {"dijkstra", {20, 5}},    {"dijkstra", {100, 45}},
        {"dijkstra", {250, 150}}, {"dijkstra", {249, 249}},

        {"DFS", {10, 5}},         {"DFS", {20, 5}},         {"DFS", {100, 45}},
        {"DFS", {250, 150}},      {"DFS", {249, 249}},

        {"BIBFS", {10, 5}},       {"BIBFS", {20, 5}},       {"BIBFS", {100, 45}},
        {"BIBFS", {250, 150}},    {"BIBFS", {249, 249}},

        {"ASTAR", {10, 5}},       {"ASTAR", {20, 5}},       {"ASTAR", {100, 45}},
        {"ASTAR", {250, 150}},    {"ASTAR", {249, 249}},

        {"GBFS", {10, 5}},        {"GBFS", {20, 5}},        {"GBFS", {100, 45}},
        {"GBFS", {250, 150}},     {"GBFS", {249, 249}}};

const std::vector<std::pair<std::string, uint16_t>> testCasesPathfindingGraph{
    {"BFS", 50},
    {"BFS", 100},
    {"BFS", 4500},
    {"BFS", 37500},
    {"BFS", MAX_ELEMENT_COUNT_BFS - 1},

    {"dijkstra", 50},
    {"dijkstra", 100},
    {"dijkstra", 4500},
    {"dijkstra", 37500},
    {"dijkstra", MAX_ELEMENT_COUNT_DIJKSTRA - 1},

    {"DFS", 50},
    {"DFS", 100},
    {"DFS", 4500},
    {"DFS", 37500},
    {"DFS", MAX_ELEMENT_COUNT_DFS - 1},

    {"BIBFS", 50},
    {"BIBFS", 100},
    {"BIBFS", 4500},
    {"BIBFS", 37500},
    {"BIBFS", MAX_ELEMENT_COUNT_BIBFS - 1},

    {"ASTAR", 50},
    {"ASTAR", 100},
    {"ASTAR", 4500},
    {"ASTAR", 37500},
    {"ASTAR", MAX_ELEMENT_COUNT_ASTAR - 1},

    {"GBFS", 50},
    {"GBFS", 100},
    {"GBFS", 4500},
    {"GBFS", 50},
    {"GBFS", MAX_ELEMENT_COUNT_GBFS - 1}};

const std::vector<std::pair<std::string, int>> testCasesSorting{

    {"selection", 5},
    {"selection", 25},
    {"selection", 100},
    {"selection", 1000},
    {"selection", MAX_ELEMENT_COUNT_SELECTION - 1},

    {"bubble", 5},
    {"bubble", 25},
    {"bubble", 100},
    {"bubble", 1000},
    {"bubble", MAX_ELEMENT_COUNT_BUBBLE - 1},

    {"quick", 5},
    {"quick", 25},
    {"quick", 100},
    {"quick", 1000},
    {"quick", MAX_ELEMENT_COUNT_QUICK - 1},

    {"merge", 5},
    {"merge", 25},
    {"merge", 100},
    {"merge", 1000},
    {"merge", MAX_ELEMENT_COUNT_MERGE - 1},

    {"counting", 5},
    {"counting", 25},
    {"counting", 100},
    {"counting", 1000},
    {"counting", MAX_ELEMENT_COUNT_COUNTING - 1},

    {"cycle", 5},
    {"cycle", 25},
    {"cycle", 100},
    {"cycle", 1000},
    {"cycle", MAX_ELEMENT_COUNT_CYCLE - 1},

    {"heap", 5},
    {"heap", 25},
    {"heap", 100},
    {"heap", 1000},
    {"heap", MAX_ELEMENT_COUNT_HEAP - 1},

    // bogo is factorial time: keep sizes tiny
    {"bogo", 3},
    {"bogo", 5},
    {"bogo", MAX_ELEMENT_COUNT_BOGO - 1}};

const std::unordered_map<std::string, std::vector<std::string>>
    implementedAlgoExpectedResponseType{
        {"selection", {"MOVES", "SORTED"}},
        {"bubble", {"MOVES", "SORTED"}},
        {"quick", {"MOVES", "SORTED"}},
        {"merge",
         {"SPLIT (ORIGIN,LEFT,RIGHT)", "SORTED (LEFT,RIGHT)",
          "MERGED (LEFT,RIGHT,RESULT)", "SORTED"}},
        {"counting", {"COUNT ARRAY", "SORTED"}},
        {"cycle", {"SWAPED", "SORTED"}},
        {"bogo", {"TRIES", "SORTED"}},
        {"heap", {"HEAP", "ARRAY"}},

        {"BFS", {"VISITED", "SEARCHMAP", "PATH"}},
        {"dijkstra", {"DISTANCE", "PATH"}},
        {"DFS", {"VISITED", "SEARCHMAP", "PATH"}},
        {"BIBFS", {"VISITEDFROMSTART", "VISITEDFROMEND", "PATH"}},
        {"ASTAR", {"DISTANCE", "PATH"}},
        {"IMAGE", {"SHUFFLED (INDEXES)"}}, // image is a special case because its returns
                                           // this array as well
        {"GBFS", {"DISTANCE", "PATH"}}};

#endif