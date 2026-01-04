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

#include <base/assert.h>
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

#define LINES(location, file)                                                  \
    Usize start_position =                                                     \
      location->start_position == file->len - 1                                \
        ? location->start_position != 0 ? location->start_position - 1 : 0     \
        : location->start_position;                                            \
    Usize end_position = location->end_position;                               \
                                                                               \
    for (; start_position > 0 && file->content[start_position] != '\n';        \
         --start_position)                                                     \
        ;                                                                      \
    for (;                                                                     \
         end_position < file->len - 1 &&                                       \
         (file->content[end_position] != '\n' && file->content[end_position]); \
         ++end_position)                                                       \
        ;                                                                      \
                                                                               \
    if (start_position + 1 != file->len - 1 && start_position != 0 &&          \
        end_position != 0)                                                     \
        ++start_position;                                                      \
                                                                               \
    Vec *lines = NULL;                                                         \
                                                                               \
    if (location->end_line - location->start_line == 0) {                      \
        ASSERT(start_position <= end_position);                                \
                                                                               \
        if (start_position < end_position) {                                   \
            char *slice =                                                      \
              get_slice__Str(file->content, start_position, end_position);     \
                                                                               \
            lines = split__Str(slice, '\n');                                   \
                                                                               \
            lily_free(slice);                                                  \
        } else {                                                               \
            lines = NEW(Vec);                                                  \
        }                                                                      \
    } else {                                                                   \
        String *slice = NEW(String);                                           \
        Usize position = start_position;                                       \
                                                                               \
        for (Usize i = location->start_line; i < location->end_line; ++i) {    \
            while (file->content[position] != '\n' &&                          \
                   file->content[position]) {                                  \
                push__String(slice, file->content[position++]);                \
            }                                                                  \
                                                                               \
            ++position;                                                        \
        }                                                                      \
                                                                               \
        while (position < end_position) {                                      \
            push__String(slice, file->content[position++]);                    \
        }                                                                      \
                                                                               \
        lines = split__Str(slice->buffer, '\n');                               \
                                                                               \
        FREE(String, slice);                                                   \
    }                                                                          \
                                                                               \
    if (lines->len == 0) {                                                     \
        char *empty_str = malloc(1);                                           \
                                                                               \
        empty_str[0] = '\0';                                                   \
                                                                               \
        push__Vec(lines, empty_str);                                           \
    }

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
    for (Usize i = 0; i < self->lines->len; ++i)
        lily_free(self->lines->buffer[i]);

    FREE(Vec, self->lines);

    if (self->msg)
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

    if (self->helps) {
        for (Usize i = 0; i < self->helps->len; ++i) {
            char *s =
              format("{Sr} {sa}: {S}\n",
                     repeat__String(
                       " ", calc_usize_length(self->location->end_line) + 1),
                     GREEN("- help"),
                     self->helps->buffer[i]);

            PUSH_STR_AND_FREE(res, s);
        }
    }

    if (self->notes) {
        for (Usize i = 0; i < self->notes->len; ++i) {
            char *s =
              format("{Sr} {sa}: {S}\n",
                     repeat__String(
                       " ", calc_usize_length(self->location->end_line) + 1),
                     CYAN("- note"),
                     self->notes->buffer[i]);

            PUSH_STR_AND_FREE(res, s);
        }
    }

    return res;
}

