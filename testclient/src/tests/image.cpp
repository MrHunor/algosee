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
#include <string>
#include <utility>

void runImageTest(std::vector<run> &exitMessage, stateClass &state)
{
    for (const auto &element : testCasesImage)
    {
        run currentrun;
        currentrun.algoname=element.first;
        currentrun.mapSize={element.second.first,element.second.second};
        rawImage img;
        img.width = currentrun.mapSize.first;
        img.height = currentrun.mapSize.second;
        img.channels = 4;
        state.out("Making img in memory",0);
        img.data = makeRandomImage(img.width, img.height);
        state.out("Writing to disk...",0);
        writeMemToImage(img);
        std::string command = "curl -N --fail-with-body -F 'image=@./testimage.png' "
                      "'http://127.0.0.1:8080/image?algo='" +
                      currentrun.algoname;
        std::string output = executeCommand(command);
        state.out("Recived",0);
        json response = parseNDJSON(output);
        indexPixel returnarr =IPFromJson(response);
        checkForAllResponseTypes(response);
        checkForCertainResponseType(response, implementedAlgoExpectedResponseType.at(element.first));

        for(int i = 0; i<img.width*img.height*img.channels;i++)
        {
        
            if(img.data[i]!=response["SORTED"])
        }
    }
}