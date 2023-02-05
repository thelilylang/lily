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

#include <base/new.h>

#include <core/lily/token.h>

#include <stdlib.h>

// Free LilyToken type (LILY_TOKEN_KIND_COMMENT_DOC).
static inline VARIANT_DESTRUCTOR(LilyToken, comment_doc, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_MACRO).
static inline VARIANT_DESTRUCTOR(LilyToken, identifier_macro, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_NORMAL).
static inline VARIANT_DESTRUCTOR(LilyToken, identifier_normal, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_OPERATOR).
static inline VARIANT_DESTRUCTOR(LilyToken,
                                 identifier_operator,
                                 LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_FLOAT).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_float, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_INT).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_int, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_STRING).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_string, LilyToken *self);

CONSTRUCTOR(LilyToken *, LilyToken, enum LilyTokenKind kind, Location location)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = kind;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    comment_doc,
                    Location location,
                    String *comment_doc)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_COMMENT_DOC;
    self->location = location;
    self->comment_doc = comment_doc;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_macro,
                    Location location,
                    String *identifier_macro)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_IDENTIFIER_MACRO;
    self->location = location;
    self->identifier_macro = identifier_macro;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_normal,
                    Location location,
                    String *identifier_normal)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_IDENTIFIER_NORMAL;
    self->location = location;
    self->identifier_normal = identifier_normal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_operator,
                    Location location,
                    String *identifier_operator)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_IDENTIFIER_OPERATOR;
    self->location = location;
    self->identifier_operator = identifier_operator;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_bit_char,
                    Location location,
                    Uint8 literal_bit_char)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_BIT_CHAR;
    self->location = location;
    self->literal_bit_char = literal_bit_char;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_bit_string,
                    Location location,
                    Uint8 *literal_bit_string)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_BIT_STRING;
    self->location = location;
    self->literal_bit_string = literal_bit_string;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_char,
                    Location location,
                    char literal_char)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_CHAR;
    self->location = location;
    self->literal_char = literal_char;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_float,
                    Location location,
                    String *literal_float)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_FLOAT;
    self->location = location;
    self->literal_float = literal_float;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int,
                    Location location,
                    String *literal_int)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_INT;
    self->location = location;
    self->literal_int = literal_int;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_string,
                    Location location,
                    String *literal_string)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_STRING;
    self->location = location;
    self->literal_string = literal_string;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_float32,
                    Location location,
                    Float32 literal_suffix_float32)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32;
    self->location = location;
    self->literal_suffix_float32 = literal_suffix_float32;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_float64,
                    Location location,
                    Float64 literal_suffix_float64)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64;
    self->location = location;
    self->literal_suffix_float64 = literal_suffix_float64;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int16,
                    Location location,
                    Int16 literal_suffix_int16)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16;
    self->location = location;
    self->literal_suffix_int16 = literal_suffix_int16;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int32,
                    Location location,
                    Int32 literal_suffix_int32)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32;
    self->location = location;
    self->literal_suffix_int32 = literal_suffix_int32;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int64,
                    Location location,
                    Int64 literal_suffix_int64)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64;
    self->location = location;
    self->literal_suffix_int64 = literal_suffix_int64;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int8,
                    Location location,
                    Int8 literal_suffix_int8)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8;
    self->location = location;
    self->literal_suffix_int8 = literal_suffix_int8;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_isize,
                    Location location,
                    Isize literal_suffix_isize)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE;
    self->location = location;
    self->literal_suffix_isize = literal_suffix_isize;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint16,
                    Location location,
                    Uint16 literal_suffix_uint16)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16;
    self->location = location;
    self->literal_suffix_uint16 = literal_suffix_uint16;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint32,
                    Location location,
                    Uint32 literal_suffix_uint32)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32;
    self->location = location;
    self->literal_suffix_uint32 = literal_suffix_uint32;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint64,
                    Location location,
                    Uint64 literal_suffix_uint64)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64;
    self->location = location;
    self->literal_suffix_uint64 = literal_suffix_uint64;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint8,
                    Location location,
                    Uint8 literal_suffix_uint8)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8;
    self->location = location;
    self->literal_suffix_uint8 = literal_suffix_uint8;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_usize,
                    Location location,
                    Usize literal_suffix_usize)
{
    LilyToken *self = malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE;
    self->location = location;
    self->literal_suffix_usize = literal_suffix_usize;

    return self;
}

VARIANT_DESTRUCTOR(LilyToken, comment_doc, LilyToken *self)
{
    FREE(String, self->comment_doc);
    free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_macro, LilyToken *self)
{
    FREE(String, self->identifier_macro);
    free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_operator, LilyToken *self)
{
    FREE(String, self->identifier_operator);
    free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_normal, LilyToken *self)
{
    FREE(String, self->identifier_normal);
    free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_float, LilyToken *self)
{
    FREE(String, self->literal_float);
    free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_int, LilyToken *self)
{
    FREE(String, self->literal_int);
    free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_string, LilyToken *self)
{
    FREE(String, self->literal_string);
    free(self);
}

DESTRUCTOR(LilyToken, LilyToken *self)
{
    switch (self->kind) {
        case LILY_TOKEN_KIND_COMMENT_DOC:
            FREE_VARIANT(LilyToken, comment_doc, self);
            break;
        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
            FREE_VARIANT(LilyToken, identifier_macro, self);
            break;
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            FREE_VARIANT(LilyToken, identifier_normal, self);
            break;
        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            FREE_VARIANT(LilyToken, identifier_operator, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            FREE_VARIANT(LilyToken, literal_float, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_INT:
            FREE_VARIANT(LilyToken, literal_int, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_STRING:
            FREE_VARIANT(LilyToken, literal_string, self);
            break;
        default:
            free(self);
    }
}