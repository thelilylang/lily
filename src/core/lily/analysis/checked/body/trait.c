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

#include <core/lily/analysis/checked/body/trait.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedBodyTraitItem type
// (LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyCheckedBodyTraitItem,
                          attribute,
                          LilyCheckedBodyTraitItem *self);

// Free LilyCheckedBodyTraitItem type
// (LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyCheckedBodyTraitItem,
                          prototype,
                          LilyCheckedBodyTraitItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyTraitItemKind,
               enum LilyCheckedBodyTraitItemKind self)
{
    switch (self) {
        case LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE:
            return "LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE";
        case LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE:
            return "LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedBodyTraitItem *,
                    LilyCheckedBodyTraitItem,
                    attribute,
                    Location location,
                    LilyCheckedDeclAttribute attribute)
{
    LilyCheckedBodyTraitItem *self =
      lily_malloc(sizeof(LilyCheckedBodyTraitItem));

    self->kind = LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE;
    self->location = location;
    self->attribute = attribute;

    return self;
}

/**
 *
 * @brief Construct LilyCheckedBodyTraitItem type
 * (LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedBodyTraitItem *,
                    LilyCheckedBodyTraitItem,
                    prototype,
                    Location location,
                    LilyCheckedDeclPrototype prototype)
{
    LilyCheckedBodyTraitItem *self =
      lily_malloc(sizeof(LilyCheckedBodyTraitItem));

    self->kind = LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE;
    self->location = location;
    self->prototype = prototype;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyTraitItem,
               const LilyCheckedBodyTraitItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE:
            return format__String(
              "LilyCheckedBodyTraitItem{{ kind = {s}, location = {sa}, "
              "attribute = "
              "{Sr} }",
              to_string__Debug__LilyCheckedBodyTraitItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclAttribute(&self->attribute));
        case LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE:
            return format__String(
              "LilyCheckedBodyTraitItem{{ kind = {s}, location = {sa}, "
              "prototype = "
              "{Sr} }",
              to_string__Debug__LilyCheckedBodyTraitItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyCheckedDeclPrototype(&self->prototype));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedBodyTraitItem,
                   attribute,
                   LilyCheckedBodyTraitItem *self)
{
    FREE(LilyCheckedDeclAttribute, &self->attribute);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyTraitItem,
                   prototype,
                   LilyCheckedBodyTraitItem *self)
{
    FREE(LilyCheckedDeclPrototype, &self->prototype);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedBodyTraitItem, LilyCheckedBodyTraitItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_TRAIT_ITEM_KIND_ATTRIBUTE:
            FREE_VARIANT(LilyCheckedBodyTraitItem, attribute, self);
            break;
        case LILY_CHECKED_BODY_TRAIT_ITEM_KIND_PROTOTYPE:
            FREE_VARIANT(LilyCheckedBodyTraitItem, prototype, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
