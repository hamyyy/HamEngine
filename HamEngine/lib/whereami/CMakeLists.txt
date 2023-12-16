cmake_minimum_required(VERSION 3.15)
project(whereami)


file(GLOB WHEREAMI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/*.h
)

message(STATUS "whereami sources: ${WHEREAMI_SOURCES}")

add_library(${PROJECT_NAME} STATIC ${WHEREAMI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "whereami/src")
