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

#include <base/atoi.h>

#include <core/lily/ast/decl.h>
#include <core/lily/lily.h>
#include <core/lily/package/package.h>
#include <core/lily/parser.h>

#include <stdio.h>

// Advance to one declaration.
static void
next_decl__LilyParser(LilyParser *self);

// Advance to one token.
static void
next_token__LilyParseBlock(LilyParseBlock *self);

// Advance to n token.
static void
jump__LilyParseBlock(LilyParseBlock *self, Usize n);

// Peek token at position + n.
static LilyToken *
peek_token__LilyParseBlock(LilyParseBlock *self, Usize n);

// Check if it need to parse a data type.
static inline bool
is_data_type__LilyParseBlock(LilyParseBlock *self);

// Parse data type
static LilyAstDataType *
parse_data_type__LilyParseBlock(LilyParseBlock *self);

// Parse dot access
// a.b.c
static LilyAstExpr *
parse_path_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *begin);

// Parse access expression
static LilyAstExpr *
parse_access_expr__LilyParseBlock(LilyParseBlock *self);

// Parse array expression
static LilyAstExpr *
parse_array_expr__LilyParseBlock(LilyParseBlock *self);

// Parse binary expression
static LilyAstExpr *
parse_binary_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *expr);

// Parse fun/method call expression
static LilyAstExpr *
parse_fun_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse record call expression
static LilyAstExpr *
parse_record_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse variant call expression
static LilyAstExpr *
parse_variant_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse call expression
static LilyAstExpr *
parse_call_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id);

// Parse lambda expression
static LilyAstExpr *
parse_lambda_expr__LilyParseBlock(LilyParseBlock *self);

static LilyAstExpr *
parse_primary_expr__LilyParseBlock(LilyParseBlock *self);

// Parse expression
static LilyAstExpr *
parse_expr__LilyParseBlock(LilyParseBlock *self);

#define SKIP_TO_TOKEN(k)                                 \
    while (self->current->kind != k &&                   \
           self->current->kind != LILY_TOKEN_KIND_EOF) { \
        next_token__LilyParseBlock(self);                \
    }                                                    \
    next_token__LilyParseBlock(self);

#define CHECK_COMMA(closing)                                                  \
    if (self->current->kind != closing) {                                     \
        switch (self->current->kind) {                                        \
            case LILY_TOKEN_KIND_COMMA:                                       \
                next_token__LilyParseBlock(self);                             \
                break;                                                        \
            default:                                                          \
                emit__Diagnostic(                                             \
                  NEW_VARIANT(Diagnostic,                                     \
                              simple_lily_error,                              \
                              self->file,                                     \
                              &self->current->location,                       \
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN), \
                              NULL,                                           \
                              NULL,                                           \
                              from__String("expected `,`")),                  \
                  self->count_error);                                         \
                break;                                                        \
        }                                                                     \
    }

CONSTRUCTOR(LilyParseBlock, LilyParseBlock, LilyParser *parser, Vec *tokens)
{
    Location location_eof =
      clone__Location(&CAST(LilyToken *, last__Vec(tokens))->location);
    start__Location(
      &location_eof, location_eof.end_line, location_eof.end_column);

    push__Vec(tokens,
              NEW(LilyToken, LILY_TOKEN_KIND_EOF, location_eof)); // Add EOF.

    return (LilyParseBlock){ .tokens = tokens,
                             .current = get__Vec(tokens, 0),
                             .file = &parser->package->file,
                             .count_error = &parser->package->count_error,
                             .count_warning = &parser->package->count_warning,
                             .position = 0 };
}

void
next_decl__LilyParser(LilyParser *self)
{
    if (self->position + 1 < self->package->preparser_info.decls->len) {
        ++self->position;
        self->current =
          get__Vec(self->package->preparser_info.decls, self->position);
    }
}

void
next_token__LilyParseBlock(LilyParseBlock *self)
{
    if (self->position + 1 < self->tokens->len) {
        ++self->position;
        self->current = get__Vec(self->tokens, self->position);
    }
}

