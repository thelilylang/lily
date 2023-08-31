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
OPTION(prefix_1, llvm::StringLiteral("###"), _HASH_HASH_HASH, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print (but do not run) the commands to run for this compilation", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("allow-multiple-definition"), allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiple definitions", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("appcontainer"), appcontainer, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the appcontainer flag in the executable", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("as-needed"), anonymous_1, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bdynamic"), Bdynamic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Link against shared libraries", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Bstatic"), Bstatic, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not link against shared libraries", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("build-id"), anonymous_2, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("call_shared"), alias_Bdynamic_call_shared, Flag, INVALID, Bdynamic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("delayload="), delayload_eq, Joined, INVALID, delayload, nullptr, 0, 0,
       "DLL to load only on demand", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("delayload"), delayload, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("demangle"), demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbol names (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-auto-image-base"), anonymous_3, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-auto-import"), disable_auto_import, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't automatically import data symbols from other DLLs without dllimport", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-dynamicbase"), disable_dynamicbase, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable ASLR", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-high-entropy-va"), disable_high_entropy_va, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'high entropy VA' flag", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-no-seh"), disable_no_seh, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'no SEH' flag", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-nxcompat"), disable_nxcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'nxcompat' flag", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-reloc-section"), disable_reloc_section, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable base relocations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-runtime-pseudo-reloc"), disable_runtime_pseudo_reloc, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't do automatic imports that require runtime fixups", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-stdcall-fixup"), disable_stdcall_fixup, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't resolve stdcall/fastcall/vectorcall to undecorated symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("disable-tsaware"), disable_tsaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't set the 'Terminal Server aware' flag", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dn"), alias_Bstatic_dn, Flag, INVALID, Bstatic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("dynamicbase"), dynamicbase, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable ASLR", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dy"), alias_Bdynamic_dy, Flag, INVALID, Bdynamic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("enable-auto-image-base"), anonymous_4, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("enable-auto-import"), enable_auto_import, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Automatically import data symbols from other DLLs where needed", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("enable-reloc-section"), enable_reloc_section, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable base relocations", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("enable-runtime-pseudo-reloc"), enable_runtime_pseudo_reloc, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow automatic imports that require runtime fixups", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("enable-stdcall-fixup"), enable_stdcall_fixup, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Resolve stdcall/fastcall/vectorcall to undecorated symbols without warnings", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("end-group"), anonymous_5, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("entry="), entry_eq, Joined, INVALID, entry, nullptr, 0, 0,
       "Name of entry point symbol", "<entry>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("entry"), entry, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<entry>", nullptr)
