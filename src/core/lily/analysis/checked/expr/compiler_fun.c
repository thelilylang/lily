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

#include <base/assert.h>

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/compiler_fun.h>

#include <stdio.h>
#include <stdlib.h>

static inline VARIANT_DESTRUCTOR(LilyCheckedExprCompilerFun,
                                 get_field,
                                 const LilyCheckedExprCompilerFun *self);

CONSTRUCTOR(LilyCheckedExprCompilerFunGetField,
            LilyCheckedExprCompilerFunGetField,
            LilyCheckedExpr *record,
            LilyCheckedExpr *id)
{
    ASSERT(id->kind == LILY_CHECKED_EXPR_KIND_LITERAL);

    return (LilyCheckedExprCompilerFunGetField){ .record = record, .id = id };
}

DESTRUCTOR(LilyCheckedExprCompilerFunGetField,
           const LilyCheckedExprCompilerFunGetField *self)
{
    FREE(LilyCheckedExpr, self->record);
    FREE(LilyCheckedExpr, self->id);
}

VARIANT_DESTRUCTOR(LilyCheckedExprCompilerFun,
                   get_field,
                   const LilyCheckedExprCompilerFun *self)
{
    FREE(LilyCheckedExprCompilerFunGetField, &self->get_field);
}

DESTRUCTOR(LilyCheckedExprCompilerFun, const LilyCheckedExprCompilerFun *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD:
            FREE_VARIANT(LilyCheckedExprCompilerFun, get_field, self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
