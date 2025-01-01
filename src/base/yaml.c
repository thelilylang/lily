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
#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>
#include <base/yaml.h>

#include <stdarg.h>

static bool
has_mapping__YAML(const YAMLDocument *document);

static Int32
get_mapping__YAML(YAMLDocument *document);

bool
has_mapping__YAML(const YAMLDocument *document)
{
    for (YAMLNode *node = document->nodes.start; node < document->nodes.top;
         ++node) {
        if (GET_NODE_TYPE__YAML(node) == YAML_MAPPING_NODE) {
            return true;
        }
    }

    return false;
}

Int32
get_mapping__YAML(YAMLDocument *document)
{
    if (!has_mapping__YAML(document)) {
        // Delete old document, with no mapping.
        yaml_document_delete(document);

        yaml_document_initialize(document, NULL, NULL, NULL, 0, 1);
        yaml_document_add_mapping(document, NULL, YAML_ANY_MAPPING_STYLE);
    }

    return 1;
}

YAMLLoadRes
init__YAMLLoadRes()
{
    YAMLDocument *documents = malloc(sizeof(YAMLDocument));

    yaml_document_initialize(&*documents, NULL, NULL, NULL, 0, 1);
    yaml_document_add_mapping(&*documents, NULL, YAML_ANY_MAPPING_STYLE);

    return NEW(YAMLLoadRes, documents, 1);
}

DESTRUCTOR(YAMLLoadRes, const YAMLLoadRes *self)
{
    if (self->documents) {
        for (Usize i = 0; i < self->len; ++i) {
            yaml_document_delete(&self->documents[i]);
        }

        lily_free(self->documents);
    }
}

YAMLLoadRes
load__YAML(const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (!file) {
        FAILED("YAML: cannot open file");
    }

    YAMLParser parser;
    YAMLDocument document;
    bool done = false;
    Usize documents_len = 0;

    if (!yaml_parser_initialize(&parser)) {
        FAILED("YAML: failed to initialize parser");
    }

    YAMLDocument *documents = NULL;

    yaml_parser_set_input_file(&parser, file);

    while (!done) {
        if (!yaml_parser_load(&parser, &document)) {
            FAILED("error occured in YAML file");
        }

        done = (!yaml_document_get_root_node(&document));

        if (!documents) {
            documents = malloc(sizeof(YAMLDocument));
        } else {
            documents = lily_realloc(
              documents, sizeof(YAMLDocument) * (documents_len + 1));
        }

        documents[documents_len++] = document;
    }

    yaml_parser_delete(&parser);

    if (fclose(file) != 0) {
        FAILED("YAML: failed to close file");
    }

    return NEW(YAMLLoadRes, documents, documents_len);
}

void
write__YAML(const YAMLLoadRes *self, const char *filename)
{
    FILE *file = fopen(filename, "wb");

    if (!file) {
        FAILED("YAML: cannot open file");
    }

    YAMLEmitter emitter;

    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_file(&emitter, file);

    for (Usize i = 0; i < self->len; ++i) {
        yaml_emitter_dump(&emitter, &self->documents[i]);
    }

    yaml_emitter_delete(&emitter);

    if (fclose(file) != 0) {
        FAILED("YAML: failed to close file");
    }
}

void
dump__YAML(const YAMLLoadRes *self)
{
    YAMLEmitter emitter;

    yaml_emitter_initialize(&emitter);
    yaml_emitter_set_output_file(&emitter, stdout);

    for (Usize i = 0; i < self->len; ++i) {
        yaml_emitter_dump(&emitter, &self->documents[i]);
    }

    yaml_emitter_delete(&emitter);
}

#define GET_DOCUMENT(self, id) self->documents[id]

