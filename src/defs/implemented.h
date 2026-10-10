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
#ifndef DEF_IMPLEMENTED_H
#define DEF_IMPLEMENTED_H
#include <vector>
#include <unordered_map>
#include <string>
#include "max_sizes.h"
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