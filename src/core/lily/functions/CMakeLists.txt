# lily_core_lily_functions
set(LILY_CORE_LILY_FUNCTIONS_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/functions/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/functions/sys.c)

add_library(
  lily_core_lily_functions STATIC
  ${LILY_CORE_LILY_FUNCTIONS_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_functions.c)
target_link_libraries(lily_core_lily_functions PRIVATE lily_core_lily_analysis)
target_include_directories(lily_core_lily_functions PRIVATE ${LILY_INCLUDE})
