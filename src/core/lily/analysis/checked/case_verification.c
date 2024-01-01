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

#include <core/lily/analysis/checked/case_verification.h>

bool
valid_snake_case__LilyCheckedCaseVerification(char *s)
{
    for (char *current = s; *current; ++current) {
        if (!((*current >= 'a' && *current <= 'z') || *current == '_')) {
            return false;
        }
    }

    return true;
}

bool
valid_pascal_case__LilyCheckedCaseVerification(char *s)
{
    if (!(*s >= 'A' && *s <= 'Z')) {
        return false;
    }

    for (char *current = s + 1; *current; ++current) {
        if (!((*current >= 'a' && *current <= 'z') ||
              (*current >= 'A' && *current <= 'Z'))) {
            return false;
        }
    }

    return true;
}

bool
valid_upper_snake_case__LilyCheckedCaseVerification(char *s)
{
    for (char *current = s; *current; ++current) {
        if (!((*current >= 'A' && *current <= 'Z') || *current == '_')) {
            return false;
        }
    }

    return true;
}
