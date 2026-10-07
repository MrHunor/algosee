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
#include <vector>

std::string checkMapValidness(const std::vector<std::vector<int>> &map,
                              const std::pair<int, int> &start,
                              const std::pair<int, int> &goal) {
  if (map.empty()) {
    return "Bad Map: Map is empty.";
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
    return "Bad Map:Rows dont have same row length.";
  }

  if (start.first < 0 || start.first > cols - 1 || start.second < 0 ||
      start.second > rows - 1) {
    return "Bad Map: Start out of Map Bounds.";
  }

  if (goal.first < 0 || goal.first > cols - 1 || goal.second < 0 ||
      goal.second > rows - 1) {
    return "Bad Map: End out of Map Bounds";
  }
  return "";
}

std::string checkMapValidness(const std::vector<std::vector<bool>> &map,
                              const std::pair<int, int> &start,
                              const std::pair<int, int> &goal) {
  if (map.empty()) {
    return "Bad Map: Map is empty.";
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
    return "Bad Map:Rows dont have same row length.";
  }

  if (start.first < 0 || start.first > cols - 1 || start.second < 0 ||
      start.second > rows - 1) {
    return "Bad Map: Start out of Map Bounds.";
  }

  if (goal.first < 0 || goal.first > cols - 1 || goal.second < 0 ||
      goal.second > rows - 1) {
    return "Bad Map: End out of Map Bounds";
  }
  return "";
}

std::vector<std::vector<bool>> castIntMapToBoolIfNeeded(const json &input) {
  bool isBool = true;
  // check if input array is already boolean
  try {
    for (const auto &row : input["MAP"]) {
      for (const auto &element : row) {
        if (!element.is_boolean())
          isBool = false;
      }
    }

  } catch (const std::exception &e) {
    return {};
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
