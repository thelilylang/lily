/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#ifndef LILY_CORE_LILY_CHECKED_DECL_RECORD_H
#define LILY_CORE_LILY_CHECKED_DECL_RECORD_H

#include <base/string.h>
#include <base/vec.h>

#include <core/lily/checked/field.h>
#include <core/lily/checked/scope.h>
#include <core/lily/checked/signature.h>
#include <core/lily/visibility.h>

typedef struct LilyCheckedDeclRecord
{
    String *name; // String* (&)
    String *global_name;
    Vec *generic_params; // Vec<LilyCheckedGenericParam*>*?
    Vec *fields;         // Vec<LilyCheckedField*>*
    Vec *signatures;     // Vec<LilyCheckedSignatureType*>*
    LilyCheckedScope *scope;
    enum LilyVisibility visibility;
    bool is_checked;
    bool is_recursive;
} LilyCheckedDeclRecord;

inline CONSTRUCTOR(LilyCheckedDeclRecord,
                   LilyCheckedDeclRecord,
                   String *name,
                   String *global_name,
                   Vec *generic_params,
                   Vec *fields,
                   LilyCheckedScope *scope,
                   enum LilyVisibility visibility)
{
    return (LilyCheckedDeclRecord){ .name = name,
                                    .global_name = global_name,
                                    .generic_params = generic_params,
                                    .fields = fields,
                                    .signatures = NEW(Vec),
                                    .scope = scope,
                                    .visibility = visibility,
                                    .is_checked = false,
                                    .is_recursive = false };
}

/**
 *
 * @brief Add signature to signatures field.
 */
inline int
add_signature__LilyCheckedDeclRecord(LilyCheckedDeclRecord *self,
                                     OrderedHashMap *generic_params)
{
    return add_signature__LilyCheckedSignatureType(
      self->global_name, generic_params, self->signatures);
}

/**
 *
 * @brief Convert LilyCheckedDeclRecord in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclRecord,
               const LilyCheckedDeclRecord *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclRecord type.
 */
DESTRUCTOR(LilyCheckedDeclRecord, const LilyCheckedDeclRecord *self);

#endif // LILY_CORE_LILY_CHECKED_DECL_RECORD_H
