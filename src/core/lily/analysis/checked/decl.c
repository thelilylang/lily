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

#include <core/lily/analysis/checked/decl.h>

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
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclConstant constant)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_CONSTANT;
    self->location = location;
    self->ast_decl = ast_decl;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    error,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclError error)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_ERROR;
    self->location = location;
    self->ast_decl = ast_decl;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    fun,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclFun fun)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_FUN;
    self->location = location;
    self->ast_decl = ast_decl;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    method,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclMethod method)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_METHOD;
    self->location = location;
    self->ast_decl = ast_decl;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    module,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclModule module)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_MODULE;
    self->location = location;
    self->ast_decl = ast_decl;
    self->module = module;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    object,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclObject object)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_OBJECT;
    self->location = location;
    self->ast_decl = ast_decl;
    self->object = object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedDecl *,
                    LilyCheckedDecl,
                    type,
                    const Location *location,
                    const LilyAstDecl *ast_decl,
                    LilyCheckedDeclType type)
{
    LilyCheckedDecl *self = lily_malloc(sizeof(LilyCheckedDecl));

    self->kind = LILY_CHECKED_DECL_KIND_TYPE;
    self->location = location;
    self->ast_decl = ast_decl;
    self->type = type;

    return self;
}

String *
get_global_name__LilyCheckedDecl(const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            return self->constant.global_name;
        case LILY_CHECKED_DECL_KIND_ERROR:
            return self->error.global_name;
        case LILY_CHECKED_DECL_KIND_FUN:
            return self->fun.global_name;
        case LILY_CHECKED_DECL_KIND_METHOD:
            return self->method.global_name;
        case LILY_CHECKED_DECL_KIND_OBJECT:
            switch (self->object.kind) {
                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                    return self->object.class.global_name;
                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                    return self->object.enum_.global_name;
                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                    return self->object.record.global_name;
                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                    return self->object.trait.global_name;
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_DECL_KIND_TYPE:
            switch (self->type.kind) {
                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                    return self->type.alias.global_name;
                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                    return self->type.enum_.global_name;
                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                    return self->type.record.global_name;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

Vec *
get_signatures__LilyCheckedDecl(const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            UNREACHABLE("not expected in this context.");
        case LILY_CHECKED_DECL_KIND_ERROR:
            TODO("get signatures from error declaration");
        case LILY_CHECKED_DECL_KIND_FUN:
            return self->fun.signatures;
        case LILY_CHECKED_DECL_KIND_METHOD:
            TODO("get signatures from method declaration");
        case LILY_CHECKED_DECL_KIND_OBJECT:
            switch (self->object.kind) {
                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                    TODO("get signatures from class declaration");
                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                    TODO("get signatures from enum object declaration");
                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                    TODO("get signatures from record object declaration");
                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                    TODO("get signatures from trait declaration");
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_DECL_KIND_TYPE:
            switch (self->type.kind) {
                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                    TODO("get signatures from alias declaration");
                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                    return self->type.enum_.signatures;
                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                    return self->type.record.signatures;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

LilyCheckedScope *
get_scope__LilyCheckedDecl(const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
        case LILY_CHECKED_DECL_KIND_ERROR:
            return NULL;
        case LILY_CHECKED_DECL_KIND_FUN:
            return self->fun.scope;
        case LILY_CHECKED_DECL_KIND_METHOD:
            return self->method.scope;
        case LILY_CHECKED_DECL_KIND_MODULE:
            return self->module.scope;
        case LILY_CHECKED_DECL_KIND_OBJECT:
            switch (self->object.kind) {
                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                    return self->object.class.scope;
                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                    return self->object.enum_.scope;
                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                    return self->object.record.scope;
                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                    return self->object.trait.scope;
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_DECL_KIND_TYPE:
            switch (self->type.kind) {
                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                    return NULL;
                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                    return self->type.enum_.scope;
                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                    return self->type.record.scope;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

const Vec *
get_generic_params__LilyCheckedDecl(const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
        case LILY_CHECKED_DECL_KIND_MODULE:
            return NULL;
        case LILY_CHECKED_DECL_KIND_ERROR:
            return self->error.generic_params;
        case LILY_CHECKED_DECL_KIND_FUN:
            return self->fun.generic_params;
        case LILY_CHECKED_DECL_KIND_METHOD:
            return self->method.generic_params;
        case LILY_CHECKED_DECL_KIND_OBJECT:
            switch (self->object.kind) {
                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                    return self->object.class.generic_params;
                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                    return self->object.enum_.generic_params;
                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                    return self->object.record.generic_params;
                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                    return self->object.trait.generic_params;
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_DECL_KIND_TYPE:
            switch (self->type.kind) {
                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                    return self->type.alias.generic_params;
                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                    return self->type.enum_.generic_params;
                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                    return self->type.record.generic_params;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

int
verify_generic_params__LilyCheckedDecl(const LilyCheckedDecl *self,
                                       Vec *called_generic_params)
{
    // TODO: maybe in the future we have to check default generic param.
    const Vec *generic_params = get_generic_params__LilyCheckedDecl(self);

    if (called_generic_params && generic_params) {
        if (called_generic_params->len == generic_params->len) {
            return 0;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

bool
is_checked__LilyCheckedDecl(const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            return self->constant.is_checked;
        case LILY_CHECKED_DECL_KIND_ERROR:
            return self->error.is_checked;
        case LILY_CHECKED_DECL_KIND_FUN:
            return self->fun.is_checked;
        case LILY_CHECKED_DECL_KIND_METHOD:
            return self->method.is_checked;
        case LILY_CHECKED_DECL_KIND_MODULE:
            return self->module.is_checked;
        case LILY_CHECKED_DECL_KIND_OBJECT:
            switch (self->object.kind) {
                case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
                    return self->object.class.is_checked;
                case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
                    return self->object.enum_.is_checked;
                case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
                    return self->object.record.is_checked;
                case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
                    return self->object.trait.is_checked;
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_DECL_KIND_TYPE:
            switch (self->type.kind) {
                case LILY_CHECKED_DECL_TYPE_KIND_ALIAS:
                    return self->type.alias.is_checked;
                case LILY_CHECKED_DECL_TYPE_KIND_ENUM:
                    return self->type.enum_.is_checked;
                case LILY_CHECKED_DECL_TYPE_KIND_RECORD:
                    return self->type.record.is_checked;
                default:
                    UNREACHABLE("unknown variant");
            }
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDecl, const LilyCheckedDecl *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_KIND_CONSTANT:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "constant = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
              to_string__Debug__LilyCheckedDeclConstant(&self->constant));
        case LILY_CHECKED_DECL_KIND_ERROR:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "error = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
              to_string__Debug__LilyCheckedDeclError(&self->error));
        case LILY_CHECKED_DECL_KIND_FUN:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "fun = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
              to_string__Debug__LilyCheckedDeclFun(&self->fun));
        case LILY_CHECKED_DECL_KIND_MODULE:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "module = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
              to_string__Debug__LilyCheckedDeclModule(&self->module));
        case LILY_CHECKED_DECL_KIND_METHOD:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "method = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
              to_string__Debug__LilyCheckedDeclMethod(&self->method));
        case LILY_CHECKED_DECL_KIND_OBJECT:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "object = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
              to_string__Debug__LilyCheckedDeclObject(&self->object));
        case LILY_CHECKED_DECL_KIND_TYPE:
            return format__String(
              "LilyCheckedDecl{{ kind = {s}, location = {sa}, ast_decl = {Sr}, "
              "type = {Sr} }",
              to_string__Debug__LilyCheckedDeclKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyAstDecl(self->ast_decl),
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
