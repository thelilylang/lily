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

/*
 * ISO C11 Standard: 7.15 Alignment <stdalign.h>
 * ISO C23 Standard: 7.15 Alignment <stdalign.h>
 */

#ifndef _CC_STD_STDALIGN_H
#define _CC_STD_STDALIGN_H

/* Since C23 `alignas` and `alignof` are keywords, and the header provides no
   content. Before that, it is what spells them. */
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L
#undef alignas
#undef alignof

#define alignas _Alignas
#define alignof _Alignof

#define __alignas_is_defined 1
#define __alignof_is_defined 1
#endif /* !defined(__STDC_VERSION__) || __STDC_VERSION__ <= 201710L */

#endif /* _CC_STD_STDALIGN_H */
