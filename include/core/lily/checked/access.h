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

#ifndef LILY_CORE_LILY_CHECKED_ACCESS_H
#define LILY_CORE_LILY_CHECKED_ACCESS_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/types.h>

#include <stdbool.h>

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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessConstant;

/**
 *
 * @brief Construct LilyCheckedAccessConstant type.
 */
inline CONSTRUCTOR(LilyCheckedAccessConstant,
                   LilyCheckedAccessConstant,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessConstant){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessEnum;

/**
 *
 * @brief Construct LilyCheckedAccessEnum type.
 */
inline CONSTRUCTOR(LilyCheckedAccessEnum,
                   LilyCheckedAccessEnum,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessEnum){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessRecord;

/**
 *
 * @brief Construct LilyCheckedAccessRecord type.
 */
inline CONSTRUCTOR(LilyCheckedAccessRecord,
                   LilyCheckedAccessRecord,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessRecord){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessAlias;

/**
 *
 * @brief Construct LilyCheckedAccessAlias type.
 */
inline CONSTRUCTOR(LilyCheckedAccessAlias,
                   LilyCheckedAccessAlias,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessAlias){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessError;

/**
 *
 * @brief Construct LilyCheckedAccessError type.
 */
inline CONSTRUCTOR(LilyCheckedAccessError,
                   LilyCheckedAccessError,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessError){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessEnumObject;

/**
 *
 * @brief Construct LilyCheckedAccessEnumObject type.
 */
inline CONSTRUCTOR(LilyCheckedAccessEnumObject,
                   LilyCheckedAccessEnumObject,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessEnumObject){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessRecordObject;

/**
 *
 * @brief Construct LilyCheckedAccessRecordObject type.
 */
inline CONSTRUCTOR(LilyCheckedAccessRecordObject,
                   LilyCheckedAccessRecordObject,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessRecordObject){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessClass;

/**
 *
 * @brief Construct LilyCheckedAccessClass type.
 */
inline CONSTRUCTOR(LilyCheckedAccessClass,
                   LilyCheckedAccessClass,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessClass){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessTrait;

/**
 *
 * @brief Construct LilyCheckedAccessTrait type.
 */
inline CONSTRUCTOR(LilyCheckedAccessTrait,
                   LilyCheckedAccessTrait,
                   LilyCheckedAccessModule module,
                   Usize id)
{
    return (LilyCheckedAccessTrait){ .module = module, .id = id };
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessFun;

/**
 *
 * @brief Construct LilyCheckedAccessFun type.
 */
CONSTRUCTOR(LilyCheckedAccessFun *,
            LilyCheckedAccessFun,
            LilyCheckedAccessModule module,
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
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessLabel;

/**
 *
 * @brief Construct LilyCheckedAccessLabel type.
 */
CONSTRUCTOR(LilyCheckedAccessLabel *,
            LilyCheckedAccessLabel,
            LilyCheckedAccessModule module,
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

typedef struct LilyCheckedAccessScope
{
    LilyCheckedAccessModule module;
    Usize id;
} LilyCheckedAccessScope;

/**
 *
 * @brief Construct LilyCheckedAccessScope type.
 */
CONSTRUCTOR(LilyCheckedAccessScope *,
            LilyCheckedAccessScope,
            LilyCheckedAccessModule module,
            Usize id);

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

#endif // LILY_CORE_LILY_CHECKED_ACCESS_H
