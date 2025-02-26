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

#include <base/assert.h>
#include <base/dir.h>
#include <base/dir_separator.h>
#include <base/file.h>
#include <base/format.h>
#include <base/new.h>
#include <base/path.h>

#include <core/cc/ci/include.h>
#include <core/cc/ci/project_config.h>

#include <core/shared/search.h>

#include <stdio.h>
#include <stdlib.h>

#define CI_CONFIG "CI.yaml"

struct CIProjectConfigContext
{
    YAMLLoadRes *yaml_load_res;      // YAMLLoadRes* (&)
    const char *absolute_config_dir; // const char* (&)
};

/// @brief Construct CIProjectConfigContext type.
static inline CONSTRUCTOR(struct CIProjectConfigContext,
                          CIProjectConfigContext,
                          YAMLLoadRes *yaml_load_res,
                          const char *absolute_config_dir);

/// @param path char* (&)
/// @param absolute_config_dir const char* (&)
/// @return String*
static String *
convert_path_to_absolute_path__CIProjectConfig(char *path,
                                               const char *absolute_config_dir);

/// @param mapping_id node id of mapping or -1 (for NULL)
/// @return Return the node id.
static Int32
get_node__CIProjectConfig(struct CIProjectConfigContext *ctx,
                          Int32 mapping_id,
                          char *key);

/// @param mapping_id node id of mapping or -1 (for NULL)
/// @return Return the node id or -1.
static Int32
get_optional_node__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                   Int32 mapping_id,
                                   char *key);

/// @param mapping_id node id of mapping or -1 (for NULL)
/// @return String*
static String *
get_scalar_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                  Int32 mapping_id,
                                  char *key);

/// @param mapping_id node id of mapping or -1 (for NULL)
/// @return String*?
static String *
get_optional_scalar_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                           Int32 mapping_id,
                                           char *key);

/// @param mapping_id node id of mapping or -1 (for NULL)
static Int32
get_integer_from_scalar_value__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  SizedStr values[],
  Int32 value_ids[],
  Usize values_len);

/// @param mapping_id node id of mapping or -1 (for NULL)
static Int32
get_integer_from_optional_scalar_value__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  SizedStr values[],
  Int32 value_ids[],
  Usize values_len,
  Int32 null_value);

/// @param mapping_id node id of mapping or -1 (for NULL)
/// @return Vec<String*>*
static Vec *
get_sequence_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                    Int32 mapping_id,
                                    char *key);

/// @param mapping_id node id of mapping or -1 (for NULL)
/// @return Vec<String*>*?
static Vec *
get_optional_sequence_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                             Int32 mapping_id,
                                             char *key);

/// @return Vec<void*>*
static Vec *
iter_on_mapping__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  void *(*parse_item)(struct CIProjectConfigContext *ctx,
                      Int32 mapping_id,
                      const char *key_name));

/// @return Vec<void*>*?
static Vec *
iter_on_optional_mapping__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  void *(*parse_item)(struct CIProjectConfigContext *ctx,
                      Int32 mapping_id,
                      const char *key_name));

static enum CIStandard
get_standard__CIProjectConfig(struct CIProjectConfigContext *ctx);

static CIProjectConfigCompiler
get_compiler__CIProjectConfig(struct CIProjectConfigContext *ctx);

static void
get_std_include__CIProjectConfig(struct CIProjectConfigContext *ctx);

static void
get_include_dirs__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                  const String *compiler_path);

/// @param library_name const char* (&)
static CIProjectConfigLibrary *
get_library__CIProjectConfig(struct CIProjectConfigContext *ctx,
                             Int32 mapping_id,
                             const char *library_name);

/// @return Vec<CIProjectConfigLibrary*>*
static Vec *
get_libraries__CIProjectConfig(struct CIProjectConfigContext *ctx);

/// @param bin_name const char* (&)
static CIProjectConfigBin *
get_bin__CIProjectConfig(struct CIProjectConfigContext *ctx,
                         Int32 mapping_id,
                         const char *bin_name);

/// @return Vec<CIProjectConfigBin*>*
static Vec *
get_bins__CIProjectConfig(struct CIProjectConfigContext *ctx);

static Vec *
get_self_tests__CIProjectConfig(struct CIProjectConfigContext *ctx);

static bool
get_no_state_check__CIProjectConfig(struct CIProjectConfigContext *ctx);

static CIProjectConfig
parse_ci_self_test__CIProjectConfig(const CIConfigSelfTest *cli_config);

