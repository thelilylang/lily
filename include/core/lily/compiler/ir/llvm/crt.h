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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_CRT_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_CRT_H

#include <base/platform.h>

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)

/**
 *
 * @brief Get the path of crt0 library.
 * @return const char*?
 */
const char *
get_crt0_library_path__LilyIrLlvmLinker();

/**
 *
 * @brief Get the path of crt1 library.
 * @return const char*?
 */
const char *
get_crt1_library_path__LilyIrLlvmLinker();

/**
 *
 * @brief Get the path of crti library.
 * @return const char*
 */
const char *
get_crti_library_path__LilyIrLlvmLinker();

/**
 *
 * @brief Get the path of crtn library.
 * @return const char*
 */
const char *
get_crtn_library_path__LilyIrLlvmLinker();

/**
 *
 * @brief Destroy static crt variables.
 */
void
destroy_crt__LilyIrLlvmLinker();

#endif

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_CRT_H
