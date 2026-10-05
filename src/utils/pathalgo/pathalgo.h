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

#ifndef PATHALGO_UTILS_H
#define PATHALGO_UTILS_H
#include "../../config.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

json swapVisitedArray(const std::vector<std::vector<bool>> &visited);
json swapVisitedArray(const std::vector<std::vector<int>> &visited);
void checkMapValidness(const std::vector<std::vector<int>> &mapInt,
                       const std::pair<int, int> &start,
                       const std::pair<int, int> &goal);
void checkMapValidness(const std::vector<std::vector<bool>> &map,
                       const std::pair<int, int> &start,
                       const std::pair<int, int> &goal);
std::vector<std::vector<bool>> castIntMapToBoolIfNeeded(const json &input);
#endif