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
#include "../../src/defs/general.h"
#include "../../src/defs/testcases.h"
#include "../../src/logger/logger.h"
#include "../../src/utils/general/utils.h"
#include "../utils/utils.h"
#include <algorithm>
#include <httplib.h>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

void runSortalgoTest(std::vector<run> &exitMessage, httplib::Client &client,
                     StateClass &state)
{
    for (const auto &testcase : testCasesSorting)
    {

        run currentrun;
        currentrun.failure = false;
        currentrun.algoname = testcase.first;
        currentrun.size = testcase.second;
        currentrun.mapSize = {-1, -1};

        state.out("Testing Sorting validness with: " + currentrun.algoname +
                      " size:" + std::to_string(currentrun.size),
                  0);
        try
        {

            std::vector<int> testValues(currentrun.size);
            std::iota(testValues.begin(), testValues.end(), 1);
            std::sort(testValues.begin(), testValues.end());
            std::vector<int> sorted = testValues;
            std::vector<int> sendValues = testValues;
            std::shuffle(sendValues.begin(), sendValues.end(), getRandomGen());

            json body = {
                {"values",sendValues}
            };

            auto res = client.Post("/sortalgo?algo="+currentrun.algoname,body.dump(),"application/json");
            if(!res)throw std::runtime_error("Request failed");
            if(res->status!=200)throw std::runtime_error("Server returned non 200 exit code;\nExitCode:"+std::to_string(res->status)+"\nErrorMessage:"+res->body);
            

            state.out("Recived.", 0);
            json response = parseNDJSON(res->body);
            state.out("Parsed", 0);
            checkForAllResponseTypes(response);
            checkForCertainResponseType(
                response, implementedAlgoExpectedResponseType.at(currentrun.algoname));
            std::vector<int> sortedByAlgosee = response["SORTED"];

            if (sortedByAlgosee != sorted)
            {

                currentrun.failure = true;
            }
            if (currentrun.failure)
                state.out("Failed.", 0);
            else
                state.out("Sucess.", 0);
            exitMessage.push_back(currentrun);
        }
        catch (const std::exception &e)
        {
            state.out("Failed. Details:" + std::string(e.what()), 0);
            currentrun.failure = true;
            exitMessage.push_back(currentrun);
        }
    }
}