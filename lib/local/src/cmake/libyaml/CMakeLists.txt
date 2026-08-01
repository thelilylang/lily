# lily_libyaml
set(LIBYAML_SRC
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/api.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/dumper.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/emitter.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/loader.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/parser.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/reader.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/scanner.c
    ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src/writer.c)

add_library(lily_libyaml STATIC ${LIBYAML_SRC})
target_include_directories(
  lily_libyaml PRIVATE ${LILY_INCLUDE}
                       ${CMAKE_SOURCE_DIR}/lib/local/src/libyaml/src)

# Unlike glibc, Emscripten's libc keeps `strdup` behind the POSIX feature
# macros, which `-std=c23` with extensions off does not set. Everything else in
# the tree gets them from `base/env.h`, which libyaml does not include.
if(LILY_WASM)
  target_compile_definitions(lily_libyaml PRIVATE _GNU_SOURCE)
endif()
