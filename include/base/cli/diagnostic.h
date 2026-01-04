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

#ifndef LILY_BASE_CLI_DIAGNOSTIC_H
#define LILY_BASE_CLI_DIAGNOSTIC_H

#include <base/string.h>
#include <base/types.h>

enum CliDiagnosticKind
{
    CLI_DIAGNOSTIC_KIND_ERROR,
    CLI_DIAGNOSTIC_KIND_NOTE,
    CLI_DIAGNOSTIC_KIND_WARNING,
};

typedef struct CliDiagnostic
{
    enum CliDiagnosticKind kind;
    const char *msg;
    Usize arg_count;
    String *full_command; // String* (&)
} CliDiagnostic;

/**
 *
 * @brief Construct CliDiagnostic type.
 */
inline CONSTRUCTOR(CliDiagnostic,
                   CliDiagnostic,
                   enum CliDiagnosticKind kind,
                   const char *msg,
                   Usize arg_count,
                   String *full_command)
{
    return (CliDiagnostic){ .kind = kind,
                            .msg = msg,
                            .arg_count = arg_count,
                            .full_command = full_command };
}

/**
 *
 * @brief Emit a diagnostic (can exit(1) when it emits an error).
 */
void
emit__CliDiagnostic(const CliDiagnostic *self);

#endif // LILY_BASE_CLI_DIAGNOSTIC_H
