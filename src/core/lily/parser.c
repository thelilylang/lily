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

#include <base/atof.h>
#include <base/atoi.h>
#include <base/optional.h>

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

// Parse hook access
static LilyAstExpr *
parse_hook_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access);

// Parse object access
// A.@Object@Object2@Object3
/// @param access LilyAstExpr*?
static LilyAstExpr *
parse_object_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access);

// Parse access expression
static LilyAstExpr *
parse_access_expr__LilyParseBlock(LilyParseBlock *self);

// Parse array expression
static LilyAstExpr *
parse_array_expr__LilyParseBlock(LilyParseBlock *self);

// Parse tuple expression
static LilyAstExpr *
parse_tuple_expr__LilyParseBlock(LilyParseBlock *self);

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

// Parse literal expression
static LilyAstExpr *
parse_literal_expr__LilyParseBlock(LilyParseBlock *self);

static LilyAstExpr *
parse_primary_expr__LilyParseBlock(LilyParseBlock *self);

// Parse expression
static LilyAstExpr *
parse_expr__LilyParseBlock(LilyParseBlock *self);

// Parse asm statement.
static LilyAstBodyFunItem *
parse_asm_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item);

// Parse await statement.
static LilyAstBodyFunItem *
parse_await_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse block statement.
static inline LilyAstBodyFunItem *
parse_block_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse break statement.
static LilyAstBodyFunItem *
parse_break_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse defer statement.
static LilyAstBodyFunItem *
parse_defer_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse drop statement.
static LilyAstBodyFunItem *
parse_drop_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item);

// Parse for statement.
static LilyAstBodyFunItem *
parse_for_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item);

// Parse if statement.
static LilyAstBodyFunItem *
parse_if_stmt__LilyParser(LilyParser *self,
                          const LilyPreparserFunBodyItem *item);

// Parse match statement.
static LilyAstBodyFunItem *
parse_match_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse next statement.
static LilyAstBodyFunItem *
parse_next_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item);

// Parse return statement.
static LilyAstBodyFunItem *
parse_return_stmt__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *item);

// Parse try statement.
static LilyAstBodyFunItem *
parse_try_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item);

// Parse variable statement.
static LilyAstBodyFunItem *
parse_variable_stmt__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item);

// Parse while statement.
static LilyAstBodyFunItem *
parse_while_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item);

// Parse statement.
static LilyAstBodyFunItem *
parse_stmt__LilyParser(LilyParser *self, const LilyPreparserFunBodyItem *item);

// Parse item of the body of the fun/method.
// NOTE: It's used to parse expr/stmt in statement (e.g. defer).
static LilyAstBodyFunItem *
parse_fun_body_item_for_stmt__LilyParser(LilyParser *self,
                                         const LilyPreparserFunBodyItem *item);

// Parse item of the body of the fun/method.
/// @param body Vec<LilyAstBodyFunItem*>*
static void
parse_fun_body_item__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item,
                                Vec *body);

/// @return Vec<LilyAstBodyFunItem*>*
static Vec *
parse_fun_body__LilyParser(LilyParser *self, Vec *block);

// Parse array pattern
// [a, _, b, ..]
static LilyAstPattern *
parse_array_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse as pattern
// _ as a
static LilyAstPattern *
parse_as_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse exception pattern
// error InvalidFile:x
static LilyAstPattern *
parse_exception_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse literal pattern
// "Hello"
static LilyAstPattern *
parse_literal_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse range pattern
// 'a' .. 'z'
static LilyAstPattern *
parse_range_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse record call pattern
// Person { name, .. }
static LilyAstPattern *
parse_record_call_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse tuple pattern
// (1, 2, _)
static LilyAstPattern *
parse_tuple_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse variant call pattern
// Cat:cat
static LilyAstPattern *
parse_variant_call_pattern__LilyParseBlock(LilyParseBlock *self);

// Parse all patterns.
static LilyAstPattern *
parse_pattern__LilyParseBlock(LilyParseBlock *self);

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

// Check if the ParseBlock has reached the end.
#define HAS_REACHED_THE_END(block) (block.position == block.tokens->len - 1)

