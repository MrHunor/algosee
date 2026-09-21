/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#define SALGOS

#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;
void selectionSort(std::vector<int> &unsorted,
                   std::vector<std::pair<int, int>> &moves);
void bogoSort(std::vector<int> &unsorted, std::vector<std::vector<int>> &tries);
void bubbleSort(std::vector<int> &unsorted,
                std::vector<std::pair<int, int>> &moves);
void quickSort(std::vector<int> &unsorted, int low, int high,
               std::vector<std::pair<int, int>> &moves);
void mergeSort(std::vector<int> &unsorted, json &response);

std::vector<int> countingSort(const std::vector<int> &unsorted, json &response);
#endif