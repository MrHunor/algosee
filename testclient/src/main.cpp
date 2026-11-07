/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
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
#include "../../src/logger/logger.h"
#include "CLI/CLI.hpp"
#include "tests/tests.h"
#include "utils/utils.h"
#include <httplib.h>
#include <string>

/**
 * @brief Main entry point
 *
 * @param argc Integer of arguments passed, usually at least one, which is the name of the
 * executable
 *
 * @param argv Pointer to a array of char containing the arguments which are passed
 *
 * @throws When bad input from the user is passed, but is handled internally. (Unless ofc
 * something goes very wrong)
 *
 * @returns Exitcode
 *
 */
int main(int argc, char *argv[])
{
    remove("testClientLog.txt");
    StateClass state("testClientLog.txt");
    bool sortalgo=false;
    bool pathalgo=false;
    bool image=false;
    CLI::App algosee_testclient{
        "Algosee_testclient: Runs tests on all implemented algorithms of all types on "
        "the algosee server\n© 2026 MrHunor, siryanni, vilmos45 (as "
        "equals) · GPLv3"};
    algosee_testclient.add_flag(
        "-v,--verbose", state.verbose,
        "Set the verbosity of the server output, 0 being nothing and 4 being "
        "the most detailed. (Stack as -vvvv)");
 
    algosee_testclient.add_flag("-s, --sortalgo",sortalgo,"Run tests on all sorting algorithms.");
    algosee_testclient.add_flag("-p,--pathalgo",pathalgo, "Run tests on all pathfinding algorithms.");
    algosee_testclient.add_flag("-i,--image",image,"Run tests on all image sorting algorithms.");
   

    algosee_testclient.callback([&](){
    if(!sortalgo&&!pathalgo&&!image)
    {
        InvalidInputMessage("No tests specified.");
    }
    std::vector<run> exitMessage;
    httplib::Client client("0.0.0.0", 8080);

    if(sortalgo)runSortalgoTest(exitMessage, client, state);
    if(pathalgo)runPathalgoTest(exitMessage, client, state);
    if(image)runImageTest(exitMessage, state);

    state.out("Finished", 0);

    printReport(exitMessage, state);
    for (const auto &element : exitMessage)
    {
        if (element.failure == true)
            return -1;
    }

    return 0;
    });
}