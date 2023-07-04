/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#include <base/new.h>

#include <cli/lilyc/config.h>

#include <command/lilyc/lilyc.h>

#include <core/lily/lily.h>
#include <core/lily/compiler/package.h>
#include <core/lily/compiler/package/program.h>

#include <stdlib.h>

void
run__Lilyc(const LilycConfig *config)
{
    // Get the default path
    char *default_path = generate_default_path((char *)config->filename);

    LilyProgram program = NEW(LilyProgram, LILY_PROGRAM_KIND_EXE);

    LilyPackage *pkg = compile__LilyPackage(config,
                                            LILY_VISIBILITY_PUBLIC,
                                            LILY_PACKAGE_STATUS_MAIN,
                                            default_path,
                                            &program);

    lily_free(default_path);

#if !defined(RUN_UNTIL_PREPARSER) && !defined(RUN_UNTIL_PRECOMPILE)
    FREE(LilyPackage, pkg);
#endif

    // NOTE: Do not move this `free` otherwise it could cause double free. In
    // short, always free the program pointer after the package
    // pointer.
    FREE(LilyProgram, &program);
}