#define CHECK_EXPR(expr, expr_block, help, detail_msg, block_free)        \
    if (!expr) {                                                          \
        block_free;                                                       \
    }                                                                     \
                                                                          \
    if (!HAS_REACHED_THE_END(expr_block)) {                               \
        emit__Diagnostic(                                                 \
          NEW_VARIANT(                                                    \
            Diagnostic,                                                   \
            simple_lily_error,                                            \
            expr_block.file,                                              \
            &expr_block.current->location,                                \
            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ONLY_ONE_EXPRESSION), \
            NULL,                                                         \
            help,                                                         \
            from__String(detail_msg)),                                    \
          expr_block.count_error);                                        \
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
                             .previous = get__Vec(tokens, 0),
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
        self->previous = get__Vec(self->tokens, self->position++);
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
    next_token__LilyParseBlock(self);

    Location location = clone__Location(&self->previous->location);

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            end__Location(&location,
                          self->previous->location.end_line,
                          self->previous->location.end_column);

            if (!strcmp(self->previous->identifier_normal->buffer, "Any")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_ANY, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Bool")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BOOL, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Byte")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BYTE, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Bytes")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_BYTES, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Char")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_CHAR, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Float32")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_FLOAT32, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Float64")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_FLOAT64, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int16")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT16, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int32")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT32, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int64")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT64, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Int8")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_INT8, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Isize")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_ISIZE, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Never")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_NEVER, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Str")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_STR, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint16")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT16, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint32")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT32, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint64")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT64, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Uint8")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UINT8, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Unit")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_UNIT, location);
            } else if (!strcmp(self->previous->identifier_normal->buffer,
                               "Usize")) {
                return NEW(
                  LilyAstDataType, LILY_AST_DATA_TYPE_KIND_USIZE, location);
            } else {
                // 1. Parse name
                String *name = clone__String(self->previous->identifier_normal);

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
        case LILY_TOKEN_KIND_KEYWORD_SELF:
            end__Location(&location,
                          self->previous->location.end_line,
                          self->previous->location.end_column);

            switch (self->previous->kind) {
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

        case LILY_TOKEN_KIND_L_HOOK:
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
            enum LilyTokenKind kind = self->previous->kind;

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
            String *token_s = to_string__LilyToken(self->previous);

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
parse_hook_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access)
{
    // <access>[<expr>]

    next_token__LilyParseBlock(self); // skip `[`

    Location location = clone__Location(&access->location);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

    if (!expr) {
        FREE(LilyAstExpr, access);

        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_R_HOOK:
            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyParseBlock(self);

            return NEW_VARIANT(
              LilyAstExpr,
              access,
              location,
              NEW_VARIANT(LilyAstExprAccess,
                          hook,
                          NEW(LilyAstExprAccessHook, access, expr)));
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `]`")),
              self->count_error);

            FREE(LilyAstExpr, access);
            FREE(LilyAstExpr, expr);

            return NULL;
        }
    }
}

LilyAstExpr *
parse_object_access__LilyParseBlock(LilyParseBlock *self, LilyAstExpr *access)
{
    // <access>@Object@Object2
    Location location;

    if (access) {
        location = clone__Location(&access->location);
    } else {
        location = clone__Location(&self->current->location);
    }

    Vec *object = NEW(Vec); // Vec<LilyAstDataType*>*

    while (self->current->kind == LILY_TOKEN_KIND_AT) {
        next_token__LilyParseBlock(self);

        LilyAstDataType *dt = parse_data_type__LilyParseBlock(self);

        if (!dt) {
            FREE(LilyAstExpr, access);
            FREE_BUFFER_ITEMS(object->buffer, object->len, LilyAstDataType);
            FREE(Vec, object);

            return NULL;
        }

        push__Vec(object, dt);
    }

    {
        LilyAstDataType *last = last__Vec(object);

        end__Location(
          &location, last->location.end_line, last->location.end_column);
    }

    return NEW_VARIANT(
      LilyAstExpr,
      access,
      location,
      NEW_VARIANT(LilyAstExprAccess,
                  object,
                  NEW(LilyAstExprAccessObject, access, object)));
}

LilyAstExpr *
parse_access_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);
    enum LilyTokenKind kind = self->previous->kind;

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

#define PARSE_CLOSING(closing)                                          \
    Vec *exprs = NEW(Vec); /* Vec<LilyAstExpr*>* */                     \
    while (self->current->kind != closing) {                            \
        LilyAstExpr *expr = parse_expr__LilyParseBlock(self);           \
                                                                        \
        if (expr) {                                                     \
            push__Vec(exprs, expr);                                     \
        } else {                                                        \
            SKIP_TO_TOKEN(closing);                                     \
                                                                        \
            FREE_BUFFER_ITEMS(exprs->buffer, exprs->len, LilyAstExpr);  \
            FREE(Vec, exprs);                                           \
                                                                        \
            return NULL;                                                \
        }                                                               \
                                                                        \
        if (self->current->kind != closing) {                           \
            switch (self->current->kind) {                              \
                case LILY_TOKEN_KIND_COMMA:                             \
                    next_token__LilyParseBlock(self);                   \
                    break;                                              \
                default:                                                \
                    emit__Diagnostic(                                   \
                      NEW_VARIANT(                                      \
                        Diagnostic,                                     \
                        simple_lily_error,                              \
                        self->file,                                     \
                        &self->current->location,                       \
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN), \
                        NULL,                                           \
                        NULL,                                           \
                        from__String("expected `,`")),                  \
                      self->count_error);                               \
            }                                                           \
        }                                                               \
    }                                                                   \
                                                                        \
    end__Location(&location,                                            \
                  self->current->location.end_line,                     \
                  self->current->location.end_column);                  \
    next_token__LilyParseBlock(self); /* skip closing */

