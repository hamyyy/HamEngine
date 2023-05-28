cmake_minimum_required(VERSION 3.6)
project(optick)

set(CMAKE_CXX_STANDARD 11)
file(GLOB_RECURSE OPTICK_SOURCES src/*.cpp)
file(GLOB_RECURSE OPTICK_HEADERS src/*.h)
add_library(optick STATIC ${OPTICK_SOURCES} ${OPTICK_HEADERS})
target_include_directories(optick PUBLIC include)
target_compile_definitions(optick PUBLIC -DOPTICK_ENABLE_GPU=0)