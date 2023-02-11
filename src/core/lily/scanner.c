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

#include <base/atof.h>
#include <base/atoi.h>

#include <core/lily/error.h>
#include <core/lily/scanner.h>
#include <core/shared/diagnostic.h>

#include <ctype.h>
#include <string.h>

// Get keyword from id.
static enum LilyTokenKind
get_keyword(char *id);

static void
skip_space__Scanner(Scanner *self);

// Next char n times.
static void
jump__Scanner(Scanner *self, Usize n);

// Assign to line and column to start_line and start_column Location's field.
static void
start_token__Scanner(Scanner *self, Usize line, Usize column);

// Assign to line and column to end_line and end_column Location's field.
static void
end_token__Scanner(Scanner *self, Usize line, Usize column);

// Get character at position + n.
static char *
peek_char__Scanner(const Scanner *self, Usize n);

// Next char according the token.
static void
next_char_by_token__Scanner(Scanner *self, LilyToken *token);

// Push token to tokens.
static inline void
push_token__Scanner(Scanner *self, LilyToken *token);

// Check if current is a digit.
static inline bool
is_digit__Scanner(const Scanner *self);

// Check if current can be an identifier.
static inline bool
is_ident__Scanner(const Scanner *self);

// Check if current can be an hexadecimal.
static inline bool
is_hex__Scanner(const Scanner *self);

// Check if current can be an octal.
static inline bool
is_oct__Scanner(const Scanner *self);

// Check if current can be a binary.
static inline bool
is_bin__Scanner(const Scanner *self);

// Check if current can be a number.
static inline bool
is_num__Scanner(const Scanner *self);

// Get escape character and other character.
static String *
get_character__Scanner(Scanner *self, char previous);

static void
skip_comment_line__Scanner(Scanner *self);

static void
skip_comment_block__Scanner(Scanner *self);

static String *
scan_comment_doc__Scanner(Scanner *self);

static String *
scan_identifier__Scanner(Scanner *self);

static char *
scan_char__Scanner(Scanner *self);

static String *
scan_string__Scanner(Scanner *self);

static LilyToken *
scan_hex__Scanner(Scanner *self);

static LilyToken *
scan_oct__Scanner(Scanner *self);

#ifdef PLATFORM_64
#define ISIZE_OUT_OF_RANGE_HELP               \
    "the range of the Isize type is "         \
    "between -9_223_372_036_854_775_808 and " \
    "9_223_372_036_854_775_807"
#define USIZE_OUT_OF_RANGE_HELP       \
    "the range of the Usize type is " \
    "between 0 and "                  \
    "18_446_744_073_709_551_615"
#else
#define ISIZE_OUT_OF_RANGE_HELP       \
    "the range of the Isize type is " \
    "between -2_147_483_648 and 2_147_483_647"
#define USIZE_OUT_OF_RANGE_HELP       \
    "the range of the Usize type is " \
    "between 0 and 4_294_967_295"
#endif

