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

#ifndef LILY_CORE_LILY_H
#define LILY_CORE_LILY_H

#include <base/macros.h>

// This is a macro used to display the scanner in debug mode.
#define DEBUG_SCANNER

#ifndef ENV_DEBUG
#undef DEBUG_SCANNER
#endif

// This is a macro used to display the preparser in debug mode.
#define DEBUG_PREPARSER

#ifndef ENV_DEBUG
#undef DEBUG_PREPARSER
#endif

// This is a macro used to display the precompiler in debug mode.
#define DEBUG_PRECOMPILE

#ifndef ENV_DEBUG
#undef DEBUG_PRECOMPILE
#endif

// This is a macro used to display the parser in debug mode.
#define DEBUG_PARSER

#ifndef ENV_DEBUG
#undef DEBUG_PARSER
#endif

// This is a macro used to display the analysis in debug mode.
#define DEBUG_ANALYSIS

#ifndef ENV_DEBUG
#undef DEBUG_ANALYSIS
#endif

// This is a macro used to display the MIR in debug mode.
#define DEBUG_MIR

#ifndef ENV_DEBUG
#undef DEBUG_MIR
#endif

// This is a macro used to display the IR in debug mode.
#define DEBUG_IR

#ifndef ENV_DEBUG
#undef DEBUG_IR
#endif

// This is a macro used to stop the compiler process after the preparser.
#define RUN_UNTIL_PREPARSER
#undef RUN_UNTIL_PREPARSER

// This is a macro used to stop the compiler process after the precompiler.
#define RUN_UNTIL_PRECOMPILER
#undef RUN_UNTIL_PRECOMPILER

#endif // LILY_CORE_LILY_H
