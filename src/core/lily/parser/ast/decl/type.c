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

#include <core/lily/parser/ast/decl/type.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstDeclType type (LILY_AST_DECL_TYPE_KIND_ALIAS).
static inline VARIANT_DESTRUCTOR(LilyAstDeclType,
                                 alias,
                                 const LilyAstDeclType *self);

// Free LilyAstDeclType type (LILY_AST_DECL_TYPE_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(LilyAstDeclType,
                                 enum,
                                 const LilyAstDeclType *self);

// Free LilyAstDeclType type (LILY_AST_DECL_TYPE_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyAstDeclType,
                                 record,
                                 const LilyAstDeclType *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstDeclTypeKind, enum LilyAstDeclTypeKind self)
{
    switch (self) {
        case LILY_AST_DECL_TYPE_KIND_ALIAS:
            return "LILY_AST_DECL_TYPE_KIND_ALIAS";
        case LILY_AST_DECL_TYPE_KIND_ENUM:
            return "LILY_AST_DECL_TYPE_KIND_ENUM";
        case LILY_AST_DECL_TYPE_KIND_RECORD:
            return "LILY_AST_DECL_TYPE_KIND_RECORD";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclType, const LilyAstDeclType *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_TYPE_KIND_ALIAS:
            return format__String(
              "LilyAstDeclType{{ kind = {s}, alias = {Sr} }",
              to_string__Debug__LilyAstDeclTypeKind(self->kind),
              to_string__Debug__LilyAstDeclAlias(&self->alias));
        case LILY_AST_DECL_TYPE_KIND_ENUM:
            return format__String(
              "LilyAstDeclType{{ kind = {s}, enum = {Sr} }",
              to_string__Debug__LilyAstDeclTypeKind(self->kind),
              to_string__Debug__LilyAstDeclEnum(&self->enum_));
        case LILY_AST_DECL_TYPE_KIND_RECORD:
            return format__String(
              "LilyAstDeclType{{ kind = {s}, record = {Sr} }",
              to_string__Debug__LilyAstDeclTypeKind(self->kind),
              to_string__Debug__LilyAstDeclRecord(&self->record));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstDeclType, alias, const LilyAstDeclType *self)
{
    FREE(LilyAstDeclAlias, &self->alias);
}

VARIANT_DESTRUCTOR(LilyAstDeclType, enum, const LilyAstDeclType *self)
{
    FREE(LilyAstDeclEnum, &self->enum_);
}

VARIANT_DESTRUCTOR(LilyAstDeclType, record, const LilyAstDeclType *self)
{
    FREE(LilyAstDeclRecord, &self->record);
}

DESTRUCTOR(LilyAstDeclType, const LilyAstDeclType *self)
{
    switch (self->kind) {
        case LILY_AST_DECL_TYPE_KIND_ALIAS:
            FREE_VARIANT(LilyAstDeclType, alias, self);
            break;
        case LILY_AST_DECL_TYPE_KIND_ENUM:
            FREE_VARIANT(LilyAstDeclType, enum, self);
            break;
        case LILY_AST_DECL_TYPE_KIND_RECORD:
            FREE_VARIANT(LilyAstDeclType, record, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