LilyAstExpr *
parse_array_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);

    PARSE_CLOSING(LILY_TOKEN_KIND_R_HOOK);

    return NEW_VARIANT(
      LilyAstExpr, array, location, NEW(LilyAstExprArray, exprs));
}

LilyAstExpr *
parse_tuple_expr__LilyParseBlock(LilyParseBlock *self)
{
    Location location = clone__Location(&self->previous->location);

    PARSE_CLOSING(LILY_TOKEN_KIND_R_PAREN);

    return NEW_VARIANT(
      LilyAstExpr, tuple, location, NEW(LilyAstExprTuple, exprs));
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

                break;
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
parse_literal_expr__LilyParseBlock(LilyParseBlock *self)
{
    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
            return NEW_VARIANT(LilyAstExpr,
                               literal,
                               clone__Location(&self->previous->location),
                               NEW_VARIANT(LilyAstExprLiteral, bool, true));
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
            return NEW_VARIANT(LilyAstExpr,
                               literal,
                               clone__Location(&self->previous->location),
                               NEW_VARIANT(LilyAstExprLiteral, bool, false));
        case LILY_TOKEN_KIND_KEYWORD_NIL:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW(LilyAstExprLiteral, LILY_AST_EXPR_LITERAL_KIND_NIL));
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW(LilyAstExprLiteral, LILY_AST_EXPR_LITERAL_KIND_UNDEF));
        case LILY_TOKEN_KIND_LITERAL_BYTE:
            return NEW_VARIANT(LilyAstExpr,
                               literal,
                               clone__Location(&self->previous->location),
                               NEW_VARIANT(LilyAstExprLiteral, bool, false));
        case LILY_TOKEN_KIND_LITERAL_BYTES: {
            Usize size = strlen((const char *)self->previous->literal_bytes);
            Uint8 *bytes_copy = lily_malloc(size);

            memcpy(bytes_copy, self->previous->literal_bytes, size);

            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral, bytes, bytes_copy));
        }
        case LILY_TOKEN_KIND_LITERAL_CHAR:
            return NEW_VARIANT(LilyAstExpr,
                               literal,
                               clone__Location(&self->previous->location),
                               NEW_VARIANT(LilyAstExprLiteral,
                                           char,
                                           self->previous->literal_char));
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(
                LilyAstExprLiteral,
                float64,
                atof__Float64(self->previous->literal_float->buffer)));
        case LILY_TOKEN_KIND_LITERAL_INT_2:
        case LILY_TOKEN_KIND_LITERAL_INT_8:
        case LILY_TOKEN_KIND_LITERAL_INT_10:
        case LILY_TOKEN_KIND_LITERAL_INT_16: {
            int base = 10;

            switch (self->previous->kind) {
                case LILY_TOKEN_KIND_LITERAL_INT_2:
                    base = 2;
                    break;
                case LILY_TOKEN_KIND_LITERAL_INT_8:
                    base = 8;
                    break;
                case LILY_TOKEN_KIND_LITERAL_INT_16:
                    base = 16;
                    break;
                default:
                    break;
            }

            // try to cast to Int32
            Optional *op =
              atoi_safe__Int32(self->previous->literal_int_2->buffer, base);

            if (is_some__Optional(op)) {
                Int32 int32 = (Int32)(Uptr)get__Optional(op);

                FREE(Optional, op);

                return NEW_VARIANT(
                  LilyAstExpr,
                  literal,
                  clone__Location(&self->previous->location),
                  NEW_VARIANT(LilyAstExprLiteral, int32, int32));
            } else {
                FREE(Optional, op);

                // try to cast to Int64
                op =
                  atoi_safe__Int64(self->previous->literal_int_2->buffer, base);

                if (is_some__Optional(op)) {
                    Int64 int64 = (Int64)(Uptr)get__Optional(op);

                    FREE(Optional, op);

                    return NEW_VARIANT(
                      LilyAstExpr,
                      literal,
                      clone__Location(&self->previous->location),
                      NEW_VARIANT(LilyAstExprLiteral, int64, int64));
                } else {
                    FREE(Optional, op);

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->previous->location,
                        NEW(LilyError,
                            LILY_ERROR_KIND_FEATURE_NOT_YET_SUPPORTED),
                        NULL,
                        NULL,
                        from__String(
                          "Int128 and Uint128 data type not yet supported")),
                      self->count_error);

                    return NULL;
                }
            }
        }
        case LILY_TOKEN_KIND_LITERAL_STRING:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          str,
                          clone__String(self->previous->literal_string)));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_float32,
                          self->previous->literal_suffix_float32));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_float64,
                          self->previous->literal_suffix_float64));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_int16,
                          self->previous->literal_suffix_int16));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_int32,
                          self->previous->literal_suffix_int32));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_int64,
                          self->previous->literal_suffix_int64));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_int8,
                          self->previous->literal_suffix_int8));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_isize,
                          self->previous->literal_suffix_isize));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_uint8,
                          self->previous->literal_suffix_uint8));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_uint16,
                          self->previous->literal_suffix_uint16));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_uint32,
                          self->previous->literal_suffix_uint32));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_uint64,
                          self->previous->literal_suffix_uint64));
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return NEW_VARIANT(
              LilyAstExpr,
              literal,
              clone__Location(&self->previous->location),
              NEW_VARIANT(LilyAstExprLiteral,
                          suffix_usize,
                          self->previous->literal_suffix_usize));
        default:
            UNREACHABLE("this way is impossible");
    }

    return NULL;
}

