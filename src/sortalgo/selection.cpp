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
#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

void selectionSort(std::vector<int> &unsorted, json &response) {
  if (unsorted.size() <= 1)
    return; // an array of the size one is already sorted
  int index;

  for (size_t i = 0; i < unsorted.size() - 1;
       i++) // unsorted.size() is valid because the last element is sorted due
            // to the other elements already being sorted
  {
    auto minT = std::min_element(unsorted.begin() + i, unsorted.end());
    index = minT - unsorted.begin();
    response["MOVES"].push_back({i, index});
    std::swap(unsorted[i], unsorted[index]);
  }
}

//--------------IMAGE-----------------------------
void selectionSort(indexPixel& unsorted, json &response) {
  if (unsorted.size() <= 1)
    return; // an array of the size one is already sorted
  int index;

  for (size_t i = 0; i < unsorted.size() - 1;
       i++) // unsorted.size() is valid because the last element is sorted due
            // to the other elements already being sorted
  {
    auto minT = std::min_element(
        unsorted.begin() + i, unsorted.end(),
        [](const auto &a, const auto &b) { return a.first < b.first; });
    index = minT - unsorted.begin();
    response["MOVES"].push_back({i, index});
    std::swap(unsorted[i], unsorted[index]);
  }
}