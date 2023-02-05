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

#include <base/color.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/print.h>
#include <base/str.h>

#include <core/shared/diagnostic.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Free DiagnosticLevel type.
static DESTRUCTOR(DiagnosticLevel, const DiagnosticLevel *self);

// Free DiagnosticDetail type.
static inline DESTRUCTOR(DiagnosticDetail, const DiagnosticDetail *self);

// Construct DiagnosticLevelUtil type.
static inline CONSTRUCTOR(DiagnosticLevelUtil,
                          DiagnosticLevelUtil,
                          Vec *helps,
                          Vec *notes,
                          const Location *location);

static String *
to_string__DiagnosticLevelUtil(const DiagnosticLevelUtil *self);

// Free DiagnosticLevelUtil type.
static DESTRUCTOR(DiagnosticLevelUtil, const DiagnosticLevelUtil *self);

// Construct DiagnosticReferencingItem type.
static inline CONSTRUCTOR(DiagnosticReferencingItem,
                          DiagnosticReferencingItem,
                          String *msg,
                          String *detail_msg,
                          const DiagnosticDetail *detail,
                          const File *file);

// Convert DiagnosticReferencingItem in String.
static String *
to_string__DiagnosticReferencingItem(const DiagnosticReferencingItem *self,
                                     const DiagnosticLevel *level);

static DESTRUCTOR(DiagnosticReferencingItem, DiagnosticReferencingItem *self);

// Construct DiagnosticReferencing type.
static inline CONSTRUCTOR(DiagnosticReferencing,
                          DiagnosticReferencing,
                          Vec *items,
                          const DiagnosticLevelUtil level_util);

// Convert DiagnosticReferencing in String.
static String *
to_string__DiagnosticReferencing(const DiagnosticReferencing *self,
                                 const DiagnosticLevel *level);

// Free DiagnosticReferencing type.
static inline DESTRUCTOR(DiagnosticReferencing,
                         const DiagnosticReferencing *self);

// Construct DiagnosticSimple type.
static inline CONSTRUCTOR(DiagnosticSimple,
                          DiagnosticSimple,
                          const DiagnosticDetail detail,
                          const DiagnosticLevelUtil level_util);

static DESTRUCTOR(DiagnosticSimple, const DiagnosticSimple *self);

// Convert Simple in String.
static String *
to_string__Simple(const DiagnosticSimple *self, const DiagnosticLevel *level);

// Construct DiagnosticLevelFormat type (DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE).
static inline VARIANT_CONSTRUCTOR(DiagnosticLevelFormat,
                                  DiagnosticLevelFormat,
                                  simple,
                                  const DiagnosticSimple simple);

// Construct DiagnosticLevelFormat type
// (DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING).
static inline VARIANT_CONSTRUCTOR(DiagnosticLevelFormat,
                                  DiagnosticLevelFormat,
                                  referencing,
                                  const DiagnosticReferencing referencing);

// Convert DiagnosticLevelFormat in String.
static String *
to_string__DiagnosticLevelFormat(const DiagnosticLevelFormat *self,
                                 const DiagnosticLevel *level);

// Free DiagnosticLevelFormat type (DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE).
static VARIANT_DESTRUCTOR(DiagnosticLevelFormat,
                          simple,
                          const DiagnosticLevelFormat *self);

// Free DiagnosticLevelFormat type (DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING).
static VARIANT_DESTRUCTOR(DiagnosticLevelFormat,
                          referencing,
                          const DiagnosticLevelFormat *self);

// Free DiagnosticLevelFormat type.
static DESTRUCTOR(DiagnosticLevelFormat, const DiagnosticLevelFormat *self);

// Get the length of integer.
static Usize
calc_usize_length(Usize line);

// Convert DiagnosticDetail in String.
static String *
to_string__DiagnosticDetail(const DiagnosticDetail *self,
                            const DiagnosticLevel *level);

// Convert Diagnostic in String.
static String *
to_string__Diagnostic(const Diagnostic *self);

// Free Diagnostic type.
static inline DESTRUCTOR(Diagnostic, const Diagnostic *self);

