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

#include <cli/ci/config.h>

#include <stdio.h>
#include <stdlib.h>

/// @brief Free CIConfig type (CI_CONFIG_KIND_COMPILE).
static inline VARIANT_DESTRUCTOR(CIConfig, compile, const CIConfig *self);

/// @brief Free CIConfig type (CI_CONFIG_KIND_SELF_TEST).
static inline VARIANT_DESTRUCTOR(CIConfig, self_test, const CIConfig *self);

VARIANT_DESTRUCTOR(CIConfig, compile, const CIConfig *self)
{
    FREE(CIConfigCompile, &self->compile);
}

VARIANT_DESTRUCTOR(CIConfig, self_test, const CIConfig *self)
{
    // NOTE: Nothing to free yet.
}

DESTRUCTOR(CIConfig, const CIConfig *self)
{
    switch (self->kind) {
        case CI_CONFIG_KIND_COMPILE:
            FREE_VARIANT(CIConfig, compile, self);

            break;
        case CI_CONFIG_KIND_SELF_TEST:
            FREE_VARIANT(CIConfig, self_test, self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
