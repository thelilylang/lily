# Lily

![Total lines](https://img.shields.io/tokei/lines/github/ArthurPV/lily?style=for-the-badge)
![Code size](https://img.shields.io/github/languages/code-size/ArthurPV/lily?style=for-the-badge)
![Codacy code quality](https://img.shields.io/codacy/grade/7f4284cccba541ca9ec576272fa88134?style=for-the-badge)
![Build state](https://img.shields.io/github/actions/workflow/status/ArthurPV/lily/cmake.yml?branch=main&style=for-the-badge)
![Issues](https://img.shields.io/github/issues/ArthurPV/lily?style=for-the-badge)
![Pull requests](https://img.shields.io/github/issues-pr/ArthurPV/lily?style=for-the-badge)
![Commit activity](https://img.shields.io/github/commit-activity/w/ArthurPV/lily?style=for-the-badge)
![Last commit](https://img.shields.io/github/last-commit/ArthurPV/lily?style=for-the-badge)
![License](https://img.shields.io/github/license/ArthurPV/lily?style=for-the-badge)

Work in progress...

## Summary

* [Contribute](#contribute)
* [Examples](#examples)
* [Dependencies](#dependencies)
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

## Dependencies

- CMake >= 3.20
- GCC >= 4.6 or Clang >= 3.0

## Contribute

### Build

```bash
make debug
ninja -C build/Debug
```

### Test

> You can run `test.sh` to test with the `ctest` utility.

NOTE: Using `test.sh` the `make clean` command will be run then the scripts will recompile the whole project and finally run `ctest`. 

```bash
./scripts/test.sh
```

> However, using `test.sh` it is not possible to see exactly which test fails for example, so in this case use `full_test.sh`.

```bash
./scripts/full_test.sh
```

## Build

```bash
make configure
ninja -C build
```

## License
Lily is licensed under a MIT license.
