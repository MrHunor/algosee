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
#include "../stream/stream.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

int getMax(std::vector<int> &unsorted, int n)
{
    int max = unsorted[0];
    for (size_t i = 1; i < n; i++)
    {
        if (unsorted[i] > max)
            max = unsorted[i];
    }
    return max;
}

void countingSort(std::vector<int> &unsorted, int n, int place){
    int outp[n];
    int count[10];
    for (int i = 0; i < 10; i++) count[i] = 0;
    for (int i = 0; i < n; i++)  count[(unsorted[i] / place) % 10]++; //count
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];//cumulative count
    for (int i = n - 1; i >= 0; i--)//sort
    {
        outp[count[(unsorted[i] / place) % 10] - 1] = unsorted[i];
        count[(unsorted[i] / place) % 10]--;
    }
    for (int i = 0; i < n; i++) unsorted[i] = outp[i];
}

void RadixSort(std::vector<int> &unsorted, json &response, StreamClass &stream)
{
    const int max = getMax(unsorted, unsorted.size());
    int i = 0;
    for (int place = 1; max / place > 0; place *= 10){
        countingSort(unsorted, unsorted.size(), place);
        i++;
    }
    response["MOVES"].push_back({i});
}

//--------------IMAGE-----------------------------
int getMax(indexPixel &unsorted, int n)
{
    int max = unsorted[0].first;
    for (size_t i = 1; i < n; i++)
    {
        if (unsorted[i].first > max)
            max = unsorted[i].first;
    }
    return max;
}

void countingSort(indexPixel &unsorted, int n, int place){
    int outp[n];
    int count[10];
    for (int i = 0; i < 10; i++) count[i] = 0;
    for (int i = 0; i < n; i++)  count[(unsorted[i].first / place) % 10]++; //count
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];//cumulative count
    for (int i = n - 1; i >= 0; i--)//sort
    {
        outp[count[(unsorted[i].first / place) % 10] - 1] = unsorted[i].first;
        count[(unsorted[i].first / place) % 10]--;
    }
    for (int i = 0; i < n; i++) unsorted[i].first = outp[i];
}

void RadixSort(indexPixel &unsorted, json &response, StreamClass &stream)
{
    const int max = getMax(unsorted, unsorted.size());
    int i = 0;
    for (int place = 1; max / place > 0; place *= 10){
        countingSort(unsorted, unsorted.size(), place);
        i++;
    }
    response["MOVES"].push_back({i});
}