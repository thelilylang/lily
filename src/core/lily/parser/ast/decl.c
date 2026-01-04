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

#include <core/lily/parser/ast/decl.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/print.h>
#endif

// Free LilyAstDecl type (LILY_AST_DECL_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyAstDecl, constant, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_ERROR).
static VARIANT_DESTRUCTOR(LilyAstDecl, error, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_FUN).
static VARIANT_DESTRUCTOR(LilyAstDecl, fun, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_INCLUDE).
static VARIANT_DESTRUCTOR(LilyAstDecl, include, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_MODULE).
static VARIANT_DESTRUCTOR(LilyAstDecl, module, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyAstDecl, method, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_OBJECT).
static VARIANT_DESTRUCTOR(LilyAstDecl, object, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_TYPE).
static VARIANT_DESTRUCTOR(LilyAstDecl, type, LilyAstDecl *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstDeclKind, enum LilyAstDeclKind self)
{
    switch (self) {
        case LILY_AST_DECL_KIND_CONSTANT:
            return "LILY_AST_DECL_KIND_CONSTANT";
        case LILY_AST_DECL_KIND_ERROR:
            return "LILY_AST_DECL_KIND_ERROR";
        case LILY_AST_DECL_KIND_FUN:
            return "LILY_AST_DECL_KIND_FUN";
        case LILY_AST_DECL_KIND_INCLUDE:
            return "LILY_AST_DECL_KIND_INCLUDE";
        case LILY_AST_DECL_KIND_METHOD:
            return "LILY_AST_DECL_KIND_METHOD";
        case LILY_AST_DECL_KIND_MODULE:
            return "LILY_AST_DECL_KIND_MODULE";
        case LILY_AST_DECL_KIND_OBJECT:
            return "LILY_AST_DECL_KIND_OBJECT";
        case LILY_AST_DECL_KIND_TYPE:
            return "LILY_AST_DECL_KIND_TYPE";
        case LILY_AST_DECL_KIND_USE:
            return "LILY_AST_DECL_KIND_USE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    constant,
                    Location location,
                    LilyAstDeclConstant constant)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    error,
                    Location location,
                    LilyAstDeclError error)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_ERROR;
    self->location = location;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    fun,
                    Location location,
                    LilyAstDeclFun fun)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_FUN;
    self->location = location;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    include,
                    Location location,
                    LilyAstDeclInclude include)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_INCLUDE;
    self->location = location;
    self->include = include;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    method,
                    Location location,
                    LilyAstDeclMethod method)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    module,
                    Location location,
                    LilyAstDeclModule module)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_MODULE;
    self->location = location;
    self->module = module;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    object,
                    Location location,
                    LilyAstDeclObject object)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_OBJECT;
    self->location = location;
    self->object = object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    type,
                    Location location,
                    LilyAstDeclType type)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_TYPE;
    self->location = location;
    self->type = type;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    use,
                    Location location,
                    LilyAstDeclUse use)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_USE;
    self->location = location;
    self->use = use;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDecl, const LilyAstDecl *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_KIND_CONSTANT:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, constant = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclConstant(&self->constant));
        case LILY_AST_DECL_KIND_ERROR:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, error = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclError(&self->error));
        case LILY_AST_DECL_KIND_FUN:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, fun = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclFun(&self->fun));
        case LILY_AST_DECL_KIND_INCLUDE:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, include = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclInclude(&self->include));
        case LILY_AST_DECL_KIND_MODULE:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, module = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclModule(&self->module));
        case LILY_AST_DECL_KIND_METHOD:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, method = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclMethod(&self->method));
        case LILY_AST_DECL_KIND_OBJECT:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, object = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclObject(&self->object));
        case LILY_AST_DECL_KIND_TYPE:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, type = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclType(&self->type));
        case LILY_AST_DECL_KIND_USE:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, use = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclUse(&self->use));
        default:
            UNREACHABLE("unknown variant");
    }
}

void
IMPL_FOR_DEBUG(debug, LilyAstDecl, const LilyAstDecl *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyAstDecl(self));
}
#endif

VARIANT_DESTRUCTOR(LilyAstDecl, constant, LilyAstDecl *self)
{
    FREE(LilyAstDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, error, LilyAstDecl *self)
{
    FREE(LilyAstDeclError, &self->error);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, fun, LilyAstDecl *self)
{
    FREE(LilyAstDeclFun, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, include, LilyAstDecl *self)
{
    FREE(LilyAstDeclInclude, &self->include);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, method, LilyAstDecl *self)
{
    FREE(LilyAstDeclMethod, &self->method);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, module, LilyAstDecl *self)
{
    FREE(LilyAstDeclModule, &self->module);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, object, LilyAstDecl *self)
{
    FREE(LilyAstDeclObject, &self->object);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, type, LilyAstDecl *self)
{
    FREE(LilyAstDeclType, &self->type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, use, LilyAstDecl *self)
{
    FREE(LilyAstDeclUse, &self->use);
    lily_free(self);
}

DESTRUCTOR(LilyAstDecl, LilyAstDecl *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_KIND_CONSTANT:
            FREE_VARIANT(LilyAstDecl, constant, self);
            break;
        case LILY_AST_DECL_KIND_ERROR:
            FREE_VARIANT(LilyAstDecl, error, self);
            break;
        case LILY_AST_DECL_KIND_FUN:
            FREE_VARIANT(LilyAstDecl, fun, self);
            break;
        case LILY_AST_DECL_KIND_INCLUDE:
            FREE_VARIANT(LilyAstDecl, include, self);
            break;
        case LILY_AST_DECL_KIND_METHOD:
            FREE_VARIANT(LilyAstDecl, method, self);
            break;
        case LILY_AST_DECL_KIND_MODULE:
            FREE_VARIANT(LilyAstDecl, module, self);
            break;
        case LILY_AST_DECL_KIND_OBJECT:
            FREE_VARIANT(LilyAstDecl, object, self);
            break;
        case LILY_AST_DECL_KIND_TYPE:
            FREE_VARIANT(LilyAstDecl, type, self);
            break;
        case LILY_AST_DECL_KIND_USE:
            FREE_VARIANT(LilyAstDecl, use, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
