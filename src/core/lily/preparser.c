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

#include <base/new.h>

#include <core/lily/error.h>
#include <core/lily/lily.h>
#include <core/lily/preparser.h>
#include <core/lily/token.h>
#include <core/shared/diagnostic.h>

#include <stdio.h>

// Advance to the one position and update the current.
static void
next_token__LilyPreparser(LilyPreparser *self);

// Remove an item at the current position and update the current.
static void
eat_token__LilyPreparser(LilyPreparser *self);

// Remove an item at the current position and update the current (without free
// the token).
static void
eat_token_w_free__LilyPreparser(LilyPreparser *self);

// Peek token at position + n.
static LilyToken *
peek_token__LilyPreparser(const LilyPreparser *self, Usize n);

// Combine next_token and eat_token function.
static void
eat_and_next_token__LilyPreparser(LilyPreparser *self);

static void
eat_w_free_and_next_token__LilyPreparser(LilyPreparser *self);

static LilyPreparserImport *
preparse_import__LilyPreparser(LilyPreparser *self);

static LilyPreparserMacro *
preparser_macro__LilyPreparser(LilyPreparser *self);

static LilyPreparserPackage *
preparse_package__LilyPreparser(LilyPreparser *self);

CONSTRUCTOR(LilyPreparserImport *,
            LilyPreparserImport,
            String *value,
            String *as)
{
    LilyPreparserImport *self = lily_malloc(sizeof(LilyPreparserImport));

    self->value = value;
    self->as = as;

    return self;
}

DESTRUCTOR(LilyPreparserImport, LilyPreparserImport *self)
{
    FREE(String, self->value);

    if (self->as) {
        FREE(String, self->as);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyPreparserMacro *, LilyPreparserMacro, String *name, Vec *tokens)
{
    LilyPreparserMacro *self = lily_malloc(sizeof(LilyPreparserMacro));

    self->name = name;
    self->tokens = tokens;

    return self;
}

DESTRUCTOR(LilyPreparserMacro, LilyPreparserMacro *self)
{
    FREE(String, self->name);
    FREE_BUFFER_ITEMS(self->tokens->buffer, self->tokens->len, LilyToken);
    FREE(Vec, self->tokens);
    lily_free(self);
}

CONSTRUCTOR(LilyPreparserSubPackage *,
            LilyPreparserSubPackage,
            enum LilyVisibility visibility,
            String *name)
{
    LilyPreparserSubPackage *self =
      lily_malloc(sizeof(LilyPreparserSubPackage));

    self->name = name;
    self->visibility = visibility;

    return self;
}

DESTRUCTOR(LilyPreparserSubPackage, LilyPreparserSubPackage *self)
{
    FREE(String, self->name);
    lily_free(self);
}

CONSTRUCTOR(LilyPreparserPackage *,
            LilyPreparserPackage,
            String *name,
            Vec *sub_packages)
{
    LilyPreparserPackage *self = lily_malloc(sizeof(LilyPreparserPackage));

    self->name = name;
    self->sub_packages = sub_packages;

    return self;
}

DESTRUCTOR(LilyPreparserPackage, LilyPreparserPackage *self)
{
    FREE(String, self->name);
    FREE_BUFFER_ITEMS(self->sub_packages->buffer,
                      self->sub_packages->len,
                      LilyPreparserSubPackage);
    FREE(Vec, self->sub_packages);
    lily_free(self);
}

void
next_token__LilyPreparser(LilyPreparser *self)
{
    if (self->position + 1 < self->scanner->tokens->len) {
        ++self->position;
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

void
eat_token__LilyPreparser(LilyPreparser *self)
{
    if (self->current->kind != LILY_TOKEN_KIND_EOF) {
        FREE(LilyToken, remove__Vec(self->scanner->tokens, self->position));
    }
}

void
eat_token_w_free__LilyPreparser(LilyPreparser *self)
{
    if (self->current->kind != LILY_TOKEN_KIND_EOF) {
        remove__Vec(self->scanner->tokens, self->position);
    }
}

LilyToken *
peek_token__LilyPreparser(const LilyPreparser *self, Usize n)
{
    if (self->position + n < self->scanner->tokens->len) {
        return get__Vec(self->scanner->tokens, self->position + n);
    }

    return NULL;
}

void
eat_and_next_token__LilyPreparser(LilyPreparser *self)
{
    eat_token__LilyPreparser(self);

    if (self->position < self->scanner->tokens->len) {
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

void
eat_w_free_and_next_token__LilyPreparser(LilyPreparser *self)
{
    eat_token_w_free__LilyPreparser(self);

    if (self->position < self->scanner->tokens->len) {
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

LilyPreparserImport *
preparse_import__LilyPreparser(LilyPreparser *self)
{
    String *import_value = NULL;
    String *as_value = NULL;

    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_LITERAL_STRING:
            import_value = clone__String(self->current->literal_string);
            eat_and_next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->scanner->source.file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IMPORT_VALUE),
                          NULL,
                          NULL,
                          NULL),
              &self->count_error);
            return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_AS:
            eat_and_next_token__LilyPreparser(self);

            if (self->current->kind == LILY_TOKEN_KIND_IDENTIFIER_NORMAL) {
                as_value = clone__String(self->current->identifier_normal);
            } else {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->scanner->source.file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected identifier after `as` keyword")),
                  &self->count_error);

                FREE(String, current_s);

                return NULL;
            }

            break;
        default:
            break;
    }

    return NEW(LilyPreparserImport, import_value, as_value);
}

LilyPreparserMacro *
preparser_macro__LilyPreparser(LilyPreparser *self)
{
    String *name = NULL;
    Vec *tokens = NEW(Vec);

    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            eat_and_next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->scanner->source.file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                          NULL,
                          NULL,
                          NULL),
              &self->count_error);

            return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            eat_and_next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `=`")),
              &self->count_error);

            FREE(String, current_s);

            return NULL;
        }
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_BRACE:
            eat_and_next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `{`")),
              &self->count_error);

            FREE(String, current_s);

            return NULL;
        }
    }

