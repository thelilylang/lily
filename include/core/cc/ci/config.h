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

#ifndef LILY_CORE_CC_CI_CONFIG_H
#define LILY_CORE_CC_CI_CONFIG_H

#include <base/alloc.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <base/yaml.h>

#include <core/cc/ci/features.h>

enum CICompilerKind
{
    CI_COMPILER_KIND_CLANG,
    CI_COMPILER_KIND_GCC,
};

typedef struct CICompiler
{
    enum CICompilerKind kind;
    String *path;
} CICompiler;

/**
 *
 * @brief Construct CICompiler type.
 */
inline CONSTRUCTOR(CICompiler,
                   CICompiler,
                   enum CICompilerKind kind,
                   String *path)
{
    return (CICompiler){ .kind = kind, .path = path };
}

/**
 *
 * @brief Free CICompiler type.
 */
inline DESTRUCTOR(CICompiler, const CICompiler *self)
{
    FREE(String, self->path);
}

typedef struct CILibrary
{
    const char *name; // const char* (&)
    Vec *paths;       // Vec<String*>*
} CILibrary;

/**
 *
 * @brief Construct CILibrary type.
 */
CONSTRUCTOR(CILibrary *, CILibrary, const char *name, Vec *paths);

/**
 *
 * @brief Free CILibrary type.
 */
DESTRUCTOR(CILibrary, CILibrary *self);

typedef struct CIBin
{
    const char *name; // const char* (&)
    String *path;
} CIBin;

/**
 *
 * @brief Construct CIBin type.
 */
CONSTRUCTOR(CIBin *, CIBin, const char *name, String *path);

/**
 *
 * @brief Free CIBin type.
 */
DESTRUCTOR(CIBin, CIBin *self);

typedef struct CIConfig
{
    YAMLLoadRes yaml_load_res;
    enum CIStandard standard;
    CICompiler compiler;
    const Vec *include_dirs; // Vec<char* (&)>* (&)
    Vec *libraries;          // Vec<CILibrary*>*
    Vec *bins;               // Vec<CIBin*>*
} CIConfig;

/**
 *
 * @brief Construct CIConfig type.
 */
inline CONSTRUCTOR(CIConfig,
                   CIConfig,
                   YAMLLoadRes yaml_load_res,
                   enum CIStandard standard,
                   CICompiler compiler,
                   const Vec *include_dirs,
                   Vec *libraries,
                   Vec *bins)
{
    return (CIConfig){ .yaml_load_res = yaml_load_res,
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
CIConfig
parse__CIConfig(const char *config_dir);

/**
 *
 * @brief Free CIConfig type.
 */
DESTRUCTOR(CIConfig, const CIConfig *self);

#endif // LILY_CORE_CC_CI_CONFIG_H
