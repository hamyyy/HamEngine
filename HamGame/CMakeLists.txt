cmake_minimum_required(VERSION 3.6)
project(HamGame)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} HamEngine)
add_compile_definitions(SOURCE_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

target_copy_assets(${PROJECT_NAME} assets)
target_kill_executable(${PROJECT_NAME})