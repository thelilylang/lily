add_custom_target(
	lily-lsp
    ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/lily/lsp/
    COMMAND cargo build --color=always
)
