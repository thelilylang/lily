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

#ifndef _CC_STD_SYSCALL_LINUX_X86_H
#define _CC_STD_SYSCALL_LINUX_X86_H

#define syscall0(sid, ...)         \
    __asm("movl $" sid ", %%eax\n" \
          "int 80h\n" __VA_ARGS__  \
          : "%eax")

#define syscall1(sid, _0, ...)     \
    __asm("movl $" sid ", %%eax\n" \
          "movl "_0                \
          ", %%ebx\n"              \
          "int 80h\n" __VA_ARGS__  \
          : "%eax", "%ebx")

#define syscall2(sid, _0, _1, ...) \
    __asm("movl $" sid ", %%eax\n" \
          "movl "_0                \
          ", %%ebx\n"              \
          "movl "_1                \
          ", %%ecx\n"              \
          "int 80h\n" __VA_ARGS__  \
          : "%eax", "%ebx", "%ecx")

#define syscall3(sid, _0, _1, _2, ...) \
    __asm("movl $" sid ", %%eax\n"     \
          "movl "_0                    \
          ", %%ebx\n"                  \
          "movl "_1                    \
          ", %%ecx\n"                  \
          "movl "_2                    \
          ", %%edx\n"                  \
          "int 80h\n" __VA_ARGS__      \
          : "%eax", "%ebx", "%ecx", "%edx")

#define syscall4(sid, _0, _1, _2, _3, ...) \
    __asm("movl $" sid ", %%eax\n"         \
          "movl "_0                        \
          ", %%ebx\n"                      \
          "movl "_1                        \
          ", %%ecx\n"                      \
          "movl "_2                        \
          ", %%edx\n"                      \
          "movl "_3                        \
          ", %%esi\n"                      \
          "int 80h\n" __VA_ARGS__          \
          : "%eax", "%ebx", "%ecx", "%edx", "%esi")

#define syscall5(sid, _0, _1, _2, _3, _4, ...) \
    __asm("movl $" sid ", %%eax\n"             \
          "movl "_0                            \
          ", %%ebx\n"                          \
          "movl "_1                            \
          ", %%ecx\n"                          \
          "movl "_2                            \
          ", %%edx\n"                          \
          "movl "_3                            \
          ", %%esi\n"                          \
          "movl "_4                            \
          ", %%edi\n"                          \
          "int 80h\n" __VA_ARGS__              \
          : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi")

#define syscall6(sid, _0, _1, _2, _3, _4, _5, ...) \
    __asm("movl $" sid ", %%eax\n"                 \
          "movl "_0                                \
          ", %%ebx\n"                              \
          "movl "_1                                \
          ", %%ecx\n"                              \
          "movl "_2                                \
          ", %%edx\n"                              \
          "movl "_3                                \
          ", %%esi\n"                              \
          "movl "_4                                \
          ", %%ebp\n"                              \
          "int 80h\n" __VA_ARGS__                  \
          : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%ebp")

#define get_syscall_return_value(res) __asm("mov %%eax, %0\n" : "=r"(res)::);

#endif /* _CC_STD_SYSCALL_LINUX_X86_H */
