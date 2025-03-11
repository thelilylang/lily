# lily_core_lily_interpreter_vm
set(LILY_CORE_LILY_INTERPRETER_VM_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/builtin.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/error.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/operator.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/runtime/sys.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/memory.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/value.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/interpreter/vm/vm.c)

add_library(
  lily_core_lily_interpreter_vm STATIC
  ${LILY_CORE_LILY_INTERPRETER_VM_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_interpreter_vm.c)
target_link_libraries(lily_core_lily_interpreter_vm PRIVATE lily_base lily_sys
                                                            lily_builtin)
target_include_directories(lily_core_lily_interpreter_vm
                           PRIVATE ${LILY_INCLUDE})