OPTION(prefix_3, llvm::StringLiteral("error-limit="), error_limit_eq, Joined, INVALID, error_limit, nullptr, 0, 0,
       "Maximum number of errors to emit before stopping (0 = no limit)", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("error-limit"), error_limit, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("exclude-all-symbols"), exclude_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't automatically export any symbols", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("exclude-symbols="), exclude_symbols_eq, Joined, INVALID, exclude_symbols, nullptr, 0, 0,
       "Exclude symbols from automatic export", "<symbol[,symbol,...]>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("exclude-symbols"), exclude_symbols, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol[,symbol,...]>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("export-all-symbols"), export_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Export all symbols even if a def file or dllexport attributes are used", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("e"), alias_entry_e, JoinedOrSeparate, INVALID, entry, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("fatal-warnings"), fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("file-alignment="), file_alignment_eq, Joined, INVALID, file_alignment, nullptr, 0, 0,
       "Set file alignment", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("file-alignment"), file_alignment, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("full-shutdown"), anonymous_6, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("gc-sections"), gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove unused sections", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("guard-cf"), guard_cf, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable Control Flow Guard", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("guard-longjmp"), guard_longjmp, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable Control Flow Guard long jump hardening (default for --guard-cf)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("heap="), heap_eq, Joined, INVALID, heap, nullptr, 0, 0,
       "Set size of the initial heap", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("heap"), heap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("help"), help, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Print option help", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("high-entropy-va"), high_entropy_va, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'high entropy VA' flag", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("icf="), icf_eq, Joined, INVALID, icf, nullptr, 0, 0,
       "Identical code folding", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("icf"), icf, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("image-base="), image_base_eq, Joined, INVALID, image_base, nullptr, 0, 0,
       "Base address of the program", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("image-base"), image_base, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("insert-timestamp"), insert_timestamp, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include PE header timestamp", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("kill-at"), kill_at, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove @n from exported symbols", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("large-address-aware"), large_address_aware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable large addresses", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("L"), L, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Add a directory to the library search path", "<dir>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("l"), l, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Root name of library to use", "<libName>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("major-image-version="), anonymous_7_eq, Joined, INVALID, anonymous_7, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("major-image-version"), anonymous_7, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("major-os-version="), major_os_version_eq, Joined, INVALID, major_os_version, nullptr, 0, 0,
       "Set the OS and subsystem major version", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("major-os-version"), major_os_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("major-subsystem-version="), major_subsystem_version_eq, Joined, INVALID, major_subsystem_version, nullptr, 0, 0,
       "Set the OS and subsystem major version", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("major-subsystem-version"), major_subsystem_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Map="), map_eq, Joined, INVALID, map, nullptr, 0, 0,
       "Output a linker map", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("Map"), map, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("minor-image-version="), anonymous_8_eq, Joined, INVALID, anonymous_8, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("minor-image-version"), anonymous_8, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("minor-os-version="), minor_os_version_eq, Joined, INVALID, minor_os_version, nullptr, 0, 0,
       "Set the OS and subsystem minor version", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("minor-os-version"), minor_os_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("minor-subsystem-version="), minor_subsystem_version_eq, Joined, INVALID, minor_subsystem_version, nullptr, 0, 0,
       "Set the OS and subsystem minor version", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("minor-subsystem-version"), minor_subsystem_version, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("mllvm="), mllvm_eq, Joined, INVALID, mllvm, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("mllvm"), mllvm, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("m"), m, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Set target emulation", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-allow-multiple-definition"), no_allow_multiple_definition, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not allow multiple definitions (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-demangle"), no_demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbol names", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-dynamicbase"), alias_no_dynamicbase, Flag, INVALID, disable_dynamicbase, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-fatal-warnings"), no_fatal_warnings, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not treat warnings as errors (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-gc-sections"), no_gc_sections, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't remove unused sections", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-guard-cf"), no_guard_cf, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not enable Control Flow Guard (default)", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-guard-longjmp"), no_guard_longjmp, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not enable Control Flow Guard long jump hardening", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-insert-timestamp"), no_insert_timestamp, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't include PE header timestamp", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-seh"), no_seh, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'no SEH' flag in the executable", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-undefined"), anonymous_9, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-whole-archive"), no_whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "No longer include all object files for following archives", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("non_shared"), alias_Bstatic_non_shared, Flag, INVALID, Bstatic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("nxcompat"), nxcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'nxcompat' flag in the executable", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("out-implib="), out_implib_eq, Joined, INVALID, out_implib, nullptr, 0, 0,
       "Import library name", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("out-implib"), out_implib, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("output-def="), output_def_eq, Joined, INVALID, output_def, nullptr, 0, 0,
       "Output def file", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("output-def"), output_def, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("O"), anonymous_0, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("o"), o, JoinedOrSeparate, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", "<path>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("pdb="), pdb_eq, Joined, INVALID, pdb, nullptr, 0, 0,
       "Output PDB debug info file, chosen implicitly if the argument is empty", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("pdb"), pdb, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("pic-executable"), anonymous_10, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt="), anonymous_12_eq, Joined, INVALID, anonymous_12, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin-opt"), anonymous_12, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin="), anonymous_11_eq, Joined, INVALID, anonymous_11, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("plugin"), anonymous_11, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("reproduce="), reproduce_eq, Joined, INVALID, reproduce, nullptr, 0, 0,
       "Write a tar file containing input files and command line options to reproduce link", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("reproduce"), reproduce, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("require-defined="), require_defined_eq, Joined, INVALID, require_defined, nullptr, 0, 0,
       "Force symbol to be added to symbol table as an undefined one", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("require-defined"), require_defined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("section-alignment="), section_alignment_eq, Joined, INVALID, section_alignment, nullptr, 0, 0,
       "Set section alignment", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("section-alignment"), section_alignment, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("shared"), shared, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Build a shared object", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("stack="), stack_eq, Joined, INVALID, stack, nullptr, 0, 0,
       "Set size of the initial stack", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("stack"), stack, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("start-group"), anonymous_14, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("static"), alias_Bstatic_static, Flag, INVALID, Bstatic, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("strip-all"), strip_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Omit all symbol information from the output binary", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("strip-debug"), strip_debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Omit all debug information, but keep symbol information", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("subsystem="), subs_eq, Joined, INVALID, subs, nullptr, 0, 0,
       "Specify subsystem", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("subsystem"), subs, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sysroot="), anonymous_13_eq, Joined, INVALID, anonymous_13, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("sysroot"), anonymous_13, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("S"), alias_strip_S, Flag, INVALID, strip_debug, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("s"), alias_strip_s, Flag, INVALID, strip_all, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-cache-dir="), thinlto_cache_dir_eq, Joined, INVALID, thinlto_cache_dir, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral("thinlto-cache-dir"), thinlto_cache_dir, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("tsaware"), tsaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the 'Terminal Server aware' flag", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("undefined="), undefined_eq, Joined, INVALID, undefined, nullptr, 0, 0,
       "Include symbol in the link, if available", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("undefined"), undefined, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("u"), alias_undefined_u, JoinedOrSeparate, INVALID, undefined, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("verbose"), verbose, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Verbose mode", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("version"), version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("v"), v, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("whole-archive"), whole_archive, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include all object files for following archives", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("wrap="), wrap_eq, Joined, INVALID, wrap, nullptr, 0, 0,
       "Use wrapper functions for symbol", "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("wrap"), wrap, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<symbol>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("Xlink="), Xlink_eq, Joined, INVALID, Xlink, nullptr, 0, 0,
       "Pass <arg> to the COFF linker", "<arg>", nullptr)
OPTION(prefix_2, llvm::StringLiteral("Xlink"), Xlink, Separate, INVALID, INVALID, nullptr, 0, 0, nullptr, "<arg>", nullptr)
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


