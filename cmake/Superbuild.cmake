# Superbuild driver.
#
# Included by the root `CMakeLists.txt` when it is configured directly, this
# spawns one `ExternalProject` per toolchain. Each of them re-configures the
# repository root with `LILY_INNER_BUILD=ON`, which is what actually defines the
# targets:
#
# <build>/native   the host compiler   lily, lilyc, ci, cic, the tests
# <build>/wasm     emcc                cic_wasm.js
#
# A single `cmake --build <build>` therefore builds both at once.
include(ExternalProject)

option(LILY_BUILD_WASM "Also build the wasm targets, with Emscripten" OFF)

# Options meaningful to every inner build, whatever its toolchain.
set(LILY_INNER_CMAKE_ARGS
    -DLILY_INNER_BUILD=ON
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}
    -DLILY_DEBUG=${LILY_DEBUG}
    -DLILY_DEBUG_MEM=${LILY_DEBUG_MEM}
    -DLILY_FULL_DEBUG=${LILY_FULL_DEBUG})

# Build the same source tree in `${dir}`, under the toolchain described by the
# remaining arguments, which are passed on to `ExternalProject_Add`.
function(add_lily_build name dir)
  ExternalProject_Add(
    ${name}
    SOURCE_DIR ${CMAKE_SOURCE_DIR}
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${dir}
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${dir}-prefix
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    # The driver cannot know what the inner build considers out of date, so it
    # always hands the decision over to the inner build system.
    BUILD_ALWAYS ON
    USES_TERMINAL_CONFIGURE ON
    USES_TERMINAL_BUILD ON
    ${ARGN})
endfunction()

# The compilers the driver itself was configured with are the native build's, so
# `CC=clang cmake ..` on the driver still selects clang inside. The wasm build
# gets its own from the Emscripten toolchain file instead.
add_lily_build(
  lily_native
  native
  CMAKE_ARGS
  ${LILY_INNER_CMAKE_ARGS}
  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
  -DLILY_BUILD_LLVM=${LILY_BUILD_LLVM}
  -DLILY_LINK_DYNAMIC=${LILY_LINK_DYNAMIC}
  -DLLVM_ENABLE_PROJECTS=${LLVM_ENABLE_PROJECTS}
  -DLILY_COMPILE_LIB_CC_STD=${LILY_COMPILE_LIB_CC_STD}
  # `find_package` runs in the inner build, so the package directories a caller
  # names have to reach it.
  -DLLVM_DIR=${LLVM_DIR}
  -DLLD_DIR=${LLD_DIR}
  -DClang_DIR=${Clang_DIR})

if(LILY_BUILD_WASM)
  # The toolchain file ships next to `emcc`, wherever emsdk was installed.
  find_program(LILY_EMCC NAMES emcc REQUIRED)
  get_filename_component(LILY_EMSCRIPTEN_DIR ${LILY_EMCC} DIRECTORY)
  set(LILY_EMSCRIPTEN_TOOLCHAIN
      ${LILY_EMSCRIPTEN_DIR}/cmake/Modules/Platform/Emscripten.cmake)

  if(NOT EXISTS ${LILY_EMSCRIPTEN_TOOLCHAIN})
    message(
      FATAL_ERROR
        "found `emcc` at ${LILY_EMCC}, but not its CMake toolchain file at ${LILY_EMSCRIPTEN_TOOLCHAIN}"
    )
  endif()

  message(STATUS "Build wasm with ${LILY_EMCC}")

  # emcc caches the sysroot libraries it builds, and it links nothing until it
  # can lock that cache. A system-wide emsdk (`/usr/lib/emsdk`, say) is not
  # writable, so fall back to a cache of our own inside the build directory.
  if(DEFINED ENV{EM_CACHE})
    set(LILY_EM_CACHE $ENV{EM_CACHE})
  else()
    set(LILY_EM_CACHE ${LILY_EMSCRIPTEN_DIR}/cache)

    execute_process(
      COMMAND ${CMAKE_COMMAND} -E touch ${LILY_EM_CACHE}/.lily_write_test
      RESULT_VARIABLE LILY_EM_CACHE_WRITABLE
      ERROR_QUIET)

    if(LILY_EM_CACHE_WRITABLE EQUAL 0)
      file(REMOVE ${LILY_EM_CACHE}/.lily_write_test)
    else()
      set(LILY_EM_CACHE ${CMAKE_CURRENT_BINARY_DIR}/wasm-cache)

      message(STATUS "Read-only emcc cache, using ${LILY_EM_CACHE} instead")
    endif()
  endif()

  # `EM_CACHE` is read from the environment, so both steps of the wasm build
  # have to run under it.
  set(LILY_EM_ENV ${CMAKE_COMMAND} -E env EM_CACHE=${LILY_EM_CACHE})

  add_lily_build(
    lily_wasm
    wasm
    CONFIGURE_COMMAND
    ${LILY_EM_ENV}
    ${CMAKE_COMMAND}
    -G${CMAKE_GENERATOR}
    -DCMAKE_TOOLCHAIN_FILE=${LILY_EMSCRIPTEN_TOOLCHAIN}
    ${LILY_INNER_CMAKE_ARGS}
    -S${CMAKE_SOURCE_DIR}
    -B${CMAKE_CURRENT_BINARY_DIR}/wasm
    BUILD_COMMAND
    ${LILY_EM_ENV}
    ${CMAKE_COMMAND}
    --build
    ${CMAKE_CURRENT_BINARY_DIR}/wasm)
endif()

# `compile_commands.json` is only useful for the native build; point at it from
# the top of the build directory so tooling configured for `<build>` finds it.
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  file(CREATE_LINK ${CMAKE_CURRENT_BINARY_DIR}/native/compile_commands.json
       ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json SYMBOLIC)
endif()
