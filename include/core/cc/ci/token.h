/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/stack.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/cc/ci/features.h>
#include <core/shared/location.h>

typedef struct CIResultFile CIResultFile;

// NOTE: Does not take multi-part keywords into account.
// e.g. unsigned int, unsigned long long int, ...
#define CI_N_SINGLE_KEYWORD 60

#define CI_N_ATTRIBUTE 8

#define CI_N_PREPROCESSOR 16

// NOTE#1: Used only in the scanner
enum CITokenKind
{
    CI_TOKEN_KIND_AMPERSAND,
    CI_TOKEN_KIND_AMPERSAND_AMPERSAND,
    CI_TOKEN_KIND_AMPERSAND_EQ,
    CI_TOKEN_KIND_ARROW,
    CI_TOKEN_KIND_AT, // only available in CI
    CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED,
    CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH,
    CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED,
    CI_TOKEN_KIND_ATTRIBUTE_NODISCARD,
    CI_TOKEN_KIND_ATTRIBUTE_NORETURN,
    CI_TOKEN_KIND_ATTRIBUTE__NORETURN,
    CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED,
    CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE,
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
    CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX,
    CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY,
    CI_TOKEN_KIND_KEYWORD_ELSE,
    CI_TOKEN_KIND_KEYWORD_ELSE_IF,
    CI_TOKEN_KIND_KEYWORD_ENUM,
    CI_TOKEN_KIND_KEYWORD_EXTERN,
    CI_TOKEN_KIND_KEYWORD_FALSE,
    CI_TOKEN_KIND_KEYWORD_FLOAT,
    CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX,
    CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY,
    CI_TOKEN_KIND_KEYWORD_FOR,
    CI_TOKEN_KIND_KEYWORD_GOTO,
    CI_TOKEN_KIND_KEYWORD_IF,
    CI_TOKEN_KIND_KEYWORD_INLINE,
    CI_TOKEN_KIND_KEYWORD_INT,
    CI_TOKEN_KIND_KEYWORD_LONG, // NOTE: #1
    CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE,
    CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX,
    CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY,
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
    CI_TOKEN_KIND_MACRO_PARAM,
    CI_TOKEN_KIND_MINUS,
    CI_TOKEN_KIND_MINUS_EQ,
    CI_TOKEN_KIND_MINUS_MINUS,
    CI_TOKEN_KIND_PAREN_CALL, // <id>(...)
    CI_TOKEN_KIND_PERCENTAGE,
    CI_TOKEN_KIND_PERCENTAGE_EQ,
    CI_TOKEN_KIND_PLUS,
    CI_TOKEN_KIND_PLUS_EQ,
    CI_TOKEN_KIND_PLUS_PLUS,
    CI_TOKEN_KIND_PREPROCESSOR_DEFINE,
    CI_TOKEN_KIND_PREPROCESSOR_ELIF,
    CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF,
    CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF,
    CI_TOKEN_KIND_PREPROCESSOR_ELSE,
    CI_TOKEN_KIND_PREPROCESSOR_EMBED,
    CI_TOKEN_KIND_PREPROCESSOR_ENDIF, // NOTE: #1
    CI_TOKEN_KIND_PREPROCESSOR_ERROR,
    CI_TOKEN_KIND_PREPROCESSOR_IF,
    CI_TOKEN_KIND_PREPROCESSOR_IFDEF,
    CI_TOKEN_KIND_PREPROCESSOR_IFNDEF,
    CI_TOKEN_KIND_PREPROCESSOR_INCLUDE,
    CI_TOKEN_KIND_PREPROCESSOR_LINE,
    CI_TOKEN_KIND_PREPROCESSOR_PRAGMA,
    CI_TOKEN_KIND_PREPROCESSOR_UNDEF,   // NOTE: #2
    CI_TOKEN_KIND_PREPROCESSOR_WARNING, // NOTE: #2
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

typedef struct CITokenPreprocessorDefine
{
    String *name;
    Vec *params; // Vec<String*>*?
    Vec *tokens; // Vec<CIToken*>*?
} CITokenPreprocessorDefine;

/**
 *
 * @brief Construct CITokenPreprocessorDefine type.
 */
inline CONSTRUCTOR(CITokenPreprocessorDefine,
                   CITokenPreprocessorDefine,
                   String *name,
                   Vec *params,
                   Vec *tokens)
{
    return (CITokenPreprocessorDefine){ .name = name,
                                        .params = params,
                                        .tokens = tokens };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorDefine type.
 */
String *
to_string__CITokenPreprocessorDefine(const CITokenPreprocessorDefine *self);

/**
 *
 * @brief Convert CITokenPreprocessorDefine in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorDefine,
               const CITokenPreprocessorDefine *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorDefine type.
 */
DESTRUCTOR(CITokenPreprocessorDefine, const CITokenPreprocessorDefine *self);

typedef struct CITokenPreprocessorEmbed
{
    String *value;
} CITokenPreprocessorEmbed;

/**
 *
 * @brief Construct CITokenPreprocessorEmbed type.
 */
inline CONSTRUCTOR(CITokenPreprocessorEmbed,
                   CITokenPreprocessorEmbed,
                   String *value)
{
    return (CITokenPreprocessorEmbed){ .value = value };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorEmbed type.
 */
String *
to_string__CITokenPreprocessorEmbed(const CITokenPreprocessorEmbed *self);

/**
 *
 * @brief Convert CITokenPreprocessorEmbed in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorEmbed,
               const CITokenPreprocessorEmbed *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorEmbed type.
 */
DESTRUCTOR(CITokenPreprocessorEmbed, const CITokenPreprocessorEmbed *self);

typedef struct CITokenPreprocessorLine
{
    Usize line;
    String *filename; // String*?
} CITokenPreprocessorLine;

/**
 *
 * @brief Construct CITokenPreprocessorLine type.
 */
inline CONSTRUCTOR(CITokenPreprocessorLine,
                   CITokenPreprocessorLine,
                   Usize line,
                   String *filename)
{
    return (CITokenPreprocessorLine){ .line = line, .filename = filename };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorLine type.
 */
String *
to_string__CITokenPreprocessorLine(const CITokenPreprocessorLine *self);

/**
 *
 * @brief Convert CITokenPreprocessorLine in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorLine,
               const CITokenPreprocessorLine *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorLine type.
 */
DESTRUCTOR(CITokenPreprocessorLine, const CITokenPreprocessorLine *self);

typedef struct CITokenPreprocessorIf
{
    Vec *cond;    // Vec<CIToken*>*
    Vec *content; // Vec<CIToken*>*?
} CITokenPreprocessorIf;

/**
 *
 * @brief Construct CITokenPreprocessorIf type.
 */
inline CONSTRUCTOR(CITokenPreprocessorIf,
                   CITokenPreprocessorIf,
                   Vec *cond,
                   Vec *content)
{
    return (CITokenPreprocessorIf){ .cond = cond, .content = content };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorIf type.
 */
String *
to_string__CITokenPreprocessorIf(const CITokenPreprocessorIf *self);

/**
 *
 * @brief Convert CITokenPreprocessorIf in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorIf,
               const CITokenPreprocessorIf *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorIf type.
 */
DESTRUCTOR(CITokenPreprocessorIf, const CITokenPreprocessorIf *self);

typedef CITokenPreprocessorIf CITokenPreprocessorElif;

/**
 *
 * @brief Construct CITokenPreprocessorElif type.
 */
inline CONSTRUCTOR(CITokenPreprocessorElif,
                   CITokenPreprocessorElif,
                   Vec *cond,
                   Vec *content)
{
    return (CITokenPreprocessorElif){ .cond = cond, .content = content };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorElif type.
 */
String *
to_string__CITokenPreprocessorElif(const CITokenPreprocessorElif *self);

/**
 *
 * @brief Convert CITokenPreprocessorElif in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElif,
               const CITokenPreprocessorElif *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorElif type.
 */
DESTRUCTOR(CITokenPreprocessorElif, const CITokenPreprocessorElif *self);

typedef struct CITokenPreprocessorIfdef
{
    String *identifier;
    Vec *content; // Vec<CIToken*>*?
} CITokenPreprocessorIfdef;

/**
 *
 * @brief Construct CITokenPreprocessorIfdef type.
 */
inline CONSTRUCTOR(CITokenPreprocessorIfdef,
                   CITokenPreprocessorIfdef,
                   String *identifier,
                   Vec *content)
{
    return (CITokenPreprocessorIfdef){ .identifier = identifier,
                                       .content = content };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorIfdef type.
 */
String *
to_string__CITokenPreprocessorIfdef(const CITokenPreprocessorIfdef *self);

/**
 *
 * @brief Convert CITokenPreprocessorIfdef in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorIfdef,
               const CITokenPreprocessorIfdef *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorIfdef type.
 */
DESTRUCTOR(CITokenPreprocessorIfdef, const CITokenPreprocessorIfdef *self);

typedef CITokenPreprocessorIfdef CITokenPreprocessorIfndef;

/**
 *
 * @brief Construct CITokenPreprocessorIfndef type.
 */
inline CONSTRUCTOR(CITokenPreprocessorIfndef,
                   CITokenPreprocessorIfndef,
                   String *identifier,
                   Vec *content)
{
    return NEW(CITokenPreprocessorIfdef, identifier, content);
}

/**
 *
 * @brief Convert to string CITokenPreprocessorIfndef type.
 */
String *
to_string__CITokenPreprocessorIfndef(const CITokenPreprocessorIfndef *self);

/**
 *
 * @brief Convert CITokenPreprocessorIfndef in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorIfndef,
               const CITokenPreprocessorIfndef *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorIfndef type.
 */
DESTRUCTOR(CITokenPreprocessorIfndef, const CITokenPreprocessorIfndef *self);

typedef CITokenPreprocessorIfdef CITokenPreprocessorElifdef;

/**
 *
 * @brief Construct CITokenPreprocessorElifdef type.
 */
inline CONSTRUCTOR(CITokenPreprocessorElifdef,
                   CITokenPreprocessorElifdef,
                   String *identifier,
                   Vec *content)
{
    return NEW(CITokenPreprocessorIfdef, identifier, content);
}

/**
 *
 * @brief Convert to string CITokenPreprocessorElifdef type.
 */
String *
to_string__CITokenPreprocessorElifdef(const CITokenPreprocessorElifdef *self);

/**
 *
 * @brief Convert CITokenPreprocessorElifdef in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElifdef,
               const CITokenPreprocessorElifdef *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorElifdef type.
 */
DESTRUCTOR(CITokenPreprocessorElifdef, const CITokenPreprocessorElifdef *self);

typedef CITokenPreprocessorIfdef CITokenPreprocessorElifndef;

/**
 *
 * @brief Construct CITokenPreprocessorElifndef type.
 */
inline CONSTRUCTOR(CITokenPreprocessorElifndef,
                   CITokenPreprocessorElifndef,
                   String *identifier,
                   Vec *content)
{
    return NEW(CITokenPreprocessorIfdef, identifier, content);
}

/**
 *
 * @brief Convert to string CITokenPreprocessorElifndef type.
 */
String *
to_string__CITokenPreprocessorElifndef(const CITokenPreprocessorElifndef *self);

/**
 *
 * @brief Convert CITokenPreprocessorElifndef in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElifndef,
               const CITokenPreprocessorElifndef *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorElifndef type.
 */
DESTRUCTOR(CITokenPreprocessorElifndef,
           const CITokenPreprocessorElifndef *self);

typedef struct CITokenPreprocessorElse
{
    Vec *content; // Vec<CIToken*>*?
} CITokenPreprocessorElse;

/**
 *
 * @brief Construct CITokenPreprocessorElse type.
 */
inline CONSTRUCTOR(CITokenPreprocessorElse,
                   CITokenPreprocessorElse,
                   Vec *content)
{
    return (CITokenPreprocessorElse){ .content = content };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorElse type.
 */
String *
to_string__CITokenPreprocessorElse(const CITokenPreprocessorElse *self);

/**
 *
 * @brief Convert CITokenPreprocessorElse in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorElse,
               const CITokenPreprocessorElse *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorElse type.
 */
DESTRUCTOR(CITokenPreprocessorElse, const CITokenPreprocessorElse *self);

typedef struct CITokenPreprocessorInclude
{
    String *value;
} CITokenPreprocessorInclude;

/**
 *
 * @brief Construct CITokenPreprocessorInclude type.
 */
inline CONSTRUCTOR(CITokenPreprocessorInclude,
                   CITokenPreprocessorInclude,
                   String *value)
{
    return (CITokenPreprocessorInclude){ .value = value };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorInclude type.
 */
String *
to_string__CITokenPreprocessorInclude(const CITokenPreprocessorInclude *self);

/**
 *
 * @brief Convert CITokenPreprocessorInclude in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorInclude,
               const CITokenPreprocessorInclude *self);
#endif

/**
 *
 * @brief Free CITokenPreprocessorInclude type.
 */
inline DESTRUCTOR(CITokenPreprocessorInclude,
                  const CITokenPreprocessorInclude *self)
{
    FREE(String, self->value);
}

typedef struct CIToken
{
    enum CITokenKind kind;
    Location location;
    union
    {
        String *attribute_deprecated; // String*?
        String *attribute_nodiscard;  // String*?
        String *comment_doc;
        CITokenPreprocessorDefine preprocessor_define;
        CITokenPreprocessorElif preprocessor_elif;
        CITokenPreprocessorElifdef preprocessor_elifdef;
        CITokenPreprocessorElifndef preprocessor_elifndef;
        CITokenPreprocessorElse preprocessor_else;
        CITokenPreprocessorEmbed preprocessor_embed;
        String *preprocessor_error;
        CITokenPreprocessorIf preprocessor_if;
        CITokenPreprocessorIfdef preprocessor_ifdef;
        CITokenPreprocessorIfdef preprocessor_ifndef;
        CITokenPreprocessorInclude preprocessor_include;
        CITokenPreprocessorLine preprocessor_line;
        String *preprocessor_undef;
        String *preprocessor_warning;
        String *identifier;
        String *literal_constant_int;
        String *literal_constant_float;
        String *literal_constant_octal;
        String *literal_constant_hex;
        String *literal_constant_bin;
        char literal_constant_character;
        String *literal_constant_string;
        // The `macro_param` corresponds to the position index in the macro
        // (#define) params vector (preprocessor_define.params).
        Usize macro_param;
    };
} CIToken;

/**
 *
 * @brief Construct CIToken type.
 */
CONSTRUCTOR(CIToken *, CIToken, enum CITokenKind kind, Location location);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED).
 * @param attribute_deprecated String*?
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    attribute_deprecated,
                    Location location,
                    String *attribute_deprecated);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_ATTRIBUTE_NODISCARD).
 * @param attribute_nodiscard String*?
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    attribute_nodiscard,
                    Location location,
                    String *attribute_nodiscard);

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
 * @brief Construct CIToken type (CI_TOKEN_KIND_MACRO_PARAM).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_param,
                    Location location,
                    Usize macro_param);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_DEFINE).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_define,
                    Location location,
                    CITokenPreprocessorDefine preprocessor_define);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_elif,
                    Location location,
                    CITokenPreprocessorElif preprocessor_elif);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_elifdef,
                    Location location,
                    CITokenPreprocessorElifdef preprocessor_elifdef);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_elifndef,
                    Location location,
                    CITokenPreprocessorElifndef preprocessor_elifndef);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ELSE).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_else,
                    Location location,
                    CITokenPreprocessorElse preprocessor_else);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_EMBED).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_embed,
                    Location location,
                    CITokenPreprocessorEmbed preprocessor_embed);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_ERROR).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_error,
                    Location location,
                    String *preprocessor_error);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_IF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_if,
                    Location location,
                    CITokenPreprocessorIf preprocessor_if);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_IFDEF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_ifdef,
                    Location location,
                    CITokenPreprocessorIfdef preprocessor_ifdef);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_IFNDEF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_ifndef,
                    Location location,
                    CITokenPreprocessorIfdef preprocessor_ifndef);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_INCLUDE).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_include,
                    Location location,
                    CITokenPreprocessorInclude preprocessor_include);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_LINE).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_line,
                    Location location,
                    CITokenPreprocessorLine preprocessor_line);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_WARNING).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_warning,
                    Location location,
                    String *preprocessor_warning);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_UNDEF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_undef,
                    Location location,
                    String *preprocessor_undef);

