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

#include <base/macros.h>

#ifdef ENV_DEBUG
#include <core/lily/shared/visibility.h>

#include <stdio.h>
#include <stdlib.h>

char *
IMPL_FOR_DEBUG(to_string, LilyVisibility, enum LilyVisibility self)
{
    switch (self) {
        case LILY_VISIBILITY_PUBLIC:
            return "LILY_VISIBILITY_PUBLIC";
        case LILY_VISIBILITY_PRIVATE:
            return "LILY_VISIBILITY_PRIVATE";
        case LILY_VISIBILITY_STATIC:
            return "LILY_VISIBILITY_STATIC";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif
