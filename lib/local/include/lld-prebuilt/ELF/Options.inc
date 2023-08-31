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
PREFIX(prefix_0, {llvm::StringLiteral("")})
PREFIX(prefix_1, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("")})
PREFIX(prefix_4, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_3, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_2, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("-") COMMA llvm::StringLiteral("")})
#undef COMMA
#endif // PREFIX

/////////
// Prefix Union

#ifdef PREFIX_UNION
#define COMMA ,
PREFIX_UNION({
llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
#undef COMMA
#endif // PREFIX_UNION

/////////
// ValuesCode

#ifdef OPTTABLE_VALUES_CODE
#endif
/////////
// Groups

#ifdef OPTION

//////////
// Options

OPTION(prefix_0, llvm::StringLiteral("<input>"), INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, llvm::StringLiteral("<unknown>"), UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("("), anonymous_34, Flag, INVALID, start_group, nullptr, 0, 0,
       "Alias for --start-group", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral(")"), anonymous_14, Flag, INVALID, end_group, nullptr, 0, 0,
       "Alias for --end-group", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("allow-multiple-definition"), allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiple definitions", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("allow-shlib-undefined"), allow_shlib_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow unresolved references in shared libraries (default when linking a shared library)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("android-memtag-heap"), android_memtag_heap, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Instruct the dynamic loader to enable MTE protection for the heap", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("android-memtag-mode="), android_memtag_mode_eq, Joined, INVALID, android_memtag_mode, nullptr, 0, 0,
       "Instruct the dynamic loader to start under MTE mode {async, sync, none}", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("android-memtag-mode"), android_memtag_mode, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("android-memtag-stack"), android_memtag_stack, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Instruct the dynamic loader to prepare for MTE stack instrumentation", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("apply-dynamic-relocs"), apply_dynamic_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Apply link-time values for dynamic relocations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("as-needed"), as_needed, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Only set DT_NEEDED for shared libraries if used", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("auxiliary="), auxiliary_eq, Joined, INVALID, auxiliary, nullptr, 0, 0,
       "Set DT_AUXILIARY field to the specified name", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("auxiliary"), auxiliary, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bdynamic"), Bdynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Link against shared libraries (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bno-symbolic"), Bno_symbolic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't bind default visibility defined symbols locally for -shared (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bshareable"), anonymous_32, Flag, INVALID, shared, nullptr, 0, 0,
       "Alias for --shared", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bstatic"), Bstatic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not link against shared libraries", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bsymbolic-functions"), Bsymbolic_functions, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind default visibility defined function symbols locally for -shared", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bsymbolic-non-weak-functions"), Bsymbolic_non_weak_functions, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind default visibility defined STB_GLOBAL function symbols locally for -shared", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bsymbolic"), Bsymbolic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Bind default visibility defined symbols locally for -shared", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("build-id="), build_id, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Generate build ID note", "[fast,md5,sha1,uuid,0x<hexstring>]", nullptr)
OPTION(prefix_2, llvm::StringLiteral("build-id"), anonymous_0, Flag, INVALID, build_id, "fast\0", 0, 0,
       "Alias for --build-id=fast", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("b"), anonymous_18, Separate, INVALID, format, nullptr, 0, 0,
       "Alias for --format", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("call-graph-ordering-file="), call_graph_ordering_file_eq, Joined, INVALID, call_graph_ordering_file, nullptr, 0, 0,
       "Layout sections to optimize the given callgraph", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("call-graph-ordering-file"), call_graph_ordering_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("call-graph-profile-sort"), call_graph_profile_sort, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Reorder sections with call graph profile (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("call_shared"), anonymous_6, Flag, INVALID, Bdynamic, nullptr, 0, 0,
       "Alias for --Bdynamic", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("check-dynamic-relocations"), check_dynamic_relocations, Flag, INVALID, INVALID, nullptr, HelpHidden, 0,
       "Perform additional validation of the written dynamic relocations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("check-sections"), check_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Check section addresses for overlaps (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("chroot"), chroot, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("color-diagnostics="), color_diagnostics, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("color-diagnostics"), anonymous_1, Flag, INVALID, color_diagnostics, "always\0", 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("compress-debug-sections="), compress_debug_sections_eq, Joined, INVALID, compress_debug_sections, nullptr, 0, 0,
       "Compress DWARF debug sections", "[none,zlib,zstd]", nullptr)
OPTION(prefix_2, llvm::StringLiteral("compress-debug-sections"), compress_debug_sections, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[none,zlib,zstd]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("cref"), cref, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Output cross reference table. If -Map is specified, print to the map file", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("defsym="), defsym_eq, Joined, INVALID, defsym, nullptr, 0, 0,
       "Define a symbol alias", "<symbol>=<value>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("defsym"), defsym, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>=<value>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("demangle"), demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("dependency-file="), dependency_file_eq, Joined, INVALID, dependency_file, nullptr, 0, 0,
       "Write a dependency file", "<file>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("dependency-file"), dependency_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<file>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("dependent-libraries"), dependent_libraries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Process dependent library specifiers from input files (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-new-dtags"), disable_new_dtags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable new dynamic tags", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-verify"), disable_verify, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("discard-all"), discard_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Delete all local symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("discard-locals"), discard_locals, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Delete temporary local symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("discard-none"), discard_none, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Keep all symbols in the symbol table", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("dn"), anonymous_8, Flag, INVALID, Bstatic, nullptr, 0, 0,
       "Alias for --Bstatic", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("dynamic-linker="), dynamic_linker_eq, Joined, INVALID, dynamic_linker, nullptr, 0, 0,
       "Which dynamic linker to use", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("dynamic-linker"), dynamic_linker, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("dynamic-list="), dynamic_list_eq, Joined, INVALID, dynamic_list, nullptr, 0, 0,
       "Similar to --export-dynamic-symbol-list. When creating a shared object, this additionally implies -Bsymbolic but does not set DF_SYMBOLIC", "<file>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("dynamic-list"), dynamic_list, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<file>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("dy"), anonymous_7, Flag, INVALID, Bdynamic, nullptr, 0, 0,
       "Alias for --Bdynamic", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("d"), anonymous_66, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("EB"), eb, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Select the big-endian format in OUTPUT_FORMAT", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("eh-frame-hdr"), eh_frame_hdr, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Request creation of .eh_frame_hdr section and PT_GNU_EH_FRAME segment header", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("EL"), el, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Select the little-endian format in OUTPUT_FORMAT", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("emit-relocs"), emit_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate relocations in output", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("enable-new-dtags"), enable_new_dtags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable new dynamic tags (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("end-group"), end_group, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignored for compatibility with GNU unless you pass --warn-backrefs", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("end-lib"), end_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "End a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("entry="), entry_eq, Joined, INVALID, entry, nullptr, 0, 0,
       "Name of entry point symbol", "<entry>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("entry"), entry, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<entry>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("error-handling-script="), error_handling_script_eq, Joined, INVALID, error_handling_script, nullptr, 0, 0,
       "Specify an error handling script", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("error-handling-script"), error_handling_script, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("error-limit="), error_limit_eq, Joined, INVALID, error_limit, nullptr, 0, 0,
       "Maximum number of errors to emit before stopping (0 = no limit)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("error-limit"), error_limit, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("error-unresolved-symbols"), error_unresolved_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols as errors", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("exclude-libs="), exclude_libs_eq, Joined, INVALID, exclude_libs, nullptr, 0, 0,
       "Exclude static libraries from automatic export", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("exclude-libs"), exclude_libs, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("execute-only"), execute_only, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mark executable sections unreadable", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("export-dynamic-symbol-list="), export_dynamic_symbol_list_eq, Joined, INVALID, export_dynamic_symbol_list, nullptr, 0, 0,
       "Read a list of dynamic symbol patterns. Apply --export-dynamic-symbol on each pattern", "file", nullptr)
OPTION(prefix_3, llvm::StringLiteral("export-dynamic-symbol-list"), export_dynamic_symbol_list, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "file", nullptr)
OPTION(prefix_3, llvm::StringLiteral("export-dynamic-symbol="), export_dynamic_symbol_eq, Joined, INVALID, export_dynamic_symbol, nullptr, 0, 0,
       "(executable) Put matched symbols in the dynamic symbol table. (shared object) References to matched non-local STV_DEFAULT symbols shouldn't be bound to definitions within the shared object. Does not imply -Bsymbolic.", "glob", nullptr)
OPTION(prefix_3, llvm::StringLiteral("export-dynamic-symbol"), export_dynamic_symbol, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "glob", nullptr)
OPTION(prefix_2, llvm::StringLiteral("export-dynamic"), export_dynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Put symbols in the dynamic symbol table", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("E"), anonymous_16, Flag, INVALID, export_dynamic, nullptr, 0, 0,
       "Alias for --export-dynamic", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("e"), anonymous_15, JoinedOrSeparate, INVALID, entry, nullptr, 0, 0,
       "Alias for --entry", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("fatal-warnings"), fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("filter="), filter_eq, Joined, INVALID, filter, nullptr, 0, 0,
       "Set DT_FILTER field to the specified name", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("filter"), filter, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("fini="), fini_eq, Joined, INVALID, fini, nullptr, 0, 0,
       "Specify a finalizer function", "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("fini"), fini, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("fix-cortex-a53-843419"), fix_cortex_a53_843419, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Apply fixes for AArch64 Cortex-A53 erratum 843419", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("fix-cortex-a8"), fix_cortex_a8, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Apply fixes for ARM Cortex-A8 erratum 657417", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("format="), format_eq, Joined, INVALID, format, nullptr, 0, 0,
       "Change the input format of the inputs following this option", "[default,elf,binary]", nullptr)
OPTION(prefix_2, llvm::StringLiteral("format"), format, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[default,elf,binary]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("fortran-common"), fortran_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Search archive members for definitions to override COMMON symbols (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("F"), anonymous_17, Separate, INVALID, filter, nullptr, 0, 0,
       "Alias for --filter", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("f"), anonymous_5, Separate, INVALID, auxiliary, nullptr, 0, 0,
       "Alias for --auxiliary", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("gc-sections"), gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable garbage collection of unused sections", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("gdb-index"), gdb_index, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate .gdb_index section", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("gnu-unique"), gnu_unique, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable STB_GNU_UNIQUE symbol binding (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("G"), anonymous_82, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("g"), anonymous_67, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("hash-style="), hash_style_eq, Joined, INVALID, hash_style, nullptr, 0, 0,
       "Specify hash style (sysv, gnu or both)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("hash-style"), hash_style, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("help"), help, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print option help", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("h"), anonymous_33, JoinedOrSeparate, INVALID, soname, nullptr, 0, 0,
       "Alias for --soname", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("icf=all"), icf_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable identical code folding", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("icf=none"), icf_none, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable identical code folding (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("icf=safe"), icf_safe, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable safe identical code folding", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("ignore-data-address-equality"), ignore_data_address_equality, Flag, INVALID, INVALID, nullptr, 0, 0,
       "lld can break the address equality of data", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("ignore-function-address-equality"), ignore_function_address_equality, Flag, INVALID, INVALID, nullptr, 0, 0,
       "lld can break the address equality of functions", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("image-base="), image_base_eq, Joined, INVALID, image_base, nullptr, 0, 0,
       "Set the base address", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("image-base"), image_base, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("init="), init_eq, Joined, INVALID, init, nullptr, 0, 0,
       "Specify an initializer function", "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("init"), init, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("just-symbols="), just_symbols_eq, Joined, INVALID, just_symbols, nullptr, 0, 0,
       "Just link symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("just-symbols"), just_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("keep-unique="), keep_unique_eq, Joined, INVALID, keep_unique, nullptr, 0, 0,
       "Do not fold this symbol during ICF", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("keep-unique"), keep_unique, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("library-path="), anonymous_22, Joined, INVALID, library_path, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("library-path"), anonymous_21, Separate, INVALID, library_path, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("library="), anonymous_20, Joined, INVALID, library, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("library"), anonymous_19, Separate, INVALID, library, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("load-pass-plugin="), load_pass_plugins_eq, Joined, INVALID, load_pass_plugins, nullptr, 0, 0,
       "Load passes from plugin library", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("load-pass-plugin"), load_pass_plugins, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("long-plt"), anonymous_68, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-aa-pipeline="), lto_aa_pipeline, Joined, INVALID, INVALID, nullptr, 0, 0,
       "AA pipeline to run during LTO. Used in conjunction with -lto-newpm-passes", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-basic-block-sections="), lto_basic_block_sections, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Enable basic block sections for LTO", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-cs-profile-file="), lto_cs_profile_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Context sensitive profile file path", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-cs-profile-generate"), lto_cs_profile_generate, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Perform context sensitive PGO instrumentation", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-debug-pass-manager"), lto_debug_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Debug new pass manager", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-emit-asm"), lto_emit_asm, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Emit assembly code", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-newpm-passes="), lto_newpm_passes, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Passes to run during LTO", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-obj-path="), lto_obj_path_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-O"), lto_O, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Optimization level for LTO", "<opt-level>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-partitions="), lto_partitions, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of LTO codegen partitions", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-pgo-warn-mismatch"), lto_pgo_warn_mismatch, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn on warnings about profile cfg mismatch (default)>", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-sample-profile="), lto_sample_profile, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Sample profile file path", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-unique-basic-block-section-names"), lto_unique_basic_block_section_names, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Give unique names to every basic block section for LTO", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("lto-whole-program-visibility"), lto_whole_program_visibility, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Asserts that the LTO link has whole program visibility", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("L"), library_path, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Add <dir> to the library search path", "<dir>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("l"), library, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Search for library <libname>", "<libname>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("Map="), Map_eq, Joined, INVALID, Map, nullptr, 0, 0,
       "Print a link map to the specified file", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Map"), Map, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("merge-exidx-entries"), merge_exidx_entries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable merging .ARM.exidx entries (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("mips-got-size="), mips_got_size_eq, Joined, INVALID, mips_got_size, nullptr, HelpHidden, 0,
       "Max size of a single MIPS GOT. 0x10000 by default.", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("mips-got-size"), mips_got_size, Separate, INVALID, INVALID, nullptr, HelpHidden, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("mllvm="), mllvm_eq, Joined, INVALID, mllvm, nullptr, 0, 0,
       "Additional arguments to forward to LLVM's option processing", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("mllvm"), mllvm, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("mmap-output-file"), mmap_output_file, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mmap the output file for writing (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("M"), anonymous_28, Flag, INVALID, print_map, nullptr, 0, 0,
       "Alias for --print-map", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("m"), m, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Set target emulation", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("nmagic"), nmagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not page align sections, link against static libraries.", "<magic>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-add-needed"), anonymous_69, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-allow-multiple-definition"), no_allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not allow multiple definitions (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-allow-shlib-undefined"), no_allow_shlib_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not allow unresolved references in shared libraries (default when linking an executable)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-android-memtag-heap"), no_android_memtag_heap, Flag, INVALID, INVALID, nullptr, 0, 0,
       "", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-android-memtag-stack"), no_android_memtag_stack, Flag, INVALID, INVALID, nullptr, 0, 0,
       "", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-apply-dynamic-relocs"), no_apply_dynamic_relocs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not apply link-time values for dynamic relocations (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-as-needed"), no_as_needed, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Always set DT_NEEDED for shared libraries (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-call-graph-profile-sort"), no_call_graph_profile_sort, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not reorder sections with call graph profile", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-check-dynamic-relocations"), no_check_dynamic_relocations, Flag, INVALID, INVALID, nullptr, HelpHidden, 0,
       "Do not perform additional validation of the written dynamic relocations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-check-sections"), no_check_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not check section addresses for overlaps", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-color-diagnostics"), anonymous_2, Flag, INVALID, color_diagnostics, "never\0", 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-copy-dt-needed-entries"), anonymous_70, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-ctors-in-init-array"), anonymous_71, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-demangle"), no_demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbol names", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-dependent-libraries"), no_dependent_libraries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignore dependent library specifiers from input files", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-dynamic-linker"), no_dynamic_linker, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Inhibit output of .interp section", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-eh-frame-hdr"), no_eh_frame_hdr, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not create .eh_frame_hdr section", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-execute-only"), no_execute_only, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mark executable sections readable (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-export-dynamic"), no_export_dynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not put symbols in the dynamic symbol table (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-fatal-warnings"), no_fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not treat warnings as errors (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-fortran-common"), no_fortran_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not search archive members for definitions to override COMMON symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-gc-sections"), no_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable garbage collection of unused sections (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-gdb-index"), no_gdb_index, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not generate .gdb_index section (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-gnu-unique"), no_gnu_unique, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable STB_GNU_UNIQUE symbol binding", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-keep-memory"), anonymous_72, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-lto-legacy-pass-manager"), no_lto_legacy_pass_manager, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use the new pass manager in LLVM", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-lto-pgo-warn-mismatch"), no_lto_pgo_warn_mismatch, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn off warnings about profile cfg mismatch", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-lto-unique-basic-block-section-names"), no_lto_unique_basic_block_section_names, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not give unique names to every basic block section for LTO (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-lto-whole-program-visibility"), no_lto_whole_program_visibility, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Asserts that the LTO link does not have whole program visibility", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-merge-exidx-entries"), no_merge_exidx_entries, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable merging .ARM.exidx entries", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-mmap-output-file"), no_mmap_output_file, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not mmap the output file for writing", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-nmagic"), no_nmagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Page align sections (default)", "<magic>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-omagic"), no_omagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not set the text data sections to be writable, page align sections (default)", "<magic>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-optimize-bb-jumps"), no_optimize_bb_jumps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not remove any direct jumps at the end to the next basic block (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-pcrel-optimize"), no_pcrel_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Disable PC-relative optimizations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-pie"), no_pie, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not create a position independent executable (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-power10-stubs"), no_power10_stubs, Flag, INVALID, power10_stubs_eq, "no\0", 0, 0,
       "Alias for --power10-stubs=no", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-print-gc-sections"), no_print_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not list removed unused sections (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-print-icf-sections"), no_print_icf_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not list identical folded sections (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-relax"), no_relax, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable target-specific relaxations", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-rosegment"), no_rosegment, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not put read-only non-executable sections in their own segment", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-toc-optimize"), no_toc_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Disable TOC related optimizations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-undefined-version"), no_undefined_version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report version scripts that refer undefined symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-undefined"), no_undefined, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols even if the linker is creating a shared library", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-use-android-relr-tags"), no_use_android_relr_tags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use SHT_RELR / DT_RELR* tags (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-warn-backrefs"), no_warn_backrefs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about backward symbol references to extract archive members (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-warn-common"), no_warn_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about duplicate common symbols (default)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-warn-ifunc-textrel"), no_warn_ifunc_textrel, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about using ifunc symbols with text relocations (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-warn-mismatch"), anonymous_73, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-warn-symbol-ordering"), no_warn_symbol_ordering, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not warn about problems with the symbol ordering file", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("no-warnings"), no_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Suppress warnings and cancel --fatal-warnings", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-whole-archive"), no_whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not force load of all members in a static library (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("noinhibit-exec"), noinhibit_exec, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Retain the executable output file whenever it is still usable", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("non_shared"), anonymous_9, Flag, INVALID, Bstatic, nullptr, 0, 0,
       "Alias for --Bstatic", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("nostdlib"), nostdlib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Only search directories specified on the command line", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("N"), anonymous_24, Flag, INVALID, omagic, nullptr, 0, 0,
       "Alias for --omagic", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("n"), anonymous_23, Flag, INVALID, nmagic, nullptr, 0, 0,
       "Alias for --nmagic", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("oformat="), oformat_eq, Joined, INVALID, oformat, nullptr, 0, 0,
       "Specify the binary format for the output object file", "[elf,binary]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("oformat"), oformat, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[elf,binary]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("omagic"), omagic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the text and data sections to be readable and writable, do not page align sections, link against static libraries", "<magic>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("opt-remarks-filename"), opt_remarks_filename, Separate, INVALID, INVALID, nullptr, 0, 0,
       "YAML output file for optimization remarks", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("opt-remarks-format"), opt_remarks_format, Separate, INVALID, INVALID, nullptr, 0, 0,
       "The format used for serializing remarks (default: YAML)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("opt-remarks-hotness-threshold="), opt_remarks_hotness_threshold_eq, Joined, INVALID, opt_remarks_hotness_threshold, nullptr, 0, 0,
       "Minimum profile count required for an optimization remark to be output. Use 'auto' to apply the threshold from profile summary.", "<value>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("opt-remarks-hotness-threshold"), opt_remarks_hotness_threshold, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<value>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("opt-remarks-passes"), opt_remarks_passes, Separate, INVALID, INVALID, nullptr, 0, 0,
       "Regex for the passes that need to be serialized to the output file", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("opt-remarks-with-hotness"), opt_remarks_with_hotness, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include hotness information in the optimization remarks file", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("optimize-bb-jumps"), optimize_bb_jumps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove direct jumps at the end to the next basic block", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("orphan-handling="), orphan_handling_eq, Joined, INVALID, orphan_handling, nullptr, 0, 0,
       "Control how orphan sections are handled when linker script used", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("orphan-handling"), orphan_handling, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("output="), anonymous_25, Joined, INVALID, o, nullptr, 0, 0,
       "Alias for -o", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("output"), anonymous_26, Separate, INVALID, o, nullptr, 0, 0,
       "Alias for -o", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("O"), O, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Optimize output file size", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("o"), o, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", "<path>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("pack-dyn-relocs="), pack_dyn_relocs_eq, Joined, INVALID, pack_dyn_relocs, nullptr, 0, 0,
       "Pack dynamic relocations in the given format", "[none,android,relr,android+relr]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("pack-dyn-relocs"), pack_dyn_relocs, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[none,android,relr,android+relr]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("package-metadata="), package_metadata, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Emit package metadata note", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("pcrel-optimize"), pcrel_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Enable PC-relative optimizations (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("pic-executable"), anonymous_27, Flag, INVALID, pie, nullptr, 0, 0,
       "Alias for --pie", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("pic-veneer"), pic_veneer, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Always generate position independent thunks (veneers)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("pie"), pie, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create a position independent executable", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=-fresolution="), anonymous_64, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=-pass-through="), anonymous_65, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=-"), plugin_opt_eq_minus, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify an LLVM option for compatibility with LLVMgold.so", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=cs-profile-generate"), anonymous_48, Flag, INVALID, lto_cs_profile_generate, nullptr, 0, 0,
       "Alias for --lto-cs-profile-generate", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=cs-profile-path="), anonymous_49, Joined, INVALID, lto_cs_profile_file, nullptr, 0, 0,
       "Alias for --lto-cs-profile-file", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=debug-pass-manager"), anonymous_43, Flag, INVALID, lto_debug_pass_manager, nullptr, 0, 0,
       "Alias for --lto-debug-pass-manager", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=disable-verify"), anonymous_44, Flag, INVALID, disable_verify, nullptr, 0, 0,
       "Alias for --disable-verify", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=dwo_dir="), plugin_opt_dwo_dir_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Directory to store .dwo files when LTO and debug fission are used", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=emit-asm"), plugin_opt_emit_asm, Flag, INVALID, lto_emit_asm, nullptr, 0, 0,
       "Alias for --lto-emit-asm", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=emit-llvm"), plugin_opt_emit_llvm, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=jobs="), anonymous_45, Joined, INVALID, thinlto_jobs_eq, nullptr, 0, 0,
       "Alias for --thinlto-jobs=", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=lto-partitions="), anonymous_46, Joined, INVALID, lto_partitions, nullptr, 0, 0,
       "Alias for --lto-partitions", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=mcpu="), plugin_opt_mcpu_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=new-pass-manager"), anonymous_47, Flag, INVALID, no_lto_legacy_pass_manager, nullptr, 0, 0,
       "Alias for --no-lto-legacy-pass-manager", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=no-opaque-pointers"), plugin_opt_no_opaque_pointers, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use typed pointers in IR during LTO", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=obj-path="), anonymous_50, Joined, INVALID, lto_obj_path_eq, nullptr, 0, 0,
       "Alias for --lto-obj-path=", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=opaque-pointers"), plugin_opt_opaque_pointers, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use opaque pointers in IR during LTO (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=opt-remarks-filename="), anonymous_51, Joined, INVALID, opt_remarks_filename, nullptr, 0, 0,
       "Alias for --opt-remarks-filename", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=opt-remarks-format="), anonymous_53, Joined, INVALID, opt_remarks_format, nullptr, 0, 0,
       "Alias for --opt-remarks-format", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=opt-remarks-hotness-threshold="), anonymous_55, Joined, INVALID, opt_remarks_hotness_threshold, nullptr, 0, 0,
       "Alias for --opt-remarks-hotness-threshold", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=opt-remarks-passes="), anonymous_52, Joined, INVALID, opt_remarks_passes, nullptr, 0, 0,
       "Alias for --opt-remarks-passes", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=opt-remarks-with-hotness"), anonymous_54, Flag, INVALID, opt_remarks_with_hotness, nullptr, 0, 0,
       "Alias for --opt-remarks-with_hotness", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=O"), anonymous_42, Joined, INVALID, lto_O, nullptr, 0, 0,
       "Alias for --lto-O", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=sample-profile="), anonymous_56, Joined, INVALID, lto_sample_profile, nullptr, 0, 0,
       "Alias for --lto-sample-profile", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=save-temps"), anonymous_57, Flag, INVALID, save_temps, nullptr, 0, 0,
       "Alias for --save-temps", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=stats-file="), plugin_opt_stats_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Filename to write LTO statistics to", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=thinlto-emit-imports-files"), anonymous_58, Flag, INVALID, thinlto_emit_imports_files, nullptr, 0, 0,
       "Alias for --thinlto-emit-imports-files", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=thinlto-index-only="), anonymous_60, Joined, INVALID, thinlto_index_only_eq, nullptr, 0, 0,
       "Alias for --thinlto-index-only=", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=thinlto-index-only"), anonymous_59, Flag, INVALID, thinlto_index_only, nullptr, 0, 0,
       "Alias for --thinlto-index-only", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=thinlto-object-suffix-replace="), anonymous_61, Joined, INVALID, thinlto_object_suffix_replace_eq, nullptr, 0, 0,
       "Alias for --thinlto-object-suffix-replace=", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=thinlto-prefix-replace="), anonymous_62, Joined, INVALID, thinlto_prefix_replace_eq, nullptr, 0, 0,
       "Alias for --thinlto-prefix-replace=", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt=thinlto"), anonymous_63, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt="), plugin_opt_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin="), plugin_eq, Joined, INVALID, plugin, nullptr, 0, 0,
       "Ignored for compatibility with GNU linkers", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin"), plugin, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("pop-state"), pop_state, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Restore the states saved by --push-state", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("power10-stubs="), power10_stubs_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Whether to use Power10 instructions in call stubs for R_PPC64_REL24_NOTOC and TOC/NOTOC interworking (yes (default): use; no: don't use). \"auto\" is currently the same as \"yes\"", "<mode>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("power10-stubs"), power10_stubs, Flag, INVALID, power10_stubs_eq, "yes\0", 0, 0,
       "Alias for --power10-stubs=auto", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-archive-stats="), print_archive_stats, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Write archive usage statistics to the specified file. Print the numbers of members and extracted members for each archive", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-gc-sections"), print_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "List removed unused sections", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-icf-sections"), print_icf_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "List identical folded sections", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-map"), print_map, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print a link map to the standard output", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-symbol-order="), print_symbol_order_eq, Joined, INVALID, print_symbol_order, nullptr, 0, 0,
       "Print a symbol order specified by --call-graph-ordering-file into the specified file", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-symbol-order"), print_symbol_order, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("push-state"), push_state, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save the current state of --as-needed, -static and --whole-archive", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("q"), anonymous_13, Flag, INVALID, emit_relocs, nullptr, 0, 0,
       "Alias for --emit-relocs", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("relax"), relax, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable target-specific relaxations if supported (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("relocatable"), relocatable, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create relocatable object file", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("reproduce="), reproduce_eq, Joined, INVALID, reproduce, nullptr, 0, 0,
       "Write tar file containing inputs and command to reproduce link", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("reproduce"), reproduce, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("retain-symbols-file="), retain_symbols_file_eq, Joined, INVALID, retain_symbols_file, nullptr, 0, 0,
       "Retain only the symbols listed in the file", "<file>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("retain-symbols-file"), retain_symbols_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<file>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("rosegment"), rosegment, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Put read-only non-executable sections in their own segment (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("rpath-link="), anonymous_75, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("rpath-link"), anonymous_74, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("rpath="), rpath_eq, Joined, INVALID, rpath, nullptr, 0, 0,
       "Add a DT_RUNPATH to the output", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("rpath"), rpath, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("rsp-quoting="), rsp_quoting_eq, Joined, INVALID, rsp_quoting, nullptr, 0, 0,
       "Quoting style for response files", "[posix,windows]", nullptr)