void
jump__LilyParseBlock(LilyParseBlock *self, Usize n)
{
    if (self->position + n < self->tokens->len) {
        self->position += n;
        self->current = get__Vec(self->tokens, self->position);
    }
}

LilyToken *
peek_token__LilyParseBlock(LilyParseBlock *self, Usize n)
{
    if (self->position + n < self->tokens->len) {
        return get__Vec(self->tokens, self->position + n);
    }

    return NULL;
}

bool
is_data_type__LilyParseBlock(LilyParseBlock *self)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
        case LILY_TOKEN_KIND_BANG:
        case LILY_TOKEN_KIND_KEYWORD_FUN:
        case LILY_TOKEN_KIND_KEYWORD_MUT:
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_REF:
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
        case LILY_TOKEN_KIND_L_PAREN:
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            return true;
        default:
            return false;
    }
}

LilyAstDataType *
parse_data_type__LilyParseBlock(LilyParseBlock *self)
{
    // Any
    // [_]<dt>
    // [*]<dt>
    // [<size>]<dt>
    // [?]<dt>
    // Bool
    // Byte
    // Bytes
    // Char
    // !<dt>
    // Float32
    // Float64
    // Int16
    // Int32
    // Int64
    // Int8
    // Isize
    // fun(<dt>, ...) <return_dt>
    // mut <dt>
    // Never
    // Object
    // ?<dt>
    // *<dt>
    // ref <dt>
    // Self
    // Str
    // trace <dt>
    // (<dt>, ...)
    // Uint16
    // Uint32
    // Uint64
    // Uint8
    // Unit
    // Usize
    // <name>.<name>...[<dt>]
    Location location = clone__Location(&self->current->location);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);

            if (!strcmp(self->current->identifier_normal->buffer, "Any")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_ANY, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Bool")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BOOL, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Byte")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BYTE, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Bytes")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BYTES, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Char")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CHAR, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Float32")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_FLOAT32, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Float64")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_FLOAT64, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Int16")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT16, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Int32")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT32, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Int64")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT64, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Int8")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT8, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Isize")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_ISIZE, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Never")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_NEVER, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Str")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_STR, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Uint16")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT16, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Uint32")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT32, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Uint64")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT64, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Uint8")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT8, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Unit")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UNIT, location);
            } else if (!strcmp(self->current->identifier_normal->buffer,
                               "Usize")) {
                next_token__LilyParseBlock(self);

                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_USIZE, location);
            } else {
                // 1. Parse name
                String *name = clone__String(self->current->identifier_normal);

                next_token__LilyParseBlock(self);

                while (self->current->kind == LILY_TOKEN_KIND_DOT) {
                    next_token__LilyParseBlock(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                            push__String(name, '.');
                            append__String(name,
                                           self->current->identifier_normal);
                            next_token__LilyParseBlock(self);

                            break;
                        default: {
                            String *token_s =
                              to_string__LilyToken(self->current);

                            emit__Diagnostic(
                              NEW_VARIANT(Diagnostic,
                                          simple_lily_error,
                                          self->file,
                                          &self->current->location,
                                          NEW_VARIANT(LilyError,
                                                      unexpected_token,
                                                      token_s->buffer),
                                          NULL,
                                          NULL,
                                          from__String("expected identifier")),
                              self->count_error);

                            FREE(String, token_s);
                            FREE(String, name);

                            return NULL;
                        }
                    }
                }

                // 2. Parse generics
                Vec *generics = NULL; // Vec<LilyAstDataType*>*?

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_L_HOOK:
                        generics = NEW(Vec);
                        next_token__LilyParseBlock(self);

                        while (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                            LilyAstDataType *dt =
                              parse_data_type__LilyParseBlock(self);

                            if (!dt) {
                                FREE_BUFFER_ITEMS(generics->buffer,
                                                  generics->len,
                                                  LilyAstDataType);
                                FREE(Vec, generics);

                                return NULL;
                            }

                            push__Vec(generics, dt);

                            if (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                                switch (self->current->kind) {
                                    case LILY_TOKEN_KIND_COMMA:
                                        next_token__LilyParseBlock(self);
                                        break;
                                    default:
                                        emit__Diagnostic(
                                          NEW_VARIANT(
                                            Diagnostic,
                                            simple_lily_error,
                                            self->file,
                                            &self->current->location,
                                            NEW(LilyError,
                                                LILY_ERROR_KIND_EXPECTED_TOKEN),
                                            NULL,
                                            NULL,
                                            from__String("expected `,`")),
                                          self->count_error);

                                        break;
                                }
                            }
                        }

                        end__Location(&location,
                                      self->current->location.end_line,
                                      self->current->location.end_column);
                        next_token__LilyParseBlock(self);

                        break;
                    default:
                        end__Location(
                          &location,
                          CAST(LilyToken *,
                               get__Vec(self->tokens, self->position - 1))
                            ->location.end_line,
                          CAST(LilyToken *,
                               get__Vec(self->tokens, self->position - 1))
                            ->location.end_column);

                        break;
                }

                return NEW_VARIANT(LilyAstDataType,
                                   custom,
                                   location,
                                   NEW(LilyAstDataTypeCustom, name, generics));
            }

        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_SELF: {
            enum LilyTokenKind kind = self->current->kind;

            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);

            next_token__LilyParseBlock(self);

            switch (kind) {
                case LILY_TOKEN_KIND_KEYWORD_OBJECT:
                    return NEW(LilyAstDataType,
                               LILY_AST_DATA_TYPE_KIND_OBJECT,
                               location);
                case LILY_TOKEN_KIND_KEYWORD_SELF:
                    return NEW(
                      LilyAstDataType, LILY_AST_DATA_TYPE_KIND_SELF, location);
                default:
                    UNREACHABLE("this way is impossible");
            }
        }

        case LILY_TOKEN_KIND_L_HOOK:
            next_token__LilyParseBlock(self);

