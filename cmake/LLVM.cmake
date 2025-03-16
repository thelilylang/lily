if(LILY_BUILD_LLVM)
  set(LLVM_DIR ${CMAKE_SOURCE_DIR}/lib/local/src/llvm-project/llvm)
  set(LLVM_VERSION "18.1.x")
  set(LILY_LLVM_BUILD_DIR
      ${CMAKE_CURRENT_BINARY_DIR}/lib/local/src/llvm-project/llvm)

  message(STATUS "Build LLVM v${LLVM_VERSION}")

  add_subdirectory(${LLVM_DIR} ${LILY_LLVM_BUILD_DIR})

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

  # Set LLD libraries
  set(LILY_LLD_LIBS lldCOFF lldCommon lldELF lldMachO lldMinGW lldWasm)
else()
  # Require LLVM
  find_package(LLVM 18.1 CONFIG REQUIRED)
  set(LLVM_VERSION ${LLVM_PACKAGE_VERSION})

  message(STATUS "Found LLVM v${LLVM_VERSION}")

  # Require LLD
  find_package(LLD 18)
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
  find_package(Clang 18)
  set(CLANG_VERSION ${LLVM_PACKAGE_VERSION})

  message(STATUS "Found Clang v${CLANG_VERSION}")

  # Add include directories
  include_directories(${LLVM_INCLUDE_DIRS})
  include_directories(${LLD_INCLUDE_DIRS})

  # Set LLVM libraries
  if(LILY_APPLE)
    set(LILY_LLVM_LIBS LLVM xar)
  else()
    set(LILY_LLVM_LIBS LLVM)
  endif()

  set(LILY_LLD_LIBS ${LLD_COFF} ${LLD_COMMON} ${LLD_ELF} ${LLD_MACHO}
                    ${LLD_MINGW} ${LLD_WASM})
endif()

find_library(ZLIB NAMES libz.a libz.so zlib libz z)
find_library(ZSTD NAMES libzstd.a libzstdstatic.a zstd NAMES_PER_DIR)

list(APPEND LILY_LLVM_LIBS "${ZLIB};${ZSTD}")