DESTRUCTOR(DiagnosticLevelUtil, const DiagnosticLevelUtil *self)
{
    if (self->helps) {
        FREE_BUFFER_ITEMS(self->helps->buffer, self->helps->len, String);
        FREE(Vec, self->helps);
    }

    if (self->notes) {
        FREE_BUFFER_ITEMS(self->notes->buffer, self->notes->len, String);
        FREE(Vec, self->notes);
    }
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

    if (self->msg)
        FREE(String, self->msg);

    lily_free(self);
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

    for (Usize i = 0; i < self->items->len; ++i) {
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
        case DIAGNOSTIC_LEVEL_FORMAT_KIND_SIMPLE: {
            String *simple = to_string__Simple(&self->simple, level);
            String *level_util =
              to_string__DiagnosticLevelUtil(&self->simple.level_util);

            APPEND_AND_FREE(res, simple);
            APPEND_AND_FREE(res, level_util);

            break;
        }
        case DIAGNOSTIC_LEVEL_FORMAT_KIND_REFERENCING: {
            String *referencing =
              to_string__DiagnosticReferencing(&self->referencing, level);
            String *level_util =
              to_string__DiagnosticLevelUtil(&self->referencing.level_util);

            APPEND_AND_FREE(res, referencing);
            APPEND_AND_FREE(res, level_util);

            break;
        }
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

    {
        char *s = format(
          "{Sr}{sa}\n", repeat__String(" ", line_number_length + 1), BLUE("|"));
        PUSH_STR_AND_FREE(res, s);
    }

    if (self->lines->len == 1) {
        char *line = format("{s}", CAST(char *, get__Vec(self->lines, 0)));
        Usize count_whitespace = 0;

        for (Usize i = 0; line[i]; ++i) {
            if (isblank(line[i])) {
                count_whitespace++;
            } else
                break;
        }

        {
            char *s =
              format("\x1b[34m{d} |\x1b[0m", self->location->start_line);

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s = format(" {s}\n", line);

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s = format(
              "{Sr} {sa}", repeat__String(" ", line_number_length), BLUE("|"));

            PUSH_STR_AND_FREE(res, s);
        }

        push__String(res, ' ');

        for (Usize i = 0; i < count_whitespace; ++i) {
            push__String(res, line[i]);
        }

        {
            char *s =
              format("{Sr}",
                     repeat__String(
                       " ",
                       self->location->start_column - count_whitespace >= 1
                         ? self->location->start_column - count_whitespace - 1
                         : 0));

            PUSH_STR_AND_FREE(res, s);
        }

        {
            Usize diff =
              self->location->end_column - self->location->start_column;
            String *repeat = repeat__String("^", diff == 0 ? 1 : diff + 1);

            switch (level->kind) {
                case DIAGNOSTIC_LEVEL_KIND_CC_ERROR:
                case DIAGNOSTIC_LEVEL_KIND_CI_ERROR:
                case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR:
                case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR: {
                    char *red = RED(repeat->buffer);
                    PUSH_STR_AND_FREE(res, red);

                    break;
                }
                case DIAGNOSTIC_LEVEL_KIND_CC_NOTE:
                case DIAGNOSTIC_LEVEL_KIND_CI_NOTE:
                case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE:
                case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE: {
                    char *cyan = CYAN(repeat->buffer);
                    PUSH_STR_AND_FREE(res, cyan);

                    break;
                }
                case DIAGNOSTIC_LEVEL_KIND_CC_WARNING:
                case DIAGNOSTIC_LEVEL_KIND_CI_WARNING:
                case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING:
                case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING: {
                    char *yellow = YELLOW(repeat->buffer);
                    PUSH_STR_AND_FREE(res, yellow);

                    break;
                }
                default:
                    UNREACHABLE("unknown variant");
            }

            FREE(String, repeat);
        }

        if (self->msg) {
            char *s = format(" {S}\n", self->msg);
            PUSH_STR_AND_FREE(res, s);
        } else {
            push_str__String(res, "\n");
        }

        lily_free(line);
    } else {
        // TODO: diagnostic on one more line.
        // printf("%s\n", CAST(char*, get__Vec(self->lines, 0)));
        // printf("%s\n", CAST(char*, get__Vec(self->lines, 1)));
        char *first_line =
          format(" {s}", CAST(char *, get__Vec(self->lines, 0)));
        char *last_line = format(" {s}", CAST(char *, last__Vec(self->lines)));

        {
            char *s = format("\x1b[34m{d}\x1b[0m{Sr}",
                             self->location->start_line,
                             repeat__String(" ", line_number_length));

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s = format("{sa} {s}", BLUE("|"), first_line);

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s =
              format("\n{Sr}",
                     repeat__String(
                       " ", self->location->start_column + line_number_length));

            PUSH_STR_AND_FREE(res, s);
        }

        switch (level->kind) {
            case DIAGNOSTIC_LEVEL_KIND_CC_ERROR:
            case DIAGNOSTIC_LEVEL_KIND_CI_ERROR:
            case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR:
            case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR: {
                char *red = RED("^");
                PUSH_STR_AND_FREE(res, red);

                break;
            }
            case DIAGNOSTIC_LEVEL_KIND_CC_NOTE:
            case DIAGNOSTIC_LEVEL_KIND_CI_NOTE:
            case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE:
            case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE: {
                char *cyan = CYAN("^");
                PUSH_STR_AND_FREE(res, cyan);

                break;
            }
            case DIAGNOSTIC_LEVEL_KIND_CC_WARNING:
            case DIAGNOSTIC_LEVEL_KIND_CI_WARNING:
            case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING:
            case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING: {
                char *yellow = YELLOW("^");
                PUSH_STR_AND_FREE(res, yellow);

                break;
            }
            default:
                UNREACHABLE("unknown variant");
        }

        if (self->msg) {
            char *s = format(" {S}\n", self->msg);
            PUSH_STR_AND_FREE(res, s);
        } else {
            push_str__String(res, "\n");
        }

        {
            char *s = format("\x1b[34m{Sr}\x1b[0m {sa} \x1b[34m{Sr}\x1b[0m\n",
                             repeat__String("~", line_number_length),
                             BLUE("|"),
                             repeat__String("_", strlen(first_line)));

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s = format("{Sr} {sa}\n",
                             repeat__String(" ", line_number_length),
                             BLUE("|"));

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s = format("\x1b[34m{d}\x1b[0m{Sr}",
                             self->location->end_line,
                             repeat__String(" ", line_number_length - 1));

            PUSH_STR_AND_FREE(res, s);
        }

        {
            char *s = format(" {sa} {s}\n", BLUE("|"), last_line);

            PUSH_STR_AND_FREE(res, s);
        }

        lily_free(first_line);
        lily_free(last_line);
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
                    simple_ci_error,
                    const File *file,
                    const Location *location,
                    const CIError err,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CI_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, ci_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_ci_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CI_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, ci_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    simple_ci_warning,
                    const File *file,
                    const Location *location,
                    const CIWarning warning,
                    Vec *helps,
                    Vec *notes,
                    String *detail_msg)
{
    LINES(location, file);

    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_SIMPLE_CI_WARNING,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     simple,
                     NEW(DiagnosticSimple,
                         NEW(DiagnosticDetail, lines, detail_msg, location),
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, ci_warning, warning) };
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
                    referencing_ci_error,
                    const File *file,
                    const Location *location,
                    const CIError err,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CI_ERROR,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, ci_error, err) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_ci_note,
                    const File *file,
                    const Location *location,
                    String *note,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CI_NOTE,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, ci_note, note) };
}

