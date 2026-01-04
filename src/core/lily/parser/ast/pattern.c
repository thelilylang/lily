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

#include <core/lily/parser/ast/pattern.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_ARRAY).
static VARIANT_DESTRUCTOR(LilyAstPattern, array, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_AS).
static VARIANT_DESTRUCTOR(LilyAstPattern, as, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_ERROR).
static VARIANT_DESTRUCTOR(LilyAstPattern, error, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_LIST).
static VARIANT_DESTRUCTOR(LilyAstPattern, list, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_LIST_HEAD).
static VARIANT_DESTRUCTOR(LilyAstPattern, list_head, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_LIST_TAIL).
static VARIANT_DESTRUCTOR(LilyAstPattern, list_tail, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_LITERAL).
static VARIANT_DESTRUCTOR(LilyAstPattern, literal, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_NAME).
static VARIANT_DESTRUCTOR(LilyAstPattern, name, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_RANGE).
static VARIANT_DESTRUCTOR(LilyAstPattern, range, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_RECORD_CALL).
static VARIANT_DESTRUCTOR(LilyAstPattern, record_call, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_TUPLE).
static VARIANT_DESTRUCTOR(LilyAstPattern, tuple, LilyAstPattern *self);

// Free LilyAstPattern type (LILY_AST_PATTERN_KIND_VARIANT_CALL).
static VARIANT_DESTRUCTOR(LilyAstPattern, variant_call, LilyAstPattern *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstPatternKind, enum LilyAstPatternKind self)
{
    switch (self) {
        case LILY_AST_PATTERN_KIND_ARRAY:
            return "LILY_AST_PATTERN_KIND_ARRAY";
        case LILY_AST_PATTERN_KIND_AS:
            return "LILY_AST_PATTERN_KIND_AS";
        case LILY_AST_PATTERN_KIND_AUTO_COMPLETE:
            return "LILY_AST_PATTERN_KIND_AUTO_COMPLETE";
        case LILY_AST_PATTERN_KIND_ERROR:
            return "LILY_AST_PATTERN_KIND_ERROR";
        case LILY_AST_PATTERN_KIND_LIST:
            return "LILY_AST_PATTERN_KIND_LIST";
        case LILY_AST_PATTERN_KIND_LIST_HEAD:
            return "LILY_AST_PATTERN_KIND_LIST_HEAD";
        case LILY_AST_PATTERN_KIND_LIST_TAIL:
            return "LILY_AST_PATTERN_KIND_LIST_TAIL";
        case LILY_AST_PATTERN_KIND_LITERAL:
            return "LILY_AST_PATTERN_KIND_LITERAL";
        case LILY_AST_PATTERN_KIND_NAME:
            return "LILY_AST_PATTERN_KIND_NAME";
        case LILY_AST_PATTERN_KIND_RANGE:
            return "LILY_AST_PATTERN_KIND_RANGE";
        case LILY_AST_PATTERN_KIND_RECORD_CALL:
            return "LILY_AST_PATTERN_KIND_RECORD_CALL";
        case LILY_AST_PATTERN_KIND_TUPLE:
            return "LILY_AST_PATTERN_KIND_TUPLE";
        case LILY_AST_PATTERN_KIND_VARIANT_CALL:
            return "LILY_AST_PATTERN_KIND_VARIANT_CALL";
        case LILY_AST_PATTERN_KIND_WILDCARD:
            return "LILY_AST_PATTERN_KIND_WILDCARD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    array,
                    Location location,
                    LilyAstPatternArray array)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_ARRAY;
    self->location = location;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    as,
                    Location location,
                    LilyAstPatternAs as)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_AS;
    self->location = location;
    self->as = as;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    error,
                    Location location,
                    LilyAstPatternError error)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_ERROR;
    self->location = location;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    list,
                    Location location,
                    LilyAstPatternList list)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_LIST;
    self->location = location;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    list_head,
                    Location location,
                    LilyAstPatternListHead list_head)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_LIST_HEAD;
    self->location = location;
    self->list_head = list_head;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    list_tail,
                    Location location,
                    LilyAstPatternListTail list_tail)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_LIST_TAIL;
    self->location = location;
    self->list_tail = list_tail;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    literal,
                    Location location,
                    LilyAstPatternLiteral literal)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_LITERAL;
    self->location = location;
    self->literal = literal;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    name,
                    Location location,
                    LilyAstPatternName name)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_NAME;
    self->location = location;
    self->name = name;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    range,
                    Location location,
                    LilyAstPatternRange range)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_RANGE;
    self->location = location;
    self->range = range;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    record_call,
                    Location location,
                    LilyAstPatternRecordCall record_call)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_RECORD_CALL;
    self->location = location;
    self->record_call = record_call;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    tuple,
                    Location location,
                    LilyAstPatternTuple tuple)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_TUPLE;
    self->location = location;
    self->tuple = tuple;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    variant_call,
                    Location location,
                    LilyAstPatternVariantCall variant_call)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = LILY_AST_PATTERN_KIND_VARIANT_CALL;
    self->location = location;
    self->variant_call = variant_call;

    return self;
}

