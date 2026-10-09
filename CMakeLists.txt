#   algosee; A algorithm visulizer
#   Copyright (C) 2026  MrHunor, siryanni (as equals)
#   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
#   
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.(root/LICENSE)
#   If not, see <https://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE release CACHE STRING "Build type" FORCE)
endif()


project(algosee VERSION 0.8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE ALL_SOURCES_MAIN CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)


add_executable(algosee
    ${ALL_SOURCES_MAIN}
)

if(CMAKE_BUILD_TYPE STREQUAL "debug")

    file(GLOB_RECURSE ALL_SOURCES_TESTCLIENT CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/testclient/src/*.cpp"
    )

    add_executable(algosee_testclient
        ${ALL_SOURCES_TESTCLIENT}
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "release")
    target_compile_options(algosee PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Release>:-march=native>
    )
endif()

string(TIMESTAMP BUILD_TIME "%Y-%m-%dT%H:%M:%S")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/build_info.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/build_info.h"
    @ONLY
)


target_include_directories(algosee PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

find_package(nlohmann_json REQUIRED)


find_package(httplib REQUIRED)

find_package(stb REQUIRED)

target_link_libraries(algosee
    nlohmann_json::nlohmann_json
    httplib::httplib
    stb::stb
    stdc++exp
)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
target_link_libraries(algosee_testclient
nlohmann_json::nlohmann_json
stdc++exp
)
endif()