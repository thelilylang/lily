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
#include <base/new.h>

#include <core/lily/scanner/token.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#include <base/print.h>
#endif

// Free LilyToken type (LILY_TOKEN_KIND_COMMENT_DOC).
static inline VARIANT_DESTRUCTOR(LilyToken, comment_doc, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_DOLLAR).
static inline VARIANT_DESTRUCTOR(LilyToken, identifier_dollar, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_MACRO).
static inline VARIANT_DESTRUCTOR(LilyToken, identifier_macro, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_NORMAL).
static inline VARIANT_DESTRUCTOR(LilyToken, identifier_normal, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_OPERATOR).
static inline VARIANT_DESTRUCTOR(LilyToken,
                                 identifier_operator,
                                 LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_STRING).
static inline VARIANT_DESTRUCTOR(LilyToken, identifier_string, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_BYTES).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_bytes, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_CSTR).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_cstr, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_FLOAT).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_float, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_2).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_int_2, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_8).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_int_8, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_10).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_int_10, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_16).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_int_16, LilyToken *self);

// Free LilyToken type (LILY_TOKEN_KIND_LITERAL_STR).
static inline VARIANT_DESTRUCTOR(LilyToken, literal_str, LilyToken *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyTokenExpandKind, enum LilyTokenExpandKind self)
{
    switch (self) {
        case LILY_TOKEN_EXPAND_KIND_EXPR:
            return "LILY_TOKEN_EXPAND_KIND_EXPR";
        case LILY_TOKEN_EXPAND_KIND_PATT:
            return "LILY_TOKEN_EXPAND_KIND_PATT";
        case LILY_TOKEN_EXPAND_KIND_PATH:
            return "LILY_TOKEN_EXPAND_KIND_PATH";
        case LILY_TOKEN_EXPAND_KIND_DT:
            return "LILY_TOKEN_EXPAND_KIND_DT";
        default:
            UNREACHABLE("unknown variant");
    }
}

String *
IMPL_FOR_DEBUG(to_string, LilyTokenExpand, const LilyTokenExpand *self)
{
    String *res =
      format__String("LilyTokenExpand{{ kind = {s}, tokens =",
                     to_string__Debug__LilyTokenExpandKind(self->kind));

    DEBUG_VEC_STR(self->tokens, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

CONSTRUCTOR(LilyToken *, LilyToken, enum LilyTokenKind kind, Location location)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = kind;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    comment_debug,
                    Location location,
                    String *comment_debug)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_COMMENT_DEBUG;
    self->location = location;
    self->comment_debug = comment_debug;

    return self;
}
#endif

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    comment_doc,
                    Location location,
                    String *comment_doc)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_COMMENT_DOC;
    self->location = location;
    self->comment_doc = comment_doc;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    expand,
                    Location location,
                    LilyTokenExpand expand)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_EXPAND;
    self->location = location;
    self->expand = expand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_dollar,
                    Location location,
                    String *identifier_dollar)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_IDENTIFIER_DOLLAR;
    self->location = location;
    self->identifier_dollar = identifier_dollar;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_macro,
                    Location location,
                    String *identifier_macro)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_IDENTIFIER_OPERATOR;
    self->location = location;
    self->identifier_operator = identifier_operator;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_string,
                    Location location,
                    String *identifier_string)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_IDENTIFIER_STRING;
    self->location = location;
    self->identifier_string = identifier_string;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_byte,
                    Location location,
                    Uint8 literal_byte)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_BYTE;
    self->location = location;
    self->literal_byte = literal_byte;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_bytes,
                    Location location,
                    Uint8 *literal_bytes)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_BYTES;
    self->location = location;
    self->literal_bytes = literal_bytes;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_char,
                    Location location,
                    char literal_char)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_CHAR;
    self->location = location;
    self->literal_char = literal_char;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_cstr,
                    Location location,
                    char *literal_cstr)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_CSTR;
    self->location = location;
    self->literal_cstr = literal_cstr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_float,
                    Location location,
                    String *literal_float)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_FLOAT;
    self->location = location;
    self->literal_float = literal_float;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_2,
                    Location location,
                    String *literal_int_2)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_INT_2;
    self->location = location;
    self->literal_int_2 = literal_int_2;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_8,
                    Location location,
                    String *literal_int_8)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_INT_8;
    self->location = location;
    self->literal_int_8 = literal_int_8;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_10,
                    Location location,
                    String *literal_int_10)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_INT_10;
    self->location = location;
    self->literal_int_10 = literal_int_10;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_16,
                    Location location,
                    String *literal_int_16)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_INT_16;
    self->location = location;
    self->literal_int_16 = literal_int_16;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_str,
                    Location location,
                    String *literal_str)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_STR;
    self->location = location;
    self->literal_str = literal_str;

    return self;
}

VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_float32,
                    Location location,
                    Float32 literal_suffix_float32)
{
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

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
    LilyToken *self = lily_malloc(sizeof(LilyToken));

    self->kind = LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE;
    self->location = location;
    self->literal_suffix_usize = literal_suffix_usize;

    return self;
}

