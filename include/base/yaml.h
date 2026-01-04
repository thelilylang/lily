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

#ifndef LILY_BASE_YAML_H
#define LILY_BASE_YAML_H

#include <base/alloc.h>
#include <base/assert.h>
#include <base/vec.h>

#include <local/src/libyaml/src/yaml.h>

#define FIRST_DOCUMENT 0

#define GET_KEY_ON_DEFAULT_MAPPING__YAML(self, document_id, key) \
    get_key__YAML(self, document_id, -1, key)

#define GET_NODE_TYPE__YAML(node) node->type
#define GET_NODE_SCALAR_VALUE__YAML(node) (char *)node->data.scalar.value

// - data1
// - data2
// - data3
// ...
#define ITER_ON_SEQUENCE_NODE__YAML(load, document, node, item, body) \
    for (yaml_node_item_t *item = node->data.sequence.items.start;    \
         item < node->data.sequence.items.top;                        \
         ++item) {                                                    \
        [[maybe_unused]] YAMLNode *item##_node =                      \
          get_node_from_id__YAML(load, document, *item);              \
                                                                      \
        ASSERT(item##_node);                                          \
                                                                      \
        [[maybe_unused]] char *item##_value =                         \
          GET_NODE_SCALAR_VALUE__YAML(item##_node);                   \
                                                                      \
        body;                                                         \
    }

// ...:
//   <key1>: <data1>
//   <key2>: <data2>
//   <key3>: <data3>
//   ...
#define ITER_ON_MAPPING_NODE__YAML(load, document, node, pair, body) \
    for (yaml_node_pair_t *pair = node->data.mapping.pairs.start;    \
         pair < node->data.mapping.pairs.top;                        \
         ++pair) {                                                   \
        [[maybe_unused]] YAMLNode *pair##_key_node =                 \
          get_node_from_id__YAML(load, document, pair->key);         \
        [[maybe_unused]] YAMLNode *pair##_value_node =               \
          get_node_from_id__YAML(load, document, pair->value);       \
                                                                     \
        ASSERT(pair##_key_node);                                     \
        ASSERT(pair##_value_node);                                   \
                                                                     \
        [[maybe_unused]] char *pair##_key =                          \
          GET_NODE_SCALAR_VALUE__YAML(pair##_key_node);              \
        [[maybe_unused]] char *pair##_value =                        \
          GET_NODE_SCALAR_VALUE__YAML(pair##_value_node);            \
                                                                     \
        body;                                                        \
    }

#define ITER_ON_MAPPING_NODE_WITH_EXPECTED_KEYS__YAML(                   \
  load, document, node, pair, expected_keys, body)                       \
    ITER_ON_MAPPING_NODE__YAML(load, document, node, pair, {             \
        /* Check if the key matches with an expected key. */             \
        bool is_match = false;                                           \
                                                                         \
        for (Usize i = 0; i < LEN(expected_keys, *expected_keys); ++i) { \
            if (!strcmp(pair##_key, expected_keys[i])) {                 \
                is_match = true;                                         \
                break;                                                   \
            }                                                            \
        }                                                                \
                                                                         \
        if (!is_match) {                                                 \
            FAILED("this key doesn't match with expected keys");         \
        }                                                                \
                                                                         \
        body;                                                            \
    });

typedef yaml_char_t YAMLChar;
typedef yaml_document_t YAMLDocument;
typedef yaml_emitter_t YAMLEmitter;
typedef yaml_event_t YAMLEvent;
typedef yaml_node_t YAMLNode;
typedef yaml_node_pair_t YAMLNodePair;
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
                 Usize document_id,
                 const char *key,
                 const char *value);

/**
 *
 * @brief Add YAML sequence to YAMLDocument.
 */
void
add_sequence__YAML(YAMLLoadRes *self,
                   Usize document_id,
                   const char *key,
                   Usize n,
                   ...);

/**
 *
 * @brief Get YAML document from YAMLLoadRes type.
 */
inline YAMLDocument *
get_document__YAML(const YAMLLoadRes *self, Usize document_id)
{
    return &self->documents[document_id];
}

/**
 *
 * @brief Add YAML document.
 */
void
add_new_document__YAML(YAMLLoadRes *self);

/**
 *
 * @brief Get value from key.
 * @param mapping YAMLNode*? (&) - If `mapping` is not equal to `-1`, the
 * function will try to match the key to the `mapping` node, otherwise it will
 * try to match the key to the root node.
 * @return If the key is not found return -1, otherwise return the index of the
 * node value.
 */
Int32
get_key__YAML(YAMLLoadRes *self,
              Usize document_id,
              Int32 mapping_id,
              const char *key);

/**
 *
 * @brief Get node from id.
 * @return YAMLNode*? (&)
 */
inline YAMLNode *
get_node_from_id__YAML(YAMLLoadRes *self, Usize document_id, Int32 node_id)
{
    ASSERT(document_id < self->len);

    return yaml_document_get_node(&self->documents[document_id], node_id);
}

/**
 *
 * @brief Get root node from document id.
 * @rerturn YAMLNode*? (&)
 */
inline YAMLNode *
get_root_node__YAML(YAMLLoadRes *self, Usize document_id)
{
    ASSERT(document_id < self->len);

    return yaml_document_get_root_node(&self->documents[document_id]);
}

#endif // LILY_BASE_YAML_H
