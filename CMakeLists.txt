cmake_minimum_required(VERSION 3.6)
project(HamEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

include(Utils)

add_subdirectory(HamEngine)
add_subdirectory(HamGame)