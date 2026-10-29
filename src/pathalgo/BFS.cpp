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
#include "../defs/general.h"
#include "../defs/types.h"
#include "../utils/pathalgo/pathalgo.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <queue>
#include <utility>
#include <vector>

//------------------MAP BASED------------------------------

std::vector<std::pair<int, int>> BreadthFirstSearch(const MapBool &map,
                                                    std::pair<int, int> start,
                                                    std::pair<int, int> goal,
                                                    json &response)
{
    // Frontier means what is it going to explore next, hence it being a queue

    unsigned int iteration = 0;
    const int rows = map.size();
    const int collums = map[0].size();

    std::vector<std::vector<int>> visited(
        rows, std::vector<int>(collums, -1)); // weird ass constructor
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
    std::vector<std::pair<int, int>> path;

    std::vector<std::vector<int>> searchMap(
        rows, std::vector<int>(collums, -1)); // weird ass constructor

    std::queue<std::pair<int, int>> frontier;
    frontier.push({start.second, start.first});
    visited[start.second][start.first] = 0;

    std::vector<int> directionsRows = {-1, 1, 0, 0};
    std::vector<int> directionsCollums = {0, 0, 1, -1};

    while (!frontier.empty())
    {

        int y = frontier.front().first;
        int x = frontier.front().second;
        searchMap[y][x] = iteration;
        // because the first frontier element is now in x,y the first element of
        // frontier can be deleted
        frontier.pop();

        if (std::make_pair(x, y) == goal)
            break; // reached goal

        for (int i = 0; i < 4; i++) // check all possible neighbors
        {
            int yNeighbour = y + directionsRows[i];
            int xNeighbour = x + directionsCollums[i];

            if (yNeighbour < 0 || yNeighbour > rows - 1 || xNeighbour < 0 ||
                xNeighbour > collums - 1)
                continue; // current neighboor out of map, skip this iteration

            if (map[yNeighbour][xNeighbour] == true)
                continue; // current Neighboor not a valid tile, skip

            if (visited[yNeighbour][xNeighbour] != -1)
                continue; // been there done that, so skip

            // if everything thus far was negative that means we have a valid
            // unvisited tile
            visited[yNeighbour][xNeighbour] = 1;
            parent[yNeighbour][xNeighbour] = {y, x};
            frontier.push({yNeighbour, xNeighbour});
        }
        iteration++;
    }
    // being out of the while loop means every possible tile was explored

    response["VISITED"] = SwapVisitedArray(visited);
    response["SEARCHMAP"] = searchMap;
    if (visited[goal.second][goal.first] == -1)
        return {}; // never once reached the goal means there was no path found to
                   // the goal

    // retrace the steps via parent
    std::pair<int, int> current = {goal.second, goal.first};

    while (current != std::make_pair(start.second, start.first))
    {
        path.push_back(current);
        current = parent[current.first][current.second];
    }

    // push back start pour fini
    path.push_back({start.second, start.first});

    // due to starting with goal, the path must be reversed to start from start
    std::reverse(path.begin(), path.end());

    for (int i = 0; i < path.size(); i++)
    {
        std::swap(path[i].first, path[i].second);
    }
    return path;
}

//-----------------GRAPH BASED-------------------------------

std::vector<int> BreadthFirstSearch(const GraphUnweight &graph, int start, int goal,
                                    json &response)
{
    unsigned int iteration = 0;
    const int rows = graph.size();

    std::vector<int> visited(rows, -1);
    std::vector<int> parent(rows,-1);

    std::vector<int> path;

    std::vector<int> searchMap(rows, -1);

    std::queue<int> frontier;
    frontier.push(start);
    visited[start] = 0;

    while (!frontier.empty())
    {
        int ID = frontier.front();//which Node we are currently looking at 
        searchMap[ID] = iteration;
        frontier.pop();

        if (ID == goal)
            break; // reached goal

        for (int i = 0; i < graph[ID].size(); i++)
        {
            int NeighboorID = graph[ID].at(i);

            if (visited[NeighboorID] != -1)
                continue;

            visited[NeighboorID] = 1;
            parent[NeighboorID] = ID;
            frontier.push(NeighboorID);
        }
        iteration++;
    }

    response["VISITED"] = visited;
    response["SEARCHMAP"] = searchMap;
    if (visited[goal] == -1)
        return {};

    int current = goal;
    while (current != start)
    {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return path;
}
