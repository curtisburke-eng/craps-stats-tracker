cmake_minimum_required(VERSION 3.22)

project(craps-stats-tracker VERSION 1.0 LANGUAGES CXX)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION APP_VERSION)
string(STRIP "${APP_VERSION}" APP_VERSION) # Removes \n, \r, leading/trailing whitespace

#only search for libs in vcpkg, not globally
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

find_package(spdlog REQUIRED)

add_subdirectory(lib/cst)
add_subdirectory(lib/cst-gui)

add_subdirectory(apps/craps-stats-tracker)
add_subdirectory(apps/cst-tests)
