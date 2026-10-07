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
#include <algorithm>
#include <nlohmann/json.hpp>
#include <queue>
#include <utility>
#include <vector>

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

  frontierFromStart.push({start.second, start.first});
  frontierFromEnd.push({goal.second, goal.first});
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

  while (current != std::make_pair(start.second, start.first)) {
    pathFromStart.push_back(current);
    current = parentFromStart[current.first][current.second];
  }
  pathFromStart.push_back({start.second, start.first});

  std::reverse(pathFromStart.begin(), pathFromStart.end());

  // and now same thing from the other direciton
  current = std::make_pair(meetingPointX, meetingPointY);

  while (current != std::make_pair(goal.second, goal.first)) {
    pathFromEnd.push_back(current);
    current = parentFromEnd[current.first][current.second];
  }
  pathFromEnd.push_back({goal.second, goal.first});

  // we do not have to reverse here because it is already the right order

  // now put em together
  // NOTE: +1 to skip the meeting point which is present in both
  pathFromStart.insert(pathFromStart.end(), pathFromEnd.begin() + 1,
                       pathFromEnd.end());

  for (int i = 0; i < pathFromStart.size(); i++) {
    std::swap(pathFromStart[i].first, pathFromStart[i].second);
  }

  return pathFromStart;
}