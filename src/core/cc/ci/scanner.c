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

#include <base/assert.h>
#include <base/print.h>

#include <core/cc/ci/scanner.h>
#include <core/shared/diagnostic.h>

#include <stdio.h>
#include <stdlib.h>

struct CITokenKindWithID
{
    String *id; // String*
    Location location;
    bool is_merged;
    enum CITokenKind kind;
};

/// @brief Move next one character.
/// @see `include/core/shared/scanner.h`
static inline void
next_char__CIScanner(CIScanner *self);

/// @brief Move back one character.
/// @see `include/core/shared/scanner.h`
static inline void
previous_char__CIScanner(CIScanner *self);

/// @brief Skip space (new line, tab, ...).
/// @see `include/core/shared/scanner.h`
static inline void
skip_space__CIScanner(CIScanner *self);

/// @brief Skip a blank space. If the current character is not an empty space,
/// we won't do anything.
static void
skip_one_blank_space__CIScanner(CIScanner *self);

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

/// @brief Check if current can be an identifier.
static inline bool
is_ident__CIScanner(const CIScanner *self);

/// @brief Check if current can be a start of identifier.
static inline bool
is_start_ident__CIScanner(const CIScanner *self);

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

/// @brief Push token to tokens.
static inline void
push_token__CIScanner(CIScanner *self, CIToken *token);

/// @brief Next char according the token.
static void
next_char_by_token__CIScanner(CIScanner *self, const CIToken *token);

/// @brief Get attribute from id.
static enum CITokenKind
get_attribute__CIScanner(const char *id);

/// @brief Get single keyword from id.
static enum CITokenKind
get_single_keyword__CIScanner(const String *id);

/// @example long => long int, long int => long int, signed long => long int
static enum CITokenKind
standarize_keyword__CIScanner(enum CITokenKind kind);

/// @brief Check whether we can execute the keyword part 1.
static inline bool can_run_keyword_part1__CIScanner(enum CITokenKind);

/// @brief Check whether we can execute the keyword part 2.
static inline bool
can_run_keyword_part2__CIScanner(enum CITokenKind part1);

/// @brief Check whether we can execute the keyword part 3.
static inline bool
can_run_keyword_part3__CIScanner(enum CITokenKind part2);

/// @brief Check whether we can execute the keyword part 4.
static inline bool
can_run_keyword_part4__CIScanner(enum CITokenKind part3);

/// @brief Get keyword in on part.
/// @example int, long, ...
static struct CITokenKindWithID
get_keyword_part1__CIScanner(CIScanner *self,
                             [[maybe_unused]] struct CITokenKindWithID *);

/// @brief Get keyword in two parts.
/// @example long long, signed short, ...
static struct CITokenKindWithID
get_keyword_part2__CIScanner(CIScanner *self, struct CITokenKindWithID *part1);

/// @brief Get keyword in three parts.
/// @example unsigned long long, long long int, ...
static struct CITokenKindWithID
get_keyword_part3__CIScanner(CIScanner *self, struct CITokenKindWithID *part2);

/// @brief Get keyword in four parts.
/// @example unsigned long long int, ...
static struct CITokenKindWithID
get_keyword_part4__CIScanner(CIScanner *self, struct CITokenKindWithID *part3);

/// @brief Scan multi-part keyword.
/// @example long int, long long, unsigned long long, ...
static CIToken *
scan_multi_part_keyword__CIScanner(CIScanner *self);

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

/// @brief Scan character constant literal.
static char *
scan_character__CIScanner(CIScanner *self);

/// @brief Scan string constant literal.
static String *
scan_string__CIScanner(CIScanner *self);

/// @brief Scan octal constant literal.
static CIToken *
scan_hex__CIScanner(CIScanner *self);

/// @brief Scan octal or binary constant literal.
/// @example 00101, 00334
static CIToken *
scan_octal_or_binary__CIScanner(CIScanner *self);

/// @brief Scan number (decimal and float).
static CIToken *
scan_num__CIScanner(CIScanner *self);

/// @brief Scan all numbers (hexadecimal, octal, binary, decimal and float).
static CIToken *
get_num__CIScanner(CIScanner *self);

