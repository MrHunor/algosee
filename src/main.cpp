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

    state.out(
        "algosee @ https://github.com/MrHunor/algosee\n"
        "GNU General Public License v3 (GPLv3) © 2026 MrHunor,siryanni "
        "(as equals)\n"
        "This Program is provided \"AS IS\" without warranty of any kind.",
        0, RED
    );

    state.out("Starting...", 0);

    httplib::Server server;

    server.Post("/sortalgo",
        [&](const httplib::Request& req, httplib::Response& res) {

            std::cout << ">>> REQUEST RECEIVED" << std::endl;
            std::cout << "Target: " << req.target << std::endl;
            std::cout << "Body: " << req.body << std::endl;

            auto parsed = json::parse(req.body);
            std::vector<int> values = parsed["values"];

            std::vector<std::pair<int, int>> moves;
            moves.clear();
            if (!req.has_param("algo")) {
                res.status = 400;
                res.set_content(
                    R"({"error":"Missing algo parameter"})",
                    "application/json"
                );
                return;
            }

            std::string algo = req.get_param_value("algo");

            std::cout << "Algorithm: " << algo << std::endl;

            if (algo == "selection") {
                selectionSort(values, moves);

                res.set_content(
                    json(moves).dump(),
                    "application/json"
                );
                return;
            }

            if (algo == "bogo") {
                std::cout << "Recognised bogo" << std::endl;

                std::vector<std::vector<int>> tries;
                tries.clear();
                bogoSort(values, tries);

                std::cout << "Returning "
                          << tries.size()
                          << " tries"
                          << std::endl;

                res.set_content(
                    json(tries).dump(),
                    "application/json"
                );
                return;
            }

            res.status = 400;
            res.set_content(
                R"({"error":"Unknown algorithm"})",
                "application/json"
            );
        }
    );

    std::cout << "Listening on 127.0.0.1:8080..." << std::endl;

    if (!server.listen("127.0.0.1", 8080)) {
        std::cerr << "ERROR: Could not listen on 127.0.0.1:8080"
                  << std::endl;
        return 1;
    }

    return 0;
}
