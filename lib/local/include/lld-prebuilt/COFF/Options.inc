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
PREFIX(prefix_2, {llvm::StringLiteral("--") COMMA llvm::StringLiteral("")})
PREFIX(prefix_1, {llvm::StringLiteral("/") COMMA llvm::StringLiteral("-") COMMA llvm::StringLiteral("/?") COMMA llvm::StringLiteral("-?") COMMA llvm::StringLiteral("")})
PREFIX(prefix_3, {llvm::StringLiteral("/??") COMMA llvm::StringLiteral("-??") COMMA llvm::StringLiteral("/?") COMMA llvm::StringLiteral("-?") COMMA llvm::StringLiteral("")})
#undef COMMA
#endif // PREFIX

/////////
// Prefix Union

#ifdef PREFIX_UNION
#define COMMA ,
PREFIX_UNION({
llvm::StringLiteral("-") COMMA llvm::StringLiteral("--") COMMA llvm::StringLiteral("-?") COMMA llvm::StringLiteral("-??") COMMA llvm::StringLiteral("/") COMMA llvm::StringLiteral("/?") COMMA llvm::StringLiteral("/??") COMMA llvm::StringLiteral("")})
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
OPTION(prefix_1, llvm::StringLiteral("align:"), align, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Section alignment", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("aligncomm:"), aligncomm, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set common symbol alignment", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allowbind:no"), allowbind_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable DLL binding", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allowbind"), allowbind, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable DLL binding (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allowisolation:no"), allowisolation_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable DLL isolation", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("allowisolation"), allowisolation, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable DLL isolation (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("alternatename:"), alternatename, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Define weak alias", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("appcontainer:no"), appcontainer_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Image can run outside an app container (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("appcontainer"), appcontainer, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Image can only be run in an app container", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("assemblydebug:"), assemblydebug_opt, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("assemblydebug"), assemblydebug, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("auto-import:no"), auto_import_no, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("auto-import"), auto_import, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("base:"), base, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Base address of the program", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("Brepro"), repro, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use a hash of the executable as the PE header timestamp", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("call-graph-ordering-file:"), call_graph_ordering_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Layout sections to optimize the given callgraph", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("call-graph-profile-sort:no"), call_graph_profile_sort_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not reorder sections with call graph profile", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("call-graph-profile-sort"), call_graph_profile_sort, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Reorder sections with call graph profile (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("cetcompat:no"), cetcompat_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't mark executable image as compatible with Control-flow Enforcement Technology (CET) Shadow Stack (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("cetcompat"), cetcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mark executable image as compatible with Control-flow Enforcement Technology (CET) Shadow Stack", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("color-diagnostics="), color_diagnostics_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_2, llvm::StringLiteral("color-diagnostics"), color_diagnostics, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("debug:"), debug_opt, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Embed a symbol table in the image with option", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("debugtype:"), debugtype, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Debug Info Options", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("debug"), debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Embed a symbol table in the image", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("def:"), deffile, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use module-definition file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("defaultlib:"), defaultlib, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Add the library to the list of input files", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("delay:"), delay, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("delayload:"), delayload, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Delay loaded DLL name", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("demangle:no"), demangle_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbols in output", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("demangle"), demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbols in output (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("diasdkdir:"), diasdkdir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set the location of the DIA SDK", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("disallowlib:"), disallowlib, Joined, INVALID, nodefaultlib, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dll"), dll, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create a DLL", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("driver:uponly,wdm"), driver_uponly_wdm, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("driver:uponly"), driver_uponly, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set IMAGE_DLL_CHARACTERISTICS_WDM_DRIVER bit in PE header", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("driver:wdm,uponly"), driver_wdm_uponly, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("driver:wdm"), driver_wdm, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set IMAGE_FILE_UP_SYSTEM_ONLY bit in PE header", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("driver"), driver, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate a Windows NT Kernel Mode Driver", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dynamicbase:no"), dynamicbase_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable ASLR (default when /fixed)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("dynamicbase"), dynamicbase, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable ASLR (default unless /fixed)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("editandcontinue"), editandcontinue, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("emitpogophaseinfo"), emitpogophaseinfo, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("end-lib"), end_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "End group of objects treated as if they were in a library", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("entry:"), entry, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Name of entry point symbol", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("errorlimit:"), errorlimit, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Maximum number of errors to emit before stopping (0 = no limit)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("errorreport:"), errorreport, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("exclude-all-symbols"), exclude_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("exclude-symbols:"), exclude_symbols, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Exclude symbols from automatic export", "<symbol[,symbol,...]>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("export-all-symbols"), export_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("export:"), export, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Export a function", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("failifmismatch:"), failifmismatch, Joined, INVALID, INVALID, nullptr, 0, 0,
       "", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("fastfail"), fastfail, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("filealign:"), filealign, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Section alignment in the output file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("fixed:no"), fixed_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable base relocations (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("fixed"), fixed, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable base relocations", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force:multipleres"), force_multipleres, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiply defined resources when creating executables", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force:multiple"), force_multiple, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiply defined symbols when creating executables", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force:unresolved"), force_unresolved, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow undefined symbols when creating executables", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("force"), force, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow undefined and multiply defined symbols", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("functionpadmin:"), functionpadmin_opt, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Prepares an image for hotpatching", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("functionpadmin"), functionpadmin, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("guard:"), guard, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Control flow guard", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("guardsym:"), guardsym, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("heap:"), heap, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Size of the heap", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("help"), help, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("highentropyva:no"), highentropyva_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable 64-bit ASLR", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("highentropyva"), highentropyva, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable 64-bit ASLR (default on 64-bit)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("idlout:"), idlout, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ignore:"), ignore, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify warning codes to ignore", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ignoreidl"), ignoreidl, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ilk:"), ilk, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("implib:"), implib, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Import library name", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("include:"), incl, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Force symbol to be added to symbol table as undefined one", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("includeoptional:"), include_optional, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Add symbol as undefined, but allow it to remain undefined", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("incremental:no"), incremental_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Overwrite import library even if contents are unchanged", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("incremental"), incremental, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Keep original import library if contents are unchanged", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("inferasanlibs:no"), inferasanlibs_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "No effect (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("inferasanlibs"), inferasanlibs, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Unused, generates a warning", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("integritycheck:no"), integritycheck_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "No effect (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("integritycheck"), integritycheck, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set FORCE_INTEGRITY bit in PE header", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("kernel"), kernel, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("kill-at"), kill_at, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("largeaddressaware:no"), largeaddressaware_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable large addresses (default on 32-bit)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("largeaddressaware"), largeaddressaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable large addresses (default on 64-bit)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("libpath:"), libpath, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Additional library search path", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lib"), lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Act like lib.exe; must be first argument if present", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("linkrepro:"), linkrepro, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Write repro.tar containing inputs and command to reproduce link", "directory", nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldignoreenv"), lldignoreenv, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignore environment variables like %LIB%", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldltocache:"), lldltocache, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldltocachepolicy:"), lldltocachepolicy, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldmap:"), lldmap_file, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldmap"), lldmap, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldmingw"), lldmingw, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lldsavetemps"), lldsavetemps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ltcg:"), ltcg_opt, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ltcgout:"), ltcgout, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("ltcg"), ltcg, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lto-cs-profile-file:"), lto_cs_profile_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Context sensitive profile file path", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lto-cs-profile-generate"), lto_cs_profile_generate, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Perform context sensitive PGO instrumentation", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lto-obj-path:"), lto_obj_path, Joined, INVALID, INVALID, nullptr, 0, 0,
       "output native object for merged LTO unit to this path", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lto-pgo-warn-mismatch:no"), lto_pgo_warn_mismatch_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn off warnings about profile cfg mismatch", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("lto-pgo-warn-mismatch"), lto_pgo_warn_mismatch, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn on warnings about profile cfg mismatch (default)>", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("machine:"), machine, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify target platform", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("manifest:"), manifest_colon, Joined, INVALID, INVALID, nullptr, 0, 0,
       "NO disables manifest output; EMBED[,ID=#] embeds manifest as resource in the image", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("manifestdependency:"), manifestdependency, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Attributes for <dependency> element in manifest file; implies /manifest", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("manifestfile:"), manifestfile, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Manifest output path, with /manifest", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("manifestinput:"), manifestinput, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Additional manifest inputs; only valid with /manifest:embed", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("manifestuac:"), manifestuac, Joined, INVALID, INVALID, nullptr, 0, 0,
       "User access control", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("manifest"), manifest, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create .manifest file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("map:"), map_file, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("mapinfo:"), map_info, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Include the specified information in a map file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("map"), map, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("maxilksize:"), maxilksize, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("merge:"), merge, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Combine sections", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("mllvm:"), mllvm, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Options to pass to LLVM", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("natvis:"), natvis, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to natvis file to embed in the PDB", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("no-color-diagnostics"), no_color_diagnostics, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nodefaultlib:"), nodefaultlib, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Remove a default library", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nodefaultlib"), nodefaultlib_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove all default libraries", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("noentry"), noentry, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't add reference to DllMainCRTStartup; only valid with /dll", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("noimplib"), noimplib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't output an import lib", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nologo"), nologo, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("noseh"), noseh, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nxcompat:no"), nxcompat_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable data execution provention", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("nxcompat"), nxcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable data execution prevention (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("opt:"), opt, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Control optimizations", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("order:"), order, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Put functions in order", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("osversion:"), osversion, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("out:"), out, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("output-def:"), output_def, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdb:"), pdb, Joined, INVALID, INVALID, nullptr, 0, 0,
       "PDB file path", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdbaltpath:"), pdbaltpath, Joined, INVALID, INVALID, nullptr, 0, 0,
       "PDB file path to embed in the image", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdbcompress"), pdbcompress, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdbpagesize:"), pdbpagesize, Joined, INVALID, INVALID, nullptr, 0, 0,
       "PDB page size", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdbsourcepath:"), pdb_source_path, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Base path used to make relative source file path absolute in PDB", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdbstream:"), pdbstream, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Embed the contents of <file> in the PDB as named stream <name>", "<name>=<file>", nullptr)
