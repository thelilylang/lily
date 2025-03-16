# lily
add_executable(lily ${CMAKE_SOURCE_DIR}/src/bin/lily/main.c
                    ${CMAKE_SOURCE_DIR}/src/ex/bin/lily.c)
target_link_libraries(lily PRIVATE lily_cli lily_command)
target_include_directories(lily PRIVATE ${LILY_INCLUDE})
