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
PREFIX(prefix_3, {llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_2, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
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
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "bitcode", grp_bitcode, Group, INVALID, INVALID, nullptr, 0, 0,
       "BITCODE BUILD FLOW", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "bundle", grp_bundle, Group, INVALID, INVALID, nullptr, 0, 0,
       "CREATING A BUNDLE", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "content", grp_content, Group, INVALID, INVALID, nullptr, 0, 0,
       "ADDITIONAL CONTENT", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "deprecated", grp_deprecated, Group, INVALID, INVALID, nullptr, 0, 0,
       "DEPRECATED", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "dylib", grp_dylib, Group, INVALID, INVALID, nullptr, 0, 0,
       "DYNAMIC LIBRARIES (DYLIB)", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "ignored", grp_ignored, Group, INVALID, INVALID, nullptr, 0, 0,
       "IGNORED", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "ignored_silently", grp_ignored_silently, Group, INVALID, INVALID, nullptr, 0, 0,
       "IGNORED SILENTLY", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "introspect", grp_introspect, Group, INVALID, INVALID, nullptr, 0, 0,
       "INTROSPECTING THE LINKER", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "kind", grp_kind, Group, INVALID, INVALID, nullptr, 0, 0,
       "OUTPUT KIND", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "libs", grp_libs, Group, INVALID, INVALID, nullptr, 0, 0,
       "LIBRARIES", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "kind", grp_lld, Group, INVALID, INVALID, nullptr, 0, 0,
       "LLD-SPECIFIC", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "main", grp_main, Group, INVALID, INVALID, nullptr, 0, 0,
       "MAIN EXECUTABLE", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "object", grp_object, Group, INVALID, INVALID, nullptr, 0, 0,
       "CREATING AN OBJECT FILE", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "obsolete", grp_obsolete, Group, INVALID, INVALID, nullptr, 0, 0,
       "OBSOLETE", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "opts", grp_opts, Group, INVALID, INVALID, nullptr, 0, 0,
       "OPTIMIZATIONS", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "rare", grp_rare, Group, INVALID, INVALID, nullptr, 0, 0,
       "RARELY USED", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "resolve", grp_resolve, Group, INVALID, INVALID, nullptr, 0, 0,
       "SYMBOL RESOLUTION", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "symtab", grp_symtab, Group, INVALID, INVALID, nullptr, 0, 0,
       "SYMBOL TABLE", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "undocumented", grp_undocumented, Group, INVALID, INVALID, nullptr, 0, 0,
       "UNDOCUMENTED", nullptr, nullptr)
OPTION(llvm::ArrayRef<llvm::StringLiteral>(), "version", grp_version, Group, INVALID, INVALID, nullptr, 0, 0,
       "VERSION TARGETING", nullptr, nullptr)

//////////
// Options

OPTION(prefix_0, llvm::StringLiteral("<input>"), INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, llvm::StringLiteral("<unknown>"), UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("add_ast_path"), add_ast_path, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "AST paths will be emitted as STABS", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("add_empty_section"), add_empty_section, MultiArg, grp_content, INVALID, nullptr, 0, 2,
       "Create an empty <section> in <segment>", "<segment> <section>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("add_linker_option"), add_linker_option, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("add_source_version"), add_source_version, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("add_split_seg_info"), add_split_seg_info, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("adhoc_codesign"), adhoc_codesign, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Write an ad-hoc code signature to the output file (default for arm64 binaries)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("alias_list"), alias_list, Separate, grp_resolve, INVALID, nullptr, HelpHidden, 0,
       "Create symbol aliases specified in <file>", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("alias"), alias, MultiArg, grp_resolve, INVALID, nullptr, 0, 2,
       "Create a symbol alias with default global visibility", "<symbol_name> <alternate_name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("all_load"), all_load, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Load all members of all static archive libraries", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allow_dead_duplicates"), allow_dead_duplicates, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allow_heap_execute"), allow_heap_execute, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "On i386, allow any page to execute code", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allow_simulator_linking_to_macosx_dylibs"), allow_simulator_linking_to_macosx_dylibs, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allow_stack_execute"), allow_stack_execute, Flag, grp_main, INVALID, nullptr, HelpHidden, 0,
       "Mark stack segment as executable", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allow_sub_type_mismatches"), allow_sub_type_mismatches, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Permit mixing objects compiled for different ARM CPU subtypes", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allowable_client"), allowable_client, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specify <name> of a dylib or framework that is allowed to link to this dylib", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("application_extension"), application_extension, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Mark output as safe for use in an application extension, and validate that linked dylibs are safe", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("arch_errors_fatal"), arch_errors_fatal, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Escalate to errors any warnings about inputs whose architecture does not match the -arch option", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("arch_multiple"), arch_multiple, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Augment error and warning messages with the architecture name", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("arch"), arch, Separate, grp_kind, INVALID, nullptr, 0, 0,
       "The architecture (e.g. ppc, ppc64, i386, x86_64)", "<arch_name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("A"), A, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<basefile>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("bind_at_load"), bind_at_load, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Tell dyld to bind all symbols at load time, rather than lazily", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("bitcode_bundle"), bitcode_bundle, Flag, grp_bitcode, INVALID, nullptr, 0, 0,
       "Generate an embedded bitcode bundle in the __LLVM,__bundle section of the output", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("bitcode_hide_symbols"), bitcode_hide_symbols, Flag, grp_bitcode, INVALID, nullptr, HelpHidden, 0,
       "With -bitcode_bundle, hide all non-exported symbols from output bitcode bundle.", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("bitcode_process_mode"), bitcode_process_mode, Separate, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("bitcode_symbol_map"), bitcode_symbol_map, Separate, grp_bitcode, INVALID, nullptr, HelpHidden, 0,
       "Write the bitcode symbol reverse mapping to file <path>, or if a directory, to <path>/UUID.bcsymbolmap", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("bitcode_verify"), bitcode_verify, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("bridgeos_version_min"), bridgeos_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest bridgeOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("bundle_loader"), bundle_loader, Separate, grp_bundle, INVALID, nullptr, 0, 0,
       "Resolve undefined symbols from <executable>", "<executable>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("bundle"), bundle, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Produce a bundle", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("b"), b, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("cache_path_lto"), cache_path_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Use <path> as a directory for the incremental LTO cache", "<path>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("call-graph-profile-sort"), call_graph_profile_sort, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Reorder sections with call graph profile (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("classic_linker"), classic_linker, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("client_name"), client_name, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specifies a <name> this client should match with the -allowable_client <name> in a dependent dylib", "<name>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("color-diagnostics="), color_diagnostics_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_2, llvm::StringLiteral("color-diagnostics"), color_diagnostics, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("commons"), commons, Separate, grp_resolve, INVALID, nullptr, HelpHidden, 0,
       "Resolve tentative definitions in dylibs according to <treatment>: ignore_dylibs, use_dylibs, error (default is ignore_dylibs)", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("compatibility_version"), compatibility_version, Separate, grp_dylib, INVALID, nullptr, 0, 0,
       "Compatibility <version> of this library", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("current_version"), current_version, Separate, grp_dylib, INVALID, nullptr, 0, 0,
       "Current <version> of this library", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("data_const"), data_const, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Force migration of readonly data into __DATA_CONST segment", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("data_in_code_info"), data_in_code_info, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Emit data-in-code information (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("dead-strip-duplicates"), dead_strip_duplicates, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Do not error on duplicate symbols that will be dead stripped.", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dead_strip_dylibs"), dead_strip_dylibs, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Remove dylibs that are unreachable by the entry point or exported symbols", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dead_strip"), dead_strip, Flag, grp_opts, INVALID, nullptr, 0, 0,
       "Remove unreachable functions and data", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("debug_snapshot"), debug_snapshot, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("debug_variant"), debug_variant, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Suppress warnings germane to binaries shipping to customers", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("deduplicate-strings"), deduplicate_strings, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Enable string deduplication", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("demangle"), demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names in diagnostics", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dependency_info"), dependency_info, Separate, grp_introspect, INVALID, nullptr, 0, 0,
       "Dump dependency info", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dependent_dr_info"), dependent_dr_info, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dirty_data_list"), dirty_data_list, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specify data symbols in <path> destined for the __DATA_DIRTY segment", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dot"), dot, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Write a graph of symbol dependencies to <path> as a .dot file viewable with GraphViz", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("driverkit_version_min"), driverkit_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest DriverKit version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dtrace"), dtrace, Separate, grp_content, INVALID, nullptr, HelpHidden, 0,
       "Enable DTrace static probes according to declarations in <script>", "<script>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dyld_env"), dyld_env, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Specifies a LC_DYLD_ENVIRONMENT variable value pair.", "<dyld_env_var>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylib_compatibility_version"), dylib_compatibility_version, Separate, grp_dylib, compatibility_version, nullptr, HelpHidden, 0,
       "Alias for -compatibility_version", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylib_current_version"), dylib_current_version, Separate, grp_dylib, current_version, nullptr, HelpHidden, 0,
       "Alias for -current_version", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylib_file"), dylib_file, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Specify <current_path> as different from where a dylib normally resides at <install_path>", "<install_path:current_path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylib_install_name"), dylib_install_name, Separate, grp_dylib, install_name, nullptr, 0, 0,
       "Alias for -install_name", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylib"), dylib, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Produce a shared library", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylinker_install_name"), dylinker_install_name, Separate, grp_dylib, install_name, nullptr, 0, 0,
       "Alias for -install_name", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("dylinker"), dylinker, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Produce a dylinker only used when building dyld", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dynamic"), dynamic, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Link dynamically (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("d"), d, Flag, grp_object, INVALID, nullptr, HelpHidden, 0,
       "Force tentative into real definitions for common symbols", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("encryptable"), encryptable, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Generate the LC_ENCRYPTION_INFO load command", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("end-lib"), end_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "End a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("error-limit="), error_limit_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Maximum number of errors to print before exiting (default: 20)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("executable_path"), executable_path, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("execute"), execute, Flag, grp_kind, INVALID, nullptr, 0, 0,
       "Produce a main executable (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("export_dynamic"), export_dynamic, Flag, grp_main, INVALID, nullptr, 0, 0,
       "Preserve all global symbols during LTO and when dead-stripping executables", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("exported_symbols_list"), exported_symbols_list, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Symbols specified in <file> remain global, while others become private externs", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("exported_symbols_order"), exported_symbols_order, Separate, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Specify frequently-used symbols in <file> to optimize symbol exports", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("exported_symbol"), exported_symbol, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "<symbol> remains global, while others become private externs", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("e"), e, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Make <symbol> the entry point of an executable (default is \"start\" from crt1.o)", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("fatal_warnings"), fatal_warnings, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("filelist"), filelist, Separate, grp_content, INVALID, nullptr, 0, 0,
       "Read names of files to link from <file>", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("final_output"), final_output, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Specify dylib install name if -install_name is not used; used by compiler driver for multiple -arch arguments", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("fixup_chains_section"), fixup_chains_section, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("fixup_chains"), fixup_chains, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Emit chained fixups", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("flat_namespace"), flat_namespace, Flag, grp_resolve, INVALID, nullptr, 0, 0,
       "Resolve symbols from all dylibs, both direct and transitive. Do not record source libraries: dyld must re-search at runtime and use the first definition found", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("flto-codegen-only"), flto_codegen_only, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_cpusubtype_ALL"), force_cpusubtype_ALL, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Mark binary as runnable on any PowerPC, ignoring any PowerPC cpu requirements encoded in the object files", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_flat_namespace"), force_flat_namespace, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Tell dyld to use a flat namespace on this executable and all its dependent dylibs & bundles", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_load_swift_libs"), force_load_swift_libs, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Apply -force_load to libraries listed in LC_LINKER_OPTIONS whose names start with 'swift'", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_load"), force_load, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Load all members static archive library at <path>", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_symbol_not_weak"), force_symbol_not_weak, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_symbol_weak"), force_symbol_weak, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_symbols_coalesce_list"), force_symbols_coalesce_list, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_symbols_not_weak_list"), force_symbols_not_weak_list, Separate, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force_symbols_weak_list"), force_symbols_weak_list, Separate, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("framework"), framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Search for <name>.framework/<name> on the framework search path", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("function_starts"), function_starts, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Create table of function start addresses (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("fvmlib"), fvmlib, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("F"), F, JoinedOrSeparate, grp_libs, INVALID, nullptr, 0, 0,
       "Add dir to the framework search path", "<dir>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("headerpad_max_install_names"), headerpad_max_install_names, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Allocate extra space so all load-command paths can expand to MAXPATHLEN via install_name_tool", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("headerpad"), headerpad, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Allocate hex <size> extra space for future expansion of the load commands via install_name_tool (default is 0x20)", "<size>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("help-hidden"), help_hidden, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Display help for hidden options", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("help"), help, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("hidden-l"), hidden_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but load all symbols with hidden visibility", "<name>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("icf="), icf_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Set level for identical code folding (default: none)", "[none,safe,all]", nullptr)
