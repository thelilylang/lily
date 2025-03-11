# lily_core_lily_preparser
set(LILY_CORE_LILY_PREPARSER_SRC
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/allow.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/arch.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/deny.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/doc.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/forbid.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/link_info.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/os.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/repr.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess/warn.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preparser.c
    ${CMAKE_SOURCE_DIR}/src/core/lily/preparser/preprocess.c)

add_library(
  lily_core_lily_preparser STATIC
  ${LILY_CORE_LILY_PREPARSER_SRC}
  ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_lily_preparser.c)
target_link_libraries(lily_core_lily_preparser PRIVATE lily_core_lily_scanner
                                                       lily_core_lily_shared)
target_include_directories(lily_core_lily_preparser PRIVATE ${LILY_INCLUDE})