LilyAstExpr *
parse_primary_expr__LilyParseBlock(LilyParseBlock *self)
{
    next_token__LilyParseBlock(self);

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
        case LILY_TOKEN_KIND_KEYWORD_NIL:
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
        case LILY_TOKEN_KIND_LITERAL_BYTE:
        case LILY_TOKEN_KIND_LITERAL_BYTES:
        case LILY_TOKEN_KIND_LITERAL_CHAR:
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
        case LILY_TOKEN_KIND_LITERAL_INT_2:
        case LILY_TOKEN_KIND_LITERAL_INT_8:
        case LILY_TOKEN_KIND_LITERAL_INT_10:
        case LILY_TOKEN_KIND_LITERAL_INT_16:
        case LILY_TOKEN_KIND_LITERAL_STRING:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            return parse_literal_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_L_HOOK:
            return parse_array_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_L_PAREN: {
            LilyToken *peeked = self->current;

            for (Usize i = 1; peeked; i++) {
                if (peeked->kind == LILY_TOKEN_KIND_COMMA) {
                    return parse_tuple_expr__LilyParseBlock(self);
                } else if (peeked->kind == LILY_TOKEN_KIND_R_PAREN) {
                    Location location =
                      clone__Location(&self->previous->location);
                    LilyAstExpr *expr = parse_expr__LilyParseBlock(self);

                    if (!expr) {
                        return NULL;
                    }

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_R_PAREN:
                            end__Location(&location,
                                          self->current->location.end_line,
                                          self->current->location.end_column);
                            next_token__LilyParseBlock(self);

                            return NEW_VARIANT(
                              LilyAstExpr, grouping, location, expr);
                        default:
                            // ERROR: expected `)`
                            return NULL;
                    }
                }

                peeked = peek_token__LilyParseBlock(self, i);
            }
        }
        case LILY_TOKEN_KIND_KEYWORD_GLOBAL:
        case LILY_TOKEN_KIND_KEYWORD_SELF:
        case LILY_TOKEN_KIND_AT:
            if (self->previous->kind == LILY_TOKEN_KIND_KEYWORD_SELF &&
                self->current->kind != LILY_TOKEN_KIND_DOT) {
                return NEW(LilyAstExpr,
                           clone__Location(&self->previous->location),
                           LILY_AST_EXPR_KIND_SELF);
            } else if (self->previous->kind == LILY_TOKEN_KIND_AT &&
                       self->current->kind ==
                         LILY_TOKEN_KIND_IDENTIFIER_NORMAL) {
                return parse_object_access__LilyParseBlock(self, NULL);
            }

            return parse_access_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            if (!strcmp(self->previous->identifier_normal->buffer, "_")) {
                return NEW(LilyAstExpr,
                           clone__Location(&self->previous->location),
                           LILY_AST_EXPR_KIND_WILDCARD);
            }

            if (self->current->kind == LILY_TOKEN_KIND_DOT) {
                return parse_path_access__LilyParseBlock(
                  self,
                  NEW_VARIANT(
                    LilyAstExpr,
                    identifier,
                    clone__Location(&self->previous->location),
                    NEW(LilyAstExprIdentifier,
                        clone__String(self->previous->identifier_normal))));
            }

            return NEW_VARIANT(
              LilyAstExpr,
              identifier,
              clone__Location(&self->previous->location),
              NEW(LilyAstExprIdentifier,
                  clone__String(self->previous->identifier_normal)));
        case LILY_TOKEN_KIND_IDENTIFIER_DOLLAR:
            return NEW_VARIANT(
              LilyAstExpr,
              identifier_dollar,
              clone__Location(&self->previous->location),
              NEW(LilyAstExprIdentifierDollar,
                  clone__String(self->previous->identifier_dollar)));
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            return parse_lambda_expr__LilyParseBlock(self);
        case LILY_TOKEN_KIND_KEYWORD_REF:
        case LILY_TOKEN_KIND_KEYWORD_TRACE:
        case LILY_TOKEN_KIND_MINUS:
        case LILY_TOKEN_KIND_KEYWORD_NOT: {
            enum LilyAstExprUnaryKind op = 0;

            switch (self->previous->kind) {
                case LILY_TOKEN_KIND_MINUS:
                    op = LILY_AST_EXPR_UNARY_KIND_NEG;
                    break;
                case LILY_TOKEN_KIND_KEYWORD_NOT:
                    op = LILY_AST_EXPR_UNARY_KIND_NOT;
                    break;
                case LILY_TOKEN_KIND_KEYWORD_REF: {
                    if (self->current->kind == LILY_TOKEN_KIND_KEYWORD_MUT) {
                        next_token__LilyParseBlock(self);

                        op = LILY_AST_EXPR_UNARY_KIND_REF_MUT;
                    } else {
                        op = LILY_AST_EXPR_UNARY_KIND_REF;
                    }
                }
                case LILY_TOKEN_KIND_KEYWORD_TRACE: {
                    if (self->current->kind == LILY_TOKEN_KIND_KEYWORD_MUT) {
                        next_token__LilyParseBlock(self);

                        op = LILY_AST_EXPR_UNARY_KIND_TRACE_MUT;
                    } else {
                        op = LILY_AST_EXPR_UNARY_KIND_TRACE;
                    }
                }
                default:
                    break;
            }

            Location location = clone__Location(&self->previous->location);
            LilyAstExpr *right = parse_expr__LilyParseBlock(self);

            if (!right) {
                return NULL;
            }

            end__Location(
              &location, right->location.end_line, right->location.end_column);

            return NEW_VARIANT(
              LilyAstExpr, unary, location, NEW(LilyAstExprUnary, op, right));
        }
        default:
            return NULL;
    }
}

