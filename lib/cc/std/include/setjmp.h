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
 * ISO C99 Standard: 7.13 Nonlocal jumps <setjmp.h>
 */

#ifndef _CC_STD_SETJMP_H
#define _CC_STD_SETJMP_H

#include <platform.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.13: jmp_buf is an array type holding what is needed to restore a calling
   environment. What is held is the machine's to say, so this is written the
   way the C library of the platform lays it out. */
#ifdef _CC_STD_LINUX
#ifdef _CC_STD_X86_64
typedef long int __jmp_buf[8];
#else
typedef int __jmp_buf[6];
#endif /* _CC_STD_X86_64 */

/* The signal mask is written after what the jump itself needs, and is only
   saved where the call asks for it. */
#define _CC_STD_SIGSET_NWORDS (1024 / (8 * sizeof(unsigned long int)))

struct __jmp_buf_tag
{
    __jmp_buf __jmpbuf;
    int __mask_was_saved;
    unsigned long int __saved_mask[_CC_STD_SIGSET_NWORDS];
};

typedef struct __jmp_buf_tag jmp_buf[1];

extern int
_setjmp(struct __jmp_buf_tag __env[1]);
extern int
__sigsetjmp(struct __jmp_buf_tag __env[1], int __savemask);

/* 7.13.1.1: setjmp is a macro, and what it is written in may only be one of
   the four contexts the standard gives it. */
#define setjmp(env) _setjmp(env)

extern void
longjmp(struct __jmp_buf_tag __env[1], int __val);
#endif /* _CC_STD_LINUX */

__END_DECLS

#endif /* _CC_STD_SETJMP_H */