/// @brief Skip and verify if the next character is the target.
static bool
skip_and_verify__CIScanner(CIScanner *self, char target);

/// @brief Scan to the next closing character.
static CIToken *
get_closing__CIScanner(CIScanner *self, char target);

/// @brief Check that the token is available in accordance with the standard.
/// @note This function can modify the token type.
static void
check_standard(CIScanner *self, CIToken *token);

/// @brief Get token from characters.
static CIToken *
get_token__CIScanner(CIScanner *self, bool check_match);

static const CIFeature tokens_feature[CI_TOKEN_KIND_MAX] = {
    [CI_TOKEN_KIND_AMPERSAND] = { .since = CI_STANDARD_NONE,
                                  .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_AMPERSAND_AMPERSAND] = { .since = CI_STANDARD_NONE,
                                            .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_AMPERSAND_EQ] = { .since = CI_STANDARD_NONE,
                                     .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_ARROW] = { .since = CI_STANDARD_NONE,
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
    [CI_TOKEN_KIND_EQ] = { .since = CI_STANDARD_NONE,
                           .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_EQ_EQ] = { .since = CI_STANDARD_NONE,
                              .until = CI_STANDARD_NONE },
    [CI_TOKEN_KIND_HASHTAG] = { .since = CI_STANDARD_NONE,
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
    [CI_TOKEN_KIND_PREPROCESSOR_EMBED] = { .since = CI_STANDARD_23,
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
    [CI_TOKEN_KIND_PREPROCESSOR_WARNING] = { .since = CI_STANDARD_23,
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

#define CHECK_STANDARD_SINCE(since, block) \
    if (self->standard < since) {          \
        block;                             \
    }

#define CHECK_STANDARD_UNTIL(until, block) \
    if (self->standard >= until) {         \
        block;                             \
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

void
skip_space__CIScanner(CIScanner *self)
{
    return skip_space__Scanner(&self->base);
}

void
skip_one_blank_space__CIScanner(CIScanner *self)
{
    if (self->base.source.cursor.current == ' ') {
        next_char__CIScanner(self);
    }
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
is_ident__CIScanner(const CIScanner *self)
{
    return is_digit__CIScanner(self) ||
           (self->base.source.cursor.current >= 'a' &&
            self->base.source.cursor.current <= 'z') ||
           (self->base.source.cursor.current >= 'A' &&
            self->base.source.cursor.current <= 'Z') ||
           self->base.source.cursor.current == '_' ||
           self->base.source.cursor.current == '$';
}

bool
is_start_ident__CIScanner(const CIScanner *self)
{
    return (self->base.source.cursor.current >= 'a' &&
            self->base.source.cursor.current <= 'z') ||
           (self->base.source.cursor.current >= 'A' &&
            self->base.source.cursor.current <= 'Z') ||
           self->base.source.cursor.current == '_';
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

void
push_token__CIScanner(CIScanner *self, CIToken *token)
{
    push__Vec(self->tokens, token);
}

void
next_char_by_token__CIScanner(CIScanner *self, const CIToken *token)
{
    switch (token->kind) {
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
            return next_char__CIScanner(self);
        case CI_TOKEN_KIND_LSHIFT_LSHIFT_EQ:
        case CI_TOKEN_KIND_RSHIFT_RSHIFT_EQ:
            return jump__CIScanner(self, 2);
        default:
            return;
    }
}

enum CITokenKind
get_attribute__CIScanner(const char *id)
{
    if (!strcmp(id, "deprecated"))
        return CI_TOKEN_KIND_ATTRIBUTE_DEPRECATED;
    if (!strcmp(id, "fallthrough"))
        return CI_TOKEN_KIND_ATTRIBUTE_FALLTHROUGH;
    if (!strcmp(id, "maybe_unused"))
        return CI_TOKEN_KIND_ATTRIBUTE_MAYBE_UNUSED;
    if (!strcmp(id, "nodiscard"))
        return CI_TOKEN_KIND_ATTRIBUTE_NODISCARD;
    if (!strcmp(id, "noreturn"))
        return CI_TOKEN_KIND_ATTRIBUTE_NORETURN;
    if (!strcmp(id, "_Noreturn"))
        return CI_TOKEN_KIND_ATTRIBUTE__NORETURN;
    if (!strcmp(id, "unsequenced"))
        return CI_TOKEN_KIND_ATTRIBUTE_UNSEQUENCED;
    if (!strcmp(id, "reproducible"))
        return CI_TOKEN_KIND_ATTRIBUTE_REPRODUCIBLE;

    return CI_TOKEN_KIND_IDENTIFIER;
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
standarize_keyword__CIScanner(enum CITokenKind kind)
{
    switch (kind) {
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

bool can_run_keyword_part1__CIScanner(enum CITokenKind)
{
    return true;
}

bool
can_run_keyword_part2__CIScanner(enum CITokenKind part1)
{
    switch (part1) {
        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
        case CI_TOKEN_KIND_KEYWORD_FLOAT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
        case CI_TOKEN_KIND_KEYWORD_LONG:
            return true;
        default:
            return false;
    }
}

bool
can_run_keyword_part3__CIScanner(enum CITokenKind part2)
{
    switch (part2) {
        case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT:
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG:
        case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
        case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE:
            return true;
        default:
            return false;
    }
}

bool
can_run_keyword_part4__CIScanner(enum CITokenKind part3)
{
    switch (part3) {
        case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG:
        case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG:
            return true;
        default:
            return false;
    }
}

// TODO: For get_keyword_part*__CIScanner functions, perhaps optimize and make
// the function less redundant.

// FIXME: e.g. signed long long int
// input: signed long long int
// standard=c89
//
// expected:
// long int
// long int
//
// obtained:
// long int
// long int
// int
//
// Bug: There's a problem with merging types, for example when the standard is
// C89, because the `signed long long int` keyword (type) is not available in
// this standard (since C99). On the other hand, I don't think this is such a
// serious bug.
//
// Potential fix: To fix the bug, you'd probably have to use the
// get_multi_part_keyword__CIScanner function, just after checking the standard
// in the get_keyword_part*__CIScanner functions, if the keyword is not
// available in this standard.

struct CITokenKindWithID
get_keyword_part1__CIScanner(CIScanner *self,
                             [[maybe_unused]] struct CITokenKindWithID *)
{
    ASSERT(is_start_ident__CIScanner(self));

    String *id = scan_identifier__CIScanner(self);
    enum CITokenKind kind = get_single_keyword__CIScanner(id);

    end_token__CIScanner(self,
                         self->base.source.cursor.line,
                         self->base.source.cursor.column,
                         self->base.source.cursor.position);

    struct CITokenKindWithID res =
      (struct CITokenKindWithID){ .id = id,
                                  .is_merged = false,
                                  .location =
                                    clone__Location(&self->base.location),
                                  .kind = kind };
    const CIFeature *feature = &tokens_feature[res.kind];

    CHECK_STANDARD_SINCE(feature->since,
                         { res.kind = CI_TOKEN_KIND_IDENTIFIER; });

    return res;
}

struct CITokenKindWithID
get_keyword_part2__CIScanner(CIScanner *self, struct CITokenKindWithID *part1)
{
    ASSERT(is_start_ident__CIScanner(self));

    start_token__CIScanner(self,
                           self->base.source.cursor.line,
                           self->base.source.cursor.column,
                           self->base.source.cursor.position);

    String *id = scan_identifier__CIScanner(self);
    enum CITokenKind unmerged_kind = get_single_keyword__CIScanner(id);

    end_token__CIScanner(self,
                         self->base.source.cursor.line,
                         self->base.source.cursor.column,
                         self->base.source.cursor.position);

    switch (unmerged_kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return (struct CITokenKindWithID){ .kind = unmerged_kind,
                                               .is_merged = false,
                                               .location = clone__Location(
                                                 &self->base.location),
                                               .id = id };
        default: {
            enum CITokenKind merged_kind;

            switch (part1->kind) {
                case CI_TOKEN_KIND_KEYWORD_DOUBLE:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_DOUBLE__COMPLEX;
                            break;
                        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_DOUBLE__IMAGINARY;
                            break;
                        default:
                            FAILED("error, expected _Complex or _Imaginary "
                                   "after double");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_FLOAT:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_FLOAT__COMPLEX;
                            break;
                        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_FLOAT__IMAGINARY;
                            break;
                        default:
                            FAILED("error, expected _Complex or _Imaginary "
                                   "after float");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_LONG:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_DOUBLE:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_LONG_INT;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_LONG:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_LONG_LONG;
                            break;
                        default:
                            FAILED("error, expected int or long after long");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_SIGNED:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_CHAR:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_SIGNED_CHAR;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_SHORT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_SIGNED_INT;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_LONG:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_SIGNED_LONG;
                            break;
                        default:
                            FAILED(
                              "error, expected char, short, int after signed");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNSIGNED:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_CHAR:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_UNSIGNED_CHAR;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_SHORT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT;
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_UNSIGNED_INT;
                        case CI_TOKEN_KIND_KEYWORD_LONG:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG;
                        default:
                            FAILED("error, expected char, short, int or long "
                                   "after unsigned");
                    }

                    break;
                default:
                    UNREACHABLE("this situation is impossible");
            }

            struct CITokenKindWithID res = (struct CITokenKindWithID){
                .kind = merged_kind,
                .is_merged = false,
                .location = clone__Location(&self->base.location),
                .id = id
            };
            const CIFeature *feature = &tokens_feature[res.kind];

            CHECK_STANDARD_SINCE(feature->since, { res.kind = unmerged_kind; })
            else
            {
                part1->is_merged = true;
            }

            return res;
        }
    }
}

struct CITokenKindWithID
get_keyword_part3__CIScanner(CIScanner *self, struct CITokenKindWithID *part2)
{
    ASSERT(is_start_ident__CIScanner(self));

    start_token__CIScanner(self,
                           self->base.source.cursor.line,
                           self->base.source.cursor.column,
                           self->base.source.cursor.position);

    String *id = scan_identifier__CIScanner(self);
    enum CITokenKind unmerged_kind = get_single_keyword__CIScanner(id);

    end_token__CIScanner(self,
                         self->base.source.cursor.line,
                         self->base.source.cursor.column,
                         self->base.source.cursor.position);

    switch (unmerged_kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return (struct CITokenKindWithID){ .kind = unmerged_kind,
                                               .is_merged = false,
                                               .location = clone__Location(
                                                 &self->base.location),
                                               .id = id };
        default: {
            enum CITokenKind merged_kind;

            switch (part2->kind) {
                case CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD__COMPLEX:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__COMPLEX;
                            break;
                        case CI_TOKEN_KIND_KEYWORD__IMAGINARY:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_LONG_DOUBLE__IMAGINARY;
                            break;
                        default:
                            FAILED("error, expected _Complex or _Imaginary "
                                   "after long double");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_SIGNED_SHORT_INT;
                            break;
                        default:
                            FAILED("expected int, after signed short");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_UNSIGNED_SHORT_INT;
                            break;
                        default:
                            FAILED("expected int, after unsigned short");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_INT;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_LONG:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG;
                            break;
                        default:
                            UNREACHABLE(
                              "expected int or long, after signed long");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_INT;
                            break;
                        case CI_TOKEN_KIND_KEYWORD_LONG:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG;
                            break;
                        default:
                            UNREACHABLE(
                              "expected int or long, after unsigned long");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_LONG_LONG:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind = CI_TOKEN_KIND_KEYWORD_LONG_LONG_INT;
                            break;
                        default:
                            UNREACHABLE("expected int, after long long");
                    }

                    break;
                default:
                    UNREACHABLE("this situation is impossible");
            }

            struct CITokenKindWithID res = (struct CITokenKindWithID){
                .kind = merged_kind,
                .is_merged = false,
                .location = clone__Location(&self->base.location),
                .id = id
            };

            const CIFeature *feature = &tokens_feature[res.kind];

            CHECK_STANDARD_SINCE(feature->since, { res.kind = unmerged_kind; })
            else
            {
                part2->is_merged = true;
            }

            return res;
        }
    }
}

struct CITokenKindWithID
get_keyword_part4__CIScanner(CIScanner *self, struct CITokenKindWithID *part3)
{
    ASSERT(is_start_ident__CIScanner(self));

    start_token__CIScanner(self,
                           self->base.source.cursor.line,
                           self->base.source.cursor.column,
                           self->base.source.cursor.position);

    String *id = scan_identifier__CIScanner(self);
    enum CITokenKind unmerged_kind = get_single_keyword__CIScanner(id);

    end_token__CIScanner(self,
                         self->base.source.cursor.line,
                         self->base.source.cursor.column,
                         self->base.source.cursor.position);

    switch (unmerged_kind) {
        case CI_TOKEN_KIND_IDENTIFIER:
            return (struct CITokenKindWithID){ .kind = unmerged_kind,
                                               .is_merged = false,
                                               .location = clone__Location(
                                                 &self->base.location),
                                               .id = id };
        default: {
            enum CITokenKind merged_kind;

            switch (part3->kind) {
                case CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_SIGNED_LONG_LONG_INT;
                            break;
                        default:
                            FAILED("expected int, after signed short");
                    }

                    break;
                case CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG:
                    switch (unmerged_kind) {
                        case CI_TOKEN_KIND_KEYWORD_INT:
                            merged_kind =
                              CI_TOKEN_KIND_KEYWORD_UNSIGNED_LONG_LONG_INT;
                            break;
                        default:
                            FAILED("expected int, after unsigned short");
                    }

                    break;
                default:
                    UNREACHABLE("this situation is impossible");
            }

            struct CITokenKindWithID res = (struct CITokenKindWithID){
                .kind = merged_kind,
                .is_merged = false,
                .location = clone__Location(&self->base.location),
                .id = id
            };

            const CIFeature *feature = &tokens_feature[res.kind];

            CHECK_STANDARD_SINCE(feature->since, { res.kind = unmerged_kind; })
            else
            {
                part3->is_merged = true;
            }

            return res;
        }
    }
}

CIToken *
scan_multi_part_keyword__CIScanner(CIScanner *self)
{
#define MAX_KEYWORD_PART 4
    struct CITokenKindWithID (*const get_keyword_part[MAX_KEYWORD_PART])(
      CIScanner *, struct CITokenKindWithID *) = {
        &get_keyword_part1__CIScanner,
        &get_keyword_part2__CIScanner,
        &get_keyword_part3__CIScanner,
        &get_keyword_part4__CIScanner,
    };
    bool (*const can_run_keyword_part[MAX_KEYWORD_PART])(
      enum CITokenKind) = { &can_run_keyword_part1__CIScanner,
                            &can_run_keyword_part2__CIScanner,
                            &can_run_keyword_part3__CIScanner,
                            &can_run_keyword_part4__CIScanner };
    struct CITokenKindWithID token_kind_with_id_s[MAX_KEYWORD_PART] = { 0 };
    Usize part = 0;

    for (;
         part < MAX_KEYWORD_PART && is_start_ident__CIScanner(self) &&
         can_run_keyword_part[part](
           part == 0 ? CI_TOKEN_KIND_MAX : token_kind_with_id_s[part - 1].kind);
         ++part,
         next_char__CIScanner(self),
         skip_one_blank_space__CIScanner(self)) {
        const struct CITokenKindWithID token_kind_with_id =
          get_keyword_part[part](
            self, part == 0 ? NULL : &token_kind_with_id_s[part - 1]);

        token_kind_with_id_s[part] = token_kind_with_id;
    }

    previous_char__CIScanner(self);

    CIToken *last_token = NULL;

    for (Usize i = 0; i < part; ++i) {
        struct CITokenKindWithID *current = &token_kind_with_id_s[i];

        if (current->is_merged) {
            FREE(String, current->id);
            continue;
        }

        CIToken *current_token = NULL;

        switch (current->kind) {
            case CI_TOKEN_KIND_IDENTIFIER:
                current_token = NEW_VARIANT(
                  CIToken, identifier, current->location, current->id);
                break;
            default:
                FREE(String, current->id);
                current_token =
                  NEW(CIToken,
                      standarize_keyword__CIScanner(current->kind),
                      current->location);
        }

        if (i + 1 == part) {
            last_token = current_token;
        } else {
            push_token__CIScanner(self, current_token);
        }
    }

    return last_token;
}

CIToken *
scan_attribute__CIScanner(CIScanner *self)
{
    // TODO: Add function to expect a character.

    // Skip `[[`
    jump__CIScanner(self, 2);

    String *attribute = scan_identifier__CIScanner(self);
    enum CITokenKind kind = get_attribute__CIScanner(attribute->buffer);
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
    while (self->base.source.cursor.current != '*' &&
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
    while (self->base.source.cursor.current != '*' &&
           peek_char__CIScanner(self, 1) != (char *)'*' &&
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
                    res = from__String("\'");
                    break;
                case '\"':
                    res = from__String("\"");
                    break;
                default:
                    end__Location(&location_error,
                                  self->base.source.cursor.line,
                                  self->base.source.cursor.column,
                                  self->base.source.cursor.position);

                    if (self->base.source.cursor.position >=
                        self->base.source.file->len - 1) {
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
            break;
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
scan_hex__CIScanner(CIScanner *self)
{
    String *res = NEW(String);

    scan_and_append_chars__CIScanner(self, res, &is_hex__CIScanner);
    previous_char__CIScanner(self);

    return NEW_VARIANT(CIToken,
                       literal_constant_hex,
                       clone__Location(&self->base.location),
                       res);
}

CIToken *
scan_octal_or_binary__CIScanner(CIScanner *self)
{
    String *res = NEW(String);

    scan_and_append_chars__CIScanner(self, res, &is_bin__CIScanner);

    if (is_oct__CIScanner(self)) {
        scan_and_append_chars__CIScanner(self, res, &is_oct__CIScanner);
        previous_char__CIScanner(self);

        return NEW_VARIANT(CIToken,
                           literal_constant_octal,
                           clone__Location(&self->base.location),
                           res);
    }

    previous_char__CIScanner(self);

    return NEW_VARIANT(CIToken,
                       literal_constant_bin,
                       clone__Location(&self->base.location),
                       res);
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
                           res);
    }

    return NEW_VARIANT(CIToken,
                       literal_constant_int,
                       clone__Location(&self->base.location),
                       res);
}

CIToken *
get_num__CIScanner(CIScanner *self)
{
    switch (self->base.source.cursor.current) {
        // 00, 0x
        case '0': {
            char *c1 = peek_char__CIScanner(self, 1);

            if (c1 == (char *)'x') {
                jump__CIScanner(self, 2);
                return scan_hex__CIScanner(self);
            } else if (c1 == (char *)'0') {
                jump__CIScanner(self, 2);
                return scan_octal_or_binary__CIScanner(self);
            } else {
                return scan_num__CIScanner(self);
            }
        }
        // 1-9
        default:
            return scan_num__CIScanner(self);
    }
}

bool
skip_and_verify__CIScanner(CIScanner *self, char target)
{
    skip_space__CIScanner(self);
    return self->base.source.cursor.current != target;
}

CIToken *
get_closing__CIScanner(CIScanner *self, char target)
{
    Location location_error = clone__Location(&self->base.location);

    skip_space__CIScanner(self);

    // Check if the closing delimiter is not closed.
    while (skip_and_verify__CIScanner(self, target)) {
        if (self->base.source.cursor.position >=
            self->base.source.file->len - 1) {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_ci_error,
                self->base.source.file,
                &location_error,
                NEW(CIError, CI_ERROR_KIND_MISMATCHED_CLOSING_DELIMITER),
                NULL,
                NULL,
                from__String("expected closing delimiter after this token, "
                             "such as `)`, `}` or `]`")),
              self->base.count_error);

            return NULL;
        }

        CIToken *token = get_token__CIScanner(self, true);

        if (token) {
            next_char_by_token__CIScanner(self, token);
            end_token__CIScanner(self,
                                 self->base.source.cursor.line,
                                 self->base.source.cursor.column,
                                 self->base.source.cursor.position);

            switch (token->kind) {
                case CI_TOKEN_KIND_LPAREN:
                case CI_TOKEN_KIND_LBRACE:
                case CI_TOKEN_KIND_LHOOK:
                    break;
                default:
                    set_all__Location(&token->location, &self->base.location);
            }

            check_standard(self, token);

            switch (token->kind) {
                case CI_TOKEN_KIND_COMMENT_LINE:
                case CI_TOKEN_KIND_COMMENT_BLOCK:
                    FREE(CIToken, token);
                    break;
                default:
                    next_char__CIScanner(self);
                    push_token__CIScanner(self, token);
            }
        }
    }

    start_token__CIScanner(self,
                           self->base.source.cursor.line,
                           self->base.source.cursor.column,
                           self->base.source.cursor.position);

    switch (target) {
        case ')':
            return NEW(CIToken,
                       CI_TOKEN_KIND_RPAREN,
                       clone__Location(&self->base.location));
        case '}':
            return NEW(CIToken,
                       CI_TOKEN_KIND_RBRACE,
                       clone__Location(&self->base.location));
        case ']':
            return NEW(CIToken,
                       CI_TOKEN_KIND_RHOOK,
                       clone__Location(&self->base.location));
        default:
            UNREACHABLE("this way is not possible");
    }
}

void
check_standard(CIScanner *self, CIToken *token)
{
    ASSERT(token->kind < CI_TOKEN_KIND_MAX);

    Location location_error = clone__Location(&token->location);
    const CIFeature *feature = &tokens_feature[token->kind];

    CHECK_STANDARD_SINCE(feature->since, {
        enum CIErrorKind error_kind;

        switch (feature->since) {
            case CI_STANDARD_NONE:
                return;
            case CI_STANDARD_KR:
                UNREACHABLE("since: no error with K&R standard");
            case CI_STANDARD_89:
                error_kind = CI_ERROR_KIND_REQUIRED_C89_OR_LATER;

                break;
            case CI_STANDARD_95:
                error_kind = CI_ERROR_KIND_REQUIRED_C95_OR_LATER;

                break;
            case CI_STANDARD_99:
                error_kind = CI_ERROR_KIND_REQUIRED_C99_OR_LATER;

                break;
            case CI_STANDARD_11:
                error_kind = CI_ERROR_KIND_REQUIRED_C11_OR_LATER;

                break;
            case CI_STANDARD_17:
                error_kind = CI_ERROR_KIND_REQUIRED_C17_OR_LATER;

                break;
            case CI_STANDARD_23:
                error_kind = CI_ERROR_KIND_REQUIRED_C23_OR_LATER;

                break;
            default:
                UNREACHABLE("unknown standard");
        }

        emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                     simple_ci_error,
                                     self->base.source.file,
                                     &location_error,
                                     NEW(CIError, error_kind),
                                     NULL,
                                     NULL,
                                     NULL),
                         self->base.count_error);
    });

    CHECK_STANDARD_UNTIL(feature->until, {
        String *note = NULL;

        switch (feature->until) {
            case CI_STANDARD_NONE:
                return;
            case CI_STANDARD_KR:
                UNREACHABLE("since: no error with K&R standard");
            case CI_STANDARD_89:
                note = from__String("this feature is only available up to C89");

                break;
            case CI_STANDARD_95:
                note = from__String("this feature is only available up to C95");

                break;
            case CI_STANDARD_99:
                note = from__String("this feature is only available up to C99");

                break;
            case CI_STANDARD_11:
                note = from__String("this feature is only available up to C11");

                break;
            case CI_STANDARD_17:
                note = from__String("this feature is only available up to C17");

                break;
            case CI_STANDARD_23:
                note = from__String("this feature is only available up to C23");

                break;
            default:
                UNREACHABLE("unknown standard");
        }

        emit_note__Diagnostic(NEW_VARIANT(Diagnostic,
                                          simple_ci_note,
                                          self->base.source.file,
                                          &location_error,
                                          note,
                                          NULL,
                                          NULL,
                                          NULL));
    });
}

CIToken *
get_token__CIScanner(CIScanner *self, bool check_match)
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
        // ->, -
        case '-':
            if (c1 == (char *)'>') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_ARROW,
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
        case '#':
            return NEW(CIToken,
                       CI_TOKEN_KIND_HASHTAG,
                       clone__Location(&self->base.location));
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
        // IDENTIFIER
        case IS_ID:
            return scan_multi_part_keyword__CIScanner(self);
        // ?
        case '?':
            return NEW(CIToken,
                       CI_TOKEN_KIND_INTERROGATION,
                       clone__Location(&self->base.location));
        // {}, [], ()
        case '{':
        case '[':
        case '(': {
            char match = self->base.source.cursor.current;
            CIToken *token = NULL;

            switch (match) {
                case '{':
                    token = NEW(CIToken,
                                CI_TOKEN_KIND_LBRACE,
                                clone__Location(&self->base.location));

                    break;
                case '[':
                    if (c1 == (char *)'[') {
                        return scan_attribute__CIScanner(self);
                    }

                    token = NEW(CIToken,
                                CI_TOKEN_KIND_LHOOK,
                                clone__Location(&self->base.location));

                    break;
                case '(':
                    token = NEW(CIToken,
                                CI_TOKEN_KIND_LPAREN,
                                clone__Location(&self->base.location));

                    break;
                default:
                    UNREACHABLE("this situation is impossible");
            }

            if (check_match) {
                end_token__CIScanner(self,
                                     self->base.source.cursor.line,
                                     self->base.source.cursor.column,
                                     self->base.source.cursor.position);
                end__Location(&token->location,
                              self->base.location.end_line,
                              self->base.location.end_column,
                              self->base.location.end_position);

                next_char__CIScanner(self);
                push_token__CIScanner(self, token);

                switch (match) {
                    case '{':
                        return get_closing__CIScanner(self, '}');
                    case '[':
                        return get_closing__CIScanner(self, ']');
                    case '(':
                        return get_closing__CIScanner(self, ')');
                    default:
                        UNREACHABLE("this situation is impossible");
                }
            }

            return token;
        }
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

        case ';':
            return NEW(CIToken,
                       CI_TOKEN_KIND_SEMICOLON,
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
                return NEW_VARIANT(CIToken,
                                   literal_constant_int,
                                   clone__Location(&self->base.location),
                                   from__String("0"));
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
            TODO("error!!");
    }

    return NULL;
}

void
run__CIScanner(CIScanner *self, bool dump_scanner)
{
    if (self->base.source.file->len > 1) {
        while (self->base.source.cursor.position <
               self->base.source.file->len - 1) {
            skip_space__CIScanner(self);

            if (self->base.source.cursor.position >=
                self->base.source.file->len - 1) {
                break;
            }

            CIToken *token = get_token__CIScanner(self, true);

            if (token) {
                next_char_by_token__CIScanner(self, token);
                end_token__CIScanner(self,
                                     self->base.source.cursor.line,
                                     self->base.source.cursor.column,
                                     self->base.source.cursor.position);
                set_all__Location(&token->location, &self->base.location);
                check_standard(self, token);

                switch (token->kind) {
                    case CI_TOKEN_KIND_COMMENT_LINE:
                    case CI_TOKEN_KIND_COMMENT_BLOCK:
                        FREE(CIToken, token);
                        break;
                    default:
                        next_char__CIScanner(self);
                        push_token__CIScanner(self, token);
                }

                if (self->base.source.cursor.position >=
                    self->base.source.file->len - 1) {
                    break;
                }
            }
        }
    }

    start_token__CIScanner(self,
                           self->base.source.cursor.line,
                           self->base.source.cursor.column,
                           self->base.source.cursor.position);
    end_token__CIScanner(self,
                         self->base.source.cursor.line,
                         self->base.source.cursor.column,
                         self->base.source.cursor.position);
    push_token__CIScanner(
      self,
      NEW(CIToken, CI_TOKEN_KIND_EOF, clone__Location(&self->base.location)));

    if (dump_scanner) {
        printf("====Scanner(%s)====\n", self->base.source.file->name);

        for (Usize i = 0; i < self->tokens->len; ++i) {
            PRINTLN("{Sr}", to_string__CIToken(get__Vec(self->tokens, i)));
        }
    }

    if (*self->base.count_error > 0) {
        exit(1);
    }
}

DESTRUCTOR(CIScanner, const CIScanner *self)
{
    FREE_BUFFER_ITEMS(self->tokens->buffer, self->tokens->len, CIToken)
    FREE(Vec, self->tokens);
}
