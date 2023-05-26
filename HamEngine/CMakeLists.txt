cmake_minimum_required(VERSION 3.6)
project(HamEngine)

add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)

file(GLOB_RECURSE HAM_SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HAM_HEADERS "src/*.hpp" "src/*.h")

add_library(${PROJECT_NAME} STATIC ${HAM_SOURCES} ${HAM_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    src
)

target_link_libraries(${PROJECT_NAME}
    glad
    glfw
    glm
    imgui
)
