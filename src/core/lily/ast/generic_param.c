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

#include <base/alloc.h>
#include <base/format.h>

#include <core/lily/ast/generic_param.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstGenericParamKind,
               enum LilyAstGenericParamKind self)
{
    switch (self) {
        case LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT:
            return "LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT";
        case LILY_AST_GENERIC_PARAM_KIND_NORMAL:
            return "LILY_AST_GENERIC_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstGenericParamConstraint,
               const LilyAstGenericParamConstraint *self)
{
    String *res =
      format__String("LilyAstGenericParamConstraint{{ id = {Sr}, constraints =",
                     to_string__Debug__LilyAstExpr(self->id));

    DEBUG_VEC_STRING(self->constraints, res, LilyAstGenericParamConstraint);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstGenericParamConstraint,
           const LilyAstGenericParamConstraint *self)
{
    FREE(LilyAstExpr, self->id);
    FREE_BUFFER_ITEMS(
      self->constraints->buffer, self->constraints->len, LilyAstExpr);
    FREE(Vec, self->constraints);
}

VARIANT_CONSTRUCTOR(LilyAstGenericParam *,
                    LilyAstGenericParam,
                    normal,
                    LilyAstExpr *normal)
{
    LilyAstGenericParam *self = lily_malloc(sizeof(LilyAstGenericParam));

    self->kind = LILY_AST_GENERIC_PARAM_KIND_NORMAL;
    self->normal = normal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstGenericParam *,
                    LilyAstGenericParam,
                    constraint,
                    LilyAstGenericParamConstraint constraint)
{
    LilyAstGenericParam *self = lily_malloc(sizeof(LilyAstGenericParam));

    self->kind = LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT;
    self->constraint = constraint;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstGenericParam, const LilyAstGenericParam *self)
{
    String *res =
      format__String("LilyAstGenericParam{{ kind = {s}",
                     to_string__Debug__LilyAstGenericParamKind(self->kind));

    switch (self->kind) {
        case LILY_AST_GENERIC_PARAM_KIND_CONSTRAINT: {
            char *s = format(", constraint = {Sr} }",
                             to_string__Debug__LilyAstGenericParamConstraint(
                               &self->constraint));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_GENERIC_PARAM_KIND_NORMAL: {
            char *s = format(", normal = {Sr} }",
                             to_string__Debug__LilyAstExpr(self->normal));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyAstGenericParam, constraint, LilyAstGenericParam *self)
{
    FREE(LilyAstGenericParamConstraint, &self->constraint);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstGenericParam, normal, LilyAstGenericParam *self)
{
    FREE(LilyAstExpr, self->normal);
    lily_free(self);
}