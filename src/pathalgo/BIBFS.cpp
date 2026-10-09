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

std::vector<std::pair<int, int>> BIBFS(std::vector<std::vector<bool>> map,
                                       std::pair<int, int> start,
                                       std::pair<int, int> goal,
                                       json &response) {
                                        int iteration = 0;
  const int rows = map.size();
  const int collums = map[0].size();

  int meetingPointX = -1;
  int meetingPointY = -1;
  std::vector<std::vector<int>> visitedFromStart(
      rows,
      std::vector<int>(collums, -1)); // weird ass constructor once again
  std::vector<std::vector<int>> visitedFromEnd(
      rows,
      std::vector<int>(collums, -1)); // weird ass constructor once again

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
  visitedFromStart[start.second][start.first] = 0;
  visitedFromEnd[goal.second][goal.first] = 0;

  std::vector<int> directionsRows = {-1, 1, 0, 0};
  std::vector<int> directionsCollums = {0, 0, 1, -1};

  while (!frontierFromStart.empty() && !frontierFromEnd.empty() &&
         meetingPointX == -1) {
    int yFromStart = frontierFromStart.front().first;
    int xFromStart = frontierFromStart.front().second;
    int yFromEnd = frontierFromEnd.front().first;
    int xFromEnd = frontierFromEnd.front().second;

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
      int yNFromStart = yFromStart + directionsRows[i];
      int xNFromStart = xFromStart + directionsCollums[i];
      int yNFromEnd = yFromEnd + directionsRows[i];
      int xNFromEnd = xFromEnd + directionsCollums[i];

      bool NFromStartValid = true;
      bool NFromEndValid = true;

      // check bounds
      if (yNFromStart < 0 || yNFromStart > rows - 1 || xNFromStart < 0 ||
          xNFromStart > collums - 1)
        NFromStartValid = false;

      if (yNFromEnd < 0 || yNFromEnd > rows - 1 || xNFromEnd < 0 ||
          xNFromEnd > collums - 1)
        NFromEndValid = false;

      // checked for blocked tile & if already visited
      if (NFromStartValid) {
        if (map[yNFromStart][xNFromStart] == true)
          NFromStartValid = false;
        if (visitedFromStart[yNFromStart][xNFromStart]!=-1)
          NFromStartValid = false;
      }

      if (NFromEndValid) {
        if (map[yNFromEnd][xNFromEnd] == true)
          NFromEndValid = false;
        if (visitedFromEnd[yNFromEnd][xNFromEnd]!=-1)
          NFromEndValid = false;
      }

      if (NFromStartValid) {
        visitedFromStart[yNFromStart][xNFromStart] = iteration;
        parentFromStart[yNFromStart][xNFromStart] =
            std::make_pair(yFromStart, xFromStart);
        frontierFromStart.push(std::make_pair(yNFromStart, xNFromStart));
        if (visitedFromEnd[yNFromStart][xNFromStart]!=-1) {
          meetingPointX = xNFromStart;
          meetingPointY = yNFromStart;
          break;
        }
      }

      if (NFromEndValid) {
        visitedFromEnd[yNFromEnd][xNFromEnd] = iteration;
        parentFromEnd[yNFromEnd][xNFromEnd] =
            std::make_pair(yFromEnd, xFromEnd);
        frontierFromEnd.push(std::make_pair(yNFromEnd, xNFromEnd));
        if (visitedFromStart[yNFromEnd][xNFromEnd]!=-1) {
          meetingPointX = xNFromEnd;
          meetingPointY = yNFromEnd;

          break;
        }
      }
    }
    iteration++;
  }
  response["VISITEDFROMSTART"] = swapVisitedArray(visitedFromStart);
  response["VISITEDFROMEND"] =swapVisitedArray(visitedFromEnd);

  if (meetingPointX == -1)
    return {};

  // fist retrace all steps from the BFS starting at start
  std::pair<int, int> current = std::make_pair(meetingPointY, meetingPointX);

  while (current != std::make_pair(start.second, start.first)) {
    pathFromStart.push_back(current);
    current = parentFromStart[current.first][current.second];
  }
  pathFromStart.push_back({start.second, start.first});

  std::reverse(pathFromStart.begin(), pathFromStart.end());

  // and now same thing from the other direciton
  current = std::make_pair(meetingPointY, meetingPointX);

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