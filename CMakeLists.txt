cmake_minimum_required(VERSION 3.20)
project(
  lily
  VERSION 0.12.0
  HOMEPAGE_URL "https://github.com/thelilylang/lily"
  LANGUAGES C CXX)

# This file is configured twice, in two different roles.
#
# Configured directly, it is only a superbuild driver: it builds nothing and
# instead spawns one `ExternalProject` per toolchain (native, and wasm when
# `LILY_BUILD_WASM` is on). Each of those re-configures this same file with
# `LILY_INNER_BUILD` on, which is the real project below.
#
# The inner builds keep the repository root as their `CMAKE_SOURCE_DIR`, so
# every `${CMAKE_SOURCE_DIR}/...` path in the tree stays valid; only the binary
# directory differs (`<build>/native`, `<build>/wasm`).
if(NOT LILY_INNER_BUILD)
  include(${CMAKE_SOURCE_DIR}/cmake/Superbuild.cmake)

  return()
endif()

# The Emscripten toolchain file sets `EMSCRIPTEN`, so the wasm inner build turns
# `LILY_WASM` on by itself. What is gated on it below is what the wasm target
# cannot, or does not need to, build: LLVM, and with it the `lily`/`lilyc`
# chain, plus the test suite.
if(EMSCRIPTEN)
  set(LILY_WASM ON)
endif()

if(NOT LILY_WASM)
  include(${CMAKE_SOURCE_DIR}/cmake/LLVM.cmake)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/Lily.cmake)

if(LILY_DEBUG)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Debug)
  add_compile_options(-DLILY_BIN_DEBUG_PATH_AVAILABLE)
else()
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
  add_compile_options(-DLILY_BIN_PATH_AVAILABLE)
endif()

# Keep `cic_wasm.js` and its `.wasm` out of the native binaries' way.
if(LILY_WASM)
  set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/wasm)
endif()

# Avoid to get `__FILE__`, with an absolute path.
add_compile_options(-ffile-prefix-map=${CMAKE_SOURCE_DIR}=./)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LILY_VERSION_MAJOR 0)
set(LILY_VERSION_MINOR 12)
set(LILY_VERSION_PATCH 0)
set(LILY_VERSION
    "${LILY_VERSION_MAJOR}.${LILY_VERSION_MINOR}.${LILY_VERSION_PATCH}")

message(STATUS "Build lily v${LILY_VERSION}")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    FATAL_ERROR
      "expected -DCMAKE_BUILD_TYPE=Debug|Release|RelWithDebInfo|MinSizeRel")
endif()

if(LILY_FULL_DEBUG)
  set(LILY_DEBUG 1)
  set(LILY_DEBUG_MEM 1)
endif()

if(NOT WIN32)
  # NOTE: On WSL you must to disable `-fdiagnostics-color=always`
  add_compile_options(-Wall -fdiagnostics-color=always)

  if(LILY_BUILD_LLVM)
    add_compile_options(-fno-rtti)
  endif()
else()
  add_compile_options(-Wall -fno-rtti)
endif()

# NOTE: Emscripten wants the sanitizer at compile time too, and with its own
# flags, so a `DEBUG_MEM=1 BUILD_WASM=1` build sanitizes the native half only.
if(LILY_DEBUG_MEM AND NOT LILY_WASM)
  add_link_options(-fsanitize=address)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "/wd4710 /wd4711 /wd4255")
endif()

if(APPLE)
  set(LILY_APPLE 1)
endif()

if(UNIX
   AND NOT APPLE
   AND NOT LILY_WASM)
  set(LILY_LINUX 1)
endif()

if(BSD)
  set(LILY_BSD 1)
endif()

# Emscripten builds a shared library as a side module, which the final link
# would then have to load at runtime. Everything is static there.
if(LILY_WASM)
  set(LILY_LIB_TYPE STATIC)
else()
  set(LILY_LIB_TYPE SHARED)
endif()

if(LILY_DEBUG AND NOT LILY_WASM)
  enable_testing()
  list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if(NOT LILY_WASM)
  add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif()

if(LILY_BSD)
  set(LILY_THREAD_LIB pthread)
else()
  set(LILY_THREAD_LIB)
endif()

if(LILY_DEBUG AND NOT LILY_WASM)
  function(move_tests_dir)
    file(COPY tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    message("Move `tests` directory in build directory")
  endfunction()

  if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/tests)
    file(REMOVE tests)
    move_tests_dir()
  else()
    move_tests_dir()
  endif()
endif()
