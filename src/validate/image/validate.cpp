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
#include "../../exception/exception.h"
#include "../../defs/implemented.h"
#include "../../logger/logger.h"
#include "../../utils/image/image.h"
#include <exception>
#include <httplib/httplib.h>
#include <stdexcept>
#include <vector>
ImageValidReturn validateImage(const httplib::Request &req, stateClass &state) {
  ImageValidReturn retval;
 
    retval.img = loadImage(req, state);

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
    if(retval.img.width*retval.img.height>MAX_PIXEL_COUNT_IMAGE){
      throw excep("Too many pixels provided. Max:"+std::to_string(MAX_PIXEL_COUNT_IMAGE));
    }
 
  return retval;
}