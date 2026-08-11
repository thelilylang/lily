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

#ifndef LILY_CORE_CC_CI_PARSE_CONFIG_H
#define LILY_CORE_CC_CI_PARSE_CONFIG_H

#include <base/alloc.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <base/yaml.h>

#include <cli/ci/config.h>
#include <cli/cic/config.h>
#include <core/cc/ci/features.h>

enum CIProjectConfigCompilerKind
{
    CI_PROJECT_CONFIG_COMPILER_KIND_CLANG,
    CI_PROJECT_CONFIG_COMPILER_KIND_GCC,
    CI_PROJECT_CONFIG_COMPILER_KIND_NONE
};

typedef struct CIProjectConfigCompiler
{
    enum CIProjectConfigCompilerKind kind;
    String *command; // String*?
} CIProjectConfigCompiler;

/**
 *
 * @brief Construct CIProjectConfigCompiler type.
 */
inline CONSTRUCTOR(CIProjectConfigCompiler,
                   CIProjectConfigCompiler,
                   enum CIProjectConfigCompilerKind kind,
                   String *command)
{
    return (CIProjectConfigCompiler){ .kind = kind, .command = command };
}

/**
 *
 * @brief Free CIProjectConfigCompiler type.
 */
inline DESTRUCTOR(CIProjectConfigCompiler, const CIProjectConfigCompiler *self)
{
    if (self->command) {
        FREE(String, self->command);
    }
}

typedef struct CIProjectConfigLibrary
{
    String *name;
    Vec *paths; // Vec<String*>*
} CIProjectConfigLibrary;

/**
 *
 * @brief Construct CIProjectConfigLibrary type.
 * @param paths Vec<String*>*
 */
CONSTRUCTOR(CIProjectConfigLibrary *,
            CIProjectConfigLibrary,
            String *name,
            Vec *paths);

/**
 *
 * @brief Free CIProjectConfigLibrary type.
 */
DESTRUCTOR(CIProjectConfigLibrary, CIProjectConfigLibrary *self);

typedef struct CIProjectConfigBin
{
    String *name;
    String *path;
} CIProjectConfigBin;

/**
 *
 * @brief Construct CIProjectConfigBin type.
 */
CONSTRUCTOR(CIProjectConfigBin *,
            CIProjectConfigBin,
            String *name,
            String *path);

/**
 *
 * @brief Free CIProjectConfigBin type.
 */
DESTRUCTOR(CIProjectConfigBin, CIProjectConfigBin *self);

typedef struct CIProjectConfigSelfTest
{
    String *path;
} CIProjectConfigSelfTest;

/**
 *
 * @brief Construct CIProjectConfigSelfTest type.
 */
CONSTRUCTOR(CIProjectConfigSelfTest *, CIProjectConfigSelfTest, String *path);

/**
 *
 * @brief Free CIProjectConfigSelfTest type.
 */
DESTRUCTOR(CIProjectConfigSelfTest, CIProjectConfigSelfTest *self);

// Name convention a function follows to be callable as a method, written with
// `$type` standing for the name of the receiver and `$name` for the name the
// method is called by - `$type_$name` reads `str_len` as `len` on `str`.
//
// The convention belongs to the declaration, not to the call: a name is built
// forward from a receiver and a method name, and what is found is only kept
// where it was declared under the convention it was built with. Nothing is
// ever matched backwards, as `$type_$name` splits `str__len` two ways.
typedef struct CIProjectConfigMethodConvention
{
    String *pattern; // as written, kept to report on
    String *prefix;  // what precedes `$type`
    String *sep;     // what separates `$type` from `$name`
    String *suffix;  // what follows `$name`
} CIProjectConfigMethodConvention;

/**
 *
 * @brief Read a method convention out of the pattern it is written as.
 * @param pattern The pattern, which must hold `$type` once and `$name` once,
 * in that order.
 */
CONSTRUCTOR(CIProjectConfigMethodConvention *,
            CIProjectConfigMethodConvention,
            const char *pattern);

/**
 *
 * @brief Build the name a method is declared by on a receiver.
 * @return String*
 */
String *
build_name__CIProjectConfigMethodConvention(
  const CIProjectConfigMethodConvention *self,
  const String *type_name,
  const String *method_name);

/**
 *
 * @brief Free CIProjectConfigMethodConvention type.
 */
DESTRUCTOR(CIProjectConfigMethodConvention,
           CIProjectConfigMethodConvention *self);

// Name convention a type follows to be given methods, written with `$name`
// standing for the name of the type - `$name_t` gives methods to `vec_t` and
// to nothing else. Where none is written every type is eligible.
typedef struct CIProjectConfigTypeConvention
{
    String *pattern; // as written, kept to report on
    String *prefix;  // what precedes `$name`
    String *suffix;  // what follows `$name`
} CIProjectConfigTypeConvention;