#define SCAN_LITERAL_SUFFIX(value, base, is_int)                               \
    {                                                                          \
        LilyToken *token_res = NULL;                                               \
        end__Location(&location_error,                                         \
                      self->source.cursor.line,                                \
                      self->source.cursor.column);                             \
                                                                               \
        char *c1 = peek_char__Scanner(self, 1);                                \
                                                                               \
        char *c2 = peek_char__Scanner(self, 2);                                \
                                                                               \
        char *c3 = peek_char__Scanner(self, 3);                                \
                                                                               \
        if (c1 == (char *)'I' && c2 == (char *)'8' && is_int) {                \
            if (!CHECK_INT8_OVERFLOW_FROM_STRING(value, base)) {               \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_INT8_OUT_OF_RANGE),         \
                    init__Vec(1,                                               \
                              from__String("the range of the Int8 type is "    \
                                           "between -128 and 127")),           \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_int8,                       \
                                    clone__Location(&self->location),          \
                                    atoi__Int8(value, base));                  \
        } else if (c1 == (char *)'I' && c2 == (char *)'1' &&                   \
                   c3 == (char *)'6' && is_int) {                              \
            if (!CHECK_INT16_OVERFLOW_FROM_STRING(value, base)) {              \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_INT16_OUT_OF_RANGE),        \
                    init__Vec(1,                                               \
                              from__String("the range of the Int16 type is "   \
                                           "between -32_768 and 32_767")),     \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_int16,                      \
                                    clone__Location(&self->location),          \
                                    atoi__Int16(value, base));                 \
        } else if (c1 == (char *)'I' && c2 == (char *)'3' &&                   \
                   c3 == (char *)'2' && is_int) {                              \
            if (!CHECK_INT32_OVERFLOW_FROM_STRING(value, base)) {              \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_INT32_OUT_OF_RANGE),        \
                    init__Vec(1,                                               \
                              from__String(                                    \
                                "the range of the Int32 type is "              \
                                "between -2_147_483_648 and 2_147_483_647")),  \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_int32,                      \
                                    clone__Location(&self->location),          \
                                    atoi__Int32(value, base));                 \
        } else if (c1 == (char *)'I' && c2 == (char *)'6' &&                   \
                   c3 == (char *)'4' && is_int) {                              \
            if (!CHECK_INT64_OVERFLOW_FROM_STRING(value, base)) {              \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_INT64_OUT_OF_RANGE),        \
                    init__Vec(                                                 \
                      1,                                                       \
                      from__String("the range of the Int64 type is "           \
                                   "between -9_223_372_036_854_775_808 and "   \
                                   "9_223_372_036_854_775_807")),              \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_int64,                      \
                                    clone__Location(&self->location),          \
                                    atoi__Int64(value, base));                 \
        } else if (c1 == (char *)'I' && c2 == (char *)'z' && is_int) {         \
            if (!CHECK_ISIZE_OVERFLOW_FROM_STRING(value, base)) {              \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_INT64_OUT_OF_RANGE),        \
                    init__Vec(1, from__String(ISIZE_OUT_OF_RANGE_HELP)),       \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_isize,                      \
                                    clone__Location(&self->location),          \
                                    atoi__Isize(value, base));                 \
        } else if (c1 == (char *)'U' && c2 == (char *)'8' && is_int) {         \
            if (!CHECK_UINT8_OVERFLOW_FROM_STRING(value, base)) {              \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_UINT8_OUT_OF_RANGE),        \
                    init__Vec(1,                                               \
                              from__String("the range of the Uint8 type is "   \
                                           "between 0 and 255")),              \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_uint8,                      \
                                    clone__Location(&self->location),          \
                                    atoi__Uint8(value, base));                 \
        } else if (c1 == (char *)'U' && c2 == (char *)'1' &&                   \
                   c3 == (char *)'6' && is_int) {                              \
            if (!CHECK_UINT16_OVERFLOW_FROM_STRING(value, base)) {             \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_UINT16_OUT_OF_RANGE),       \
                    init__Vec(1,                                               \
                              from__String("the range of the Uint16 type is "  \
                                           "between 0 and 65_535")),           \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_uint16,                     \
                                    clone__Location(&self->location),          \
                                    atoi__Uint16(value, base));                \
        } else if (c1 == (char *)'U' && c2 == (char *)'3' &&                   \
                   c3 == (char *)'2' && is_int) {                              \
            if (!CHECK_UINT32_OVERFLOW_FROM_STRING(value, base)) {             \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_UINT32_OUT_OF_RANGE),       \
                    init__Vec(1,                                               \
                              from__String("the range of the Uint32 type is "  \
                                           "between 0 and 4_294_967_295")),    \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_uint32,                     \
                                    clone__Location(&self->location),          \
                                    atoi__Uint32(value, base));                \
        } else if (c1 == (char *)'U' && c2 == (char *)'6' &&                   \
                   c3 == (char *)'4' && is_int) {                              \
            if (!CHECK_UINT64_OVERFLOW_FROM_STRING(value, base)) {             \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_UINT64_OUT_OF_RANGE),       \
                    init__Vec(1,                                               \
                              from__String(                                    \
                                "the range of the Uint64 type is "             \
                                "between 0 and 18_446_744_073_709_551_615")),  \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_uint64,                     \
                                    clone__Location(&self->location),          \
                                    atoi__Uint64(value, base));                \
        } else if (c1 == (char *)'U' && c2 == (char *)'z' && is_int) {         \
            if (!CHECK_USIZE_OVERFLOW_FROM_STRING(value, base)) {              \
                emit__Diagnostic(                                              \
                  NEW_VARIANT(                                                 \
                    Diagnostic,                                                \
                    simple_lily_error,                                         \
                    self->source.file,                                         \
                    &location_error,                                           \
                    NEW(LilyError, LILY_ERROR_KIND_USIZE_OUT_OF_RANGE),        \
                    init__Vec(1, from__String(USIZE_OUT_OF_RANGE_HELP)),       \
                    NULL,                                                      \
                    NULL),                                                     \
                  &self->count_error);                                         \
                FREE(String, res);                                             \
                return NULL;                                                   \
            }                                                                  \
                                                                               \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_usize,                      \
                                    clone__Location(&self->location),          \
                                    atoi__Usize(value, base));                 \
        } else if (c1 == (char *)'F' && c2 == (char *)'3' &&                   \
                   c3 == (char *)'2') {                                        \
            /*                                                                 \
            TODO: Check if the float is overflow/underflow.                    \
            */                                                                 \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_float32,                    \
                                    clone__Location(&self->location),          \
                                    atof__Float32(value));                     \
        } else if (c1 == (char *)'F' && c2 == (char *)'6' &&                   \
                   c3 == (char *)'4') {                                        \
            /*                                                                 \
            TODO: Check if the float is overflow/underflow.                    \
            */                                                                 \
            token_res = NEW_VARIANT(LilyToken,                                 \
                                    literal_suffix_float64,                    \
                                    clone__Location(&self->location),          \
                                    atof__Float64(value));                     \
        } else if (c1 == (char *)'I' || c1 == (char *)'U' ||                   \
                   c1 == (char *)'F') {                                        \
            emit__Diagnostic(                                                  \
              NEW_VARIANT(                                                     \
                Diagnostic,                                                    \
                simple_lily_error,                                             \
                self->source.file,                                             \
                &location_error,                                               \
                NEW(LilyError, LILY_ERROR_KIND_INVALID_LITERAL_SUFFIX),        \
                init__Vec(1,                                                   \
                          from__String(                                        \
                            "here is the valid literal suffix: I8, I16, I32, " \
                            "I64, U8, U16, U32, U64, F32, F64, Uz, Iz")),      \
                init__Vec(1,                                                   \
                          from__String("it is not possible to use an integer " \
                                       "suffix on a float literal")),          \
                NULL),                                                         \
              &self->count_error);                                             \
            FREE(String, res);                                                 \
            return NULL;                                                       \
        }                                                                      \
