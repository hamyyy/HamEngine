cmake_minimum_required(VERSION 3.6)
project(HamEngine)

set(CMAKE_CXX_STANDARD 20)
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

include(Utils)

add_compile_definitions(HAM_DEBUG_ENABLED)

add_subdirectory(HamEngine)
add_subdirectory(HamGame)