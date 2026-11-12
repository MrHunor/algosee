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

 /**
 * @file intro.cpp
 * @brief Contains the intro sort algorithms
 */
#include "../defs/general.h"
#include "../defs/types.h"
#include "../stream/stream.h"
#include <algorithm>
#include <cmath>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

/// @brief an insertion sort for the intro sort algorithm, sorts between the begin and end
/// pointers
/// @param unsorted the unsorted number vector
/// @param response the response json
/// @param begin the begin pointer (from where it will be sorted, inclusive)
/// @param end the end pointer (to where it will be sorted, inclusive)
/// @param stream the stream class to send the response to
void InsertionSort(std::vector<int> &unsorted, json &response, int *begin, int *end, StreamClass &stream)
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
            response["MOVES"].push_back({j + 1, j});
            j--;
        }
        unsorted[j + 1] = key;
    }
}

/// @brief Finds the pivot index for quicksort
/// @param unsorted the unsorted number vector
/// @param low from where to start searching for the pivot
/// @param high to where to search for the pivot
/// @param response the response json
/// @return the pivot index, for quicksort
int FindQuickSortPivot2(std::vector<int> &unsorted, int low, int high,
                        json &response) // the first one is in quick.cpp (for compiler)
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

/// @brief A quicksort for the intro sort algorithm, sorts between the low and high indexes
/// @param unsorted the unsorted number vector
/// @param low from where to start quicksorting
/// @param high to where to quicksort
/// @param response the response json
/// @param stream the stream class to send the response to
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
        QuickSort2(unsorted, low, pivot - 1, response, stream); // sort left side recursivly
        QuickSort2(unsorted, pivot + 1, high, response, stream); // sort right side recursivly
    }
}

/// @brief Returns the median of the three int pointed values
/// @param a the first pointer
/// @param b the second pointer
/// @param c the third pointer
/// @return the pointer to the median value, of the three imputs
int *MedianOfThree(int *a, int *b, int *c)
{
    if (*a < *b)
    {
        if (*b < *c)
            return b;
        if (*a < *c)
            return c;
        return a;
    }

    if (*a < *c)
        return a;
    if (*b < *c)
        return c;
    return b;
}

/// @brief Partitions the vector between the begin and end pointers
/// @param begin from where to start partitioning
/// @param end to where to partition
/// @param response the response json
/// @param stream the stream class to send the response to
/// @return the pointer to the partition point (the border between the 2 partitions)
int *Partition(int *begin, int *end, json &response, StreamClass &stream)
{
    int *pivot = end;
    int *store = begin;
    for (int *current = begin; current < end; ++current)
    {
        if (*current < *pivot)
        {
            std::swap(*current, *store);
            response["MOVES"].push_back({current - begin, store - begin});
            stream.sendChunk(std::move(response));
            response.clear();
            ++store;
        }
    }
    std::swap(*store, *end);
    response["MOVES"].push_back({store - begin, end - begin});
    stream.sendChunk(std::move(response));
    response.clear();
    return store;
}

/// @brief a separate function for the intro sort, with recursive logic
/// @param unsorted the unsorted number vector
/// @param response the response json
/// @param begin from where to start sorting
/// @param end to where to sort
/// @param depthLimit the maximum depth of recursion
/// @param stream the stream class to send the response to
void IntroSortUtil(std::vector<int> &unsorted, json &response, int *begin, int *end, int depthLimit, StreamClass &stream)
{
    int size = end - begin + 1;
    if (size <= 1)
        return;
    if (size < 16)
    {
        InsertionSort(unsorted, response, begin, end, stream);
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
    int *partitionPoint = Partition(begin, end, response, stream);
    if (partitionPoint > begin)
        IntroSortUtil(unsorted, response, begin, partitionPoint - 1, depthLimit - 1, stream);
    if (partitionPoint < end)
        IntroSortUtil(unsorted, response, partitionPoint + 1, end, depthLimit - 1, stream);
}

/// @brief The main intro sort function
/// @param unsorted the unsorted number vector
/// @param response the response json
/// @param stream the stream class to send the response to
void IntroSort(std::vector<int> &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() <= 1)
        return;
    int depthLimit = 2 * (int)(std::log2(unsorted.size()));
    IntroSortUtil(unsorted, response, unsorted.data(), unsorted.data() + unsorted.size() - 1, depthLimit, stream);
    stream.sendChunk(std::move(response));
    response.clear();
}

//-------------- IMAGE -----------------------------
/// @brief an insertion sort for the intro sort algorithm, sorts between the begin and end
/// pointers
/// @param unsorted the unsorted indexPixels
/// @param begin the begin pointer (from where it will be sorted, inclusive)
/// @param end the end pointer (to where it will be sorted, inclusive)
/// @param response the response json
/// @param stream the stream class to send the response to
void InsertionSort(indexPixel &unsorted, indexPixel::value_type *begin, indexPixel::value_type *end, json &response, StreamClass &stream)
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
            stream.sendChunk(std::move(response));
            response.clear();
            j--;
        }
        unsorted[j + 1] = std::move(key);
    }
}

/// @brief Returns the median of the three indexPixel type pointed values
/// @param a the first pointer
/// @param b the second pointer
/// @param c the third pointer
/// @return the pointer to the median value, of the three imputs, an indexPixel
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

/// @brief Partitions the indexPixels between the begin and end pointers
/// @param begin from where to start partitioning
/// @param end to where to partition
/// @param response the response json
/// @param stream the stream class to send the response to
/// @return the pointer to the partition point (the border between the 2 partitions)
indexPixel::value_type *Partition(indexPixel::value_type *begin, indexPixel::value_type *end, json &response, StreamClass &stream)
{
    indexPixel::value_type *pivot = end;
    indexPixel::value_type *store = begin;
    for (indexPixel::value_type *current = begin; current < end; ++current)
    {
        if (current->first < pivot->first)
        {
            std::swap(*current, *store);
            response["MOVES"].push_back({current->first - begin->first, store->first});
            stream.sendChunk(std::move(response));
            response.clear();
            ++store;
        }
    }
    std::swap(*store, *end);
    response["MOVES"].push_back({store->first - begin->first, store->first});
    stream.sendChunk(std::move(response));
    response.clear();
    return store;
}

/// @brief a separate function for the intro sort, with recursive logic
/// @param unsorted the unsorted indexPixels
/// @param response the response json
/// @param begin from where to start sorting
/// @param end to where to sort
/// @param depthLimit the maximum depth of recursion
/// @param stream the stream class to send the response to
void IntroSortUtil(indexPixel &unsorted, json &response, indexPixel::value_type *begin, indexPixel::value_type *end, int depthLimit, StreamClass &stream)
{
    int size = end - begin + 1;
    if (size <= 1)
        return;
    if (size < 16)
    {
        InsertionSort(unsorted, begin, end, response, stream);
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
    indexPixel::value_type *partitionPoint = Partition(begin, end, response, stream);
    if (partitionPoint > begin)
        IntroSortUtil(unsorted, response, begin, partitionPoint - 1, depthLimit - 1, stream);
    if (partitionPoint < end)
        IntroSortUtil(unsorted, response, partitionPoint + 1, end, depthLimit - 1, stream);
}

/// @brief The main intro sort function
/// @param unsorted the unsorted indexPixels
/// @param response the response json
/// @param stream the stream class to send the response to
void IntroSort(indexPixel &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() <= 1)
        return;
    int depthLimit = 2 * (int)(std::log2(unsorted.size()));
    IntroSortUtil(unsorted, response, unsorted.data(), unsorted.data() + unsorted.size() - 1, depthLimit, stream);
}