\
        if (token_res) { \
            FREE(String, res); \
            return token_res; \
        } \
    }

enum LilyTokenKind
get_keyword(char *id)
{
    if (!strcmp(id, "alias"))
        return LILY_TOKEN_KIND_KEYWORD_ALIAS;
    else if (!strcmp(id, "and"))
        return LILY_TOKEN_KIND_KEYWORD_AND;
    else if (!strcmp(id, "as"))
        return LILY_TOKEN_KIND_KEYWORD_AS;
    else if (!strcmp(id, "asm"))
        return LILY_TOKEN_KIND_KEYWORD_ASM;
    else if (!strcmp(id, "async"))
        return LILY_TOKEN_KIND_KEYWORD_ASYNC;
    else if (!strcmp(id, "await"))
        return LILY_TOKEN_KIND_KEYWORD_AWAIT;
    else if (!strcmp(id, "begin"))
        return LILY_TOKEN_KIND_KEYWORD_BEGIN;
    else if (!strcmp(id, "break"))
        return LILY_TOKEN_KIND_KEYWORD_BREAK;
    else if (!strcmp(id, "cast"))
        return LILY_TOKEN_KIND_KEYWORD_CAST;
    else if (!strcmp(id, "catch"))
        return LILY_TOKEN_KIND_KEYWORD_CATCH;
    else if (!strcmp(id, "class"))
        return LILY_TOKEN_KIND_KEYWORD_CLASS;
    else if (!strcmp(id, "comptime"))
        return LILY_TOKEN_KIND_KEYWORD_COMPTIME;
    else if (!strcmp(id, "do"))
        return LILY_TOKEN_KIND_KEYWORD_DO;
    else if (!strcmp(id, "drop"))
        return LILY_TOKEN_KIND_KEYWORD_DROP;
    else if (!strcmp(id, "elif"))
        return LILY_TOKEN_KIND_KEYWORD_ELIF;
    else if (!strcmp(id, "else"))
        return LILY_TOKEN_KIND_KEYWORD_ELSE;
    else if (!strcmp(id, "end"))
        return LILY_TOKEN_KIND_KEYWORD_END;
    else if (!strcmp(id, "enum"))
        return LILY_TOKEN_KIND_KEYWORD_ENUM;
    else if (!strcmp(id, "error"))
        return LILY_TOKEN_KIND_KEYWORD_ERROR;
    else if (!strcmp(id, "false"))
        return LILY_TOKEN_KIND_KEYWORD_FALSE;
    else if (!strcmp(id, "for"))
        return LILY_TOKEN_KIND_KEYWORD_FOR;
    else if (!strcmp(id, "fun"))
        return LILY_TOKEN_KIND_KEYWORD_FUN;
    else if (!strcmp(id, "global"))
        return LILY_TOKEN_KIND_KEYWORD_GLOBAL;
    else if (!strcmp(id, "if"))
        return LILY_TOKEN_KIND_KEYWORD_IF;
    else if (!strcmp(id, "impl"))
        return LILY_TOKEN_KIND_KEYWORD_IMPL;
    else if (!strcmp(id, "import"))
        return LILY_TOKEN_KIND_KEYWORD_IMPORT;
    else if (!strcmp(id, "in"))
        return LILY_TOKEN_KIND_KEYWORD_IN;
    else if (!strcmp(id, "inherit"))
        return LILY_TOKEN_KIND_KEYWORD_INHERIT;
    else if (!strcmp(id, "is"))
        return LILY_TOKEN_KIND_KEYWORD_IS;
    else if (!strcmp(id, "macro"))
        return LILY_TOKEN_KIND_KEYWORD_MACRO;
    else if (!strcmp(id, "match"))
        return LILY_TOKEN_KIND_KEYWORD_MATCH;
    else if (!strcmp(id, "module"))
        return LILY_TOKEN_KIND_KEYWORD_MODULE;
    else if (!strcmp(id, "mut"))
        return LILY_TOKEN_KIND_KEYWORD_MUT;
    else if (!strcmp(id, "next"))
        return LILY_TOKEN_KIND_KEYWORD_NEXT;
    else if (!strcmp(id, "nil"))
        return LILY_TOKEN_KIND_KEYWORD_NIL;
    else if (!strcmp(id, "none"))
        return LILY_TOKEN_KIND_KEYWORD_NONE;
    else if (!strcmp(id, "not"))
        return LILY_TOKEN_KIND_KEYWORD_NOT;
    else if (!strcmp(id, "object"))
        return LILY_TOKEN_KIND_KEYWORD_object;
    else if (!strcmp(id, "Object"))
        return LILY_TOKEN_KIND_KEYWORD_OBJECT;
    else if (!strcmp(id, "or"))
        return LILY_TOKEN_KIND_KEYWORD_OR;
    else if (!strcmp(id, "package"))
        return LILY_TOKEN_KIND_KEYWORD_PACKAGE;
    else if (!strcmp(id, "pub"))
        return LILY_TOKEN_KIND_KEYWORD_PUB;
    else if (!strcmp(id, "raise"))
        return LILY_TOKEN_KIND_KEYWORD_RAISE;
    else if (!strcmp(id, "record"))
        return LILY_TOKEN_KIND_KEYWORD_RECORD;
    else if (!strcmp(id, "ref"))
        return LILY_TOKEN_KIND_KEYWORD_REF;
    else if (!strcmp(id, "req"))
        return LILY_TOKEN_KIND_KEYWORD_REQ;
    else if (!strcmp(id, "return"))
        return LILY_TOKEN_KIND_KEYWORD_RETURN;
    else if (!strcmp(id, "self"))
        return LILY_TOKEN_KIND_KEYWORD_self;
    else if (!strcmp(id, "Self"))
        return LILY_TOKEN_KIND_KEYWORD_SELF;
    else if (!strcmp(id, "test"))
        return LILY_TOKEN_KIND_KEYWORD_TEST;
    else if (!strcmp(id, "trace"))
        return LILY_TOKEN_KIND_KEYWORD_TRACE;
    else if (!strcmp(id, "trait"))
        return LILY_TOKEN_KIND_KEYWORD_TRAIT;
    else if (!strcmp(id, "true"))
        return LILY_TOKEN_KIND_KEYWORD_TRUE;
    else if (!strcmp(id, "try"))
        return LILY_TOKEN_KIND_KEYWORD_TRY;
    else if (!strcmp(id, "type"))
        return LILY_TOKEN_KIND_KEYWORD_TYPE;
    else if (!strcmp(id, "undef"))
        return LILY_TOKEN_KIND_KEYWORD_UNDEF;
    else if (!strcmp(id, "unsafe"))
        return LILY_TOKEN_KIND_KEYWORD_UNSAFE;
    else if (!strcmp(id, "val"))
        return LILY_TOKEN_KIND_KEYWORD_VAL;
    else if (!strcmp(id, "when"))
        return LILY_TOKEN_KIND_KEYWORD_WHEN;
    else if (!strcmp(id, "while"))
        return LILY_TOKEN_KIND_KEYWORD_WHILE;
    else if (!strcmp(id, "xor"))
        return LILY_TOKEN_KIND_KEYWORD_XOR;
    else
        return LILY_TOKEN_KIND_IDENTIFIER_NORMAL;
}

