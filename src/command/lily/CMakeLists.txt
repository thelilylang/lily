# lily_command
set(COMMAND_LILY_SRC
    ${CMAKE_SOURCE_DIR}/src/command/lily/build/build.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/cc/cc.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/compile/compile.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/cpp/cpp.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/init/init.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/new/new.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/run/run.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/test/test.c
    ${CMAKE_SOURCE_DIR}/src/command/lily/to/to.c)

add_library(lily_command STATIC ${COMMAND_LILY_SRC}
                                ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_command.c)
target_link_libraries(lily_command PRIVATE lily_base lily_core_lily_package)
target_include_directories(lily_command PRIVATE ${LILY_INCLUDE})
