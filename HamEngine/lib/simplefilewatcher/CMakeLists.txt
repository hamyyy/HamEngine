cmake_minimum_required(VERSION 3.6)
project(simplefilewatcher)

file(GLOB SIMPLE_FILE_WATCHER_SOURCES
    "simplefilewatcher/source/*.cpp"
)

add_library(simplefilewatcher STATIC ${SIMPLE_FILE_WATCHER_SOURCES})

target_include_directories(simplefilewatcher PUBLIC "simplefilewatcher/include")