void
skip_space__Scanner(Scanner *self)
{
    while (isspace(self->source.cursor.current) &&
           self->source.cursor.position <
             strlen(self->source.file->content) - 1) {
        next_char__Source(&self->source);
    }
}

void
jump__Scanner(Scanner *self, Usize n)
{
    for (Usize i = 0; i < n; i++)
        next_char__Source(&self->source);
}

void
start_token__Scanner(Scanner *self, Usize line, Usize column)
{
    self->location.start_line = line;
    self->location.start_column = column;
}

void
end_token__Scanner(Scanner *self, Usize line, Usize column)
{
    self->location.end_line = line;
    self->location.end_column = column;
}

char *
peek_char__Scanner(const Scanner *self, Usize n)
{
    if (self->source.cursor.position < strlen(self->source.file->content) - 1) {
        return (char *)(Uptr)
          self->source.file->content[self->source.cursor.position + n];
    }

    return NULL;
}

void
next_char_by_token__Scanner(Scanner *self, LilyToken *token)
{
    switch (token->kind) {
        case LILY_TOKEN_KIND_COMMENT_BLOCK:
        case LILY_TOKEN_KIND_COMMENT_DOC:
        case LILY_TOKEN_KIND_COMMENT_LINE:
            return;
        case LILY_TOKEN_KIND_NOT_EQ:
        case LILY_TOKEN_KIND_XOR_EQ:
        case LILY_TOKEN_KIND_L_BRACE:
        case LILY_TOKEN_KIND_L_HOOK:
        case LILY_TOKEN_KIND_L_PAREN:
        case LILY_TOKEN_KIND_R_BRACE:
        case LILY_TOKEN_KIND_R_HOOK:
        case LILY_TOKEN_KIND_R_PAREN:
        case LILY_TOKEN_KIND_IDENTIFIER_MACRO:
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
        case LILY_TOKEN_KIND_KEYWORD_ALIAS:
        case LILY_TOKEN_KIND_KEYWORD_AND:
        case LILY_TOKEN_KIND_KEYWORD_AS:
        case LILY_TOKEN_KIND_KEYWORD_ASM:
        case LILY_TOKEN_KIND_KEYWORD_ASYNC:
        case LILY_TOKEN_KIND_KEYWORD_AWAIT:
        case LILY_TOKEN_KIND_KEYWORD_BEGIN:
        case LILY_TOKEN_KIND_KEYWORD_BREAK:
        case LILY_TOKEN_KIND_KEYWORD_CAST:
        case LILY_TOKEN_KIND_KEYWORD_CATCH:
        case LILY_TOKEN_KIND_KEYWORD_CLASS:
        case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
        case LILY_TOKEN_KIND_KEYWORD_DO:
        case LILY_TOKEN_KIND_KEYWORD_DROP:
        case LILY_TOKEN_KIND_KEYWORD_ELIF:
        case LILY_TOKEN_KIND_KEYWORD_ELSE:
        case LILY_TOKEN_KIND_KEYWORD_END:
        case LILY_TOKEN_KIND_KEYWORD_ENUM:
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
        case LILY_TOKEN_KIND_KEYWORD_FOR:
        case LILY_TOKEN_KIND_KEYWORD_FUN:
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
        case LILY_TOKEN_KIND_KEYWORD_IF:
        case LILY_TOKEN_KIND_KEYWORD_IMPL:
        case LILY_TOKEN_KIND_KEYWORD_IMPORT:
        case LILY_TOKEN_KIND_KEYWORD_IN:
        case LILY_TOKEN_KIND_KEYWORD_INHERIT:
        case LILY_TOKEN_KIND_KEYWORD_IS:
        case LILY_TOKEN_KIND_KEYWORD_MACRO:
        case LILY_TOKEN_KIND_KEYWORD_MATCH:
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
        case LILY_TOKEN_KIND_KEYWORD_MUT:
        case LILY_TOKEN_KIND_KEYWORD_NEXT:
        case LILY_TOKEN_KIND_KEYWORD_NIL:
        case LILY_TOKEN_KIND_KEYWORD_NONE:
        case LILY_TOKEN_KIND_KEYWORD_NOT:
        case LILY_TOKEN_KIND_KEYWORD_object:
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_OR:
        case LILY_TOKEN_KIND_KEYWORD_PACKAGE:
        case LILY_TOKEN_KIND_KEYWORD_PUB:
        case LILY_TOKEN_KIND_KEYWORD_RAISE:
        case LILY_TOKEN_KIND_KEYWORD_RECORD:
        case LILY_TOKEN_KIND_KEYWORD_REF:
        case LILY_TOKEN_KIND_KEYWORD_REQ:
        case LILY_TOKEN_KIND_KEYWORD_RETURN:
        case LILY_TOKEN_KIND_KEYWORD_self:
        case LILY_TOKEN_KIND_KEYWORD_SELF:
        case LILY_TOKEN_KIND_KEYWORD_TEST:
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
        case LILY_TOKEN_KIND_KEYWORD_TRAIT:
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
        case LILY_TOKEN_KIND_KEYWORD_TRY:
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
        case LILY_TOKEN_KIND_KEYWORD_UNSAFE:
        case LILY_TOKEN_KIND_KEYWORD_VAL:
        case LILY_TOKEN_KIND_KEYWORD_WHEN:
        case LILY_TOKEN_KIND_KEYWORD_WHILE:
        case LILY_TOKEN_KIND_KEYWORD_XOR:
        case LILY_TOKEN_KIND_LITERAL_BIT_CHAR:
        case LILY_TOKEN_KIND_LITERAL_BIT_STRING:
        case LILY_TOKEN_KIND_LITERAL_CHAR:
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
        case LILY_TOKEN_KIND_LITERAL_INT_2:
        case LILY_TOKEN_KIND_LITERAL_INT_8:
        case LILY_TOKEN_KIND_LITERAL_INT_10:
        case LILY_TOKEN_KIND_LITERAL_INT_16:
        case LILY_TOKEN_KIND_LITERAL_STRING:
            next_char__Source(&self->source);
            return;
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            jump__Scanner(self, 3);
            return;
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            jump__Scanner(self, 4);
            return;
        default: {
            String *token_s = to_string__LilyToken(token);

            jump__Scanner(self, token_s->len);

            FREE(String, token_s);
        }
    }
}

