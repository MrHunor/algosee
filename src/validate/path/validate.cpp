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
#include <stdexcept>

PathValidReturn validatePath(const httplib::Request &req, stateClass state) {
  PathValidReturn retval;
  try {
    if (!req.has_param("algo")) {
      throw std::runtime_error("URL does not contain a url parameter.");
    }
    retval.algo = req.get_param_value("algo");
    if (retval.algo.empty()) {
      throw std::runtime_error(
          "URL contains a algo parameter but no algo is set.");
    }
    if (!implementedSortAlgos.contains(retval.algo)) {
      throw std::runtime_error(
          "Provided algo does not exist or is not implemented.");
    }

    json parsed = json(req.body);

    if (!parsed.contains("MAP")) {
      throw std::runtime_error("Body does not contain a MAP parameter.");
    }
    if (!parsed.contains("START")) {
      throw std::runtime_error("Body does not contain a START parameter.");
    }
    if (!parsed.contains("GOAL")) {
      throw std::runtime_error("Body does not contain a GOAL parameter.");
    }

    if (retval.algo == "dijkstra") {
      retval.mapInt = parsed["MAP"];
      if (retval.mapInt.empty()) {
        throw std::runtime_error("MAP does not contain any values.");
      }
    } else {
      retval.mapBool = castIntMapToBoolIfNeeded(parsed);
      if (retval.mapBool.empty()) {
        throw std::runtime_error("MAP does not contain any values.");
      }
    }

    //this will throw itself it is empty
    retval.start = parsed["START"];
    retval.goal = parsed["GOAL"];

  } catch (const std::exception &e) {
    state.out("An Exception was thrown while validating the input:" +
                  std::string(e.what()),
              0);

    retval.algo = "";
    retval.start = {};
    retval.goal = {};
    retval.mapBool = {};
    retval.mapInt = {};

    retval.failureString =
        "An Exception was thrown while validating the input:" +
        std::string(e.what());
    retval.failure = true;
    return retval;
  }
}