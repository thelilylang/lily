set(CIC_WASM_EXPORTED_FUNCTIONS "_free,_malloc,_run__WASMCIc")
set(CIC_WASM_EXPORTED_RUNTIME_METHODS
    "ccall,cwrap,stringToUTF8,UTF8ToString,lengthBytesUTF8,getValue,setValue,FS"
)

# cic_wasm
add_executable(cic_wasm ${CMAKE_SOURCE_DIR}/src/wasm/cic/main.c)
target_link_libraries(cic_wasm PRIVATE lily_core_cc_ci cic_cli cic_command)
target_include_directories(cic_wasm PRIVATE ${LILY_INCLUDE})
target_link_options(
  cic_wasm
  PRIVATE
  -sEXPORTED_FUNCTIONS=${CIC_WASM_EXPORTED_FUNCTIONS}
  -sEXPORTED_RUNTIME_METHODS=${CIC_WASM_EXPORTED_RUNTIME_METHODS}
  # `cic` reads and writes its inputs through the standard file API, so the
  # caller has to be able to populate MEMFS before running it.
  -sFORCE_FILESYSTEM=1
  -sALLOW_MEMORY_GROWTH=1
  # A transpilation that fails calls `exit`, which leaves the runtime unusable.
  # As a factory, the caller can build a fresh instance per run instead of
  # having the first error end the session.
  -sMODULARIZE=1
  -sEXPORT_NAME=createCIc
  --preload-file
  ${CMAKE_SOURCE_DIR}/lib/cc/std/include)

# Emitting `.js` is what makes emcc produce the loader next to the `.wasm`.
set_target_properties(cic_wasm PROPERTIES SUFFIX ".js")
