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
#include <base/dir_separator.h>
#include <base/file.h>
#include <base/format.h>
#include <base/new.h>

#include <core/cc/ci/include.h>
#include <core/cc/ci/project_config.h>

#include <stdio.h>
#include <stdlib.h>

#define CI_CONFIG "CI.yaml"

static enum CIStandard
parse_standard__CIProjectConfig(YAMLLoadRes *yaml_load_res);

static CIProjectConfigCompiler
parse_compiler__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                const char *absolute_config_dir);

static void
parse_include_dirs__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                    const String *compiler_path,
                                    const char *base_path);

/// @return Vec<CIProjectConfigLibrary*>*
static Vec *
parse_libraries__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                 const char *absolute_config_dir);

/// @return Vec<CIProjectConfigBin*>*
static Vec *
parse_bins__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                            const char *absolute_config_dir);

enum CIStandard
parse_standard__CIProjectConfig(YAMLLoadRes *yaml_load_res)
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

CIProjectConfigCompiler
parse_compiler__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                const char *absolute_config_dir)
{
    // compiler:
    //   name: clang | gcc
    //   path: <path_of_command>
    Int32 compiler_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "compiler");
    enum CIProjectConfigCompilerKind compiler_kind;
    const char *path = NULL;

    if (compiler_value_id == -1) {
        FAILED("expected compiler key");
    }

    YAMLNode *compiler_value_node =
      get_node_from_id__YAML(yaml_load_res, FIRST_DOCUMENT, compiler_value_id);

    ASSERT(compiler_value_node);

    switch (GET_NODE_TYPE__YAML(compiler_value_node)) {
        case YAML_MAPPING_NODE: {
            const char *expected_mapping_keys[] = { "name", "path" };

            ITER_ON_MAPPING_NODE_WITH_EXPECTED_KEYS__YAML(
              yaml_load_res,
              FIRST_DOCUMENT,
              compiler_value_node,
              compiler_pair,
              expected_mapping_keys,
              {
                  if (!strcmp(compiler_pair_key, "name")) {
                      if (!strcmp(compiler_pair_value, "clang")) {
                          compiler_kind = CI_COMPILER_KIND_CLANG;
                      } else if (!strcmp(compiler_pair_value, "gcc")) {
                          compiler_kind = CI_COMPILER_KIND_GCC;
                      } else {
                          UNREACHABLE("unknown compiler");
                      }
                  } else if (!strcmp(compiler_pair_key, "path")) {
                      ASSERT(!path);

                      path = compiler_pair_value;
                  } else {
                      UNREACHABLE("this situation is impossible");
                  }
              });

            break;
        }
        default:
            FAILED("expected compiler:\n"
                   "  name: clang | gcc\n"
                   "  path: <path_of_command>");
    }

    return NEW(CIProjectConfigCompiler,
               compiler_kind,
               path[0] == '/' // Check if the path is absolute
                 ? from__String((char *)path)
                 : format__String(
                     "{s}" DIR_SEPARATOR_S "{s}", absolute_config_dir, path));
}

