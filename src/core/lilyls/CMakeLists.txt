add_custom_target(
	lilyls
    ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/lilyls
    COMMAND cargo build --color=always
)
