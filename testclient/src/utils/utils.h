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
#ifndef UTILS_H
#define UTILS_H
#include "../../src/defs/general.h"
#include "../../src/defs/types.h"
#include "../../src/logger/logger.h"
#include <source_location>
#include <string>
#include <vector>

struct run
{
    std::string algoname="";
    int size=-1;
    std::pair<int, int> mapSize={-1,-1};
    bool failure=false;
};

unsigned char *makeRandomImage(size_t w,size_t h);
rawImage RawImageFromJson(const json &j);
int randomInt(int lower, int upper);
std::string executeCommand(const std::string &command);
void InvalidInputMessage(const std::string &details,
                         std::source_location location = std::source_location::current());
void checkForCertainResponseType(const json &response, std::vector<std::string> expected);
void checkForAllResponseTypes(const json &response);
json parseNDJSON(const std::string &output);
void writeMemToImage(const rawImage &img);
void printReport(const std::vector<run> &exitMessage, StateClass &state);
#endif