#define EXPECTED_R_HOOK()                                                 \
    switch (self->current->kind) {                                        \
        case LILY_TOKEN_KIND_R_HOOK:                                      \
            next_token__LilyParseBlock(self);                             \
            break;                                                        \
        default:                                                          \
            emit__Diagnostic(                                             \
              NEW_VARIANT(Diagnostic,                                     \
                          simple_lily_error,                              \
                          self->file,                                     \
                          &self->current->location,                       \
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN), \
                          NULL,                                           \
                          NULL,                                           \
                          from__String("expected `]`")),                  \
              self->count_error);                                         \
            return NULL;                                                  \
    }

#define PARSE_ARRAY_DATA_TYPE()                                           \
    LilyAstDataType *dt = NULL;                                           \
    if (is_data_type__LilyParseBlock(self)) {                             \
        dt = parse_data_type__LilyParseBlock(self);                       \
                                                                          \
        if (!dt) {                                                        \
            return NULL;                                                  \
        }                                                                 \
                                                                          \
        end__Location(                                                    \
          &location, dt->location.end_line, dt->location.end_column);     \
    } else {                                                              \
        LilyToken *previous = get__Vec(self->tokens, self->position - 1); \
                                                                          \
        end__Location(&location,                                          \
                      previous->location.end_line,                        \
                      previous->location.end_column);                     \
    }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    if (!strcmp(self->current->identifier_normal->buffer,
                                "_")) {
                        next_token__LilyParseBlock(self);

                        EXPECTED_R_HOOK();
                        PARSE_ARRAY_DATA_TYPE();

                        return NEW_VARIANT(
                          LilyAstDataType,
                          array,
                          location,
                          NEW(LilyAstDataTypeArray,
                              LILY_AST_DATA_TYPE_ARRAY_KIND_DYNAMIC,
                              dt));
                    } else {
                        String *token_s = to_string__LilyToken(self->current);

                        emit__Diagnostic(
                          NEW_VARIANT(Diagnostic,
                                      simple_lily_error,
                                      self->file,
                                      &self->current->location,
                                      NEW_VARIANT(LilyError,
                                                  unexpected_token,
                                                  token_s->buffer),
                                      NULL,
                                      NULL,
                                      from__String("expected `_`")),
                          self->count_error);

                        FREE(String, token_s);

                        return NULL;
                    }
                case LILY_TOKEN_KIND_STAR: {
                    next_token__LilyParseBlock(self);

                    EXPECTED_R_HOOK();
                    PARSE_ARRAY_DATA_TYPE();

                    return NEW_VARIANT(
                      LilyAstDataType,
                      array,
                      location,
                      NEW(LilyAstDataTypeArray,
                          LILY_AST_DATA_TYPE_ARRAY_KIND_MULTI_POINTERS,
                          dt));
                }

                case LILY_TOKEN_KIND_LITERAL_INT_10: {
                    Optional *size_op = atoi_safe__Usize(
                      self->current->literal_int_10->buffer, 10);

                    if (is_none__Optional(size_op)) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_USIZE_OUT_OF_RANGE),
                            NULL,
                            NULL,
                            NULL),
                          self->count_error);

                        FREE(Optional, size_op);

                        return NULL;
                    }

                    Usize size = (Usize)(Uptr)get__Optional(size_op);

                    next_token__LilyParseBlock(self);

                    FREE(Optional, size_op);
                    EXPECTED_R_HOOK();
                    PARSE_ARRAY_DATA_TYPE();

                    return NEW_VARIANT(
                      LilyAstDataType,
                      array,
                      location,
                      NEW_VARIANT(LilyAstDataTypeArray, sized, dt, size));
                }

                case LILY_TOKEN_KIND_INTERROGATION: {
                    next_token__LilyParseBlock(self);

                    EXPECTED_R_HOOK();
                    PARSE_ARRAY_DATA_TYPE();

                    return NEW_VARIANT(
                      LilyAstDataType,
                      array,
                      location,
                      NEW(LilyAstDataTypeArray,
                          LILY_AST_DATA_TYPE_ARRAY_KIND_UNDETERMINED,
                          dt));
                }

                default: {
                    String *token_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, token_s->buffer),
                        NULL,
                        NULL,
                        from__String(
                          "expected `_`, `*`, `?` or literal integer")),
                      self->count_error);

                    FREE(String, token_s);

                    return NULL;
                }
            }

            break;

        case LILY_TOKEN_KIND_BANG:
        case LILY_TOKEN_KIND_INTERROGATION:
        case LILY_TOKEN_KIND_STAR:
        case LILY_TOKEN_KIND_KEYWORD_REF:
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
        case LILY_TOKEN_KIND_KEYWORD_MUT: {
            enum LilyTokenKind kind = self->current->kind;

            next_token__LilyParseBlock(self);

            LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

            if (!dt) {
                return NULL;
            }

            end__Location(
              &location, dt->location.end_line, dt->location.end_column);

            switch (kind) {
                case LILY_TOKEN_KIND_BANG:
                    return NEW_VARIANT(
                      LilyAstDataType, exception, location, dt);
                case LILY_TOKEN_KIND_INTERROGATION:
                    return NEW_VARIANT(LilyAstDataType, optional, location, dt);
                case LILY_TOKEN_KIND_STAR:
                    return NEW_VARIANT(LilyAstDataType, ptr, location, dt);
                case LILY_TOKEN_KIND_KEYWORD_REF:
                    return NEW_VARIANT(LilyAstDataType, ref, location, dt);
                case LILY_TOKEN_KIND_KEYWORD_TRACE:
                    return NEW_VARIANT(LilyAstDataType, trace, location, dt);
                case LILY_TOKEN_KIND_KEYWORD_MUT:
                    return NEW_VARIANT(LilyAstDataType, mut, location, dt);
                default:
                    UNREACHABLE("this way is impossible");
            }
        }

        case LILY_TOKEN_KIND_KEYWORD_FUN: {
            next_token__LilyParseBlock(self);

            Vec *params = NULL;                       // Vec<LilyAstDataType*>*?
            LilyAstDataType *return_data_type = NULL; // LilyAstDataType*?

            // 1. Parse params
            if (self->current->kind == LILY_TOKEN_KIND_L_PAREN) {
                next_token__LilyParseBlock(self);

                params = NEW(Vec);

                while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                    LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

                    if (!dt) {
                        // TODO: skip to next right paren.

                        FREE_BUFFER_ITEMS(
                          params->buffer, params->len, LilyAstDataType);
                        FREE(Vec, params);

                        return NULL;
                    }

                    push__Vec(params, dt);

                    if (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_COMMA:
                                next_token__LilyParseBlock(self);
                                break;
                            default:
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_EXPECTED_TOKEN),
                                    NULL,
                                    NULL,
                                    from__String("expected `,`")),
                                  self->count_error);

                                break;
                        }
                    }
                }

                next_token__LilyParseBlock(self);
            }

            // 2. Parse return data type
            if (is_data_type__LilyParseBlock(self)) {
                return_data_type = parse_data_type__LilyParseBlock(self);

                if (!return_data_type) {
                    if (params) {
                        FREE_BUFFER_ITEMS(
                          params->buffer, params->len, LilyAstDataType);
                        FREE(Vec, params);
                    }

                    return NULL;
                }

                end__Location(&location,
                              return_data_type->location.end_line,
                              return_data_type->location.end_column);
            } else {
                if (params) {
                    end__Location(
                      &location,
                      CAST(LilyToken *, last__Vec(params))->location.end_line,
                      CAST(LilyToken *, last__Vec(params))
                        ->location.end_column);
                } else {
                    end__Location(
                      &location,
                      CAST(LilyToken *,
                           get__Vec(self->tokens, self->position - 1))
                        ->location.end_line,
                      CAST(LilyToken *,
                           get__Vec(self->tokens, self->position - 1))
                        ->location.end_column);
                }
            }

            return NEW_VARIANT(
              LilyAstDataType,
              lambda,
              location,
              NEW(LilyAstDataTypeLambda, params, return_data_type));
        }

        case LILY_TOKEN_KIND_L_PAREN: {
            Vec *tuple = NEW(Vec); // Vec<LilyAstDataType*>*

            next_token__LilyParseBlock(self);

            while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

                if (!dt) {
                    FREE_BUFFER_ITEMS(
                      tuple->buffer, tuple->len, LilyAstDataType);
                    FREE(Vec, tuple);

                    return NULL;
                }

                push__Vec(tuple, dt);

                if (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_COMMA:
                            next_token__LilyParseBlock(self);
                            break;
                        default:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                                NULL,
                                NULL,
                                from__String("expected `,`")),
                              self->count_error);

                            break;
                    }
                }
            }

            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyParseBlock(self);

            return NEW_VARIANT(LilyAstDataType, tuple, location, tuple);
        }

        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String(
                  "expected `Any`, `[`, `Bool`, `Byte`, `Bytes`, `Char`, "
                  "`Float32`, `Float64`, `Int16`, `Int32`, `Int64`, `Int8`, "
                  "`Isize`, `fun`, `mut`, `Never`, `Object`, `?`, `*`, `ref`, "
                  "`Self`, `Str`, `trace`, `(`, `Uint16`, `Uint32`, `Uint64`, "
                  "`Uint8`, `Unit`, `Usize` or other identifier")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }

    return NULL;
}