/**
 *
 * @brief Convert CIToken in String (without Location).
 * @note This function is only used to show the token in the parser.
 */
String *
to_string__CIToken(CIToken *self);

/**
 *
 * @brief Check if the kind of token is a conditional preprocessor.
 */
bool
is_conditional_preprocessor__CITokenKind(enum CITokenKind kind);

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

#define CI_TOKENS_ITERS_MAX_SIZE 512

typedef struct CITokensIter
{
    VecIter iter;
    struct
    {
        Usize count;
        bool in_use;
    } peek;
} CITokensIter;

/**
 *
 * @brief Construct CITokensIter type.
 */
CONSTRUCTOR(CITokensIter *, CITokensIter, const Vec *vec);

/**
 *
 * @brief Free CITokensIter type.
 */
inline DESTRUCTOR(CITokensIter, CITokensIter *self)
{
    lily_free(self);
}

typedef struct CITokensIters
{
    Stack *iters; // Stack<CITokensIter*>*
    // NOTE: The `current_iter` is supposed to be NULL at then end of the
    // process (parsing).
    CIToken *current_token;  // CIToken*? (&)
    CIToken *previous_token; // CIToken*? (&)
} CITokensIters;

/**
 *
 * @brief Construct CITokensIters type.
 */
inline CONSTRUCTOR(CITokensIters, CITokensIters)
{
    return (CITokensIters){ .iters = NEW(Stack, CI_TOKENS_ITERS_MAX_SIZE),
                            .current_token = NULL,
                            .previous_token = NULL };
}

/**
 *
 * @brief Add new iter from iters.
 */
inline void
add_iter__CITokensIters(const CITokensIters *self, CITokensIter *iter)
{
    push__Stack(self->iters, iter);
}

/**
 *
 * @brief Pop iter from iters.
 */
void
pop_iter__CITokensIters(CITokensIters *self);

/**
 *
 * @brief Free CITokensIters type.
 */
DESTRUCTOR(CITokensIters, const CITokensIters *self);

#endif // LILY_CORE_CC_CI_TOKEN_H
