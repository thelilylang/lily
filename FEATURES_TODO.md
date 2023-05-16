# The list of features to do

Examples of task definition

```
Compilation stage option:
scanner
preparser (if applicable)
precompiler (if applicable)
parser
analysis
codegen (IR)
linker (if applicable)

- [ ] Task name -> Compilation stage, issue (if opened) or PR (if opened)
- [x] Task name
```

## Data types

- [x] Int8
- [x] Int16
- [x] Int32
- [x] Int64
- [ ] Int128 -> parser
- [x] Isize
- [x] Uint8
- [x] Uint16
- [x] Uint32
- [x] Uint64
- [ ] Uint128 -> parser
- [x] Usize
- [x] Float32
- [x] Float64
- [x] Bool
- [x] Never
- [x] Unit
- [x] Any
- [x] CShort
- [x] CUshort
- [x] CInt
- [x] CUint
- [x] CLong
- [x] CUlong
- [x] CUlonglong
- [x] CFloat
- [x] CDouble
- [x] CStr
- [x] CVoid
- [x] Str
- [ ] Bytes -> codegen
- [ ] Self -> analysis
- [ ] Object -> analysis
- [x] Char
- [x] Byte

## Defined user data type

### Record

#### Analysis

- [x] Check field
- [ ] Check the visibility of the field data type (linkage) -> analysis
- [ ] Support of recursive data type -> analysis
- [ ] Support of generic record -> analysis

#### Codegen (LLVM)

- [x] Support of basic record
- [ ] Support of recursive data type -> codegen
- [ ] Support of generic record -> codegen
- [ ] Add type linkage -> codegen

### Enum

#### Analysis

- [ ] Check variant
- [ ] Check the visibility of the variant data type (linkage) -> analysis
- [ ] Support of recursive data type -> analysis
- [ ] Support of generic enum -> analysis

#### Codegen (LLVM)

- [ ] Support of basic enum -> codegen
- [ ] Support of recursive data type -> codegen
- [ ] Support of generic enum -> codegen
- [ ] Add type linkage -> codegen

### Class

#### Analysis

- [ ] Check attribute
- [ ] Check method
- [ ] Check the visibility of the attribute data type (linkage) -> analysis
- [ ] Check the visibility of the method data type (linkage) -> analysis
- [ ] Support of recursive data type -> analysis
- [ ] Check implementation data type -> analysis
- [ ] Check inheritance data type -> analysis
- [ ] Check attribute initialization class -> analysis

#### Codegen (LLVM)

- [ ] Support of basic class -> codegen
- [ ] Support of recursive data type -> codegen
- [ ] Add type linkage -> codegen
- [ ] Support of generic class -> codegen

### Trait

#### Analysis

- [ ] Check method
- [ ] Check the visibility of the method data type (linkage) -> analysis
- [ ] Support of recursive data type -> analysis

### Record object

#### Analysis

- [ ] Check field
- [ ] Check the visibility of the field data type (linkage) -> analysis
- [ ] Support of recursive data type -> analysis
- [ ] Support of generic record -> analysis
- [ ] Check method
- [ ] Check implementation data type -> analysis

#### Codegen (LLVM)

- [ ] Support of basic record object -> codegen
- [ ] Support of recursive data type -> codegen
- [ ] Support of generic record object -> codegen
- [ ] Add type linkage -> codegen

### Enum object

#### Analysis

- [ ] Check variant
- [ ] Check the visibility of the variant data type (linkage) -> analysis
- [ ] Support of recursive data type -> analysis
- [ ] Support of generic enum -> analysis
- [ ] Check method
- [ ] Check implementation data type -> analysis

#### Codegen (LLVM)

- [ ] Support of basic enum object -> codegen
- [ ] Support of recursive data type -> codegen
- [ ] Support of generic enum object -> codegen
- [ ] Add type linkage -> codegen

### Constant

#### Analysis

- [x] Check expression data type -> analysis
- [x] Check expression value -> analysis

#### Codegen (LLVM)

- [x] Support of basic constant -> codegen
- [x] Support of constant expression -> codegen

### Function

#### Analysis

