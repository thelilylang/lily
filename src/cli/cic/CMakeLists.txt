set(CIC_CLI_SRC
    ${CMAKE_SOURCE_DIR}/src/cli/cic/cic.c
    ${CMAKE_SOURCE_DIR}/src/cli/cic/config.c
    ${CMAKE_SOURCE_DIR}/src/cli/cic/parse_config.c)

# cic_cli
add_library(cic_cli STATIC ${CIC_CLI_SRC}
                           ${CMAKE_SOURCE_DIR}/src/ex/lib/cic_cli.c)
target_link_libraries(cic_cli PRIVATE lily_base)
target_include_directories(cic_cli PRIVATE ${LILY_INCLUDE})
