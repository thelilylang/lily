# Lily

## Summary

- [Introduction](#introduction)
- [Why?](#why)
- [Examples](#examples)
- [Documentation](#documentation)

## Introduction

> [!CAUTION]
> It is currently in the early stages of development and is not yet ready for production use.

## Why?

The aim of the Lily programming language is to be simple, flexible, performance, low-level programming access and (pretty) safe. The language is designed to be easy to learn and use, with a syntax that is similar to other languages like OCaml and Ruby. 

## Examples

> Create a simple function called `add` that adds the parameter x to the parameter y.

```lily
fun add(x, y) = x + y end

fun main =
    val res := add(10, 20);
end
```

> Create a simple record (similar to OCaml) named `Person`. 

```lily
type Person record =
    name Str;
    age Uint8;
end

fun main =
    val person := Person { name := "John", age := 25 };
end
```

You can find more code examples in the following [directory](/examples/core/lily).

## Documentation

You can find the documentation in the following [directory](/docs/core/lily).