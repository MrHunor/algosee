/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano
 * Zapata.
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
#ifndef ALGOSEE_UTILS_H
#define ALGOSEE_UTILS_H
#include "../../defs/general.h"
#include "../../defs/types.h"
#include "../../stream/stream.h"
#include <httplib.h>
#include <random>
#include <source_location>
#include <string>
int RandomInt(int lower, int upper);
std::mt19937 getRandomGen();
std::string ExecuteCommand(const std::string &command);
void InvalidInputMessage(const std::string &details,
                         const std::source_location location = std::source_location::current());
void ReturnFailedAnswer(httplib::Response &res, const std::string &Details,
                        int exitCode = 400, StreamClass *stream = nullptr);
void ReturnFailedAnswerStreamOnly(const std::string &Details, int exitCode,
                                  StreamClass *stream);
void AddClient(clientListType &cList,
               const std::pair<std::chrono::time_point<std::chrono::system_clock>,
                               const std::string> &newEntry);
bool CheckRateLimit(const clientListType &cList, int index);
std::string GenerateRandomString(const int &size);
std::string readFile(const std::string& path);
std::mt19937 getRandomGen();
void generateJsonRetrunArras(json &response);
void fillWithMetaData(json &response);
#endif