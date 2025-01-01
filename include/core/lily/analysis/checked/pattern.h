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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_H

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/pattern/array.h>
#include <core/lily/analysis/checked/pattern/as.h>
#include <core/lily/analysis/checked/pattern/error.h>
#include <core/lily/analysis/checked/pattern/list.h>
#include <core/lily/analysis/checked/pattern/list_head.h>
#include <core/lily/analysis/checked/pattern/list_tail.h>
#include <core/lily/analysis/checked/pattern/literal.h>
#include <core/lily/analysis/checked/pattern/name.h>
#include <core/lily/analysis/checked/pattern/range.h>
#include <core/lily/analysis/checked/pattern/record_call.h>
#include <core/lily/analysis/checked/pattern/tuple.h>
#include <core/lily/analysis/checked/pattern/variant_call.h>
#include <core/lily/analysis/checked/stmt/switch.h>
#include <core/lily/parser/ast/pattern.h>

#include <core/shared/location.h>

typedef struct LilyAnalysis LilyAnalysis;

enum LilyCheckedPatternKind
{
    LILY_CHECKED_PATTERN_KIND_ARRAY,
    // NOTE: only used when `use_switch` is false
    LILY_CHECKED_PATTERN_KIND_AS,
    // NOTE: only used when `use_switch` is false
    LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE,
    LILY_CHECKED_PATTERN_KIND_ELSE,
    LILY_CHECKED_PATTERN_KIND_ERROR,
    LILY_CHECKED_PATTERN_KIND_LIST,
    LILY_CHECKED_PATTERN_KIND_LIST_HEAD,
    LILY_CHECKED_PATTERN_KIND_LIST_TAIL,
    LILY_CHECKED_PATTERN_KIND_LITERAL,
    // NOTE: only used when `use_switch` is false
    LILY_CHECKED_PATTERN_KIND_NAME,
    LILY_CHECKED_PATTERN_KIND_NONE, // TODO: remove this variant
    LILY_CHECKED_PATTERN_KIND_RANGE,
    LILY_CHECKED_PATTERN_KIND_RECORD_CALL,
    LILY_CHECKED_PATTERN_KIND_TUPLE,
    LILY_CHECKED_PATTERN_KIND_UNKNOWN,
    LILY_CHECKED_PATTERN_KIND_VARIANT_CALL,
};

/**
 *
 * @brief Convert LilyCheckedPatternKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternKind,
               enum LilyCheckedPatternKind self);
#endif

typedef struct LilyCheckedPattern
{
    enum LilyCheckedPatternKind kind;
    const Location *location; // const Location* (&)
    LilyCheckedDataType
      *data_type; // This is the result data type of the pattern.
    const LilyAstPattern *ast_pattern; // const LilyAstPattern* (&)
    union
    {
        LilyCheckedPatternArray array;
        LilyCheckedPatternAs as;
        LilyCheckedPatternError error;
        LilyCheckedPatternList list;
        LilyCheckedPatternListHead list_head;
        LilyCheckedPatternListTail list_tail;
        LilyCheckedPatternLiteral literal;
        LilyCheckedPatternName name;
        LilyCheckedPatternRange range;
        LilyCheckedPatternRecordCall record_call;
        LilyCheckedPatternTuple tuple;
        LilyCheckedPatternVariantCall variant_call;
    };
} LilyCheckedPattern;

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_ARRAY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    array,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternArray array);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_AS).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    as,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternAs as);

/**
 *
 * @brief Construct LilyCheckedPattern type
 * (LILY_CHECKED_PATTERN_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    error,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternError error);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_LIST).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    list,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternList list);

/**
 *
 * @brief Construct LilyCheckedPattern type
 * (LILY_CHECKED_PATTERN_KIND_LIST_HEAD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    list_head,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternListHead list_head);

/**
 *
 * @brief Construct LilyCheckedPattern type
 * (LILY_CHECKED_PATTERN_KIND_LIST_TAIL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    list_tail,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternListTail list_tail);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_LITERAL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    literal,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternLiteral literal);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_NAME).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    name,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternName name);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_RANGE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    range,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternRange range);

/**
 *
 * @brief Construct LilyCheckedPattern type
 * (LILY_CHECKED_PATTERN_KIND_RECORD_CALL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    record_call,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternRecordCall record_call);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_TUPLE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    tuple,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternTuple tuple);

/**
 *
 * @brief Construct LilyCheckedPattern type (LILY_CHECKED_PATTERN_KIND_UNKNOWN).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    unknown,
                    const Location *location,
                    const LilyAstPattern *ast_pattern);

/**
 *
 * @brief Construct LilyCheckedPattern type
 * (LILY_CHECKED_PATTERN_KIND_VARIANT_CALL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedPattern *,
                    LilyCheckedPattern,
                    variant_call,
                    const Location *location,
                    LilyCheckedDataType *data_type,
                    const LilyAstPattern *ast_pattern,
                    LilyCheckedPatternVariantCall variant_call);

/**
 *
 * @brief Construct LilyCheckedPattern type.
 */
CONSTRUCTOR(LilyCheckedPattern *,
            LilyCheckedPattern,
            enum LilyCheckedPatternKind kind,
            const Location *location,
            LilyCheckedDataType *data_type,
            const LilyAstPattern *ast_pattern);

/**
 *
 * @brief Check if the both pattern are equal.
 */
bool
eq__LilyCheckedPattern(const LilyCheckedPattern *self,
                       const LilyCheckedPattern *other);

/**
 *
 * @brief Check if the body pattern are equal (lazy).
 */
bool
lazy_eq__LilyCheckedPattern(const LilyCheckedPattern *self,
                            const LilyCheckedPattern *other);

/**
 *
 * @brief Convert a pattern to an expression.
 * @param variable_name Name of a variable or a virtual variable.
 */
LilyCheckedExpr *
to_expr__LilyCheckedPattern(const LilyCheckedPattern *self,
                            LilyAnalysis *analysis,
                            const Location *location,
                            LilyCheckedScope *scope,
                            LilyCheckedExpr *current_expr);

/**
 *
 * @brief Convert pattern in switch case value.
 */
LilyCheckedStmtSwitchCaseValue *
to_switch_case_value__LilyCheckedPattern(const LilyCheckedPattern *self,
                                         LilyCheckedScope *scope);

/**
 *
 * @brief Convert LilyCheckedPattern in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedPattern, const LilyCheckedPattern *self);
#endif

/**
 *
 * @brief Free LilyCheckedPattern type.
 */
DESTRUCTOR(LilyCheckedPattern, LilyCheckedPattern *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_PATTERN_H
