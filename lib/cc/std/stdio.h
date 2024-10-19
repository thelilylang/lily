/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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
 * ISO C99 Standard: 7.19 Input/output <stdio.h> 
 */

#ifndef _CC_STDIO_H
#define _CC_STDIO_H

#define _STDIO_H

#include "stddef.h"
#include "stdarg.h"

#define __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION

#include <bits/libc-header-start.h>

#include <bits/types.h>
#include <bits/types/__fpos_t.h>
#include <bits/types/__fpos64_t.h>
#include <bits/types/__FILE.h>
#include <bits/types/struct_FILE.h>
#include <bits/types/FILE.h>

#include "utils/__extern.h"

typedef __FILE FILE;
typedef __fpos_t fpos_t;
typedef __fpos64_t fpos64_t;

#undef _IOFBF
#undef _IOLBF
#undef _IONBF

#define _IOFBF 0 /* Fully buffered. */
#define _IOLBF 1 /* Line buffered. */
#define _IONBF 2 /* No buffering. */

#undef BUFSIZ

#define BUFSIZ 8192 /* Default buffer size. */

#undef EOF

/* The value returned by fgetc and similar functions to indicate the
   end of the file. */
#define EOF (-1)

#undef FOPEN_MAX

#define FOPEN_MAX 16

/* #define FILENAME_MAX ... */
#include <bits/stdio_lim.h>

#undef L_tmpnam

#define L_tmpnam 20

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END

#define SEEK_SET 0 /* Seek from beginning of file. */
#define SEEK_CUR 1 /* Seek from current position. */
#define SEEK_END 2 /* Seek from end of file. */

#undef TMP_MAX

#define TMP_MAX 238328

#undef stdin
#undef stdout
#undef stderr

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

__BEGIN_DECLS

extern void clearerr(FILE *stream);
extern int fclose(FILE *stream);
extern int ferror(FILE *stream);
extern int feof(FILE *stream);
extern int fflush(FILE *stream);
extern int fgetc(FILE *stream);
extern int fgetpos(FILE * /* restrict */ stream, fpos_t * /* restrict */ pos);
extern char *fgets(char * /* restrict */ s, int n, FILE * /* restrict */ stream);
extern FILE *fopen(const char * /* restrict */ filename, const char * /* restrict */ mode);
extern int fprintf(FILE * /* restrict */ stream, const char * /* restrict */ format, ...);
extern int fputc(int c, FILE *stream);
extern int fputs(const char * /* restrict */ s, FILE * /* restrict */ stream);
extern size_t fread(void * /* restrict */ ptr, size_t size, size_t nmemb, FILE * /* restrict */ stream);
extern FILE *freopen(const char * /* restrict */ filename, const char * /* restrict */ mode, FILE * /* restrict */ stream);
extern int fscanf(FILE * /* restrict */ stream, const char * /* restrict */ format, ...);
extern int fseek(FILE *stream, long int offset, int whence);
extern int fsetpos(FILE *stream, const fpos_t *pos);
extern long int ftell(FILE *stream);
extern size_t fwrite(const void * /* restrict */ ptr, size_t size, size_t nmemb, FILE * /* restrict */ stream);
extern int getc(FILE *stream);
extern int getchar();
extern char *gets(char *s);
extern void perror(const char *s);
extern int printf(const char * /* restrict */ format, ...);
extern int putc(int c, FILE *stream);
extern int putchar(int c);
extern int puts(const char *s);
extern int remove(const char *filename);
extern int rename(const char *old, const char *new);
extern void rewind(FILE *stream);
extern int scanf(const char * /* restrict */ format, ...);
extern void setbuf(FILE * /* restrict */ stream, char * /* restrict */ buf);
extern int setvbuf(FILE * /* restrict */ stream, char * /* restrict */ buf, int mode, size_t size);
extern int sprintf(char * /* restrict */ s, const char * /* restrict */ format, ...);
extern int snprintf(char * /* restrict */ s, size_t n, const char * /* restrict */ format, ...);
extern int sscanf(const char * /* restrict */ s, const char * /* restrict */ format, ...);
extern FILE *tmpfile();
extern char *tmpnam(char *s);
extern int ungetc(int c, FILE *stream);
extern int vfprintf(FILE * /* restrict */ stream, const char * /* restrict */ format, va_list arg);
extern int vfscanf(FILE * /* restrict */ stream, const char * /* restrict */ format, va_list arg);
extern int vprintf(const char * /* restrict */ format, va_list arg);
extern int vscanf(const char * /* restrict */ format, va_list arg);
extern int vsnprintf(char * /* restrict */ s, size_t n, const char * /* restrict */ format, va_list arg);
extern int vsprintf(char * /* restrict */ s, const char * /* restrict */ format, va_list arg);
extern int vsscanf(const char * /* restrict */ s, const char * /* restrict */ format, va_list arg);

__END_DECLS

#endif /* _CC_STDIO_H */
