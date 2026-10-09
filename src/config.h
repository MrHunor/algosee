/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#ifndef CONFIG_H
#define CONFIG_H

#include <nlohmann/json.hpp>
#include "exception/exception.h"
#include "build_info.h"
#include <vector>


#define QUOTE "\"Es mejor morir de pie que vivir toda una vida arrodillado\" ~ Emiliano Zapata."

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

    const std::vector<std::pair<std::string,int>> testCasesSorting
{
   
    {"selection",5},
    {"selection",25},
    {"selection",100},
    {"selection",1000},
    {"selection",MAX_ELEMENT_COUNT_SORT-1},

    {"bubble",5},
    {"bubble",25},
    {"bubble",100},
    {"bubble",1000},
    {"bubble",MAX_ELEMENT_COUNT_SORT-1},

    {"quick",5},
    {"quick",25},
    {"quick",100},
    {"quick",1000},
    {"quick",MAX_ELEMENT_COUNT_SORT-1},

    {"merge",5},
    {"merge",25},
    {"merge",100},
    {"merge",1000},
    {"merge",MAX_ELEMENT_COUNT_SORT-1},

    {"counting",5},
    {"counting",25},
    {"counting",100},
    {"counting",1000},
    {"counting",MAX_ELEMENT_COUNT_SORT-1},

    {"cycle",5},
    {"cycle",25},
    {"cycle",100},
    {"cycle",1000},
    {"cycle",MAX_ELEMENT_COUNT_SORT-1},

    // bogo is factorial time: keep sizes tiny
    {"bogo",3},
    {"bogo",5},
    {"bogo",MAX_ELEMENT_COUNT_BOGO-1},
};

const std::vector<std::pair<std::string,std::pair<int,int>>> testCasesPathfinding
{
    {"BFS",{10,5}},
    {"BFS",{20,5}},
    {"BFS",{100,45}},
    {"BFS",{250,150}},
    {"BFS",{249,249}},

    {"dijkstra",{10,5}},
    {"dijkstra",{20,5}},
    {"dijkstra",{100,45}},
    {"dijkstra",{250,150}},
    {"dijkstra",{249,249}},

    {"DFS",{10,5}},
    {"DFS",{20,5}},
    {"DFS",{100,45}},
    {"DFS",{250,150}},
    {"DFS",{249,249}},

    {"BIBFS",{10,5}},
    {"BIBFS",{20,5}},
    {"BIBFS",{100,45}},
    {"BIBFS",{250,150}},
    {"BIBFS",{249,249}},
};

#endif
