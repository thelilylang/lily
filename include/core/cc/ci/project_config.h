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

#ifndef LILY_CORE_CC_CI_PARSE_CONFIG_H
#define LILY_CORE_CC_CI_PARSE_CONFIG_H

#include <base/alloc.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <base/yaml.h>

#include <core/cc/ci/features.h>

enum CIProjectConfigCompilerKind
{
    CI_COMPILER_KIND_CLANG,
    CI_COMPILER_KIND_GCC,
};

typedef struct CIProjectConfigCompiler
{
    enum CIProjectConfigCompilerKind kind;
    String *path;
} CIProjectConfigCompiler;

/**
 *
 * @brief Construct CIProjectConfigCompiler type.
 */
inline CONSTRUCTOR(CIProjectConfigCompiler,
                   CIProjectConfigCompiler,
                   enum CIProjectConfigCompilerKind kind,
                   String *path)
{
    return (CIProjectConfigCompiler){ .kind = kind, .path = path };
}

/**
 *
 * @brief Free CIProjectConfigCompiler type.
 */
inline DESTRUCTOR(CIProjectConfigCompiler, const CIProjectConfigCompiler *self)
{
    FREE(String, self->path);
}

typedef struct CIProjectConfigLibrary
{
    const char *name; // const char* (&)
    Vec *paths;       // Vec<String*>*
} CIProjectConfigLibrary;

/**
 *
 * @brief Construct CIProjectConfigLibrary type.
 */
CONSTRUCTOR(CIProjectConfigLibrary *,
            CIProjectConfigLibrary,
            const char *name,
            Vec *paths);

/**
 *
 * @brief Free CIProjectConfigLibrary type.
 */
DESTRUCTOR(CIProjectConfigLibrary, CIProjectConfigLibrary *self);

typedef struct CIProjectConfigBin
{
    const char *name; // const char* (&)
    String *path;
} CIProjectConfigBin;

/**
 *
 * @brief Construct CIProjectConfigBin type.
 */
CONSTRUCTOR(CIProjectConfigBin *,
            CIProjectConfigBin,
            const char *name,
            String *path);

/**
 *
 * @brief Free CIProjectConfigBin type.
 */
DESTRUCTOR(CIProjectConfigBin, CIProjectConfigBin *self);

typedef struct CIProjectConfig
{
    YAMLLoadRes yaml_load_res;
    enum CIStandard standard;
    CIProjectConfigCompiler compiler;
    const Vec *include_dirs; // Vec<char* (&)>* (&)
    Vec *libraries;          // Vec<CIProjectConfigLibrary*>*
    Vec *bins;               // Vec<CIProjectConfigBin*>*
} CIProjectConfig;

/**
 *
 * @brief Construct CIProjectConfig type.
 */
inline CONSTRUCTOR(CIProjectConfig,
                   CIProjectConfig,
                   YAMLLoadRes yaml_load_res,
                   enum CIStandard standard,
                   CIProjectConfigCompiler compiler,
                   const Vec *include_dirs,
                   Vec *libraries,
                   Vec *bins)
{
    return (CIProjectConfig){ .yaml_load_res = yaml_load_res,
                              .standard = standard,
                              .compiler = compiler,
                              .include_dirs = include_dirs,
                              .libraries = libraries,
                              .bins = bins };
}

/**
 *
 * @brief Parse CI configuration.
 */
CIProjectConfig
parse__CIProjectConfig(const char *config_dir);

/**
 *
 * @brief Free CIProjectConfig type.
 */
DESTRUCTOR(CIProjectConfig, const CIProjectConfig *self);

#endif // LILY_CORE_CC_CI_PARSE_CONFIG_H
