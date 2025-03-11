# lily_core_lily_compiler_ir_cc
set(LILY_CORE_LILY_COMPILER_IR_CC_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/condition.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/loop.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/switch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/function/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/macro.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/struct.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/typedef.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder/union.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/builder.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/alias.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/macro.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cc/generator.c)

add_library(
  lily_core_lily_compiler_ir_cc STATIC
  ${LILY_CORE_LILY_COMPILER_IR_CC_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir_cc.c)
target_include_directories(lily_core_lily_compiler_ir_cc
                           PRIVATE ${LILY_INCLUDE})
