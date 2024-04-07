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
#include <base/file.h>
#include <base/new.h>
#include <base/yaml.h>

#include <core/cc/ci/config.h>
#include <core/cc/ci/include.h>

#include <stdio.h>
#include <stdlib.h>

#define CI_CONFIG "CI.yaml"

static enum CIStandard
parse_standard__CIConfig(YAMLLoadRes *yaml_load_res);

static CICompiler
parse_compiler__CIConfig(YAMLLoadRes *yaml_load_res);

static void
parse_include_dirs__CIConfig(YAMLLoadRes *yaml_load_res);

/// @return Vec<CILibrary*>*
static Vec *
parse_libraries__CIConfig(YAMLLoadRes *yaml_load_res);

/// @return Vec<CIBin*>*
static Vec *
parse_bins__CIConfig(YAMLLoadRes *yaml_load_res);

enum CIStandard
parse_standard__CIConfig(YAMLLoadRes *yaml_load_res)
{
    enum CIStandard standard;

    // standard: c89 | c95 | c99 | c11 | c17 | c23
    Int32 standard_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "standard");

    if (standard_value_id == -1) {
        FAILED("expected standard key");
    }

    YAMLNode *standard_value_node =
      get_node_from_id__YAML(yaml_load_res, FIRST_DOCUMENT, standard_value_id);

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

    return standard;
}

CICompiler
parse_compiler__CIConfig(YAMLLoadRes *yaml_load_res)
{
    CICompiler compiler;

    // compiler:
    //   gcc: <path> | clang: <path>
    Int32 compiler_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "compiler");
    enum CICompilerKind compiler_kind;
    const char *path;

    if (compiler_value_id == -1) {
        FAILED("expected compiler key");
    }

    YAMLNode *compiler_value_node =
      get_node_from_id__YAML(yaml_load_res, FIRST_DOCUMENT, compiler_value_id);
    char *compiler_kind_value;

    ASSERT(compiler_value_node);

    switch (GET_NODE_TYPE__YAML(compiler_value_node)) {
        case YAML_SCALAR_NODE: {
            compiler_kind_value =
              GET_NODE_SCALAR_VALUE__YAML(compiler_value_node);

            if (!strcmp(compiler_kind_value, "gcc")) {
                compiler_kind = CI_COMPILER_KIND_GCC;
            } else if (!strcmp(compiler_kind_value, "clang")) {
                compiler_kind = CI_COMPILER_KIND_CLANG;
            } else {
                FAILED("unknown compiler");
            }
        }
        default:
            FAILED("expected compiler:\n"
                   "  clang: <path>, gcc: <path>");
    }

    Int32 path_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, compiler_kind_value);

    if (path_value_id == -1) {
        path = compiler_kind_value;
    } else {
        YAMLNode *path_value_node =
          get_node_from_id__YAML(yaml_load_res, FIRST_DOCUMENT, path_value_id);

        ASSERT(path_value_node);

        switch (GET_NODE_TYPE__YAML(path_value_node)) {
            case YAML_SCALAR_NODE: {
                path = GET_NODE_SCALAR_VALUE__YAML(path_value_node);
            }
            default:
                FAILED("expected clang: <path>, gcc: <path>");
        }
    }

    compiler = NEW(CICompiler, compiler_kind, path);

    return compiler;
}

