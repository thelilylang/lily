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

#ifndef LILY_CORE_LILY_CHECKED_SCOPE_CONTAINER_H
#define LILY_CORE_LILY_CHECKED_SCOPE_CONTAINER_H

#include <base/alloc.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/checked/access.h>

typedef struct LilyCheckedScopeContainerModule
{
    String *name; // String* (&)
    LilyCheckedAccessModule access;
} LilyCheckedScopeContainerModule;

/**
 *
 * @brief Construct LilyCheckedScopeContainerModule type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerModule *,
            LilyCheckedScopeContainerModule,
            String *name,
            LilyCheckedAccessModule access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerModule in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerModule,
               const LilyCheckedScopeContainerModule *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerModule type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerModule,
                  LilyCheckedScopeContainerModule *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerConstant
{
    String *name; // String* (&)
    LilyCheckedAccessConstant access;
} LilyCheckedScopeContainerConstant;

/**
 *
 * @brief Construct LilyCheckedScopeContainerConstant type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerConstant *,
            LilyCheckedScopeContainerConstant,
            String *name,
            LilyCheckedAccessConstant access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerConstant in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerConstant,
               const LilyCheckedScopeContainerConstant *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerConstant type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerConstant,
                  LilyCheckedScopeContainerConstant *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerEnum
{
    String *name; // String* (&)
    LilyCheckedAccessEnum access;
} LilyCheckedScopeContainerEnum;

/**
 *
 * @brief Construct LilyCheckedScopeContainerEnum type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerEnum *,
            LilyCheckedScopeContainerEnum,
            String *name,
            LilyCheckedAccessEnum access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerEnum in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerEnum,
               const LilyCheckedScopeContainerEnum *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerEnum type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerEnum,
                  LilyCheckedScopeContainerEnum *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerRecord
{
    String *name; // String* (&)
    LilyCheckedAccessRecord access;
} LilyCheckedScopeContainerRecord;

/**
 *
 * @brief Construct LilyCheckedScopeContainerRecord type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerRecord *,
            LilyCheckedScopeContainerRecord,
            String *name,
            LilyCheckedAccessRecord access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerRecord in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerRecord,
               const LilyCheckedScopeContainerRecord *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerRecord type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerRecord,
                  LilyCheckedScopeContainerRecord *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerAlias
{
    String *name; // String* (&)
    LilyCheckedAccessAlias access;
} LilyCheckedScopeContainerAlias;

/**
 *
 * @brief Construct LilyCheckedScopeContainerAlias type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerAlias *,
            LilyCheckedScopeContainerAlias,
            String *name,
            LilyCheckedAccessAlias access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerAlias in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerAlias,
               const LilyCheckedScopeContainerAlias *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerAlias type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerAlias,
                  LilyCheckedScopeContainerAlias *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerEnumObject
{
    String *name; // String* (&)
    LilyCheckedAccessEnumObject access;
} LilyCheckedScopeContainerEnumObject;

/**
 *
 * @brief Construct LilyCheckedScopeContainerEnumObject type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerEnumObject *,
            LilyCheckedScopeContainerEnumObject,
            String *name,
            LilyCheckedAccessEnumObject access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerEnumObject in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerEnumObject,
               const LilyCheckedScopeContainerEnumObject *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerEnumObject type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerEnumObject,
                  LilyCheckedScopeContainerEnumObject *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerRecordObject
{
    String *name; // String* (&)
    LilyCheckedAccessRecordObject access;
} LilyCheckedScopeContainerRecordObject;

/**
 *
 * @brief Construct LilyCheckedScopeContainerRecordObject type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerRecordObject *,
            LilyCheckedScopeContainerRecordObject,
            String *name,
            LilyCheckedAccessRecordObject access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerRecordObject in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerRecordObject,
               const LilyCheckedScopeContainerRecordObject *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerRecordObject type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerRecordObject,
                  LilyCheckedScopeContainerRecordObject *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerClass
{
    String *name; // String* (&)
    LilyCheckedAccessClass access;
} LilyCheckedScopeContainerClass;

/**
 *
 * @brief Construct LilyCheckedScopeContainerClass type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerClass *,
            LilyCheckedScopeContainerClass,
            String *name,
            LilyCheckedAccessClass access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerClass in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerClass,
               const LilyCheckedScopeContainerClass *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerClass type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerClass,
                  LilyCheckedScopeContainerClass *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerTrait
{
    String *name; // String* (&)
    LilyCheckedAccessTrait access;
} LilyCheckedScopeContainerTrait;

/**
 *
 * @brief Construct LilyCheckedScopeContainerTrait type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerTrait *,
            LilyCheckedScopeContainerTrait,
            String *name,
            LilyCheckedAccessTrait access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerTrait in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerTrait,
               const LilyCheckedScopeContainerTrait *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerTrait type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerTrait,
                  LilyCheckedScopeContainerTrait *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerFun
{
    String *name; // String* (&)
    // overload of function
    Vec *accesses; // Vec<LilyCheckedAccessFun*>*
} LilyCheckedScopeContainerFun;

/**
 *
 * @brief Construct LilyCheckedScopeContainerFun type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerFun *,
            LilyCheckedScopeContainerFun,
            String *name,
            Vec *accesses);

/**
 *
 * @brief Convert LilyCheckedScopeContainerFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerFun,
               const LilyCheckedScopeContainerFun *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerFun type.
 */
DESTRUCTOR(LilyCheckedScopeContainerFun, LilyCheckedScopeContainerFun *self);

typedef struct LilyCheckedScopeContainerVariable
{
    String *name; // String* (&)
    LilyCheckedAccessScope access;
} LilyCheckedScopeContainerVariable;

/**
 *
 * @brief Construct LilyCheckedScopeContainerVariable type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerVariable *,
            LilyCheckedScopeContainerVariable,
            String *name,
            LilyCheckedAccessScope access);

/**
 *
 * @brief Convert LilyCheckedScopeContainerVariable in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerVariable,
               const LilyCheckedScopeContainerVariable *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerVariable type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerVariable,
                  LilyCheckedScopeContainerVariable *self)
{
    lily_free(self);
}

#endif // LILY_CORE_LILY_CHECKED_SCOPE_CONTAINER_H
