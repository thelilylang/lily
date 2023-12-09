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

#include <stdio.h>
#include <stdlib.h>

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

/// @brief Skip and verify if the next character is the target.
static bool
skip_and_verify__CIScanner(CIScanner *self, char target);

/// @brief Scan to the next closing character.
static CIToken *
get_closing__CIScanner(CIScanner *self, char target);

/// @brief Get token from characters.
static CIToken *
get_token__CIScanner(CIScanner *self, bool check_match);

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
    if (!strcmp(id, "break"))
        return CI_TOKEN_KIND_KEYWORD_BREAK;
    if (!strcmp(id, "case"))
        return CI_TOKEN_KIND_KEYWORD_CASE;
    if (!strcmp(id, "const"))
        return CI_TOKEN_KIND_KEYWORD_CONST;
    if (!strcmp(id, "continue"))
        return CI_TOKEN_KIND_KEYWORD_CONTINUE;
    if (!strcmp(id, "default"))
        return CI_TOKEN_KIND_KEYWORD_DEFAULT;
    if (!strcmp(id, "do"))
        return CI_TOKEN_KIND_KEYWORD_DO;
    if (!strcmp(id, "double"))
        return CI_TOKEN_KIND_KEYWORD_DOUBLE;
    if (!strcmp(id, "else"))
        return CI_TOKEN_KIND_KEYWORD_ELSE;
    if (!strcmp(id, "enum"))
        return CI_TOKEN_KIND_KEYWORD_ENUM;
    if (!strcmp(id, "extern"))
        return CI_TOKEN_KIND_KEYWORD_EXTERN;
    if (!strcmp(id, "float"))
        return CI_TOKEN_KIND_KEYWORD_FLOAT;
    if (!strcmp(id, "for"))
        return CI_TOKEN_KIND_KEYWORD_FOR;
    if (!strcmp(id, "goto"))
        return CI_TOKEN_KIND_KEYWORD_GOTO;
    if (!strcmp(id, "if"))
        return CI_TOKEN_KIND_KEYWORD_IF;
    if (!strcmp(id, "int"))
        return CI_TOKEN_KIND_KEYWORD_INT;
    if (!strcmp(id, "long"))
        return CI_TOKEN_KIND_KEYWORD_LONG;
    if (!strcmp(id, "register"))
        return CI_TOKEN_KIND_KEYWORD_REGISTER;
    if (!strcmp(id, "return"))
        return CI_TOKEN_KIND_KEYWORD_RETURN;
    if (!strcmp(id, "short"))
        return CI_TOKEN_KIND_KEYWORD_SHORT;
    if (!strcmp(id, "signed"))
        return CI_TOKEN_KIND_KEYWORD_SIGNED;
    if (!strcmp(id, "sizeof"))
        return CI_TOKEN_KIND_KEYWORD_SIZEOF;
    if (!strcmp(id, "static"))
        return CI_TOKEN_KIND_KEYWORD_STATIC;
    if (!strcmp(id, "struct"))
        return CI_TOKEN_KIND_KEYWORD_STRUCT;
    if (!strcmp(id, "switch"))
        return CI_TOKEN_KIND_KEYWORD_SWITCH;
    if (!strcmp(id, "thread_local"))
        return CI_TOKEN_KIND_KEYWORD_THREAD_LOCAL;
    if (!strcmp(id, "typedef"))
        return CI_TOKEN_KIND_KEYWORD_TYPEDEF;
    if (!strcmp(id, "union"))
        return CI_TOKEN_KIND_KEYWORD_UNION;
    if (!strcmp(id, "unsigned"))
        return CI_TOKEN_KIND_KEYWORD_UNSIGNED;
    if (!strcmp(id, "void"))
        return CI_TOKEN_KIND_KEYWORD_VOID;
    if (!strcmp(id, "volatile"))
        return CI_TOKEN_KIND_KEYWORD_VOLATILE;
    if (!strcmp(id, "while"))
        return CI_TOKEN_KIND_KEYWORD_WHILE;

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
        case IS_ID: {
            String *id = scan_identifier__CIScanner(self);
            enum CITokenKind kind = get_keyword__CIScanner(id->buffer);

            switch (kind) {
                case CI_TOKEN_KIND_IDENTIFIER:
                    return NEW_VARIANT(CIToken,
                                       identifier,
                                       clone__Location(&self->base.location),
                                       id);
                default:
                    return NEW(
                      CIToken, kind, clone__Location(&self->base.location));
            }
        }
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

        case IS_ZERO:
            break;

            // number
        case IS_DIGIT_WITHOUT_ZERO:
            break;

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
