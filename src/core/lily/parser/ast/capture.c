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

#include <base/macros.h>

#include <core/lily/parser/ast/capture.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyAstCapture type (LILY_AST_CAPTURE_KIND_SINGLE).
static VARIANT_DESTRUCTOR(LilyAstCapture, single, LilyAstCapture *self);

// Free LilyAstCapture type (LILY_AST_CAPTURE_KIND_MULTIPLE).
static VARIANT_DESTRUCTOR(LilyAstCapture, multiple, LilyAstCapture *self);

VARIANT_CONSTRUCTOR(LilyAstCapture *,
                    LilyAstCapture,
                    single,
                    LilyAstExpr *single)
{
    LilyAstCapture *self = lily_malloc(sizeof(LilyAstCapture));

    self->kind = LILY_AST_CAPTURE_KIND_SINGLE;
    self->single = single;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstCapture *, LilyAstCapture, multiple, Vec *multiple)
{
    LilyAstCapture *self = lily_malloc(sizeof(LilyAstCapture));

    self->kind = LILY_AST_CAPTURE_KIND_MULTIPLE;
    self->multiple = multiple;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstCaptureKind, enum LilyAstCaptureKind self)
{
    switch (self) {
        case LILY_AST_CAPTURE_KIND_SINGLE:
            return "LILY_AST_CAPTURE_KIND_SINGLE";
        case LILY_AST_CAPTURE_KIND_MULTIPLE:
            return "LILY_AST_CAPTURE_KIND_MULTIPLE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstCapture, const LilyAstCapture *self)
{
    String *res =
      format__String("LilyAstCapture{{ kind = {s}",
                     to_string__Debug__LilyAstCaptureKind(self->kind));

    switch (self->kind) {
        case LILY_AST_CAPTURE_KIND_SINGLE: {
            char *s = format(", single = {Sr} }",
                             to_string__Debug__LilyAstExpr(self->single));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_CAPTURE_KIND_MULTIPLE:
            push_str__String(res, ", multiple =");

            DEBUG_VEC_STRING(self->multiple, res, LilyAstExpr);

            push_str__String(res, " }");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyAstCapture, single, LilyAstCapture *self)
{
    FREE(LilyAstExpr, self->single);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstCapture, multiple, LilyAstCapture *self)
{
    FREE_BUFFER_ITEMS(self->multiple->buffer, self->multiple->len, LilyAstExpr);
    FREE(Vec, self->multiple);
    lily_free(self);
}

DESTRUCTOR(LilyAstCapture, LilyAstCapture *self)
{
    switch (self->kind) {
        case LILY_AST_CAPTURE_KIND_SINGLE:
            FREE_VARIANT(LilyAstCapture, single, self);
            break;
        case LILY_AST_CAPTURE_KIND_MULTIPLE:
            FREE_VARIANT(LilyAstCapture, multiple, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
