/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */
#ifndef UTILS_H
#define UTILS_H
#include <filesystem>
#include <httplib.h>
#include <source_location>
#include <string>
int randomInt(int lower, int upper);
std::filesystem::path getExecutableDir();
std::string removeNewLineAndReturnCharacters(const std::string &inputString);
std::string executeCommand(const std::string &command);
void InvalidInputMessage(
    const std::string &details,
    std::source_location location = std::source_location::current());
std::string extractID(const std::string &filename);
std::string findFileByID(const std::string &dirPath, const std::string &id);
void restartSong();
std::string getRidOfESCCharactersinAstrics(const std::string &str);
std::string scaleImage(const std::string Inputimage,
                       const std::string outputImage, int height);
std::string SecToMinAndSec(int num);
void returnFailedAnswer(httplib::Response &res, const std::string &Details,
                        int exitCode = 400);
#endif