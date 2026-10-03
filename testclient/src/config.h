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
#include <set>
#include <unordered_set>

#define VERSION 0.8

#define DEFAULT_COLOUR MAGENTA

#define VERBOSE_LEVEL_NEEDED_FOR_TIME 0

#define MAX_ELEMENT_COUNT                                                      \
  2500 // you could customise this for every algo, but thats a future issue
       // (Issue #38)

#define MAX_ELEMENT_COUNT_BOGO 9

using json = nlohmann::json;
const std::unordered_set<std::string> implementedSortAlgos = {
    "selection",  "bubble", "quick", "merge", "counting", "cycle","bogo"};

const std::unordered_set<std::string> implementedPathAlgos = {"BFS", "dijkstra",
                                                              "DFS", "BIBFS"};
                                                              //algo,size
const std::vector<std::pair<std::string,int>> testCasesSorting
{
   
    {"selection",5},
    {"selection",25},
    {"selection",100},
    {"selection",1000},
    {"selection",MAX_ELEMENT_COUNT-1},

    {"bubble",5},
    {"bubble",25},
    {"bubble",100},
    {"bubble",1000},
    {"bubble",MAX_ELEMENT_COUNT-1},

    {"quick",5},
    {"quick",25},
    {"quick",100},
    {"quick",1000},
    {"quick",MAX_ELEMENT_COUNT-1},

    {"merge",5},
    {"merge",25},
    {"merge",100},
    {"merge",1000},
    {"merge",MAX_ELEMENT_COUNT-1},

    {"counting",5},
    {"counting",25},
    {"counting",100},
    {"counting",1000},
    {"counting",MAX_ELEMENT_COUNT-1},

    {"cycle",5},
    {"cycle",25},
    {"cycle",100},
    {"cycle",1000},
    {"cycle",MAX_ELEMENT_COUNT-1},

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
    {"BFS",{250,250}},

    {"dijkstra",{10,5}},
    {"dijkstra",{20,5}},
    {"dijkstra",{100,45}},
    {"dijkstra",{250,150}},
    {"dijkstra",{250,250}},

    {"DFS",{10,5}},
    {"DFS",{20,5}},
    {"DFS",{100,45}},
    {"DFS",{250,150}},
    {"DFS",{250,250}},

    {"BIBFS",{10,5}},
    {"BIBFS",{20,5}},
    {"BIBFS",{100,45}},
    {"BIBFS",{250,150}},
    {"BIBFS",{250,250}},
};


                                                              #endif