LilyAstExpr *
parse_expr__LilyParseBlock(LilyParseBlock *self)
{
    LilyAstExpr *expr = parse_primary_expr__LilyParseBlock(self);

    // Parse dereference
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_DOT_STAR: {
            Location location = clone__Location(&expr->location);

            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyParseBlock(self);

            expr = NEW_VARIANT(LilyAstExpr,
                               unary,
                               location,
                               NEW(LilyAstExprUnary,
                                   LILY_AST_EXPR_UNARY_KIND_DEREFERENCE,
                                   expr));

            break;
        }
        default:
            break;
    }

    // parse call and object access
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_AT:
            switch (expr->kind) {
                case LILY_AST_EXPR_KIND_ACCESS:
                    expr = parse_object_access__LilyParseBlock(self, expr);

                    if (!expr) {
                        return NULL;
                    }
                default:
                    break;
            }

            break;
        case LILY_TOKEN_KIND_L_PAREN:
        case LILY_TOKEN_KIND_L_BRACE:
        case LILY_TOKEN_KIND_COLON:
            switch (expr->kind) {
                case LILY_AST_EXPR_KIND_ACCESS:
                case LILY_AST_EXPR_KIND_IDENTIFIER:
                    expr = parse_call_expr__LilyParseBlock(self, expr);

                    if (!expr) {
                        return NULL;
                    }
                default:
                    break;
            }

            break;
        default:
            break;
    }

    // parse hook access
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            switch (expr->kind) {
                case LILY_AST_EXPR_KIND_ACCESS:
                case LILY_AST_EXPR_KIND_IDENTIFIER:
                    expr = parse_hook_access__LilyParseBlock(self, expr);
                    break;
                default:
                    break;
            }

            break;
        default:
            break;
    }

    // parse binary expression
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_PLUS:
        case LILY_TOKEN_KIND_KEYWORD_AND:
        case LILY_TOKEN_KIND_PLUS_EQ:
        case LILY_TOKEN_KIND_AMPERSAND_EQ:
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT_EQ:
        case LILY_TOKEN_KIND_BAR_EQ:
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT_EQ:
        case LILY_TOKEN_KIND_SLASH_EQ:
        case LILY_TOKEN_KIND_STAR_STAR_EQ:
        case LILY_TOKEN_KIND_PERCENTAGE_EQ:
        case LILY_TOKEN_KIND_STAR_EQ:
        case LILY_TOKEN_KIND_MINUS_EQ:
        case LILY_TOKEN_KIND_XOR_EQ:
        case LILY_TOKEN_KIND_EQ:
        case LILY_TOKEN_KIND_AMPERSAND:
        case LILY_TOKEN_KIND_BAR:
        case LILY_TOKEN_KIND_BAR_R_SHIFT:
        case LILY_TOKEN_KIND_SLASH:
        case LILY_TOKEN_KIND_EQ_EQ:
        case LILY_TOKEN_KIND_STAR_STAR:
        case LILY_TOKEN_KIND_R_SHIFT_EQ:
        case LILY_TOKEN_KIND_R_SHIFT:
        case LILY_TOKEN_KIND_L_SHIFT_L_SHIFT:
        case LILY_TOKEN_KIND_L_SHIFT_EQ:
        case LILY_TOKEN_KIND_L_SHIFT:
        case LILY_TOKEN_KIND_PERCENTAGE:
        case LILY_TOKEN_KIND_STAR:
        case LILY_TOKEN_KIND_NOT_EQ:
        case LILY_TOKEN_KIND_KEYWORD_OR:
        case LILY_TOKEN_KIND_R_SHIFT_R_SHIFT:
        case LILY_TOKEN_KIND_DOT_DOT:
        case LILY_TOKEN_KIND_MINUS:
        case LILY_TOKEN_KIND_KEYWORD_XOR:
            return parse_binary_expr__LilyParseBlock(self, expr);
        default:
            return expr;
    }
}

