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

#include <core/lily/analysis/checked/decl/object.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedDeclObject type (LILY_CHECKED_DECL_OBJECT_KIND_CLASS).
static inline VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                                 class,
                                 const LilyCheckedDeclObject *self);

// Free LilyCheckedDeclObject type (LILY_CHECKED_DECL_OBJECT_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                                 enum,
                                 const LilyCheckedDeclObject *self);

// Free LilyCheckedDeclObject type (LILY_CHECKED_DECL_OBJECT_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                                 record,
                                 const LilyCheckedDeclObject *self);

// Free LilyCheckedDeclObject type (LILY_CHECKED_DECL_OBJECT_KIND_TRAIT).
static inline VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                                 trait,
                                 const LilyCheckedDeclObject *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclObjectKind,
               enum LilyCheckedDeclObjectKind self)
{
    switch (self) {
        case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
            return "LILY_CHECKED_DECL_OBJECT_KIND_CLASS";
        case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
            return "LILY_CHECKED_DECL_OBJECT_KIND_ENUM";
        case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
            return "LILY_CHECKED_DECL_OBJECT_KIND_RECORD";
        case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
            return "LILY_CHECKED_DECL_OBJECT_KIND_TRAIT";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclObject,
               const LilyCheckedDeclObject *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
            return format__String(
              "LilyCheckedDeclObject{{ kind = {s}, class = {Sr} }",
              to_string__Debug__LilyCheckedDeclObjectKind(self->kind),
              to_string__Debug__LilyCheckedDeclClass(&self->class));
        case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
            return format__String(
              "LilyCheckedDeclObject{{ kind = {s}, enum = {Sr} }",
              to_string__Debug__LilyCheckedDeclObjectKind(self->kind),
              to_string__Debug__LilyCheckedDeclEnumObject(&self->enum_));
        case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
            return format__String(
              "LilyCheckedDeclObject{{ kind = {s}, record = {Sr} }",
              to_string__Debug__LilyCheckedDeclObjectKind(self->kind),
              to_string__Debug__LilyCheckedDeclRecordObject(&self->record));
        case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
            return format__String(
              "LilyCheckedDeclObject{{ kind = {s}, trait = {Sr} }",
              to_string__Debug__LilyCheckedDeclObjectKind(self->kind),
              to_string__Debug__LilyCheckedDeclTrait(&self->trait));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                   class,
                   const LilyCheckedDeclObject *self)
{
    FREE(Vec, self->deps);
    FREE(LilyCheckedDeclClass, &self->class);
}

VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                   enum,
                   const LilyCheckedDeclObject *self)
{
    FREE(Vec, self->deps);
    FREE(LilyCheckedDeclEnumObject, &self->enum_);
}

VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                   record,
                   const LilyCheckedDeclObject *self)
{
    FREE(Vec, self->deps);
    FREE(LilyCheckedDeclRecordObject, &self->record);
}

VARIANT_DESTRUCTOR(LilyCheckedDeclObject,
                   trait,
                   const LilyCheckedDeclObject *self)
{
    FREE(Vec, self->deps);
    FREE(LilyCheckedDeclTrait, &self->trait);
}

DESTRUCTOR(LilyCheckedDeclObject, const LilyCheckedDeclObject *self)
{
    switch (self->kind) {
        case LILY_CHECKED_DECL_OBJECT_KIND_CLASS:
            FREE_VARIANT(LilyCheckedDeclObject, class, self);
            break;
        case LILY_CHECKED_DECL_OBJECT_KIND_ENUM:
            FREE_VARIANT(LilyCheckedDeclObject, enum, self);
            break;
        case LILY_CHECKED_DECL_OBJECT_KIND_RECORD:
            FREE_VARIANT(LilyCheckedDeclObject, record, self);
            break;
        case LILY_CHECKED_DECL_OBJECT_KIND_TRAIT:
            FREE_VARIANT(LilyCheckedDeclObject, trait, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
