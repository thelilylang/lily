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

#include <base/assert.h>
#include <base/atoi.h>
#include <base/macros.h>
#include <base/optional.h>
#include <base/print.h>

#include <core/cc/ci/ci.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/scanner.h>
#include <core/shared/diagnostic.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum CIKeywordMultiPart
{
    CI_KEYWORD_MULTI_PART_CHAR = 1 << 0,
    CI_KEYWORD_MULTI_PART_DOUBLE = 1 << 1,
    CI_KEYWORD_MULTI_PART_FLOAT = 1 << 2,
    CI_KEYWORD_MULTI_PART_INT = 1 << 3,
    CI_KEYWORD_MULTI_PART_LONG = 1 << 4,
    CI_KEYWORD_MULTI_PART_LONG_LONG = 1 << 5,
    CI_KEYWORD_MULTI_PART_SHORT = 1 << 6,
    CI_KEYWORD_MULTI_PART_SIGNED = 1 << 7,
    CI_KEYWORD_MULTI_PART_UNSIGNED = 1 << 8,
    CI_KEYWORD_MULTI_PART__COMPLEX = 1 << 9,
    CI_KEYWORD_MULTI_PART__IMAGINARY = 1 << 10,
};

/// @brief Check if the token kind is multi part keyword.
static bool
is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
  enum CITokenKind kind);

static enum CIKeywordMultiPart
convert_token_kind_to_multi_part_keyword__CIKeywordMultiPart(
  enum CITokenKind kind);

/// @brief Add flag to multi part keyword.
static void
add_flag__CIKeywordMultiPart(Uint32 *flags, enum CIKeywordMultiPart flag);

/// @brief Convert multi part keyword to token kind.
static enum CITokenKind
convert_multi_part_keyword_to_token_kind__CIKeywordMultiPart(Uint32 flags);

/// @brief Move next one character.
/// @see `include/core/shared/scanner.h`
static inline void
next_char__CIScanner(CIScanner *self);

/// @brief Move back one character.
/// @see `include/core/shared/scanner.h`
static inline void
previous_char__CIScanner(CIScanner *self);

/// @brief Check if the current is space (new line, tab, ...).
/// @see `include/core/shared/scanner.h`
static inline bool
is_space__CIScanner(CIScanner *self);

/// @brief Check if the peeked char is space (new line, tab, ...).
/// @see `include/core/shared/scanner.h`
static inline bool
is_space_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *self,
                                     const char *c);

/// @brief Skip space (new line, tab, ...).
/// @see `include/core/shared/scanner.h`
static inline void
skip_space__CIScanner(CIScanner *self);

/// @brief Skip space, but except new line (tab, ...).
/// @see `include/core/shared/scanner.h`
static inline void
skip_space_except_new_line__CIScanner(CIScanner *self);

/// @brief Next char n times.
/// @see `include/core/shared/scanner.h`
static inline void
jump__CIScanner(CIScanner *self, const Usize n);

/// @brief Assign to line and column to start_line and start_column Location's
/// field.
/// @see `include/core/shared/scanner.h`
static inline void
start_token__CIScanner(CIScanner *self,
                       const Usize line,
                       const Usize column,
                       const Usize position);

/// @brief Assign to line and column to end_line and end_column Location's
/// field.
/// @see `include/core/shared/scanner.h`
static inline void
end_token__CIScanner(CIScanner *self,
                     const Usize line,
                     const Usize column,
                     const Usize position);

/// @brief Get character at position + n.
/// @see `include/core/shared/scanner.h`
static inline char *
peek_char__CIScanner(const CIScanner *self, const Usize n);

/// @brief Scan and append characters to res while is_valid return true.
static void
scan_and_append_chars__CIScanner(CIScanner *self,
                                 String *res,
                                 bool (*is_valid)(const CIScanner *self));

/// @brief Check if current is a digit.
static inline bool
is_digit__CIScanner(const CIScanner *self);

/// @brief Check if current can be a start of identifier.
static inline bool
is_start_ident__CIScanner(const CIScanner *self);

/// @brief Check if current can be an identifier.
static inline bool
is_ident__CIScanner(const CIScanner *self);

/// @brief Check if `c` can be a digit with peeked char (used with
/// peek_char__CIScanner function).
/// @param c char*?
static inline bool
is_digit_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *,
                                     const char *c);

/// @brief Check if `c` can be a start identifier with peeked char (used
/// with peek_char__CIScanner function).
/// @param c char*?
static inline bool
is_start_ident_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *,
                                           const char *c);

/// @brief Check if `c` can be an identifier with peeked char (used with
/// peek_char__CIScanner function).
/// @param c char*?
static inline bool
is_ident_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *,
                                     const char *c);

/// @brief Check if current can be an hexadecimal.
static inline bool
is_hex__CIScanner(const CIScanner *self);

/// @brief Check if current can be an octal.
static inline bool
is_oct__CIScanner(const CIScanner *self);

/// @brief Check if current can be an octal.
static inline bool
is_bin__CIScanner(const CIScanner *self);

/// @brief Check if current can be a binary.
static inline bool
is_num__CIScanner(const CIScanner *self);

/// @brief Add EOT token to passed scanner context.
static void
add_eot_token__CIScanner(CIScanner *self,
                         CIScannerContext *ctx,
                         CITokenEot eot);

/// @brief Add EOF token to passed scanner context.
static void
add_eof_token__CIScanner(CIScanner *self, CIScannerContext *ctx);

/// @brief Push token to tokens.
static inline void
push_token__CIScanner(CIScanner *self, CIScannerContext *ctx, CIToken *token);

/// @brief Next char according the token.
static void
next_char_by_token__CIScanner(CIScanner *self, const CIToken *token);

/// @brief Get attribute from id.
static enum CITokenKind
get_attribute__CIScanner(const String *id);

/// @brief Get builtin macro from id.
static enum CITokenKind
get_builtin_macro__CIScanner(const String *id);

/// @brief Get standard predefined macro from id.
static enum CITokenKind
get_standard_predefined_macro__CIScanner(const String *id);

/// @brief Get preprocessor from id.
static enum CITokenKind
get_preprocessor__CIScanner(const String *id);

/// @brief Get single keyword from id.
static enum CITokenKind
get_single_keyword__CIScanner(const String *id);

/// @brief Standardize keyword.
///
/// @example
///
/// short => short int
/// signed short => short int
enum CITokenKind
standardize_keyword__CIScanner(enum CITokenKind kind);

/// @brief Scan multi-part keyword.
/// @example long int, long long, unsigned long long, ...
static CIToken *
scan_multi_part_keyword__CIScanner(CIScanner *self, CIScannerContext *ctx);

/// @brief Scan attribute.
/// @example [[maybe_unused]], ...
static CIToken *
scan_attribute__CIScanner(CIScanner *self);

/// @brief Skip comment line.
static void
skip_comment_line__CIScanner(CIScanner *self);

/// @brief Skip comment block.
static void
skip_comment_block__CIScanner(CIScanner *self);

/// @brief Scan comment doc.
static String *
scan_comment_doc__CIScanner(CIScanner *self);

/// @brief Get escape character and other character.
static String *
get_character__CIScanner(CIScanner *self, char previous);

/// @brief Scan identifier.
static String *
scan_identifier__CIScanner(CIScanner *self);

/// @brief Check if identifier is standard defined macro.
static inline bool
identifier_is_standard_predefined_macro__CIScanner(CIScanner *self,
                                                   const String *name);

/// @brief Scan identifier with peek_char function (without use
/// next_char__CIScanner).
static String *
scan_identifier_with_peek_char__CIScanner(const CIScanner *self);

/// @brief Scan character constant literal.
static char *
scan_character__CIScanner(CIScanner *self);

/// @brief Scan string constant literal.
static String *
scan_string__CIScanner(CIScanner *self);

/// @brief Scan binary constant literal.
/// @example 0b111, 0b101
static CIToken *
scan_bin__CIScanner(CIScanner *self);

/// @brief Scan hexadecimal constant literal.
/// @example 0xff, 0x33
static CIToken *
scan_hex__CIScanner(CIScanner *self);

/// @brief Scan octal constant literal.
/// @example 00122, 00334
static CIToken *
scan_oct__CIScanner(CIScanner *self);

/// @brief Scan number (decimal and float).
static CIToken *
scan_num__CIScanner(CIScanner *self);

/// @brief Skip space and backslash
static void
skip_space_and_backslash__CIScanner(CIScanner *self);

/// @brief Skip backslash.
static void
skip_backslash__CIScanner(CIScanner *self);

/// @brief In some cases we want to skip spaces and backslashes when we're
/// inside a macro, and if not, we skip all spaces.
static void
skip_space_according_ctx__CIScanner(CIScanner *self,
                                    const CIScannerContext *ctx);

/// @brief Scan macro name of #ifdef, #ifndef, #elifdef, #elifndef, #define or
/// #undef preprocessor.
/// @example
///
/// #ifdef <macro_name>
/// ...
/// #endif
///
/// #ifndef <macro_name>
/// ...
/// #endif
///
/// #elifdef <macro_name>
/// ...
/// #endif
///
/// #elifndef <macro_name>
/// ...
/// #endif
///
/// #define <macro_name>(...) ...
///
/// #undef <macro_name>
///
/// @note If there is an error, we give the macro name an arbitrary value.
/// @param ctx_parent CIScannerContext*?
/// @param preprocessor_ctx CI_TOKEN_KIND_PREPROCESSOR_IFDEF (same value for
/// #ifndef, #elifdef or #elifndef preprocessor case) |
/// CI_TOKEN_KIND_PREPROCESSOR_DEFINE | CI_TOKEN_KIND_PREPROCESSOR_UNDEF
/// @return String*
static String *
scan_macro_name__CIScanner(CIScanner *self,
                           CIScannerContext *ctx,
                           CIScannerContext *ctx_parent,
                           enum CITokenKind preprocessor_ctx);

/// @brief Scan defined preprcoessor params.
/// @return Vec<CITokenPreprocessorDefineParam*>*
static Vec *
scan_define_preprocessor_params__CIScanner(CIScanner *self,
                                           bool *macro_is_variadic);

/// @brief Scan preprocessor content.
/// @return CIToken*
static CITokens
scan_preprocessor_content__CIScanner(CIScanner *self,
                                     enum CIScannerContextLocation ctx_location,
                                     Vec *params);

/// @brief Scan define preprocessor.
static CIToken *
scan_define_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan elif preprocessor.
static CIToken *
scan_elif_preprocessor__CIScanner(CIScanner *self,
                                  CIScannerContext *ctx_parent);

/// @brief Scan elifdef preprocessor.
static CIToken *
scan_elifdef_preprocessor__CIScanner(CIScanner *self,
                                     CIScannerContext *ctx_parent);

/// @brief Scan elifndef preprocessor.
static CIToken *
scan_elifndef_preprocessor__CIScanner(CIScanner *self,
                                      CIScannerContext *ctx_parent);

/// @brief Scan else preprocessor.
static CIToken *
scan_else_preprocessor__CIScanner(CIScanner *self,
                                  CIScannerContext *ctx_parent);

/// @brief Scan embed preprocessor.
static CIToken *
scan_embed_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan error preprocessor.
static CIToken *
scan_error_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan if preprocessor.
static CIToken *
scan_if_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan ifdef preprocessor.
static CIToken *
scan_ifdef_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan ifndef preprocessor.
static CIToken *
scan_ifndef_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan include preprocessor.
static CIToken *
scan_include_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan line preprocessor.
static CIToken *
scan_line_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan pragma preprocessor.
static CIToken *
scan_pragma_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan undef preprocessor.
static CIToken *
scan_undef_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan warning preprocessor.
static CIToken *
scan_warning_preprocessor__CIScanner(CIScanner *self);

/// @brief Scan GNU attribute.
static CIToken *
scan_gnu_attribute__CIScanner(CIScanner *self,
                              CIScannerContext *ctx,
                              Location gnu_attribute_location);

/// @brief Scan all numbers (hexadecimal, octal, binary, decimal and float).
static CIToken *
get_num__CIScanner(CIScanner *self);

/// @brief Get token from characters.
/// @param in_macro If `in_macro` is true, the scanner checks whether the
/// paren, brace or bracket are closed.
/// @param in_prepro_cond If `in_prepro_cond` is true, the scanner allow to use
/// #elif, #elifdef, #elifndef, #else.
/// @param ctx CIScannerContext* (&)
/// @param ctx_parent CIScannerContext*? (&)
static CIToken *
get_token__CIScanner(CIScanner *self,
                     CIScannerContext *ctx,
                     CIScannerContext *ctx_parent);

#define HAS_REACH_END(self) SCANNER_HAS_REACH_END((self)->base)

