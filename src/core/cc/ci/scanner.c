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

#include <core/cc/ci/features.h>
#include <core/cc/ci/scanner.h>
#include <core/shared/diagnostic.h>

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

/// @brief Next char n times.
/// @see `include/core/shared/scanner.h`
static inline void
jump__CIScanner(CIScanner *self, Usize n);

/// @brief Assign to line and column to start_line and start_column Location's
/// field.
/// @see `include/core/shared/scanner.h`
static inline void
start_token__CIScanner(CIScanner *self,
                       Usize line,
                       Usize column,
                       Usize position);

/// @brief Assign to line and column to end_line and end_column Location's
/// field.
/// @see `include/core/shared/scanner.h`
static inline void
end_token__CIScanner(CIScanner *self, Usize line, Usize column, Usize position);

/// @brief Get character at position + n.
/// @see `include/core/shared/scanner.h`
static inline char *
peek_char__CIScanner(const CIScanner *self, Usize n);

/// @brief Check if current is a digit.
static inline bool
is_digit__CIScanner(const CIScanner *self);

/// @brief Check if current can be an identifier.
static inline bool
is_ident__CIScanner(const CIScanner *self);

/// @brief Check if current can be an hexadecimal.
static inline bool
is_hex__CIScanner(const CIScanner *self);

/// @brief Check if current can be an octal.
static inline bool
is_oct__CIScanner(const CIScanner *self);

/// @brief Check if current can be an octal.
static inline bool
is_bin__CIScanner(const CIScanner *self);

/// @brief Push token to tokens.
static inline void
push_token__CIScanner(CIScanner *self, CIToken *token);

/// @brief Next char according the token.
static void
next_char_by_token__CIScanner(CIScanner *self, const CIToken *token);

/// @brief Get keyword from id.
static enum CITokenKind
get_keyword__CIScanner(const char *id);

/// @brief Skip comment line.
static void
skip_comment_line__CIScanner(CIScanner *self);

/// @brief Skip comment block.
static void
skip_comment_block__CIScanner(CIScanner *self);

/// @brief Scan identifier.
static String *
scan_identifier__CIScanner(CIScanner *self);

/// @brief Scan token.
static CIToken *
get_token__CIScanner(CIScanner *self);

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
jump__CIScanner(CIScanner *self, Usize n)
{
    return jump__Scanner(&self->base, n);
}

void
start_token__CIScanner(CIScanner *self,
                       Usize line,
                       Usize column,
                       Usize position)
{
    return start_token__Scanner(&self->base, line, column, position);
}

void
end_token__CIScanner(CIScanner *self, Usize line, Usize column, Usize position)
{
    return end_token__Scanner(&self->base, line, column, position);
}

