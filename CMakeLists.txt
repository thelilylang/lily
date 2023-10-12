cmake_minimum_required(VERSION 3.20)
project(
  lily
  VERSION 0.0.2
  HOMEPAGE_URL "https://github.com/thelilylang/lily"
  LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LILY_VERSION_MAJOR 0)
set(LILY_VERSION_MINOR 0)
set(LILY_VERSION_PATCH 2)
set(LILY_VERSION
    "${LILY_VERSION_MAJOR}.${LILY_VERSION_MINOR}.${LILY_VERSION_PATCH}")
message(STATUS "Build lily v${LILY_VERSION}")

if(NOT WIN32)
  # NOTE: On WSL you must to disable `-fdiagnostics-color=always`
  add_compile_options(-Wall -O3 -fdiagnostics-color=always)
else()
  add_compile_options(-Wall -O3)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "/wd4710 /wd4711 /wd4255")
endif()

if(UNIX AND NOT APPLE)
  set(LILY_LINUX 1)
endif()

find_package(LLVM 16 CONFIG REQUIRED)
message(STATUS "Found LLVM v${LLVM_PACKAGE_VERSION}")

# find_package(clang 15 CONFIG REQUIRED) find_package(lld 15 CONFIG REQUIRED)
set(LILY_LLVM_LIBS LLVM)
set(LILY_LLD_LIBS lily_lld_common lily_lld_coff lily_lld_elf lily_lld_mach_o
                  lily_lld_mingw lily_lld_wasm)

find_library(ZLIB NAMES libz.a libz.so zlib libz z)
find_library(ZSTD NAMES libzstd.a libzstdstatic.a zstd NAMES_PER_DIR)

list(APPEND LILY_LLVM_LIBS "${ZLIB};${ZSTD}")

set(LILY_INCLUDE include lib/local/include)

if(MSVC)
  set(LILY_LLD_COMPILE_FLAGS "-std=c++17")
else()
  set(LILY_LLD_COMPILE_FLAGS "-std=c++17")
endif()

# lily_lld_common
set(LLD_COMMON_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Args.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/CommonLinkerContext.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/DWARF.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/ErrorHandler.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Filesystem.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Memory.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Reproduce.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Strings.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/TargetOptionsCommandFlags.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Timer.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/Common/Version.cpp)

add_library(lily_lld_common STATIC ${LLD_COMMON_SRC})
target_link_libraries(lily_lld_common PRIVATE ${LILY_LLVM_LIBS})
target_include_directories(
  lily_lld_common PRIVATE ${LILY_INCLUDE} lib/local/include/lld-prebuilt
                          lib/local/include/lld-prebuilt/Common)
set_target_properties(
  lily_lld_common PROPERTIES COMPILE_FLAGS ${LILY_LLD_COMPILE_FLAGS} LINK_FLAGS
                                                                     " ")

# lily_lld_coff
set(LLD_COFF_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/CallGraphSort.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/Chunks.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/COFFLinkerContext.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/DebugTypes.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/DLL.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/Driver.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/DriverUtils.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/ICF.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/InputFiles.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/LLDMapFile.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/LTO.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/MapFile.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/MarkLive.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/MinGW.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/PDB.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/Symbols.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/SymbolTable.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/COFF/Writer.cpp)

add_library(lily_lld_coff STATIC ${LLD_COFF_SRC})
target_link_libraries(lily_lld_coff PRIVATE ${LILY_LLVM_LIBS} lily_lld_common)
target_include_directories(
  lily_lld_coff PRIVATE ${LILY_INCLUDE} lib/local/include/lld-prebuilt/COFF
                        lib/local/include/lld-prebuilt)
set_target_properties(
  lily_lld_coff PROPERTIES COMPILE_FLAGS ${LILY_LLD_COMPILE_FLAGS} LINK_FLAGS
                                                                   " ")

