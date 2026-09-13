cmake_minimum_required(VERSION 3.20)

project(algosee VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB UTI_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp")

file(GLOB SAL_FILES CONFIGURE_DEPENDS"${CMAKE_CURRENT_SOURCE_DIR}/src/sortalgo/*.cpp")

set(ALL_SOURCES
${SRC_FILES}
${UTI_FILES}
${SAL_FILES}
)

add_executable(algosee
${ALL_SOURCES}
)

find_package(nlohmann_json REQUIRED)

find_package(Boost CONFIG REQUIRED)

find_package(httplib REQUIRED)

target_link_libraries(algosee 
nlohmann_json::nlohmann_json
Boost::boost
httplib::httplib
stdc++exp

)