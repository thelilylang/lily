# lily_core_lily_precompiler
set(LILY_CORE_LILY_PRECOMPILER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/precompiler/precompiler.c)

add_library(
  lily_core_lily_precompiler STATIC
  ${LILY_CORE_LILY_PRECOMPILER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_precompiler.c)
target_link_libraries(lily_core_lily_precompiler PRIVATE lily_core_lily_package
                                                         lily_base)
target_include_directories(lily_core_lily_precompiler PRIVATE ${LILY_INCLUDE})
