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

#ifndef LILY_CLI_LILY_CONFIG_H
#define LILY_CLI_LILY_CONFIG_H

#include <cli/lily/config/build.h>
#include <cli/lily/config/cc.h>
#include <cli/lily/config/cpp.h>
#include <cli/lily/config/init.h>
#include <cli/lily/config/new.h>
#include <cli/lily/config/run.h>
#include <cli/lily/config/test.h>
#include <cli/lily/config/to.h>

enum LilyConfigKind
{
    LILY_CONFIG_KIND_BUILD,
    LILY_CONFIG_KIND_CC,
    LILY_CONFIG_KIND_COMPILE,
    LILY_CONFIG_KIND_CPP,
    LILY_CONFIG_KIND_INIT,
    LILY_CONFIG_KIND_NEW,
    LILY_CONFIG_KIND_RUN,
    LILY_CONFIG_KIND_TEST,
    LILY_CONFIG_KIND_TO
};

typedef struct LilyConfig
{
    enum LilyConfigKind kind;
    union
    {
        LilyConfigBuild build;
        LilyConfigCc cc;
        LilyConfigCpp cpp;
        LilyConfigInit init;
        LilyConfigNew new;
        LilyConfigRun run;
        LilyConfigTest test;
        LilyConfigTo to;
    };
} LilyConfig;

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_BUILD).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, build, LilyConfigBuild build)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_BUILD, .build = build };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_CC).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, cc, LilyConfigCc cc)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_CC, .cc = cc };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_COMPILE).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, compile)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_COMPILE };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_CPP).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, cpp, LilyConfigCpp cpp)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_CPP, .cpp = cpp };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_INIT).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, init, LilyConfigInit init)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_INIT, .init = init };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_NEW).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, new, LilyConfigNew new)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_NEW, .new = new };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_RUN).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, run, LilyConfigRun run)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_RUN, .run = run };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_TEST).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, test, LilyConfigTest test)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_TEST, .test = test };
}

/**
 *
 * @brief Construct LilyConfig type (LILY_CONFIG_KIND_TO).
 */
inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, to, LilyConfigTo to)
{
    return (LilyConfig){ .kind = LILY_CONFIG_KIND_TO, .to = to };
}

#endif // LILY_CLI_LILY_CONFIG_H
