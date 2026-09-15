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
                                  if(unsorted.size()<=1)return unsorted; //an array of the size one is already sorted
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
                              if(unsorted.size()==1)return unsorted; //an array of the size one is already sorted
  while (!std::is_sorted(unsorted.begin(), unsorted.end())) {
    randomise(unsorted);
    tries.push_back(unsorted);
  }
  return unsorted;
}


std::vector<int> bubbleSort(std::vector<int> unsorted, std::vector<std::pair<int,int>> &moves)
{
    if(unsorted.size()<=1)return unsorted; //an array of the size one is already sorted
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

std::vector<int> merge(const std::vector<int>& arr1, const std::vector<int>& arr2)
{
int arr1point=0;
int arr2point=0;
int mid = arr1.size()-1;
int end = arr2.size()-1;
std::vector<int> retval;
retval.reserve(arr1.size()+arr2.size());

while(arr1point<=mid && arr2point<=end)
{
if(arr1[arr1point]<arr2[arr2point])
{
  retval.push_back(arr1[arr1point]);
  arr1point++;
}
else if(arr2[arr2point]<arr1[arr1point])
{
  retval.push_back(arr2[arr2point]);
  arr2point++;
}
else {
 retval.push_back(arr1[arr1point]);
  arr1point++;
  retval.push_back(arr2[arr2point]);
  arr2point++;
}

}

while (arr1point <= mid) { retval.push_back(arr1[arr1point]); arr1point++; }
while (arr2point <= end) { retval.push_back(arr2[arr2point]); arr2point++; }
return retval;

}

std::vector<int> mergeSort(const std::vector<int>& unsorted, std::vector<std::pair<int,int>> &moves)
{
  if(unsorted.size()<=1)return unsorted; //an array of the size one is already sorted

  int mid = unsorted.size()/2;//beg to god rounding logic works

  std::vector<int> left(unsorted.begin(),unsorted.begin()+mid);//fun fact; last iterator is the one NOT to copy anymore, so actually the last one to copy is last-1
  std::vector<int> right(unsorted.begin()+mid,unsorted.end()); 

  //recursion magic
  left = mergeSort(left,moves);
  right = mergeSort(right,moves);

 return merge(left,right);

}

std::vector<int> countingSort(const std::vector<int>& unsorted, std::vector<std::pair<int,int>> &moves)
{
    if(unsorted.size()<=1)return unsorted; //an array of the size one is already sorted
  int countI=0;
std::vector<int> count(*std::max_element(unsorted.begin(),unsorted.end())+1);
std::vector<int> retval;
retval.reserve(unsorted.size());
for(int i : unsorted)
{
count[i]++;
}
for(int i = 0; i<unsorted.size(); i++)
{
while(count[countI]==0)countI++;
retval.push_back(countI);
count[countI]--;
}

return retval;
}

