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
#ifndef ROUTING_H
#define ROUTING_H
#include "../logger/logger.h"
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

void afterSelfTestRun(std::string algorithmName,
                      std::vector<int> &paramPassValues,
                      std::vector<int> &testvalues,
                      std::vector<int> &sortedValues, json &response,
                      json &parampassTest,
                      const std::chrono::steady_clock::time_point &startTime);

void RunStatus(const httplib::Request &req, httplib::Response &res,
               stateClass &state);

void runSortalgo(const httplib::Request &req, httplib::Response &res,
                 stateClass &state);

void RunPathalgo(const httplib::Request &req, httplib::Response &res,
                 stateClass &state);
void runImageSort(const httplib::Request &req, httplib::Response &res,
                  stateClass &state);
#endif