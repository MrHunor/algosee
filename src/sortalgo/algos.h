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

#ifndef SALGOS_H
#define SALGOS_H

#include "../defs/general.h"
#include "../defs/types.h"
#include "../stream/stream.h"
#include <nlohmann/json.hpp>
#include <vector>

//--------------------NUMBERS------------------------------
void SelectionSort(std::vector<int> &unsorted, json &response, StreamClass &stream);
void BogoSort(std::vector<int> &unsorted, json &response, StreamClass &stream);
void BubbleSort(std::vector<int> &unsorted, json &response, StreamClass &stream);
void QuickSort(std::vector<int> &unsorted, int low, int high, json &response,
               StreamClass &stream);
void MergeSort(std::vector<int> &unsorted, json &response, StreamClass &stream);
void CountingSort(std::vector<int> &unsorted, json &response, StreamClass &stream);
void CycleSort(std::vector<int> &unsorted, json &response, StreamClass &stream);

//-------------------IMAGES------------------------------------
void SelectionSort(indexPixel &unsorted, json &response, StreamClass &stream);
void BogoSort(indexPixel &unsorted, json &response, StreamClass &stream);
void BubbleSort(indexPixel &unsorted, json &response, StreamClass &stream);
void QuickSort(indexPixel &unsorted, int low, int high, json &response,
               StreamClass &stream);
void CountingSort(indexPixel &unsorted, json &response, StreamClass &stream);
void CycleSort(indexPixel &unsorted, json &response, StreamClass &stream);
void MergeSort(indexPixel &unsorted, json &response, StreamClass &stream);
#endif