String *
to_string__LilyToken(LilyToken *self)
{
    switch (self->kind) {
        case LILY_TOKEN_KIND_AMPERSAND_EQ:
            return from__String("&=");
        case LILY_TOKEN_KIND_AMPERSAND:
            return from__String("&");
        case LILY_TOKEN_KIND_ARROW:
            return from__String("->");
        case LILY_TOKEN_KIND_AT:
            return from__String("@");
        case LILY_TOKEN_KIND_BACKSLASH:
            return from__String("\\");
        case LILY_TOKEN_KIND_BANG:
            return from__String("!");
        case LILY_TOKEN_KIND_BAR_EQ:
            return from__String("|=");
        case LILY_TOKEN_KIND_BAR_R_SHIFT:
            return from__String("|>");
        case LILY_TOKEN_KIND_BAR:
            return from__String("|");
        case LILY_TOKEN_KIND_COLON_COLON:
            return from__String("::");
        case LILY_TOKEN_KIND_COLON_DOLLAR:
            return from__String(":$");
        case LILY_TOKEN_KIND_COLON_EQ:
            return from__String(":=");
        case LILY_TOKEN_KIND_COLON_R_SHIFT:
            return from__String(":>");
        case LILY_TOKEN_KIND_COLON:
            return from__String(":");
        case LILY_TOKEN_KIND_COMMA:
            return from__String(",");
        case LILY_TOKEN_KIND_COMMENT_BLOCK:
            return from__String("COMMENT(BLOCK)");
#ifdef ENV_DEBUG
        case LILY_TOKEN_KIND_COMMENT_DEBUG:
            return format__String("COMMENT(DEBUG({S}))", self->comment_debug);
#endif
        case LILY_TOKEN_KIND_COMMENT_DOC:
            return format__String("COMMENT(DOC({S}))", self->comment_doc);
        case LILY_TOKEN_KIND_COMMENT_LINE:
            return from__String("COMMENT(LINE)");
        case LILY_TOKEN_KIND_DOLLAR:
            return from__String("$");
        case LILY_TOKEN_KIND_DOT_DOT_DOT:
            return from__String("...");
        case LILY_TOKEN_KIND_DOT_DOT:
            return from__String("..");
        case LILY_TOKEN_KIND_DOT_INTERROGATION:
            return from__String(".?");
        case LILY_TOKEN_KIND_DOT_STAR:
            return from__String(".*");
        case LILY_TOKEN_KIND_DOT:
            return from__String(".");
        case LILY_TOKEN_KIND_EOF:
            return from__String("EOF");
        case LILY_TOKEN_KIND_EQ_EQ:
            return from__String("==");
        case LILY_TOKEN_KIND_EQ:
            return from__String("=");
        case LILY_TOKEN_KIND_EXPAND: {
            String *res = from__String("EXPAND: {");

            for (Usize i = 0; i < self->expand.tokens->len; ++i) {
                String *s =
                  to_string__LilyToken(get__Vec(self->expand.tokens, i));

                APPEND_AND_FREE(res, s);

                if (i + 1 != self->expand.tokens->len) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, " }");

            return res;
        }
        case LILY_TOKEN_KIND_FAT_ARROW:
            return from__String("=>");
        case LILY_TOKEN_KIND_HASHTAG:
            return from__String("#");
        case LILY_TOKEN_KIND_HAT_EQ:
            return from__String("^=");
        case LILY_TOKEN_KIND_HAT:
            return from__String("^");
        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            return format__String("${S}", self->identifier_dollar);
        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
            return format__String("{{|{S}|}", self->identifier_macro);
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            return format__String("{S}", self->identifier_normal);
        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            return format__String("`{S}`", self->identifier_operator);
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            return format__String("@\"{S}\"", self->identifier_string);
        case LILY_TOKEN_KIND_INTERROGATION:
            return from__String("?");
        case LILY_TOKEN_KIND_INVERSE_ARROW:
            return from__String("<-");
        case LILY_TOKEN_KIND_KEYWORD_ALIAS:
            return from__String("alias");
        case LILY_TOKEN_KIND_KEYWORD_AND:
            return from__String("and");
        case LILY_TOKEN_KIND_KEYWORD_AS:
            return from__String("as");
        case LILY_TOKEN_KIND_KEYWORD_ASM:
            return from__String("asm");
        case LILY_TOKEN_KIND_KEYWORD_ASYNC:
            return from__String("async");
        case LILY_TOKEN_KIND_KEYWORD_AT_BUILTIN:
            return from__String("@builtin");
        case LILY_TOKEN_KIND_KEYWORD_AT_CC:
            return from__String("@cc");
        case LILY_TOKEN_KIND_KEYWORD_AT_CPP:
            return from__String("@cpp");
        case LILY_TOKEN_KIND_KEYWORD_AT_HIDE:
            return from__String("@hide");
        case LILY_TOKEN_KIND_KEYWORD_AT_HIDEOUT:
            return from__String("@hideout");
        case LILY_TOKEN_KIND_KEYWORD_AT_LEN:
            return from__String("@len");
        case LILY_TOKEN_KIND_KEYWORD_AT_SYS:
            return from__String("@sys");
        case LILY_TOKEN_KIND_KEYWORD_AWAIT:
            return from__String("await");
        case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            return from__String("begin");
        case LILY_TOKEN_KIND_KEYWORD_BREAK:
            return from__String("break");
        case LILY_TOKEN_KIND_KEYWORD_CAST:
            return from__String("cast");
        case LILY_TOKEN_KIND_KEYWORD_CATCH:
            return from__String("catch");
        case LILY_TOKEN_KIND_KEYWORD_CLASS:
            return from__String("class");
        case LILY_TOKEN_KIND_KEYWORD_CLOSE:
            return from__String("close");
        case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
            return from__String("comptime");
        case LILY_TOKEN_KIND_KEYWORD_DEFER:
            return from__String("defer");
        case LILY_TOKEN_KIND_KEYWORD_DO:
            return from__String("do");
        case LILY_TOKEN_KIND_KEYWORD_DROP:
            return from__String("drop");
        case LILY_TOKEN_KIND_KEYWORD_ELIF:
            return from__String("elif");
        case LILY_TOKEN_KIND_KEYWORD_ELSE:
            return from__String("else");
        case LILY_TOKEN_KIND_KEYWORD_END:
            return from__String("end");
        case LILY_TOKEN_KIND_KEYWORD_ENUM:
            return from__String("enum");
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
            return from__String("error");
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
            return from__String("false");
        case LILY_TOKEN_KIND_KEYWORD_FOR:
            return from__String("for");
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            return from__String("fun");
        case LILY_TOKEN_KIND_KEYWORD_GET:
            return from__String("get");
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            return from__String("global");
        case LILY_TOKEN_KIND_KEYWORD_IF:
            return from__String("if");
        case LILY_TOKEN_KIND_KEYWORD_IMPL:
            return from__String("impl");
        case LILY_TOKEN_KIND_KEYWORD_IMPORT:
            return from__String("import");
        case LILY_TOKEN_KIND_KEYWORD_IN:
            return from__String("in");
        case LILY_TOKEN_KIND_KEYWORD_INCLUDE:
            return from__String("include");
        case LILY_TOKEN_KIND_KEYWORD_INHERIT:
            return from__String("inherit");
        case LILY_TOKEN_KIND_KEYWORD_IS:
            return from__String("is");
        case LILY_TOKEN_KIND_KEYWORD_LIB:
            return from__String("lib");
        case LILY_TOKEN_KIND_KEYWORD_MACRO:
            return from__String("macro");
        case LILY_TOKEN_KIND_KEYWORD_MATCH:
            return from__String("match");
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
            return from__String("module");
        case LILY_TOKEN_KIND_KEYWORD_MUT:
            return from__String("mut");
        case LILY_TOKEN_KIND_KEYWORD_NEXT:
            return from__String("next");
        case LILY_TOKEN_KIND_KEYWORD_NIL:
            return from__String("nil");
        case LILY_TOKEN_KIND_KEYWORD_NONE:
            return from__String("none");
        case LILY_TOKEN_KIND_KEYWORD_NOT:
            return from__String("not");
        case LILY_TOKEN_KIND_KEYWORD_object:
            return from__String("object");
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
            return from__String("Object");
        case LILY_TOKEN_KIND_KEYWORD_OR:
            return from__String("or");
        case LILY_TOKEN_KIND_KEYWORD_PACKAGE:
            return from__String("package");
        case LILY_TOKEN_KIND_KEYWORD_PUB:
            return from__String("pub");
        case LILY_TOKEN_KIND_KEYWORD_RAISE:
            return from__String("raise");
        case LILY_TOKEN_KIND_KEYWORD_RECORD:
            return from__String("record");
        case LILY_TOKEN_KIND_KEYWORD_REF:
            return from__String("ref");
        case LILY_TOKEN_KIND_KEYWORD_REQ:
            return from__String("req");
        case LILY_TOKEN_KIND_KEYWORD_RETURN:
            return from__String("return");
        case LILY_TOKEN_KIND_KEYWORD_self:
            return from__String("self");
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            return from__String("Self");
        case LILY_TOKEN_KIND_KEYWORD_SET:
            return from__String("set");
        case LILY_TOKEN_KIND_KEYWORD_TEST:
            return from__String("test");
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
            return from__String("trace");
        case LILY_TOKEN_KIND_KEYWORD_TRAIT:
            return from__String("trait");
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
            return from__String("true");
        case LILY_TOKEN_KIND_KEYWORD_TRY:
            return from__String("try");
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
            return from__String("type");
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
            return from__String("undef");
        case LILY_TOKEN_KIND_KEYWORD_UNSAFE:
            return from__String("unsafe");
        case LILY_TOKEN_KIND_KEYWORD_USE:
            return from__String("use");
        case LILY_TOKEN_KIND_KEYWORD_VAL:
            return from__String("val");
        case LILY_TOKEN_KIND_KEYWORD_WHEN:
            return from__String("when");
        case LILY_TOKEN_KIND_KEYWORD_WHILE:
            return from__String("while");
        case LILY_TOKEN_KIND_KEYWORD_XOR:
            return from__String("xor");
        case LILY_TOKEN_KIND_L_BRACE:
            return from__String("{");
        case LILY_TOKEN_KIND_L_HOOK:
            return from__String("[");
        case LILY_TOKEN_KIND_L_PAREN:
            return from__String("(");
        case LILY_TOKEN_KIND_L_SHIFT_EQ:
            return from__String("<=");
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ:
            return from__String("<<=");
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT:
            return from__String("<<");
        case LILY_TOKEN_KIND_L_SHIFT:
            return from__String("<");
        case LILY_TOKEN_KIND_LITERAL_BYTE:
            return format__String("b\'{c}\'", self->literal_byte);
        case LILY_TOKEN_KIND_LITERAL_BYTES:
            return format__String("b\"{s}\"", (char *)self->literal_bytes);
        case LILY_TOKEN_KIND_LITERAL_CHAR:
            return format__String("'{c}'", self->literal_char);
        case LILY_TOKEN_KIND_LITERAL_CSTR:
            return format__String("{s}", self->literal_cstr);
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            return format__String("{S}", self->literal_float);
        case LILY_TOKEN_KIND_LITERAL_INT_2:
            return format__String("0b{S}", self->literal_int_2);
        case LILY_TOKEN_KIND_LITERAL_INT_8:
            return format__String("0o{S}", self->literal_int_8);
        case LILY_TOKEN_KIND_LITERAL_INT_10:
            return format__String("{S}", self->literal_int_10);
        case LILY_TOKEN_KIND_LITERAL_INT_16:
            return format__String("0x{S}", self->literal_int_16);
        case LILY_TOKEN_KIND_LITERAL_STR:
            return format__String("\"{S}\"", self->literal_str);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            return format__String("{f}F32", self->literal_suffix_float32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            return format__String("{f}F64", self->literal_suffix_float64);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            return format__String("{d}I16", self->literal_suffix_int16);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            return format__String("{d}I32", self->literal_suffix_int32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            return format__String("{d}I64", self->literal_suffix_int64);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            return format__String("{d}I8", self->literal_suffix_int8);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            return format__String("{d}Iz", self->literal_suffix_isize);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            return format__String("{d}U16", self->literal_suffix_uint16);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            return format__String("{d}U32", self->literal_suffix_uint32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            return format__String("{d}U64", self->literal_suffix_uint64);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            return format__String("{d}U8", self->literal_suffix_uint8);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return format__String("{d}Uz", self->literal_suffix_usize);
        case LILY_TOKEN_KIND_MINUS_EQ:
            return from__String("-=");
        case LILY_TOKEN_KIND_MINUS_MINUS_EQ:
            return from__String("--=");
        case LILY_TOKEN_KIND_MINUS_MINUS:
            return from__String("--");
        case LILY_TOKEN_KIND_MINUS:
            return from__String("-");
        case LILY_TOKEN_KIND_NOT_EQ:
            return from__String("not=");
        case LILY_TOKEN_KIND_PERCENTAGE_EQ:
            return from__String("%=");
        case LILY_TOKEN_KIND_PERCENTAGE:
            return from__String("%");
        case LILY_TOKEN_KIND_PLUS_EQ:
            return from__String("+=");
        case LILY_TOKEN_KIND_PLUS_PLUS_EQ:
            return from__String("++=");
        case LILY_TOKEN_KIND_PLUS_PLUS:
            return from__String("++");
        case LILY_TOKEN_KIND_PLUS:
            return from__String("+");
        case LILY_TOKEN_KIND_R_BRACE:
            return from__String("}");
        case LILY_TOKEN_KIND_R_HOOK:
            return from__String("]");
        case LILY_TOKEN_KIND_R_PAREN:
            return from__String(")");
        case LILY_TOKEN_KIND_R_SHIFT_EQ:
            return from__String(">=");
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ:
            return from__String(">>=");
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT:
            return from__String(">>");
        case LILY_TOKEN_KIND_R_SHIFT:
            return from__String(">");
        case LILY_TOKEN_KIND_SEMICOLON:
            return from__String(";");
        case LILY_TOKEN_KIND_SLASH_EQ:
            return from__String("/=");
        case LILY_TOKEN_KIND_SLASH:
            return from__String("/");
        case LILY_TOKEN_KIND_STAR_EQ:
            return from__String("*=");
        case LILY_TOKEN_KIND_STAR_STAR_EQ:
            return from__String("**=");
        case LILY_TOKEN_KIND_STAR_STAR:
            return from__String("**");
        case LILY_TOKEN_KIND_STAR:
            return from__String("*");
        case LILY_TOKEN_KIND_WAVE_EQ:
            return from__String("~=");
        case LILY_TOKEN_KIND_WAVE:
            return from__String("~");
        case LILY_TOKEN_KIND_XOR_EQ:
            return from__String("xor=");
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyTokenKind, enum LilyTokenKind self)
{
    switch (self) {
        case LILY_TOKEN_KIND_AMPERSAND_EQ:
            return "LILY_TOKEN_KIND_AMPERSAND_EQ";
        case LILY_TOKEN_KIND_AMPERSAND:
            return "LILY_TOKEN_KIND_AMPERSAND";
        case LILY_TOKEN_KIND_ARROW:
            return "LILY_TOKEN_KIND_ARROW";
        case LILY_TOKEN_KIND_AT:
            return "LILY_TOKEN_KIND_AT";
        case LILY_TOKEN_KIND_BACKSLASH:
            return "LILY_TOKEN_KIND_BACKSLASH";
        case LILY_TOKEN_KIND_BANG:
            return "LILY_TOKEN_KIND_BANG";
        case LILY_TOKEN_KIND_BAR_EQ:
            return "LILY_TOKEN_KIND_BAR_EQ";
        case LILY_TOKEN_KIND_BAR_R_SHIFT:
            return "LILY_TOKEN_KIND_BAR_R_SHIFT";
        case LILY_TOKEN_KIND_BAR:
            return "LILY_TOKEN_KIND_BAR";
        case LILY_TOKEN_KIND_COLON_COLON:
            return "LILY_TOKEN_KIND_COLON_COLON";
        case LILY_TOKEN_KIND_COLON_DOLLAR:
            return "LILY_TOKEN_KIND_COLON_DOLLAR";
        case LILY_TOKEN_KIND_COLON_EQ:
            return "LILY_TOKEN_KIND_COLON_EQ";
        case LILY_TOKEN_KIND_COLON_R_SHIFT:
            return "LILY_TOKEN_KIND_COLON_R_SHIFT";
        case LILY_TOKEN_KIND_COLON:
            return "LILY_TOKEN_KIND_COLON";
        case LILY_TOKEN_KIND_COMMA:
            return "LILY_TOKEN_KIND_COMMA";
        case LILY_TOKEN_KIND_COMMENT_BLOCK:
            return "LILY_TOKEN_KIND_COMMENT_BLOCK";
        case LILY_TOKEN_KIND_COMMENT_DEBUG:
            return "LILY_TOKEN_KIND_COMMENT_DEBUG";
        case LILY_TOKEN_KIND_COMMENT_DOC:
            return "LILY_TOKEN_KIND_COMMENT_DOC";
        case LILY_TOKEN_KIND_COMMENT_LINE:
            return "LILY_TOKEN_KIND_COMMENT_LINE";
        case LILY_TOKEN_KIND_DOLLAR:
            return "LILY_TOKEN_KIND_DOLLAR";
        case LILY_TOKEN_KIND_DOT_DOT_DOT:
            return "LILY_TOKEN_KIND_DOT_DOT_DOT";
        case LILY_TOKEN_KIND_DOT_DOT:
            return "LILY_TOKEN_KIND_DOT_DOT";
        case LILY_TOKEN_KIND_DOT_INTERROGATION:
            return "LILY_TOKEN_KIND_DOT_INTERROGATION";
        case LILY_TOKEN_KIND_DOT_STAR:
            return "LILY_TOKEN_KIND_DOT_STAR";
        case LILY_TOKEN_KIND_DOT:
            return "LILY_TOKEN_KIND_DOT";
        case LILY_TOKEN_KIND_EOF:
            return "LILY_TOKEN_KIND_EOF";
        case LILY_TOKEN_KIND_EQ_EQ:
            return "LILY_TOKEN_KIND_EQ_EQ";
        case LILY_TOKEN_KIND_EQ:
            return "LILY_TOKEN_KIND_EQ";
        case LILY_TOKEN_KIND_EXPAND:
            return "LILY_TOKEN_KIND_EXPAND";
        case LILY_TOKEN_KIND_FAT_ARROW:
            return "LILY_TOKEN_KIND_FAT_ARROW";
        case LILY_TOKEN_KIND_HASHTAG:
            return "LILY_TOKEN_KIND_HASHTAG";
        case LILY_TOKEN_KIND_HAT_EQ:
            return "LILY_TOKEN_KIND_HAT_EQ";
        case LILY_TOKEN_KIND_HAT:
            return "LILY_TOKEN_KIND_HAT";
        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            return "LILY_TOKEN_KIND_IDENTIFIER_DOLLAR";
        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
            return "LILY_TOKEN_KIND_IDENTIFIER_MACRO";
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            return "LILY_TOKEN_KIND_IDENTIFIER_NORMAL";
        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            return "LILY_TOKEN_KIND_IDENTIFIER_OPERATOR";
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            return "LILY_TOKEN_KIND_IDENTIFIER_STRING";
        case LILY_TOKEN_KIND_INTERROGATION:
            return "LILY_TOKEN_KIND_INTERROGATION";
        case LILY_TOKEN_KIND_INVERSE_ARROW:
            return "LILY_TOKEN_KIND_INVERSE_ARROW";
        case LILY_TOKEN_KIND_KEYWORD_ALIAS:
            return "LILY_TOKEN_KIND_KEYWORD_ALIAS";
        case LILY_TOKEN_KIND_KEYWORD_AND:
            return "LILY_TOKEN_KIND_KEYWORD_AND";
        case LILY_TOKEN_KIND_KEYWORD_AS:
            return "LILY_TOKEN_KIND_KEYWORD_AS";
        case LILY_TOKEN_KIND_KEYWORD_ASM:
            return "LILY_TOKEN_KIND_KEYWORD_ASM";
        case LILY_TOKEN_KIND_KEYWORD_ASYNC:
            return "LILY_TOKEN_KIND_KEYWORD_ASYNC";
        case LILY_TOKEN_KIND_KEYWORD_AT_BUILTIN:
            return "LILY_TOKEN_KIND_KEYWORD_AT_BUILTIN";
        case LILY_TOKEN_KIND_KEYWORD_AT_CC:
            return "LILY_TOKEN_KIND_KEYWORD_AT_CC";
        case LILY_TOKEN_KIND_KEYWORD_AT_CPP:
            return "LILY_TOKEN_KIND_KEYWORD_AT_CPP";
        case LILY_TOKEN_KIND_KEYWORD_AT_HIDE:
            return "LILY_TOKEN_KIND_KEYWORD_AT_HIDE";
        case LILY_TOKEN_KIND_KEYWORD_AT_HIDEOUT:
            return "LILY_TOKEN_KIND_KEYWORD_AT_HIDEOUT";
        case LILY_TOKEN_KIND_KEYWORD_AT_LEN:
            return "LILY_TOKEN_KIND_KEYWORD_AT_LEN";
        case LILY_TOKEN_KIND_KEYWORD_AT_SYS:
            return "LILY_TOKEN_KIND_KEYWORD_AT_SYS";
        case LILY_TOKEN_KIND_KEYWORD_AWAIT:
            return "LILY_TOKEN_KIND_KEYWORD_AWAIT";
        case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            return "LILY_TOKEN_KIND_KEYWORD_BEGIN";
        case LILY_TOKEN_KIND_KEYWORD_BREAK:
            return "LILY_TOKEN_KIND_KEYWORD_BREAK";
        case LILY_TOKEN_KIND_KEYWORD_CAST:
            return "LILY_TOKEN_KIND_KEYWORD_CAST";
        case LILY_TOKEN_KIND_KEYWORD_CATCH:
            return "LILY_TOKEN_KIND_KEYWORD_CATCH";
        case LILY_TOKEN_KIND_KEYWORD_CLASS:
            return "LILY_TOKEN_KIND_KEYWORD_CLASS";
        case LILY_TOKEN_KIND_KEYWORD_CLOSE:
            return "LILY_TOKEN_KIND_KEYWORD_CLOSE";
        case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
            return "LILY_TOKEN_KIND_KEYWORD_COMPTIME";
        case LILY_TOKEN_KIND_KEYWORD_DEFER:
            return "LILY_TOKEN_KIND_KEYWORD_DEFER";
        case LILY_TOKEN_KIND_KEYWORD_DO:
            return "LILY_TOKEN_KIND_KEYWORD_DO";
        case LILY_TOKEN_KIND_KEYWORD_DROP:
            return "LILY_TOKEN_KIND_KEYWORD_DROP";
        case LILY_TOKEN_KIND_KEYWORD_ELIF:
            return "LILY_TOKEN_KIND_KEYWORD_ELIF";
        case LILY_TOKEN_KIND_KEYWORD_ELSE:
            return "LILY_TOKEN_KIND_KEYWORD_ELSE";
        case LILY_TOKEN_KIND_KEYWORD_END:
            return "LILY_TOKEN_KIND_KEYWORD_END";
        case LILY_TOKEN_KIND_KEYWORD_ENUM:
            return "LILY_TOKEN_KIND_KEYWORD_ENUM";
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
            return "LILY_TOKEN_KIND_KEYWORD_ERROR";
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
            return "LILY_TOKEN_KIND_KEYWORD_FALSE";
        case LILY_TOKEN_KIND_KEYWORD_FOR:
            return "LILY_TOKEN_KIND_KEYWORD_FOR";
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            return "LILY_TOKEN_KIND_KEYWORD_FUN";
        case LILY_TOKEN_KIND_KEYWORD_GET:
            return "LILY_TOKEN_KIND_KEYWORD_GET";
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            return "LILY_TOKEN_KIND_KEYWORD_GLOBAL";
        case LILY_TOKEN_KIND_KEYWORD_IF:
            return "LILY_TOKEN_KIND_KEYWORD_IF";
        case LILY_TOKEN_KIND_KEYWORD_IMPL:
            return "LILY_TOKEN_KIND_KEYWORD_IMPL";
        case LILY_TOKEN_KIND_KEYWORD_IMPORT:
            return "LILY_TOKEN_KIND_KEYWORD_IMPORT";
        case LILY_TOKEN_KIND_KEYWORD_IN:
            return "LILY_TOKEN_KIND_KEYWORD_IN";
        case LILY_TOKEN_KIND_KEYWORD_INCLUDE:
            return "LILY_TOKEN_KIND_KEYWORD_INCLUDE";
        case LILY_TOKEN_KIND_KEYWORD_INHERIT:
            return "LILY_TOKEN_KIND_KEYWORD_INHERIT";
        case LILY_TOKEN_KIND_KEYWORD_IS:
            return "LILY_TOKEN_KIND_KEYWORD_IS";
        case LILY_TOKEN_KIND_KEYWORD_LIB:
            return "LILY_TOKEN_KIND_KEYWORD_LIB";
        case LILY_TOKEN_KIND_KEYWORD_MACRO:
            return "LILY_TOKEN_KIND_KEYWORD_MACRO";
        case LILY_TOKEN_KIND_KEYWORD_MATCH:
            return "LILY_TOKEN_KIND_KEYWORD_MATCH";
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
            return "LILY_TOKEN_KIND_KEYWORD_MODULE";
        case LILY_TOKEN_KIND_KEYWORD_MUT:
            return "LILY_TOKEN_KIND_KEYWORD_MUT";
        case LILY_TOKEN_KIND_KEYWORD_NEXT:
            return "LILY_TOKEN_KIND_KEYWORD_NEXT";
        case LILY_TOKEN_KIND_KEYWORD_NIL:
            return "LILY_TOKEN_KIND_KEYWORD_NIL";
        case LILY_TOKEN_KIND_KEYWORD_NONE:
            return "LILY_TOKEN_KIND_KEYWORD_NONE";
        case LILY_TOKEN_KIND_KEYWORD_NOT:
            return "LILY_TOKEN_KIND_KEYWORD_NOT";
        case LILY_TOKEN_KIND_KEYWORD_object:
            return "LILY_TOKEN_KIND_KEYWORD_object";
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
            return "LILY_TOKEN_KIND_KEYWORD_OBJECT";
        case LILY_TOKEN_KIND_KEYWORD_OR:
            return "LILY_TOKEN_KIND_KEYWORD_OR";
        case LILY_TOKEN_KIND_KEYWORD_PACKAGE:
            return "LILY_TOKEN_KIND_KEYWORD_PACKAGE";
        case LILY_TOKEN_KIND_KEYWORD_PUB:
            return "LILY_TOKEN_KIND_KEYWORD_PUB";
        case LILY_TOKEN_KIND_KEYWORD_RAISE:
            return "LILY_TOKEN_KIND_KEYWORD_RAISE";
        case LILY_TOKEN_KIND_KEYWORD_RECORD:
            return "LILY_TOKEN_KIND_KEYWORD_RECORD";
        case LILY_TOKEN_KIND_KEYWORD_REF:
            return "LILY_TOKEN_KIND_KEYWORD_REF";
        case LILY_TOKEN_KIND_KEYWORD_REQ:
            return "LILY_TOKEN_KIND_KEYWORD_REQ";
        case LILY_TOKEN_KIND_KEYWORD_RETURN:
            return "LILY_TOKEN_KIND_KEYWORD_RETURN";
        case LILY_TOKEN_KIND_KEYWORD_self:
            return "LILY_TOKEN_KIND_KEYWORD_self";
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            return "LILY_TOKEN_KIND_KEYWORD_SELF";
        case LILY_TOKEN_KIND_KEYWORD_SET:
            return "LILY_TOKEN_KIND_KEYWORD_SET";
        case LILY_TOKEN_KIND_KEYWORD_TEST:
            return "LILY_TOKEN_KIND_KEYWORD_TEST";
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
            return "LILY_TOKEN_KIND_KEYWORD_TRACE";
        case LILY_TOKEN_KIND_KEYWORD_TRAIT:
            return "LILY_TOKEN_KIND_KEYWORD_TRAIT";
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
            return "LILY_TOKEN_KIND_KEYWORD_TRUE";
        case LILY_TOKEN_KIND_KEYWORD_TRY:
            return "LILY_TOKEN_KIND_KEYWORD_TRY";
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
            return "LILY_TOKEN_KIND_KEYWORD_TYPE";
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
            return "LILY_TOKEN_KIND_KEYWORD_UNDEF";
        case LILY_TOKEN_KIND_KEYWORD_UNSAFE:
            return "LILY_TOKEN_KIND_KEYWORD_UNSAFE";
        case LILY_TOKEN_KIND_KEYWORD_USE:
            return "LILY_TOKEN_KIND_KEYWORD_USE";
        case LILY_TOKEN_KIND_KEYWORD_VAL:
            return "LILY_TOKEN_KIND_KEYWORD_VAL";
        case LILY_TOKEN_KIND_KEYWORD_WHEN:
            return "LILY_TOKEN_KIND_KEYWORD_WHEN";
        case LILY_TOKEN_KIND_KEYWORD_WHILE:
            return "LILY_TOKEN_KIND_KEYWORD_WHILE";
        case LILY_TOKEN_KIND_KEYWORD_XOR:
            return "LILY_TOKEN_KIND_KEYWORD_XOR";
        case LILY_TOKEN_KIND_L_BRACE:
            return "LILY_TOKEN_KIND_L_BRACE";
        case LILY_TOKEN_KIND_L_HOOK:
            return "LILY_TOKEN_KIND_L_HOOK";
        case LILY_TOKEN_KIND_L_PAREN:
            return "LILY_TOKEN_KIND_L_PAREN";
        case LILY_TOKEN_KIND_L_SHIFT_EQ:
            return "LILY_TOKEN_KIND_L_SHIFT_EQ";
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ:
            return "LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ";
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT:
            return "LILY_TOKEN_KIND_L_SHIFT_L_SHIFT";
        case LILY_TOKEN_KIND_L_SHIFT:
            return "LILY_TOKEN_KIND_L_SHIFT";
        case LILY_TOKEN_KIND_LITERAL_BYTE:
            return "LILY_TOKEN_KIND_LITERAL_BYTE";
        case LILY_TOKEN_KIND_LITERAL_BYTES:
            return "LILY_TOKEN_KIND_LITERAL_BYTES";
        case LILY_TOKEN_KIND_LITERAL_CHAR:
            return "LILY_TOKEN_KIND_LITERAL_CHAR";
        case LILY_TOKEN_KIND_LITERAL_CSTR:
            return "LILY_TOKEN_KIND_LITERAL_CSTR";
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            return "LILY_TOKEN_KIND_LITERAL_FLOAT";
        case LILY_TOKEN_KIND_LITERAL_INT_2:
            return "LILY_TOKEN_KIND_LITERAL_INT_2";
        case LILY_TOKEN_KIND_LITERAL_INT_8:
            return "LILY_TOKEN_KIND_LITERAL_INT_8";
        case LILY_TOKEN_KIND_LITERAL_INT_10:
            return "LILY_TOKEN_KIND_LITERAL_INT_10";
        case LILY_TOKEN_KIND_LITERAL_INT_16:
            return "LILY_TOKEN_KIND_LITERAL_INT_16";
        case LILY_TOKEN_KIND_LITERAL_STR:
            return "LILY_TOKEN_KIND_LITERAL_STR";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8";
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return "LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE";
        case LILY_TOKEN_KIND_MINUS_EQ:
            return "LILY_TOKEN_KIND_MINUS_EQ";
        case LILY_TOKEN_KIND_MINUS_MINUS_EQ:
            return "LILY_TOKEN_KIND_MINUS_MINUS_EQ";
        case LILY_TOKEN_KIND_MINUS_MINUS:
            return "LILY_TOKEN_KIND_MINUS_MINUS";
        case LILY_TOKEN_KIND_MINUS:
            return "LILY_TOKEN_KIND_MINUS";
        case LILY_TOKEN_KIND_NOT_EQ:
            return "LILY_TOKEN_KIND_NOT_EQ";
        case LILY_TOKEN_KIND_PERCENTAGE_EQ:
            return "LILY_TOKEN_KIND_PERCENTAGE_EQ";
        case LILY_TOKEN_KIND_PERCENTAGE:
            return "LILY_TOKEN_KIND_PERCENTAGE";
        case LILY_TOKEN_KIND_PLUS_EQ:
            return "LILY_TOKEN_KIND_PLUS_EQ";
        case LILY_TOKEN_KIND_PLUS_PLUS_EQ:
            return "LILY_TOKEN_KIND_PLUS_PLUS_EQ";
        case LILY_TOKEN_KIND_PLUS_PLUS:
            return "LILY_TOKEN_KIND_PLUS_PLUS";
        case LILY_TOKEN_KIND_PLUS:
            return "LILY_TOKEN_KIND_PLUS";
        case LILY_TOKEN_KIND_R_BRACE:
            return "LILY_TOKEN_KIND_R_BRACE";
        case LILY_TOKEN_KIND_R_HOOK:
            return "LILY_TOKEN_KIND_R_HOOK";
        case LILY_TOKEN_KIND_R_PAREN:
            return "LILY_TOKEN_KIND_R_PAREN";
        case LILY_TOKEN_KIND_R_SHIFT_EQ:
            return "LILY_TOKEN_KIND_R_SHIFT_EQ";
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ:
            return "LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ";
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT:
            return "LILY_TOKEN_KIND_R_SHIFT_R_SHIFT";
        case LILY_TOKEN_KIND_R_SHIFT:
            return "LILY_TOKEN_KIND_R_SHIFT";
        case LILY_TOKEN_KIND_SEMICOLON:
            return "LILY_TOKEN_KIND_SEMICOLON";
        case LILY_TOKEN_KIND_SLASH_EQ:
            return "LILY_TOKEN_KIND_SLASH_EQ";
        case LILY_TOKEN_KIND_SLASH:
            return "LILY_TOKEN_KIND_SLASH";
        case LILY_TOKEN_KIND_STAR_EQ:
            return "LILY_TOKEN_KIND_STAR_EQ";
        case LILY_TOKEN_KIND_STAR_STAR_EQ:
            return "LILY_TOKEN_KIND_STAR_STAR_EQ";
        case LILY_TOKEN_KIND_STAR_STAR:
            return "LILY_TOKEN_KIND_STAR_STAR";
        case LILY_TOKEN_KIND_STAR:
            return "LILY_TOKEN_KIND_STAR";
        case LILY_TOKEN_KIND_WAVE_EQ:
            return "LILY_TOKEN_KIND_WAVE_EQ";
        case LILY_TOKEN_KIND_WAVE:
            return "LILY_TOKEN_KIND_WAVE";
        case LILY_TOKEN_KIND_XOR_EQ:
            return "LILY_TOKEN_KIND_XOR_EQ";
        default:
            UNREACHABLE("unknown variant");
    }
}

char *
IMPL_FOR_DEBUG(to_string, LilyToken, const LilyToken *self)
{
    switch (self->kind) {
        case LILY_TOKEN_KIND_COMMENT_DEBUG:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, comment_debug = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->comment_debug);

        case LILY_TOKEN_KIND_COMMENT_DOC:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, comment_doc = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->comment_doc);

        case LILY_TOKEN_KIND_EXPAND:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, expand = {Sr} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              CALL_DEBUG_IMPL(to_string, LilyTokenExpand, &self->expand));

        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "identifier_dollar = {S} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->identifier_dollar);

        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "identifier_macro = {S} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->identifier_macro);

        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "identifier_normal = {S} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->identifier_normal);

        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "identifier_operator = {S} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->identifier_operator);

        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "identifier_string = {S} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->identifier_string);

        case LILY_TOKEN_KIND_LITERAL_BYTE:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_byte = b'{c}' }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_byte);

        case LILY_TOKEN_KIND_LITERAL_BYTES:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_bytes = b\"{s}\" }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_bytes);

        case LILY_TOKEN_KIND_LITERAL_CHAR:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_float = '{c}' }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_char);

        case LILY_TOKEN_KIND_LITERAL_CSTR:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_cstr = {s} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_cstr);

        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_float = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_float);

        case LILY_TOKEN_KIND_LITERAL_INT_2:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_int_2 = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_int_2);

        case LILY_TOKEN_KIND_LITERAL_INT_8:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_int_8 = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_int_8);

        case LILY_TOKEN_KIND_LITERAL_INT_10:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_int_10 = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_int_10);

        case LILY_TOKEN_KIND_LITERAL_INT_16:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_int_16 = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_int_16);

        case LILY_TOKEN_KIND_LITERAL_STR:
            return format(
              "LilyToken{{ kind = {s}, location = {sa}, literal_str = {S} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location),
              self->literal_str);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_float32 = {f} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_float32);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_float64 = {f} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_float64);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_int16 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_int16);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_int32 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_int32);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_int64 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_int64);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_int8 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_int8);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_isize = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_isize);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_uint16 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_uint16);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_uint32 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_uint32);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_uint64 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_uint64);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_uint8 = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_uint8);

        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return format("LilyToken{{ kind = {s}, location = {sa}, "
                          "literal_suffix_usize = {d} }",
                          CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
                          CALL_DEBUG_IMPL(to_string, Location, &self->location),
                          self->literal_suffix_usize);

        default:
            return format(
              "LilyToken{{ kind = {s}, location = {sa} }",
              CALL_DEBUG_IMPL(to_string, LilyTokenKind, self->kind),
              CALL_DEBUG_IMPL(to_string, Location, &self->location));
    }
}