LilyAstBodyFunItem *
parse_asm_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_asm.params->len > 0) {
        // 1. Parse first expression
        LilyAstExpr *value = NULL;

        {
            LilyParseBlock value_block =
              NEW(LilyParseBlock, self, get__Vec(item->stmt_asm.params, 0));
            value = parse_primary_expr__LilyParseBlock(&value_block);

            if (!value) {
                return NULL;
            }
        }

        // 2. Parse all other params
        Vec *params = NEW(Vec); // Vec<LilyAstExpr*>*

        for (Usize i = 1; i < item->stmt_asm.params->len; i++) {
            LilyParseBlock param_block =
              NEW(LilyParseBlock, self, get__Vec(item->stmt_asm.params, i));
            LilyAstExpr *param = parse_expr__LilyParseBlock(&param_block);

            CHECK_EXPR(param, param_block, NULL, "expected `,`", {
                FREE(LilyAstExpr, value);
                FREE_BUFFER_ITEMS(params->buffer, params->len, LilyAstExpr);
                FREE(Vec, params);

                return NULL;
            });

            push__Vec(params, param);
        }

        return NEW_VARIANT(LilyAstBodyFunItem,
                           stmt,
                           NEW_VARIANT(LilyAstStmt,
                                       asm,
                                       item->location,
                                       NEW(LilyAstStmtAsm, value, params)));
    } else {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      &self->package->file,
                      &item->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_ASM_PARAM),
                      NULL,
                      NULL,
                      NULL),
          &self->package->count_error);

        return NULL;
    }
}

LilyAstBodyFunItem *
parse_await_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    LilyParseBlock expr_block =
      NEW(LilyParseBlock, self, item->stmt_await.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    if (!expr) {
        return NULL;
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, await, item->location, NEW(LilyAstStmtAwait, expr)));
}

LilyAstBodyFunItem *
parse_block_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   block,
                                   item->location,
                                   NEW(LilyAstStmtBlock,
                                       parse_fun_body__LilyParser(
                                         self, item->stmt_block.block))));
}

LilyAstBodyFunItem *
parse_break_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    if (item->stmt_break.name) {
        return NEW_VARIANT(
          LilyAstBodyFunItem,
          stmt,
          NEW_VARIANT(LilyAstStmt,
                      break,
                      item->location,
                      NEW(LilyAstStmtBreak, item->stmt_break.name)));
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, break, item->location, NEW(LilyAstStmtBreak, NULL)));
}

LilyAstBodyFunItem *
parse_defer_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    LilyAstBodyFunItem *item_ast =
      parse_fun_body_item_for_stmt__LilyParser(self, item->stmt_defer.item);

    if (!item_ast) {
        return NULL;
    }

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, defer, item->location, NEW(LilyAstStmtDefer, item_ast)));
}

LilyAstBodyFunItem *
parse_drop_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item)
{
    LilyParseBlock expr_block = NEW(LilyParseBlock, self, item->stmt_drop.expr);
    LilyAstExpr *expr = parse_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr, expr_block, NULL, "expected `;`", { return NULL; });

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, drop, item->location, NEW(LilyAstStmtDrop, expr)));
}

LilyAstBodyFunItem *
parse_for_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item)
{
    LilyParseBlock expr_block = NEW(LilyParseBlock, self, item->stmt_for.expr);
    LilyAstExpr *expr_left = parse_primary_expr__LilyParseBlock(&expr_block);

    if (!expr_left) {
        return NULL;
    }

    switch (expr_block.current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_IN:
            next_token__LilyParseBlock(&expr_block);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          expr_block.file,
                          &expr_block.current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `in` keyword")),
              expr_block.count_error);
    }

    LilyAstExpr *expr_right = parse_primary_expr__LilyParseBlock(&expr_block);

    CHECK_EXPR(expr_right,
               expr_block,
               init__Vec(1, from__String("<expr> in <expr>")),
               "expected `do` keyword",
               {
                   FREE(LilyAstExpr, expr_left);
                   return NULL;
               });

    return NEW_VARIANT(
      LilyAstBodyFunItem,
      stmt,
      NEW_VARIANT(
        LilyAstStmt, for, item->location, NEW(LilyAstStmtFor, expr_left, expr_right, parse_fun_body__LilyParser(self, item->stmt_for.block))));
}

