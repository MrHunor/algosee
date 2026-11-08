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

#ifndef DEF_MAX_SIZES_H
#define DEF_MAX_SIZES_H

// Following limits are only estimates (made by AI), which calculate with 512MB of
// availabe ram, of which 75% may be used -32MB for the execuatble (so very safe
// assumtions), ill run the numbers with a systemd-run and see if they hold up to that,
// you'll be able to see this in docs/tests

#define MAX_ELEMENT_COUNT_SORT                                                           \
    2500 // you could customise this for every algo, but thats a future issue
         // (Issue #38)

#define MAX_ELEMENT_COUNT_SELECTION MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_BOGO MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_BUBBLE MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_QUICK MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_MERGE MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_COUNTING MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_CYCLE MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_RADIX MAX_ELEMENT_COUNT_SORT
#define MAX_ELEMENT_COUNT_HEAP MAX_ELEMENT_COUNT_SORT
#define HIGHEST_ELEMENT_COUNTING 100000

#define MAX_ELEMENT_COUNT_PATH 1000
#define MAX_ELEMENT_COUNT_BFS MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_DIJKSTRA MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_DFS MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_BIBFS MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_ASTAR MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_GBFS MAX_ELEMENT_COUNT_PATH
#define MAX_ELEMENT_COUNT_BELLMAN_FORD MAX_ELEMENT_COUNT_PATH
#define HIGHEST_ELEMENT_PATH std::numeric_limits<int>::max() - 1

#define MAX_PIXEL_COUNT_IMAGE 2500

#define MAX_NODE_COUNT_PATH_WEIGHTED MAX_ELEMENT_COUNT_PATH
#define MAX_NODE_COUNT_PATH_UNWEIGHTED MAX_ELEMENT_COUNT_PATH

#endif