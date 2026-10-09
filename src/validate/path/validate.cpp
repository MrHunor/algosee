/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#include "validate.h"
#include "../../config.h"
#include "../../logger/logger.h"
#include "../../utils/pathalgo/pathalgo.h"
#include <httplib/httplib.h>

PathValidReturn validatePath(const httplib::Request &req, stateClass &state) {
  PathValidReturn retval;
  state.out("Checking algo URL parameter...", 0);
    if (!req.has_param("algo")) {
      throw excep("URL does not contain a url parameter.");
    }
    retval.algo = req.get_param_value("algo");
    if (retval.algo.empty()) {
      throw excep(
          "URL contains a algo parameter but no algo is set.");
    }
    if (!implementedPathAlgos.contains(retval.algo)) {
      throw excep(
          "Provided algo does not exist or is not implemented.");
    }

    state.out("Parsing body...", 0);
    json parsed = json::parse(req.body);

    state.out("Checking MAP, START and GOAL parameters existance...", 0);
    if (!parsed.contains("MAP")) {
      throw excep("Body does not contain a MAP parameter.");
    }
    if (!parsed.contains("START")) {
      throw excep("Body does not contain a START parameter.");
    }
    if (!parsed.contains("GOAL")) {
      throw excep("Body does not contain a GOAL parameter.");
    }

    state.out("Casting Map (if needed)...", 0);
    if (retval.algo == "dijkstra") {
      retval.mapInt = parsed["MAP"];
      if (retval.mapInt.empty()) {
        throw excep("MAP does not contain any values.");
      }
    } else {
      retval.mapBool = castIntMapToBoolIfNeeded(parsed);
      if (retval.mapBool.empty()) {
        throw excep(
            "MAP does not contain any values or failed to parse.");
      }
    }

    // this will throw itself it is empty
    retval.start = parsed["START"];
    retval.goal = parsed["GOAL"];

    state.out("Checking Maps internal validness...", 0);
    if (retval.algo != "dijkstra")
      checkMapValidness(retval.mapBool, retval.start, retval.goal);
    else
      checkMapValidness(retval.mapInt, retval.start, retval.goal);

    auto it = implementedPathAlgos.find(retval.algo);
    if (retval.algo == "dijkstra" &&
        retval.mapInt.size() * retval.mapInt[0].size() > it->second)
      throw excep("Too many element specified. " + it->first +
                               " only accepts less then " +
                               std::to_string(it->second) + " elements.");

    else if (retval.algo != "dijkstra"&&retval.mapBool.size() * retval.mapBool[0].size() > it->second)
      throw excep("Too many element specified. " + it->first +
                               " only accepts less then " +
                               std::to_string(it->second) + " elements.");
  return retval;
}