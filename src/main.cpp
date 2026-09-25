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

#include "server/server.h"
#include "utils/defs.h"
#include "utils/utils.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>

int main(int argc, char *argv[]) {
  stateClass state;
  state.verbose = 0;
  state.out("algosee Copyright (C) 2026  MrHunor, siryanni (as equals)\n"
            "This program comes with ABSOLUTELY NO WARRANTY; for details "
            "visit:'https://www.gnu.org/licenses/gpl-3.0.en.html'.\n"
            "This is free software, and you are welcome to redistribute it\n"
            "under certain conditions; visit "
            "'https://www.gnu.org/licenses/gpl-3.0.en.html' for details.\n",
            0, RED);

  state.out("Starting...", 0);

  httplib::Server server;

  //--------------------------OPTIONS-----------------------------------------
  // handle a preflight request from a diffrent origin; firefox & co. considers
  // diffrent ports on the same computer to be diffrent origins so this fix is
  // needed to work
  const auto serverOptionHandler =
      [](const httplib::Request &req,
         httplib::Response
             &res) { // first time using callbacks like this kinda nervous >_<
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 200;
      };

  server.Options("(/sortalgo)", serverOptionHandler);
  server.Options("(/pathalgo)", serverOptionHandler);
  server.Options("(/status)", serverOptionHandler);
  server.Options("(/selftest)", serverOptionHandler);

  //---------------------------STATUS---------------------------------------------------------------------
  server.Get("/status",
             [&](const httplib::Request &req, httplib::Response &res) {
               RunStatus(req, res, state);
               return;
             });

  //------------------------------SELFTEST---------------------------------------------------------------

  server.Get("/selftest",
             [&](const httplib::Request &req, httplib::Response &res) {
               RunSelftest(req,res, state);
               return;
             });

  //------------------------------SORTALGO---------------------------------------------------------------
  server.Post("/sortalgo",
              [&](const httplib::Request &req, httplib::Response &res) {
                runSortalgo(req, res, state);
                return;
              });

  //------------------------------PATHALGO----------------------------------------------------------------
  server.Post("/pathalgo",
              [&](const httplib::Request &req, httplib::Response &res) {
                RunPathalgo(req, res, state);
                return;
              });

  // render port detection magic
  const char *port_env = std::getenv("PORT");
  int port = port_env ? std::stoi(port_env) : 8080;

  state.out("Listining on:0.0.0.0:" + std::to_string(port), 0);
  if (!server.listen("0.0.0.0", port)) {
    InvalidInputMessage("Failed to listen on 0.0.0.0:" + std::to_string(port));
  }

  return 0;
}
