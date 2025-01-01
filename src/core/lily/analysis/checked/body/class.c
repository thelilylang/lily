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

#include <base/alloc.h>

#include <core/lily/analysis/checked/body/class.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedBodyClassItem type
// (LILY_CHECKED_BODY_CLASS_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyCheckedBodyClassItem,
                          attribute,
                          LilyCheckedBodyClassItem *self);

// Free LilyCheckedBodyClassItem type
// (LILY_CHECKED_BODY_CLASS_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyCheckedBodyClassItem,
                          method,
                          LilyCheckedBodyClassItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyClassItemKind,
               enum LilyCheckedBodyClassItemKind self)
{
    switch (self) {
        case LILY_CHECKED_BODY_CLASS_ITEM_KIND_ATTRIBUTE:
            return "LILY_CHECKED_BODY_CLASS_ITEM_KIND_ATTRIBUTE";
        case LILY_CHECKED_BODY_CLASS_ITEM_KIND_METHOD:
            return "LILY_CHECKED_BODY_CLASS_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedBodyClassItem *,
                    LilyCheckedBodyClassItem,
                    attribute,
                    Location location,
                    LilyCheckedDeclAttribute attribute)
{
    LilyCheckedBodyClassItem *self =
      lily_malloc(sizeof(LilyCheckedBodyClassItem));

    self->kind = LILY_CHECKED_BODY_CLASS_ITEM_KIND_ATTRIBUTE;
    self->location = location;
    self->attribute = attribute;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedBodyClassItem *,
                    LilyCheckedBodyClassItem,
                    method,
                    Location location,
                    LilyCheckedDeclMethod method)
{
    LilyCheckedBodyClassItem *self =
      lily_malloc(sizeof(LilyCheckedBodyClassItem));

    self->kind = LILY_CHECKED_BODY_CLASS_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyClassItem,
               const LilyCheckedBodyClassItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_CLASS_ITEM_KIND_ATTRIBUTE:
            return format__String(
              "LilyCheckedBodyClassItem{{ kind = {s}, location = {sa}, "
              "attribute = "
              "{Sr} }",
              to_string__Debug__LilyCheckedBodyClassItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclAttribute(&self->attribute));
        case LILY_CHECKED_BODY_CLASS_ITEM_KIND_METHOD:
            return format__String(
              "LilyCheckedBodyClassItem{{ kind = {s}, location = {sa}, method "
              "= "
              "{Sr} }",
              to_string__Debug__LilyCheckedBodyClassItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclMethod(&self->method));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedBodyClassItem,
                   attribute,
                   LilyCheckedBodyClassItem *self)
{
    FREE(LilyCheckedDeclAttribute, &self->attribute);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyClassItem,
                   method,
                   LilyCheckedBodyClassItem *self)
{
    FREE(LilyCheckedDeclMethod, &self->method);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedBodyClassItem, LilyCheckedBodyClassItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_CLASS_ITEM_KIND_ATTRIBUTE:
            FREE_VARIANT(LilyCheckedBodyClassItem, attribute, self);
            break;
        case LILY_CHECKED_BODY_CLASS_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyCheckedBodyClassItem, method, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
