/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include <utility>
#include <vector>
#include <string>
#include <algorithm>

std::vector<int> selectionSort(std::vector<int> unsorted, std::vector<std::pair<int,int>>& moves)
{
int index;

for(int i = 0; i<unsorted.size()-1;i++)//unsorted.size() is valid because the last element is sorted due to the other elements already being sorted
{
auto minT = std::min_element(unsorted.begin()+i,unsorted.end());
index = minT - unsorted.begin();
moves.push_back({i,index});
std::swap(unsorted[i],unsorted[index]);
}

return unsorted;
}