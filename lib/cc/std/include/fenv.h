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
 * ISO C99 Standard: 7.6 Floating-point environment <fenv.h>
 */

#ifndef _CC_STD_FENV_H
#define _CC_STD_FENV_H

#include <platform.h>
#include <utils/__extern.h>

__BEGIN_DECLS

#ifdef _CC_STD_LINUX
/* 7.6: the whole of the floating-point environment, and the flags on their
   own. Both are written the way the C library lays them out, since it is that
   one that reads and writes what is handed to it. */
typedef unsigned short int fexcept_t;

typedef struct
{
    unsigned short int __control_word;
    unsigned short int __glibc_reserved1;
    unsigned short int __status_word;
    unsigned short int __glibc_reserved2;
    unsigned short int __tags;
    unsigned short int __glibc_reserved3;
    unsigned int __eip;
    unsigned short int __cs_selector;
    unsigned int __opcode : 11;
    unsigned int __glibc_reserved4 : 5;
    unsigned int __data_offset;
    unsigned short int __data_selector;
    unsigned short int __glibc_reserved5;
#ifdef _CC_STD_X86_64
    unsigned int __mxcsr;
#endif /* _CC_STD_X86_64 */
} fenv_t;

/* 7.6: the exceptions the implementation raises, each a bit of its own, and
   FE_ALL_EXCEPT the whole of them. */
#define FE_INVALID 0x01
#define FE_DIVBYZERO 0x04
#define FE_OVERFLOW 0x08
#define FE_UNDERFLOW 0x10
#define FE_INEXACT 0x20

#define FE_ALL_EXCEPT \
    (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* 7.6: the rounding directions fegetround gives back and fesetround is
   written with. */
#define FE_TONEAREST 0
#define FE_DOWNWARD 0x400
#define FE_UPWARD 0x800
#define FE_TOWARDZERO 0xc00

/* The environment the program starts under. */
#define FE_DFL_ENV ((const fenv_t *)-1)
#endif /* _CC_STD_LINUX */

/* 7.6.2 Floating-point exceptions */
extern int
feclearexcept(int excepts);
extern int
fegetexceptflag(fexcept_t *flagp, int excepts);
extern int
feraiseexcept(int excepts);
extern int
fesetexceptflag(const fexcept_t *flagp, int excepts);
extern int
fetestexcept(int excepts);

/* 7.6.3 Rounding */
extern int
fegetround(void);
extern int
fesetround(int round);

/* 7.6.4 Environment */
extern int
fegetenv(fenv_t *envp);
extern int
feholdexcept(fenv_t *envp);
extern int
fesetenv(const fenv_t *envp);
extern int
feupdateenv(const fenv_t *envp);

__END_DECLS

#endif /* _CC_STD_FENV_H */
