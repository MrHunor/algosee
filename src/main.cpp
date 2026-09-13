/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

#include "utils/utils.h"
#include "utils/defs.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

using json = nlohmann::json;

int main(int argc, char *argv[])
{
stateClass state;
state.out("Starting...",0);

httplib::Server server;
server.Post("/sortalgo",[&](const httplib::Request& req, httplib::Response& res)
{

std::vector<int> values = json::parse(req.body)["values"];

if(req.has_param("algo"))
{
std::string algo = req.get_param_value("algo");
if(algo=="selection")   
}
else InvalidInputMessage("Request does not contain a algo parameter");


});

return 0;    
}