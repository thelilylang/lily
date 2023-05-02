add_custom_target(
    c_comptime_gen
    ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/cc/comptime_gen
    COMMAND cargo build --color=always
)