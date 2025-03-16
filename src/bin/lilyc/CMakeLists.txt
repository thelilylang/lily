# lilyc
add_executable(lilyc ${CMAKE_SOURCE_DIR}/src/bin/lilyc/main.c
                     ${CMAKE_SOURCE_DIR}/src/ex/bin/lilyc.c)
target_link_libraries(lilyc PRIVATE lilyc_cli lilyc_command ${LILY_LLD_LIBS}
                                    ${LILY_LLVM_LIBS})
target_include_directories(lilyc PRIVATE ${LILY_INCLUDE})
