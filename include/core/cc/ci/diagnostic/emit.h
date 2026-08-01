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

#ifndef LILY_CORE_CC_CI_DIAGNOSTIC_EMIT_H
#define LILY_CORE_CC_CI_DIAGNOSTIC_EMIT_H

#include <base/print.h>

#include <core/shared/diagnostic.h>

#include <stdio.h>
#include <stdlib.h>

// Emit a located error and count it, without stopping the current stage: the
// caller is expected to carry on with a poisoned value, so that a single run
// reports as many errors as it can. Each stage checks its error count at its
// own boundary and stops there.
//
// `file_input` is the `File` the location belongs to, and `count_error` the
// `Usize *` the stage accumulates into.
#define EMIT_ERROR__CI(file_input, location, error, count_error) \
    emit__Diagnostic(NEW_VARIANT(Diagnostic,                     \
                                 simple_ci_error,                \
                                 file_input,                     \
                                 location,                       \
                                 error,                          \
                                 NULL,                           \
                                 NULL,                           \
                                 NULL),                          \
                     count_error)

// Same, with a vector of help messages (`Vec<String*>*`, moved).
#define EMIT_ERROR_WITH_HELP__CI(                  \
  file_input, location, error, helps, count_error) \
    emit__Diagnostic(NEW_VARIANT(Diagnostic,       \
                                 simple_ci_error,  \
                                 file_input,       \
                                 location,         \
                                 error,            \
                                 helps,            \
                                 NULL,             \
                                 NULL),            \
                     count_error)

// Emit a located warning. Warnings never stop a stage.
#define EMIT_WARNING__CI(                                      \
  file_input, location, warning, disable_codes, count_warning) \
    emit_warning__Diagnostic(NEW_VARIANT(Diagnostic,           \
                                         simple_ci_warning,    \
                                         file_input,           \
                                         location,             \
                                         warning,              \
                                         NULL,                 \
                                         NULL,                 \
                                         NULL),                \
                             disable_codes,                    \
                             count_warning)

// Emit an error that has no source position to point at: a configuration or
// environment failure (an unreadable `CI.yaml`, missing include paths, a
// duplicated input file, ...). Such an error is fatal - there is no program to
// keep analysing - so, unlike the located diagnostics, this one stops the
// process. It still carries the same code as the located errors.
#define FATAL_ERROR__CI(error)                              \
    {                                                       \
        CIError fatal_error = error;                        \
                                                            \
        EPRINTLN("{sa}", to_string__CIError(&fatal_error)); \
                                                            \
        exit(1);                                            \
    }

#endif // LILY_CORE_CC_CI_DIAGNOSTIC_EMIT_H
