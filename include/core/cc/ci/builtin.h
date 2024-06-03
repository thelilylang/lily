/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CORE_CC_CI_BUILTIN_H
#define LILY_CORE_CC_CI_BUILTIN_H

#include <base/sized_array.h>
#include <base/sized_str.h>

#include <core/cc/ci/ast.h>

#define CI_BUILTIN_FUNCTION_COUNT 1
#define CI_BUILTIN_TYPE_COUNT 1

/**
 *
 * @see for clang buitlins, check: `clang/include/clang/Basic/Builtins.td` on
 * `https://github.com/llvm/llvm-project`.
 */
typedef struct CIBuiltinFunction
{
    SizedStr name;
    CIDataType *return_data_type;
    Vec *params; // Vec<CIDataType*>*
} CIBuiltinFunction;

/**
 *
 * @brief Load builtin functions.
 */
CIBuiltinFunction *
load__CIBuiltinFunction();

/**
 *
 * @brief Free CIBuiltinFunction type.
 */
DESTRUCTOR(CIBuiltinFunction, const CIBuiltinFunction *self);

typedef struct CIBuiltinType
{
    SizedStr name;
} CIBuiltinType;

/**
 *
 * @brief Load builtin types.
 */
CIBuiltinType *
load__CIBuiltinType();

typedef struct CIBuiltin
{
    CIBuiltinFunction *functions;
    CIBuiltinType *types;
} CIBuiltin;

/**
 *
 * @brief Construct CIBuiltin type.
 */
CONSTRUCTOR(CIBuiltin, CIBuiltin);

/**
 *
 * @brief Free CIBuiltin type.
 */
DESTRUCTOR(CIBuiltin, const CIBuiltin *self);

#endif // LILY_CORE_CC_CI_BUILTIN_H