#define LINES(location, file)                                                 \
    char **split = split__Str(file->content, '\n');                           \
    Vec *lines = NEW(Vec);                                                    \
    if (location->start_line == location->end_line) {                         \
        push__Vec(lines, split[location->start_line - 1]);                    \
        for (Usize i = 0; i < LEN(split, *split); i++) {                      \
            if (i != location->start_line - 1)                                \
                free(split[i]);                                               \
        }                                                                     \
    } else {                                                                  \
        push__Vec(lines, split[location->start_line - 1]);                    \
        push__Vec(lines, split[location->end_line - 1]);                      \
        for (Usize i = 0; i < LEN(split, *split); i++) {                      \
            if (i != location->start_line - 1 && i != location->end_line - 1) \
                free(split[i]);                                               \
        }                                                                     \
    }                                                                         \
    free(split);

DESTRUCTOR(DiagnosticLevel, const DiagnosticLevel *self)
{
    switch (self->kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_NOTE:
            FREE(String, self->cc_note);
            break;
        case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE:
            FREE(String, self->cpp_note);
            break;
        case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE:
            FREE(String, self->lily_note);
            break;
        default:
            break;
    }
}

DESTRUCTOR(DiagnosticDetail, const DiagnosticDetail *self)
{
    for (Usize i = 0; i < self->lines->len; i++)
        free(self->lines->buffer[i]);

    FREE(Vec, self->lines);
    FREE(String, self->msg);
}

CONSTRUCTOR(DiagnosticLevelUtil,
            DiagnosticLevelUtil,
            Vec *helps, // Vec<String*>*
            Vec *notes, // Vec<String*>*
            const Location *location)
{
    return (DiagnosticLevelUtil){ .helps = helps,
                                  .notes = notes,
                                  .location = location };
}

String *
to_string__DiagnosticLevelUtil(const DiagnosticLevelUtil *self)
{
    String *res = NEW(String);

    for (Usize i = 0; i < self->helps->len; i++) {
        PUSH_STR_AND_FREE(
          res,
          format("{Sr} {sa}: {S}\n",
                 repeat__String(
                   " ", calc_usize_length(self->location->end_line) + 1),
                 GREEN("- help"),
                 self->helps->buffer[i]));
    }

    for (Usize i = 0; i < self->notes->len; i++) {
        PUSH_STR_AND_FREE(
          res,
          format("{Sr} {sa}: {S}\n",
                 repeat__String(
                   " ", calc_usize_length(self->location->end_line) + 1),
                 CYAN("- note"),
                 self->notes->buffer[i]));
    }

    return res;
}

DESTRUCTOR(DiagnosticLevelUtil, const DiagnosticLevelUtil *self)
{
    for (Usize i = self->helps->len; --i;) {
        FREE(String, self->helps->buffer[i]);
    }

    for (Usize i = self->notes->len; --i;) {
        FREE(String, self->notes->buffer[i]);
    }

    FREE(Vec, self->helps);
    FREE(Vec, self->notes);
}

CONSTRUCTOR(DiagnosticReferencingItem,
            DiagnosticReferencingItem,
            String *msg,
            String *detail_msg,
            const DiagnosticDetail *detail,
            const File *file)
{
    LINES(detail->location, file);

    return (DiagnosticReferencingItem){
        .msg = msg,
        .detail = NEW(DiagnosticDetail, lines, detail_msg, detail->location)
    };
}

String *
to_string__DiagnosticReferencingItem(const DiagnosticReferencingItem *self,
                                     const DiagnosticLevel *level)
{
    String *res = NEW(String);

    PUSH_STR_AND_FREE(res, format("{sa} {S}", BLUE("--------> "), self->msg));
    APPEND_AND_FREE(res, to_string__DiagnosticDetail(&self->detail, level));

    return res;
}

DESTRUCTOR(DiagnosticReferencingItem, DiagnosticReferencingItem *self)
{
    FREE(DiagnosticDetail, &self->detail);
    free(self->msg);
    free(self);
}

CONSTRUCTOR(DiagnosticReferencing,
            DiagnosticReferencing,
            Vec *items,
            const DiagnosticLevelUtil level_util)
{
    return (DiagnosticReferencing){ .items = items, .level_util = level_util };
}

