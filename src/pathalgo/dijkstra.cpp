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


using Node = std::pair<int, std::pair<int, int>>;
struct CompareNodeDijkstra
{
    bool operator()(const Node& a, const Node& b)
    {
        return a.second>b.second;
    }
};

//-------------------------MAP BASED---------------------------------------
std::vector<std::pair<int, int>> Dijkstra(const MapInt &map,
                                          std::pair<int, int> start,
                                          std::pair<int, int> goal, json &response)
{
    const int rows = map.size();
    const int collums = map[0].size();

    // dijkstra uses something similar to BFS but instead of vector<bool> visited,
    // it uses a cost based vector<int> so this is effectivly just: "What is the
    // cheapest possible distance to this point in the map"
    std::vector<std::vector<int>> distance(
        rows,
        std::vector<int>(collums,
                         INFINITY_INT)); // i dont think i'll ever unstand this contructor
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
    std::vector<std::pair<int, int>> path;

    // in contrast to BFS we need a reverse "priority" queue, this means the first
    // eleent of the queue is the one with the lowest "priority" which in Djkstras
    // case is the distance
    
    std::priority_queue<Node, std::vector<Node>, CompareNodeDijkstra> frontier;

    distance[start.second][start.first] = 0;

    // push the start into the frontier, because its the lowest distance and also
    // the only element it will be the first element
    frontier.push({0, {start.second, start.first}});

    std::vector<int> directionsRows = {-1, 1, 0, 0};
    std::vector<int> directionsCollums = {0, 0, 1, -1};

    while (!frontier.empty())
    {
        // extract info from the first frontier element
        int currentDistance = frontier.top().first;
        int y = frontier.top().second.first;
        int x = frontier.top().second.second;

        // because the info fron the first frontier element is now stored in the
        // above declared variables, we can delete the first element from frontier
        frontier.pop();

        if (currentDistance != distance[y][x])
            continue; // already discovered

        if (std::make_pair(x, y) == goal)
            break; // finished, goal discovered

        // check all neighbours
        for (int i = 0; i < 4; i++)
        {
            int yNeighbour = y + directionsRows[i];
            int xNeighbour = x + directionsCollums[i];

            if (yNeighbour < 0 || yNeighbour > rows - 1 || xNeighbour < 0 ||
                xNeighbour > collums - 1)
                continue; // current neighboor out of map, skip this iteration

            if (map[yNeighbour][xNeighbour] == -1)
                continue; // this tile is blocked, skip

            int newDistance = 0;
            if (currentDistance > INFINITY_INT - map[yNeighbour][xNeighbour])
            {
                newDistance = INFINITY_INT;
            }
            else
            {
                newDistance = currentDistance + map[yNeighbour][xNeighbour];
            }

            // is the current path to this tile cheaper then the already known path
            // from start to goal?
            if (newDistance < distance[yNeighbour][xNeighbour])
            {
                // yes, save all info about the new tile and add it to frontier to be
                // explored next
                distance[yNeighbour][xNeighbour] = newDistance;
                parent[yNeighbour][xNeighbour] = {y, x};
                frontier.push({newDistance, {yNeighbour, xNeighbour}});
            }
        }
    }

    // Explored all possible tiles

    response["DISTANCE"] = SwapVisitedArray(distance);

    // checking if the goal was ever reached, be careful this could theoretically
    // be a false positive if the addition of tiles matches exatly the intmax, but
    // the likelyhood is very small
    if (distance[goal.second][goal.first] == INFINITY_INT)
        return {};

    // copy paste from BFS, same logic
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


//-----------------------GRAPH BASED-----------------------------------------



std::vector<int> Dijkstra(const GraphWeighted& graph,
                                          int start,
                                          int goal, json &response)
{
    const int rows = graph.size();

    // dijkstra uses something similar to BFS but instead of vector<bool> visited,
    // it uses a cost based vector<int> so this is effectivly just: "What is the
    // cheapest possible distance to this point in the map"
    std::vector<int> distance(rows,INFINITY_INT); // i dont think i'll ever unstand this contructor
    std::vector<int> parent(rows,-1);
    std::vector<int> path;

    // in contrast to BFS we need a reverse "priority" queue, this means the first
    // element of the queue is the one with the lowest "priority" which in Djkstras
    // case is the distance
    using Node = std::pair<int, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;//yeah so in contrary to A* this implementation used weight,ID

    distance[start] = 0;

    // push the start into the frontier, because its the lowest distance and also
    // the only element it will be the first element
    frontier.push({0, start});

    while (!frontier.empty())
    {
        // extract info from the first frontier element
        int currentDistance = frontier.top().first;
        int currentID = frontier.top().second;

        // because the info fron the first frontier element is now stored in the
        // above declared variables, we can delete the first element from frontier
        frontier.pop();

        if (currentDistance != distance[currentID])
            continue; // already discovered

        if (currentID == goal)
            break; // finished, goal discovered

        // check all neighbours
        for (int i = 0; i < graph[currentID].size(); i++)
        {
            std::pair<int,int> currentNID = graph[currentID].at(i);


            int newDistance = 0;
            if (currentDistance > INFINITY_INT - currentNID.first)
            {
                newDistance = INFINITY_INT;
            }
            else
            {
                newDistance = currentDistance + currentNID.first;
            }

            // is the current path to this tile cheaper then the already known path
            // from start to goal?
            if (newDistance < distance[currentNID.second])
            {
                // yes, save all info about the new tile and add it to frontier to be
                // explored next
                distance[currentNID.second] = newDistance;
                parent[currentNID.second] = currentID;
                frontier.push({newDistance,currentNID.second});
            }
        }
    }

    // Explored all possible tiles

    response["DISTANCE"] = distance;

    // checking if the goal was ever reached, be careful this could theoretically
    // be a false positive if the addition of tiles matches exatly the intmax, but
    // the likelyhood is very small
    if (distance[goal] == INFINITY_INT)
        return {};

    // copy paste from BFS, same logic
    int current = goal;

    while (current != start)
    {
        path.push_back(current);
        current = parent[current];
    }

    // push back start pour fini
    path.push_back(start);

    // due to starting with goal, the path must be reversed to start from start
    std::reverse(path.begin(), path.end());

    return path;
}
