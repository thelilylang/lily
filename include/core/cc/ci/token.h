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

#ifndef LILY_CORE_CC_CI_TOKEN_H
#define LILY_CORE_CC_CI_TOKEN_H

#include <base/macros.h>
#include <base/string.h>

#include <core/cc/ci/features.h>
#include <core/shared/location.h>

// NOTE#1: Used only in the scanner
enum CITokenKind
{
    CI_TOKEN_KIND_AMPERSAND,
    CI_TOKEN_KIND_AMPERSAND_AMPERSAND,
    CI_TOKEN_KIND_AMPERSAND_EQ,
    CI_TOKEN_KIND_ARROW,
    CI_TOKEN_KIND_BANG,
    CI_TOKEN_KIND_BANG_EQ,
    CI_TOKEN_KIND_BAR,
    CI_TOKEN_KIND_BAR_BAR,
    CI_TOKEN_KIND_BAR_EQ,
    CI_TOKEN_KIND_COLON,
    CI_TOKEN_KIND_COMMA,
    CI_TOKEN_KIND_COMMENT_LINE,
    CI_TOKEN_KIND_COMMENT_BLOCK,
    CI_TOKEN_KIND_COMMENT_DOC,
    CI_TOKEN_KIND_DOT,
    CI_TOKEN_KIND_DOT_DOT_DOT,
    CI_TOKEN_KIND_EOF,
    CI_TOKEN_KIND_EQ,
    CI_TOKEN_KIND_EQ_EQ,
    CI_TOKEN_KIND_HASHTAG,
    CI_TOKEN_KIND_HAT,
    CI_TOKEN_KIND_HAT_EQ,
    CI_TOKEN_KIND_IDENTIFIER,
    CI_TOKEN_KIND_INTERROGATION,
    CI_TOKEN_KIND_KEYWORD_ALIGNAS,
    CI_TOKEN_KIND_KEYWORD_ALIGNOF,
    CI_TOKEN_KIND_KEYWORD_ASM,
    CI_TOKEN_KIND_KEYWORD_AUTO,
    CI_TOKEN_KIND_KEYWORD_BOOL,
    CI_TOKEN_KIND_KEYWORD_BREAK,
    CI_TOKEN_KIND_KEYWORD_CASE,
    CI_TOKEN_KIND_KEYWORD_CHAR,
    CI_TOKEN_KIND_KEYWORD_CONST,
    CI_TOKEN_KIND_KEYWORD_CONSTEXPR,
    CI_TOKEN_KIND_KEYWORD_CONTINUE,
    CI_TOKEN_KIND_KEYWORD_DEFAULT,
    CI_TOKEN_KIND_KEYWORD_DO,
    CI_TOKEN_KIND_KEYWORD_DOUBLE,
    CI_TOKEN_KIND_KEYWORD_ELSE,
    CI_TOKEN_KIND_KEYWORD_ENUM,
    CI_TOKEN_KIND_KEYWORD_EXTERN,
    CI_TOKEN_KIND_KEYWORD_FALSE,
    CI_TOKEN_KIND_KEYWORD_FLOAT,
    CI_TOKEN_KIND_KEYWORD_FOR,
    CI_TOKEN_KIND_KEYWORD_GOTO,
    CI_TOKEN_KIND_KEYWORD_IF,
    CI_TOKEN_KIND_KEYWORD_INLINE,
    CI_TOKEN_KIND_KEYWORD_INT,
    CI_TOKEN_KIND_KEYWORD_LONG, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_LONG_INT,
    CI_TOKEN_KIND_KEYWORD_LONG_LONG,     // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_NULLPTR,
    CI_TOKEN_KIND_KEYWORD_REGISTER,
    CI_TOKEN_KIND_KEYWORD_RESTRICT,
    CI_TOKEN_KIND_KEYWORD_RETURN,
    CI_TOKEN_KIND_KEYWORD_SHORT, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SHORT_INT,
    CI_TOKEN_KIND_KEYWORD_SIGNED, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR,
    CI_TOKEN_KIND_KEYWORD_SIGNED_INT,           // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_LONG,          // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT,      // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG,     // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT,         // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT,     // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_SIZEOF,
    CI_TOKEN_KIND_KEYWORD_STATIC,
    CI_TOKEN_KIND_KEYWORD_STATIC_ASSERT,
    CI_TOKEN_KIND_KEYWORD_STRUCT,
    CI_TOKEN_KIND_KEYWORD_SWITCH,
    CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL,
    CI_TOKEN_KIND_KEYWORD_TRUE,
    CI_TOKEN_KIND_KEYWORD_TYPEDEF,
    CI_TOKEN_KIND_KEYWORD_TYPEOF,
    CI_TOKEN_KIND_KEYWORD_TYPEOF_UNQUAL,
    CI_TOKEN_KIND_KEYWORD_UNION,
    CI_TOKEN_KIND_KEYWORD_UNSIGNED, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR,
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT,
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT,
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG,     // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT,         // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT,
    CI_TOKEN_KIND_KEYWORD_VOID,
    CI_TOKEN_KIND_KEYWORD_VOLATILE,
    CI_TOKEN_KIND_KEYWORD_WHILE,
    CI_TOKEN_KIND_KEYWORD__ALIGNAS,
    CI_TOKEN_KIND_KEYWORD__ALIGNOF,
    CI_TOKEN_KIND_KEYWORD__ATOMIC,
    CI_TOKEN_KIND_KEYWORD__BITINT,
    CI_TOKEN_KIND_KEYWORD__BOOL,
    CI_TOKEN_KIND_KEYWORD__COMPLEX,
    CI_TOKEN_KIND_KEYWORD__DECIMAL128,
    CI_TOKEN_KIND_KEYWORD__DECIMAL32,
    CI_TOKEN_KIND_KEYWORD__DECIMAL64,
    CI_TOKEN_KIND_KEYWORD__GENERIC,
    CI_TOKEN_KIND_KEYWORD__IMAGINARY,
    CI_TOKEN_KIND_KEYWORD__NORETURN,
    CI_TOKEN_KIND_KEYWORD__STATIC_ASSERT,
    CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL,
    CI_TOKEN_KIND_LBRACE,
    CI_TOKEN_KIND_LHOOK,
    CI_TOKEN_KIND_LPAREN,
    CI_TOKEN_KIND_LSHIFT,
    CI_TOKEN_KIND_LSHIFT_EQ,
    CI_TOKEN_KIND_LSHIFT_LSHIFT,
    CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ,
    CI_TOKEN_KIND_LITERAL_CONSTANT_INT,
    CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT,
    CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL,
    CI_TOKEN_KIND_LITERAL_CONSTANT_HEX,
    CI_TOKEN_KIND_LITERAL_CONSTANT_BIN,
    CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER,
    CI_TOKEN_KIND_LITERAL_CONSTANT_STRING,
    CI_TOKEN_KIND_MINUS,
    CI_TOKEN_KIND_MINUS_EQ,
    CI_TOKEN_KIND_MINUS_MINUS,
    CI_TOKEN_KIND_PERCENTAGE,
    CI_TOKEN_KIND_PERCENTAGE_EQ,
    CI_TOKEN_KIND_PLUS,
    CI_TOKEN_KIND_PLUS_EQ,
    CI_TOKEN_KIND_PLUS_PLUS,
    CI_TOKEN_KIND_RBRACE,
    CI_TOKEN_KIND_RHOOK,
    CI_TOKEN_KIND_RPAREN,
    CI_TOKEN_KIND_RSHIFT,
    CI_TOKEN_KIND_RSHIFT_EQ,
    CI_TOKEN_KIND_RSHIFT_RSHIFT,
    CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ,
    CI_TOKEN_KIND_SEMICOLON,
    CI_TOKEN_KIND_SLASH,
    CI_TOKEN_KIND_SLASH_EQ,
    CI_TOKEN_KIND_STAR,
    CI_TOKEN_KIND_STAR_EQ,
    CI_TOKEN_KIND_WAVE,
    CI_TOKEN_KIND_WAVE_EQ,

