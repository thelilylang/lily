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

#### Parametter

If the function is associated to a type, the first parametter, must be named `self`.

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

## C++

We using [C++20](https://en.cppreference.com/w/cpp/20) standard.

## Lily

## Abreviations
