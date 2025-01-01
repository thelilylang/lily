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

#ifndef LILY_CORE_CC_CI_TOKEN_H
#define LILY_CORE_CC_CI_TOKEN_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/rc.h>
#include <base/stack.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/cc/ci/extensions/__has_feature.h>
#include <core/cc/ci/features.h>
#include <core/shared/location.h>

typedef struct CIResultFile CIResultFile;
struct CIToken;

#define CI_N_KEYWORD (CI_TOKEN_KIND_KEYWORD_MAX - CI_TOKEN_KIND_KEYWORD_MIN - 1)

#define CI_N_ATTRIBUTE 8

#define CI_N_STANDARD_PREDEFINED_MACRO 7

#define CI_N_BUILTIN_MACRO 1

#define CI_N_PREPROCESSOR 16

#define CI_MACROS_CALL_MAX_SIZE 512

typedef struct CITokens
{
    struct CIToken *first; // CIToken*?
    struct CIToken *last;  // CIToken*? (&)
} CITokens;

/**
 *
 * @brief Construct CITokens type.
 */
inline CONSTRUCTOR(CITokens, CITokens)
{
    return (CITokens){ .first = NULL, .last = NULL };
}

/**
 *
 * @brief Add a token to the CITokens type.
 */
void
add__CITokens(CITokens *self, struct CIToken *token);

/**
 *
 * @brief Insert `token` after the match of `match` token.
 * @return true if the `token`, is inserted otherwise return false.
 */
bool
insert_after_match__CITokens(CITokens *self,
                             struct CIToken *match,
                             struct CIToken *token);

/**
 *
 * @brief Remove the matched token (if exists).
 * @return CIToken*?
 */
struct CIToken *
remove_when_match__CITokens(CITokens *self, struct CIToken *match);

/**
 *
 * @brief Check if CITokens type is empty.
 */
inline bool
is_empty__CITokens(const CITokens *self)
{
    return !self->first && !self->last;
}

/**
 *
 * @brief Convert CITokens in String (without Location).
 * @note This function is only used to show the token in the parser.
 */
String *
to_string__CITokens(const CITokens *self);

/**
 *
 * @brief Convert CIToken in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokens, const CITokens *self);
#endif

/**
 *
 * @brief Print debug CIToken struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, CITokens, const CITokens *self);
#endif

/**
 *
 * @brief Free CITokens type.
 */
DESTRUCTOR(CITokens, const CITokens *self);

// NOTE#1: Used only in the scanner
// See: https://en.cppreference.com/w/cpp/language/types
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
    CI_TOKEN_KIND_EOF, // End Of File
    CI_TOKEN_KIND_EOT, // End Of Token (only a transition token)
    CI_TOKEN_KIND_EQ,
    CI_TOKEN_KIND_EQ_EQ,
    CI_TOKEN_KIND_GNU_ATTRIBUTE, // __attribute__((x))
    CI_TOKEN_KIND_HASHTAG,
    CI_TOKEN_KIND_HASHTAG_HASHTAG,
    CI_TOKEN_KIND_HAT,
    CI_TOKEN_KIND_HAT_EQ,
    CI_TOKEN_KIND_IDENTIFIER,
    CI_TOKEN_KIND_INTERROGATION,

    // NOTE: Never add a keyword before this variant.
    CI_TOKEN_KIND_KEYWORD_MIN,

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
    CI_TOKEN_KIND_KEYWORD_LONG,
    CI_TOKEN_KIND_KEYWORD_NULLPTR,
    CI_TOKEN_KIND_KEYWORD_REGISTER,
    CI_TOKEN_KIND_KEYWORD_RESTRICT,
    CI_TOKEN_KIND_KEYWORD_RETURN,
    CI_TOKEN_KIND_KEYWORD_SHORT,
    CI_TOKEN_KIND_KEYWORD_SIGNED,
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
    CI_TOKEN_KIND_KEYWORD_UNSIGNED,
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
    CI_TOKEN_KIND_KEYWORD___ATTRIBUTE__, // NOTE: #1
    // See https://gcc.gnu.org/onlinedocs/gcc/Alternate-Keywords.html
    CI_TOKEN_KIND_KEYWORD___EXTENSION__,
    // See https://gcc.gnu.org/onlinedocs/gcc/Restricted-Pointers.html
    CI_TOKEN_KIND_KEYWORD___RESTRICT,
    CI_TOKEN_KIND_KEYWORD___RESTRICT__,

    // NOTE: Never add a keyword after this variant.
    CI_TOKEN_KIND_KEYWORD_MAX,

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
    CI_TOKEN_KIND_MACRO_DEFINED,
    CI_TOKEN_KIND_MACRO_PARAM,
