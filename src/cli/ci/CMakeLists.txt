set(CI_CLI_SRC
    ${CMAKE_SOURCE_DIR}/src/cli/ci/ci.c ${CMAKE_SOURCE_DIR}/src/cli/ci/config.c
    ${CMAKE_SOURCE_DIR}/src/cli/ci/parse_config.c)

# ci_cli
add_library(ci_cli STATIC ${CI_CLI_SRC} ${CMAKE_SOURCE_DIR}/src/ex/lib/ci_cli.c)
target_link_libraries(ci_cli PRIVATE lily_base cic_cli)
target_include_directories(ci_cli PRIVATE ${LILY_INCLUDE})