# lily_lld_elf
set(LLD_ELF_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/AArch64ErrataFix.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/AArch64.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/AMDGPU.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/ARM.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/AVR.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/Hexagon.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/MipsArchTree.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/Mips.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/MSP430.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/PPC64.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/PPC.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/RISCV.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/SPARCV9.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/X86_64.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Arch/X86.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/ARMErrataFix.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/CallGraphSort.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Driver.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/DriverUtils.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/DWARF.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/EhFrame.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/ICF.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/InputFiles.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/InputSection.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/LinkerScript.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/LTO.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/MapFile.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/MarkLive.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/OutputSections.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Relocations.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/ScriptLexer.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/ScriptParser.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Symbols.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/SymbolTable.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/SyntheticSections.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Target.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Thunks.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/ELF/Writer.cpp)

add_library(lily_lld_elf STATIC ${LLD_ELF_SRC})
target_link_libraries(lily_lld_elf PRIVATE ${LILY_LLVM_LIBS} lily_lld_common)
target_include_directories(
  lily_lld_elf
  PRIVATE ${LILY_INCLUDE} lib/local/src/lld/ELF
          lib/local/include/lld-prebuilt/ELF lib/local/include/lld-prebuilt)
set_target_properties(
  lily_lld_elf PROPERTIES COMPILE_FLAGS ${LILY_LLD_COMPILE_FLAGS} LINK_FLAGS
                                                                  " ")

# lily_lld_mach_o
set(LLD_MACH_O_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Arch/ARM64_32.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Arch/ARM64Common.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Arch/ARM64.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Arch/ARM.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Arch/X86_64.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/ConcatOutputSection.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Driver.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/DriverUtils.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Dwarf.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/EhFrame.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/ExportTrie.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/ICF.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/InputFiles.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/InputSection.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/LTO.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/MapFile.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/MarkLive.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/ObjC.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/OutputSection.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/OutputSegment.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Relocations.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/SectionPriorities.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Symbols.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/SymbolTable.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/SyntheticSections.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Target.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/UnwindInfoSection.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/MachO/Writer.cpp)

add_library(lily_lld_mach_o STATIC ${LLD_MACH_O_SRC})
target_link_libraries(lily_lld_mach_o PRIVATE ${LILY_LLVM_LIBS} lily_lld_common)
target_include_directories(
  lily_lld_mach_o
  PRIVATE ${LILY_INCLUDE} lib/local/src/lld/MachO
          lib/local/include/lld-prebuilt/MachO lib/local/include/libunwind/
          lib/local/include/lld-prebuilt)
set_target_properties(
  lily_lld_mach_o PROPERTIES COMPILE_FLAGS ${LILY_LLD_COMPILE_FLAGS} LINK_FLAGS
                                                                     " ")

# lily_lld_mingw
set(LLD_MINGW_SRC lib/local/src/lld/MinGW/Driver.cpp)

add_library(lily_lld_mingw STATIC ${LLD_MINGW_SRC})
target_link_libraries(lily_lld_mingw PRIVATE ${LILY_LLVM_LIBS} lily_lld_common)
target_include_directories(
  lily_lld_mingw
  PRIVATE ${LILY_INCLUDE} lib/local/src/lld/MinGW
          lib/local/include/lld-prebuilt/MinGW lib/local/include/lld-prebuilt)
set_target_properties(
  lily_lld_mingw PROPERTIES COMPILE_FLAGS ${LILY_LLD_COMPILE_FLAGS} LINK_FLAGS
                                                                    " ")

# lily_lld_wasm
set(LLD_WASM_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/Driver.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/InputChunks.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/InputFiles.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/LTO.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/MapFile.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/MarkLive.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/OutputSections.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/OutputSegment.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/Relocations.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/Symbols.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/SymbolTable.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/SyntheticSections.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/Writer.cpp
    ${CMAKE_SOURCE_DIR}/lib/local/src/lld/wasm/WriterUtils.cpp)

