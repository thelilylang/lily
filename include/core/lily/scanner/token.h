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

#ifndef LILY_CORE_LILY_SCANNER_TOKEN_H
#define LILY_CORE_LILY_SCANNER_TOKEN_H

#include <base/macros.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/shared/location.h>

#define LILY_N_KEYWORD 68
#define LILY_N_AT_KEYWORD 7

enum LilyTokenKind
{
    LILY_TOKEN_KIND_AMPERSAND_EQ,
    LILY_TOKEN_KIND_AMPERSAND,
    LILY_TOKEN_KIND_ARROW,
    LILY_TOKEN_KIND_AT,
    LILY_TOKEN_KIND_BACKSLASH,
    LILY_TOKEN_KIND_BANG,
    LILY_TOKEN_KIND_BAR_EQ,
    LILY_TOKEN_KIND_BAR_R_SHIFT,
    LILY_TOKEN_KIND_BAR,
    LILY_TOKEN_KIND_COLON_COLON,
    LILY_TOKEN_KIND_COLON_DOLLAR,
    LILY_TOKEN_KIND_COLON_EQ,
    LILY_TOKEN_KIND_COLON_R_SHIFT,
    LILY_TOKEN_KIND_COLON,
    LILY_TOKEN_KIND_COMMA,
    LILY_TOKEN_KIND_COMMENT_BLOCK,
#ifdef ENV_DEBUG
    LILY_TOKEN_KIND_COMMENT_DEBUG,
#endif
    LILY_TOKEN_KIND_COMMENT_DOC,
    LILY_TOKEN_KIND_COMMENT_LINE,
    LILY_TOKEN_KIND_DOLLAR,
    LILY_TOKEN_KIND_DOT_DOT_DOT,
    LILY_TOKEN_KIND_DOT_DOT,
    LILY_TOKEN_KIND_DOT_INTERROGATION,
    LILY_TOKEN_KIND_DOT_STAR,
    LILY_TOKEN_KIND_DOT,
    LILY_TOKEN_KIND_EOF,
    LILY_TOKEN_KIND_EQ_EQ,
    LILY_TOKEN_KIND_EQ,
    LILY_TOKEN_KIND_EXPAND,
    LILY_TOKEN_KIND_FAT_ARROW,
    LILY_TOKEN_KIND_HASHTAG,
    LILY_TOKEN_KIND_HAT_EQ,
    LILY_TOKEN_KIND_HAT,
    LILY_TOKEN_KIND_IDENTIFIER_DOLLAR,
    LILY_TOKEN_KIND_IDENTIFIER_MACRO,
    LILY_TOKEN_KIND_IDENTIFIER_NORMAL,
    LILY_TOKEN_KIND_IDENTIFIER_OPERATOR,
    LILY_TOKEN_KIND_IDENTIFIER_STRING,
    LILY_TOKEN_KIND_INTERROGATION,
    LILY_TOKEN_KIND_INVERSE_ARROW,
    LILY_TOKEN_KIND_KEYWORD_ALIAS,
    LILY_TOKEN_KIND_KEYWORD_AND,
    LILY_TOKEN_KIND_KEYWORD_AS,
    LILY_TOKEN_KIND_KEYWORD_ASM,
    LILY_TOKEN_KIND_KEYWORD_ASYNC,
    LILY_TOKEN_KIND_KEYWORD_AT_BUILTIN,
    LILY_TOKEN_KIND_KEYWORD_AT_CC,
    LILY_TOKEN_KIND_KEYWORD_AT_CPP,
    LILY_TOKEN_KIND_KEYWORD_AT_HIDE,
    LILY_TOKEN_KIND_KEYWORD_AT_HIDEOUT,
    LILY_TOKEN_KIND_KEYWORD_AT_LEN,
    LILY_TOKEN_KIND_KEYWORD_AT_SYS,
    LILY_TOKEN_KIND_KEYWORD_AWAIT,
    LILY_TOKEN_KIND_KEYWORD_BEGIN,
    LILY_TOKEN_KIND_KEYWORD_BREAK,
    LILY_TOKEN_KIND_KEYWORD_CAST,
    LILY_TOKEN_KIND_KEYWORD_CATCH,
    LILY_TOKEN_KIND_KEYWORD_CLASS,
    LILY_TOKEN_KIND_KEYWORD_CLOSE,
    LILY_TOKEN_KIND_KEYWORD_COMPTIME,
    LILY_TOKEN_KIND_KEYWORD_DEFER,
    LILY_TOKEN_KIND_KEYWORD_DO,
    LILY_TOKEN_KIND_KEYWORD_DROP,
    LILY_TOKEN_KIND_KEYWORD_ELIF,
    LILY_TOKEN_KIND_KEYWORD_ELSE,
    LILY_TOKEN_KIND_KEYWORD_END,
    LILY_TOKEN_KIND_KEYWORD_ENUM,
    LILY_TOKEN_KIND_KEYWORD_ERROR,
    LILY_TOKEN_KIND_KEYWORD_FALSE,
    LILY_TOKEN_KIND_KEYWORD_FOR,
    LILY_TOKEN_KIND_KEYWORD_FUN,
    LILY_TOKEN_KIND_KEYWORD_GET,
    LILY_TOKEN_KIND_KEYWORD_GLOBAL,
    LILY_TOKEN_KIND_KEYWORD_IF,
    LILY_TOKEN_KIND_KEYWORD_IMPL,
    LILY_TOKEN_KIND_KEYWORD_IMPORT,
    LILY_TOKEN_KIND_KEYWORD_IN,
    LILY_TOKEN_KIND_KEYWORD_INCLUDE,
    LILY_TOKEN_KIND_KEYWORD_INHERIT,
    LILY_TOKEN_KIND_KEYWORD_IS,
    LILY_TOKEN_KIND_KEYWORD_LIB,
    LILY_TOKEN_KIND_KEYWORD_MACRO,
    LILY_TOKEN_KIND_KEYWORD_MATCH,
    LILY_TOKEN_KIND_KEYWORD_MODULE,
    LILY_TOKEN_KIND_KEYWORD_MUT,
    LILY_TOKEN_KIND_KEYWORD_NEXT,
    LILY_TOKEN_KIND_KEYWORD_NIL,
    LILY_TOKEN_KIND_KEYWORD_NONE,
    LILY_TOKEN_KIND_KEYWORD_NOT,
    LILY_TOKEN_KIND_KEYWORD_object,
    LILY_TOKEN_KIND_KEYWORD_OBJECT,
    LILY_TOKEN_KIND_KEYWORD_OR,
    LILY_TOKEN_KIND_KEYWORD_PACKAGE,
    LILY_TOKEN_KIND_KEYWORD_PUB,
    LILY_TOKEN_KIND_KEYWORD_RAISE,
    LILY_TOKEN_KIND_KEYWORD_RECORD,
    LILY_TOKEN_KIND_KEYWORD_REF,
    LILY_TOKEN_KIND_KEYWORD_REQ,
    LILY_TOKEN_KIND_KEYWORD_RETURN,
    LILY_TOKEN_KIND_KEYWORD_self,
    LILY_TOKEN_KIND_KEYWORD_SELF,
    LILY_TOKEN_KIND_KEYWORD_SET,
    LILY_TOKEN_KIND_KEYWORD_TEST,
    LILY_TOKEN_KIND_KEYWORD_TRACE,
    LILY_TOKEN_KIND_KEYWORD_TRAIT,
    LILY_TOKEN_KIND_KEYWORD_TRUE,
    LILY_TOKEN_KIND_KEYWORD_TRY,
    LILY_TOKEN_KIND_KEYWORD_TYPE,
    LILY_TOKEN_KIND_KEYWORD_UNDEF,
    LILY_TOKEN_KIND_KEYWORD_UNSAFE,
    LILY_TOKEN_KIND_KEYWORD_USE,
    LILY_TOKEN_KIND_KEYWORD_VAL,
    LILY_TOKEN_KIND_KEYWORD_WHEN,
    LILY_TOKEN_KIND_KEYWORD_WHILE,
    LILY_TOKEN_KIND_KEYWORD_XOR,
    LILY_TOKEN_KIND_L_BRACE,
    LILY_TOKEN_KIND_L_HOOK,
    LILY_TOKEN_KIND_L_PAREN,
    LILY_TOKEN_KIND_L_SHIFT_EQ,
    LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ,
    LILY_TOKEN_KIND_L_SHIFT_L_SHIFT,
    LILY_TOKEN_KIND_L_SHIFT,
    LILY_TOKEN_KIND_LITERAL_BYTE,
    LILY_TOKEN_KIND_LITERAL_BYTES,
    LILY_TOKEN_KIND_LITERAL_CHAR,
    LILY_TOKEN_KIND_LITERAL_CSTR,
    LILY_TOKEN_KIND_LITERAL_FLOAT,
    LILY_TOKEN_KIND_LITERAL_INT_2,
    LILY_TOKEN_KIND_LITERAL_INT_8,
    LILY_TOKEN_KIND_LITERAL_INT_10,
    LILY_TOKEN_KIND_LITERAL_INT_16,
    LILY_TOKEN_KIND_LITERAL_STR,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8,
    LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE,
    LILY_TOKEN_KIND_MINUS_EQ,
    LILY_TOKEN_KIND_MINUS_MINUS_EQ,
    LILY_TOKEN_KIND_MINUS_MINUS,
    LILY_TOKEN_KIND_MINUS,
    LILY_TOKEN_KIND_NOT_EQ,
    LILY_TOKEN_KIND_PERCENTAGE_EQ,
    LILY_TOKEN_KIND_PERCENTAGE,
    LILY_TOKEN_KIND_PLUS_EQ,
    LILY_TOKEN_KIND_PLUS_PLUS_EQ,
    LILY_TOKEN_KIND_PLUS_PLUS,
    LILY_TOKEN_KIND_PLUS,
    LILY_TOKEN_KIND_R_BRACE,
    LILY_TOKEN_KIND_R_HOOK,
    LILY_TOKEN_KIND_R_PAREN,
    LILY_TOKEN_KIND_R_SHIFT_EQ,
    LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ,
    LILY_TOKEN_KIND_R_SHIFT_R_SHIFT,
    LILY_TOKEN_KIND_R_SHIFT,
    LILY_TOKEN_KIND_SEMICOLON,
    LILY_TOKEN_KIND_SLASH_EQ,
    LILY_TOKEN_KIND_SLASH,
    LILY_TOKEN_KIND_STAR_EQ,
    LILY_TOKEN_KIND_STAR_STAR_EQ,
    LILY_TOKEN_KIND_STAR_STAR,
    LILY_TOKEN_KIND_STAR,
    LILY_TOKEN_KIND_WAVE_EQ,
    LILY_TOKEN_KIND_WAVE,
    LILY_TOKEN_KIND_XOR_EQ
};

