# lilyc_command
set(COMMAND_LILYC_SRC ${CMAKE_SOURCE_DIR}/src/command/lilyc/lilyc.c)

add_library(lilyc_command STATIC ${COMMAND_LILYC_SRC}
                                 ${CMAKE_SOURCE_DIR}/src/ex/lib/lilyc_command.c)
target_link_libraries(lilyc_command PRIVATE lily_core_lily_compiler_package)
target_include_directories(lilyc_command PRIVATE ${LILY_INCLUDE})
