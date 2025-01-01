/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_CORE_LILY_PARSER_AST_PATTERN_H
#define LILY_CORE_LILY_PARSER_AST_PATTERN_H

#include <core/lily/parser/ast/pattern/array.h>
#include <core/lily/parser/ast/pattern/as.h>
#include <core/lily/parser/ast/pattern/error.h>
#include <core/lily/parser/ast/pattern/list.h>
#include <core/lily/parser/ast/pattern/list_head.h>
#include <core/lily/parser/ast/pattern/list_tail.h>
#include <core/lily/parser/ast/pattern/literal.h>
#include <core/lily/parser/ast/pattern/name.h>
#include <core/lily/parser/ast/pattern/range.h>
#include <core/lily/parser/ast/pattern/record_call.h>
#include <core/lily/parser/ast/pattern/tuple.h>
#include <core/lily/parser/ast/pattern/variant_call.h>

#include <core/shared/location.h>

enum LilyAstPatternKind
{
    LILY_AST_PATTERN_KIND_ARRAY,
    LILY_AST_PATTERN_KIND_AS,
    LILY_AST_PATTERN_KIND_AUTO_COMPLETE,
    LILY_AST_PATTERN_KIND_ERROR,
    LILY_AST_PATTERN_KIND_LIST,
    LILY_AST_PATTERN_KIND_LIST_HEAD,
    LILY_AST_PATTERN_KIND_LIST_TAIL,
    LILY_AST_PATTERN_KIND_LITERAL,
    LILY_AST_PATTERN_KIND_NAME,
    LILY_AST_PATTERN_KIND_RANGE,
    LILY_AST_PATTERN_KIND_RECORD_CALL,
    LILY_AST_PATTERN_KIND_TUPLE,
    LILY_AST_PATTERN_KIND_VARIANT_CALL,
    LILY_AST_PATTERN_KIND_WILDCARD
};

/**
 *
 * @brief Convert LilyAstPatternKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyAstPatternKind, enum LilyAstPatternKind self);
#endif

typedef struct LilyAstPattern
{
    enum LilyAstPatternKind kind;
    Location location;
    union
    {
        LilyAstPatternArray array;
        LilyAstPatternAs as;
        LilyAstPatternError error;
        LilyAstPatternList list;
        LilyAstPatternListHead list_head;
        LilyAstPatternListTail list_tail;
        LilyAstPatternLiteral literal;
        LilyAstPatternName name;
        LilyAstPatternRange range;
        LilyAstPatternRecordCall record_call;
        LilyAstPatternTuple tuple;
        LilyAstPatternVariantCall variant_call;
    };
} LilyAstPattern;

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    array,
                    Location location,
                    LilyAstPatternArray array);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_AS).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    as,
                    Location location,
                    LilyAstPatternAs as);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    error,
                    Location location,
                    LilyAstPatternError error);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    list,
                    Location location,
                    LilyAstPatternList list);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_LIST_HEAD).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    list_head,
                    Location location,
                    LilyAstPatternListHead list_head);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_LIST_TAIL).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    list_tail,
                    Location location,
                    LilyAstPatternListTail list_tail);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_LITERAL).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    literal,
                    Location location,
                    LilyAstPatternLiteral literal);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_NAME).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    name,
                    Location location,
                    LilyAstPatternName name);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_RANGE).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    range,
                    Location location,
                    LilyAstPatternRange range);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_RECORD_CALL).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    record_call,
                    Location location,
                    LilyAstPatternRecordCall record_call);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    tuple,
                    Location location,
                    LilyAstPatternTuple tuple);

/**
 *
 * @brief Construct LilyAstPattern type (LILY_AST_PATTERN_KIND_VARIANT_CALL).
 */
VARIANT_CONSTRUCTOR(LilyAstPattern *,
                    LilyAstPattern,
                    variant_call,
                    Location location,
                    LilyAstPatternVariantCall variant_call);

/**
 *
 * @brief Construct LilyAstPattern type.
 */
CONSTRUCTOR(LilyAstPattern *,
            LilyAstPattern,
            Location location,
            enum LilyAstPatternKind kind);

/**
 *
 * @brief Get name from pattern.
 */
const LilyAstPattern *
get_name__LilyAstPattern(const LilyAstPattern *self);

/**
 *
 * @brief Is else pattern.
 */
bool
is_else_pattern__LilyAstPattern(const LilyAstPattern *self);

/**
 *
 * @brief Is the last else pattern.
 */
bool
is_final_else_pattern__LilyAstPattern(const LilyAstPattern *self);

/**
 *
 * @brief Convert LilyAstPattern in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstPattern, const LilyAstPattern *self);
#endif

/**
 *
 * @brief Convert LilyAstPattern in String.
 */
String *
to_string__LilyAstPattern(const LilyAstPattern *self);

/**
 *
 * @brief Free LilyAstPattern type.
 */
DESTRUCTOR(LilyAstPattern, LilyAstPattern *self);

#endif // LILY_CORE_LILY_PARSER_AST_PATTERN_H