enum LilyTokenExpandKind
{
    LILY_TOKEN_EXPAND_KIND_DT,
    LILY_TOKEN_EXPAND_KIND_EXPR,
    LILY_TOKEN_EXPAND_KIND_PATT,
    LILY_TOKEN_EXPAND_KIND_PATH,
};

/**
 *
 * @brief Convert LilyTokenExpandKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyTokenExpandKind, enum LilyTokenExpandKind self);
#endif

typedef struct LilyTokenExpand
{
    enum LilyTokenExpandKind kind;
    Vec *tokens; // Vec<LilyToken*>* (&)
} LilyTokenExpand;

/**
 *
 * @brief Convert LilyTokenExpand in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyTokenExpand, const LilyTokenExpand *self);
#endif

inline CONSTRUCTOR(LilyTokenExpand,
                   LilyTokenExpand,
                   enum LilyTokenExpandKind kind,
                   Vec *tokens)
{
    return (LilyTokenExpand){ .kind = kind, .tokens = tokens };
}

typedef struct LilyToken
{
    enum LilyTokenKind kind;
    Location location;
    union
    {
#ifdef ENV_DEBUG
        String *comment_debug;
#endif
        String *comment_doc;
        LilyTokenExpand expand;
        String *identifier_dollar;
        String *identifier_macro;
        String *identifier_normal;
        String *identifier_operator;
        String *identifier_string;
        Uint8 literal_byte;
        Uint8 *literal_bytes;
        char literal_char;
        char *literal_cstr;
        String *literal_float;
        String *literal_int_2;
        String *literal_int_8;
        String *literal_int_10;
        String *literal_int_16;
        String *literal_str;
        Float32 literal_suffix_float32;
        Float64 literal_suffix_float64;
        Int16 literal_suffix_int16;
        Int32 literal_suffix_int32;
        Int64 literal_suffix_int64;
        Int8 literal_suffix_int8;
        Isize literal_suffix_isize;
        Uint16 literal_suffix_uint16;
        Uint32 literal_suffix_uint32;
        Uint64 literal_suffix_uint64;
        Uint8 literal_suffix_uint8;
        Usize literal_suffix_usize;
    };
} LilyToken;

/**
 *
 * @brief Construct LilyToken type.
 */
