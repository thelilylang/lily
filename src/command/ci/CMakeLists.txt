set(CI_COMMAND_SRC
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/diagnostic.c
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/metadata/scanner.c
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/metadata.c
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/poll.c
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/process_unit.c
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/run.c
    ${CMAKE_SOURCE_DIR}/src/command/ci/self_test/self_test.c)

# ci_command
add_library(ci_command STATIC ${CI_COMMAND_SRC}
                              ${CMAKE_SOURCE_DIR}/src/ex/lib/ci_command.c)
target_link_libraries(ci_command PRIVATE lily_base lily_core_cc_ci cic_command)
target_include_directories(ci_command PRIVATE ${LILY_INCLUDE})
