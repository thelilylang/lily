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
