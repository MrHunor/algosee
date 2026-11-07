/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
#include <cmath>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

void InsertionSort(std::vector<int> &unsorted, json &response, int *begin, int *end)
{
    int left = begin - &unsorted[0];
    int right = end - &unsorted[0];
    for (int i = left + 1; i <= right; i++)
    {
        int key = unsorted[i];
        int j = i - 1;
        while (j >= left && unsorted[j] > key)
        {
            unsorted[j + 1] = unsorted[j];
            j--;
        }
        unsorted[j + 1] = key;
        response["MOVES"].push_back({i, unsorted});
    }
}

int FindQuickSortPivot2(std::vector<int> &unsorted, int low, int high,
                        json &response) // the first one in quick.cpp (for compiler)
{
    int pivot = unsorted[high];
    int lowerPivotIndexBoundry = low - 1;
    for (int i = low; i < high; i++)
    {
        if (unsorted[i] < pivot)
        {
            lowerPivotIndexBoundry++;
            std::swap(unsorted[i], unsorted[lowerPivotIndexBoundry]);
            response["MOVES"].push_back({i, lowerPivotIndexBoundry});
        }
    }
    std::swap(unsorted[high], unsorted[lowerPivotIndexBoundry + 1]);
    response["MOVES"].push_back({high, lowerPivotIndexBoundry + 1});
    return lowerPivotIndexBoundry + 1;
}

void QuickSort2(std::vector<int> &unsorted, int low, int high, json &response,
                StreamClass &stream) // same naming as previously
{
    if (response["MOVES"].size() > 1000)
    {
        stream.sendChunk(std::move(response));
        response.clear();
    }
    if (low < high)
    {
        int pivot = FindQuickSortPivot2(unsorted, low, high, response);
        QuickSort2(unsorted, low, pivot - 1, response,
                   stream); // sort left side recursivly
        QuickSort2(unsorted, pivot + 1, high, response,
                   stream); // sort right side recursivly
    }
}

int *MedianOfThree(int *a, int *b, int *c)
{
    if (a < b)
    {
        if (b < c)
            return b;
        if (a < c)
            return c;
        return a;
    }
    if (a < c)
        return a;
    if (b < c)
        return c;
    return b;
}

int *Partition(int *begin, int *end)
{
    int *pivot = end;
    int *store = begin;
    for (int *current = begin; current < end; ++current)
    {
        if (*current < *pivot)
        {
            std::swap(*current, *store);
            ++store;
        }
    }
    std::swap(*store, *end);
    return store;
}

void IntroSortUtil(std::vector<int> &unsorted, json &response, int *begin, int *end,
                   int depthLimit)
{
    int size = end - begin + 1;
    if (size <= 1)
        return;
    if (size < 16)
    {
        InsertionSort(unsorted, response, begin, end);
        return;
    }
    if (depthLimit == 0)
    {
        std::make_heap(begin, end + 1);
        std::sort_heap(begin, end + 1);
        return;
    }

    int *pivot = MedianOfThree(begin, begin + (size - 1) / 2, end);
    std::swap(*pivot, *end);
    int *partitionPoint = Partition(begin, end);
    if (partitionPoint > begin)
        IntroSortUtil(unsorted, response, begin, partitionPoint - 1, depthLimit - 1);
    if (partitionPoint < end)
        IntroSortUtil(unsorted, response, partitionPoint + 1, end, depthLimit - 1);
}

void IntroSort(std::vector<int> &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() <= 1)
        return;
    int depthLimit = 2 * (int)(std::log2(unsorted.size()));
    IntroSortUtil(unsorted, response, unsorted.data(),
                  unsorted.data() + unsorted.size() - 1, depthLimit);
}

//-------------- IMAGE -----------------------------

void InsertionSort(indexPixel &unsorted, indexPixel::value_type *begin,
                   indexPixel::value_type *end)
{
    int left = begin - unsorted.data();
    int right = end - unsorted.data();

    for (int i = left + 1; i <= right; i++)
    {
        auto key = std::move(unsorted[i]);
        int j = i - 1;
        while (j >= left && unsorted[j].first > key.first)
        {
            unsorted[j + 1] = std::move(unsorted[j]);
            j--;
        }
        unsorted[j + 1] = std::move(key);
    }
}

indexPixel::value_type *MedianOfThree(indexPixel::value_type *a,
                                      indexPixel::value_type *b,
                                      indexPixel::value_type *c)
{
    if (a->first < b->first)
    {
        if (b->first < c->first)
            return b;
        if (a->first < c->first)
            return c;
        return a;
    }
    if (a->first < c->first)
        return a;
    if (b->first < c->first)
        return c;
    return b;
}

indexPixel::value_type *Partition(indexPixel::value_type *begin,
                                  indexPixel::value_type *end)
{
    indexPixel::value_type *pivot = end;
    indexPixel::value_type *store = begin;
    for (indexPixel::value_type *current = begin; current < end; ++current)
    {
        if (current->first < pivot->first)
        {
            std::swap(*current, *store);
            ++store;
        }
    }
    std::swap(*store, *end);
    return store;
}

void IntroSortUtil(indexPixel &unsorted, indexPixel::value_type *begin,
                   indexPixel::value_type *end, int depthLimit)
{
    int size = end - begin + 1;
    if (size <= 1)
        return;
    if (size < 16)
    {
        InsertionSort(unsorted, begin, end);
        return;
    }
    if (depthLimit == 0)
    {
        auto comp = [](const indexPixel::value_type &a, const indexPixel::value_type &b) {
            return a.first < b.first;
        };

        std::make_heap(begin, end + 1, comp);
        std::sort_heap(begin, end + 1, comp);
        return;
    }

    indexPixel::value_type *pivot = MedianOfThree(begin, begin + (size - 1) / 2, end);
    std::swap(*pivot, *end);
    indexPixel::value_type *partitionPoint = Partition(begin, end);
    if (partitionPoint > begin)
        IntroSortUtil(unsorted, begin, partitionPoint - 1, depthLimit - 1);
    if (partitionPoint < end)
        IntroSortUtil(unsorted, partitionPoint + 1, end, depthLimit - 1);
}

void IntroSort(indexPixel &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() <= 1)
        return;
    int depthLimit = 2 * (int)(std::log2(unsorted.size()));
    IntroSortUtil(unsorted, unsorted.data(), unsorted.data() + unsorted.size() - 1,
                  depthLimit);
}