#define CI_VA_ARGS "__VA_ARGS__"
    CI_TOKEN_KIND_MACRO_PARAM_VARIADIC, // __VA_ARGS__
    CI_TOKEN_KIND_MINUS,
    CI_TOKEN_KIND_MINUS_EQ,
    CI_TOKEN_KIND_MINUS_MINUS,
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
    CI_TOKEN_KIND_PREPROCESSOR_UNDEF,
    CI_TOKEN_KIND_PREPROCESSOR_WARNING,
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
    // Builtin macros:
    // https://clang.llvm.org/docs/LanguageExtensions.html#builtin-macros
    CI_TOKEN_KIND_BUILTIN_MACRO, // NOTE: #1
    CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE,
    // Standard predefined macros:
    // Link: https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
    // NOTE: We don't include `__ASSEMBLER__`, `__OBJC__` and `__cplusplus`
    // predefined macro, because as we only deal with C.
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO, // NOTE: #1
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__,
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__,
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__,
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC__,
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_VERSION__,
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_HOSTED__,
    CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__,
    CI_TOKEN_KIND_STAR,
    CI_TOKEN_KIND_STAR_EQ,
    CI_TOKEN_KIND_WAVE,
    CI_TOKEN_KIND_WAVE_EQ,

    // NOTE: Never add an enumeration variant after this variant.
    CI_TOKEN_KIND_MAX
};

enum CITokenEotContext
{
    CI_TOKEN_EOT_CONTEXT_DEFINE,          // #define ...
    CI_TOKEN_EOT_CONTEXT_STRINGIFICATION, // #<token>
    CI_TOKEN_EOT_CONTEXT_OTHER
};

typedef struct CITokenEot
{
    enum CITokenEotContext ctx;
} CITokenEot;

/**
 *
 * @brief Convert CITokenEotContext in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, CITokenEotContext, enum CITokenEotContext self);
#endif

/**
 *
 * @brief Construct CITokenEot type.
 */
inline CONSTRUCTOR(CITokenEot, CITokenEot, enum CITokenEotContext ctx)
{
    return (CITokenEot){ .ctx = ctx };
}

/**
 *
 * @brief Check if is eot break.
 */
inline bool
is_eot_break__CITokenEot(const CITokenEot *self)
{
    return self->ctx == CI_TOKEN_EOT_CONTEXT_DEFINE ||
           self->ctx == CI_TOKEN_EOT_CONTEXT_OTHER;
}

/**
 *
 * @brief Convert CITokenEot in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokenEot, const CITokenEot *self);
#endif

typedef struct CITokenGNUAttribute
{
    // content __attribute__((<X>))
    CITokens content;
} CITokenGNUAttribute;

/**
 *
 * @brief Construct CITokenGNUAttribute type.
 */
CONSTRUCTOR(CITokenGNUAttribute, CITokenGNUAttribute, CITokens content);

/**
 *
 * @brief Convert to string CITokenGNUAttribute type.
 */
String *
to_string__CITokenGNUAttribute(const CITokenGNUAttribute *self);

/**
 *
 * @brief Convert CITokenGNUAttribute in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokenGNUAttribute, const CITokenGNUAttribute *self);
#endif

/**
 *
 * @brief Free CITokenGNUAttribute type.
 */
DESTRUCTOR(CITokenGNUAttribute, const CITokenGNUAttribute *self);

enum CITokenLiteralConstantIntSuffix
{
    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_L,
    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LL,
    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LU,
    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LLU,
    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_U,
    CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE
};

typedef struct CITokenLiteralConstantInt
{
    enum CITokenLiteralConstantIntSuffix suffix;
    String *value;
} CITokenLiteralConstantInt;

/**
 *
 * @brief Construct CITokenLiteralConstantInt type.
 */
inline CONSTRUCTOR(CITokenLiteralConstantInt,
                   CITokenLiteralConstantInt,
                   enum CITokenLiteralConstantIntSuffix suffix,
                   String *value)
{
    return (CITokenLiteralConstantInt){ .suffix = suffix, .value = value };
}

/**
 *
 * @brief Convert to string CITokenLiteralConstantIntSuffix type.
 */
