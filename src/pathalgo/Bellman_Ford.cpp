/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
#include <cstdint>
#include <limits>
#include <nlohmann/json.hpp>
#include <queue>
#include <utility>
#include <vector>

bool checkAndUpdateTile(const MapInt &map, std::vector<std::vector<int64_t>> &d, int x,
                        int y, int parentX, int parentY)
{
    int64_t newd = d[parentY][parentX] + map[y][x];
    if (d[y][x] > newd)
    {
        d[y][x] = newd;
        return true;
    }
    return false;
}

//-------------------------MAP BASED---------------------------------------
std::vector<std::pair<int, int>> Bellman_Ford(const MapInt &map,
                                              const std::pair<int, int> start,
                                              const std::pair<int, int> goal,
                                              json &response)
{
    bool updated = false;
    const int rows = map.size();
    const int cols = map[0].size();
    std::vector<std::vector<int64_t>> d(
        rows,
        std::vector<int64_t>(
            cols, std::numeric_limits<int64_t>::max())); // d stands for distance which
    // seems to be the trivial name
    // for this pathfinding algo
    d[start.second][start.first] = 0;
    std::vector<std::vector<std::pair<int, int>>> parent(
        rows, std::vector<std::pair<int, int>>(cols, {-1, -1}));
    std::vector<std::vector<int>> searchMap(rows, std::vector<int>(cols, -1));
    std::vector<int> directionsRows = {-1, 1, 0, 0};
    std::vector<int> directionsCollums = {0, 0, 1, -1};

    for (int searchI = 0; searchI < cols * rows - 1;
         searchI++) // this is where BF is intresting, it iterates through every Tile for
                    // the amount of tiles there are (its called 'relaxing each edge'),
                    // also somehow you only have to search tiles-1 because the shortest
                    // path in a map of n tiles can only take tiles-1
    {

        for (int searchX = 0; searchX < cols; searchX++)
        {
            for (int searchY = 0; searchY < rows; searchY++)
            {
                if (d[searchY][searchX] == std::numeric_limits<int64_t>::max())
                    continue; // no yet discovered by the algo, avoid integer overflow
                for (int i = 0; i < 4; i++)
                {
                    int nX = searchX + directionsCollums[i];
                    int nY = searchY + directionsRows[i];

                    // check bounds
                    if (nX < 0 || nX > cols - 1 || nY < 0 || nY > rows - 1)
                    {
                        continue;
                    }

                    if (checkAndUpdateTile(map, d, nX, nY, searchX, searchY))
                    {
                        parent[nY][nX] = {searchY, searchX};
                        updated = true;
                    }
                }
            }
        }
        if (!updated) // if no updates to tiles were made during a hole iteration then
                      // every tile must have its shortest distance assigned
            break;
    }
    
    std::vector<std::pair<int, int>> path;
    if (d[goal.second][goal.first] ==
        std::numeric_limits<int64_t>::max()) // check if goal was ever really reached
    {
        return {};
    }
    // copy paste from BFS, same logic
    std::pair<int, int> current = {goal.second, goal.first};

    while (current != std::make_pair(start.second, start.first))
    {
        if (current.first < 0 ||
            current.second <
                0) // parent is constucted as -1 so if a tiles remains -1 atfter every
                   // should have been explored that means it has no parent (this should
                   // no be possible to happen but im scared to delete this check )
        {
            return {};
        }
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
