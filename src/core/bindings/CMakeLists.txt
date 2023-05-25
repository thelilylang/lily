add_custom_target(
    bindings
    ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/lilyls
    COMMAND cargo build --color=always
)