void
push_token__Scanner(Scanner *self, LilyToken *token)
{
    push__Vec(self->tokens, token);
}

bool
is_digit__Scanner(const Scanner *self)
{
    return isdigit(self->source.cursor.current);
}

bool
is_ident__Scanner(const Scanner *self)
{
    return is_digit__Scanner(self) || isalpha(self->source.cursor.current) ||
           self->source.cursor.current == '_';
}

bool
is_hex__Scanner(const Scanner *self)
{
    return is_digit__Scanner(self) ||
           (self->source.cursor.current >= 'a' &&
            self->source.cursor.current <= 'f') ||
           (self->source.cursor.current >= 'A' &&
            self->source.cursor.current <= 'f') ||
           self->source.cursor.current == '_';
}

bool
is_oct__Scanner(const Scanner *self)
{
    return (self->source.cursor.current >= '0' &&
            self->source.cursor.current <= '7') ||
           self->source.cursor.current == '_';
}

bool
is_bin__Scanner(const Scanner *self)
{
    return (self->source.cursor.current >= '0' &&
            self->source.cursor.current <= '1') ||
           self->source.cursor.current == '_';
}

bool
is_num__Scanner(const Scanner *self)
{
    return is_digit__Scanner(self) ||
           (self->source.cursor.current == '.' &&
            peek_char__Scanner(self, 1) != (char *)'.') ||
           self->source.cursor.current == 'e' ||
           self->source.cursor.current == 'E' ||
           self->source.cursor.current == '_';
}

