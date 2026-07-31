# Coding Styles

## C

We using [C23](https://en.cppreference.com/w/c/23) standard. An other thing to know, we use `lily_base` library that's include some data structures, such as File, Vec, Stack, etc.

### Header Guard

When we name a header guard, we use the file path to name the guard. In addition, headers must be written in upper case. Additionally, the guard name must start with `LILY` and must end with the file extension, so most often `H` or `HPP`. Plus, the last two rules are that for example the name of the folder and the file is repeated with the same name without taking into account the extension of the file, we avoid repeating the same name. Finally, the header guard of the project file must be unique (exception in certain cases, like in `tests/core/lily/scanner/util.c`).

```c
// path: include/core/lily/analysis/analysis.h
// guard name: CORE_LILY_ANALYSIS_H (bad)
// guard name: LILY_CORE_LILY_ANALYSIS_ANALYSIS_H (bad)
// guard name: LILY_CORE_LILY_ANALYSIS_H (ok)
```

### Function

When you want to name a function, you must add a suffix `__` + type name. Plus, the base of the function name must be write in snake-case.

```c
typedef struct Person {
    const char *name;
    Uint8 age;
} Person;

const char *
get_name(const Person *self); // bad

const char *
get_name__Person(const Person *self); // ok
```

However, if the function is not associated to a type (enum or struct), add a suffix `__` + name access.

```c
// path: src/str.c
Usize
len__Str(const char *s);
```

NOTE: The prefix must be unique, to avoid function name conflicts.

#### Parameter

If the function is associated to a type, the first parameter, must be named `self`.

#### Inline

Take a look at [src/ex/README.md](./src/ex/README.md) to find out how inline functions are managed.

### Global variable

You must be define global variable with `static` keyword.

### Constant

All constants must be defined in uppercase.

```c
const int a = 340; // bad
const int A = 340; // ok
```

### Macro

All macros must be defined in uppercase.

```c
#define hello(s) "Hello "s // bad
#define HELLO(s) "Hello "s // ok
```

### Include

For most includes we use the one with angle brackets (`#include <header>`), but sometimes (e.g. in tests) we use the version with double quotes (`#include "header"`).

### Enum

The name of the enumeration must be written in `PascalCase`. Additionally, if the enumeration is used with a structure (to be used with a union), you must add `Kind` to the end of the enumeration name.<br>

The items in the enumeration must be written in `UpperCase` and each item name must begin with the name of the enum (in `UpperCase`).<br>

Finally, in most cases, you should add `Lily` in front of the enumeration name (with a few exceptions).<br>

EXCEPTION(S): `lily_base`.<br>

Here is a small example of using an enumeration with the rules previously listed:

```c
enum LilyAnimalKind {
    LILY_ANIMAL_KIND_CAT,
    LILY_ANIMAL_KIND_DOG,
};

typedef struct LilyCat {
  // ...
} LilyCat;

typedef struct LilyDog {
  // ...
} LilyDog;

typedef struct LilyAnimal {
    enum LilyAnimalKind kind;
    union {
        LilyCat cat;
        LilyDog dog;
    };
} LilyAnimal;
```

### Struct

The name of the structure must be written in `PascalCase`.<br>

As in enumerations, you must add `Lily` in front of the structure name (with exceptions).<br>

EXCEPTION(S): `lily_base`.<br>

### Type Annotation

C erases three things that matter to whoever reads the code next: whether a
pointer may be `NULL`, who is responsible for freeing it, and what a type-erased
container holds. We write those in a comment next to the type.

They go on struct fields and local variables as a trailing comment, and in doc
comments on `@param` and `@return`.

```c
typedef struct CIResolver
{
    CIResultFile *file;                // CIResultFile* (&)
    CIResolvedTokens *resolved_tokens; // CIResolvedTokens*?
    CIResolverMacroCall *macro_call;   // CIResolverMacroCall*?
} CIResolver;

/// @param attributes const Vec<CIAttribute*>*? (&)
/// @return CIDataType*? (&)
```

#### Grammar

| Notation | Meaning |
|---|---|
| `T*` | Owned. Whoever holds it is responsible for freeing it. |
| `T*?` | May be `NULL`. The `?` follows the `*` it qualifies. |
| `T* (&)` | Borrowed. Someone else owns it, do not free it. |
| `Vec<T*>*` | A container, and what it holds — `Vec`, `HashMap`, `Rc`, ... |
| `Vec<T* \| U*>*` | Holds either, told apart at run time. |

They combine, and the order reads outward from the type:
`const Vec<CIAttribute*>*? (&)` is a borrowed, possibly-`NULL` pointer to a
`Vec` of `CIAttribute*`.

Note where a qualifier sits — it applies to what it follows, so the two below
mean different things:

```c
Vec<CIDeclStructField* (&)>*  // an owned Vec of fields it does not own
Vec<CIDeclStructField*>* (&)  // a borrowed Vec of fields it owns
```

#### On a parameter, `(&)` says who frees it

This is the part worth reading twice, because getting it wrong is a
use-after-free rather than a leak.

- **`(&)` on a parameter** — the callee only borrows it. The caller still owns
  it and must release it.
- **no `(&)` on a parameter** — the callee takes it over. The caller must not
  release it afterwards.

The two string literal constructors differ on exactly this point:

```c
/// @param string Rc<String*>* (&)
inline VARIANT_CONSTRUCTOR(CIExprLiteral, CIExprLiteral, string, Rc *string);
//                     the expression refs it, so the caller still FREE_RC's

/// @param literal_constant_string Rc<String*>*
VARIANT_CONSTRUCTOR(CIToken *, CIToken, literal_constant_string,
                    Location location, Rc *literal_constant_string);
//                     the token takes it over, the caller must not FREE_RC
```

Freed memory usually reads back correctly for a while, so releasing something
the callee took over does not fail where you wrote it — it fails somewhere else,
much later. Read the annotation before matching a `NEW`/`FREE` pattern you saw
elsewhere.

#### Keeping them honest

They are comments, so nothing checks them. When you move a declaration, move its
doc comment with it, and when you change what a function takes or gives back,
change the annotation in the same edit.

## C++

We're using [C++20](https://en.cppreference.com/w/cpp/20) standard.

## Lily

...

## Abreviations

- ptr: pointer
- ast, AST: Abstract Syntax Tree
- ref: reference
- vm: virtual machine
- decl: declaration
- expr: expression
- stmt: statement
- gen: generation, generator
