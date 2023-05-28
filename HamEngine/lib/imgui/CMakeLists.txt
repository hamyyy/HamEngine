cmake_minimum_required(VERSION 3.6)
project(imgui)

set(CMAKE_CXX_STANDARD 11)

file(GLOB IMGUI_SOURCES
    imgui/*.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC imgui)

target_link_libraries(imgui glfw)