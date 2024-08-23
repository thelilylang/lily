# Lily

![Code size](https://img.shields.io/github/languages/code-size/thelilylang/lily?style=for-the-badge)
![Codacy code quality](https://img.shields.io/codacy/grade/7f4284cccba541ca9ec576272fa88134?style=for-the-badge)
![Build state](https://img.shields.io/github/actions/workflow/status/thelilylang/lily/cmake.yml?branch=main&style=for-the-badge)
![Issues](https://img.shields.io/github/issues/thelilylang/lily?style=for-the-badge)
![Pull requests](https://img.shields.io/github/issues-pr/thelilylang/lily?style=for-the-badge)
![Lines of code](https://tokei.rs/b1/github/thelilylang/lily?caterogy=code&style=for-the-badge)
![Commit activity](https://img.shields.io/github/commit-activity/w/thelilylang/lily?style=for-the-badge)
![Last commit](https://img.shields.io/github/last-commit/thelilylang/lily?style=for-the-badge)
![License](https://img.shields.io/github/license/thelilylang/lily?style=for-the-badge)

Work in progress...

## Summary

* [Progression](#progression)
* [Release Schedule](#release-schedule)
* [Examples](#examples)
* [Dependencies](#dependencies)
* [Contribute](#contribute)
* [Build](#build)
* [Compatibility](#compatibility)
* [License](#license)

## Progression

- Scanner: 99% finished
- Preparser: 97% finished
- Precompiler: 80% finished
- Parser: 90% finished
- Analysis: 60% finished
- MIR: 55% finished
- Code generator (LLVM): 60% finished
- Code generator (C): 0% finished
- Code generator (C++): 0% finished
- Linker (LLVM) 30% finished
- Linker (C) 0% finished
- Linker (C++) 0% finished

## Release Schedule

| Version | Content                                                          | Release date |
|---------|------------------------------------------------------------------|--------------|
| v0.0.0  | First release.                                                   | 2023-09-21   |
| v0.1.0  | Implement CI. (And gradually convert the codebase from C to CI.) | 2024-05-21   |
| v0.2.0  | Continue to implement CI.                                        | 2024-06-22   |
| v0.3.0  | Continue to implement CI (2).                                    | 2024-07-21   |
| v0.4.0  | Continue to implement CI (3).                                    | 2024-08-22   |
| v0.5.0  | Continue to implement CI (4).                                    | 2024-09-21   |
| v0.6.0  | Add some basic features                                          | 2024-10-21   |
| v0.7.0  | Implement OOP                                                    | 2024-11-21   |
| v0.8.0  | Begin to write Std library + Core library                        | 2024-12-21   |
| v0.9.0  | Implement @std.C.*                                               | 2025-03-21   |
| v0.10.0 | Implement contract oriented programming                          | 2025-07-21   |
| v0.11.0 | Continue to write Std library                                    | 2025-10-21   |
| v0.12.0 | Continue to write Std library (Containers)                       | 2025-12-21   |
| v0.13.0 | Implement C mode                                                 | 2026-03-21   |
| v0.14.0 | Implement C++ mode                                               | 2026-06-21   |
| v0.15.0 | ASM features                                                     | 2026-09-21   |
| v0.16.0 | Thread + Future data type                                        | 2026-12-21   |
| v1.0.0  | Self host and add some other features                            |--------------|

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

## Dependencies

- CMake >= 3.20
- Clang >= 16.0
- LLVM 16
- Zlib
- Zstd

## Contribute

See [CONTRIBUTING.md](./CONTRIBUTING.md).

## Build

See [BUILD.md](./BUILD.md).

## Compatibility

| OS              | Support (Yes/No) |
|-----------------|------------------|
| Windows         | No               |
| MacOS Intel     | No               |
| MacOS Arm       | No               |
| ArchLinux       | Yes              |
| Ubuntu          | Yes              |
| Linux           | Yes              |
| BSD             | No               |

## License

Lily is licensed under a MIT license.
