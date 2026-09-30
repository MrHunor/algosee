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
#include <exception>
#include <httplib/httplib.h>
#include <stdexcept>
#include <vector>

SortValidReturn validateSort(const httplib::Request &req, stateClass state) {
  SortValidReturn retval;

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

    json parsed = json::parse(req.body);

    if (!parsed.contains("values")) {
      throw std::runtime_error("Body does not contain a values parameter.");
    }

    if (!parsed["values"].is_array()) {
      throw std::runtime_error("Values is not an array.");
    }
    // you could check if all elements are integers but nlohman does that
    // internelly and throws itself so it is not really needed
    retval.values = parsed["values"].get<std::vector<int>>();

  } catch (const std::exception &e) {
    state.out("An Exception was thrown while validating the input:" +
                  std::string(e.what()),
              0);

    retval.algo = "";
    retval.values = {};
    retval.failureString =
        "An Exception was thrown while validating the input:" +
        std::string(e.what());
    retval.failure = true;
    return retval;
  }
  return retval;
}