OPTION(prefix_3, llvm::StringLiteral("rsp-quoting"), rsp_quoting, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "[posix,windows]", nullptr)
OPTION(prefix_1, llvm::StringLiteral("R"), anonymous_30, JoinedOrSeparate, INVALID, rpath, nullptr, 0, 0,
       "Alias for --rpath", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("r"), anonymous_29, Flag, INVALID, relocatable, nullptr, 0, 0,
       "Alias for --relocatable", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("save-temps="), save_temps_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Save select intermediate LTO compilation results", nullptr, "resolution,preopt,promote,internalize,import,opt,precodegen,prelink,combinedindex")
OPTION(prefix_2, llvm::StringLiteral("save-temps"), save_temps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("script="), script_eq, Joined, INVALID, script, nullptr, 0, 0,
       "Read linker script", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("script"), script, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("section-start="), section_start_eq, Joined, INVALID, section_start, nullptr, 0, 0,
       "Set address of section", "<address>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("section-start"), section_start, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<address>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("secure-plt"), anonymous_76, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("shared"), shared, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Build a shared object", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("shuffle-sections="), shuffle_sections_eq, Joined, INVALID, shuffle_sections, nullptr, 0, 0,
       "Shuffle matched sections using the given seed before mapping them to the output sections. If -1, reverse the section order. If 0, use a random seed", "<section-glob>=<seed>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("shuffle-sections"), shuffle_sections, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<section-glob>=<seed>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("soname="), soname_eq, Joined, INVALID, soname, nullptr, 0, 0,
       "Set DT_SONAME", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("soname"), soname, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sort-common"), anonymous_77, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sort-section="), sort_section_eq, Joined, INVALID, sort_section, nullptr, 0, 0,
       "Specifies sections sorting rule when linkerscript is used", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sort-section"), sort_section, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("split-stack-adjust-size="), split_stack_adjust_size_eq, Joined, INVALID, split_stack_adjust_size, nullptr, 0, 0,
       "Specify adjustment to stack size when a split-stack function calls a non-split-stack function", "<value>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("split-stack-adjust-size"), split_stack_adjust_size, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<value>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("start-group"), start_group, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignored for compatibility with GNU unless you pass --warn-backrefs", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("start-lib"), start_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Start a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("static"), anonymous_10, Flag, INVALID, Bstatic, nullptr, 0, 0,
       "Alias for --Bstatic", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("stats"), anonymous_78, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("strip-all"), strip_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Strip all symbols. Implies --strip-debug", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("strip-debug"), strip_debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Strip debugging information", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("symbol-ordering-file="), symbol_ordering_file_eq, Joined, INVALID, symbol_ordering_file, nullptr, 0, 0,
       "Layout sections to place symbols in the order specified by symbol ordering file", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("symbol-ordering-file"), symbol_ordering_file, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sysroot="), sysroot_eq, Joined, INVALID, sysroot, nullptr, 0, 0,
       "Set the system root", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sysroot"), sysroot, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("S"), anonymous_36, Flag, INVALID, strip_debug, nullptr, 0, 0,
       "Alias for --strip-debug", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("s"), anonymous_35, Flag, INVALID, strip_all, nullptr, 0, 0,
       "Alias for --strip-all", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("target1-abs"), target1_abs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Interpret R_ARM_TARGET1 as R_ARM_ABS32 (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("target1-rel"), target1_rel, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Interpret R_ARM_TARGET1 as R_ARM_REL32", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("target2="), target2_eq, Joined, INVALID, target2, nullptr, 0, 0,
       "Interpret R_ARM_TARGET2 as <type>, where <type> is one of rel, abs, or got-rel", "<type>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("target2"), target2, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<type>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("Tbss="), Tbss_eq, Joined, INVALID, Tbss, nullptr, 0, 0,
       "Same as --section-start with .bss as the sectionname", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Tbss"), Tbss, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Tdata="), Tdata_eq, Joined, INVALID, Tdata, nullptr, 0, 0,
       "Same as --section-start with .data as the sectionname", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Tdata"), Tdata, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-cache-dir="), thinlto_cache_dir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-cache-policy="), thinlto_cache_policy_eq, Joined, INVALID, thinlto_cache_policy, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-cache-policy"), thinlto_cache_policy, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-emit-imports-files"), thinlto_emit_imports_files, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-emit-index-files"), thinlto_emit_index_files, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-index-only="), thinlto_index_only_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-index-only"), thinlto_index_only, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-jobs="), thinlto_jobs_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of ThinLTO jobs. Default to --threads=", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-object-suffix-replace="), thinlto_object_suffix_replace_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-prefix-replace="), thinlto_prefix_replace_eq, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-single-module="), thinlto_single_module_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify a single module to compile in ThinLTO mode, for debugging only", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("threads="), threads_eq, Joined, INVALID, threads, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("threads"), threads, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("time-trace-granularity="), time_trace_granularity_eq, Joined, INVALID, time_trace_granularity, nullptr, 0, 0,
       "Minimum time granularity (in microseconds) traced by time profiler", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("time-trace-granularity"), time_trace_granularity, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("time-trace="), time_trace_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Record time trace to <file>", "<file>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("time-trace"), anonymous_4, Flag, INVALID, time_trace_eq, nullptr, 0, 0,
       "Record time trace to file next to output", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("toc-optimize"), toc_optimize, Flag, INVALID, INVALID, nullptr, 0, 0,
       "(PowerPC64) Enable TOC related optimizations (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("trace-symbol="), trace_symbol_eq, Joined, INVALID, trace_symbol, nullptr, 0, 0,
       "Trace references to symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("trace-symbol"), trace_symbol, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("trace"), trace, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print the names of the input files", nullptr, nullptr)
