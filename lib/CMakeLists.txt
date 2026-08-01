add_subdirectory(${CMAKE_SOURCE_DIR}/lib/builtin)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/local/src/cmake)

# `lily_sys` and the freestanding libc are only ever loaded by a native binary
# produced by `lilyc`, which the wasm build does not have.
if(NOT LILY_WASM)
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib/cc/std/src)
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib/sys)
endif()
