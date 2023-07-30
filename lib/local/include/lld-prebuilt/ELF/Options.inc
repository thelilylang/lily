/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Option Parsing Definitions                                                 *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

/////////
// Prefixes

#ifdef PREFIX
#define COMMA ,
PREFIX(prefix_0, {nullptr})
PREFIX(prefix_1, {"-" COMMA nullptr})
PREFIX(prefix_4, {"-" COMMA "--" COMMA nullptr})
PREFIX(prefix_3, {"--" COMMA nullptr})
PREFIX(prefix_2, {"--" COMMA "-" COMMA nullptr})
#undef COMMA
#endif // PREFIX

/////////
// Groups

#ifdef OPTION

//////////
// Options

OPTION(prefix_0, &"<input>"[0], INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, &"<unknown>"[0], UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-("[1], anonymous_33, Flag, INVALID, start_group, nullptr, 0, 0,
       "Alias for --start-group", nullptr, nullptr)
OPTION(prefix_1, &"-)"[1], anonymous_13, Flag, INVALID, end_group, nullptr, 0, 0,
       "Alias for --end-group", nullptr, nullptr)
OPTION(prefix_2, &"--allow-multiple-definition"[2], allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiple definitions", nullptr, nullptr)
OPTION(prefix_2, &"--allow-shlib-undefined"[2], allow_shlib_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow unresolved references in shared libraries (default when linking a shared library)", nullptr, nullptr)
OPTION(prefix_3, &"--android-memtag-heap"[2], android_memtag_heap, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Instruct the dynamic loader to enable MTE protection for the heap", nullptr, nullptr)
OPTION(prefix_3, &"--android-memtag-mode="[2], android_memtag_mode_eq, Joined, INVALID, android_memtag_mode, nullptr, 0, 0,
       "Instruct the dynamic loader to start under MTE mode {async, sync, none}", nullptr, nullptr)
OPTION(prefix_3, &"--android-memtag-mode"[2], android_memtag_mode, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--android-memtag-stack"[2], android_memtag_stack, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Instruct the dynamic loader to prepare for MTE stack instrumentation", nullptr, nullptr)
OPTION(prefix_3, &"--apply-dynamic-relocs"[2], apply_dynamic_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Apply link-time values for dynamic relocations", nullptr, nullptr)
OPTION(prefix_2, &"--as-needed"[2], as_needed, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Only set DT_NEEDED for shared libraries if used", nullptr, nullptr)
OPTION(prefix_2, &"--auxiliary="[2], auxiliary_eq, Joined, INVALID, auxiliary, nullptr, 0, 0,
       "Set DT_AUXILIARY field to the specified name", nullptr, nullptr)
OPTION(prefix_2, &"--auxiliary"[2], auxiliary, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--Bdynamic"[2], Bdynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Link against shared libraries (default)", nullptr, nullptr)
OPTION(prefix_2, &"--Bno-symbolic"[2], Bno_symbolic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't bind default visibility defined symbols locally for -shared (default)", nullptr, nullptr)
OPTION(prefix_2, &"--Bshareable"[2], anonymous_31, Flag, INVALID, shared, nullptr, 0, 0,
       "Alias for --shared", nullptr, nullptr)
OPTION(prefix_2, &"--Bstatic"[2], Bstatic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not link against shared libraries", nullptr, nullptr)
OPTION(prefix_2, &"--Bsymbolic-functions"[2], Bsymbolic_functions, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind default visibility defined function symbols locally for -shared", nullptr, nullptr)
OPTION(prefix_2, &"--Bsymbolic-non-weak-functions"[2], Bsymbolic_non_weak_functions, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind default visibility defined STB_GLOBAL function symbols locally for -shared", nullptr, nullptr)
OPTION(prefix_2, &"--Bsymbolic"[2], Bsymbolic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind default visibility defined symbols locally for -shared", nullptr, nullptr)
OPTION(prefix_2, &"--build-id="[2], build_id, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Generate build ID note", "[fast,md5,sha1,uuid,0x<hexstring>]", nullptr)
OPTION(prefix_2, &"--build-id"[2], anonymous_0, Flag, INVALID, build_id, "fast\0", 0, 0,
       "Alias for --build-id=fast", nullptr, nullptr)
OPTION(prefix_1, &"-b"[1], anonymous_17, Separate, INVALID, format, nullptr, 0, 0,
       "Alias for --format", nullptr, nullptr)
OPTION(prefix_2, &"--call-graph-ordering-file="[2], call_graph_ordering_file_eq, Joined, INVALID, call_graph_ordering_file, nullptr, 0, 0,
       "Layout sections to optimize the given callgraph", nullptr, nullptr)
OPTION(prefix_2, &"--call-graph-ordering-file"[2], call_graph_ordering_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--call-graph-profile-sort"[2], call_graph_profile_sort, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Reorder sections with call graph profile (default)", nullptr, nullptr)
OPTION(prefix_2, &"--call_shared"[2], anonymous_5, Flag, INVALID, Bdynamic, nullptr, 0, 0,
       "Alias for --Bdynamic", nullptr, nullptr)
OPTION(prefix_3, &"--check-dynamic-relocations"[2], check_dynamic_relocations, Flag, INVALID, INVALID, nullptr, HelpHidden, 0,
       "Perform additional validation of the written dynamic relocations", nullptr, nullptr)
OPTION(prefix_2, &"--check-sections"[2], check_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Check section addresses for overlaps (default)", nullptr, nullptr)
OPTION(prefix_3, &"--chroot"[2], chroot, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--color-diagnostics="[2], color_diagnostics, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_3, &"--color-diagnostics"[2], anonymous_1, Flag, INVALID, color_diagnostics, "always\0", 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_2, &"--compress-debug-sections="[2], compress_debug_sections_eq, Joined, INVALID, compress_debug_sections, nullptr, 0, 0,
       "Compress DWARF debug sections", "[none,zlib]", nullptr)
OPTION(prefix_2, &"--compress-debug-sections"[2], compress_debug_sections, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[none,zlib]", nullptr)
OPTION(prefix_3, &"--cref"[2], cref, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Output cross reference table. If -Map is specified, print to the map file", nullptr, nullptr)
OPTION(prefix_2, &"--defsym="[2], defsym_eq, Joined, INVALID, defsym, nullptr, 0, 0,
       "Define a symbol alias", "<symbol>=<value>", nullptr)
OPTION(prefix_2, &"--defsym"[2], defsym, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>=<value>", nullptr)
OPTION(prefix_2, &"--demangle"[2], demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names (default)", nullptr, nullptr)
OPTION(prefix_3, &"--dependency-file="[2], dependency_file_eq, Joined, INVALID, dependency_file, nullptr, 0, 0,
       "Write a dependency file", "<file>", nullptr)
OPTION(prefix_3, &"--dependency-file"[2], dependency_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<file>", nullptr)
OPTION(prefix_3, &"--dependent-libraries"[2], dependent_libraries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Process dependent library specifiers from input files (default)", nullptr, nullptr)
OPTION(prefix_2, &"--detect-odr-violations"[2], anonymous_65, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--disable-new-dtags"[2], disable_new_dtags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable new dynamic tags", nullptr, nullptr)
OPTION(prefix_2, &"--disable-verify"[2], disable_verify, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--discard-all"[2], discard_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Delete all local symbols", nullptr, nullptr)
OPTION(prefix_2, &"--discard-locals"[2], discard_locals, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Delete temporary local symbols", nullptr, nullptr)
OPTION(prefix_2, &"--discard-none"[2], discard_none, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Keep all symbols in the symbol table", nullptr, nullptr)
OPTION(prefix_2, &"--dn"[2], anonymous_7, Flag, INVALID, Bstatic, nullptr, 0, 0,
       "Alias for --Bstatic", nullptr, nullptr)
OPTION(prefix_2, &"--dynamic-linker="[2], dynamic_linker_eq, Joined, INVALID, dynamic_linker, nullptr, 0, 0,
       "Which dynamic linker to use", nullptr, nullptr)
OPTION(prefix_2, &"--dynamic-linker"[2], dynamic_linker, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--dynamic-list="[2], dynamic_list_eq, Joined, INVALID, dynamic_list, nullptr, 0, 0,
       "Similar to --export-dynamic-symbol-list. When creating a shared object, this additionally implies -Bsymbolic but does not set DF_SYMBOLIC", "<file>", nullptr)
OPTION(prefix_2, &"--dynamic-list"[2], dynamic_list, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<file>", nullptr)
OPTION(prefix_2, &"--dy"[2], anonymous_6, Flag, INVALID, Bdynamic, nullptr, 0, 0,
       "Alias for --Bdynamic", nullptr, nullptr)
OPTION(prefix_1, &"-d"[1], anonymous_66, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--EB"[2], eb, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Select the big-endian format in OUTPUT_FORMAT", nullptr, nullptr)
OPTION(prefix_2, &"--eh-frame-hdr"[2], eh_frame_hdr, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Request creation of .eh_frame_hdr section and PT_GNU_EH_FRAME segment header", nullptr, nullptr)
OPTION(prefix_2, &"--EL"[2], el, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Select the little-endian format in OUTPUT_FORMAT", nullptr, nullptr)
OPTION(prefix_2, &"--emit-relocs"[2], emit_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate relocations in output", nullptr, nullptr)
OPTION(prefix_2, &"--enable-new-dtags"[2], enable_new_dtags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable new dynamic tags (default)", nullptr, nullptr)
OPTION(prefix_2, &"--end-group"[2], end_group, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignored for compatibility with GNU unless you pass --warn-backrefs", nullptr, nullptr)
OPTION(prefix_2, &"--end-lib"[2], end_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "End a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_2, &"--entry="[2], entry_eq, Joined, INVALID, entry, nullptr, 0, 0,
       "Name of entry point symbol", "<entry>", nullptr)
OPTION(prefix_2, &"--entry"[2], entry, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<entry>", nullptr)
OPTION(prefix_3, &"--error-handling-script="[2], error_handling_script_eq, Joined, INVALID, error_handling_script, nullptr, 0, 0,
       "Specify an error handling script", nullptr, nullptr)
OPTION(prefix_3, &"--error-handling-script"[2], error_handling_script, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--error-limit="[2], error_limit_eq, Joined, INVALID, error_limit, nullptr, 0, 0,
       "Maximum number of errors to emit before stopping (0 = no limit)", nullptr, nullptr)
OPTION(prefix_3, &"--error-limit"[2], error_limit, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--error-unresolved-symbols"[2], error_unresolved_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols as errors", nullptr, nullptr)
OPTION(prefix_2, &"--exclude-libs="[2], exclude_libs_eq, Joined, INVALID, exclude_libs, nullptr, 0, 0,
       "Exclude static libraries from automatic export", nullptr, nullptr)
OPTION(prefix_2, &"--exclude-libs"[2], exclude_libs, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--execute-only"[2], execute_only, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mark executable sections unreadable", nullptr, nullptr)
OPTION(prefix_3, &"--export-dynamic-symbol-list="[2], export_dynamic_symbol_list_eq, Joined, INVALID, export_dynamic_symbol_list, nullptr, 0, 0,
       "Read a list of dynamic symbol patterns. Apply --export-dynamic-symbol on each pattern", "file", nullptr)
OPTION(prefix_3, &"--export-dynamic-symbol-list"[2], export_dynamic_symbol_list, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "file", nullptr)
OPTION(prefix_3, &"--export-dynamic-symbol="[2], export_dynamic_symbol_eq, Joined, INVALID, export_dynamic_symbol, nullptr, 0, 0,
       "(executable) Put matched symbols in the dynamic symbol table. (shared object) References to matched non-local STV_DEFAULT symbols shouldn't be bound to definitions within the shared object. Does not imply -Bsymbolic.", "glob", nullptr)
OPTION(prefix_3, &"--export-dynamic-symbol"[2], export_dynamic_symbol, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "glob", nullptr)
OPTION(prefix_2, &"--export-dynamic"[2], export_dynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Put symbols in the dynamic symbol table", nullptr, nullptr)
OPTION(prefix_1, &"-E"[1], anonymous_15, Flag, INVALID, export_dynamic, nullptr, 0, 0,
       "Alias for --export-dynamic", nullptr, nullptr)
OPTION(prefix_1, &"-e"[1], anonymous_14, JoinedOrSeparate, INVALID, entry, nullptr, 0, 0,
       "Alias for --entry", nullptr, nullptr)
OPTION(prefix_2, &"--fatal-warnings"[2], fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_2, &"--filter="[2], filter_eq, Joined, INVALID, filter, nullptr, 0, 0,
       "Set DT_FILTER field to the specified name", nullptr, nullptr)
OPTION(prefix_2, &"--filter"[2], filter, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--fini="[2], fini_eq, Joined, INVALID, fini, nullptr, 0, 0,
       "Specify a finalizer function", "<symbol>", nullptr)
OPTION(prefix_2, &"--fini"[2], fini, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, &"--fix-cortex-a53-843419"[2], fix_cortex_a53_843419, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Apply fixes for AArch64 Cortex-A53 erratum 843419", nullptr, nullptr)
OPTION(prefix_2, &"--fix-cortex-a8"[2], fix_cortex_a8, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Apply fixes for ARM Cortex-A8 erratum 657417", nullptr, nullptr)
OPTION(prefix_2, &"--format="[2], format_eq, Joined, INVALID, format, nullptr, 0, 0,
       "Change the input format of the inputs following this option", "[default,elf,binary]", nullptr)
OPTION(prefix_2, &"--format"[2], format, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[default,elf,binary]", nullptr)
OPTION(prefix_3, &"--fortran-common"[2], fortran_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Search archive members for definitions to override COMMON symbols (default)", nullptr, nullptr)
OPTION(prefix_1, &"-F"[1], anonymous_16, Separate, INVALID, filter, nullptr, 0, 0,
       "Alias for --filter", nullptr, nullptr)
OPTION(prefix_1, &"-f"[1], anonymous_4, Separate, INVALID, auxiliary, nullptr, 0, 0,
       "Alias for --auxiliary", nullptr, nullptr)
OPTION(prefix_2, &"--gc-sections"[2], gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable garbage collection of unused sections", nullptr, nullptr)
OPTION(prefix_3, &"--gdb-index"[2], gdb_index, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate .gdb_index section", nullptr, nullptr)
OPTION(prefix_3, &"--gnu-unique"[2], gnu_unique, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable STB_GNU_UNIQUE symbol binding (default)", nullptr, nullptr)
OPTION(prefix_1, &"-G"[1], anonymous_82, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-g"[1], anonymous_67, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--hash-style="[2], hash_style_eq, Joined, INVALID, hash_style, nullptr, 0, 0,
       "Specify hash style (sysv, gnu or both)", nullptr, nullptr)
OPTION(prefix_2, &"--hash-style"[2], hash_style, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--help"[2], help, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print option help", nullptr, nullptr)
OPTION(prefix_1, &"-h"[1], anonymous_32, JoinedOrSeparate, INVALID, soname, nullptr, 0, 0,
       "Alias for --soname", nullptr, nullptr)
OPTION(prefix_2, &"--icf=all"[2], icf_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable identical code folding", nullptr, nullptr)
OPTION(prefix_2, &"--icf=none"[2], icf_none, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable identical code folding (default)", nullptr, nullptr)
OPTION(prefix_2, &"--icf=safe"[2], icf_safe, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable safe identical code folding", nullptr, nullptr)
OPTION(prefix_3, &"--ignore-data-address-equality"[2], ignore_data_address_equality, Flag, INVALID, INVALID, nullptr, 0, 0,
       "lld can break the address equality of data", nullptr, nullptr)
OPTION(prefix_3, &"--ignore-function-address-equality"[2], ignore_function_address_equality, Flag, INVALID, INVALID, nullptr, 0, 0,
       "lld can break the address equality of functions", nullptr, nullptr)
OPTION(prefix_3, &"--image-base="[2], image_base_eq, Joined, INVALID, image_base, nullptr, 0, 0,
       "Set the base address", nullptr, nullptr)
OPTION(prefix_3, &"--image-base"[2], image_base, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--init="[2], init_eq, Joined, INVALID, init, nullptr, 0, 0,
       "Specify an initializer function", "<symbol>", nullptr)
OPTION(prefix_2, &"--init"[2], init, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, &"--just-symbols="[2], just_symbols_eq, Joined, INVALID, just_symbols, nullptr, 0, 0,
       "Just link symbols", nullptr, nullptr)
OPTION(prefix_2, &"--just-symbols"[2], just_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--keep-unique="[2], keep_unique_eq, Joined, INVALID, keep_unique, nullptr, 0, 0,
       "Do not fold this symbol during ICF", nullptr, nullptr)
OPTION(prefix_2, &"--keep-unique"[2], keep_unique, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--library-path="[2], anonymous_21, Joined, INVALID, library_path, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--library-path"[2], anonymous_20, Separate, INVALID, library_path, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--library="[2], anonymous_19, Joined, INVALID, library, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--library"[2], anonymous_18, Separate, INVALID, library, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--load-pass-plugin="[2], load_pass_plugins_eq, Joined, INVALID, load_pass_plugins, nullptr, 0, 0,
       "Load passes from plugin library", nullptr, nullptr)
OPTION(prefix_3, &"--load-pass-plugin"[2], load_pass_plugins, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--long-plt"[2], anonymous_68, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--lto-aa-pipeline="[2], lto_aa_pipeline, Joined, INVALID, INVALID, nullptr, 0, 0,
       "AA pipeline to run during LTO. Used in conjunction with -lto-newpm-passes", nullptr, nullptr)
OPTION(prefix_3, &"--lto-basic-block-sections="[2], lto_basic_block_sections, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Enable basic block sections for LTO", nullptr, nullptr)
OPTION(prefix_3, &"--lto-cs-profile-file="[2], lto_cs_profile_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Context sensitive profile file path", nullptr, nullptr)
OPTION(prefix_3, &"--lto-cs-profile-generate"[2], lto_cs_profile_generate, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Perform context sensitive PGO instrumentation", nullptr, nullptr)
OPTION(prefix_3, &"--lto-debug-pass-manager"[2], lto_debug_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Debug new pass manager", nullptr, nullptr)
OPTION(prefix_3, &"--lto-emit-asm"[2], lto_emit_asm, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Emit assembly code", nullptr, nullptr)
OPTION(prefix_3, &"--lto-newpm-passes="[2], lto_newpm_passes, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Passes to run during LTO", nullptr, nullptr)
OPTION(prefix_3, &"--lto-obj-path="[2], lto_obj_path_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--lto-O"[2], lto_O, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Optimization level for LTO", "<opt-level>", nullptr)
OPTION(prefix_3, &"--lto-partitions="[2], lto_partitions, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of LTO codegen partitions", nullptr, nullptr)
OPTION(prefix_3, &"--lto-pgo-warn-mismatch"[2], lto_pgo_warn_mismatch, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn on warnings about profile cfg mismatch (default)>", nullptr, nullptr)
OPTION(prefix_3, &"--lto-sample-profile="[2], lto_sample_profile, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Sample profile file path", nullptr, nullptr)
OPTION(prefix_3, &"--lto-unique-basic-block-section-names"[2], lto_unique_basic_block_section_names, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Give unique names to every basic block section for LTO", nullptr, nullptr)
OPTION(prefix_3, &"--lto-whole-program-visibility"[2], lto_whole_program_visibility, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Asserts that the LTO link has whole program visibility", nullptr, nullptr)
OPTION(prefix_1, &"-L"[1], library_path, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Add <dir> to the library search path", "<dir>", nullptr)
OPTION(prefix_1, &"-l"[1], library, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Search for library <libname>", "<libname>", nullptr)
OPTION(prefix_2, &"--Map="[2], Map_eq, Joined, INVALID, Map, nullptr, 0, 0,
       "Print a link map to the specified file", nullptr, nullptr)
OPTION(prefix_2, &"--Map"[2], Map, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--merge-exidx-entries"[2], merge_exidx_entries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable merging .ARM.exidx entries (default)", nullptr, nullptr)
OPTION(prefix_2, &"--mips-got-size="[2], mips_got_size_eq, Joined, INVALID, mips_got_size, nullptr, HelpHidden, 0,
       "Max size of a single MIPS GOT. 0x10000 by default.", nullptr, nullptr)
OPTION(prefix_2, &"--mips-got-size"[2], mips_got_size, Separate, INVALID, INVALID, nullptr, HelpHidden, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--mllvm="[2], mllvm_eq, Joined, INVALID, mllvm, nullptr, 0, 0,
       "Additional arguments to forward to LLVM's option processing", nullptr, nullptr)
OPTION(prefix_2, &"--mllvm"[2], mllvm, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--mmap-output-file"[2], mmap_output_file, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mmap the output file for writing (default)", nullptr, nullptr)
OPTION(prefix_1, &"-M"[1], anonymous_27, Flag, INVALID, print_map, nullptr, 0, 0,
       "Alias for --print-map", nullptr, nullptr)
OPTION(prefix_1, &"-m"[1], m, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Set target emulation", nullptr, nullptr)
OPTION(prefix_2, &"--nmagic"[2], nmagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not page align sections, link against static libraries.", "<magic>", nullptr)
OPTION(prefix_3, &"--no-add-needed"[2], anonymous_69, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--no-allow-multiple-definition"[2], no_allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not allow multiple definitions (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-allow-shlib-undefined"[2], no_allow_shlib_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not allow unresolved references in shared libraries (default when linking an executable)", nullptr, nullptr)
OPTION(prefix_3, &"--no-android-memtag-heap"[2], no_android_memtag_heap, Flag, INVALID, INVALID, nullptr, 0, 0,
       "", nullptr, nullptr)
OPTION(prefix_3, &"--no-android-memtag-stack"[2], no_android_memtag_stack, Flag, INVALID, INVALID, nullptr, 0, 0,
       "", nullptr, nullptr)
OPTION(prefix_3, &"--no-apply-dynamic-relocs"[2], no_apply_dynamic_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not apply link-time values for dynamic relocations (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-as-needed"[2], no_as_needed, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Always set DT_NEEDED for shared libraries (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-call-graph-profile-sort"[2], no_call_graph_profile_sort, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not reorder sections with call graph profile", nullptr, nullptr)
OPTION(prefix_3, &"--no-check-dynamic-relocations"[2], no_check_dynamic_relocations, Flag, INVALID, INVALID, nullptr, HelpHidden, 0,
       "Do not perform additional validation of the written dynamic relocations", nullptr, nullptr)
OPTION(prefix_2, &"--no-check-sections"[2], no_check_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not check section addresses for overlaps", nullptr, nullptr)
OPTION(prefix_3, &"--no-color-diagnostics"[2], anonymous_2, Flag, INVALID, color_diagnostics, "never\0", 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_2, &"--no-copy-dt-needed-entries"[2], anonymous_70, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--no-ctors-in-init-array"[2], anonymous_71, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--no-demangle"[2], no_demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbol names", nullptr, nullptr)
OPTION(prefix_3, &"--no-dependent-libraries"[2], no_dependent_libraries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignore dependent library specifiers from input files", nullptr, nullptr)
OPTION(prefix_2, &"--no-dynamic-linker"[2], no_dynamic_linker, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Inhibit output of .interp section", nullptr, nullptr)
OPTION(prefix_2, &"--no-eh-frame-hdr"[2], no_eh_frame_hdr, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not create .eh_frame_hdr section", nullptr, nullptr)
OPTION(prefix_3, &"--no-execute-only"[2], no_execute_only, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mark executable sections readable (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-export-dynamic"[2], no_export_dynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not put symbols in the dynamic symbol table (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-fatal-warnings"[2], no_fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not treat warnings as errors (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-fortran-common"[2], no_fortran_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not search archive members for definitions to override COMMON symbols", nullptr, nullptr)
OPTION(prefix_2, &"--no-gc-sections"[2], no_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable garbage collection of unused sections (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-gdb-index"[2], no_gdb_index, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not generate .gdb_index section (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-gnu-unique"[2], no_gnu_unique, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable STB_GNU_UNIQUE symbol binding", nullptr, nullptr)
OPTION(prefix_2, &"--no-keep-memory"[2], anonymous_72, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--no-lto-legacy-pass-manager"[2], no_lto_legacy_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use the new pass manager in LLVM", nullptr, nullptr)
OPTION(prefix_3, &"--no-lto-pgo-warn-mismatch"[2], no_lto_pgo_warn_mismatch, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn off warnings about profile cfg mismatch", nullptr, nullptr)
OPTION(prefix_3, &"--no-lto-unique-basic-block-section-names"[2], no_lto_unique_basic_block_section_names, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not give unique names to every basic block section for LTO (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-lto-whole-program-visibility"[2], no_lto_whole_program_visibility, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Asserts that the LTO link does not have whole program visibility", nullptr, nullptr)
OPTION(prefix_2, &"--no-merge-exidx-entries"[2], no_merge_exidx_entries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable merging .ARM.exidx entries", nullptr, nullptr)
OPTION(prefix_3, &"--no-mmap-output-file"[2], no_mmap_output_file, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not mmap the output file for writing", nullptr, nullptr)
OPTION(prefix_2, &"--no-nmagic"[2], no_nmagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Page align sections (default)", "<magic>", nullptr)
OPTION(prefix_2, &"--no-omagic"[2], no_omagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not set the text data sections to be writable, page align sections (default)", "<magic>", nullptr)
OPTION(prefix_3, &"--no-optimize-bb-jumps"[2], no_optimize_bb_jumps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not remove any direct jumps at the end to the next basic block (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-pcrel-optimize"[2], no_pcrel_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Disable PC-relative optimizations", nullptr, nullptr)
OPTION(prefix_2, &"--no-pie"[2], no_pie, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not create a position independent executable (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-power10-stubs"[2], no_power10_stubs, Flag, INVALID, power10_stubs_eq, "no\0", 0, 0,
       "Alias for --power10-stubs=no", nullptr, nullptr)
OPTION(prefix_2, &"--no-print-gc-sections"[2], no_print_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not list removed unused sections (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-print-icf-sections"[2], no_print_icf_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not list identical folded sections (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-relax"[2], no_relax, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable target-specific relaxations", nullptr, nullptr)
OPTION(prefix_3, &"--no-rosegment"[2], no_rosegment, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not put read-only non-executable sections in their own segment", nullptr, nullptr)
OPTION(prefix_3, &"--no-toc-optimize"[2], no_toc_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Disable TOC related optimizations", nullptr, nullptr)
OPTION(prefix_2, &"--no-undefined-version"[2], no_undefined_version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report version scripts that refer undefined symbols", nullptr, nullptr)
OPTION(prefix_2, &"--no-undefined"[2], no_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols even if the linker is creating a shared library", nullptr, nullptr)
OPTION(prefix_3, &"--no-use-android-relr-tags"[2], no_use_android_relr_tags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use SHT_RELR / DT_RELR* tags (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-warn-backrefs"[2], no_warn_backrefs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about backward symbol references to extract archive members (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-warn-common"[2], no_warn_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about duplicate common symbols (default)", nullptr, nullptr)
OPTION(prefix_3, &"--no-warn-ifunc-textrel"[2], no_warn_ifunc_textrel, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about using ifunc symbols with text relocations (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-warn-mismatch"[2], anonymous_73, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--no-warn-symbol-ordering"[2], no_warn_symbol_ordering, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about problems with the symbol ordering file", nullptr, nullptr)
OPTION(prefix_2, &"--no-whole-archive"[2], no_whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not force load of all members in a static library (default)", nullptr, nullptr)
OPTION(prefix_2, &"--noinhibit-exec"[2], noinhibit_exec, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Retain the executable output file whenever it is still usable", nullptr, nullptr)
OPTION(prefix_2, &"--non_shared"[2], anonymous_8, Flag, INVALID, Bstatic, nullptr, 0, 0,
       "Alias for --Bstatic", nullptr, nullptr)
OPTION(prefix_2, &"--nostdlib"[2], nostdlib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Only search directories specified on the command line", nullptr, nullptr)
OPTION(prefix_1, &"-N"[1], anonymous_23, Flag, INVALID, omagic, nullptr, 0, 0,
       "Alias for --omagic", nullptr, nullptr)
OPTION(prefix_1, &"-n"[1], anonymous_22, Flag, INVALID, nmagic, nullptr, 0, 0,
       "Alias for --nmagic", nullptr, nullptr)
OPTION(prefix_3, &"--oformat="[2], oformat_eq, Joined, INVALID, oformat, nullptr, 0, 0,
       "Specify the binary format for the output object file", "[elf,binary]", nullptr)
OPTION(prefix_3, &"--oformat"[2], oformat, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[elf,binary]", nullptr)
OPTION(prefix_3, &"--omagic"[2], omagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the text and data sections to be readable and writable, do not page align sections, link against static libraries", "<magic>", nullptr)
OPTION(prefix_3, &"--opt-remarks-filename"[2], opt_remarks_filename, Separate, INVALID, INVALID, nullptr, 0, 0,
       "YAML output file for optimization remarks", nullptr, nullptr)
OPTION(prefix_3, &"--opt-remarks-format"[2], opt_remarks_format, Separate, INVALID, INVALID, nullptr, 0, 0,
       "The format used for serializing remarks (default: YAML)", nullptr, nullptr)
OPTION(prefix_3, &"--opt-remarks-hotness-threshold="[2], opt_remarks_hotness_threshold_eq, Joined, INVALID, opt_remarks_hotness_threshold, nullptr, 0, 0,
       "Minimum profile count required for an optimization remark to be output. Use 'auto' to apply the threshold from profile summary.", "<value>", nullptr)
OPTION(prefix_3, &"--opt-remarks-hotness-threshold"[2], opt_remarks_hotness_threshold, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<value>", nullptr)
OPTION(prefix_3, &"--opt-remarks-passes"[2], opt_remarks_passes, Separate, INVALID, INVALID, nullptr, 0, 0,
       "Regex for the passes that need to be serialized to the output file", nullptr, nullptr)
OPTION(prefix_3, &"--opt-remarks-with-hotness"[2], opt_remarks_with_hotness, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include hotness information in the optimization remarks file", nullptr, nullptr)
OPTION(prefix_3, &"--optimize-bb-jumps"[2], optimize_bb_jumps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove direct jumps at the end to the next basic block", nullptr, nullptr)
OPTION(prefix_2, &"--orphan-handling="[2], orphan_handling_eq, Joined, INVALID, orphan_handling, nullptr, 0, 0,
       "Control how orphan sections are handled when linker script used", nullptr, nullptr)
OPTION(prefix_2, &"--orphan-handling"[2], orphan_handling, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--output="[2], anonymous_24, Joined, INVALID, o, nullptr, 0, 0,
       "Alias for -o", nullptr, nullptr)
OPTION(prefix_3, &"--output"[2], anonymous_25, Separate, INVALID, o, nullptr, 0, 0,
       "Alias for -o", nullptr, nullptr)
OPTION(prefix_1, &"-O"[1], O, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Optimize output file size", nullptr, nullptr)
OPTION(prefix_1, &"-o"[1], o, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", "<path>", nullptr)
OPTION(prefix_3, &"--pack-dyn-relocs="[2], pack_dyn_relocs_eq, Joined, INVALID, pack_dyn_relocs, nullptr, 0, 0,
       "Pack dynamic relocations in the given format", "[none,android,relr,android+relr]", nullptr)
OPTION(prefix_3, &"--pack-dyn-relocs"[2], pack_dyn_relocs, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[none,android,relr,android+relr]", nullptr)
OPTION(prefix_3, &"--package-metadata="[2], package_metadata, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Emit package metadata note", nullptr, nullptr)
OPTION(prefix_3, &"--pcrel-optimize"[2], pcrel_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Enable PC-relative optimizations (default)", nullptr, nullptr)
OPTION(prefix_2, &"--pic-executable"[2], anonymous_26, Flag, INVALID, pie, nullptr, 0, 0,
       "Alias for --pie", nullptr, nullptr)
OPTION(prefix_2, &"--pic-veneer"[2], pic_veneer, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Always generate position independent thunks (veneers)", nullptr, nullptr)
OPTION(prefix_2, &"--pie"[2], pie, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create a position independent executable", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=-fresolution="[2], anonymous_63, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=-pass-through="[2], anonymous_64, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=-"[2], plugin_opt_eq_minus, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify an LLVM option for compatibility with LLVMgold.so", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=cs-profile-generate"[2], anonymous_47, Flag, INVALID, lto_cs_profile_generate, nullptr, 0, 0,
       "Alias for --lto-cs-profile-generate", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=cs-profile-path="[2], anonymous_48, Joined, INVALID, lto_cs_profile_file, nullptr, 0, 0,
       "Alias for --lto-cs-profile-file", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=debug-pass-manager"[2], anonymous_42, Flag, INVALID, lto_debug_pass_manager, nullptr, 0, 0,
       "Alias for --lto-debug-pass-manager", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=disable-verify"[2], anonymous_43, Flag, INVALID, disable_verify, nullptr, 0, 0,
       "Alias for --disable-verify", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=dwo_dir="[2], plugin_opt_dwo_dir_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Directory to store .dwo files when LTO and debug fission are used", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=emit-asm"[2], plugin_opt_emit_asm, Flag, INVALID, lto_emit_asm, nullptr, 0, 0,
       "Alias for --lto-emit-asm", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=emit-llvm"[2], plugin_opt_emit_llvm, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=jobs="[2], anonymous_44, Joined, INVALID, thinlto_jobs, nullptr, 0, 0,
       "Alias for --thinlto-jobs", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=lto-partitions="[2], anonymous_45, Joined, INVALID, lto_partitions, nullptr, 0, 0,
       "Alias for --lto-partitions", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=mcpu="[2], plugin_opt_mcpu_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=new-pass-manager"[2], anonymous_46, Flag, INVALID, no_lto_legacy_pass_manager, nullptr, 0, 0,
       "Alias for --no-lto-legacy-pass-manager", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=no-opaque-pointers"[2], plugin_opt_no_opaque_pointers, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use typed pointers in IR during LTO", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=obj-path="[2], anonymous_49, Joined, INVALID, lto_obj_path_eq, nullptr, 0, 0,
       "Alias for --lto-obj-path=", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=opaque-pointers"[2], plugin_opt_opaque_pointers, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use opaque pointers in IR during LTO (default)", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=opt-remarks-filename="[2], anonymous_50, Joined, INVALID, opt_remarks_filename, nullptr, 0, 0,
       "Alias for --opt-remarks-filename", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=opt-remarks-format="[2], anonymous_52, Joined, INVALID, opt_remarks_format, nullptr, 0, 0,
       "Alias for --opt-remarks-format", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=opt-remarks-hotness-threshold="[2], anonymous_54, Joined, INVALID, opt_remarks_hotness_threshold, nullptr, 0, 0,
       "Alias for --opt-remarks-hotness-threshold", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=opt-remarks-passes="[2], anonymous_51, Joined, INVALID, opt_remarks_passes, nullptr, 0, 0,
       "Alias for --opt-remarks-passes", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=opt-remarks-with-hotness"[2], anonymous_53, Flag, INVALID, opt_remarks_with_hotness, nullptr, 0, 0,
       "Alias for --opt-remarks-with_hotness", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=O"[2], anonymous_41, Joined, INVALID, lto_O, nullptr, 0, 0,
       "Alias for --lto-O", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=sample-profile="[2], anonymous_55, Joined, INVALID, lto_sample_profile, nullptr, 0, 0,
       "Alias for --lto-sample-profile", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=save-temps"[2], anonymous_56, Flag, INVALID, save_temps, nullptr, 0, 0,
       "Alias for --save-temps", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=stats-file="[2], plugin_opt_stats_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Filename to write LTO statistics to", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=thinlto-emit-imports-files"[2], anonymous_57, Flag, INVALID, thinlto_emit_imports_files, nullptr, 0, 0,
       "Alias for --thinlto-emit-imports-files", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=thinlto-index-only="[2], anonymous_59, Joined, INVALID, thinlto_index_only_eq, nullptr, 0, 0,
       "Alias for --thinlto-index-only=", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=thinlto-index-only"[2], anonymous_58, Flag, INVALID, thinlto_index_only, nullptr, 0, 0,
       "Alias for --thinlto-index-only", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=thinlto-object-suffix-replace="[2], anonymous_60, Joined, INVALID, thinlto_object_suffix_replace_eq, nullptr, 0, 0,
       "Alias for --thinlto-object-suffix-replace=", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=thinlto-prefix-replace="[2], anonymous_61, Joined, INVALID, thinlto_prefix_replace_eq, nullptr, 0, 0,
       "Alias for --thinlto-prefix-replace=", nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt=thinlto"[2], anonymous_62, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt="[2], plugin_opt_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin="[2], plugin_eq, Joined, INVALID, plugin, nullptr, 0, 0,
       "Ignored for compatibility with GNU linkers", nullptr, nullptr)
OPTION(prefix_2, &"--plugin"[2], plugin, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--pop-state"[2], pop_state, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Restore the states saved by --push-state", nullptr, nullptr)
OPTION(prefix_3, &"--power10-stubs="[2], power10_stubs_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Whether to use Power10 instructions in call stubs for R_PPC64_REL24_NOTOC and TOC/NOTOC interworking (yes (default): use; no: don't use). \"auto\" is currently the same as \"yes\"", "<mode>", nullptr)
OPTION(prefix_3, &"--power10-stubs"[2], power10_stubs, Flag, INVALID, power10_stubs_eq, "yes\0", 0, 0,
       "Alias for --power10-stubs=auto", nullptr, nullptr)
OPTION(prefix_2, &"--print-archive-stats="[2], print_archive_stats, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Write archive usage statistics to the specified file. Print the numbers of members and extracted members for each archive", nullptr, nullptr)
OPTION(prefix_2, &"--print-gc-sections"[2], print_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "List removed unused sections", nullptr, nullptr)
OPTION(prefix_2, &"--print-icf-sections"[2], print_icf_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "List identical folded sections", nullptr, nullptr)
OPTION(prefix_2, &"--print-map"[2], print_map, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print a link map to the standard output", nullptr, nullptr)
OPTION(prefix_2, &"--print-symbol-order="[2], print_symbol_order_eq, Joined, INVALID, print_symbol_order, nullptr, 0, 0,
       "Print a symbol order specified by --call-graph-ordering-file into the specified file", nullptr, nullptr)
OPTION(prefix_2, &"--print-symbol-order"[2], print_symbol_order, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--push-state"[2], push_state, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save the current state of --as-needed, -static and --whole-archive", nullptr, nullptr)
OPTION(prefix_1, &"-q"[1], anonymous_12, Flag, INVALID, emit_relocs, nullptr, 0, 0,
       "Alias for --emit-relocs", nullptr, nullptr)
OPTION(prefix_3, &"--relax"[2], relax, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable target-specific relaxations if supported (default)", nullptr, nullptr)
OPTION(prefix_2, &"--relocatable"[2], relocatable, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create relocatable object file", nullptr, nullptr)
OPTION(prefix_3, &"--reproduce="[2], reproduce_eq, Joined, INVALID, reproduce, nullptr, 0, 0,
       "Write tar file containing inputs and command to reproduce link", nullptr, nullptr)
OPTION(prefix_3, &"--reproduce"[2], reproduce, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--retain-symbols-file="[2], retain_symbols_file_eq, Joined, INVALID, retain_symbols_file, nullptr, 0, 0,
       "Retain only the symbols listed in the file", "<file>", nullptr)
OPTION(prefix_2, &"--retain-symbols-file"[2], retain_symbols_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<file>", nullptr)
OPTION(prefix_3, &"--rosegment"[2], rosegment, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Put read-only non-executable sections in their own segment (default)", nullptr, nullptr)
OPTION(prefix_2, &"--rpath-link="[2], anonymous_75, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--rpath-link"[2], anonymous_74, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--rpath="[2], rpath_eq, Joined, INVALID, rpath, nullptr, 0, 0,
       "Add a DT_RUNPATH to the output", nullptr, nullptr)
OPTION(prefix_2, &"--rpath"[2], rpath, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--rsp-quoting="[2], rsp_quoting_eq, Joined, INVALID, rsp_quoting, nullptr, 0, 0,
       "Quoting style for response files", "[posix,windows]", nullptr)
OPTION(prefix_3, &"--rsp-quoting"[2], rsp_quoting, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[posix,windows]", nullptr)
OPTION(prefix_1, &"-R"[1], anonymous_29, JoinedOrSeparate, INVALID, rpath, nullptr, 0, 0,
       "Alias for --rpath", nullptr, nullptr)
OPTION(prefix_1, &"-r"[1], anonymous_28, Flag, INVALID, relocatable, nullptr, 0, 0,
       "Alias for --relocatable", nullptr, nullptr)
OPTION(prefix_3, &"--save-temps="[2], save_temps_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Save select intermediate LTO compilation results", nullptr, "resolution,preopt,promote,internalize,import,opt,precodegen,prelink,combinedindex")
OPTION(prefix_2, &"--save-temps"[2], save_temps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_2, &"--script="[2], script_eq, Joined, INVALID, script, nullptr, 0, 0,
       "Read linker script", nullptr, nullptr)
OPTION(prefix_2, &"--script"[2], script, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--section-start="[2], section_start_eq, Joined, INVALID, section_start, nullptr, 0, 0,
       "Set address of section", "<address>", nullptr)
OPTION(prefix_2, &"--section-start"[2], section_start, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<address>", nullptr)
OPTION(prefix_2, &"--secure-plt"[2], anonymous_76, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--shared"[2], shared, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Build a shared object", nullptr, nullptr)
OPTION(prefix_3, &"--shuffle-sections="[2], shuffle_sections_eq, Joined, INVALID, shuffle_sections, nullptr, 0, 0,
       "Shuffle matched sections using the given seed before mapping them to the output sections. If -1, reverse the section order. If 0, use a random seed", "<section-glob>=<seed>", nullptr)
OPTION(prefix_3, &"--shuffle-sections"[2], shuffle_sections, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<section-glob>=<seed>", nullptr)
OPTION(prefix_2, &"--soname="[2], soname_eq, Joined, INVALID, soname, nullptr, 0, 0,
       "Set DT_SONAME", nullptr, nullptr)
OPTION(prefix_2, &"--soname"[2], soname, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--sort-common"[2], anonymous_77, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--sort-section="[2], sort_section_eq, Joined, INVALID, sort_section, nullptr, 0, 0,
       "Specifies sections sorting rule when linkerscript is used", nullptr, nullptr)
OPTION(prefix_2, &"--sort-section"[2], sort_section, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--split-stack-adjust-size="[2], split_stack_adjust_size_eq, Joined, INVALID, split_stack_adjust_size, nullptr, 0, 0,
       "Specify adjustment to stack size when a split-stack function calls a non-split-stack function", "<value>", nullptr)
OPTION(prefix_2, &"--split-stack-adjust-size"[2], split_stack_adjust_size, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<value>", nullptr)
OPTION(prefix_2, &"--start-group"[2], start_group, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignored for compatibility with GNU unless you pass --warn-backrefs", nullptr, nullptr)
OPTION(prefix_2, &"--start-lib"[2], start_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Start a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_2, &"--static"[2], anonymous_9, Flag, INVALID, Bstatic, nullptr, 0, 0,
       "Alias for --Bstatic", nullptr, nullptr)
OPTION(prefix_2, &"--stats"[2], anonymous_78, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--strip-all"[2], strip_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Strip all symbols. Implies --strip-debug", nullptr, nullptr)
OPTION(prefix_2, &"--strip-debug"[2], strip_debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Strip debugging information", nullptr, nullptr)
OPTION(prefix_3, &"--symbol-ordering-file="[2], symbol_ordering_file_eq, Joined, INVALID, symbol_ordering_file, nullptr, 0, 0,
       "Layout sections to place symbols in the order specified by symbol ordering file", nullptr, nullptr)
OPTION(prefix_3, &"--symbol-ordering-file"[2], symbol_ordering_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--sysroot="[2], sysroot_eq, Joined, INVALID, sysroot, nullptr, 0, 0,
       "Set the system root", nullptr, nullptr)
OPTION(prefix_2, &"--sysroot"[2], sysroot, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-S"[1], anonymous_35, Flag, INVALID, strip_debug, nullptr, 0, 0,
       "Alias for --strip-debug", nullptr, nullptr)
OPTION(prefix_1, &"-s"[1], anonymous_34, Flag, INVALID, strip_all, nullptr, 0, 0,
       "Alias for --strip-all", nullptr, nullptr)
OPTION(prefix_2, &"--target1-abs"[2], target1_abs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Interpret R_ARM_TARGET1 as R_ARM_ABS32 (default)", nullptr, nullptr)
OPTION(prefix_2, &"--target1-rel"[2], target1_rel, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Interpret R_ARM_TARGET1 as R_ARM_REL32", nullptr, nullptr)
OPTION(prefix_2, &"--target2="[2], target2_eq, Joined, INVALID, target2, nullptr, 0, 0,
       "Interpret R_ARM_TARGET2 as <type>, where <type> is one of rel, abs, or got-rel", "<type>", nullptr)
OPTION(prefix_2, &"--target2"[2], target2, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<type>", nullptr)
OPTION(prefix_2, &"--Tbss="[2], Tbss_eq, Joined, INVALID, Tbss, nullptr, 0, 0,
       "Same as --section-start with .bss as the sectionname", nullptr, nullptr)
OPTION(prefix_2, &"--Tbss"[2], Tbss, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--Tdata="[2], Tdata_eq, Joined, INVALID, Tdata, nullptr, 0, 0,
       "Same as --section-start with .data as the sectionname", nullptr, nullptr)
OPTION(prefix_2, &"--Tdata"[2], Tdata, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-cache-dir="[2], thinlto_cache_dir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-cache-policy="[2], thinlto_cache_policy_eq, Joined, INVALID, thinlto_cache_policy, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-cache-policy"[2], thinlto_cache_policy, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-emit-imports-files"[2], thinlto_emit_imports_files, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-emit-index-files"[2], thinlto_emit_index_files, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-index-only="[2], thinlto_index_only_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-index-only"[2], thinlto_index_only, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-jobs="[2], thinlto_jobs, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of ThinLTO jobs. Default to --threads=", nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-object-suffix-replace="[2], thinlto_object_suffix_replace_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-prefix-replace="[2], thinlto_prefix_replace_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-single-module="[2], thinlto_single_module_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specific a single module to compile in ThinLTO mode, for debugging only", nullptr, nullptr)
OPTION(prefix_3, &"--threads="[2], threads_eq, Joined, INVALID, threads, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_3, &"--threads"[2], threads, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--time-trace-granularity="[2], time_trace_granularity_eq, Joined, INVALID, time_trace_granularity, nullptr, 0, 0,
       "Minimum time granularity (in microseconds) traced by time profiler", nullptr, nullptr)
OPTION(prefix_3, &"--time-trace-granularity"[2], time_trace_granularity, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--time-trace="[2], time_trace_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Record time trace to <file>", "<file>", nullptr)
OPTION(prefix_3, &"--time-trace"[2], anonymous_3, Flag, INVALID, time_trace_eq, nullptr, 0, 0,
       "Record time trace to file next to output", nullptr, nullptr)
OPTION(prefix_3, &"--toc-optimize"[2], toc_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Enable TOC related optimizations (default)", nullptr, nullptr)
OPTION(prefix_2, &"--trace-symbol="[2], trace_symbol_eq, Joined, INVALID, trace_symbol, nullptr, 0, 0,
       "Trace references to symbols", nullptr, nullptr)
OPTION(prefix_2, &"--trace-symbol"[2], trace_symbol, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--trace"[2], trace, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print the names of the input files", nullptr, nullptr)
OPTION(prefix_4, &"-Ttext-segment="[1], anonymous_37, Joined, INVALID, Ttext_segment, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_4, &"-Ttext-segment"[1], Ttext_segment, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--Ttext="[2], Ttext_eq, Joined, INVALID, Ttext, nullptr, 0, 0,
       "Same as --section-start with .text as the sectionname", nullptr, nullptr)
OPTION(prefix_2, &"--Ttext"[2], Ttext, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-T"[1], anonymous_30, JoinedOrSeparate, INVALID, script, nullptr, 0, 0,
       "Alias for --script", nullptr, nullptr)
OPTION(prefix_1, &"-t"[1], anonymous_36, Flag, INVALID, trace, nullptr, 0, 0,
       "Alias for --trace", nullptr, nullptr)
OPTION(prefix_3, &"--undefined-glob="[2], undefined_glob_eq, Joined, INVALID, undefined_glob, nullptr, 0, 0,
       "Force undefined symbol during linking", "<pattern>", nullptr)
OPTION(prefix_3, &"--undefined-glob"[2], undefined_glob, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<pattern>", nullptr)
OPTION(prefix_2, &"--undefined-version"[2], undefined_version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow unused version in version script (default)", nullptr, nullptr)
OPTION(prefix_2, &"--undefined="[2], undefined_eq, Joined, INVALID, undefined, nullptr, 0, 0,
       "Force undefined symbol during linking", "<symbol>", nullptr)
OPTION(prefix_2, &"--undefined"[2], undefined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, &"--unique"[2], unique, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Creates a separate output section for every orphan input section", nullptr, nullptr)
OPTION(prefix_2, &"--unresolved-symbols="[2], unresolved_symbols_eq, Joined, INVALID, unresolved_symbols, nullptr, 0, 0,
       "Determine how to handle unresolved symbols", nullptr, nullptr)
OPTION(prefix_2, &"--unresolved-symbols"[2], unresolved_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--use-android-relr-tags"[2], use_android_relr_tags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use SHT_ANDROID_RELR / DT_ANDROID_RELR* tags instead of SHT_RELR / DT_RELR*", nullptr, nullptr)
OPTION(prefix_1, &"-u"[1], anonymous_39, JoinedOrSeparate, INVALID, undefined, nullptr, 0, 0,
       "Alias for --undefined", nullptr, nullptr)
OPTION(prefix_2, &"--verbose"[2], verbose, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Verbose mode", nullptr, nullptr)
OPTION(prefix_2, &"--version-script="[2], version_script_eq, Joined, INVALID, version_script, nullptr, 0, 0,
       "Read a version script", nullptr, nullptr)
OPTION(prefix_2, &"--version-script"[2], version_script, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--version"[2], version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_3, &"--vs-diagnostics"[2], visual_studio_diagnostics_format, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Format diagnostics for Visual Studio compatibility", nullptr, nullptr)
OPTION(prefix_1, &"-V"[1], anonymous_40, Flag, INVALID, version, nullptr, 0, 0,
       "Alias for --version", nullptr, nullptr)
OPTION(prefix_1, &"-v"[1], v, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number", nullptr, nullptr)
OPTION(prefix_3, &"--warn-backrefs-exclude="[2], warn_backrefs_exclude_eq, Joined, INVALID, warn_backrefs_exclude, nullptr, 0, 0,
       "Glob describing an archive (or an object file within --start-lib) which should be ignored for --warn-backrefs.", "<glob>", nullptr)
OPTION(prefix_3, &"--warn-backrefs-exclude"[2], warn_backrefs_exclude, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<glob>", nullptr)
OPTION(prefix_3, &"--warn-backrefs"[2], warn_backrefs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about backward symbol references to extract archive members", nullptr, nullptr)
OPTION(prefix_2, &"--warn-common"[2], warn_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about duplicate common symbols", nullptr, nullptr)
OPTION(prefix_2, &"--warn-execstack"[2], anonymous_79, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--warn-ifunc-textrel"[2], warn_ifunc_textrel, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about using ifunc symbols with text relocations", nullptr, nullptr)
OPTION(prefix_2, &"--warn-once"[2], anonymous_80, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--warn-shared-textrel"[2], anonymous_81, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--warn-symbol-ordering"[2], warn_symbol_ordering, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about problems with the symbol ordering file (default)", nullptr, nullptr)
OPTION(prefix_2, &"--warn-unresolved-symbols"[2], warn_unresolved_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols as warnings", nullptr, nullptr)
OPTION(prefix_2, &"--whole-archive"[2], whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Force load of all members in a static library", nullptr, nullptr)
OPTION(prefix_3, &"--why-extract="[2], why_extract, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Print to a file about why archive members are extracted", nullptr, nullptr)
OPTION(prefix_2, &"--wrap="[2], wrap_eq, Joined, INVALID, wrap, nullptr, 0, 0,
       "Redirect symbol references to __wrap_symbol and __real_symbol references to symbol", "<symbol>", nullptr)
OPTION(prefix_2, &"--wrap"[2], wrap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_1, &"-X"[1], anonymous_11, Flag, INVALID, discard_locals, nullptr, 0, 0,
       "Alias for --discard-locals", nullptr, nullptr)
OPTION(prefix_1, &"-x"[1], anonymous_10, Flag, INVALID, discard_all, nullptr, 0, 0,
       "Alias for --discard-all", nullptr, nullptr)
OPTION(prefix_1, &"-y"[1], anonymous_38, JoinedOrSeparate, INVALID, trace_symbol, nullptr, 0, 0,
       "Alias for --trace-symbol", nullptr, nullptr)
OPTION(prefix_1, &"-z"[1], z, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Linker option extensions", "<option>", nullptr)
#endif // OPTION

#ifdef SIMPLE_ENUM_VALUE_TABLE

struct SimpleEnumValue {
  const char *Name;
  unsigned Value;
};

struct SimpleEnumValueTable {
  const SimpleEnumValue *Table;
  unsigned Size;
};
static const SimpleEnumValueTable SimpleEnumValueTables[] = {};
static const unsigned SimpleEnumValueTablesSize = sizeof(SimpleEnumValueTables) / sizeof(SimpleEnumValueTable);
#endif // SIMPLE_ENUM_VALUE_TABLE


#ifdef OPTTABLE_ARG_INIT
//////////
// Option Values


#endif // OPTTABLE_ARG_INIT
