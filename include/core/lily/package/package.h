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
#include <base/types.h>
#include <base/vec.h>

#include <cli/config/compile.h>

#include <core/lily/analysis.h>
#include <core/lily/builtin.h>
#include <core/lily/checked/operator_register.h>
#include <core/lily/ir.h>
#include <core/lily/linker.h>
#include <core/lily/package/config.h>
#include <core/lily/package/library.h>
#include <core/lily/parser.h>
#include <core/lily/precompile.h>
#include <core/lily/preparser.h>
#include <core/lily/scanner.h>
#include <core/lily/sys.h>
#include <core/lily/visibility.h>

enum LilyPackageStatus
{
    LILY_PACKAGE_STATUS_MAIN, // Can contain the main function
    LILY_PACKAGE_STATUS_NORMAL,
    LILY_PACKAGE_STATUS_SUB_MAIN,
};

typedef struct LilyPackage
{
    String *name;
    String *global_name;
    Vec *public_macros;              // Vec<LilyMacro*>*?
    Vec *private_macros;             // Vec<LilyMacro*>*
    Vec *public_imports;             // Vec<LilyImport*>*
    Vec *private_imports;            // Vec<LilyImport*>*
    Vec *sub_packages;               // Vec<LilyPackage*>*
    Vec *package_dependencies;       // Vec<LilyPackage* (&)>*
    Vec *lib_dependencies;           // Vec<LilyLibrary*>*
    const LilyPackageConfig *config; // LilyPackageConfig* (&)
    File file;
    LilyScanner scanner;     // LilyScanner
    LilyPreparser preparser; // LilyPreparser
    LilyPreparserInfo preparser_info;
    LilyPrecompile precompile; // LilyPrecompile
    LilyParser parser;
    LilyAnalysis analysis;
    LilyIr ir;
    LilyLinker linker;
    LilyBuiltinFun *builtins;
    LilySysFun *syss;
    Usize count_error;
    Usize count_warning;
    // count all errors and warnings after the precompiler step
    enum LilyVisibility visibility;
    enum LilyPackageStatus status;

    bool sys_is_loaded;
    bool std_is_loaded;
    bool core_is_loaded;
    bool builtin_is_loaded;

    bool main_is_found;

    Vec *builtin_usage; // Vec<LilyBuiltinFun* (&)>*
    Vec *sys_usage;     // Vec<LilySysFun* (&)>*

    // NOTE: this field is not defined in all packages except in the main one
    LilyCheckedOperatorRegister global_operator_register;
    LilyCheckedOperatorRegister local_operator_register;
} LilyPackage;

/**
 *
 * @brief Construct LilyPackage type.
 * @param root LilyPackage*?
 */
CONSTRUCTOR(LilyPackage *,
            LilyPackage,
            String *name,
            String *global_name,
            enum LilyVisibility visibility,
            char *filename,
            enum LilyPackageStatus status,
            const char *default_path,
            const char *default_package_acccess,
            LilyPackage *root);

/**
 *
 * @brief Build all packages.
 * @return LilyPackage*?
 */
LilyPackage *
build__LilyPackage(const CompileConfig *config,
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
                     enum LilyVisibility visibility,
                     enum LilyPackageStatus status,
                     const char *default_path);

/**
 *
 * @brief Look for the name of the file among all the packages and return the
 * File.
 */
const File *
get_file_from_filename__LilyPackage(const LilyPackage *self,
                                    const char *filename);

/**
 *
 * @brief Look for the name of the file among all the packages and return the
 * LilyPackage.
 */
LilyPackage *
search_package_from_filename__LilyPackage(LilyPackage *self,
                                          const char *filename);

/**
 *
 * @brief Search for the package from the name and return LilyPackage if found
 * otherwise return NULL.
 * @return LilyPackage*?
 */
LilyPackage *
search_package_from_name__LilyPackage(LilyPackage *self, String *name);

/**
 *
 * @brief Try to add a used builtin function to the register of uses of builtin
 * functions.
 * @note There is no duplication of builtin functions in the function register
 * of the system used.
 */
void
add_builtin_fun_to_builtin_usage__LilyPackage(LilyPackage *self,
                                              LilyBuiltinFun *fun_builtin);

/**
 *
 * @brief Try to add a used sys function to the register of uses of system
 * functions.
 * @note There is no duplication of system functions in the function register of
 * the system used.
 */
void
add_sys_fun_to_sys_usage__LilyPackage(LilyPackage *self, LilySysFun *fun_sys);

/**
 *
 * @brief Free LilyPackage type.
 */
DESTRUCTOR(LilyPackage, LilyPackage *self);

#endif // LILY_CORE_LILY_PACKAGE_PACKAGE_H
