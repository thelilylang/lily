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
