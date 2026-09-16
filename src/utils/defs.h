/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */
#ifndef DEFS_H
#define DEFS_H
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <source_location>

#define VERSION 0.4

#define DEFAULT_CAPTURE_TIME 5
#define DEFAULT_SAMPLE_RATE 44100
#define HDH 720
#define FHDH 1080
#define WQHDH 1440
#define QHDPLUSH 1800
#define UHDH 2160

// Colors
//  Reset
#define RESET "\033[0m"

// Regular colors
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Bright colors
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"

// Background colors
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

// Bright background colors
#define BG_BRIGHT_BLACK "\033[100m"
#define BG_BRIGHT_RED "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_BLUE "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"

// Text styles
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"
#define STRIKE "\033[9m"

#define DEFAULT_COLOUR MAGENTA

#define VERBOSE_LEVEL_NEEDED_FOR_TIME 0

class stateClass {
public:
  std::ofstream logFile{"log.txt"};
  int verbose;
  bool deleteOverflow;
  int CaptureTime = DEFAULT_CAPTURE_TIME; // in seconds
  int resX;
  int resY;
  int resYRequested = HDH;
  void out(const std::string &output, int importance,
           const std::string &colour = DEFAULT_COLOUR,
           std::source_location location = std::source_location::current()) {
    if (importance > verbose)
      return;
    std::string message = colour;
    if (verbose >= VERBOSE_LEVEL_NEEDED_FOR_TIME) {
      auto now = std::chrono::system_clock::now();
      message += std::format("@{} -> ", now);
    }
    if (verbose >= 4)
      message += std::format("{}->", location.function_name());
    message += output;
    message += RESET;
    std::cout << message;
    std::cout << std::endl;
    if (logFile.is_open())
      logFile << message << std::endl;
  }
};

#endif