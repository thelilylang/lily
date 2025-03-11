# lily_core_lily_mir
set(LILY_CORE_LILY_MIR_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/case_value.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/constant.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/dt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/enum.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/assignable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/binary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/call.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/cond.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr/unary.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/expr.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/fun.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/record.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/stmt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/val.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator/stmt/variable.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/block_limit.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/debug_info.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/dt.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/instruction.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/linkage.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/generator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/mir.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/name_manager.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/mir/scope.c)

add_library(
  lily_core_lily_mir STATIC ${LILY_CORE_LILY_MIR_SRC}
                            ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_mir.c)
target_link_libraries(lily_core_lily_mir PRIVATE lily_base
                                                 lily_core_lily_analysis)
target_include_directories(lily_core_lily_mir PRIVATE ${LILY_INCLUDE})
