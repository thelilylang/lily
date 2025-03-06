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

#ifndef LILY_CLI_CIC_CONFIG_H
#define LILY_CLI_CIC_CONFIG_H

#include <base/macros.h>
#include <base/vec.h>

#include <core/cc/ci/features.h>

enum CIcConfigMode
{
    CIC_CONFIG_MODE_DEBUG,
    CIC_CONFIG_MODE_NONE,
    CIC_CONFIG_MODE_RELEASE,
};

typedef struct CIcConfig
{
    // PROJECT_PATH | FILE_PATH
    const char *path; // const char* (&)
    enum CIcConfigMode mode;
    bool file;
    enum CIStandard standard;
    // Store values passed via the `-I`/`--include` option
    Vec *includes; // Vec<char* (&)>*

    // Store values passed via the `--include0` option
    Vec *includes0; // Vec<char* (&)>*
    bool no_state_check;
} CIcConfig;

/**
 *
 * @brief Construct CIcConfig type.
 */
inline CONSTRUCTOR(CIcConfig,
                   CIcConfig,
                   const char *path,
                   enum CIcConfigMode mode,
                   bool file,
                   enum CIStandard standard,
                   Vec *includes,
                   Vec *includes0,
                   bool no_state_check)
{
    return (CIcConfig){ .path = path,
                        .mode = mode,
                        .file = file,
                        .standard = standard,
                        .includes = includes,
                        .includes0 = includes0,
                        .no_state_check = no_state_check };
}

/**
 *
 * @brief Free CIcConfig type.
 */
DESTRUCTOR(CIcConfig, const CIcConfig *self);

#endif // LILY_CLI_CIC_CONFIG_H
