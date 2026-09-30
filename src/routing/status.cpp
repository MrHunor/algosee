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
#include "../logger/logger.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>


void RunStatus(const httplib::Request &req, httplib::Response &res,
               stateClass &state) {
  json response;
  response["STATUS"] = "ONLINE";
  response["VERION"] = "VERSION";
  if (req.has_header("X-Forwarded-For")) {
    state.out("Status request from:" + req.get_header_value("X-Forwarded-For"),
              0);
  } else {
    state.out(req.remote_addr, 0);
  }
  state.out("Send status signal.", 0);
  res.status = 200;
  res.set_header("Access-Control-Allow-Origin", "*");
  res.set_content(response.dump(), "application/json");
}
