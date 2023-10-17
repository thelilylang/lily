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

#include <core/lily/compiler/package.h>
#include <core/lily/lily.h>
#include <core/lily/package/default_path.h>
#include <core/lily/package/package.h>
#include <core/lily/package/program.h>

#include <stdlib.h>

void
run__Lilyc(const LilycConfig *config)
{
    if (config->run_scanner) {
        return run_scanner__LilyPackage(config);
    } else if (config->run_preparser) {
        return run_preparser__LilyPackage(config);
    } else if (config->run_precompiler) {
        return run_precompiler__LilyPackage(config);
    } else if (config->run_parser) {
        return run_parser__LilyPackage(config);
    } else if (config->run_analysis) {
        return run_analysis__LilyPackage(config);
    } else if (config->run_mir) {
        return run_mir__LilyPackage(config);
    } else if (config->run_ir) {
        return run_ir__LilyPackage(config);
    }

    // Get the default path
    char *default_path = generate_default_path((char *)config->filename);

    LilyProgram program =
      NEW(LilyProgram,
          config->lib ? config->static_   ? LILY_PROGRAM_KIND_STATIC_LIB
                        : config->dynamic ? LILY_PROGRAM_KIND_DYNAMIC_LIB
                                          : LILY_PROGRAM_KIND_STATIC_LIB
                      : LILY_PROGRAM_KIND_EXE);

    if (config->lib) {
        LilyLibrary *lib = compile_lib__LilyPackage(config,
                                                    LILY_VISIBILITY_PUBLIC,
                                                    LILY_PACKAGE_STATUS_MAIN,
                                                    default_path,
                                                    &program);

#if !defined(RUN_UNTIL_PREPARSER) && !defined(RUN_UNTIL_PRECOMPILER)
        FREE(LilyLibrary, lib);
#endif
    } else {
        LilyPackage *pkg = compile__LilyPackage(config,
                                                LILY_VISIBILITY_PUBLIC,
                                                LILY_PACKAGE_STATUS_MAIN,
                                                default_path,
                                                &program);

        // Run exe
        if (config->run) {
#ifdef LILY_WINDOWS_OS
            char *command = format(".\\{s}", pkg->compiler.output_exe_path);
#else
            char *command = format("./{s}", pkg->compiler.output_exe_path);
#endif

            system(command);
            lily_free(command);
        }

#if !defined(RUN_UNTIL_PREPARSER) && !defined(RUN_UNTIL_PRECOMPILER)
        FREE(LilyPackage, pkg);
#endif
    }

    lily_free(default_path);

    // NOTE: Do not move this `free` otherwise it could cause double free. In
    // short, always free the program pointer after the package
    // pointer or library pointer.
    FREE(LilyProgram, &program);
}
