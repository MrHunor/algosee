/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, Vilmos45 (as equals)
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
 * @file validate.h
 *
 * @brief Defines Sorting validation function and its custom retrun type
 *
 */
#ifndef SORTVALIDATE_H
#define SORTVALIDATE_H
#include "../../logger/logger.h"
#include <httplib/httplib.h>
#include <vector>
struct SortValidReturn
{
    std::vector<int> values;
    std::string algo;
};

/**
 * @brief Check a request fits the requirements
 *
 * @details Checks for: algo parameter being present, algo parameter being equal to an
 *          implemented sorting algorithm, body containing a values array, values array
 *          not being
 *          empty, values not containing too many elements and values not containing too
 *          high or too
 *          low numbers for algo counting
 *
 * @param req A httplib::Request
 *
 * @param state The global StateClass variable
 *
 * @throws When encountering a case of the request not fitting the requirements
 *
 * @returns A custom struct with the needed variables for executing the algorithm
 *
 */
SortValidReturn ValidateSort(const httplib::Request &req, StateClass &state);
#endif