LilyAstBodyFunItem *
parse_if_stmt__LilyParser(LilyParser *self,
                          const LilyPreparserFunBodyItem *item)
{
    // 1. Parse if expression
    LilyParseBlock if_expr_block =
      NEW(LilyParseBlock, self, item->stmt_if.if_expr);
    LilyAstExpr *if_expr = parse_expr__LilyParseBlock(&if_expr_block);

    CHECK_EXPR(if_expr, if_expr_block, NULL, "expected `do` keyword", {});

    // 2. Parse if block
    Vec *if_body = parse_fun_body__LilyParser(self, item->stmt_if.if_block);

    Vec *elif_exprs = NULL;  // Vec<LilyAstExpr*>*
    Vec *elif_bodies = NULL; // Vec<Vec<LilyAstBodyFunItem*>*>*

    if (item->stmt_if.elif_exprs) {
        // 3. Parse elif expressions
        elif_exprs = NEW(Vec); // Vec<LilyAstExpr*>*

        for (Usize i = 0; i < item->stmt_if.elif_exprs->len; i++) {
            LilyParseBlock expr_block =
              NEW(LilyParseBlock, self, get__Vec(item->stmt_if.elif_exprs, i));
            LilyAstExpr *elif_expr = parse_expr__LilyParseBlock(&expr_block);

            CHECK_EXPR(
              elif_expr, expr_block, NULL, "expected `do` keyword", {});

            if (elif_expr) {
                push__Vec(elif_exprs, elif_expr);
            }
        }

        // 4. Parse elif blocks
        elif_bodies = NEW(Vec); // Vec<Vec<LilyAstBodyFunItem*>*>*

        for (Usize i = 0; i < item->stmt_if.elif_blocks->len; i++) {
            push__Vec(elif_bodies,
                      parse_fun_body__LilyParser(
                        self, get__Vec(item->stmt_if.elif_blocks, i)));
        }
    }

    Vec *else_body = NULL;

    if (item->stmt_if.else_block) {
        // 4. Parse else block
        else_body = parse_fun_body__LilyParser(self, item->stmt_if.else_block);
    }

    return NEW_VARIANT(LilyAstBodyFunItem,
                       stmt,
                       NEW_VARIANT(LilyAstStmt,
                                   if,
                                   item->location,
                                   NEW(LilyAstStmtIf,
                                       if_expr,
                                       if_body,
                                       elif_exprs,
                                       elif_bodies,
                                       else_body)));
}

LilyAstBodyFunItem *
parse_match_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    TODO("Issue #41");
}

LilyAstBodyFunItem *
parse_next_stmt__LilyParser(LilyParser *self,
                            const LilyPreparserFunBodyItem *item)
{
    TODO("Issue #42");
}

LilyAstBodyFunItem *
parse_return_stmt__LilyParser(LilyParser *self,
                              const LilyPreparserFunBodyItem *item)
{
    TODO("Issue #43");
}

LilyAstBodyFunItem *
parse_try_stmt__LilyParser(LilyParser *self,
                           const LilyPreparserFunBodyItem *item)
{
    TODO("Issue #101");
}

LilyAstBodyFunItem *
parse_variable_stmt__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item)
{
    TODO("Issue #102");
}

LilyAstBodyFunItem *
parse_while_stmt__LilyParser(LilyParser *self,
                             const LilyPreparserFunBodyItem *item)
{
    TODO("Issue #103");
}

LilyAstBodyFunItem *
parse_stmt__LilyParser(LilyParser *self, const LilyPreparserFunBodyItem *item)
{
    switch (item->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM:
            return parse_asm_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT:
            return parse_await_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
            return parse_block_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK:
            return parse_break_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER:
            return parse_defer_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP:
            return parse_drop_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR:
            return parse_for_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF:
            return parse_if_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH:
            return parse_match_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT:
            return parse_next_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN:
            return parse_return_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
            return parse_try_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE:
            return parse_variable_stmt__LilyParser(self, item);
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE:
            return parse_while_stmt__LilyParser(self, item);
        default:
            UNREACHABLE("no other statement");
    }

    return NULL;
}