CONSTRUCTOR(struct CIProjectConfigContext,
            CIProjectConfigContext,
            YAMLLoadRes *yaml_load_res,
            const char *absolute_config_dir)
{
    return (struct CIProjectConfigContext){ .yaml_load_res = yaml_load_res,
                                            .absolute_config_dir =
                                              absolute_config_dir };
}

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

Int32
get_node__CIProjectConfig(struct CIProjectConfigContext *ctx,
                          Int32 mapping_id,
                          char *key)
{
    Int32 res = get_optional_node__CIProjectConfig(ctx, mapping_id, key);

    if (res == -1) {
        FAILED("expected to have this key");
    }

    return res;
}

Int32
get_optional_node__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                   Int32 mapping_id,
                                   char *key)
{
    return get_key__YAML(ctx->yaml_load_res, FIRST_DOCUMENT, mapping_id, key);
}

String *
get_scalar_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                  Int32 mapping_id,
                                  char *key)
{
    String *res =
      get_optional_scalar_value__CIProjectConfig(ctx, mapping_id, key);

    if (!res) {
        FAILED("expected to have this key");
    }

    return res;
}

String *
get_optional_scalar_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                           Int32 mapping_id,
                                           char *key)
{
    YAMLNode *node;

    Int32 key_value_id =
      mapping_id != -1
        ? get_optional_node__CIProjectConfig(ctx, mapping_id, key)
        : GET_KEY_ON_DEFAULT_MAPPING__YAML(
            ctx->yaml_load_res, FIRST_DOCUMENT, key);

    if (key_value_id == -1) {
        return NULL;
    }

    node =
      get_node_from_id__YAML(ctx->yaml_load_res, FIRST_DOCUMENT, key_value_id);

    ASSERT(node);

    switch (GET_NODE_TYPE__YAML(node)) {
        case YAML_SCALAR_NODE: {
            char *key_value = GET_NODE_SCALAR_VALUE__YAML(node);

            return from__String(key_value);
        }
        default:
            FAILED("expected scalar value");
    }
}

Int32
get_integer_from_scalar_value__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  SizedStr values[],
  Int32 value_ids[],
  Usize values_len)
{
    String *value = get_scalar_value__CIProjectConfig(ctx, mapping_id, key);
    Int32 res = get_id__Search(value, values, value_ids, values_len);

    if (res == -1) {
        FAILED("invalid value");
    }

    FREE(String, value);

    return res;
}

Int32
get_integer_from_optional_scalar_value__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  SizedStr values[],
  Int32 value_ids[],
  Usize values_len,
  Int32 null_value)
{
    String *value =
      get_optional_scalar_value__CIProjectConfig(ctx, mapping_id, key);

    if (!value) {
        return null_value;
    }

    Int32 res = get_id__Search(value, values, value_ids, values_len);

    FREE(String, value);

    if (res == -1) {
        FAILED("invalid value");
    }

    return res;
}

Vec *
get_sequence_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                    Int32 mapping_id,
                                    char *key)
{
    Vec *res =
      get_optional_sequence_value__CIProjectConfig(ctx, mapping_id, key);

    if (!res) {
        FAILED("expected to have this key");
    }

    return res;
}

Vec *
get_optional_sequence_value__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                             Int32 mapping_id,
                                             char *key)
{
    YAMLNode *node;

    Int32 key_value_id =
      mapping_id != -1
        ? get_optional_node__CIProjectConfig(ctx, mapping_id, key)
        : GET_KEY_ON_DEFAULT_MAPPING__YAML(
            ctx->yaml_load_res, FIRST_DOCUMENT, key);

    if (key_value_id == -1) {
        return NULL;
    }

    node =
      get_node_from_id__YAML(ctx->yaml_load_res, FIRST_DOCUMENT, key_value_id);

    ASSERT(node);

    Vec *res = NEW(Vec); // Vec<String*>*

    switch (GET_NODE_TYPE__YAML(node)) {
        case YAML_SEQUENCE_NODE:
            ITER_ON_SEQUENCE_NODE__YAML(
              ctx->yaml_load_res, FIRST_DOCUMENT, node, sequence_item, {
                  push__Vec(res, from__String(sequence_item_value));
              });

            break;
        default:
            FAILED("expected sequence value");
    }

    return res;
}

[[maybe_unused]] Vec *
iter_on_mapping__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  void *(*parse_item)(struct CIProjectConfigContext *ctx,
                      Int32 mapping_id,
                      const char *key_name))
{
    Vec *res = iter_on_optional_mapping__CIProjectConfig(
      ctx, mapping_id, key, parse_item);

    if (!res) {
        FAILED("expected to have mapping here");
    }

    return res;
}

