cmake_minimum_required(VERSION 3.20)
project(
  lily
  VERSION 0.12.0
  HOMEPAGE_URL "https://github.com/thelilylang/lily"
  LANGUAGES C CXX)

include(${CMAKE_SOURCE_DIR}/cmake/LLVM.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Lily.cmake)

if(LILY_DEBUG)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Debug)
  add_compile_options(-DLILY_BIN_DEBUG_PATH_AVAILABLE)
else()
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
  add_compile_options(-DLILY_BIN_PATH_AVAILABLE)
endif()

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

if(LILY_DEBUG_MEM)
  add_link_options(-fsanitize=address)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "/wd4710 /wd4711 /wd4255")
endif()

if(APPLE)
  set(LILY_APPLE 1)
endif()

if(UNIX AND NOT APPLE)
  set(LILY_LINUX 1)
endif()

if(BSD)
  set(LILY_BSD 1)
endif()

if(LILY_DEBUG)
  enable_testing()
  list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

if(LILY_BSD)
  set(LILY_THREAD_LIB pthread)
else()
  set(LILY_THREAD_LIB)
endif()

if(LILY_DEBUG)
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
