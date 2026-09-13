/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include <utility>
#include <vector>
#include <bits/stdc++.h>
#include <random>
#include <string>
#include <algorithm>
#include "../utils/utils.h"
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

void randomise(std::vector<int>& vec)
{
   static std::random_device rd;
    static std::mt19937 gen(rd());

    std::shuffle(vec.begin(), vec.end(), gen);

}
bool checkSorted(std::vector<int>& vec)
{
    int index = 0;
for(int i = 0; i<vec.size(); i++)
{
auto minT = std::min_element(vec.begin()+i,vec.end());
index = minT -vec.begin();
if(i != index) return false;
}
return true;
}

std::vector<int> bogoSort(std::vector<int> unsorted,std::vector<std::vector<int>>& tries)
{
std::cout<<"Bogo started";
    while(!std::is_sorted(unsorted.begin(),unsorted.end()))
{
    randomise(unsorted);
            tries.push_back(unsorted);
}
std::cout<<"Bogo finished";
   return unsorted;
}