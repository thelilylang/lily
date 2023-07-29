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
PREFIX(prefix_3, {"-" COMMA "--" COMMA nullptr})
PREFIX(prefix_2, {"--" COMMA nullptr})
#undef COMMA
#endif // PREFIX

/////////
// Groups

#ifdef OPTION
OPTION(nullptr, "bitcode", grp_bitcode, Group, INVALID, INVALID, nullptr, 0, 0,
       "BITCODE BUILD FLOW", nullptr, nullptr)
OPTION(nullptr, "bundle", grp_bundle, Group, INVALID, INVALID, nullptr, 0, 0,
       "CREATING A BUNDLE", nullptr, nullptr)
OPTION(nullptr, "content", grp_content, Group, INVALID, INVALID, nullptr, 0, 0,
       "ADDITIONAL CONTENT", nullptr, nullptr)
OPTION(nullptr, "deprecated", grp_deprecated, Group, INVALID, INVALID, nullptr, 0, 0,
       "DEPRECATED", nullptr, nullptr)
OPTION(nullptr, "dylib", grp_dylib, Group, INVALID, INVALID, nullptr, 0, 0,
       "DYNAMIC LIBRARIES (DYLIB)", nullptr, nullptr)
OPTION(nullptr, "ignored", grp_ignored, Group, INVALID, INVALID, nullptr, 0, 0,
       "IGNORED", nullptr, nullptr)
OPTION(nullptr, "ignored_silently", grp_ignored_silently, Group, INVALID, INVALID, nullptr, 0, 0,
       "IGNORED SILENTLY", nullptr, nullptr)
OPTION(nullptr, "introspect", grp_introspect, Group, INVALID, INVALID, nullptr, 0, 0,
       "INTROSPECTING THE LINKER", nullptr, nullptr)
OPTION(nullptr, "kind", grp_kind, Group, INVALID, INVALID, nullptr, 0, 0,
       "OUTPUT KIND", nullptr, nullptr)
OPTION(nullptr, "libs", grp_libs, Group, INVALID, INVALID, nullptr, 0, 0,
       "LIBRARIES", nullptr, nullptr)
OPTION(nullptr, "kind", grp_lld, Group, INVALID, INVALID, nullptr, 0, 0,
       "LLD-SPECIFIC", nullptr, nullptr)
OPTION(nullptr, "main", grp_main, Group, INVALID, INVALID, nullptr, 0, 0,
       "MAIN EXECUTABLE", nullptr, nullptr)
OPTION(nullptr, "object", grp_object, Group, INVALID, INVALID, nullptr, 0, 0,
       "CREATING AN OBJECT FILE", nullptr, nullptr)
OPTION(nullptr, "obsolete", grp_obsolete, Group, INVALID, INVALID, nullptr, 0, 0,
       "OBSOLETE", nullptr, nullptr)
OPTION(nullptr, "opts", grp_opts, Group, INVALID, INVALID, nullptr, 0, 0,
       "OPTIMIZATIONS", nullptr, nullptr)
OPTION(nullptr, "rare", grp_rare, Group, INVALID, INVALID, nullptr, 0, 0,
       "RARELY USED", nullptr, nullptr)
OPTION(nullptr, "resolve", grp_resolve, Group, INVALID, INVALID, nullptr, 0, 0,
       "SYMBOL RESOLUTION", nullptr, nullptr)
OPTION(nullptr, "symtab", grp_symtab, Group, INVALID, INVALID, nullptr, 0, 0,
       "SYMBOL TABLE", nullptr, nullptr)
OPTION(nullptr, "undocumented", grp_undocumented, Group, INVALID, INVALID, nullptr, 0, 0,
       "UNDOCUMENTED", nullptr, nullptr)
OPTION(nullptr, "version", grp_version, Group, INVALID, INVALID, nullptr, 0, 0,
       "VERSION TARGETING", nullptr, nullptr)

//////////
// Options