CONSTRUCTOR(LilyToken *, LilyToken, enum LilyTokenKind kind, Location location);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_COMMENT_DEBUG).
 */
#ifdef ENV_DEBUG
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    comment_debug,
                    Location location,
                    String *comment_debug);
#endif

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_COMMENT_DOC).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    comment_doc,
                    Location location,
                    String *comment_doc);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_EXPAND).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    expand,
                    Location location,
                    LilyTokenExpand expand);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_DOLLAR).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_dollar,
                    Location location,
                    String *identifier_dollar);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_MACRO).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_macro,
                    Location location,
                    String *identifier_macro);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_NORMAL).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_normal,
                    Location location,
                    String *identifier_normal);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_OPERATOR).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_operator,
                    Location location,
                    String *identifier_operator);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_IDENTIFIER_STRING).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    identifier_string,
                    Location location,
                    String *identifier_string);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_BYTE).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_byte,
                    Location location,
                    Uint8 literal_byte);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_BYTES).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_bytes,
                    Location location,
                    Uint8 *literal_bytes);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_CHAR).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_char,
                    Location location,
                    char literal_char);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_CSTR).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_cstr,
                    Location location,
                    char *literal_cstr);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_FLOAT).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_float,
                    Location location,
                    String *literal_float);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_2).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_2,
                    Location location,
                    String *literal_int_2);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_8).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_8,
                    Location location,
                    String *literal_int_8);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_10).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_10,
                    Location location,
                    String *literal_int_10);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_INT_16).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_int_16,
                    Location location,
                    String *literal_int_16);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_STR).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_str,
                    Location location,
                    String *literal_str);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_float32,
                    Location location,
                    Float32 literal_suffix_float32);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_float64,
                    Location location,
                    Float64 literal_suffix_float64);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int16,
                    Location location,
                    Int16 literal_suffix_int16);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int32,
                    Location location,
                    Int32 literal_suffix_int32);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int64,
                    Location location,
                    Int64 literal_suffix_int64);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_int8,
                    Location location,
                    Int8 literal_suffix_int8);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_isize,
                    Location location,
                    Isize literal_suffix_isize);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint16,
                    Location location,
                    Uint16 literal_suffix_uint16);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint32,
                    Location location,
                    Uint32 literal_suffix_uint32);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint64,
                    Location location,
                    Uint64 literal_suffix_uint64);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_uint8,
                    Location location,
                    Uint8 literal_suffix_uint8);

/**
 *
 * @brief Construct LilyToken type (LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE).
 */
VARIANT_CONSTRUCTOR(LilyToken *,
                    LilyToken,
                    literal_suffix_usize,
                    Location location,
                    Usize literal_suffix_usize);

/**
 *
 * @brief Convert LilyToken in String (without Location).
 * @note This function is only used to show the token in the parser.
 */
String *
to_string__LilyToken(LilyToken *self);

/**
 *
 * @brief Convert LilyTokenKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyTokenKind, enum LilyTokenKind self);
#endif

/**
 *
 * @brief Convert LilyToken in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyToken, const LilyToken *self);
#endif

/**
 *
 * @brief Print debug LilyToken struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyToken, const LilyToken *self);
#endif

/**
 *
 * @brief Clone LilyToken.
 */
LilyToken *
clone__LilyToken(const LilyToken *self);

/**
 *
 * @brief Free LilyToken type.
 */
DESTRUCTOR(LilyToken, LilyToken *self);

#endif // LILY_CORE_LILY_SCANNER_TOKEN_H