LilyAstBodyFunItem *
parse_fun_body_item_for_stmt__LilyParser(LilyParser *self,
                                         const LilyPreparserFunBodyItem *item)
{
    switch (item->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS: {
            LilyParseBlock exprs_block =
              NEW(LilyParseBlock, self, item->exprs.tokens);
            LilyAstExpr *expr = parse_expr__LilyParseBlock(&exprs_block);

            CHECK_EXPR(
              expr, exprs_block, NULL, "expected `;`", { return NULL; });

            return NEW_VARIANT(LilyAstBodyFunItem, expr, expr);
        }
        default:
            return parse_stmt__LilyParser(self, item);
    }
}

void
parse_fun_body_item__LilyParser(LilyParser *self,
                                const LilyPreparserFunBodyItem *item,
                                Vec *body)
{
    switch (item->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS: {
            LilyParseBlock exprs_block =
              NEW(LilyParseBlock, self, item->exprs.tokens);

            do {
                LilyAstExpr *expr = parse_expr__LilyParseBlock(&exprs_block);

                if (expr) {
                    push__Vec(body,
                              NEW_VARIANT(LilyAstBodyFunItem, expr, expr));
                }
            } while (!HAS_REACHED_THE_END(exprs_block));
        }
        default: {
            LilyAstBodyFunItem *stmt = parse_stmt__LilyParser(self, item);

            if (stmt) {
                push__Vec(body, stmt);
            }
        }
    }
}

Vec *
parse_fun_body__LilyParser(LilyParser *self, Vec *block)
{
    Vec *body = NEW(Vec); // Vec<LilyAstBodyFunItem*>*

    for (Usize i = 0; i < block->len; i++) {
        parse_fun_body_item__LilyParser(self, get__Vec(block, i), body);
    }

    return body;
}

LilyAstPattern *
parse_array_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #58");
}

LilyAstPattern *
parse_as_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #59");
}

LilyAstPattern *
parse_exception_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #60");
}

LilyAstPattern *
parse_literal_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #61");
}

LilyAstPattern *
parse_range_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #61");
}

LilyAstPattern *
parse_record_call_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #62");
}

LilyAstPattern *
parse_tuple_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #63");
}

LilyAstPattern *
parse_variant_call_pattern__LilyParseBlock(LilyParseBlock *self)
{
    TODO("Issue #64");
}

LilyAstPattern *
parse_pattern__LilyParseBlock(LilyParseBlock *self)
{
    next_token__LilyParseBlock(self);

    LilyAstPattern *pattern = NULL;

    switch (self->previous->kind) {
        case LILY_TOKEN_KIND_KEYWORD_TRUE:
        case LILY_TOKEN_KIND_KEYWORD_FALSE:
        case LILY_TOKEN_KIND_KEYWORD_NIL:
        case LILY_TOKEN_KIND_KEYWORD_UNDEF:
        case LILY_TOKEN_KIND_LITERAL_BYTE:
        case LILY_TOKEN_KIND_LITERAL_BYTES:
        case LILY_TOKEN_KIND_LITERAL_CHAR:
        case LILY_TOKEN_KIND_LITERAL_FLOAT:
        case LILY_TOKEN_KIND_LITERAL_INT_2:
        case LILY_TOKEN_KIND_LITERAL_INT_8:
        case LILY_TOKEN_KIND_LITERAL_INT_10:
        case LILY_TOKEN_KIND_LITERAL_INT_16:
        case LILY_TOKEN_KIND_LITERAL_STRING:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_FLOAT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_INT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_ISIZE:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT16:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT32:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT64:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_UINT8:
        case LILY_TOKEN_KIND_LITERAL_SUFFIX_USIZE:
            pattern = parse_literal_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_L_HOOK:
            pattern = parse_array_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
            pattern = parse_exception_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            // Check if it's a path
            switch (self->current->kind) {
                case LILY_TOKEN_KIND_DOT:
                    break;
                default:
                    break;
            }

            if (!strcmp(self->previous->identifier_normal->buffer, "_")) {
                pattern = NEW(LilyAstPattern,
                              self->previous->location,
                              LILY_AST_PATTERN_KIND_WILDCARD);
            } else {
                pattern = NEW_VARIANT(
                  LilyAstPattern,
                  name,
                  self->previous->location,
                  NEW(LilyAstPatternName,
                      clone__String(self->previous->identifier_normal)));
            }

            break;
        case LILY_TOKEN_KIND_L_PAREN:
            pattern = parse_tuple_pattern__LilyParseBlock(self);

            break;
        case LILY_TOKEN_KIND_DOT_DOT:
            pattern = NEW(LilyAstPattern,
                          self->previous->location,
                          LILY_AST_PATTERN_KIND_AUTO_COMPLETE);

            break;
        default:
            // ERROR: unexpected token.
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_AS:
            break;
        case LILY_TOKEN_KIND_DOT_DOT:
            break;
        default:
            break;
    }

    return pattern;
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
