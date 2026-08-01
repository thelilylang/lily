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
 * ISO C99 Standard: 7.14 Signal handling <signal.h>
 */

#ifndef _CC_STD_SIGNAL_H
#define _CC_STD_SIGNAL_H

#include <platform.h>
#include <utils/__extern.h>

__BEGIN_DECLS

/* 7.14: an integer type an object of which is accessed as an atomic entity,
   even where an asynchronous signal is taken. */
typedef int sig_atomic_t;

typedef void (*__sighandler_t)(int);

/* The three values a handler is never written as, since each stands for what
   the implementation does with the signal itself. */
#define SIG_ERR ((__sighandler_t)-1)
#define SIG_DFL ((__sighandler_t)0)
#define SIG_IGN ((__sighandler_t)1)

#ifdef _CC_STD_LINUX
/* 7.14: the signals the standard gives, with the numbers the platform names
   them by. */
#define SIGINT 2
#define SIGILL 4
#define SIGABRT 6
#define SIGFPE 8
#define SIGSEGV 11
#define SIGTERM 15
#endif /* _CC_STD_LINUX */

extern void (*signal(int sig, void (*func)(int)))(int);
extern int
raise(int sig);

__END_DECLS

#endif /* _CC_STD_SIGNAL_H */
