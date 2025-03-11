# cic
add_executable(cic ${CMAKE_SOURCE_DIR}/src/bin/cic/main.c
                   ${CMAKE_SOURCE_DIR}/src/ex/bin/cic.c)
target_link_libraries(cic PRIVATE lily_core_cc_ci cic_cli cic_command)
target_include_directories(cic PRIVATE ${LILY_INCLUDE})
