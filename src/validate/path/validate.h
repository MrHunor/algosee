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
#ifndef PATHVALIDATE_H
#define PATHVALIDATE_H
#include "../../config.h"
#include "../../logger/logger.h"
#include <httplib/httplib.h>

struct PathValidReturn {
  std::string algo;

  std::vector<std::vector<int>> mapInt;
  std::vector<std::vector<bool>> mapBool;

  std::pair<int, int> start;
  std::pair<int, int> goal;

  bool failure = false;
  std::string failureString;
};

PathValidReturn validatePath(const httplib::Request &req, stateClass& state);
#endif