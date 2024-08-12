cmake_minimum_required(VERSION 3.30.2)
include(FetchContent)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_GENERATOR "Ninja")

project("Escape The Mansion")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(BUILD_EXAMPLES OFF)
set(FLECS_SHARED OFF)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib
  GIT_TAG 5.0)

FetchContent_Declare(
  flecs
  GIT_REPOSITORY https://github.com/SanderMertens/flecs
  GIT_TAG v4.0.0)

FetchContent_Declare(
  libtmx
  GIT_REPOSITORY https://github.com/baylej/tmx
  GIT_TAG tmx_1.10.0)

FetchContent_Declare(
  arena
  GIT_REPOSITORY https://github.com/tsoding/arena
  GIT_TAG c2d1c1ce4360d13d5fb94201bfb72cc34a22fa69)

FetchContent_MakeAvailable(raylib flecs libtmx arena)

add_executable(
  escape-the-mansion
  ./src/input.c
  ./src/main.c
  ./src/physics.c
  ./src/render.c
  ./src/resources.c
  ./src/player.c
  ./src/arena.c)
target_include_directories(
  escape-the-mansion PUBLIC ./src "${CMAKE_CURRENT_LIST_DIR}/include"
                            ${arena_SOURCE_DIR})
target_link_libraries(escape-the-mansion PUBLIC flecs_static raylib tmx)
set_target_properties(escape-the-mansion PROPERTIES CMAKE_C_STANDARD 99)
set_target_properties(escape-the-mansion PROPERTIES CMAKE_C_STANDARD_REQUIRED
                                                    True)
set_target_properties(
  escape-the-mansion PROPERTIES CMAKE_C_FLAGS
                                "${CMAKE_C_FLAGS} -g -Wall -Wextra")

set_target_properties(escape-the-mansion PROPERTIES C_CPPCHECK "cppcheck")
set_target_properties(escape-the-mansion PROPERTIES C_CLANG_TIDY
                                                    "clang-tidy;--use-color")
