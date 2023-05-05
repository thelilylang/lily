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

#include <core/lily/checked/decl.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/print.h>
#endif

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, constant, LilyCheckedDecl *self);

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_ERROR).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, error, LilyCheckedDecl *self);

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_FUN).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, fun, LilyCheckedDecl *self);

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_MODULE).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, module, LilyCheckedDecl *self);

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, method, LilyCheckedDecl *self);

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_OBJECT).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, object, LilyCheckedDecl *self);

// Free LilyCheckedDecl type (LILY_CHECKED_DECL_KIND_TYPE).
static VARIANT_DESTRUCTOR(LilyCheckedDecl, type, LilyCheckedDecl *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclKind, enum LilyCheckedDeclKind self)
{
    switch (self) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            return "LILY_CHECKED_DECL_KIND_CONSTANT";
        case LILY_CHECKED_DECL_KIND_ERROR:
            return "LILY_CHECKED_DECL_KIND_ERROR";
        case LILY_CHECKED_DECL_KIND_FUN:
            return "LILY_CHECKED_DECL_KIND_FUN";
        case LILY_CHECKED_DECL_KIND_METHOD:
            return "LILY_CHECKED_DECL_KIND_METHOD";
        case LILY_CHECKED_DECL_KIND_MODULE:
            return "LILY_CHECKED_DECL_KIND_MODULE";
        case LILY_CHECKED_DECL_KIND_OBJECT:
            return "LILY_CHECKED_DECL_KIND_OBJECT";
        case LILY_CHECKED_DECL_KIND_TYPE:
            return "LILY_CHECKED_DECL_KIND_TYPE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    constant,
                    Location location,
                    LilyCheckedDeclConstant constant)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    error,
                    Location location,
                    LilyCheckedDeclError error)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_ERROR;
    self->location = location;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    fun,
                    Location location,
                    LilyCheckedDeclFun fun)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_FUN;
    self->location = location;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    method,
                    Location location,
                    LilyCheckedDeclMethod method)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    module,
                    Location location,
                    LilyCheckedDeclModule module)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_MODULE;
    self->location = location;
    self->module = module;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    object,
                    Location location,
                    LilyCheckedDeclObject object)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_OBJECT;
    self->location = location;
    self->object = object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    type,
                    Location location,
                    LilyCheckedDeclType type)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_TYPE;
    self->location = location;
    self->type = type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDecl, const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, constant = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclConstant(&self->constant));
        case LILY_CHECKED_DECL_KIND_ERROR:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, error = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclError(&self->error));
        case LILY_CHECKED_DECL_KIND_FUN:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, fun = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclFun(&self->fun));
        case LILY_CHECKED_DECL_KIND_MODULE:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, module = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclModule(&self->module));
        case LILY_CHECKED_DECL_KIND_METHOD:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, method = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclMethod(&self->method));
        case LILY_CHECKED_DECL_KIND_OBJECT:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, object = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclObject(&self->object));
        case LILY_CHECKED_DECL_KIND_TYPE:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, type = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclType(&self->type));
        default:
            UNREACHABLE("unknown variant");
    }
}

void
IMPL_FOR_DEBUG(debug, LilyCheckedDecl, const LilyCheckedDecl *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyCheckedDecl(self));
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedDecl, constant, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDecl, error, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclError, &self->error);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDecl, fun, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclFun, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDecl, method, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclMethod, &self->method);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDecl, module, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclModule, &self->module);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDecl, object, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclObject, &self->object);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedDecl, type, LilyCheckedDecl *self)
{
    FREE(LilyCheckedDeclType, &self->type);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedDecl, LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            FREE_VARIANT(LilyCheckedDecl, constant, self);
            break;
        case LILY_CHECKED_DECL_KIND_ERROR:
            FREE_VARIANT(LilyCheckedDecl, error, self);
            break;
        case LILY_CHECKED_DECL_KIND_FUN:
            FREE_VARIANT(LilyCheckedDecl, fun, self);
            break;
        case LILY_CHECKED_DECL_KIND_METHOD:
            FREE_VARIANT(LilyCheckedDecl, method, self);
            break;
        case LILY_CHECKED_DECL_KIND_MODULE:
            FREE_VARIANT(LilyCheckedDecl, module, self);
            break;
        case LILY_CHECKED_DECL_KIND_OBJECT:
            FREE_VARIANT(LilyCheckedDecl, object, self);
            break;
        case LILY_CHECKED_DECL_KIND_TYPE:
            FREE_VARIANT(LilyCheckedDecl, type, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
