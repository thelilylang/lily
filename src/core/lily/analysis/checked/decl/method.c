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

#include <base/alloc.h>

#include <core/lily/analysis/checked/decl/method.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyCheckedDeclMethodParam type
// (LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT).
static VARIANT_DESTRUCTOR(LilyCheckedDeclMethodParam,
                          default,
                          LilyCheckedDeclMethodParam *self);

// Free LilyCheckedDeclFunParam type
// (LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL).
static VARIANT_DESTRUCTOR(LilyCheckedDeclMethodParam,
                          normal,
                          LilyCheckedDeclMethodParam *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclMethodParamKind,
               enum LilyCheckedDeclMethodParamKind self)
{
    switch (self) {
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT:
            return "LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT";
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL:
            return "LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL";
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_MUT_SELF:
            return "LILY_CHECKED_DECL_METHOD_PARAM_KIND_MUT_SELF";
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_MUT_SELF:
            return "LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_MUT_SELF";
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_SELF:
            return "LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_SELF";
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_SELF:
            return "LILY_CHECKED_DECL_METHOD_PARAM_KIND_SELF";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedDeclMethodParam *,
                    LilyCheckedDeclMethodParam,
                    default,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location,
                    LilyCheckedExpr *default_)
{
    LilyCheckedDeclMethodParam *self =
      lily_malloc(sizeof(LilyCheckedDeclMethodParam));

    self->name = name;
    self->data_type = data_type;
    self->kind = LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT;
    self->location = location;
    self->default_ = default_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDeclMethodParam *,
                    LilyCheckedDeclMethodParam,
                    normal,
                    String *name,
                    LilyCheckedDataType *data_type,
                    Location location)
{
    LilyCheckedDeclMethodParam *self =
      lily_malloc(sizeof(LilyCheckedDeclMethodParam));

    self->name = name;
    self->data_type = data_type;
    self->kind = LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL;
    self->location = location;

    return self;
}

CONSTRUCTOR(LilyCheckedDeclMethodParam *,
            LilyCheckedDeclMethodParam,
            enum LilyCheckedDeclMethodParamKind kind,
            Location location)
{
    LilyCheckedDeclMethodParam *self =
      lily_malloc(sizeof(LilyCheckedDeclMethodParam));

    self->name = NULL;
    self->data_type = NULL;
    self->kind = kind;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclMethodParam,
               const LilyCheckedDeclMethodParam *self)
{
    String *res = from__String("LilyCheckedDeclMethodParam{{ name = ");

    switch (self->kind) {
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_SELF:
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_MUT_SELF:
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_SELF:
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_MUT_SELF:
            push_str__String(res, "NULL");
            break;
        default:
            append__String(res, self->name);
            break;
    }

    push_str__String(res, ", data_type =");

    if (self->data_type) {
        String *s = to_string__Debug__LilyCheckedDataType(self->data_type);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s =
          format(", kind = {s}, location = {sa}",
                 to_string__Debug__LilyCheckedDeclMethodParamKind(self->kind),
                 to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    switch (self->kind) {
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT: {
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

VARIANT_DESTRUCTOR(LilyCheckedDeclMethodParam,
                   default,
                   LilyCheckedDeclMethodParam *self)
{
    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    FREE(LilyCheckedExpr, self->default_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDeclMethodParam,
                   normal,
                   LilyCheckedDeclMethodParam *self)
{
    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    lily_free(self);
}

DESTRUCTOR(LilyCheckedDeclMethodParam, LilyCheckedDeclMethodParam *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_DEFAULT:
            FREE_VARIANT(LilyCheckedDeclMethodParam, default, self);
            break;
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_NORMAL:
            FREE_VARIANT(LilyCheckedDeclMethodParam, normal, self);
            break;
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_MUT_SELF:
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_MUT_SELF:
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_SELF:
        case LILY_CHECKED_DECL_METHOD_PARAM_KIND_REF_SELF:
            lily_free(self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclMethod,
               const LilyCheckedDeclMethod *self)
{
    String *res = format__String(
      "LilyCheckedDeclMethod{{ name = {S}, global_name = {S}, object_impl = ",
      self->name,
      self->global_name);

    if (self->object_impl) {
        append__String(res, self->object_impl);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", generic_params =");

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, LilyCheckedGenericParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", params =");

    if (self->params) {
        DEBUG_VEC_STRING(self->params, res, LilyCheckedDeclMethodParam);
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

    push_str__String(res, ", req =");

    {
        char *s = format(", visibility = {s}, is_async = {b}, is_operator = "
                         "{b}, is_virtual = {b}, is_override = {b}, can_raise "
                         "= {b}, can_inline = {b}, is_checked = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_async,
                         self->is_operator,
                         self->is_virtual,
                         self->is_override,
                         self->can_raise,
                         self->can_inline,
                         self->is_checked);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDeclMethod, const LilyCheckedDeclMethod *self)
{
    FREE(String, self->global_name);

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(self->generic_params->buffer,
                          self->generic_params->len,
                          LilyCheckedGenericParam);
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyCheckedDeclMethodParam);
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
