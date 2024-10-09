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
#include <base/path.h>

#include <core/cc/ci/include.h>
#include <core/cc/ci/project_config.h>

#include <stdio.h>
#include <stdlib.h>

#define CI_CONFIG "CI.yaml"

/// @param path char* (&)
/// @param absolute_config_dir const char* (&)
/// @return String*
static String *
convert_path_to_absolute_path__CIProjectConfig(char *path,
                                               const char *absolute_config_dir);

static enum CIStandard
parse_standard__CIProjectConfig(YAMLLoadRes *yaml_load_res);

static CIProjectConfigCompiler
parse_compiler__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                const char *absolute_config_dir);

static void
parse_std_include__CIProjectConfig(YAMLLoadRes *yaml_load_res,
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

String *
convert_path_to_absolute_path__CIProjectConfig(char *path,
                                               const char *absolute_config_dir)
{
    ASSERT(!is_relative__Path(absolute_config_dir));

    if (is_relative__Path(path)) {
        return format__String("{s}/{s}", absolute_config_dir, path);
    }

    return from__String(path);
}

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
    //   command: <path_of_command> | <command>
    Int32 compiler_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "compiler");
    enum CIProjectConfigCompilerKind compiler_kind;
    const char *command = NULL;

    if (compiler_value_id == -1) {
        FAILED("expected compiler key");
    }

    YAMLNode *compiler_value_node =
      get_node_from_id__YAML(yaml_load_res, FIRST_DOCUMENT, compiler_value_id);

    ASSERT(compiler_value_node);

    switch (GET_NODE_TYPE__YAML(compiler_value_node)) {
        case YAML_MAPPING_NODE: {
            const char *expected_mapping_keys[] = { "name", "command" };

            ITER_ON_MAPPING_NODE_WITH_EXPECTED_KEYS__YAML(
              yaml_load_res,
              FIRST_DOCUMENT,
              compiler_value_node,
              compiler_pair,
              expected_mapping_keys,
              {
                  if (!strcmp(compiler_pair_key, "name")) {
                      if (!strcmp(compiler_pair_value, "clang")) {
                          compiler_kind = CI_PROJECT_CONFIG_COMPILER_KIND_CLANG;
                      } else if (!strcmp(compiler_pair_value, "gcc")) {
                          compiler_kind = CI_PROJECT_CONFIG_COMPILER_KIND_GCC;
                      } else {
                          UNREACHABLE("unknown compiler");
                      }
                  } else if (!strcmp(compiler_pair_key, "command")) {
                      ASSERT(!command);

                      command = compiler_pair_value;
                  } else {
                      UNREACHABLE("this situation is impossible");
                  }
              });

            break;
        }
        default:
            FAILED("expected compiler:\n"
                   "  name: clang | gcc\n"
                   "  command: <path_of_command> | <command>");
    }

    return NEW(CIProjectConfigCompiler,
               compiler_kind,
               command[0] == '/' // Check if the path is absolute
                 ? from__String((char *)command)
                 : format__String("{s}" DIR_SEPARATOR_S "{s}",
                                  absolute_config_dir,
                                  command));
}

void
parse_std_include__CIProjectConfig(YAMLLoadRes *yaml_load_res,
                                   const char *absolute_config_dir)
{
    // std_include: <path>
    Int32 std_include_value_id = GET_KEY_ON_DEFAULT_MAPPING__YAML(
      yaml_load_res, FIRST_DOCUMENT, "std_include");

    if (std_include_value_id == -1) {
        return;
    }

    YAMLNode *std_include_value_node = get_node_from_id__YAML(
      yaml_load_res, FIRST_DOCUMENT, std_include_value_id);

    ASSERT(std_include_value_node);

    switch (GET_NODE_TYPE__YAML(std_include_value_node)) {
        case YAML_SCALAR_NODE:
            // NOTE: We insert the standard include directory in first, to take
            // precedence over other include directories.
            insert_include_dir__CIInclude(
              convert_path_to_absolute_path__CIProjectConfig(
                GET_NODE_SCALAR_VALUE__YAML(std_include_value_node),
                absolute_config_dir),
              0);

            return;
        default:
            FAILED("expected: std_include: <path>");
    }
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
                        convert_path_to_absolute_path__CIProjectConfig(
                          include_dir_value, base_path));
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
                      String *name = from__String(library_key);
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
                      String *name = from__String(bin_key);
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
            String *name,
            Vec *paths)
{
    CIProjectConfigLibrary *self = lily_malloc(sizeof(CIProjectConfigLibrary));

    self->name = name;
    self->paths = paths;

    return self;
}

