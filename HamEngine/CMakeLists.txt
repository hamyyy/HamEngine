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
add_subdirectory(lib/whereami)
add_subdirectory(lib/Mathter)
add_subdirectory(lib/simplefilewatcher)

# find open casecade
find_package(OpenCASCADE REQUIRED)

file(GLOB_RECURSE HAM_SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HAM_HEADERS "src/*.hpp" "src/*.h")

add_library(${PROJECT_NAME} STATIC ${HAM_SOURCES} ${HAM_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    lib/stduuid/include
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
    whereami
    Mathter
    simplefilewatcher
)

# foreach(LIB ${OpenCASCADE_LIBRARIES})
#     target_link_libraries(${PROJECT_NAME} debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
#     target_link_libraries(${PROJECT_NAME} optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
# endforeach()


add_compile_definitions(SOURCE_ASSETS_PATH_CORE="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

target_copy_assets(${PROJECT_NAME} assets)