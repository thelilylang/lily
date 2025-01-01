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

#ifndef LILY_CORE_CC_CI_PARSE_CONFIG_H
#define LILY_CORE_CC_CI_PARSE_CONFIG_H

#include <base/alloc.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <base/yaml.h>

#include <cli/ci/config.h>
#include <core/cc/ci/features.h>

enum CIProjectConfigCompilerKind
{
    CI_PROJECT_CONFIG_COMPILER_KIND_CLANG,
    CI_PROJECT_CONFIG_COMPILER_KIND_GCC,
};

typedef struct CIProjectConfigCompiler
{
    enum CIProjectConfigCompilerKind kind;
    String *command;
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
    FREE(String, self->command);
}

typedef struct CIProjectConfigLibrary
{
    String *name;
    Vec *paths; // Vec<String*>*
} CIProjectConfigLibrary;

/**
 *
 * @brief Construct CIProjectConfigLibrary type.
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
    Vec *libraries;          // Vec<CIProjectConfigLibrary*>*
    Vec *bins;               // Vec<CIProjectConfigBin*>*
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
                           Vec *bins)
{
    return (CIProjectConfig){ .kind = CI_PROJECT_CONFIG_KIND_CLI,
                              .standard = standard,
                              .compiler = compiler,
                              .include_dirs = include_dirs,
                              .libraries = libraries,
                              .bins = bins };
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
                           Vec *bins)
{
    return (CIProjectConfig){ .kind = CI_PROJECT_CONFIG_KIND_YAML,
                              .yaml = yaml,
                              .standard = standard,
                              .compiler = compiler,
                              .include_dirs = include_dirs,
                              .libraries = libraries,
                              .bins = bins };
}

/**
 *
 * @brief Parse CLI configuration.
 */
CIProjectConfig
parse_cli__CIProjectConfig(const CIConfig *cli_config);

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
