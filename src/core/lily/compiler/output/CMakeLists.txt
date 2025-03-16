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
