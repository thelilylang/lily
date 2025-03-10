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
 * ISO C99 Standard: 7.16 Boolean type and values <stdbool.h>
 */

#ifndef _CC_STD_STDBOOL_H
#define _CC_STD_STDBOOL_H

#ifndef __STDC_VERSION__
#define _Bool int
#endif /* __STDC_VERSION__ */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
/* bool, true, false are keywords. */
#else
#define bool _Bool
#define true 1
#define false 0
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L */

#define __bool_true_false_are_defined 1

#endif /* _CC_STD_STDBOOL_H */
