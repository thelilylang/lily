# lily_core_shared
set(LILY_CORE_SHARED_SRC
    ${CMAKE_SOURCE_DIR}/src/core/shared/target/arch.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/target/os.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/cursor.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/diagnostic.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/location.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/scanner.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/search.c
    ${CMAKE_SOURCE_DIR}/src/core/shared/source.c)

add_library(
  lily_core_shared STATIC ${LILY_CORE_SHARED_SRC}
                          ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_core_shared.c)
target_link_libraries(
  lily_core_shared
  PRIVATE lily_base lily_core_lily_diagnostic lily_core_cc_diagnostic
          lily_core_cc_ci_diagnostic lily_core_cpp_diagnostic)
target_include_directories(lily_core_shared PRIVATE ${LILY_INCLUDE})
