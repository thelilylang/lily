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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_CLI_CI_CONFIG_H
#define LILY_CLI_CI_CONFIG_H

#include <base/macros.h>

#include <cli/ci/config/compile.h>
#include <cli/ci/config/self_test.h>

enum CIConfigKind
{
    CI_CONFIG_KIND_COMPILE,
    CI_CONFIG_KIND_SELF_TEST
};

typedef struct CIConfig
{
    enum CIConfigKind kind;
    union
    {
        CIConfigCompile compile;
        CIConfigSelfTest self_test;
    };
} CIConfig;

/**
 *
 * @brief Construct CIConfig type (CI_CONFIG_KIND_COMPILE).
 */
inline VARIANT_CONSTRUCTOR(CIConfig, CIConfig, compile, CIConfigCompile compile)
{
    return (CIConfig){ .kind = CI_CONFIG_KIND_COMPILE, .compile = compile };
}

/**
 *
 * @brief Construct CIConfig type (CI_CONFIG_KIND_SELF_TEST).
 */
inline VARIANT_CONSTRUCTOR(CIConfig,
                           CIConfig,
                           self_test,
                           CIConfigSelfTest self_test)
{
    return (CIConfig){ .kind = CI_CONFIG_KIND_SELF_TEST,
                       .self_test = self_test };
}

/**
 *
 * @brief Free CIConfig type.
 * @param self const CIConfig* (&)
 */
DESTRUCTOR(CIConfig, const CIConfig *self);

#endif // LILY_CLI_CI_CONFIG_H
