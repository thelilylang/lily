add_custom_target(
	ci
    ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/cc/ci
    COMMAND cargo build --color=always
)