add_library(lily_lld_wasm STATIC ${LLD_WASM_SRC})
target_link_libraries(lily_lld_wasm PRIVATE ${LILY_LLVM_LIBS} lily_lld_common)
target_include_directories(
  lily_lld_wasm
  PRIVATE ${LILY_INCLUDE} lib/local/src/lld/wasm
          lib/local/include/lld-prebuilt/wasm lib/local/include/lld-prebuilt)
set_target_properties(
  lily_lld_wasm PROPERTIES COMPILE_FLAGS ${LILY_LLD_COMPILE_FLAGS} LINK_FLAGS
                                                                   " ")

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
    ${CMAKE_SOURCE_DIR}/src/base/dir.c
    ${CMAKE_SOURCE_DIR}/src/base/error.c
    ${CMAKE_SOURCE_DIR}/src/base/file.c
    ${CMAKE_SOURCE_DIR}/src/base/format.c
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
    ${CMAKE_SOURCE_DIR}/src/base/optional.c
    ${CMAKE_SOURCE_DIR}/src/base/ordered_hash_map.c
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
    ${CMAKE_SOURCE_DIR}/src/base/stack.c
    ${CMAKE_SOURCE_DIR}/src/base/str.c
    ${CMAKE_SOURCE_DIR}/src/base/string.c
    ${CMAKE_SOURCE_DIR}/src/base/test.c
    ${CMAKE_SOURCE_DIR}/src/base/tree.c
    ${CMAKE_SOURCE_DIR}/src/base/tree_map.c
    ${CMAKE_SOURCE_DIR}/src/base/vec.c)

add_library(lily_base STATIC ${BASE_SRC})
target_link_libraries(lily_base PUBLIC lily_builtin)
target_include_directories(lily_base PRIVATE ${LILY_INCLUDE})

# lily_cli
set(CLI_LILY_SRC src/cli/lily/lily.c src/cli/lily/parse_config.c)

add_library(lily_cli STATIC ${CLI_LILY_SRC})
target_link_libraries(lily_cli PUBLIC lily_base)
target_include_directories(lily_cli PRIVATE ${LILY_INCLUDE})

# lilyc_cli
set(CLI_LILYC_SRC src/cli/lilyc/lilyc.c src/cli/lilyc/parse_config.c)

add_library(lilyc_cli STATIC ${CLI_LILYC_SRC})
target_link_libraries(lilyc_cli PUBLIC lily_base)
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

add_library(lily_command STATIC ${COMMAND_LILY_SRC})
target_link_libraries(lily_command PUBLIC lily_base)
target_include_directories(lily_command PRIVATE ${LILY_INCLUDE})

# lilyc_command
set(COMMAND_LILYC_SRC ${CMAKE_SOURCE_DIR}/src/command/lilyc/lilyc.c)

add_library(lilyc_command STATIC ${COMMAND_LILYC_SRC})
target_link_libraries(lilyc_command PUBLIC lily_core_lily_compiler_package)
target_include_directories(lilyc_command PRIVATE ${LILY_INCLUDE})

# lily_core_cc_diagnostic
set(LILY_CORE_CC_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/diagnostic/warning.c)

