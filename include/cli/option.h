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

#ifndef LILY_CLI_OPTION_H
#define LILY_CLI_OPTION_H

#include <base/macros.h>

#include <cli/option/build.h>
#include <cli/option/cc.h>
#include <cli/option/compile.h>
#include <cli/option/cpp.h>
#include <cli/option/init.h>
#include <cli/option/new.h>
#include <cli/option/run.h>
#include <cli/option/test.h>
#include <cli/option/to.h>

enum OptionKind
{
    OPTION_KIND_BUILD,
    OPTION_KIND_CC,
    OPTION_KIND_COMPILE,
    OPTION_KIND_CPP,
    OPTION_KIND_INIT,
    OPTION_KIND_NEW,
    OPTION_KIND_RUN,
    OPTION_KIND_TEST,
    OPTION_KIND_TO
};

typedef struct Option
{
    enum OptionKind kind;
    union
    {
        Vec *build;   // Vec<BuildOption*>*
        Vec *cc;      // Vec<BuildOption*>*
        Vec *compile; // Vec<CompileOption*>*
        Vec *cpp;     // Vec<CppOption*>*
        Vec *init;    // Vec<InitOption*>*
        Vec *new;     // Vec<NewOption*>*
        Vec *run;     // Vec<RunOption*>*
        Vec *test;    // Vec<TestOption*>*
        Vec *to;      // Vec<ToOption*>*
    };
} Option;

/**
 *
 * @brief Construct Option type (OPTION_KIND_BUILD).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, build, Vec *build)
{
    return (Option){ .kind = OPTION_KIND_BUILD, .build = build };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_CC).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, cc, Vec *cc)
{
    return (Option){
        .kind = OPTION_KIND_CC,
        .cc = cc,
    };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_COMPILE).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, compile, Vec *compile)
{
    return (Option){ .kind = OPTION_KIND_COMPILE, .compile = compile };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_CPP).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, cpp, Vec *cpp)
{
    return (Option){ .kind = OPTION_KIND_CPP, .cpp = cpp };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_INIT).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, init, Vec *init)
{
    return (Option){ .kind = OPTION_KIND_INIT, .init = init };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_NEW).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, new, Vec *new)
{
    return (Option){ .kind = OPTION_KIND_NEW, .new = new };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_RUN).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, run, Vec *run)
{
    return (Option){ .kind = OPTION_KIND_RUN, .run = run };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_TEST).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, test, Vec *test)
{
    return (Option){ .kind = OPTION_KIND_TEST, .test = test };
}

/**
 *
 * @brief Construct Option type (OPTION_KIND_TO).
 */
inline VARIANT_CONSTRUCTOR(Option, Option, to, Vec *to)
{
    return (Option){ .kind = OPTION_KIND_TO, .to = to };
}

/**
 *
 * @brief Free Option type.
 */
DESTRUCTOR(Option, Option self);

#endif // LILY_CLI_OPTION_H
