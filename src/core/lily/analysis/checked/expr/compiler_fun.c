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

#include <base/assert.h>

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/expr/compiler_fun.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

static inline VARIANT_DESTRUCTOR(LilyCheckedExprCompilerFun,
                                 get_field,
                                 const LilyCheckedExprCompilerFun *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCompilerFunKind,
               enum LilyCheckedExprCompilerFunKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD:
            return "LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCompilerFunGetField,
               const LilyCheckedExprCompilerFunGetField *self)
{
    return format(
      "LilyCheckedExprCompilerFunGetField{{ record = {Sr}, id = {zu} }",
      to_string__Debug__LilyCheckedExpr(self->record),
      self->id);
}
#endif

DESTRUCTOR(LilyCheckedExprCompilerFunGetField,
           const LilyCheckedExprCompilerFunGetField *self)
{
    FREE(LilyCheckedExpr, self->record);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprCompilerFun,
               const LilyCheckedExprCompilerFun *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_COMPILER_FUN_KIND_GET_FIELD:
            return format(
              "LilyCheckedExprCompilerFun{{ kind = {s}, get_field = {sa} }",
              to_string__Debug__LilyCheckedExprCompilerFunKind(self->kind),
              to_string__Debug__LilyCheckedExprCompilerFunGetField(
                &self->get_field));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

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
