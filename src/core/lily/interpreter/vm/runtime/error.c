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

#include <base/macros.h>
#include <base/print.h>

#include <core/lily/interpreter/vm/runtime/error.h>

#include <stdio.h>
#include <stdlib.h>

#define PRINT_ERROR_HEADER(error_name) \
    PRINTLN("\x1b[31mcrash\x1b[0m({s}):", error_name);

static const char *s_LilyInterpreterRuntimeError[5] = {
    [LILY_INTERPRETER_RUNTIME_ERROR_COMMON] = "Common",
    [LILY_INTERPRETER_RUNTIME_ERROR_RAISE_EXCEPTION] = "RaiseException",
    [LILY_INTERPRETER_RUNTIME_ERROR_STACK_EMPTY] = "StackEmpty",
    [LILY_INTERPRETER_RUNTIME_ERROR_STACK_OVERFLOW] = "StackOverflow",
    [LILY_INTERPRETER_RUNTIME_ERROR_UNREACHABLE] = "Unreachable",
};

void
emit_runtime_error_common__LilyInterpreterRuntimeError(const char *msg)
{
    PRINT_ERROR_HEADER(
      s_LilyInterpreterRuntimeError[LILY_INTERPRETER_RUNTIME_ERROR_COMMON]);
    PRINTLN("{s}", msg);
    exit(1);
}

void
emit_runtime_error_raise_exception__LilyInterpreterRuntimeError(
  const char *raise_name,
  const char *filename,
  Usize line,
  Usize column)
{
    PRINT_ERROR_HEADER(s_LilyInterpreterRuntimeError
                         [LILY_INTERPRETER_RUNTIME_ERROR_RAISE_EXCEPTION]);
    PRINTLN("raised at {s}:{zu}:{zu}", raise_name, filename, line, column);
    exit(1);
}

void
emit_runtime_error_stack_empty__LilyInterpreterRuntimeError()
{
    PRINT_ERROR_HEADER(s_LilyInterpreterRuntimeError
                         [LILY_INTERPRETER_RUNTIME_ERROR_STACK_EMPTY]);
    PRINTLN("please submit a bug at https://github.com/thelilylang/lily");
    exit(1);
}

void
emit_runtime_error_stack_overflow__LilyInterpreterRuntimeError()
{
    PRINT_ERROR_HEADER(s_LilyInterpreterRuntimeError
                         [LILY_INTERPRETER_RUNTIME_ERROR_STACK_OVERFLOW]);
    PRINTLN("please increase the size of the stack");
    PRINTLN("help: there can be an infinite recursive function");
    exit(1);
}

void
emit_runtime_error_unreachable__LilyInterpreterRuntimeError(const char *msg)
{
    PRINT_ERROR_HEADER(s_LilyInterpreterRuntimeError
                         [LILY_INTERPRETER_RUNTIME_ERROR_UNREACHABLE]);
    PRINTLN("{s}", msg);
    exit(1);
}
