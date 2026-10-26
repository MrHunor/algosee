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
 * @file exception.h
 *
 * @brief Header containing the excep class defintion
 *
 *
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <chrono>
#include <format>
#include <source_location>
#include <stdexcept>
#include <string>

/**
 * @brief A custom exception class that inherits from std::runtime_error
 *
 * @details Adds source_location and exitCode functionality to the basic function of
 * std::runtime_error
 *
 * @note For constructor details look inside function defintion
 *
 */
class excep : public std::runtime_error
{
  public:
    unsigned short exitCode;

  private:
    /**
     * @brief Adds location info to message
     *
     * @param message A std::string contains the basic error message
     *
     * @param location A std::source_location of where the exception was called
     *
     * @returns The newly constructed std::string
     *
     */
    std::string formatMessage(const std::string &message, std::source_location location)
    {

        std::string retval;
        auto now = std::chrono::system_clock::now();
        retval += std::format("@{}->Function name:{}\nFile "
                              "name:{}\nLine:{}\nCollum:{}:Threw an exception:{}",
                              now, location.function_name(), location.file_name(),
                              location.line(), location.column(), message);
        return retval;
    }

  public:
    /**
     * @brief excep Class contructor
     *
     * @details Inherits from std::runtime_error
     *
     * @param message A std::string containig the error message
     *
     * @param exitCode_ A unsigned short that contains a exitCode, intended to be a http
     * error code, defaults to 400
     *
     * @param location A std::source location with the default of
     * source::location::current() describing which function called the exception
     *
     * @returns excep object
     *
     */
    excep(const std::string &message, unsigned short exitCode_ = 400,
          std::source_location location = std::source_location::current())
        : std::runtime_error(formatMessage(message, location))
    {
        exitCode = exitCode_;
    };
};
#endif