// e.g.
// year: 2024
// Add `day` key, with `Monday` value
//
// Result:
// year: 2024
// value: Monday
void
add_scalar__YAML(YAMLLoadRes *self,
                 Usize document_id,
                 const char *key,
                 const char *value)
{
    YAMLDocument *document = &GET_DOCUMENT(self, document_id);

    ASSERT(document);

    YAMLNode *root_node = yaml_document_get_root_node(document);

    if (!root_node) {
        FAILED("YAML: the document is empty");
    }

    int mapping = get_mapping__YAML(document);
    int key_node = yaml_document_add_scalar(
      document, NULL, (YAMLChar *)key, strlen(key), YAML_ANY_SCALAR_STYLE);

    if (!key_node) {
        FAILED("YAML: failed to add scalar (key)");
    }

    int value_node = yaml_document_add_scalar(
      document, NULL, (YAMLChar *)value, strlen(value), YAML_ANY_SCALAR_STYLE);

    if (!value_node) {
        FAILED("YAML: failed to add scalar (value)");
    }

    yaml_document_append_mapping_pair(document, mapping, key_node, value_node);
}

void
add_sequence__YAML(YAMLLoadRes *self,
                   Usize document_id,
                   const char *key,
                   Usize n,
                   ...)
{
    YAMLDocument *document = &GET_DOCUMENT(self, document_id);
    va_list vl;

    va_start(vl, n);

    int sequence_id =
      yaml_document_add_sequence(document, NULL, YAML_BLOCK_SEQUENCE_STYLE);

    if (!sequence_id) {
        FAILED("YAML: failed to add sequence");
    }

    for (Usize i = 0; i < n; ++i) {
        const char *value = va_arg(vl, const char *);

        int value_id = yaml_document_add_scalar(document,
                                                NULL,
                                                (YAMLChar *)value,
                                                strlen(value),
                                                YAML_ANY_SCALAR_STYLE);

        if (!value_id) {
            FAILED("YAML: failed to add scalar value");
        }

        if (!yaml_document_append_sequence_item(
              document, sequence_id, value_id)) {
            FAILED("YAML: failed to append sequence item");
        }
    }

    Int32 sequence_key = yaml_document_add_scalar(
      document, NULL, (YAMLChar *)key, strlen(key), YAML_ANY_SCALAR_STYLE);

    if (!sequence_key) {
        FAILED("YAML: failed to add sequence key");
    }

    yaml_document_append_mapping_pair(
      document, get_mapping__YAML(document), sequence_key, sequence_id);

    va_end(vl);
}

void
add_new_document__YAML(YAMLLoadRes *self)
{
    YAMLDocument document;

    yaml_document_initialize(&document, NULL, NULL, NULL, 0, 1);
    yaml_document_add_mapping(&document, NULL, YAML_ANY_MAPPING_STYLE);

    self->documents = lily_realloc(self->documents, self->len + 1);
    self->documents[self->len++] = document;
}

Int32
get_key__YAML(YAMLLoadRes *self,
              Usize document_id,
              Int32 mapping_id,
              const char *key)
{
    Int32 id = 0;
    YAMLDocument *document = &GET_DOCUMENT(self, document_id);

    for (YAMLNode *node = document->nodes.start; node < document->nodes.top;
         ++node, ++id) {
        switch (GET_NODE_TYPE__YAML(node)) {
            case YAML_SCALAR_NODE:
                if (!strcmp(GET_NODE_SCALAR_VALUE__YAML(node), key)) {
                    return id;
                }

                break;
            case YAML_MAPPING_NODE:
                for (YAMLNodePair *pair = node->data.mapping.pairs.start;
                     pair < node->data.mapping.pairs.top;
                     ++pair) {
                    YAMLNode *node_key =
                      yaml_document_get_node(document, pair->key);

                    ASSERT(node_key &&
                           GET_NODE_TYPE__YAML(node_key) == YAML_SCALAR_NODE);

                    if (!strcmp(GET_NODE_SCALAR_VALUE__YAML(node_key), key)) {
                        return pair->value;
                    }
                }

                break;
            default:
                break;
        }
    }

    return -1;
}
