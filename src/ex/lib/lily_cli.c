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

#ifndef LILY_EX_LIB_LILY_CLI_C
#define LILY_EX_LIB_LILY_CLI_C

#include <cli/lily/config.h>
#include <cli/lily/config/build.h>
#include <cli/lily/config/cc.h>
#include <cli/lily/config/cpp.h>
#include <cli/lily/config/init.h>
#include <cli/lily/config/new.h>
#include <cli/lily/config/run.h>
#include <cli/lily/config/test.h>
#include <cli/lily/config/to.h>

// #include "lily_base.c"

// <cli/lily/config.h>
extern inline VARIANT_CONSTRUCTOR(LilyConfig,
                                  LilyConfig,
                                  build,
                                  LilyConfigBuild build);

extern inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, cc, LilyConfigCc cc);

extern inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, compile);

extern inline VARIANT_CONSTRUCTOR(LilyConfig,
                                  LilyConfig,
                                  cpp,
                                  LilyConfigCpp cpp);

extern inline VARIANT_CONSTRUCTOR(LilyConfig,
                                  LilyConfig,
                                  init,
                                  LilyConfigInit init);

extern inline VARIANT_CONSTRUCTOR(LilyConfig,
                                  LilyConfig,
                                  new,
                                  LilyConfigNew new);

extern inline VARIANT_CONSTRUCTOR(LilyConfig,
                                  LilyConfig,
                                  run,
                                  LilyConfigRun run);

extern inline VARIANT_CONSTRUCTOR(LilyConfig,
                                  LilyConfig,
                                  test,
                                  LilyConfigTest test);

extern inline VARIANT_CONSTRUCTOR(LilyConfig, LilyConfig, to, LilyConfigTo to);

extern inline DESTRUCTOR(LilyConfig, const LilyConfig *self);

// <cli/lily/config/build.h>
extern inline CONSTRUCTOR(LilyConfigBuild, LilyConfigBuild, bool verbose);

// <cli/lily/config/cc.h>
extern inline CONSTRUCTOR(LilyConfigCc, LilyConfigCc, const char *filename);

// <cli/lily/config/cpp.h>
extern inline CONSTRUCTOR(LilyConfigCpp, LilyConfigCpp, const char *filename);

// <cli/lily/config/init.h>
extern inline CONSTRUCTOR(LilyConfigInit, LilyConfigInit, const char *path);

// <cli/lily/config/new.h>
extern inline CONSTRUCTOR(LilyConfigNew, LilyConfigNew, const char *name);

// <cli/lily/config/run.h>
extern inline CONSTRUCTOR(LilyConfigRun,
                          LilyConfigRun,
                          const char *filename,
                          bool verbose,
                          Vec *args,
                          Usize max_stack,
                          Usize max_heap);

// <cli/lily/config/test.h>
extern inline CONSTRUCTOR(LilyConfigTest, LilyConfigTest, const char *filename);

// <cli/lily/config/to.h>
extern inline CONSTRUCTOR(LilyConfigTo,
                          LilyConfigTo,
                          const char *filename,
                          bool cc,
                          bool cpp,
                          bool js);

#endif // LILY_EX_LIB_LILY_CLI_C
