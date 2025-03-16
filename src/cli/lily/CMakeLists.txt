# lily_cli
set(CLI_LILY_SRC ${CMAKE_SOURCE_DIR}/src/cli/lily/lily.c
                 ${CMAKE_SOURCE_DIR}/src/cli/lily/parse_config.c)

add_library(lily_cli STATIC ${CLI_LILY_SRC}
                            ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_cli.c)
target_link_libraries(lily_cli PRIVATE lily_base)
target_include_directories(lily_cli PRIVATE ${LILY_INCLUDE})
