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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, PATTERNESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/alloc.h>
#include <base/assert.h>
#include <base/macros.h>

#include <core/lily/analysis/checked/pattern.h>
#include <core/lily/analysis/checked/scope.h>
#include <core/lily/analysis/checked/stmt/variable.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, array, LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_ERROR).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, error, LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_LIST).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, list, LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_LIST_HEAD).
static VARIANT_DESTRUCTOR(LilyCheckedPattern,
                          list_head,
                          LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_LIST_TAIL).
static VARIANT_DESTRUCTOR(LilyCheckedPattern,
                          list_tail,
                          LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_LITERAL).
static VARIANT_DESTRUCTOR(LilyCheckedPattern,
                          literal,
                          LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_RANGE).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, range, LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_RECORD_CALL).
static VARIANT_DESTRUCTOR(LilyCheckedPattern,
                          record_call,
                          LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, tuple, LilyCheckedPattern *self);

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_VARIANT_CALL).
static VARIANT_DESTRUCTOR(LilyCheckedPattern,
                          variant_call,
                          LilyCheckedPattern *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternKind,
               enum LilyCheckedPatternKind self)
{
    switch (self) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            return "LILY_CHECKED_PATTERN_KIND_ARRAY";
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            return "LILY_CHECKED_PATTERN_KIND_ERROR";
        case LILY_CHECKED_PATTERN_KIND_ELSE:
            return "LILY_CHECKED_PATTERN_KIND_ELSE";
        case LILY_CHECKED_PATTERN_KIND_LIST:
            return "LILY_CHECKED_PATTERN_KIND_LIST";
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            return "LILY_CHECKED_PATTERN_KIND_LIST_HEAD";
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            return "LILY_CHECKED_PATTERN_KIND_LIST_TAIL";
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            return "LILY_CHECKED_PATTERN_KIND_LITERAL";
        case LILY_CHECKED_PATTERN_KIND_NONE:
            return "LILY_CHECKED_PATTERN_KIND_NONE";
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            return "LILY_CHECKED_PATTERN_KIND_RANGE";
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            return "LILY_CHECKED_PATTERN_KIND_RECORD_CALL";
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            return "LILY_CHECKED_PATTERN_KIND_TUPLE";
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            return "LILY_CHECKED_PATTERN_KIND_UNKNOWN";
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            return "LILY_CHECKED_PATTERN_KIND_VARIANT_CALL";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    array,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternArray array)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_ARRAY;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    error,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternError error)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_ERROR;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    list,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternList list)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_LIST;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    list_head,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternListHead list_head)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_LIST_HEAD;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->list_head = list_head;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    list_tail,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternListTail list_tail)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_LIST_TAIL;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->list_tail = list_tail;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    literal,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternLiteral literal)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_LITERAL;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->literal = literal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    range,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternRange range)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_RANGE;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->range = range;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    record_call,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternRecordCall record_call)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_RECORD_CALL;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->record_call = record_call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    tuple,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternTuple tuple)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_TUPLE;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->tuple = tuple;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    unknown,
                    const Location *location,
                    const LilyAstPattern *ast_pattern)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_UNKNOWN;
    self->location = location;
    self->data_type =
      NEW(LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN, location);
    self->ast_pattern = ast_pattern;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    variant_call,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternVariantCall variant_call)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_VARIANT_CALL;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->variant_call = variant_call;

    return self;
}

CONSTRUCTOR(LilyCheckedPattern *,
            LilyCheckedPattern,
            enum LilyCheckedPatternKind kind,
            const Location *location,
            LilyCheckedDataType *data_type,
            const LilyAstPattern *ast_pattern)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = kind;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;

    return self;
}

