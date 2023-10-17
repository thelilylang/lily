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

#ifndef LILY_CORE_LILY_INTERPRETER_PACKAGE_H
#define LILY_CORE_LILY_INTERPRETER_PACKAGE_H

#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/analysis/analysis.h>
#include <core/lily/analysis/checked/operator.h>
#include <core/lily/analysis/checked/operator_register.h>
#include <core/lily/functions/builtin.h>
#include <core/lily/functions/sys.h>
#include <core/lily/interpreter/vm/vm.h>
#include <core/lily/mir/mir.h>
#include <core/lily/package/program.h>
#include <core/lily/parser/parser.h>
#include <core/lily/precompiler/precompiler.h>
#include <core/lily/preparser/preparser.h>
#include <core/lily/scanner/scanner.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyPackage LilyPackage;

typedef struct LilyInterpreterAdapter
{
    LilyInterpreterVM vm;
} LilyInterpreterAdapter;

/**
 *
 * @brief Construct LilyInterpreterAdapter type.
 */
inline CONSTRUCTOR(LilyInterpreterAdapter,
                   LilyInterpreterAdapter,
                   Usize heap_capacity,
                   Usize statck_capacity,
                   const LilyMirModule *module,
                   LilyInterpreterVMResources resources,
                   bool check_overflow)
{
    return (LilyInterpreterAdapter){ .vm = NEW(LilyInterpreterVM,
                                               heap_capacity,
                                               statck_capacity,
                                               module,
                                               resources,
                                               check_overflow) };
}

/**
 *
 * @brief Free LilyInterpreterAdapter type.
 */
inline DESTRUCTOR(LilyInterpreterAdapter, const LilyInterpreterAdapter *self)
{
    //    FREE(LilyInterpreterVM, &self->vm);
}

// LilyPackageAdapter(LilyInterpreterAdapter);

#endif // LILY_CORE_LILY_INTERPRETER_PACKAGE_H
