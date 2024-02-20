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

#ifndef LILY_BASE_YAML
#define LILY_BASE_YAML

#include <base/alloc.h>
#include <base/vec.h>

#include <local/src/libyaml/src/yaml.h>

typedef yaml_char_t YAMLChar;
typedef yaml_document_t YAMLDocument;
typedef yaml_emitter_t YAMLEmitter;
typedef yaml_event_t YAMLEvent;
typedef yaml_node_t YAMLNode;
typedef yaml_parser_t YAMLParser;

typedef struct YAMLLoadRes
{
    YAMLDocument *documents; // YAMLDocument*?
    Usize len;
} YAMLLoadRes;

/**
 *
 * @brief Construct YAMLLoadRes type.
 */
inline CONSTRUCTOR(YAMLLoadRes, YAMLLoadRes, YAMLDocument *documents, Usize len)
{
    return (YAMLLoadRes){ .documents = documents, .len = len };
}

YAMLLoadRes
init__YAMLLoadRes();

/**
 *
 * @brief Free YAMLLoadRes type.
 */
DESTRUCTOR(YAMLLoadRes, const YAMLLoadRes *self);

/**
 *
 * @brief Load YAML file.
 * @url https://github.com/thelilylang/libyaml/blob/master/tests/run-loader.c
 * @return YAMLDocument*
 */
YAMLLoadRes
load__YAML(const char *filename);

/**
 *
 * @brief Write YAML file from YAMLLoadRes type.
 */
void
write__YAML(const YAMLLoadRes *self, const char *filename);

/**
 *
 * @brief Print (parsed) YAML file.
 */
void
dump__YAML(const YAMLLoadRes *self);

/**
 *
 * @brief Add YAML scalar to YAMLDocument.
 */
void
add_scalar__YAML(YAMLLoadRes *self,
                 Int32 document_id,
                 const char *key,
                 const char *value);

/**
 *
 * @brief Add YAML sequence to YAMLDocument.
 */
void
add_sequence__YAML(YAMLLoadRes *self,
                   Int32 document_id,
                   const char *key,
                   Usize n,
                   ...);

#endif // LILY_BASE_YAML
