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

#ifndef LILY_CORE_LILY_INTERPRETER_RUNTIME_SYS_H
#define LILY_CORE_LILY_INTERPRETER_RUNTIME_SYS_H

#include <core/lily/interpreter/vm/value.h>

/**
 *
 * @brief Call read system call with runtime values.
 */
LilyInterpreterValue
read__LilyInterpreterVMRuntimeSys(LilyInterpreterValue fd,
                                  LilyInterpreterValue buf,
                                  LilyInterpreterValue n);

/**
 *
 * @brief Call write system call with runtime values.
 */
LilyInterpreterValue
write__LilyInterpreterVMRuntimeSys(LilyInterpreterValue fd,
                                   LilyInterpreterValue buf,
                                   LilyInterpreterValue n);

/**
 *
 * @brief Call open system call with runtime values.
 */
LilyInterpreterValue
open__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname,
                                  LilyInterpreterValue flags,
                                  LilyInterpreterValue mode);

/**
 *
 * @brief Call close system call with runtime values.
 */
LilyInterpreterValue
close__LilyInterpreterVMRuntimeSys(LilyInterpreterValue fd);

/**
 *
 * @brief Call stat (mode) system call with runtime values.
 */
LilyInterpreterValue
stat_mode__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (ino) system call with runtime values.
 */
LilyInterpreterValue
stat_ino__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (dev) system call with runtime values.
 */
LilyInterpreterValue
stat_dev__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (nlink) system call with runtime values.
 */
LilyInterpreterValue
stat_nlink__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (uid) system call with runtime values.
 */
LilyInterpreterValue
stat_uid__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (gid) system call with runtime values.
 */
LilyInterpreterValue
stat_gid__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (size) system call with runtime values.
 */
LilyInterpreterValue
stat_size__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (atime) system call with runtime values.
 */
LilyInterpreterValue
stat_atime__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (mtime) system call with runtime values.
 */
LilyInterpreterValue
stat_mtime__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

/**
 *
 * @brief Call stat (ctime) system call with runtime values.
 */
LilyInterpreterValue
stat_ctime__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname);

#endif // LILY_CORE_LILY_INTERPRETER_RUNTIME_SYS_H
