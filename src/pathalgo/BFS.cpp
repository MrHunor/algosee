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
#include "../config.h"
#include "../utils/pathalgo/pathalgo.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <queue>
#include <utility>
#include <vector>

std::vector<std::pair<int, int>>
BreadthFirstSearch(std::vector<std::vector<bool>> map,
                   std::pair<int, int> start, std::pair<int, int> goal,
                   json &response) {
  // Frontier means what is it going to explore next, hence it being a queue

  const int rows = map.size();
  const int collums = map[0].size();

  std::vector<std::vector<bool>> visited(
      rows, std::vector<bool>(collums, false)); // weird ass constructor
  std::vector<std::vector<std::pair<int, int>>> parent(
      rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
  std::vector<std::pair<int, int>> path;

  std::queue<std::pair<int, int>> frontier;
  frontier.push({start.second, start.first});
  visited[start.second][start.first] = true;

  std::vector<int> directionsRows = {-1, 1, 0, 0};
  std::vector<int> directionsCollums = {0, 0, 1, -1};

  while (!frontier.empty()) {
    int x = frontier.front().first;
    int y = frontier.front().second;
    // because the first frontier element is now in x,y the first element of
    // frontier can be deleted
    frontier.pop();

    if (std::make_pair(y, x) == goal)
      break; // reached goal

    for (int i = 0; i < 4; i++) // check all possible neighbors
    {
      int xNeighbour = x + directionsRows[i];
      int yNeighbour = y + directionsCollums[i];

      if (xNeighbour < 0 || xNeighbour > rows - 1 || yNeighbour < 0 ||
          yNeighbour > collums - 1)
        continue; // current neighboor out of map, skip this iteration

      if (map[xNeighbour][yNeighbour] == true)
        continue; // current Neighboor not a valid tile, skip

      if (visited[xNeighbour][yNeighbour] == true)
        continue; // been there done that, so skip

      // if everything thus far was negative that means we have a valid
      // unvisited tile
      visited[xNeighbour][yNeighbour] = true;
      parent[xNeighbour][yNeighbour] = {x, y};
      frontier.push({xNeighbour, yNeighbour});
    }
  }
  // being out of the while loop means every possible tile was explored

 
  response["VISITED"] = swapVisitedArray(visited);

  if (!visited[goal.second][goal.first])
    return {}; // never once reached the goal means there was no path found to
               // the goal

  // retrace the steps via parent
  std::pair<int, int> current = {goal.second, goal.first};

  while (current != std::make_pair(start.second, start.first)) {
    path.push_back(current);
    current = parent[current.first][current.second];
  }

  // push back start pour fini
  path.push_back({start.second, start.first});

  // due to starting with goal, the path must be reversed to start from start
  std::reverse(path.begin(), path.end());

  for (int i = 0; i < path.size(); i++) {
    std::swap(path[i].first, path[i].second);
  }
  return path;
}