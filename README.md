# Lily

![Code size](https://img.shields.io/github/languages/code-size/ArthurPV/lily?style=flat)
![Codacy code quality](https://img.shields.io/codacy/grade/7f4284cccba541ca9ec576272fa88134?style=flat)
![Build state](https://img.shields.io/github/actions/workflow/status/ArthurPV/lily/cmake.yml?branch=main&style=flat)
![License](https://img.shields.io/github/license/ArthurPV/lily)

## Summary

* [Contribute](#contribute)
* [Examples](#examples)
* [Build](#build)
* [License](#license)

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
    val person := Person { name = "John", age = 25 };
end
```

## Contribute

### Build

```bash
make debug
ninja -C build/Debug
```

## Build

```bash
make configure
ninja -C build
```

## License
Lily is licensed under a MIT license.