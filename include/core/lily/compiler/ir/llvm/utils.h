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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_UTILS_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_UTILS_H

#include <base/platform.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/compiler/ir/llvm.h>
#include <core/lily/compiler/package/package.h>

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
#define OBJ_FORMAT LILY_COMPILER_LINKER_OBJECT_FORMAT_ELF
#elifdef LILY_APPLE_OS
#define OBJ_FORMAT LILY_COMPILER_LINKER_OBJECT_FORMAT_MACHO
#elifdef LILY_WINDOWS_OS
#define OBJ_FORMAT LILY_COMPILER_LINKER_OBJECT_FORMAT_COFF
#else
#error "unknown OS"
#endif

#if defined(LILY_LINUX_OS) || defined(LILY_BSD_OS)
#define LINKER_CMD "ld.lld"
#elifdef LILY_APPLE_OS
#define LINKER_CMD "ld64.lld"
#elifdef LILY_WINDOWS_OS
#define LINKER_CMD "lld-link"
#else
#error "unknown OS"
#endif

// NOTE: As a minimum we use ld version 2.x.x
#define LD_MAJOR_VERSION "2"

#ifdef LILY_LINUX_OS
#ifdef LILY_X86_ARCH
#define DYNAMIC_LINKER "/lib/ld-linux.so." LD_MAJOR_VERSION
#elifdef LILY_X86_64_ARCH
#define DYNAMIC_LINKER "/lib64/ld-linux-x86-64.so." LD_MAJOR_VERSION
#elifdef LILY_ARM64_ARCH
#define DYNAMIC_LINKER "/lib64/ld-linux-aarch64.so." LD_MAJOR_VERSION
#elifdef LILY_ARM_ARCH
#define DYNAMIC_LINKER "/lib/ld-linux-armhd.so." LD_MAJOR_VERSION
#elifdef LILY_PPC64_ARCH
#define DYNAMIC_LINKER "/lib64/ld64.so." LD_MAJOR_VERSION
#elifdef LILY_PPC_ARCH
#define DYNAMIC_LINKER "/lib/ld.so." LD_MAJOR_VERSION
#elifdef LILY_MIPS_ARCH
#define DYNAMIC_LINKER "/lib/ld.so." LD_MAJOR_VERSION
#else
#error "this Linux arch is not yet supported"
#endif
#elifdef LILY_BSD_OS
#ifdef LILY_FREE_BSD_OS
#define DYNAMIC_LINKER "/libexec/ld-elf.so." LD_MAJOR_VERSION
#elifdef LILY_NET_BSD_OS
#define DYNAMIC_LINKER "/libexec/ld.elf_so"
#elifdef LILY_OPEN_BSD_OS
#define DYNAMIC_LINKER "/usr/libexec/ld.so"
#elifdef LILY_DRAGONFLY_OS
#define DYNAMIC_LINKER "/libexec/ld-elf.so." LD_MAJOR_VERSION
#elifdef LILY_BSDI_OS
#error "add your dynamic linker"
#else
#error "unknown BSD"
#endif
#elifdef LILY_APPLE_OS
#define DYNAMIC_LINKER "/usr/lib/dyld"
#elifdef LILY_WINDOWS_OS
#else
#error "unknown OS"
#endif

/**
 *
 * @brief Check if the arg is unique.
 * @param args Vec<char*>*
 */
bool
is_unique_arg__LilyCompilerIrLlvmUtils(Vec *args, char *arg);

/**
 *
 * @brief Print command arguments.
 * @param args Vec<char*>*
 */
void
print_cmd_args__LilyCompilerIrLlvmUtils(const char *cmd, Vec *args);

/**
 *
 * @brief Add all library dependencies.
 */
void
add_lib_dependencies__LilyCompilerIrLlvmUtils(LilyPackage *self, Vec *args);

/**
 *
 * @brief Add all object files.
 */
void
add_object_files__LilyCompilerIrLlvmUtils(LilyPackage *self, Vec *args);

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_UTILS_H
