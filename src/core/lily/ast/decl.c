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

#include <core/lily/ast/decl.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/print.h>
#endif

// Free LilyAstDecl type (LILY_AST_DECL_KIND_ALIAS).
static VARIANT_DESTRUCTOR(LilyAstDecl, alias, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyAstDecl, constant, LilyAstDecl *self);

// Free LilyAstDecl type (LILY_AST_DECL_KIND_FUN).
static VARIANT_DESTRUCTOR(LilyAstDecl, fun, LilyAstDecl *self);

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
        case LILY_AST_DECL_KIND_ALIAS:
            return "LILY_AST_DECL_KIND_ALIAS";
        case LILY_AST_DECL_KIND_CONSTANT:
            return "LILY_AST_DECL_KIND_CONSTANT";
        case LILY_AST_DECL_KIND_FUN:
            return "LILY_AST_DECL_KIND_FUN";
        case LILY_AST_DECL_KIND_METHOD:
            return "LILY_AST_DECL_KIND_METHOD";
        case LILY_AST_DECL_KIND_OBJECT:
            return "LILY_AST_DECL_KIND_OBJECT";
        case LILY_AST_DECL_KIND_TYPE:
            return "LILY_AST_DECL_KIND_TYPE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstDecl *,
                    LilyAstDecl,
                    alias,
                    Location location,
                    LilyAstDeclAlias alias)
{
    LilyAstDecl *self = lily_malloc(sizeof(LilyAstDecl));

    self->kind = LILY_AST_DECL_KIND_ALIAS;
    self->location = location;
    self->alias = alias;

    return self;
}

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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDecl, const LilyAstDecl *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_KIND_ALIAS:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, alias = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclAlias(&self->alias));
        case LILY_AST_DECL_KIND_CONSTANT:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, constant = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclConstant(&self->constant));
        case LILY_AST_DECL_KIND_FUN:
            return format__String(
              "LilyAstDecl{{ kind = {s}, location = {sa}, fun = {Sr} }",
              to_string__Debug__LilyAstDeclKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclFun(&self->fun));
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

VARIANT_DESTRUCTOR(LilyAstDecl, alias, LilyAstDecl *self)
{
    FREE(LilyAstDeclAlias, &self->alias);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, constant, LilyAstDecl *self)
{
    FREE(LilyAstDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, fun, LilyAstDecl *self)
{
    FREE(LilyAstDeclFun, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstDecl, method, LilyAstDecl *self)
{
    FREE(LilyAstDeclMethod, &self->method);
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

DESTRUCTOR(LilyAstDecl, LilyAstDecl *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_KIND_ALIAS:
            FREE_VARIANT(LilyAstDecl, alias, self);
            break;
        case LILY_AST_DECL_KIND_CONSTANT:
            FREE_VARIANT(LilyAstDecl, constant, self);
            break;
        case LILY_AST_DECL_KIND_FUN:
            FREE_VARIANT(LilyAstDecl, fun, self);
            break;
        case LILY_AST_DECL_KIND_METHOD:
            FREE_VARIANT(LilyAstDecl, method, self);
            break;
        case LILY_AST_DECL_KIND_OBJECT:
            FREE_VARIANT(LilyAstDecl, object, self);
            break;
        case LILY_AST_DECL_KIND_TYPE:
            FREE_VARIANT(LilyAstDecl, type, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
