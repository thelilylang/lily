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
OPTION(prefix_1, &"-###"[1], _HASH_HASH_HASH, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print (but do not run) the commands to run for this compilation", nullptr, nullptr)
OPTION(prefix_2, &"--allow-multiple-definition"[2], allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiple definitions", nullptr, nullptr)
OPTION(prefix_2, &"--appcontainer"[2], appcontainer, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the appcontainer flag in the executable", nullptr, nullptr)
OPTION(prefix_2, &"--as-needed"[2], anonymous_1, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--Bdynamic"[2], Bdynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Link against shared libraries", nullptr, nullptr)
OPTION(prefix_2, &"--Bstatic"[2], Bstatic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not link against shared libraries", nullptr, nullptr)
OPTION(prefix_2, &"--build-id"[2], anonymous_2, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-call_shared"[1], alias_Bdynamic_call_shared, Flag, INVALID, Bdynamic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--delayload="[2], delayload_eq, Joined, INVALID, delayload, nullptr, 0, 0,
       "DLL to load only on demand", nullptr, nullptr)
OPTION(prefix_2, &"--delayload"[2], delayload, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--demangle"[2], demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names (default)", nullptr, nullptr)
OPTION(prefix_2, &"--disable-auto-image-base"[2], anonymous_3, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--disable-auto-import"[2], disable_auto_import, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't automatically import data symbols from other DLLs without dllimport", nullptr, nullptr)
OPTION(prefix_2, &"--disable-dynamicbase"[2], disable_dynamicbase, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable ASLR", nullptr, nullptr)
OPTION(prefix_2, &"--disable-high-entropy-va"[2], disable_high_entropy_va, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'high entropy VA' flag", nullptr, nullptr)
OPTION(prefix_2, &"--disable-no-seh"[2], disable_no_seh, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'no SEH' flag", nullptr, nullptr)
OPTION(prefix_2, &"--disable-nxcompat"[2], disable_nxcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'nxcompat' flag", nullptr, nullptr)
OPTION(prefix_2, &"--disable-reloc-section"[2], disable_reloc_section, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable base relocations", nullptr, nullptr)
OPTION(prefix_2, &"--disable-runtime-pseudo-reloc"[2], disable_runtime_pseudo_reloc, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't do automatic imports that require runtime fixups", nullptr, nullptr)
OPTION(prefix_2, &"--disable-stdcall-fixup"[2], disable_stdcall_fixup, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't resolve stdcall/fastcall/vectorcall to undecorated symbols", nullptr, nullptr)
OPTION(prefix_2, &"--disable-tsaware"[2], disable_tsaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'Terminal Server aware' flag", nullptr, nullptr)
OPTION(prefix_1, &"-dn"[1], alias_Bstatic_dn, Flag, INVALID, Bstatic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--dynamicbase"[2], dynamicbase, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable ASLR", nullptr, nullptr)
OPTION(prefix_1, &"-dy"[1], alias_Bdynamic_dy, Flag, INVALID, Bdynamic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--enable-auto-image-base"[2], anonymous_4, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--enable-auto-import"[2], enable_auto_import, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Automatically import data symbols from other DLLs where needed", nullptr, nullptr)
OPTION(prefix_2, &"--enable-reloc-section"[2], enable_reloc_section, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable base relocations", nullptr, nullptr)
OPTION(prefix_2, &"--enable-runtime-pseudo-reloc"[2], enable_runtime_pseudo_reloc, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow automatic imports that require runtime fixups", nullptr, nullptr)
OPTION(prefix_2, &"--enable-stdcall-fixup"[2], enable_stdcall_fixup, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Resolve stdcall/fastcall/vectorcall to undecorated symbols without warnings", nullptr, nullptr)
OPTION(prefix_2, &"--end-group"[2], anonymous_5, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--entry="[2], entry_eq, Joined, INVALID, entry, nullptr, 0, 0,
       "Name of entry point symbol", "<entry>", nullptr)
OPTION(prefix_2, &"--entry"[2], entry, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<entry>", nullptr)
OPTION(prefix_2, &"--exclude-all-symbols"[2], exclude_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't automatically export any symbols", nullptr, nullptr)
OPTION(prefix_2, &"--exclude-symbols="[2], exclude_symbols_eq, Joined, INVALID, exclude_symbols, nullptr, 0, 0,
       "Exclude symbols from automatic export", "<symbol[,symbol,...]>", nullptr)
OPTION(prefix_2, &"--exclude-symbols"[2], exclude_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol[,symbol,...]>", nullptr)
OPTION(prefix_2, &"--export-all-symbols"[2], export_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Export all symbols even if a def file or dllexport attributes are used", nullptr, nullptr)
OPTION(prefix_1, &"-e"[1], alias_entry_e, JoinedOrSeparate, INVALID, entry, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--fatal-warnings"[2], fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_2, &"--file-alignment="[2], file_alignment_eq, Joined, INVALID, file_alignment, nullptr, 0, 0,
       "Set file alignment", nullptr, nullptr)
OPTION(prefix_2, &"--file-alignment"[2], file_alignment, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--full-shutdown"[2], anonymous_6, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--gc-sections"[2], gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove unused sections", nullptr, nullptr)
OPTION(prefix_2, &"--heap="[2], heap_eq, Joined, INVALID, heap, nullptr, 0, 0,
       "Set size of the initial heap", nullptr, nullptr)
OPTION(prefix_2, &"--heap"[2], heap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--help"[2], help, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print option help", nullptr, nullptr)
OPTION(prefix_2, &"--high-entropy-va"[2], high_entropy_va, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'high entropy VA' flag", nullptr, nullptr)
OPTION(prefix_2, &"--icf="[2], icf_eq, Joined, INVALID, icf, nullptr, 0, 0,
       "Identical code folding", nullptr, nullptr)
OPTION(prefix_2, &"--icf"[2], icf, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--image-base="[2], image_base_eq, Joined, INVALID, image_base, nullptr, 0, 0,
       "Base address of the program", nullptr, nullptr)
OPTION(prefix_2, &"--image-base"[2], image_base, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--insert-timestamp"[2], insert_timestamp, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include PE header timestamp", nullptr, nullptr)
OPTION(prefix_2, &"--kill-at"[2], kill_at, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove @n from exported symbols", nullptr, nullptr)
OPTION(prefix_3, &"--large-address-aware"[2], large_address_aware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable large addresses", nullptr, nullptr)
OPTION(prefix_1, &"-L"[1], L, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Add a directory to the library search path", "<dir>", nullptr)
OPTION(prefix_1, &"-l"[1], l, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Root name of library to use", "<libName>", nullptr)
OPTION(prefix_2, &"--major-image-version="[2], anonymous_7_eq, Joined, INVALID, anonymous_7, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--major-image-version"[2], anonymous_7, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--major-os-version="[2], major_os_version_eq, Joined, INVALID, major_os_version, nullptr, 0, 0,
       "Set the OS and subsystem major version", nullptr, nullptr)
OPTION(prefix_3, &"--major-os-version"[2], major_os_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--major-subsystem-version="[2], major_subsystem_version_eq, Joined, INVALID, major_subsystem_version, nullptr, 0, 0,
       "Set the OS and subsystem major version", nullptr, nullptr)
OPTION(prefix_3, &"--major-subsystem-version"[2], major_subsystem_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--Map="[2], map_eq, Joined, INVALID, map, nullptr, 0, 0,
       "Output a linker map", nullptr, nullptr)
OPTION(prefix_2, &"--Map"[2], map, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--minor-image-version="[2], anonymous_8_eq, Joined, INVALID, anonymous_8, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--minor-image-version"[2], anonymous_8, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--minor-os-version="[2], minor_os_version_eq, Joined, INVALID, minor_os_version, nullptr, 0, 0,
       "Set the OS and subsystem minor version", nullptr, nullptr)
OPTION(prefix_3, &"--minor-os-version"[2], minor_os_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--minor-subsystem-version="[2], minor_subsystem_version_eq, Joined, INVALID, minor_subsystem_version, nullptr, 0, 0,
       "Set the OS and subsystem minor version", nullptr, nullptr)
OPTION(prefix_3, &"--minor-subsystem-version"[2], minor_subsystem_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--mllvm="[2], mllvm_eq, Joined, INVALID, mllvm, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--mllvm"[2], mllvm, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-m"[1], m, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Set target emulation", nullptr, nullptr)
OPTION(prefix_2, &"--no-allow-multiple-definition"[2], no_allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not allow multiple definitions (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-demangle"[2], no_demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbol names", nullptr, nullptr)
OPTION(prefix_2, &"--no-dynamicbase"[2], alias_no_dynamicbase, Flag, INVALID, disable_dynamicbase, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--no-fatal-warnings"[2], no_fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not treat warnings as errors (default)", nullptr, nullptr)
OPTION(prefix_2, &"--no-gc-sections"[2], no_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't remove unused sections", nullptr, nullptr)
OPTION(prefix_2, &"--no-insert-timestamp"[2], no_insert_timestamp, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't include PE header timestamp", nullptr, nullptr)
OPTION(prefix_2, &"--no-seh"[2], no_seh, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'no SEH' flag in the executable", nullptr, nullptr)
OPTION(prefix_2, &"--no-undefined"[2], anonymous_9, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--no-whole-archive"[2], no_whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "No longer include all object files for following archives", nullptr, nullptr)
OPTION(prefix_1, &"-non_shared"[1], alias_Bstatic_non_shared, Flag, INVALID, Bstatic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--nxcompat"[2], nxcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'nxcompat' flag in the executable", nullptr, nullptr)
OPTION(prefix_2, &"--out-implib="[2], out_implib_eq, Joined, INVALID, out_implib, nullptr, 0, 0,
       "Import library name", nullptr, nullptr)
OPTION(prefix_2, &"--out-implib"[2], out_implib, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--output-def="[2], output_def_eq, Joined, INVALID, output_def, nullptr, 0, 0,
       "Output def file", nullptr, nullptr)
OPTION(prefix_2, &"--output-def"[2], output_def, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-O"[1], anonymous_0, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-o"[1], o, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", "<path>", nullptr)
OPTION(prefix_2, &"--pdb="[2], pdb_eq, Joined, INVALID, pdb, nullptr, 0, 0,
       "Output PDB debug info file, chosen implicitly if the argument is empty", nullptr, nullptr)
OPTION(prefix_2, &"--pdb"[2], pdb, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--pic-executable"[2], anonymous_10, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt="[2], anonymous_12_eq, Joined, INVALID, anonymous_12, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin-opt"[2], anonymous_12, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin="[2], anonymous_11_eq, Joined, INVALID, anonymous_11, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--plugin"[2], anonymous_11, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--reproduce="[2], reproduce_eq, Joined, INVALID, reproduce, nullptr, 0, 0,
       "Write a tar file containing input files and command line options to reproduce link", nullptr, nullptr)
OPTION(prefix_2, &"--reproduce"[2], reproduce, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--require-defined="[2], require_defined_eq, Joined, INVALID, require_defined, nullptr, 0, 0,
       "Force symbol to be added to symbol table as an undefined one", nullptr, nullptr)
OPTION(prefix_2, &"--require-defined"[2], require_defined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--section-alignment="[2], section_alignment_eq, Joined, INVALID, section_alignment, nullptr, 0, 0,
       "Set section alignment", nullptr, nullptr)
OPTION(prefix_2, &"--section-alignment"[2], section_alignment, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--shared"[2], shared, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Build a shared object", nullptr, nullptr)
OPTION(prefix_2, &"--stack="[2], stack_eq, Joined, INVALID, stack, nullptr, 0, 0,
       "Set size of the initial stack", nullptr, nullptr)
OPTION(prefix_2, &"--stack"[2], stack, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--start-group"[2], anonymous_14, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-static"[1], alias_Bstatic_static, Flag, INVALID, Bstatic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--strip-all"[2], strip_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Omit all symbol information from the output binary", nullptr, nullptr)
OPTION(prefix_2, &"--strip-debug"[2], strip_debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Omit all debug information, but keep symbol information", nullptr, nullptr)
OPTION(prefix_2, &"--subsystem="[2], subs_eq, Joined, INVALID, subs, nullptr, 0, 0,
       "Specify subsystem", nullptr, nullptr)
OPTION(prefix_2, &"--subsystem"[2], subs, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--sysroot="[2], anonymous_13_eq, Joined, INVALID, anonymous_13, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--sysroot"[2], anonymous_13, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-S"[1], alias_strip_S, Flag, INVALID, strip_debug, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-s"[1], alias_strip_s, Flag, INVALID, strip_all, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-cache-dir="[2], thinlto_cache_dir_eq, Joined, INVALID, thinlto_cache_dir, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_3, &"--thinlto-cache-dir"[2], thinlto_cache_dir, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--tsaware"[2], tsaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'Terminal Server aware' flag", nullptr, nullptr)
OPTION(prefix_2, &"--undefined="[2], undefined_eq, Joined, INVALID, undefined, nullptr, 0, 0,
       "Include symbol in the link, if available", nullptr, nullptr)
OPTION(prefix_2, &"--undefined"[2], undefined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"-u"[1], alias_undefined_u, JoinedOrSeparate, INVALID, undefined, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, &"--verbose"[2], verbose, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Verbose mode", nullptr, nullptr)
OPTION(prefix_2, &"--version"[2], version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_1, &"-v"[1], v, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number", nullptr, nullptr)
OPTION(prefix_2, &"--whole-archive"[2], whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include all object files for following archives", nullptr, nullptr)
OPTION(prefix_2, &"--wrap="[2], wrap_eq, Joined, INVALID, wrap, nullptr, 0, 0,
       "Use wrapper functions for symbol", "<symbol>", nullptr)
OPTION(prefix_2, &"--wrap"[2], wrap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, &"--Xlink="[2], Xlink_eq, Joined, INVALID, Xlink, nullptr, 0, 0,
       "Pass <arg> to the COFF linker", "<arg>", nullptr)
OPTION(prefix_2, &"--Xlink"[2], Xlink, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<arg>", nullptr)
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
