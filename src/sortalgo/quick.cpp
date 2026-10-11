/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
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

#include "../defs/general.h"
#include "../defs/types.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

// caution, you have to pass the vector as a reference here because the vector
// is being modified
int findQuickSortPivot(std::vector<int> &unsorted, int low, int high,
                       json &response) {
  int index;
  int pivot = unsorted[high];
  int lowerPivotIndexBoundry = low - 1;
  for (int i = low; i < high; i++) {
    if (unsorted[i] < pivot) {
      lowerPivotIndexBoundry++;
      std::swap(unsorted[i], unsorted[lowerPivotIndexBoundry]);
      response["MOVES"].push_back({i, lowerPivotIndexBoundry});
    }
  }
  // move pivot to correct pos;
  std::swap(unsorted[high], unsorted[lowerPivotIndexBoundry + 1]);
  response["MOVES"].push_back({high, lowerPivotIndexBoundry + 1});
  return lowerPivotIndexBoundry + 1;
}

void quickSort(std::vector<int> &unsorted, int low, int high, json &response) {
  if (low < high) // otherwise already sorted
  {
    int pivot = findQuickSortPivot(unsorted, low, high, response);

    quickSort(unsorted, low, pivot - 1, response); // sort left side recursivly
    quickSort(unsorted, pivot + 1, high,
              response); // sort right side recursivly
  }
}

//-----------------------IMAGE-----------------------------


// caution, you have to pass the vector as a reference here because the vector
// is being modified
int findQuickSortPivot(indexPixel& unsorted, int low, int high,
                       json &response) {
  int index;
  int pivot = unsorted[high].first;
  int lowerPivotIndexBoundry = low - 1;
  for (int i = low; i < high; i++) {
    if (unsorted[i].first < pivot) {
      lowerPivotIndexBoundry++;
      std::swap(unsorted[i], unsorted[lowerPivotIndexBoundry]);
      response["MOVES"].push_back({i, lowerPivotIndexBoundry});
    }
  }
  // move pivot to correct pos;
  std::swap(unsorted[high], unsorted[lowerPivotIndexBoundry + 1]);
  response["MOVES"].push_back({high, lowerPivotIndexBoundry + 1});
  return lowerPivotIndexBoundry + 1;
}

void quickSort(indexPixel& unsorted, int low, int high, json &response) {
  if (low < high) // otherwise already sorted
  {
    int pivot = findQuickSortPivot(unsorted, low, high, response);

    quickSort(unsorted, low, pivot - 1, response); // sort left side recursivly
    quickSort(unsorted, pivot + 1, high,
              response); // sort right side recursivly
  }
}