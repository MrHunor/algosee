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
 * @file logger.h
 *
 * @brief Header containing the StateClass defintion
 *
 */
#ifndef LOGGER_H
#define LOGGER_H
#include "../defs/general.h"
#include "colours.h"
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <source_location>
#include <string>

/**
 * @brief Class used for printing to console and writing that output to a logfile
 *
 * @note For constructor details see in class definition
 *
 */
class StateClass
{
  private:
    std::ofstream logFile;
    std::mutex outMutex;

  public:
    int verbose;
    /**
     * @brief Print something to console and write it to log.txt
     *
     * @details Based on verbosity level set in this.verbose will print a timestamp and a
     * calling function location
     *
     * @param output A std::string containing the message to print
     *
     * @param importance A integer setting the importance of the message, the lower the
     * more important, specifies weather time and location is printed
     *
     * @param colour A std::string containing a ansi colour the output should be coloured
     * in in the terminal, defaults to DEFAULT_COLOUR
     *
     * @param location A std::source_location specifiying which function is calling this
     * functionl, defaults to source_location::current()
     *
     * @note For Valid colours have a look at colours.h
     *
     * @throws Never
     *
     * @returns Nothing
     */

    StateClass(std::string logFilePath)
    {
        logFile.open(logFilePath);
    }
    void out(const std::string &output, int importance,
             const std::string &colour = DEFAULT_COLOUR,
             std::source_location location = std::source_location::current())
    {
        if (importance > verbose)
            return;
        std::string message = colour;
        if (verbose >= VERBOSE_LEVEL_NEEDED_FOR_TIME)
        {
            auto now = std::chrono::system_clock::now();
            message += std::format("@{} -> ", now);
        }
        if (verbose >= 0)
            message += std::format("{}->", location.function_name());
        message += output;
        message += RESET;
        std::lock_guard<std::mutex> lock(outMutex);
        std::cout << message;
        std::cout << std::endl;
        if (logFile.is_open())
            logFile << message << std::endl;
    }
};

#endif