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
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <chrono>
#include <format>
#include <source_location>
#include <stdexcept>
#include <string>
class excep : public std::runtime_error {
private:
  std::string formatMessage(const std::string &message,
                            std::source_location location) {

    std::string retval;
    auto now = std::chrono::system_clock::now();
    retval += std::format("@{}->Function name:{}\nFile "
                          "name:{}\nLine:{}\nCollum:{}:Threw an exception:{}",
                          now, location.function_name(), location.file_name(),
                          location.line(), location.column(), message);
    return retval;
  }

public:
  excep(const std::string &message,
        std::source_location location = std::source_location::current(),int SetexitCode = 400)
      : std::runtime_error(formatMessage(message,location)) {};
};
#endif