/**
 *
 * @brief Read a type convention out of the pattern it is written as.
 * @param pattern The pattern, which must hold `$name` once.
 */
CONSTRUCTOR(CIProjectConfigTypeConvention *,
            CIProjectConfigTypeConvention,
            const char *pattern);

/**
 *
 * @brief Whether a type written by this name is given methods.
 */
bool
match__CIProjectConfigTypeConvention(const CIProjectConfigTypeConvention *self,
                                     const String *type_name);

/**
 *
 * @brief Free CIProjectConfigTypeConvention type.
 */
DESTRUCTOR(CIProjectConfigTypeConvention, CIProjectConfigTypeConvention *self);

enum CIProjectConfigKind
{
    CI_PROJECT_CONFIG_KIND_CLI,  // config values passed by a CLI
    CI_PROJECT_CONFIG_KIND_YAML, // config values passed by a YAML config
};

typedef struct CIProjectConfig
{
    enum CIProjectConfigKind kind;
    union
    {
        YAMLLoadRes yaml;
    };
    enum CIStandard standard;
    CIProjectConfigCompiler compiler;
    const Vec *include_dirs; // Vec<char* (&)>* (&)
    Vec *libraries;          // Vec<CIProjectConfigLibrary*>*?
    Vec *bins;               // Vec<CIProjectConfigBin*>*?
    Vec *self_tests;         // Vec<CIProjectConfigSelfTest*>*?
    bool no_state_check;

    // Where no method convention is written, no function is callable as a
    // method. Where no type convention is written, every type is eligible.
    CIProjectConfigMethodConvention
      *method_convention; // CIProjectConfigMethodConvention*?
    CIProjectConfigTypeConvention
      *type_convention; // CIProjectConfigTypeConvention*?
} CIProjectConfig;

/**
 *
 * @brief Construct CIProjectConfig type (CI_PROJECT_CONFIG_KIND_CLI).
 */
inline VARIANT_CONSTRUCTOR(CIProjectConfig,
                           CIProjectConfig,
                           cli,
                           enum CIStandard standard,
                           CIProjectConfigCompiler compiler,
                           const Vec *include_dirs,
                           Vec *libraries,
                           Vec *bins,
                           Vec *self_tests,
                           bool no_state_check,
                           CIProjectConfigMethodConvention *method_convention,
                           CIProjectConfigTypeConvention *type_convention)
{
    return (CIProjectConfig){ .kind = CI_PROJECT_CONFIG_KIND_CLI,
                              .standard = standard,
                              .compiler = compiler,
                              .include_dirs = include_dirs,
                              .libraries = libraries,
                              .bins = bins,
                              .self_tests = self_tests,
                              .no_state_check = no_state_check,
                              .method_convention = method_convention,
                              .type_convention = type_convention };
}

/**
 *
 * @brief Construct CIProjectConfig type (CI_PROJECT_CONFIG_KIND_YAML).
 */
inline VARIANT_CONSTRUCTOR(CIProjectConfig,
                           CIProjectConfig,
                           yaml,
                           YAMLLoadRes yaml,
                           enum CIStandard standard,
                           CIProjectConfigCompiler compiler,
                           const Vec *include_dirs,
                           Vec *libraries,
                           Vec *bins,
                           Vec *self_tests,
                           bool no_state_check,
                           CIProjectConfigMethodConvention *method_convention,
                           CIProjectConfigTypeConvention *type_convention)
{
    return (CIProjectConfig){ .kind = CI_PROJECT_CONFIG_KIND_YAML,
                              .yaml = yaml,
                              .standard = standard,
                              .compiler = compiler,
                              .include_dirs = include_dirs,
                              .libraries = libraries,
                              .bins = bins,
                              .self_tests = self_tests,
                              .no_state_check = no_state_check,
                              .method_convention = method_convention,
                              .type_convention = type_convention };
}

/**
 *
 * @brief Parse CLI of cci configuration.
 */
CIProjectConfig
parse_cic_cli__CIProjectConfig(const CIcConfig *cli_config);

/**
 *
 * @brief Parse CLI of ci configuration.
 */
CIProjectConfig
parse_ci_cli__CIProjectConfig(const CIConfig *cli_config);

/**
 *
 * @brief Parse YAML CI configuration.
 */
CIProjectConfig
parse_yaml__CIProjectConfig(const char *config_dir);

/**
 *
 * @brief Free CIProjectConfig type.
 */
DESTRUCTOR(CIProjectConfig, const CIProjectConfig *self);

#endif // LILY_CORE_CC_CI_PARSE_CONFIG_H
