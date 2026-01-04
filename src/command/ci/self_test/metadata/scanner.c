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
static enum CISelfTestMetadataScannerError
set_metadata__CISelfTestMetadataScanner(
  struct CISelfTestMetadataScannerContext *ctx,
  Int32 metadata_kind,
  String *metadata_value);

/// @param line const char* (&)
/// @param end_match const char* (&)
static int
get_metadata_from_line__CISelfTestMetadataScanner(
  const char *line,
  const char *end_match,
  struct CISelfTestMetadataScannerContext *ctx);

/// @param line_ref String** (&)
static int
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
#define MATCHES_EXPECTED_BIN_STDOUT 0
#define MATCHES_COMPILE_OPTIONS 1
#define MATCHES_EXPECTED_COMPILER_ERROR 2

    switch (metadata_kind) {
        case MATCHES_EXPECTED_BIN_STDOUT:
            return true;
        case MATCHES_COMPILE_OPTIONS:
        case MATCHES_EXPECTED_COMPILER_ERROR:
            return false;
        default:
            UNREACHABLE("unknown variant");
    }
}

enum CISelfTestMetadataScannerError
set_metadata__CISelfTestMetadataScanner(
  struct CISelfTestMetadataScannerContext *ctx,
  Int32 metadata_kind,
  String *metadata_value)
{
    String **metadata_ctx_value = NULL; // String* (&)* (&)

    switch (metadata_kind) {
        case MATCHES_EXPECTED_BIN_STDOUT:
            metadata_ctx_value = &ctx->metadata->expected_bin_stdout;

            break;
        case MATCHES_COMPILE_OPTIONS:
            metadata_ctx_value = &ctx->metadata->compile_options;

            break;
        case MATCHES_EXPECTED_COMPILER_ERROR:
            metadata_ctx_value = &ctx->metadata->expected_compiler_error;

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
        return CI_SELF_TEST_METADATA_SCANNER_ERROR_CANNOT_BE_DUPLICATED;
    }

    return CI_SELF_TEST_METADATA_SCANNER_ERROR_NONE;
}

int
get_metadata_from_line__CISelfTestMetadataScanner(
  const char *line,
  const char *end_match,
  struct CISelfTestMetadataScannerContext *ctx)
{
#define MATCHES_SINGLE_LINE 0
#define MATCHES_MULTI_LINE 1

#define MATCHES_NOT_FOUND -1

#define MATCHES_SPACE_S " "
#define MATCHES_EXPECTED_BIN_STDOUT_S "@expected_bin_stdout" MATCHES_SPACE_S
#define MATCHES_COMPILE_OPTIONS_S "@compile_options" MATCHES_SPACE_S
#define MATCHES_EXPECTED_COMPILER_ERROR_S \
    "@expected_compiler_error" MATCHES_SPACE_S
    static const char *matches[] = {
        [MATCHES_EXPECTED_BIN_STDOUT] = MATCHES_EXPECTED_BIN_STDOUT_S,
        [MATCHES_COMPILE_OPTIONS] = MATCHES_COMPILE_OPTIONS_S,
        [MATCHES_EXPECTED_COMPILER_ERROR] = MATCHES_EXPECTED_COMPILER_ERROR_S,
    };
    static const Int32 matches_len[] = {
        [MATCHES_EXPECTED_BIN_STDOUT] =
          sizeof(MATCHES_EXPECTED_BIN_STDOUT_S) - 1,
        [MATCHES_COMPILE_OPTIONS] = sizeof(MATCHES_COMPILE_OPTIONS_S) - 1,
        [MATCHES_EXPECTED_COMPILER_ERROR] =
          sizeof(MATCHES_EXPECTED_COMPILER_ERROR_S) - 1,
    };
    static const Usize matches_n = LEN(matches, *matches);
    static const Usize matches_len_n = LEN(matches_len, *matches_len);

    static_assert(matches_len_n == matches_len_n);

    Isize match_at;

    strstr_list__Str(line, matches, matches_n, &match_at);

