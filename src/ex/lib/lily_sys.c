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

#ifndef LILY_EX_LIB_LILY_SYS_C
#define LILY_EX_LIB_LILY_SYS_C

#include <sys/sys.h>

// <sys/sys.h>
extern Usize
__sys__$read(Int32 fd, void *buf, Usize n) __attribute__((always_inline))
__attribute__((hot));

extern Usize
__sys__$write(Int32 fd, const char *buf, Usize n) __attribute__((always_inline))
__attribute__((hot));

extern Int32
__sys__$open(const char *pathname, Int32 flags, Int32 mode)
  __attribute__((always_inline)) __attribute__((hot));

extern Int32
__sys__$close(Int32 fd) __attribute__((always_inline)) __attribute__((hot));

extern Int32
__sys__$stat_mode(const char *pathname);

extern Uint32
__sys__$stat_ino(const char *pathname);

extern Uint32
__sys__$stat_dev(const char *pathname);

extern Uint32
__sys__$stat_nlink(const char *pathname);

extern Uint64
__sys__$stat_uid(const char *pathname);

extern Uint64
__sys__$stat_gid(const char *pathname);

extern Int32
__sys__$stat_size(const char *pathname);

extern Int32
__sys__$stat_atime(const char *pathname);

extern Int32
__sys__$stat_mtime(const char *pathname);

extern Int32
__sys__$stat_ctime(const char *pathname);

#endif // LILY_EX_LIB_LILY_SYS_C