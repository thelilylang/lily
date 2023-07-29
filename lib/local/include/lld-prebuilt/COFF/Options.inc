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
PREFIX(prefix_2, {"--" COMMA nullptr})
PREFIX(prefix_1, {"/" COMMA "-" COMMA "/?" COMMA "-?" COMMA nullptr})
PREFIX(prefix_3, {"/??" COMMA "-??" COMMA "/?" COMMA "-?" COMMA nullptr})
#undef COMMA
#endif // PREFIX

/////////
// Groups

#ifdef OPTION

//////////
// Options

OPTION(prefix_0, &"<input>"[0], INPUT, Input, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_0, &"<unknown>"[0], UNKNOWN, Unknown, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/align:"[1], align, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Section alignment", nullptr, nullptr)
OPTION(prefix_1, &"/aligncomm:"[1], aligncomm, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set common symbol alignment", nullptr, nullptr)
OPTION(prefix_1, &"/allowbind:no"[1], allowbind_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable DLL binding", nullptr, nullptr)
OPTION(prefix_1, &"/allowbind"[1], allowbind, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable DLL binding (default)", nullptr, nullptr)
OPTION(prefix_1, &"/allowisolation:no"[1], allowisolation_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable DLL isolation", nullptr, nullptr)
OPTION(prefix_1, &"/allowisolation"[1], allowisolation, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable DLL isolation (default)", nullptr, nullptr)
OPTION(prefix_1, &"/alternatename:"[1], alternatename, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Define weak alias", nullptr, nullptr)
OPTION(prefix_1, &"/appcontainer:no"[1], appcontainer_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Image can run outside an app container (default)", nullptr, nullptr)
OPTION(prefix_1, &"/appcontainer"[1], appcontainer, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Image can only be run in an app container", nullptr, nullptr)
OPTION(prefix_1, &"/auto-import:no"[1], auto_import_no, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/auto-import"[1], auto_import, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/base:"[1], base, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Base address of the program", nullptr, nullptr)
OPTION(prefix_1, &"/Brepro"[1], repro, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Use a hash of the executable as the PE header timestamp", nullptr, nullptr)
OPTION(prefix_1, &"/call-graph-ordering-file:"[1], call_graph_ordering_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Layout sections to optimize the given callgraph", nullptr, nullptr)
OPTION(prefix_1, &"/call-graph-profile-sort:no"[1], call_graph_profile_sort_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not reorder sections with call graph profile", nullptr, nullptr)
OPTION(prefix_1, &"/call-graph-profile-sort"[1], call_graph_profile_sort, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Reorder sections with call graph profile (default)", nullptr, nullptr)
OPTION(prefix_1, &"/cetcompat:no"[1], cetcompat_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't mark executable image as compatible with Control-flow Enforcement Technology (CET) Shadow Stack (default)", nullptr, nullptr)
OPTION(prefix_1, &"/cetcompat"[1], cetcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Mark executable image as compatible with Control-flow Enforcement Technology (CET) Shadow Stack", nullptr, nullptr)
OPTION(prefix_2, &"--color-diagnostics="[2], color_diagnostics_eq, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use colors in diagnostics (default: auto)", "[auto,always,never]", nullptr)
OPTION(prefix_2, &"--color-diagnostics"[2], color_diagnostics, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=always", nullptr, nullptr)
OPTION(prefix_1, &"/debug:"[1], debug_opt, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Embed a symbol table in the image with option", nullptr, nullptr)
OPTION(prefix_1, &"/debugtype:"[1], debugtype, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Debug Info Options", nullptr, nullptr)
OPTION(prefix_1, &"/debug"[1], debug, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Embed a symbol table in the image", nullptr, nullptr)
OPTION(prefix_1, &"/def:"[1], deffile, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Use module-definition file", nullptr, nullptr)
OPTION(prefix_1, &"/defaultlib:"[1], defaultlib, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Add the library to the list of input files", nullptr, nullptr)
OPTION(prefix_1, &"/delay:"[1], delay, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/delayload:"[1], delayload, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Delay loaded DLL name", nullptr, nullptr)
OPTION(prefix_1, &"/demangle:no"[1], demangle_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Do not demangle symbols in output", nullptr, nullptr)
OPTION(prefix_1, &"/demangle"[1], demangle, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Demangle symbols in output (default)", nullptr, nullptr)
OPTION(prefix_1, &"/diasdkdir:"[1], diasdkdir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set the location of the DIA SDK", nullptr, nullptr)
OPTION(prefix_1, &"/disallowlib:"[1], disallowlib, Joined, INVALID, nodefaultlib, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/dll"[1], dll, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create a DLL", nullptr, nullptr)
OPTION(prefix_1, &"/driver:uponly,wdm"[1], driver_uponly_wdm, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/driver:uponly"[1], driver_uponly, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set IMAGE_DLL_CHARACTERISTICS_WDM_DRIVER bit in PE header", nullptr, nullptr)
OPTION(prefix_1, &"/driver:wdm,uponly"[1], driver_wdm_uponly, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/driver:wdm"[1], driver_wdm, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set IMAGE_FILE_UP_SYSTEM_ONLY bit in PE header", nullptr, nullptr)
OPTION(prefix_1, &"/driver"[1], driver, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Generate a Windows NT Kernel Mode Driver", nullptr, nullptr)
OPTION(prefix_1, &"/dynamicbase:no"[1], dynamicbase_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable ASLR (default when /fixed)", nullptr, nullptr)
OPTION(prefix_1, &"/dynamicbase"[1], dynamicbase, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable ASLR (default unless /fixed)", nullptr, nullptr)
OPTION(prefix_1, &"/editandcontinue"[1], editandcontinue, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/end-lib"[1], end_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "End group of objects treated as if they were in a library", nullptr, nullptr)
OPTION(prefix_1, &"/entry:"[1], entry, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Name of entry point symbol", nullptr, nullptr)
OPTION(prefix_1, &"/errorlimit:"[1], errorlimit, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Maximum number of errors to emit before stopping (0 = no limit)", nullptr, nullptr)
OPTION(prefix_1, &"/errorreport:"[1], errorreport, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/exclude-all-symbols"[1], exclude_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/exclude-symbols:"[1], exclude_symbols, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Exclude symbols from automatic export", "<symbol[,symbol,...]>", nullptr)
OPTION(prefix_1, &"/export-all-symbols"[1], export_all_symbols, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/export:"[1], export, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Export a function", nullptr, nullptr)
OPTION(prefix_1, &"/failifmismatch:"[1], failifmismatch, Joined, INVALID, INVALID, nullptr, 0, 0,
       "", nullptr, nullptr)
OPTION(prefix_1, &"/fastfail"[1], fastfail, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/filealign:"[1], filealign, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Section alignment in the output file", nullptr, nullptr)
OPTION(prefix_1, &"/fixed:no"[1], fixed_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable base relocations (default)", nullptr, nullptr)
OPTION(prefix_1, &"/fixed"[1], fixed, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable base relocations", nullptr, nullptr)
OPTION(prefix_1, &"/force:multipleres"[1], force_multipleres, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiply defined resources when creating executables", nullptr, nullptr)
OPTION(prefix_1, &"/force:multiple"[1], force_multiple, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow multiply defined symbols when creating executables", nullptr, nullptr)
OPTION(prefix_1, &"/force:unresolved"[1], force_unresolved, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow undefined symbols when creating executables", nullptr, nullptr)
OPTION(prefix_1, &"/force"[1], force, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Allow undefined and multiply defined symbols", nullptr, nullptr)
OPTION(prefix_1, &"/functionpadmin:"[1], functionpadmin_opt, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Prepares an image for hotpatching", nullptr, nullptr)
OPTION(prefix_1, &"/functionpadmin"[1], functionpadmin, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/guard:"[1], guard, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Control flow guard", nullptr, nullptr)
OPTION(prefix_1, &"/guardsym:"[1], guardsym, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/heap:"[1], heap, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Size of the heap", nullptr, nullptr)
OPTION(prefix_1, &"/help"[1], help, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/highentropyva:no"[1], highentropyva_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable 64-bit ASLR", nullptr, nullptr)
OPTION(prefix_1, &"/highentropyva"[1], highentropyva, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable 64-bit ASLR (default on 64-bit)", nullptr, nullptr)
OPTION(prefix_1, &"/idlout:"[1], idlout, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/ignore:"[1], ignore, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify warning codes to ignore", nullptr, nullptr)
OPTION(prefix_1, &"/ignoreidl"[1], ignoreidl, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/ilk:"[1], ilk, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/implib:"[1], implib, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Import library name", nullptr, nullptr)
OPTION(prefix_1, &"/include:"[1], incl, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Force symbol to be added to symbol table as undefined one", nullptr, nullptr)
OPTION(prefix_1, &"/includeoptional:"[1], include_optional, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Add symbol as undefined, but allow it to remain undefined", nullptr, nullptr)
OPTION(prefix_1, &"/incremental:no"[1], incremental_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Overwrite import library even if contents are unchanged", nullptr, nullptr)
OPTION(prefix_1, &"/incremental"[1], incremental, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Keep original import library if contents are unchanged", nullptr, nullptr)
OPTION(prefix_1, &"/integritycheck:no"[1], integritycheck_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "No effect (default)", nullptr, nullptr)
OPTION(prefix_1, &"/integritycheck"[1], integritycheck, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Set FORCE_INTEGRITY bit in PE header", nullptr, nullptr)
OPTION(prefix_1, &"/kernel"[1], kernel, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/kill-at"[1], kill_at, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/largeaddressaware:no"[1], largeaddressaware_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable large addresses (default on 32-bit)", nullptr, nullptr)
OPTION(prefix_1, &"/largeaddressaware"[1], largeaddressaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable large addresses (default on 64-bit)", nullptr, nullptr)
OPTION(prefix_1, &"/libpath:"[1], libpath, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Additional library search path", nullptr, nullptr)
OPTION(prefix_1, &"/lib"[1], lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Act like lib.exe; must be first argument if present", nullptr, nullptr)
OPTION(prefix_1, &"/linkrepro:"[1], linkrepro, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Write repro.tar containing inputs and command to reproduce link", "directory", nullptr)
OPTION(prefix_1, &"/lldignoreenv"[1], lldignoreenv, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Ignore environment variables like %LIB%", nullptr, nullptr)
OPTION(prefix_1, &"/lldltocache:"[1], lldltocache, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to ThinLTO cached object file directory", nullptr, nullptr)
OPTION(prefix_1, &"/lldltocachepolicy:"[1], lldltocachepolicy, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Pruning policy for the ThinLTO cache", nullptr, nullptr)
OPTION(prefix_1, &"/lldmap:"[1], lldmap_file, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/lldmap"[1], lldmap, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/lldmingw"[1], lldmingw, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/lldsavetemps"[1], lldsavetemps, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Save intermediate LTO compilation results", nullptr, nullptr)
OPTION(prefix_1, &"/ltcg:"[1], ltcg_opt, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/ltcgout:"[1], ltcgout, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/ltcg"[1], ltcg, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/lto-cs-profile-file:"[1], lto_cs_profile_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Context sensitive profile file path", nullptr, nullptr)
OPTION(prefix_1, &"/lto-cs-profile-generate"[1], lto_cs_profile_generate, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Perform context sensitive PGO instrumentation", nullptr, nullptr)
OPTION(prefix_1, &"/lto-obj-path:"[1], lto_obj_path, Joined, INVALID, INVALID, nullptr, 0, 0,
       "output native object for merged LTO unit to this path", nullptr, nullptr)
OPTION(prefix_1, &"/lto-pgo-warn-mismatch:no"[1], lto_pgo_warn_mismatch_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn off warnings about profile cfg mismatch", nullptr, nullptr)
OPTION(prefix_1, &"/lto-pgo-warn-mismatch"[1], lto_pgo_warn_mismatch, Flag, INVALID, INVALID, nullptr, 0, 0,
       "turn on warnings about profile cfg mismatch (default)>", nullptr, nullptr)
OPTION(prefix_1, &"/machine:"[1], machine, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify target platform", nullptr, nullptr)
OPTION(prefix_1, &"/manifest:"[1], manifest_colon, Joined, INVALID, INVALID, nullptr, 0, 0,
       "NO disables manifest output; EMBED[,ID=#] embeds manifest as resource in the image", nullptr, nullptr)
OPTION(prefix_1, &"/manifestdependency:"[1], manifestdependency, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Attributes for <dependency> element in manifest file; implies /manifest", nullptr, nullptr)
OPTION(prefix_1, &"/manifestfile:"[1], manifestfile, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Manifest output path, with /manifest", nullptr, nullptr)
OPTION(prefix_1, &"/manifestinput:"[1], manifestinput, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Additional manifest inputs; only valid with /manifest:embed", nullptr, nullptr)
OPTION(prefix_1, &"/manifestuac:"[1], manifestuac, Joined, INVALID, INVALID, nullptr, 0, 0,
       "User access control", nullptr, nullptr)
OPTION(prefix_1, &"/manifest"[1], manifest, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create .manifest file", nullptr, nullptr)
OPTION(prefix_1, &"/map:"[1], map_file, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/map"[1], map, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/maxilksize:"[1], maxilksize, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/merge:"[1], merge, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Combine sections", nullptr, nullptr)
OPTION(prefix_1, &"/mllvm:"[1], mllvm, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Options to pass to LLVM", nullptr, nullptr)
OPTION(prefix_1, &"/natvis:"[1], natvis, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to natvis file to embed in the PDB", nullptr, nullptr)
OPTION(prefix_2, &"--no-color-diagnostics"[2], no_color_diagnostics, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Alias for --color-diagnostics=never", nullptr, nullptr)
OPTION(prefix_1, &"/nodefaultlib:"[1], nodefaultlib, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Remove a default library", nullptr, nullptr)
OPTION(prefix_1, &"/nodefaultlib"[1], nodefaultlib_all, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Remove all default libraries", nullptr, nullptr)
OPTION(prefix_1, &"/noentry"[1], noentry, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't add reference to DllMainCRTStartup; only valid with /dll", nullptr, nullptr)
OPTION(prefix_1, &"/noimplib"[1], noimplib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't output an import lib", nullptr, nullptr)
OPTION(prefix_1, &"/nologo"[1], nologo, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/noseh"[1], noseh, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/nxcompat:no"[1], nxcompat_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Disable data execution provention", nullptr, nullptr)
OPTION(prefix_1, &"/nxcompat"[1], nxcompat, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Enable data execution prevention (default)", nullptr, nullptr)
OPTION(prefix_1, &"/opt:"[1], opt, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Control optimizations", nullptr, nullptr)
OPTION(prefix_1, &"/order:"[1], order, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Put functions in order", nullptr, nullptr)
OPTION(prefix_1, &"/osversion:"[1], osversion, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/out:"[1], out, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to file to write output", nullptr, nullptr)
OPTION(prefix_1, &"/output-def:"[1], output_def, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/pdb:"[1], pdb, Joined, INVALID, INVALID, nullptr, 0, 0,
       "PDB file path", nullptr, nullptr)
OPTION(prefix_1, &"/pdbaltpath:"[1], pdbaltpath, Joined, INVALID, INVALID, nullptr, 0, 0,
       "PDB file path to embed in the image", nullptr, nullptr)
OPTION(prefix_1, &"/pdbcompress"[1], pdbcompress, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/pdbpagesize:"[1], pdbpagesize, Joined, INVALID, INVALID, nullptr, 0, 0,
       "PDB page size", nullptr, nullptr)
OPTION(prefix_1, &"/pdbsourcepath:"[1], pdb_source_path, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Base path used to make relative source file path absolute in PDB", nullptr, nullptr)
OPTION(prefix_1, &"/pdbstream:"[1], pdbstream, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Embed the contents of <file> in the PDB as named stream <name>", "<name>=<file>", nullptr)
OPTION(prefix_1, &"/pdbstripped:"[1], pdbstripped, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Stripped PDB file path", nullptr, nullptr)
OPTION(prefix_1, &"/print-symbol-order:"[1], print_symbol_order, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Print a symbol order specified by /call-graph-ordering-file and /call-graph-profile-sort into the specified file", nullptr, nullptr)
OPTION(prefix_1, &"/profile"[1], profile, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/reproduce:"[1], reproduce, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Write tar file containing inputs and command to reproduce link", "filename", nullptr)
OPTION(prefix_2, &"--rsp-quoting="[2], rsp_quoting, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Quoting style for response files, 'windows' (default) or 'posix'", nullptr, nullptr)
OPTION(prefix_1, &"/runtime-pseudo-reloc:no"[1], runtime_pseudo_reloc_no, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/runtime-pseudo-reloc"[1], runtime_pseudo_reloc, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/safeseh:no"[1], safeseh_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't produce an image with Safe Exception Handler", nullptr, nullptr)
OPTION(prefix_1, &"/safeseh"[1], safeseh, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Produce an image with Safe Exception Handler (only for x86)", nullptr, nullptr)
OPTION(prefix_1, &"/section:"[1], section, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify section attributes", nullptr, nullptr)
OPTION(prefix_1, &"/stack:"[1], stack, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Size of the stack", nullptr, nullptr)
OPTION(prefix_1, &"/start-lib"[1], start_lib, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Start group of objects treated as if they were in a library", nullptr, nullptr)
OPTION(prefix_1, &"/stdcall-fixup:no"[1], stdcall_fixup_no, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/stdcall-fixup"[1], stdcall_fixup, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/stub:"[1], stub, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify DOS stub file", nullptr, nullptr)
OPTION(prefix_1, &"/subsystem:"[1], subsystem, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify subsystem", nullptr, nullptr)
OPTION(prefix_1, &"/summary"[1], summary, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/swaprun:cd"[1], swaprun_cd, Flag, INVALID, swaprun, "cd\0", 0, 0,
       "Make loader run output binary from swap instead of from CD", nullptr, nullptr)
OPTION(prefix_1, &"/swaprun:net"[1], swaprun_net, Flag, INVALID, swaprun, "net\0", 0, 0,
       "Make loader run output binary from swap instead of from network", nullptr, nullptr)
OPTION(prefix_1, &"/swaprun:"[1], swaprun, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Comma-separated list of 'cd' or 'net'", nullptr, nullptr)
OPTION(prefix_1, &"/thinlto-emit-imports-files"[1], thinlto_emit_imports_files, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Emit .imports files with -thinlto-index-only", nullptr, nullptr)
OPTION(prefix_1, &"/thinlto-index-only:"[1], thinlto_index_only_arg, Joined, INVALID, INVALID, nullptr, 0, 0,
       "-thinlto-index-only and also write native module names to file", nullptr, nullptr)
OPTION(prefix_1, &"/thinlto-index-only"[1], thinlto_index_only, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Instead of linking, emit ThinLTO index files", nullptr, nullptr)
OPTION(prefix_1, &"/thinlto-object-suffix-replace:"[1], thinlto_object_suffix_replace, Joined, INVALID, INVALID, nullptr, 0, 0,
       "'old;new' replace old suffix with new suffix in ThinLTO index", nullptr, nullptr)
OPTION(prefix_1, &"/thinlto-prefix-replace:"[1], thinlto_prefix_replace, Joined, INVALID, INVALID, nullptr, 0, 0,
       "'old;new' replace old prefix with new prefix in ThinLTO outputs", nullptr, nullptr)
OPTION(prefix_1, &"/threads:"[1], threads, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Number of threads. '1' disables multi-threading. By default all available hardware threads are used", nullptr, nullptr)
OPTION(prefix_1, &"/throwingnew"[1], throwingnew, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/timestamp:"[1], timestamp, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify the PE header timestamp", nullptr, nullptr)
OPTION(prefix_1, &"/time"[1], show_timing, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/tlbid:"[1], tlbid, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/tlbout:"[1], tlbout, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/tsaware:no"[1], tsaware_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create non-Terminal Server aware executable", nullptr, nullptr)
OPTION(prefix_1, &"/tsaware"[1], tsaware, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Create Terminal Server aware executable (default)", nullptr, nullptr)
OPTION(prefix_1, &"/vctoolsdir:"[1], vctoolsdir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set the location of the VC tools", nullptr, nullptr)
OPTION(prefix_1, &"/vctoolsversion:"[1], vctoolsversion, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify which VC tools version to use", nullptr, nullptr)
OPTION(prefix_1, &"/verbose:"[1], verbose_all, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/verbose"[1], verbose, Flag, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/version:"[1], version, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify a version number in the PE header", nullptr, nullptr)
OPTION(prefix_2, &"--version"[2], dash_dash_version, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Display the version number and exit", nullptr, nullptr)
OPTION(prefix_1, &"/vfsoverlay:"[1], vfsoverlay, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Path to a vfsoverlay yaml file to optionally look for /defaultlib's in", nullptr, nullptr)
OPTION(prefix_1, &"/wholearchive:"[1], wholearchive_file, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Include all object files from this library", nullptr, nullptr)
OPTION(prefix_1, &"/wholearchive"[1], wholearchive_flag, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Include all object files from all libraries", nullptr, nullptr)
OPTION(prefix_1, &"/winsdkdir:"[1], winsdkdir, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Set the location of the Windows SDK", nullptr, nullptr)
OPTION(prefix_1, &"/winsdkversion:"[1], winsdkversion, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Specify which SDK version to use", nullptr, nullptr)
OPTION(prefix_1, &"/winsysroot:"[1], winsysroot, Joined, INVALID, INVALID, nullptr, 0, 0,
       "Adds several subdirectories to the library search paths", nullptr, nullptr)
OPTION(prefix_1, &"/wrap:"[1], wrap, Joined, INVALID, INVALID, nullptr, 0, 0, nullptr, nullptr, nullptr)
OPTION(prefix_1, &"/WX:no"[1], WX_no, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Don't treat warnings as errors (default)", nullptr, nullptr)
OPTION(prefix_1, &"/WX"[1], WX, Flag, INVALID, INVALID, nullptr, 0, 0,
       "Treat warnings as errors", nullptr, nullptr)
OPTION(prefix_3, &"/??"[3], help_q, Flag, INVALID, help, nullptr, 0, 0, nullptr, nullptr, nullptr)
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
