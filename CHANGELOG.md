# Changelog

## 0.0.1 - 2023-09-21

### Fixed

- Fix linking errors on MacOS [#401](https://github.com/thelilylang/lily/pull/401)

## 0.0.2 - 2023-09-22

### Fixed

- Fix infinite wait when running child packages [#403](https://github.com/thelilylang/lily/pull/403)

## 0.1.0 - 2024-05-21

### Added

- Begin to implement CI [#424](https://github.com/thelilylang/lily/pull/424)
- Implement anonymous struct/union [#433](https://github.com/thelilylang/lily/pull/433)
- feat(cc/ci): Scan preprocessors [#438](https://github.com/thelilylang/lily/pull/438)
- Resolve preprocessor [#439](https://github.com/thelilylang/lily/pull/439)
- Scan & Resolve macro defined [#440](https://github.com/thelilylang/lily/pull/440)
- Use `HAS_REACH_END` macro in Lily scanner [#441](https://github.com/thelilylang/lily/pull/441)
- Scan integer and float suffixes [#443](https://github.com/thelilylang/lily/pull/443)
- Add standard predefined macros [#444](https://github.com/thelilylang/lily/pull/444)
- Parse array access [#450](https://github.com/thelilylang/lily/pull/450)
- Parse list of variables [#452](https://github.com/thelilylang/lily/pull/452)
- Parse typedef declaration [#453](https://github.com/thelilylang/lily/pull/453)
- Manage recursive type [#455](https://github.com/thelilylang/lily/pull/455)
- Parse array [#462](https://github.com/thelilylang/lily/pull/462)
- Manage prototype rule [#465](https://github.com/thelilylang/lily/pull/465)
- Add `_XOPEN_SOURCE` macro as builtin [#468](https://github.com/thelilylang/lily/pull/468)
- Define standard macros [#471](https://github.com/thelilylang/lily/pull/471)
- Build entity (library, binary, ...) [#472](https://github.com/thelilylang/lily/pull/472)
- Add static storage class to all functions gen [#475](https://github.com/thelilylang/lily/pull/475)
- Parse ternary expression [#477](https://github.com/thelilylang/lily/pull/477)
- Implement ID merge [#478](https://github.com/thelilylang/lily/pull/478)

### Fixed

- Fix bug on expression with defined macro [#442](https://github.com/thelilylang/lily/pull/442)
- Fix semicolon bug in the parser [#446](https://github.com/thelilylang/lily/pull/446)
- Fix `for` statement init-clause [#447](https://github.com/thelilylang/lily/pull/447)
- Fix bug on `while` statement parsing [#448](https://github.com/thelilylang/lily/pull/448)
- Fix: The parser incorrectly parse the `do..while` statement. [#449](https://github.com/thelilylang/lily/pull/449)
- Fix bug on parsing of `enum` and `struct` [#451](https://github.com/thelilylang/lily/pull/451)
- Fix parsing of post expression [#454](https://github.com/thelilylang/lily/pull/454)
- Fix bug on for loop parsing [#459](https://github.com/thelilylang/lily/pull/459)
- Fix parsing on sizeof expression [#460](https://github.com/thelilylang/lily/pull/460)
- Fix miss standarization of keyword [#461](https://github.com/thelilylang/lily/pull/461)
- Fix bug on switch statement parsing [#463](https://github.com/thelilylang/lily/pull/463)
- Fix bug in generation of builtin value [#473](https://github.com/thelilylang/lily/pull/473)
- Fix leak during macro param injection [#479](https://github.com/thelilylang/lily/pull/479)

### Changed

- Change generic syntax [#458](https://github.com/thelilylang/lily/pull/458)
- Update initialization rule [#464](https://github.com/thelilylang/lily/pull/464)
- Turn token as node [#469](https://github.com/thelilylang/lily/pull/469)
- Update include system [#476](https://github.com/thelilylang/lily/pull/476)
