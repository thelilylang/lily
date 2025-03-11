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