char *
to_string__CITokenLiteralConstantIntSuffix(
  enum CITokenLiteralConstantIntSuffix self);

/**
 *
 * @brief Convert CITokenLiteralConstantIntSuffix in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantIntSuffix,
               enum CITokenLiteralConstantIntSuffix self);
#endif

/**
 *
 * @brief Convert CITokenLiteralConstantInt in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantInt,
               const CITokenLiteralConstantInt *self);
#endif

/**
 *
 * @brief Free CITokenLiteralConstantInt type.
 */
inline DESTRUCTOR(CITokenLiteralConstantInt,
                  const CITokenLiteralConstantInt *self)
{
    FREE(String, self->value);
}

enum CITokenLiteralConstantFloatSuffix
{
    CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_F,
    CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_L,
    CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_NONE
};

typedef struct CITokenLiteralConstantFloat
{
    enum CITokenLiteralConstantFloatSuffix suffix;
    String *value;
} CITokenLiteralConstantFloat;

/**
 *
 * @brief Construct CITokenLiteralConstantFloat type.
 */
inline CONSTRUCTOR(CITokenLiteralConstantFloat,
                   CITokenLiteralConstantFloat,
                   enum CITokenLiteralConstantFloatSuffix suffix,
                   String *value)
{
    return (CITokenLiteralConstantFloat){ .suffix = suffix, .value = value };
}

/**
 *
 * @brief Convert to string CITokenLiteralConstantFloatSuffix type.
 */
char *
to_string__CITokenLiteralConstantFloatSuffix(
  enum CITokenLiteralConstantFloatSuffix self);

/**
 *
 * @brief Convert CITokenLiteralConstantFloatSuffix in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantFloatSuffix,
               enum CITokenLiteralConstantFloatSuffix self);
#endif

/**
 *
 * @brief Convert CITokenLiteralConstantFloat in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenLiteralConstantFloat,
               const CITokenLiteralConstantFloat *self);
#endif

/**
 *
 * @brief Free CITokenLiteralConstantFloat type.
 */
inline DESTRUCTOR(CITokenLiteralConstantFloat,
                  const CITokenLiteralConstantFloat *self)
{
    FREE(String, self->value);
}

typedef struct CITokenPreprocessorDefineParam
{
    String *name; // String*?
    bool is_variadic;
    bool is_used;
} CITokenPreprocessorDefineParam;

/**
 *
 * @brief Construct CITokenPreprocessorDefineParam type (is_variadic=true).
 */
VARIANT_CONSTRUCTOR(CITokenPreprocessorDefineParam *,
                    CITokenPreprocessorDefineParam,
                    variadic);

/**
 *
 * @brief Construct CITokenPreprocessorDefineParam type (is_variadic=false).
 */
VARIANT_CONSTRUCTOR(CITokenPreprocessorDefineParam *,
                    CITokenPreprocessorDefineParam,
                    normal,
                    String *name);

/**
 *
 * @brief Convert to string CITokenPreprocessorDefineParam type.
 */
String *
to_string__CITokenPreprocessorDefineParam(
  const CITokenPreprocessorDefineParam *self);

/**
 *
 * @brief Convert CITokenPreprocessorDefineParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenPreprocessorDefineParam,
               const CITokenPreprocessorDefineParam *self);
#endif

/**
 *
 * @brief Free Free CITokenPreprocessorDefineParam type.
 */
DESTRUCTOR(CITokenPreprocessorDefineParam,
           CITokenPreprocessorDefineParam *self);

typedef struct CITokenPreprocessorDefine
{
    String *name;
    Vec *params; // Vec<CITokenPreprocessorDefineParam*>*?
    CITokens tokens;
    bool is_variadic;
} CITokenPreprocessorDefine;

/**
 *
 * @brief Construct CITokenPreprocessorDefine type.
 */
inline CONSTRUCTOR(CITokenPreprocessorDefine,
                   CITokenPreprocessorDefine,
                   String *name,
                   Vec *params,
                   CITokens tokens,
                   bool is_variadic)
{
    return (CITokenPreprocessorDefine){ .name = name,
                                        .params = params,
                                        .tokens = tokens,
                                        .is_variadic = is_variadic };
}

/**
 *
 * @brief Convert to string CITokenPreprocessorDefine type.
 */
String *
to_string__CITokenPreprocessorDefine(const CITokenPreprocessorDefine *self);

