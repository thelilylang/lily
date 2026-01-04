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

#ifndef LILY_CORE_CC_CI_INCLUDE_H
#define LILY_CORE_CC_CI_INCLUDE_H

#include <base/platform.h>
#include <base/vec.h>

#include <core/cc/ci/project_config.h>

/**
 *
 * @brief Initialize include directories vector.
 * @param compiler_command const String* (&)
 */
void
init_include_dirs__CIInclude(const String *compiler_command,
                             const char *base_path);

/**
 *
 * @brief Add include directory to `include_dirs` vector.
 */
void
add_include_dir__CIInclude(String *include_dir);

/**
 *
 * @brief Insert include directory to `include_dirs` vector at index.
 */
void
insert_include_dir__CIInclude(String *include_dir, Usize index);

/**
 *
 * @brief Get `include_dirs` vector.
 * @return Vec<String*>* (&)
 */
const Vec *
get_include_dirs__CIInclude();

/**
 *
 * @brief Free `include_dirs` vector.
 * @see src/core/cc/ci/include.c
 */
void
destroy__CIInclude();

#endif // LILY_CORE_CC_CI_INCLUDE_H