OPTION(prefix_2, llvm::StringLiteral("ignore-auto-link-option="), ignore_auto_link_option_eq, Joined, grp_lld, ignore_auto_link_option, nullptr, 0, 0,
       "Ignore a single auto-linked library or framework. Useful to ignore invalid options that ld64 ignores", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("ignore-auto-link-option"), ignore_auto_link_option, Separate, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ignore_auto_link"), ignore_auto_link, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Ignore LC_LINKER_OPTIONs", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ignore_optimization_hints"), ignore_optimization_hints, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Ignore Linker Optimization Hints", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("image_base"), image_base, Separate, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Preferred hex load address for a dylib or bundle.", "<address>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("init_offsets"), init_offsets, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Store __TEXT segment offsets of static initializers", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("init"), init, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Run <symbol> as the first initializer in a dylib", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("install_name"), install_name, Separate, grp_dylib, INVALID, nullptr, 0, 0,
       "Set an internal install path in a dylib", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("interposable_list"), interposable_list, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Access global symbols listed in <path> indirectly", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("interposable"), interposable, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Indirects access to all to exported symbols in a dylib", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ios_simulator_version_min"), ios_simulator_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest iOS simulator version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("ios_version_min"), ios_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest iOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("iosmac_version_min"), iosmac_version_min, Separate, grp_version, maccatalyst_version_min, nullptr, HelpHidden, 0,
       "Alias for -maccatalyst_version_min", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("iphoneos_version_min"), iphoneos_version_min, Separate, grp_version, ios_version_min, nullptr, HelpHidden, 0,
       "Alias for -ios_version_min", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("i"), i, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("keep_dwarf_unwind"), keep_dwarf_unwind, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("keep_private_externs"), keep_private_externs, Flag, grp_object, INVALID, nullptr, HelpHidden, 0,
       "Do not convert private external symbols to static symbols (only valid with -r)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("keep_relocs"), keep_relocs, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Retain section-based relocation records in the output, which are ignored at runtime by dyld", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("kext_objects_dir"), kext_objects_dir, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("kexts_use_stubs"), kexts_use_stubs, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("kext"), kext, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lazy-l"), lazy_l, Joined, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "This option is deprecated and is now an alias for -l<path>.", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("lazy_framework"), lazy_framework, Separate, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "This option is deprecated and is now an alias for -framework.", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("lazy_library"), lazy_library, Separate, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "This option is deprecated and is now an alias for regular linking", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("load_hidden"), load_hidden, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Load all symbols from static library with hidden visibility", "<path>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("lto-O"), lto_O, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Set optimization level for LTO (default: 2)", "<opt-level>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("lto_library"), lto_library, Separate, grp_obsolete, INVALID, nullptr, 0, 0,
       "Obsolete. LLD supports LTO directly, without using an external dylib.", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("L"), L, JoinedOrSeparate, grp_libs, INVALID, nullptr, 0, 0,
       "Add dir to the library search path", "<dir>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("l"), l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Search for lib<name>.dylib or lib<name>.a on the library search path", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("maccatalyst_version_min"), maccatalyst_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest MacCatalyst version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("macos_version_min"), macos_version_min, Separate, grp_version, INVALID, nullptr, 0, 0,
       "Oldest macOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("macosx_version_min"), macosx_version_min, Separate, grp_version, macos_version_min, nullptr, HelpHidden, 0,
       "Alias for -macos_version_min", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("map"), map, Separate, grp_introspect, INVALID, nullptr, 0, 0,
       "Writes all symbols and their addresses to <path>", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("mark_dead_strippable_dylib"), mark_dead_strippable_dylib, Flag, grp_dylib, INVALID, nullptr, 0, 0,
       "Mark output dylib as dead-strippable: When a client links against it but does not use any of its symbols, the dylib will not be added to the client's list of needed dylibs", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("max_default_common_align"), max_default_common_align, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Reduce maximum alignment for common symbols to a hex power-of-2 <boundary>", "<boundary>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("max_relative_cache_size_lto"), max_relative_cache_size_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Limit the incremental LTO cache growth to <percent> of free disk, space", "<percent>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("mcpu"), mcpu, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Processor family target for LTO code generation", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("merge_zero_fill_sections"), merge_zero_fill_sections, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Merge all zeroed data into the __zerofill section", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("mllvm"), mllvm, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Options to pass to LLVM", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("move_to_ro_segment"), move_to_ro_segment, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Move code symbols listed in <path> to another <segment>", "<segment> <path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("move_to_rw_segment"), move_to_rw_segment, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Move data symbols listed in <path> to another <segment>", "<segment> <path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("multi_module"), multi_module, Flag, grp_rare, interposable, nullptr, HelpHidden, 0,
       "Alias for -interposable", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("multiply_defined_unused"), multiply_defined_unused, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("multiply_defined"), multiply_defined, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("M"), M, Flag, grp_ignored, INVALID, nullptr, HelpHidden, 0,
       "This option is ignored in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("m"), m, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("needed-l"), needed_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but link library even if its symbols are not used and -dead_strip_dylibs is active", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("needed_framework"), needed_framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like -framework <name>, but link <name> even if none of its symbols are used and -dead_strip_dylibs is active", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("needed_library"), needed_library, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like bare <path>, but link library even if its symbols are not used and -dead_strip_dylibs is active", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("new_linker"), new_linker, Flag, grp_ignored, INVALID, nullptr, HelpHidden, 0,
       "This option is ignored in ld64", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-call-graph-profile-sort"), no_call_graph_profile_sort, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Do not reorder sections with call graph profile", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-color-diagnostics"), no_color_diagnostics, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-deduplicate-strings"), no_deduplicate_strings, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Disable string deduplication. This helps uncover cases of comparing string addresses instead of equality and might have a link time performance benefit.", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-lto-legacy-pass-manager"), no_lto_legacy_pass_manager, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Use the new pass manager in LLVM", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-warn-dylib-install-name"), no_warn_dylib_install_name, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Do not warn on -install_name if -dylib is not passed (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_adhoc_codesign"), no_adhoc_codesign, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Do not write an ad-hoc code signature to the output file (default for x86_64 binaries)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_application_extension"), no_application_extension, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Disable application extension functionality (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_arch_warnings"), no_arch_warnings, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Suppresses warnings about inputs whose architecture does not match the -arch option", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_branch_islands"), no_branch_islands, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Disable infra for branches beyond the maximum branch distance.", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_compact_linkedit"), no_compact_linkedit, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_compact_unwind"), no_compact_unwind, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_data_const"), no_data_const, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Block migration of readonly data away from __DATA segment", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_data_in_code_info"), no_data_in_code_info, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Do not emit data-in-code information", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_dead_strip_inits_and_terms"), no_dead_strip_inits_and_terms, Flag, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "Unnecessary option: initialization and termination are roots of the dead strip graph, so never dead stripped", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_deduplicate"), no_deduplicate, Flag, grp_opts, icf_eq, "none\0", 0, 0,
       "Disable code deduplication (synonym for `--icf=none')", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_dependent_dr_info"), no_dependent_dr_info, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_dtrace_dof"), no_dtrace_dof, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Disable dtrace-dof processing (default).", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_eh_labels"), no_eh_labels, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "In -r mode, suppress .eh labels in the __eh_frame section", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_encryption"), no_encryption, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Do not generate the LC_ENCRYPTION_INFO load command", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_exported_symbols"), no_exported_symbols, Flag, grp_resolve, INVALID, nullptr, 0, 0,
       "Don't export any symbols from the binary, useful for main executables that don't have plugins", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_fixup_chains"), no_fixup_chains, Flag, grp_undocumented, INVALID, nullptr, 0, 0,
       "Emit fixup information as classic dyld opcodes", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_function_starts"), no_function_starts, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Do not create table of function start addresses", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_implicit_dylibs"), no_implicit_dylibs, Flag, grp_opts, INVALID, nullptr, 0, 0,
       "Do not optimize public dylib transitive symbol references", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_inits"), no_inits, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Fail if the output contains static initializers", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_keep_dwarf_unwind"), no_keep_dwarf_unwind, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_kext_objects"), no_kext_objects, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_new_main"), no_new_main, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_objc_category_merging"), no_objc_category_merging, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Do not merge Objective-C categories into their classes", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_order_data"), no_order_data, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Disable default reordering of global data accessed at launch time", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_order_inits"), no_order_inits, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Disable default reordering of initializer and terminator functions", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_pie"), no_pie, Flag, grp_main, INVALID, nullptr, 0, 0,
       "Do not build a position independent executable", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_source_version"), no_source_version, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_uuid"), no_uuid, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Do not generate the LC_UUID load command", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_warn_inits"), no_warn_inits, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Suppress warnings for static initializers in the output", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_weak_exports"), no_weak_exports, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Fail if the linked image contains weak external symbols", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_weak_imports"), no_weak_imports, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Fail if any symbols are weak imports, allowed to be NULL at runtime", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("no_zero_fill_sections"), no_zero_fill_sections, Flag, grp_opts, INVALID, nullptr, HelpHidden, 0,
       "Explicitly store zeroed data in the final image", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("noall_load"), noall_load, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Don't load all static members from archives, this is the default, this negates -all_load", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nofixprebinding"), nofixprebinding, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nomultidefs"), nomultidefs, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("non_global_symbols_no_strip_list"), non_global_symbols_no_strip_list, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "Specify in <path> the non-global symbols that should remain in the output symbol table", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("non_global_symbols_strip_list"), non_global_symbols_strip_list, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "Specify in <path> the non-global symbols that should be removed from the output symbol table", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("noprebind_all_twolevel_modules"), noprebind_all_twolevel_modules, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("noprebind"), noprebind, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("noseglinkedit"), noseglinkedit, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("not_for_dyld_shared_cache"), not_for_dyld_shared_cache, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Prevent system dylibs from being placed into the dylib shared cache", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("objc_abi_version"), objc_abi_version, Separate, grp_ignored_silently, INVALID, nullptr, HelpHidden, 0,
       "This option only applies to i386 in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("objc_gc_compaction"), objc_gc_compaction, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Mark the Objective-C image as compatible with compacting garbage collection", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("objc_gc_only"), objc_gc_only, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Verify that all code was compiled with -fobjc-gc-only", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("objc_gc"), objc_gc, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Verify that all code was compiled with -fobjc-gc or -fobjc-gc-only", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("objc_stubs_fast"), objc_stubs_fast, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Produce larger stubs for Objective-C method calls with fewer jumps (default).", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("objc_stubs_small"), objc_stubs_small, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Produce smaller stubs for Objective-C method calls with more jumps.", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ObjC"), ObjC, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Load all members of static archives that are an Objective-C class or category.", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("object_path_lto"), object_path_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Retain any temporary mach-o file in <path> that would otherwise be deleted during LTO", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("order_file_statistics"), order_file_statistics, Flag, grp_introspect, INVALID, nullptr, HelpHidden, 0,
       "Logs information about -order_file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("order_file"), order_file, Separate, grp_opts, INVALID, nullptr, 0, 0,
       "Layout functions and data according to specification in <file>", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("oso_prefix"), oso_prefix, Separate, grp_symtab, INVALID, nullptr, 0, 0,
       "Remove the prefix <path> from OSO symbols in the debug map", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("O"), O, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Optimize output file size", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("o"), o, Separate, grp_kind, INVALID, nullptr, 0, 0,
       "The name of the output file (default: `a.out')", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("page_align_data_atoms"), page_align_data_atoms, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Distribute global variables on separate pages so page used/dirty status can guide creation of an order file to cluster commonly used/dirty globals", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pagezero_size"), pagezero_size, Separate, grp_main, INVALID, nullptr, 0, 0,
       "Size of unreadable segment at address zero is hex <size> (default is 4KB on 32-bit and 4GB on 64-bit)", "<size>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("pause"), pause, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pie"), pie, Flag, grp_main, INVALID, nullptr, 0, 0,
       "Build a position independent executable (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("platform_version"), platform_version, MultiArg, grp_version, INVALID, nullptr, 0, 3,
       "Platform (e.g., macos, ios, tvos, watchos, bridgeos, mac-catalyst, ios-sim, tvos-sim, watchos-sim, driverkit) and version numbers", "<platform> <min_version> <sdk_version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("prebind_all_twolevel_modules"), prebind_all_twolevel_modules, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("prebind_allow_overlap"), prebind_allow_overlap, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("prebind"), prebind, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("preload"), preload, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Produce an unsegmented binary for embedded systems", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-dylib-search"), print_dylib_search, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Print which paths lld searched when trying to find dylibs", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("print-symbol-order="), print_symbol_order_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Print a symbol order specified by --call-graph-profile-sort into the specified file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("print_statistics"), print_statistics, Flag, grp_introspect, INVALID, nullptr, HelpHidden, 0,
       "Log the linker's memory and CPU usage", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("private_bundle"), private_bundle, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("prune_after_lto"), prune_after_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Remove LTO cache entries after <seconds>", "<seconds>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("prune_interval_lto"), prune_interval_lto, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Prune the incremental LTO cache after <seconds> (-1 disables pruning)", "<seconds>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("random_uuid"), random_uuid, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("read_only_relocs"), read_only_relocs, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Handle relocations that modify read-only pages according to <treatment> of warning, error, or suppress (i.e., allow)", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("read_only_stubs"), read_only_stubs, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "On i386, make the __IMPORT segment of a final linked image read-only", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("reexport-l"), reexport_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but export all symbols of <name> from newly created library", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("reexport_framework"), reexport_framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like -framework <name>, but export all symbols of <name> from the newly created library", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("reexport_library"), reexport_library, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like bare <path>, but export all symbols of <path> from newly created library", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("reexported_symbols_list"), reexported_symbols_list, Separate, grp_resolve, INVALID, nullptr, HelpHidden, 0,
       "Symbols from dependent dylibs specified in <file> are reexported by this dylib", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("rename_section"), rename_section, MultiArg, grp_rare, INVALID, nullptr, 0, 4,
       "Rename <from_segment>/<from_section> as <to_segment>/<to_section>", "<from_segment> <from_section> <to_segment> <to_section>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("rename_segment"), rename_segment, MultiArg, grp_rare, INVALID, nullptr, 0, 2,
       "Rename <from_segment> as <to_segment>", "<from_segment> <to_segment>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("reproduce="), reproduce_eq, Joined, grp_lld, reproduce, nullptr, 0, 0,
       "Write tar file containing inputs and command to reproduce link", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("reproduce"), reproduce, Separate, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("root_safe"), root_safe, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Set the MH_ROOT_SAFE bit in the mach-o header", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("rpath"), rpath, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Add <path> to dyld search list for dylibs with load path prefix `@rpath/'", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("run_init_lazily"), run_init_lazily, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("r"), r, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Merge multiple object files into one, retaining relocations", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("save-temps"), save_temps, Flag, grp_introspect, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("sdk_version"), sdk_version, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("search_dylibs_first"), search_dylibs_first, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Search for lib<name>.dylib on first pass, then for lib<name>.a on second pass through search path (default for Xcode 3 and earlier)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("search_paths_first"), search_paths_first, Flag, grp_libs, INVALID, nullptr, 0, 0,
       "Search for lib<name>.dylib and lib<name>.a at each step in traversing search path (default for Xcode 4 and later)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("sect_diff_relocs"), sect_diff_relocs, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("sectalign"), sectalign, MultiArg, grp_rare, INVALID, nullptr, 0, 3,
       "Align <section> within <segment> to hex power-of-2 <boundary>", "<segment> <section> <boundary>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("sectcreate"), sectcreate, MultiArg, grp_content, INVALID, nullptr, 0, 3,
       "Create <section> in <segment> from the contents of <file>", "<segment> <section> <file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("section_order"), section_order, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "With -preload, specify layout sequence of colon-separated <sections> in <segment>", "<segment> <sections>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("sectobjectsymbols"), sectobjectsymbols, MultiArg, grp_obsolete, INVALID, nullptr, HelpHidden, 2,
       "This option is obsolete in ld64", "<segname> <sectname>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("sectorder_detail"), sectorder_detail, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("sectorder"), sectorder, MultiArg, grp_obsolete, INVALID, nullptr, 0, 3,
       "Obsolete. Replaced by more general -order_file option", "<segname> <sectname> <orderfile>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("seg1addr"), seg1addr, Separate, grp_opts, image_base, nullptr, HelpHidden, 0,
       "Alias for -image_base", "<address>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("seg_addr_table_filename"), seg_addr_table_filename, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("seg_page_size"), seg_page_size, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Specifies the page <size> for <segment>. Segment size will be a multiple of its page size", "<segment> <size>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("segaddr"), segaddr, MultiArg, grp_rare, INVALID, nullptr, HelpHidden, 2,
       "Specify the starting hex <address> at a 4KiB page boundary for <segment>", "<segment> <address>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("segalign"), segalign, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Align all segments to hex power-of-2 <boundary>", "<boundary>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("segcreate"), segcreate, MultiArg, grp_content, sectcreate, nullptr, 0, 3,
       "Alias for -sectcreate", "<segment> <section> <file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("seglinkedit"), seglinkedit, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("segment_order"), segment_order, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "With -preload, specify layout sequence of colon-separated <segments>", "<colon_separated_segment_list>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("segprot"), segprot, MultiArg, grp_rare, INVALID, nullptr, 0, 3,
       "Specifies the <max> and <init> virtual memory protection of <segment> as r/w/x/-seg_addr_table path", "<segment> <max> <init>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("segs_read_only_addr"), segs_read_only_addr, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete", "<address>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("segs_read_write_addr"), segs_read_write_addr, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete", "<address>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("setuid_safe"), setuid_safe, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Set the MH_SETUID_SAFE bit in the mach-o header", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("simulator_support"), simulator_support, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("single_module"), single_module, Flag, grp_deprecated, INVALID, nullptr, HelpHidden, 0,
       "Unnecessary option: this is already the default", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("Si"), Si, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("slow_stubs"), slow_stubs, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("snapshot_dir"), snapshot_dir, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("Sn"), Sn, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("source_version"), source_version, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("Sp"), Sp, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("stack_addr"), stack_addr, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Initialize stack pointer to hex <address> rounded to a page boundary", "<address>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("stack_size"), stack_size, Separate, grp_main, INVALID, nullptr, HelpHidden, 0,
       "Maximum hex stack size for the main thread in a program. (default is 8MB)", "<size>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("start-lib"), start_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Start a grouping of objects that should be treated as if they were together in an archive", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("static"), static, Flag, grp_kind, INVALID, nullptr, HelpHidden, 0,
       "Link statically", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("strict-auto-link"), strict_auto_link, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Always warn for missing frameworks or libraries if they are loaded via LC_LINKER_OPTIONS", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("sub_library"), sub_library, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Re-export the dylib as <name>", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("sub_umbrella"), sub_umbrella, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Re-export the framework as <name>", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("syslibroot"), syslibroot, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Prepend <rootdir> to all library and framework search paths", "<rootdir>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("S"), S, Flag, grp_symtab, INVALID, nullptr, 0, 0,
       "Strip debug information (STABS or DWARF) from the output", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("s"), s, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("text_exec"), text_exec, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Rename __segment TEXT to __TEXT_EXEC for sections __text and __stubs", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-cache-policy="), thinlto_cache_policy_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-emit-imports-files"), thinlto_emit_imports_files, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-emit-index-files"), thinlto_emit_index_files, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-index-only="), thinlto_index_only_eq, Joined, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-index-only"), thinlto_index_only, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-jobs="), thinlto_jobs_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Number of ThinLTO jobs. Default to --threads=", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-object-suffix-replace="), thinlto_object_suffix_replace_eq, Joined, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("thinlto-prefix-replace="), thinlto_prefix_replace_eq, Joined, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("threaded_starts_section"), threaded_starts_section, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("threads="), threads_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("time-trace-granularity="), time_trace_granularity_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Minimum time granularity (in microseconds) traced by time profiler", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("time-trace="), time_trace_eq, Joined, grp_lld, INVALID, nullptr, 0, 0,
       "Record time trace to <file>", "<file>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("time-trace"), anonymous_0, Flag, grp_lld, time_trace_eq, nullptr, 0, 0,
       "Record time trace to file next to output", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("trace_symbol_layout"), trace_symbol_layout, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Show where and why symbols move, as specified by -move_to_ro_segment, -move_to_rw_segment, -rename_section, and -rename_segment", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("tvos_version_min"), tvos_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest tvOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("twolevel_namespace_hints"), twolevel_namespace_hints, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("twolevel_namespace"), twolevel_namespace, Flag, grp_resolve, INVALID, nullptr, 0, 0,
       "Make dyld look up symbols by (dylib,name) pairs (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("t"), t, Flag, grp_introspect, INVALID, nullptr, 0, 0,
       "Log every file the linker loads: object, archive, and dylib", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("uikitformac_version_min"), uikitformac_version_min, Separate, grp_version, maccatalyst_version_min, nullptr, HelpHidden, 0,
       "Alias for -maccatalyst_version_min", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("umbrella"), umbrella, Separate, grp_rare, INVALID, nullptr, 0, 0,
       "Re-export this dylib through the umbrella framework <name>", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("unaligned_pointers"), unaligned_pointers, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Handle unaligned pointers in __DATA segments according to <treatment>: warning, error, or suppress (default for arm64e is error, otherwise suppress)", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("undefined"), undefined, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Handle undefined symbols according to <treatment>: error, warning, suppress, or dynamic_lookup (default is error)", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("unexported_symbols_list"), unexported_symbols_list, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Global symbols specified in <file> become private externs", "<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("unexported_symbol"), unexported_symbol, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Global <symbol> becomes private extern", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("upward-l"), upward_l, Joined, grp_libs, INVALID, nullptr, HelpHidden, 0,
       "Like -l<name>, but specify dylib as an upward dependency", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("upward_framework"), upward_framework, Separate, grp_libs, INVALID, nullptr, HelpHidden, 0,
       "Like -framework <name>, but specify the framework as an upward dependency", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("upward_library"), upward_library, Separate, grp_libs, INVALID, nullptr, HelpHidden, 0,
       "Like bare <path>, but specify dylib as an upward dependency", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("U"), U, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Allow <symbol> to have no definition", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("u"), u, Separate, grp_resolve, INVALID, nullptr, 0, 0,
       "Require that <symbol> be defined for the link to succeed", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("verbose_deduplicate"), verbose_deduplicate, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Print function names eliminated by deduplication and the total size of code savings", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("verbose_optimization_hints"), verbose_optimization_hints, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("verbose"), verbose, Flag, grp_lld, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("version_details"), version_details, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Print the linker version in JSON form", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("version_load_command"), version_load_command, Flag, grp_undocumented, INVALID, nullptr, HelpHidden, 0,
       "This option is undocumented in ld64", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("version"), version, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("v"), v, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Print the linker version and search paths in addition to linking", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("warn-dylib-install-name"), warn_dylib_install_name, Flag, grp_lld, INVALID, nullptr, 0, 0,
       "Warn on -install_name if -dylib is not passed", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("warn_commons"), warn_commons, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn when a tentative definition in an object file matches an external symbol in a dylib, which often means \"extern\" is missing from a variable declaration in a header file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("warn_compact_unwind"), warn_compact_unwind, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn for each FDE that cannot compact into the __unwind_info section and must remain in the __eh_frame section", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("warn_stabs"), warn_stabs, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn when bad stab symbols inside a BINCL/EINCL prevent optimization", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("warn_weak_exports"), warn_weak_exports, Flag, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Warn if the linked image contains weak external symbols", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("watchos_version_min"), watchos_version_min, Separate, grp_version, INVALID, nullptr, HelpHidden, 0,
       "Oldest watchOS version for which linked output is usable", "<version>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("weak-l"), weak_l, Joined, grp_libs, INVALID, nullptr, 0, 0,
       "Like -l<name>, but mark library and its references as weak imports", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("weak_framework"), weak_framework, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like -framework <name>, but mark framework and its references as weak imports", "<name>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("weak_library"), weak_library, Separate, grp_libs, INVALID, nullptr, 0, 0,
       "Like bare <path>, but mark library and its references as weak imports", "<path>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("weak_reference_mismatches"), weak_reference_mismatches, Separate, grp_rare, INVALID, nullptr, HelpHidden, 0,
       "Resolve symbol imports of conflicting weakness according to <treatment> as weak, non-weak, or error (default is non-weak)", "<treatment>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("whatsloaded"), whatsloaded, Flag, grp_introspect, INVALID, nullptr, HelpHidden, 0,
       "Logs only the object files the linker loads", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("why_live"), why_live, Separate, grp_introspect, INVALID, nullptr, 0, 0,
       "Log a chain of references to <symbol>, for use with -dead_strip", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("why_load"), why_load, Flag, grp_introspect, INVALID, nullptr, 0, 0,
       "Log why each object file is loaded from a static library", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("whyload"), whyload, Flag, grp_introspect, why_load, nullptr, 0, 0,
       "Alias for -why_load", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("w"), w, Flag, grp_rare, INVALID, nullptr, 0, 0,
       "Suppress all warnings", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("X"), X, Flag, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("x"), x, Flag, grp_symtab, INVALID, nullptr, 0, 0,
       "Exclude non-global symbols from the output symbol table", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("Y"), Y, Separate, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<number>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("y"), y, Joined, grp_obsolete, INVALID, nullptr, HelpHidden, 0,
       "This option is obsolete in ld64", "<symbol>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("Z"), Z, Flag, grp_libs, INVALID, nullptr, 0, 0,
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
static const unsigned SimpleEnumValueTablesSize = std::size(SimpleEnumValueTables);
#endif // SIMPLE_ENUM_VALUE_TABLE


