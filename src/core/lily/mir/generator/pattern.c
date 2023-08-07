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

#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/literal.h>
#include <core/lily/mir/generator/pattern.h>

LilyMirInstructionVal *
generate_pattern__LilyMir(LilyMirModule *module,
                          LilyCheckedSignatureFun *fun_signature,
                          LilyMirScope *scope,
                          LilyCheckedPattern *pattern)
{
    switch (pattern->kind) {
        case LILY_CHECKED_PATTERN_KIND_ARRAY:
            TODO("pattern array");
        case LILY_CHECKED_PATTERN_KIND_AS:
            TODO("pattern as");
        case LILY_CHECKED_PATTERN_KIND_AUTO_COMPLETE:
        case LILY_CHECKED_PATTERN_KIND_WILDCARD:
            return NULL;
        case LILY_CHECKED_PATTERN_KIND_ERROR:
            TODO("pattern error");
        case LILY_CHECKED_PATTERN_KIND_LIST:
            TODO("pattern list");
        case LILY_CHECKED_PATTERN_KIND_LIST_HEAD:
            TODO("pattern list head");
        case LILY_CHECKED_PATTERN_KIND_LIST_TAIL:
            TODO("pattern list tail");
        case LILY_CHECKED_PATTERN_KIND_LITERAL:
            GENERATE_LITERAL(pattern, PATTERN);
        case LILY_CHECKED_PATTERN_KIND_NAME:
            TODO("pattern name");
        case LILY_CHECKED_PATTERN_KIND_NONE:
            TODO("pattern none");
        case LILY_CHECKED_PATTERN_KIND_RANGE:
            TODO("pattern range");
        case LILY_CHECKED_PATTERN_KIND_RECORD_CALL:
            TODO("pattern record call");
        case LILY_CHECKED_PATTERN_KIND_TUPLE:
            TODO("pattern tuple");
        case LILY_CHECKED_PATTERN_KIND_UNKNOWN:
            UNREACHABLE("unknown pattern");
        case LILY_CHECKED_PATTERN_KIND_VARIANT_CALL:
            TODO("pattern variant call");
        default:
            UNREACHABLE("unknown variant");
    }
}
