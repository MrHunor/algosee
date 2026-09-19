/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */
#ifndef SERVER_H
#define SERVER_H
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;

void afterSelfTestRun(std::string algorithmName,
                      std::vector<int> &paramPassValues,
                      std::vector<int> &testvalues,
                      std::vector<int> &sortedValues,
                      std::vector<std::pair<int, int>> &moves,
                      std::vector<std::vector<int>> &tries, json &response,
                      json &parampassTest,
                      const std::chrono::steady_clock::time_point &startTime);

#endif