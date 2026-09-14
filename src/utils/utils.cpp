/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */
#include "utils.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <httplib.h>
#include <iostream>
#include <limits.h>
#include <nlohmann/json.hpp>
#include <portaudio.h>
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

std::filesystem::path getExecutableDir() {
  char buffer[PATH_MAX];

  ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

  if (len == -1)
    InvalidInputMessage("Failed to get executable directory.");

  buffer[len] = '\0';

  return std::filesystem::path(buffer).parent_path();
}

std::string removeNewLineAndReturnCharacters(const std::string &inputString) {
  std::string retval = inputString;
  while (!retval.empty() && (retval.back() == '\n' || retval.back() == '\r')) {
    retval.pop_back(); // delete last character
  }
  return retval;
}

std::string getRidOfESCCharactersinAstrics(const std::string &str) {
  std::string result = "'";

  for (char c : str) {
    if (c == '\'')
      result += "'\\''";
    else
      result += c;
  }

  result += "'";
  return result;
}

std::string scaleImage(const std::string Inputimage,
                       const std::string outputImage, int height) {
  return executeCommand(
      std::format("ffmpeg -i \"{}\" -vf \"scale=-1:{}\" \"{}\"", Inputimage,
                  height, outputImage));
}

std::string SecToMinAndSec(int num) {
  std::string retval = "";
  int min = 0;
  int sec = 0;
  while (num > 59) {
    num = num - 60;
    min++;
  }
  sec = num;
  retval += std::to_string(min) + ":";
  if (sec < 10)
    retval += "0";
  retval += std::to_string(sec);
  return retval;
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

std::string extractID(const std::string &filename) {
  size_t start = filename.rfind('[');
  size_t end = filename.rfind(']');

  if (start != std::string::npos && end != std::string::npos &&
      (end - start - 1) == 11) {
    return filename.substr(start + 1, 11);
  }
  return "";
}

void restartSong() {

  executeCommand("playerctl pause");
  executeCommand("playerctl position 0");

  // Wait until the seek has actually taken effect.
  while (true) {
    std::string output = executeCommand("playerctl position");

    double position = std::stod(output);

    if (position <= 0.01)
      break;

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  executeCommand("playerctl play");
}

std::string findFileByID(const std::string &dirPath, const std::string &id) {
  for (const auto &entry : fs::directory_iterator(dirPath)) {
    std::string filename = entry.path().string();
    if (filename.find(id) != std::string::npos) {
      return filename;
    }
  }
  return "";
}

void returnFailedAnswer(httplib::Response &res, const std::string &Details,
                        int exitCode) {
  std::cout << "returnFailedAnswer called:" + Details;
  res.status = exitCode;
  res.set_content(json(Details), "application/json");
}

void InvalidInputMessage(const std::string &details,
                         std::source_location location) {
  // yes i thought of making this state out for colour but that would require
  // passing state (which is not always present), or make state global which is
  // not pretty
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