OPTION(prefix_0, &"<input>"[0], INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, &"<unknown>"[0], UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-add_ast_path"[1], add_ast_path, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "AST paths will be emitted as STABS", "<path>", nullptr)
OPTION(prefix_1, &"-add_empty_section"[1], add_empty_section, MultiArg, grp_content, INVALID, nullptr, 0, 2,
       "Create an empty <section> in <segment>", "<segment> <section>", nullptr)
OPTION(prefix_1, &"-add_linker_option"[1], add_linker_option, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-add_source_version"[1], add_source_version, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-add_split_seg_info"[1], add_split_seg_info, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-adhoc_codesign"[1], adhoc_codesign, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Write an ad-hoc code signature to the output file (default for arm64 binaries)", nullptr, nullptr)
OPTION(prefix_1, &"-alias_list"[1], alias_list, Separate, grp_resolve, INVALID, nullptr, HelpHidden, 0,
       "Create symbol aliases specified in <file>", "<file>", nullptr)
OPTION(prefix_1, &"-alias"[1], alias, MultiArg, grp_resolve, INVALID, nullptr, 0, 2,
       "Create a symbol alias with default global visibility", "<symbol_name> <alternate_name>", nullptr)
OPTION(prefix_1, &"-all_load"[1], all_load, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Load all members of all static archive libraries", nullptr, nullptr)
OPTION(prefix_1, &"-allow_dead_duplicates"[1], allow_dead_duplicates, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-allow_heap_execute"[1], allow_heap_execute, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "On i386, allow any page to execute code", nullptr, nullptr)
OPTION(prefix_1, &"-allow_simulator_linking_to_macosx_dylibs"[1], allow_simulator_linking_to_macosx_dylibs, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-allow_stack_execute"[1], allow_stack_execute, Flag, grp_main, INVALID, nullptr, HelpHidden, 0,
       "Mark stack segment as executable", nullptr, nullptr)
OPTION(prefix_1, &"-allow_sub_type_mismatches"[1], allow_sub_type_mismatches, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Permit mixing objects compiled for different ARM CPU subtypes", nullptr, nullptr)
OPTION(prefix_1, &"-allowable_client"[1], allowable_client, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specify <name> of a dylib or framework that is allowed to link to this dylib", "<name>", nullptr)
OPTION(prefix_1, &"-application_extension"[1], application_extension, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Mark output as safe for use in an application extension, and validate that linked dylibs are safe", nullptr, nullptr)
OPTION(prefix_1, &"-arch_errors_fatal"[1], arch_errors_fatal, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Escalate to errors any warnings about inputs whose architecture does not match the -arch option", nullptr, nullptr)
OPTION(prefix_1, &"-arch_multiple"[1], arch_multiple, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Augment error and warning messages with the architecture name", nullptr, nullptr)
OPTION(prefix_1, &"-arch"[1], arch, Separate, grp_kind, INVALID, nullptr, 0, 0,
       "The architecture (e.g. ppc, ppc64, i386, x86_64)", "<arch_name>", nullptr)
OPTION(prefix_1, &"-A"[1], A, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<basefile>", nullptr)
OPTION(prefix_1, &"-bind_at_load"[1], bind_at_load, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Tell dyld to bind all symbols at load time, rather than lazily", nullptr, nullptr)
OPTION(prefix_1, &"-bitcode_bundle"[1], bitcode_bundle, Flag, grp_bitcode, INVALID, nullptr, 0, 0,
       "Generate an embedded bitcode bundle in the __LLVM,__bundle section of the output", nullptr, nullptr)
OPTION(prefix_1, &"-bitcode_hide_symbols"[1], bitcode_hide_symbols, Flag, grp_bitcode, INVALID, nullptr, HelpHidden, 0,
       "With -bitcode_bundle, hide all non-exported symbols from output bitcode bundle.", nullptr, nullptr)
OPTION(prefix_1, &"-bitcode_process_mode"[1], bitcode_process_mode, Separate, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-bitcode_symbol_map"[1], bitcode_symbol_map, Separate, grp_bitcode, INVALID, nullptr, HelpHidden, 0,
       "Write the bitcode symbol reverse mapping to file <path>, or if a directory, to <path>/UUID.bcsymbolmap", "<path>", nullptr)
OPTION(prefix_1, &"-bitcode_verify"[1], bitcode_verify, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-bridgeos_version_min"[1], bridgeos_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest bridgeOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-bundle_loader"[1], bundle_loader, Separate, grp_bundle, INVALID, nullptr, 0, 0,
       "Resolve undefined symbols from <executable>", "<executable>", nullptr)
OPTION(prefix_1, &"-bundle"[1], bundle, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Produce a bundle", nullptr, nullptr)
OPTION(prefix_1, &"-b"[1], b, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-cache_path_lto"[1], cache_path_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Use <path> as a directory for the incremental LTO cache", "<path>", nullptr)
OPTION(prefix_2, &"--call-graph-profile-sort"[2], call_graph_profile_sort, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Reorder sections with call graph profile (default)", nullptr, nullptr)
OPTION(prefix_1, &"-classic_linker"[1], classic_linker, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-client_name"[1], client_name, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specifies a <name> this client should match with the -allowable_client <name> in a dependent dylib", "<name>", nullptr)
OPTION(prefix_2, &"--color-diagnostics="[2], color_diagnostics_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_2, &"--color-diagnostics"[2], color_diagnostics, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_1, &"-commons"[1], commons, Separate, grp_resolve, INVALID, nullptr, HelpHidden, 0,
       "Resolve tentative definitions in dylibs according to <treatment>: ignore_dylibs, use_dylibs, error (default is ignore_dylibs)", "<treatment>", nullptr)
OPTION(prefix_1, &"-compatibility_version"[1], compatibility_version, Separate, grp_dylib, INVALID, nullptr, 0, 0,
       "Compatibility <version> of this library", "<version>", nullptr)
OPTION(prefix_1, &"-current_version"[1], current_version, Separate, grp_dylib, INVALID, nullptr, 0, 0,
       "Current <version> of this library", "<version>", nullptr)
OPTION(prefix_1, &"-data_const"[1], data_const, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Force migration of readonly data into __DATA_CONST segment", nullptr, nullptr)
OPTION(prefix_1, &"-data_in_code_info"[1], data_in_code_info, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Emit data-in-code information (default)", nullptr, nullptr)
OPTION(prefix_1, &"-dead_strip_dylibs"[1], dead_strip_dylibs, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Remove dylibs that are unreachable by the entry point or exported symbols", nullptr, nullptr)
OPTION(prefix_1, &"-dead_strip"[1], dead_strip, Flag, grp_opts, INVALID, nullptr, 0, 0,
       "Remove unreachable functions and data", nullptr, nullptr)
OPTION(prefix_1, &"-debug_snapshot"[1], debug_snapshot, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-debug_variant"[1], debug_variant, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Suppress warnings germane to binaries shipping to customers", nullptr, nullptr)
OPTION(prefix_2, &"--deduplicate-literals"[2], deduplicate_literals, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Enable literal deduplication. This is implied by --icf={safe,all}", nullptr, nullptr)
OPTION(prefix_1, &"-demangle"[1], demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names in diagnostics", nullptr, nullptr)
OPTION(prefix_1, &"-dependency_info"[1], dependency_info, Separate, grp_introspect, INVALID, nullptr, 0, 0,
       "Dump dependency info", "<path>", nullptr)
OPTION(prefix_1, &"-dependent_dr_info"[1], dependent_dr_info, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-dirty_data_list"[1], dirty_data_list, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specify data symbols in <path> destined for the __DATA_DIRTY segment", "<path>", nullptr)
OPTION(prefix_1, &"-dot"[1], dot, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Write a graph of symbol dependencies to <path> as a .dot file viewable with GraphViz", "<path>", nullptr)
OPTION(prefix_1, &"-driverkit_version_min"[1], driverkit_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest DriverKit version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-dtrace"[1], dtrace, Separate, grp_content, INVALID, nullptr, HelpHidden, 0,
       "Enable DTrace static probes according to declarations in <script>", "<script>", nullptr)
OPTION(prefix_1, &"-dyld_env"[1], dyld_env, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-dylib_compatibility_version"[1], dylib_compatibility_version, Separate, grp_dylib, compatibility_version, nullptr, HelpHidden, 0,
       "Alias for -compatibility_version", "<version>", nullptr)
OPTION(prefix_1, &"-dylib_current_version"[1], dylib_current_version, Separate, grp_dylib, current_version, nullptr, HelpHidden, 0,
       "Alias for -current_version", "<version>", nullptr)
OPTION(prefix_1, &"-dylib_file"[1], dylib_file, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specify <current_path> as different from where a dylib normally resides at <install_path>", "<install_path:current_path>", nullptr)
OPTION(prefix_1, &"-dylib_install_name"[1], dylib_install_name, Separate, grp_dylib, install_name, nullptr, 0, 0,
       "Alias for -install_name", "<name>", nullptr)
OPTION(prefix_1, &"-dylib"[1], dylib, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Produce a shared library", nullptr, nullptr)
OPTION(prefix_1, &"-dylinker_install_name"[1], dylinker_install_name, Separate, grp_dylib, install_name, nullptr, 0, 0,
       "Alias for -install_name", "<name>", nullptr)
OPTION(prefix_1, &"-dylinker"[1], dylinker, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Produce a dylinker only used when building dyld", nullptr, nullptr)
OPTION(prefix_1, &"-dynamic"[1], dynamic, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Link dynamically (default)", nullptr, nullptr)
OPTION(prefix_1, &"-d"[1], d, Flag, grp_object, INVALID, nullptr, HelpHidden, 0,
       "Force tentative into real definitions for common symbols", nullptr, nullptr)
OPTION(prefix_1, &"-encryptable"[1], encryptable, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Generate the LC_ENCRYPTION_INFO load command", nullptr, nullptr)
OPTION(prefix_2, &"--end-lib"[2], end_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "End a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_2, &"--error-limit="[2], error_limit_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Maximum number of errors to print before exiting (default: 20)", nullptr, nullptr)
OPTION(prefix_1, &"-executable_path"[1], executable_path, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-execute"[1], execute, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Produce a main executable (default)", nullptr, nullptr)
OPTION(prefix_1, &"-export_dynamic"[1], export_dynamic, Flag, grp_main, INVALID, nullptr, 0, 0,
       "Preserve all global symbols during LTO and when dead-stripping executables", nullptr, nullptr)
OPTION(prefix_1, &"-exported_symbols_list"[1], exported_symbols_list, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Symbols specified in <file> remain global, while others become private externs", "<file>", nullptr)
OPTION(prefix_1, &"-exported_symbols_order"[1], exported_symbols_order, Separate, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Specify frequently-used symbols in <file> to optimize symbol exports", "<file>", nullptr)
OPTION(prefix_1, &"-exported_symbol"[1], exported_symbol, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "<symbol> remains global, while others become private externs", "<symbol>", nullptr)
OPTION(prefix_1, &"-e"[1], e, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Make <symbol> the entry point of an executable (default is \"start\" from crt1.o)", "<symbol>", nullptr)
OPTION(prefix_1, &"-fatal_warnings"[1], fatal_warnings, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_1, &"-filelist"[1], filelist, Separate, grp_content, INVALID, nullptr, 0, 0,
       "Read names of files to link from <file>", "<file>", nullptr)
OPTION(prefix_1, &"-final_output"[1], final_output, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Specify dylib install name if -install_name is not used; used by compiler driver for multiple -arch arguments", "<name>", nullptr)
OPTION(prefix_1, &"-fixup_chains_section"[1], fixup_chains_section, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-fixup_chains"[1], fixup_chains, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-flat_namespace"[1], flat_namespace, Flag, grp_resolve, INVALID, nullptr, 0, 0,
       "Resolve symbols from all dylibs, both direct and transitive. Do not record source libraries: dyld must re-search at runtime and use the first definition found", nullptr, nullptr)
OPTION(prefix_1, &"-flto-codegen-only"[1], flto_codegen_only, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-force_cpusubtype_ALL"[1], force_cpusubtype_ALL, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Mark binary as runnable on any PowerPC, ignoring any PowerPC cpu requirements encoded in the object files", nullptr, nullptr)
OPTION(prefix_1, &"-force_flat_namespace"[1], force_flat_namespace, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Tell dyld to use a flat namespace on this executable and all its dependent dylibs & bundles", nullptr, nullptr)
OPTION(prefix_1, &"-force_load_swift_libs"[1], force_load_swift_libs, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Apply -force_load to libraries listed in LC_LINKER_OPTIONS whose names start with 'swift'", nullptr, nullptr)
OPTION(prefix_1, &"-force_load"[1], force_load, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Load all members static archive library at <path>", "<path>", nullptr)
OPTION(prefix_1, &"-force_symbol_not_weak"[1], force_symbol_not_weak, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-force_symbol_weak"[1], force_symbol_weak, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-force_symbols_coalesce_list"[1], force_symbols_coalesce_list, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-force_symbols_not_weak_list"[1], force_symbols_not_weak_list, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-force_symbols_weak_list"[1], force_symbols_weak_list, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-framework"[1], framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Search for <name>.framework/<name> on the framework search path", "<name>", nullptr)
OPTION(prefix_1, &"-function_starts"[1], function_starts, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Create table of function start addresses (default)", nullptr, nullptr)
OPTION(prefix_1, &"-fvmlib"[1], fvmlib, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-F"[1], F, JoinedOrSeparate, grp_libs, INVALID, nullptr, 0, 0,
       "Add dir to the framework search path", "<dir>", nullptr)
OPTION(prefix_1, &"-headerpad_max_install_names"[1], headerpad_max_install_names, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Allocate extra space so all load-command paths can expand to MAXPATHLEN via install_name_tool", nullptr, nullptr)
OPTION(prefix_1, &"-headerpad"[1], headerpad, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Allocate hex <size> extra space for future expansion of the load commands via install_name_tool (default is 0x20)", "<size>", nullptr)
OPTION(prefix_2, &"--help-hidden"[2], help_hidden, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Display help for hidden options", nullptr, nullptr)
OPTION(prefix_3, &"-help"[1], help, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-hidden-l"[1], hidden_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but load all symbols with hidden visibility", "<name>", nullptr)
OPTION(prefix_2, &"--icf="[2], icf_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Set level for identical code folding (default: none)", "[none,safe,all]", nullptr)
OPTION(prefix_1, &"-ignore_auto_link"[1], ignore_auto_link, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-ignore_optimization_hints"[1], ignore_optimization_hints, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Ignore Linker Optimization Hints", nullptr, nullptr)
OPTION(prefix_1, &"-image_base"[1], image_base, Separate, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Preferred hex load address for a dylib or bundle.", "<address>", nullptr)
OPTION(prefix_1, &"-init_offsets"[1], init_offsets, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-init"[1], init, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Run <symbol> as the first initializer in a dylib", "<symbol>", nullptr)
OPTION(prefix_1, &"-install_name"[1], install_name, Separate, grp_dylib, INVALID, nullptr, 0, 0,
       "Set an internal install path in a dylib", "<name>", nullptr)
OPTION(prefix_1, &"-interposable_list"[1], interposable_list, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Access global symbols listed in <path> indirectly", "<path>", nullptr)
OPTION(prefix_1, &"-interposable"[1], interposable, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Indirects access to all to exported symbols in a dylib", nullptr, nullptr)
OPTION(prefix_1, &"-ios_simulator_version_min"[1], ios_simulator_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest iOS simulator version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-ios_version_min"[1], ios_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest iOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-iosmac_version_min"[1], iosmac_version_min, Separate, grp_version, maccatalyst_version_min, nullptr, HelpHidden, 0,
       "Alias for -maccatalyst_version_min", "<version>", nullptr)
OPTION(prefix_1, &"-iphoneos_version_min"[1], iphoneos_version_min, Separate, grp_version, ios_version_min, nullptr, HelpHidden, 0,
       "Alias for -ios_version_min", "<version>", nullptr)
OPTION(prefix_1, &"-i"[1], i, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-keep_dwarf_unwind"[1], keep_dwarf_unwind, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-keep_private_externs"[1], keep_private_externs, Flag, grp_object, INVALID, nullptr, HelpHidden, 0,
       "Do not convert private external symbols to static symbols (only valid with -r)", nullptr, nullptr)
OPTION(prefix_1, &"-keep_relocs"[1], keep_relocs, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Retain section-based relocation records in the output, which are ignored at runtime by dyld", nullptr, nullptr)
OPTION(prefix_1, &"-kext_objects_dir"[1], kext_objects_dir, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-kexts_use_stubs"[1], kexts_use_stubs, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-kext"[1], kext, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-lazy-l"[1], lazy_l, Joined, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "This option is deprecated and is now an alias for -l<path>.", "<name>", nullptr)
OPTION(prefix_1, &"-lazy_framework"[1], lazy_framework, Separate, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "This option is deprecated and is now an alias for -framework.", "<name>", nullptr)
OPTION(prefix_1, &"-lazy_library"[1], lazy_library, Separate, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "This option is deprecated and is now an alias for regular linking", "<path>", nullptr)
OPTION(prefix_1, &"-load_hidden"[1], load_hidden, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Load all symbols from static library with hidden visibility", "<path>", nullptr)
OPTION(prefix_2, &"--lto-O"[2], lto_O, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Set optimization level for LTO (default: 2)", "<opt-level>", nullptr)
OPTION(prefix_1, &"-lto_library"[1], lto_library, Separate, grp_obsolete, INVALID, nullptr, 0, 0,
       "Obsolete. LLD supports LTO directly, without using an external dylib.", "<path>", nullptr)
OPTION(prefix_1, &"-L"[1], L, JoinedOrSeparate, grp_libs, INVALID, nullptr, 0, 0,
       "Add dir to the library search path", "<dir>", nullptr)
OPTION(prefix_1, &"-l"[1], l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Search for lib<name>.dylib or lib<name>.a on the library search path", "<name>", nullptr)
OPTION(prefix_1, &"-maccatalyst_version_min"[1], maccatalyst_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest MacCatalyst version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-macos_version_min"[1], macos_version_min, Separate, grp_version, INVALID, nullptr, 0, 0,
       "Oldest macOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-macosx_version_min"[1], macosx_version_min, Separate, grp_version, macos_version_min, nullptr, HelpHidden, 0,
       "Alias for -macos_version_min", "<version>", nullptr)
OPTION(prefix_1, &"-map"[1], map, Separate, grp_introspect, INVALID, nullptr, 0, 0,
       "Writes all symbols and their addresses to <path>", "<path>", nullptr)
OPTION(prefix_1, &"-mark_dead_strippable_dylib"[1], mark_dead_strippable_dylib, Flag, grp_dylib, INVALID, nullptr, 0, 0,
       "Mark output dylib as dead-strippable: When a client links against it but does not use any of its symbols, the dylib will not be added to the client's list of needed dylibs", nullptr, nullptr)
OPTION(prefix_1, &"-max_default_common_align"[1], max_default_common_align, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Reduce maximum alignment for common symbols to a hex power-of-2 <boundary>", "<boundary>", nullptr)
OPTION(prefix_1, &"-max_relative_cache_size_lto"[1], max_relative_cache_size_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Limit the incremental LTO cache growth to <percent> of free disk, space", "<percent>", nullptr)
OPTION(prefix_1, &"-mcpu"[1], mcpu, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Processor family target for LTO code generation", nullptr, nullptr)
OPTION(prefix_1, &"-merge_zero_fill_sections"[1], merge_zero_fill_sections, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Merge all zeroed data into the __zerofill section", nullptr, nullptr)
OPTION(prefix_1, &"-mllvm"[1], mllvm, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Options to pass to LLVM", nullptr, nullptr)
OPTION(prefix_1, &"-move_to_ro_segment"[1], move_to_ro_segment, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Move code symbols listed in <path> to another <segment>", "<segment> <path>", nullptr)
OPTION(prefix_1, &"-move_to_rw_segment"[1], move_to_rw_segment, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Move data symbols listed in <path> to another <segment>", "<segment> <path>", nullptr)
OPTION(prefix_1, &"-multi_module"[1], multi_module, Flag, grp_rare, interposable, nullptr, HelpHidden, 0,
       "Alias for -interposable", nullptr, nullptr)
OPTION(prefix_1, &"-multiply_defined_unused"[1], multiply_defined_unused, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<treatment>", nullptr)
OPTION(prefix_1, &"-multiply_defined"[1], multiply_defined, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<treatment>", nullptr)
OPTION(prefix_1, &"-M"[1], M, Flag, grp_ignored, INVALID, nullptr, HelpHidden, 0,
       "This option is ignored in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-m"[1], m, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-needed-l"[1], needed_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but link library even if its symbols are not used and -dead_strip_dylibs is active", "<name>", nullptr)
OPTION(prefix_1, &"-needed_framework"[1], needed_framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like -framework <name>, but link <name> even if none of its symbols are used and -dead_strip_dylibs is active", "<name>", nullptr)
OPTION(prefix_1, &"-needed_library"[1], needed_library, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like bare <path>, but link library even if its symbols are not used and -dead_strip_dylibs is active", "<path>", nullptr)
OPTION(prefix_1, &"-new_linker"[1], new_linker, Flag, grp_ignored, INVALID, nullptr, HelpHidden, 0,
       "This option is ignored in ld64", nullptr, nullptr)
OPTION(prefix_2, &"--no-call-graph-profile-sort"[2], no_call_graph_profile_sort, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Do not reorder sections with call graph profile", nullptr, nullptr)
OPTION(prefix_2, &"--no-color-diagnostics"[2], no_color_diagnostics, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_2, &"--no-lto-legacy-pass-manager"[2], no_lto_legacy_pass_manager, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Use the new pass manager in LLVM", nullptr, nullptr)
OPTION(prefix_2, &"--no-warn-dylib-install-name"[2], no_warn_dylib_install_name, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Do not warn on -install-name if -dylib is not passed (default)", nullptr, nullptr)
OPTION(prefix_1, &"-no_adhoc_codesign"[1], no_adhoc_codesign, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Do not write an ad-hoc code signature to the output file (default for x86_64 binaries)", nullptr, nullptr)
OPTION(prefix_1, &"-no_application_extension"[1], no_application_extension, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Disable application extension functionality (default)", nullptr, nullptr)
OPTION(prefix_1, &"-no_arch_warnings"[1], no_arch_warnings, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Suppresses warnings about inputs whose architecture does not match the -arch option", nullptr, nullptr)
OPTION(prefix_1, &"-no_branch_islands"[1], no_branch_islands, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Disable infra for branches beyond the maximum branch distance.", nullptr, nullptr)
OPTION(prefix_1, &"-no_compact_linkedit"[1], no_compact_linkedit, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_compact_unwind"[1], no_compact_unwind, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_data_const"[1], no_data_const, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Block migration of readonly data away from __DATA segment", nullptr, nullptr)
OPTION(prefix_1, &"-no_data_in_code_info"[1], no_data_in_code_info, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Do not emit data-in-code information", nullptr, nullptr)
OPTION(prefix_1, &"-no_dead_strip_inits_and_terms"[1], no_dead_strip_inits_and_terms, Flag, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "Unnecessary option: initialization and termination are roots of the dead strip graph, so never dead stripped", nullptr, nullptr)
OPTION(prefix_1, &"-no_deduplicate"[1], no_deduplicate, Flag, grp_opts, icf_eq, "none\0", 0, 0,
       "Disable code deduplicaiton (synonym for `--icf=none')", nullptr, nullptr)
OPTION(prefix_1, &"-no_dependent_dr_info"[1], no_dependent_dr_info, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_dtrace_dof"[1], no_dtrace_dof, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Disable dtrace-dof processing (default).", nullptr, nullptr)
OPTION(prefix_1, &"-no_eh_labels"[1], no_eh_labels, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "In -r mode, suppress .eh labels in the __eh_frame section", nullptr, nullptr)
OPTION(prefix_1, &"-no_encryption"[1], no_encryption, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Do not generate the LC_ENCRYPTION_INFO load command", nullptr, nullptr)
OPTION(prefix_1, &"-no_exported_symbols"[1], no_exported_symbols, Flag, grp_resolve, INVALID, nullptr, 0, 0,
       "Don't export any symbols from the binary, useful for main executables that don't have plugins", nullptr, nullptr)
OPTION(prefix_1, &"-no_function_starts"[1], no_function_starts, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Do not create table of function start addresses", nullptr, nullptr)
OPTION(prefix_1, &"-no_implicit_dylibs"[1], no_implicit_dylibs, Flag, grp_opts, INVALID, nullptr, 0, 0,
       "Do not optimize public dylib transitive symbol references", nullptr, nullptr)
OPTION(prefix_1, &"-no_inits"[1], no_inits, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Fail if the output contains static initializers", nullptr, nullptr)
OPTION(prefix_1, &"-no_keep_dwarf_unwind"[1], no_keep_dwarf_unwind, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_kext_objects"[1], no_kext_objects, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_new_main"[1], no_new_main, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_objc_category_merging"[1], no_objc_category_merging, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Do not merge Objective-C categories into their classes", nullptr, nullptr)
OPTION(prefix_1, &"-no_order_data"[1], no_order_data, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Disable default reordering of global data accessed at launch time", nullptr, nullptr)
OPTION(prefix_1, &"-no_order_inits"[1], no_order_inits, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Disable default reordering of initializer and terminator functions", nullptr, nullptr)
OPTION(prefix_1, &"-no_pie"[1], no_pie, Flag, grp_main, INVALID, nullptr, 0, 0,
       "Do not build a position independent executable", nullptr, nullptr)
OPTION(prefix_1, &"-no_source_version"[1], no_source_version, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-no_uuid"[1], no_uuid, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Do not generate the LC_UUID load command", nullptr, nullptr)
OPTION(prefix_1, &"-no_warn_inits"[1], no_warn_inits, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Suppress warnings for static initializers in the output", nullptr, nullptr)
OPTION(prefix_1, &"-no_weak_exports"[1], no_weak_exports, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Fail if the linked image contains weak external symbols", nullptr, nullptr)
OPTION(prefix_1, &"-no_weak_imports"[1], no_weak_imports, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Fail if any symbols are weak imports, allowed to be NULL at runtime", nullptr, nullptr)
OPTION(prefix_1, &"-no_zero_fill_sections"[1], no_zero_fill_sections, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Explicitly store zeroed data in the final image", nullptr, nullptr)
OPTION(prefix_1, &"-noall_load"[1], noall_load, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Don't load all static members from archives, this is the default, this negates -all_load", nullptr, nullptr)
OPTION(prefix_1, &"-nofixprebinding"[1], nofixprebinding, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-nomultidefs"[1], nomultidefs, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-non_global_symbols_no_strip_list"[1], non_global_symbols_no_strip_list, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "Specify in <path> the non-global symbols that should remain in the output symbol table", "<path>", nullptr)
OPTION(prefix_1, &"-non_global_symbols_strip_list"[1], non_global_symbols_strip_list, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "Specify in <path> the non-global symbols that should be removed from the output symbol table", "<path>", nullptr)
OPTION(prefix_1, &"-noprebind_all_twolevel_modules"[1], noprebind_all_twolevel_modules, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-noprebind"[1], noprebind, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-noseglinkedit"[1], noseglinkedit, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-not_for_dyld_shared_cache"[1], not_for_dyld_shared_cache, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Prevent system dylibs from being placed into the dylib shared cache", nullptr, nullptr)
OPTION(prefix_1, &"-objc_abi_version"[1], objc_abi_version, Separate, grp_ignored_silently, INVALID, nullptr, HelpHidden, 0,
       "This option only applies to i386 in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-objc_gc_compaction"[1], objc_gc_compaction, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Mark the Objective-C image as compatible with compacting garbage collection", nullptr, nullptr)
OPTION(prefix_1, &"-objc_gc_only"[1], objc_gc_only, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Verify that all code was compiled with -fobjc-gc-only", nullptr, nullptr)
OPTION(prefix_1, &"-objc_gc"[1], objc_gc, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Verify that all code was compiled with -fobjc-gc or -fobjc-gc-only", nullptr, nullptr)
OPTION(prefix_1, &"-ObjC"[1], ObjC, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Load all members of static archives that are an Objective-C class or category.", nullptr, nullptr)
OPTION(prefix_1, &"-object_path_lto"[1], object_path_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Retain any temporary mach-o file in <path> that would otherwise be deleted during LTO", "<path>", nullptr)
OPTION(prefix_1, &"-order_file_statistics"[1], order_file_statistics, Flag, grp_introspect, INVALID, nullptr, HelpHidden, 0,
       "Logs information about -order_file", nullptr, nullptr)
OPTION(prefix_1, &"-order_file"[1], order_file, Separate, grp_opts, INVALID, nullptr, 0, 0,
       "Layout functions and data according to specification in <file>", "<file>", nullptr)
OPTION(prefix_1, &"-oso_prefix"[1], oso_prefix, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "Remove the prefix <path> from OSO symbols in the debug map", "<path>", nullptr)
OPTION(prefix_1, &"-O"[1], O, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Optimize output file size", nullptr, nullptr)
OPTION(prefix_1, &"-o"[1], o, Separate, grp_kind, INVALID, nullptr, 0, 0,
       "The name of the output file (default: `a.out')", "<path>", nullptr)
OPTION(prefix_1, &"-page_align_data_atoms"[1], page_align_data_atoms, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Distribute global variables on separate pages so page used/dirty status can guide creation of an order file to cluster commonly used/dirty globals", nullptr, nullptr)
OPTION(prefix_1, &"-pagezero_size"[1], pagezero_size, Separate, grp_main, INVALID, nullptr, 0, 0,
       "Size of unreadable segment at address zero is hex <size> (default is 4KB on 32-bit and 4GB on 64-bit)", "<size>", nullptr)
OPTION(prefix_1, &"-pause"[1], pause, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-pie"[1], pie, Flag, grp_main, INVALID, nullptr, 0, 0,
       "Build a position independent executable (default)", nullptr, nullptr)
OPTION(prefix_1, &"-platform_version"[1], platform_version, MultiArg, grp_version, INVALID, nullptr, 0, 3,
       "Platform (e.g., macos, ios, tvos, watchos, bridgeos, mac-catalyst, ios-sim, tvos-sim, watchos-sim, driverkit) and version numbers", "<platform> <min_version> <sdk_version>", nullptr)
OPTION(prefix_1, &"-prebind_all_twolevel_modules"[1], prebind_all_twolevel_modules, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-prebind_allow_overlap"[1], prebind_allow_overlap, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-prebind"[1], prebind, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-preload"[1], preload, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Produce an unsegmented binary for embedded systems", nullptr, nullptr)
OPTION(prefix_2, &"--print-dylib-search"[2], print_dylib_search, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Print which paths lld searched when trying to find dylibs", nullptr, nullptr)
OPTION(prefix_2, &"--print-symbol-order="[2], print_symbol_order, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Print a symbol order specified by --call-graph-profile-sort into the specified file", nullptr, nullptr)
OPTION(prefix_1, &"-print_statistics"[1], print_statistics, Flag, grp_introspect, INVALID, nullptr, HelpHidden, 0,
       "Log the linker's memory and CPU usage", nullptr, nullptr)
OPTION(prefix_1, &"-private_bundle"[1], private_bundle, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-prune_after_lto"[1], prune_after_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Remove LTO cache entries after <seconds>", "<seconds>", nullptr)
OPTION(prefix_1, &"-prune_interval_lto"[1], prune_interval_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Prune the incremental LTO cache after <seconds> (-1 disables pruning)", "<seconds>", nullptr)
OPTION(prefix_1, &"-random_uuid"[1], random_uuid, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-read_only_relocs"[1], read_only_relocs, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Handle relocations that modify read-only pages according to <treatment> of warning, error, or suppress (i.e., allow)", "<treatment>", nullptr)
OPTION(prefix_1, &"-read_only_stubs"[1], read_only_stubs, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "On i386, make the __IMPORT segment of a final linked image read-only", nullptr, nullptr)
OPTION(prefix_1, &"-reexport-l"[1], reexport_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but export all symbols of <name> from newly created library", "<name>", nullptr)
OPTION(prefix_1, &"-reexport_framework"[1], reexport_framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like -framework <name>, but export all symbols of <name> from the newly created library", "<name>", nullptr)
OPTION(prefix_1, &"-reexport_library"[1], reexport_library, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like bare <path>, but export all symbols of <path> from newly created library", "<path>", nullptr)
OPTION(prefix_1, &"-reexported_symbols_list"[1], reexported_symbols_list, Separate, grp_resolve, INVALID, nullptr, HelpHidden, 0,
       "Symbols from dependent dylibs specified in <file> are reexported by this dylib", "<file>", nullptr)
OPTION(prefix_1, &"-rename_section"[1], rename_section, MultiArg, grp_rare, INVALID, nullptr, 0, 4,
       "Rename <from_segment>/<from_section> as <to_segment>/<to_section>", "<from_segment> <from_section> <to_segment> <to_section>", nullptr)
OPTION(prefix_1, &"-rename_segment"[1], rename_segment, MultiArg, grp_rare, INVALID, nullptr, 0, 2,
       "Rename <from_segment> as <to_segment>", "<from_segment> <to_segment>", nullptr)
OPTION(prefix_2, &"--reproduce="[2], reproduce_eq, Joined, grp_lld, reproduce, nullptr, 0, 0,
       "Write tar file containing inputs and command to reproduce link", nullptr, nullptr)
OPTION(prefix_2, &"--reproduce"[2], reproduce, Separate, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-root_safe"[1], root_safe, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Set the MH_ROOT_SAFE bit in the mach-o header", nullptr, nullptr)
OPTION(prefix_1, &"-rpath"[1], rpath, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Add <path> to dyld search list for dylibs with load path prefix `@rpath/'", "<path>", nullptr)
OPTION(prefix_1, &"-run_init_lazily"[1], run_init_lazily, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-r"[1], r, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Merge multiple object files into one, retaining relocations", nullptr, nullptr)
OPTION(prefix_1, &"-save-temps"[1], save_temps, Flag, grp_introspect, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_1, &"-sdk_version"[1], sdk_version, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-search_dylibs_first"[1], search_dylibs_first, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Search for lib<name>.dylib on first pass, then for lib<name>.a on second pass through search path (default for Xcode 3 and earlier)", nullptr, nullptr)
OPTION(prefix_1, &"-search_paths_first"[1], search_paths_first, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Search for lib<name>.dylib and lib<name>.a at each step in traversing search path (default for Xcode 4 and later)", nullptr, nullptr)
OPTION(prefix_1, &"-sect_diff_relocs"[1], sect_diff_relocs, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<treatment>", nullptr)
OPTION(prefix_1, &"-sectalign"[1], sectalign, MultiArg, grp_rare, INVALID, nullptr, 0, 3,
       "Align <section> within <segment> to hex power-of-2 <boundary>", "<segment> <section> <boundary>", nullptr)
OPTION(prefix_1, &"-sectcreate"[1], sectcreate, MultiArg, grp_content, INVALID, nullptr, 0, 3,
       "Create <section> in <segment> from the contents of <file>", "<segment> <section> <file>", nullptr)
OPTION(prefix_1, &"-section_order"[1], section_order, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "With -preload, specify layout sequence of colon-separated <sections> in <segment>", "<segment> <sections>", nullptr)
OPTION(prefix_1, &"-sectobjectsymbols"[1], sectobjectsymbols, MultiArg, grp_obsolete, INVALID, nullptr, HelpHidden, 2,
       "This option is obsolete in ld64", "<segname> <sectname>", nullptr)
OPTION(prefix_1, &"-sectorder_detail"[1], sectorder_detail, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-sectorder"[1], sectorder, MultiArg, grp_obsolete, INVALID, nullptr, 0, 3,
       "Obsolete. Replaced by more general -order_file option", "<segname> <sectname> <orderfile>", nullptr)
OPTION(prefix_1, &"-seg1addr"[1], seg1addr, Separate, grp_opts, image_base, nullptr, HelpHidden, 0,
       "Alias for -image_base", "<address>", nullptr)
OPTION(prefix_1, &"-seg_addr_table_filename"[1], seg_addr_table_filename, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<path>", nullptr)
OPTION(prefix_1, &"-seg_page_size"[1], seg_page_size, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Specifies the page <size> for <segment>. Segment size will be a multiple of its page size", "<segment> <size>", nullptr)
OPTION(prefix_1, &"-segaddr"[1], segaddr, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Specify the starting hex <address> at a 4KiB page boundary for <segment>", "<segment> <address>", nullptr)
OPTION(prefix_1, &"-segalign"[1], segalign, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Align all segments to hex power-of-2 <boundary>", "<boundary>", nullptr)
OPTION(prefix_1, &"-segcreate"[1], segcreate, MultiArg, grp_content, sectcreate, nullptr, 0, 3,
       "Alias for -sectcreate", "<segment> <section> <file>", nullptr)
OPTION(prefix_1, &"-seglinkedit"[1], seglinkedit, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-segment_order"[1], segment_order, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "With -preload, specify layout sequence of colon-separated <segments>", "<colon_separated_segment_list>", nullptr)
OPTION(prefix_1, &"-segprot"[1], segprot, MultiArg, grp_rare, INVALID, nullptr, 0, 3,
       "Specifies the <max> and <init> virtual memory protection of <segment> as r/w/x/-seg_addr_table path", "<segment> <max> <init>", nullptr)
OPTION(prefix_1, &"-segs_read_only_addr"[1], segs_read_only_addr, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete", "<address>", nullptr)
OPTION(prefix_1, &"-segs_read_write_addr"[1], segs_read_write_addr, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete", "<address>", nullptr)
OPTION(prefix_1, &"-setuid_safe"[1], setuid_safe, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Set the MH_SETUID_SAFE bit in the mach-o header", nullptr, nullptr)
OPTION(prefix_1, &"-simulator_support"[1], simulator_support, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-single_module"[1], single_module, Flag, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "Unnecessary option: this is already the default", nullptr, nullptr)
OPTION(prefix_1, &"-Si"[1], Si, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-slow_stubs"[1], slow_stubs, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-snapshot_dir"[1], snapshot_dir, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-Sn"[1], Sn, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-source_version"[1], source_version, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-Sp"[1], Sp, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-stack_addr"[1], stack_addr, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Initialize stack pointer to hex <address> rounded to a page boundary", "<address>", nullptr)
OPTION(prefix_1, &"-stack_size"[1], stack_size, Separate, grp_main, INVALID, nullptr, HelpHidden, 0,
       "Maximum hex stack size for the main thread in a program. (default is 8MB)", "<size>", nullptr)
OPTION(prefix_2, &"--start-lib"[2], start_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Start a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_1, &"-static"[1], static, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Link statically", nullptr, nullptr)
OPTION(prefix_1, &"-sub_library"[1], sub_library, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Re-export the dylib as <name>", "<name>", nullptr)
OPTION(prefix_1, &"-sub_umbrella"[1], sub_umbrella, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Re-export the framework as <name>", "<name>", nullptr)
OPTION(prefix_1, &"-syslibroot"[1], syslibroot, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Prepend <rootdir> to all library and framework search paths", "<rootdir>", nullptr)
OPTION(prefix_1, &"-S"[1], S, Flag, grp_symtab, INVALID, nullptr, 0, 0,
       "Strip debug information (STABS or DWARF) from the output", nullptr, nullptr)
OPTION(prefix_1, &"-s"[1], s, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-text_exec"[1], text_exec, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Rename __segment TEXT to __TEXT_EXEC for sections __text and __stubs", nullptr, nullptr)
OPTION(prefix_2, &"--thinlto-cache-policy="[2], thinlto_cache_policy, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_2, &"--thinlto-jobs="[2], thinlto_jobs_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Number of ThinLTO jobs. Default to --threads=", nullptr, nullptr)
OPTION(prefix_1, &"-threaded_starts_section"[1], threaded_starts_section, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_2, &"--threads="[2], threads_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_2, &"--time-trace-granularity="[2], time_trace_granularity_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Minimum time granularity (in microseconds) traced by time profiler", nullptr, nullptr)
OPTION(prefix_2, &"--time-trace="[2], time_trace_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Record time trace to <file>", "<file>", nullptr)
OPTION(prefix_2, &"--time-trace"[2], anonymous_0, Flag, grp_lld, time_trace_eq, nullptr, 0, 0,
       "Record time trace to file next to output", nullptr, nullptr)
OPTION(prefix_1, &"-trace_symbol_layout"[1], trace_symbol_layout, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Show where and why symbols move, as specified by -move_to_ro_segment, -move_to_rw_segment, -rename_section, and -rename_segment", nullptr, nullptr)
OPTION(prefix_1, &"-tvos_version_min"[1], tvos_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest tvOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-twolevel_namespace_hints"[1], twolevel_namespace_hints, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-twolevel_namespace"[1], twolevel_namespace, Flag, grp_resolve, INVALID, nullptr, 0, 0,
       "Make dyld look up symbols by (dylib,name) pairs (default)", nullptr, nullptr)
OPTION(prefix_1, &"-t"[1], t, Flag, grp_introspect, INVALID, nullptr, 0, 0,
       "Log every file the linker loads: object, archive, and dylib", nullptr, nullptr)
OPTION(prefix_1, &"-uikitformac_version_min"[1], uikitformac_version_min, Separate, grp_version, maccatalyst_version_min, nullptr, HelpHidden, 0,
       "Alias for -maccatalyst_version_min", "<version>", nullptr)
OPTION(prefix_1, &"-umbrella"[1], umbrella, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Re-export this dylib through the umbrella framework <name>", "<name>", nullptr)
OPTION(prefix_1, &"-unaligned_pointers"[1], unaligned_pointers, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Handle unaligned pointers in __DATA segments according to <treatment>: warning, error, or suppress (default for arm64e is error, otherwise suppress)", "<treatment>", nullptr)
OPTION(prefix_1, &"-undefined"[1], undefined, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Handle undefined symbols according to <treatment>: error, warning, suppress, or dynamic_lookup (default is error)", "<treatment>", nullptr)
OPTION(prefix_1, &"-unexported_symbols_list"[1], unexported_symbols_list, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Global symbols specified in <file> become private externs", "<file>", nullptr)
OPTION(prefix_1, &"-unexported_symbol"[1], unexported_symbol, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Global <symbol> becomes private extern", "<symbol>", nullptr)
OPTION(prefix_1, &"-upward-l"[1], upward_l, Joined, grp_libs, INVALID, nullptr, HelpHidden, 0,
       "Like -l<name>, but specify dylib as an upward dependency", "<name>", nullptr)
OPTION(prefix_1, &"-upward_framework"[1], upward_framework, Separate, grp_libs, INVALID, nullptr, HelpHidden, 0,
       "Like -framework <name>, but specify the framework as an upward dependency", "<name>", nullptr)
OPTION(prefix_1, &"-upward_library"[1], upward_library, Separate, grp_libs, INVALID, nullptr, HelpHidden, 0,
       "Like bare <path>, but specify dylib as an upward dependency", "<path>", nullptr)
OPTION(prefix_1, &"-U"[1], U, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Allow <symbol> to have no definition", "<symbol>", nullptr)
OPTION(prefix_1, &"-u"[1], u, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Require that <symbol> be defined for the link to succeed", "<symbol>", nullptr)
OPTION(prefix_1, &"-verbose_deduplicate"[1], verbose_deduplicate, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Print function names eliminated by deduplication and the total size of code savings", nullptr, nullptr)
OPTION(prefix_1, &"-verbose_optimization_hints"[1], verbose_optimization_hints, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_2, &"--verbose"[2], verbose, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-version_details"[1], version_details, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Print the linker version in JSON form", nullptr, nullptr)
OPTION(prefix_1, &"-version_load_command"[1], version_load_command, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_2, &"--version"[2], version, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_1, &"-v"[1], v, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Print the linker version and search paths in addition to linking", nullptr, nullptr)
OPTION(prefix_2, &"--warn-dylib-install-name"[2], warn_dylib_install_name, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Warn on -install-name if -dylib is not passed", nullptr, nullptr)
OPTION(prefix_1, &"-warn_commons"[1], warn_commons, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn when a tentative definition in an object file matches an external symbol in a dylib, which often means \"extern\" is missing from a variable declaration in a header file", nullptr, nullptr)
OPTION(prefix_1, &"-warn_compact_unwind"[1], warn_compact_unwind, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn for each FDE that cannot compact into the __unwind_info section and must remain in the __eh_frame section", nullptr, nullptr)
OPTION(prefix_1, &"-warn_stabs"[1], warn_stabs, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn when bad stab symbols inside a BINCL/EINCL prevent optimization", nullptr, nullptr)
OPTION(prefix_1, &"-warn_weak_exports"[1], warn_weak_exports, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn if the linked image contains weak external symbols", nullptr, nullptr)
OPTION(prefix_1, &"-watchos_version_min"[1], watchos_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest watchOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, &"-weak-l"[1], weak_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but mark library and its references as weak imports", "<name>", nullptr)
OPTION(prefix_1, &"-weak_framework"[1], weak_framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like -framework <name>, but mark framework and its references as weak imports", "<name>", nullptr)
OPTION(prefix_1, &"-weak_library"[1], weak_library, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like bare <path>, but mark library and its references as weak imports", "<path>", nullptr)
OPTION(prefix_1, &"-weak_reference_mismatches"[1], weak_reference_mismatches, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Resolve symbol imports of conflicting weakness according to <treatment> as weak, non-weak, or error (default is non-weak)", "<treatment>", nullptr)
OPTION(prefix_1, &"-whatsloaded"[1], whatsloaded, Flag, grp_introspect, INVALID, nullptr, HelpHidden, 0,
       "Logs only the object files the linker loads", nullptr, nullptr)
OPTION(prefix_1, &"-why_live"[1], why_live, Separate, grp_introspect, INVALID, nullptr, 0, 0,
       "Log a chain of references to <symbol>, for use with -dead_strip", "<symbol>", nullptr)
OPTION(prefix_1, &"-why_load"[1], why_load, Flag, grp_introspect, INVALID, nullptr, 0, 0,
       "Log why each object file is loaded from a static library", nullptr, nullptr)
OPTION(prefix_1, &"-whyload"[1], whyload, Flag, grp_introspect, why_load, nullptr, 0, 0,
       "Alias for -why_load", nullptr, nullptr)
OPTION(prefix_1, &"-w"[1], w, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Suppress all warnings", nullptr, nullptr)
OPTION(prefix_1, &"-X"[1], X, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, &"-x"[1], x, Flag, grp_symtab, INVALID, nullptr, 0, 0,
       "Exclude non-global symbols from the output symbol table", nullptr, nullptr)
OPTION(prefix_1, &"-Y"[1], Y, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<number>", nullptr)
OPTION(prefix_1, &"-y"[1], y, Joined, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<symbol>", nullptr)
OPTION(prefix_1, &"-Z"[1], Z, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Remove standard directories from the library and framework search paths", nullptr, nullptr)
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
