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
    const String *path;           // const String* (&)
    CISelfTestMetadata *metadata; // CISelfTestMetadata* (&)
    Usize line_count;
};

/// @param path char* (&)
static inline CONSTRUCTOR(struct CISelfTestMetadataScannerContext,
                          CISelfTestMetadataScannerContext,
                          const String *path,
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

/// @param line const char* (&)
/// @param end_match const char* (&)
static void
get_metadata_from_line__CISelfTestMetadataScanner(
  const char *line,
  const char *end_match,
  struct CISelfTestMetadataScannerContext *ctx);

/// @param line_ref String** (&)
static bool
scan_line__CISelfTestMetadataScanner(
  String **line_ref,
  struct CISelfTestMetadataScannerContext *ctx);

CONSTRUCTOR(struct CISelfTestMetadataScannerContext,
            CISelfTestMetadataScannerContext,
            const String *path,
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
#define MATCHES_COMPILE_OPTIONS 1

    switch (metadata_kind) {
        case MATCHES_EXPECTED_STDOUT:
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
  const char *line,
  const char *end_match,
  struct CISelfTestMetadataScannerContext *ctx)
{
#define MATCHES_SINGLE_LINE 0
#define MATCHES_MULTI_LINE 1

#define MATCHES_NOT_FOUND -1

#define MATCHES_SPACE_S " "
#define MATCHES_EXPECTED_STDOUT_S "@expected_stdout" MATCHES_SPACE_S
#define MATCHES_COMPILE_OPTIONS_S "@compile_options" MATCHES_SPACE_S
    static const char *matches[] = {
        [MATCHES_EXPECTED_STDOUT] = MATCHES_EXPECTED_STDOUT_S,
        [MATCHES_COMPILE_OPTIONS] = MATCHES_COMPILE_OPTIONS_S,
    };
    static const Int32 matches_len[] = {
        [MATCHES_EXPECTED_STDOUT] = sizeof(MATCHES_EXPECTED_STDOUT_S) - 1,
        [MATCHES_COMPILE_OPTIONS] = sizeof(MATCHES_COMPILE_OPTIONS_S) - 1,
    };
    static const Usize matches_n = LEN(matches, *matches);
    static const Usize matches_len_n = LEN(matches_len, *matches_len);

    static_assert(matches_len_n == matches_len_n);

    Isize match_at;

    strstr_list__Str(line, matches, matches_n, &match_at);

    switch (match_at) {
        case MATCHES_EXPECTED_STDOUT:
        case MATCHES_COMPILE_OPTIONS: {
            line += matches_len[match_at];

            const char *current = line;
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

            enable_constant_escapes__String(metadata_value);

            set_metadata__CISelfTestMetadataScanner(
              ctx, match_at, metadata_value);

            break;
        }
        case MATCHES_NOT_FOUND:
            emit_error__CISelfTestMetadataScanner(
              ctx,
              "expected `@expected_stdout`, or "
              "`@compile_options` as flag");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

#undef MATCHES_EXPECTED_STDOUT
#undef MATCHES_COMPILE_OPTIONS

#undef MATCHES_SPACE_S
#undef MATCHES_EXPECTED_STDOUT_S
#undef MATCHES_COMPILE_OPTIONS_S
}

bool
scan_line__CISelfTestMetadataScanner(
  String **line_ref,
  struct CISelfTestMetadataScannerContext *ctx)
{
    String *line = *line_ref;
    const char *matches[] = {
        [MATCHES_SINGLE_LINE] = "//@", [MATCHES_MULTI_LINE] = "/*@"
    };
    const Int32 matches_n = LEN(matches, *matches);
    const char *end_matches[] = {
        [MATCHES_SINGLE_LINE] = "\n",
        [MATCHES_MULTI_LINE] = "*/",
    };
    Isize match_at;
    char *begin = find_sub_over_many__String(
      line, matches, matches_n, &match_at); // char*? (&)
    bool res = false;

    if (line->buffer == begin) {
        get_metadata_from_line__CISelfTestMetadataScanner(
          line->buffer + 2 /* 2 = `//` or `/\*` */, end_matches[match_at], ctx);

        res = true;
    }

    FREE(String, line);

    *line_ref = NULL;

    return res;
#undef MATCHES_SINGLE_LINE
#undef MATCHES_MULTI_LINE
#undef MATCHES_NOT_FOUND
}

void
run__CISelfTestMetadataScanner(const String *path, CISelfTestMetadata *metadata)
{
    FILE *f = open__File(path->buffer, "r");
    String *line = NULL; // String*?
    struct CISelfTestMetadataScannerContext ctx =
      NEW(CISelfTestMetadataScannerContext, path, metadata);

    if (!f) {
        emit_error__CISelfTestMetadataScanner(&ctx, "cannot read the file");
    }

    while (getline__File(&line, f)) {
        if (!scan_line__CISelfTestMetadataScanner(&line, &ctx)) {
            break;
        }

        ++ctx.line_count;
    }

    close__File(f);
}
