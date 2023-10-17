#ifndef UTIL_C
#define UTIL_C

#include <base/file.h>

#include <core/lily/compiler/package/package.h>
#include <core/lily/package/default_path.h>
#include <core/lily/package/package.h>
#include <core/lily/package/program.h>
#include <core/lily/precompiler/precompiler.h>

#define FILE_IMPORT "./tests/core/lily/precompiler/input/import.lily"
#define FILE_MACRO "./tests/core/lily/precompiler/input/macro.lily"
#define FILE_PACKAGE "./tests/core/lily/precompiler/input/package/main.lily"

#define RUN_PRECOMPILER(filename)                                  \
    LilyLibrary *lib = NULL;                                       \
    LilyProgram program = NEW(LilyProgram, LILY_PROGRAM_KIND_EXE); \
    char *default_path = generate_default_path(filename);          \
    LilyPackage *self = NEW_VARIANT(LilyPackage,                   \
                                    compiler,                      \
                                    NULL,                          \
                                    NULL,                          \
                                    LILY_VISIBILITY_PUBLIC,        \
                                    filename,                      \
                                    LILY_PACKAGE_STATUS_MAIN,      \
                                    default_path,                  \
                                    NULL,                          \
                                    NULL);                         \
                                                                   \
    LilyPackageConfig pkg_config = default__LilyPackageConfig();   \
                                                                   \
    self->config = &pkg_config;                                    \
                                                                   \
    run__LilyScanner(&self->scanner, self->config->dump_scanner);  \
                                                                   \
    /* NOTE: Set `destroy_all` on true to free all. */             \
    /* NOTE: If we run the program after the analysis, we will set \
    `destroy_all` to false. */                                     \
    self->preparser.destroy_all = true;                            \
                                                                   \
    run__LilyPreparser(&self->preparser, &self->preparser_info);   \
                                                                   \
    SET_ROOT_PACKAGE_NAME(self);                                   \
    SET_ROOT_PACKAGE_IR(self->config, self);                       \
    SET_ROOT_PACKAGE_PROGRAM(self, (&program), lib);               \
    LOAD_ROOT_PACKAGE_RESOURCES(self, (&program));                \
                                                                   \
    run__LilyPrecompiler(&self->precompiler, self, false);

#define FREE_PRECOMPILER()                      \
    init_module__LilyAnalysis(&self->analysis); \
    FREE(LilyPackage, self);                    \
    lily_free(default_path);                    \
    FREE(LilyProgram, &program);

#endif // UTIL_C
