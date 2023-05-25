add_custom_target(
    lsp 
    ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/lsp
    COMMAND zig build
)