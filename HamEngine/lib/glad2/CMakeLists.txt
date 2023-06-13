cmake_minimum_required(VERSION 3.6)
project(glad2)

add_library(glad2-GL src/gl.c)
target_include_directories(glad2-GL PUBLIC include)

add_library(glad2-VK src/vulkan.c)
target_include_directories(glad2-VK PUBLIC include)