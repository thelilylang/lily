set(CIC_WASM_EXPORTED_FUNCTIONS "_free,_malloc,_run__WASMCIc")
set(CIC_WASM_EXPORTED_RUNTIME_METHODS
    "ccall,cwrap,stringToUTF8,UTF8ToString,getValue,FS")

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
  -sALLOW_MEMORY_GROWTH=1)

# Emitting `.js` is what makes emcc produce the loader next to the `.wasm`.
set_target_properties(cic_wasm PROPERTIES SUFFIX ".js")
