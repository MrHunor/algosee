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

/**
 * @file routing.h
 *
 * @brief Provides function defintions for API endpoints
 *
 */
#ifndef ROUTING_H
#define ROUTING_H
#include "../defs/types.h"
#include "../logger/logger.h"
#include "../stream/stream.h"
#include "../validate/image/validate.h"
#include "../validate/sort/validate.h"
#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>

/**
 * @brief Run the Status endpoint
 *
 * @details Fills response with a server online notice
 *
 * @param req A httplib::Request
 *
 * @param res A httplib::Response
 *
 * @param state The global logger object
 *
 * @throws Only when something goes very wrong
 *
 * @returns Nothing
 *
 */
void RunStatus(const httplib::Request &req, httplib::Response &res, StateClass &state);

/**
 * @brief Run the Sorting Algorithm Endpoint
 *
 * @details Streams the visulizasion Instructions on how to order the input array to a
 * ascending order and an array containing that sorted order
 *
 * @param info A SortValidReturn Object containing the nessasery info
 *
 * @param state The global logger object
 *
 * @param stream A StreamClass object used for this request
 *
 * @throws On Invalid info, but is handled within the function
 *
 * @returns Nothing
 *
 *
 */
void RunSortalgo(SortValidReturn info, StateClass &state, StreamClass &stream);

/**
 * @brief Run pathfinding algorithm endpoint
 *
 * @details Sets response to be the path from Start to Goal and some type of "visited" map
 * that shows which tiles it visited
 *
 * @param req A httplib::Request containing
 *
 * @param res A httplib::Response
 *
 * @param state The global logger object
 *
 *
 * @throws On Invalid input in req, but exception is handled inside the function
 *
 * @returns Nothing
 *
 */

void RunPathalgoMap(const httplib::Request &req, httplib::Response &res, StateClass &state);
/**
 * @brief Run The Image Sorting Endpoint
 *
 * @details Streams json chunks of the input image but randomised pixel positions,
 * instructions on how to viszulise the way to the original from the randomised image and
 * the sorted image (which should be the original)
 *
 * @param info A ImageValidReturn struct containing all of the needed information
 * @param state The Global Logger object
 * @param StreamClass The Request bound stream object
 *
 * @throws If any information inside info is invalid, but exception is handled within the
 * function
 *
 * @warning Construct info only via ValidateImage because this function itself will not
 * obey any boundrys
 *
 * @returns Nothing, will throw on error
 */

void RunImageSort(ImageValidReturn info, StateClass &state, StreamClass &stream);
/**
 * @brief Runs Preflight Handler
 *
 * @details Logs IP, prints last seens IPs and checks weather the request should be rate
 * limited
 *
 * @param req A httplib::Request
 *
 * @param res A httplib::response
 *
 * @param state The global logger object
 *
 * @param clientList A clientListType vector used for checking if rate limiting should
 * take place
 *
 * @param clientListMutex A std::mutex to temporarily lock acess to clientlist and prevent
 * a datarace
 *
 * @throws Never, unless someone manipulates the X-Forwarded-For header
 *
 * @returns httplib::Server::HandlerReponse, HandlerResponse::Handled in case of rate
 * limiting, HandlerResponse::Unhandled in case of not limiting
 */

httplib::Server::HandlerResponse RunPreflightHandler(const httplib::Request &req,
                                                     httplib::Response &res,
                                                     StateClass &state,
                                                     clientListType &clientList,
                                                     std::mutex &clientListMutex);

/**
 * @brief Run the Limits endpoint
 *
 * @details Sets response to the Limits
 *
 * @see ../defs/max_sizes.h for Limit defintions
 *
 * @param res A httplib::Response
 *
 * @param state The global logger object
 *
 * @throws Never, unless something goes really wrong
 *
 * @returns Nothing
 *
 *
 */
void RunLimits(httplib::Response &res, StateClass &state);

#endif