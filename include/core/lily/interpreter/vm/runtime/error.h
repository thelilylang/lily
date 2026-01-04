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

#ifndef LILY_CORE_LILY_INTERPRETER_RUNTIME_ERROR_H
#define LILY_CORE_LILY_INTERPRETER_RUNTIME_ERROR_H

#include <base/types.h>

#define RUNTIME_ERROR_COMMON(msg) \
    emit_runtime_error_common__LilyInterpreterRuntimeError(msg)
#define RUNTIME_ERROR_RAISE_EXCEPTION(raise_name, filename, line, column) \
    emit_runtime_error_raise_exception__LilyInterpreterRuntimeError(      \
      raise_name, filename, line, column)
#define RUNTIME_ERROR_STACK_EMPTY() \
    emit_runtime_error_stack_empty__LilyInterpreterRuntimeError()
#define RUNTIME_ERROR_STACK_OVERFLOW() \
    emit_runtime_error_stack_overflow__LilyInterpreterRuntimeError()
#define RUNTIME_ERROR_UNREACHABLE(msg) \
    emit_runtime_error_unreachable__LilyInterpreterRuntimeError(msg)

enum LilyInterpreterRuntimeError
{
    LILY_INTERPRETER_RUNTIME_ERROR_COMMON,
    LILY_INTERPRETER_RUNTIME_ERROR_RAISE_EXCEPTION,
    LILY_INTERPRETER_RUNTIME_ERROR_STACK_EMPTY,
    LILY_INTERPRETER_RUNTIME_ERROR_STACK_OVERFLOW,
    LILY_INTERPRETER_RUNTIME_ERROR_UNREACHABLE,
};

/**
 *
 * @brief Emit runtime common error.
 */
void
emit_runtime_error_common__LilyInterpreterRuntimeError(const char *msg);

/**
 *
 * @brief Emit runtime raise exception error.
 */
void
emit_runtime_error_raise_exception__LilyInterpreterRuntimeError(
  const char *raise_name,
  const char *filename,
  Usize line,
  Usize column);

/**
 *
 * @brief Emit runtime stack empty error.
 */
void
emit_runtime_error_stack_empty__LilyInterpreterRuntimeError();

/**
 *
 * @brief Emit runtime stack overflow error.
 */
void
emit_runtime_error_stack_overflow__LilyInterpreterRuntimeError();

/**
 *
 * @brief Emit runtime unreachable error.
 */
void
emit_runtime_error_unreachable__LilyInterpreterRuntimeError(const char *msg);

#endif // LILY_CORE_LILY_INTERPRETER_RUNTIME_ERROR_H
