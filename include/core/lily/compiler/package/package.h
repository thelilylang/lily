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

#ifndef LILY_CORE_LILY_COMPILER_PACKAGE_PACKAGE_H
#define LILY_CORE_LILY_COMPILER_PACKAGE_PACKAGE_H

#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <cli/lilyc/config.h>

#include <core/lily/analysis/analysis.h>
#include <core/lily/analysis/checked/operator.h>
#include <core/lily/analysis/checked/operator_register.h>
#include <core/lily/compiler/ir.h>
#include <core/lily/compiler/linker/linker.h>
#include <core/lily/functions/builtin.h>
#include <core/lily/functions/sys.h>
#include <core/lily/mir/mir.h>
#include <core/lily/package/compiler/config.h>
#include <core/lily/package/library.h>
#include <core/lily/package/program.h>
#include <core/lily/parser/parser.h>
#include <core/lily/precompiler/precompiler.h>
#include <core/lily/preparser/preparser.h>
#include <core/lily/scanner/scanner.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyPackage LilyPackage;
enum LilyPackageStatus;

#define COMPILER_SET_ROOT_PACKAGE_IR(config, root_package)                    \
    if (config->cc_ir) {                                                      \
        /* TODO: add a linker for CC */                                       \
        root_package->compiler.ir = NEW_VARIANT(LilyIr, cc, NEW(LilyIrCc));   \
        root_package->compiler.linker = LILY_LINKER_KIND_CC;                  \
    } else if (config->cpp_ir) {                                              \
        /* TODO: add a linker for CPP */                                      \
        root_package->compiler.ir = NEW_VARIANT(LilyIr, cpp, NEW(LilyIrCpp)); \
        root_package->compiler.linker = LILY_LINKER_KIND_CPP;                 \
    } else if (config->js_ir) {                                               \
        self->compiler.ir = NEW_VARIANT(LilyIr, js, NEW(LilyIrJs));           \
    } else {                                                                  \
        root_package->compiler.ir = NEW_VARIANT(                              \
          LilyIr, llvm, NEW(LilyIrLlvm, root_package->global_name->buffer));  \
        root_package->compiler.linker = LILY_LINKER_KIND_LLVM;                \
    }

#define COMPILER_SET_ROOT_PACKAGE_PROGRAM(root_package, p, lib)       \
    /* Set the kind of program (static lib, dynamic lib, exe, ...) */ \
    root_package->program = p;                                        \
                                                                      \
    switch (root_package->program->kind) {                            \
        case LILY_PROGRAM_KIND_EXE:                                   \
            root_package->is_exe = true;                              \
                                                                      \
            break;                                                    \
        case LILY_PROGRAM_KIND_STATIC_LIB:                            \
            ASSERT(lib);                                              \
                                                                      \
            finish_set__LilyLibrary(                                  \
              lib,                                                    \
              root_package->name,                                     \
              (enum LilyArKind)root_package->compiler.linker);        \
                                                                      \
            root_package->is_lib = true;                              \
            root_package->is_static_lib = true;                       \
            lib->package = root_package;                              \
            root_package->compiler.lib = lib;                         \
                                                                      \
            break;                                                    \
        case LILY_PROGRAM_KIND_DYNAMIC_LIB:                           \
            ASSERT(lib);                                              \
                                                                      \
            finish_set__LilyLibrary(                                  \
              lib,                                                    \
              root_package->name,                                     \
              (enum LilyArKind)root_package->compiler.linker);        \
                                                                      \
            root_package->is_lib = true;                              \
            root_package->is_dynamic_lib = true;                      \
            lib->package = root_package;                              \
            root_package->compiler.lib = lib;                         \
                                                                      \
            break;                                                    \
        default:                                                      \
            UNREACHABLE("unknown variant");                           \
    }