LilyAstExpr *
parse_path_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *begin)
{
    Location location = clone__Location(&begin->location);
    Vec *access = NULL; // Vec<LilyAstExpr*>*

    if (begin) {
        access = init__Vec(1, begin);
    } else {
        access = NEW(Vec);
    }

    while (self->current->kind == LILY_TOKEN_KIND_DOT) {
        next_token__LilyParseBlock(self);

        LilyAstExpr *expr = parse_primary_expr__LilyParseBlock(self);

        if (!expr) {
            FREE_BUFFER_ITEMS(access->buffer, access->len, LilyAstExpr);
            FREE(Vec, access);

            return NULL;
        }

        push__Vec(access, expr);
    }

    return NEW_VARIANT(LilyAstExpr,
                       access,
                       location,
                       NEW_VARIANT(LilyAstExprAccess, path, access));
}

LilyAstExpr *
parse_access_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->current->location);
    enum LilyTokenKind kind = self->current->kind;

    next_token__LilyParseBlock(self);

    LilyAstExpr *path = parse_path_access__LilyParseBlock(self, NULL);

    if (path) {
        end__Location(
          &location, path->location.end_line, path->location.end_column);
    } else {
        return NULL;
    }

    switch (kind) {
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
            return NEW_VARIANT(LilyAstExpr,
                               access,
                               location,
                               NEW_VARIANT(LilyAstExprAccess, global, path));
        case LILY_TOKEN_KIND_AT:
            return NEW_VARIANT(
              LilyAstExpr,
              access,
              location,
              NEW_VARIANT(LilyAstExprAccess, property_init, path));
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            return NEW_VARIANT(LilyAstExpr,
                               access,
                               location,
                               NEW_VARIANT(LilyAstExprAccess, self, path));
        default:
            UNREACHABLE("this way is impossible");
    }

    return NULL;
}

