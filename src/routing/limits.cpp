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
#include "../defs/max_sizes.h"
#include "../logger/logger.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <stb_image.h>

void runLimits(httplib::Response &res, stateClass &state)
{
    json response;
    // sort
    response["MAX_ELEMENT_COUNT_SORT"] = MAX_ELEMENT_COUNT_SORT;
    response["MAX_ELEMENT_COUNT_SELECTION"] = MAX_ELEMENT_COUNT_SELECTION;
    response["MAX_ELEMENT_COUNT_BOGO"] = MAX_ELEMENT_COUNT_BOGO;
    response["MAX_ELEMENT_COUNT_BUBBLE"] = MAX_ELEMENT_COUNT_BUBBLE;
    response["MAX_ELEMENT_COUNT_QUICK"] = MAX_ELEMENT_COUNT_QUICK;
    response["MAX_ELEMENT_COUNT_MERGE"] = MAX_ELEMENT_COUNT_MERGE;
    response["MAX_ELEMENT_COUNT_COUNTING"] = MAX_ELEMENT_COUNT_COUNTING;
    response["MAX_ELEMENT_COUNT_CYCLE"] = MAX_ELEMENT_COUNT_CYCLE;
    response["HIGHEST_ELEMENT_COUNTING"] = HIGHEST_ELEMENT_COUNTING;

    // path
    response["MAX_ELEMENT_COUNT_BFS"] = MAX_ELEMENT_COUNT_BFS;
    response["MAX_ELEMENT_COUNT_DIJKSTRA"] = MAX_ELEMENT_COUNT_DIJKSTRA;
    response["MAX_ELEMENT_COUNT_DFS"] = MAX_ELEMENT_COUNT_DFS;
    response["MAX_ELEMENT_COUNT_BIBFS"] = MAX_ELEMENT_COUNT_BIBFS;
    response["MAX_ELEMENT_COUNT_ASTAR"] = MAX_ELEMENT_COUNT_ASTAR;
    response["MAX_PIXEL_COUNT_IMAGE"] = MAX_PIXEL_COUNT_IMAGE;

    // image
    response["MAX_PIXEL_COUNT_IMAGE"] = MAX_PIXEL_COUNT_IMAGE;
    res.status = 200;
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_content(response.dump(), "application/json");
    state.out("Sending response...", 0);
    return;
}