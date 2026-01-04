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
#include <base/assert.h>
#include <base/format.h>
#include <base/new.h>

#include <core/lily/parser/ast/data_type.h>
#include <core/lily/parser/ast/expr.h>
#include <core/lily/parser/ast/expr/access.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstExprAccessHook type.
static DESTRUCTOR(LilyAstExprAccessHook, const LilyAstExprAccessHook *self);

// Free LilyAstExprAccessAt type.
static DESTRUCTOR(LilyAstExprAccessAt, const LilyAstExprAccessAt *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_AT).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 at,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          global_path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_HOOK).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 hook,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          object_path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          property_init,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_SELF_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          Self_path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_self_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          self_path,
                          const LilyAstExprAccess *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessHook,
               const LilyAstExprAccessHook *self)
{
    return format("LilyAstExprAccessHook{{ access = {Sr}, expr = {Sr} }",
                  to_string__Debug__LilyAstExpr(self->access),
                  to_string__Debug__LilyAstExpr(self->expr));
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstExprAccessAt, const LilyAstExprAccessAt *self)
{
    String *res = from__String("LilyAstExprAccessAt{ access = ");

    if (self->access) {
        String *s = to_string__Debug__LilyAstExpr(self->access);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", objects =");
    DEBUG_VEC_STRING(self->objects, res, LilyAstDataType);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprAccessHook, const LilyAstExprAccessHook *self)
{
    FREE(LilyAstExpr, self->access);
    FREE(LilyAstExpr, self->expr);
}

DESTRUCTOR(LilyAstExprAccessAt, const LilyAstExprAccessAt *self)
{
    if (self->access) {
        FREE(LilyAstExpr, self->access);
    }

    FREE_BUFFER_ITEMS(
      self->objects->buffer, self->objects->len, LilyAstDataType);
    FREE(Vec, self->objects);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessKind,
               enum LilyAstExprAccessKind self)
{
    switch (self) {
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_HOOK:
            return "LILY_AST_EXPR_ACCESS_KIND_HOOK";
        case LILY_AST_EXPR_ACCESS_KIND_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return "LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT";
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_SELF_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_self_PATH";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstExprAccess, const LilyAstExprAccess *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, global_path =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->global_path, res, LilyAstExpr);

            push_str__String(res, " }");

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_HOOK:
            return format__String(
              "LilyAstExprAccess{{ kind = {s}, hook = {sa} }",
              to_string__Debug__LilyAstExprAccessKind(self->kind),
              to_string__Debug__LilyAstExprAccessHook(&self->hook));
        case LILY_AST_EXPR_ACCESS_KIND_AT: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, object =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->at.objects, res, LilyAstDataType);

            push_str__String(res, " }");

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_PATH: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, path =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->path, res, LilyAstExpr);

            push_str__String(res, " }");

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, property_init =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->property_init, res, LilyAstExpr);

            push_str__String(res, " }");

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, Self_path =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->Self_path, res, LilyAstExpr);

            push_str__String(res, " }");

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_self_PATH: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, self_path =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->self_path, res, LilyAstExpr);

            push_str__String(res, " }");

            return res;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

