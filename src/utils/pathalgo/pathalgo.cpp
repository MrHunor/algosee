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
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

std::string checkMapValidness(json parsed, std::pair<int, int> start,
                              std::pair<int, int> goal) {
  int collums = parsed["MAP"][0].size();
  bool sameRowLength = true;
  for (int i = 1; i < parsed["MAP"].size(); i++) {
    if (parsed["MAP"][i].size() != collums) {
      sameRowLength = false;
      break;
    }
  }
  if (!sameRowLength) {
    return "Rows have diffrent lengths.";
  }

  if (start.first < 0 || start.first >= parsed["MAP"][0].size() ||
      start.second < 0 || start.second >= parsed["MAP"].size() ||
      goal.first < 0 || goal.first >= parsed["MAP"][0].size() ||
      goal.second < 0 || goal.second >= parsed["MAP"].size()) {
    return "Start or goal coordinates invalid (out of map).";
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
