# CImproved (CI)

## Why?

The aim of this project is to make it possible to use generics in C, with automatic generation. It's true that in C it's already possible to use pseudo-generic, but this is done manually and is not really safe for linking. For example, you can easily end up with large projects with duplicate symbols or undefined references, and this is clearly not an elegant way to write generic code.

Here's one of the current ways of doing generics in C:

```c
// ...

#define Vec(T) \
    struct Vec__##T { \
        T* buffer; \
        size_t len; \
        size_t capacity; \
    }; \
    \
    inline struct Vec__##T init__Vec__##T() { \
        return (struct Vec__##T){ .buffer = NULL, .len = 0, .capacity = 4 }; \
    }

// For example:

Vec(int);
Vec(double);
```

It's clearly not a pretty sight, and one of the problems with this way of working is that you can't use all the types (like pointers), so you're forced to alias back and forth to use these types.

The Lily project is currently written without generic, but only with `void*`, but this is clearly not the best way of doing things here either, because problems like bad casting can happen, so the use of memory is clearly not optimized. So, it was all these things that forced me to develop this transpiler in order to be able to have generic in C. 

## About the solution

For the instance, my only desire was to develop a transpiler to be able to generate generic code, automatically in a completely link-safe way, but I'll probably add things in the future to make C even better. The big advantage of CI over solutions like Zig, Rust, Go (ZRG) is that its syntax is very close to C (in fact, you can write C), which makes converting C code to CI very fast and inexpensive. Many people talk about languages like ZRG, as replacements for C (or C++, but that's not the point), but these people often forget that most maintainers of large projects like Linux, GNU, GCC, don't have the time to translate all their source code of several million lines into ZRG, because it would take too long and they need new features. CI's big advantage in this respect is that it can translate the code base of this kind of project as it goes along, not all at once, as it fully supports C syntax ((the aim is to support all standards).

In my case, the CI advantage is that I can translate Lily's source code in a very short time and with very little rewriting effort.

## Features

- Real generic support
- C syntax support
- Data type contexts
- Built-in build system
- Method call syntax

## Method call syntax

A function can be called as a method on what it takes first, so `s.len()` is
written where `str_len(s)` is meant. Nothing is added to the syntax: `.` and
`->` on anything but a structure or a union is a constraint violation in C
(6.5.2.3p1), so what is read as a method is what C reads as nothing.

What makes a function a method is the name it is declared by, which is written
in `CI.yaml` as a convention:

```yaml
method_convention: $type_$name  # `str_len` is `len` on `str`
type_convention: $name_t        # only `vec_t` and the like are given methods
```

`type_convention` is optional, and every type is given methods where none is
written. Where no `method_convention` is written nothing is a method, so a
project that writes none is left as it was. Both are passed to `cic` as
`--method-convention` and `--type-convention` as well.

```c
typedef char *str;

size_t str_len(str s) { return strlen(s); }

int main() {
	str s = "Hello";

	printf("%zu\n", s.len()); // str_len(s)
}
```

What the receiver is read as:

- The name is built forward, out of the receiver and the method name, and
  never read backwards out of the name of a function - `$type_$name` splits
  `str__len` two ways.
- A method is looked for on the name the receiver is **declared** as rather
  than on what that name aliases, so a `typedef` is what gives a type its
  methods. Two typedefs of the same type are given methods of their own, and a
  cast is what says which of them is meant: `((str)raw).len()`.
- What a method is called on is read as the first param it takes: the address
  is taken of what is written as a value, and what is written as a pointer is
  read through, so `v.size()` and `p->size()` both reach `Vec_size(const Vec *)`.
  A method written to take nothing is written on nothing, and is reported on.
- A method written on a generic type is instantiated for what its receiver is
  written with, so `Vec_push.[@T]` called on a `Vec.[int]` is the instance for
  `[int]` - see `examples/ci/vec.ci`.
- Where C already reads the call as one made through a member holding a
  function, that is what it is left as. A method of that name as well leaves
  nothing to say which is called, and is reported on.

## Examples

```c
#include <stdio.h>

// Generic function
@T
get.[@T](@T value) {
    return value;
}

int main() {
    printf("%d\n", get.[int](10));
    printf("%s\n", get.[char*]("Hello, World!"));
}
```

You can find more code examples in the following [directory](/examples/core/cc/ci).

## Documentation

You can find the documentation in the following [directory](/docs/core/cc/ci).

## References

- [C99 standard](https://www.open-std.org/Jtc1/sc22/wg14/www/docs/n1256.pdf)
- [C11 standard](https://www.open-std.org/JTC1/SC22/WG14/www/docs/n1570.pdf)
- [C23 standard](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3220.pdf)
- [C2Y standard](https://www.open-std.org/JTC1/SC22/WG14/www/docs/n3301.pdf)
