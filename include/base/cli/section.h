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

#ifndef LILY_BASE_CLI_SECTION_H
#define LILY_BASE_CLI_SECTION_H

#include <base/string.h>

enum CliSectionKind
{
    CLI_SECTION_KIND_AUTHOR,
    CLI_SECTION_KIND_BUG,
    CLI_SECTION_KIND_DESCRIPTION,
    CLI_SECTION_KIND_HELP,
    CLI_SECTION_KIND_USAGE,
};

typedef struct CliSection
{
    enum CliSectionKind kind;
    union
    {
        String *author;
        String *bug;
        String *description;
        String *help;
        String *usage; // String* (&)
    };
} CliSection;

/**
 *
 * @brief Construct CliSection type (CLI_SECTION_KIND_AUTHOR).
 */
VARIANT_CONSTRUCTOR(CliSection *, CliSection, author, String *author);

/**
 *
 * @brief Construct CliSection type (CLI_SECTION_KIND_BUG).
 */
VARIANT_CONSTRUCTOR(CliSection *, CliSection, bug, String *bug);

/**
 *
 * @brief Construct CliSection type (CLI_SECTION_KIND_DESCRIPTION).
 */
VARIANT_CONSTRUCTOR(CliSection *, CliSection, description, String *description);

/**
 *
 * @brief Construct CliSection type (CLI_SECTION_KIND_HELP).
 */
VARIANT_CONSTRUCTOR(CliSection *, CliSection, help, String *help);

/**
 *
 * @brief Construct CliSection type (CLI_SECTION_KIND_USAGE).
 */
VARIANT_CONSTRUCTOR(CliSection *, CliSection, usage, String *usage);

/**
 *
 * @brief Free CliSection type.
 */
DESTRUCTOR(CliSection, CliSection *self);

#endif // LILY_BASE_CLI_SECTION_H
