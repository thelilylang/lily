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

#ifndef _CC_STD_ERRNO_LINUX_H
#define _CC_STD_ERRNO_LINUX_H

/* https://chromium.googlesource.com/chromiumos/docs/+/master/constants/errnos.md
 */

#define EPERM 0x01   /* Operation not permitted */
#define ENOENT 0x02  /* No such file or directory */
#define ESRCH 0x03   /* No such process */
#define EINTR 0x04   /* Interrupted system call */
#define EIO 0x05     /* Input/output error */
#define ENXIO 0x06   /* No such device or address */
#define E2BIG 0x07   /* Argument list too long */
#define ENOEXEC 0x08 /* Exec format error */
#define EBADF 0x09   /* Bad file descriptor */
#define ECHILD 0x0A  /* No child processes */
#define EAGAIN 0x0B  /* Resource temporarily unavailable */
#define EWOULDBLOCK \
    0x0B /* (Same value as EAGAIN) Resource temporarily unavailable */
#define ENOMEM 0x0C    /* Cannot allocate memory */
#define EACCES 0x0D    /* Permission denied */
#define EFAULT 0x0E    /* Bad address */
#define ENOTBLK 0x0F   /* Block device required */
#define EBUSY 0x10     /* Device or resource busy */
#define EEXIST 0x11    /* File exists */
#define EXDEV 0x12     /* Invalid cross-device link */
#define ENODEV 0x13    /* No such device */
#define ENOTDIR 0x14   /* Not a directory */
#define EISDIR 0x15    /* Is a directory */
#define EINVAL 0x16    /* Invalid argument */
#define ENFILE 0x17    /* Too many open files in system */
#define EMFILE 0x18    /* Too many open files */
#define ENOTTY 0x19    /* Inappropriate ioctl for device */
#define ETXTBSY 0x1A   /* Text file busy */
#define EFBIG 0x1B     /* File too large */
#define ENOSPC 0x1C    /* No space left on device */
#define ESPIPE 0x1D    /* Illegal seek */
#define EROFS 0x1E     /* Read-only file system */
#define EMLINK 0x1F    /* Too many links */
#define EPIPE 0x20     /* Broken pipe */
#define EDOM 0x21      /* Numerical argument out of domain */
#define ERANGE 0x22    /* Numerical result out of range */
#define EDEADLK 0x23   /* Resource deadlock avoided */
#define EDEADLOCK 0x23 /* (Same value as EDEADLK) Resource deadlock avoided */
#define ENAMETOOLONG 0x24 /* File name too long */
#define ENOLCK 0x25       /* No locks available */
#define ENOSYS 0x26       /* Function not implemented */
#define ENOTEMPTY 0x27    /* Directory not empty */
#define ELOOP 0x28        /* Too many levels of symbolic links */
/* 0x29 not implemented */
#define ENOMSG 0x2A   /* No message of desired type */
#define EIDRM 0x2B    /* Identifier removed */
#define ECHRNG 0x2C   /* Channel number out of range */
#define EL2NSYNC 0x2D /* Level 2 not synchronized */
#define EL3HLT 0x2E   /* Level 3 halted */
#define EL3RST 0x2F   /* Level 3 reset */
#define ELNRNG 0x30   /* Link number out of range */
#define EUNATCH 0x31  /* Protocol driver not attached */
#define ENOCSI 0x32   /* No CSI structure available */
#define EL2HLT 0x33   /* Level 2 halted */
#define EBADE 0x34    /* Invalid exchange */
#define EBADR 0x35    /* Invalid request descriptor */
#define EXFULL 0x36   /* Exchange full */
#define ENOANO 0x37   /* No anode */
#define EBADRQC 0x38  /* Invalid request code */
#define EBADSLT 0x39  /* Invalid slot */
/* 0x3A not implemented */
#define EBFONT 0x3B    /* Bad font file format */
#define ENOSTR 0x3C    /* Device not a stream */
#define ENODATA 0x3D   /* No data available */
#define ETIME 0x3E     /* Timer expired */
#define ENOSR 0x3F     /* Out of streams resources */
#define ENONET 0x40    /* Machine is not on the network */
#define ENOPKG 0x41    /* Package not installed */
#define EREMOTE 0x42   /* Object is remote */
#define ENOLINK 0x43   /* Link has been severed */
#define EADV 0x44      /* Advertise error */
#define ESRMNT 0x45    /* Srmount error */
#define ECOMM 0x46     /* Communication error on send */
#define EPROTO 0x47    /* Protocol error */
#define EMULTIHOP 0x48 /* Multihop attempted */
#define EDOTDOT 0x49   /* RFS specific error */
#define EBADMSG 0x4A   /* Bad message */
#define EOVERFLOW 0x4B /* Value too large for defined data type */
#define ENOTUNIQ 0x4C  /* Name not unique on network */
#define EBADFD 0x4D    /* File descriptor in bad state */
#define EREMCHG 0x4E   /* Remote address changed */
#define ELIBACC 0x4F   /* Can not access a needed shared library */
#define ELIBBAD 0x50   /* Accessing a corrupted shared library */
#define ELIBSCN 0x51   /* .lib section in a.out corrupted */
#define ELIBMAX 0x52   /* Attempting to link in too many shared libraries */
#define ELIBEXEC 0x53  /* Cannot exec a shared library directly */
#define EILSEQ 0x54    /* Invalid or incomplete multibyte or wide character */
#define ERESTART 0x55  /* Interrupted system call should be restarted */
#define ESTRPIPE 0x56  /* Streams pipe error */
#define EUSERS 0x57    /* Too many users */
#define ENOTSOCK 0x58  /* Socket operation on non-socket */
#define EDESTADDRREQ 0x59    /* Destination address required */
#define EMSGSIZE 0x5A        /* Message too long */
#define EPROTOTYPE 0x5B      /* Protocol wrong type for socket */
#define ENOPROTOOPT 0x5C     /* Protocol not available */
#define EPROTONOSUPPORT 0x5D /* Protocol not supported */
#define ESOCKTNOSUPPORT 0x5E /* Socket type not supported */
#define EOPNOTSUPP 0x5F      /* Operation not supported */
#define ENOTSUP 0x5F /* (Same value as EOPNOTSUPP) Operation not supported */
#define EPFNOSUPPORT 0x60    /* Protocol family not supported */
#define EAFNOSUPPORT 0x61    /* Address family not supported by protocol */
#define EADDRINUSE 0x62      /* Address already in use */
#define EADDRNOTAVAIL 0x63   /* Cannot assign requested address */
#define ENETDOWN 0x64        /* Network is down */
#define ENETUNREACH 0x65     /* Network is unreachable */
#define ENETRESET 0x66       /* Network dropped connection on reset */
#define ECONNABORTED 0x67    /* Software caused connection abort */
#define ECONNRESET 0x68      /* Connection reset by peer */
#define ENOBUFS 0x69         /* No buffer space available */
#define EISCONN 0x6A         /* Transport endpoint is already connected */
#define ENOTCONN 0x6B        /* Transport endpoint is not connected */
#define ESHUTDOWN 0x6C       /* Cannot send after transport endpoint shutdown */
#define ETOOMANYREFS 0x6D    /* Too many references: cannot splice */
#define ETIMEDOUT 0x6E       /* Connection timed out */
#define ECONNREFUSED 0x6F    /* Connection refused */
#define EHOSTDOWN 0x70       /* Host is down */
#define EHOSTUNREACH 0x71    /* No route to host */
#define EALREADY 0x72        /* Operation already in progress */
#define EINPROGRESS 0x73     /* Operation now in progress */
#define ESTALE 0x74          /* Stale file handle */
#define EUCLEAN 0x75         /* Structure needs cleaning */
#define ENOTNAM 0x76         /* Not a XENIX named type file */
#define ENAVAIL 0x77         /* No XENIX semaphores available */
#define EISNAM 0x78          /* Is a named type file */
#define EREMOTEIO 0x79       /* Remote I/O error */
#define EDQUOT 0x7A          /* Disk quota exceeded */
#define ENOMEDIUM 0x7B       /* No medium found */
#define EMEDIUMTYPE 0x7C     /* Wrong medium type */
#define ECANCELED 0x7D       /* Operation canceled */
#define ENOKEY 0x7E          /* Required key not available */
#define EKEYEXPIRED 0x7F     /* Key has expired */
#define EKEYREVOKED 0x80     /* Key has been revoked */
#define EKEYREJECTED 0x81    /* Key was rejected by service */
#define EOWNERDEAD 0x82      /* Owner died */
#define ENOTRECOVERABLE 0x83 /* State not recoverable */
#define ERFKILL 0x84         /* Operation not possible due to RF-kill */
#define EHWPOISON 0x85       /* Memory page has hardware error */

#endif /* _CC_STD_ERRNO_LINUX_H */
