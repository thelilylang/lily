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

#ifndef LILY_CORE_LILY_CHECKED_PATTERN_H
#define LILY_CORE_LILY_CHECKED_PATTERN_H

#include <core/lily/checked/pattern/array.h>
#include <core/lily/checked/pattern/as.h>
#include <core/lily/checked/pattern/exception.h>
#include <core/lily/checked/pattern/list.h>
#include <core/lily/checked/pattern/list_head.h>
#include <core/lily/checked/pattern/list_tail.h>
#include <core/lily/checked/pattern/literal.h>
#include <core/lily/checked/pattern/name.h>
#include <core/lily/checked/pattern/range.h>
#include <core/lily/checked/pattern/record_call.h>
#include <core/lily/checked/pattern/tuple.h>
#include <core/lily/checked/pattern/variant_call.h>

#include <core/shared/location.h>

enum LilyCheckedPatternKind
{
    LILY_CHECKED_PATTERN_KIND_ARRAY,
    LILY_CHECKED_PATTERN_KIND_AS,
    LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE,
    LILY_CHECKED_PATTERN_KIND_EXCEPTION,
    LILY_CHECKED_PATTERN_KIND_LIST,
    LILY_CHECKED_PATTERN_KIND_LIST_HEAD,
    LILY_CHECKED_PATTERN_KIND_LIST_TAIL,
    LILY_CHECKED_PATTERN_KIND_LITERAL,
    LILY_CHECKED_PATTERN_KIND_NAME,
    LILY_CHECKED_PATTERN_KIND_RANGE,
    LILY_CHECKED_PATTERN_KIND_RECORD_CALL,
    LILY_CHECKED_PATTERN_KIND_TUPLE,
    LILY_CHECKED_PATTERN_KIND_VARIANT_CALL,
    LILY_CHECKED_PATTERN_KIND_WILDCARD
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
    Location location;
    union
    {};
} LilyCheckedPattern;

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
 * @brief Convert LilyCheckedPattern in String.
 */
String *
to_string__LilyCheckedPattern(const LilyCheckedPattern *self);

/**
 *
 * @brief Free LilyCheckedPattern type.
 */
DESTRUCTOR(LilyCheckedPattern, LilyCheckedPattern *self);

#endif // LILY_CORE_LILY_CHECKED_PATTERN_H
