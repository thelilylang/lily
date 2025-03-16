# lily_core_cc_diagnostic
set(LILY_CORE_CC_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cc/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/cc/diagnostic/warning.c)

add_library(
  lily_core_cc_diagnostic STATIC
  ${LILY_CORE_CC_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_cc_diagnostic.c)
target_link_libraries(lily_core_cc_diagnostic PRIVATE lily_core_shared)
target_include_directories(lily_core_cc_diagnostic PRIVATE ${LILY_INCLUDE})