static const CIFeature tokens_feature[CI_TOKEN_KIND_MAX] = {
    [CI_TOKEN_KIND_AMPERSAND] = { .since = CI_STANDARD_NONE,
                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_AMPERSAND_AMPERSAND] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_AMPERSAND_EQ] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ARROW] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_AT] = { .since = CI_STANDARD_NONE,
                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED] = { .since = CI_STANDARD_23,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH] = { .since = CI_STANDARD_23,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED] = { .since = CI_STANDARD_23,
                                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE_NODISCARD] = { .since = CI_STANDARD_23,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE_NORETURN] = { .since = CI_STANDARD_23,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE__NORETURN] = { .since = CI_STANDARD_23,
                                            .until = CI_STANDARD_23 },
    [CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED] = { .since = CI_STANDARD_23,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE] = { .since = CI_STANDARD_23,
                                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BANG] = { .since = CI_STANDARD_NONE,
                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BANG_EQ] = { .since = CI_STANDARD_NONE,
                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BAR] = { .since = CI_STANDARD_NONE,
                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BAR_BAR] = { .since = CI_STANDARD_NONE,
                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BAR_EQ] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_COLON] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_COMMA] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_COMMENT_LINE] = { .since = CI_STANDARD_99,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_COMMENT_BLOCK] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_COMMENT_DOC] = { .since = CI_STANDARD_NONE,
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_DOT] = { .since = CI_STANDARD_NONE,
                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_DOT_DOT_DOT] = { .since = CI_STANDARD_NONE,
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_EOF] = { .since = CI_STANDARD_NONE,
                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_EOT] = { .since = CI_STANDARD_NONE,
                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_EQ] = { .since = CI_STANDARD_NONE,
                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_EQ_EQ] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_HASHTAG] = { .since = CI_STANDARD_NONE,
                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_HASHTAG_HASHTAG] = { .since = CI_STANDARD_NONE,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_HAT] = { .since = CI_STANDARD_NONE,
                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_HAT_EQ] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_IDENTIFIER] = { .since = CI_STANDARD_NONE,
                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_INTERROGATION] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_ALIGNAS] = { .since = CI_STANDARD_23,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_ALIGNOF] = { .since = CI_STANDARD_23,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_ASM] = { .since =
                                      CI_STANDARD_NONE, // NOTE: The asm keyword
                                                        // is an extension, so
                                                        // in some compilers it
                                                        // may be potentially
                                                        // incompatible.
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_AUTO] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_BOOL] = { .since = CI_STANDARD_23,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_BREAK] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_CASE] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_CHAR] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_CONST] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_CONSTEXPR] = { .since = CI_STANDARD_23,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_CONTINUE] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_DEFAULT] = { .since = CI_STANDARD_NONE,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_DO] = { .since = CI_STANDARD_NONE,
                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_DOUBLE] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX] = { .since = CI_STANDARD_99,
                                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY] = { .since = CI_STANDARD_99,
                                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_ELSE] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_ELSE_IF] = { .since = CI_STANDARD_NONE,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_ENUM] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_EXTERN] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_FALSE] = { .since = CI_STANDARD_23,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_FLOAT] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX] = { .since = CI_STANDARD_99,
                                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY] = { .since = CI_STANDARD_99,
                                                 .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_FOR] = { .since = CI_STANDARD_NONE,
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_GOTO] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_IF] = { .since = CI_STANDARD_NONE,
                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_INLINE] = { .since = CI_STANDARD_99,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_INT] = { .since = CI_STANDARD_NONE,
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX] = { .since = CI_STANDARD_99,
                                                     .until =
                                                       CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY] = { .since = CI_STANDARD_99,
                                                       .until =
                                                         CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG_INT] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG_LONG] = { .since = CI_STANDARD_99,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT] = { .since = CI_STANDARD_99,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_NULLPTR] = { .since = CI_STANDARD_23,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_REGISTER] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_RESTRICT] = { .since = CI_STANDARD_99,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_RETURN] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SHORT] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SHORT_INT] = { .since = CI_STANDARD_NONE,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_INT] = { .since = CI_STANDARD_NONE,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_LONG] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT] = { .since = CI_STANDARD_NONE,
                                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG] = { .since = CI_STANDARD_99,
                                                 .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT] = { .since = CI_STANDARD_99,
                                                     .until =
                                                       CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT] = { .since = CI_STANDARD_NONE,
                                                 .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SIZEOF] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_STATIC] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_STATIC_ASSERT] = { .since = CI_STANDARD_23,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_STRUCT] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_SWITCH] = { .since = CI_STANDARD_NONE,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL] = { .since = CI_STANDARD_23,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_TRUE] = { .since = CI_STANDARD_23,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_TYPEDEF] = { .since = CI_STANDARD_NONE,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_TYPEOF] = { .since = CI_STANDARD_23,
                                       .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_TYPEOF_UNQUAL] = { .since = CI_STANDARD_23,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNION] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR] = { .since = CI_STANDARD_NONE,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG] = { .since = CI_STANDARD_NONE,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT] = { .since = CI_STANDARD_NONE,
                                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG] = { .since = CI_STANDARD_99,
                                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT] = { .since = CI_STANDARD_99,
                                                       .until =
                                                         CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT] = { .since = CI_STANDARD_NONE,
                                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT] = { .since = CI_STANDARD_NONE,
                                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_VOID] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_VOLATILE] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD_WHILE] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__ALIGNAS] = { .since = CI_STANDARD_11,
                                         .until = CI_STANDARD_23 },
    [CI_TOKEN_KIND_KEYWORD__ALIGNOF] = { .since = CI_STANDARD_11,
                                         .until = CI_STANDARD_23 },
    [CI_TOKEN_KIND_KEYWORD__ATOMIC] = { .since = CI_STANDARD_11,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__BITINT] = { .since = CI_STANDARD_23,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__BOOL] = { .since = CI_STANDARD_99,
                                      .until = CI_STANDARD_23 },
    [CI_TOKEN_KIND_KEYWORD__COMPLEX] = { .since = CI_STANDARD_99,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__DECIMAL128] = { .since = CI_STANDARD_23,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__DECIMAL32] = { .since = CI_STANDARD_23,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__DECIMAL64] = { .since = CI_STANDARD_23,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__GENERIC] = { .since = CI_STANDARD_11,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__IMAGINARY] = { .since = CI_STANDARD_99,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__NORETURN] = { .since = CI_STANDARD_11,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_KEYWORD__STATIC_ASSERT] = { .since = CI_STANDARD_11,
                                               .until = CI_STANDARD_23 },
    [CI_TOKEN_KIND_KEYWORD__THREAD_LOCAL] = { .since = CI_STANDARD_11,
                                              .until = CI_STANDARD_23 },
    [CI_TOKEN_KIND_KEYWORD___ATTRIBUTE__] = { .since = CI_STANDARD_NONE,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LBRACE] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LHOOK] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LPAREN] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LSHIFT] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LSHIFT_EQ] = { .since = CI_STANDARD_NONE,
                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LSHIFT_LSHIFT] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_INT] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT] = { .since = CI_STANDARD_NONE,
                                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL] = { .since = CI_STANDARD_NONE,
                                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_HEX] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_BIN] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_CHARACTER] = { .since = CI_STANDARD_NONE,
                                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_LITERAL_CONSTANT_STRING] = { .since = CI_STANDARD_NONE,
                                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_MACRO_DEFINED] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_MACRO_PARAM] = { .since = CI_STANDARD_NONE,
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_MINUS] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_MINUS_EQ] = { .since = CI_STANDARD_NONE,
                                 .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_MINUS_MINUS] = { .since = CI_STANDARD_NONE,
                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PERCENTAGE] = { .since = CI_STANDARD_NONE,
                                   .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PERCENTAGE_EQ] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PLUS] = { .since = CI_STANDARD_NONE,
                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PLUS_EQ] = { .since = CI_STANDARD_NONE,
                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PLUS_PLUS] = { .since = CI_STANDARD_NONE,
                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_DEFINE] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_ELIF] = { .since = CI_STANDARD_NONE,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF] = { .since = CI_STANDARD_23,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF] = { .since = CI_STANDARD_23,
                                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_ELSE] = { .since = CI_STANDARD_NONE,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_EMBED] = { .since = CI_STANDARD_23,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_ENDIF] = { .since = CI_STANDARD_NONE,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_ERROR] = { .since = CI_STANDARD_NONE,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_IF] = { .since = CI_STANDARD_NONE,
                                        .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_IFDEF] = { .since = CI_STANDARD_NONE,
                                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_IFNDEF] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_INCLUDE] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_LINE] = { .since = CI_STANDARD_NONE,
                                          .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_PRAGMA] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_PREPROCESSOR_UNDEF] = { .since = CI_STANDARD_NONE,
                                           .until = CI_STANDARD_NONE },
    // NOTE: Normally, the preprocessor `#warning` is only supported in C23, but
    // it seems that Clang and GCC use it as an extension so that this feature
    // is valid in all standards.
    [CI_TOKEN_KIND_PREPROCESSOR_WARNING] = { .since = CI_STANDARD_NONE,
                                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RBRACE] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RHOOK] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RPAREN] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RSHIFT] = { .since = CI_STANDARD_NONE,
                               .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RSHIFT_EQ] = { .since = CI_STANDARD_NONE,
                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RSHIFT_RSHIFT] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ] = { .since = CI_STANDARD_NONE,
                                         .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_SEMICOLON] = { .since = CI_STANDARD_NONE,
                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_SLASH] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_SLASH_EQ] = { .since = CI_STANDARD_NONE,
                                 .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BUILTIN_MACRO] = { .since = CI_STANDARD_NONE,
                                      .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE] = { .since = CI_STANDARD_NONE,
                                                    .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO] = { .since = CI_STANDARD_NONE,
                                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__] = { .since =
                                                             CI_STANDARD_NONE,
                                                           .until =
                                                             CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__] = { .since =
                                                             CI_STANDARD_NONE,
                                                           .until =
                                                             CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__] = { .since =
                                                             CI_STANDARD_NONE,
                                                           .until =
                                                             CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC__] = { .since =
                                                             CI_STANDARD_NONE,
                                                           .until =
                                                             CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_VERSION__] = { .since =
                                                                     CI_STANDARD_NONE,
                                                                   .until =
                                                                     CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_HOSTED__] = { .since =
                                                                    CI_STANDARD_NONE,
                                                                  .until =
                                                                    CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__] = { .since =
                                                             CI_STANDARD_NONE,
                                                           .until =
                                                             CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STAR] = { .since = CI_STANDARD_NONE,
                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_STAR_EQ] = { .since = CI_STANDARD_NONE,
                                .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_WAVE] = { .since = CI_STANDARD_NONE,
                             .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_WAVE_EQ] = { .since = CI_STANDARD_NONE,
                                .until = CI_STANDARD_NONE },
};

// NOTE: This table must be sorted in ascending order.
static const SizedStr ci_single_keywords[CI_N_SINGLE_KEYWORD] = {
    SIZED_STR_FROM_RAW("_Alignas"),
    SIZED_STR_FROM_RAW("_Alignof"),
    SIZED_STR_FROM_RAW("_Atomic"),
    SIZED_STR_FROM_RAW("_BitInt"),
    SIZED_STR_FROM_RAW("_Bool"),
    SIZED_STR_FROM_RAW("_Complex"),
    SIZED_STR_FROM_RAW("_Decimal128"),
    SIZED_STR_FROM_RAW("_Decimal32"),
    SIZED_STR_FROM_RAW("_Decimal64"),
    SIZED_STR_FROM_RAW("_Generic"),
    SIZED_STR_FROM_RAW("_Imaginary"),
    SIZED_STR_FROM_RAW("_Noreturn"),
    SIZED_STR_FROM_RAW("_Static_assert"),
    SIZED_STR_FROM_RAW("_Thread_local"),
    SIZED_STR_FROM_RAW("__attribute__"),
    SIZED_STR_FROM_RAW("alignas"),
    SIZED_STR_FROM_RAW("alignof"),
    SIZED_STR_FROM_RAW("asm"),
    SIZED_STR_FROM_RAW("auto"),
    SIZED_STR_FROM_RAW("bool"),
    SIZED_STR_FROM_RAW("break"),
    SIZED_STR_FROM_RAW("case"),
    SIZED_STR_FROM_RAW("char"),
    SIZED_STR_FROM_RAW("const"),
    SIZED_STR_FROM_RAW("constexpr"),
    SIZED_STR_FROM_RAW("continue"),
    SIZED_STR_FROM_RAW("default"),
    SIZED_STR_FROM_RAW("do"),
    SIZED_STR_FROM_RAW("double"),
    SIZED_STR_FROM_RAW("else"),
    SIZED_STR_FROM_RAW("enum"),
    SIZED_STR_FROM_RAW("extern"),
    SIZED_STR_FROM_RAW("false"),
    SIZED_STR_FROM_RAW("float"),
    SIZED_STR_FROM_RAW("for"),
    SIZED_STR_FROM_RAW("goto"),
    SIZED_STR_FROM_RAW("if"),
    SIZED_STR_FROM_RAW("inline"),
    SIZED_STR_FROM_RAW("int"),
    SIZED_STR_FROM_RAW("long"),
    SIZED_STR_FROM_RAW("nullptr"),
    SIZED_STR_FROM_RAW("register"),
    SIZED_STR_FROM_RAW("restrict"),
    SIZED_STR_FROM_RAW("return"),
    SIZED_STR_FROM_RAW("short"),
    SIZED_STR_FROM_RAW("signed"),
    SIZED_STR_FROM_RAW("sizeof"),
    SIZED_STR_FROM_RAW("static"),
    SIZED_STR_FROM_RAW("static_assert"),
    SIZED_STR_FROM_RAW("struct"),
    SIZED_STR_FROM_RAW("switch"),
    SIZED_STR_FROM_RAW("thread_local"),
    SIZED_STR_FROM_RAW("true"),
    SIZED_STR_FROM_RAW("typedef"),
    SIZED_STR_FROM_RAW("typeof"),
    SIZED_STR_FROM_RAW("typeof_unqual"),
    SIZED_STR_FROM_RAW("union"),
    SIZED_STR_FROM_RAW("unsigned"),
    SIZED_STR_FROM_RAW("void"),
    SIZED_STR_FROM_RAW("volatile"),
    SIZED_STR_FROM_RAW("while"),
};

// NOTE: This array must have the same order as the ci_single_keywords array.
static const enum CITokenKind ci_single_keyword_ids[CI_N_SINGLE_KEYWORD] = {
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
    CI_TOKEN_KIND_KEYWORD___ATTRIBUTE__,
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
    CI_TOKEN_KIND_KEYWORD_WHILE
};

// NOTE: This table must be sorted in ascending order.
static const SizedStr ci_attributes[CI_N_ATTRIBUTE] = {
    SIZED_STR_FROM_RAW("_Noreturn"),    SIZED_STR_FROM_RAW("deprecated"),
    SIZED_STR_FROM_RAW("fallthrough"),  SIZED_STR_FROM_RAW("maybe_unused"),
    SIZED_STR_FROM_RAW("nodiscard"),    SIZED_STR_FROM_RAW("noreturn"),
    SIZED_STR_FROM_RAW("reproducible"), SIZED_STR_FROM_RAW("unsequenced"),
};

// NOTE: This array must have the same order as the ci_attributes array.
static const enum CITokenKind ci_attribute_ids[CI_N_ATTRIBUTE] = {
    CI_TOKEN_KIND_ATTRIBUTE__NORETURN,    CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED,
    CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH,  CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED,
    CI_TOKEN_KIND_ATTRIBUTE_NODISCARD,    CI_TOKEN_KIND_ATTRIBUTE_NORETURN,
    CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE, CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED,
};

// NOTE: This table must be sorted in ascending order.
static const SizedStr ci_builtin_macros[CI_N_BUILTIN_MACRO] = {
    SIZED_STR_FROM_RAW("__has_feature"),
};

// NOTE: This array must have the same order as the
// ci_builtin_macros array.
static const enum CITokenKind ci_builtin_macro_ids[CI_N_BUILTIN_MACRO] = {
    CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE
};

// NOTE: This table must be sorted in ascending order.
static const SizedStr
  ci_standard_predefined_macros[CI_N_STANDARD_PREDEFINED_MACRO] = {
      SIZED_STR_FROM_RAW("__DATE__"),
      SIZED_STR_FROM_RAW("__FILE__"),
      SIZED_STR_FROM_RAW("__LINE__"),
      SIZED_STR_FROM_RAW("__STDC__"),
      SIZED_STR_FROM_RAW("__STDC_HOSTED__"),
      SIZED_STR_FROM_RAW("__STDC_VERSION__"),
      SIZED_STR_FROM_RAW("__TIME__"),
  };

// NOTE: This array must have the same order as the
// ci_standard_predefined_macros array.
static const enum CITokenKind
  ci_standard_predefined_macro_ids[CI_N_STANDARD_PREDEFINED_MACRO] = {
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__,
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__,
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__,
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC__,
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_HOSTED__,
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___STDC_VERSION__,
      CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__,
  };

// NOTE: This table must be sorted in ascending order.
static const SizedStr ci_preprocessors[CI_N_PREPROCESSOR] = {
    SIZED_STR_FROM_RAW("define"),  SIZED_STR_FROM_RAW("elif"),
    SIZED_STR_FROM_RAW("elifdef"), SIZED_STR_FROM_RAW("elifndef"),
    SIZED_STR_FROM_RAW("else"),    SIZED_STR_FROM_RAW("embed"),
    SIZED_STR_FROM_RAW("endif"),   SIZED_STR_FROM_RAW("error"),
    SIZED_STR_FROM_RAW("if"),      SIZED_STR_FROM_RAW("ifdef"),
    SIZED_STR_FROM_RAW("ifndef"),  SIZED_STR_FROM_RAW("include"),
    SIZED_STR_FROM_RAW("line"),    SIZED_STR_FROM_RAW("pragma"),
    SIZED_STR_FROM_RAW("undef"),   SIZED_STR_FROM_RAW("warning"),
};

// NOTE: This array must have the same order as the ci_preprocessors array.
static const enum CITokenKind ci_preprocessor_ids[CI_N_PREPROCESSOR] = {
    CI_TOKEN_KIND_PREPROCESSOR_DEFINE,  CI_TOKEN_KIND_PREPROCESSOR_ELIF,
    CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF, CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF,
    CI_TOKEN_KIND_PREPROCESSOR_ELSE,    CI_TOKEN_KIND_PREPROCESSOR_EMBED,
    CI_TOKEN_KIND_PREPROCESSOR_ENDIF,   CI_TOKEN_KIND_PREPROCESSOR_ERROR,
    CI_TOKEN_KIND_PREPROCESSOR_IF,      CI_TOKEN_KIND_PREPROCESSOR_IFDEF,
    CI_TOKEN_KIND_PREPROCESSOR_IFNDEF,  CI_TOKEN_KIND_PREPROCESSOR_INCLUDE,
    CI_TOKEN_KIND_PREPROCESSOR_LINE,    CI_TOKEN_KIND_PREPROCESSOR_PRAGMA,
    CI_TOKEN_KIND_PREPROCESSOR_UNDEF,   CI_TOKEN_KIND_PREPROCESSOR_WARNING,
};

