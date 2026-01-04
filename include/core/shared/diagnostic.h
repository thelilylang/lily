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

#ifndef LILY_CORE_SHARED_DIAGNOSTIC_H
#define LILY_CORE_SHARED_DIAGNOSTIC_H

#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/cc/ci/diagnostic/error.h>
#include <core/cc/ci/diagnostic/warning.h>
#include <core/cc/diagnostic/error.h>
#include <core/cc/diagnostic/warning.h>
#include <core/cpp/diagnostic/error.h>
#include <core/cpp/diagnostic/warning.h>
#include <core/lily/diagnostic/error.h>
#include <core/lily/diagnostic/warning.h>
#include <core/shared/file.h>
#include <core/shared/location.h>

#include <stdlib.h>

enum DiagnosticLevelKind
{
    DIAGNOSTIC_LEVEL_KIND_CC_ERROR,
    DIAGNOSTIC_LEVEL_KIND_CC_NOTE,
    DIAGNOSTIC_LEVEL_KIND_CC_WARNING,
    DIAGNOSTIC_LEVEL_KIND_CI_ERROR,
    DIAGNOSTIC_LEVEL_KIND_CI_NOTE,
    DIAGNOSTIC_LEVEL_KIND_CI_WARNING,
    DIAGNOSTIC_LEVEL_KIND_CPP_ERROR,
    DIAGNOSTIC_LEVEL_KIND_CPP_NOTE,
    DIAGNOSTIC_LEVEL_KIND_CPP_WARNING,
    DIAGNOSTIC_LEVEL_KIND_LILY_ERROR,
    DIAGNOSTIC_LEVEL_KIND_LILY_NOTE,
    DIAGNOSTIC_LEVEL_KIND_LILY_WARNING
};

typedef struct DiagnosticLevel
{
    enum DiagnosticLevelKind kind;
    union
    {
        CcError cc_error;
        String *cc_note;
        CcWarning cc_warning;
        CIError ci_error;
        String *ci_note;
        CIWarning ci_warning;
        CppError cpp_error;
        String *cpp_note;
        CppWarning cpp_warning;
        LilyError lily_error;
        String *lily_note;
        LilyWarning lily_warning;
    };
} DiagnosticLevel;

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CC_ERROR).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           cc_error,
                           CcError cc_error)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CC_ERROR,
                              .cc_error = cc_error };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CC_NOTE).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           cc_note,
                           String *cc_note)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CC_NOTE,
                              .cc_note = cc_note };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CC_WARNING).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           cc_warning,
                           CcWarning cc_warning)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CC_WARNING,
                              .cc_warning = cc_warning };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CI_ERROR).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           ci_error,
                           CIError ci_error)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CI_ERROR,
                              .ci_error = ci_error };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CI_NOTE).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           ci_note,
                           String *ci_note)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CI_NOTE,
                              .ci_note = ci_note };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CI_WARNING).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           ci_warning,
                           CIWarning ci_warning)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CI_WARNING,
                              .ci_warning = ci_warning };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CPP_ERROR).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           cpp_error,
                           CppError cpp_error)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CPP_ERROR,
                              .cpp_error = cpp_error };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CPP_NOTE).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           cpp_note,
                           String *cpp_note)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CPP_NOTE,
                              .cpp_note = cpp_note };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_CPP_WARNING).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           cpp_warning,
                           CppWarning cpp_warning)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_CPP_WARNING,
                              .cpp_warning = cpp_warning };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_LILY_ERROR).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           lily_error,
                           LilyError lily_error)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_LILY_ERROR,
                              .lily_error = lily_error };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_LILY_NOTE).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           lily_note,
                           String *lily_note)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_LILY_NOTE,
                              .lily_note = lily_note };
}

/**
 *
 * @brief Construct DiagnosticLevel type (DIAGNOSTIC_LEVEL_KIND_LILY_WARNING).
 */
inline VARIANT_CONSTRUCTOR(DiagnosticLevel,
                           DiagnosticLevel,
                           lily_warning,
                           LilyWarning lily_warning)
{
    return (DiagnosticLevel){ .kind = DIAGNOSTIC_LEVEL_KIND_LILY_WARNING,
                              .lily_warning = lily_warning };
}

typedef struct DiagnosticDetail
{
    Vec *lines;  // Vec<char*>*
    String *msg; // String*?
    const Location *location;
} DiagnosticDetail;

/**
 *
 * @brief Construct DiagnosticDetail.
 */
inline CONSTRUCTOR(DiagnosticDetail,
                   DiagnosticDetail,
                   Vec *lines,
                   String *msg,
                   const Location *location)
{
    return (
      DiagnosticDetail){ .lines = lines, .msg = msg, .location = location };
}

typedef struct DiagnosticLevelUtil
{
    Vec *helps; // Vec<String*>*?
    Vec *notes; // Vec<String*>*?
    const Location *location;
} DiagnosticLevelUtil;

typedef struct DiagnosticReferencingItem
{
    String *msg;
    const DiagnosticDetail detail;
} DiagnosticReferencingItem;

typedef struct DiagnosticReferencing
{
    Vec *items; // Vec<DiagnosticReferencingItem*>*
    const DiagnosticLevelUtil level_util;
} DiagnosticReferencing;

