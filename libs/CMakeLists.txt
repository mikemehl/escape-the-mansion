add_library(flecs STATIC ./flecs/flecs.c)
target_include_directories(flecs PUBLIC ./flecs)

add_subdirectory(./tmx)
