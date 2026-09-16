/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#ifndef SALGOS_H
#define SALGOS

#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;
std::vector<int> selectionSort(std::vector<int> unsorted,
                               std::vector<std::pair<int, int>> &moves);
std::vector<int> bogoSort(std::vector<int> unsorted,
                          std::vector<std::vector<int>> &tries);
                          std::vector<int> bubbleSort(std::vector<int> unsorted, std::vector<std::pair<int,int>> &moves);
                          std::vector<int> mergeSort(const std::vector<int>& unsorted, json& response);
                          std::vector<int> countingSort(const std::vector<int>& unsorted, json& response);
#endif