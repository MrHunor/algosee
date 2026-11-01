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
#include "../../src/defs/types.h"
#include "../../src/logger/logger.h"
#include "../utils/utils.h"
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>

void runImageTest(std::vector<run> &exitMessage, StateClass &state)
{
    for (const auto &element : testCasesImage)
    {
        run currentrun;
        currentrun.algoname = element.first;
        currentrun.mapSize = {element.second.first, element.second.second};
        state.out("Testing image sorting validness with algo" + currentrun.algoname +
                      " and dimesions:" + std::to_string(element.second.first) + "x" +
                      std::to_string(element.second.second),
                  0);
        rawImage img;
        rawImage returnImg;
        try
        {

            img.width = currentrun.mapSize.first;
            img.height = currentrun.mapSize.second;
            img.channels = 4;
            img.size = img.width * img.height * img.channels;
            state.out("Making img in memory", 0);
            img.data = makeRandomImage(img.width, img.height);
            state.out("Writing to disk...", 0);
            remove("testimage.png");
            writeMemToImage(img);
            std::string command = "curl -N --fail-with-body -F 'image=@./testimage.png' "
                                  "'http://127.0.0.1:8080/image?algo='" +
                                  currentrun.algoname;
            std::string output = executeCommand(command);
            state.out("Recived", 0);
            json response = parseNDJSON(output);
            checkForAllResponseTypes(response);
            checkForCertainResponseType(
                response, implementedAlgoExpectedResponseType.at(element.first));
            returnImg = RawImageFromJson(response);

            if(img.size!=returnImg.size)throw std::runtime_error("Data size mismatch:\nSend Size:"+std::to_string(img.size)+"\nRecived Size"+std::to_string(returnImg.size));
            for (int i = 0; i < img.size; i++)
            {
                if (img.data[i] != returnImg.data[i])
                {
                    throw std::runtime_error(
                        "At iteration:" + std::to_string(i) + "\nOriginal Image:" +
                        std::to_string(img.data[i]) + "\nSorted Answer from server:" +
                        std::to_string(returnImg.data[i]) + "\n MISMATCH.");
                }
            }
            delete[] img.data;
            delete[] returnImg.data;
            state.out("Sucess.",0);
            exitMessage.push_back(currentrun);
        }
        catch (const std::exception &e)
        {
            delete[] img.data;
            delete[] returnImg.data;
            state.out("Failed. Details:" + std::string(e.what()), 0);
            currentrun.failure = true;
            exitMessage.push_back(currentrun);
        }
    }
}