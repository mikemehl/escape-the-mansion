add_library(flecs STATIC ./flecs/flecs.c)
target_include_directories(flecs PUBLIC ./flecs)

add_library(cute_tiled STATIC ./cute_tiled.h)
target_compile_definitions(cute_tiled PUBLIC CUTE_TILED_IMPLEMENTATION)
set_target_properties(cute_tiled PROPERTIES LINKER_LANGUAGE C)

execute_process(
  COMMAND
    bash -c
    "cd ${CMAKE_CURRENT_SOURCE_DIR}/libtmj && cmake -DCMAKE_BUILD_TYPE=Debug -DLIBTMJ_ZSTD=ON -DLIBTMJ_ZLIB=ON -DBUILD_SHARED_LIBS=OFF . && make && cp ${CMAKE_CURRENT_SOURCE_DIR}/libtmj/lib/libtmj.a ${CMAKE_BINARY_DIR}/libtmj.a"
)