void
parse_include_dirs__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                    const String *compiler_path,
                                    const char *base_path)
{
    // include_dirs:
    //   - dir
    //   - dir2
    //   ...

    // Initialize include directories workspace
    init_include_dirs__CIInclude(compiler_path, base_path);

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
                  {
                      add_include_dir__CIInclude(
                        from__String(include_dir_value));
                  });

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
parse_libraries__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                 const char *absolute_config_dir)
{
    Vec *libraries = NEW(Vec);

    // libraries:
    //   <lib_name>:
    //     paths:
    //       - <path_1>
    //       - <path_2>
    //       - <path_3>
    //       - ...
    //   <lib_name>:
    //     paths:
    //       - <path_1>
    //       - <path_2>
    //       - ...
    //   ...

    Int32 libraries_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "libraries");

    if (libraries_value_id != -1) {
        YAMLNode *libraries_value_node = get_node_from_id__YAML(
          yaml_load_res, FIRST_DOCUMENT, libraries_value_id);

        ASSERT(libraries_value_node);

        switch (GET_NODE_TYPE__YAML(libraries_value_node)) {
            case YAML_MAPPING_NODE: {
                //   <lib_name>:
                //     ...
                //   ...
                ITER_ON_MAPPING_NODE__YAML(
                  yaml_load_res,
                  FIRST_DOCUMENT,
                  libraries_value_node,
                  library,
                  {
                      const char *name = library_key;
                      Vec *paths = NULL; // Vec<String*>*

                      switch (GET_NODE_TYPE__YAML(library_value_node)) {
                          case YAML_MAPPING_NODE: {
                              const char *expected_mapping_keys[] = { "paths" };

                              // paths:
                              //   ...
                              ITER_ON_MAPPING_NODE_WITH_EXPECTED_KEYS__YAML(
                                yaml_load_res,
                                FIRST_DOCUMENT,
                                library_value_node,
                                library_mapping,
                                expected_mapping_keys,
                                {
                                    if (!strcmp(library_mapping_key, "paths")) {
                                        ASSERT(!paths);

                                        paths = NEW(Vec);

                                        // paths:
                                        //   - <path1>
                                        //   - <path2>
                                        //   ...
                                        switch (GET_NODE_TYPE__YAML(
                                          library_mapping_value_node)) {
                                            case YAML_SEQUENCE_NODE:
                                                ITER_ON_SEQUENCE_NODE__YAML(
                                                  yaml_load_res,
                                                  FIRST_DOCUMENT,
                                                  library_mapping_value_node,
                                                  path,
                                                  {
                                                      push__Vec(
                                                        paths,
                                                        format__String(
                                                          "{s}" DIR_SEPARATOR_S
                                                          "{s}",
                                                          absolute_config_dir,
                                                          path_value));
                                                  });

                                                break;
                                            default:
                                                goto error;
                                        }
                                    } else {
                                        UNREACHABLE("this key is not expected");
                                    }
                                });

                              break;
                          }
                          default:
                              goto error;
                      }

                      push__Vec(libraries,
                                NEW(CIProjectConfigLibrary, name, paths));
                  });

                break;
            }
            default:
            error:
                FAILED("expected mapping node:\n"
                       "libraries:\n"
                       "  <lib_name>:\n"
                       "    paths:\n"
                       "      - <path_1>\n"
                       "      - <path_2>\n"
                       "      - <path_3>\n"
                       "      - ...\n"
                       "  <lib_name>:\n"
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
parse_bins__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                            const char *absolute_config_dir)
{
    Vec *bins = NEW(Vec);

    // bins:
    //   <bin_name>:
    //     path: <file_path>

    Int32 bins_value_id =
      GET_KEY_ON_DEFAULT_MAPPING__YAML(yaml_load_res, FIRST_DOCUMENT, "bins");

    if (bins_value_id != -1) {
        YAMLNode *bins_value_node =
          get_node_from_id__YAML(yaml_load_res, FIRST_DOCUMENT, bins_value_id);

        ASSERT(bins_value_node);

        switch (GET_NODE_TYPE__YAML(bins_value_node)) {
            case YAML_MAPPING_NODE:
                //   <bin_name>:
                //     ...
                //   ...
                ITER_ON_MAPPING_NODE__YAML(
                  yaml_load_res, FIRST_DOCUMENT, bins_value_node, bin, {
                      const char *name = bin_key;
                      String *path = NULL;

                      switch (GET_NODE_TYPE__YAML(bin_value_node)) {
                          case YAML_MAPPING_NODE: {
                              // path: <file_path>
                              const char *expected_mapping_keys[] = { "path" };

                              ITER_ON_MAPPING_NODE_WITH_EXPECTED_KEYS__YAML(
                                yaml_load_res,
                                FIRST_DOCUMENT,
                                bin_value_node,
                                bin_mapping,
                                expected_mapping_keys,
                                {
                                    if (!strcmp(bin_mapping_key, "path")) {
                                        ASSERT(!path);

                                        path = format__String(
                                          "{s}" DIR_SEPARATOR_S "{s}",
                                          absolute_config_dir,
                                          bin_mapping_value);
                                    } else {
                                        UNREACHABLE("this key is not expected");
                                    }
                                });

                              break;
                          }
                          default:
                              goto error;
                      }

                      push__Vec(bins, NEW(CIProjectConfigBin, name, path));
                  });

                break;
            default:
            error:
                FAILED("expected mapping node:\n"
                       "bins:\n"
                       "  <bin_name>:\n"
                       "    path: <file_path>\n"
                       "  ...");
        }
    }

    return bins;
}

CONSTRUCTOR(CIProjectConfigLibrary *,
            CIProjectConfigLibrary,
            const char *name,
            Vec *paths)
{
    CIProjectConfigLibrary *self = lily_malloc(sizeof(CIProjectConfigLibrary));

    self->name = name;
    self->paths = paths;

    return self;
}

DESTRUCTOR(CIProjectConfigLibrary, CIProjectConfigLibrary *self)
{
    FREE_BUFFER_ITEMS(self->paths->buffer, self->paths->len, String);
    FREE(Vec, self->paths);
    lily_free(self);
}

CONSTRUCTOR(CIProjectConfigBin *,
            CIProjectConfigBin,
            const char *name,
            String *path)
{
    CIProjectConfigBin *self = lily_malloc(sizeof(CIProjectConfigBin));

    self->name = name;
    self->path = path;

    return self;
}

DESTRUCTOR(CIProjectConfigBin, CIProjectConfigBin *self)
{
    FREE(String, self->path);
    lily_free(self);
}

CIProjectConfig
parse__CIProjectConfig(const char *config_dir)
{
    char *absolute_config_dir = format("{sa}/{s}", get_cwd__Dir(), config_dir);
    String *path_ci_config =
      exists_rec__File(absolute_config_dir, CI_CONFIG); // <path>/CI.yaml

    if (!path_ci_config) {
        FAILED("could not find CI.yaml in current working directory or parent "
               "directory");
    }

    YAMLLoadRes yaml_load_res = load__YAML(path_ci_config->buffer);
    enum CIStandard standard = parse_standard__CIProjectConfig(&yaml_load_res);
    CIProjectConfigCompiler compiler =
      parse_compiler__CIProjectConfig(&yaml_load_res, absolute_config_dir);

    parse_include_dirs__CIProjectConfig(
      &yaml_load_res, compiler.path, config_dir);

    Vec *libraries =
      parse_libraries__CIProjectConfig(&yaml_load_res, absolute_config_dir);
    Vec *bins =
      parse_bins__CIProjectConfig(&yaml_load_res, absolute_config_dir);

    FREE(String, path_ci_config);
    lily_free(absolute_config_dir);

    return NEW(CIProjectConfig,
               yaml_load_res,
               standard,
               compiler,
               get_include_dirs__CIInclude(),
               libraries,
               bins);
}

DESTRUCTOR(CIProjectConfig, const CIProjectConfig *self)
{
    FREE(YAMLLoadRes, &self->yaml_load_res);
    FREE(CIProjectConfigCompiler, &self->compiler);
    FREE_BUFFER_ITEMS(
      self->libraries->buffer, self->libraries->len, CIProjectConfigLibrary);
    FREE(Vec, self->libraries);

    FREE_BUFFER_ITEMS(self->bins->buffer, self->bins->len, CIProjectConfigBin);
    FREE(Vec, self->bins);
}
