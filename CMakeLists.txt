cmake_minimum_required(VERSION 3.20)
project(
  lily
  VERSION 0.5.0
  HOMEPAGE_URL "https://github.com/thelilylang/lily"
  LANGUAGES C CXX)

include(${CMAKE_SOURCE_DIR}/cmake/Lily.cmake)

if(LILY_DEBUG)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Debug)
else()
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LILY_VERSION_MAJOR 0)
set(LILY_VERSION_MINOR 5)
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

if(LILY_BUILD_LLVM)
  message(
    WARNING
      "Make sure your machine is powerful enough to build LLVM. If not, you can install LLVM on your system and build without having to build LLVM (takes far fewer resources and is much faster)."
  )

  set(LLVM_DIR "${CMAKE_SOURCE_DIR}/lib/local/src/llvm-project/llvm")
  set(LLVM_VERSION "16.0.6")
  set(LLVM_ENABLE_PROJECTS "lld")

  # Add include directories
  include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/lib/local/src/llvm-project/llvm/include)
  include_directories(
    ${CMAKE_SOURCE_DIR}/lib/local/src/llvm-project/llvm/include)
  include_directories(
    ${CMAKE_SOURCE_DIR}/lib/local/src/llvm-project/lld/include)

  message(STATUS "Build LLVM v${LLVM_VERSION}")
