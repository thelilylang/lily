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

#include <base/format.h>
#include <base/macros.h>

#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/parent.h>
#include <core/lily/analysis/checked/scope.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedParentKind,
               enum LilyCheckedParentKind self)
{
    switch (self) {
        case LILY_CHECKED_PARENT_KIND_DECL:
            return "LILY_CHECKED_PARENT_KIND_DECL";
        case LILY_CHECKED_PARENT_KIND_MODULE:
            return "LILY_CHECKED_PARENT_KIND_MODULE";
        case LILY_CHECKED_PARENT_KIND_SCOPE:
            return "LILY_CHECKED_PARENT_KIND_SCOPE";
        case LILY_CHECKED_PARENT_KIND_STMT:
            return "LILY_CHECKED_PARENT_KIND_STMT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    decl,
                    LilyCheckedScope *scope,
                    LilyCheckedDecl *decl)
{
    LilyCheckedParent *self = lily_malloc(sizeof(LilyCheckedParent));

    self->kind = LILY_CHECKED_PARENT_KIND_DECL;
    self->scope = scope;
    self->decl = decl;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    module,
                    LilyCheckedScope *scope,
                    LilyCheckedDeclModule *module)
{
    LilyCheckedParent *self = lily_malloc(sizeof(LilyCheckedParent));

    self->kind = LILY_CHECKED_PARENT_KIND_MODULE;
    self->scope = scope;
    self->module = module;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    scope,
                    LilyCheckedScope *scope,
                    Vec *scope_body)
{
    LilyCheckedParent *self = lily_malloc(sizeof(LilyCheckedParent));

    self->kind = LILY_CHECKED_PARENT_KIND_SCOPE;
    self->scope = scope;
    self->scope_body = scope_body;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    stmt,
                    LilyCheckedScope *scope,
                    LilyCheckedScopeStmt stmt)
{
    LilyCheckedParent *self = lily_malloc(sizeof(LilyCheckedParent));

    self->kind = LILY_CHECKED_PARENT_KIND_STMT;
    self->scope = scope;
    self->stmt = stmt;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedParent, const LilyCheckedParent *self)
{
    String *res =
      format__String("LilyCheckedParent{{ kind = {s}, scope = {Sr}",
                     to_string__Debug__LilyCheckedParentKind(self->kind),
                     to_string__Debug__LilyCheckedScope(self->scope));

    switch (self->kind) {
        case LILY_CHECKED_PARENT_KIND_DECL: {
            char *s = format(", decl = {d} }", self->kind);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PARENT_KIND_MODULE: {
            char *s = format(", module = {S} }", self->module->global_name);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PARENT_KIND_SCOPE:
            break;
        case LILY_CHECKED_PARENT_KIND_STMT: {
            char *s = format(", stmt = {S} }");

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif
