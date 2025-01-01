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

#include <base/platform.h>

#ifdef LILY_WINDOWS_OS
#include <io.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>

#include <sys/sys.h>

#ifdef LILY_WINDOWS_OS
struct _stat st;
#else
struct stat st;
#endif

#ifdef LILY_WINDOWS_OS
Usize
__sys__$read(Int32 fd, void *buf, Usize n)
{
    return (Usize)_read(fd, buf, n);
}
#else
Usize
__sys__$read(Int32 fd, void *buf, Usize n)
{
    return read(fd, buf, n);
}
#endif

#ifdef LILY_WINDOWS_OS
Usize
__sys__$write(Int32 fd, const char *buf, Usize n)
{
    return (Usize)_write(fd, buf, n);
}
#else
Usize
__sys__$write(Int32 fd, const char *buf, Usize n)
{
    return write(fd, buf, n);
}
#endif

#ifdef LILY_WINDOWS_OS
Int32
__sys__$open(const char *pathname, Int32 flags, Int32 mode)
{
    return _open(pathname, flags, mode);
}
#else
Int32
__sys__$open(const char *pathname, Int32 flags, Int32 mode)
{
    return open(pathname, flags, mode);
}
#endif

#ifdef LILY_WINDOWS_OS
Int32
__sys__$close(Int32 fd)
{
    return _close(fd);
}
#else
Int32
__sys__$close(Int32 fd)
{
    return close(fd);
}
#endif

#ifdef LILY_WINDOWS_OS
Int32
__sys__$stat_mode(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_mode;
}

Uint32
__sys__$stat_ino(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_ino;
}

Uint32
__sys__$stat_dev(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_dev;
}

Uint32
__sys__$stat_nlink(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_nlink;
}

Uint64
__sys__$stat_uid(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_uid;
}

Uint64
__sys__$stat_gid(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_gid;
}

Int32
__sys__$stat_size(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_size;
}

Int32
__sys__$stat_atime(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_atime;
}

Int32
__sys__$stat_mtime(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_mtime;
}

Int32
__sys__$stat_ctime(const char *pathname)
{
    _stat(pathname, &st);
    return st.st_ctime;
}
#else
Int32
__sys__$stat_mode(const char *pathname)
{
    stat(pathname, &st);
    return (Int32)st.st_mode;
}

Uint32
__sys__$stat_ino(const char *pathname)
{
    stat(pathname, &st);
    return st.st_ino;
}

Uint32
__sys__$stat_dev(const char *pathname)
{
    stat(pathname, &st);
    return st.st_dev;
}

Uint32
__sys__$stat_nlink(const char *pathname)
{
    stat(pathname, &st);
    return st.st_nlink;
}

Uint64
__sys__$stat_uid(const char *pathname)
{
    stat(pathname, &st);
    return st.st_uid;
}

Uint64
__sys__$stat_gid(const char *pathname)
{
    stat(pathname, &st);
    return st.st_gid;
}

Int32
__sys__$stat_size(const char *pathname)
{
    stat(pathname, &st);
    return st.st_size;
}

Int32
__sys__$stat_atime(const char *pathname)
{
    stat(pathname, &st);
    return st.st_atime;
}

Int32
__sys__$stat_mtime(const char *pathname)
{
    stat(pathname, &st);
    return st.st_mtime;
}

Int32
__sys__$stat_ctime(const char *pathname)
{
    stat(pathname, &st);
    return st.st_ctime;
}
#endif