#define IS_ZERO '0'

#define IS_DIGIT_WITHOUT_ZERO \
    '1' : case '2':           \
    case '3':                 \
    case '4':                 \
    case '5':                 \
    case '6':                 \
    case '7':                 \
    case '8':                 \
    case '9'

#define IS_ID       \
    'a' : case 'b': \
    case 'c':       \
    case 'd':       \
    case 'e':       \
    case 'f':       \
    case 'g':       \
    case 'h':       \
    case 'i':       \
    case 'j':       \
    case 'k':       \
    case 'l':       \
    case 'm':       \
    case 'n':       \
    case 'o':       \
    case 'p':       \
    case 'q':       \
    case 'r':       \
    case 's':       \
    case 't':       \
    case 'u':       \
    case 'v':       \
    case 'w':       \
    case 'x':       \
    case 'y':       \
    case 'z':       \
    case 'A':       \
    case 'B':       \
    case 'C':       \
    case 'D':       \
    case 'E':       \
    case 'F':       \
    case 'G':       \
    case 'H':       \
    case 'I':       \
    case 'J':       \
    case 'K':       \
    case 'L':       \
    case 'M':       \
    case 'N':       \
    case 'O':       \
    case 'P':       \
    case 'Q':       \
    case 'R':       \
    case 'S':       \
    case 'T':       \
    case 'U':       \
    case 'V':       \
    case 'W':       \
    case 'X':       \
    case 'Y':       \
    case 'Z':       \
    case '_'

const CIFeature *
get_tokens_feature__CIScanner()
{
    return tokens_feature;
}

bool
is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_INT:
        case CI_TOKEN_KIND_KEYWORD_LONG:
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
        case CI_TOKEN_KIND_KEYWORD_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return true;
        default:
            return false;
    }
}

enum CIKeywordMultiPart
convert_token_kind_to_multi_part_keyword__CIKeywordMultiPart(
  enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
            return CI_KEYWORD_MULTI_PART_CHAR;
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
            return CI_KEYWORD_MULTI_PART_DOUBLE;
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
            return CI_KEYWORD_MULTI_PART_FLOAT;
        case CI_TOKEN_KIND_KEYWORD_INT:
            return CI_KEYWORD_MULTI_PART_INT;
        case CI_TOKEN_KIND_KEYWORD_LONG:
            return CI_KEYWORD_MULTI_PART_LONG;
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
            return CI_KEYWORD_MULTI_PART_LONG_LONG;
        case CI_TOKEN_KIND_KEYWORD_SHORT:
            return CI_KEYWORD_MULTI_PART_SHORT;
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
            return CI_KEYWORD_MULTI_PART_SIGNED;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
            return CI_KEYWORD_MULTI_PART_UNSIGNED;
        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
            return CI_KEYWORD_MULTI_PART__COMPLEX;
        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
            return CI_KEYWORD_MULTI_PART__IMAGINARY;
        default:
            UNREACHABLE("cannot convert to multi part keyword");
    }
}

void
add_flag__CIKeywordMultiPart(Uint32 *flags, enum CIKeywordMultiPart flag)
{
    Uint32 old_flags = *flags;

    *flags |= flag;

    if (old_flags == *flags) {
        // Set `long long` flag if `long` flag is already set.
        if (flag == CI_KEYWORD_MULTI_PART_LONG) {
            *flags &= ~CI_KEYWORD_MULTI_PART_LONG;
            *flags |= CI_KEYWORD_MULTI_PART_LONG_LONG;
        } else {
            FAILED("flag already set");
        }
    }
}

enum CITokenKind
convert_multi_part_keyword_to_token_kind__CIKeywordMultiPart(Uint32 flags)
{
    switch (flags) {
        case CI_KEYWORD_MULTI_PART_CHAR:
            return CI_TOKEN_KIND_KEYWORD_CHAR;
        case CI_KEYWORD_MULTI_PART_DOUBLE:
            return CI_TOKEN_KIND_KEYWORD_DOUBLE;
        case CI_KEYWORD_MULTI_PART_DOUBLE | CI_KEYWORD_MULTI_PART__COMPLEX:
            return CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX;
        case CI_KEYWORD_MULTI_PART_DOUBLE | CI_KEYWORD_MULTI_PART__IMAGINARY:
            return CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY;
        case CI_KEYWORD_MULTI_PART_FLOAT:
            return CI_TOKEN_KIND_KEYWORD_FLOAT;
        case CI_KEYWORD_MULTI_PART_FLOAT | CI_KEYWORD_MULTI_PART__COMPLEX:
            return CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX;
        case CI_KEYWORD_MULTI_PART_FLOAT | CI_KEYWORD_MULTI_PART__IMAGINARY:
            return CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY;
        case CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_INT;
        case CI_KEYWORD_MULTI_PART_LONG:
            return CI_TOKEN_KIND_KEYWORD_LONG;
        case CI_KEYWORD_MULTI_PART_LONG | CI_KEYWORD_MULTI_PART_DOUBLE:
            return CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE;
        case CI_KEYWORD_MULTI_PART_LONG | CI_KEYWORD_MULTI_PART_DOUBLE |
          CI_KEYWORD_MULTI_PART__COMPLEX:
            return CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX;
        case CI_KEYWORD_MULTI_PART_LONG | CI_KEYWORD_MULTI_PART_DOUBLE |
          CI_KEYWORD_MULTI_PART__IMAGINARY:
            return CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY;
        case CI_KEYWORD_MULTI_PART_LONG | CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_LONG_INT;
        case CI_KEYWORD_MULTI_PART_LONG_LONG:
            return CI_TOKEN_KIND_KEYWORD_LONG_LONG;
        case CI_KEYWORD_MULTI_PART_LONG_LONG | CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT;
        case CI_KEYWORD_MULTI_PART_SHORT:
            return CI_TOKEN_KIND_KEYWORD_SHORT;
        case CI_KEYWORD_MULTI_PART_SHORT | CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_SHORT_INT;
        case CI_KEYWORD_MULTI_PART_SIGNED:
            return CI_TOKEN_KIND_KEYWORD_SIGNED;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_CHAR:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_INT;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_LONG:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_LONG;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_LONG |
          CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_LONG_LONG:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_LONG_LONG |
          CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_SHORT:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT;
        case CI_KEYWORD_MULTI_PART_SIGNED | CI_KEYWORD_MULTI_PART_SHORT |
          CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT;
        case CI_KEYWORD_MULTI_PART_UNSIGNED:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_CHAR:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_LONG:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_LONG |
          CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_LONG_LONG:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_LONG_LONG |
          CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_SHORT:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT;
        case CI_KEYWORD_MULTI_PART_UNSIGNED | CI_KEYWORD_MULTI_PART_SHORT |
          CI_KEYWORD_MULTI_PART_INT:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT;
        default:
            FAILED("bad combinaison of flags");
    }
}

void
next_char__CIScanner(CIScanner *self)
{
    return next_char__Scanner(&self->base);
}

void
previous_char__CIScanner(CIScanner *self)
{
    return previous_char__Scanner(&self->base);
}

bool
is_space__CIScanner(CIScanner *self)
{
    return is_space__Scanner(&self->base);
}

bool
is_space_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *self,
                                     const char *c)
{
    return is_space_with_peeked_char__Scanner(&self->base, c);
}

void
skip_space__CIScanner(CIScanner *self)
{
    return skip_space__Scanner(&self->base);
}

void
skip_space_except_new_line__CIScanner(CIScanner *self)
{
    return skip_space_except_new_line__Scanner(&self->base);
}

void
jump__CIScanner(CIScanner *self, const Usize n)
{
    return jump__Scanner(&self->base, n);
}

void
start_token__CIScanner(CIScanner *self,
                       const Usize line,
                       const Usize column,
                       const Usize position)
{
    return start_token__Scanner(&self->base, line, column, position);
}

void
end_token__CIScanner(CIScanner *self,
                     const Usize line,
                     const Usize column,
                     const Usize position)
{
    return end_token__Scanner(&self->base, line, column, position);
}

char *
peek_char__CIScanner(const CIScanner *self, const Usize n)
{
    return peek_char__Scanner(&self->base, n);
}

void
scan_and_append_chars__CIScanner(CIScanner *self,
                                 String *res,
                                 bool (*is_valid)(const CIScanner *self))
{
    while (is_valid(self)) {
        next_char__CIScanner(self);
        push__String(res,
                     self->base.source.file
                       ->content[self->base.source.cursor.position - 1]);
    }
}

bool
is_digit__CIScanner(const CIScanner *self)
{
    return self->base.source.cursor.current >= '0' &&
           self->base.source.cursor.current <= '9';
}

bool
is_start_ident__CIScanner(const CIScanner *self)
{
    return (self->base.source.cursor.current >= 'a' &&
            self->base.source.cursor.current <= 'z') ||
           (self->base.source.cursor.current >= 'A' &&
            self->base.source.cursor.current <= 'Z') ||
           self->base.source.cursor.current == '_' ||
           self->base.source.cursor.current == '$';
}

bool
is_ident__CIScanner(const CIScanner *self)
{
    return is_digit__CIScanner(self) || is_start_ident__CIScanner(self);
}

bool
is_digit_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *,
                                     const char *c)
{
    return c >= (char *)'0' && c <= (char *)'9';
}

bool
is_start_ident_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *,
                                           const char *c)
{
    return (c >= (char *)'a' && c <= (char *)'z') ||
           (c >= (char *)'A' && c <= (char *)'Z') || c == (char *)'_' ||
           c == (char *)'$';
}

bool
is_ident_with_peeked_char__CIScanner([[maybe_unused]] const CIScanner *,
                                     const char *c)
{
    return is_digit_with_peeked_char__CIScanner(NULL, c) ||
           is_start_ident_with_peeked_char__CIScanner(NULL, c);
}

bool
is_hex__CIScanner(const CIScanner *self)
{
    return is_digit__CIScanner(self) ||
           (self->base.source.cursor.current >= 'a' &&
            self->base.source.cursor.current <= 'f') ||
           (self->base.source.cursor.current >= 'A' &&
            self->base.source.cursor.current <= 'F');
}

bool
is_oct__CIScanner(const CIScanner *self)
{
    return self->base.source.cursor.current >= '0' &&
           self->base.source.cursor.current <= '7';
}

bool
is_bin__CIScanner(const CIScanner *self)
{
    return self->base.source.cursor.current >= '0' &&
           self->base.source.cursor.current <= '1';
}

bool
is_num__CIScanner(const CIScanner *self)
{
    return is_digit__CIScanner(self) ||
           (self->base.source.cursor.current == '.' &&
            peek_char__CIScanner(self, 1) != (char *)'.') ||
           self->base.source.cursor.current == 'e' ||
           self->base.source.cursor.current == 'E' ||
           self->base.source.cursor.current == '_';
}

#define ADD_K_TOKEN_BASE(tokens, token)                        \
    start_token__CIScanner(self,                               \
                           self->base.source.cursor.line,      \
                           self->base.source.cursor.column,    \
                           self->base.source.cursor.position); \
    end_token__CIScanner(self,                                 \
                         self->base.source.cursor.line,        \
                         self->base.source.cursor.column,      \
                         self->base.source.cursor.position);   \
    add__CITokens(tokens, token)

#define ADD_K_TOKEN(k) ADD_K_TOKEN_BASE(ctx->tokens, k);

void
add_eot_token__CIScanner(CIScanner *self, CIScannerContext *ctx, CITokenEot eot)
{
    ADD_K_TOKEN(
      NEW_VARIANT(CIToken, eot, clone__Location(&self->base.location), eot));
}

void
add_eof_token__CIScanner(CIScanner *self, CIScannerContext *ctx)
{
    ADD_K_TOKEN(
      NEW(CIToken, CI_TOKEN_KIND_EOF, clone__Location(&self->base.location)));
}

void
push_token__CIScanner(CIScanner *self, CIScannerContext *ctx, CIToken *token)
{
    add__CITokens(ctx->tokens, token);
}

void
next_char_by_token__CIScanner(CIScanner *self, const CIToken *token)
{
    switch (token->kind) {
        case CI_TOKEN_KIND_ARROW:
        case CI_TOKEN_KIND_PLUS_PLUS:
        case CI_TOKEN_KIND_MINUS_MINUS:
        case CI_TOKEN_KIND_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_EQ:
        case CI_TOKEN_KIND_LSHIFT_LSHIFT:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT:
        case CI_TOKEN_KIND_EQ_EQ:
        case CI_TOKEN_KIND_BANG_EQ:
        case CI_TOKEN_KIND_AMPERSAND_AMPERSAND:
        case CI_TOKEN_KIND_BAR_BAR:
        case CI_TOKEN_KIND_PLUS_EQ:
        case CI_TOKEN_KIND_MINUS_EQ:
        case CI_TOKEN_KIND_STAR_EQ:
        case CI_TOKEN_KIND_SLASH_EQ:
        case CI_TOKEN_KIND_PERCENTAGE_EQ:
        case CI_TOKEN_KIND_AMPERSAND_EQ:
        case CI_TOKEN_KIND_HAT_EQ:
        case CI_TOKEN_KIND_WAVE_EQ:
        case CI_TOKEN_KIND_BAR_EQ:
        case CI_TOKEN_KIND_HASHTAG_HASHTAG:
            return next_char__CIScanner(self);
        case CI_TOKEN_KIND_DOT_DOT_DOT:
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
            return jump__CIScanner(self, 2);
        default:
            return;
    }
}

enum CITokenKind
get_attribute__CIScanner(const String *id)
{
    Int32 res = get_keyword__Scanner(
      id, ci_attributes, (const Int32 *)ci_attribute_ids, CI_N_ATTRIBUTE);

    if (res == -1) {
        return CI_TOKEN_KIND_IDENTIFIER;
    }

    return (enum CITokenKind)res;
}

enum CITokenKind
get_builtin_macro__CIScanner(const String *id)
{
    Int32 res = get_keyword__Scanner(id,
                                     ci_builtin_macros,
                                     (const Int32 *)ci_builtin_macro_ids,
                                     CI_N_BUILTIN_MACRO);

    if (res == -1) {
        return CI_TOKEN_KIND_IDENTIFIER;
    }

    return (enum CITokenKind)res;
}

enum CITokenKind
get_standard_predefined_macro__CIScanner(const String *id)
{
    Int32 res =
      get_keyword__Scanner(id,
                           ci_standard_predefined_macros,
                           (const Int32 *)ci_standard_predefined_macro_ids,
                           CI_N_STANDARD_PREDEFINED_MACRO);

    if (res == -1) {
        return CI_TOKEN_KIND_IDENTIFIER;
    }

    return (enum CITokenKind)res;
}

enum CITokenKind
get_preprocessor__CIScanner(const String *id)
{
    Int32 res = get_keyword__Scanner(id,
                                     ci_preprocessors,
                                     (const Int32 *)ci_preprocessor_ids,
                                     CI_N_PREPROCESSOR);

    if (res == -1) {
        return CI_TOKEN_KIND_IDENTIFIER;
    }

    return (enum CITokenKind)res;
}

enum CITokenKind
get_single_keyword__CIScanner(const String *id)
{
    Int32 res = get_keyword__Scanner(id,
                                     ci_single_keywords,
                                     (const Int32 *)ci_single_keyword_ids,
                                     CI_N_SINGLE_KEYWORD);

    if (res == -1) {
        return CI_TOKEN_KIND_IDENTIFIER;
    }

    return (enum CITokenKind)res;
}