CONSTRUCTOR(LilyAstPattern *,
            LilyAstPattern,
            Location location,
            enum LilyAstPatternKind kind)
{
    LilyAstPattern *self = lily_malloc(sizeof(LilyAstPattern));

    self->kind = kind;
    self->location = location;

    return self;
}

const LilyAstPattern *
get_name__LilyAstPattern(const LilyAstPattern *self)
{
    switch (self->kind) {
        case LILY_AST_PATTERN_KIND_ARRAY:
        case LILY_AST_PATTERN_KIND_AUTO_COMPLETE:
        case LILY_AST_PATTERN_KIND_ERROR:
        case LILY_AST_PATTERN_KIND_LIST:
        case LILY_AST_PATTERN_KIND_LIST_HEAD:
        case LILY_AST_PATTERN_KIND_LIST_TAIL:
        case LILY_AST_PATTERN_KIND_LITERAL:
        case LILY_AST_PATTERN_KIND_RANGE:
        case LILY_AST_PATTERN_KIND_RECORD_CALL:
        case LILY_AST_PATTERN_KIND_TUPLE:
        case LILY_AST_PATTERN_KIND_VARIANT_CALL:
        case LILY_AST_PATTERN_KIND_WILDCARD:
            return NULL;
        case LILY_AST_PATTERN_KIND_AS:
            return get_name__LilyAstPattern(self->as.pattern);
        case LILY_AST_PATTERN_KIND_NAME:
            return self;
        default:
            UNREACHABLE("unknown variant");
    }
}

bool
is_else_pattern__LilyAstPattern(const LilyAstPattern *self)
{
    ASSERT(self);

    switch (self->kind) {
        case LILY_AST_PATTERN_KIND_ARRAY:
            for (Usize i = 0; i < self->array.patterns->len; ++i) {
                if (!is_else_pattern__LilyAstPattern(
                      get__Vec(self->array.patterns, i))) {
                    return false;
                }
            }

            return true;
        case LILY_AST_PATTERN_KIND_AS:
            return is_else_pattern__LilyAstPattern(self->as.pattern);
        case LILY_AST_PATTERN_KIND_ERROR:
        case LILY_AST_PATTERN_KIND_LITERAL:
            return false;
        case LILY_AST_PATTERN_KIND_LIST:
            for (Usize i = 0; i < self->list.patterns->len; ++i) {
                if (!is_else_pattern__LilyAstPattern(
                      get__Vec(self->list.patterns, i))) {
                    return false;
                }
            }

            return true;
        case LILY_AST_PATTERN_KIND_LIST_HEAD:
            return is_else_pattern__LilyAstPattern(self->list_head.left) &&
                   is_else_pattern__LilyAstPattern(self->list_head.right);
        case LILY_AST_PATTERN_KIND_LIST_TAIL:
            return is_else_pattern__LilyAstPattern(self->list_tail.left) &&
                   is_else_pattern__LilyAstPattern(self->list_tail.right);
        case LILY_AST_PATTERN_KIND_RANGE:
            return is_else_pattern__LilyAstPattern(self->range.left) &&
                   is_else_pattern__LilyAstPattern(self->range.right);
        case LILY_AST_PATTERN_KIND_RECORD_CALL:
            for (Usize i = 0; i < self->record_call.fields->len; ++i) {
                LilyAstPatternRecordField *field =
                  get__Vec(self->record_call.fields, i);

                if (!is_else_pattern__LilyAstPattern(field->pattern)) {
                    return false;
                }
            }

            return true;
        case LILY_AST_PATTERN_KIND_TUPLE:
            for (Usize i = 0; i < self->tuple.patterns->len; ++i) {
                if (!is_else_pattern__LilyAstPattern(
                      get__Vec(self->tuple.patterns, i))) {
                    return false;
                }
            }

            return true;
        case LILY_AST_PATTERN_KIND_VARIANT_CALL:
            return self->variant_call.pattern ? is_else_pattern__LilyAstPattern(
                                                  self->variant_call.pattern)
                                              : true;
        case LILY_AST_PATTERN_KIND_NAME:
        case LILY_AST_PATTERN_KIND_WILDCARD:
        case LILY_AST_PATTERN_KIND_AUTO_COMPLETE:
            return true;
    }
}

