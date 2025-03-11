# lily_core_lily_diagnostic
set(LILY_CORE_LILY_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/diagnostic/warning.c)

add_library(
  lily_core_lily_diagnostic STATIC
  ${LILY_CORE_LILY_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_diagnostic.c)
target_link_libraries(lily_core_lily_diagnostic PRIVATE lily_core_shared
                                                        lily_base)
target_include_directories(lily_core_lily_diagnostic PRIVATE ${LILY_INCLUDE})
