/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#include <core/lily/analysis/checked/decl/enum.h>
#include <core/lily/analysis/checked/decl/enum_object.h>
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

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_AS).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, as, LilyCheckedPattern *self);

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

// Free LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_NAME).
static VARIANT_DESTRUCTOR(LilyCheckedPattern, name, LilyCheckedPattern *self);

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
                    as,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternAs as)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_AS;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->as = as;

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
                    name,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternName name)
{
    LilyCheckedPattern *self = lily_malloc(sizeof(LilyCheckedPattern));

    self->kind = LILY_CHECKED_PATTERN_KIND_NAME;
    self->location = location;
    self->data_type = data_type;
    self->ast_pattern = ast_pattern;
    self->name = name;

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
        case LILY_CHECKED_PATTERN_KIND_AS:
            return eq__LilyCheckedPatternAs(&self->as, &other->as);
        case LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE:
            return true;
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
        case LILY_CHECKED_PATTERN_KIND_NAME:
            return eq__LilyCheckedPatternName(&self->name, &other->name);
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
        case LILY_CHECKED_PATTERN_KIND_AS:
            break;
        case LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE:
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
        case LILY_CHECKED_PATTERN_KIND_NAME:
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

    return false;
}

LilyCheckedStmtSwitchCaseValue *
to_switch_case_value__LilyCheckedPattern(const LilyCheckedPattern *self,
                                         LilyCheckedScope *scope)
{
    switch (self->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            UNREACHABLE("impossible to convert array in switch case value");
        case LILY_CHECKED_PATTERN_KIND_AS:
            UNREACHABLE(
              "as pattern is not expected to convert in switch case value");
        case LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE:
            UNREACHABLE("auto complete pattern is not expected to convert in "
                        "switch case value");
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            TODO("convert error in switch case value");
        case LILY_CHECKED_PATTERN_KIND_LIST:
            UNREACHABLE("impossible to convert list in switch case value");
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            UNREACHABLE("impossible to convert list head in switch case value");
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            UNREACHABLE("impossible to convert list tail in switch case value");
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            switch (self->literal.kind) {
                case LILY_CHECKED_PATTERN_LITERAL_KIND_BOOL:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       bool,
                                       self->literal.bool_);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_BYTE:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint8,
                                       self->literal.byte);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_BYTES:
                    UNREACHABLE(
                      "impossible to convert bytes in switch case value");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_CHAR:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint8,
                                       self->literal.char_);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_CSTR:
                    UNREACHABLE(
                      "impossible to convert cstr in switch case value");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT32:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       float32,
                                       self->literal.float32);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_FLOAT64:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       float64,
                                       self->literal.float64);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_INT32:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       int32,
                                       self->literal.int32);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_INT64:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       int64,
                                       self->literal.int64);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_NIL:
                    UNREACHABLE(
                      "impossible to convert nil in switch case value");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_NONE:
                    UNREACHABLE(
                      "impossible to convert none in switch case value");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_STR:
                    UNREACHABLE(
                      "impossible to convert str in switch case value");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT32:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       float32,
                                       self->literal.suffix_float32);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_FLOAT64:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       float64,
                                       self->literal.suffix_float64);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT8:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       int8,
                                       self->literal.suffix_int8);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT16:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       int16,
                                       self->literal.suffix_int16);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT32:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       int32,
                                       self->literal.suffix_int32);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_INT64:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       int64,
                                       self->literal.suffix_int64);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_ISIZE:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       isize,
                                       self->literal.suffix_isize);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT8:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint8,
                                       self->literal.suffix_uint8);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT16:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint16,
                                       self->literal.suffix_uint16);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT32:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint32,
                                       self->literal.suffix_uint32);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_UINT64:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint64,
                                       self->literal.suffix_uint64);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_SUFFIX_USIZE:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       usize,
                                       self->literal.suffix_usize);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UINT32:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint32,
                                       self->literal.uint32);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UINT64:
                    return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue,
                                       uint64,
                                       self->literal.uint64);
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UNDEF:
                    UNREACHABLE(
                      "impossible to convert undef in switch case value");
                case LILY_CHECKED_PATTERN_LITERAL_KIND_UNIT:
                    UNREACHABLE(
                      "impossible to convert unit in switch case value");
                default:
                    UNREACHABLE("unknown variant");
            }
        case LILY_CHECKED_PATTERN_KIND_NAME:
            UNREACHABLE(
              "name pattern is not expected to convert in switch case value");
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            // NOTE: for the moment we can only use range with integer typing
            // pattern.
            TODO("convert range in switch case value");
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            UNREACHABLE(
              "impossible to convert record call in switch case value");
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            UNREACHABLE("impossible to convert tuple in switch case value");
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            UNREACHABLE("impossible to convert unknown in switch case value");
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL: {
            ASSERT(self->variant_call.id->kind == LILY_CHECKED_EXPR_KIND_CALL);

            String *name =
              get_last_name__LilyAstExpr(self->variant_call.id->ast_expr);

            ASSERT(name);

            // TODO: add support for enum object variant
            LilyCheckedScopeResponse response =
              search_from_scope_id__LilyCheckedScope(
                scope,
                self->variant_call.id->call.scope.id,
                name,
                LILY_CHECKED_SCOPE_RESPONSE_KIND_ENUM_VARIANT);

            ASSERT(response.kind != LILY_CHECKED_SCOPE_RESPONSE_KIND_NOT_FOUND);

            // TODO: use the uint switch case value if applicable (in the case
            // of a preset value of the enum variant).
            return NEW_VARIANT(
              LilyCheckedStmtSwitchCaseValue, uint8, response.enum_variant->id);
        }
        case LILY_CHECKED_PATTERN_KIND_ELSE:
            return NEW_VARIANT(LilyCheckedStmtSwitchCaseValue, else);
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
        case LILY_CHECKED_PATTERN_KIND_AS: {
            char *s = format(", as = {Sr} }",
                             to_string__Debug__LilyCheckedPatternAs(&self->as));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE:
        case LILY_CHECKED_PATTERN_KIND_ELSE:
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
        case LILY_CHECKED_PATTERN_KIND_NAME: {
            char *s =
              format(", name = {Sr} }",
                     to_string__Debug__LilyCheckedPatternName(&self->name));

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

VARIANT_DESTRUCTOR(LilyCheckedPattern, as, LilyCheckedPattern *self)
{
    FREE(LilyCheckedPatternAs, &self->as);
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

VARIANT_DESTRUCTOR(LilyCheckedPattern, name, LilyCheckedPattern *self)
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
        case LILY_CHECKED_PATTERN_KIND_AS:
            FREE_VARIANT(LilyCheckedPattern, as, self);
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
        case LILY_CHECKED_PATTERN_KIND_NAME:
            FREE_VARIANT(LilyCheckedPattern, name, self);
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