String *
to_string__DiagnosticReferencing(const DiagnosticReferencing *self,
                                 const DiagnosticLevel *level)
{
    String *res = NEW(String);

    for (Usize i = 0; i < self->items->len; i++) {
        APPEND_AND_FREE(
          res,
          to_string__DiagnosticReferencingItem(self->items->buffer[i], level));
    }

    return res;
}

DESTRUCTOR(DiagnosticReferencing, const DiagnosticReferencing *self)
{
    FREE_BUFFER_ITEMS(
      self->items->buffer, self->items->len, DiagnosticReferencingItem);
    FREE(Vec, self->items);
    FREE(DiagnosticLevelUtil, &self->level_util);
}

CONSTRUCTOR(DiagnosticSimple,
            DiagnosticSimple,
            const DiagnosticDetail detail,
            const DiagnosticLevelUtil level_util)
{
    return (DiagnosticSimple){ .detail = detail, .level_util = level_util };
}

DESTRUCTOR(DiagnosticSimple, const DiagnosticSimple *self)
{
    FREE(DiagnosticDetail, &self->detail);
    FREE(DiagnosticLevelUtil, &self->level_util);
}

String *
to_string__Simple(const DiagnosticSimple *self, const DiagnosticLevel *level)
{
    return to_string__DiagnosticDetail(&self->detail, level);
}

VARIANT_CONSTRUCTOR(DiagnosticLevelFormat,
                    DiagnosticLevelFormat,
                    simple,
                    const DiagnosticSimple simple)
{
    return (DiagnosticLevelFormat){ .kind = DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE,
                                    .simple = simple };
}

VARIANT_CONSTRUCTOR(DiagnosticLevelFormat,
                    DiagnosticLevelFormat,
                    referencing,
                    const DiagnosticReferencing referencing)
{
    return (DiagnosticLevelFormat){ .kind =
                                      DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING,
                                    .referencing = referencing };
}

