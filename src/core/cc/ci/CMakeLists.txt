add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/cc/ci/diagnostic)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/cc/ci/extensions)

set(CI_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/ast.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/compile.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/features.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/include.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/infer.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/parser.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/predefined.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/project_config.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/resolver.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/resolver/data_type.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/resolver/data_type_access.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/resolver/expr.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/result.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/state_checker.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/token.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/typecheck.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/ci/visitor.c)

# lily_core_cc_ci
add_library(lily_core_cc_ci STATIC ${CI_SRC})
target_link_libraries(
  lily_core_cc_ci PRIVATE lily_base lily_core_cc_ci_diagnostic
                          lily_core_cc_ci_extensions lily_core_shared)
target_include_directories(lily_core_cc_ci PRIVATE ${LILY_INCLUDE})