Vec *
iter_on_optional_mapping__CIProjectConfig(
  struct CIProjectConfigContext *ctx,
  Int32 mapping_id,
  char *key,
  void *(*parse_item)(struct CIProjectConfigContext *ctx,
                      Int32 mapping_id,
                      const char *key_name))
{
    Int32 node_id = get_optional_node__CIProjectConfig(ctx, mapping_id, key);

    if (node_id == -1) {
        return NULL;
    }

    YAMLNode *node =
      get_node_from_id__YAML(ctx->yaml_load_res, FIRST_DOCUMENT, node_id);

    switch (GET_NODE_TYPE__YAML(node)) {
        case YAML_MAPPING_NODE: {
            Vec *res = NEW(Vec);

            ITER_ON_MAPPING_NODE__YAML(
              ctx->yaml_load_res, FIRST_DOCUMENT, node, pair, {
                  push__Vec(res, parse_item(ctx, pair->value, pair_key));
              });

            return res;
        }
        default:
            return NULL;
    }
}

enum CIStandard
get_standard__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // standard: k&r | c89 | c95 | c99 | c11 | c17 | c23

    // NOTE: This array must be in alphabetical order.
    SizedStr values[] = {
        SIZED_STR_FROM_RAW("c11"), SIZED_STR_FROM_RAW("c17"),
        SIZED_STR_FROM_RAW("c23"), SIZED_STR_FROM_RAW("c89"),
        SIZED_STR_FROM_RAW("c95"), SIZED_STR_FROM_RAW("c99"),
        SIZED_STR_FROM_RAW("k&r"),
    };
    Int32 value_ids[] = {
        CI_STANDARD_11, CI_STANDARD_17, CI_STANDARD_23, CI_STANDARD_89,
        CI_STANDARD_95, CI_STANDARD_99, CI_STANDARD_KR,
    };
    const Usize values_len = LEN(values, *values);
    const Usize value_ids_len = LEN(value_ids, *value_ids);

    static_assert(values_len == value_ids_len);

    return get_integer_from_optional_scalar_value__CIProjectConfig(
      ctx, -1, "standard", values, value_ids, values_len, CI_STANDARD_99);
}

CIProjectConfigCompiler
get_compiler__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // compiler:
    //   name: clang | gcc
    //   command: <path_of_command> | <command>

    Int32 compiler_value_id = get_node__CIProjectConfig(ctx, -1, "compiler");
    // NOTE: This array must be in alphabetical order.
    SizedStr compiler_names[] = {
        SIZED_STR_FROM_RAW("clang"),
        SIZED_STR_FROM_RAW("gcc"),
    };
    Int32 compiler_ids[] = { CI_PROJECT_CONFIG_COMPILER_KIND_CLANG,
                             CI_PROJECT_CONFIG_COMPILER_KIND_GCC };
    const Usize compiler_names_len = LEN(compiler_names, *compiler_names);
    const Usize compiler_ids_len = LEN(compiler_ids, *compiler_ids);

    static_assert(compiler_names_len == compiler_ids_len);

    enum CIProjectConfigCompilerKind compiler_kind =
      get_integer_from_scalar_value__CIProjectConfig(ctx,
                                                     compiler_value_id,
                                                     "name",
                                                     compiler_names,
                                                     compiler_ids,
                                                     compiler_names_len);
    String *command =
      get_scalar_value__CIProjectConfig(ctx, compiler_value_id, "command");
    bool command_is_absolute_path =
      !is_empty__String(command) && get__String(command, 0) == '/';

    return NEW(CIProjectConfigCompiler,
               compiler_kind,
               command_is_absolute_path
                 ? command
                 : format__String("{s}" DIR_SEPARATOR_S "{Sr}",
                                  ctx->absolute_config_dir,
                                  command));
}

void
get_std_include__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // std_include: <path>

    String *path =
      get_optional_scalar_value__CIProjectConfig(ctx, -1, "std_include");

    if (path) {
        // NOTE: We insert the standard include directory in first, to take
        // precedence over other include directories.
        insert_include_dir__CIInclude(
          convert_path_to_absolute_path__CIProjectConfig(
            path->buffer, ctx->absolute_config_dir),
          0);

        FREE(String, path);
    }
}

