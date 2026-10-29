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
#include "../defs/general.h"
#include "../defs/types.h"
#include "../utils/pathalgo/pathalgo.h"
#include <algorithm>
#include <cmath>
#include <nlohmann/json.hpp>
#include <queue>
#include <utility>
#include <vector>

//--------------------MAP BASED--------------------------------------
// Overall Greedy BestFirst search is more or less just ASTAR but f(x)=h(x) without the
// g(x)
struct NodeGBFSMap
{
    int g;
    int h = 0;
    int x = 0;
    int y = 0;
    int f() const
    {
        return h;
    }
};

struct CompareNodeGBFSMap
{ // this is needed for priority queue
    bool operator()(const NodeGBFSMap &a, const NodeGBFSMap &b) const
    {
        return a.f() > b.f();
    }
};
// A* is similar to djkstra, only that is uses a diffrent priority queue, with
// f(n)=g(n)+h(n) where g(n) is the cost from start to node n (same as djkstra)
// but also h(n) estimated cost from node n to goal (manhatten distance)
std::vector<std::pair<int, int>> GBFS(const MapInt &map, std::pair<int, int> start,
                                      std::pair<int, int> goal, json &response)
{
    const int rows = map.size();
    const int collums = map[0].size();

    int h = 0;

    std::vector<std::vector<int>> distance(
        rows,
        std::vector<int>(collums,
                         INFINITY_INT)); // i dont think i'll ever unstand this contructor
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
    std::vector<std::pair<int, int>> path;

    std::priority_queue<NodeGBFSMap, std::vector<NodeGBFSMap>, CompareNodeGBFSMap>
        frontier;

    distance[start.second][start.first] = 0;

    // push the start into the frontier, because its the lowest distance and also
    // the only element it will be the first element
    frontier.push(
        {0, std::abs(start.first - goal.first) + std::abs(start.second - goal.second),
         start.first, start.second});

    std::vector<int> directionsRows = {-1, 1, 0, 0};
    std::vector<int> directionsCollums = {0, 0, 1, -1};

    while (!frontier.empty())
    {
        // extract info from the first frontier element
        int currentDistance = frontier.top().g;
        int y = frontier.top().y;
        int x = frontier.top().x;

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
                h = std::abs(xNeighbour - goal.first) +
                    std::abs(yNeighbour - goal.second);
                frontier.push({newDistance, h, xNeighbour, yNeighbour});
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

//--------------------GRAPH BASED--------------------------------
struct NodeGBFSGraph
{
    int g;
    int h = 0;
    int ID = -1;
    int f() const
    {
        return h;
    }
};

struct CompareNodeGBFSGraph
{ // this is needed for priority queue
    bool operator()(const NodeGBFSGraph &a, const NodeGBFSGraph &b) const
    {
        return a.f() > b.f();
    }
};
std::vector<int> GBFS(const GraphWeighted &graph, int start, int goal, json &response)
{
    // implementaion wise, graph[i] is a array of pair<int,int> with .first being the ID
    // of the node the connection is to and .second is the weight
    const size_t rows = graph.size();

    int h = 0;
    std::vector<int> distance(rows, INFINITY_INT);
    std::vector<int> parent(rows, -1);
    std::vector<int> path;

    std::priority_queue<NodeGBFSGraph, std::vector<NodeGBFSGraph>, CompareNodeGBFSGraph>
        frontier;

    distance[start] = 0;

    NodeGBFSGraph startNode{0, 0, start};
    frontier.push(startNode);

    while (!frontier.empty())
    {
        // extract info from the first frontier element
        int currentDistance = frontier.top().g;
        int currentID = frontier.top().ID;
        frontier.pop();

        if (currentDistance != distance[currentID])
            continue; // already discovered

        if (currentID == goal)
            break; // finished, goal discovered

        // check all neighbours
        for (int i = 0; i < graph[currentID].size(); i++)
        {

            std::pair<int, int> loopID = graph[currentID].at(i);

            int newDistance = 0;

            if (currentDistance > INFINITY_INT - loopID.second)
            {
                newDistance = INFINITY_INT;
            }
            else
            {
                newDistance = currentDistance + loopID.second;
            }

            // is the current path to this tile cheaper then the already known path
            // from start to goal?
            if (newDistance < distance[loopID.first])
            {
                // yes, save all info about the new tile and add it to frontier to be
                // explored next
                distance[loopID.first] = newDistance;
                parent[loopID.first] = currentID;
                h = goal - loopID.first;
                frontier.push({newDistance, h, loopID.first});
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