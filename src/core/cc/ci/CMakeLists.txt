set(CI_DIAGNOSTIC_SRC ${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic/error.c
                      ${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic/warning.c)

add_library(lily_core_cc_ci_diagnostic STATIC ${CI_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_cc_ci_diagnostic PRIVATE lily_base
                                                         lily_core_shared)
target_include_directories(lily_core_cc_ci_diagnostic PRIVATE ${LILY_INCLUDE})

set(CI_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/ast.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/file.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/parser.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/token.c)

add_library(lily_core_cc_ci STATIC ${CI_SRC})
target_link_libraries(
  lily_core_cc_ci PRIVATE lily_base lily_core_cc_ci_diagnostic lily_core_shared)
target_include_directories(lily_core_cc_ci PRIVATE ${LILY_INCLUDE})

add_executable(ci ${CMAKE_SOURCE_DIR}/src/bin/ci/main.c)
target_link_libraries(ci PRIVATE lily_core_cc_ci)
target_include_directories(ci PRIVATE ${LILY_INCLUDE})
