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
#include <base/new.h>

#include <core/lily/ast/data_type.h>
#include <core/lily/ast/expr.h>
#include <core/lily/ast/expr/access.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstAccessHook type.
static DESTRUCTOR(LilyAstAccessHook, const LilyAstAccessHook *self);

// Free LilyAstAccess type (LILY_AST_ACCESS_KIND_GLOBAL).
static VARIANT_DESTRUCTOR(LilyAstAccess, global, LilyAstAccess *self);

// Free LilyAstAccess type (LILY_AST_ACCESS_KIND_HOOK).
static VARIANT_DESTRUCTOR(LilyAstAccess, hook, LilyAstAccess *self);

// Free LilyAstAccess type (LILY_AST_ACCESS_KIND_OBJECT).
static VARIANT_DESTRUCTOR(LilyAstAccess, object, LilyAstAccess *self);

// Free LilyAstAccess type (LILY_AST_ACCESS_KIND_PATH).
static VARIANT_DESTRUCTOR(LilyAstAccess, path, LilyAstAccess *self);

// Free LilyAstAccess type (LILY_AST_ACCESS_KIND_PROPERTY_INIT).
static VARIANT_DESTRUCTOR(LilyAstAccess, property_init, LilyAstAccess *self);

// Free LilyAstAccess type (LILY_AST_ACCESS_KIND_SELF).
static VARIANT_DESTRUCTOR(LilyAstAccess, self, LilyAstAccess *self);

DESTRUCTOR(LilyAstAccessHook, const LilyAstAccessHook *self)
{
    FREE(LilyAstExpr, self->access);
    FREE(LilyAstExpr, self->id);
}

VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, global, LilyAstExpr *global)
{
    LilyAstAccess *self = lily_malloc(sizeof(LilyAstExpr));

    self->kind = LILY_AST_ACCESS_KIND_GLOBAL;
    self->global = global;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstAccess *,
                    LilyAstAccess,
                    hook,
                    LilyAstAccessHook hook)
{
    LilyAstAccess *self = lily_malloc(sizeof(LilyAstAccess));

    self->kind = LILY_AST_ACCESS_KIND_HOOK;
    self->hook = hook;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, object, Vec *object)
{
    LilyAstAccess *self = lily_malloc(sizeof(LilyAstAccess));

    self->kind = LILY_AST_ACCESS_KIND_OBJECT;
    self->object = object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, path, Vec *path)
{
    LilyAstAccess *self = lily_malloc(sizeof(LilyAstAccess));

    self->kind = LILY_AST_ACCESS_KIND_PATH;
    self->path = path;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstAccess *,
                    LilyAstAccess,
                    property_init,
                    LilyAstExpr *property_init)
{
    LilyAstAccess *self = lily_malloc(sizeof(LilyAstAccess));

    self->kind = LILY_AST_ACCESS_KIND_PROPERTY_INIT;
    self->property_init = property_init;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstAccess *, LilyAstAccess, self, LilyAstExpr *self_)
{
    LilyAstAccess *self = lily_malloc(sizeof(LilyAstAccess));

    self->kind = LILY_AST_ACCESS_KIND_SELF;
    self->self = self_;

    return self;
}

VARIANT_DESTRUCTOR(LilyAstAccess, global, LilyAstAccess *self)
{
    FREE(LilyAstExpr, self->global);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstAccess, hook, LilyAstAccess *self)
{
    FREE(LilyAstAccessHook, &self->hook);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstAccess, object, LilyAstAccess *self)
{
    FREE_BUFFER_ITEMS(self->object->buffer, self->object->len, LilyAstDataType);
    FREE(Vec, self->object);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstAccess, path, LilyAstAccess *self)
{
    FREE_BUFFER_ITEMS(self->path->buffer, self->path->len, LilyAstExpr);
    FREE(Vec, self->path);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstAccess, property_init, LilyAstAccess *self)
{
    FREE(LilyAstExpr, self->property_init);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstAccess, self, LilyAstAccess *self)
{
    FREE(LilyAstExpr, self->self);
    lily_free(self);
}

DESTRUCTOR(LilyAstAccess, LilyAstAccess *self)
{
    switch (self->kind) {
        case LILY_AST_ACCESS_KIND_GLOBAL:
            FREE_VARIANT(LilyAstAccess, global, self);
            break;
        case LILY_AST_ACCESS_KIND_HOOK:
            FREE_VARIANT(LilyAstAccess, hook, self);
            break;
        case LILY_AST_ACCESS_KIND_OBJECT:
            FREE_VARIANT(LilyAstAccess, object, self);
            break;
        case LILY_AST_ACCESS_KIND_PATH:
            FREE_VARIANT(LilyAstAccess, path, self);
            break;
        case LILY_AST_ACCESS_KIND_PROPERTY_INIT:
            FREE_VARIANT(LilyAstAccess, property_init, self);
            break;
        case LILY_AST_ACCESS_KIND_SELF:
            FREE_VARIANT(LilyAstAccess, self, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}