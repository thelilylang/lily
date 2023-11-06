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
#include <base/types.h>

#include <core/lily/shared/visibility.h>

enum LilyCheckedSymbolScopeKind
{
    LILY_CHECKED_SYMBOL_SCOPE_KIND_LOCAL,
    LILY_CHECKED_SYMBOL_SCOPE_KIND_MODULE,
    LILY_CHECKED_SYMBOL_SCOPE_KIND_PACKAGE
};

typedef struct LilyCheckedSymbolScopeLocal
{
    Usize scope_id;
    Usize id;
} LilyCheckedSymbolScopeLocal;

/**
 *
 * @brief Construct LilyCheckedSymbolScopeLocal type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolScopeLocal,
                   LilyCheckedSymbolScopeLocal,
                   Usize scope_id,
                   Usize id)
{
    return (LilyCheckedSymbolScopeLocal){ .scope_id = scope_id, .id = id };
}

typedef struct LilyCheckedSymbolScopeModule
{
    Usize module_id;
    Usize id;
} LilyCheckedSymbolScopeModule;

/**
 *
 * @brief Construct LilyCheckedSymbolScopeModule type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolScopeModule,
                   LilyCheckedSymbolScopeModule,
                   Usize module_id,
                   Usize id)
{
    return (LilyCheckedSymbolScopeModule){ .module_id = module_id, .id = id };
}

typedef struct LilyCheckedSymbolScopePackage
{
    Usize package_id;
    Usize module_id;
    Usize id;
} LilyCheckedSymbolScopePackage;

/**
 *
 * @brief Construct LilyCheckedSymbolScopePackage type.
 */
inline CONSTRUCTOR(LilyCheckedSymbolScopePackage,
                   LilyCheckedSymbolScopePackage,
                   Usize package_id,
                   Usize module_id,
                   Usize id)
{
    return (LilyCheckedSymbolScopePackage){ .package_id = package_id,
                                            .module_id = module_id,
                                            .id = id };
}

typedef struct LilyCheckedSymbolScope
{
    enum LilyCheckedSymbolScopeKind kind;
    union
    {
        LilyCheckedSymbolScopeLocal local;
        LilyCheckedSymbolScopeModule module;
        LilyCheckedSymbolScopePackage package;
    };
} LilyCheckedSymbolScope;

/**
 *
 * @brief Construct LilyCheckedSymbolScope type
 * (LILY_CHECKED_SYMBOL_SCOPE_KIND_LOCAL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolScope,
                           LilyCheckedSymbolScope,
                           local,
                           LilyCheckedSymbolScopeLocal local)
{
    return (LilyCheckedSymbolScope){ .kind =
                                       LILY_CHECKED_SYMBOL_SCOPE_KIND_LOCAL,
                                     .local = local };
}

/**
 *
 * @brief Construct LilyCheckedSymbolScope type
 * (LILY_CHECKED_SYMBOL_SCOPE_KIND_MODULE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolScope,
                           LilyCheckedSymbolScope,
                           module,
                           LilyCheckedSymbolScopeModule module)
{
    return (LilyCheckedSymbolScope){ .kind =
                                       LILY_CHECKED_SYMBOL_SCOPE_KIND_MODULE,
                                     .module = module };
}

/**
 *
 * @brief Construct LilyCheckedSymbolScope type
 * (LILY_CHECKED_SYMBOL_SCOPE_KIND_PACKAGE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedSymbolScope,
                           LilyCheckedSymbolScope,
                           package,
                           LilyCheckedSymbolScopePackage package)
{
    return (LilyCheckedSymbolScope){ .kind =
                                       LILY_CHECKED_SYMBOL_SCOPE_KIND_PACKAGE,
                                     .package = package };
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
    LILY_CHECKED_SYMBOL_KIND_FUN,
    LILY_CHECKED_SYMBOL_KIND_LABEL,
    LILY_CHECKED_SYMBOL_KIND_METHOD,
    LILY_CHECKED_SYMBOL_KIND_MODULE,
    LILY_CHECKED_SYMBOL_KIND_PARAM,
    LILY_CHECKED_SYMBOL_KIND_RECORD,
    LILY_CHECKED_SYMBOL_KIND_RECORD_OBJECT,
    LILY_CHECKED_SYMBOL_KIND_TRAIT,
    LILY_CHECKED_SYMBOL_KIND_VARIABLE,
    LILY_CHECKED_SYMBOL_KIND_VARIANT,
};

typedef struct LilyCheckedSymbol
{
    enum LilyCheckedSymbolKind kind;
    enum LilyVisibility visibility;
    LilyCheckedSymbolScope scope;
} LilyCheckedSymbol;

/**
 *
 * @brief Construct LilyCheckedSymbol type.
 */
CONSTRUCTOR(LilyCheckedSymbol *,
            LilyCheckedSymbol,
            enum LilyCheckedSymbolKind kind,
            enum LilyVisibility visibility,
            LilyCheckedSymbolScope scope);

/**
 *
 * @brief Free LilyCheckedSymbol type.
 */
inline DESTRUCTOR(LilyCheckedSymbol, LilyCheckedSymbol *self)
{
    lily_free(self);
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SYMBOL_H
