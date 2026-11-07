/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
 * @file implemented.h
 *
 * @brief Contains arrays and maps of implemented algorithms and their boundries as well
 * as what json return objects should be returned
 *
 *
 */
#ifndef DEF_IMPLEMENTED_H
#define DEF_IMPLEMENTED_H
#include "max_sizes.h"

#include <string>
#include <unordered_map>
#include <vector>
const std::unordered_map<std::string, int> implementedSortAlgos = {
    {"bogo", MAX_ELEMENT_COUNT_BOGO},         {"bubble", MAX_ELEMENT_COUNT_BUBBLE},
    {"counting", MAX_ELEMENT_COUNT_COUNTING}, {"cycle", MAX_ELEMENT_COUNT_CYCLE},
    {"heap", MAX_ELEMENT_COUNT_HEAP},         {"merge", MAX_ELEMENT_COUNT_MERGE},
    {"quick", MAX_ELEMENT_COUNT_QUICK},       {"selection", MAX_ELEMENT_COUNT_SELECTION}};

const std::unordered_map<std::string, int> implementedPathAlgosSize = {
    {"ASTAR", MAX_ELEMENT_COUNT_ASTAR},
    {"BFS", MAX_ELEMENT_COUNT_BFS},
    {"BIBFS", MAX_ELEMENT_COUNT_BIBFS},
    {"DFS", MAX_ELEMENT_COUNT_DFS},
    {"dijkstra", MAX_ELEMENT_COUNT_DIJKSTRA},
    {"GBFS", MAX_ELEMENT_COUNT_GBFS},
    {"bellman-ford", MAX_ELEMENT_COUNT_BELLMAN_FORD}};

const std::unordered_map<std::string, char> implementedPathAlgoCatagory = {
    // U(nweighted) or W(eighted)
    {"ASTAR", 'W'},    {"BFS", 'U'},  {"BIBFS", 'U'},       {"DFS", 'U'},
    {"dijkstra", 'W'}, {"GBFS", 'W'}, {"bellman-ford", 'W'}};

const std::vector<std::string> implementedArrayReturnTypes = {
    "COUNT ARRAY",
    "DISTANCE",
    "MERGED (LEFT,RIGHT,RESULT)",
    "MOVES",
    "PATH",
    "SEARCHMAP",
    "SORTED",
    "SORTED (LEFT,RIGHT)",
    "SPLIT (ORIGIN,LEFT,RIGHT)",
    "SWAPED",
    "TRIES",
    "VISITED",
    "VISITEDFROMEND",
    "VISITEDFROMSTART",
    "ARRAY",
    "HEAP"};

const std::vector<std::string> implementedNonArrayReturnTypes = {"BUILDTIME", "STATUS",
                                                                 "TIME", "VERSION"};
#endif
