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

void bubbleSort(std::vector<int> &unsorted, json &response) {
  if (unsorted.size() <= 1)
    return; // an array of the size one is already sorted
  for (size_t i = 0; i < unsorted.size(); i++) {
    for (size_t z = 0; z < unsorted.size() - 1 - i;
         z++) // you can subtract i because the last elements have already been
              // orderd before, this saves you some time but doesnt make the
              // horrible O(n^2) much better
    {
      if (unsorted[z] > unsorted[z + 1]) {
        response["MOVES"].push_back({z, z + 1});
        std::swap(unsorted[z], unsorted[z + 1]);
      }
    }
  }
}

//--------------IMAGE-----------------------------------

void bubbleSort(indexPixel& unsorted, json &response) {
  if (unsorted.size() <= 1)
    return; // an array of the size one is already sorted
  for (size_t i = 0; i < unsorted.size(); i++) {
    for (size_t z = 0; z < unsorted.size() - 1 - i;
         z++) // you can subtract i because the last elements have already been
              // orderd before, this saves you some time but doesnt make the
              // horrible O(n^2) much better
    {
      if (unsorted[z].first > unsorted[z + 1].first) {
        response["MOVES"].push_back({z, z + 1});
        std::swap(unsorted[z],unsorted[z + 1]);
      }
    }
  }
}
