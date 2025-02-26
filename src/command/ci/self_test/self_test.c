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

#include <base/dir.h>
#include <base/fork.h>
#include <base/new.h>

#include <command/ci/self_test/metadata/scanner.h>
#include <command/ci/self_test/run.h>
#include <command/ci/self_test/self_test.h>

#include <core/cc/ci/project_config.h>

void
run__CISelfTest(const CIConfig *config)
{
    CIProjectConfig project_config;

    if (is__Dir(config->self_test.path)) {
        project_config = parse_yaml__CIProjectConfig(config->self_test.path);
    } else {
        project_config = parse_ci_cli__CIProjectConfig(config);
    }

    if (project_config.self_tests) {
        for (Usize i = 0; i < project_config.self_tests->len; ++i) {
            const CIProjectConfigSelfTest *self_test =
              get__Vec(project_config.self_tests, i);
            CISelfTestMetadata metadata = NEW(CISelfTestMetadata);

            run__CISelfTestMetadataScanner(self_test, &metadata);
            run__CISelfTestRun(&metadata, self_test);

            FREE(CISelfTestMetadata, &metadata);
        }
    }

    FREE(CIProjectConfig, &project_config);
}