String *
to_string__DiagnosticLevelFormat(const DiagnosticLevelFormat *self,
                                 const DiagnosticLevel *level)
{
    String *res = NEW(String);

    switch (self->kind) {
        case DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE:
            APPEND_AND_FREE(res, to_string__Simple(&self->simple, level));
            APPEND_AND_FREE(
              res, to_string__DiagnosticLevelUtil(&self->simple.level_util));
            break;
        case DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING:
            APPEND_AND_FREE(
              res, to_string__DiagnosticReferencing(&self->referencing, level));
            APPEND_AND_FREE(
              res,
              to_string__DiagnosticLevelUtil(&self->referencing.level_util));
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}

VARIANT_DESTRUCTOR(DiagnosticLevelFormat,
                   simple,
                   const DiagnosticLevelFormat *self)
{
    FREE(DiagnosticSimple, &self->simple);
}

VARIANT_DESTRUCTOR(DiagnosticLevelFormat,
                   referencing,
                   const DiagnosticLevelFormat *self)
{
    FREE(DiagnosticReferencing, &self->referencing);
}

DESTRUCTOR(DiagnosticLevelFormat, const DiagnosticLevelFormat *self)
{
    switch (self->kind) {
        case DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE:
            FREE_VARIANT(DiagnosticLevelFormat, simple, self);
            break;
        case DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING:
            FREE_VARIANT(DiagnosticLevelFormat, referencing, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

Usize
calc_usize_length(Usize line)
{
    Usize i = 0;

    while (line > 0) {
        line /= 10;
        i++;
    }

    return i;
}

String *
to_string__DiagnosticDetail(const DiagnosticDetail *self,
                            const DiagnosticLevel *level)
{
    String *res = NEW(String);
    Usize line_number_length = calc_usize_length(self->location->end_line);

    PUSH_STR_AND_FREE(
      res, format("|{Sr}", repeat__String(" ", line_number_length + 1)));

    if (self->lines->len == 1) {
        char *line = format(" {s}", CAST(char *, get__Vec(self->lines, 0)));
        Usize count_whitespace = 0;

        for (Usize i = 0; line[i++];) {
            if (isspace(line[i - 1])) {
                count_whitespace++;
            } else
                break;
        }

        PUSH_STR_AND_FREE(res,
                          format("\x1b[34m{d} |\x1b[0m {Sr}",
                                 self->location->start_line,
                                 repeat__String(" ", line_number_length - 1)));
        PUSH_STR_AND_FREE(res, format("{sa}\n", line));
        PUSH_STR_AND_FREE(res,
                          format("{Sr} {sa}",
                                 repeat__String(" ", line_number_length),
                                 BLUE("|")));

        for (Usize i = 0; line[i++];) {
            if (isspace(line[i - 1])) {
                push__String(res, line[i - 1]);
            } else {
                break;
            }
        }

        PUSH_STR_AND_FREE(
          res,
          format("{Sr}",
                 repeat__String(
                   " ", self->location->start_column - count_whitespace)));

        {
            String *repeat = repeat__String("^",
                                            self->location->end_column -
                                              self->location->start_column + 1);

            switch (level->kind) {
                case DIAGNOSTIC_LEVEL_KIND_CC_ERROR:
                case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR:
                case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR:
                    PUSH_STR_AND_FREE(res, RED(repeat->buffer));
                    break;
                case DIAGNOSTIC_LEVEL_KIND_CC_NOTE:
                case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE:
                case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE:
                    PUSH_STR_AND_FREE(res, CYAN(repeat->buffer));
                    break;
                case DIAGNOSTIC_LEVEL_KIND_CC_WARNING:
                case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING:
                case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING:
                    PUSH_STR_AND_FREE(res, YELLOW(repeat->buffer));
                    break;
            }

            FREE(String, repeat);
        }

        PUSH_STR_AND_FREE(res, format("~ {s}\n", self->msg));
    } else {
        TODO("make diagnostic with one more line");
    }

    return res;
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cc_error,
                    const File *file,
                    const Location *location,
                    const CcError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CC_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cc_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cc_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CC_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cc_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cc_warning,
                    const File *file,
                    const Location *location,
                    const CcWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CC_WARNING,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cc_warning, warning) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cpp_error,
                    const File *file,
                    const Location *location,
                    const CppError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CPP_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cpp_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cpp_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CPP_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cpp_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_cpp_warning,
                    const File *file,
                    const Location *location,
                    const CppWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (Diagnostic){
        .kind = DIAGNOSTIC_KIND_SIMPLE_CPP_WARNING,
        .level_format =
          NEW_VARIANT(DiagnosticLevelFormat,
                      simple,
                      NEW(DiagnosticSimple,
                          NEW(DiagnosticDetail, lines, detail_msg, location),
                          NEW(DiagnosticLevelUtil, helps, notes, location))),
        .file = file,
        .location = location,
        .level = NEW_VARIANT(DiagnosticLevel, cpp_warning, warning)
    };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_lily_error,
                    const File *file,
                    const Location *location,
                    const LilyError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_LILY_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, lily_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_lily_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_LILY_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, lily_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_lily_warning,
                    const File *file,
                    const Location *location,
                    const LilyWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{

    LINES(location, file);

    return (Diagnostic){
        .kind = DIAGNOSTIC_KIND_SIMPLE_LILY_WARNING,
        .level_format =
          NEW_VARIANT(DiagnosticLevelFormat,
                      simple,
                      NEW(DiagnosticSimple,
                          NEW(DiagnosticDetail, lines, detail_msg, location),
                          NEW(DiagnosticLevelUtil, helps, notes, location))),
        .file = file,
        .location = location,
        .level = NEW_VARIANT(DiagnosticLevel, lily_warning, warning)
    };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cc_error,
                    const File *file,
                    const Location *location,
                    const CcError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CC_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cc_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cc_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CC_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cc_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cc_warning,
                    const File *file,
                    const Location *location,
                    const CcWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CC_WARNING,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cc_warning, warning) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cpp_error,
                    const File *file,
                    const Location *location,
                    const CppError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CPP_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cpp_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cpp_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CPP_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, cpp_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_cpp_warning,
                    const File *file,
                    const Location *location,
                    const CppWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (Diagnostic){
        .kind = DIAGNOSTIC_KIND_REFERENCING_CPP_WARNING,
        .level_format =
          NEW_VARIANT(DiagnosticLevelFormat,
                      referencing,
                      NEW(DiagnosticReferencing,
                          items,
                          NEW(DiagnosticLevelUtil, helps, notes, location))),
        .file = file,
        .location = location,
        .level = NEW_VARIANT(DiagnosticLevel, cpp_warning, warning)
    };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_lily_error,
                    const File *file,
                    const Location *location,
                    const LilyError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_LILY_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, lily_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_lily_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_LILY_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, lily_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_lily_warning,
                    const File *file,
                    const Location *location,
                    const LilyWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (Diagnostic){
        .kind = DIAGNOSTIC_KIND_REFERENCING_LILY_WARNING,
        .level_format =
          NEW_VARIANT(DiagnosticLevelFormat,
                      referencing,
                      NEW(DiagnosticReferencing,
                          items,
                          NEW(DiagnosticLevelUtil, helps, notes, location))),
        .file = file,
        .location = location,
        .level = NEW_VARIANT(DiagnosticLevel, lily_warning, warning)
    };
}

String *
to_string__Diagnostic(const Diagnostic *self)
{
    String *res = NEW(String);

    PUSH_STR_AND_FREE(res,
                      format("{s}:{d}:{d}: ",
                             self->file->filename,
                             self->location->start_line,
                             self->location->start_column));

    switch (self->level.kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_ERROR:
            PUSH_STR_AND_FREE(res, to_string__CcError(&self->level.cc_error));
            break;
        case DIAGNOSTIC_LEVEL_KIND_CC_NOTE:
            PUSH_STR_AND_FREE(res, format("note: {S}", self->level.cc_note));
            break;
        case DIAGNOSTIC_LEVEL_KIND_CC_WARNING:
            PUSH_STR_AND_FREE(res,
                              to_string__CcWarning(&self->level.cc_warning));
            break;
        case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR:
            PUSH_STR_AND_FREE(res, to_string__CppError(&self->level.cpp_error));
            break;
        case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE:
            PUSH_STR_AND_FREE(res, format("note: {S}", self->level.cpp_note));
            break;
        case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING:
            PUSH_STR_AND_FREE(res,
                              to_string__CppWarning(&self->level.cpp_warning));
            break;
        case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR:
            PUSH_STR_AND_FREE(res,
                              to_string__LilyError(&self->level.lily_error));
            break;
        case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE:
            PUSH_STR_AND_FREE(res, format("note: {S}", self->level.lily_note));
            break;
        case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING:
            PUSH_STR_AND_FREE(
              res, to_string__LilyWarning(&self->level.lily_warning));
            break;
        default:
            UNREACHABLE("unknown variant");
    }

    push__String(res, '\n');
    APPEND_AND_FREE(
      res, to_string__DiagnosticLevelFormat(&self->level_format, &self->level));

    return res;
}

void
emit_warning__Diagnostic(Diagnostic self,
                         Vec *disable_codes, // Vec<String*>*
                         Usize *count_warning)
{
    char *code;

    switch (self.level.kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_WARNING:
            code = to_code__CcWarning(&self.level.cc_warning);
            break;
        case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING:
            code = to_code__CppWarning(&self.level.cpp_warning);
            break;
        case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING:
            code = to_code__LilyWarning(&self.level.lily_warning);
            break;
        default:
            UNREACHABLE("expected warning diagnostic");
    }

    for (Usize i = 0; i < disable_codes->len; i++) {
        if (!strcmp(CAST(String *, disable_codes->buffer[i])->buffer, code)) {
            FREE(Diagnostic, &self);
            return;
        }
    }

    *count_warning += 1;

    PRINTLN("{Sr}", to_string__Diagnostic(&self));

    FREE(Diagnostic, &self);
}

void
emit__Diagnostic(Diagnostic self, Usize *count_error)
{
    switch (self.level.kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_ERROR:
        case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR:
        case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR:
            *count_error += 1;
            break;
        default:
            break;
    }

    PRINTLN("{Sr}", to_string__Diagnostic(&self));

    FREE(Diagnostic, &self);
}

// Free Diagnostic type.
DESTRUCTOR(Diagnostic, const Diagnostic *self)
{
    FREE(DiagnosticLevelFormat, &self->level_format);
    FREE(DiagnosticLevel, &self->level);
}