typedef struct DiagnosticSimple
{
    const DiagnosticDetail detail;
    const DiagnosticLevelUtil level_util;
} DiagnosticSimple;

enum DiagnosticLevelFormatKind
{
    DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE,
    DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING
};

typedef struct DiagnosticLevelFormat
{
    enum DiagnosticLevelFormatKind kind;
    union
    {
        const DiagnosticSimple simple;
        const DiagnosticReferencing referencing;
    };
} DiagnosticLevelFormat;

enum DiagnosticKind
{
    DIAGNOSTIC_KIND_SIMPLE_CC_ERROR,
    DIAGNOSTIC_KIND_SIMPLE_CI_ERROR,
    DIAGNOSTIC_KIND_SIMPLE_CPP_ERROR,
    DIAGNOSTIC_KIND_SIMPLE_LILY_ERROR,
    DIAGNOSTIC_KIND_SIMPLE_CC_NOTE,
    DIAGNOSTIC_KIND_SIMPLE_CI_NOTE,
    DIAGNOSTIC_KIND_SIMPLE_CPP_NOTE,
    DIAGNOSTIC_KIND_SIMPLE_LILY_NOTE,
    DIAGNOSTIC_KIND_SIMPLE_CC_WARNING,
    DIAGNOSTIC_KIND_SIMPLE_CI_WARNING,
    DIAGNOSTIC_KIND_SIMPLE_CPP_WARNING,
    DIAGNOSTIC_KIND_SIMPLE_LILY_WARNING,
    DIAGNOSTIC_KIND_REFERENCING_CC_ERROR,
    DIAGNOSTIC_KIND_REFERENCING_CI_ERROR,
    DIAGNOSTIC_KIND_REFERENCING_CPP_ERROR,
    DIAGNOSTIC_KIND_REFERENCING_LILY_ERROR,
    DIAGNOSTIC_KIND_REFERENCING_CC_NOTE,
    DIAGNOSTIC_KIND_REFERENCING_CI_NOTE,
    DIAGNOSTIC_KIND_REFERENCING_CPP_NOTE,
    DIAGNOSTIC_KIND_REFERENCING_LILY_NOTE,
    DIAGNOSTIC_KIND_REFERENCING_CC_WARNING,
    DIAGNOSTIC_KIND_REFERENCING_CI_WARNING,
    DIAGNOSTIC_KIND_REFERENCING_CPP_WARNING,
    DIAGNOSTIC_KIND_REFERENCING_LILY_WARNING
};

typedef struct Diagnostic
{
    enum DiagnosticKind kind;
    const DiagnosticLevelFormat level_format;
    const File *file;
    const Location *location;
    DiagnosticLevel level;
} Diagnostic;

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CC_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cc_error,
                    const File *file,
                    const Location *location,
                    const CcError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CC_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cc_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CC_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cc_warning,
                    const File *file,
                    const Location *location,
                    const CcWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CI_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_ci_error,
                    const File *file,
                    const Location *location,
                    const CIError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CI_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_ci_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CI_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_ci_warning,
                    const File *file,
                    const Location *location,
                    const CIWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CPP_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cpp_error,
                    const File *file,
                    const Location *location,
                    const CppError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CPP_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cpp_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_CPP_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cpp_warning,
                    const File *file,
                    const Location *location,
                    const CppWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_LILY_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_lily_error,
                    const File *file,
                    const Location *location,
                    const LilyError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_LILY_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_lily_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_SIMPLE_LILY_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_lily_warning,
                    const File *file,
                    const Location *location,
                    const LilyWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CC_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cc_error,
                    const File *file,
                    const Location *location,
                    const CcError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CC_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cc_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CC_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cc_warning,
                    const File *file,
                    const Location *location,
                    const CcWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CI_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_ci_error,
                    const File *file,
                    const Location *location,
                    const CIError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CI_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_ci_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CI_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_ci_warning,
                    const File *file,
                    const Location *location,
                    const CIWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CPP_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cpp_error,
                    const File *file,
                    const Location *location,
                    const CppError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CPP_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cpp_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_CPP_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cpp_warning,
                    const File *file,
                    const Location *location,
                    const CppWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_LILY_ERROR).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_lily_error,
                    const File *file,
                    const Location *location,
                    const LilyError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_LILY_NOTE).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_lily_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Construct Diagnostic (DIAGNOSTIC_KIND_REFERENCING_LILY_WARNING).
 */
VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_lily_warning,
                    const File *file,
                    const Location *location,
                    const LilyWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps);

/**
 *
 * @brief Emit warning.
 * @note If the current warning code is find in disable_codes, this function
 * will emit nothing.
 */
void
emit_warning__Diagnostic(Diagnostic self,
                         Vec *disable_codes,
                         Usize *count_warning);

/**
 *
 * @brief Emit note.
 */
void
emit_note__Diagnostic(Diagnostic self);

/**
 *
 * @brief Emit error.
 */
void
emit__Diagnostic(Diagnostic self, Usize *count_error);

#endif // LILY_CORE_SHARED_DIAGNOSTIC_H