else()
  # Require LLVM
  find_package(LLVM 16 CONFIG REQUIRED)
  set(LLVM_VERSION ${LLVM_PACKAGE_VERSION})

  message(STATUS "Found LLVM v${LLVM_VERSION}")

  # Require LLD
  find_package(LLD 16)
  set(LLD_VERSION ${LLVM_PACKAGE_VERSION})

  if(LILY_LINK_DYNAMIC)
    if(WIN32)
      message(
        WARNING
          "Make sure you have lldCOFF, lldCommon, lldELF, lldMachO, lldMinGW, lldWasm, installed on your OS"
      )
    else()
      find_library(
        LLD_COFF
        NAMES liblldCOFF.so
        PATHS ${LLVM_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
      find_library(
        LLD_COMMON
        NAMES liblldCommon.so
        PATHS ${LLVM_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
      find_library(
        LLD_ELF
        NAMES liblldELF.so
        PATHS ${LLVM_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
      find_library(
        LLD_MACHO
        NAMES liblldMachO.so
        PATHS ${LLVM_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
      find_library(
        LLD_MINGW
        NAMES liblldMinGW.so
        PATHS ${LLVM_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
      find_library(
        LLD_WASM
        NAMES liblldWasm.so
        PATHS ${LLVM_LIBRARY_DIRS}
        NO_DEFAULT_PATH)
    endif()
  else()
    # NOTE: It seems that in some cases it is only the dynamic lld libraries
    # that are available on the host machine.
    find_library(
      LLD_COFF
      NAMES lldCOFF.lib lldCOFF.a liblldCOFF.a liblldCOFF.so
      PATHS ${LLVM_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
    find_library(
      LLD_COMMON
      NAMES lldCommon.lib lldCommon.a liblldCommon.a liblldCommon.so
      PATHS ${LLVM_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
    find_library(
      LLD_ELF
      NAMES lldELF.lib lldELF.a liblldELF.a liblldELF.so
      PATHS ${LLVM_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
    find_library(
      LLD_MACHO
      NAMES lldMachO.lib lldMachO.a liblldMachO.a liblldMachO.so
      PATHS ${LLVM_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
    find_library(
      LLD_MINGW
      NAMES lldMinGW.lib lldMinGW.a liblldMinGW.a liblldMinGW.so
      PATHS ${LLVM_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
    find_library(
      LLD_WASM
      NAMES lldWasm.lib lldWasm.a liblldWasm.a liblldWasm.so
      PATHS ${LLVM_LIBRARY_DIRS}
      NO_DEFAULT_PATH)
  endif()

  message(STATUS "Found LLD v${LLD_VERSION}")

  # Require Clang
  find_package(Clang 16)
  set(CLANG_VERSION ${LLVM_PACKAGE_VERSION})

  message(STATUS "Found Clang v${CLANG_VERSION}")

  # Add include directories
  include_directories(${LLVM_INCLUDE_DIRS})
  include_directories(${LLD_INCLUDE_DIRS})
endif()

if(LILY_BUILD_LLVM)
  add_subdirectory(${LLVM_DIR})

  # Set LLVM libraries
  llvm_map_components_to_libnames(
    LILY_LLVM_LIBS
    amdgputargetmca
    AllTargetsAsmParsers
    AllTargetsCodeGens
    AllTargetsDescs
    AllTargetsDisassemblers
    AllTargetsInfos
    analysis
    asmprinter
    core
    target
    irprinter
    irreader
    bitreader
    bitstreamreader
    bitwriter
    transformutils
    passes
    dlltooldriver
    support
    binaryformat
    lto
    object
    windowsdriver
    windowsmanifest
    native
    debuginfopdb
    libdriver
    option
    mc
    mcparser)
else()
  # Set LLVM libraries
  if(LILY_APPLE)
    set(LILY_LLVM_LIBS LLVM xar)
  else()
    set(LILY_LLVM_LIBS LLVM)
  endif()
endif()

if(BSD)
  set(LILY_THREAD_LIB pthread)
else()
  set(LILY_THREAD_LIB)
endif()

# Set LLD libraries
if(LILY_BUILD_LLVM)
  set(LILY_LLD_LIBS lldCOFF lldCommon lldELF lldMachO lldMinGW lldWasm)
else()
  set(LILY_LLD_LIBS ${LLD_COFF} ${LLD_COMMON} ${LLD_ELF} ${LLD_MACHO}
                    ${LLD_MINGW} ${LLD_WASM})
endif()

find_library(ZLIB NAMES libz.a libz.so zlib libz z)
find_library(ZSTD NAMES libzstd.a libzstdstatic.a zstd NAMES_PER_DIR)

list(APPEND LILY_LLVM_LIBS "${ZLIB};${ZSTD}")

add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/cc/ci)

# lily_libyaml
set(LIBYAML_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/api.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/dumper.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/emitter.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/loader.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/parser.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/reader.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/scanner.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/writer.c)

add_library(lily_libyaml STATIC ${LIBYAML_SRC})
target_include_directories(lily_libyaml PRIVATE ${LILY_INCLUDE}
                                                lib/local/src/libyaml/src)

# lily_base
set(BASE_SRC
    ${CMAKE_SOURCE_DIR}/src/base/allocator.c
    ${CMAKE_SOURCE_DIR}/src/base/alloc.c
    ${CMAKE_SOURCE_DIR}/src/base/arc.c
    ${CMAKE_SOURCE_DIR}/src/base/atof.c
    ${CMAKE_SOURCE_DIR}/src/base/atoi.c
    ${CMAKE_SOURCE_DIR}/src/base/binary_heap.c
    ${CMAKE_SOURCE_DIR}/src/base/binary_search.c
    ${CMAKE_SOURCE_DIR}/src/base/bitmap.c
    ${CMAKE_SOURCE_DIR}/src/base/buffer.c
    ${CMAKE_SOURCE_DIR}/src/base/char.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/args.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/command.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/default_action.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/diagnostic.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/help.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/option.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/value.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/result/option.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/result/value.c
    ${CMAKE_SOURCE_DIR}/src/base/cli/result.c
    ${CMAKE_SOURCE_DIR}/src/base/cli.c
    ${CMAKE_SOURCE_DIR}/src/base/color.c
    ${CMAKE_SOURCE_DIR}/src/base/command.c
    ${CMAKE_SOURCE_DIR}/src/base/dir.c
    ${CMAKE_SOURCE_DIR}/src/base/env.c
    ${CMAKE_SOURCE_DIR}/src/base/error.c
    ${CMAKE_SOURCE_DIR}/src/base/file.c
    ${CMAKE_SOURCE_DIR}/src/base/format.c
    ${CMAKE_SOURCE_DIR}/src/base/fork.c
    ${CMAKE_SOURCE_DIR}/src/base/fs_watcher.c
    ${CMAKE_SOURCE_DIR}/src/base/hash/custom.c
    ${CMAKE_SOURCE_DIR}/src/base/hash/fnv.c
    ${CMAKE_SOURCE_DIR}/src/base/hash/jenkins.c
    ${CMAKE_SOURCE_DIR}/src/base/hash/sip.c
    ${CMAKE_SOURCE_DIR}/src/base/hash_map.c
    ${CMAKE_SOURCE_DIR}/src/base/hash_set.c
    ${CMAKE_SOURCE_DIR}/src/base/heap.c
    ${CMAKE_SOURCE_DIR}/src/base/int128.c
    ${CMAKE_SOURCE_DIR}/src/base/io.c
    ${CMAKE_SOURCE_DIR}/src/base/itoa.c
    ${CMAKE_SOURCE_DIR}/src/base/linked_list.c
    ${CMAKE_SOURCE_DIR}/src/base/list.c
    ${CMAKE_SOURCE_DIR}/src/base/memory/api.c
    ${CMAKE_SOURCE_DIR}/src/base/memory/arena.c
    ${CMAKE_SOURCE_DIR}/src/base/memory/block.c
    ${CMAKE_SOURCE_DIR}/src/base/memory/global.c
    ${CMAKE_SOURCE_DIR}/src/base/memory/page.c
    ${CMAKE_SOURCE_DIR}/src/base/mutex.c
    ${CMAKE_SOURCE_DIR}/src/base/non_null.c
    ${CMAKE_SOURCE_DIR}/src/base/object/schema.c
    ${CMAKE_SOURCE_DIR}/src/base/object/value.c
    ${CMAKE_SOURCE_DIR}/src/base/object/value/list.c
    ${CMAKE_SOURCE_DIR}/src/base/object/value/object.c
    ${CMAKE_SOURCE_DIR}/src/base/optional.c
    ${CMAKE_SOURCE_DIR}/src/base/ordered_hash_map.c
    ${CMAKE_SOURCE_DIR}/src/base/path.c
    ${CMAKE_SOURCE_DIR}/src/base/queue.c
    ${CMAKE_SOURCE_DIR}/src/base/quick_select.c
    ${CMAKE_SOURCE_DIR}/src/base/quick_sort.c
    ${CMAKE_SOURCE_DIR}/src/base/rc.c
    ${CMAKE_SOURCE_DIR}/src/base/ref.c
    ${CMAKE_SOURCE_DIR}/src/base/ref_mut.c
    ${CMAKE_SOURCE_DIR}/src/base/ref_non_null.c
    ${CMAKE_SOURCE_DIR}/src/base/result.c
    ${CMAKE_SOURCE_DIR}/src/base/singleton.c
    ${CMAKE_SOURCE_DIR}/src/base/sized_array.c
    ${CMAKE_SOURCE_DIR}/src/base/sized_str.c
    ${CMAKE_SOURCE_DIR}/src/base/stack.c
    ${CMAKE_SOURCE_DIR}/src/base/str.c
    ${CMAKE_SOURCE_DIR}/src/base/string.c
    ${CMAKE_SOURCE_DIR}/src/base/test.c
    ${CMAKE_SOURCE_DIR}/src/base/tree.c
    ${CMAKE_SOURCE_DIR}/src/base/tree_map.c
    ${CMAKE_SOURCE_DIR}/src/base/vec.c
    ${CMAKE_SOURCE_DIR}/src/base/yaml.c)

add_library(lily_base STATIC ${BASE_SRC}
                             ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_base.c)
target_link_libraries(lily_base PRIVATE lily_builtin lily_libyaml)
target_include_directories(lily_base PRIVATE ${LILY_INCLUDE})

# lily_cli
set(CLI_LILY_SRC ${CMAKE_SOURCE_DIR}/src/cli/lily/lily.c
                 ${CMAKE_SOURCE_DIR}/src/cli/lily/parse_config.c)

add_library(lily_cli STATIC ${CLI_LILY_SRC}
                            ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_cli.c)
target_link_libraries(lily_cli PRIVATE lily_base)
target_include_directories(lily_cli PRIVATE ${LILY_INCLUDE})

# lilyc_cli
set(CLI_LILYC_SRC src/cli/lilyc/lilyc.c src/cli/lilyc/parse_config.c)

add_library(lilyc_cli STATIC ${CLI_LILYC_SRC}
                             ${CMAKE_SOURCE_DIR}/src/ex/lib/lilyc_cli.c)
target_link_libraries(lilyc_cli PRIVATE lily_base)
target_include_directories(lilyc_cli PRIVATE ${LILY_INCLUDE})

# lily_command
set(COMMAND_LILY_SRC
    ${CMAKE_SOURCE_DIR}/src/command/lily/build/build.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/cc/cc.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/compile/compile.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/cpp/cpp.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/init/init.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/new/new.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/run/run.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/test/test.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/to/to.c)

add_library(lily_command STATIC ${COMMAND_LILY_SRC}
                                ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_command.c)
target_link_libraries(lily_command PRIVATE lily_base lily_core_lily_package)
target_include_directories(lily_command PRIVATE ${LILY_INCLUDE})

# lilyc_command
set(COMMAND_LILYC_SRC ${CMAKE_SOURCE_DIR}/src/command/lilyc/lilyc.c)

add_library(lilyc_command STATIC ${COMMAND_LILYC_SRC}
                                 ${CMAKE_SOURCE_DIR}/src/ex/lib/lilyc_command.c)
target_link_libraries(lilyc_command PRIVATE lily_core_lily_compiler_package)
target_include_directories(lilyc_command PRIVATE ${LILY_INCLUDE})

# lily_core_cc_diagnostic
set(LILY_CORE_CC_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/diagnostic/warning.c)

add_library(
  lily_core_cc_diagnostic STATIC
  ${LILY_CORE_CC_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_cc_diagnostic.c)
target_link_libraries(lily_core_cc_diagnostic PRIVATE lily_core_shared)
target_include_directories(lily_core_cc_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_cpp_diagnostic
set(LILY_CORE_CPP_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cpp/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/cpp/diagnostic/warning.c)

add_library(
  lily_core_cpp_diagnostic STATIC
  ${LILY_CORE_CPP_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_cpp_diagnostic.c)
target_link_libraries(lily_core_cpp_diagnostic PRIVATE lily_core_shared)
target_include_directories(lily_core_cpp_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_lily_analysis
set(LILY_CORE_LILY_ANALYSIS_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/body/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/body/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/body/fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/body/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/body/trait.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/alias.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/attribute.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/method.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/module.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/prototype.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/trait.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl/type.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/access.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/array.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/binary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/cast.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/compiler_fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/lambda.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/list.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/literal.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/tuple.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr/unary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/array.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/as.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/list_head.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/list_tail.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/list.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/literal.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/name.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/range.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/record_call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/table.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/tuple.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern/variant_call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/asm.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/await.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/block.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/break.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/drop.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/for.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/if.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/match.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/next.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/raise.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/return.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/switch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/try.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/unsafe.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt/while.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/access.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/captured_variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/compiler_generic.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/data_type.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/decl.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/expr.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/field.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/field_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/generic_param.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/global_name.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/history.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/impl_param.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/inherit_param.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/operator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/operator_register.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/parent.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/pattern.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/safety_mode.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/scope.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/scope_container.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/scope_decls.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/scope_response.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/scope_stmt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/signature.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/stmt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/variant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/virtual_scope.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/checked/virtual_variable_manager.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/analysis/analysis.c)

add_library(
  lily_core_lily_analysis STATIC
  ${LILY_CORE_LILY_ANALYSIS_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_analysis.c)
target_link_libraries(lily_core_lily_analysis PRIVATE lily_core_lily_functions)
target_include_directories(lily_core_lily_analysis PRIVATE ${LILY_INCLUDE})

# lily_core_lily_interpreter_package
set(LILY_CORE_LILY_INTERPRETER_PACKAGE_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/package/package.c)

add_library(
  lily_core_lily_interpreter_package STATIC
  ${LILY_CORE_LILY_INTERPRETER_PACKAGE_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_interpreter_package.c)
target_link_libraries(
  lily_core_lily_interpreter_package
  PRIVATE lily_core_lily_mir lily_core_lily_analysis lily_core_lily_parser
          lily_core_lily_precompiler lily_core_lily_interpreter_vm)
target_include_directories(lily_core_lily_interpreter_package
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_interpreter_vm
set(LILY_CORE_LILY_INTERPRETER_VM_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/operator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/sys.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/memory.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/value.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/vm.c)

add_library(
  lily_core_lily_interpreter_vm STATIC
  ${LILY_CORE_LILY_INTERPRETER_VM_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_interpreter_vm.c)
target_link_libraries(lily_core_lily_interpreter_vm PRIVATE lily_base lily_sys
                                                            lily_builtin)
target_include_directories(lily_core_lily_interpreter_vm
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler add_library(LilyCoreLilyCompiler STATIC)

# lily_core_lily_compiler_ar
set(LILY_CORE_LILY_COMPILER_AR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ar/ar.c)

add_library(
  lily_core_lily_compiler_ar STATIC
  ${LILY_CORE_LILY_COMPILER_AR_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ar.c)
target_link_libraries(lily_core_lily_compiler_ar
                      PRIVATE lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_compiler_ar PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_driver
set(LILY_CORE_LILY_COMPILER_DRIVER
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/driver/lld.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/driver/llvm-ar.cpp)

add_library(
  lily_core_lily_compiler_driver STATIC
  ${LILY_CORE_LILY_COMPILER_DRIVER}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_driver.c)
target_link_libraries(lily_core_lily_compiler_driver
                      PRIVATE lily_base ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
target_include_directories(lily_core_lily_compiler_driver
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir
set(LILY_CORE_LILY_COMPILER_IR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/ir.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/js.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm.cpp)

add_library(
  lily_core_lily_compiler_ir STATIC
  ${LILY_CORE_LILY_COMPILER_IR_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir.c)
target_link_libraries(
  lily_core_lily_compiler_ir
  PRIVATE lily_core_lily_compiler_ir_cc lily_core_lily_compiler_ir_cpp
          lily_core_lily_compiler_ir_llvm)
target_include_directories(lily_core_lily_compiler_ir PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_cc
set(LILY_CORE_LILY_COMPILER_IR_CC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/condition.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/loop.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/switch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/macro.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/struct.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/typedef.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/union.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/alias.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/macro.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator.c)

add_library(
  lily_core_lily_compiler_ir_cc STATIC
  ${LILY_CORE_LILY_COMPILER_IR_CC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir_cc.c)
target_include_directories(lily_core_lily_compiler_ir_cc
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_cpp
set(LILY_CORE_LILY_COMPILER_IR_CPP_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/class/attribute.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/class/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/class/method.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/condition.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/loop.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/switch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/namespace.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/struct.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/typedef.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/union.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/alias.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/macro.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator.c)

add_library(
  lily_core_lily_compiler_ir_cpp STATIC
  ${LILY_CORE_LILY_COMPILER_IR_CPP_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir_cpp.c)
target_include_directories(lily_core_lily_compiler_ir_cpp
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_js

# lily_core_lily_compiler_ir_llvm
set(LILY_CORE_LILY_COMPILER_IR_LLVM_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/ar.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/compile.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/crt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/dl.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/emit.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/lily.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/linker.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/optimize.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/pending.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/scope.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/utils.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/linker/object_format.c)

add_library(
  lily_core_lily_compiler_ir_llvm STATIC
  ${LILY_CORE_LILY_COMPILER_IR_LLVM_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir_llvm.c)
target_link_libraries(
  lily_core_lily_compiler_ir_llvm
  PRIVATE lily_base lily_core_lily_mir lily_core_lily_compiler_driver
          ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
target_include_directories(lily_core_lily_compiler_ir_llvm
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_linker
set(LILY_CORE_LILY_COMPILER_LINKER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/linker/linker.c)

add_library(
  lily_core_lily_compiler_linker STATIC
  ${LILY_CORE_LILY_COMPILER_LINKER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_linker.c)
target_link_libraries(lily_core_lily_compiler_linker
                      PRIVATE lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_compiler_linker
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_output
set(LILY_CORE_LILY_COMPILER_OUTPUT
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/output/cache.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/output/bin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/output/lib.c)

add_library(
  lily_core_lily_compiler_output STATIC
  ${LILY_CORE_LILY_COMPILER_OUTPUT}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_output.c)
target_link_libraries(lily_core_lily_compiler_output PRIVATE lily_base)
target_include_directories(lily_core_lily_compiler_output
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_package
set(LILY_CORE_LILY_COMPILER_PACKAGE_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/package.c)

add_library(
  lily_core_lily_compiler_package STATIC
  ${LILY_CORE_LILY_COMPILER_PACKAGE_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_package.c)
target_link_libraries(
  lily_core_lily_compiler_package
  PRIVATE lily_core_lily_compiler_ar
          lily_core_lily_compiler_ir
          lily_core_lily_compiler_linker
          lily_core_lily_mir
          lily_core_lily_parser
          lily_core_lily_precompiler
          lily_core_lily_compiler_output
          ${LILY_THREAD_LIB})
target_include_directories(lily_core_lily_compiler_package
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_diagnostic
set(LILY_CORE_LILY_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/diagnostic/warning.c)

add_library(
  lily_core_lily_diagnostic STATIC
  ${LILY_CORE_LILY_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_diagnostic.c)
target_link_libraries(lily_core_lily_diagnostic PRIVATE lily_core_shared
                                                        lily_base)
target_include_directories(lily_core_lily_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_lily_functions
set(LILY_CORE_LILY_FUNCTIONS_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/functions/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/functions/sys.c)

add_library(
  lily_core_lily_functions STATIC
  ${LILY_CORE_LILY_FUNCTIONS_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_functions.c)
target_link_libraries(lily_core_lily_functions PRIVATE lily_core_lily_analysis)
target_include_directories(lily_core_lily_functions PRIVATE ${LILY_INCLUDE})

# lily_core_lily_jit

# lily_core_lily_mir
set(LILY_CORE_LILY_MIR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/case_value.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/dt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/assignable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/binary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/cond.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/unary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/stmt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/val.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/stmt/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/block_limit.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/debug_info.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/dt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/instruction.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/linkage.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/mir.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/name_manager.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/scope.c)

add_library(
  lily_core_lily_mir STATIC ${LILY_CORE_LILY_MIR_SRC}
                            ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_mir.c)
target_link_libraries(lily_core_lily_mir PRIVATE lily_base
                                                 lily_core_lily_analysis)
target_include_directories(lily_core_lily_mir PRIVATE ${LILY_INCLUDE})

# lily_core_lily_package
set(LILY_CORE_LILY_PACKAGE_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/compiler/config.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/default_path.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/dependency_tree.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/interpreter/config.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/library.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/package.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/package/program.c)

add_library(
  lily_core_lily_package STATIC
  ${LILY_CORE_LILY_PACKAGE_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_package.c)
target_link_libraries(
  lily_core_lily_package
  PRIVATE lily_base lily_core_lily_mir lily_core_lily_parser
          lily_core_lily_precompiler lily_core_lily_compiler_package
          lily_core_lily_interpreter_package)
target_include_directories(lily_core_lily_package PRIVATE ${LILY_INCLUDE})

# lily_core_lily_parser
set(LILY_CORE_LILY_PARSER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/body/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/body/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/body/fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/body/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/body/trait.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/alias.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/attribute.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/include.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/method.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/module.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/prototype.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/trait.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/type.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl/use.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/access.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/array.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/binary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/cast.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/identifier.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/lambda.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/list.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/literal.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/try.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/tuple.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr/unary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/array.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/as.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/list.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/list_head.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/list_tail.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/literal.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/name.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/range.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/record_call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/tuple.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern/variant_call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/asm.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/await.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/block.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/break.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/defer.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/drop.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/for.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/if.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/match.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/next.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/raise.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/return.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/try.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/unsafe.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt/while.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/capture.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/data_type.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/decl.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/expr.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/field.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/field_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/generic_param.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/impl_param.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/inherit_param.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/pattern.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/stmt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/ast/variant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/parser/parser.c)

add_library(
  lily_core_lily_parser STATIC
  ${LILY_CORE_LILY_PARSER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_parser.c)
target_link_libraries(lily_core_lily_parser PRIVATE lily_core_lily_preparser
                                                    lily_core_lily_shared)
target_include_directories(lily_core_lily_parser PRIVATE ${LILY_INCLUDE})

# lily_core_lily_precompiler
set(LILY_CORE_LILY_PRECOMPILER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/precompiler/precompiler.c)

add_library(
  lily_core_lily_precompiler STATIC
  ${LILY_CORE_LILY_PRECOMPILER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_precompiler.c)
target_link_libraries(lily_core_lily_precompiler PRIVATE lily_core_lily_package
                                                         lily_base)
target_include_directories(lily_core_lily_precompiler PRIVATE ${LILY_INCLUDE})

# lily_core_lily_preparser
set(LILY_CORE_LILY_PREPARSER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/allow.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/arch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/deny.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/doc.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/forbid.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/link_info.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/os.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/repr.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/warn.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preparser.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess.c)

add_library(
  lily_core_lily_preparser STATIC
  ${LILY_CORE_LILY_PREPARSER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_preparser.c)
target_link_libraries(lily_core_lily_preparser PRIVATE lily_core_lily_scanner
                                                       lily_core_lily_shared)
target_include_directories(lily_core_lily_preparser PRIVATE ${LILY_INCLUDE})

# lily_core_lily_scanner
set(LILY_CORE_LILY_SCANNER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/scanner/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/scanner/token.c)

add_library(
  lily_core_lily_scanner STATIC
  ${LILY_CORE_LILY_SCANNER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_scanner.c)
target_link_libraries(lily_core_lily_scanner PRIVATE lily_base lily_core_shared)
target_include_directories(lily_core_lily_scanner PRIVATE ${LILY_INCLUDE})

# lily_core_lily_shared
set(LILY_CORE_LILY_SHARED_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/shared/visibility.c)

add_library(
  lily_core_lily_shared STATIC
  ${LILY_CORE_LILY_SHARED_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_shared.c)
target_include_directories(lily_core_lily_shared PRIVATE ${LILY_INCLUDE})

# lily_core_shared
set(LILY_CORE_SHARED_SRC
    ${CMAKE_SOURCE_DIR}/src/core/shared/target/arch.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/target/os.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/cursor.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/diagnostic.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/location.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/search.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/source.c)

add_library(
  lily_core_shared STATIC ${LILY_CORE_SHARED_SRC}
                          ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_shared.c)
target_link_libraries(
  lily_core_shared
  PRIVATE lily_base lily_core_lily_diagnostic lily_core_cc_diagnostic
          lily_core_cc_ci_diagnostic lily_core_cpp_diagnostic)
target_include_directories(lily_core_shared PRIVATE ${LILY_INCLUDE})

# lily
add_executable(lily ${CMAKE_SOURCE_DIR}/src/bin/lily/main.c
                    ${CMAKE_SOURCE_DIR}/src/ex/bin/lily.c)
target_link_libraries(lily PRIVATE lily_cli lily_command)
target_include_directories(lily PRIVATE ${LILY_INCLUDE})

# lilyc
add_executable(lilyc ${CMAKE_SOURCE_DIR}/src/bin/lilyc/main.c
                     ${CMAKE_SOURCE_DIR}/src/ex/bin/lilyc.c)
target_link_libraries(lilyc PRIVATE lilyc_cli lilyc_command ${LILY_LLD_LIBS}
                                    ${LILY_LLVM_LIBS})
target_include_directories(lilyc PRIVATE ${LILY_INCLUDE})

# lily_builtin
set(BUILTIN_SRC
    ${CMAKE_SOURCE_DIR}/lib/builtin/alloc.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/cstr.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/math.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/pointer.c)

add_library(lily_builtin SHARED ${BUILTIN_SRC}
                                ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_builtin.c)
target_include_directories(lily_builtin PUBLIC lib include)

# lily_sys
set(SYS_SRC ${CMAKE_SOURCE_DIR}/lib/sys/sys.c)

add_library(lily_sys SHARED ${SYS_SRC}
                            ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_sys.c)
target_include_directories(lily_sys PUBLIC lib include)

function(move_tests_dir)
  file(COPY tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  message("Move `tests` directory in build directory")
endfunction()

if(LILY_DEBUG)
  if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/tests)
    file(REMOVE tests)
    move_tests_dir()
  else()
    move_tests_dir()
  endif()

  enable_testing()

  add_executable(test_base ${CMAKE_SOURCE_DIR}/tests/base/base.c
                           ${CMAKE_SOURCE_DIR}/src/ex/bin/test_base.c)
  target_link_libraries(test_base PRIVATE lily_base)
  target_include_directories(test_base PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_base COMMAND test_base)

  add_executable(
    test_core_scanner ${CMAKE_SOURCE_DIR}/tests/core/lily/scanner/scanner.c
                      ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_scanner.c)
  target_link_libraries(test_core_scanner PRIVATE lily_core_lily_scanner)
  target_include_directories(test_core_scanner PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_scanner COMMAND test_core_scanner)

  add_executable(
    test_core_preparser
    ${CMAKE_SOURCE_DIR}/tests/core/lily/preparser/preparser.c
    ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_preparser.c)
  target_link_libraries(test_core_preparser PRIVATE lily_core_lily_preparser)
  target_include_directories(test_core_preparser PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_preparser COMMAND test_core_preparser)

  add_executable(
    test_core_precompiler
    ${CMAKE_SOURCE_DIR}/tests/core/lily/precompiler/precompiler.c
    ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_precompiler.c)
  target_link_libraries(test_core_precompiler
                        PRIVATE lily_core_lily_precompiler)
  target_include_directories(test_core_precompiler PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_precompiler COMMAND test_core_precompiler)

  add_executable(
    test_core_parser ${CMAKE_SOURCE_DIR}/tests/core/lily/parser/parser.c
                     ${CMAKE_SOURCE_DIR}/src/ex/bin/test_core_parser.c)
  target_link_libraries(
    test_core_parser PRIVATE lily_core_lily_parser lily_core_lily_package
                             ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
  target_include_directories(test_core_parser PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_parser COMMAND test_core_parser)

  add_executable(bench_buffer ${CMAKE_SOURCE_DIR}/benchmarks/base/buffer.c
                              ${CMAKE_SOURCE_DIR}/src/ex/bin/bench_buffer.c)
  target_link_libraries(bench_buffer PRIVATE lily_base)
  target_include_directories(bench_buffer PRIVATE ${LILY_INCLUDE})

  add_executable(bench_vec ${CMAKE_SOURCE_DIR}/benchmarks/base/vec.c
                           ${CMAKE_SOURCE_DIR}/src/ex/bin/bench_vec.c)
  target_link_libraries(bench_vec PRIVATE lily_base)
  target_include_directories(bench_vec PRIVATE ${LILY_INCLUDE})

  add_executable(bench_string ${CMAKE_SOURCE_DIR}/benchmarks/base/string.c
                              ${CMAKE_SOURCE_DIR}/src/ex/bin/bench_string.c)
  target_link_libraries(bench_string PRIVATE lily_base)
  target_include_directories(bench_string PRIVATE ${LILY_INCLUDE})

  add_executable(bench_hash_map ${CMAKE_SOURCE_DIR}/benchmarks/base/hash_map.c
                                ${CMAKE_SOURCE_DIR}/src/ex/bin/bench_hash_map.c)
  target_link_libraries(bench_hash_map PRIVATE lily_base)
  target_include_directories(bench_hash_map PRIVATE ${LILY_INCLUDE})

  add_executable(
    bench_global_allocator
    ${CMAKE_SOURCE_DIR}/benchmarks/base/memory/global.c
    ${CMAKE_SOURCE_DIR}/src/ex/bin/bench_global_allocator.c)
  target_link_libraries(bench_global_allocator PRIVATE lily_base)
  target_include_directories(bench_global_allocator PRIVATE ${LILY_INCLUDE})

  add_executable(
    bench_page_allocator ${CMAKE_SOURCE_DIR}/benchmarks/base/memory/page.c
                         ${CMAKE_SOURCE_DIR}/src/ex/bin/bench_page_allocator.c)
  target_link_libraries(bench_page_allocator PRIVATE lily_base)
  target_include_directories(bench_page_allocator PRIVATE ${LILY_INCLUDE})
endif()