enum CITokenKind
standardize_keyword__CIScanner(enum CITokenKind kind)
{
    switch (kind) {
        case CI_TOKEN_KIND_KEYWORD_CHAR:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR:
            return CI_TOKEN_KIND_KEYWORD_CHAR;
        case CI_TOKEN_KIND_KEYWORD_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT:
            return CI_TOKEN_KIND_KEYWORD_SHORT_INT;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT;
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_INT:
            return CI_TOKEN_KIND_KEYWORD_INT;
        case CI_TOKEN_KIND_KEYWORD_LONG:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT:
            return CI_TOKEN_KIND_KEYWORD_LONG_INT;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT;
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT:
            return CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT;
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG:
            return CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT;
        default:
            return kind;
    }
}

CIToken *
scan_multi_part_keyword__CIScanner(CIScanner *self, CIScannerContext *ctx)
{
// This macro allows you to make the final configuration of the
// token location and checks whether the token is available in the
// standard configured by the user.
#define DEFAULT_LAST_SET_AND_CHECK(token)                    \
    next_char_by_token__CIScanner(self, token);              \
    end_token__CIScanner(self,                               \
                         self->base.source.cursor.line,      \
                         self->base.source.cursor.column,    \
                         self->base.source.cursor.position); \
    set_all__Location(&token->location, &self->base.location);

#define MAX_KEYWORD_PART 4
#define PUSH_TOKEN_FROM_FLAGS(flags)                                        \
    {                                                                       \
        if (flags != 0) {                                                   \
            CIToken *t = NEW(                                               \
              CIToken,                                                      \
              convert_multi_part_keyword_to_token_kind__CIKeywordMultiPart( \
                flags),                                                     \
              clone__Location(&self->base.location));                       \
            t->kind = standardize_keyword__CIScanner(t->kind);              \
            DEFAULT_LAST_SET_AND_CHECK(t);                                  \
            push_token__CIScanner(self, ctx, t);                            \
        }                                                                   \
    }

    Uint32 flags = 0;
    CIToken *last_token = NULL;

    for (Usize i = 0; i < MAX_KEYWORD_PART && is_start_ident__CIScanner(self);
         ++i,
               next_char__CIScanner(self),
               skip_space_except_new_line__CIScanner(self)) {
        start_token__CIScanner(self,
                               self->base.source.cursor.line,
                               self->base.source.cursor.column,
                               self->base.source.cursor.position);

        String *id = scan_identifier__CIScanner(self);
        enum CITokenKind current_kind = get_single_keyword__CIScanner(id);

        end_token__CIScanner(self,
                             self->base.source.cursor.line,
                             self->base.source.cursor.column,
                             self->base.source.cursor.position);

        CHECK_STANDARD_SINCE(
          self->config->standard, tokens_feature[current_kind].since, {
              PUSH_TOKEN_FROM_FLAGS(flags)

              last_token = NEW_VARIANT(
                CIToken, identifier, clone__Location(&self->base.location), id);

              break;
          })
        else
        {
            if (is_multi_part_keyword_from_token_kind__CIKeywordMultiPart(
                  current_kind)) {
                FREE(String, id);

                add_flag__CIKeywordMultiPart(
                  &flags,
                  convert_token_kind_to_multi_part_keyword__CIKeywordMultiPart(
                    current_kind));
            } else {
                PUSH_TOKEN_FROM_FLAGS(flags);

                switch (current_kind) {
                    case CI_TOKEN_KIND_IDENTIFIER:
                        last_token =
                          NEW_VARIANT(CIToken,
                                      identifier,
                                      clone__Location(&self->base.location),
                                      id);

                        break;
                    default:
                        FREE(String, id);

                        last_token = NEW(CIToken,
                                         current_kind,
                                         clone__Location(&self->base.location));
                }

                break;
            }
        }
    }

    if (!last_token) {
        last_token = NEW(
          CIToken,
          convert_multi_part_keyword_to_token_kind__CIKeywordMultiPart(flags),
          clone__Location(&self->base.location));
        previous_char__CIScanner(self);
    }

    CIToken *res = NULL;

    switch (last_token->kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            if (is_in_macro__CIScannerContext(ctx) && ctx->macro) {
                // Determine whether the `last_token->identifier` does not
                // correspond to a macro parameter.
                bool has_variadic = false;

                for (Usize i = 0; i < ctx->macro->len; ++i) {
                    CITokenPreprocessorDefineParam *param =
                      get__Vec(ctx->macro, i);

                    if (param->is_variadic) {
                        has_variadic = true;

                        if (!strcmp(last_token->identifier->buffer,
                                    CI_VA_ARGS)) {
                            res = NEW_VARIANT(CIToken,
                                              macro_param_variadic,
                                              last_token->location,
                                              NEW(CITokenMacroParamVariadic));

                            FREE(CIToken, last_token);
                            last_token = NULL;

                            break;
                        }
                    } else {
                        ASSERT(param->name);

                        if (!strcmp(param->name->buffer,
                                    last_token->identifier->buffer)) {
                            res = NEW_VARIANT(CIToken,
                                              macro_param,
                                              last_token->location,
                                              NEW(CITokenMacroParam, i));

                            FREE(CIToken, last_token);
                            last_token = NULL;

                            break;
                        }
                    }
                }

                if (!has_variadic && last_token &&
                    !strcmp(last_token->identifier->buffer, CI_VA_ARGS)) {
                    FAILED("you cannot use `__VA_ARGS__` in macro which have "
                           "no variadic param");
                }

                if (res) {
                    goto exit;
                }
            }

            if (is_in_prepro_cond__CIScannerContext(ctx)) {
                // e.g. #if defined(...)
                if (!strcmp(last_token->identifier->buffer, "defined")) {
                    String *defined_identifier = NULL;
                    bool has_open_paren = false;

                    next_char__CIScanner(
                      self); // skip last character of identifier
                    skip_space_and_backslash__CIScanner(self);

                    switch (self->base.source.cursor.current) {
                        case '(':
                            has_open_paren = true;

                            next_char__CIScanner(self);
                            skip_space_and_backslash__CIScanner(self);

                            break;
                        default:
                            break;
                    }

                    if (is_ident__CIScanner(self)) {
                        defined_identifier = scan_identifier__CIScanner(self);
                        next_char__CIScanner(self);
                    }

                    if (has_open_paren) {
                        skip_space_and_backslash__CIScanner(self);

                        switch (self->base.source.cursor.current) {
                            case ')':
                                break;
                            default:
                                FAILED("expected `)`");
                        }
                    } else {
                        previous_char__CIScanner(self);
                    }

                    res = NEW_VARIANT(CIToken,
                                      macro_defined,
                                      last_token->location,
                                      defined_identifier);

                    FREE(CIToken, last_token);

                    goto exit;
                }
            }

            // Check if it's a standard predefined macro
            // https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
            {
                enum CITokenKind standard_predefined_macro =
                  get_standard_predefined_macro__CIScanner(
                    last_token->identifier);

                switch (standard_predefined_macro) {
                    // Standard predefined macro case
                    case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___DATE__: {
#define DATE_LEN 100

                        // TODO: Improve this part of code.
                        time_t current_time;
                        struct tm *time_info;
                        char date_s[DATE_LEN];

                        memset(date_s, 0, DATE_LEN);
                        time(&current_time);

                        time_info = localtime(&current_time);

                        strftime(date_s, sizeof(date_s), "%Y-%m-%d", time_info);

                        res = NEW_VARIANT(CIToken,
                                          standard_predefined_macro___date__,
                                          last_token->location,
                                          from__String(date_s));

                        goto free_last_token;

#undef DATE_LEN
                    }
                    case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__:
                        res =
                          NEW(CIToken,
                              CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___FILE__,
                              last_token->location);

                        goto free_last_token;
                    case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__:
                        res =
                          NEW(CIToken,
                              CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___LINE__,
                              last_token->location);

                        goto free_last_token;
                    case CI_TOKEN_KIND_STANDARD_PREDEFINED_MACRO___TIME__: {
#define TIME_LEN 100

                        // TODO: Improve this part of code.
                        time_t current_time;
                        struct tm *time_info;
                        char time_s[TIME_LEN];

                        memset(time_s, 0, TIME_LEN);
                        time(&current_time);

                        time_info = localtime(&current_time);

                        strftime(time_s, sizeof(time_s), "%H:%M:%S", time_info);

                        res = NEW_VARIANT(CIToken,
                                          standard_predefined_macro___time__,
                                          last_token->location,
                                          from__String(time_s));

                        goto free_last_token;

#undef TIME_LEN
                    }
                    default:
                        break;
                }
            }

            // Check if it's a builtin macro
            // https://clang.llvm.org/docs/LanguageExtensions.html#builtin-macros
            {
                enum CITokenKind builtin_macro =
                  get_builtin_macro__CIScanner(last_token->identifier);

                switch (builtin_macro) {
                    case CI_TOKEN_KIND_BUILTIN_MACRO___HAS_FEATURE:
                        if (self->config->compiler.kind ==
                            CI_PROJECT_CONFIG_COMPILER_KIND_CLANG) {
                            next_char__CIScanner(self); // __has_feature(x)
                                                        //             ^
                            skip_space_and_backslash__CIScanner(self);

                            switch (self->base.source.cursor.current) {
                                case '(':
                                    next_char__CIScanner(self);
                                    break;
                                default:
                                    FAILED("expected `(`");
                            }

                            enum CIExtensionsHasFeature feature = -1;

                            skip_space_and_backslash__CIScanner(self);

                            if (is_ident__CIScanner(self)) {
                                String *id = scan_identifier__CIScanner(self);

                                if (is_valid__CIExtensionsHasFeature(id)) {
                                    feature = get__CIExtensionsHasFeature(id);
                                } else {
                                    FAILED(
                                      "the feature of called in __has_feature "
                                      "is not valid");
                                }

                                next_char__CIScanner(self); // __has_feature(x)
                                                            //               ^
                                skip_space_and_backslash__CIScanner(self);

                                switch (self->base.source.cursor.current) {
                                    case ')':
                                        break;
                                    default:
                                        FAILED("expected `)`");
                                }

                                FREE(String, id);
                            } else {
                                FAILED("expected identifier");
                            }

                            res = NEW_VARIANT(CIToken,
                                              builtin_macro_has_feature,
                                              last_token->location,
                                              feature);

                            goto free_last_token;
                        }

                        break;
                    default:
                        break;
                }
            }

            res = last_token;
        exit:
            break;
        free_last_token:
            FREE(CIToken, last_token);
            break;
        case CI_TOKEN_KIND_KEYWORD___ATTRIBUTE__: {
            Location location = clone__Location(&last_token->location);

            FREE(CIToken, last_token);

            return scan_gnu_attribute__CIScanner(self, ctx, location);
        }
        default:
            res = last_token;
            res->kind = standardize_keyword__CIScanner(res->kind);
    }

#undef MAX_KEYWORD_PART
#undef PUSH_TOKEN_FROM_FLAGS

    return res;
}

CIToken *
scan_attribute__CIScanner(CIScanner *self)
{
    // TODO: Add function to expect a character.

    // Skip `[[`
    jump__CIScanner(self, 2);

    String *attribute = scan_identifier__CIScanner(self);
    enum CITokenKind kind = get_attribute__CIScanner(attribute);
    CIToken *res = NULL;

    FREE(String, attribute);

    next_char__CIScanner(self);

    switch (kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            FAILED("unknown standard attribute");
        case CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED:
        case CI_TOKEN_KIND_ATTRIBUTE_NODISCARD: {
            String *attribute_value = NULL;

            if (self->base.source.cursor.current == '(' &&
                peek_char__CIScanner(self, 1) == (char *)'\"') {
                next_char__CIScanner(self);
                attribute_value = scan_string__CIScanner(self);

                next_char__CIScanner(self);

                if (self->base.source.cursor.current == ')') {
                    next_char__CIScanner(self);
                } else {
                    FAILED("expected `)`");
                }
            }

            switch (kind) {
                case CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED:
                    res = NEW_VARIANT(CIToken,
                                      attribute_deprecated,
                                      clone__Location(&self->base.location),
                                      attribute_value);
                    break;
                case CI_TOKEN_KIND_ATTRIBUTE_NODISCARD:
                    res = NEW_VARIANT(CIToken,
                                      attribute_nodiscard,
                                      clone__Location(&self->base.location),
                                      attribute_value);
                    break;
                default:
                    UNREACHABLE("this situation is impossible");
            }

            break;
        }
        default:
            res = NEW(CIToken, kind, clone__Location(&self->base.location));
    }

    if (self->base.source.cursor.current == ']' &&
        peek_char__CIScanner(self, 1) == (char *)']') {
        next_char__CIScanner(self);
    } else {
        FAILED("expected `]]` to close attribute");
    }

    return res;
}

void
skip_comment_line__CIScanner(CIScanner *self)
{
    while (self->base.source.cursor.current != '\n') {
        next_char__CIScanner(self);
    }
}

void
skip_comment_block__CIScanner(CIScanner *self)
{
    Location location_error = default__Location(self->base.source.file->name);

    start__Location(&location_error,
                    self->base.source.cursor.line,
                    self->base.source.cursor.column - 2,
                    self->base.source.cursor.position - 2); // 2 = `/*`

    // Check if the comment block is closed. While the current character is not
    // a `*` and the next character is not a `/`, we continue to scan the
    // comment block.
    while (self->base.source.cursor.current != '*' ||
           peek_char__CIScanner(self, 1) != (char *)'/') {
        // Check if the comment block is not closed.
        if (self->base.source.cursor.position >=
            self->base.source.file->len - 2) {
            end__Location(&location_error,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_ci_error,
                self->base.source.file,
                &location_error,
                NEW(CIError, CI_ERROR_KIND_UNCLOSED_COMMENT_BLOCK),
                init__Vec(1,
                          from__String("close comment multi line with `*/`")),
                NULL,
                NULL),
              self->base.count_error);
        }

        next_char__CIScanner(self);
    }

    jump__CIScanner(self, 2);
}

String *
scan_comment_doc__CIScanner(CIScanner *self)
{
    String *res = NEW(String);
    Location location_error = default__Location(self->base.source.file->name);

    start__Location(&location_error,
                    self->base.source.cursor.line,
                    self->base.source.cursor.column - 3,
                    self->base.source.cursor.position - 3); // 3 = `/**`

    // Check if the comment block is closed. While the current character is not
    // a `*`,the next character one is not a `*` and the next second is not a
    // `/`, we continue to scan the comment block.
    while (self->base.source.cursor.current != '*' ||
           peek_char__CIScanner(self, 1) != (char *)'*' ||
           peek_char__CIScanner(self, 2) != (char *)'/') {
        // Check if the comment block is not closed.
        if (self->base.source.cursor.position >=
            self->base.source.file->len - 2) {
            end__Location(&location_error,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_ci_error,
                self->base.source.file,
                &location_error,
                NEW(CIError, CI_ERROR_KIND_UNCLOSED_COMMENT_DOC),
                init__Vec(1, from__String("close comment bloc kwith `**/`")),
                NULL,
                NULL),
              self->base.count_error);
        }

        next_char__CIScanner(self);
    }

    jump__CIScanner(self, 3);

    return res;
}

