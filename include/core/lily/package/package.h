/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#ifndef LILY_CORE_LILY_PACKAGE_H
#define LILY_CORE_LILY_PACKAGE_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/compiler/package/package.h>
#include <core/lily/interpreter/package/package.h>
#include <core/lily/package/program.h>
#include <core/lily/precompiler/precompiler.h>

#define LOG_VERBOSE(package, msg)                                          \
    switch (package->kind) {                                               \
        case LILY_PACKAGE_KIND_COMPILER:                                   \
            if (package->compiler.config->verbose) {                       \
                printf("+ %s package %s\n",                                \
                       package->global_name ? package->global_name->buffer \
                                            : "(not defined)",             \
                       msg);                                               \
            }                                                              \
            break;                                                         \
        case LILY_PACKAGE_KIND_INTERPRETER:                                \
            if (package->interpreter.config->verbose) {                    \
                printf("+ %s package %s\n",                                \
                       package->global_name ? package->global_name->buffer \
                                            : "(not defined)",             \
                       msg);                                               \
            }                                                              \
            break;                                                         \
        case LILY_PACKAGE_KIND_JIT:                                        \
            TODO("verbose: JIT");                                          \
        default:                                                           \
            UNREACHABLE("unknown variant");                                \
    }

#define SET_ROOT_PACKAGE_NAME(root_package)                            \
    if (root_package->preparser_info.package->name) {                  \
        root_package->name =                                           \
          clone__String(root_package->preparser_info.package->name);   \
        root_package->global_name = clone__String(root_package->name); \
    } else {                                                           \
        root_package->name = from__String("main");                     \
        root_package->global_name = from__String("main");              \
    }

#define LOAD_ROOT_PACKAGE_RESOURCES(root_package, p)                          \
    /* Load builtins and syss */                                              \
    root_package->builtins = p->resources.builtins;                           \
    root_package->syss = p->resources.syss;                                   \
                                                                              \
    /* Load default operators in operator register                            \
    In a normal case where we wanted to add an operator to the register, we'd \
    use `add_operator__LilyCheckedOperatorRegister`, but in this case it's    \
    guaranteed that no operator repeats itself, so to increase loading speed  \
    we'll add it directly to the vector without checking. */                  \
    for (Usize i = 0; i < DEFAULT_OPERATORS_COUNT; ++i) {                     \
        push__Vec(root_package->operator_register.operators,                  \
                  ref__LilyCheckedOperator(                                   \
                    root_package->program->resources.default_operators[i]));  \
    }

enum LilyPackageStatus
{
    LILY_PACKAGE_STATUS_LIB_MAIN,
    LILY_PACKAGE_STATUS_MAIN, // Must contain the main function
    LILY_PACKAGE_STATUS_NORMAL,
    LILY_PACKAGE_STATUS_SUB_MAIN,
    LILY_PACKAGE_STATUS_IND // Independent package e.g. a single file
};

enum LilyPackageKind
{
    LILY_PACKAGE_KIND_COMPILER,
    LILY_PACKAGE_KIND_INTERPRETER,
    LILY_PACKAGE_KIND_JIT,
};

typedef struct LilyPackage
{
    String *name;              // String* | String* (&)
    String *global_name;       // String* | String* (&)
    Vec *public_macros;        // Vec<LilyMacro*>*?
    Vec *private_macros;       // Vec<LilyMacro*>*
    Vec *public_imports;       // Vec<LilyImport*>*
    Vec *private_imports;      // Vec<LilyImport*>*
    Vec *sub_packages;         // Vec<LilyPackage*>*
    Vec *package_dependencies; // Vec<LilyPackage* (&)>*
    Vec *lib_dependencies;     // Vec<LilyLibrary* (&)>*

    // count all errors and warnings after the precompiler step
    Usize count_error;
    Usize count_warning;

    File file;
    LilyScanner scanner;
    LilyPreparser preparser;
    LilyPreparserInfo preparser_info;
    LilyPrecompiler precompiler;
    LilyParser parser;
    LilyAnalysis analysis;
    LilyMirModule mir_module;

    // NOTE: builtins and syss fields are NULL when the status of the package is
    // not equal to LILY_PACKAGE_STATUS_MAIN
    LilyBuiltinFun *builtins; // LilyBuiltinFun*? (&)
    LilySysFun *syss;         // LilySysFun*? (&)

    enum LilyVisibility visibility;
    enum LilyPackageStatus status;

    bool sys_is_loaded;
    bool std_is_loaded;
    bool core_is_loaded;
    bool builtin_is_loaded;

    bool main_is_found;

    // NOTE: The real value are only defined in the main package. In the other
    // package the value is false by default.
    bool is_lib;
    bool is_exe;
    bool is_dynamic_lib;
    bool is_static_lib;

    Vec *builtin_usage; // Vec<LilyBuiltinFun* (&)>*
    Vec *sys_usage;     // Vec<LilySysFun* (&)>*

    LilyCheckedOperatorRegister operator_register;

    const LilyProgram *program; // const LilyProgram* (&)

    enum LilyPackageKind kind;
    union
    {
        LilyCompilerAdapter compiler;
        LilyInterpreterAdapter interpreter;
        //        LilyJitAdapter jit;
    };
} LilyPackage;

/**
 *
 * @brief Construct LilyPackage type (LILY_PACKAGE_KIND_COMPILER).
 */
VARIANT_CONSTRUCTOR(LilyPackage *,
                    LilyPackage,
                    compiler,
                    String *name,
                    String *global_name,
                    enum LilyVisibility visibility,
                    char *filename,
                    enum LilyPackageStatus status,
                    const char *default_path,
                    const char *default_package_access,
                    LilyPackage *root);

/**
 *
 * @brief Construct LilyPackage type (LILY_PACKAGE_KIND_INTERPRETER).
 */
VARIANT_CONSTRUCTOR(LilyPackage *,
                    LilyPackage,
                    interpreter,
                    String *name,
                    String *global_name,
                    enum LilyVisibility visibility,
                    char *filename,
                    enum LilyPackageStatus status,
                    const char *default_path,
                    const char *default_package_access,
                    LilyPackage *root);

/**
 *
 * @brief Construct LilyPackage type (LILY_PACKAGE_KIND_JIT).
 */
// VARIANT_CONSTRUCTOR(LilyPackage *, LilyPackage, jit,
//                     String *name,
//                     String *global_name,
//                     enum LilyVisibility visibility,
//                     char *filename,
//                     enum LilyPackageStatus status,
//                     const char *default_path,
//                     const char *default_package_access,
//                     LilyPackage *root);

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

#endif // LILY_CORE_LILY_PACKAGE_H
