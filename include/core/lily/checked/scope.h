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

enum LilyCheckedGlobalScopeKind
{
    LILY_CHECKED_GLOBAL_SCOPE_KIND_ATTRIBUTE,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_CLASS,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_CONSTANT,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_ENUM,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_ENUM_OBJECT,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_FIELD,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_FIELD_OBJECT,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_FUN,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_METHOD,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_MODULE,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_PROTOTYPE,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_RECORD,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_RECORD_OBJECT,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_TRAIT,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_VARIANT,
    LILY_CHECKED_GLOBAL_SCOPE_KIND_VARIANT_OBJECT,
};

/**
 *
 * @brief Convert LilyCheckedGlobalScopeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGlobalScopeKind,
               enum LilyCheckedGlobalScopeKind self);
#endif

typedef Usize LilyCheckedGlobalScopeId;

typedef struct LilyCheckedGlobalScope
{
    enum LilyCheckedGlobalScopeKind kind;
    LilyCheckedGlobalScopeId id;
    Vec *access;   // Vec<String*>* (&)
    Vec *children; // Vec<LilyCheckedGlobalScope*>*?
    enum LilyVisibility visibility;
    Location location;
} LilyCheckedGlobalScope;

/**
 *
 * @brief Construct LilyCheckedGlobalScope type.
 */
CONSTRUCTOR(LilyCheckedGlobalScope *,
            LilyCheckedGlobalScope,
            enum LilyCheckedGlobalScopeKind kind,
            LilyCheckedGlobalScopeId id,
            Vec *access,
            Vec *children,
            enum LilyVisibility visibility,
            Location location);

/**
 *
 * @brief Convert LilyCheckedGlobalScope in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedGlobalScope,
               const LilyCheckedGlobalScope *self);
#endif

/**
 *
 * @brief Free LilyCheckedGlobalScope type.
 */
DESTRUCTOR(LilyCheckedGlobalScope, LilyCheckedGlobalScope *self);

enum LilyCheckedExternScopeKind
{
    LILY_CHECKED_EXTERN_SCOPE_KIND_FILE,
    LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY,
    LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE,
};

/**
 *
 * @brief Convert LilyCheckedExternScopeKind in string.
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
    LilyCheckedGlobalScope *global; // LilyCheckedGlobalScope* (&)
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
                    LilyCheckedGlobalScope *global,
                    LilyFile *file);

/**
 *
 * @brief Construct LilyCheckedExternScope type
 * (LILY_CHECKED_EXTERN_SCOPE_KIND_LIBRARY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    library,
                    LilyCheckedGlobalScope *global,
                    LilyLibrary *library);

/**
 *
 * @brief Construct LilyCheckedExternScope type
 * (LILY_CHECKED_EXTERN_SCOPE_KIND_PACKAGE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedExternScope *,
                    LilyCheckedExternScope,
                    package,
                    LilyCheckedGlobalScope *global,
                    LilyPackage *package);

/**
 *
 * @brief Convert LilyCheckedExternScope in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExternScope,
               const LilyCheckedExternScope *self);
#endif

/**
 *
 * @brief Free LilyCheckedExternScope type.
 */
inline DESTRUCTOR(LilyCheckedExternScope, LilyCheckedExternScope *self)
{
    lily_free(self);
}

enum LilyCheckedLocalScopeKind
{
    LILY_CHECKED_LOCAL_SCOPE_KIND_CONSTANT,
    LILY_CHECKED_LOCAL_SCOPE_KIND_GENERIC_DATA_TYPE,
    LILY_CHECKED_LOCAL_SCOPE_KIND_VARIABLE,
};

/**
 *
 * @brief Convert LilyCheckedLocalScopeKind in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedLocalScopeKind,
               enum LilyCheckedLocalScopeKind self);
#endif

typedef struct LilyCheckedLocalScope
{
    enum LilyCheckedLocalScopeKind kind;
    Vec *access; // Vec<Usize*>*
    // 0 -> a, b
    // 1 -> c, d
    // 2 -> e, f
    // Access: [0, 1]
} LilyCheckedLocalScope;

/**
 *
 * @brief Construct LilyCheckedLocalScope type.
 */
CONSTRUCTOR(LilyCheckedLocalScope *,
            LilyCheckedLocalScope,
            enum LilyCheckedLocalScopeKind kind,
            Vec *access);

/**
 *
 * @brief Convert LilyCheckedLocalScope in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedLocalScope,
               const LilyCheckedLocalScope *self);
#endif

/**
 *
 * @brief Free LilyCheckedLocalScope type.
 */
DESTRUCTOR(LilyCheckedLocalScope, LilyCheckedLocalScope *self);

enum LilyCheckedScopeKind
{
    LILY_CHECKED_SCOPE_KIND_EXTERN,
    LILY_CHECKED_SCOPE_KIND_GLOBAL,
    LILY_CHECKED_SCOPE_KIND_LOCAL
};

/**
 *
 * @brief Convert LilyCheckedScopeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedScopeKind, enum LilyCheckedScopeKind self);
#endif

typedef struct LilyCheckedScope
{
    enum LilyCheckedScopeKind kind;
    union
    {
        const LilyCheckedExternScope
          *extern_; // const LilyCheckedExternScope* (&)
        const LilyCheckedGlobalScope
          *global;                          // const LilyCheckedGlobalScope* (&)
        const LilyCheckedLocalScope *local; // const LilyCheckedLocalScope* (&)
    };
} LilyCheckedScope;

/**
 *
 * @brief Construct LilyCheckedScope type
 * (LILY_CHECKED_SCOPE_KIND_EXTERN).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScope,
                           LilyCheckedScope,
                           extern,
                           const LilyCheckedExternScope *extern_)
{
    return (LilyCheckedScope){ .kind = LILY_CHECKED_SCOPE_KIND_EXTERN,
                               .extern_ = extern_ };
}

/**
 *
 * @brief Construct LilyCheckedScope type
 * (LILY_CHECKED_SCOPE_KIND_GLOBAL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScope,
                           LilyCheckedScope,
                           global,
                           const LilyCheckedGlobalScope *global)
{
    return (LilyCheckedScope){ .kind = LILY_CHECKED_SCOPE_KIND_GLOBAL,
                               .global = global };
}

/**
 *
 * @brief Construct LilyCheckedScope type
 * (LILY_CHECKED_SCOPE_KIND_LOCAL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScope,
                           LilyCheckedScope,
                           local,
                           const LilyCheckedLocalScope *local)
{
    return (LilyCheckedScope){ .kind = LILY_CHECKED_SCOPE_KIND_LOCAL,
                               .local = local };
}

/**
 *
 * @brief Convert LilyCheckedScope in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyCheckedScope, const LilyCheckedScope *self);
#endif

#endif // LILY_CORE_LILY_CHECKED_SCOPE_H
