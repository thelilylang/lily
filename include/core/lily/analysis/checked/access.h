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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_ACCESS_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_ACCESS_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/types.h>

#include <stdbool.h>

typedef struct LilyCheckedAccessScope
{
    Usize id;
} LilyCheckedAccessScope;

/**
 *
 * @brief Construct LilyCheckedAccessScope type.
 */
CONSTRUCTOR(LilyCheckedAccessScope *, LilyCheckedAccessScope, Usize id);

/**
 *
 * @brief Convert LilyCheckedAccessScope in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessScope,
               const LilyCheckedAccessScope *self);
#endif

/**
 *
 * @brief Free LilyCheckedAccessScope type.
 */
inline DESTRUCTOR(LilyCheckedAccessScope, LilyCheckedAccessScope *self)
{
    lily_free(self);
}

typedef struct LilyCheckedAccessModule
{
    Usize id;
} LilyCheckedAccessModule;

/**
 *
 * @brief Construct LilyCheckedAccessModule type.
 */
inline CONSTRUCTOR(LilyCheckedAccessModule, LilyCheckedAccessModule, Usize id)
{
    return (LilyCheckedAccessModule){ .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessModule in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessModule,
               const LilyCheckedAccessModule *self);
#endif

/**
 *
 * @brief Return true if the both are equal otherwise return false.
 */
inline bool
eq__LilyCheckedAccessModule(const LilyCheckedAccessModule *self,
                            const LilyCheckedAccessModule *other)
{
    return self->id == other->id;
}

typedef struct LilyCheckedAccessConstant
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessConstant;

/**
 *
 * @brief Construct LilyCheckedAccessConstant type.
 */
inline CONSTRUCTOR(LilyCheckedAccessConstant,
                   LilyCheckedAccessConstant,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessConstant){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessConstant in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessConstant,
               const LilyCheckedAccessConstant *self);
#endif

typedef struct LilyCheckedAccessEnum
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessEnum;

/**
 *
 * @brief Construct LilyCheckedAccessEnum type.
 */
inline CONSTRUCTOR(LilyCheckedAccessEnum,
                   LilyCheckedAccessEnum,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessEnum){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessEnum in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessEnum,
               const LilyCheckedAccessEnum *self);
#endif

typedef struct LilyCheckedAccessRecord
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessRecord;

/**
 *
 * @brief Construct LilyCheckedAccessRecord type.
 */
inline CONSTRUCTOR(LilyCheckedAccessRecord,
                   LilyCheckedAccessRecord,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessRecord){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessRecord in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessRecord,
               const LilyCheckedAccessRecord *self);
#endif

typedef struct LilyCheckedAccessAlias
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessAlias;

/**
 *
 * @brief Construct LilyCheckedAccessAlias type.
 */
inline CONSTRUCTOR(LilyCheckedAccessAlias,
                   LilyCheckedAccessAlias,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessAlias){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessAlias in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessAlias,
               const LilyCheckedAccessAlias *self);
#endif

typedef struct LilyCheckedAccessError
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessError;

/**
 *
 * @brief Construct LilyCheckedAccessError type.
 */
inline CONSTRUCTOR(LilyCheckedAccessError,
                   LilyCheckedAccessError,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessError){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessError in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessError,
               const LilyCheckedAccessError *self);
#endif

typedef struct LilyCheckedAccessEnumObject
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessEnumObject;

/**
 *
 * @brief Construct LilyCheckedAccessEnumObject type.
 */
inline CONSTRUCTOR(LilyCheckedAccessEnumObject,
                   LilyCheckedAccessEnumObject,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessEnumObject){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessEnumObject in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessEnumObject,
               const LilyCheckedAccessEnumObject *self);
#endif

typedef struct LilyCheckedAccessRecordObject
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessRecordObject;

/**
 *
 * @brief Construct LilyCheckedAccessRecordObject type.
 */
inline CONSTRUCTOR(LilyCheckedAccessRecordObject,
                   LilyCheckedAccessRecordObject,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessRecordObject){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessRecordObject in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessRecordObject,
               const LilyCheckedAccessRecordObject *self);
#endif

typedef struct LilyCheckedAccessClass
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessClass;

/**
 *
 * @brief Construct LilyCheckedAccessClass type.
 */
inline CONSTRUCTOR(LilyCheckedAccessClass,
                   LilyCheckedAccessClass,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessClass){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessClass in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessClass,
               const LilyCheckedAccessClass *self);
#endif

typedef struct LilyCheckedAccessTrait
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessTrait;

/**
 *
 * @brief Construct LilyCheckedAccessTrait type.
 */
inline CONSTRUCTOR(LilyCheckedAccessTrait,
                   LilyCheckedAccessTrait,
                   LilyCheckedAccessScope scope,
                   Usize id)
{
    return (LilyCheckedAccessTrait){ .scope = scope, .id = id };
}

/**
 *
 * @brief Convert LilyCheckedAccessTrait in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessTrait,
               const LilyCheckedAccessTrait *self);
#endif

typedef struct LilyCheckedAccessFun
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessFun;

/**
 *
 * @brief Construct LilyCheckedAccessFun type.
 */
CONSTRUCTOR(LilyCheckedAccessFun *,
            LilyCheckedAccessFun,
            LilyCheckedAccessScope scope,
            Usize id);

/**
 *
 * @brief Convert LilyCheckedAccessFun in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessFun,
               const LilyCheckedAccessFun *self);
#endif

inline DESTRUCTOR(LilyCheckedAccessFun, LilyCheckedAccessFun *self)
{
    lily_free(self);
}

typedef struct LilyCheckedAccessLabel
{
    LilyCheckedAccessScope scope;
    Usize id;
} LilyCheckedAccessLabel;

/**
 *
 * @brief Construct LilyCheckedAccessLabel type.
 */
CONSTRUCTOR(LilyCheckedAccessLabel *,
            LilyCheckedAccessLabel,
            LilyCheckedAccessScope scope,
            Usize id);

/**
 *
 * @brief Convert LilyCheckedAccessLabel in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessLabel,
               const LilyCheckedAccessLabel *self);
#endif

inline DESTRUCTOR(LilyCheckedAccessLabel, LilyCheckedAccessLabel *self)
{
    lily_free(self);
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_ACCESS_H
