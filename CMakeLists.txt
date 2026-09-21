 #   algosee; a algorithm visulizer
 #   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
 #   along with this program.(root/LICENSE)  If not, see <https://www.gnu.org/licenses/>.
 
cmake_minimum_required(VERSION 3.20)

project(algosee VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/#.cpp")

file(GLOB UTI_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/#.cpp")

file(GLOB SAL_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/sortalgo/#.cpp")

file(GLOB SVR_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/server/#.cpp")

file(GLOB PAL_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/pathalgo/#.cpp")

set(ALL_SOURCES
${SRC_FILES}
${UTI_FILES}
${SAL_FILES}
${SVR_FILES}
${PAL_FILES}
)

add_executable(algosee
${ALL_SOURCES}
)

find_package(nlohmann_json REQUIRED)


find_package(httplib REQUIRED)


target_link_libraries(algosee 
nlohmann_json::nlohmann_json
httplib::httplib
stdc++exp

)