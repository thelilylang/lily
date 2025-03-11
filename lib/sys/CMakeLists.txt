# lily_sys
set(SYS_SRC ${CMAKE_SOURCE_DIR}/lib/sys/sys.c)

add_library(lily_sys SHARED ${SYS_SRC}
                            ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_sys.c)
target_include_directories(lily_sys PUBLIC ${CMAKE_SOURCE_DIR}/lib
                                           ${CMAKE_SOURCE_DIR}/include)