LilyAstExpr *
parse_array_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->current->location);

    next_token__LilyParseBlock(self); // skip `[`

    Vec *exprs = NEW(Vec); // Vec<LilyAstExpr*>*

    while (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
        LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

        if (expr) {
            push__Vec(exprs, expr);
        } else {
            SKIP_TO_TOKEN(LILY_TOKEN_KIND_R_HOOK);

            return NULL;
        }

        if (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
            switch (self->current->kind) {
                case LILY_TOKEN_KIND_COMMA:
                    next_token__LilyParseBlock(self);
                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `,`")),
                      self->count_error);
            }
        }
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyParseBlock(self); // skip `]`

    return NEW_VARIANT(
      LilyAstExpr, array, location, NEW(LilyAstExprArray, exprs));
}

LilyAstExpr *
parse_binary_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *expr)
{
    Vec *stack = NEW(Vec); // Vec<LilyAstExpr* | enum LilyAstExprBinaryKind*>*
    Usize last_precedence = MAX_LILY_AST_EXPR_BINARY_PRECEDENCE + 1;

    push__Vec(stack, expr);

    while (self->current->kind == LILY_TOKEN_KIND_PLUS ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_AND ||
           self->current->kind == LILY_TOKEN_KIND_PLUS_EQ ||
           self->current->kind == LILY_TOKEN_KIND_AMPERSAND_EQ ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_BAR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_SLASH_EQ ||
           self->current->kind == LILY_TOKEN_KIND_STAR_STAR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_PERCENTAGE_EQ ||
           self->current->kind == LILY_TOKEN_KIND_STAR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_MINUS_EQ ||
           self->current->kind == LILY_TOKEN_KIND_XOR_EQ ||
           self->current->kind == LILY_TOKEN_KIND_EQ ||
           self->current->kind == LILY_TOKEN_KIND_AMPERSAND ||
           self->current->kind == LILY_TOKEN_KIND_BAR ||
           self->current->kind == LILY_TOKEN_KIND_BAR_R_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_SLASH ||
           self->current->kind == LILY_TOKEN_KIND_EQ_EQ ||
           self->current->kind == LILY_TOKEN_KIND_STAR_STAR ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT_L_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_L_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_PERCENTAGE ||
           self->current->kind == LILY_TOKEN_KIND_STAR ||
           self->current->kind == LILY_TOKEN_KIND_NOT_EQ ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_OR ||
           self->current->kind == LILY_TOKEN_KIND_R_SHIFT_R_SHIFT ||
           self->current->kind == LILY_TOKEN_KIND_DOT_DOT ||
           self->current->kind == LILY_TOKEN_KIND_MINUS ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_XOR) {
        enum LilyAstExprBinaryKind op =
          from_token__LilyAstExprBinary(self->current);
        Usize precedence = to_precedence__LilyAstExprBinaryKind(op);

        next_token__LilyParseBlock(self);

        LilyAstExpr *right = parse_primary_expr__LilyParseBlock(self);

        if (!right) {
            for (Usize i = 0; i < stack->len; i += 2) {
                FREE(LilyAstExpr, get__Vec(stack, i));
            }

            FREE(Vec, stack);

            return NULL;
        }

        while (precedence <= last_precedence && stack->len > 1) {
            LilyAstExpr *right = pop__Vec(stack);
            enum LilyAstExprBinaryKind op =
              (enum LilyAstExprBinaryKind)(Uptr)pop__Vec(stack);

            last_precedence = to_precedence__LilyAstExprBinaryKind(op);

            if (last_precedence < precedence) {
                push__Vec(stack, (int *)op);
                push__Vec(stack, right);
            }

            LilyAstExpr *left = pop__Vec(stack);

            push__Vec(
              stack,
              NEW_VARIANT(
                LilyAstExpr,
                binary,
                (Location){ .filename = self->current->location.filename,
                            .start_line = left->location.start_line,
                            .end_line = right->location.end_line,
                            .start_column = left->location.start_column,
                            .end_column = right->location.end_column },
                NEW(LilyAstExprBinary, op, left, right)));
        }

        push__Vec(stack, (int *)op);
        push__Vec(stack, right);

        last_precedence = precedence;
    }

    while (stack->len > 1) {
        LilyAstExpr *rhs = pop__Vec(stack);
        enum LilyAstExprBinaryKind op =
          (enum LilyAstExprBinaryKind)(Uptr)pop__Vec(stack);
        LilyAstExpr *lhs = pop__Vec(stack);

        push__Vec(
          stack,
          NEW_VARIANT(LilyAstExpr,
                      binary,
                      (Location){ .filename = self->current->location.filename,
                                  .start_line = lhs->location.start_line,
                                  .end_line = rhs->location.end_line,
                                  .start_column = lhs->location.start_column,
                                  .end_column = rhs->location.end_column },
                      NEW(LilyAstExprBinary, op, lhs, rhs)));
    }

    LilyAstExpr *res = pop__Vec(stack);

    FREE(Vec, stack);

    return res;
}

