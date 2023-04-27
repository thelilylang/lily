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

#include <core/lily/checked/pattern.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternKind,
               enum LilyCheckedPatternKind self)
{
    switch (self) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            return "LILY_CHECKED_PATTERN_KIND_ARRAY";
        case LILY_CHECKED_PATTERN_KIND_AS:
            return "LILY_CHECKED_PATTERN_KIND_AS";
        case LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE:
            return "LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE";
        case LILY_CHECKED_PATTERN_KIND_EXCEPTION:
            return "LILY_CHECKED_PATTERN_KIND_EXCEPTION";
        case LILY_CHECKED_PATTERN_KIND_LIST:
            return "LILY_CHECKED_PATTERN_KIND_LIST";
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            return "LILY_CHECKED_PATTERN_KIND_LIST_HEAD";
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            return "LILY_CHECKED_PATTERN_KIND_LIST_TAIL";
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            return "LILY_CHECKED_PATTERN_KIND_LITERAL";
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            return "LILY_CHECKED_PATTERN_KIND_RANGE";
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            return "LILY_CHECKED_PATTERN_KIND_RECORD_CALL";
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            return "LILY_CHECKED_PATTERN_KIND_TUPLE";
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            return "LILY_CHECKED_PATTERN_KIND_VARIANT_CALL";
        case LILY_CHECKED_PATTERN_KIND_WILDCARD:
            return "LILY_CHECKED_PATTERN_KIND_WILDCARD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedPattern, const LilyCheckedPattern *self)
{
    TODO("impl to_string__Debug__LilyCheckedPattern");
}
#endif

String *
to_string__LilyCheckedPattern(const LilyCheckedPattern *self)
{
    TODO("impl to_string__LilyCheckedPattern");
}

DESTRUCTOR(LilyCheckedPattern, LilyCheckedPattern *self) {}