String *
to_string__LilyAstExprAccess(const LilyAstExprAccess *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH: {
            String *res = from__String("global.");

            for (Usize i = 0; i < self->global_path->len; i++) {
                String *s =
                  to_string__LilyAstExprAccess(get__Vec(self->global_path, i));

                APPEND_AND_FREE(res, s);

                if (i != self->global_path->len - 1) {
                    push__String(res, '.');
                }
            }

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_HOOK:
            return format__String("{Sr}[{Sr}]",
                                  to_string__LilyAstExpr(self->hook.access),
                                  to_string__LilyAstExpr(self->hook.expr));
        case LILY_AST_EXPR_ACCESS_KIND_AT: {
            String *res = to_string__LilyAstExpr(self->at.access);

            for (Usize i = 0; i < self->at.objects->len; i++) {
                String *s =
                  to_string__LilyAstDataType(get__Vec(self->at.objects, i));

                push__String(res, '@');
                APPEND_AND_FREE(res, s);
            }

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_PATH: {
            String *res = NEW(String);

            for (Usize i = 0; i < self->path->len; i++) {
                String *s =
                  to_string__LilyAstExprAccess(get__Vec(self->path, i));

                APPEND_AND_FREE(res, s);

                if (i != self->path->len - 1) {
                    push__String(res, '.');
                }
            }

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT: {
            String *res = from__String("@.");

            for (Usize i = 0; i < self->property_init->len; i++) {
                String *s = to_string__LilyAstExprAccess(
                  get__Vec(self->property_init, i));

                APPEND_AND_FREE(res, s);

                if (i != self->property_init->len - 1) {
                    push__String(res, '.');
                }
            }

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH: {
            String *res = from__String("Self.");

            for (Usize i = 0; i < self->Self_path->len; i++) {
                String *s =
                  to_string__LilyAstExprAccess(get__Vec(self->Self_path, i));

                APPEND_AND_FREE(res, s);

                if (i != self->Self_path->len - 1) {
                    push__String(res, '.');
                }
            }

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_self_PATH: {
            String *res = from__String("self.");

            for (Usize i = 0; i < self->self_path->len; i++) {
                String *s =
                  to_string__LilyAstExprAccess(get__Vec(self->self_path, i));

                APPEND_AND_FREE(res, s);

                if (i != self->self_path->len - 1) {
                    push__String(res, '.');
                }
            }

            return res;
        }
        default:
            UNREACHABLE("unknown variant");
    }
}

Vec *
get_path__LilyAstExprAccess(const LilyAstExprAccess *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
            return self->global_path;
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH:
            return self->object_path;
        case LILY_AST_EXPR_ACCESS_KIND_PATH:
            return self->path;
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
            return self->Self_path;
        case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
            return self->self_path;
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return self->property_init;
        default:
            UNREACHABLE("cannot get a path");
    }
}

int
add_item_to_path__LilyAstExprAccess(LilyAstExpr **self, LilyAstExpr **item)
{
    LilyAstExpr *self_ref = *self;
    LilyAstExpr *item_ref = *item;

    ASSERT(self_ref->kind == LILY_AST_EXPR_KIND_ACCESS);
    ASSERT(item_ref);

    Vec *path = get_path__LilyAstExprAccess(&self_ref->access);

    switch (item_ref->kind) {
        case LILY_AST_EXPR_KIND_ACCESS:
            switch (item_ref->access.kind) {
                case LILY_AST_EXPR_ACCESS_KIND_HOOK:
                    END_LOCATION(&self_ref->location,
                                 item_ref->access.hook.access->location);
                    push__Vec(path, item_ref->access.hook.access);

                    item_ref->access.hook.access = self_ref;
                    self_ref = item_ref;

                    break;
                default:
                    UNREACHABLE("the parser has a bug!!");
            }

            break;
        case LILY_AST_EXPR_KIND_CALL:
            switch (item_ref->call.kind) {
                case LILY_AST_EXPR_CALL_KIND_FUN:
                    END_LOCATION(&self_ref->location,
                                 item_ref->call.fun.id->location);
                    push__Vec(path, item_ref->call.fun.id);

                    item_ref->call.fun.id = self_ref;
                    self_ref = item_ref;

                    break;
                case LILY_AST_EXPR_CALL_KIND_RECORD:
                    END_LOCATION(&self_ref->location,
                                 item_ref->call.record.id->location);
                    push__Vec(path, item_ref->call.record.id);

                    item_ref->call.record.id = self_ref;
                    self_ref = item_ref;

                    break;
                case LILY_AST_EXPR_CALL_KIND_VARIANT:
                    END_LOCATION(&self_ref->location,
                                 item_ref->call.variant.id->location);
                    push__Vec(path, item_ref->call.variant.id);

                    item_ref->call.variant.id = self_ref;
                    self_ref = item_ref;

                    break;
                default:
                    return 1;
            }

            break;
        default:
            push__Vec(path, item_ref);
    }

    *self = self_ref;

    return 0;
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, at, const LilyAstExprAccess *self)
{
    FREE(LilyAstExprAccessAt, &self->at);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess,
                   global_path,
                   const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(self->path->buffer, self->path->len, LilyAstExpr);
    FREE(Vec, self->path);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, hook, const LilyAstExprAccess *self)
{
    FREE(LilyAstExprAccessHook, &self->hook);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess,
                   object_path,
                   const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(
      self->object_path->buffer, self->object_path->len, LilyAstExpr);
    FREE(Vec, self->object_path);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, path, const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(self->path->buffer, self->path->len, LilyAstExpr);
    FREE(Vec, self->path);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess,
                   property_init,
                   const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(
      self->property_init->buffer, self->property_init->len, LilyAstExpr);
    FREE(Vec, self->property_init);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, Self_path, const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(
      self->Self_path->buffer, self->Self_path->len, LilyAstExpr);
    FREE(Vec, self->Self_path);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, self_path, const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(
      self->self_path->buffer, self->self_path->len, LilyAstExpr);
    FREE(Vec, self->self_path);
}

DESTRUCTOR(LilyAstExprAccess, const LilyAstExprAccess *self)
{
    switch (self->kind) {
        case LILY_AST_EXPR_ACCESS_KIND_AT:
            FREE_VARIANT(LilyAstExprAccess, at, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
            FREE_VARIANT(LilyAstExprAccess, global_path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_HOOK:
            FREE_VARIANT(LilyAstExprAccess, hook, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT_PATH:
            FREE_VARIANT(LilyAstExprAccess, object_path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_PATH:
            FREE_VARIANT(LilyAstExprAccess, path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            FREE_VARIANT(LilyAstExprAccess, property_init, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
            FREE_VARIANT(LilyAstExprAccess, Self_path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
            FREE_VARIANT(LilyAstExprAccess, self_path, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
