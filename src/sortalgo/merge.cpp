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
#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <vector>

// NEVER EVER CALL THIS FUNCTION DIRECTLY IF YOU DIDNT CHECK FOR arr1.size() &
// arr2.size()>=1
std::vector<int> merge(const std::vector<int> &arr1,
                       const std::vector<int> &arr2) {
  int arr1point = 0;
  int arr2point = 0;
  size_t mid = arr1.size() - 1;
  size_t end = arr2.size() - 1;
  std::vector<int> retval;
  retval.reserve(arr1.size() + arr2.size());

  while (arr1point <= mid && arr2point <= end) {
    if (arr1[arr1point] < arr2[arr2point]) {
      retval.push_back(arr1[arr1point]);
      arr1point++;
    } else if (arr2[arr2point] < arr1[arr1point]) {
      retval.push_back(arr2[arr2point]);
      arr2point++;
    } else {
      retval.push_back(arr1[arr1point]);
      arr1point++;
      retval.push_back(arr2[arr2point]);
      arr2point++;
    }
  }

  while (arr1point <= mid) {
    retval.push_back(arr1[arr1point]);
    arr1point++;
  }
  while (arr2point <= end) {
    retval.push_back(arr2[arr2point]);
    arr2point++;
  }
  return retval;
}

void mergeSort(std::vector<int> &unsorted, json &response) {
  if (unsorted.size() <= 1)
    return; // an array of the size one is already sorted

  size_t mid = unsorted.size() / 2; // beg to god rounding logic works

  std::vector<int> left(
      unsorted.begin(),
      unsorted.begin() +
          mid); // fun fact; last iterator is the one NOT to copy anymore, so
                // actually the last one to copy is last-1
  std::vector<int> right(unsorted.begin() + mid, unsorted.end());
  response["SPLIT (ORIGIN,LEFT,RIGHT)"].push_back({unsorted, left, right});

  // recursion magic
  mergeSort(left, response);
  mergeSort(right, response);
  response["SORTED (LEFT,RIGHT)"].push_back({left, right});
  unsorted = merge(left, right);
  response["MERGED (LEFT,RIGHT,RESULT)"].push_back({left, right, unsorted});
}