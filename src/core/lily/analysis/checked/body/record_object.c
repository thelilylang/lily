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

#include <core/lily/analysis/checked/body/record_object.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedBodyRecordObjectItem type
// (LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT).
VARIANT_DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
                   constant,
                   LilyCheckedBodyRecordObjectItem *self);

// Free LilyCheckedBodyRecordObjectItem type
// (LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD).
VARIANT_DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
                   field,
                   LilyCheckedBodyRecordObjectItem *self);

// Free LilyCheckedBodyRecordObjectItem type
// (LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD).
VARIANT_DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
                   method,
                   LilyCheckedBodyRecordObjectItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyRecordObjectItemKind,
               enum LilyCheckedBodyRecordObjectItemKind self)
{
    switch (self) {
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT:
            return "LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT";
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD:
            return "LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD";
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD:
            return "LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedBodyRecordObjectItem *,
                    LilyCheckedBodyRecordObjectItem,
                    constant,
                    const Location *location,
                    LilyCheckedDeclConstant constant)
{
    LilyCheckedBodyRecordObjectItem *self =
      lily_malloc(sizeof(LilyCheckedBodyRecordObjectItem));

    self->kind = LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedBodyRecordObjectItem *,
                    LilyCheckedBodyRecordObjectItem,
                    field,
                    const Location *location,
                    LilyCheckedFieldObject field)
{
    LilyCheckedBodyRecordObjectItem *self =
      lily_malloc(sizeof(LilyCheckedBodyRecordObjectItem));

    self->kind = LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD;
    self->location = location;
    self->field = field;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedBodyRecordObjectItem *,
                    LilyCheckedBodyRecordObjectItem,
                    method,
                    const Location *location,
                    LilyCheckedDeclMethod method)
{
    LilyCheckedBodyRecordObjectItem *self =
      lily_malloc(sizeof(LilyCheckedBodyRecordObjectItem));

    self->kind = LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyRecordObjectItem,
               const LilyCheckedBodyRecordObjectItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT:
            return format__String(
              "LilyCheckedBodyRecordObjectItem{{ kind = {s}, location = {sa}, "
              "constant = {Sr} }",
              to_string__Debug__LilyCheckedBodyRecordObjectItemKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyCheckedDeclConstant(&self->constant));
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD:
            return format__String(
              "LilyCheckedBodyRecordObjectItem{{ kind = {s}, location = {sa}, "
              "field = {Sr} }",
              to_string__Debug__LilyCheckedBodyRecordObjectItemKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyCheckedFieldObject(&self->field));
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD:
            return format__String(
              "LilyCheckedBodyRecordObjectItem{{ kind = {s}, location = {sa}, "
              "method = {Sr} }",
              to_string__Debug__LilyCheckedBodyRecordObjectItemKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyCheckedDeclMethod(&self->method));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
                   constant,
                   LilyCheckedBodyRecordObjectItem *self)
{
    FREE(LilyCheckedDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
                   field,
                   LilyCheckedBodyRecordObjectItem *self)
{
    FREE(LilyCheckedFieldObject, &self->field);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
                   method,
                   LilyCheckedBodyRecordObjectItem *self)
{
    FREE(LilyCheckedDeclMethod, &self->method);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedBodyRecordObjectItem,
           LilyCheckedBodyRecordObjectItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyCheckedBodyRecordObjectItem, constant, self);
            break;
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_FIELD:
            FREE_VARIANT(LilyCheckedBodyRecordObjectItem, field, self);
            break;
        case LILY_CHECKED_BODY_RECORD_OBJECT_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyCheckedBodyRecordObjectItem, method, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