String *
get_character__CIScanner(CIScanner *self, char previous)
{
    String *res = NULL;
    Location location_error = default__Location(self->base.source.file->name);

    start__Location(&location_error,
                    self->base.source.cursor.line,
                    self->base.source.cursor.column,
                    self->base.source.cursor.position);

    switch (previous) {
        // TODO: We're probably missing some escapes characters.
        case '\\':
            switch (self->base.source.cursor.current) {
                // NOTE: For the moment we're using \\ for some escapes, because
                // for the moment CI is only a transpiler to C.
                case '\"':
                    res = from__String("\"");
                    break;
                case '\'':
                    res = from__String("'");
                    break;
                case 'b':
                    res = from__String("\b");
                    break;
                case 'f':
                    res = from__String("\f");
                    break;
                case 'n':
                    res = from__String("\n");
                    break;
                case 't':
                    res = from__String("\t");
                    break;
                case 'r':
                    res = from__String("\r");
                    break;
                case 'v':
                    res = from__String("\v");
                    break;
                case '\\':
                    res = from__String("\\");
                    break;
                case '0':
                    res = from__String("\0");
                    break;
                default:
                    end__Location(&location_error,
                                  self->base.source.cursor.line,
                                  self->base.source.cursor.column,
                                  self->base.source.cursor.position);

                    if (HAS_REACH_END(self)) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->base.source.file,
                            &location_error,
                            NEW(LilyError,
                                LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL),
                            NULL,
                            NULL,
                            NULL),
                          self->base.count_error);
                    } else {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->base.source.file,
                            &location_error,
                            NEW(LilyError, LILY_ERROR_KIND_INVALID_ESCAPE),
                            NULL,
                            NULL,
                            NULL),
                          self->base.count_error);

                        next_char__CIScanner(self);
                    }

                    return NULL;
            }

            break;
        default:
            res = format__String("{c}", previous);
    }

    if (previous == '\\') {
        next_char__CIScanner(self);
    }

    return res;
}

String *
scan_identifier__CIScanner(CIScanner *self)
{
    String *id = NEW(String);

    scan_and_append_chars__CIScanner(self, id, &is_ident__CIScanner);
    previous_char__CIScanner(self);

    return id;
}

bool
identifier_is_standard_predefined_macro__CIScanner(CIScanner *self,
                                                   const String *name)
{
    return get_standard_predefined_macro__CIScanner(name) !=
           CI_TOKEN_KIND_IDENTIFIER;
}

String *
scan_identifier_with_peek_char__CIScanner(const CIScanner *self)
{
    String *res = NEW(String);
    char *peeked = peek_char__CIScanner(self, 1);

    for (Usize i = 1; is_ident_with_peeked_char__CIScanner(self, peeked);) {
        push__String(res, (char)(Uptr)peeked);
        peeked = peek_char__CIScanner(self, ++i);
    }

    return res;
}

char *
scan_character__CIScanner(CIScanner *self)
{
    Location location_error = default__Location(self->base.source.file->name);

    start__Location(&location_error,
                    self->base.source.cursor.line,
                    self->base.source.cursor.column,
                    self->base.source.cursor.position);
    next_char__CIScanner(self);

    // Check if the char literal is not closed.
    if (self->base.source.cursor.current != '\'') {
        next_char__CIScanner(self);

        char target = self->base.source.cursor.current;
        String *character = get_character__CIScanner(
          self,
          self->base.source.file
            ->content[self->base.source.cursor.position - 1]);

        end__Location(&location_error,
                      self->base.source.cursor.line,
                      self->base.source.cursor.column,
                      self->base.source.cursor.position);

        // Check if the char literal is not closed.
        if (target != '\'' && self->base.source.cursor.current != '\'') {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->base.source.file,
                &location_error,
                NEW(LilyError, LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL),
                init__Vec(
                  1, from__String("please close this char literal with `\'`")),
                NULL,
                NULL),
              self->base.count_error);

            if (character) {
                FREE(String, character);
            }

            return NULL;
        }

        if (character) {
            char *res = (char *)(Uptr)character->buffer[0];

            FREE(String, character);

            return res;
        }

        return NULL;
    }

    end__Location(&location_error,
                  self->base.source.cursor.line,
                  self->base.source.cursor.column,
                  self->base.source.cursor.position);

    emit__Diagnostic(
      NEW_VARIANT(
        Diagnostic,
        simple_lily_error,
        self->base.source.file,
        &location_error,
        NEW(LilyError, LILY_ERROR_KIND_UNCLOSED_CHAR_LITERAL),
        init__Vec(1, from__String("please close this char literal with `\"`")),
        NULL,
        from__String("unexpected token here: `'`")),
      self->base.count_error);

    return NULL;
}

String *
scan_string__CIScanner(CIScanner *self)
{
    Location location_error = default__Location(self->base.source.file->name);
    String *res = NEW(String);

    start__Location(&location_error,
                    self->base.source.cursor.line,
                    self->base.source.cursor.column,
                    self->base.source.cursor.position);
    next_char__CIScanner(self);

    // Check if the string literal is not closed. While the current character is
    // not a `"` or the next character is not a `"`, we continue to scan the
    // string literal.
    while (self->base.source.cursor.current != '\"') {
        if (self->base.source.cursor.position >
            self->base.source.file->len - 2) {
            end__Location(&location_error,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_ci_error,
                self->base.source.file,
                &location_error,
                NEW(CIError, CI_ERROR_KIND_UNCLOSED_STRING_LITERAL),
                init__Vec(
                  1, from__String("add `\"` to the end of string literal")),
                NULL,
                NULL),
              self->base.count_error);

            FREE(String, res);

            return NULL;
        }

        next_char__CIScanner(self);

        // Scan the escape character. If the `get_character__CIScanner` return
        // NULL that's mean the escape character is invalid.
        {
            String *c = get_character__CIScanner(
              self,
              self->base.source.file
                ->content[self->base.source.cursor.position - 1]);

            if (!c) {
                FREE(String, res);

                return NULL;
            }

            APPEND_AND_FREE(res, c);
        }
    }

    return res;
}

CIToken *
scan_bin__CIScanner(CIScanner *self)
{
    String *res = NEW(String);

    scan_and_append_chars__CIScanner(self, res, &is_bin__CIScanner);
    previous_char__CIScanner(self);

    return NEW_VARIANT(CIToken,
                       literal_constant_bin,
                       clone__Location(&self->base.location),
                       NEW(CITokenLiteralConstantInt,
                           CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                           res));
}

CIToken *
scan_hex__CIScanner(CIScanner *self)
{
    String *res = NEW(String);

    scan_and_append_chars__CIScanner(self, res, &is_hex__CIScanner);
    previous_char__CIScanner(self);

    return NEW_VARIANT(CIToken,
                       literal_constant_hex,
                       clone__Location(&self->base.location),
                       NEW(CITokenLiteralConstantInt,
                           CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                           res));
}

CIToken *
scan_oct__CIScanner(CIScanner *self)
{
    String *res = NEW(String);

    scan_and_append_chars__CIScanner(self, res, &is_oct__CIScanner);
    previous_char__CIScanner(self);

    return NEW_VARIANT(CIToken,
                       literal_constant_octal,
                       clone__Location(&self->base.location),
                       NEW(CITokenLiteralConstantInt,
                           CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                           res));
}

CIToken *
scan_num__CIScanner(CIScanner *self)
{
    String *res = NEW(String);
    bool is_float = false;
    bool is_scientific = false;
    Location location_error = default__Location(self->base.source.file->name);

    start__Location(&location_error,
                    self->base.source.cursor.line,
                    self->base.source.cursor.column,
                    self->base.source.cursor.position);

    while (is_num__CIScanner(self)) {
        // Check if the float literal is valid. If the current character is `.`
        // and the number is not a float, we continue to scan the float,
        // otherwise we emit an error.
        if (self->base.source.cursor.current == '.' && !is_float) {
            is_float = true;
        } else if (self->base.source.cursor.current == '.' && is_float) {
            start__Location(&location_error,
                            self->base.source.cursor.line,
                            self->base.source.cursor.column,
                            self->base.source.cursor.position);
            end__Location(&location_error,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);

            next_char__CIScanner(self);

            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->base.source.file,
                          &location_error,
                          NEW(LilyError, LILY_ERROR_KIND_INVALID_FLOAT_LITERAL),
                          NULL,
                          NULL,
                          from__String("in a float literal it is forbidden to "
                                       "add more than one `.`")),
              self->base.count_error);

            FREE(String, res);

            return NULL;
        }

        // Check if the scientific notation is valid. If the current character
        // is `e` or `E` and the number is not a scientific notation, we
        // continue to scan the scientific notation, otherwise we emit an error.
        if ((self->base.source.cursor.current == 'e' ||
             self->base.source.cursor.current == 'E') &&
            !is_scientific) {
            push__String(res, self->base.source.cursor.current);
            is_scientific = true;
            next_char__CIScanner(self);

            if (self->base.source.cursor.current == '-' ||
                self->base.source.cursor.current == '+') {
                push__String(res, self->base.source.cursor.current);
                next_char__CIScanner(self);
            }
        } else if ((self->base.source.cursor.current == 'e' ||
                    self->base.source.cursor.current == 'E') &&
                   is_scientific) {
            start__Location(&location_error,
                            self->base.source.cursor.line,
                            self->base.source.cursor.column,
                            self->base.source.cursor.position);
            end__Location(&location_error,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);

            next_char__CIScanner(self);

            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->base.source.file,
                          &location_error,
                          NEW(LilyError, LILY_ERROR_KIND_INVALID_FLOAT_LITERAL),
                          NULL,
                          NULL,
                          from__String("in a float literal it is forbidden to "
                                       "add more than one `e` or `E`")),
              self->base.count_error);

            FREE(String, res);

            return NULL;
        }

        if (self->base.source.cursor.current != '_') {
            push__String(res, self->base.source.cursor.current);
        }

        next_char__CIScanner(self);
    }

    previous_char__CIScanner(self);

    if (is_float || is_scientific) {
        end__Location(&location_error,
                      self->base.source.cursor.line,
                      self->base.source.cursor.column,
                      self->base.source.cursor.position);
        return NEW_VARIANT(CIToken,
                           literal_constant_float,
                           clone__Location(&self->base.location),
                           NEW(CITokenLiteralConstantFloat,
                               CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_NONE,
                               res));
    }

    return NEW_VARIANT(CIToken,
                       literal_constant_int,
                       clone__Location(&self->base.location),
                       NEW(CITokenLiteralConstantInt,
                           CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                           res));
}

#define BACKSLASH_DIAGNOSTIC()                               \
    if (is_space__CIScanner(self)) {                         \
        /* TODO: skip blank space */                         \
        FAILED("warning: space is not expected after `\\`"); \
    } else {                                                 \
        FAILED("expected new line");                         \
    }

void
skip_space_and_backslash__CIScanner(CIScanner *self)
{
    while (
      self->base.source.cursor.current != '\n' &&
      (is_space__CIScanner(self) || self->base.source.cursor.current == '\\') &&
      !HAS_REACH_END(self)) {
        skip_space__CIScanner(self);

        if (self->base.source.cursor.current == '\\') {
            next_char__CIScanner(self);

            switch (self->base.source.cursor.current) {
                case '\n':
                    next_char__CIScanner(self);
                    break;
                default:
                    BACKSLASH_DIAGNOSTIC();
            }
        }
    }
}

void
skip_backslash__CIScanner(CIScanner *self)
{
    while (self->base.source.cursor.current == '\\') {
        next_char__CIScanner(self);

        switch (self->base.source.cursor.current) {
            case '\n':
                next_char__CIScanner(self);
                break;
            default:
                BACKSLASH_DIAGNOSTIC();
        }
    }
}

#undef BACKSLASH_DIAGNOSTIC

void
skip_space_according_ctx__CIScanner(CIScanner *self,
                                    const CIScannerContext *ctx)
{
    switch (ctx->ctx_location) {
        case CI_SCANNER_CONTEXT_LOCATION_MACRO:
            skip_space_and_backslash__CIScanner(self);

            break;
        default:
            skip_space__CIScanner(self);
    }
}

String *
scan_macro_name__CIScanner(CIScanner *self,
                           CIScannerContext *ctx,
                           CIScannerContext *ctx_parent,
                           enum CITokenKind preprocessor_ctx)
{
    ASSERT(preprocessor_ctx == CI_TOKEN_KIND_PREPROCESSOR_IFDEF ||
           preprocessor_ctx == CI_TOKEN_KIND_PREPROCESSOR_DEFINE ||
           preprocessor_ctx == CI_TOKEN_KIND_PREPROCESSOR_UNDEF);

    skip_space_and_backslash__CIScanner(self);

    if (!is_ident__CIScanner(self)) {
        FAILED("expected identifier");
    }

    String *name = scan_identifier__CIScanner(self);

    if (identifier_is_standard_predefined_macro__CIScanner(self, name)) {
        if (!(self->is_predefined ||
              preprocessor_ctx == CI_TOKEN_KIND_PREPROCESSOR_IFDEF)) {
            FAILED("not expected to undef or re-define standard predefined "
                   "macro, outside of predefined file");
        }
    } else if (!strcmp(name->buffer, CI_VA_ARGS)) {
        FAILED("cannot define __CI_VA_ARGS__");
    }

    next_char__CIScanner(self);

    return name;
}

Vec *
scan_define_preprocessor_params__CIScanner(CIScanner *self,
                                           bool *macro_is_variadic)
{
    Vec *params = NEW(Vec);
    CIScannerContext ctx = NEW_VARIANT(CIScannerContext, macro, NULL, NULL);

    next_char__CIScanner(self); // skip `(`

    while (self->base.source.cursor.current != ')') {
        CIToken *param = get_token__CIScanner(self, &ctx, NULL);
        bool expected_close_paren = false;

        if (param) {
            switch (param->kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    push__Vec(params,
                              NEW_VARIANT(CITokenPreprocessorDefineParam,
                                          normal,
                                          param->identifier));
                    next_char__CIScanner(self);

                    lily_free(param);

                    goto skip_sep;
                case CI_TOKEN_KIND_DOT_DOT_DOT:
                    push__Vec(
                      params,
                      NEW_VARIANT(CITokenPreprocessorDefineParam, variadic));

                    jump__CIScanner(self, 3);
                    FREE(CIToken, param);

                    *macro_is_variadic = true;
                    expected_close_paren = true;

                    goto skip_sep;
                default:
                    FAILED("expected identifier");
            }

            next_char_by_token__CIScanner(self, param);

            FREE(CIToken, param);
        }

    skip_sep:
        skip_space_and_backslash__CIScanner(self);

        switch (self->base.source.cursor.current) {
            case ')':
                break;
            case ',':
                if (expected_close_paren) {
                    FAILED(
                      "the last parametter is `...`, so `,` is not expected");
                }

                next_char__CIScanner(self);
                break;
            default:
                if (expected_close_paren) {
                    FAILED("expected `)`");
                } else {
                    FAILED("expected `,` or `)`");
                }
        }

        skip_space_and_backslash__CIScanner(self);
    }

    next_char__CIScanner(self); // skip `)`

    return params;
}