void
get_include_dirs__CIProjectConfig(struct CIProjectConfigContext *ctx,
                                  const String *compiler_path)
{
    // include_dirs:
    //   - dir
    //   - dir2
    //   ...

    // Initialize include directories workspace
    init_include_dirs__CIInclude(compiler_path, ctx->absolute_config_dir);

    Vec *include_dir_values = get_optional_sequence_value__CIProjectConfig(
      ctx, -1, "include_dirs"); // Vec<String*>*

    if (include_dir_values) {
        for (Usize i = 0; i < include_dir_values->len; ++i) {
            add_include_dir__CIInclude(get__Vec(include_dir_values, i));
        }

        FREE(Vec, include_dir_values);
    }
}

CIProjectConfigLibrary *
get_library__CIProjectConfig(struct CIProjectConfigContext *ctx,
                             Int32 mapping_id,
                             const char *library_name)
{
    // <library_name>:
    //   paths:
    //     - <path_1>
    //     - <path_2>
    //     - <path_3>

    Vec *paths = get_sequence_value__CIProjectConfig(
      ctx, mapping_id, "paths"); // Vec<String*>*

    // Make paths absolute.
    for (Usize i = 0; i < paths->len; ++i) {
        replace__Vec(paths,
                     i,
                     format__String("{s}" DIR_SEPARATOR_S "{Sr}",
                                    ctx->absolute_config_dir,
                                    get__Vec(paths, i)));
    }

    return NEW(
      CIProjectConfigLibrary, from__String((char *)library_name), paths);
}

Vec *
get_libraries__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // libraries:
    //   <library_name>:
    //     paths:
    //       - <path_1>
    //       - <path_2>
    //       - <path_3>
    //       - ...
    //   <library_name>:
    //     paths:
    //       - <path_1>
    //       - <path_2>
    //       - ...
    //   ...

    return iter_on_optional_mapping__CIProjectConfig(
      ctx,
      -1,
      "libraries",
      (void *(*)(struct CIProjectConfigContext *, Int32, const char *)) &
        get_library__CIProjectConfig);
}

CIProjectConfigBin *
get_bin__CIProjectConfig(struct CIProjectConfigContext *ctx,
                         Int32 mapping_id,
                         const char *bin_name)
{
    // <bin_name>:
    //   path: <file_path>

    String *path = get_scalar_value__CIProjectConfig(ctx, mapping_id, "path");
    path = format__String(
      "{s}" DIR_SEPARATOR_S "{Sr}", ctx->absolute_config_dir, path);

    return NEW(CIProjectConfigBin, from__String((char *)bin_name), path);
}

Vec *
get_bins__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // bins:
    //   <bin_name>:
    //     path: <file_path>
    //  ...

    return iter_on_optional_mapping__CIProjectConfig(
      ctx,
      -1,
      "bins",
      (void *(*)(struct CIProjectConfigContext *, Int32, const char *)) &
        get_bin__CIProjectConfig);
}

Vec *
get_self_tests__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // self_tests:
    //   - <path_1>
    //   - <path_2>
    //   - <path_3>
    // ...

    Vec *self_tests = get_optional_sequence_value__CIProjectConfig(
      ctx, -1, "self_tests"); // Vec<String* | CIProjectConfigSelfTest*>*?

    if (self_tests) {
        for (Usize i = 0; i < self_tests->len; ++i) {
            // self_tests becomes only Vec<CIProjectConfigSelfTest*>*
            replace__Vec(self_tests,
                         i,
                         NEW(CIProjectConfigSelfTest,
                             format__String("{s}/{Sr}",
                                            ctx->absolute_config_dir,
                                            get__Vec(self_tests, i))));
        }
    }

    return self_tests;
}

