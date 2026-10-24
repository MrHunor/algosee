/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano
 * Zapata.
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
#include "../../defs/general.h"
#include "../../defs/implemented.h"
#include "../../defs/ratelimits.h"
#include "../../defs/types.h"
#include "../../stream/stream.h"
#include "build_info.h"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <httplib.h>
#include <iostream>
#include <limits.h>
#include <nlohmann/json.hpp>
#include <ostream>
#include <random>
#include <source_location>
#include <stacktrace>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

std::vector<char> randomStringDataset{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void fillWithMetaData(json &response)
{
    response["STATUS"] = "ONLINE";
    response["VERSION"] = ALGOSEE_VERSION;
    response["BUILDTIME"] = ALGOSEE_BUILD_TIME;
}

std::string readFile(std::string path)
{
    std::ifstream file{path};
    if (!file)
        throw std::runtime_error("Failed to open " + path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void generateJsonRetrunArras(json &response)
{
    for (const auto &element : implementedArrayReturnTypes)
    {
        response[element] = json::array();
    }
    for (const auto &element : implementedNonArrayReturnTypes)
    {
        response[element] = -1;
    }
}

std::mt19937 getRandomGen()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen;
}

int RandomInt(int lower, int upper)
{
    std::uniform_int_distribution<int> dist(lower, upper);
    auto gen = getRandomGen();
    return dist(gen);
}

std::string GenerateRandomString(const int &size)
{
    std::string retval;
    for (int i = 0; i < size; i++)
    {
        retval += randomStringDataset[RandomInt(0, randomStringDataset.size() - 1)];
    }
    return retval;
}

bool CheckRateLimit(const clientListType &cList, const int &index)
{
    std::string targetIP = cList[index].second;
    int matchingEntrysCount = 0;
    for (const auto &element : cList)
    {
        if (element.second == targetIP &&
            element.first > std::chrono::system_clock::now() -
                                std::chrono::minutes(RATE_LIMIT_INTERVALL_MIN))
        {
            matchingEntrysCount++;
        }
    }
    if (matchingEntrysCount >= RATE_LIMIT_COUNT)
    {
        return true;
    }
    return false;
}

void AddClient(clientListType &cList,
               const std::pair<std::chrono::time_point<std::chrono::system_clock>,
                               std::string> &newEntry)
{
    int index = 0;
    auto now = std::chrono::system_clock::now();

    if (!cList.empty())
    {
        for (const auto &element : cList)
        {
            if (now > element.first + std::chrono::hours(1))
                index++; // current viewing entry older then one hour, move counter up
        }
    }
    for (int i = 0; i < index; i++)
    {
        cList.erase(cList.begin()); // delete all entrys older then an hour
    }
    cList.push_back(newEntry);
}

std::string ExecuteCommand(const std::string &command)
{
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
    if (WIFEXITED(status))
    {
        exitCode = WEXITSTATUS(status);
    }
    if (exitCode != 0)
        InvalidInputMessage("Shell Command:'" + command + "' has failed.");
    return result;
}

void ReturnFailedAnswer(httplib::Response &res, const std::string &Details, int exitCode,
                        StreamClass *stream)
{

    std::cout << "returnFailedAnswer called:" + Details
              << "; Code:" + std::to_string(exitCode) << std::endl;
    res.status = exitCode;
    res.set_header("Access-Control-Allow-Origin", "*");

    json out;
    out["ERROR"] = Details;
    out["ERRORCODE"] = exitCode;
    if (stream == nullptr)
        res.set_content(out, "application/json");
    else
    { // stream instead of sending, i think the sending once solution could work on a
      // streaming implemented endpoint too but ill just leave it like this, also you have
      // to clean up the stream with stream->finish
        stream->sendChunk(out);
        stream->finish();
    }
}

void ReturnFailedAnswerStreamOnly(const std::string &Details, int exitCode,
                                  StreamClass *stream)
{
    std::cout << "returnFailedAnswerStream called:" + Details
              << "; Code:" + std::to_string(exitCode) << std::endl;

    json out;
    out["ERROR"] = Details;
    out["ERRORCODE"] = exitCode;
    stream->sendChunk(json(out));
    stream->finish();
}

void InvalidInputMessage(const std::string &details, std::source_location location)
{

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
    if (!details.empty())
    {
        std::cout << "Details provided:" << details << std::endl;
    }
    exit(1);
}