void
parse_include_dirs__CIConfig(YAMLLoadRes *yaml_load_res)
{
    // include_dirs:
    //   - dir
    //   - dir2
    //   ...

    // Initialize include directories workspace
    init_include_dirs__CIInclude();

    Int32 include_dirs_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "include_dirs");

    if (include_dirs_value_id != -1) {
        YAMLNode *include_dirs_value_node = get_node_from_id__YAML(
          yaml_load_res, FIRST_DOCUMENT, include_dirs_value_id);

        ASSERT(include_dirs_value_node);

        switch (GET_NODE_TYPE__YAML(include_dirs_value_node)) {
            case YAML_SEQUENCE_NODE:
                ITER_ON_SEQUENCE_NODE__YAML(
                  yaml_load_res,
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

Vec *
parse_libraries__CIConfig(YAMLLoadRes *yaml_load_res)
{
    Vec *libraries = NEW(Vec);

    // libraries:
    //   - <lib_name>:
    //     paths:
    //       - <path_1>
    //       - <path_2>
    //       - <path_3>
    //       - ...
    //   - <lib_name>:
    //     paths:
    //       - <path_1>
    //       - <path_2>
    //       - ...
    //   ...

    Int32 libraries_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "include_dirs");

    if (libraries_value_id != -1) {
        YAMLNode *libraries_value_node = get_node_from_id__YAML(
          yaml_load_res, FIRST_DOCUMENT, libraries_value_id);

        ASSERT(libraries_value_node);

        switch (GET_NODE_TYPE__YAML(libraries_value_node)) {
            case YAML_SEQUENCE_NODE:
                ITER_ON_SEQUENCE_NODE__YAML(
                  yaml_load_res,
                  FIRST_DOCUMENT,
                  libraries_value_node,
                  library,
                  {
                      // Get library value
                      // libraries:
                      //   - <name>:
                      //     ...
                      //   - <name2>:
                      //     ...
                      //   ...
                      const char *name = library_value;
                      Int32 library_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
                        yaml_load_res, FIRST_DOCUMENT, name);

                      ASSERT(library_value_id != -1);

                      YAMLNode *library_value_node = get_node_from_id__YAML(
                        yaml_load_res, FIRST_DOCUMENT, library_value_id);

                      ASSERT(library_value_node);

                      // paths:
                      //   - <path1>
                      //   - <path2>
                      //   ...
                      Vec *paths = NULL; // Vec<char* (&)>*

                      switch (GET_NODE_TYPE__YAML(library_value_node)) {
                          case YAML_MAPPING_NODE: {
                              const char *expected_mapping_keys[] = { "paths" };

                              ITER_ON_MAPPING_NODE__YAML(
                                yaml_load_res,
                                FIRST_DOCUMENT,
                                library_value_node,
                                library_pair,
                                {
                                    // Check if the key matches with an expected
                                    // key
                                    bool is_match = false;

                                    for (Usize i = 0;
                                         i < LEN(expected_mapping_keys,
                                                 *expected_mapping_keys);
                                         ++i) {
                                        if (!strcmp(library_pair_key,
                                                    expected_mapping_keys[i])) {
                                            is_match = true;
                                            break;
                                        }
                                    }

                                    if (!is_match) {
                                        FAILED("this key doesn't match with "
                                               "expected keys\n"
                                               "expected: paths");
                                    }

                                    // Parse paths sequence.
                                    // e.g.:
                                    //   - <path1>
                                    //   - <path2>
                                    //   ...
                                    if (!strcmp(library_pair_key, "paths")) {
                                        ASSERT(!paths);

                                        paths = NEW(Vec);

                                        ITER_ON_SEQUENCE_NODE__YAML(
                                          yaml_load_res,
                                          FIRST_DOCUMENT,
                                          library_pair_value_node,
                                          path,
                                          { push__Vec(paths, path_value); });
                                    }
                                });

                              break;
                          }
                          default:
                              goto error;
                      }

                      push__Vec(libraries, NEW(CILibrary, name, paths));
                  });

                break;
            default:
            error:
                FAILED("expected sequence node:\n"
                       "libraries:\n"
                       "  - <lib_name>:\n"
                       "    paths:\n"
                       "      - <path_1>\n"
                       "      - <path_2>\n"
                       "      - <path_3>\n"
                       "      - ...\n"
                       "  - <lib_name>:\n"
                       "    paths:\n"
                       "      - <path_1>\n"
                       "      - <path_2>\n"
                       "      - ...\n"
                       "  ...");
        }
    }

    return libraries;
}

Vec *
parse_bins__CIConfig(YAMLLoadRes *yaml_load_res)
{
    Vec *bins = NEW(Vec);

    return bins;
}

CONSTRUCTOR(CILibrary *, CILibrary, const char *name, Vec *paths)
{
    CILibrary *self = lily_malloc(sizeof(CILibrary));

    self->name = name;
    self->paths = paths;

    return self;
}

CONSTRUCTOR(CIBin *, CIBin, const char *name, const char *path)
{
    CIBin *self = lily_malloc(sizeof(CIBin));

    self->name = name;
    self->path = path;

    return self;
}

CIConfig
parse__CIConfig()
{
    char *cwd = get_cwd__Dir();
    String *path_ci_config = exists_rec__File(cwd, CI_CONFIG); // <path>/CI.yaml

    if (!path_ci_config) {
        FAILED("could not find CI.yaml in current working directory or parent "
               "directory");
    }

    free(cwd);

    YAMLLoadRes yaml_load_res = load__YAML(path_ci_config->buffer);
    enum CIStandard standard = parse_standard__CIConfig(&yaml_load_res);
    CICompiler compiler = parse_compiler__CIConfig(&yaml_load_res);

    parse_include_dirs__CIConfig(&yaml_load_res);

    Vec *libraries = parse_libraries__CIConfig(&yaml_load_res);
    Vec *bins = parse_bins__CIConfig(&yaml_load_res);

    return NEW(CIConfig,
               standard,
               compiler,
               get_include_dirs__CIInclude(),
               libraries,
               bins);
}

DESTRUCTOR(CIConfig, const CIConfig *self)
{
    FREE_BUFFER_ITEMS(self->libraries->buffer, self->libraries->len, CILibrary);
    FREE(Vec, self->libraries);

    FREE_BUFFER_ITEMS(self->bins->buffer, self->bins->len, CIBin);
    FREE(Vec, self->bins);
}