VARIANT_CONSTRUCTOR(Diagnostic,
                    Diagnostic,
                    referencing_ci_warning,
                    const File *file,
                    const Location *location,
                    const CIWarning warning,
                    Vec *items,
                    Vec *notes,
                    Vec *helps)
{
    return (
      Diagnostic){ .kind = DIAGNOSTIC_KIND_REFERENCING_CI_WARNING,
                   .level_format = NEW_VARIANT(
                     DiagnosticLevelFormat,
                     referencing,
                     NEW(DiagnosticReferencing,
                         items,
                         NEW(DiagnosticLevelUtil, helps, notes, location))),
                   .file = file,
                   .location = location,
                   .level = NEW_VARIANT(DiagnosticLevel, ci_warning, warning) };
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

    {
        char *s = format("{s}:{d}:{d}: ",
                         self->file->name,
                         self->location->start_line,
                         self->location->start_column);

        PUSH_STR_AND_FREE(res, s);
    }

    switch (self->level.kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_ERROR: {
            char *s = to_string__CcError(&self->level.cc_error);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CC_NOTE: {
            char *s = format("{sa}: {S}", CYAN("note"), self->level.cc_note);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CC_WARNING: {
            char *s = to_string__CcWarning(&self->level.cc_warning);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CI_ERROR: {
            char *s = to_string__CIError(&self->level.ci_error);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CI_NOTE: {
            char *s = format("{sa}: {S}", CYAN("note"), self->level.ci_note);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CI_WARNING: {
            char *s = to_string__CIWarning(&self->level.ci_warning);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR: {
            char *s = to_string__CppError(&self->level.cpp_error);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE: {
            char *s = format("{sa}: {S}", CYAN("note"), self->level.cpp_note);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING: {
            char *s = to_string__CppWarning(&self->level.cpp_warning);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR: {
            char *s = to_string__LilyError(&self->level.lily_error);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE: {
            char *s = format("{sa}: {S}", CYAN("note"), self->level.lily_note);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING: {
            char *s = to_string__LilyWarning(&self->level.lily_warning);

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    push__String(res, '\n');

    {
        String *s =
          to_string__DiagnosticLevelFormat(&self->level_format, &self->level);

        APPEND_AND_FREE(res, s);
    }

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
        case DIAGNOSTIC_LEVEL_KIND_CI_WARNING:
            code = to_code__CIWarning(&self.level.ci_warning);
            break;
        case DIAGNOSTIC_LEVEL_KIND_CPP_WARNING:
            code = to_code__CppWarning(&self.level.cpp_warning);
            break;
        case DIAGNOSTIC_LEVEL_KIND_LILY_WARNING:
            code = to_code__LilyWarning(&self.level.lily_warning);
            break;
        default:
            UNREACHABLE("expected warning diagnostic level");
    }

    // TODO: Remove the verification after adding the disabled codes in the
    // package.
    if (disable_codes) {
        for (Usize i = 0; i < disable_codes->len; ++i) {
            if (!strcmp(CAST(String *, disable_codes->buffer[i])->buffer,
                        code)) {
                FREE(Diagnostic, &self);
                return;
            }
        }
    }

    *count_warning += 1;

    EPRINTLN("{Sr}", to_string__Diagnostic(&self));

    FREE(Diagnostic, &self);
}

void
emit_note__Diagnostic(Diagnostic self)
{
    switch (self.level.kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_NOTE:
        case DIAGNOSTIC_LEVEL_KIND_CI_NOTE:
        case DIAGNOSTIC_LEVEL_KIND_CPP_NOTE:
        case DIAGNOSTIC_LEVEL_KIND_LILY_NOTE:
            break;
        default:
            UNREACHABLE("expected note diagnostic level");
    }

    EPRINTLN("{Sr}", to_string__Diagnostic(&self));

    FREE(Diagnostic, &self);
}

void
emit__Diagnostic(Diagnostic self, Usize *count_error)
{
    switch (self.level.kind) {
        case DIAGNOSTIC_LEVEL_KIND_CC_ERROR:
        case DIAGNOSTIC_LEVEL_KIND_CI_ERROR:
        case DIAGNOSTIC_LEVEL_KIND_CPP_ERROR:
        case DIAGNOSTIC_LEVEL_KIND_LILY_ERROR:
            *count_error += 1;
            break;
        default:
            UNREACHABLE("expected error diagnostic level");
    }

    EPRINTLN("{Sr}", to_string__Diagnostic(&self));

    FREE(Diagnostic, &self);
}

// Free Diagnostic type.
DESTRUCTOR(Diagnostic, const Diagnostic *self)
{
    FREE(DiagnosticLevelFormat, &self->level_format);
    FREE(DiagnosticLevel, &self->level);
}
