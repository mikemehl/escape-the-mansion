cmake_minimum_required(VERSION 3.30.2)

project("Escape The Mansion")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(ASSETS_DIR ./assets)

add_subdirectory(libs)
add_executable(escape-the-mansion ./src/input.c ./src/main.c ./src/physics.c
                                  ./src/render.c ./src/resources.c)
target_include_directories(escape-the-mansion PUBLIC ./src ./libs/flecs ./libs)
target_link_libraries(escape-the-mansion PUBLIC flecs cute_tiled -lraylib)
