cmake_minimum_required(VERSION 3.8)
project(Mathter)


add_library(${PROJECT_NAME} INTERFACE)

message("${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

if (${MATHTER_USE_XSIMD})
	find_package(xsimd)
	target_link_libraries(${PROJECT_NAME} INTERFACE xsimd)
endif()