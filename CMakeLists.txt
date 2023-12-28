cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SDL2Test)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

add_subdirectory(cryptopp-cmake)
# Added to get the GUI going
# Required libraries for linking against nngui (all targets)
# set(NNGUI_EXTRA_LIBS "")
# # Platform-dependent files for nngui
# set(NNGUI_EXTRA_SOURCE "")
# find_package(SDL2IMAGE REQUIRED)
# find_package(SDL2TTF REQUIRED)
# include_directories(${SDL2_INCLUDE_DIR})
# include_directories(${SDL2TTF_INCLUDE_DIR})
# include_directories(${SDL2_IMAGE_INCLUDE_DIR})


add_executable(SnakeGame src/main.cpp src/game.cpp src/controller.cpp src/renderer.cpp src/snake.cpp src/obstacle.cpp src/leaderboard.cpp src/scenario.h)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(SnakeGame ${SDL2_LIBRARIES} cryptopp)
