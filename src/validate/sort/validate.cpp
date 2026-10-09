/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
#include "validate.h"
#include "../../config.h"
#include "../../logger/logger.h"
#include <httplib/httplib.h>
#include <vector>

SortValidReturn validateSort(const httplib::Request &req, stateClass& state) {
  SortValidReturn retval;

    if (!req.has_param("algo")) {
      throw excep("URL does not contain a url parameter.");
    }
    retval.algo = req.get_param_value("algo");
    if (retval.algo.empty()) {
      throw excep(
          "URL contains a algo parameter but no algo is set.");
    }
    if (!implementedSortAlgos.contains(retval.algo)) {
      throw excep(
          "Provided algo does not exist or is not implemented.");
    }

    json parsed = json::parse(req.body);

    if (!parsed.contains("values")) {
      throw excep("Body does not contain a values parameter.");
    }

    if (!parsed["values"].is_array()) {
      throw excep("Values is not an array.");
    }
    // you could check if all elements are integers but nlohman does that
    // internelly and throws itself so it is not really needed
    retval.values = parsed["values"].get<std::vector<int>>();

    

    if(retval.algo == "counting")
    {
      bool hasNegNumbers = std::any_of(retval.values.begin(), retval.values.end(),
                                     [](int x) { // some weird callback shit
                                       return x < 0;
                                     });
      if(hasNegNumbers)throw excep("Selected algorithm (counting) does not support negative numbers.");
    }
    
    auto it = implementedSortAlgos.find(retval.algo);
    if(retval.values.size()>it->second)
    {
      throw excep("Too many element specified. "+it->first+" only accepts less then "+ std::to_string(it->second)+ " elements.");
    }


  return retval;
}