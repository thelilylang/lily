# lily_core_lily_compiler_ir_cpp
set(LILY_CORE_LILY_COMPILER_IR_CPP_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/class/attribute.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/class/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/class/method.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/condition.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/loop.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/switch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/function/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/namespace.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/struct.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/typedef.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder/union.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/builder.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/alias.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/class.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/enum_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/function.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/macro.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator/record_object.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/compiler/ir/cpp/generator.c)

add_library(
  lily_core_lily_compiler_ir_cpp STATIC
  ${LILY_CORE_LILY_COMPILER_IR_CPP_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_compiler_ir_cpp.c)
target_include_directories(lily_core_lily_compiler_ir_cpp
                           PRIVATE ${LILY_INCLUDE})