    switch (match_at) {
        case MATCHES_EXPECTED_BIN_STDOUT:
        case MATCHES_COMPILE_OPTIONS:
        case MATCHES_EXPECTED_COMPILER_ERROR: {
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
                return CI_SELF_TEST_METADATA_SCANNER_ERROR_EXPECTED_NEW_LINE_OR_CLOSING_COMMENT;
            }

            enable_constant_escapes__String(metadata_value);

            // Replace (e.g.) @path by its value.
            {
                static const char *subs[] = { "@path" }; // const char* (&)* (&)
                const char *replacements[] = {
                    ctx->path->buffer,
                }; // const char* (&)* (&)
                const Usize subs_len = sizeof(subs) / sizeof(*subs);
                const Usize replacements_len =
                  sizeof(replacements) / sizeof(*replacements);

                static_assert(subs_len == replacements_len);

                replace_many_sub__String(
                  metadata_value, subs, replacements, subs_len);
            }

            return set_metadata__CISelfTestMetadataScanner(
              ctx, match_at, metadata_value);
        }
        case MATCHES_NOT_FOUND:
            return CI_SELF_TEST_METADATA_SCANNER_ERROR_UNEXPECTED_FLAG;
        default:
            UNREACHABLE("unknown variant");
    }

#undef MATCHES_EXPECTED_BIN_STDOUT
#undef MATCHES_COMPILE_OPTIONS
#undef MATCHES_EXPECTED_COMPILER_ERROR

#undef MATCHES_SPACE_S
#undef MATCHES_EXPECTED_BIN_STDOUT_S
#undef MATCHES_COMPILE_OPTIONS_S
#undef MATCHES_EXPECTED_COMPILER_ERROR_S
}

int
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
    enum CISelfTestMetadataScannerError res =
      CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_SCAN_LINE;

    if (line->buffer == begin) {
        get_metadata_from_line__CISelfTestMetadataScanner(
          line->buffer + 2 /* 2 = `//` or `/\*` */, end_matches[match_at], ctx);

        res = CI_SELF_TEST_METADATA_SCANNER_ERROR_NONE;
    }

    FREE(String, line);

    *line_ref = NULL;

    return res;
#undef MATCHES_SINGLE_LINE
#undef MATCHES_MULTI_LINE
#undef MATCHES_NOT_FOUND
}

enum CISelfTestMetadataScannerError
run__CISelfTestMetadataScanner(const String *path, CISelfTestMetadata *metadata)
{
    FILE *f = open__File(path->buffer, "r");
    String *line = NULL; // String*?
    struct CISelfTestMetadataScannerContext ctx =
      NEW(CISelfTestMetadataScannerContext, path, metadata);

    if (!f) {
        return CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_READ_FILE;
    }

    while (getline__File(&line, f)) {
        enum CISelfTestMetadataScannerError error =
          scan_line__CISelfTestMetadataScanner(&line, &ctx);

        switch (error) {
            case CI_SELF_TEST_METADATA_SCANNER_ERROR_NONE:
                continue;
            case CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_SCAN_LINE:
                goto out;
            default:
                return error;
        }

        ++ctx.line_count;
    }

out:

    close__File(f);

    return CI_SELF_TEST_METADATA_SCANNER_ERROR_NONE;
}

const char *
get_error_message__CISelfTestMetadataScanner(
  enum CISelfTestMetadataScannerError error)
{
    switch (error) {
        case CI_SELF_TEST_METADATA_SCANNER_ERROR_NONE:
            return "";
        case CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_READ_FILE:
            return "cannot read the file";
        case CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_SCAN_LINE:
            UNREACHABLE("this error should never be returned");
        case CI_SELF_TEST_METADATA_SCANNER_ERROR_UNEXPECTED_FLAG:
            return "expected `@expected_bin_stdout`, or `@compile_options` as "
                   "flag";
        case CI_SELF_TEST_METADATA_SCANNER_ERROR_EXPECTED_NEW_LINE_OR_CLOSING_COMMENT:
            return "expected new line or `*/` before the end of line";
        case CI_SELF_TEST_METADATA_SCANNER_ERROR_CANNOT_BE_DUPLICATED:
            return "this metadata cannot be duplicated";
        default:
            UNREACHABLE("unknown variant");
    }
}
