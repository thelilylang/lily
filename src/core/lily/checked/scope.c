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

#include <base/format.h>
#include <base/new.h>

#include <core/lily/checked/scope.h>

#ifdef ENV_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGlobalScopeKind,
               enum LilyCheckedGlobalScopeKind self)
{
    switch (self) {
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_ATTRIBUTE:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_ATTRIBUTE";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_CLASS:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_CLASS";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_CONSTANT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_CONSTANT";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_ENUM:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_ENUM";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_ENUM_OBJECT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_ENUM_OBJECT";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_FIELD:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_FIELD";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_FIELD_OBJECT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_FIELD_OBJECT";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_FUN:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_FUN";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_METHOD:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_METHOD";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_MODULE:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_MODULE";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_PROTOTYPE:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_PROTOTYPE";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_RECORD:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_RECORD";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_RECORD_OBJECT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_RECORD_OBJECT";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_TRAIT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_TRAIT";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_VARIANT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_VARIANT";
        case LILY_CHECKED_GLOBAL_SCOPE_KIND_VARIANT_OBJECT:
            return "LILY_CHECKED_GLOBAL_SCOPE_KIND_VARIANT_OBJECT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(LilyCheckedGlobalScope *,
            LilyCheckedGlobalScope,
            enum LilyCheckedGlobalScopeKind kind,
            LilyCheckedGlobalScopeId id,
            Vec *access,
            Vec *children,
            enum LilyVisibility visibility,
            Location location)
{
    LilyCheckedGlobalScope *self = lily_malloc(sizeof(LilyCheckedGlobalScope));

    self->kind = kind;
    self->id = id;
    self->access = access;
    self->children = children;
    self->visibility = visibility;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGlobalScope,
               const LilyCheckedGlobalScope *self)
{
    String *res = format__String(
      "LilyCheckedGlobalScopeKind{ kind = {s}, id = {d}, access = { ",
      to_string__Debug__LilyCheckedGlobalScopeKind(self->kind),
      self->id);

    for (Usize i = 0; i < self->access->len; i++) {
        String *item = get__Vec(self->access, i);

        DEBUG_STRING(item, res);

        if (i != self->access->len - 1) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, " }");
    push_str__String(res, ", children =");

    if (self->children) {
        DEBUG_VEC_STRING(self->children, res, LilyCheckedGlobalScope);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", visibility = {s}, location = {sa} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedGlobalScope, LilyCheckedGlobalScope *self)
{
    if (self->children) {
        FREE_BUFFER_ITEMS(
          self->children->buffer, self->children->len, LilyCheckedGlobalScope);
        FREE(Vec, self->children);
    }

    lily_free(self);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExternScopeKind,
               enum LilyCheckedExternScopeKind self)
{
    switch (self) {
        case LILY_CHECKED_EXTERN_SCOPE_KIND_FILE:
            return "LILY_CHECKED_EXTERN_SCOPE_KIND_FILE";
        case LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY:
            return "LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY";
        case LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE:
            return "LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    file,
                    LilyCheckedGlobalScope *global,
                    LilyFile *file)
{
    LilyCheckedExternScope *self = lily_malloc(sizeof(LilyCheckedExternScope));

    self->global = global;
    self->kind = LILY_CHECKED_EXTERN_SCOPE_KIND_FILE;
    self->file = file;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    library,
                    LilyCheckedGlobalScope *global,
                    LilyLibrary *library)
{
    LilyCheckedExternScope *self = lily_malloc(sizeof(LilyCheckedExternScope));

    self->global = global;
    self->kind = LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY;
    self->library = library;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    package,
                    LilyCheckedGlobalScope *global,
                    LilyPackage *package)
{
    LilyCheckedExternScope *self = lily_malloc(sizeof(LilyCheckedExternScope));

    self->global = global;
    self->kind = LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE;
    self->package = package;

    return self;
}

CONSTRUCTOR(LilyCheckedLocalScope *,
            LilyCheckedLocalScope,
            enum LilyCheckedLocalScopeKind kind,
            Vec *access)
{
    LilyCheckedLocalScope *self = lily_malloc(sizeof(LilyCheckedLocalScope));

    self->kind = kind;
    self->access = access;

    return self;
}

DESTRUCTOR(LilyCheckedLocalScope, LilyCheckedLocalScope *self)
{
    FREE(Vec, self->access);
    lily_free(self);
}