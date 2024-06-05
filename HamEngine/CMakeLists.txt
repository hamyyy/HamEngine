cmake_minimum_required(VERSION 3.6)
project(HamEngine)

# add_subdirectory(lib/glad)
add_subdirectory(lib/glad2)
add_subdirectory(lib/glfw)
add_subdirectory(lib/imgui)
add_subdirectory(lib/ImGuizmo)
add_subdirectory(lib/fmt)
add_subdirectory(lib/spdlog)
# add_subdirectory(lib/optick)
add_subdirectory(lib/entt)
add_subdirectory(lib/stduuid)
add_subdirectory(lib/whereami)
add_subdirectory(lib/Mathter)
add_subdirectory(lib/simplefilewatcher)
add_subdirectory(lib/lua-cmake)
add_subdirectory(lib/sol2)
# add_subdirectory(lib/tracy)
file(GLOB TRACY_SOURCES "lib/tracy/public/TracyClient.cpp")

file(GLOB_RECURSE HAM_SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HAM_HEADERS "src/*.hpp" "src/*.h")

add_library(${PROJECT_NAME} STATIC ${HAM_SOURCES} ${HAM_HEADERS} ${TRACY_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    include
    lib/stduuid/include
    lib/tracy/public
)

target_link_libraries(${PROJECT_NAME}
    # glad
    glad2-GL
    # glad2-VK
    glfw
    imgui
    ImGuizmo
    fmt
    spdlog
    # optick
    EnTT
    stduuid
    whereami
    Mathter
    simplefilewatcher
    lua::lib
    sol2
)

# foreach(LIB ${OpenCASCADE_LIBRARIES})
#     target_link_libraries(${PROJECT_NAME} debug ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
#     target_link_libraries(${PROJECT_NAME} optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
# endforeach()


add_compile_definitions(
    SOURCE_ASSETS_PATH_CORE="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
    TRACY_ENABLE
)

target_copy_assets(${PROJECT_NAME} assets)