#define COMPILER_SET_ROOT_PACKAGE_USE_SWITCH(root_package)              \
    /* Set `use_switch` on true for CC IR, CPP IR, JS IR or LLVM IR. */ \
    if (root_package->compiler.config->cc_ir ||                         \
        root_package->compiler.config->cpp_ir ||                        \
        root_package->compiler.config->js_ir ||                         \
        root_package->compiler.config->llvm_ir) {                       \
        root_package->analysis.use_switch = true;                       \
    }

typedef struct LilyCompilerAdapter
{
    char *output_path; // char*? - Normally the output path cannot be NULL at
                       // the end of the compilation, except if the compilation
                       // is stopped before the end.
    char *output_exe_path; // char*? - The output exe path is NULL if the
                           // compilation is stopped before the end or the
                           // package status is not LILY_PACKAGE_STATUS_MAIN.
    const LilyPackageCompilerConfig
      *config; // const LilyPackageCompilerConfig* (&)
    LilyIr ir;
    enum LilyLinkerKind linker;
    LilyLibrary *lib; // LilyLibrary*? (&)
} LilyCompilerAdapter;

/**
 *
 * @brief Construct LilyCompilerAdapter type.
 */
inline CONSTRUCTOR(LilyCompilerAdapter,
                   LilyCompilerAdapter,
                   const LilyPackageCompilerConfig *config)
{
    return (LilyCompilerAdapter){
        .output_path = NULL,
        .output_exe_path = NULL,
        .config = config,
        .lib = NULL,
    };
}

/**
 *
 * @brief Free LilyCompilerAdapter type.
 */
DESTRUCTOR(LilyCompilerAdapter, const LilyCompilerAdapter *self);

/**
 *
 * @brief Build all packages.
 * @param lib LilyLibrary*?
 * @return LilyPackage*?
 */
LilyPackage *
build__LilyCompilerPackage(const LilyPackageCompilerConfig *config,
                           const char *filename,
                           enum LilyVisibility visibility,
                           enum LilyPackageStatus status,
                           const char *default_path,
                           const LilyProgram *program,
                           LilyLibrary *lib);

/**
 *
 * @brief Build all packages of the library.
 * @return LilyLibrary*?
 */
LilyLibrary *
build_lib__LilyCompilerPackage(const LilycConfig *config,
                               enum LilyVisibility visibility,
                               enum LilyPackageStatus status,
                               const char *default_path,
                               const LilyProgram *program,
                               String *version,
                               String *url,
                               String *path);

/**
 *
 * @brief Compile package.
 * @note Begin by the main package.
 * @return LilyPackage*?
 */
LilyPackage *
compile__LilyCompilerPackage(const LilycConfig *config,
                             enum LilyVisibility visibility,
                             enum LilyPackageStatus status,
                             const char *default_path,
                             const LilyProgram *program);

/**
 *
 * @brief Compile library.
 * @note Begin by the main package.
 * @return LilyLibrary*?
 */
LilyLibrary *
compile_lib__LilyCompilerPackage(const LilycConfig *config,
                                 enum LilyVisibility visibility,
                                 enum LilyPackageStatus status,
                                 const char *default_path,
                                 const LilyProgram *program);

/**
 *
 * @brief Run until the scanner.
 */
void
run_scanner__LilyCompilerPackage(const LilycConfig *config);

/**
 *
 * @brief Run until the preparser.
 */
void
run_preparser__LilyCompilerPackage(const LilycConfig *config);

/**
 *
 * @brief Run until the precompiler.
 */
void
run_precompiler__LilyCompilerPackage(const LilycConfig *config);

/**
 *
 * @brief Run until the parser.
 */
void
run_parser__LilyCompilerPackage(const LilycConfig *config);

/**
 *
 * @brief Run until the analysis.
 */
void
run_analysis__LilyCompilerPackage(const LilycConfig *config);

/**
 *
 * @brief Run until the MIR.
 */
void
run_mir__LilyCompilerPackage(const LilycConfig *config);

/**
 *
 * @brief Run until the IR.
 */
void
run_ir__LilyCompilerPackage(const LilycConfig *config);

#endif // LILY_CORE_LILY_COMPILER_PACKAGE_PACKAGE_H