DESTRUCTOR(CIProjectConfigLibrary, CIProjectConfigLibrary *self)
{
    FREE(String, self->name);
    FREE_BUFFER_ITEMS(self->paths->buffer, self->paths->len, String);
    FREE(Vec, self->paths);
    lily_free(self);
}

CONSTRUCTOR(CIProjectConfigBin *,
            CIProjectConfigBin,
            String *name,
            String *path)
{
    CIProjectConfigBin *self = lily_malloc(sizeof(CIProjectConfigBin));

    self->name = name;
    self->path = path;

    return self;
}

DESTRUCTOR(CIProjectConfigBin, CIProjectConfigBin *self)
{
    FREE(String, self->name);
    FREE(String, self->path);
    lily_free(self);
}

CIProjectConfig
parse_cli__CIProjectConfig(const CIConfig *cli_config)
{
    String *bin_name = get_filename__File(cli_config->path);
    CIProjectConfigCompiler compiler =
      NEW(CIProjectConfigCompiler,
          CI_PROJECT_CONFIG_COMPILER_KIND_CLANG,
          from__String("clang"));
    String *absolute_path_file_dir = get_absolute_dir__File(cli_config->path);

    init_include_dirs__CIInclude(compiler.command,
                                 absolute_path_file_dir->buffer);

    FREE(String, absolute_path_file_dir);

    // Add include dir passed via the CLI with the option `-I`/`--include` to
    // the end of the list of include dirs.
    for (Usize i = 0; i < cli_config->includes->len; ++i) {
        add_include_dir__CIInclude(
          convert_to_absolute_path__Path(get__Vec(cli_config->includes, i)));
    }

    // Add include dir passed via the CLI with the option `--include0` to
    // the begin of the list of include dirs.
    for (Usize i = 0; i < cli_config->includes0->len; ++i) {
        insert_include_dir__CIInclude(
          convert_to_absolute_path__Path(get__Vec(cli_config->includes0, i)),
          0);
    }

    return NEW_VARIANT(CIProjectConfig,
                       cli,
                       cli_config->standard,
                       compiler,
                       get_include_dirs__CIInclude(),
                       NEW(Vec),
                       init__Vec(1,
                                 NEW(CIProjectConfigBin,
                                     bin_name,
                                     from__String((char *)cli_config->path))));
}

CIProjectConfig
parse_yaml__CIProjectConfig(const char *config_dir)
{
    String *absolute_config_dir = convert_to_absolute_path__Path(config_dir);
    String *path_ci_config = exists_rec__File(absolute_config_dir->buffer,
                                              CI_CONFIG); // <path>/CI.yaml

    if (!path_ci_config) {
        FAILED("could not find CI.yaml in current working directory or parent "
               "directory");
    }

    FREE(String, absolute_config_dir);

    absolute_config_dir = get_dir__File(path_ci_config->buffer);

    YAMLLoadRes yaml_load_res = load__YAML(path_ci_config->buffer);
    enum CIStandard standard = parse_standard__CIProjectConfig(&yaml_load_res);
    CIProjectConfigCompiler compiler = parse_compiler__CIProjectConfig(
      &yaml_load_res, absolute_config_dir->buffer);

    parse_include_dirs__CIProjectConfig(
      &yaml_load_res, compiler.command, absolute_config_dir->buffer);
    parse_std_include__CIProjectConfig(&yaml_load_res,
                                       absolute_config_dir->buffer);

    Vec *libraries = parse_libraries__CIProjectConfig(
      &yaml_load_res, absolute_config_dir->buffer);
    Vec *bins =
      parse_bins__CIProjectConfig(&yaml_load_res, absolute_config_dir->buffer);

    FREE(String, path_ci_config);
    FREE(String, absolute_config_dir);

    return NEW_VARIANT(CIProjectConfig,
                       yaml,
                       yaml_load_res,
                       standard,
                       compiler,
                       get_include_dirs__CIInclude(),
                       libraries,
                       bins);
}

DESTRUCTOR(CIProjectConfig, const CIProjectConfig *self)
{
    switch (self->kind) {
        case CI_PROJECT_CONFIG_KIND_YAML:
            FREE(YAMLLoadRes, &self->yaml);

            break;
        default:
            break;
    }

    FREE(CIProjectConfigCompiler, &self->compiler);
    FREE_BUFFER_ITEMS(
      self->libraries->buffer, self->libraries->len, CIProjectConfigLibrary);
    FREE(Vec, self->libraries);

    FREE_BUFFER_ITEMS(self->bins->buffer, self->bins->len, CIProjectConfigBin);
    FREE(Vec, self->bins);
}
