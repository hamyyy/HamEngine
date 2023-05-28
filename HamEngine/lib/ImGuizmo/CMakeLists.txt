cmake_minimum_required(VERSION 3.6)
project(ImGuizmo)

set(CMAKE_CXX_STANDARD 11)

file(GLOB IMGUIZMO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/*.cpp # TODO: only add the cpp files that are needed
    ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/*.h
)

add_library(ImGuizmo STATIC ${IMGUIZMO_SOURCES})
target_include_directories(ImGuizmo PUBLIC ImGuizmo/)
target_link_libraries(ImGuizmo PUBLIC imgui)