get_tokens : {
    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(tokens, self->current);
        eat_w_free_and_next_token__LilyPreparser(self);
    }

    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            eat_and_next_token__LilyPreparser(self);
            break;
        case LILY_TOKEN_KIND_EOF: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;` after `}` in macro declaration")),
              &self->count_error);

            FREE(String, current_s);

            return NULL;
        }
        default:
            goto get_tokens;
    }
}

    return NEW(LilyPreparserMacro, name, tokens);
}

LilyPreparserPackage *
preparse_package__LilyPreparser(LilyPreparser *self)
{
    TODO("preparse package");
}

void
run__LilyPreparser(LilyPreparser *self)
{
    while (self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_IMPORT: {
                LilyPreparserImport *import =
                  preparse_import__LilyPreparser(self);

                if (import) {
                    push__Vec(self->imports, import);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_MACRO: {
                LilyPreparserMacro *macro =
                  preparser_macro__LilyPreparser(self);

                if (macro) {
                    push__Vec(self->public_macros, macro);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_PACKAGE:
                break;
            case LILY_TOKEN_KIND_KEYWORD_PUB:
                break;
            case LILY_TOKEN_KIND_KEYWORD_MODULE:
                break;
            case LILY_TOKEN_KIND_KEYWORD_TEST:
                break;
            case LILY_TOKEN_KIND_KEYWORD_FUN:
                break;
            case LILY_TOKEN_KIND_KEYWORD_object:
                break;
            case LILY_TOKEN_KIND_KEYWORD_TYPE:
                break;
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    if (peeked->kind == LILY_TOKEN_KIND_BANG) {
                    } else {
                        goto unexpected_token;
                    }
                } else {
                    goto unexpected_token;
                }

                break;
            }
            case LILY_TOKEN_KIND_COMMENT_DOC:
                break;
            case LILY_TOKEN_KIND_AT:
                break;
            default: {
            unexpected_token : {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->scanner->source.file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    NULL),
                  &self->count_error);

                FREE(String, current_s);
            }

            break;
            }
        }

        next_token__LilyPreparser(self);
    }

#ifdef DEBUG_PREPARSER
    puts("\n====Preparser====\n");
    for (Usize i = 0; i < self->scanner->tokens->len; i++) {
        CALL_DEBUG(LilyToken, get__Vec(self->scanner->tokens, i));
    }
#endif

    if (self->count_error > 0) {
        exit(1);
    }
}