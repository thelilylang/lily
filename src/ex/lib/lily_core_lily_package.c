/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_EX_LIB_LILY_CORE_LILY_PACKAGE_C
#define LILY_EX_LIB_LILY_CORE_LILY_PACKAGE_C

#include <core/lily/package/compiler/config.h>
#include <core/lily/package/interpreter/config.h>
#include <core/lily/package/library.h>
#include <core/lily/package/program.h>

#include "lily_core_lily_compiler_package.c"
#include "lily_core_lily_interpreter_package.c"
#include "lily_core_lily_mir.c"
#include "lily_core_lily_parser.c"
#include "lily_core_lily_precompiler.c"

// <core/lily/package/library.h>
extern inline void
finish_set__LilyLibrary(LilyLibrary *self, String *name, enum LilyArKind ar);

// <core/lily/package/program.h>
extern inline CONSTRUCTOR(LilyProgramResources, LilyProgramResources);

extern inline DESTRUCTOR(LilyProgram, const LilyProgram *self);

// <core/lily/package/compiler/config.h>
extern inline LilyPackageCompilerConfig
default__LilyPackageCompilerConfig();

extern inline LilyPackageCompilerConfig
default__LilyPackageCompilerConfig();

extern inline LilyPackageCompilerConfig
from_CompileConfig__LilyPackageCompilerConfig(const LilycConfig *lilyc_config);

// <core/lily/package/interpreter/config.h>
extern inline CONSTRUCTOR(LilyPackageInterpreterConfig,
                          LilyPackageInterpreterConfig,
                          Vec *args,
                          bool verbose,
                          Usize max_heap,
                          Usize max_stack);

extern inline LilyPackageInterpreterConfig
default__LilyPackageInterpreterConfig();

#endif // LILY_EX_LIB_LILY_CORE_LILY_PACKAGE_C