add_library(lily_core_cc_diagnostic STATIC ${LILY_CORE_CC_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_cc_diagnostic PUBLIC lily_core_shared)
target_include_directories(lily_core_cc_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_cpp_diagnostic
set(LILY_CORE_CPP_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cpp/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/cpp/diagnostic/warning.c)

add_library(lily_core_cpp_diagnostic STATIC ${LILY_CORE_CPP_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_cpp_diagnostic PUBLIC lily_core_shared)
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

add_library(lily_core_lily_analysis STATIC ${LILY_CORE_LILY_ANALYSIS_SRC})
target_link_libraries(lily_core_lily_analysis PUBLIC lily_core_lily_functions)
target_include_directories(lily_core_lily_analysis PRIVATE ${LILY_INCLUDE})

# lily_core_lily_interpreter_package
set(LILY_CORE_LILY_INTERPRETER_PACKAGE_SRC
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/package/package.c)

add_library(lily_core_lily_interpreter_package STATIC ${LILY_CORE_LILY_INTERPRETER_PACKAGE_SRC})
target_link_libraries(lily_core_lily_interpreter_package PRIVATE lily_core_lily_mir lily_core_lily_parser lily_core_lily_precompiler)
target_include_directories(lily_core_lily_interpreter_package PRIVATE ${LILY_INCLUDE})

# lily_core_lily_interpreter_vm
set(LILY_CORE_LILY_INTERPRETER_VM_SRC
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/builtin.c
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/error.c
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/operator.c
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/sys.c
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/memory.c
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/value.c
	${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/vm.c)

add_library(lily_core_lily_interpreter_vm STATIC ${LILY_CORE_LILY_INTERPRETER_VM_SRC})
target_link_libraries(lily_core_lily_interpreter_vm PUBLIC lily_base)
target_include_directories(lily_core_lily_interpreter_vm PRIVATE include)

# lily_core_lily_compiler add_library(LilyCoreLilyCompiler STATIC)

# lily_core_lily_compiler_ar
set(LILY_CORE_LILY_COMPILER_AR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ar/ar.c)

add_library(lily_core_lily_compiler_ar STATIC ${LILY_CORE_LILY_COMPILER_AR_SRC})
target_link_libraries(lily_core_lily_compiler_ar
                      PUBLIC lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_compiler_ar PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_driver
set(LILY_CORE_LILY_COMPILER_DRIVER
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/driver/lld.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/driver/llvm-ar.cpp)

add_library(lily_core_lily_compiler_driver STATIC
            ${LILY_CORE_LILY_COMPILER_DRIVER})
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
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm.c)

add_library(lily_core_lily_compiler_ir STATIC ${LILY_CORE_LILY_COMPILER_IR_SRC})
target_link_libraries(
  lily_core_lily_compiler_ir
  PUBLIC lily_core_lily_compiler_ir_cc lily_core_lily_compiler_ir_cpp
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

add_library(lily_core_lily_compiler_ir_cc STATIC
            ${LILY_CORE_LILY_COMPILER_IR_CC_SRC})
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

add_library(lily_core_lily_compiler_ir_cpp STATIC
            ${LILY_CORE_LILY_COMPILER_IR_CPP_SRC})
target_include_directories(lily_core_lily_compiler_ir_cpp
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_js

# lily_core_lily_compiler_ir_llvm
set(LILY_CORE_LILY_COMPILER_IR_LLVM_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/ar.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/compile.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/emit.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/lily.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/linker.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/optimize.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/pending.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/scope.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/utils.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/linker/object_format.c)

add_library(lily_core_lily_compiler_ir_llvm STATIC
            ${LILY_CORE_LILY_COMPILER_IR_LLVM_SRC})
target_link_libraries(
  lily_core_lily_compiler_ir_llvm
  PRIVATE lily_base lily_core_lily_mir lily_core_lily_compiler_driver
          ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
target_include_directories(lily_core_lily_compiler_ir_llvm
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_linker
set(LILY_CORE_LILY_COMPILER_LINKER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/linker/linker.c)

add_library(lily_core_lily_compiler_linker STATIC
            ${LILY_CORE_LILY_COMPILER_LINKER_SRC})
target_link_libraries(lily_core_lily_compiler_linker
                      PUBLIC lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_compiler_linker
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_output
set(LILY_CORE_LILY_COMPILER_OUTPUT
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/output/cache.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/output/bin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/output/lib.c)

add_library(lily_core_lily_compiler_output STATIC
            ${LILY_CORE_LILY_COMPILER_OUTPUT})
target_link_libraries(lily_core_lily_compiler_output PRIVATE lily_base)
target_include_directories(lily_core_lily_compiler_output
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_package
set(LILY_CORE_LILY_COMPILER_PACKAGE_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/config.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/default_path.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/dependency_tree.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/library.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/package.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/package/program.c)

add_library(lily_core_lily_compiler_package STATIC
            ${LILY_CORE_LILY_COMPILER_PACKAGE_SRC})
target_link_libraries(
  lily_core_lily_compiler_package
  PRIVATE lily_core_lily_compiler_ar
          lily_core_lily_compiler_ir
          lily_core_lily_compiler_linker
          lily_core_lily_mir
          lily_core_lily_parser
          lily_core_lily_precompiler
          lily_core_lily_compiler_output)
target_include_directories(lily_core_lily_compiler_package
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_diagnostic
set(LILY_CORE_LILY_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/diagnostic/warning.c)

add_library(lily_core_lily_diagnostic STATIC ${LILY_CORE_LILY_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_lily_diagnostic PUBLIC lily_core_shared
                                                       lily_base)
target_include_directories(lily_core_lily_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_lily_functions
set(LILY_CORE_LILY_FUNCTIONS_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/functions/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/functions/sys.c)

add_library(lily_core_lily_functions STATIC ${LILY_CORE_LILY_FUNCTIONS_SRC})
target_link_libraries(lily_core_lily_functions PUBLIC lily_core_lily_analysis)
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

add_library(lily_core_lily_mir STATIC ${LILY_CORE_LILY_MIR_SRC})
target_link_libraries(lily_core_lily_mir PUBLIC lily_base
                                                lily_core_lily_analysis)
target_include_directories(lily_core_lily_mir PRIVATE ${LILY_INCLUDE})

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

add_library(lily_core_lily_parser STATIC ${LILY_CORE_LILY_PARSER_SRC})
target_link_libraries(lily_core_lily_parser PUBLIC lily_core_lily_preparser
                                                   lily_core_lily_shared)
target_include_directories(lily_core_lily_parser PRIVATE ${LILY_INCLUDE})

# lily_core_lily_precompiler
set(LILY_CORE_LILY_PRECOMPILER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/precompiler/precompiler.c)

add_library(lily_core_lily_precompiler STATIC ${LILY_CORE_LILY_PRECOMPILER_SRC})
target_link_libraries(lily_core_lily_precompiler
                      PUBLIC lily_core_lily_compiler_package)
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

add_library(lily_core_lily_preparser STATIC ${LILY_CORE_LILY_PREPARSER_SRC})
target_link_libraries(lily_core_lily_preparser PUBLIC lily_core_lily_scanner)
target_include_directories(lily_core_lily_preparser PRIVATE ${LILY_INCLUDE})

# lily_core_lily_scanner
set(LILY_CORE_LILY_SCANNER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/scanner/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/scanner/token.c)

add_library(lily_core_lily_scanner STATIC ${LILY_CORE_LILY_SCANNER_SRC})
target_link_libraries(lily_core_lily_scanner PUBLIC lily_base lily_core_shared)
target_include_directories(lily_core_lily_scanner PRIVATE ${LILY_INCLUDE})

# lily_core_lily_shared
set(LILY_CORE_LILY_SHARED_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/shared/visibility.c)

add_library(lily_core_lily_shared STATIC ${LILY_CORE_LILY_SHARED_SRC})
target_include_directories(lily_core_lily_shared PRIVATE ${LILY_INCLUDE})

# lily_core_shared
set(LILY_CORE_SHARED
    ${CMAKE_SOURCE_DIR}/src/core/shared/target/arch.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/target/os.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/cursor.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/diagnostic.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/location.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/source.c)

add_library(lily_core_shared STATIC ${LILY_CORE_SHARED})
target_link_libraries(
  lily_core_shared PRIVATE lily_base lily_core_lily_diagnostic
                           lily_core_cc_diagnostic lily_core_cpp_diagnostic)
target_include_directories(lily_core_shared PRIVATE ${LILY_INCLUDE})

add_executable(lily ${CMAKE_SOURCE_DIR}/src/bin/lily/main.c)
target_link_libraries(lily PRIVATE lily_cli lily_command)
target_include_directories(lily PRIVATE ${LILY_INCLUDE})

add_executable(lilyc ${CMAKE_SOURCE_DIR}/src/bin/lilyc/main.c)
target_link_libraries(lilyc PRIVATE lilyc_cli lilyc_command ${LILY_LLD_LIBS}
                                    ${LILY_LLVM_LIBS})
target_include_directories(lilyc PRIVATE ${LILY_INCLUDE})

# lily_builtin
set(BUILTIN_SRC
    ${CMAKE_SOURCE_DIR}/lib/builtin/alloc.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/cstr.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/math.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/pointer.c)

add_library(lily_builtin SHARED ${BUILTIN_SRC})
target_include_directories(lily_builtin PUBLIC lib include)

# lily_sys
set(SYS_SRC ${CMAKE_SOURCE_DIR}/lib/sys/sys.c)

add_library(lily_sys SHARED ${SYS_SRC})
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

  add_executable(test_base ${CMAKE_SOURCE_DIR}/tests/base/base.c)
  target_link_libraries(test_base lily_base)
  target_include_directories(test_base PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_base COMMAND test_base)

  add_executable(test_core_scanner
                 ${CMAKE_SOURCE_DIR}/tests/core/lily/scanner/scanner.c)
  target_link_libraries(test_core_scanner lily_base lily_core_lily_scanner)
  target_include_directories(test_core_scanner PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_scanner COMMAND test_core_scanner)

  add_executable(test_core_preparser
                 ${CMAKE_SOURCE_DIR}/tests/core/lily/preparser/preparser.c)
  target_link_libraries(test_core_preparser lily_base lily_core_lily_preparser
                        lily_core_lily_shared)
  target_include_directories(test_core_preparser PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_preparser COMMAND test_core_preparser)

  add_executable(test_core_precompiler
                 ${CMAKE_SOURCE_DIR}/tests/core/lily/precompiler/precompiler.c)
  target_link_libraries(test_core_precompiler lily_base
                        lily_core_lily_precompiler)
  target_include_directories(test_core_precompiler PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_precompiler COMMAND test_core_precompiler)

  add_executable(test_core_parser
                 ${CMAKE_SOURCE_DIR}/tests/core/lily/parser/parser.c)
  target_link_libraries(
    test_core_parser lily_base lily_core_lily_parser lily_core_lily_preparser
    lily_core_lily_compiler_package)
  target_include_directories(test_core_parser PRIVATE ${LILY_INCLUDE})

  add_test(NAME test_core_parser COMMAND test_core_parser)

  add_executable(bench_buffer ${CMAKE_SOURCE_DIR}/benchmarks/base/buffer.c)
  target_link_libraries(bench_buffer lily_base)
  target_include_directories(bench_buffer PRIVATE ${LILY_INCLUDE})

  add_executable(bench_vec ${CMAKE_SOURCE_DIR}/benchmarks/base/vec.c)
  target_link_libraries(bench_vec lily_base)
  target_include_directories(bench_vec PRIVATE ${LILY_INCLUDE})

  add_executable(bench_string ${CMAKE_SOURCE_DIR}/benchmarks/base/string.c)
  target_link_libraries(bench_string lily_base)
  target_include_directories(bench_string PRIVATE ${LILY_INCLUDE})

  add_executable(bench_hash_map ${CMAKE_SOURCE_DIR}/benchmarks/base/hash_map.c)
  target_link_libraries(bench_hash_map lily_base)
  target_include_directories(bench_hash_map PRIVATE ${LILY_INCLUDE})

  add_executable(bench_global_allocator
                 ${CMAKE_SOURCE_DIR}/benchmarks/base/memory/global.c)
  target_link_libraries(bench_global_allocator lily_base)
  target_include_directories(bench_global_allocator PRIVATE ${LILY_INCLUDE})

  add_executable(bench_page_allocator
                 ${CMAKE_SOURCE_DIR}/benchmarks/base/memory/page.c)
  target_link_libraries(bench_page_allocator lily_base)
  target_include_directories(bench_page_allocator PRIVATE ${LILY_INCLUDE})
endif()
