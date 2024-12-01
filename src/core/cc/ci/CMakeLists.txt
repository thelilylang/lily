include(${CMAKE_SOURCE_DIR}/cmake/Lily.cmake)

set(CI_CLI_SRC
    ${CMAKE_SOURCE_DIR}/src/cli/ci/ci.c ${CMAKE_SOURCE_DIR}/src/cli/ci/config.c
    ${CMAKE_SOURCE_DIR}/src/cli/ci/parse_config.c)

# ci_cli
add_library(ci_cli STATIC ${CI_CLI_SRC} ${CMAKE_SOURCE_DIR}/src/ex/lib/ci_cli.c)
target_link_libraries(ci_cli PRIVATE lily_base)
target_include_directories(ci_cli PRIVATE ${LILY_INCLUDE})

set(CI_COMMAND_SRC ${CMAKE_SOURCE_DIR}/src/command/ci/ci.c)

# ci_command
add_library(ci_command STATIC ${CI_COMMAND_SRC}
                              ${CMAKE_SOURCE_DIR}/src/ex/lib/ci_command.c)
target_link_libraries(ci_command PRIVATE lily_base lily_core_cc_ci)
target_include_directories(ci_command PRIVATE ${LILY_INCLUDE})

set(CI_DIAGNOSTIC_SRC ${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic/error.c
                      ${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic/warning.c)

# lily_core_cc_ci_diagnostic
add_library(
  lily_core_cc_ci_diagnostic STATIC
  ${CI_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_cc_ci_diagnostic.c)
target_link_libraries(lily_core_cc_ci_diagnostic PRIVATE lily_base
                                                         lily_core_shared)
target_include_directories(lily_core_cc_ci_diagnostic PRIVATE ${LILY_INCLUDE})

set(CI_EXTENSIONS_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/extensions/__has_feature.c)

# lily_core_cc_ci_extensions
add_library(
  lily_core_cc_ci_extensions STATIC
  ${CI_EXTENSIONS_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_cc_ci_extensions.c)
target_link_libraries(lily_core_cc_ci_extensions PRIVATE lily_base
                                                         lily_core_shared)
target_include_directories(lily_core_cc_ci_extensions PRIVATE ${LILY_INCLUDE})

set(CI_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/ast.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/features.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/include.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/parser.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/predefined.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/project_config.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/resolver.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/resolver/expr.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/result.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/state_checker.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/token.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/visitor.c)

# lily_core_cc_ci
add_library(lily_core_cc_ci STATIC ${CI_SRC})
target_link_libraries(
  lily_core_cc_ci PRIVATE lily_base lily_core_cc_ci_diagnostic
                          lily_core_cc_ci_extensions lily_core_shared)
target_include_directories(lily_core_cc_ci PRIVATE ${LILY_INCLUDE})

# ci
add_executable(ci ${CMAKE_SOURCE_DIR}/src/bin/ci/main.c
                  ${CMAKE_SOURCE_DIR}/src/ex/bin/ci.c)
target_link_libraries(ci PRIVATE lily_core_cc_ci ci_cli ci_command)
target_include_directories(ci PRIVATE ${LILY_INCLUDE})