/**
 *
 * @brief Get the index of the first variadic macro param. If the
 * macro has no variadic parameter, the value `-1` is assigned.
 *
 * @example
 *
 * #define A(a, ...)
 *
 * A(1, 2, 3, 4, 5)
 *
 * index=1
 *
 * #define B(a)
 *
 * B(1)
 *
 * index=-1
 *
 * #define C(a, b, c, ...)
 *
 * C(1, 2, 3, 4, 5)
 *
 * index=3
 */
Isize
get_variadic_param_index__CITokenPreprocessorDefine(
  const CITokenPreprocessorDefine *self);

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
    CITokens content;
} CITokenPreprocessorEmbed;

/**
 *
 * @brief Construct CITokenPreprocessorEmbed type.
 */
inline CONSTRUCTOR(CITokenPreprocessorEmbed,
                   CITokenPreprocessorEmbed,
                   String *value,
                   CITokens content)
{
    return (CITokenPreprocessorEmbed){ .value = value, .content = content };
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
    CITokens cond;
    CITokens content;
} CITokenPreprocessorIf;

/**
 *
 * @brief Construct CITokenPreprocessorIf type.
 */
inline CONSTRUCTOR(CITokenPreprocessorIf,
                   CITokenPreprocessorIf,
                   CITokens cond,
                   CITokens content)
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
                   CITokens cond,
                   CITokens content)
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
inline DESTRUCTOR(CITokenPreprocessorElif, const CITokenPreprocessorElif *self)
{
    FREE(CITokenPreprocessorIf, self);
}

typedef struct CITokenPreprocessorIfdef
{
    String *identifier;
    CITokens content;
} CITokenPreprocessorIfdef;

/**
 *
 * @brief Construct CITokenPreprocessorIfdef type.
 */
inline CONSTRUCTOR(CITokenPreprocessorIfdef,
                   CITokenPreprocessorIfdef,
                   String *identifier,
                   CITokens content)
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
                   CITokens content)
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
                   CITokens content)
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
                   CITokens content)
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
    CITokens content;
} CITokenPreprocessorElse;

/**
 *
 * @brief Construct CITokenPreprocessorElse type.
 */
inline CONSTRUCTOR(CITokenPreprocessorElse,
                   CITokenPreprocessorElse,
                   CITokens content)
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

typedef struct CITokenMacroCallId
{
    Usize id;
} CITokenMacroCallId;

/**
 *
 * @brief Construct CITokenMacroCallId type.
 */
CONSTRUCTOR(CITokenMacroCallId *, CITokenMacroCallId, Usize id);

/**
 *
 * @brief Free CITokenMacroCallId type.
 */
inline DESTRUCTOR(CITokenMacroCallId, CITokenMacroCallId *self)
{
    lily_free(self);
}

typedef struct CITokenMacroParam
{
    // The `id` corresponds to the position index in the macro
    // (#define) params vector (preprocessor_define.params).
    Usize id;
    Stack *macro_call_ids; // Stack<CITokenMacroParamCallId*>*
} CITokenMacroParam;

/**
 *
 * @brief Construct CITokenMacroParam type.
 */
inline CONSTRUCTOR(CITokenMacroParam, CITokenMacroParam, Usize id)
{
    return (CITokenMacroParam){ .id = id,
                                .macro_call_ids =
                                  NEW(Stack, CI_MACROS_CALL_MAX_SIZE) };
}

/**
 *
 * @brief Convert CITokenMacroParam in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, CITokenMacroParam, const CITokenMacroParam *self);
#endif

/**
 *
 * @brief Free CITokenMacroParam type.
 */
inline DESTRUCTOR(CITokenMacroParam, const CITokenMacroParam *self)
{
    // NOTE: It's not necessary to free items from the stack, because normally
    // at the end of the program there will be no more items in the stack.
    FREE(Stack, self->macro_call_ids);
}

typedef struct CITokenMacroParamVariadic
{
    Stack *macro_call_ids; // Stack<CITokenMacroParamCallId*>*
} CITokenMacroParamVariadic;

/**
 *
 * @brief Construct CITokenMacroParamVariadic type.
 */
inline CONSTRUCTOR(CITokenMacroParamVariadic, CITokenMacroParamVariadic)
{
    return (CITokenMacroParamVariadic){ .macro_call_ids =
                                          NEW(Stack, CI_MACROS_CALL_MAX_SIZE) };
}