CITokens
scan_preprocessor_content__CIScanner(CIScanner *self,
                                     enum CIScannerContextLocation ctx_location,
                                     Vec *params)
{
    CITokens tokens = NEW(CITokens);
    CIScannerContext ctx =
      ctx_location == CI_SCANNER_CONTEXT_LOCATION_MACRO
        ? NEW_VARIANT(CIScannerContext, macro, &tokens, params)
        : NEW(CIScannerContext, ctx_location, &tokens);

    skip_space_except_new_line__CIScanner(self);

    while (self->base.source.cursor.current != '\n') {
        skip_space_and_backslash__CIScanner(self);

        if (self->base.source.cursor.current == '\n') {
            break;
        }

        CIToken *token = get_token__CIScanner(self, &ctx, NULL);

        if (token) {
            // This macro automatically pushes a token onto the tokens vector or
            // releases it (depending on the token type). It also advances by a
            // single character, with the exception of preprocessors and, of
            // course, block comments, which are freed.
            //
            // What's more, you might have noticed that this macro has been
            // designed so that custom cases can be added if required.
#define DEFAULT_FILTER_TOKEN(token, ctx)                                     \
    /* NOTE: This switch case should be left in first position. */           \
    case CI_TOKEN_KIND_COMMENT_BLOCK:                                        \
    /* FIXME: This case has been added temporarily to skip the GNU attribute \
     * token, as it is not yet supported. */                                 \
    case CI_TOKEN_KIND_GNU_ATTRIBUTE:                                        \
        FREE(CIToken, token);                                                \
        break;                                                               \
    case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:                                  \
    case CI_TOKEN_KIND_PREPROCESSOR_ELIF:                                    \
    case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:                                 \
    case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:                                \
    case CI_TOKEN_KIND_PREPROCESSOR_ELSE:                                    \
    case CI_TOKEN_KIND_PREPROCESSOR_EMBED:                                   \
    case CI_TOKEN_KIND_PREPROCESSOR_ENDIF:                                   \
    case CI_TOKEN_KIND_PREPROCESSOR_ERROR:                                   \
    case CI_TOKEN_KIND_PREPROCESSOR_IF:                                      \
    case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:                                   \
    case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:                                  \
    case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:                                 \
    case CI_TOKEN_KIND_PREPROCESSOR_LINE:                                    \
    case CI_TOKEN_KIND_PREPROCESSOR_PRAGMA:                                  \
    case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:                                   \
    case CI_TOKEN_KIND_PREPROCESSOR_WARNING:                                 \
        push_token__CIScanner(self, ctx, token);                             \
        break;                                                               \
    default:                                                                 \
        next_char__CIScanner(self);                                          \
        push_token__CIScanner(self, ctx, token);                             \
        break;

            DEFAULT_LAST_SET_AND_CHECK(token);

            switch (token->kind) {
                DEFAULT_FILTER_TOKEN(token, &ctx);
                case CI_TOKEN_KIND_COMMENT_LINE:
                    FREE(CIToken, token);
                    goto exit;
            }
        }
    }

exit:

    switch (ctx_location) {
        case CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_COND:
            add_eof_token__CIScanner(self, &ctx);

            break;
        case CI_SCANNER_CONTEXT_LOCATION_MACRO:
            add_eot_token__CIScanner(
              self, &ctx, NEW_VARIANT(CITokenEot, macro_call));

            break;
        default:
            add_eot_token__CIScanner(
              self, &ctx, NEW(CITokenEot, CI_TOKEN_EOT_CONTEXT_OTHER));
    }

    return tokens;
}

CIToken *
scan_define_preprocessor__CIScanner(CIScanner *self)
{
    Location preprocessor_define_location =
      clone__Location(&self->base.location);
    String *name = NULL;

    skip_space_and_backslash__CIScanner(self);

    // Get macro name
    {
        CIScannerContext ctx =
          NEW(CIScannerContext, CI_SCANNER_CONTEXT_LOCATION_NONE, NULL);

        name = scan_macro_name__CIScanner(
          self, &ctx, NULL, CI_TOKEN_KIND_PREPROCESSOR_DEFINE);
    }

    Vec *params = NULL; // Vec<CITokenpreprocessorDefineParam*>*?
    bool is_variadic = false;

    switch (self->base.source.cursor.current) {
        case '(':
            params =
              scan_define_preprocessor_params__CIScanner(self, &is_variadic);

            break;
        default:
            break;
    }

    return NEW_VARIANT(CIToken,
                       preprocessor_define,
                       preprocessor_define_location,
                       NEW(CITokenPreprocessorDefine,
                           name,
                           params,
                           scan_preprocessor_content__CIScanner(
                             self, CI_SCANNER_CONTEXT_LOCATION_MACRO, params),
                           is_variadic));
}

CIToken *
scan_elif_preprocessor__CIScanner(CIScanner *self, CIScannerContext *ctx_parent)
{
#define ADD_EOT(tokens, eot_ctx)                                        \
    ADD_K_TOKEN_BASE(tokens,                                            \
                     NEW_VARIANT(CIToken,                               \
                                 eot,                                   \
                                 clone__Location(&self->base.location), \
                                 NEW(CITokenEot, eot_ctx)));

#define SCAN_IF_PREPROCESSOR_CONTENT(                                        \
  cond, is_def_preprocessor, ctx_parent, ctx_location, in_prepro_else)       \
                                                                             \
    [[maybe_unused]] Location preprocessor_if_location =                     \
      clone__Location(&self->base.location);                                 \
    /*                                                                       \
     We scan the if condition (`preprocessor_if_cond`), if it's not a        \
     conditional preprocessor looking at a macro is defined or not (like     \
    #ifdef, #ifndef,                                                         \
     ...).                                                                   \
     \                                                                       \
    In other words, when `is_def_preprocessor` is true, it means it's a      \
    conditional preprocessor like #ifdef, #ifndef, (also can be useful for   \
    #else), etc. Otherwise, it means it's a conditional preprocessor with a  \
    condition such as #if, #elif, etc.                                       \
    */                                                                       \
    CITokens preprocessor_if_cond =                                          \
      is_def_preprocessor || in_prepro_else                                  \
        ? NEW(CITokens)                                                      \
        : scan_preprocessor_content__CIScanner(                              \
            self, CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_COND, NULL);      \
    CITokens preprocessor_if_content = NEW(CITokens);                        \
    CIScannerContext ctx =                                                   \
      NEW(CIScannerContext, ctx_location, &preprocessor_if_content);         \
    CIToken *current_token = NULL;                                           \
                                                                             \
    skip_space__CIScanner(self);                                             \
                                                                             \
    [[maybe_unused]] Usize old_position =                                    \
      self->base.source.cursor                                               \
        .position; /* Stores the old position so that it can be restored if  \
                      #endif is scanned outside the #if, #ifdef or #ifndef   \
                      preprocessor content.  */                              \
                                                                             \
    current_token =                                                          \
      get_token__CIScanner(self, &ctx, ctx_parent ? ctx_parent : &ctx);      \
                                                                             \
    while (!HAS_REACH_END(self) && current_token && cond) {                  \
        if (current_token) {                                                 \
            DEFAULT_LAST_SET_AND_CHECK(current_token);                       \
                                                                             \
            switch (current_token->kind) {                                   \
                case CI_TOKEN_KIND_COMMENT_LINE:                             \
                    DEFAULT_FILTER_TOKEN(current_token, &ctx);               \
            }                                                                \
                                                                             \
            if (HAS_REACH_END(self)) {                                       \
                FAILED("expected #endif");                                   \
                                                                             \
                break;                                                       \
            }                                                                \
        }                                                                    \
                                                                             \
        skip_space__CIScanner(self);                                         \
                                                                             \
        if (!HAS_REACH_END(self)) {                                          \
            old_position = self->base.source.cursor.position;                \
            current_token = get_token__CIScanner(                            \
              self, &ctx, ctx_parent ? ctx_parent : &ctx);                   \
        }                                                                    \
    }                                                                        \
                                                                             \
    if (!current_token) {                                                    \
        FAILED("unexpected error in #if preprocessor condition");            \
    }                                                                        \
                                                                             \
    if (is_empty__CITokens(&preprocessor_if_cond) && !is_def_preprocessor && \
        !in_prepro_else) {                                                   \
        FAILED("expected expression in #if preprocessor condition");         \
    }                                                                        \
                                                                             \
    ADD_EOT((&preprocessor_if_content), CI_TOKEN_EOT_CONTEXT_OTHER);

#define PREPROCESSOR_FILTER_CURRENT_TOKEN(ret)                                      \
    if (current_token) {                                                            \
        switch (current_token->kind) {                                              \
            case CI_TOKEN_KIND_PREPROCESSOR_ELIF:                                   \
            case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:                                \
            case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:                               \
            case CI_TOKEN_KIND_PREPROCESSOR_ELSE:                                   \
                end_token__CIScanner(self,                                          \
                                     self->base.source.cursor.line,                 \
                                     self->base.source.cursor.column,               \
                                     self->base.source.cursor.position);            \
                set_all__Location(&ret->location, &self->base.location);            \
                push_token__CIScanner(self, ctx_parent, ret);                       \
                                                                                    \
                /* Returns the last token scanned instead of the #elif              \
                preprocessor to avoid inversion when pusing tokens in the           \
                vector.                                                             \
                                                                                  \ \
                For example, if we return the #elif preprocessor in all             \
                cases, and directly push the last `current_token` (when it's        \
                a conditional preprocessor #elif, #elifdef, #elifndef or            \
                #else), the two tokens won't be in the right order in the           \
                vector (e.g. [..., current_token, preprocessor_elif] instead        \
                of [..., preprocessor_elif, current_token]). */                     \
                return current_token;                                               \
            case CI_TOKEN_KIND_PREPROCESSOR_ENDIF: {                                \
                /* When we obtain #endif in a preprocessor conditional block        \
                other than #if or #ifdef or #ifndef, we return to the               \
                initial position of the #endif.                                     \
                                                                                \   \
                NOTE:                                                               \
                cp: current_position                                                \
                op: old position                                                    \
                                                                               \    \
                  #endif   =>                  current_position - old_position      \
                ^ ^^^^^^ ^                     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      \
                |        |                                                          \
                op       cp */                                                      \
                Usize restore_count =                                               \
                  self->base.source.cursor.position - old_position;                 \
                                                                                    \
                for (Usize i = 0; i < restore_count; ++i) {                         \
                    previous_char__CIScanner(self);                                 \
                }                                                                   \
                                                                                    \
                /* Reset token position */                                          \
                start_token__CIScanner(self,                                        \
                                       self->base.source.cursor.line,               \
                                       self->base.source.cursor.column,             \
                                       self->base.source.cursor.position);          \
                                                                                    \
                FREE(CIToken, current_token);                                       \
                                                                                    \
                break;                                                              \
            }                                                                       \
            default:                                                                \
                UNREACHABLE("this situation is impossible");                        \
        }                                                                           \
    }                                                                               \
                                                                                    \
    return ret;

#define ELIF_END_COND                                               \
    current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_ELIF &&       \
      current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF &&  \
      current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF && \
      current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_ELSE &&     \
      current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_ENDIF

#define SCAN_ELIF_PREPROCESSOR()                                              \
    SCAN_IF_PREPROCESSOR_CONTENT(ELIF_END_COND,                               \
                                 false,                                       \
                                 ctx_parent,                                  \
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_IF, \
                                 false);                                      \
                                                                              \
    CIToken *preprocessor_elif = NEW_VARIANT(CIToken,                         \
                                             preprocessor_elif,               \
                                             preprocessor_if_location,        \
                                             NEW(CITokenPreprocessorElif,     \
                                                 preprocessor_if_cond,        \
                                                 preprocessor_if_content));   \
                                                                              \
    PREPROCESSOR_FILTER_CURRENT_TOKEN(preprocessor_elif);

    SCAN_ELIF_PREPROCESSOR();
}

CIToken *
scan_elifdef_preprocessor__CIScanner(CIScanner *self,
                                     CIScannerContext *ctx_parent)
{
#define SCAN_IFDEF_PREPROCESSOR(cond, ctx_parent)                             \
    Location preprocessor_ifdef_location =                                    \
      clone__Location(&self->base.location);                                  \
    String *preprocessor_ifdef_identifier = NULL;                             \
                                                                              \
    {                                                                         \
        CIScannerContext ctx =                                                \
          NEW_VARIANT(CIScannerContext, preprocessor_if, NULL);               \
        preprocessor_ifdef_identifier = scan_macro_name__CIScanner(           \
          self, &ctx, ctx_parent, CI_TOKEN_KIND_PREPROCESSOR_IFDEF);          \
    }                                                                         \
                                                                              \
    SCAN_IF_PREPROCESSOR_CONTENT(cond,                                        \
                                 true,                                        \
                                 ctx_parent,                                  \
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_IF, \
                                 false);

#define SCAN_ELIFDEF_PREPROCESSOR(ty, k)                                \
    SCAN_IFDEF_PREPROCESSOR(ELIF_END_COND, ctx_parent);                 \
                                                                        \
    CIToken *preprocessor_elifdef = NEW_VARIANT(                        \
      CIToken,                                                          \
      k,                                                                \
      preprocessor_ifdef_location,                                      \
      NEW(ty, preprocessor_ifdef_identifier, preprocessor_if_content)); \
                                                                        \
    PREPROCESSOR_FILTER_CURRENT_TOKEN(preprocessor_elifdef);

    SCAN_ELIFDEF_PREPROCESSOR(CITokenPreprocessorElifdef, preprocessor_elifdef);
}

CIToken *
scan_elifndef_preprocessor__CIScanner(CIScanner *self,
                                      CIScannerContext *ctx_parent)
{
    SCAN_ELIFDEF_PREPROCESSOR(CITokenPreprocessorElifndef,
                              preprocessor_elifndef);
}

CIToken *
scan_else_preprocessor__CIScanner(CIScanner *self, CIScannerContext *ctx_parent)
{
    skip_space_and_backslash__CIScanner(self);

    SCAN_IF_PREPROCESSOR_CONTENT(current_token->kind !=
                                   CI_TOKEN_KIND_PREPROCESSOR_ENDIF,
                                 false,
                                 ctx_parent,
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_ELSE,
                                 true);

    CIToken *res =
      NEW_VARIANT(CIToken,
                  preprocessor_else,
                  preprocessor_if_location,
                  NEW(CITokenPreprocessorElse, preprocessor_if_content));

    PREPROCESSOR_FILTER_CURRENT_TOKEN(res);
}

CIToken *
scan_embed_preprocessor__CIScanner(CIScanner *self)
{
    // TODO: Parse the content of the file of the embed preprocessor.
    CITokens content = NEW(CITokens);
    Location preprocessor_embed_location =
      clone__Location(&self->base.location);
    String *preprocessor_embed_value = NULL;

    skip_space_and_backslash__CIScanner(self);

    switch (self->base.source.cursor.current) {
        case '\"': {
            CIScannerContext ctx =
              NEW(CIScannerContext, CI_SCANNER_CONTEXT_LOCATION_NONE, NULL);
            CIToken *string_token = get_token__CIScanner(self, &ctx, NULL);

            switch (string_token->kind) {
                case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
                    preprocessor_embed_value =
                      string_token->literal_constant_string;

                    lily_free(string_token);

                    break;
                default:
                    FREE(CIToken, string_token);

                    FAILED("expected string token");
            }

            break;
        }
        default:
            FAILED("expected string literal");
    }

    // TODO: Scan parameters (limit, suffix, prefix, if_empty)
    // https://en.cppreference.com/w/c/preprocessor/embed

    return NEW_VARIANT(
      CIToken,
      preprocessor_embed,
      preprocessor_embed_location,
      NEW(CITokenPreprocessorEmbed, preprocessor_embed_value, content));
}

CIToken *
scan_error_preprocessor__CIScanner(CIScanner *self)
{
    Location preprocessor_error_location =
      clone__Location(&self->base.location);
    String *preprocessor_error_value = NEW(String);

    skip_space_and_backslash__CIScanner(self);

    while (self->base.source.cursor.current != '\n') {
        push__String(preprocessor_error_value,
                     self->base.source.cursor.current);
        next_char__CIScanner(self);
        skip_backslash__CIScanner(self);
    }

    return NEW_VARIANT(CIToken,
                       preprocessor_error,
                       preprocessor_error_location,
                       preprocessor_error_value);
}

