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

/*
 * ISO C99 Standard: 7.2 Diagnostics <assert.h>
 */

#ifndef _CC_ASSERT_H
#define _CC_ASSERT_H

#include "utils/__attr.h"
#include "utils/__cast.h"
#include "utils/__extern.h"
 
#undef __ASSERT_VOID_CAST
#undef assert
 
#define __ASSERT_VOID_CAST(x) __CAST(x, void)
 
#ifdef NDEBUG
#define assert(ignore) __ASSERT_VOID_CAST(0)
#else
__BEGIN_DECLS
  
__ATTR(noreturn) extern void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function);
  
__ATTR(noreturn) extern void __assert(const char *__assertion, const char *__file, int __line);
  
__END_DECLS
  
#define assert(expr) \
  	(expr) ? __ASSERT_VOID_CAST(0) : __assert_fail(#expr, __FILE__, __LINE__, __func__)
#endif /* NDEBUG */

#endif /* _CC_ASSERT_H */
