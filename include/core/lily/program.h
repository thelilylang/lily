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

#ifndef LILY_CORE_LILY_PROGRAM_H
#define LILY_CORE_LILY_PROGRAM_H

#include <base/macros.h>
#include <base/vec.h>

#include <core/lily/builtin.h>
#include <core/lily/checked/operator.h>
#include <core/lily/sys.h>

typedef struct LilyPackage LilyPackage;

// This types is used to load the basic ressources of the program
typedef struct LilyProgramRessources
{
    LilyBuiltinFun *builtins;
    LilySysFun *syss;
    LilyCheckedOperator *default_operators;
} LilyProgramRessources;

/**
 *
 * @brief Construct LilyProgram type.
 */
inline CONSTRUCTOR(LilyProgramRessources, LilyProgramRessources)
{
    return (LilyProgramRessources){
        .builtins = load_builtins__LilyBuiltin(),
        .syss = load_syss__LilySys(),
        .default_operators = load_default_operators__LilyCheckedOperator()
    };
}

/**
 *
 * @brief Free LilyProgram type.
 */
DESTRUCTOR(LilyProgramRessources, const LilyProgramRessources *self);

enum LilyProgramKind
{
    LILY_PROGRAM_KIND_EXE,
    LILY_PROGRAM_KIND_STATIC_LIB,
    LILY_PROGRAM_KIND_DYNAMIC_LIB
};

typedef struct LilyProgram
{
    enum LilyProgramKind kind;
    LilyProgramRessources ressources;
} LilyProgram;

/**
 *
 * @brief Construct LilyProgram type.
 */
inline CONSTRUCTOR(LilyProgram, LilyProgram, enum LilyProgramKind kind)
{
    return (LilyProgram){ .kind = kind,
                          .ressources = NEW(LilyProgramRessources) };
}

/**
 *
 * @brief Free LilyProgram type.
 */
inline DESTRUCTOR(LilyProgram, const LilyProgram *self)
{
    FREE(LilyProgramRessources, &self->ressources);
}

#endif // LILY_CORE_LILY_PROGRAM_H