CIToken *
scan_if_preprocessor__CIScanner(CIScanner *self)
{
    SCAN_IF_PREPROCESSOR_CONTENT(current_token->kind !=
                                   CI_TOKEN_KIND_PREPROCESSOR_ENDIF,
                                 false,
                                 NULL,
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_IF,
                                 false);

#define DROP_ENDIF()                                               \
    if (current_token &&                                           \
        current_token->kind == CI_TOKEN_KIND_PREPROCESSOR_ENDIF) { \
        FREE(CIToken, current_token);                              \
    }

    DROP_ENDIF();

    return NEW_VARIANT(CIToken,
                       preprocessor_if,
                       preprocessor_if_location,
                       NEW(CITokenPreprocessorIf,
                           preprocessor_if_cond,
                           preprocessor_if_content));
}

CIToken *
scan_ifdef_preprocessor__CIScanner(CIScanner *self)
{
    SCAN_IFDEF_PREPROCESSOR(
      current_token->kind != CI_TOKEN_KIND_PREPROCESSOR_ENDIF, NULL);
    DROP_ENDIF();

    return NEW_VARIANT(CIToken,
                       preprocessor_ifdef,
                       preprocessor_ifdef_location,
                       NEW(CITokenPreprocessorIfdef,
                           preprocessor_ifdef_identifier,
                           preprocessor_if_content));
}

CIToken *
scan_ifndef_preprocessor__CIScanner(CIScanner *self)
{
    CIToken *token = scan_ifdef_preprocessor__CIScanner(self);

    token->kind = CI_TOKEN_KIND_PREPROCESSOR_IFNDEF;
    token->preprocessor_ifndef = token->preprocessor_ifdef;

    return token;
}

CIToken *
scan_include_preprocessor__CIScanner(CIScanner *self)
{
    // TODO: Scan content of #include
    Location preprocessor_include_location =
      clone__Location(&self->base.location);
    String *preprocessor_include_value = NULL;

    skip_space_and_backslash__CIScanner(self);

    switch (self->base.source.cursor.current) {
        case '<': {
            preprocessor_include_value = NEW(String);

            next_char__CIScanner(self);

            while (self->base.source.cursor.current != '>' &&
                   !HAS_REACH_END(self)) {
                push__String(preprocessor_include_value,
                             self->base.source.cursor.current);
                next_char__CIScanner(self);
            }

            if (self->base.source.cursor.current != '>') {
                FAILED("expected `>`");
            } else {
                next_char__CIScanner(self); // skip `>`
            }

            break;
        }
        case '\"': {
            CIScannerContext ctx =
              NEW(CIScannerContext, CI_SCANNER_CONTEXT_LOCATION_NONE, NULL);
            CIToken *token_include_value =
              get_token__CIScanner(self, &ctx, NULL);

            if (token_include_value) {
                switch (token_include_value->kind) {
                    case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
                        preprocessor_include_value =
                          token_include_value->literal_constant_string;

                        next_char__CIScanner(self);
                        lily_free(token_include_value);

                        break;
                    default:
                        FREE(CIToken, token_include_value);

                        goto expected_string_literal;
                }
            } else {
            expected_string_literal:
                FAILED("expected string literal");
            }

            break;
        }
        default:
            FAILED("expected `<` or `\"`");
    }

    return NEW_VARIANT(
      CIToken,
      preprocessor_include,
      preprocessor_include_location,
      NEW(CITokenPreprocessorInclude, preprocessor_include_value));
}

CIToken *
scan_line_preprocessor__CIScanner(CIScanner *self)
{
    Location preprocessor_line_location = clone__Location(&self->base.location);
    CIScannerContext ctx =
      NEW(CIScannerContext, CI_SCANNER_CONTEXT_LOCATION_NONE, NULL);
    Usize lineno = self->base.source.cursor.line;
    String *filename = NULL; // String*?

    if (is_digit__CIScanner(self)) {
        CIToken *token = get_token__CIScanner(self, &ctx, NULL);

        if (token) {
            switch (token->kind) {
                case CI_TOKEN_KIND_LITERAL_CONSTANT_INT: {
                    Optional *op = atoi_safe__Uint64(
                      token->literal_constant_int.value->buffer, 10);

                    if (is_some__Optional(op)) {
                        lineno = (Usize)(Uptr)(Usize *)op->some;
                    }

                    FREE(Optional, op);
                    lily_free(token);

                    break;
                }
                default:
                    FREE(CIToken, token);

                    goto expected_digit;
            }
        } else {
            goto expected_digit;
        }
    } else {
    expected_digit:
        FAILED("expected digit");
    }

    if (self->base.source.cursor.current != '\n') {
        switch (self->base.source.cursor.current) {
            case '\"': {
                CIToken *token_filename =
                  get_token__CIScanner(self, &ctx, NULL);

                if (token_filename) {
                    switch (token_filename->kind) {
                        case CI_TOKEN_KIND_LITERAL_CONSTANT_STRING:
                            filename = token_filename->literal_constant_string;

                            lily_free(token_filename);

                            break;
                        default:
                            FREE(CIToken, token_filename);

                            goto expected_string_literal;
                    }
                } else {
                    goto expected_string_literal;
                }

                break;
            }
            default:
            expected_string_literal:
                FAILED("expected string literal");
        }
    }

    return NEW_VARIANT(CIToken,
                       preprocessor_line,
                       preprocessor_line_location,
                       NEW(CITokenPreprocessorLine, lineno, filename));
}

CIToken *
scan_pragma_preprocessor__CIScanner(CIScanner *self)
{
    TODO("scan #pragma");
}

CIToken *
scan_undef_preprocessor__CIScanner(CIScanner *self)
{
    Location preprocessor_undef_location =
      clone__Location(&self->base.location);
    String *preprocessor_undef_identifier = NULL;

    {
        CIScannerContext ctx =
          NEW(CIScannerContext, CI_SCANNER_CONTEXT_LOCATION_NONE, NULL);

        preprocessor_undef_identifier = scan_macro_name__CIScanner(
          self, &ctx, NULL, CI_TOKEN_KIND_PREPROCESSOR_UNDEF);
    }

    return NEW_VARIANT(CIToken,
                       preprocessor_undef,
                       preprocessor_undef_location,
                       preprocessor_undef_identifier);
}

CIToken *
scan_warning_preprocessor__CIScanner(CIScanner *self)
{
    CIToken *token = scan_error_preprocessor__CIScanner(self);

    token->kind = CI_TOKEN_KIND_PREPROCESSOR_WARNING;
    token->preprocessor_warning = token->preprocessor_error;

    return token;
}

CIToken *
scan_gnu_attribute__CIScanner(CIScanner *self,
                              CIScannerContext *ctx,
                              Location gnu_attribute_location)
{
    ASSERT(self->base.source.cursor.current == '_');

    // Eat the last character of __attrbiute__
    //                                       ^
    next_char__CIScanner(self);

    skip_space_according_ctx__CIScanner(self, ctx);

    if (self->base.source.cursor.current == '(' &&
        peek_char__CIScanner(self, 1) == (char *)'(') {
        jump__CIScanner(self, 2);
    } else {
        FAILED("expected `((` after __attribute__");
    }

    CITokens gnu_attribute_content = NEW(CITokens);
    CIScannerContext gnu_attribute_ctx = NEW(CIScannerContext,
                                             CI_SCANNER_CONTEXT_LOCATION_NONE,
                                             &gnu_attribute_content);
    bool has_reach_end = false;
    Uint32 in_paren_count = 0;

    while (self->base.source.cursor.current != ')' ||
           peek_char__CIScanner(self, 1) != (char *)')' || in_paren_count > 0) {
        if (HAS_REACH_END(self)) {
            has_reach_end = true;
            goto not_closed;
        }

        skip_space_according_ctx__CIScanner(self, ctx);

        CIToken *token = get_token__CIScanner(self, &gnu_attribute_ctx, ctx);

        if (token) {
            switch (token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                    ++in_paren_count;

                    break;
                case CI_TOKEN_KIND_RPAREN:
                    if (in_paren_count > 0) {
                        --in_paren_count;
                    } else {
                        FAILED("expected `(` before `)`");
                    }

                    break;
                default:
                    break;
            }

            DEFAULT_LAST_SET_AND_CHECK(token);

            switch (token->kind) {
                DEFAULT_FILTER_TOKEN(token, &gnu_attribute_ctx);
            }
        }
    }

    jump__CIScanner(self, 2);

not_closed:
    if (has_reach_end) {
        FAILED("GNU attribute not closed expected `))`");
    }

    add_eof_token__CIScanner(self, &gnu_attribute_ctx);

    return NEW_VARIANT(CIToken,
                       gnu_attribute,
                       gnu_attribute_location,
                       NEW(CITokenGNUAttribute, gnu_attribute_content));
}

CIToken *
get_num__CIScanner(CIScanner *self)
{
    CIToken *res = NULL; // CIToken*?

    switch (self->base.source.cursor.current) {
        // 0b, 00, 0x
        case '0': {
            char *c1 = peek_char__CIScanner(self, 1);

            if (c1 == (char *)'b') {
                jump__CIScanner(self, 2);
                res = scan_bin__CIScanner(self);
            } else if (c1 == (char *)'x') {
                jump__CIScanner(self, 2);
                res = scan_hex__CIScanner(self);
            } else if (c1 == (char *)'0') {
                jump__CIScanner(self, 2);
                res = scan_oct__CIScanner(self);
            } else {
                res = scan_num__CIScanner(self);
            }

            break;
        }
        // 1-9
        default:
            res = scan_num__CIScanner(self);
    }

    if (res) {
        // Scan suffix
        // Integer literal suffix:
        // https://en.cppreference.com/w/cpp/language/integer_literal
        // Floating point literal suffix:
        // https://en.cppreference.com/w/c/language/floating_constant
        char *c1 = peek_char__CIScanner(self, 1);
        char *c2 = peek_char__CIScanner(self, 2);
        char *c3 = peek_char__CIScanner(self, 3);
        bool is_int = res->kind == CI_TOKEN_KIND_LITERAL_CONSTANT_INT ||
                      res->kind == CI_TOKEN_KIND_LITERAL_CONSTANT_BIN ||
                      res->kind == CI_TOKEN_KIND_LITERAL_CONSTANT_HEX ||
                      res->kind == CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL;

#define SUFFIX_U (1 << 0)
#define SUFFIX_L (1 << 1)
#define SUFFIX_L2 (1 << 2)
#define SUFFIX_F (1 << 3)
#define SUFFIX_D (1 << 4)
#define SUFFIXES_ARRAY_LENGTH 5

        const int suffixes_arr[SUFFIXES_ARRAY_LENGTH] = {
            SUFFIX_U, SUFFIX_L, SUFFIX_L2, SUFFIX_F, SUFFIX_D
        };

#define ADD_SUFFIX(s)              \
    switch (s) {                   \
        case SUFFIX_L2:            \
            suffixes &= ~SUFFIX_L; \
            break;                 \
        default:                   \
            break;                 \
    }                              \
                                   \
    suffixes |= s;

#define UPDATE_FLOAT_SUFFIX(new_suffix)                      \
    switch (res->kind) {                                     \
        case CI_TOKEN_KIND_LITERAL_CONSTANT_FLOAT:           \
            res->literal_constant_float.suffix = new_suffix; \
            break;                                           \
        default:                                             \
            break;                                           \
    }

#define UPDATE_INT_SUFFIX(new_suffix)                        \
    switch (res->kind) {                                     \
        case CI_TOKEN_KIND_LITERAL_CONSTANT_INT:             \
            res->literal_constant_int.suffix = new_suffix;   \
            break;                                           \
        case CI_TOKEN_KIND_LITERAL_CONSTANT_BIN:             \
            res->literal_constant_bin.suffix = new_suffix;   \
            break;                                           \
        case CI_TOKEN_KIND_LITERAL_CONSTANT_HEX:             \
            res->literal_constant_hex.suffix = new_suffix;   \
            break;                                           \
        case CI_TOKEN_KIND_LITERAL_CONSTANT_OCTAL:           \
            res->literal_constant_octal.suffix = new_suffix; \
            break;                                           \
        default:                                             \
            break;                                           \
    }

#define CHECK_SUFFIX()                                                    \
    if (suffixes) {                                                       \
        if (suffixes & (SUFFIX_D | SUFFIX_F) && is_int) {                 \
            FAILED("these suffixes are not expected for integer");        \
        } else if (suffixes & (SUFFIX_U | SUFFIX_L2) && !is_int) {        \
            FAILED("these suffixes are not expected for floating point"); \
        }                                                                 \
    }

        int suffixes = 0;

        switch ((char)(Uptr)c1) {
            case 'e':
            case 'E':
                // For case like 0xffe3, 0b111e2 or 001234e5, ...
                TODO("exponent");
            case 'U':
            case 'u':
                ADD_SUFFIX(SUFFIX_U);

                if (c2 == (char *)'L' || c2 == (char *)'l') {
                    ADD_SUFFIX(SUFFIX_L);

                    if (c3 == (char *)'L' || c3 == (char *)'l') {
                        ADD_SUFFIX(SUFFIX_L2);
                        CHECK_SUFFIX();
                        UPDATE_INT_SUFFIX(
                          CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LLU);

                        break;
                    }

                    CHECK_SUFFIX();
                    UPDATE_INT_SUFFIX(CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LU);

                    break;
                }

                CHECK_SUFFIX();
                UPDATE_INT_SUFFIX(CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_U);

                break;
            case 'L':
            case 'l':
                ADD_SUFFIX(SUFFIX_L);

                if (c2 == (char *)'U' || c2 == (char *)'u') {
                    ADD_SUFFIX(SUFFIX_U);
                    CHECK_SUFFIX();
                    UPDATE_INT_SUFFIX(CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LU);

                    break;
                } else if (c2 == (char *)'L' || c2 == (char *)'l') {
                    ADD_SUFFIX(SUFFIX_L2);

                    if (c3 == (char *)'U' || c3 == (char *)'u') {
                        ADD_SUFFIX(SUFFIX_U);
                        CHECK_SUFFIX();
                        UPDATE_INT_SUFFIX(
                          CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LLU);

                        break;
                    }

                    CHECK_SUFFIX();
                    UPDATE_INT_SUFFIX(CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_LL);

                    break;
                }

                CHECK_SUFFIX();

                if (is_int) {
                    UPDATE_INT_SUFFIX(CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_L);
                } else {
                    UPDATE_FLOAT_SUFFIX(
                      CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_L);
                }

                break;
            case 'f':
            case 'F':
                ADD_SUFFIX(SUFFIX_F);
                CHECK_SUFFIX();
                UPDATE_FLOAT_SUFFIX(CI_TOKEN_LITERAL_CONSTANT_FLOAT_SUFFIX_F);

                break;
            // Only for GCC.
            // TODO: Finish to scan decimal suffixes
            // (https://en.cppreference.com/w/c/language/floating_constant).
            case 'd':
            case 'D':
                ADD_SUFFIX(SUFFIX_D);

                if (c2 == (char *)'F' || c2 == (char *)'f') {
                    ADD_SUFFIX(SUFFIX_F);

                    TODO("df");
                } else if (c2 == (char *)'D' || c2 == (char *)'d') {
                    ADD_SUFFIX(SUFFIX_D);

                    TODO("dd");
                } else if (c2 == (char *)'L' || c2 == (char *)'l') {
                    ADD_SUFFIX(SUFFIX_L);

                    TODO("dl");
                }

                TODO("d");
            default:
                break;
        }

        Usize character_count = 0;

        // Count the number of characters taken by the `suffixes` mask.
        for (int s = suffixes, i = 0; s > 0 && i < SUFFIXES_ARRAY_LENGTH; ++i) {
            const int current_suffix = suffixes_arr[i];

            if (s & current_suffix) {
                switch (current_suffix) {
                    case SUFFIX_L2:
                        character_count += 2;
                        break;
                    default:
                        ++character_count;
                }

                s &= ~current_suffix;
            }
        }

        jump__CIScanner(self, character_count);

#undef SUFFIX_U
#undef SUFFIX_L
#undef SUFFIX_L2
#undef SUFFIX_F
#undef SUFFIX_D
#undef SUFFIXES_ARRAY_LENGTH
#undef ADD_SUFFIX
#undef UPDATE_FLOAT_SUFFIX
#undef UPDATE_INT_SUFFIX
#undef CHECK_SUFFIX

        return res;
    }

    return NULL;
}

