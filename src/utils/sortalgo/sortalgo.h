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
 * @file sortalgo.h
 *
 * @brief Contains defintions for sortalgo utility functions
 *
 */

#include "../../defs/types.h"

#include <vector>
#ifndef SORTALGO_UTILS_H
#define SORTALGO_UTILS_H
/**
 * @brief Randomise a integer vector in place
 *
 * @param vec The vector of integer to randomise (or more specifially std::shuffle)
 *
 * @throws Never
 *
 * @returns Nothing
 */
void Randomise(std::vector<int> &vec);

/**
 * @brief Randomise a indexPixel vector in place
 *
 * @param vec The vector of indexpixel to randomise (or more specifially std::shuffle)
 *
 * @throws Never
 *
 * @returns Nothing
 */
void Randomise(indexPixel &vec);
#endif