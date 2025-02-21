# Changelog

## 0.10.0 - 2025-02-21

### Fixed

- Fix duplicate version in `docs/release_schedule.md` [#723](https://github.com/thelilylang/lily/pull/723)
- Fix bad error "data type is not expected with initializer" [#724](https://github.com/thelilylang/lily/pull/724)

### Changed

- Improve parsing of CI configuration [#725](https://github.com/thelilylang/lily/pull/725)

## 0.9.0 - 2025-02-04

### Added

- Implement embed preprocessor [#699](https://github.com/thelilylang/lily/pull/699)
- Add new preprocessor tests [#701](https://github.com/thelilylang/lily/pull/701)
- Add result example [#706](https://github.com/thelilylang/lily/pull/706)
- Typecheck variadic function called parameter [#707](https://github.com/thelilylang/lily/pull/707)
- Add a way to disable the state checker [#708](https://github.com/thelilylang/lily/pull/708)
- Continue to implement the state checker [#710](https://github.com/thelilylang/lily/pull/710)
- Add experimental cc runtime [#717](https://github.com/thelilylang/lily/pull/717)
- Implement strerror function [#719](https://github.com/thelilylang/lily/pull/719)

### Fixed

- Fix bug on null character [#700](https://github.com/thelilylang/lily/pull/700)
- Fix switch statement typecheck [#703](https://github.com/thelilylang/lily/pull/703)
- Fix bug on substitution of fields and get data type of enum variant [#704](https://github.com/thelilylang/lily/pull/704)
- Fix segfault on visit of function params [#709](https://github.com/thelilylang/lily/pull/709)
- Fix warnings in analysis [#713](https://github.com/thelilylang/lily/pull/713)
- Fix warning in checked data type [#715](https://github.com/thelilylang/lily/pull/715)

### Changed

- Update the license to 2025 [#702](https://github.com/thelilylang/lily/pull/702)
- Disable typecheck of path binary operator [#705](https://github.com/thelilylang/lily/pull/705)
- Upgrade to LLVM 18 [#712](https://github.com/thelilylang/lily/pull/712)
- Beautify files [#714](https://github.com/thelilylang/lily/pull/714)
- Rename header guard name of expression resolver [#716](https://github.com/thelilylang/lily/pull/716)
- Refactor libc organization [#718](https://github.com/thelilylang/lily/pull/718)
- Refactor parsing of declarator [#720](https://github.com/thelilylang/lily/pull/720)

## 0.8.0 - 2024-12-31

### Added

- Parse multiple fields on same line [#683](https://github.com/thelilylang/lily/pull/683)
- Add enum variants to enum data type [#684](https://github.com/thelilylang/lily/pull/684)
- Enable `restrict` in string standard header [#691](https://github.com/thelilylang/lily/pull/691)
- Add node example [#694](https://github.com/thelilylang/lily/pull/694)
- Implement `nullptr_t` for C23 standard [#696](https://github.com/thelilylang/lily/pull/696)

### Changed

- Prepare to implement state checker [#689](https://github.com/thelilylang/lily/pull/689)
- Make some improvements (typecheck, inferance, ...) [#695](https://github.com/thelilylang/lily/pull/695)

### Fixed

- Fix macro call with paren after [#681](https://github.com/thelilylang/lily/pull/681)
- Allow initialization on cast expression [#685](https://github.com/thelilylang/lily/pull/685)
- Allow array and function to cast data type [#686](https://github.com/thelilylang/lily/pull/686)
- Disallow assignment to array data type [#686](https://github.com/thelilylang/lily/pull/687)
- Fix visit of struct, union and typedef [#692](https://github.com/thelilylang/lily/pull/692)
- Fix missing typedef data type resolution [#693](https://github.com/thelilylang/lily/pull/693)

### Removed

- Remove comment doc token after scan [#682](https://github.com/thelilylang/lily/pull/682)
- Remove unused EOT contexts [#688](https://github.com/thelilylang/lily/pull/688)
- Remove unused code in the parser [#690](https://github.com/thelilylang/lily/pull/690)

## 0.7.0 - 2024-11-21

### Added

- Scan GNU attribute [#648](https://github.com/thelilylang/lily/pull/648)
- Add support to `__restrict` and `__restrict__` extension [#651](https://github.com/thelilylang/lily/pull/651)
- Add `__extension__` keyword support [#656](https://github.com/thelilylang/lily/pull/656)
- Write a preprocessor resolver [#659](https://github.com/thelilylang/lily/pull/659)
- Parse custom expression of enum variant [#661](https://github.com/thelilylang/lily/pull/661)
- Resolve indirect macro call [#663](https://github.com/thelilylang/lily/pull/663)
- Add the ability to merge keyword [#665](https://github.com/thelilylang/lily/pull/665)
- Parse bit field [#667](https://github.com/thelilylang/lily/pull/667)
- Generate bit field [#669](https://github.com/thelilylang/lily/pull/669)
- Add VecBit type [#671](https://github.com/thelilylang/lily/pull/671)
- Add `out.ci` and `out.lily` to `.gitignore` [#673](https://github.com/thelilylang/lily/pull/673)
- Validate non null expression [#678](https://github.com/thelilylang/lily/pull/678)

### Changed

- Improve scan of `__attribute__((x))` [#649](https://github.com/thelilylang/lily/pull/649)
- Move scan of multi keywords in the parser [#652](https://github.com/thelilylang/lily/pull/652)
- Disable Clang and GCC features [#658](https://github.com/thelilylang/lily/pull/658)
- Improve the resolution of custom value of enum variant [#662](https://github.com/thelilylang/lily/pull/662)
- Stop to give storage class to enum declaration [#668](https://github.com/thelilylang/lily/pull/668)
- Use fwrite instead of fprintf [#672](https://github.com/thelilylang/lily/pull/672)
- Let the OS store `stdio.h` and `stdlib.h` [#677](https://github.com/thelilylang/lily/pull/677)

### Fixed

- Fix case with paren inside `__attribute__` [#650](https://github.com/thelilylang/lily/pull/650)
- Fix some issues with preprocessor [#653](https://github.com/thelilylang/lily/pull/653)
- Fix bad parsing of dot and arrow access [#654](https://github.com/thelilylang/lily/pull/654)
- Fix leak when macro param is passed to unused macro param [#655](https://github.com/thelilylang/lily/pull/655)
- Fix infinite loop while parse post data type [#657](https://github.com/thelilylang/lily/pull/657)
- Fix bug on direct declaration of struct/union in typedef declaration [#660](https://github.com/thelilylang/lily/pull/660)
- Fix else if statement [#664](https://github.com/thelilylang/lily/pull/664)
- Limit the resolution of left or right token of `##` [#666](https://github.com/thelilylang/lily/pull/666)
- Fix bad order of declaration generation [#670](https://github.com/thelilylang/lily/pull/670)
- Fix few leaks during typechecking of path access [#675](https://github.com/thelilylang/lily/pull/675)
- Fix write file regression [#676](https://github.com/thelilylang/lily/pull/676)

### Removed

- Remove `benchmarks` directory [#674](https://github.com/thelilylang/lily/pull/674)

## 0.6.0 - 2024-10-22

### Added

- Add C standard stdlib header [#559](https://github.com/thelilylang/lily/pull/559)
- Add variadic parameter support to function [#565](https://github.com/thelilylang/lily/pull/565)
- Add stdarg function builtins [#567](https://github.com/thelilylang/lily/pull/567)
- Add `__builtin_va_copy` function builtin [#568](https://github.com/thelilylang/lily/pull/568)
- Add C standard stdarg header [#571](https://github.com/thelilylang/lily/pull/571)
- Add C standard stdio header [#572](https://github.com/thelilylang/lily/pull/572)
- Add C standard stdbool header [#573](https://github.com/thelilylang/lily/pull/573)
- Add ability to discard function call value [#574](https://github.com/thelilylang/lily/pull/574)
- Add the ability to disable use of fork in the test library [#581](https://github.com/thelilylang/lily/pull/581)
- Add `-fsanitize=address` to link options [#582](https://github.com/thelilylang/lily/pull/582)
- Add new tests to Vec type [#583](https://github.com/thelilylang/lily/pull/583)
- Add new tests to HashMap type [#585](https://github.com/thelilylang/lily/pull/585)
- Add new tests to Str (char*) type [#587](https://github.com/thelilylang/lily/pull/587)
- Add `-f`/`--file` option to CI CLI [#588](https://github.com/thelilylang/lily/pull/588)
- Add scientific notation support to `atof__*` function [#590](https://github.com/thelilylang/lily/pull/590)
- Add `--standard`/`-s` option to CI CLI [#594](https://github.com/thelilylang/lily/pull/594)
- Add `--include`/`-I` option to CI CLI [#596](https://github.com/thelilylang/lily/pull/596)
- Add `--include0` option to CI CLI [#599](https://github.com/thelilylang/lily/pull/599)
- Add some new tests to CI [#604](https://github.com/thelilylang/lily/pull/604)
- Typecheck union data type [#605](https://github.com/thelilylang/lily/pull/605)
- Add some new tests to CI [2] [#609](https://github.com/thelilylang/lily/pull/609)
- Add C standard string header [#610](https://github.com/thelilylang/lily/pull/610)
- Add C standard ctype header [#611](https://github.com/thelilylang/lily/pull/611)
- Typecheck goto statement [#618](https://github.com/thelilylang/lily/pull/618)
- Add some new tests to CI [3] [#620](https://github.com/thelilylang/lily/pull/620)
- Add some new tests to CI [4] [#622](https://github.com/thelilylang/lily/pull/622)
- Resolve array access in expression access (e.g. ->, .) [#627](https://github.com/thelilylang/lily/pull/627)
- Implement all data type qualifiers [#632](https://github.com/thelilylang/lily/pull/632)
- Add a check for data type qualifier [#636](https://github.com/thelilylang/lily/pull/636)
- Add \__gnuc_va_list support to `stdarg.h` [#641](https://github.com/thelilylang/lily/pull/641)
- Add stdnoreturn header for C standard library [#642](https://github.com/thelilylang/lily/pull/642)
- Scan `~` and `~=` token [#643](https://github.com/thelilylang/lily/pull/643)
- Add some checks to data type contexts [#645](https://github.com/thelilylang/lily/pull/645)

### Changed

- Allow void cast from all types [#575](https://github.com/thelilylang/lily/pull/575)
- Allow to implicit cast from array to struct (vice versa) [#576](https://github.com/thelilylang/lily/pull/576)
- Improve test library [#580](https://github.com/thelilylang/lily/pull/580)
- Improve installation script [#586](https://github.com/thelilylang/lily/pull/586)
- Update `BUILD.md` to add steps for FreeBSD [#591](https://github.com/thelilylang/lily/pull/591)
- Use `c2x` instead of `c23` as possible values for the `standard` option [#595](https://github.com/thelilylang/lily/pull/595)
- Display expected value per option in the help [#597](https://github.com/thelilylang/lily/pull/597)
- Update `scripts/compare_ci.sh` according the new CLI options [#601](https://github.com/thelilylang/lily/pull/601)
- Make the warning preprocessor valid in all standards [#603](https://github.com/thelilylang/lily/pull/603)
- Allow implicit cast from integer to float or vice versa [#606](https://github.com/thelilylang/lily/pull/606)
- Make `bool` implicitly castable as an integer [#607](https://github.com/thelilylang/lily/pull/607)
- Generate `_Bool` when standard is less than C23 [#608](https://github.com/thelilylang/lily/pull/608)
- Allow to discard more expressions [#612](https://github.com/thelilylang/lily/pull/612)
- Allow to pass c23 instead of c2x as value of standard option [#617](https://github.com/thelilylang/lily/pull/617)
- Assume that a `string` literal is an array of char [#619](https://github.com/thelilylang/lily/pull/619)
- Allow to use storage class inside function [#621](https://github.com/thelilylang/lily/pull/621)
- Allow multiple expressions to `for` init clause [#623](https://github.com/thelilylang/lily/pull/623)
- Update `BUILD.md` [#624](https://github.com/thelilylang/lily/pull/624)
- Return 0 when a non-defined macro is encountered [#625](https://github.com/thelilylang/lily/pull/625)
- Use `bits/libc-header-start.h` header in `stdio.h` [#628](https://github.com/thelilylang/lily/pull/628)
- Update examples [#629](https://github.com/thelilylang/lily/pull/629)
- Update documentations [#630](https://github.com/thelilylang/lily/pull/630)
- Update `README.md` [#631](https://github.com/thelilylang/lily/pull/631)
- Allow different syntaxes to data type qualifier [#633](https://github.com/thelilylang/lily/pull/633)
- Define `_Bool` when using the C89 standard [#640](https://github.com/thelilylang/lily/pull/640)

### Fixed

- Fix bug in struct or union type name generation [#561](https://github.com/thelilylang/lily/pull/561)
- Fix: Generate storage class for variable declaration [#562](https://github.com/thelilylang/lily/pull/562)
- Fix: Stop to pre-calculate size or alignment of custom type [#560](https://github.com/thelilylang/lily/pull/560)
- Fix a segfault when the config path could not be found [#563](https://github.com/thelilylang/lily/pull/563)
- Fix: Check the number of macro parameters if there are none [#564](https://github.com/thelilylang/lily/pull/564)
- Fix a segmentation fault when no expression was passed to the variable when checking the expression of the variable declaration [#566](https://github.com/thelilylang/lily/pull/566)
- Fix leak when declaration is recursive (struct, ...) [#569](https://github.com/thelilylang/lily/pull/569)
- Fix leak when the visit waiting list got collision [#570](https://github.com/thelilylang/lily/pull/570)
- Fix condition of statement typecheck [#577](https://github.com/thelilylang/lily/pull/577)
- Fix potential bug in `for` statement typecheck [#578](https://github.com/thelilylang/lily/pull/578)
- Fix potential bug in for statement typecheck [2] [#579](https://github.com/thelilylang/lily/pull/579)
- Fix some leaks during Lily compilation [#584](https://github.com/thelilylang/lily/pull/584)
- Fix `scripts/compare_ci.sh` [#589](https://github.com/thelilylang/lily/pull/589)
- Fix C suffixes integer [#592](https://github.com/thelilylang/lily/pull/592)
- Fix build when `LILY_BUILD_LLVM` is on [#593](https://github.com/thelilylang/lily/pull/593)
- Fix: Get absolute directory from include CLI option value [#598](https://github.com/thelilylang/lily/pull/598)
- Fix parser never jump in else preprocessor [#600](https://github.com/thelilylang/lily/pull/600)
- Fix lack of standardization of `signed char` keyword [#602](https://github.com/thelilylang/lily/pull/602)
- Fix segfault during typecheck of for init clause [#613](https://github.com/thelilylang/lily/pull/613)
- Fix bug in typecheck of function body item [#614](https://github.com/thelilylang/lily/pull/614)
- Fix bug in generation of for init clause semicolon [#615](https://github.com/thelilylang/lily/pull/615)
- Fix bad generation of post/pre dec/increment operator [#616](https://github.com/thelilylang/lily/pull/616)
- Fix unary operator parsing [#626](https://github.com/thelilylang/lily/pull/626)
- Fix unknown typedef while using `stdio.h` on Ubuntu [#635](https://github.com/thelilylang/lily/pull/635)
- Fix a wrong order of generation of data type qualifiers [#637](https://github.com/thelilylang/lily/pull/637)
- Fix extra space after data type generation [#638](https://github.com/thelilylang/lily/pull/638)
- Fix some bugs in parsing of typedef declaration [#639](https://github.com/thelilylang/lily/pull/639)
- Fix macro scan when encountering a comment line [#644](https://github.com/thelilylang/lily/pull/644)

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
