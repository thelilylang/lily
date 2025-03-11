# lilyc_cli
set(CLI_LILYC_SRC ${CMAKE_SOURCE_DIR}/src/cli/lilyc/lilyc.c
                  ${CMAKE_SOURCE_DIR}/src/cli/lilyc/parse_config.c)

add_library(lilyc_cli STATIC ${CLI_LILYC_SRC}
                             ${CMAKE_SOURCE_DIR}/src/ex/lib/lilyc_cli.c)
target_link_libraries(lilyc_cli PRIVATE lily_base)
target_include_directories(lilyc_cli PRIVATE ${LILY_INCLUDE})
