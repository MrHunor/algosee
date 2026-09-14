/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include "../utils/utils.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <random>
#include <string>
#include <utility>
#include <vector>


void randomise(std::vector<int> &vec) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::shuffle(vec.begin(), vec.end(), gen);
}


std::vector<int> selectionSort(std::vector<int> unsorted,
                               std::vector<std::pair<int, int>> &moves) {
  int index;

  for (int i = 0; i < unsorted.size() - 1;
       i++) // unsorted.size() is valid because the last element is sorted due
            // to the other elements already being sorted
  {
    auto minT = std::min_element(unsorted.begin() + i, unsorted.end());
    index = minT - unsorted.begin();
    moves.push_back({i, index});
    std::swap(unsorted[i], unsorted[index]);
  }

  return unsorted;
}


std::vector<int> bogoSort(std::vector<int> unsorted,
                          std::vector<std::vector<int>> &tries) {
  std::cout << "Bogo started";
  while (!std::is_sorted(unsorted.begin(), unsorted.end())) {
    randomise(unsorted);
    tries.push_back(unsorted);
  }
  std::cout << "Bogo finished";
  return unsorted;
}


std::vector<int> bubbleSort(std::vector<int> unsorted, std::vector<std::pair<int,int>> &moves)
{
for (int i = 0; i<unsorted.size(); i++) {
for(int z = 0; z<unsorted.size()-1-i; z++)//you can subtract i because the last elements have already been orderd before, this saves you some time but doesnt make the horrible O(n^2) much better
{
  if(unsorted[z]>unsorted[z+1])
  {
    moves.push_back({z,z+1});
    std::swap(unsorted[z],unsorted[z+1]);
  }
}

}
return unsorted;
}

