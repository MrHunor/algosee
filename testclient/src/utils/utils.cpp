/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
#include "utils.h"
#include "../../src/defs/implemented.h"
#include "../../src/defs/types.h"
#include "../../src/utils/general/utils.h"
#include "../../src/logger/logger.h"
#include <cstdint>
#include <stacktrace>
#include <cstdlib>
#include <limits.h>
#include <nlohmann/json.hpp>
#include <random>
#include <sstream>
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using json = nlohmann::json;

GraphUnweighted generateGU(int size, int start, int goal)
{
    GraphUnweighted retval(size);
    for (int i = 0; i < size - 1;
         i++) // this guarantees that there is always a path from start to goal so that
              // Node 1: Connection to Node 2, Node 2: Connection to Node 3, Node 3:
              // Connection to Node 4 and so on
    {
        retval[i].push_back(i + 1);
    }
    for (int i = 0; i < size;
         i++) // this jsut serves as adding random data to confuse the pathfinding
              // algorithm, sort of give it a challange
    {
        for (int z = 0; z < randomInt(0, 10); z++)
        {
            retval[i].push_back(randomInt(0, size - 1));
        }
    }
    return retval;
}

GraphWeighted generateGW(int size, int start, int goal, int weightLowerBound)
{
    GraphWeighted retval(size);
    for (int i = 0; i < size - 1;
         i++) // this guarantees that there is always a path from start to goal so that
              // Node 1: Connection to Node 2, Node 2: Connection to Node 3, Node 3:
              // Connection to Node 4 and so on
    {
        retval[i].push_back({i + 1,1});
    }
    for (int i = 0; i < size;
         i++) // this jsut serves as adding random data to confuse the pathfinding
              // algorithm, sort of give it a challange
    {
        for (int z = 0; z < randomInt(0, 10); z++)
        {
            retval[i].push_back({randomInt(0, size - 1),randomInt(weightLowerBound,std::numeric_limits<uint8_t>::max())});//1 becuase astar rejects 0 
        }
    }
    return retval;
}



void writeMemToImage(const rawImage &img)
{
    if (stbi_write_png("testimage.png", img.width, img.height, img.channels, img.data,
                       img.width * img.channels) != 1)
        throw std::runtime_error("Failed to write testimage.png");
}

rawImage RawImageFromJson(const json &j)
{

    rawImage retval;

    retval.width = -1; // not extractable from info
    retval.height = -1;

    retval.size = j["SORTED"].size() * retval.channels;
    retval.data = new unsigned char[retval.size];
    int retvalI = 0;
    int JI = 0;
    while (JI < j["SORTED"].size())
    {
        const auto entry = j["SORTED"].at(JI);
        retval.data[retvalI] = entry.at(
            1); // ignore the first value, its a index because SORTED is a indexPixel
        retval.data[retvalI + 1] = entry.at(2);
        retval.data[retvalI + 2] = entry.at(3);
        retval.data[retvalI + 3] = entry.at(4);

        retvalI = retvalI + 4;
        JI++;
    }
    return retval;
}

unsigned char *makeRandomImage(size_t w, size_t h)
{
    size_t size = w * h * 4;
    unsigned char *retval =
        new unsigned char[size]; // since stb expects a char* pointer you have to define
                                 // it as such, loosing all of the cpp perks of std::
                                 // namespace but yeah, also dont create a unsinged char
                                 // var because that would go out of scope as soon as the
                                 // function return and return a invalid pointer, you
                                 // could of course return it as an object and then point
                                 // a pointer to it but not what im going for here
    for (int i = 0; i < size; i++)
    {
        retval[i] = randomInt(0, 255);
    }
    return retval;
}


void printReport(const std::vector<run> &exitMessage, StateClass &state)
{
    state.out("Succededed for:", 0);
    for (const auto &element : exitMessage)
    {
        if (element.failure == false)
            state.out("Algorithm:" + element.algoname + "\nAlgoType:" + element.algotype +
                          "\nSize:" + std::to_string(element.size) +
                          "\nMap Size:" + std::to_string(element.mapSize.first) + "," +
                          std::to_string(element.mapSize.second),
                      0);
    }

    state.out("Failed for:", 0);
    for (const auto &element : exitMessage)
    {
        if (element.failure == true)
            state.out("Algorithm:" + element.algoname + "\nAlgoType:" + element.algotype +
                          "\nSize:" + std::to_string(element.size) +
                          "\nMap Size:" + std::to_string(element.mapSize.first) + "," +
                          std::to_string(element.mapSize.second) +
                          "\nReason:" + element.failureReason,
                      0);
    }
}

json parseNDJSON(const std::string &output) // ai made this shiiiiii
{
    json result = json::object();
    std::unordered_map<std::string, bool> arrayFields;

    std::istringstream stream(output);
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty())
            continue;

        json event = json::parse(line);

        for (auto it = event.begin(); it != event.end(); ++it)
        {
            const std::string &key = it.key();
            const json &value = it.value();

            if (value.is_array())
            {
                if (!arrayFields[key])
                {
                    result[key] = json::array();
                    arrayFields[key] = true;
                }

                for (const auto &item : value)
                    result[key].push_back(item);
            }
            else
            {
                result[key] = value;
            }
        }
    }

    return result;
}
void checkForCertainResponseType(const json &response, std::vector<std::string> expected)
{
    for (const auto &element : expected)
    {
        if (!response.contains(element) || response[element].is_null())

            if (element != "SPLIT (ORIGIN,LEFT,RIGHT)") // see checkForAllResponseTypes
            {
                throw std::runtime_error("Response is missing the value:" + element +
                                         " or has no value assinged to it.");
            }
    }
}
void checkForAllResponseTypes(const json &response)
{
    for (const auto &element : implementedArrayReturnTypes)
    {
        if (!response.contains(element) || !response[element].is_array())
            if (element !=
                "SPLIT (ORIGIN,LEFT,RIGHT)") // split is a exception as it can possibly be
                                             // null, and belive me its easier and nicer
                                             // to have it as an exception here then to
                                             // fix it in the backend code
            {
                throw std::runtime_error("Response does not contain" + element +
                                         " or is not an array.");
            }
    }
    for (const auto &element : implementedNonArrayReturnTypes)
    {
        if (!response.contains(element) || response[element].is_null())

            throw std::runtime_error("Response does not contain " + element +
                                     " or is null");
    }
}

int randomInt(int lower, int upper)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(lower, upper);

    return dist(gen);
}

std::string executeCommand(const std::string &command)
{
    int exitCode;
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
        throw std::runtime_error("Failed to open Pipe for executing command:" + command);

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
        throw std::runtime_error("Shell Command:'" + command + "' has failed.");
    return result;
}
