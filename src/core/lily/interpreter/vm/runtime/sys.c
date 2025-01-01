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

#include <core/lily/interpreter/vm/runtime/sys.h>

#include <sys/sys.h>

LilyInterpreterValue
read__LilyInterpreterVMRuntimeSys(LilyInterpreterValue fd,
                                  LilyInterpreterValue buf,
                                  LilyInterpreterValue n)
{
    Usize res = __sys__$read(fd.int32, buf.ptr, n.usize);

    FREE(LilyInterpreterValue, &fd);
    FREE(LilyInterpreterValue, &buf);
    FREE(LilyInterpreterValue, &n);

    return NEW_VARIANT(LilyInterpreterValue, usize, res);
}

LilyInterpreterValue
write__LilyInterpreterVMRuntimeSys(LilyInterpreterValue fd,
                                   LilyInterpreterValue buf,
                                   LilyInterpreterValue n)
{
    Usize res = __sys__$write(fd.int32, buf.cstr, n.usize);

    FREE(LilyInterpreterValue, &fd);
    FREE(LilyInterpreterValue, &buf);
    FREE(LilyInterpreterValue, &n);

    return NEW_VARIANT(LilyInterpreterValue, usize, res);
}

LilyInterpreterValue
open__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname,
                                  LilyInterpreterValue flags,
                                  LilyInterpreterValue mode)
{
    Int32 res = __sys__$open(pathname.cstr, flags.int32, mode.int32);

    FREE(LilyInterpreterValue, &pathname);
    FREE(LilyInterpreterValue, &flags);
    FREE(LilyInterpreterValue, &mode);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}

LilyInterpreterValue
close__LilyInterpreterVMRuntimeSys(LilyInterpreterValue fd)
{
    Int32 res = __sys__$close(fd.int32);

    FREE(LilyInterpreterValue, &fd);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}

LilyInterpreterValue
stat_mode__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Int32 res = __sys__$stat_mode(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}

LilyInterpreterValue
stat_ino__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Uint32 res = __sys__$stat_ino(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, uint32, res);
}

LilyInterpreterValue
stat_dev__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Uint32 res = __sys__$stat_dev(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, uint32, res);
}

LilyInterpreterValue
stat_nlink__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Uint32 res = __sys__$stat_nlink(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, uint32, res);
}

LilyInterpreterValue
stat_uid__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Uint64 res = __sys__$stat_uid(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, uint64, res);
}

LilyInterpreterValue
stat_gid__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Uint64 res = __sys__$stat_gid(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, uint64, res);
}

LilyInterpreterValue
stat_size__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Int32 res = __sys__$stat_size(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}

LilyInterpreterValue
stat_atime__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Int32 res = __sys__$stat_atime(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}

LilyInterpreterValue
stat_mtime__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Int32 res = __sys__$stat_mtime(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}

LilyInterpreterValue
stat_ctime__LilyInterpreterVMRuntimeSys(LilyInterpreterValue pathname)
{
    Int32 res = __sys__$stat_ctime(pathname.cstr);

    FREE(LilyInterpreterValue, &pathname);

    return NEW_VARIANT(LilyInterpreterValue, int32, res);
}
