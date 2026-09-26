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

#define VERSION 0.8

#define DEFAULT_COLOUR MAGENTA

#define VERBOSE_LEVEL_NEEDED_FOR_TIME 0

#define MAX_ELEMENT_COUNT                                                      \
  2500 // you could customise this for every algo, but thats a future issue
       // (Issue #38)

#define MAX_ELEMENT_COUNT_BOGO 9

using json = nlohmann::json;

const std::unordered_set<std::string> implementedSortAlgos = {
"selection","bogo","bubble","quick","merge","counting","cycle"
};

const std::unordered_set<std::string> implementedPathAlgos = {"BFS","dijkstra","DFS"}; 

#endif