char *
peek_char__CIScanner(const CIScanner *self, Usize n)
{
    return peek_char__Scanner(&self->base, n);
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
get_keyword__CIScanner(const char *id)
{
    if (!strcmp(id, "auto"))
        return CI_TOKEN_KIND_KEYWORD_AUTO;
    else if (!strcmp(id, "break"))
        return CI_TOKEN_KIND_KEYWORD_BREAK;
    else if (!strcmp(id, "case"))
        return CI_TOKEN_KIND_KEYWORD_CASE;
    else if (!strcmp(id, "const"))
        return CI_TOKEN_KIND_KEYWORD_CONST;
    else if (!strcmp(id, "continue"))
        return CI_TOKEN_KIND_KEYWORD_CONTINUE;
    else if (!strcmp(id, "default"))
        return CI_TOKEN_KIND_KEYWORD_DEFAULT;
    else if (!strcmp(id, "do"))
        return CI_TOKEN_KIND_KEYWORD_DO;
    else if (!strcmp(id, "double"))
        return CI_TOKEN_KIND_KEYWORD_DOUBLE;
    else if (!strcmp(id, "else"))
        return CI_TOKEN_KIND_KEYWORD_ELSE;
    else if (!strcmp(id, "enum"))
        return CI_TOKEN_KIND_KEYWORD_ENUM;
    else if (!strcmp(id, "extern"))
        return CI_TOKEN_KIND_KEYWORD_EXTERN;
    else if (!strcmp(id, "float"))
        return CI_TOKEN_KIND_KEYWORD_FLOAT;
    else if (!strcmp(id, "for"))
        return CI_TOKEN_KIND_KEYWORD_FOR;
    else if (!strcmp(id, "goto"))
        return CI_TOKEN_KIND_KEYWORD_GOTO;
    else if (!strcmp(id, "if"))
        return CI_TOKEN_KIND_KEYWORD_IF;
    else if (!strcmp(id, "int"))
        return CI_TOKEN_KIND_KEYWORD_INT;
    else if (!strcmp(id, "long"))
        return CI_TOKEN_KIND_KEYWORD_LONG;
    else if (!strcmp(id, "register"))
        return CI_TOKEN_KIND_KEYWORD_REGISTER;
    else if (!strcmp(id, "return"))
        return CI_TOKEN_KIND_KEYWORD_RETURN;
    else if (!strcmp(id, "short"))
        return CI_TOKEN_KIND_KEYWORD_SHORT;
    else if (!strcmp(id, "signed"))
        return CI_TOKEN_KIND_KEYWORD_SIGNED;
    else if (!strcmp(id, "sizeof"))
        return CI_TOKEN_KIND_KEYWORD_SIZEOF;
    else if (!strcmp(id, "static"))
        return CI_TOKEN_KIND_KEYWORD_STATIC;
    else if (!strcmp(id, "struct"))
        return CI_TOKEN_KIND_KEYWORD_STRUCT;
    else if (!strcmp(id, "switch"))
        return CI_TOKEN_KIND_KEYWORD_SWITCH;
    else if (!strcmp(id, "thread_local"))
        return CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL;
    else if (!strcmp(id, "typedef"))
        return CI_TOKEN_KIND_KEYWORD_TYPEDEF;
    else if (!strcmp(id, "union"))
        return CI_TOKEN_KIND_KEYWORD_UNION;
    else if (!strcmp(id, "unsigned"))
        return CI_TOKEN_KIND_KEYWORD_UNSIGNED;
    else if (!strcmp(id, "void"))
        return CI_TOKEN_KIND_KEYWORD_VOID;
    else if (!strcmp(id, "volatile"))
        return CI_TOKEN_KIND_KEYWORD_VOLATILE;
    else if (!strcmp(id, "while"))
        return CI_TOKEN_KIND_KEYWORD_WHILE;
    else
        return CI_TOKEN_KIND_IDENTIFIER;
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
                    self->base.source.cursor.column,
                    self->base.source.cursor.position);

    // Check if the comment block is closed. While the current character is not
    // a `*` or the next character is not a `/`, we continue to scan the comment
    // block.
    while (self->base.source.cursor.current != '*' ||
           peek_char__CIScanner(self, 1) != (char *)'/') {
        // Check if the comment block is not closed.
        if (self->base.source.cursor.position >=
            self->base.source.file->len - 2) {
            end__Location(&location_error,
                          self->base.source.cursor.line,
                          self->base.source.cursor.column,
                          self->base.source.cursor.position);

            {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_ci_error,
                    self->base.source.file,
                    &location_error,
                    NEW(CIError, CI_ERROR_KIND_UNCLOSED_COMMENT_BLOCK),
                    init__Vec(
                      1, from__String("close comment multi line with `*/`")),
                    NULL,
                    NULL),
                  self->base.count_error);
            }
        }

        next_char__CIScanner(self);
    }

    jump__CIScanner(self, 2);
}

String *
scan_identifier__CIScanner(CIScanner *self)
{
    return NULL;
}

CIToken *
get_token__CIScanner(CIScanner *self)
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
        case '!':
            if (c1 == (char *)'=') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_BANG_EQ,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_BANG,
                       clone__Location(&self->base.location));
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
        case ':':
            return NEW(CIToken,
                       CI_TOKEN_KIND_COLON,
                       clone__Location(&self->base.location));
        case ',':
            return NEW(CIToken,
                       CI_TOKEN_KIND_COMMA,
                       clone__Location(&self->base.location));
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
                CIToken *token = NEW(CIToken,
                                     CI_TOKEN_KIND_COMMENT_BLOCK,
                                     clone__Location(&self->base.location));

                jump__CIScanner(self, 3);

                return token;
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
        case '-':
            if (c1 == (char *)'>') {
                return NEW(CIToken,
                           CI_TOKEN_KIND_ARROW,
                           clone__Location(&self->base.location));
            }

            return NEW(CIToken,
                       CI_TOKEN_KIND_MINUS,
                       clone__Location(&self->base.location));
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

            CIToken *token = get_token__CIScanner(self);

            if (token) {
                next_char_by_token__CIScanner(self, token);
                end_token__CIScanner(self,
                                     self->base.source.cursor.line,
                                     self->base.source.cursor.column,
                                     self->base.source.cursor.position);
                set_all__Location(&token->location, &self->base.location);

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

    if (*self->base.count_error > 0) {
        exit(1);
    }
}

DESTRUCTOR(CIScanner, const CIScanner *self)
{
    FREE_BUFFER_ITEMS(self->tokens->buffer, self->tokens->len, CIToken)
    FREE(Vec, self->tokens);
}
