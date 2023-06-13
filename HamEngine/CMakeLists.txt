cmake_minimum_required(VERSION 3.6)
project(HamEngine)

# add_subdirectory(lib/glad)
add_subdirectory(lib/glad2)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)
add_subdirectory(lib/ImGuizmo)
add_subdirectory(lib/fmt)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/optick)
add_subdirectory(lib/entt)
add_subdirectory(lib/stduuid)
# add_subdirectory(lib/filewatch) # dont need to add subdirectory, just need to include the header file

file(GLOB_RECURSE HAM_SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HAM_HEADERS "src/*.hpp" "src/*.h")

add_library(${PROJECT_NAME} STATIC ${HAM_SOURCES} ${HAM_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    src
    lib/stduuid/include
    lib/filewatch
)

target_link_libraries(${PROJECT_NAME}
    # glad
    glad2-GL
    # glad2-VK
    glfw
    glm
    imgui
    ImGuizmo
    fmt
    spdlog
    optick
    EnTT
    stduuid
)