LilyAstExpr *
parse_fun_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
#define CLEAN_UP()                                                           \
    FREE(LilyAstExpr, id);                                                   \
    FREE_BUFFER_ITEMS(params->buffer, params->len, LilyAstExprFunParamCall); \
    FREE(Vec, params);

    next_token__LilyParseBlock(self); // skip `(`

    Vec *params = NEW(Vec); // Vec<LilyAstExprFunParamCall*>*
    Location location = clone__Location(&id->location);

    while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyToken *peeked = peek_token__LilyParseBlock(self, 1);

                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_COLON_EQ: {
                        Location location_param =
                          clone__Location(&self->current->location);
                        String *name =
                          clone__String(self->current->identifier_normal);

                        jump__LilyParseBlock(self, 2);

                        // Parse param expression
                        LilyAstExpr *value = parse_expr__LilyParseBlock(self);

                        if (!value) {
                            CLEAN_UP();

                            return NULL;
                        }

                        end__Location(&location_param,
                                      value->location.end_line,
                                      value->location.end_column);
                        push__Vec(params,
                                  NEW_VARIANT(LilyAstExprFunParamCall,
                                              default_,
                                              value,
                                              location_param,
                                              name));

                        break;
                    }
                    default:
                        goto parse_param_normal;
                }

                break;
            }
            default: {
            parse_param_normal : {
                Location location_param =
                  clone__Location(&self->current->location);
                LilyAstExpr *value = parse_expr__LilyParseBlock(self);

                if (!value) {
                    CLEAN_UP();

                    return NULL;
                }

                end__Location(&location_param,
                              value->location.end_line,
                              value->location.end_column);
                push__Vec(
                  params,
                  NEW_VARIANT(
                    LilyAstExprFunParamCall, normal, value, location_param));

                break;
            }
            }
        }

        CHECK_COMMA(LILY_TOKEN_KIND_R_PAREN);
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyParseBlock(self); // skip `)`

    return NEW_VARIANT(
      LilyAstExpr,
      call,
      location,
      NEW_VARIANT(LilyAstExprCall, fun, NEW(LilyAstExprCallFun, id, params)));
}

