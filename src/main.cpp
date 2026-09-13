/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include "utils/defs.h"
#include "utils/utils.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "sortalgo/algos.h"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
  stateClass state;
  state.out("algosee @ https://github.com/MrHunor/algosee\nGNU General Public License v3 (GPLv3) © 2026 MrHunor,siryanni (as equals)\nThis Program is provided \"AS IS\" without warranty of any kind.",0,RED);
  state.out("Starting...", 0);

  httplib::Server server;
  server.Post(
      "/sortalgo", [&](const httplib::Request &req, httplib::Response &res) {
        std::vector<int> values = json::parse(req.body)["values"];
        std::vector<std::pair<int,int>> moves;
        if (req.has_param("algo")) {
          std::string algo = req.get_param_value("algo");
          if (algo == "selection") selectionSort(values, moves);
          else InvalidInputMessage("Unknow algorithm");
        } else InvalidInputMessage("Request does not contain a algo parameter");

          res.set_content(json(moves).dump(),"application/json");
      });

      state.out("listining for incoming requests on localhost:8080",0);
  //localhost
  server.listen("127.0.0.1",8080);

  return 0;
}