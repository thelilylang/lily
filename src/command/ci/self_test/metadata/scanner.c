/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#include <base/alloc.h>
#include <base/dir.h>
#include <base/file.h>
#include <base/new.h>
#include <base/str.h>

#include <cli/emit.h>

#include <command/ci/self_test/metadata/scanner.h>

#include <string.h>

struct CISelfTestMetadataScannerContext
{
    String *path;                 // String* (&)
    CISelfTestMetadata *metadata; // CISelfTestMetadata* (&)
    Usize line_count;
};

/// @param path char* (&)
static inline CONSTRUCTOR(struct CISelfTestMetadataScannerContext,
                          CISelfTestMetadataScannerContext,
                          String *path,
                          CISelfTestMetadata *metadata);

static bool
metadata_is_duplicatable__CISelfTestMetadataScanner(Int32 metadata_kind);

/// @param metadata_value String*
static void
set_metadata__CISelfTestMetadataScanner(
  struct CISelfTestMetadataScannerContext *ctx,
  Int32 metadata_kind,
  String *metadata_value);

/// @param msg const char* (&)
[[noreturn]] static void
emit_error__CISelfTestMetadataScanner(
  const struct CISelfTestMetadataScannerContext *ctx,
  const char *msg);

/// @param line char* (&)
/// @param end_match char* (&)
static void
get_metadata_from_line__CISelfTestMetadataScanner(
  char *line,
  char *end_match,
  struct CISelfTestMetadataScannerContext *ctx);

/// @param line_ref char** (&)
static bool
scan_line__CISelfTestMetadataScanner(
  char **line_ref,
  struct CISelfTestMetadataScannerContext *ctx);

CONSTRUCTOR(struct CISelfTestMetadataScannerContext,
            CISelfTestMetadataScannerContext,
            String *path,
            CISelfTestMetadata *metadata)
{
    return (struct CISelfTestMetadataScannerContext){ .path = path,
                                                      .metadata = metadata,
                                                      .line_count = 1 };
}

bool
metadata_is_duplicatable__CISelfTestMetadataScanner(Int32 metadata_kind)
{
#define MATCHES_EXPECTED_STDOUT 0
#define MATCHES_EXPECTED_STDERR 1
#define MATCHES_COMPILE_OPTIONS 2

    switch (metadata_kind) {
        case MATCHES_EXPECTED_STDOUT:
        case MATCHES_EXPECTED_STDERR:
            return true;
        case MATCHES_COMPILE_OPTIONS:
            return false;
        default:
            UNREACHABLE("unknown variant");
    }
}