String *
get_character__Scanner(Scanner *self, char previous)
{
    String *res = NULL;
    Location location_error = default__Location(self->source.file->name);

    start__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);

    switch (previous) {
        case '\\':
            switch (self->source.cursor.current) {
                case 'n':
                    res = from__String("\\n");
                    break;
                case 't':
                    res = from__String("\\t");
                    break;
                case 'r':
                    res = from__String("\\r");
                    break;
                case 'b':
                    res = from__String("\\b");
                    break;
                case '\\':
                    res = from__String("\\");
                    break;
                case '\'':
                    res = from__String("'");
                    break;
                case '\"':
                    res = from__String("\"");
                    break;
                default:
                    end__Location(&location_error,
                                  self->source.cursor.line,
                                  self->source.cursor.column);

                    if (self->source.cursor.position >=
                        strlen(self->source.file->content) - 1) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->source.file,
                            &location_error,
                            NEW(LilyError,
                                LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL),
                            NULL,
                            NULL,
                            NULL),
                          &self->count_error);
                    } else {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->source.file,
                            &location_error,
                            NEW(LilyError, LILY_ERROR_KIND_INVALID_ESCAPE),
                            NULL,
                            NULL,
                            NULL),
                          &self->count_error);
                    }

                    return NULL;
            }
            break;
        default:
            res = format__String("{c}", previous);
            break;
    }

    if (previous == '\\') {
        next_char__Source(&self->source);
    }

    return res;
}