bool
get_no_state_check__CIProjectConfig(struct CIProjectConfigContext *ctx)
{
    // no_state_check: true | false

    enum NoStateCheckValue
    {
        NO_STATE_CHECK_VALUE_FALSE = 0,
        NO_STATE_CHECK_VALUE_TRUE = 1,
    };
    // NOTE: This array must be in alphabetical order.
    SizedStr no_state_check_values[] = { SIZED_STR_FROM_RAW("false"),
                                         SIZED_STR_FROM_RAW("true") };
    Int32 no_state_check_ids[] = { NO_STATE_CHECK_VALUE_FALSE,
                                   NO_STATE_CHECK_VALUE_TRUE };
    const Usize no_state_check_values_len =
      LEN(no_state_check_values, *no_state_check_values);
    const Usize no_state_check_ids_len =
      LEN(no_state_check_ids, *no_state_check_ids);

    static_assert(no_state_check_values_len == no_state_check_ids_len);

    return get_integer_from_optional_scalar_value__CIProjectConfig(
      ctx,
      -1,
      "no_state_check",
      no_state_check_values,
      no_state_check_ids,
      no_state_check_values_len,
      NO_STATE_CHECK_VALUE_FALSE);
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

CONSTRUCTOR(CIProjectConfigSelfTest *, CIProjectConfigSelfTest, String *path)
{
    CIProjectConfigSelfTest *self =
      lily_malloc(sizeof(CIProjectConfigSelfTest));

    self->path = path;

    return self;
}

DESTRUCTOR(CIProjectConfigSelfTest, CIProjectConfigSelfTest *self)
{
    FREE(String, self->path);
    lily_free(self);
}

CIProjectConfig
parse_cic_cli__CIProjectConfig(const CIcConfig *cli_config)
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
                       NULL,
                       init__Vec(1,
                                 NEW(CIProjectConfigBin,
                                     bin_name,
                                     from__String((char *)cli_config->path))),
                       NULL,
                       cli_config->no_state_check);
}

CIProjectConfig
parse_ci_self_test__CIProjectConfig(const CIConfigSelfTest *cli_config)
{
    return NEW_VARIANT(
      CIProjectConfig,
      cli,
      CI_STANDARD_NONE,
      NEW(CIProjectConfigCompiler, CI_PROJECT_CONFIG_COMPILER_KIND_NONE, NULL),
      NULL,
      NULL,
      NULL,
      init__Vec(
        1,
        NEW(CIProjectConfigSelfTest, from__String((char *)cli_config->path))),
      false);
}

CIProjectConfig
parse_ci_cli__CIProjectConfig(const CIConfig *cli_config)
{
    switch (cli_config->kind) {
        case CI_CONFIG_KIND_COMPILE:
            return parse_cic_cli__CIProjectConfig(&cli_config->compile);
        case CI_CONFIG_KIND_SELF_TEST:
            return parse_ci_self_test__CIProjectConfig(&cli_config->self_test);
        default:
            UNREACHABLE("unknown variant");
    }
}

CIProjectConfig
parse_yaml__CIProjectConfig(const char *config_dir)
{
    String *absolute_config_path = convert_to_absolute_path__Path(config_dir);
    String *path_ci_config = exists_rec__File(absolute_config_path->buffer,
                                              CI_CONFIG); // <path>/CI.yaml

    if (!path_ci_config) {
        FAILED("could not find CI.yaml in current working directory or parent "
               "directory");
    }

    FREE(String, absolute_config_path);

    String *absolute_config_dir = get_dir__File(path_ci_config->buffer);
    YAMLLoadRes yaml_load_res = load__YAML(path_ci_config->buffer);
    struct CIProjectConfigContext ctx =
      NEW(CIProjectConfigContext, &yaml_load_res, absolute_config_dir->buffer);
    enum CIStandard standard = get_standard__CIProjectConfig(&ctx);
    CIProjectConfigCompiler compiler = get_compiler__CIProjectConfig(&ctx);

    get_include_dirs__CIProjectConfig(&ctx, compiler.command);
    get_std_include__CIProjectConfig(&ctx);

    Vec *libraries = get_libraries__CIProjectConfig(&ctx);
    Vec *bins = get_bins__CIProjectConfig(&ctx);
    Vec *self_tests = get_self_tests__CIProjectConfig(&ctx);
    bool no_state_check = get_no_state_check__CIProjectConfig(&ctx);

    FREE(String, path_ci_config);
    FREE(String, absolute_config_dir);

    return NEW_VARIANT(CIProjectConfig,
                       yaml,
                       yaml_load_res,
                       standard,
                       compiler,
                       get_include_dirs__CIInclude(),
                       libraries,
                       bins,
                       self_tests,
                       no_state_check);
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

    destroy__CIInclude();

    if (self->libraries) {
        FREE_BUFFER_ITEMS(self->libraries->buffer,
                          self->libraries->len,
                          CIProjectConfigLibrary);
        FREE(Vec, self->libraries);
    }

    if (self->bins) {
        FREE_BUFFER_ITEMS(
          self->bins->buffer, self->bins->len, CIProjectConfigBin);
        FREE(Vec, self->bins);
    }

    if (self->self_tests) {
        FREE_BUFFER_ITEMS(self->self_tests->buffer,
                          self->self_tests->len,
                          CIProjectConfigSelfTest);
        FREE(Vec, self->self_tests);
    }
}