void
set_metadata__CISelfTestMetadataScanner(
  struct CISelfTestMetadataScannerContext *ctx,
  Int32 metadata_kind,
  String *metadata_value)
{
    String **metadata_ctx_value = NULL; // String* (&)* (&)

    switch (metadata_kind) {
        case MATCHES_EXPECTED_STDOUT:
            metadata_ctx_value = &ctx->metadata->expected_stdout;

            break;
        case MATCHES_EXPECTED_STDERR:
            metadata_ctx_value = &ctx->metadata->expected_stderr;

            break;
        case MATCHES_COMPILE_OPTIONS:
            metadata_ctx_value = &ctx->metadata->compile_options;

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    if (!(*metadata_ctx_value)) {
        *metadata_ctx_value = metadata_value;
    } else if (metadata_is_duplicatable__CISelfTestMetadataScanner(
                 metadata_kind)) {
        APPEND_AND_FREE(*metadata_ctx_value, metadata_value);
    } else {
        emit_error__CISelfTestMetadataScanner(
          ctx, "this metadata cannot be duplicated");
    }
}

void
emit_error__CISelfTestMetadataScanner(
  const struct CISelfTestMetadataScannerContext *ctx,
  const char *msg)
{
    EMIT_ERROR_FMT("{S}:{zu}: {s}", ctx->path, ctx->line_count, msg);
    exit(1);
}

void
get_metadata_from_line__CISelfTestMetadataScanner(
  char *line,
  char *end_match,
  struct CISelfTestMetadataScannerContext *ctx)
{
#define MATCHES_SINGLE_LINE 0
#define MATCHES_MULTI_LINE 1

#define MATCHES_NOT_FOUND -1

#define MATCHES_SPACE_S " "
#define MATCHES_EXPECTED_STDOUT_S "@expected_stdout" MATCHES_SPACE_S
#define MATCHES_EXPECTED_STDERR_S "@expected_stderr" MATCHES_SPACE_S
#define MATCHES_COMPILE_OPTIONS_S "@compile_options" MATCHES_SPACE_S
    char *matches[] = {
        [MATCHES_EXPECTED_STDOUT] = MATCHES_EXPECTED_STDOUT_S,
        [MATCHES_EXPECTED_STDERR] = MATCHES_EXPECTED_STDERR_S,
        [MATCHES_COMPILE_OPTIONS] = MATCHES_COMPILE_OPTIONS_S,
    };
    const Int32 matches_len[] = {
        [MATCHES_EXPECTED_STDOUT] = sizeof(MATCHES_EXPECTED_STDOUT_S) - 1,
        [MATCHES_EXPECTED_STDERR] = sizeof(MATCHES_EXPECTED_STDERR_S) - 1,
        [MATCHES_COMPILE_OPTIONS] = sizeof(MATCHES_COMPILE_OPTIONS_S) - 1,
    };
    const Int32 matches_n = LEN(matches, *matches);
    const Int32 matches_len_n = LEN(matches_len, *matches_len);

    static_assert(matches_len_n == matches_len_n);

    Int32 match_at;

    strstr_list__Str(line, matches, matches_n, &match_at);

    switch (match_at) {
        case MATCHES_EXPECTED_STDOUT:
        case MATCHES_EXPECTED_STDERR:
        case MATCHES_COMPILE_OPTIONS: {
            line += matches_len[match_at];

            char *current = line;
            String *metadata_value = NEW(String);
            char *end_match_at = strstr(line, end_match); // char*? (&)

            if (!end_match_at) {
                goto error;
            }

            while (*current && current != end_match_at) {
                push__String(metadata_value, *current);
                ++current;
            }

            if (!*current) {
            error:
                emit_error__CISelfTestMetadataScanner(
                  ctx, "expected new line or `*/` before the end of line");
            }

            set_metadata__CISelfTestMetadataScanner(
              ctx, match_at, metadata_value);

            break;
        }
        case MATCHES_NOT_FOUND:
            emit_error__CISelfTestMetadataScanner(
              ctx,
              "expected `@expected_stdout`, `@expected_stderr` or "
              "`@compile_options` as flag");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

#undef MATCHES_EXPECTED_STDOUT
#undef MATCHES_EXPECTED_STDERR
#undef MATCHES_COMPILE_OPTIONS

#undef MATCHES_SPACE_S
#undef MATCHES_EXPECTED_STDOUT_S
#undef MATCHES_EXPECTED_STDERR_S
#undef MATCHES_COMPILE_OPTIONS_S
}

bool
scan_line__CISelfTestMetadataScanner(
  char **line_ref,
  struct CISelfTestMetadataScannerContext *ctx)
{
    char *line = *line_ref;
    char *matches[] = {
        [MATCHES_SINGLE_LINE] = "//@", [MATCHES_MULTI_LINE] = "/*@"
    };
    const Int32 matches_n = LEN(matches, *matches);
    char *end_matches[] = {
        [MATCHES_SINGLE_LINE] = "\n",
        [MATCHES_MULTI_LINE] = "*/",
    };
    Int32 match_at;
    char *begin =
      strstr_list__Str(line, matches, matches_n, &match_at); // char*? (&)

    if (line == begin) {
        get_metadata_from_line__CISelfTestMetadataScanner(
          line + 2 /* 2 = `//` or `/\*` */, end_matches[match_at], ctx);

        return true;
    }

    lily_free(line);

    *line_ref = NULL;

    return false;
#undef MATCHES_SINGLE_LINE
#undef MATCHES_MULTI_LINE
#undef MATCHES_NOT_FOUND
}

void
run__CISelfTestMetadataScanner(const CIProjectConfigSelfTest *self_test,
                               CISelfTestMetadata *metadata)
{
    FILE *f = open__File(self_test->path->buffer, "r");

    char *line = NULL; // char*?
    Usize line_n = 0;
    Isize nread;
    struct CISelfTestMetadataScannerContext ctx =
      NEW(CISelfTestMetadataScannerContext, self_test->path, metadata);

    if (!f) {
        emit_error__CISelfTestMetadataScanner(&ctx, "cannot read the file");
    }

    while ((nread = getline__File(&line, &line_n, f))) {
        if (!scan_line__CISelfTestMetadataScanner(&line, &ctx)) {
            break;
        }

        ++ctx.line_count;
    }

    close__File(f);
}
