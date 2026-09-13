/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#ifndef SALGOS_H
#define SALGOS

#include <vector>

std::vector<int> selectionSort(std::vector<int> unsorted, std::vector<std::pair<int,int>>& moves);
std::vector<int> bogoSort(std::vector<int> unsorted,std::vector<std::vector<int>>& tries);
#endif