/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano
 * Zapata.
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
#include "../exception/exception.h"
#include "../logger/logger.h"
#include "../pathalgo/algos.h"
#include "../utils/general/utils.h"
#include "../utils/pathalgo/pathalgo.h"
#include "../validate/path/validate.h"
#include <chrono>
#include <exception>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

void RunPathAlgoGraph(const httplib::Request &req, httplib::Response &res,
                      StateClass &state)
{
    try
    {
        std::vector<int> path;
        json response;
        generateJsonRetrunArras(response);
        fillWithMetaData(response);

        state.out("Processing values.....",0);
        
    }
    catch (const excep &e)
    {
        state.out("Cought a excep, Details:" + std::string(e.what()) + "\n" +
                      "Status Code:" + std::to_string(e.exitCode),
                  0);
        ReturnFailedAnswer(res, std::string(e.what()), e.exitCode);
    }
    catch (const std::exception &e)
    {
        state.out("Cought an exception:" + std::string(e.what()), 0);
        ReturnFailedAnswer(res, std::string(e.what()));
    }
}