OPTION(prefix_1, llvm::StringLiteral("pdbstripped:"), pdbstripped, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Stripped PDB file path", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("print-symbol-order:"), print_symbol_order, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Print a symbol order specified by /call-graph-ordering-file and /call-graph-profile-sort into the specified file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("profile"), profile, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("release"), release, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set the Checksum in the header of an PE file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("reproduce:"), reproduce, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Write tar file containing inputs and command to reproduce link", "filename", nullptr)
OPTION(prefix_2, llvm::StringLiteral("rsp-quoting="), rsp_quoting, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Quoting style for response files, 'windows' (default) or 'posix'", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("runtime-pseudo-reloc:no"), runtime_pseudo_reloc_no, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("runtime-pseudo-reloc"), runtime_pseudo_reloc, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("safeseh:no"), safeseh_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't produce an image with Safe Exception Handler", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("safeseh"), safeseh, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Produce an image with Safe Exception Handler (only for x86)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("section:"), section, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify section attributes", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("stack:"), stack, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Size of the stack", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("start-lib"), start_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Start group of objects treated as if they were in a library", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("stdcall-fixup:no"), stdcall_fixup_no, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("stdcall-fixup"), stdcall_fixup, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("stub:"), stub, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify DOS stub file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("subsystem:"), subsystem, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify subsystem", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("summary"), summary, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("swaprun:cd"), swaprun_cd, Flag, INVALID, swaprun, "cd\0", 0, 0,
       "Make loader run output binary from swap instead of from CD", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("swaprun:net"), swaprun_net, Flag, INVALID, swaprun, "net\0", 0, 0,
       "Make loader run output binary from swap instead of from network", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("swaprun:"), swaprun, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Comma-separated list of 'cd' or 'net'", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("thinlto-emit-imports-files"), thinlto_emit_imports_files, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Emit .imports files with -thinlto-index-only", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("thinlto-index-only:"), thinlto_index_only_arg, Joined, INVALID, INVALID, nullptr, 0, 0,
       "-thinlto-index-only and also write native module names to file", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("thinlto-index-only"), thinlto_index_only, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Instead of linking, emit ThinLTO index files", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("thinlto-object-suffix-replace:"), thinlto_object_suffix_replace, Joined, INVALID, INVALID, nullptr, 0, 0,
       "'old;new' replace old suffix with new suffix in ThinLTO index", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("thinlto-prefix-replace:"), thinlto_prefix_replace, Joined, INVALID, INVALID, nullptr, 0, 0,
       "'old;new' replace old prefix with new prefix in ThinLTO outputs", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("threads:"), threads, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("throwingnew"), throwingnew, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("timestamp:"), timestamp, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify the PE header timestamp", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("time"), show_timing, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("tlbid:"), tlbid, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("tlbout:"), tlbout, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("tsaware:no"), tsaware_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create non-Terminal Server aware executable", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("tsaware"), tsaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create Terminal Server aware executable (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("vctoolsdir:"), vctoolsdir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set the location of the VC tools", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("vctoolsversion:"), vctoolsversion, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify which VC tools version to use", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("verbose:"), verbose_all, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("verbose"), verbose, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("version:"), version, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify a version number in the PE header", nullptr, nullptr)
OPTION(prefix_2, llvm::StringLiteral("version"), dash_dash_version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("vfsoverlay:"), vfsoverlay, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to a vfsoverlay yaml file to optionally look for /defaultlib's in", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("wholearchive:"), wholearchive_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Include all object files from this library", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("wholearchive"), wholearchive_flag, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include all object files from all libraries", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("winsdkdir:"), winsdkdir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set the location of the Windows SDK", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("winsdkversion:"), winsdkversion, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify which SDK version to use", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("winsysroot:"), winsysroot, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Adds several subdirectories to the library search paths", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("wrap:"), wrap, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("WX:no"), WX_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't treat warnings as errors (default)", nullptr, nullptr)
OPTION(prefix_1, llvm::StringLiteral("WX"), WX, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_3, llvm::StringLiteral(""), help_q, Flag, INVALID, help, nullptr, 0, 0, nullptr, nullptr, nullptr)
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