LilyAstExpr *
parse_record_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    next_token__LilyParseBlock(self); // skip `{`

    Vec *params = NEW(Vec); // Vec<LilyAstExprRecordParamCall*>*
    Location location = clone__Location(&id->location);

    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE) {
        String *name = NULL;

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                name = clone__String(self->current->identifier_normal);
                next_token__LilyParseBlock(self);
                break;
            default:
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                    NULL,
                    NULL,
                    NULL),
                  self->count_error);

                name = from__String("__error__");
        }

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_EQ:
                next_token__LilyParseBlock(self);
                break;
            default:
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                              NULL,
                              NULL,
                              from__String("expected `=`")),
                  self->count_error);

                break;
        }

        LilyAstExpr *value = parse_expr__LilyParseBlock(self);

        if (!value) {
            FREE(String, name);
            FREE_BUFFER_ITEMS(
              params->buffer, params->len, LilyAstExprRecordParamCall);
            FREE(Vec, params);

            return NULL;
        }

        push__Vec(params, NEW(LilyAstExprRecordParamCall, name, value));
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyParseBlock(self); // skip `}`

    return NEW_VARIANT(LilyAstExpr,
                       call,
                       location,
                       NEW_VARIANT(LilyAstExprCall,
                                   record,
                                   NEW(LilyAstExprCallRecord, id, params)));
}

