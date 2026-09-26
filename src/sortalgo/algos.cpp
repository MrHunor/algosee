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

#include "../logger/logger.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <nlohmann/json.hpp>
#include <random>
#include <utility>
#include <vector>

using json = nlohmann::json;

void randomise(std::vector<int> &vec) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::shuffle(vec.begin(), vec.end(), gen);
}

void cycleSort(std::vector<int> &unsorted, json &response) {
  int NewPosition = 0;
  int current = 0;
  for (size_t i = 0; i < unsorted.size(); i++) {
    current = unsorted[i];
    NewPosition = 0;
    for (size_t z = i+1; z < unsorted.size(); z++) {
      if (current > unsorted[z])
        NewPosition++;
    }
    // already sorted?
    if (NewPosition == 0)
      continue;

    // is current swap position a duplicate?
    while (current == unsorted[NewPosition])
      NewPosition++;

      //intresing about this: it does not swap two items in the array, it swaps one OUT with another one; so remember this takes the displaced one OUT of the array and back into current and swaps in the correct value for the index
    std::swap(current, unsorted[NewPosition]);
    response["SWAPED"].push_back({current,NewPosition});

    //sort the displaced 
    while(NewPosition!=i)
    {
      NewPosition=i;
      for(size_t z = i; z<unsorted.size();z++ )
      {
        if(current>unsorted[z])NewPosition++;
      }
        // check For duplicates once again
        while (current == unsorted[NewPosition])      NewPosition++;
    std::swap(current,unsorted[NewPosition]);
    response["SWAPED"].push_back({current,NewPosition});
    }

  }
}

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

void bogoSort(std::vector<int> &unsorted, json &response) {
  if (unsorted.size() == 1)
    return; // an array of the size one is already sorted
  while (!std::is_sorted(unsorted.begin(), unsorted.end())) {
    randomise(unsorted);
    response["TRIES"].push_back(unsorted);
  }
}

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

// you could pass this by reference but that would indeed be a pain because it
// create a completely new array instead of modyfiying in place
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
