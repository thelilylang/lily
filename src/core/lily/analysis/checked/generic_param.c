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

#include <base/alloc.h>
#include <base/format.h>

#include <core/lily/analysis/checked/generic_param.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedGenericParam type
// (LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT).
static VARIANT_DESTRUCTOR(LilyCheckedGenericParam,
                          constraint,
                          LilyCheckedGenericParam *self);

// Free LilyCheckedGenericParam type (LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL).
static inline VARIANT_DESTRUCTOR(LilyCheckedGenericParam,
                                 normal,
                                 LilyCheckedGenericParam *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGenericParamKind,
               enum LilyCheckedGenericParamKind self)
{
    switch (self) {
        case LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT:
            return "LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT";
        case LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGenericParamConstraint,
               const LilyCheckedGenericParamConstraint *self)
{
    String *res = format__String(
      "LilyCheckedGenericParamConstraint{{ name = {S}, constraints =",
      self->name);

    DEBUG_VEC_STRING(self->constraints, res, LilyCheckedDataType);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedGenericParamConstraint,
           const LilyCheckedGenericParamConstraint *self)
{
    FREE_BUFFER_ITEMS(
      self->constraints->buffer, self->constraints->len, LilyCheckedDataType);
    FREE(Vec, self->constraints);
}

VARIANT_CONSTRUCTOR(LilyCheckedGenericParam *,
                    LilyCheckedGenericParam,
                    normal,
                    const Location *location,
                    String *normal)
{
    LilyCheckedGenericParam *self =
      lily_malloc(sizeof(LilyCheckedGenericParam));

    self->kind = LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL;
    self->location = location;
    self->normal = normal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedGenericParam *,
                    LilyCheckedGenericParam,
                    constraint,
                    const Location *location,
                    LilyCheckedGenericParamConstraint constraint)
{
    LilyCheckedGenericParam *self =
      lily_malloc(sizeof(LilyCheckedGenericParam));

    self->kind = LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT;
    self->location = location;
    self->constraint = constraint;

    return self;
}

String *
get_name__LilyCheckedGenericParam(const LilyCheckedGenericParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT:
            return self->constraint.name;
        case LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL:
            return self->normal;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGenericParam,
               const LilyCheckedGenericParam *self)
{
    String *res =
      format__String("LilyCheckedGenericParam{{ kind = {s}, location = {sa}",
                     to_string__Debug__LilyCheckedGenericParamKind(self->kind),
                     to_string__Debug__Location(self->location));

    switch (self->kind) {
        case LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT: {
            char *s =
              format(", constraint = {Sr} }",
                     to_string__Debug__LilyCheckedGenericParamConstraint(
                       &self->constraint));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL: {
            char *s = format(", normal = {S} }", self->normal);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedGenericParam,
                   constraint,
                   LilyCheckedGenericParam *self)
{
    FREE(LilyCheckedGenericParamConstraint, &self->constraint);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedGenericParam,
                   normal,
                   LilyCheckedGenericParam *self)
{
    lily_free(self);
}

DESTRUCTOR(LilyCheckedGenericParam, LilyCheckedGenericParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_GENERIC_PARAM_KIND_CONSTRAINT:
            FREE_VARIANT(LilyCheckedGenericParam, constraint, self);
            break;
        case LILY_CHECKED_GENERIC_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedGenericParam, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
