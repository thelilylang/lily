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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_CONTAINER_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_CONTAINER_H

#include <base/alloc.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct LilyCheckedScopeContainerCapturedVariable
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerCapturedVariable;

/**
 *
 * @brief Construct LilyCheckedScopeContainerCapturedVariable type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerCapturedVariable *,
            LilyCheckedScopeContainerCapturedVariable,
            String *name,
            Usize id);

/**
 *
 * @brief Convert LilyCheckedScopeContainerCapturedVariable in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerCapturedVariable,
               const LilyCheckedScopeContainerCapturedVariable *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerCapturedVariable type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerCapturedVariable,
                  LilyCheckedScopeContainerCapturedVariable *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerModule
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerModule;

/**
 *
 * @brief Construct LilyCheckedScopeContainerModule type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerModule *,
            LilyCheckedScopeContainerModule,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerConstant;

/**
 *
 * @brief Construct LilyCheckedScopeContainerConstant type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerConstant *,
            LilyCheckedScopeContainerConstant,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerEnum;

/**
 *
 * @brief Construct LilyCheckedScopeContainerEnum type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerEnum *,
            LilyCheckedScopeContainerEnum,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerRecord;

/**
 *
 * @brief Construct LilyCheckedScopeContainerRecord type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerRecord *,
            LilyCheckedScopeContainerRecord,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerAlias;

/**
 *
 * @brief Construct LilyCheckedScopeContainerAlias type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerAlias *,
            LilyCheckedScopeContainerAlias,
            String *name,
            Usize id);

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

typedef struct LilyCheckedScopeContainerError
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerError;

/**
 *
 * @brief Construct LilyCheckedScopeContainerError type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerError *,
            LilyCheckedScopeContainerError,
            String *name,
            Usize id);

/**
 *
 * @brief Convert LilyCheckedScopeContainerError in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerError,
               const LilyCheckedScopeContainerError *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerError type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerError,
                  LilyCheckedScopeContainerError *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerEnumObject
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerEnumObject;

/**
 *
 * @brief Construct LilyCheckedScopeContainerEnumObject type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerEnumObject *,
            LilyCheckedScopeContainerEnumObject,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerRecordObject;

/**
 *
 * @brief Construct LilyCheckedScopeContainerRecordObject type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerRecordObject *,
            LilyCheckedScopeContainerRecordObject,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerClass;

/**
 *
 * @brief Construct LilyCheckedScopeContainerClass type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerClass *,
            LilyCheckedScopeContainerClass,
            String *name,
            Usize id);

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
    Usize id;
} LilyCheckedScopeContainerTrait;

/**
 *
 * @brief Construct LilyCheckedScopeContainerTrait type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerTrait *,
            LilyCheckedScopeContainerTrait,
            String *name,
            Usize id);

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
    Vec *ids; // Vec<Usize*>*
} LilyCheckedScopeContainerFun;

/**
 *
 * @brief Construct LilyCheckedScopeContainerFun type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerFun *,
            LilyCheckedScopeContainerFun,
            String *name,
            Vec *ids);

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

typedef struct LilyCheckedScopeContainerLabel
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerLabel;

/**
 *
 * @brief Construct LilyCheckedScopeContainerLabel type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerLabel *,
            LilyCheckedScopeContainerLabel,
            String *name,
            Usize id);

/**
 *
 * @brief Convert LilyCheckedScopeContainerLabel in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerLabel,
               const LilyCheckedScopeContainerLabel *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerLabel type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerLabel,
                  LilyCheckedScopeContainerLabel *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerVariable
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerVariable;

/**
 *
 * @brief Construct LilyCheckedScopeContainerVariable type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerVariable *,
            LilyCheckedScopeContainerVariable,
            String *name,
            Usize id);

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

typedef struct LilyCheckedScopeContainerGeneric
{
    String *name; // String* (&)
    Usize id;
} LilyCheckedScopeContainerGeneric;

/**
 *
 * @brief Construct LilyCheckedScopeContainerVariable type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerGeneric *,
            LilyCheckedScopeContainerGeneric,
            String *name,
            Usize id);

/**
 *
 * @brief Convert LilyCheckedScopeContainerGeneric in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerGeneric,
               const LilyCheckedScopeContainerGeneric *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerGeneric type.
 */
inline DESTRUCTOR(LilyCheckedScopeContainerGeneric,
                  LilyCheckedScopeContainerGeneric *self)
{
    lily_free(self);
}

typedef struct LilyCheckedScopeContainerMethod
{
    String *name; // String* (&)
    // overload of method
    Vec *ids; // Vec<Usize*>*
} LilyCheckedScopeContainerMethod;

/**
 *
 * @brief Construct LilyCheckedScopeContainerMethod type.
 */
CONSTRUCTOR(LilyCheckedScopeContainerMethod *,
            LilyCheckedScopeContainerMethod,
            String *name,
            Vec *ids);

/**
 *
 * @brief Convert LilyCheckedScopeContainerMethod in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerMethod,
               const LilyCheckedScopeContainerMethod *self);
#endif

/**
 *
 * @brief Free LilyCheckedScopeContainerMethod type.
 */
