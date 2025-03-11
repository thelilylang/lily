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
