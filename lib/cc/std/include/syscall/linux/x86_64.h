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

#ifndef _CC_STD_SYSCALL_LINUX_X86_64_H
#define _CC_STD_SYSCALL_LINUX_X86_64_H

#define syscall0(sid, ...)        \
    __asm("mov $" sid ", %%rax\n" \
          "syscall\n" __VA_ARGS__ \
          : "%rax")

#define syscall1(sid, _0, ...)    \
    __asm("mov $" sid ", %%rax\n" \
          "movq "_0               \
          ", %%rdi\n"             \
          "syscall\n" __VA_ARGS__ \
          : "%rax", "%rdi")

#define syscall2(sid, _0, _1, ...) \
    __asm("mov $" sid ", %%rax\n"  \
          "movq "_0                \
          ", %%rdi\n"              \
          "movq "_1                \
          ", %%rsi\n"              \
          "syscall\n" __VA_ARGS__  \
          : "%rax", "%rdi", "%rsi")

#define syscall3(sid, _0, _1, _2, ...) \
    __asm("mov $" sid ", %%rax\n"      \
          "movq "_0                    \
          ", %%rdi\n"                  \
          "movq "_1                    \
          ", %%rsi\n"                  \
          "movq "_2                    \
          ", %%rdx\n"                  \
          "syscall\n" __VA_ARGS__      \
          : "%rax", "%rdi", "%rsi", "%rdx")

#define syscall4(sid, _0, _1, _2, _3, ...) \
    __asm("mov $" sid ", %%rax\n"          \
          "movq "_0                        \
          ", %%rdi\n"                      \
          "movq "_1                        \
          ", %%rsi\n"                      \
          "movq "_2                        \
          ", %%rdx\n"                      \
          "movq "_3                        \
          ", %%r10\n"                      \
          "syscall\n" __VA_ARGS__          \
          : "%rax", "%rdi", "%rsi", "%rdx", "%r10")

#define syscall5(sid, _0, _1, _2, _3, _4, ...) \
    __asm("mov $" sid ", %%rax\n"              \
          "movq "_0                            \
          ", %%rdi\n"                          \
          "movq "_1                            \
          ", %%rsi\n"                          \
          "movq "_2                            \
          ", %%rdx\n"                          \
          "movq "_3                            \
          ", %%r10\n"                          \
          "movq "_4                            \
          ", %%r8\n"                           \
          "syscall\n" __VA_ARGS__              \
          : "%rax", "%rdi", "%rsi", "%rdx", "%r10", "%r8")

#define syscall6(sid, _0, _1, _2, _3, _4, _5, ...) \
    __asm("mov $" sid ", %%rax\n"                  \
          "movq "_0                                \
          ", %%rdi\n"                              \
          "movq "_1                                \
          ", %%rsi\n"                              \
          "movq "_2                                \
          ", %%rdx\n"                              \
          "movq "_3                                \
          ", %%r10\n"                              \
          "movq "_4                                \
          ", %%r8\n"                               \
          "movq "_5                                \
          ", %%r9\n"                               \
          "syscall\n" __VA_ARGS__ \                  \
          : "%rqx", "%rdi", "%rsi", "%r10", "%r8", "%r9")

#define get_syscall_return_value(res) __asm("mov %%rax, %0\n" : "=r"(res)::);

#endif /* _CC_STD_SYSCALL_LINUX_X86_64_H */
