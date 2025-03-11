add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/js)

# lily_core_lily_compiler_ir
set(LILY_CORE_LILY_COMPILER_IR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/ir.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/js.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm.cpp)

add_library(
  lily_core_lily_compiler_ir STATIC
  ${LILY_CORE_LILY_COMPILER_IR_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir.c)
target_link_libraries(
  lily_core_lily_compiler_ir
  PRIVATE lily_core_lily_compiler_ir_cc lily_core_lily_compiler_ir_cpp
          lily_core_lily_compiler_ir_llvm)
target_include_directories(lily_core_lily_compiler_ir PRIVATE ${LILY_INCLUDE})
