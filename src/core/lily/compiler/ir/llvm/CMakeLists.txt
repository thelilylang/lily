# lily_core_lily_compiler_ir_llvm
set(LILY_CORE_LILY_COMPILER_IR_LLVM_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/ar.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/compile.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/crt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/dl.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/emit.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/lily.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/linker.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/optimize.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/pending.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/scope.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/llvm/utils.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/linker/object_format.c)

add_library(
  lily_core_lily_compiler_ir_llvm STATIC
  ${LILY_CORE_LILY_COMPILER_IR_LLVM_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir_llvm.c)
target_link_libraries(
  lily_core_lily_compiler_ir_llvm
  PRIVATE lily_base lily_core_lily_mir lily_core_lily_compiler_driver
          ${LILY_LLVM_LIBS} ${LILY_LLD_LIBS})
target_include_directories(lily_core_lily_compiler_ir_llvm
                           PRIVATE ${LILY_INCLUDE})