void
IMPL_FOR_DEBUG(debug, LilyToken, const LilyToken *self)
{
    PRINTLN("{sa}", CALL_DEBUG_IMPL(to_string, LilyToken, self));
}
#endif

LilyToken *
clone__LilyToken(const LilyToken *self)
{
    switch (self->kind) {
#ifdef ENV_DEBUG
        case LILY_TOKEN_KIND_COMMENT_DEBUG:
            return NEW_VARIANT(LilyToken,
                               comment_debug,
                               self->location,
                               clone__String(self->comment_debug));
#endif
        case LILY_TOKEN_KIND_COMMENT_DOC:
            return NEW_VARIANT(LilyToken,
                               comment_doc,
                               self->location,
                               clone__String(self->comment_doc));
        case LILY_TOKEN_KIND_EXPAND:
            return NEW_VARIANT(
              LilyToken,
              expand,
              self->location,
              NEW(LilyTokenExpand, self->expand.kind, self->expand.tokens));
        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            return NEW_VARIANT(LilyToken,
                               identifier_dollar,
                               self->location,
                               clone__String(self->identifier_dollar));
        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
            return NEW_VARIANT(LilyToken,
                               identifier_macro,
                               self->location,
                               clone__String(self->identifier_macro));
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            return NEW_VARIANT(LilyToken,
                               identifier_normal,
                               self->location,
                               clone__String(self->identifier_normal));
        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            return NEW_VARIANT(LilyToken,
                               identifier_operator,
                               self->location,
                               clone__String(self->identifier_operator));
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            return NEW_VARIANT(LilyToken,
                               identifier_string,
                               self->location,
                               clone__String(self->identifier_string));
        case LILY_TOKEN_KIND_LITERAL_BYTE:
            return NEW_VARIANT(
              LilyToken, literal_byte, self->location, self->literal_byte);
        case LILY_TOKEN_KIND_LITERAL_BYTES: {
            Usize n = strlen((char *)self->literal_bytes) + 1;
            Uint8 *clone = lily_malloc(n);

            memcpy(clone, self->literal_bytes, n);

            return NEW_VARIANT(
              LilyToken, literal_bytes, self->location, self->literal_bytes);
        }
        case LILY_TOKEN_KIND_LITERAL_CHAR:
            return NEW_VARIANT(
              LilyToken, literal_char, self->location, self->literal_char);
        case LILY_TOKEN_KIND_LITERAL_CSTR: {
            Usize size = strlen(self->literal_cstr) + 1;
            char *cstr_copy = malloc(size);

            memcpy(cstr_copy, self->literal_cstr, size);

            return NEW_VARIANT(
              LilyToken, literal_cstr, self->location, cstr_copy);
        }
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            return NEW_VARIANT(LilyToken,
                               literal_float,
                               self->location,
                               clone__String(self->literal_float));
        case LILY_TOKEN_KIND_LITERAL_INT_2:
            return NEW_VARIANT(LilyToken,
                               literal_int_2,
                               self->location,
                               clone__String(self->literal_int_2));
        case LILY_TOKEN_KIND_LITERAL_INT_8:
            return NEW_VARIANT(LilyToken,
                               literal_int_8,
                               self->location,
                               clone__String(self->literal_int_8));
        case LILY_TOKEN_KIND_LITERAL_INT_10:
            return NEW_VARIANT(LilyToken,
                               literal_int_10,
                               self->location,
                               clone__String(self->literal_int_10));
        case LILY_TOKEN_KIND_LITERAL_INT_16:
            return NEW_VARIANT(LilyToken,
                               literal_int_16,
                               self->location,
                               clone__String(self->literal_int_16));
        case LILY_TOKEN_KIND_LITERAL_STR:
            return NEW_VARIANT(LilyToken,
                               literal_str,
                               self->location,
                               clone__String(self->literal_str));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_float32,
                               self->location,
                               self->literal_suffix_float32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_float32,
                               self->location,
                               self->literal_suffix_float32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_int16,
                               self->location,
                               self->literal_suffix_int16);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_int32,
                               self->location,
                               self->literal_suffix_int32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_int64,
                               self->location,
                               self->literal_suffix_int64);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_int8,
                               self->location,
                               self->literal_suffix_int8);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_isize,
                               self->location,
                               self->literal_suffix_isize);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_uint16,
                               self->location,
                               self->literal_suffix_uint16);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_uint32,
                               self->location,
                               self->literal_suffix_uint32);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_uint64,
                               self->location,
                               self->literal_suffix_uint64);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_uint8,
                               self->location,
                               self->literal_suffix_uint8);
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return NEW_VARIANT(LilyToken,
                               literal_suffix_usize,
                               self->location,
                               self->literal_suffix_usize);
        default:
            return NEW(LilyToken, self->kind, self->location);
    }
}

