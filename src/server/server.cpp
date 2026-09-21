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
                      const std::chrono::steady_clock::time_point &startTime) {
  // end time measurement
  auto endTime = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
  // cleanup
  moves.clear();
  tries.clear();
  parampassTest.clear();

  // check if sort worked
  if (paramPassValues == sortedValues) {
    response[algorithmName + " STATUS"] = "SUCESS";
    response[algorithmName + " TIME"] = elapsed.count();
  } else {
    response[algorithmName + " STATUS"] = "FAILED";
    response[algorithmName + " TIME"] = elapsed.count();
  }

  // reset passvalues to testvalues
  paramPassValues = testvalues;
}
