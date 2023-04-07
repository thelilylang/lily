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

#include <core/lily/checked/data_type.h>
#include <core/lily/checked/expr.h>
#include <core/lily/checked/expr/access.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedExprAccessHook type.
static DESTRUCTOR(LilyCheckedExprAccessHook,
                  const LilyCheckedExprAccessHook *self);

// Free LilyCheckedExprAccessObject type.
static DESTRUCTOR(LilyCheckedExprAccessObject,
                  const LilyCheckedExprAccessObject *self);

// Free LilyCheckedExprAccess type (LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 global,
                                 const LilyCheckedExprAccess *self);

// Free LilyCheckedExprAccess type (LILY_CHECKED_EXPR_ACCESS_KIND_HOOK).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 hook,
                                 const LilyCheckedExprAccess *self);

// Free LilyCheckedExprAccess type (LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 object,
                                 const LilyCheckedExprAccess *self);

// Free LilyCheckedExprAccess type (LILY_CHECKED_EXPR_ACCESS_KIND_PATH).
static VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                          path,
                          const LilyCheckedExprAccess *self);

// Free LilyCheckedExprAccess type
// (LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 property_init,
                                 const LilyCheckedExprAccess *self);

// Free LilyCheckedExprAccess type (LILY_CHECKED_EXPR_ACCESS_KIND_SELF).
static inline VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                                 self,
                                 const LilyCheckedExprAccess *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessHook,
               const LilyCheckedExprAccessHook *self)
{
    return format("LilyCheckedExprAccessHook{{ access = {Sr}, expr = {Sr} }",
                  to_string__Debug__LilyCheckedExpr(self->access),
                  to_string__Debug__LilyCheckedExpr(self->expr));
}

DESTRUCTOR(LilyCheckedExprAccessHook, const LilyCheckedExprAccessHook *self)
{
    FREE(LilyCheckedExpr, self->access);
    FREE(LilyCheckedExpr, self->expr);
}

DESTRUCTOR(LilyCheckedExprAccessObject, const LilyCheckedExprAccessObject *self)
{
    if (self->access) {
        FREE(LilyCheckedExpr, self->access);
    }

    FREE_BUFFER_ITEMS(
      self->object->buffer, self->object->len, LilyCheckedDataType);
    FREE(Vec, self->object);
}

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessObject,
               const LilyCheckedExprAccessObject *self)
{
    String *res = from__String("LilyCheckedExprAccessObject{ access = ");

    if (self->access) {
        String *s = to_string__Debug__LilyCheckedExpr(self->access);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", object =");
    DEBUG_VEC_STRING(self->object, res, LilyCheckedDataType);
    push_str__String(res, " }");

    return res;
}

char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccessKind,
               enum LilyCheckedExprAccessKind self)
{
    switch (self) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL";
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_HOOK";
        case LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT";
        case LILY_CHECKED_EXPR_ACCESS_KIND_PATH:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_PATH";
        case LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT";
        case LILY_CHECKED_EXPR_ACCESS_KIND_SELF:
            return "LILY_CHECKED_EXPR_ACCESS_KIND_SELF";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprAccess,
               const LilyCheckedExprAccess *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL:
            return format__String(
              "LilyCheckedExprAccess{{ kind = {s}, scope = {sa}, global = {Sr} "
              "}",
              to_string__Debug__LilyCheckedExprAccessKind(self->kind),
              to_string__Debug__LilyCheckedScope(&self->scope),
              to_string__Debug__LilyCheckedExpr(self->global));
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            return format__String(
              "LilyCheckedExprAccess{{ kind = {s}, scope = {sa}, hook = {sa} }",
              to_string__Debug__LilyCheckedExprAccessKind(self->kind),
              to_string__Debug__LilyCheckedScope(&self->scope),
              to_string__Debug__LilyCheckedExprAccessHook(&self->hook));
        case LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT: {
            String *res = format__String(
              "LilyCheckedExprAccess{{ kind = {s}, scope = {sa}, object =",
              to_string__Debug__LilyCheckedExprAccessKind(self->kind),
              to_string__Debug__LilyCheckedScope(&self->scope));

            DEBUG_VEC_STRING(self->object.object, res, LilyCheckedDataType);

            push_str__String(res, " }");

            return res;
        }
        case LILY_CHECKED_EXPR_ACCESS_KIND_PATH: {
            String *res = format__String(
              "LilyCheckedExprAccess{{ kind = {s}, scope = {sa}, path =",
              to_string__Debug__LilyCheckedExprAccessKind(self->kind),
              to_string__Debug__LilyCheckedScope(&self->scope));

            DEBUG_VEC_STRING(self->path, res, LilyCheckedExpr);

            push_str__String(res, " }");

            return res;
        }
        case LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT:
            return format__String(
              "LilyCheckedExprAccess{{ kind = {s}, scope = {sa}, property_init "
              "= {Sr} }",
              to_string__Debug__LilyCheckedExprAccessKind(self->kind),
              to_string__Debug__LilyCheckedScope(&self->scope),
              to_string__Debug__LilyCheckedExpr(self->property_init));
        case LILY_CHECKED_EXPR_ACCESS_KIND_SELF:
            return format__String(
              "LilyCheckedExprAccess{{ kind = {s}, scope = {sa}, self = {Sr} }",
              to_string__Debug__LilyCheckedExprAccessKind(self->kind),
              to_string__Debug__LilyCheckedScope(&self->scope),
              to_string__Debug__LilyCheckedExpr(self->self));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   global,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExpr, self->global);
}

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   hook,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExprAccessHook, &self->hook);
}

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   object,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExprAccessObject, &self->object);
}

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   path,
                   const LilyCheckedExprAccess *self)
{
    FREE_BUFFER_ITEMS(self->path->buffer, self->path->len, LilyCheckedExpr);
    FREE(Vec, self->path);
}

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   property_init,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExpr, self->property_init);
}

VARIANT_DESTRUCTOR(LilyCheckedExprAccess,
                   self,
                   const LilyCheckedExprAccess *self)
{
    FREE(LilyCheckedExpr, self->self);
}

DESTRUCTOR(LilyCheckedExprAccess, const LilyCheckedExprAccess *self)
{
    switch (self->kind) {
        case LILY_CHECKED_EXPR_ACCESS_KIND_GLOBAL:
            FREE_VARIANT(LilyCheckedExprAccess, global, self);
        case LILY_CHECKED_EXPR_ACCESS_KIND_HOOK:
            FREE_VARIANT(LilyCheckedExprAccess, hook, self);
        case LILY_CHECKED_EXPR_ACCESS_KIND_OBJECT:
            FREE_VARIANT(LilyCheckedExprAccess, object, self);
        case LILY_CHECKED_EXPR_ACCESS_KIND_PATH:
            FREE_VARIANT(LilyCheckedExprAccess, path, self);
        case LILY_CHECKED_EXPR_ACCESS_KIND_PROPERTY_INIT:
            FREE_VARIANT(LilyCheckedExprAccess, property_init, self);
        case LILY_CHECKED_EXPR_ACCESS_KIND_SELF:
            FREE_VARIANT(LilyCheckedExprAccess, self, self);
        default:
            UNREACHABLE("unknown variant");
    }
}
