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

#ifndef _CC_STD_UTILS__THREAD_LOCAL_H
#define _CC_STD_UTILS__THREAD_LOCAL_H

#undef __thread_local

#ifdef __STDC_VERSION__
#if __STDC_VERSION__ == 201112L
#define __thread_local _Thread_local
#endif /* __STDC_VERSION__ == 201112L */

#if __STDC_VERSION__ > 201710L
#define __thread_local thread_local
#endif /* __STDC_VERSION__ > 201710L */
#endif /* __STDC_VERSION__ */

#ifndef __thread_local
#define __thread_local
#endif

#endif /* _CC_STD_UTILS__THREAD_LOCAL_H */
