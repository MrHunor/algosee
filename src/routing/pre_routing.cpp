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
#include "../logger/logger.h"
#include "../utils/general/utils.h"
#include <chrono>
#include <httplib/httplib.h>
#include <mutex>
#include <nlohmann/json.hpp>

httplib::Server::HandlerResponse RunPreflightHandler(const httplib::Request &req, httplib::Response &res, 
                                                     stateClass &state, clientListType &clientList,
                                                     std::mutex &clientListMutex)
{

    std::string ip;
    if (req.has_header("X-Forwarded-For"))
    {
        std::string forwarded = req.get_header_value("X-Forwarded-For");
        auto it = forwarded.rfind(',');
        if (it != std::string::npos)
        {
            forwarded.erase(forwarded.begin(), forwarded.begin() + it + 1);
        }
        ip = forwarded;
    }
    else
    {
        ip = req.remote_addr;
    }
    state.out("Recived request from IP:" + ip, 0);

    // prevent a data race
    std::lock_guard<std::mutex> lock(clientListMutex);
    AddClient(clientList, {std::chrono::system_clock::now(), ip});
    state.out("Added to Clientlist:", 0);
    std::string output;
    for (const auto &element : clientList)
    {
        output = "";
        output = std::format("Clientlist Element: Time:{}; IP:{}", element.first,
                             element.second);
        state.out(output, 0);
    }

    if (CheckRateLimit(clientList, clientList.size() - 1))
    {
        ReturnFailedAnswer(res, "You are being rate limited. Try again later.", 429);
        return httplib::Server::HandlerResponse::Handled;
    }

  
    // continue to normal handling
    return httplib::Server::HandlerResponse::Unhandled;
}