OPTION(prefix_4, llvm::StringLiteral("Ttext-segment="), anonymous_38, Joined, INVALID, Ttext_segment, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_4, llvm::StringLiteral("Ttext-segment"), Ttext_segment, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Ttext="), Ttext_eq, Joined, INVALID, Ttext, nullptr, 0, 0,
       "Same as --section-start with .text as the sectionname", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Ttext"), Ttext, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("T"), anonymous_31, JoinedOrSeparate, INVALID, script, nullptr, 0, 0,
       "Alias for --script", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("t"), anonymous_37, Flag, INVALID, trace, nullptr, 0, 0,
       "Alias for --trace", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("undefined-glob="), undefined_glob_eq, Joined, INVALID, undefined_glob, nullptr, 0, 0,
       "Force undefined symbol during linking", "<pattern>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("undefined-glob"), undefined_glob, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<pattern>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("undefined-version"), undefined_version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow unused version in version script (disabled by default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("undefined="), undefined_eq, Joined, INVALID, undefined, nullptr, 0, 0,
       "Force undefined symbol during linking", "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("undefined"), undefined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("unique"), unique, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Creates a separate output section for every orphan input section", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("unresolved-symbols="), unresolved_symbols_eq, Joined, INVALID, unresolved_symbols, nullptr, 0, 0,
       "Determine how to handle unresolved symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("unresolved-symbols"), unresolved_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("use-android-relr-tags"), use_android_relr_tags, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use SHT_ANDROID_RELR / DT_ANDROID_RELR* tags instead of SHT_RELR / DT_RELR*", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("u"), anonymous_40, JoinedOrSeparate, INVALID, undefined, nullptr, 0, 0,
       "Alias for --undefined", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("verbose"), verbose, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Verbose mode", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("version-script="), version_script_eq, Joined, INVALID, version_script, nullptr, 0, 0,
       "Read a version script", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("version-script"), version_script, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("version"), version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("vs-diagnostics"), visual_studio_diagnostics_format, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Format diagnostics for Visual Studio compatibility", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("V"), anonymous_41, Flag, INVALID, v, nullptr, 0, 0,
       "Alias for -v", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("v"), v, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("warn-backrefs-exclude="), warn_backrefs_exclude_eq, Joined, INVALID, warn_backrefs_exclude, nullptr, 0, 0,
       "Glob describing an archive (or an object file within --start-lib) which should be ignored for --warn-backrefs.", "<glob>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("warn-backrefs-exclude"), warn_backrefs_exclude, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<glob>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("warn-backrefs"), warn_backrefs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about backward symbol references to extract archive members", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("warn-common"), warn_common, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about duplicate common symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("warn-execstack"), anonymous_79, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("warn-ifunc-textrel"), warn_ifunc_textrel, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about using ifunc symbols with text relocations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("warn-once"), anonymous_80, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("warn-shared-textrel"), anonymous_81, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("warn-symbol-ordering"), warn_symbol_ordering, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Warn about problems with the symbol ordering file (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("warn-unresolved-symbols"), warn_unresolved_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Report unresolved symbols as warnings", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("whole-archive"), whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Force load of all members in a static library", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("why-extract="), why_extract, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Print to a file about why archive members are extracted", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("wrap="), wrap_eq, Joined, INVALID, wrap, nullptr, 0, 0,
       "Redirect symbol references to __wrap_symbol and __real_symbol references to symbol", "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("wrap"), wrap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("w"), anonymous_3, Flag, INVALID, no_warnings, nullptr, 0, 0,
       "Alias for --no-warnings", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("X"), anonymous_12, Flag, INVALID, discard_locals, nullptr, 0, 0,
       "Alias for --discard-locals", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("x"), anonymous_11, Flag, INVALID, discard_all, nullptr, 0, 0,
       "Alias for --discard-all", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("y"), anonymous_39, JoinedOrSeparate, INVALID, trace_symbol, nullptr, 0, 0,
       "Alias for --trace-symbol", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("z"), z, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
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
static const unsigned SimpleEnumValueTablesSize = std::size(SimpleEnumValueTables);
#endif // SIMPLE_ENUM_VALUE_TABLE


