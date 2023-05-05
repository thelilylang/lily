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

#include <core/lily/checked/decl/fun.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyCheckedDeclFunParam type (LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                          default,
                          LilyCheckedDeclFunParam *self);

// Free LilyCheckedDeclFunParam type (LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                          normal,
                          LilyCheckedDeclFunParam *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParamKind,
               enum LilyCheckedDeclFunParamKind self)
{
    switch (self) {
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
            return "LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT";
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    default,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location,
                    LilyCheckedExpr *default_)
{
    LilyCheckedDeclFunParam *self =
      lily_malloc(sizeof(LilyCheckedDeclFunParam));

    self->name = name;
    self->data_type = data_type;
    self->kind = LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDeclFunParam *,
                    LilyCheckedDeclFunParam,
                    normal,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location)
{
    LilyCheckedDeclFunParam *self =
      lily_malloc(sizeof(LilyCheckedDeclFunParam));

    self->name = name;
    self->data_type = data_type;
    self->kind = LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclFunParam,
               const LilyCheckedDeclFunParam *self)
{
    String *res = format__String(
      "LilyCheckedDeclFunParam{{ name = {S}, kind = {s}, location = "
      "{sa}, data_type =",
      self->name,
      to_string__Debug__LilyCheckedDeclFunParamKind(self->kind),
      to_string__Debug__Location(&self->location));

    if (self->data_type) {
        String *s = to_string__Debug__LilyCheckedDataType(self->data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, " NULL");
    }

    switch (self->kind) {
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT: {
            String *s = to_string__Debug__LilyCheckedExpr(self->default_);

            push_str__String(res, ", expr = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        default:
            break;
    }

    push_str__String(res, " }");

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                   default,
                   LilyCheckedDeclFunParam *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    FREE(LilyCheckedExpr, self->default_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDeclFunParam,
                   normal,
                   LilyCheckedDeclFunParam *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    lily_free(self);
}

DESTRUCTOR(LilyCheckedDeclFunParam, LilyCheckedDeclFunParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedDeclFunParam, default, self);
            break;
        case LILY_CHECKED_DECL_FUN_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedDeclFunParam, normal, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclFun, const LilyCheckedDeclFun *self)
{
    String *res = format__String(
      "LilyCheckedDeclFun{{ name = {S}, generic_params = ", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, LilyCheckedGenericParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedDeclFunParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", return_data_type = ");

    if (self->return_data_type) {
        String *s =
          to_string__Debug__LilyCheckedDataType(self->return_data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyCheckedBodyFunItem);

    {
        char *s = format(", visibility = {s}, is_async = {b}, is_operator = "
                         "{b}, can_raise = {b}, can_inline = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_async,
                         self->is_operator,
                         self->can_raise,
                         self->can_inline);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDeclFun, const LilyCheckedDeclFun *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(self->generic_params->buffer,
                          self->generic_params->len,
                          LilyCheckedGenericParam);
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedDeclFunParam);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(LilyCheckedDataType, self->return_data_type);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyCheckedBodyFunItem);
    FREE(Vec, self->body);

    FREE(LilyCheckedScope, self->scope);
    FREE(LilyCheckedAccessFun, self->access);
}
