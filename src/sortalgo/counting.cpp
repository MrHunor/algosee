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
#include <vector>

void countingSort(std::vector<int> &unsorted, json &response) {
  if (unsorted.size() <= 1)
    return; // an array of the size one is already sorted
  int countI = 0;
  std::vector<int> count(*std::max_element(unsorted.begin(), unsorted.end()) +
                         1);
  std::vector<int> retval;
  retval.reserve(unsorted.size());
  for (int i : unsorted) {
    count[i]++;
  }
  response["COUNT ARRAY:"] = count;
  for (size_t i = 0; i < unsorted.size(); i++) {
    while (count[countI] == 0)
      countI++;
    retval.push_back(countI);
    count[countI]--;
  }

  unsorted = retval;
}