LilyAstExpr *
parse_variant_call__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    Location location = clone__Location(&id->location);

    next_token__LilyParseBlock(self); // skip `:`

    LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

    if (!expr) {
        FREE(LilyAstExpr, id);

        return NULL;
    }

    end__Location(
      &location, expr->location.end_line, expr->location.end_column);

    return NEW_VARIANT(LilyAstExpr,
                       call,
                       location,
                       NEW_VARIANT(LilyAstExprCall,
                                   variant,
                                   NEW(LilyAstExprCallVariant, id, expr)));
}

LilyAstExpr *
parse_call_expr__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *id)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_BRACE:
            return parse_fun_call__LilyParseBlock(self, id);
        case LILY_TOKEN_KIND_L_PAREN:
            return parse_record_call__LilyParseBlock(self, id);
        case LILY_TOKEN_KIND_COLON:
            return parse_variant_call__LilyParseBlock(self, id);
        default: {
            String *token_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, token_s->buffer),
                NULL,
                NULL,
                from__String("expected `{`, `(` or `:`")),
              self->count_error);

            FREE(String, token_s);

            return NULL;
        }
    }
}

LilyAstExpr *
parse_lambda_expr__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #20");
}

LilyAstExpr *
parse_primary_expr__LilyParseBlock(LilyParseBlock *self)
{
    TODO("parse primary expr");
}

LilyAstExpr *
parse_expr__LilyParseBlock(LilyParseBlock *self)
{
    TODO("parse expr");
}

TEST(LilyAstDataType *, parse_data_type, LilyParseBlock *self)
{
    return parse_data_type__LilyParseBlock(self);
}

TEST(LilyAstExpr *, parse_expr, LilyParseBlock *self)
{
    return parse_expr__LilyParseBlock(self);
}

void
run__LilyParser(LilyParser *self)
{
#ifdef DEBUG_PARSER
    printf("====Parser(%s)====\n", self->package->file.name);

    for (Usize i = 0; i < self->decls->len; i++) {
        CALL_DEBUG(LilyAstDecl, get__Vec(self->decls, i));
    }
#endif
}

DESTRUCTOR(LilyParser, const LilyParser *self)
{
    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, LilyAstDecl);
    FREE(Vec, self->decls);
}
