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

#ifndef LILY_EX_LIB_LILY_BASE_C
#define LILY_EX_LIB_LILY_BASE_C

#include <base/allocator.h>
#include <base/cli/default_action.h>
#include <base/cli/diagnostic.h>
#include <base/cli/option.h>
#include <base/cli/result/command.h>
#include <base/cli/value.h>
#include <base/env.h>
#include <base/hash_map.h>
#include <base/linked_list.h>
#include <base/memory/api.h>
#include <base/memory/arena.h>
#include <base/memory/page.h>
#include <base/object/schema.h>
#include <base/object/value/list.h>
#include <base/object/value/object.h>
#include <base/optional.h>
#include <base/ordered_hash_map.h>
#include <base/path.h>
#include <base/sized_array.h>
#include <base/string.h>
#include <base/test.h>
#include <base/vec.h>
#include <base/yaml.h>

// <base/allocator.h>
extern inline VARIANT_CONSTRUCTOR(Allocator, Allocator, arena, Usize capacity);

extern inline VARIANT_CONSTRUCTOR(Allocator, Allocator, global);

extern inline VARIANT_CONSTRUCTOR(Allocator, Allocator, page);

// <base/env.h>
extern inline char *
get__Env(const char *name);

// <base/hash_map.h>
extern inline CONSTRUCTOR(HashMapPair, HashMapPair, char *key, void *value);

extern inline Usize
hash__HashMap(HashMap *self, char *key);

extern inline Usize
index__HashMap(HashMap *self, char *key);

extern inline CONSTRUCTOR(HashMapIter, HashMapIter, HashMap *hash_map);

// <base/linked_list.h>
extern inline DESTRUCTOR(LinkedListNode, LinkedListNode *self);

// <base/object/schema.h>
extern inline ObjectSchema *
make_null__ObjectSchema(ObjectSchema *self);

extern inline ObjectSchema *
make_required__ObjectSchema(ObjectSchema *self);

// <base/object/value/list.h>
extern inline CONSTRUCTOR(ObjectValueList, ObjectValueList);

extern inline void
push__ObjectValueList(ObjectValueList *self, struct ObjectValue *object_value);

// <base/object/value/object.h>
extern inline CONSTRUCTOR(ObjectValueObject, ObjectValueObject);

extern inline void
add__ObjectValueObject(ObjectValueObject *self,
                       char *object_name,
                       struct ObjectValue *object_value);

// <base/optional.h>
extern inline bool
is_some__Optional(const Optional *self);

extern inline bool
is_none__Optional(const Optional *self);

// <base/order_hash_map.h>
extern inline CONSTRUCTOR(OrderedHashMapPair,
                          OrderedHashMapPair,
                          char *key,
                          void *value,
                          Usize id);

extern inline Usize
hash__OrderedHashMap(OrderedHashMap *self, char *key);

extern inline Usize
index__OrderedHashMap(OrderedHashMap *self, char *key);

extern inline CONSTRUCTOR(OrderedHashMapIter,
                          OrderedHashMapIter,
                          OrderedHashMap *ordered_hash_map);

// <base/path.h>
extern inline bool
is_relative__Path(const char *path);

// <base/sized_array.h>
extern inline SizedArray *
from__SizedArray(void **buffer, Usize len);

extern inline SizedArray
from_vec__SizedArray(Vec *vec);

extern inline void *
safe_get__SizedArray(const SizedArray *self, Usize index);

extern inline DESTRUCTOR(SizedArray, SizedArray *self);

extern inline CONSTRUCTOR(SizedArrayIter,
                          SizedArrayIter,
                          const SizedArray *sized_array);

extern inline void *
next__SizedArrayIter(SizedArrayIter *self);

// <base/string.h>
extern inline bool
is_empty__String(const String *self);

extern inline char
safe_get__String(const String *self, Usize index);

extern inline CONSTRUCTOR(StringIter, StringIter, String *string);

extern inline char
next__StringIter(StringIter *self);

// <base/test.h>
extern inline DESTRUCTOR(TestCase, TestCase *self);

extern inline CONSTRUCTOR(TestSimple, TestSimple, char *name, void (*f)(void));

extern inline CONSTRUCTOR(TestSuite, TestSuite, char *name, Vec *cases);

extern inline CONSTRUCTOR(Test, Test, char *name);

// <base/vec.h>
extern inline void *
safe_last__Vec(const Vec *self);

extern inline void *
safe_get__Vec(const Vec *self, Usize index);

extern inline CONSTRUCTOR(VecIter, VecIter, const Vec *vec);

// <base/yaml.h>
extern inline CONSTRUCTOR(YAMLLoadRes,
                          YAMLLoadRes,
                          YAMLDocument *documents,
                          Usize len);

extern inline YAMLDocument *
get_document__YAML(const YAMLLoadRes *self, Usize document_id);

extern inline YAMLNode *
get_node_from_id__YAML(YAMLLoadRes *self, Usize document_id, Int32 node_id);

// <base/cli/default_action.h>
extern inline DESTRUCTOR(CliDefaultAction, CliDefaultAction *self);

// <base/cli/diagnostics.h>
extern inline CONSTRUCTOR(CliDiagnostic,
                          CliDiagnostic,
                          enum CliDiagnosticKind kind,
                          const char *msg,
                          Usize arg_count,
                          String *full_command);

// <base/cli/option.h>
extern inline CliOption *
ref__CliOption(CliOption *self);

// <base/cli/value.h>
extern inline DESTRUCTOR(CliValue, CliValue *self);

// <base/cli/result/command.h>
extern inline CONSTRUCTOR(CliResultCommand,
                          CliResultCommand,
                          Usize id,
                          CliResultValue *value);

extern inline DESTRUCTOR(CliResultCommand, const CliResultCommand *self);

// <base/memory/api.h>
extern inline CONSTRUCTOR(MemoryApi, MemoryApi);

// <base/memory/arena.h>
extern inline CONSTRUCTOR(MemoryArena, MemoryArena, Usize capacity);

// <base/memory/page.h>
extern inline CONSTRUCTOR(MemoryPage, MemoryPage);

#endif // LILY_EX_LIB_LILY_BASE_C
