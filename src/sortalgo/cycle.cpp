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

void cycleSort(std::vector<int> &unsorted, json &response) {
  int NewPosition = 0;
  int current = 0;
  for (size_t i = 0; i < unsorted.size(); i++) {
    current = unsorted[i];
    NewPosition = i;
    for (size_t z = i + 1; z < unsorted.size(); z++) {
      if (current > unsorted[z])
        NewPosition++;
    }
    // already sorted?
    if (NewPosition == i)
      continue;

    // is current swap position a duplicate?
    while (current == unsorted[NewPosition])
      NewPosition++;

    // intresing about this: it does not swap two items in the array, it swaps
    // one OUT with another one; so remember this takes the displaced one OUT of
    // the array and back into current and swaps in the correct value for the
    // index
    std::swap(current, unsorted[NewPosition]);
    response["SWAPED"].push_back({current, NewPosition});

    // sort the displaced
    while (NewPosition != i) {
      NewPosition = i;
      for (size_t z = i + 1; z < unsorted.size(); z++) {
        if (current > unsorted[z])
          NewPosition++;
      }
      // check For duplicates once again
      while (current == unsorted[NewPosition])
        NewPosition++;
      std::swap(current, unsorted[NewPosition]);
      response["SWAPED"].push_back({current, NewPosition});
    }
  }
}