# ci
add_executable(ci ${CMAKE_SOURCE_DIR}/src/bin/ci/main.c
                  ${CMAKE_SOURCE_DIR}/src/ex/bin/ci.c)
target_link_libraries(ci PRIVATE lily_core_cc_ci ci_cli ci_command)
target_include_directories(ci PRIVATE ${LILY_INCLUDE})