bool
is_final_else_pattern__LilyAstPattern(const LilyAstPattern *self)
{
    ASSERT(self);

    switch (self->kind) {
        case LILY_AST_PATTERN_KIND_NAME:
        case LILY_AST_PATTERN_KIND_WILDCARD:
            return true;
        default:
            return false;
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstPattern, const LilyAstPattern *self)
{
    String *res =
      format__String("LilyAstPattern{{ kind = {s}, location = {sa}",
                     to_string__Debug__LilyAstPatternKind(self->kind),
                     to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_AST_PATTERN_KIND_ARRAY: {
            char *s =
              format(", array = {Sr} }",
                     to_string__Debug__LilyAstPatternArray(&self->array));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_AS: {
            char *s = format(", as = {Sr} }",
                             to_string__Debug__LilyAstPatternAs(&self->as));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_AUTO_COMPLETE:
        case LILY_AST_PATTERN_KIND_WILDCARD:
            push_str__String(res, " }");
            break;
        case LILY_AST_PATTERN_KIND_ERROR: {
            char *s =
              format(", error = {Sr} }",
                     to_string__Debug__LilyAstPatternError(&self->error));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_LIST: {
            char *s = format(", list = {Sr} }",
                             to_string__Debug__LilyAstPatternList(&self->list));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_LIST_HEAD: {
            char *s = format(
              ", list_head = {Sr} }",
              to_string__Debug__LilyAstPatternListHead(&self->list_head));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_LIST_TAIL: {
            char *s = format(
              ", list_tail = {Sr} }",
              to_string__Debug__LilyAstPatternListTail(&self->list_tail));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_LITERAL: {
            char *s =
              format(", literal = {Sr} }",
                     to_string__Debug__LilyAstPatternLiteral(&self->literal));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_NAME: {
            char *s = format(", name = {Sr} }",
                             to_string__Debug__LilyAstPatternName(&self->name));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_RANGE: {
            char *s =
              format(", range = {Sr} }",
                     to_string__Debug__LilyAstPatternRange(&self->range));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_RECORD_CALL: {
            char *s = format(
              ", record_call = {Sr} }",
              to_string__Debug__LilyAstPatternRecordCall(&self->record_call));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_TUPLE: {
            char *s =
              format(", tuple = {Sr} }",
                     to_string__Debug__LilyAstPatternTuple(&self->tuple));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_AST_PATTERN_KIND_VARIANT_CALL: {
            char *s = format(
              ", variant_call = {Sr} }",
              to_string__Debug__LilyAstPatternVariantCall(&self->variant_call));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

String *
to_string__LilyAstPattern(const LilyAstPattern *self)
{
    TODO("implements");
}

VARIANT_DESTRUCTOR(LilyAstPattern, array, LilyAstPattern *self)
{
    FREE(LilyAstPatternArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, as, LilyAstPattern *self)
{
    FREE(LilyAstPatternAs, &self->as);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, error, LilyAstPattern *self)
{
    FREE(LilyAstPatternError, &self->error);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, list, LilyAstPattern *self)
{
    FREE(LilyAstPatternList, &self->list);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, list_head, LilyAstPattern *self)
{
    FREE(LilyAstPatternListHead, &self->list_head);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, list_tail, LilyAstPattern *self)
{
    FREE(LilyAstPatternListTail, &self->list_tail);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, literal, LilyAstPattern *self)
{
    FREE(LilyAstPatternLiteral, &self->literal);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, name, LilyAstPattern *self)
{
    FREE(LilyAstPatternName, &self->name);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, range, LilyAstPattern *self)
{
    FREE(LilyAstPatternRange, &self->range);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, record_call, LilyAstPattern *self)
{
    FREE(LilyAstPatternRecordCall, &self->record_call);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, tuple, LilyAstPattern *self)
{
    FREE(LilyAstPatternTuple, &self->tuple);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstPattern, variant_call, LilyAstPattern *self)
{
    FREE(LilyAstPatternVariantCall, &self->variant_call);
    lily_free(self);
}

DESTRUCTOR(LilyAstPattern, LilyAstPattern *self)
{
    switch (self->kind) {
        case LILY_AST_PATTERN_KIND_ARRAY:
            FREE_VARIANT(LilyAstPattern, array, self);
            break;
        case LILY_AST_PATTERN_KIND_AS:
            FREE_VARIANT(LilyAstPattern, as, self);
            break;
        case LILY_AST_PATTERN_KIND_AUTO_COMPLETE:
        case LILY_AST_PATTERN_KIND_WILDCARD:
            lily_free(self);
            break;
        case LILY_AST_PATTERN_KIND_ERROR:
            FREE_VARIANT(LilyAstPattern, error, self);
            break;
        case LILY_AST_PATTERN_KIND_LIST:
            FREE_VARIANT(LilyAstPattern, list, self);
            break;
        case LILY_AST_PATTERN_KIND_LIST_HEAD:
            FREE_VARIANT(LilyAstPattern, list_head, self);
            break;
        case LILY_AST_PATTERN_KIND_LIST_TAIL:
            FREE_VARIANT(LilyAstPattern, list_tail, self);
            break;
        case LILY_AST_PATTERN_KIND_LITERAL:
            FREE_VARIANT(LilyAstPattern, literal, self);
            break;
        case LILY_AST_PATTERN_KIND_NAME:
            FREE_VARIANT(LilyAstPattern, name, self);
            break;
        case LILY_AST_PATTERN_KIND_RANGE:
            FREE_VARIANT(LilyAstPattern, range, self);
            break;
        case LILY_AST_PATTERN_KIND_RECORD_CALL:
            FREE_VARIANT(LilyAstPattern, record_call, self);
            break;
        case LILY_AST_PATTERN_KIND_TUPLE:
            FREE_VARIANT(LilyAstPattern, tuple, self);
            break;
        case LILY_AST_PATTERN_KIND_VARIANT_CALL:
            FREE_VARIANT(LilyAstPattern, variant_call, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
