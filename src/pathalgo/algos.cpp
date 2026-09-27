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
#include "../logger/logger.h"
#include <algorithm>
#include <limits>
#include <nlohmann/json.hpp>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

std::vector<std::pair<int, int>>
Dijkstra(const std::vector<std::vector<int>> &map, std::pair<int, int> start,
         std::pair<int, int> goal, json response) {
  const int rows = map.size();
  const int collums = map[0].size();
  const int INFINTY = std::numeric_limits<int>::max();

  // dijkstra uses something similar to BFS but instead of vector<bool> visited,
  // it uses a cost based vector<int> so this is effectivly just: "What is the
  // cheapest possible distance to this point in the map"
  std::vector<std::vector<int>> distance(
      rows,
      std::vector<int>(
          collums, INFINTY)); // i dont think i'll ever unstand this contructor
  std::vector<std::vector<std::pair<int, int>>> parent(
      rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
  std::vector<std::pair<int, int>> path;

  // in contrast to BFS we need a reverse "priority" queue, this means the first
  // eleent of the queue is the one with the lowest "priority" which in Djkstras
  // case is the distance
  using Node = std::pair<int, std::pair<int, int>>;
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;

  distance[start.second][start.first] = 0;

  // push the start into the frontier, because its the lowest distance and also
  // the only element it will be the first element
  frontier.push({0, {start.second,start.first}});

  std::vector<int> directionsRows = {-1, 1, 0, 0};
  std::vector<int> directionsCollums = {0, 0, 1, -1};

  while (!frontier.empty()) {
    // extract info from the first frontier element
    int currentDistance = frontier.top().first;
    int x = frontier.top().second.first;
    int y = frontier.top().second.second;

    // because the info fron the first frontier element is now stored in the
    // above declared variables, we can delete the first element from frontier
    frontier.pop();

    if (currentDistance != distance[x][y])
      continue; // already discovered

    if (std::make_pair(y, x) == goal)
      break; // finished, goal discovered

    // check all neighbours
    for (int i = 0; i < 4; i++) {
      int xNeighbour = x + directionsRows[i];
      int yNeighbour = y + directionsCollums[i];

      if (xNeighbour < 0 || xNeighbour > rows - 1 || yNeighbour < 0 ||
          yNeighbour > collums - 1)
        continue; // current neighboor out of map, skip this iteration

      if (map[xNeighbour][yNeighbour] == -1)
        continue; // this tile is blocked, skip

      int newDistance = currentDistance + map[xNeighbour][yNeighbour];

      // is the current path to this tile cheaper then the already known path
      // from start to goal?
      if (newDistance < distance[xNeighbour][yNeighbour]) {
        // yes, save all info about the new tile and add it to frontier to be
        // explored next
        distance[xNeighbour][yNeighbour] = newDistance;
        parent[xNeighbour][yNeighbour] = {x, y};
        frontier.push({newDistance, {xNeighbour, yNeighbour}});
      }
    }
  }

  // Explored all possible tiles

  response["DISTANCE"] = distance;

  // checking if the goal was ever reached, be careful this could theoretically
  // be a false positive if the addition of tiles matches exatly the intmax, but
  // the likelyhood is very small
  if (distance[goal.second][goal.first] == INFINTY)
    return {};

  // copy paste from BFS, same logic
  std::pair<int, int> current = {goal.second,goal.first};

  while (current != std::make_pair(start.second,start.first)) {
    path.push_back(current);
    current = parent[current.first][current.second];
  }

  // push back start pour fini
  path.push_back({start.second,start.first});

  // due to starting with goal, the path must be reversed to start from start
  std::reverse(path.begin(), path.end());

  for(int i = 0; i<path.size();i++)
  {
    std::swap(path[i].first,path[i].second);
  }


  return path;
}

std::vector<std::pair<int, int>>
BreadthFirstSearch(std::vector<std::vector<bool>> map,
                   std::pair<int, int> start, std::pair<int, int> goal,
                   json response) {
  // Frontier means what is it going to explore next, hence it being a queue

  const int rows = map.size();
  const int collums = map[0].size();

  std::vector<std::vector<bool>> visited(
      rows, std::vector<bool>(collums, false)); // weird ass constructor
  std::vector<std::vector<std::pair<int, int>>> parent(
      rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
  std::vector<std::pair<int, int>> path;

  std::queue<std::pair<int, int>> frontier;
  frontier.push({start.second,start.first});
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

  response["VISITED"] = visited;

  if (!visited[goal.second][goal.first])
    return {}; // never once reached the goal means there was no path found to
               // the goal

  // retrace the steps via parent
  std::pair<int, int> current = {goal.second,goal.first};

  while (current != std::make_pair(start.second,start.first)) {
    path.push_back(current);
    current = parent[current.first][current.second];
  }

  // push back start pour fini
  path.push_back(start);

  // due to starting with goal, the path must be reversed to start from start
  std::reverse(path.begin(), path.end());


    for(int i = 0; i<path.size();i++)
  {
    std::swap(path[i].first,path[i].second);
  }
  return path;
}

std::vector<std::pair<int, int>> BIBFS(std::vector<std::vector<bool>> map,
                                       std::pair<int, int> start,
                                       std::pair<int, int> goal,
                                       json &response) {
  const int rows = map.size();
  const int collums = map[0].size();

  int meetingPointX = -1;
  int meetingPointY = -1;
  std::vector<std::vector<bool>> visitedFromStart(
      rows,
      std::vector<bool>(collums, false)); // weird ass constructor once again
  std::vector<std::vector<bool>> visitedFromEnd(
      rows,
      std::vector<bool>(collums, false)); // weird ass constructor once again

  std::vector<std::vector<std::pair<int, int>>> parentFromStart(
      rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
  std::vector<std::vector<std::pair<int, int>>> parentFromEnd(
      rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));

  std::vector<std::pair<int, int>> pathFromStart;
  std::vector<std::pair<int, int>> pathFromEnd;

  std::queue<std::pair<int, int>> frontierFromStart;
  std::queue<std::pair<int, int>> frontierFromEnd;

  frontierFromStart.push({start.second,start.first});
  frontierFromEnd.push({goal.second,goal.first});
  visitedFromStart[start.second][start.first] = true;
  visitedFromEnd[goal.second][goal.first] = true;

  std::vector<int> directionsRows = {-1, 1, 0, 0};
  std::vector<int> directionsCollums = {0, 0, 1, -1};

  while (!frontierFromStart.empty() && !frontierFromEnd.empty() &&
         meetingPointX == -1) {
    int xFromStart = frontierFromStart.front().first;
    int yFromStart = frontierFromStart.front().second;
    int xFromEnd = frontierFromEnd.front().first;
    int yFromEnd = frontierFromEnd.front().second;

    frontierFromStart.pop();
    frontierFromEnd.pop();

    // this only works if they perfectly meet, the other condition later should
    // catch that but ill leave that here because im scared
    if (xFromStart == xFromEnd && yFromStart == yFromEnd) {
      meetingPointX = xFromStart;
      meetingPointY = yFromStart;
      break; // both paths met, so a path to the goal has been found
    }
    for (int i = 0; i < 4; i++) // check all neighbours
    {
      int xNFromStart = xFromStart + directionsRows[i];
      int yNFromStart = yFromStart + directionsCollums[i];
      int xNFromEnd = xFromEnd + directionsRows[i];
      int yNFromEnd = yFromEnd + directionsCollums[i];

      bool NFromStartValid = true;
      bool NFromEndValid = true;

      // check bounds
      if (xNFromStart < 0 || xNFromStart > rows - 1 || yNFromStart < 0 ||
          yNFromStart > collums - 1)
        NFromStartValid = false;

      if (xNFromEnd < 0 || xNFromEnd > rows - 1 || yNFromEnd < 0 ||
          yNFromEnd > collums - 1)
        NFromEndValid = false;

      // checked for blocked tile & if already visited
      if (NFromStartValid) {
        if (map[xNFromStart][yNFromStart] == true)
          NFromStartValid = false;
        if (visitedFromStart[xNFromStart][yNFromStart])
          NFromStartValid = false;
      }

      if (NFromEndValid) {
        if (map[xNFromEnd][yNFromEnd] == true)
          NFromEndValid = false;
        if (visitedFromEnd[xNFromEnd][yNFromEnd])
          NFromEndValid = false;
      }

      if (NFromStartValid) {
        visitedFromStart[xNFromStart][yNFromStart] = true;
        parentFromStart[xNFromStart][yNFromStart] =
            std::make_pair(xFromStart, yFromStart);
        frontierFromStart.push(std::make_pair(xNFromStart, yNFromStart));
        if (visitedFromEnd[xNFromStart][yNFromStart]) {
          meetingPointX = xNFromStart;
          meetingPointY = yNFromStart;
          break;
        }
      }

      if (NFromEndValid) {
        visitedFromEnd[xNFromEnd][yNFromEnd] = true;
        parentFromEnd[xNFromEnd][yNFromEnd] =
            std::make_pair(xFromEnd, yFromEnd);
        frontierFromEnd.push(std::make_pair(xNFromEnd, yNFromEnd));
        if (visitedFromStart[xNFromEnd][yNFromEnd]) {
          meetingPointX = xNFromEnd;
          meetingPointY = yNFromEnd;

          break;
        }
      }
    }
  }
  response["VISITEDFROMSTART"] = visitedFromStart;
  response["VISITEDFROMEND"] = visitedFromEnd;

  if (meetingPointX == -1)
    return {};

  // fist retrace all steps from the BFS starting at start
  std::pair<int, int> current = std::make_pair(meetingPointX, meetingPointY);

  while (current != start) {
    pathFromStart.push_back(current);
    current = parentFromStart[current.first][current.second];
  }
  pathFromStart.push_back(start);

  std::reverse(pathFromStart.begin(), pathFromStart.end());

  // and now same thing from the other direciton
  current = std::make_pair(meetingPointX, meetingPointY);

  while (current != goal) {
    pathFromEnd.push_back(current);
    current = parentFromEnd[current.first][current.second];
  }
  pathFromEnd.push_back(goal);

  // we do not have to reverse here because it is already the right order

  // now put em together
  // NOTE: +1 to skip the meeting point which is present in both
  pathFromStart.insert(pathFromStart.end(), pathFromEnd.begin() + 1,
                       pathFromEnd.end());

  for(int i = 0; i<pathFromStart.size();i++)
  {
    std::swap(pathFromStart[i].first,pathFromStart[i].second);
  }

  return pathFromStart;
}

// you can just copy the BFS and change the logic which takes the frontier
// element from "first of queue" to "last added" and boom you have DFS (and
// replace queue with stack)
std::vector<std::pair<int, int>>
DepthFirstSearch(std::vector<std::vector<bool>> map, std::pair<int, int> start,
                 std::pair<int, int> goal, json response) {
  // Frontier means what is it going to explore next, hence it being a queue

  const int rows = map.size();
  const int collums = map[0].size();

  std::vector<std::vector<bool>> visited(
      rows, std::vector<bool>(collums, false)); // weird ass constructor
  std::vector<std::vector<std::pair<int, int>>> parent(
      rows, std::vector<std::pair<int, int>>(collums, {-1, -1}));
  std::vector<std::pair<int, int>> path;

  std::stack<std::pair<int, int>> frontier;
  frontier.push({start.second,start.first});
  visited[start.second][start.first] = true;

  std::vector<int> directionsRows = {-1, 1, 0, 0};
  std::vector<int> directionsCollums = {0, 0, 1, -1};

  while (!frontier.empty()) {
    int x = frontier.top().first;
    int y = frontier.top().second;
    // because the first frontier element is now in x,y the first element of
    // frontier can be deleted
    frontier.pop();

    if (std::make_pair(y,x) == goal)
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

  response["VISITED"] = visited;

  if (!visited[goal.second][goal.first])
    return {}; // never once reached the goal means there was no path found to
               // the goal

  // retrace the steps via parent
  std::pair<int, int> current = {goal.second,goal.first};

  while (current != std::make_pair(start.second, start.first)) {
    path.push_back(current);
    current = parent[current.first][current.second];
  }

  // push back start pour fini
  path.push_back({start.second,start.first});

  // due to starting with goal, the path must be reversed to start from start
  std::reverse(path.begin(), path.end());

    for(int i = 0; i<path.size();i++)
  {
    std::swap(path[i].first,path[i].second);
  }
  return path;
}
