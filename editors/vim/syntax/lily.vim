" Vim syntax file
" Language: Lily
" Maintainer: ArthurPV

if exists("b:current_syntax")
  finish
endif
let b:current_syntax = "lily"

syn keyword lilyStorage pub object mut global val
syn keyword lilyStatement break next 
syn keyword lilyConditional if elif else match
syn keyword lilyRepeat while for

" syn keyword lilyModuleDefault

syn keyword lilyConstant nil undef
syn keyword lilyKeyword fun end or and not xor xor= not= in import class do self trait record enum return type alias error module begin as impl none test typeof cast asm macro package when req unsafe comptime inherit ref drop trace get set use include lib defer await
syn keyword lilyException try catch raise
syn keyword lilyType Bool Int8 Int16 Int32 Int64 Int128 Uint8 Uint16 Uint32 Uint64 Uint128 Float32 Float64 Str Char Unit Usize Isize Never Object Self CShort CUshort CInt CUint CLong CUlong CLonglong CUlonglong CStr CVoid CFloat CDouble

syn keyword lilyBoolean true false

syn match lilyOperator display "\V\:[-+/*=^&?|!><%~]"

syn match lilyDecNumber display   "\v<\d%(_?\d)*%(\.\.@!)?%(\d%(_?\d)*)?%([eE][+-]?\d%(_?\d)*)?"
syn match lilyHexNumber display "\v<0x\x%(_?\x)*%(\.\.@!)?%(\x%(_?\x)*)?%([pP][+-]?\d%(_?\d)*)?"
syn match lilyOctNumber display "\v<0o\o%(_?\o)*"
syn match lilyBinNumber display "\v<0b[01]%(_?[01])*"

syn match lilyClassItem display "\v\@[a-z]%(_?[a-z])*"
syn match lilyStaticClassItem display "\v\@\@[a-z-A-Z]%(_?[a-z-A-Z])*"
syn match lilyMacroCall display "\v#[a-z]%(_?[a-z])*"
syn match lilyFunctionId display "\v[a-z-A-Z]%(_?[a-z-A-Z])*\("

syn match lilyCharacterInvalid display contained /b\?'\zs[\n\r\t']\ze'/
syn match lilyCharacterInvalidUnicode display contained /b'\zs[^[:cntrl:][:graph:][:alnum:][:space:]]\ze'/
syn match lilyCharacter /b'\([^\\]\|\\\(.\|x\x\{2}\)\)'/ contains=lilyEscape,lilyEscapeError,lilyCharacterInvalid,lilyCharacterInvalidUnicode
syn match lilyCharacter /'\([^\\]\|\\\(.\|x\x\{2}\|u\x\{4}\|U\x\{6}\)\)'/ contains=lilyEscape,lilyEscapeUnicode,lilyEscapeError,lilyCharacterInvalid

"syn region lilyBlock start="{" end="}" transparent fold

" syn match lilyCommentLineDocTag /b'@'/ contains=lilyCommentLineDocItem

syn region lilyCommentLine start="//" end="$" contains=lilyTodo,@Spell
syn region lilyCommentLineDoc start="//[/!]/\@!" end="$" contains=lilyTodo,@Spell ",lilyCommentLineDocTag

" TODO: match only the first '\\' within the lilyMultilineString as lilyMultilineStringPrefix
syn match lilyMultilineStringPrefix display contained /c\?\\\\/
syn region lilyMultilineString start="c\?\\\\" end="$" contains=lilyMultilineStringPrefix

syn keyword lilyTodo contained TODO

syn match     lilyEscapeError   display contained /\\./
syn match     lilyEscape        display contained /\\\([nrt\\'"]\|x\x\{2}\)/
syn match     lilyEscapeUnicode display contained /\\\(u\x\{4}\|U\x\{6}\)/
syn region    lilyString      start=+c\?"+ skip=+\\\\\|\\"+ end=+"+ oneline contains=lilyEscape,lilyEscapeUnicode,lilyEscapeError,@Spell

hi def link lilyDecNumber lilyNumber
hi def link lilyHexNumber lilyNumber
hi def link lilyOctNumber lilyNumber
hi def link lilyBinNumber lilyNumber

hi def link lilyKeyword Keyword
hi def link lilyException Exception
hi def link lilyType Type
hi def link lilyCommentLine Comment
" hi def link lilyCommentLineDocItem PreProc
" hi def link lilyCommentLineDocTag PreProc
hi def link lilyCommentLineDoc SpecialComment
hi def link lilyTodo Todo
hi def link lilyString String
hi def link lilyMultilineString String
hi def link lilyMultilineStringContent String
hi def link lilyMultilineStringPrefix Comment
hi def link lilyCharacterInvalid Error
hi def link lilyCharacterInvalidUnicode lilyCharacterInvalid
hi def link lilyCharacter Character
hi def link lilyEscape Special
hi def link lilyEscapeUnicode lilyEscape
hi def link lilyEscapeError Error
hi def link lilyBoolean Boolean
hi def link lilyConstant Constant
hi def link lilyNumber Number
hi def link lilyOperator Operator
hi def link lilyStorage StorageClass
hi def link lilyStatement Statement
hi def link lilyConditional Conditional
hi def link lilyRepeat Repeat
hi def link lilyClassItem StorageClass
hi def link lilyStaticClassItem StorageClass
hi def link lilyFunctionId Identifier
hi def link lilyMacroCall Macro
" hi def link lilyModuleDefault PreProc
