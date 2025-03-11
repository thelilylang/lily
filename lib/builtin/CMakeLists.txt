# lily_builtin
set(BUILTIN_SRC
    ${CMAKE_SOURCE_DIR}/lib/builtin/alloc.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/cstr.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/math.c
    ${CMAKE_SOURCE_DIR}/lib/builtin/pointer.c)

add_library(lily_builtin SHARED ${BUILTIN_SRC}
                                ${CMAKE_SOURCE_DIR}/src/ex/lib/lily_builtin.c)
target_include_directories(lily_builtin PUBLIC ${CMAKE_SOURCE_DIR}/lib
                                               ${CMAKE_SOURCE_DIR}/include)
