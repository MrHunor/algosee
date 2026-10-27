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
/**
 * @file utils.h
 *
 * @brief Contains function declarations for helper functions
 *
 */
#ifndef UTILS_H
#define UTILS_H
#include "../../src/defs/general.h"
#include "../../src/defs/types.h"
#include "../../src/logger/logger.h"
#include <source_location>
#include <string>
#include <vector>

/**
 * @brief Structure for logging testruns
 *
 */
struct run
{
    std::string algoname = "";
    int size = -1;
    std::pair<int, int> mapSize = {-1, -1};
    bool failure = false;
};

/**
 * @brief Create a Image with random colours in memory with 4 channels
 *
 * @param w A size_t that specifies the width of the desired image
 *
 * @param h A size_t that specifies the height of the desired image
 *
 * @throws Probably when no more memory is left to alloc a image in memory of the desired
 * size (w*h*4)
 *
 * @returns pointer to unsigned char containing the image data as [R,G,B,A]
 *
 */
unsigned char *makeRandomImage(size_t w, size_t h);

/**
 * @brief Extract the sorted image from a json object returned by the server
 *
 * @details Extracts via looking at the "SORTED" key
 *
 * @param j A json object containing the image data
 *
 * @throws If a error occures, not unlikely, but handled withing. If a error occures it
 * will call invalid input message, kind of antipattern might change later
 *
 * @returns A rawImage object
 * */
rawImage RawImageFromJson(const json &j);

/**
 * @brief Generate a random Integer between the bounds specified in param
 *
 * @param lower Lower bound (inclusive, i think)
 *
 * @param upper Upper bound (also inclusive, i reckon)
 *
 * @throws Never, only when something goes very wrong
 *
 * @returns The Random Integer
 *
 */
int randomInt(int lower, int upper);

/**
 * @brief Execute a command in the terminal
 *
 * @param command The command to be executed
 *
 * @throws If the creation of a pipe fails or if the command failed
 *
 * @returns The output of the command
 */
std::string executeCommand(const std::string &command);

/**
 * @brief Exit the programm with a stacktrace and a Error String
 *
 * @param details A string specifying the details of the Crash
 *
 * @param location A std::source_location specifying the calling function, defaults to
 * source_location::current()
 *
 * @throws No
 *
 * @returns Nothing
 */
void InvalidInputMessage(const std::string &details,
                         std::source_location location = std::source_location::current());

/**
 * @brief Check a json object for all expected keys
 *
 * @param response The json Object to be checked
 *
 * @param expected A std::vector of std::string of the expected keys
 *
 * @throws If a expected value is not present or has not value assinged to it
 * (obj.is_null())
 *
 * @returns Nothing
 *
 */
void checkForCertainResponseType(const json &response, std::vector<std::string> expected);

/**
 * @brief Check if all implemented response keys are present in a json obj
 *
 * @note For all implemented response keys see /defs/implemented.h
 *
 * @param response The json object to be checked
 *
 * @throws If a implemented response key is not present
 *
 * @returns Nothing
 *
 */
void checkForAllResponseTypes(const json &response);
json parseNDJSON(const std::string &output);
void writeMemToImage(const rawImage &img);
void printReport(const std::vector<run> &exitMessage, StateClass &state);
#endif