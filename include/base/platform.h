/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_BASE_PLATFORM_H
#define LILY_BASE_PLATFORM_H

#define MACOS_SDK_VERSION "15"
#define MIN_MACOS_VERSION \
    "12" // NOTE: maybe upgrade/downgrade the minimum required version of MacOS

#if defined(__x86_64__) || defined(_WIN64) || defined(__arm64__) || \
  defined(__ppc64__)
#define PLATFORM_64
#else
#define PLATFORM_32
#endif

#if defined(__unix__) || defined(__unix) || \
  (defined(__APPLE__) && defined(__MACH__))
#define LILY_UNIX_OS
#endif

#if defined(__linux__)
#define OS_NAME "linux"
#define LILY_LINUX_OS
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
  defined(__bsdi__) || defined(__DragonFly__)
#define OS_NAME "BSD"
#define LILY_BSD_OS
#ifdef __FreeBSD__
#define BSD_NAME "Free"
#define LILY_FREE_BSD_OS
#elif defined(__NetBSD__)
#define BSD_NAME "Net"
#define LILY_NET_BSD_OS
#elif defined(__OpenBSD__)
#define BSD_NAME "Open"
#define LILY_OPEN_BSD_OS
#elif defined(__bsdi__)
#define BSD_NAME "BSDI"
#define LILY_BSDI_OS
#elif defined(__DragonFly__)
#define BSD_NAME "DragonFly"
#define LILY_DRAGONFLY_OS
#else
#error "unreachable: unknown BSD"
#endif
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_NAME "macos"
#define LILY_APPLE_OS
#elif defined(_WIN32) || defined(_WIN64)
#define OS_NAME "windows"
#define LILY_WINDOWS_OS
#else
#define OS_NAME "unknown"
#define LILY_UNKNOWN_OS
#endif

#if defined(__x86_64__)
#define ARCH_NAME "x86_64"
#define LILY_X86_64_ARCH
#elif defined(__i386__)
#define ARCH_NAME "x86"
#define LILY_X86_ARCH
#elif defined(__arm64__)
#define ARCH_NAME "arm64"
#define LILY_ARM64_ARCH
#elif defined(__arm__)
#define ARCH_NAME "arm"
#define LILY_ARM_ARCH
#elif defined(__ppc64_)
#define ARCH_NAME "ppc64"
#define LILY_PPC64_ARCH
#elif defined(__ppc__)
#define ARCH_NAME "ppc"
#define LILY_PPC_ARCH
#elif defined(__mips__)
#define ARCH_NAME "mips"
#define LILY_MIPS_ARCH
#else
#define ARCH_NAME "unknown"
#define LILY_UNKNOWN_ARCH
#endif

#endif // LILY_BASE_PLATFORM_H
