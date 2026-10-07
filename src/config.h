/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include <unordered_set>
#include "build_info.h"
#include <vector>


#define DEFAULT_COLOUR MAGENTA

#define VERBOSE_LEVEL_NEEDED_FOR_TIME 0

typedef unsigned char colour;
struct pixel {
  colour R;
  colour G;
  colour B;
  colour A;
};

typedef std::vector<std::pair<int, pixel>> indexPixel;

struct image {
  int width;
  int height;
  indexPixel data;
};

#define MAX_ELEMENT_COUNT_SORT                                                 \
  2500 // you could customise this for every algo, but thats a future issue
       // (Issue #38)

#define MAX_ELEMENT_COUNT_SELECTION MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_BOGO 9
#define MAX_ELEMENT_COUNT_BUBBLE MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_QUICK MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_MERGE MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_COUNTING MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_CYCLE MAX_ELEMENT_COUNT_SORT

#define MAX_ELEMENT_COUNT_PATH 62500
#define MAX_ELEMENT_COUNT_BFS MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_DIJKSTRA MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_DFS MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_BIBFS MAX_ELEMENT_COUNT_PATH

using json = nlohmann::json;

const std::unordered_map<std::string, int> implementedSortAlgos = {
    {"selection", MAX_ELEMENT_COUNT_SELECTION},
    {"bogo", MAX_ELEMENT_COUNT_BOGO},
    {"bubble", MAX_ELEMENT_COUNT_BUBBLE},
    {"quick", MAX_ELEMENT_COUNT_QUICK},
    {"merge", MAX_ELEMENT_COUNT_MERGE},
    {"counting", MAX_ELEMENT_COUNT_COUNTING},
    {"cycle", MAX_ELEMENT_COUNT_CYCLE}};

const std::unordered_map<std::string, int> implementedPathAlgos = {
    {"BFS", MAX_ELEMENT_COUNT_BFS},
    {"dijkstra", MAX_ELEMENT_COUNT_DIJKSTRA},
    {"DFS", MAX_ELEMENT_COUNT_DFS},
    {"BIBFS", MAX_ELEMENT_COUNT_BIBFS}};

#endif
