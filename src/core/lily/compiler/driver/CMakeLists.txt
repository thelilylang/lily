# lily_core_lily_compiler_driver
set(LILY_CORE_LILY_COMPILER_DRIVER
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/driver/lld.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/driver/llvm-ar.cpp)

add_library(
  lily_core_lily_compiler_driver STATIC
  ${LILY_CORE_LILY_COMPILER_DRIVER}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_driver.c)
target_link_libraries(lily_core_lily_compiler_driver
                      PRIVATE lily_base ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
target_include_directories(lily_core_lily_compiler_driver
                           PRIVATE ${LILY_INCLUDE})
