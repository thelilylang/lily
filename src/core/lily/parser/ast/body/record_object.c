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

#include <core/lily/parser/ast/body/record_object.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstBodyRecordObjectItem type
// (LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT).
VARIANT_DESTRUCTOR(LilyAstBodyRecordObjectItem,
                   constant,
                   LilyAstBodyRecordObjectItem *self);

// Free LilyAstBodyRecordObjectItem type
// (LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_FIELD).
VARIANT_DESTRUCTOR(LilyAstBodyRecordObjectItem,
                   field,
                   LilyAstBodyRecordObjectItem *self);

// Free LilyAstBodyRecordObjectItem type
// (LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_METHOD).
VARIANT_DESTRUCTOR(LilyAstBodyRecordObjectItem,
                   method,
                   LilyAstBodyRecordObjectItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyRecordObjectItemKind,
               enum LilyAstBodyRecordObjectItemKind self)
{
    switch (self) {
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT:
            return "LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT";
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_FIELD:
            return "LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_FIELD";
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_METHOD:
            return "LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstBodyRecordObjectItem *,
                    LilyAstBodyRecordObjectItem,
                    constant,
                    Location location,
                    LilyAstDeclConstant constant)
{
    LilyAstBodyRecordObjectItem *self =
      lily_malloc(sizeof(LilyAstBodyRecordObjectItem));

    self->kind = LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstBodyRecordObjectItem *,
                    LilyAstBodyRecordObjectItem,
                    field,
                    Location location,
                    LilyAstFieldObject field)
{
    LilyAstBodyRecordObjectItem *self =
      lily_malloc(sizeof(LilyAstBodyRecordObjectItem));

    self->kind = LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_FIELD;
    self->location = location;
    self->field = field;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstBodyRecordObjectItem *,
                    LilyAstBodyRecordObjectItem,
                    method,
                    Location location,
                    LilyAstDeclMethod method)
{
    LilyAstBodyRecordObjectItem *self =
      lily_malloc(sizeof(LilyAstBodyRecordObjectItem));

    self->kind = LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyRecordObjectItem,
               const LilyAstBodyRecordObjectItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT:
            return format__String(
              "LilyAstBodyRecordObjectItem{{ kind = {s}, location = {sa}, "
              "constant = {Sr} }",
              to_string__Debug__LilyAstBodyRecordObjectItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclConstant(&self->constant));
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_FIELD:
            return format__String(
              "LilyAstBodyRecordObjectItem{{ kind = {s}, location = {sa}, "
              "field = {Sr} }",
              to_string__Debug__LilyAstBodyRecordObjectItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstFieldObject(&self->field));
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_METHOD:
            return format__String(
              "LilyAstBodyRecordObjectItem{{ kind = {s}, location = {sa}, "
              "method = {Sr} }",
              to_string__Debug__LilyAstBodyRecordObjectItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclMethod(&self->method));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstBodyRecordObjectItem,
                   constant,
                   LilyAstBodyRecordObjectItem *self)
{
    FREE(LilyAstDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyRecordObjectItem,
                   field,
                   LilyAstBodyRecordObjectItem *self)
{
    FREE(LilyAstFieldObject, &self->field);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyRecordObjectItem,
                   method,
                   LilyAstBodyRecordObjectItem *self)
{
    FREE(LilyAstDeclMethod, &self->method);
    lily_free(self);
}

DESTRUCTOR(LilyAstBodyRecordObjectItem, LilyAstBodyRecordObjectItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyAstBodyRecordObjectItem, constant, self);
            break;
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_FIELD:
            FREE_VARIANT(LilyAstBodyRecordObjectItem, field, self);
            break;
        case LILY_AST_BODY_RECORD_OBJECT_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyAstBodyRecordObjectItem, method, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
