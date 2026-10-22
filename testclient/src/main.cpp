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
#include "tests/tests.h"
#include "../../src/logger/logger.h"
#include "utils/utils.h"
#include <string>
#include <utility>



int main(int argc, char *argv[])
{
    remove("testclientlog.txt");
    stateClass state;
    state.out("Testing sorting algorithms", 0);

    std::vector<run> exitMessage;

    runSortalgoTest(exitMessage, state);
    runPathalgoTest(exitMessage, state);

    state.out("Finished", 0);
    state.out("Succededed for:", 0);
    for (const auto &element : exitMessage)
    {
        if (element.failure == false)
            state.out("Algorithm:" + element.algoname +
                          ", Size:" + std::to_string(element.size) +
                          "Map Size:" + std::to_string(element.mapSize.first) + "," +
                          std::to_string(element.mapSize.second),
                      0);
    }

    state.out("Failed for:", 0);
    for (const auto &element : exitMessage)
    {
        if (element.failure == true)
            state.out("Algorithm:" + element.algoname +
                          ", Size:" + std::to_string(element.size) +
                          "Map Size:" + std::to_string(element.mapSize.first) + "," +
                          std::to_string(element.mapSize.second),
                      0);
    }

    for (const auto &element : exitMessage)
    {
        if (element.failure == true)
            return -1;
    }
    return 0;
}