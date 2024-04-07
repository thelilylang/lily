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

#include <base/assert.h>
#include <base/dir.h>
#include <base/yaml.h>

#include <core/cc/ci/generator.h>
#include <core/cc/ci/include.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/result.h>
#include <core/cc/ci/scanner.h>

#include <stdio.h>

#define CI_CONFIG "CI.yaml"

int
main(int argc, char **argv)
{
    // TODO: implement a real CLI, like in bin/lily/main.c, bin/lilyc/main.c

    char *cwd = get_cwd__Dir();
    String *path_ci_config = exists_rec__File(cwd, CI_CONFIG); // <path>/CI.yaml

    if (!path_ci_config) {
        FAILED("could not find CI.yaml in current working directory or parent "
               "directory");
    }

    free(cwd);

    YAMLLoadRes yaml_load_res = load__YAML(path_ci_config->buffer);
    enum CIStandard standard;

    {
        // standard: k&r | c89 | c95 | c99 | c11 | c17 | c23
        Int32 standard_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
          &yaml_load_res, FIRST_DOCUMENT, "standard");

        if (standard_value_id == -1) {
            FAILED("expected standard key");
        }

        YAMLNode *standard_value_node = get_node_from_id__YAML(
          &yaml_load_res, FIRST_DOCUMENT, standard_value_id);

        ASSERT(standard_value_node);

        switch (GET_NODE_TYPE__YAML(standard_value_node)) {
            case YAML_SCALAR_NODE: {
                char *standard_value =
                  GET_NODE_SCALAR_VALUE__YAML(standard_value_node);

                if (!strcmp(standard_value, "k&r")) {
                    standard = CI_STANDARD_KR;
                } else if (!strcmp(standard_value, "c89")) {
                    standard = CI_STANDARD_89;
                } else if (!strcmp(standard_value, "c95")) {
                    standard = CI_STANDARD_95;
                } else if (!strcmp(standard_value, "c99")) {
                    standard = CI_STANDARD_99;
                } else if (!strcmp(standard_value, "c11")) {
                    standard = CI_STANDARD_11;
                } else if (!strcmp(standard_value, "c17")) {
                    standard = CI_STANDARD_17;
                } else if (!strcmp(standard_value, "c23")) {
                    standard = CI_STANDARD_23;
                } else {
                    FAILED("unknown standard");
                }

                break;
            }
            default:
                FAILED("expected: standard: k&r | c89 | c95 | c99 | c11 | c17 "
                       "| c23");
        }
    }

    {
        // include_dirs:
        //   - dir
        //   - dir2
        //   ...

        // Initialize include directories workspace
        init_include_dirs__CIInclude();

        Int32 include_dirs_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
          &yaml_load_res, FIRST_DOCUMENT, "include_dirs");

        if (include_dirs_value_id != -1) {
            YAMLNode *include_dirs_value_node = get_node_from_id__YAML(
              &yaml_load_res, FIRST_DOCUMENT, include_dirs_value_id);

            ASSERT(include_dirs_value_node);

            switch (GET_NODE_TYPE__YAML(include_dirs_value_node)) {
                case YAML_SEQUENCE_NODE:
                    ITER_ON_SEQUENCE_NODE__YAML(
                      &yaml_load_res,
                      FIRST_DOCUMENT,
                      include_dirs_value_node,
                      include_dir,
                      { add_include_dir__CIInclude(include_dir_value); });

                    break;
                default:
                    FAILED("expected sequence node:\n"
                           "include_dirs:\n"
                           "  - dir\n"
                           "  - dir2\n"
                           "  ...");
            }
        }
    }

    // TODO: Get the files from YAML config
    if (argc < 2) {
        printf("ci [ci_files...]\n");
        exit(1);
    }

    CIResult result = NEW(CIResult);

    for (Usize i = 1; i < argc; ++i) {
        add_and_run__CIResult(&result, strdup(argv[i]), standard);
    }

    run__CIGenerator(&result);

    FREE(CIResult, &result);
    FREE(String, path_ci_config);
    FREE(YAMLLoadRes, &yaml_load_res);
    destroy__CIInclude();
}
