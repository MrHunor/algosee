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
#include "../utils/image/image.h"
#include "../utils/sortalgo/sortalgo.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <vector>

void BogoSort(std::vector<int> &unsorted, json &response, StreamClass &stream)
{

    if (unsorted.size() == 1)
        return; // an array of the size one is already sorted
    while (!std::is_sorted(unsorted.begin(), unsorted.end()))
    {
        Randomise(unsorted);
        response["TRIES"].push_back(unsorted);
        if (response["TRIES"].size() > 1000)
        { // if more then 100 tries recorded then send a streaming chunk
            stream.sendChunk(std::move(response));
            response.clear();
        }
    }
}

//--------------IMAGE-------------------
void BogoSort(indexPixel &unsorted, json &response, StreamClass &stream)
{
    if (unsorted.size() == 1)
        return; // an array of the size one is already sorted
    while (
        !std::is_sorted(unsorted.begin(), unsorted.end(),
                        [](const auto &a, const auto &b) { return a.first < b.first; }))
    {
        Randomise(unsorted);
        response["TRIES"].push_back(IndexPixelArrToJson(unsorted));
        if (response["TRIES"].size() > 1000)
        { // if more then 100 tries recorded then send a streaming chunk
            stream.sendChunk(std::move(response));
            response.clear();
        }
    }
}