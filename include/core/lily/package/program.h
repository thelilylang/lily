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

#ifndef LILY_CORE_LILY_PACKAGE_PROGRAM_H
#define LILY_CORE_LILY_PACKAGE_PROGRAM_H

#include <base/macros.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/operator.h>
#include <core/lily/functions/builtin.h>
#include <core/lily/functions/sys.h>

typedef struct LilyPackage LilyPackage;

// This types is used to load the basic resources of the program.
// e.g. default operator, builtins, syss, analysed library, ...
typedef struct LilyProgramResources
{
    LilyBuiltinFun *builtins;
    LilySysFun *syss;
    LilyCheckedOperator **default_operators;
    Vec *libs; // Vec<LilyLibrary*>*
} LilyProgramResources;

/**
 *
 * @brief Construct LilyProgram type.
 */
inline CONSTRUCTOR(LilyProgramResources, LilyProgramResources)
{
    return (LilyProgramResources){
        .builtins = load_builtins__LilyBuiltin(),
        .syss = load_syss__LilySys(),
        .default_operators = load_default_operators__LilyCheckedOperator(),
        .libs = NEW(Vec)
    };
}

/**
 *
 * @brief Free LilyProgram type.
 */
DESTRUCTOR(LilyProgramResources, const LilyProgramResources *self);

enum LilyProgramKind
{
    LILY_PROGRAM_KIND_EXE,
    LILY_PROGRAM_KIND_STATIC_LIB,
    LILY_PROGRAM_KIND_DYNAMIC_LIB
};

typedef struct LilyProgram
{
    enum LilyProgramKind kind;
    LilyProgramResources resources;
} LilyProgram;

/**
 *
 * @brief Construct LilyProgram type.
 */
CONSTRUCTOR(LilyProgram, LilyProgram, enum LilyProgramKind kind);

/**
 *
 * @brief Free LilyProgram type.
 */
inline DESTRUCTOR(LilyProgram, const LilyProgram *self)
{
    FREE(LilyProgramResources, &self->resources);
}

#endif // LILY_CORE_LILY_PACKAGE_PROGRAM_H