void
skip_comment_line__Scanner(Scanner *self)
{
    while (self->source.cursor.current != '\n') {
        next_char__Source(&self->source);
    }
}

void
skip_comment_block__Scanner(Scanner *self)
{
    Location location_error = default__Location(self->source.file->name);

    start__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);

    while (self->source.cursor.current != '*' ||
           peek_char__Scanner(self, 1) != (char *)'/') {
        if (self->source.cursor.position >=
            strlen(self->source.file->content) - 2) {
            end__Location(&location_error,
                          self->source.cursor.line,
                          self->source.cursor.column);

            {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->source.file,
                    &location_error,
                    NEW(LilyError, LILY_ERROR_KIND_UNCLOSED_COMMENT_BLOCK),
                    init__Vec(
                      1, from__String("close comment multi line with `*/`")),
                    NULL,
                    NULL),
                  &self->count_error);
            }
        }

        next_char__Source(&self->source);
    }

    jump__Scanner(self, 2);
}

String *
scan_comment_doc__Scanner(Scanner *self)
{
    String *doc = NEW(String);

    while (self->source.cursor.current != '\n') {
        next_char__Source(&self->source);

        if (self->source.cursor.position >=
            strlen(self->source.file->content) - 1) {
            break;
        }

        push__String(
          doc, self->source.file->content[self->source.cursor.position - 1]);
    }

    previous_char__Source(&self->source);

    return doc;
}

static String *
scan_identifier__Scanner(Scanner *self)
{
    String *id = NEW(String);

    while (is_ident__Scanner(self)) {
        next_char__Source(&self->source);
        push__String(
          id, self->source.file->content[self->source.cursor.position - 1]);
    }

    previous_char__Source(&self->source);

    return id;
}

