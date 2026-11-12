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
 * @file radix.cpp
 * @brief Contains the radix sort algorithms
 */

#include "../defs/general.h"
#include "../defs/types.h"
#include "../stream/stream.h"

#include <algorithm>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>
//working with long long, so it can handle INT_MIN
//the 

/// @brief Returns the maximum value in the vector.
/// @param unsorted the vector to find the maximum value in
/// @param n the max index to saearch in (usually the size of the vector)
/// @return the maximum value in the vector
int getMax(std::vector<long long> &unsorted, int n)
{
    long long max = unsorted[0];
    for (size_t i = 1; i < n; i++)
    {
        if (unsorted[i] > max)
            max = unsorted[i];
    }
    return max;
}

/// @brief Counting sort algorithm for radix sort, sorts the vector based on the digit
/// represented by place.
/// @param unsorted the unsorted vector to be sorted
/// @param place the digit place to sort by
/// @param response the respose json objet
void countingSort(std::vector<long long> &unsorted, int place, json &response)
{
    std::vector<long long> output(unsorted.size());
    int count[10] = {};

    for (long long value : unsorted)
        count[(value / place) % 10]++;
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    response["COUNT ARRAY"] = std::vector<int>(count, count + 10);
    for (int i = (int)(unsorted.size()) - 1; i >= 0; i--)
    {
        int digit = (unsorted[i] / place) % 10;
        output[--count[digit]] = unsorted[i];
    }
    unsorted = std::move(output);
}

void Sort(std::vector<long long> &values, json &response)
{
    if (values.empty())
        return;

    const int max = *std::max_element(values.begin(), values.end());

    for (int place = 1; max / place > 0;)
    {
        countingSort(values, place, response);

        if (place > max / 10)
            break;

        place *= 10;
    }
}

/// @brief The main function for radix sort.
/// @param unsorted the unsorted number vector
/// @param response the response json object
/// @param stream the stream class to send the response to
void RadixSort(std::vector<int> &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() <= 1)
        return;

    std::vector<long long> negative;
    std::vector<long long> positive;

    for (int value : unsorted)
    {
        if (value < 0)
            negative.push_back(-(long long)value);
        else
            positive.push_back(value);
    }

    Sort(negative, response);
    Sort(positive, response);

    int i = 0;

    for (auto it = negative.rbegin(); it != negative.rend(); ++it)
        unsorted[i++] = (int)(-*it);

    for (long long value : positive)
        unsorted[i++] = (int)(value);

    stream.sendChunk(std::move(response));
    response.clear();
}

//--------------IMAGE----------------------------- (is this even working?)
/// @brief Returns the maximum value in the vector.
/// @param unsorted the vector to find the maximum value in
/// @param n the max index to saearch in (usually the size of the vector)
/// @return the maximum value in the vector
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

/// @brief Counting sort algorithm for radix sort, sorts the indexPixels
/// @param unsorted the unsorted indexPixels to be sorted
/// @param place the digit place to sort by (the first element of the indexPixel pair)
/// @param response the respose json objet
void countingSort(indexPixel &unsorted, int place, json &response)
{
    if (unsorted.size() <= 1)
        return;

    std::vector<indexPixel::value_type> output(unsorted.size());
    int count[10] = {};
    for (const auto &item : unsorted)
        count[(item.first / place) % 10]++;
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    response["COUNT ARRAY"] = std::vector<int>(count, count + 10);
    for (int i = static_cast<int>(unsorted.size()) - 1; i >= 0; i--)
    {
        int digit = (unsorted[i].first / place) % 10;
        output[--count[digit]] = std::move(unsorted[i]);
    }
    unsorted = std::move(output);
}

/// @brief The main function for radix sort.
/// @param unsorted the unsorted indexPixels
/// @param response the response json object
/// @param stream the stream class to send the response to
void RadixSort(indexPixel &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() <= 1)
        return;

    const int max = getMax(unsorted, unsorted.size());

    for (int place = 1; max / place > 0; place *= 10)
    {
        countingSort(unsorted, place, response);
        if (place > max / 10)
            break;
    }
    stream.sendChunk(std::move(response));
    response.clear();
}