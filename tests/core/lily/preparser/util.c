#ifndef UTIL_C
#define UTIL_C

#include <base/file.h>

#include <core/lily/preparser/preparser.h>

#define FILE_IMPORT "./tests/core/lily/preparser/input/import.lily"
#define FILE_MACRO "./tests/core/lily/preparser/input/macro.lily"
#define FILE_PACKAGE "./tests/core/lily/preparser/input/package.lily"
#define FILE_MODULE "./tests/core/lily/preparser/input/module.lily"
#define FILE_MACRO_EXPAND "./tests/core/lily/preparser/input/macro_expand.lily"
#define FILE_ASM "./tests/core/lily/preparser/input/asm.lily"
#define FILE_AWAIT "./tests/core/lily/preparser/input/await.lily"
#define FILE_BREAK "./tests/core/lily/preparser/input/break.lily"
#define FILE_DEFER "./tests/core/lily/preparser/input/defer.lily"
#define FILE_DROP "./tests/core/lily/preparser/input/drop.lily"
#define FILE_IF "./tests/core/lily/preparser/input/if.lily"

LilyPreparserInfo
run_preparser(File *file, LilyScanner *scanner, LilyPreparser *preparser)
{
    LilyPreparserInfo preparser_info =
      NEW(LilyPreparserInfo, from__String("example"));

    run__LilyScanner(scanner, false);
    run__LilyPreparser(preparser, &preparser_info);

    return preparser_info;
}

#define RUN_PREPARSER(filename)                                              \
    char *content = read_file__File(filename);                               \
    File file = NEW(File, filename, content);                                \
    LilyScanner scanner =                                                    \
      NEW(LilyScanner, NEW(Source, NEW(Cursor, file.content), &file));       \
    LilyPreparser preparser = NEW(LilyPreparser, &file, scanner.tokens, ""); \
    LilyPreparserInfo preparser_info =                                       \
      run_preparser(&file, &scanner, &preparser);

#define FREE_PREPARSER()                        \
    FREE(File, &file);                          \
    FREE(LilyScanner, &scanner);                \
    FREE(String, preparser_info.package->name); \
    FREE(LilyPreparserInfo, &preparser_info);

#define GET_PUBLIC_IMPORT(idx) \
    CAST(LilyPreparserImport *, get__Vec(preparser_info.public_imports, idx))
#define GET_PRIVATE_IMPORT(idx) \
    CAST(LilyPreparserImport *, get__Vec(preparser_info.private_imports, idx))
#define GET_PUBLIC_MACROS(idx) \
    CAST(LilyPreparserMacro *, get__Vec(preparser_info.public_macros, idx))
#define GET_PRIVATE_MACROS(idx) \
    CAST(LilyPreparserMacro *, get__Vec(preparser_info.private_macros, idx))
#define GET_DECL(idx) \
    CAST(LilyPreparserDecl *, get__Vec(preparser_info.decls, idx))

#define PUBLIC_IMPORT_ITERATOR() Usize public_imports_it = 0;
#define PRIVATE_IMPORT_ITERATOR() Usize private_imports_it = 0;
#define PUBLIC_MACROS_ITERATOR() Usize public_macros_it = 0;
#define PRIVATE_MACROS_ITERATOR() Usize private_macros_it = 0;
#define DECLS_ITERATOR() Usize decls_it = 0;

#define CURRENT_PUBLIC_IMPORT() \
    CAST(LilyPreparserImport *, \
         get__Vec(preparser_info.public_imports, public_imports_it))
#define CURRENT_PRIVATE_IMPORT() \
    CAST(LilyPreparserImport *,  \
         get__Vec(preparser_info.private_imports, private_imports_it))
#define CURRENT_PUBLIC_MACRO() \
    CAST(LilyPreparserMacro *, \
         get__Vec(preparser_info.public_macros, public_macros_it))
#define CURRENT_PRIVATE_MACRO() \
    CAST(LilyPreparserMacro *,  \
         get__Vec(preparser_info.private_macros, private_macros_it))
#define CURRENT_DECL() \
    CAST(LilyPreparserDecl *, get__Vec(preparser_info.decls, decls_it))

#define NEXT_PUBLIC_IMPORT()    \
    CAST(LilyPreparserImport *, \
         get__Vec(preparser_info.public_imports, public_imports_it++))
#define NEXT_PRIVATE_IMORT()    \
    CAST(LilyPreparserImport *, \
         get__Vec(preparser_info.private_imports, private_imports_it++))
#define NEXT_PUBLIC_MACRO()    \
    CAST(LilyPreparserMacro *, \
         get__Vec(preparser_info.public_macros, public_macros_it++))
#define NEXT_PRIVATE_MACRO()   \
    CAST(LilyPreparserMacro *, \
         get__Vec(preparser_info.private_macros, private_macros_it++))
#define NEXT_DECL() \
    CAST(LilyPreparserDecl *, get__Vec(preparser_info.decls, decls_it++))

#define PREVIOUS_PUBLIC_IMPORT() \
    CAST(LilyPreparserImport *,  \
         get__Vec(preparser_info.public_imports, public_imports_it - 1))
#define PREVIOUS_PRIVATE_IMORT() \
    CAST(LilyPreparserImport *,  \
         get__Vec(preparser_info.private_imports, private_imports_it - 1))
#define PREVIOUS_PUBLIC_MACRO() \
    CAST(LilyPreparserMacro *,  \
         get__Vec(preparser_info.public_macros, public_macros_it - 1))
#define PREVIOUS_PRIVATE_MACRO() \
    CAST(LilyPreparserMacro *,   \
         get__Vec(preparser_info.private_macros, private_macros_it - 1))
#define PREVIOUS_DECL() \
    CAST(LilyPreparserDecl *, get__Vec(preparser_info.decls, decls_it - 1))

#endif // UTIL_C