/**
 *
 * @brief Convert CITokenMacroParamVariadic in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               CITokenMacroParamVariadic,
               const CITokenMacroParamVariadic *self);
#endif

/**
 *
 * @brief Free CITokenMacroParamVariadic type.
 */
inline DESTRUCTOR(CITokenMacroParamVariadic,
                  const CITokenMacroParamVariadic *self)
{
    // NOTE: It's not necessary to free items from the stack, because normally
    // at the end of the program there will be no more items in the stack.
    FREE(Stack, self->macro_call_ids);
}

typedef struct CIToken
{
    enum CITokenKind kind;
    Location location;
    struct CIToken *next; // struct CIToken*? (&)
    Usize ref_count;
    union
    {
        String *attribute_deprecated; // String*?
        String *attribute_nodiscard;  // String*?
        String *comment_doc;
        CITokenEot eot;
        CITokenGNUAttribute gnu_attribute;
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
        Rc *identifier; // Rc<String*>*
        CITokenLiteralConstantInt literal_constant_int;
        CITokenLiteralConstantFloat literal_constant_float;
        CITokenLiteralConstantInt literal_constant_octal;
        CITokenLiteralConstantInt literal_constant_hex;
        CITokenLiteralConstantInt literal_constant_bin;
        char literal_constant_character;
        Rc *literal_constant_string; // Rc<String*>*
        String *macro_defined;
        CITokenMacroParam macro_param;
        CITokenMacroParamVariadic macro_param_variadic;
        String *standard_predefined_macro___date__;
        String *standard_predefined_macro___time__;
        enum CIExtensionsHasFeature has_feature;
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
 * @brief Construct CIToken type (CI_TOKEN_KIND_EOT).
 */
VARIANT_CONSTRUCTOR(CIToken *, CIToken, eot, Location location, CITokenEot eot);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_GNU_ATTRIBUTE).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    gnu_attribute,
                    Location location,
                    CITokenGNUAttribute gnu_attribute);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_IDENTIFIER).
 * @param identifier Rc<String*>*
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    identifier,
                    Location location,
                    Rc *identifier);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_INT).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_int,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_int);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_float,
                    Location location,
                    CITokenLiteralConstantFloat literal_constant_float);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_octal,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_octal);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_HEX).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_hex,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_hex);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_LITERAL_CONSTANT_BIN).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_bin,
                    Location location,
                    CITokenLiteralConstantInt literal_constant_bin);

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
 * @param literal_constant_string Rc<String*>*
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    literal_constant_string,
                    Location location,
                    Rc *literal_constant_string);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_MACRO_PARAM).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_param,
                    Location location,
                    CITokenMacroParam macro_param);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_MACRO_PARAM_VARIADIC).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_param_variadic,
                    Location location,
                    CITokenMacroParamVariadic macro_param_variadic);

/**
 *
 * @brief Construct CIToken type (CI_TOKEN_KIND_MACRO_DEFINED).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    macro_defined,
                    Location location,
                    String *macro_defined);

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
 * @brief Construct CIToken type (CI_TOKEN_KIND_PREPROCESSOR_UNDEF).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    preprocessor_undef,
                    Location location,
                    String *preprocessor_undef);

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
 * @brief Construct CIToken type (CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    builtin_macro_has_feature,
                    Location location,
                    enum CIExtensionsHasFeature has_feature);

/**
 *
 * @brief Construct CIToken type
 * (CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    standard_predefined_macro___date__,
                    Location location,
                    String *standard_predefined_macro___date__);

/**
 *
 * @brief Construct CIToken type
 * (CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__).
 */
VARIANT_CONSTRUCTOR(CIToken *,
                    CIToken,
                    standard_predefined_macro___time__,
                    Location location,
                    String *standard_predefined_macro___time__);

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
 * @brief Check if the kind of token is a preprocessor.
 */
bool
is_preprocessor__CITokenKind(enum CITokenKind kind);

/**
 *
 * @brief Check if the given token kind is a keyword.
 */
inline bool
is_keyword__CITokenKind(enum CITokenKind kind)
{
    return kind > CI_TOKEN_KIND_KEYWORD_MIN && kind < CI_TOKEN_KIND_KEYWORD_MAX;
}

/**
 *
 * @brief Increment `ref_count`.
 * @return CIToken*
 */
inline CIToken *
ref__CIToken(CIToken *self)
{
    ++self->ref_count;
    return self;
}

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