    // NOTE: Never add an enumeration variant after this variant.
    CI_TOKEN_KIND_MAX
};

typedef struct CIToken
{
    enum CITokenKind kind;
    Location location;
    union
    {
        String *comment_doc;
        String *identifier;
        String *literal_constant_int;
        String *literal_constant_float;
        String *literal_constant_octal;
        String *literal_constant_hex;
        String *literal_constant_bin;
        char literal_constant_character;
        String *literal_constant_string;
    };
} CIToken;

/**
 *
 * @brief Construct CIToken type.
 */
CONSTRUCTOR(CIToken *, CIToken, enum CITokenKind kind, Location location);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_COMMENT_DOC).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    comment_doc,
                    Location location,
                    String *comment_doc);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_IDENTIFIER).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    identifier,
                    Location location,
                    String *identifier);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_INT).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_int,
                    Location location,
                    String *literal_constant_int);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_float,
                    Location location,
                    String *literal_constant_float);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_octal,
                    Location location,
                    String *literal_constant_octal);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_HEX).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_hex,
                    Location location,
                    String *literal_constant_hex);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_BIN).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_bin,
                    Location location,
                    String *literal_constant_bin);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_character,
                    Location location,
                    char literal_constant_character);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_STRING).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_string,
                    Location location,
                    String *literal_constant_string);

/**
 *
 * @brief Convert CIToken in String (without Location).
 * @note This function is only used to show the token in the parser.
 */
String *
to_string__CIToken(CIToken *self);

/**
 *
 * @brief Convert CITokenKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CITokenKind, enum CITokenKind self);
#endif

/**
 *
 * @brief Convert CIToken in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CIToken, const CIToken *self);
#endif

/**
 *
 * @brief Print debug CIToken struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, CIToken, const CIToken *self);
#endif

/**
 *
 * @brief Free CIToken type.
 */
DESTRUCTOR(CIToken, CIToken *self);

#endif // LILY_CORE_CC_CI_TOKEN_H
