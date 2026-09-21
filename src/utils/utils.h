/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */
#ifndef UTILS_H
#define UTILS_H
#include <httplib.h>
#include <source_location>
#include <string>
int randomInt(int lower, int upper);
std::string executeCommand(const std::string &command);
void InvalidInputMessage(
    const std::string &details,
    std::source_location location = std::source_location::current());
void returnFailedAnswer(httplib::Response &res, const std::string &Details,
                        int exitCode = 400);
#endif