VARIANT_DESTRUCTOR(LilyToken, comment_doc, LilyToken *self)
{
    FREE(String, self->comment_doc);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_dollar, LilyToken *self)
{
    FREE(String, self->identifier_dollar);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_macro, LilyToken *self)
{
    FREE(String, self->identifier_macro);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_operator, LilyToken *self)
{
    FREE(String, self->identifier_operator);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_normal, LilyToken *self)
{
    FREE(String, self->identifier_normal);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, identifier_string, LilyToken *self)
{
    FREE(String, self->identifier_string);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_bytes, LilyToken *self)
{
    lily_free(self->literal_bytes);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_cstr, LilyToken *self)
{
    lily_free(self->literal_cstr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_float, LilyToken *self)
{
    FREE(String, self->literal_float);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_int_2, LilyToken *self)
{
    FREE(String, self->literal_int_2);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_int_8, LilyToken *self)
{
    FREE(String, self->literal_int_8);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_int_10, LilyToken *self)
{
    FREE(String, self->literal_int_10);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_int_16, LilyToken *self)
{
    FREE(String, self->literal_int_16);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, literal_str, LilyToken *self)
{
    FREE(String, self->literal_str);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyToken, macro_expand, LilyToken *self)
{
    lily_free(self);
}

DESTRUCTOR(LilyToken, LilyToken *self)
{
    switch (self->kind) {
        case LILY_TOKEN_KIND_COMMENT_DOC:
            FREE_VARIANT(LilyToken, comment_doc, self);
            break;
        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            FREE_VARIANT(LilyToken, identifier_dollar, self);
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
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            FREE_VARIANT(LilyToken, identifier_string, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_BYTES:
            FREE_VARIANT(LilyToken, literal_bytes, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_CSTR:
            FREE_VARIANT(LilyToken, literal_cstr, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            FREE_VARIANT(LilyToken, literal_float, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_INT_2:
            FREE_VARIANT(LilyToken, literal_int_2, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_INT_8:
            FREE_VARIANT(LilyToken, literal_int_8, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_INT_10:
            FREE_VARIANT(LilyToken, literal_int_10, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_INT_16:
            FREE_VARIANT(LilyToken, literal_int_16, self);
            break;
        case LILY_TOKEN_KIND_LITERAL_STR:
            FREE_VARIANT(LilyToken, literal_str, self);
            break;
        default:
            lily_free(self);
    }
}
