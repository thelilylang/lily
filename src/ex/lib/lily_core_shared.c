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

#ifndef LILY_EX_LIB_LILY_CORE_SHARED_C
#define LILY_EX_LIB_LILY_CORE_SHARED_C

#include <core/shared/cursor.h>
#include <core/shared/diagnostic.h>
#include <core/shared/file.h>
#include <core/shared/location.h>
#include <core/shared/scanner.h>
#include <core/shared/source.h>

#include "lily_base.c"
#include "lily_core_cc_ci_diagnostic.c"
#include "lily_core_cc_diagnostic.c"
#include "lily_core_cpp_diagnostic.c"
#include "lily_core_lily_diagnostic.c"

// <core/shared/cursor.h>
extern inline CONSTRUCTOR(Cursor, Cursor, char *content);

// <core/shared/diagnostic.h>
extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  cc_error,
                                  CcError cc_error);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  cc_note,
                                  String *cc_note);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  cc_warning,
                                  CcWarning cc_warning);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  ci_error,
                                  CIError ci_error);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  ci_note,
                                  String *ci_note);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  ci_warning,
                                  CIWarning ci_warning);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  cpp_error,
                                  CppError cpp_error);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  cpp_note,
                                  String *cpp_note);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  cpp_warning,
                                  CppWarning cpp_warning);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  lily_error,
                                  LilyError lily_error);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  lily_note,
                                  String *lily_note);

extern inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                                  DiagnosticLevel,
                                  lily_warning,
                                  LilyWarning lily_warning);

extern inline CONSTRUCTOR(DiagnosticDetail,
                          DiagnosticDetail,
                          Vec *lines,
                          String *msg,
                          const Location *location);

// <core/shared/file.h>
extern inline CONSTRUCTOR(File, File, char *name, char *content);

extern inline DESTRUCTOR(File, const File *self);

// <core/shared/location.h>
extern inline CONSTRUCTOR(Location,
                          Location,
                          const char *filename,
                          const Usize start_line,
                          const Usize end_line,
                          const Usize start_column,
                          const Usize end_column,
                          const Usize start_position,
                          const Usize end_position);

extern inline Location
default__Location(const char *filename);

extern inline Location
clone__Location(const Location *self);

// <core/shared/scanner.h>
extern inline CONSTRUCTOR(Scanner, Scanner, Source source, Usize *count_error);

extern inline void
next_char__Scanner(Scanner *self);

extern inline void
previous_char__Scanner(Scanner *self);

extern inline bool
is_space__Scanner(Scanner *self);

extern inline bool
is_space_with_peeked_char__Scanner([[maybe_unused]] const Scanner *,
                                   const char *c);

extern inline Int32
get_keyword__Scanner(const String *id,
                     const SizedStr keywords[],
                     const Int32 keyword_ids[],
                     const Usize keywords_len);

// <core/shared/source.h>
extern inline CONSTRUCTOR(Source, Source, Cursor cursor, const File *file);

#endif // LILY_EX_LIB_LILY_CORE_SHARED_C
