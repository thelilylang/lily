/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_CORE_LILY_PACKAGE_PACKAGE_H
#define LILY_CORE_LILY_PACKAGE_PACKAGE_H

#include <base/string.h>
#include <base/vec.h>

#include <cli/config/compile.h>

#include <core/lily/package/library.h>
#include <core/lily/precompile.h>
#include <core/lily/preparser.h>
#include <core/lily/scanner.h>
#include <core/lily/visibility.h>

enum LilyPackageStatus
{
    LILY_PACKAGE_STATUS_MAIN, // Contains the main function
    LILY_PACKAGE_STATUS_NORMAL
};

typedef struct LilyDumpConfig
{
    bool dump_scanner;
    bool dump_parser;
    bool dump_typecheck;
    bool dump_ir;
} LilyDumpConfig;

inline CONSTRUCTOR(LilyDumpConfig,
                   LilyDumpConfig,
                   bool dump_scanner,
                   bool dump_parser,
                   bool dump_typecheck,
                   bool dump_ir)
{
    return (LilyDumpConfig){ .dump_scanner = dump_scanner,
                             .dump_parser = dump_parser,
                             .dump_typecheck = dump_typecheck,
                             .dump_ir = dump_ir };
}

typedef struct LilyPackage
{
    String *name;
    Vec *public_macros;        // Vec<LilyPreparserMacro*>*?
    Vec *private_macros;       // Vec<LilyPreparserMacro*>*
    Vec *public_imports;       // Vec<LilyImport*>*
    Vec *private_imports;      // Vec<LilyImport*>*
    Vec *sub_packages;         // Vec<LilyPackage*>*
    Vec *package_dependencies; // Vec<LilyPackage*>*
    Vec *lib_dependencies;     // Vec<LilyLibrary*>*
    File file;
    LilyScanner scanner;     // LilyScanner
    LilyPreparser preparser; // LilyPreparser
    LilyPreparserInfo preparser_info;
    LilyPrecompile precompile; // LilyPrecompile
    // LilyLibrary library;
    enum LilyVisibility visibility;
    enum LilyPackageStatus status;
} LilyPackage;

/**
 *
 * @brief Construct LilyPackage type.
 */
CONSTRUCTOR(LilyPackage *,
            LilyPackage,
            String *name,
            enum LilyVisibility visibility,
            char *filename,
            enum LilyPackageStatus status,
            const char *default_path);

/**
 *
 * @brief Build all packages.
 * @return LilyPackage*?
 */
LilyPackage *
build__LilyPackage(const CompileConfig *config,
                   String *name,
                   enum LilyVisibility visibility,
                   enum LilyPackageStatus status,
                   const char *default_path);

/**
 *
 * @brief Compile package.
 * @note Begin by the main package.
 * @return LilyPackage*?
 */
LilyPackage *
compile__LilyPackage(const CompileConfig *config,
                     String *name,
                     enum LilyVisibility visibility,
                     enum LilyPackageStatus status,
                     const char *default_path);

const File *
get_file_from_filename__LilyPackage(const LilyPackage *package,
                                    const char *filename);

/**
 *
 * @brief Free LilyPackage type.
 */
DESTRUCTOR(LilyPackage, LilyPackage *self);

#endif // LILY_CORE_LILY_PACKAGE_PACKAGE_H