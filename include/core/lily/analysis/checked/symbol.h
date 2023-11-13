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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H

#include <base/alloc.h>
#include <base/new.h>
#include <base/types.h>

#include <core/lily/analysis/checked/limits.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedSymbolPackage
{
    Usize id;
} LilyCheckedSymbolPackage;

/**
 *
 * @brief Construct LilyCheckedSymbolPackage type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolPackage, LilyCheckedSymbolPackage, Usize id)
{
    return (LilyCheckedSymbolPackage){ .id = id };
}

typedef struct LilyCheckedSymbolAlias
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolAlias;

/**
 *
 * @brief Construct LilyCheckedSymbolAlias type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolAlias,
                   LilyCheckedSymbolAlias,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolAlias){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolClass
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolClass;

/**
 *
 * @brief Construct LilyCheckedSymbolClass type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolClass,
                   LilyCheckedSymbolClass,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolClass){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolConstant
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolConstant;

/**
 *
 * @brief Construct LilyCheckedSymbolConstant type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolConstant,
                   LilyCheckedSymbolConstant,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolConstant){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolEnum
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolEnum;

/**
 *
 * @brief Construct LilyCheckedSymbolEnum type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolEnum,
                   LilyCheckedSymbolEnum,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolEnum){ .package =
                                      NEW(LilyCheckedSymbolPackage, package_id),
                                    .id = id };
}

typedef struct LilyCheckedSymbolEnumObject
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolEnumObject;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolEnumObject type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolEnumObject,
                   LilyCheckedSymbolEnumObject,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolEnumObject){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolError
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolError;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolError type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolError,
                   LilyCheckedSymbolError,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolError){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolRecord
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolRecord;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolRecord type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolRecord,
                   LilyCheckedSymbolRecord,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolRecord){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolField
{
    LilyCheckedSymbolRecord record;
    Usize id;
} LilyCheckedSymbolField;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolField type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolField,
                   LilyCheckedSymbolField,
                   Usize package_id,
                   Usize record_id,
                   Usize id)
{
    return (LilyCheckedSymbolField){
        .record = NEW(LilyCheckedSymbolRecord, package_id, record_id), .id = id
    };
}

typedef struct LilyCheckedSymbolRecordObject
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolRecordObject;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolRecordObject type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolRecordObject,
                   LilyCheckedSymbolRecordObject,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolRecordObject){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolFieldObject
{
    LilyCheckedSymbolRecordObject record_object;
    Usize id;
} LilyCheckedSymbolFieldObject;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolFieldObject type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolFieldObject,
                   LilyCheckedSymbolFieldObject,
                   Usize package_id,
                   Usize record_object_id,
                   Usize id)
{
    return (LilyCheckedSymbolFieldObject){
        .record_object =
          NEW(LilyCheckedSymbolRecordObject, package_id, record_object_id),
        .id = id
    };
}

typedef struct LilyCheckedSymbolFun
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolFun;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolFun type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolFun,
                   LilyCheckedSymbolFun,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolFun){ .package =
                                     NEW(LilyCheckedSymbolPackage, package_id),
                                   .id = id };
}

typedef struct LilyCheckedSymbolScope
{
    Usize id;
} LilyCheckedSymbolScope;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolScope type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolScope, LilyCheckedSymbolScope, Usize id)
{
    return (LilyCheckedSymbolScope){ .id = id };
}

typedef struct LilyCheckedSymbolLabel
{
    LilyCheckedSymbolScope scope;
    Usize id;
} LilyCheckedSymbolLabel;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolLabel type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolLabel,
                   LilyCheckedSymbolLabel,
                   Usize scope_id,
                   Usize id)
{
    return (LilyCheckedSymbolLabel){ .scope =
                                       NEW(LilyCheckedSymbolScope, scope_id),
                                     .id = id };
}

enum LilyCheckedSymbolObjectKind
{
    LILY_CHECKED_SYMBOL_OBJECT_KIND_CLASS,
    LILY_CHECKED_SYMBOL_OBJECT_KIND_RECORD_OBJECT,
    LILY_CHECKED_SYMBOL_OBJECT_KIND_ENUM_OBJECT,
    LILY_CHECKED_SYMBOL_OBJECT_KIND_TRAIT
};

typedef struct LilyCheckedSymbolTrait
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolTrait;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolTrait type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolTrait,
                   LilyCheckedSymbolTrait,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolTrait){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolObject
{
    enum LilyCheckedSymbolObjectKind kind;
    union
    {
        LilyCheckedSymbolClass class;
        LilyCheckedSymbolRecordObject record_object;
        LilyCheckedSymbolEnumObject enum_object;
        LilyCheckedSymbolTrait trait;
    };
} LilyCheckedSymbolObject;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolObject type
 * (LILY_CHECKED_SYMBOL_OBJECT_KIND_CLASS).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolObject,
                           LilyCheckedSymbolObject,
                           class,
                           Usize package_id,
                           Usize id)
{
    return (LilyCheckedSymbolObject){
        .kind = LILY_CHECKED_SYMBOL_OBJECT_KIND_CLASS,
        .class = NEW(LilyCheckedSymbolClass, package_id, id)
    };
}

/**
 *
 *
 * @brief Construct LilyCheckedSymbolObject type
 * (LILY_CHECKED_SYMBOL_OBJECT_KIND_RECORD_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolObject,
                           LilyCheckedSymbolObject,
                           record_object,
                           Usize package_id,
                           Usize id)
{
    return (LilyCheckedSymbolObject){
        .kind = LILY_CHECKED_SYMBOL_OBJECT_KIND_RECORD_OBJECT,
        .record_object = NEW(LilyCheckedSymbolRecordObject, package_id, id)
    };
}

/**
 *
 *
 * @brief Construct LilyCheckedSymbolObject type
 * (LILY_CHECKED_SYMBOL_OBJECT_KIND_ENUM_OBJECT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolObject,
                           LilyCheckedSymbolObject,
                           enum_object,
                           Usize package_id,
                           Usize id)
{
    return (LilyCheckedSymbolObject){
        .kind = LILY_CHECKED_SYMBOL_OBJECT_KIND_ENUM_OBJECT,
        .enum_object = NEW(LilyCheckedSymbolEnumObject, package_id, id)
    };
}

/**
 *
 *
 * @brief Construct LilyCheckedSymbolObject type
 * (LILY_CHECKED_SYMBOL_OBJECT_KIND_TRAIT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolObject,
                           LilyCheckedSymbolObject,
                           trait,
                           Usize package_id,
                           Usize id)
{
    return (LilyCheckedSymbolObject){
        .kind = LILY_CHECKED_SYMBOL_OBJECT_KIND_TRAIT,
        .trait = NEW(LilyCheckedSymbolTrait, package_id, id)
    };
}

typedef struct LilyCheckedSymbolMethod
{
    LilyCheckedSymbolObject object;
    Usize id;
} LilyCheckedSymbolMethod;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolMethod type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolMethod,
                   LilyCheckedSymbolMethod,
                   LilyCheckedSymbolObject object,
                   Usize id)
{
    return (LilyCheckedSymbolMethod){ .object = object, .id = id };
}

typedef struct LilyCheckedSymbolModule
{
    LilyCheckedSymbolPackage package;
    Usize id;
} LilyCheckedSymbolModule;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolModule type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolModule,
                   LilyCheckedSymbolModule,
                   Usize package_id,
                   Usize id)
{
    return (LilyCheckedSymbolModule){
        .package = NEW(LilyCheckedSymbolPackage, package_id), .id = id
    };
}

typedef struct LilyCheckedSymbolParam
{
    LilyCheckedSymbolScope scope;
    Usize id;
} LilyCheckedSymbolParam;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolParam type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolParam,
                   LilyCheckedSymbolParam,
                   Usize scope_id,
                   Usize id)
{
    return (LilyCheckedSymbolParam){ .scope =
                                       NEW(LilyCheckedSymbolScope, scope_id),
                                     .id = id };
}

typedef struct LilyCheckedSymbolProperty
{
    LilyCheckedSymbolClass class;
    Usize id;
} LilyCheckedSymbolProperty;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolProperty type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolProperty,
                   LilyCheckedSymbolProperty,
                   Usize package_id,
                   Usize class_id,
                   Usize id)
{
    return (LilyCheckedSymbolProperty){
        .class = NEW(LilyCheckedSymbolClass, package_id, class_id), .id = id
    };
}

typedef struct LilyCheckedSymbolVariable
{
    LilyCheckedSymbolScope scope;
    Usize id;
} LilyCheckedSymbolVariable;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolVariable type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolVariable,
                   LilyCheckedSymbolVariable,
                   Usize scope_id,
                   Usize id)
{
    return (LilyCheckedSymbolVariable){ .scope =
                                          NEW(LilyCheckedSymbolScope, scope_id),
                                        .id = id };
}

typedef struct LilyCheckedSymbolVariant
{
    LilyCheckedSymbolEnum enum_;
    Usize id;
} LilyCheckedSymbolVariant;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolVariant type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolVariant,
                   LilyCheckedSymbolVariant,
                   Usize package_id,
                   Usize enum_id,
                   Usize id)
{
    return (LilyCheckedSymbolVariant){
        .enum_ = NEW(LilyCheckedSymbolEnum, package_id, enum_id), .id = id
    };
}

typedef struct LilyCheckedSymbolVariantObject
{
    LilyCheckedSymbolEnumObject enum_object;
    Usize id;
} LilyCheckedSymbolVariantObject;

/**
 *
 *
 * @brief Construct LilyCheckedSymbolVariantObject type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolVariantObject,
                   LilyCheckedSymbolVariantObject,
                   Usize package_id,
                   Usize enum_object_id,
                   Usize id)
{
    return (LilyCheckedSymbolVariantObject){
        .enum_object =
          NEW(LilyCheckedSymbolEnumObject, package_id, enum_object_id),
        .id = id
    };
}

enum LilyCheckedSymbolKind
{
    LILY_CHECKED_SYMBOL_KIND_ALIAS,
    LILY_CHECKED_SYMBOL_KIND_CLASS,
    LILY_CHECKED_SYMBOL_KIND_CONSTANT,
    LILY_CHECKED_SYMBOL_KIND_ENUM,
    LILY_CHECKED_SYMBOL_KIND_ENUM_OBJECT,
    LILY_CHECKED_SYMBOL_KIND_ERROR,
    LILY_CHECKED_SYMBOL_KIND_FIELD,
    LILY_CHECKED_SYMBOL_KIND_FIELD_OBJECT,
    LILY_CHECKED_SYMBOL_KIND_FUN,
    LILY_CHECKED_SYMBOL_KIND_LABEL,
    LILY_CHECKED_SYMBOL_KIND_METHOD,
    LILY_CHECKED_SYMBOL_KIND_MODULE,
    LILY_CHECKED_SYMBOL_KIND_PACKAGE,
    LILY_CHECKED_SYMBOL_KIND_PARAM,
    LILY_CHECKED_SYMBOL_KIND_PROPERTY,
    LILY_CHECKED_SYMBOL_KIND_RECORD,
    LILY_CHECKED_SYMBOL_KIND_RECORD_OBJECT,
    LILY_CHECKED_SYMBOL_KIND_SCOPE,
    LILY_CHECKED_SYMBOL_KIND_TRAIT,
    LILY_CHECKED_SYMBOL_KIND_VARIABLE,
    LILY_CHECKED_SYMBOL_KIND_VARIANT,
    LILY_CHECKED_SYMBOL_KIND_VARIANT_OBJECT
};

typedef struct LilyCheckedSymbol
{
    enum LilyCheckedSymbolKind kind;
    union
    {
        LilyCheckedSymbolAlias alias;
        LilyCheckedSymbolClass class;
        LilyCheckedSymbolConstant constant;
        LilyCheckedSymbolEnum enum_;
        LilyCheckedSymbolEnumObject enum_object;
        LilyCheckedSymbolError error;
        LilyCheckedSymbolField field;
        LilyCheckedSymbolFieldObject field_object;
        LilyCheckedSymbolFun fun;
        LilyCheckedSymbolLabel label;
        LilyCheckedSymbolMethod method;
        LilyCheckedSymbolModule module;
        LilyCheckedSymbolPackage package;
        LilyCheckedSymbolParam param;
        LilyCheckedSymbolProperty property;
        LilyCheckedSymbolRecord record;
        LilyCheckedSymbolRecordObject record_object;
        LilyCheckedSymbolScope scope;
        LilyCheckedSymbolTrait trait;
        LilyCheckedSymbolVariable variable;
        LilyCheckedSymbolVariant variant;
        LilyCheckedSymbolVariantObject variant_object;
    };
} LilyCheckedSymbol;

/**
 *
 * @brief Construct LilyCheckedSymbol type (LILY_CHECKED_SYMBOL_KIND_ALIAS).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    alias,
                    LilyCheckedSymbolAlias alias);

/**
 *
 * @brief Construct LilyCheckedSymbol type (LILY_CHECKED_SYMBOL_KIND_CLASS).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    class,
                    LilyCheckedSymbolClass class);

/**
 *
 * @brief Construct LilyCheckedSymbol type (LILY_CHECKED_SYMBOL_KIND_CONSTANT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    constant,
                    LilyCheckedSymbolConstant constant);

/**
 *
 * @brief Construct LilyCheckedSymbol type (LILY_CHECKED_SYMBOL_KIND_ENUM).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    enum,
                    LilyCheckedSymbolEnum enum_);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_ENUM_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    enum_object,
                    LilyCheckedSymbolEnumObject enum_object);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_ERROR).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    error,
                    LilyCheckedSymbolError error);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_FIELD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    field,
                    LilyCheckedSymbolField field);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_FIELD_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    field_object,
                    LilyCheckedSymbolFieldObject field_object);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_FUN).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    fun,
                    LilyCheckedSymbolFun fun);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_LABEL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    label,
                    LilyCheckedSymbolLabel label);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_METHOD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    method,
                    LilyCheckedSymbolMethod method);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_MODULE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    module,
                    LilyCheckedSymbolModule module);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_PACKAGE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    package,
                    LilyCheckedSymbolPackage package);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_PARAM).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    param,
                    LilyCheckedSymbolParam param);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_PROPERTY).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    property,
                    LilyCheckedSymbolProperty property);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_RECORD).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    record,
                    LilyCheckedSymbolRecord record);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_RECORD_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    record_object,
                    LilyCheckedSymbolRecordObject record_object);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_SCOPE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    scope,
                    LilyCheckedSymbolScope scope);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_TRAIT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    trait,
                    LilyCheckedSymbolTrait trait);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_VARIABLE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    variable,
                    LilyCheckedSymbolVariable variable);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_VARIANT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    variant,
                    LilyCheckedSymbolVariant variant);

/**
 *
 * @brief Construct LilyCheckedSymbol type
 * (LILY_CHECKED_SYMBOL_KIND_VARIANT_OBJECT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    variant_object,
                    LilyCheckedSymbolVariantObject variant_object);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H
