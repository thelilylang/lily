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

#ifndef LILY_CLI_CONFIG_H
#define LILY_CLI_CONFIG_H

#include <cli/config/build.h>
#include <cli/config/cc.h>
#include <cli/config/compile.h>
#include <cli/config/cpp.h>
#include <cli/config/init.h>
#include <cli/config/new.h>
#include <cli/config/run.h>
#include <cli/config/test.h>
#include <cli/config/to.h>

enum ConfigKind
{
    CONFIG_KIND_BUILD,
    CONFIG_KIND_CC,
    CONFIG_KIND_COMPILE,
    CONFIG_KIND_CPP,
    CONFIG_KIND_INIT,
    CONFIG_KIND_NEW,
    CONFIG_KIND_RUN,
    CONFIG_KIND_TEST,
    CONFIG_KIND_TO
};

typedef struct Config
{
    enum ConfigKind kind;
    union
    {
        BuildConfig build;
        CcConfig cc;
        CompileConfig compile;
        CppConfig cpp;
        InitConfig init;
        NewConfig new;
        RunConfig run;
        TestConfig test;
        ToConfig to;
    };
} Config;

/**
 *
 * @brief Construct Option type (CONFIG_KIND_BUILD).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, build, BuildConfig build)
{
    return (Config){ .kind = CONFIG_KIND_BUILD, .build = build };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_CC).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, cc, CcConfig cc)
{
    return (Config){ .kind = CONFIG_KIND_CC, .cc = cc };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_COMPILE).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, compile, CompileConfig compile)
{
    return (Config){ .kind = CONFIG_KIND_COMPILE, .compile = compile };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_CPP).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, cpp, CppConfig cpp)
{
    return (Config){ .kind = CONFIG_KIND_CPP, .cpp = cpp };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_INIT).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, init, InitConfig init)
{
    return (Config){ .kind = CONFIG_KIND_INIT, .init = init };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_NEW).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, new, NewConfig new)
{
    return (Config){ .kind = CONFIG_KIND_NEW, .new = new };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_RUN).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, run, RunConfig run)
{
    return (Config){ .kind = CONFIG_KIND_RUN, .run = run };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_TEST).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, test, TestConfig test)
{
    return (Config){ .kind = CONFIG_KIND_TEST, .test = test };
}

/**
 *
 * @brief Construct Option type (CONFIG_KIND_TO).
 */
inline VARIANT_CONSTRUCTOR(Config, Config, to, ToConfig to)
{
    return (Config){ .kind = CONFIG_KIND_TO, .to = to };
}

#endif // LILY_CLI_CONFIG_H