DESTRUCTOR(LilyCheckedScopeContainerMethod,
           LilyCheckedScopeContainerMethod *self);

enum LilyCheckedScopeContainerKind
{
    LILY_CHECKED_SCOPE_CONTAINER_KIND_CAPTURED_VARIABLE,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_MODULE,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_CONSTANT,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_ENUM,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_RECORD,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_ALIAS,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_ERROR,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_ENUM_OBJECT,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_RECORD_OBJECT,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_CLASS,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_TRAIT,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_FUN,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_LABEL,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_VARIABLE,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_GENERIC,
    LILY_CHECKED_SCOPE_CONTAINER_KIND_METHOD,
};

/**
 *
 * @brief Convert LilyCheckedScopeContainerKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerKind,
               enum LilyCheckedScopeContainerKind self);
#endif

typedef struct LilyCheckedScopeContainer
{
    enum LilyCheckedScopeContainerKind kind;
    Usize scope_id;
    union
    {
        const LilyCheckedScopeContainerCapturedVariable
          *captured_variable; // const
                              // LilyCheckedScopeContainerCapturedVariable* (&)
        const LilyCheckedScopeContainerModule
          *module; // const LilyCheckedScopeContainerModule* (&)
        const LilyCheckedScopeContainerConstant
          *constant; // const LilyCheckedScopeContainerConstant* (&)
        const LilyCheckedScopeContainerEnum
          *enum_; // const LilyCheckedScopeContainerEnum* (&)
        const LilyCheckedScopeContainerRecord
          *record; // const LilyCheckedScopeContainerRecord* (&)
        const LilyCheckedScopeContainerAlias
          *alias; // const LilyCheckedScopeContainerAlias* (&)
        const LilyCheckedScopeContainerError
          *error; // const LilyCheckedScopeContainerError* (&)
        const LilyCheckedScopeContainerEnumObject
          *enum_object; // const LilyCheckedScopeContainerEnumObject* (&)
        const LilyCheckedScopeContainerRecordObject
          *record_object; // const LilyCheckedScopeContainerRecordObject* (&)
        const LilyCheckedScopeContainerClass
          *class; // const LilyCheckedScopeContainerClass* (&)
        const LilyCheckedScopeContainerTrait
          *trait; // const LilyCheckedScopeContainerTrait* (&)
        const LilyCheckedScopeContainerFun
          *fun; // const LilyCheckedScopeContainerFun* (&)
        const LilyCheckedScopeContainerLabel
          *label; // const LilyCheckedScopeContainerLabel* (&)
        const LilyCheckedScopeContainerVariable
          *variable; // const LilyCheckedScopeContainerVariable* (&)
        const LilyCheckedScopeContainerGeneric
          *generic; // const LilyCheckedScopeContainerGeneric* (&)
        const LilyCheckedScopeContainerMethod
          *method; // const LilyCheckedScopeContainerMethod* (&)
    };
} LilyCheckedScopeContainer;

#define LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(k, container, n) \
    inline VARIANT_CONSTRUCTOR(LilyCheckedScopeContainer,         \
                               LilyCheckedScopeContainer,         \
                               n,                                 \
                               Usize scope_id,                    \
                               const container *n)                \
    {                                                             \
        return (LilyCheckedScopeContainer){ .kind = k,            \
                                            .scope_id = scope_id, \
                                            .n = n };             \
    }

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_CAPTURED_VARIABLE,
  LilyCheckedScopeContainerCapturedVariable,
  captured_variable);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_MODULE,
  LilyCheckedScopeContainerModule,
  module);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_CONSTANT,
  LilyCheckedScopeContainerConstant,
  constant);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(LILY_CHECKED_SCOPE_CONTAINER_KIND_ENUM,
                                         LilyCheckedScopeContainerEnum,
                                         enum_);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_RECORD,
  LilyCheckedScopeContainerRecord,
  record);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_ALIAS,
  LilyCheckedScopeContainerAlias,
  alias);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_ERROR,
  LilyCheckedScopeContainerError,
  error);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_ENUM_OBJECT,
  LilyCheckedScopeContainerEnumObject,
  enum_object);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_RECORD_OBJECT,
  LilyCheckedScopeContainerRecordObject,
  record_object);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_CLASS,
  LilyCheckedScopeContainerClass,
  class);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_TRAIT,
  LilyCheckedScopeContainerTrait,
  trait);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(LILY_CHECKED_SCOPE_CONTAINER_KIND_FUN,
                                         LilyCheckedScopeContainerFun,
                                         fun);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_LABEL,
  LilyCheckedScopeContainerLabel,
  label);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_VARIABLE,
  LilyCheckedScopeContainerVariable,
  variable);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_GENERIC,
  LilyCheckedScopeContainerGeneric,
  generic);

LILY_CHECKED_SCOPE_CONTAINER_CONSTRUCTOR(
  LILY_CHECKED_SCOPE_CONTAINER_KIND_METHOD,
  LilyCheckedScopeContainerMethod,
  method);

/**
 *
 * @brief Convert LilyCheckedScopeContainer in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainer,
               const LilyCheckedScopeContainer *self);
#endif

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_CONTAINER_H
