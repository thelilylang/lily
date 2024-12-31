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

/*
 * ISO C99 Standard: 7.17 Common definitions <stddef.h>
 */

#ifndef _CC_STDDEF_H
#define _CC_STDDEF_H

#undef NULL
#undef offsetof

#define NULL ((void*)0)
#define offsetof(t, m) __builtin_offsetof(t, m)

#ifdef __PTRDIFF_TYPE__
typedef __PTRDIFF_TYPE__ ptrdiff_t;
#endif /* __PTRDIFF_TYPE__ */

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ size_t;
#endif /* __SIZE_TYPE__ */

#ifdef __WCHAR_TYPE__
typedef __WCHAR_TYPE__ wchar_t;
#endif /* __WCHAR_TYPE__ */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
typedef typeof(nullptr) nullptr_t;
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L */

#endif /* _CC_STDDEF_H */
