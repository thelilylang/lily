# lily_core_cpp_diagnostic
set(LILY_CORE_CPP_DIAGNOSTIC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/cpp/diagnostic/error.c
    ${CMAKE_SOURCE_DIR}/src/core/cpp/diagnostic/warning.c)

add_library(
  lily_core_cpp_diagnostic STATIC
  ${LILY_CORE_CPP_DIAGNOSTIC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_cpp_diagnostic.c)
target_link_libraries(lily_core_cpp_diagnostic PRIVATE lily_core_shared)
target_include_directories(lily_core_cpp_diagnostic PRIVATE ${LILY_INCLUDE})
