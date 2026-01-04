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

#ifndef LILY_COMMAND_CI_SELF_TEST_METADATA_SCANNER_H
#define LILY_COMMAND_CI_SELF_TEST_METADATA_SCANNER_H

#include <command/ci/self_test/metadata.h>

#include <core/cc/ci/project_config.h>

enum CISelfTestMetadataScannerError
{
    CI_SELF_TEST_METADATA_SCANNER_ERROR_NONE,
    CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_READ_FILE,
    CI_SELF_TEST_METADATA_SCANNER_ERROR_FAILED_TO_SCAN_LINE,
    CI_SELF_TEST_METADATA_SCANNER_ERROR_UNEXPECTED_FLAG,
    CI_SELF_TEST_METADATA_SCANNER_ERROR_EXPECTED_NEW_LINE_OR_CLOSING_COMMENT,
    CI_SELF_TEST_METADATA_SCANNER_ERROR_CANNOT_BE_DUPLICATED
};

/**
 *
 * @brief Scan self-test metadata.
 * @param path const String* (&)
 */
enum CISelfTestMetadataScannerError
run__CISelfTestMetadataScanner(const String *path,
                               CISelfTestMetadata *metadata);

/**
 *
 * @brief Convert error code to an error message.
 * @return const char* (&)
 */
const char *
get_error_message__CISelfTestMetadataScanner(
  enum CISelfTestMetadataScannerError error);

#endif // LILY_COMMAND_CI_SELF_TEST_METADATA_SCANNER_H
