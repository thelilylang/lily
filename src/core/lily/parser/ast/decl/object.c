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

#include <core/lily/parser/ast/decl/object.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_CLASS).
static inline VARIANT_DESTRUCTOR(LilyAstDeclObject,
                                 class,
                                 const LilyAstDeclObject *self);

// Free LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(LilyAstDeclObject,
                                 enum,
                                 const LilyAstDeclObject *self);

// Free LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyAstDeclObject,
                                 record,
                                 const LilyAstDeclObject *self);

// Free LilyAstDeclObject type (LILY_AST_DECL_OBJECT_KIND_TRAIT).
static inline VARIANT_DESTRUCTOR(LilyAstDeclObject,
                                 trait,
                                 const LilyAstDeclObject *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstDeclObjectKind,
               enum LilyAstDeclObjectKind self)
{
    switch (self) {
        case LILY_AST_DECL_OBJECT_KIND_CLASS:
            return "LILY_AST_DECL_OBJECT_KIND_CLASS";
        case LILY_AST_DECL_OBJECT_KIND_ENUM:
            return "LILY_AST_DECL_OBJECT_KIND_ENUM";
        case LILY_AST_DECL_OBJECT_KIND_RECORD:
            return "LILY_AST_DECL_OBJECT_KIND_RECORD";
        case LILY_AST_DECL_OBJECT_KIND_TRAIT:
            return "LILY_AST_DECL_OBJECT_KIND_TRAIT";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclObject, const LilyAstDeclObject *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_OBJECT_KIND_CLASS:
            return format__String(
              "LilyAstDeclObject{{ kind = {s}, class = {Sr} }",
              to_string__Debug__LilyAstDeclObjectKind(self->kind),
              to_string__Debug__LilyAstDeclClass(&self->class));
        case LILY_AST_DECL_OBJECT_KIND_ENUM:
            return format__String(
              "LilyAstDeclObject{{ kind = {s}, enum = {Sr} }",
              to_string__Debug__LilyAstDeclObjectKind(self->kind),
              to_string__Debug__LilyAstDeclEnumObject(&self->enum_));
        case LILY_AST_DECL_OBJECT_KIND_RECORD:
            return format__String(
              "LilyAstDeclObject{{ kind = {s}, record = {Sr} }",
              to_string__Debug__LilyAstDeclObjectKind(self->kind),
              to_string__Debug__LilyAstDeclRecordObject(&self->record));
        case LILY_AST_DECL_OBJECT_KIND_TRAIT:
            return format__String(
              "LilyAstDeclObject{{ kind = {s}, trait = {Sr} }",
              to_string__Debug__LilyAstDeclObjectKind(self->kind),
              to_string__Debug__LilyAstDeclTrait(&self->trait));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstDeclObject, class, const LilyAstDeclObject *self)
{
    FREE(LilyAstDeclClass, &self->class);
}

VARIANT_DESTRUCTOR(LilyAstDeclObject, enum, const LilyAstDeclObject *self)
{
    FREE(LilyAstDeclEnumObject, &self->enum_);
}

VARIANT_DESTRUCTOR(LilyAstDeclObject, record, const LilyAstDeclObject *self)
{
    FREE(LilyAstDeclRecordObject, &self->record);
}

VARIANT_DESTRUCTOR(LilyAstDeclObject, trait, const LilyAstDeclObject *self)
{
    FREE(LilyAstDeclTrait, &self->trait);
}

DESTRUCTOR(LilyAstDeclObject, const LilyAstDeclObject *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_OBJECT_KIND_CLASS:
            FREE_VARIANT(LilyAstDeclObject, class, self);
            break;
        case LILY_AST_DECL_OBJECT_KIND_ENUM:
            FREE_VARIANT(LilyAstDeclObject, enum, self);
            break;
        case LILY_AST_DECL_OBJECT_KIND_RECORD:
            FREE_VARIANT(LilyAstDeclObject, record, self);
            break;
        case LILY_AST_DECL_OBJECT_KIND_TRAIT:
            FREE_VARIANT(LilyAstDeclObject, trait, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
