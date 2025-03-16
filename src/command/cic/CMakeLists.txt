set(CIC_COMMAND_SRC ${CMAKE_SOURCE_DIR}/src/command/cic/cic.c)

# cic_command
add_library(cic_command STATIC ${CIC_COMMAND_SRC}
                               ${CMAKE_SOURCE_DIR}/src/ex/lib/cic_command.c)
target_link_libraries(cic_command PRIVATE lily_base lily_core_cc_ci)
target_include_directories(cic_command PRIVATE ${LILY_INCLUDE})