char *
scan_char__Scanner(Scanner *self)
{
    Location location_error = default__Location(self->source.file->name);

    start__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);
    next_char__Source(&self->source);

    if (self->source.cursor.current != '\'') {
        next_char__Source(&self->source);

        char target = self->source.cursor.current;
        String *character = get_character__Scanner(
          self, self->source.file->content[self->source.cursor.position - 1]);

        end__Location(&location_error,
                      self->source.cursor.line,
                      self->source.cursor.column);

        if (target != '\'' && self->source.cursor.current != '\'') {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->source.file,
                &location_error,
                NEW(LilyError, LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL),
                init__Vec(
                  1, from__String("please close this char literal with `\"`")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, character);

            return NULL;
        }

        if (character) {
            char *res = (char *)(Uptr)character->buffer[0];

            FREE(String, character);

            return res;
        }

        return NULL;
    }

    end__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);

    emit__Diagnostic(
      NEW_VARIANT(
        Diagnostic,
        simple_lily_error,
        self->source.file,
        &location_error,
        NEW(LilyError, LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL),
        init__Vec(1, from__String("please close this char literal with `\"`")),
        NULL,
        from__String("unexpected token here: `'`")),
      &self->count_error);

    return NULL;
}

static String *
scan_string__Scanner(Scanner *self)
{
    Location location_error = default__Location(self->source.file->name);
    String *res = NEW(String);

    start__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);
    next_char__Source(&self->source);

    while (self->source.cursor.current != '\"') {
        if (self->source.cursor.position >
            strlen(self->source.file->content) - 2) {
            end__Location(&location_error,
                          self->source.cursor.line,
                          self->source.cursor.column);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->source.file,
                &location_error,
                NEW(LilyError, LILY_ERROR_KIND_UNCLOSED_STRING_LITERAL),
                init__Vec(
                  1, from__String("add `\"` to the end of string literal")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, res);

            return NULL;
        }

        next_char__Source(&self->source);

        {
            String *c = get_character__Scanner(
              self,
              self->source.file->content[self->source.cursor.position - 1]);

            if (!c) {
                FREE(String, res);

                return NULL;
            }

            APPEND_AND_FREE(res, c);
        }
    }

    return res;
}

LilyToken *
scan_hex__Scanner(Scanner *self)
{
    Location location_error = default__Location(self->source.file->name);
    String *res = NEW(String);

    start__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);

    while (is_hex__Scanner(self)) {
        if (self->source.cursor.current == '_')
            continue;

        push__String(res, self->source.cursor.current);
        next_char__Source(&self->source);
    }

    if (is_empty__String(res)) {
        end__Location(&location_error,
                      self->source.cursor.line,
                      self->source.cursor.column);

        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            self->source.file,
            &location_error,
            NEW(LilyError, LILY_ERROR_KIND_INVALID_HEXADECIMAL_LITERAL),
            init__Vec(1, from__String("e.g. 0xff, 0xFF")),
            NULL,
            from__String("add a digit 0 to 9 or a letter a (A) to f (F)")),
          &self->count_error);

        FREE(String, res);

        return NULL;
    }

    previous_char__Source(&self->source);

    SCAN_LITERAL_SUFFIX(res->buffer, 16, true);

    return NEW_VARIANT(
      LilyToken, literal_int_16, clone__Location(&self->location), res);
}

LilyToken *
scan_oct__Scanner(Scanner *self)
{
    Location location_error = default__Location(self->source.file->name);
    String *res = NEW(String);

    start__Location(
      &location_error, self->source.cursor.line, self->source.cursor.column);

    while (is_oct__Scanner(self)) {
        push__String(res, self->source.cursor.current);
        next_char__Source(&self->source);
    }

    if (is_empty__String(res)) {
        end__Location(&location_error,
                      self->source.cursor.line,
                      self->source.cursor.column);

        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      self->source.file,
                      &location_error,
                      NEW(LilyError, LILY_ERROR_KIND_INVALID_OCTAL_LITERAL),
                      init__Vec(1, from__String("e.g. 0o22, 0o56")),
                      NULL,
                      from__String("add a digit 0 to 7")),
          &self->count_error);

        FREE(String, res);

        return NULL;
    }

    previous_char__Source(&self->source);

    SCAN_LITERAL_SUFFIX(res->buffer, 8, true);

    return NEW_VARIANT(
      LilyToken, literal_int_8, clone__Location(&self->location), res);
}

void
run__Scanner(Scanner *self, bool dump_scanner)
{
}

DESTRUCTOR(Scanner, const Scanner *self)
{
    FREE_BUFFER_ITEMS(self->tokens->buffer, self->tokens->len, LilyToken);
    FREE(Vec, self->tokens);
}
