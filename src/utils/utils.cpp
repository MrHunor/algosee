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
#include "utils.h"
#include <chrono>
#include <cstdlib>
#include <httplib.h>
#include <iostream>
#include <limits.h>
#include <nlohmann/json.hpp>
#include <ostream>
#include <random>
#include <source_location>
#include <stacktrace>
#include <string>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
namespace fs = std::filesystem;
using json = nlohmann::json;

int randomInt(int lower, int upper) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(lower, upper);

  return dist(gen);
}

std::string executeCommand(const std::string &command) {
  int exitCode;
  FILE *pipe = popen(command.c_str(), "r");
  if (!pipe)
    InvalidInputMessage("Failed to open Pipe for executing command:" + command);

  char buffer[256];
  std::string result;

  while (fgets(buffer, sizeof(buffer),
               pipe)) // this works because fgets returns a pointer to the data,
                      // and if no data is available it return nullptr
  {
    result += buffer;
  }

  // i know the reference exit code is kinda ugly but the other option would be
  // to return an pair or struct which would require a lot of work and im lazy
  int status = pclose(pipe);
  exitCode = -1;
  if (WIFEXITED(status)) {
    exitCode = WEXITSTATUS(status);
  }
  if (exitCode != 0)
    InvalidInputMessage("Shell Command:'" + command + "' has failed.");
  return result;
}

void returnFailedAnswer(httplib::Response &res, const std::string &Details,
                        int exitCode) {

  std::cout << "returnFailedAnswer called:" + Details;
  res.status = exitCode;
  res.set_header("Access-Control-Allow-Origin", "*");

  res.set_content(json(Details), "application/json");
}

void InvalidInputMessage(const std::string &details,
                         std::source_location location) {
  // yes i thought of making this state out for colour but that would require
  // passing state (which is not always present), or make state global which is
  // not pretty
  auto now = std::chrono::system_clock::now();
  std::cout << "TIME:" << now << std::endl;
  std::cout << std::stacktrace::current() << std::endl;
  std::cout << "Filename:" << location.file_name() << std::endl;
  std::cout << "Function:" << location.function_name() << std::endl;
  std::cout << "Line:" << location.line() << std::endl;
  std::cout << "Column:" << location.column() << std::endl;
  if (!details.empty()) {
    std::cout << "Details provided:" << details << std::endl;
  }
  exit(1);
}
