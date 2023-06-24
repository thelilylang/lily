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
#include <base/new.h>

#include <core/lily/ast/data_type.h>
#include <core/lily/ast/expr.h>
#include <core/lily/ast/expr/access.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstExprAccessHook type.
static DESTRUCTOR(LilyAstExprAccessHook, const LilyAstExprAccessHook *self);

// Free LilyAstExprAccessObject type.
static DESTRUCTOR(LilyAstExprAccessObject, const LilyAstExprAccessObject *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_GLOBAL_HOOK).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 global_hook,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          global_path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_HOOK).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 hook,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_OBJECT).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 object,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 property_init,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_SELF_HOOK).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 Self_hook,
                                 const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_SELF_PATH).
static VARIANT_DESTRUCTOR(LilyAstExprAccess,
                          Self_path,
                          const LilyAstExprAccess *self);

// Free LilyAstExprAccess type (LILY_AST_EXPR_ACCESS_KIND_self_HOOK).
static inline VARIANT_DESTRUCTOR(LilyAstExprAccess,
                                 self_hook,
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
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessObject,
               const LilyAstExprAccessObject *self)
{
    String *res = from__String("LilyAstExprAccessObject{ access = ");

    if (self->access) {
        String *s = to_string__Debug__LilyAstExpr(self->access);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", object =");
    DEBUG_VEC_STRING(self->object, res, LilyAstDataType);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstExprAccessHook, const LilyAstExprAccessHook *self)
{
    FREE(LilyAstExpr, self->access);
    FREE(LilyAstExpr, self->expr);
}

DESTRUCTOR(LilyAstExprAccessObject, const LilyAstExprAccessObject *self)
{
    if (self->access) {
        FREE(LilyAstExpr, self->access);
    }

    FREE_BUFFER_ITEMS(self->object->buffer, self->object->len, LilyAstDataType);
    FREE(Vec, self->object);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstExprAccessKind,
               enum LilyAstExprAccessKind self)
{
    switch (self) {
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_HOOK:
            return "LILY_AST_EXPR_ACCESS_KIND_GLOBAL_HOOK";
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_HOOK:
            return "LILY_AST_EXPR_ACCESS_KIND_HOOK";
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT:
            return "LILY_AST_EXPR_ACCESS_KIND_OBJECT";
        case LILY_AST_EXPR_ACCESS_KIND_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return "LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT";
        case LILY_AST_EXPR_ACCESS_KIND_SELF_HOOK:
            return "LILY_AST_EXPR_ACCESS_KIND_SELF_HOOK";
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
            return "LILY_AST_EXPR_ACCESS_KIND_SELF_PATH";
        case LILY_AST_EXPR_ACCESS_KIND_self_HOOK:
            return "LILY_AST_EXPR_ACCESS_KIND_self_HOOK";
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
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_HOOK:
            return format__String(
              "LilyAstExprAccess{{ kind = {s}, global_hook = {sa} }",
              to_string__Debug__LilyAstExprAccessKind(self->kind),
              to_string__Debug__LilyAstExprAccessHook(&self->global_hook));
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
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, object =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->object.object, res, LilyAstDataType);

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
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return format__String(
              "LilyAstExprAccess{{ kind = {s}, property_init = {Sr} }",
              to_string__Debug__LilyAstExprAccessKind(self->kind),
              to_string__Debug__LilyAstExpr(self->property_init));
        case LILY_AST_EXPR_ACCESS_KIND_SELF_HOOK:
            return format__String(
              "LilyAstExprAccess{{ kind = {s}, Self_hook = {sa} }",
              to_string__Debug__LilyAstExprAccessKind(self->kind),
              to_string__Debug__LilyAstExprAccessHook(&self->Self_hook));
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH: {
            String *res = format__String(
              "LilyAstExprAccess{{ kind = {s}, Self_path =",
              to_string__Debug__LilyAstExprAccessKind(self->kind));

            DEBUG_VEC_STRING(self->Self_path, res, LilyAstExpr);

            push_str__String(res, " }");

            return res;
        }
        case LILY_AST_EXPR_ACCESS_KIND_self_HOOK:
            return format__String(
              "LilyAstExprAccess{{ kind = {s}, self_hook = {sa} }",
              to_string__Debug__LilyAstExprAccessKind(self->kind),
              to_string__Debug__LilyAstExprAccessHook(&self->self_hook));
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
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_HOOK:
            return format__String(
              "global.{Sr}[{Sr}]",
              to_string__LilyAstExpr(self->global_hook.access),
              to_string__LilyAstExpr(self->global_hook.expr));
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
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT: {
            String *res = to_string__LilyAstExpr(self->object.access);

            for (Usize i = 0; i < self->object.object->len; i++) {
                String *s =
                  to_string__LilyAstDataType(get__Vec(self->object.object, i));

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
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return format__String("@.{Sr}",
                                  to_string__LilyAstExpr(self->property_init));
        case LILY_AST_EXPR_ACCESS_KIND_SELF_HOOK:
            return format__String(
              "Self.{Sr}[{Sr}]",
              to_string__LilyAstExpr(self->Self_hook.access),
              to_string__LilyAstExpr(self->Self_hook.expr));
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
        case LILY_AST_EXPR_ACCESS_KIND_self_HOOK:
            return format__String(
              "self.{Sr}[{Sr}]",
              to_string__LilyAstExpr(self->self_hook.access),
              to_string__LilyAstExpr(self->self_hook.expr));
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

VARIANT_DESTRUCTOR(LilyAstExprAccess,
                   global_hook,
                   const LilyAstExprAccess *self)
{
    FREE(LilyAstExprAccessHook, &self->hook);
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

VARIANT_DESTRUCTOR(LilyAstExprAccess, object, const LilyAstExprAccess *self)
{
    FREE(LilyAstExprAccessObject, &self->object);
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
    FREE(LilyAstExpr, self->property_init);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, Self_hook, const LilyAstExprAccess *self)
{
    FREE(LilyAstExprAccessHook, &self->Self_hook);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, Self_path, const LilyAstExprAccess *self)
{
    FREE_BUFFER_ITEMS(
      self->Self_path->buffer, self->Self_path->len, LilyAstExpr);
    FREE(Vec, self->Self_path);
}

VARIANT_DESTRUCTOR(LilyAstExprAccess, self_hook, const LilyAstExprAccess *self)
{
    FREE(LilyAstExprAccessHook, &self->self_hook);
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
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_HOOK:
            FREE_VARIANT(LilyAstExprAccess, global_hook, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_GLOBAL_PATH:
            FREE_VARIANT(LilyAstExprAccess, global_path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_HOOK:
            FREE_VARIANT(LilyAstExprAccess, hook, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_OBJECT:
            FREE_VARIANT(LilyAstExprAccess, object, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_PATH:
            FREE_VARIANT(LilyAstExprAccess, path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_PROPERTY_INIT:
            FREE_VARIANT(LilyAstExprAccess, property_init, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_SELF_HOOK:
            FREE_VARIANT(LilyAstExprAccess, Self_hook, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_SELF_PATH:
            FREE_VARIANT(LilyAstExprAccess, Self_path, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_self_HOOK:
            FREE_VARIANT(LilyAstExprAccess, self_hook, self);
            break;
        case LILY_AST_EXPR_ACCESS_KIND_self_PATH:
            FREE_VARIANT(LilyAstExprAccess, self_path, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