CIToken *
get_token__CIScanner(CIScanner *self,
                     CIScannerContext *ctx,
                     CIScannerContext *ctx_parent)
{
    char *c1 = peek_char__CIScanner(self, 1);
    char *c2 = peek_char__CIScanner(self, 2);

    start_token__CIScanner(self,
                           self->base.source.cursor.line,
                           self->base.source.cursor.column,
                           self->base.source.cursor.position);

    switch (self->base.source.cursor.current) {
        // &&, &=, &
        case '&':
            if (c1 == (char *)'&') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_AMPERSAND_AMPERSAND,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_AMPERSAND_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_AMPERSAND,
                       clone__Location(&self->base.location));
        // @
        case '@':
            return NEW(
              CIToken, CI_TOKEN_KIND_AT, clone__Location(&self->base.location));

        // !=, !
        case '!':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_BANG_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_BANG,
                       clone__Location(&self->base.location));
        // ||, |=, |
        case '|':
            if (c1 == (char *)'|') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_BAR_BAR,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_BAR_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_BAR,
                       clone__Location(&self->base.location));
        // :
        case ':':
            return NEW(CIToken,
                       CI_TOKEN_KIND_COLON,
                       clone__Location(&self->base.location));
        // ,
        case ',':
            return NEW(CIToken,
                       CI_TOKEN_KIND_COMMA,
                       clone__Location(&self->base.location));
        // COMMENT_LINE, /= COMMENT_BLOCK, COMMENT_DOC, /
        case '/':
            if (c1 == (char *)'/') {
                CIToken *token = NEW(CIToken,
                                     CI_TOKEN_KIND_COMMENT_LINE,
                                     clone__Location(&self->base.location));
                skip_comment_line__CIScanner(self);

                return token;
            } else if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_SLASH_EQ,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'*' && c2 == (char *)'*') {
                Location location = clone__Location(&self->base.location);

                jump__CIScanner(self, 3);

                return NEW_VARIANT(CIToken,
                                   comment_doc,
                                   location,
                                   scan_comment_doc__CIScanner(self));
            } else if (c1 == (char *)'*') {
                CIToken *token = NEW(CIToken,
                                     CI_TOKEN_KIND_COMMENT_BLOCK,
                                     clone__Location(&self->base.location));

                jump__CIScanner(self, 2);
                skip_comment_block__CIScanner(self);

                return token;
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_SLASH,
                       clone__Location(&self->base.location));

        // ++, +=, +
        case '+':
            if (c1 == (char *)'+') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_PLUS_PLUS,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_PLUS_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_PLUS,
                       clone__Location(&self->base.location));

        // ->, -=, -
        case '-':
            if (c1 == (char *)'>') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_ARROW,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_MINUS_EQ,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'-') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_MINUS_MINUS,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_MINUS,
                       clone__Location(&self->base.location));
        // ..., .
        case '.':
            if (c1 == (char *)'.' && c2 == (char *)'.') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_DOT_DOT_DOT,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_DOT,
                       clone__Location(&self->base.location));
        // ==, =
        case '=':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_EQ_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(
              CIToken, CI_TOKEN_KIND_EQ, clone__Location(&self->base.location));
        // #
        case '#': {
            if (is_in_macro__CIScannerContext(ctx)) {
                goto scan_hashtag;
            }

            next_char__CIScanner(self);
            skip_space_except_new_line__CIScanner(self);
            previous_char__CIScanner(self);

            // Update (only c1) peeked char
            c1 = peek_char__CIScanner(self, 1);

            if (is_ident_with_peeked_char__CIScanner(self, c1)) {
                String *id = scan_identifier_with_peek_char__CIScanner(self);
                enum CITokenKind kind = get_preprocessor__CIScanner(id);

                switch (kind) {
                    case CI_TOKEN_KIND_IDENTIFIER: {
                        end__Location(&self->base.location,
                                      self->base.source.cursor.line,
                                      self->base.source.cursor.column,
                                      self->base.source.cursor.position);
                        push_token__CIScanner(
                          self,
                          ctx,
                          NEW(CIToken,
                              CI_TOKEN_KIND_HASHTAG,
                              clone__Location(&self->base.location)));
                        next_char__CIScanner(self);

                        start_token__CIScanner(
                          self,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);
                        jump__CIScanner(self, id->len - 1);

                        return NEW_VARIANT(
                          CIToken,
                          identifier,
                          clone__Location(&self->base.location),
                          id);
                    }
                    default:
                        jump__CIScanner(self, id->len + 1);

                        FREE(String, id);

                        if (is_in_macro__CIScannerContext(ctx)) {
                            FAILED("preporcessors are not expected in macro");
                        }

                        switch (kind) {
                            case CI_TOKEN_KIND_PREPROCESSOR_DEFINE:
                                return scan_define_preprocessor__CIScanner(
                                  self);
                            case CI_TOKEN_KIND_PREPROCESSOR_ELIF:
                                if (!is_in_prepro_if__CIScannerContext(ctx)) {
                                    FAILED("#elif preprocessor is not expected "
                                           "here");
                                } else if (is_in_prepro_else__CIScannerContext(
                                             ctx)) {
                                    FAILED("cannot add #elif after #else");
                                } else {
                                    ASSERT(ctx_parent);

                                    return scan_elif_preprocessor__CIScanner(
                                      self, ctx_parent);
                                }
                            case CI_TOKEN_KIND_PREPROCESSOR_ELIFDEF:
                                if (!is_in_prepro_if__CIScannerContext(ctx)) {
                                    FAILED("#elifdef preprocessor is not "
                                           "expected here");
                                } else if (is_in_prepro_else__CIScannerContext(
                                             ctx)) {
                                    FAILED("cannot add #elifdef after #else");
                                } else {
                                    ASSERT(ctx_parent);

                                    return scan_elifdef_preprocessor__CIScanner(
                                      self, ctx_parent);
                                }
                            case CI_TOKEN_KIND_PREPROCESSOR_ELIFNDEF:
                                if (!is_in_prepro_if__CIScannerContext(ctx)) {
                                    FAILED(
                                      "#elifndef preprocessor is not expected "
                                      "here");
                                } else if (is_in_prepro_else__CIScannerContext(
                                             ctx)) {
                                    FAILED("cannot add #elifndef after #else");
                                } else {
                                    ASSERT(ctx_parent);

                                    return scan_elifndef_preprocessor__CIScanner(
                                      self, ctx_parent);
                                }
                            case CI_TOKEN_KIND_PREPROCESSOR_ELSE:
                                if (!is_in_prepro_if__CIScannerContext(ctx)) {
                                    FAILED("#else preprocessor is not expected "
                                           "here");
                                } else if (is_in_prepro_else__CIScannerContext(
                                             ctx)) {
                                    FAILED("cannot add #else after #else");
                                } else {
                                    ASSERT(ctx_parent);

                                    return scan_else_preprocessor__CIScanner(
                                      self, ctx_parent);
                                }
                            case CI_TOKEN_KIND_PREPROCESSOR_EMBED:
                                return scan_embed_preprocessor__CIScanner(self);
                            case CI_TOKEN_KIND_PREPROCESSOR_ENDIF:
                                return NEW(
                                  CIToken,
                                  CI_TOKEN_KIND_PREPROCESSOR_ENDIF,
                                  clone__Location(&self->base.location));
                            case CI_TOKEN_KIND_PREPROCESSOR_ERROR:
                                return scan_error_preprocessor__CIScanner(self);
                            case CI_TOKEN_KIND_PREPROCESSOR_IF:
                                return scan_if_preprocessor__CIScanner(self);
                            case CI_TOKEN_KIND_PREPROCESSOR_IFDEF:
                                return scan_ifdef_preprocessor__CIScanner(self);
                            case CI_TOKEN_KIND_PREPROCESSOR_IFNDEF:
                                return scan_ifndef_preprocessor__CIScanner(
                                  self);
                            case CI_TOKEN_KIND_PREPROCESSOR_INCLUDE:
                                return scan_include_preprocessor__CIScanner(
                                  self);
                            case CI_TOKEN_KIND_PREPROCESSOR_LINE:
                                return scan_line_preprocessor__CIScanner(self);
                            case CI_TOKEN_KIND_PREPROCESSOR_PRAGMA:
                                return scan_pragma_preprocessor__CIScanner(
                                  self);
                            case CI_TOKEN_KIND_PREPROCESSOR_UNDEF:
                                return scan_undef_preprocessor__CIScanner(self);
                            case CI_TOKEN_KIND_PREPROCESSOR_WARNING:
                                return scan_warning_preprocessor__CIScanner(
                                  self);
                            default:
                                UNREACHABLE("this situation is impossible");
                        }

                        return NEW(
                          CIToken, kind, clone__Location(&self->base.location));
                }
            }

            if (!is_in_macro__CIScannerContext(ctx)) {
                FAILED("'#' is not expected outside of a macro");
            }

        scan_hashtag:
            if (c1 == (char *)'#') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_HASHTAG_HASHTAG,
                           clone__Location(&self->base.location));
            }

            {
                end_token__CIScanner(self,
                                     self->base.source.cursor.line,
                                     self->base.source.cursor.column,
                                     self->base.source.cursor.position);
                push_token__CIScanner(
                  self,
                  ctx,
                  NEW(CIToken,
                      CI_TOKEN_KIND_HASHTAG,
                      clone__Location(&self->base.location)));
            }

            next_char__CIScanner(self);
            skip_space__CIScanner(self);
            start_token__CIScanner(self,
                                   self->base.source.cursor.line,
                                   self->base.source.cursor.column,
                                   self->base.source.cursor.position);

            if (is_ident__CIScanner(self)) {
                CIToken *token_id =
                  scan_multi_part_keyword__CIScanner(self, ctx);

                if (token_id) {
                    end__Location(&token_id->location,
                                  self->base.source.cursor.line,
                                  self->base.source.cursor.column,
                                  self->base.source.cursor.position);
                    push_token__CIScanner(self, ctx, token_id);
                }
            } else {
                return NULL;
            }

            start_token__CIScanner(self,
                                   self->base.source.cursor.line,
                                   self->base.source.cursor.column,
                                   self->base.source.cursor.position);

            return NEW_VARIANT(
              CIToken,
              eot,
              clone__Location(&self->base.location),
              NEW(CITokenEot, CI_TOKEN_EOT_CONTEXT_STRINGIFICATION));
        }
        // ^=, ^
        case '^':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_HAT_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_HAT,
                       clone__Location(&self->base.location));
        // IDENTIFIER, KEYWORD
        case IS_ID:
            return scan_multi_part_keyword__CIScanner(self, ctx);
        // ?
        case '?':
            return NEW(CIToken,
                       CI_TOKEN_KIND_INTERROGATION,
                       clone__Location(&self->base.location));
        // {
        case '{':
            return NEW(CIToken,
                       CI_TOKEN_KIND_LBRACE,
                       clone__Location(&self->base.location));
        // [
        case '[':
            return NEW(CIToken,
                       CI_TOKEN_KIND_LHOOK,
                       clone__Location(&self->base.location));
        // (
        case '(':
            return NEW(CIToken,
                       CI_TOKEN_KIND_LPAREN,
                       clone__Location(&self->base.location));
        // }
        case '}':
            return NEW(CIToken,
                       CI_TOKEN_KIND_RBRACE,
                       clone__Location(&self->base.location));
        // ]
        case ']':
            return NEW(CIToken,
                       CI_TOKEN_KIND_RHOOK,
                       clone__Location(&self->base.location));
        // )
        case ')':
            return NEW(CIToken,
                       CI_TOKEN_KIND_RPAREN,
                       clone__Location(&self->base.location));
        // <=, <<=, <<, <
        case '<':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_LSHIFT_EQ,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'<' && c2 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'<') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_LSHIFT_LSHIFT,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_LSHIFT,
                       clone__Location(&self->base.location));

        // >=, >>=, >>, >
        case '>':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_RSHIFT_EQ,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'>' && c2 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ,
                           clone__Location(&self->base.location));
            } else if (c1 == (char *)'>') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_RSHIFT_RSHIFT,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_RSHIFT,
                       clone__Location(&self->base.location));

        // ;
        case ';':
            return NEW(CIToken,
                       CI_TOKEN_KIND_SEMICOLON,
                       clone__Location(&self->base.location));

        // *=, *
        case '*':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_STAR_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_STAR,
                       clone__Location(&self->base.location));

        // %, %=
        case '%':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_PERCENTAGE_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_PERCENTAGE,
                       clone__Location(&self->base.location));

        // ~, ~=
        case '~':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_WAVE_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_WAVE,
                       clone__Location(&self->base.location));

        case IS_ZERO:
            if (c1 == (char *)'x' || c1 == (char *)'0' || c1 == (char *)'.' ||
                !(c1 >= (char *)'0' && c1 <= (char *)'9')) {
                return get_num__CIScanner(self);
            }

            while (self->base.source.cursor.current == '0') {
                next_char__CIScanner(self);
            }

            if (self->base.source.cursor.current != '.' &&
                !(self->base.source.cursor.current >= '0' &&
                  self->base.source.cursor.current <= '9')) {
                previous_char__CIScanner(self);
                return NEW_VARIANT(
                  CIToken,
                  literal_constant_int,
                  clone__Location(&self->base.location),
                  NEW(CITokenLiteralConstantInt,
                      CI_TOKEN_LITERAL_CONSTANT_INT_SUFFIX_NONE,
                      from__String("0")));
            }

            return get_num__CIScanner(self);

        // number
        case IS_DIGIT_WITHOUT_ZERO:
            return get_num__CIScanner(self);

        case '\'': {
            char *res = scan_character__CIScanner(self);

            if (res) {
                return NEW_VARIANT(CIToken,
                                   literal_constant_character,
                                   clone__Location(&self->base.location),
                                   (char)(Uptr)res);
            }

            return NULL;
        }
        case '\"': {
            String *res = scan_string__CIScanner(self);

            if (res) {
                return NEW_VARIANT(CIToken,
                                   literal_constant_string,
                                   clone__Location(&self->base.location),
                                   res);
            }

            return NULL;
        }
        default:
            FAILED("unexpected token");
    }

    return NULL;
}

void
run__CIScanner(CIScanner *self, bool dump_scanner)
{
    CIScannerContext ctx =
      NEW(CIScannerContext, CI_SCANNER_CONTEXT_LOCATION_NONE, &self->tokens);

    if (self->base.source.file->len > 1) {
        while (!HAS_REACH_END(self)) {
            skip_space__CIScanner(self);

            if (HAS_REACH_END(self)) {
                break;
            }

            CIToken *token = get_token__CIScanner(self, &ctx, NULL);

            if (token) {
                DEFAULT_LAST_SET_AND_CHECK(token);

                switch (token->kind) {
                    case CI_TOKEN_KIND_COMMENT_LINE:
                        DEFAULT_FILTER_TOKEN(token, &ctx);
                }

                if (HAS_REACH_END(self)) {
                    break;
                }
            }
        }
    }

    add_eof_token__CIScanner(self, &ctx);

#ifndef CI_DEBUG_SCANNER
    if (dump_scanner) {
        printf("====Scanner(%s)====\n", self->base.source.file->name);

        PRINTLN("{Sr}", to_string__CITokens(&self->tokens));
    }
#else
    printf("====Scanner(%s)====\n", self->base.source.file->name);

    CALL_DEBUG(CITokens, &self->tokens);
#endif

    if (*self->base.count_error > 0) {
        exit(1);
    }
}
