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

#include <base/macros.h>

#include <command/cic/cic.h>

#include <core/cc/ci/builtin.h>
#include <core/cc/ci/compile.h>
#include <core/cc/ci/generator.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/project_config.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/scanner.h>
#include <core/cc/ci/state_checker.h>
#include <core/cc/ci/typecheck.h>
#include <core/cc/ci/visitor.h>

#include <stdio.h>
#include <stdlib.h>

void
run__CIc(const CIcConfig *config,
         void (*handler)(const CIResult *result, void *other_args),
         void *other_args)
{
    if (config->mode != CIC_CONFIG_MODE_NONE) {
        TODO("implement --mode option");
    }

    CIBuiltin builtin = NEW(CIBuiltin);
    CIProjectConfig project_config =
      config->file ? parse_cic_cli__CIProjectConfig(config)
                   : parse_yaml__CIProjectConfig(config->path);
    CIResult result = NEW(CIResult, &project_config, &builtin);
    CIVisitor visitor = NEW(CIVisitor, &result);
    CITypecheck typecheck = NEW(CITypecheck, &result);
    CIStateChecker state_checker = NEW(CIStateChecker, &result);

    set__CIBuiltin(&builtin);
    build__CIResult(&result);
    run__CIVisitor(&visitor);
    run__CITypecheck(&typecheck);
    run__CIStateChecker(&state_checker);
    run__CIGenerator(&result);

    exec__CICompile(&result);

    if (handler) {
        handler(&result, other_args);
    }

    FREE(CIResult, &result);
    FREE(CIBuiltin, &builtin);
    FREE(CIProjectConfig, &project_config);
    FREE(CIStateChecker, &state_checker);
}
