add_library(flecs STATIC ./flecs/flecs.c)
target_include_directories(flecs PUBLIC ./flecs)

add_library(cute_tiled STATIC ./cute_tiled.h)
target_compile_definitions(cute_tiled PUBLIC CUTE_TILED_IMPLEMENTATION)
set_target_properties(cute_tiled PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(./tmx)
