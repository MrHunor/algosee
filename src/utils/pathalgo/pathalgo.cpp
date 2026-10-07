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
#include "../../config.h"
#include "../../logger/logger.h"
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <vector>

json swapVisitedArray(const std::vector<std::vector<bool>> &visited) {
  std::vector<std::vector<bool>> retval(visited[0].size(),std::vector<bool>(visited.size(),false));
  for (int rows = 0; rows < visited.size(); rows++) {
    for (int cols = 0; cols < visited[rows].size(); cols++) {
      retval[cols][rows] = visited[rows][cols];
    }
  }
  return json(retval);
}

//dijkstra uses the same thing but as int, so we can overload this
json swapVisitedArray(const std::vector<std::vector<int>> &visited) {
  std::vector<std::vector<int>> retval(visited[0].size(),std::vector<int>(visited.size(),0));

  for (int rows = 0; rows < visited.size(); rows++) {
    for (int cols = 0; cols < visited[rows].size(); cols++) {
      retval[cols][rows] = visited[rows][cols];
    }
  }
  return json(retval);
}

void checkMapValidness(const std::vector<std::vector<int>> &map,
                       const std::pair<int, int> &start,
                       const std::pair<int, int> &goal) {
  if (map.empty()) {
    throw std::runtime_error("Bad Map: Map is empty.");
  }
  int rows = map.size();
  int cols = map[0].size();
  size_t rowLength = map[0].size();
  bool sameRowLength = true;
  for (const auto &row : map) {
    if (row.size() != rowLength) {
      sameRowLength = false;
      break;
    }
  }
  if (!sameRowLength) {
    throw std::runtime_error("Bad Map:Rows dont have same row length.");
  }

  if (start.first < 0 || start.first > cols - 1 || start.second < 0 ||
      start.second > rows - 1) {
    throw std::runtime_error("Bad Map: Start out of Map Bounds.");
  }

  if (goal.first < 0 || goal.first > cols - 1 || goal.second < 0 ||
      goal.second > rows - 1) {
    throw std::runtime_error("Bad Map: End out of Map Bounds");
  }
}

void checkMapValidness(const std::vector<std::vector<bool>> &map,
                       const std::pair<int, int> &start,
                       const std::pair<int, int> &goal) {
  if (map.empty()) {
    throw std::runtime_error("Bad Map: Map is empty.");
  }
  int rows = map.size();
  int cols = map[0].size();
  size_t rowLength = map[0].size();
  bool sameRowLength = true;
  for (const auto &row : map) {
    if (row.size() != rowLength) {
      sameRowLength = false;
      break;
    }
  }
  if (!sameRowLength) {
    throw std::runtime_error("Bad Map:Rows dont have same row length.");
  }

  if (start.first < 0 || start.first > cols - 1 || start.second < 0 ||
      start.second > rows - 1) {
    throw std::runtime_error("Bad Map: Start out of Map Bounds.");
  }

  if (goal.first < 0 || goal.first > cols - 1 || goal.second < 0 ||
      goal.second > rows - 1) {
    throw std::runtime_error("Bad Map: End out of Map Bounds");
  }
}

std::vector<std::vector<bool>> castIntMapToBoolIfNeeded(const json &input) {
  bool isBool = true;
  // check if input array is already boolean
  for (const auto &row : input["MAP"]) {
    for (const auto &element : row) {
      if (!element.is_boolean())
        isBool = false;
    }
  }

  if (isBool == true)
    return input["MAP"];

  std::vector<std::vector<bool>> retval;
  std::vector<bool> retvalRow;
  for (const auto &row : input["MAP"]) {
    retvalRow.clear();
    for (const auto &element : row) {
      if (element == 0)
        retvalRow.push_back(false);
      else if (element == 1)
        retvalRow.push_back(true);
      else
        return {};
    }
    retval.push_back(retvalRow);
  }
  return retval;
}
