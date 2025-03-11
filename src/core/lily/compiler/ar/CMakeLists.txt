# lily_core_lily_compiler_ar
set(LILY_CORE_LILY_COMPILER_AR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ar/ar.c)

add_library(
  lily_core_lily_compiler_ar STATIC
  ${LILY_CORE_LILY_COMPILER_AR_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ar.c)
target_link_libraries(lily_core_lily_compiler_ar
                      PRIVATE lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_compiler_ar PRIVATE ${LILY_INCLUDE})
