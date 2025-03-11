# lily_core_lily_shared
set(LILY_CORE_LILY_SHARED_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/shared/visibility.c)

add_library(
  lily_core_lily_shared STATIC
  ${LILY_CORE_LILY_SHARED_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_shared.c)
target_include_directories(lily_core_lily_shared PRIVATE ${LILY_INCLUDE})
