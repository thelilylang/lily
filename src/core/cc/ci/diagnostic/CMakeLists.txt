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
