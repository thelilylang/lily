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