bool
eq__LilyCheckedPattern(const LilyCheckedPattern *self,
                       const LilyCheckedPattern *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    switch (self->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            return eq__LilyCheckedPatternArray(&self->array, &other->array);
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            return eq__LilyCheckedPatternError(&self->error, &other->error);
        case LILY_CHECKED_PATTERN_KIND_LIST:
            return eq__LilyCheckedPatternList(&self->list, &other->list);
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            return eq__LilyCheckedPatternListHead(&self->list_head,
                                                  &other->list_head);
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            return eq__LilyCheckedPatternListTail(&self->list_tail,
                                                  &other->list_tail);
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            return eq__LilyCheckedPatternLiteral(&self->literal,
                                                 &other->literal);
        case LILY_CHECKED_PATTERN_KIND_NONE:
            return true;
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            return eq__LilyCheckedPatternRange(&self->range, &other->range);
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            return eq__LilyCheckedPatternRecordCall(&self->record_call,
                                                    &other->record_call);
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            return eq__LilyCheckedPatternTuple(&self->tuple, &other->tuple);
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            return false;
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            return eq__LilyCheckedPatternVariantCall(&self->variant_call,
                                                     &other->variant_call);
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
lazy_eq__LilyCheckedPattern(const LilyCheckedPattern *self,
                            const LilyCheckedPattern *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    switch (self->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            break;
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            break;
        case LILY_CHECKED_PATTERN_KIND_LIST:
            break;
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            break;
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            break;
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            break;
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            break;
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            break;
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            break;
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            return false;
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#define CALL_VARIABLE(location, res_variable)             \
    NEW_VARIANT(LilyCheckedExpr,                          \
                call,                                     \
                location,                                 \
                res_variable.variable->data_type,         \
                NULL,                                     \
                NEW(LilyCheckedExprCall,                  \
                    LILY_CHECKED_EXPR_CALL_KIND_VARIABLE, \
                    variable_name,                        \
                    (LilyCheckedAccessScope){             \
                      .id = res_variable.scope_container.scope_id }))

LilyCheckedExpr *
to_expr__LilyCheckedPattern(const LilyCheckedPattern *self,
                            const Location *location,
                            LilyCheckedScope *scope,
                            LilyCheckedExpr *current_expr)
{
    switch (self->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY: {
            ASSERT(current_expr->data_type->kind ==
                   LILY_CHECKED_DATA_TYPE_KIND_ARRAY);

            LilyCheckedDataType *dt = NEW(
              LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, location);
            LilyCheckedExpr *left =
              NEW_VARIANT(LilyCheckedExpr,
                          binary,
                          location,
                          dt,
                          NULL,
                          NEW(LilyCheckedExprBinary,
                              self->array.must_eq
                                ? LILY_CHECKED_EXPR_BINARY_KIND_EQ
                                : LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ,
                              ref__LilyCheckedExpr(current_expr),
                              NEW_VARIANT(LilyCheckedExpr,
                                          literal,
                                          location,
                                          NEW(LilyCheckedDataType,
                                              LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                              location),
                                          NULL,
                                          NEW_VARIANT(LilyCheckedExprLiteral,
                                                      suffix_usize,
                                                      self->array.len))));

            for (Usize i = 0; i < self->array.table->len; ++i) {
                LilyCheckedPatternTableItem *item =
                  get__Vec(self->array.table, i);
                LilyCheckedExpr *new_current_expr = NEW_VARIANT(
                  LilyCheckedExpr,
                  access,
                  location,
                  ref__LilyCheckedDataType(item->value->data_type),
                  NULL,
                  NEW_VARIANT(
                    LilyCheckedExprAccess,
                    hook,
                    NEW(LilyCheckedExprAccessHook,
                        ref__LilyCheckedExpr(current_expr),
                        NEW_VARIANT(LilyCheckedExpr,
                                    literal,
                                    location,
                                    NEW(LilyCheckedDataType,
                                        LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                        location),
                                    NULL,
                                    NEW_VARIANT(LilyCheckedExprLiteral,
                                                suffix_usize,
                                                item->id)))));
                LilyCheckedExpr *right = to_expr__LilyCheckedPattern(
                  item->value, location, scope, new_current_expr);

                if (right) {
                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      binary,
                      location,
                      ref__LilyCheckedDataType(dt),
                      NULL,
                      NEW(
                        LilyCheckedExprBinary,
                        LILY_CHECKED_EXPR_BINARY_KIND_AND,
                        left,
                        NEW_VARIANT(LilyCheckedExpr,
                                    binary,
                                    location,
                                    ref__LilyCheckedDataType(dt),
                                    NULL,
                                    NEW(LilyCheckedExprBinary,
                                        LILY_CHECKED_EXPR_BINARY_KIND_EQ,
                                        ref__LilyCheckedExpr(new_current_expr),
                                        right))));
                }

                FREE(LilyCheckedExpr, new_current_expr);
            }

            return left;
        }
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            TODO("convert error in expression");
        case LILY_CHECKED_PATTERN_KIND_LIST: {
            ASSERT(current_expr->data_type->kind ==
                   LILY_CHECKED_DATA_TYPE_KIND_LIST);

            LilyCheckedDataType *dt = NEW(
              LilyCheckedDataType, LILY_CHECKED_DATA_TYPE_KIND_BOOL, location);
            LilyCheckedExpr *left = NEW_VARIANT(
              LilyCheckedExpr,
              binary,
              location,
              dt,
              NULL,
              NEW(LilyCheckedExprBinary,
                  self->list.must_eq ? LILY_CHECKED_EXPR_BINARY_KIND_EQ
                                     : LILY_CHECKED_EXPR_BINARY_KIND_GREATER_EQ,
                  ref__LilyCheckedExpr(current_expr),
                  NEW_VARIANT(LilyCheckedExpr,
                              literal,
                              location,
                              NEW(LilyCheckedDataType,
                                  LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                  location),
                              NULL,
                              NEW_VARIANT(LilyCheckedExprLiteral,
                                          suffix_usize,
                                          self->list.len))));

            for (Usize i = 0; i < self->list.table->len; ++i) {
                LilyCheckedPatternTableItem *item =
                  get__Vec(self->list.table, i);
                LilyCheckedExpr *new_current_expr = NEW_VARIANT(
                  LilyCheckedExpr,
                  access,
                  location,
                  ref__LilyCheckedDataType(item->value->data_type),
                  NULL,
                  NEW_VARIANT(
                    LilyCheckedExprAccess,
                    hook,
                    NEW(LilyCheckedExprAccessHook,
                        ref__LilyCheckedExpr(current_expr),
                        NEW_VARIANT(LilyCheckedExpr,
                                    literal,
                                    location,
                                    NEW(LilyCheckedDataType,
                                        LILY_CHECKED_DATA_TYPE_KIND_USIZE,
                                        location),
                                    NULL,
                                    NEW_VARIANT(LilyCheckedExprLiteral,
                                                suffix_usize,
                                                item->id)))));
                LilyCheckedExpr *right = to_expr__LilyCheckedPattern(
                  item->value, location, scope, new_current_expr);

                if (right) {
                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      binary,
                      location,
                      ref__LilyCheckedDataType(dt),
                      NULL,
                      NEW(
                        LilyCheckedExprBinary,
                        LILY_CHECKED_EXPR_BINARY_KIND_AND,
                        left,
                        NEW_VARIANT(LilyCheckedExpr,
                                    binary,
                                    location,
                                    ref__LilyCheckedDataType(dt),
                                    NULL,
                                    NEW(LilyCheckedExprBinary,
                                        LILY_CHECKED_EXPR_BINARY_KIND_EQ,
                                        ref__LilyCheckedExpr(new_current_expr),
                                        right))));
                }

                FREE(LilyCheckedExpr, new_current_expr);
            }

            return left;
        }
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD: {
            // TODO: set start index
            LilyCheckedExpr *left = self->list_head.left
                                      ? to_expr__LilyCheckedPattern(
                                          self, location, scope, current_expr)
                                      : NULL;
            LilyCheckedExpr *right = self->list_head.right
                                       ? to_expr__LilyCheckedPattern(
                                           self, location, scope, current_expr)
                                       : NULL;

            if (left && right) {
                return NEW_VARIANT(LilyCheckedExpr,
                                   binary,
                                   location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                                       location),
                                   NULL,
                                   NEW(LilyCheckedExprBinary,
                                       LILY_CHECKED_EXPR_BINARY_KIND_AND,
                                       left,
                                       right));
            } else if (left) {
                return left;
            } else if (right) {
                return right;
            }

            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                  location),
              NULL,
              NEW_VARIANT(LilyCheckedExprLiteral, bool_, true));
        }
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL: {
            // TODO: set start index
            LilyCheckedExpr *left = self->list_tail.left
                                      ? to_expr__LilyCheckedPattern(
                                          self, location, scope, current_expr)
                                      : NULL;
            LilyCheckedExpr *right = self->list_tail.right
                                       ? to_expr__LilyCheckedPattern(
                                           self, location, scope, current_expr)
                                       : NULL;

            if (left && right) {
                return NEW_VARIANT(LilyCheckedExpr,
                                   binary,
                                   location,
                                   NEW(LilyCheckedDataType,
                                       LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                                       location),
                                   NULL,
                                   NEW(LilyCheckedExprBinary,
                                       LILY_CHECKED_EXPR_BINARY_KIND_AND,
                                       left,
                                       right));
            } else if (left) {
                return left;
            } else if (right) {
                return right;
            }

            return NEW_VARIANT(
              LilyCheckedExpr,
              literal,
              location,
              NEW(LilyCheckedDataType,
                  LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                  location),
              NULL,
              NEW_VARIANT(LilyCheckedExprLiteral, bool_, true));
        }
        case LILY_CHECKED_PATTERN_KIND_LITERAL: {
            LilyCheckedExpr *left = NULL;

            switch (self->literal.kind) {
                case LILY_CHECKED_PATTERN_LITERAL_KIND_BOOL:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_BOOL);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, bool_, self->literal.bool_));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_BYTE:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_BYTE);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, byte, self->literal.byte));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_BYTES:
                    TODO("convert bytes in expression");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_CHAR:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_CHAR);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, char, self->literal.char_));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_CSTR:
                    TODO("convert cstr in expression");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT32:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_FLOAT32);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              float32,
                                              self->literal.float32));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT64:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_FLOAT64);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              float64,
                                              self->literal.float64));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_INT32:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_INT32);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, int32, self->literal.int32));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_INT64:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_INT64);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, int64, self->literal.int64));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_NIL:
                    ASSERT(current_expr->data_type->kind ==
                             LILY_CHECKED_DATA_TYPE_KIND_PTR ||
                           current_expr->data_type->kind ==
                             LILY_CHECKED_DATA_TYPE_KIND_PTR_MUT);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW(LilyCheckedExprLiteral,
                                      LILY_CHECKED_EXPR_LITERAL_KIND_NIL));

                    break;
                case LILY_CHECKED_EXPR_LITERAL_KIND_NONE:
                    TODO("convert none in expression");
                case LILY_CHECKED_EXPR_LITERAL_KIND_STR:
                    TODO("convert str in expression");
                case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_FLOAT32:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_FLOAT32);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_float32,
                                              self->literal.suffix_float32));

                    break;
                case LILY_CHECKED_EXPR_LITERAL_KIND_SUFFIX_FLOAT64:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_FLOAT64);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_float64,
                                              self->literal.suffix_float64));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT8:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_INT8);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_int8,
                                              self->literal.suffix_int8));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT16:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_INT16);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_int16,
                                              self->literal.suffix_int16));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT32:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_INT32);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_int32,
                                              self->literal.suffix_int32));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT64:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_INT64);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_int64,
                                              self->literal.suffix_int64));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_ISIZE:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_ISIZE);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_isize,
                                              self->literal.suffix_isize));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT8:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_UINT8);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_uint8,
                                              self->literal.suffix_uint8));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT16:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_UINT16);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_uint16,
                                              self->literal.suffix_uint16));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT32:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_UINT32);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_uint32,
                                              self->literal.suffix_uint32));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT64:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_UINT64);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_uint64,
                                              self->literal.suffix_uint64));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_USIZE:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_USIZE);

                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW_VARIANT(LilyCheckedExprLiteral,
                                              suffix_usize,
                                              self->literal.suffix_usize));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UINT32:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_UINT32);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, uint32, self->literal.uint32));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UINT64:
                    ASSERT(current_expr->data_type->kind ==
                           LILY_CHECKED_DATA_TYPE_KIND_UINT64);

                    left = NEW_VARIANT(
                      LilyCheckedExpr,
                      literal,
                      location,
                      ref__LilyCheckedDataType(self->data_type),
                      NULL,
                      NEW_VARIANT(
                        LilyCheckedExprLiteral, uint64, self->literal.uint64));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UNDEF:
                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW(LilyCheckedExprLiteral,
                                      LILY_CHECKED_EXPR_LITERAL_KIND_UNDEF));

                    break;
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UNIT:
                    // NOTE: this case is technically impossible
                    left =
                      NEW_VARIANT(LilyCheckedExpr,
                                  literal,
                                  location,
                                  ref__LilyCheckedDataType(self->data_type),
                                  NULL,
                                  NEW(LilyCheckedExprLiteral,
                                      LILY_CHECKED_EXPR_LITERAL_KIND_UNIT));

                    break;
                default:
                    UNREACHABLE("unknown variant");
            }

            return NEW_VARIANT(LilyCheckedExpr,
                               binary,
                               location,
                               NEW(LilyCheckedDataType,
                                   LILY_CHECKED_DATA_TYPE_KIND_BOOL,
                                   location),
                               NULL,
                               NEW(LilyCheckedExprBinary,
                                   LILY_CHECKED_EXPR_BINARY_KIND_EQ,
                                   left,
                                   ref__LilyCheckedExpr(current_expr)));
        }
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            TODO("convert range in expression");
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            TODO("convert record call in expression");
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            TODO("convert tuple in expression");
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            return NULL;
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            TODO("convert variant call in expression");
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedPattern, const LilyCheckedPattern *self)
{
    String *res =
      format__String("LilyCheckedPattern{{ kind = {s}, location = {sa}, "
                     "data_type = {Sr}, ast_pattern = {Sr}",
                     to_string__Debug__LilyCheckedPatternKind(self->kind),
                     to_string__Debug__Location(self->location),
                     to_string__Debug__LilyCheckedDataType(self->data_type),
                     to_string__Debug__LilyAstPattern(self->ast_pattern));

    switch (self->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY: {
            char *s =
              format(", array = {Sr} }",
                     to_string__Debug__LilyCheckedPatternArray(&self->array));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_NONE:
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            push_str__String(res, " }");
            break;
        case LILY_CHECKED_PATTERN_KIND_ERROR: {
            char *s =
              format(", error = {Sr} }",
                     to_string__Debug__LilyCheckedPatternError(&self->error));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_LIST: {
            char *s =
              format(", list = {Sr} }",
                     to_string__Debug__LilyCheckedPatternList(&self->list));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD: {
            char *s = format(
              ", list_head = {Sr} }",
              to_string__Debug__LilyCheckedPatternListHead(&self->list_head));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL: {
            char *s = format(
              ", list_tail = {Sr} }",
              to_string__Debug__LilyCheckedPatternListTail(&self->list_tail));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_LITERAL: {
            char *s = format(
              ", literal = {Sr} }",
              to_string__Debug__LilyCheckedPatternLiteral(&self->literal));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_RANGE: {
            char *s =
              format(", range = {Sr} }",
                     to_string__Debug__LilyCheckedPatternRange(&self->range));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL: {
            char *s = format(", record_call = {Sr} }",
                             to_string__Debug__LilyCheckedPatternRecordCall(
                               &self->record_call));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_TUPLE: {
            char *s =
              format(", tuple = {Sr} }",
                     to_string__Debug__LilyCheckedPatternTuple(&self->tuple));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL: {
            char *s = format(", variant_call = {Sr} }",
                             to_string__Debug__LilyCheckedPatternVariantCall(
                               &self->variant_call));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedPattern, array, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternArray, &self->array);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, error, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternError, &self->error);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, list, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternList, &self->list);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, list_head, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternListHead, &self->list_head);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, list_tail, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternListTail, &self->list_tail);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, literal, LilyCheckedPattern *self)
{
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, range, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternRange, &self->range);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, record_call, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternRecordCall, &self->record_call);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, tuple, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternTuple, &self->tuple);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedPattern, variant_call, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternVariantCall, &self->variant_call);
    FREE(LilyCheckedDataType, self->data_type);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedPattern, LilyCheckedPattern *self)
{
    switch (self->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            FREE_VARIANT(LilyCheckedPattern, array, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_NONE:
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            FREE(LilyCheckedDataType, self->data_type);
            lily_free(self);
            break;
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            FREE_VARIANT(LilyCheckedPattern, error, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_LIST:
            FREE_VARIANT(LilyCheckedPattern, list, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            FREE_VARIANT(LilyCheckedPattern, list_head, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            FREE_VARIANT(LilyCheckedPattern, list_tail, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            FREE_VARIANT(LilyCheckedPattern, literal, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            FREE_VARIANT(LilyCheckedPattern, range, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            FREE_VARIANT(LilyCheckedPattern, record_call, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            FREE_VARIANT(LilyCheckedPattern, tuple, self);
            break;
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            FREE_VARIANT(LilyCheckedPattern, variant_call, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
