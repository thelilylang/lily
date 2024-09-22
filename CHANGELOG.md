# Changelog

## 0.5.0 - 2024-09-21

### Added

- Implement implicit casting [#527](https://github.com/thelilylang/lily/pull/527)
- Finish to resolve unary expression [#530](https://github.com/thelilylang/lily/pull/530)
- Add C standard stddef header [#536](https://github.com/thelilylang/lily/pull/536)
- Add C standard stdint header [#537](https://github.com/thelilylang/lily/pull/537)
- Typecheck params for generic function [#539](https://github.com/thelilylang/lily/pull/539)
- Finish to visit generic function [#541](https://github.com/thelilylang/lily/pull/541)
- Add the ability to call generic function [#542](https://github.com/thelilylang/lily/pull/542)
- Add the ability to call struct or union in generic context [#544](https://github.com/thelilylang/lily/pull/544)
- Add file system watcher [#547](https://github.com/thelilylang/lily/pull/547)
- Typecheck dot and arrow access [#548](https://github.com/thelilylang/lily/pull/548)
- Add the ability to call function as identifier [#550](https://github.com/thelilylang/lily/pull/550)
- Add `std_include` setting to CI configuration [#553](https://github.com/thelilylang/lily/pull/553)
- Add a script to auto-format files each time they are modified [#555](https://github.com/thelilylang/lily/pull/555)
- Add object value/object schema structures [#556](https://github.com/thelilylang/lily/pull/556)

### Changed

- Improve inference of unary expression [#526](https://github.com/thelilylang/lily/pull/526)
- Implement CLI for CI [#529](https://github.com/thelilylang/lily/pull/529)
- Undef stdint macros [#538](https://github.com/thelilylang/lily/pull/538)
- Infer string as a pointer [#540](https://github.com/thelilylang/lily/pull/540)

### Fixed

- Fix `format__String` function [#528](https://github.com/thelilylang/lily/pull/528)
- Fix generic params [#531](https://github.com/thelilylang/lily/pull/531)
- Fix leak in struct call typecheck [#532](https://github.com/thelilylang/lily/pull/532)
- Fix struct call typecheck [#533](https://github.com/thelilylang/lily/pull/533)
- Fix search in scope bug [#534](https://github.com/thelilylang/lily/pull/534)
- Fix leak in parse function params [#535](https://github.com/thelilylang/lily/pull/535)
- Fix bad generation of serialized name [#543](https://github.com/thelilylang/lily/pull/543)
- Fix stack overflow while typechecking named struct [#545](https://github.com/thelilylang/lily/pull/545)
- Fix bad typecheck diagnostic while using const data type [#546](https://github.com/thelilylang/lily/pull/546)
- Fix few issues encountered on FreeBSD at compilation [#549](https://github.com/thelilylang/lily/pull/549)
- Fix `__FILE__` value [#551](https://github.com/thelilylang/lily/pull/551)
- Fix `__LINE__` value [#552](https://github.com/thelilylang/lily/pull/552)
- Fix: Add missing data types to the parser of data type [#554](https://github.com/thelilylang/lily/pull/554)

## 0.4.0 - 2024-08-22

### Added

- Implement stringification [#518](https://github.com/thelilylang/lily/pull/518)
- Add C standard assert header [#520](https://github.com/thelilylang/lily/pull/520)
- Add C standard errno header [#521](https://github.com/thelilylang/lily/pull/521)
- Parse data type contexts [#522](https://github.com/thelilylang/lily/pull/522)
- Begin to implement typecheck [#523](https://github.com/thelilylang/lily/pull/523)

### Fixed

- Fix escape character generation [#519](https://github.com/thelilylang/lily/pull/519)

## 0.3.0 - 2024-07-21

### Added

- Implement \_\_VA_ARGS__ [#500](https://github.com/thelilylang/lily/pull/500)
- Resolve sizeof and alignof operator [#504](https://github.com/thelilylang/lily/pull/504)
- Add a way to give different integer size to enum [#505](https://github.com/thelilylang/lily/pull/505)
- Resolve size and alignment of builtin type [#509](https://github.com/thelilylang/lily/pull/509)
- Implement attribute for function declaration [#513](https://github.com/thelilylang/lily/pull/513)

### Fixed

- Fix: Allow prototype duplications [#503](https://github.com/thelilylang/lily/pull/503)
- Fix dependencies order generation [#506](https://github.com/thelilylang/lily/pull/506)
- Fix space after brace of enum declaration [#507](https://github.com/thelilylang/lily/pull/507)
- Fix potential bug in expression resolver [#508](https://github.com/thelilylang/lily/pull/508)
- Fix array LLVM generation [#510](https://github.com/thelilylang/lily/pull/510)
- Fix include path access [#512](https://github.com/thelilylang/lily/pull/512)
- Fix wrapped macro call bug [#514](https://github.com/thelilylang/lily/pull/514)
- Associate macro param with the right macro call ID [#515](https://github.com/thelilylang/lily/pull/515)

### Changed

- Stop to use `_BitInt(x)` Clang extension [#511](https://github.com/thelilylang/lily/pull/511)

## 0.2.0 - 2024-06-22

### Added

- Get builtin values according to the desired standard [#483](https://github.com/thelilylang/lily/pull/483)
- Implement (scan) variadic param [#482](https://github.com/thelilylang/lily/pull/482)
- Get compiler include dirs [#486](https://github.com/thelilylang/lily/pull/486)
- Add `__has_feature` [#487](https://github.com/thelilylang/lily/pull/487)
- Parse `nullptr` expression [#488](https://github.com/thelilylang/lily/pull/488)
- Implement way to get standard types from builtin file (e.g. \_\_SIZE_TYPE__) [#489](https://github.com/thelilylang/lily/pull/489)
- Parse typeof [#491](https://github.com/thelilylang/lily/pull/491)
- Add functions and types builtin [#492](https://github.com/thelilylang/lily/pull/492)
- Generate function call builtin [#498](https://github.com/thelilylang/lily/pull/498)
- Parse fixed array and function data type [#499](https://github.com/thelilylang/lily/pull/499)

### Fixed

- Fix parse unary [#484](https://github.com/thelilylang/lily/pull/484)
- Fix bug in include system [#493](https://github.com/thelilylang/lily/pull/493)
- Fix memory leak in resolve include preprocessor function [#494](https://github.com/thelilylang/lily/pull/494)
- Fix bug in peek scanner function [#495](https://github.com/thelilylang/lily/pull/495)
- Fix: Free duplicate macro [#496](https://github.com/thelilylang/lily/pull/496)
- Fix memory leaks when resolving a ternary expression [#497](https://github.com/thelilylang/lily/pull/497)

### Changed

- Update until note message [#485](https://github.com/thelilylang/lily/pull/485)
- Move the token standard check to the parser [#490](https://github.com/thelilylang/lily/pull/490)

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

## 0.0.2 - 2023-09-22

### Fixed

- Fix infinite wait when running child packages [#403](https://github.com/thelilylang/lily/pull/403)

## 0.0.1 - 2023-09-21

### Fixed

- Fix linking errors on MacOS [#401](https://github.com/thelilylang/lily/pull/401)
