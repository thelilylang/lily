# lily_core_lily_scanner
set(LILY_CORE_LILY_SCANNER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/scanner/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/scanner/token.c)

add_library(
  lily_core_lily_scanner STATIC
  ${LILY_CORE_LILY_SCANNER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_scanner.c)
target_link_libraries(lily_core_lily_scanner PRIVATE lily_base lily_core_shared)
target_include_directories(lily_core_lily_scanner PRIVATE ${LILY_INCLUDE})
