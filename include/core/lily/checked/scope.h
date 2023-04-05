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

#ifndef LILY_CORE_LILY_CHECKED_SCOPE_H
#define LILY_CORE_LILY_CHECKED_SCOPE_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/file.h>
#include <core/lily/package.h>
#include <core/lily/visibility.h>

enum LilyCheckedScopeKind
{
    LILY_CHECKED_SCOPE_KIND_ATTRIBUTE,
    LILY_CHECKED_SCOPE_KIND_CLASS,
    LILY_CHECKED_SCOPE_KIND_CONSTANT,
    LILY_CHECKED_SCOPE_KIND_ENUM,
    LILY_CHECKED_SCOPE_KIND_ENUM_OBJECT,
    LILY_CHECKED_SCOPE_KIND_FIELD,
    LILY_CHECKED_SCOPE_KIND_FIELD_OBJECT,
    LILY_CHECKED_SCOPE_KIND_FUN,
    LILY_CHECKED_SCOPE_KIND_METHOD,
    LILY_CHECKED_SCOPE_KIND_MODULE,
    LILY_CHECKED_SCOPE_KIND_PROTOTYPE,
    LILY_CHECKED_SCOPE_KIND_RECORD,
    LILY_CHECKED_SCOPE_KIND_RECORD_OBJECT,
    LILY_CHECKED_SCOPE_KIND_TRAIT,
    LILY_CHECKED_SCOPE_KIND_VARIANT,
    LILY_CHECKED_SCOPE_KIND_VARIANT_OBJECT,
} LilyCheckedScopeKind;

/**
 *
 * @brief Convert LilyCheckedScopeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedScopeKind, enum LilyCheckedScopeKind self);
#endif

typedef Usize LilyCheckedScopeId;

typedef struct LilyCheckedScope
{
    enum LilyCheckedScopeKind kind;
    LilyCheckedScopeId id;
    Vec *access;   // Vec<String*>*
    Vec *children; // Vec<LilyCheckedScope*>*?
    enum LilyVisibility visibility;
    Location location;
} LilyCheckedScope;

/**
 *
 * @brief Construct LilyCheckedScope type.
 */
CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            enum LilyCheckedScopeKind kind,
            LilyCheckedScopeId id,
            Vec *access,
            Vec *children,
            enum LilyVisibility visibility,
            Location location);

/**
 *
 * @brief Convert LilyCheckedScope in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedScope, const LilyCheckedScope *self);
#endif

/**
 *
 * @brief Free LilyCheckedScope type.
 */
DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self);

enum LilyCheckedExternScopeKind
{
    LILY_CHECKED_EXTERN_SCOPE_KIND_FILE,
    LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY,
    LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE,
};

/**
 *
 * @brief Convert LilyCheckedDataTypeArray in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExternScopeKind,
               enum LilyCheckedExternScopeKind self);
#endif

typedef struct LilyCheckedExternScope
{
    enum LilyCheckedExternScopeKind kind;
    union
    {
        LilyFile *file;       // LilyFile* (&)
        LilyLibrary *library; // LilyLibrary* (&)
        LilyPackage *package; // LilyPackage* (&)
    };
} LilyCheckedExternScope;

/**
 *
 * @brief Construct LilyCheckedExternScope type
 * (LILY_CHECKED_EXTERN_SCOPE_KIND_FILE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    file,
                    LilyFile *file);

/**
 *
 * @brief Construct LilyCheckedExternScope type
 * (LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    library,
                    LilyLibrary *library);

/**
 *
 * @brief Construct LilyCheckedExternScope type
 * (LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    package,
                    LilyPackage *package);

/**
 *
 * @brief Free LilyCheckedExternScope type.
 */
inline DESTRUCTOR(LilyCheckedExternScope, LilyCheckedExternScope *self)
{
    lily_free(self);
}

#endif // LILY_CORE_LILY_CHECKED_SCOPE_H
