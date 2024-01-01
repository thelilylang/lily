# Lily

## Caracteristic of the language 

Paradigms:
- Functional
- Object oriented
- Contract oriented
- Modular
- Imperative
- Concurrent
- Multi-paradigm

Typing:
- Static
- Strong
- Inferred
- Memory safe
- Safe type

Philosophy:
- Simplicity
- Two ways to program (Object v Functional) (e.g. you cannot use `.` (reserved to object programming) to chain function in Functional Lily, you must only use `|>`)
- Performance
- Low level programming access
- Safe memory and safe type
- No (explicit) lifetime

## Data types

### Primitive types

| Type    | Description                                        |
|---------|--------------------------------------------------- |
| Int8    | A signed 8-bit integer                             |
| Int16   | A signed 16-bit integer                            |
| Int32   | A signed 32-bit integer                            |
| Int64   | A signed 64-bit integer                            |
| Int128  | A signed 128-bit integer                           |
| Isize   | A signed pointer sized integer                     |
| Uint8   | An unsigned 8-bit integer                          |
| Uint16  | An unsigned 16-bit integer                         |
| Uint32  | An unsigned 32-bit integer                         |
| Uint64  | An unsigned 64-bit integer                         |
| Uint128 | An unsigned 128-bit integer                        |
| Usize   | An unsigned pointer sized integer                  |
| Float32 | A 32-bit floating point                            |
| Float64 | A 64-bit floating point                            |
| Bool    | Like `i1` in LLVM IR (`0` or `1`)                  |
| Never   | Like `_Noreturn` in C                              |
| Unit    | Like `void` in C                                   |
| Any     | Can take any type (to be used only in unsafe mode) |

### C data types

| Type       | Description                    |
|------------|--------------------------------|
| CShort     | Like `short` in c              |
| CUshort    | Like `unsigned short` in c     |
| CInt       | Like `int` in c                |
| CUint      | Like `unsigned int` in c       |
| CLong      | Like `long` in c               |
| CUlong     | Like `unsigned long` in c      |
| CLonglong  | Like `long long` in C          |
| CUlonglong | Like `unsigned long long` in C |
| CFloat     | Like `float` in C              |
| CDouble    | Like `double` in C             |
| CStr       | Like `char*` in C              |
| CVoid      | Like `void` in C               |

### Other 

| Type       | Description                          |
|------------|------------------------------------- |
| Str        | A string slice                       |
| Bytes      | A string (Uint8) slice               |
| Self       | Like `Self` in Rust                  |
| Object     | Used to talk about child of `Self`   |
| Char       | A single character                   |
| Byte       | A single character (Uint8)           |

## Function

NOTE: You cannot overload function in local scope of a function (e.g. lambda).

```lily
fun add(x, y) = x + y end
fun sub(x, y) = x - y end
fun mul(x, y) = x * y end
fun div(x, y) = x / y end
```

### `<dt>(...)` data type

```lily
fun add(x, y Int32(...2)) =
    // ...
end
```

## Module

```lily
module X =
    // ...
end
```

## Type

### Record

```lily
type Person record =
    name Str;
    age Uint8;
end
```

### Enum

```lily
type Level enum =
    A;
    B;
    C;
end
```

### Alias

```lily
type PersonName alias = Str;
```

## Object

### Class

```lily
object Person class =
    val name Str :: get, set;
    val age Uint8 :: get, set;

    pub fun new(name Str, age Uint8) =
        @.name := name
        @.age := age
    end
end
```

You can add an instruction to generate a `get` or `set` or both for this property.

```lily
val name Str := "Hello" :: get, set;
```

Create static property with `global val`.

```lily
global val name Str
```

#### close

> The `close` keyword means that the class or trait cannot be added as an inheritance to another class.

```lily
// ...
close object Cat inherit Animal in class =
end
```

### Object type

> The `Object` type is used to talk about any objects.

```lily
fun add(x Object) = return x; end
```

> The `Self.Object` type is used to talk about any objects implemented or inherited in the current object.

```lily
// ...
object Cat inherit Animal in class =
    fun get_animal(x Self.Object) = return x; end
end
```

### Self type

> The `Self` type is used to talk about self definition.

```lily
Self.call()
```

### Trait

```lily
object Sum trait =
    fun sum(Int32) Int32;
end
```

### Record Object

```lily
object Human trait =
    val name Str;

    fun get_name() Str;
end

object impl Human in Work record =
    name Str;
end

// Implements some function
pub fun@Person get_name(self) = self.name;

pub fun@Person get_age(self) = self.age;

pub fun@Person set_name(mut self, name) = self.name = name;

pub fun@Person set_age(mut self, age) = self.age = age;

pub fun@Person@Human get_name(self) = self.@Human.name; // can use object accessor in name

pub fun@Person get_name(self) = self.name;

pub fun@Person new(name Str, work_name Str) =
    @.@Human.name := name
    @.name := work_name
end
```

## Enum object

```lily
object Letter enum =
    A;
    B;
    C;
    D;
end

pub fun@Letter to_string(ref self) =
    match ref self do
        A:$ => "A";
        B:$ => "B";
        C:$ => "C";
        D:$ => "D";
    end
end
```

## What is the difference between Class and Record Object?

The class can take an inheritance and an other class can take this class in inheritance, but the record cannot inherit of anything, but in other class can take it in inheritance. Can only take a trait as implemenetation.

## Implements a way to `derive` an enum or struct like in Rust, OCaml or Haskell

I think it's important to implement that in record object, enum object or class, because it's an avandage to implement that in a programming language.

```lily
@+Derive("Eq")
object App class =
end
```

## Preprocess

### Testing

```
Test
Skip
```

### Diagnostics

```
Allow
Warn
Deny
Forbid
Deprecated
MustUse
```

### ABI linking, symbols and FFI

```
Link
Repr
Main
NoOverload
```

### Code generation

```
Align
AlwaysInline
Builtin
Cold
Hot
Immarg
NoAlias
NoBuiltin
NoCallback
NoInline
NonNull
NoRecurse
NoReturn
NoSync
NoUnwind
ReadOnly
ReadNone
Speculatable
WriteOnly
WillReturn

Inline
TargetFeature
InstructionSet
```

### Documentation

```
Doc
```

### Preludes

```
NoBuiltin
NoStd
NoCore
NoSys
```

# System

```
Os
Arch
```

## Error

```lily
pub error BadArgument:Str;

pub fun err() = raise BadArgument("failed");

fun main =
    try do
        err()
    catch err do
        ()
    end 
end
```

## Macro

```lily
pub macro create_a_function($name id, $d dt) = {
    pub fun {|name|}(x {|d|}) = x;
};

create_a_function!(add1, Int32)
create_a_function!(add2, Int32)
create_a_function!(add3, Int32)
```

### Macro param types

- id -> identifier: `x`
- dt -> data type: `Int32`
- tk -> token
- tks -> tokens: `${x Int32}`
- stmt -> statement: `val x := 32;`
- expr -> expression: `x + 230`
- path -> path: `X.Y.Z`
- patt -> pattern: `_`
- block -> block: `@{ () }`

NOTE (rust compiler rules):

```
here are additional rules regarding the next token after a metavariable:
- expr and stmt variables may only be followed by one of: => , ;
- ty and path variables may only be followed by one of: => , = | ; : > [ { as where
- pat variables may only be followed by one of: => , = | if in
- Other variables may be followed by any token.
```


## Module

```lily
module X do
end

// also

module X.D do
end
```

## Import

```lily
import "@std";
import "@std.io.*" as io;
```

### import with right to overwrite imported items

```lily
// useful when writing a prelude
import "@std.io.write"?;
```

## Use

```lily
import "@std.io.*" as io;

module X =
    use io.x.y.z; // like using namespace

    // z.
    // ...
end
```

## Include

```lily
module X =
end

module Y =
    include X
end
```

## Package

```lily
package =
    // [pub] .<identifier_normal.<identifier_normal... | identifier_string>
end
```

```lily
package =
    pub .a;
    .b;
    .c;
    .@"d";
end
```

## Constant

```lily
val A Int32 := 30;
pub val (A Int32, B Int32) := (340, 400);
```

## Contract

### When contract

#### On function

```lily
fun add(x, y) when [x < 0] = x + y;
// or
// fun add(x, y) when [x < 0] = x + y;
fun add(x, y) = raise InvalidArgument("error")
```

or

```lily
when $x > 0:
fun add(x, y) = x + y;

when $x < 0:
fun add(x, y) = y;

fun add(x, y) = 0;
```

NOTE: The dollar sign tells the compiler that the symbol it is looking for is a parameter so it must wait before analysis it.

#### On method (only in trait)

```lily
```

### Req contract

```lily
fun div(x, y) req [y not= 0] = x / y;
fun add(x, y) req [x < 0] = x + y;
```
### What is the difference between the keyword `when` and `req`?

If the `req` condition failed is emit an exception at compile time if is know the value in otherwise is emit a exception at runtime.

If the `when` condition failed is go to the next function (with the same name) until is find a condition with successfull condition. If is not find a sucessfull condition an error at compile time will emit. Also `when` can take overloading.

### Rule to resolve contexpr value at compile time

If the value provide to a literal expression, the resolving of contexpr at compile time is possible, but if the value provide to a function call or method call or other way, the compile time resolving will no be possible. In this case the compiler waiting to resolve this value at runtime.

### Rule to use `undef` value

The usage of `undef` value will be only using at initialization of value. In otherwise the compiler was emit an error to the base usage of `undef` value. 

### Rule to use `nil` value on Ptr

The usage of `nil` value will be only expected in case with unsafe operation. In other case the compiler will must use of optional value.

## Global access

```lily
fun call_it() = "hey";

module X.Y.Z =
    fun call_it() = "hey";
    fun hey() = global.call_it()
end
```

## Variable

```lily
fun main =
    val x Int32 := 30;
    val y Int32 := 100;
end
```

## Optional value

```lily
val x := ?30;
```

## Block raising

> With `!:` unary operator you can block the raising of value, to transform your exception data type in result data type.

```
Int32 raise Error become <Error>!Int32
```

```lily
match !:run() do
    @ok(_) => ();
    @err(_) => ();
end
```

## Result vs. Exception

> The difference with the exception is that the result does not propagate the error to other functions. Also Result is better in a situation where memory consumption is a concern. Moreover, when you don't specify an error type for result, it implicitly passes the Error type, which accepts all error types, so the compiler will infer on result's error types.

## Result operators

### !?

```
!?<expr>
```

> The exception becomes a result data type and unpacks the values to return an optional type. If the value is Ok, it returns that value directly, otherwise it returns none. 

example:

```
Ok(value) become value
Err(value) become none
```

### !!

```
!!<expr>
```

> The exception becomes a result data type and unwrap the Ok value, and if an error is caught, it is returned directly.

### !:

```
!:<expr>
```

> The exception becomes a result data type (or vice versa). So, if we have a function that returns an Int32 and raises an error, the result data type becomes a result (Error!Int32). 

### ?

```
?<expr>
```

> The Ok value is unwrap and the Error value becomes none.


### !

```
!<expr>
```

> The Ok value is unwrap and the error value is returned. 

## Typecheck

- Check class
- Check trait
- Check enum
- Check record
- Check function
- Check macro expands
- Check module
- Check name conflict
- Lookup for data type

### Check class

- Check implementation
- Check inheritance
- Check property conflict
- Check property data type
- Check method
- Check constructor

### Check implementation

```lily
object impl Add in Value class =
//          ^^^ -> check if is a trait
end
```

- Check if data type is a trait/exists.
- Check if methods are well implement.

### Check inheritance

```lily
object inherit Human in Worker class =
//             ^^^^^ -> check if is a class/record
end
```

- Check if data type is a (class/record)/exists.
- Check if in constructor the inherit class or inherit record is well constructed.

### Case with inherit and impl on one class

```lily
object inherit [Human, Abc] + impl [Debug, Def] in Worker class =
    // ...
end
``` 

### Check property conflict

```lily
object Foo class =
    val name Str;
//      ^^^^ -> check name
    val name Int32;
//      ^^^^ -> check name
end
```

### Check property data type

```lily
object Foo class =
    val name AA;
//           ^^ -> check data type
end
```

### Check method

```lily
object Foo class =
    val name Str;

    fun get_name(self) = self.name;
end
```

### Check constructor

Constructor is any time named `new`. Also you can overload the constructor.

```lily
object Foo class =
    fun new() =;
end

fun main() =
    val a Foo := Foo.new();
    drop: val b *Foo := Foo.new();
end
```

### Loop

```lily
fun main =
    for i in 0..10 do ();

    mut i := 0;
    while i < 10 do i += 1;
end
```

### Condition

```lily
fun main =
    mut a := 30;

    if a < 4 do
        a += 40
    elif a > 10 do
        a -= 5
    end
end
```

### Pattern matching

```lily
fun main =
    val x := true;
    val y := 10;

    match x do 
        true ? y > 10 => true;
        false ? y < 10 => true;
        _ => false
    end
end
```

### Array

Static array

```lily
fun main =
    val arr [3]Int32 := [1, 2, 3];
end
```

Undetermined array size.

- Variant size at compile time.
- Static size at runtime.

NOTE: It's like `va_arg` in C.

```lily
fun main =
    val arr [?]Int32 := [1, 2, 3, 4];
end
```

Dynamic array

```lily
fun main =
    drop: val arr [_]Int32 := [];
    Array.append(ref arr, [1, 2, 3]) // append new elements
end
```

Multi Pointers (array)

- Cannot dereference that

NOTE: only available in unsafe mode

```lily
fun main =
    val arr [*]Int32 := [1, 2, 3, 4, 5]; // [*]Int32 it same than Int32* in C
end
```

### List

```lily
// {<dt>}
```

```lily
fun main =
    val list {Int32} := {1, 2, 3, 4, 5}; // Create a simple list
    mut start ?*Int32 := none;
    mut @"end" ?*Int32 := none;

    match list do
        {s, .., e} => @{
            start = ref s
            @"end" = ref e
        }
        _ => ()
    end
end
```

### Tuple

```lily
fun main =
    val tuple (Int32, Int32) := (1, 2);
    val x := tuple#0;
end
```

### Lambda
```lily
fun main =
    val f := fun (x Int32) -> x;
end
```

## Compiler design

### Move or copy value by default?

I think `move` value is better than copy value because it avoid to write program with a poor efficently in memory.

### How to make pointer safe?

- Doesn't allow null value in safe mode
- Ref on pointer (copy of pointer (in fact is not a dropable pointer))

```lily
fun main =
    drop: val x *Int32 := Ptr.new(20); // you must specifie drop except when you precise to the compiler that the drops are automaticly manage by the compiler
    val y *Int32 := nil; // error in safe mode

    unsafe =
        val y *Int32 := nil; // ok in unsafe mode
        // The compiler emit an error if the Pointer is used with nil value
        // In the runtime the compiler verify if the pointer is null 
    end

    begin =
        val y *Int32 := Ptr.new(100);
        drop y // drop at this point
        // after this point the compiler shadows the reference of y in this scope
        drop y <~ Ptr.new(200) // reassign a drop ptr value
        // after this pointer the compiler enable the reference of y in this scope and drop the pointer at the end of this scope
    end

    // y was drop here
end
```

### How to make ref safe?

- Check if the value of the ref is available in this scope
- Doesn't allow null value in safe mode (such as pointer)
- Cannot drop a ref

```lily
fun add(x ref Int32) ref Int32 = x;

fun main =
    val x Int32 := 20;
    ref: val y Int32 := add(ref x); // Int32 -> ref Int32 = Int32* in C
    // the value (x) is available in this scope, so the value (y) is available
    // or
    // val y ref Int32 := add(ref x)
end
```

This syntax is better than C because the embiguity with the Pointer type is very dangerous, because the difference between `int*` (pass by reference) and `int*` (pass by pointer). The solution of the Lily language  permits to make difference between (pass by reference and pass by pointer).

### How to check if a ref is alive ?

- You cannot return a reference to a local variable.

```lily
fun add() ref Int32 =
    val x := 30;
    ref x
end
```

- You cannot return a reference to function parameter

```lily
fun add(x Int32) ref Int32 =
    ref x
end
```

- Verify if the reference value and the source value is available in the actual scope.

```lily
type Person record =
    name Str;
    age Uint8;
end

fun get_name(p ref Person) ref Str =
    ref p.name
end

fun main =
    val p := Person { name: "John", age: 20 };
    ref: val name := ref p |> get_name;
    // both reference values are available in the actual scope
end
```

### Tracing pointer ?

- Tracing pointer cannot be droped (or in other words if the pointer is already tracing you can't drop it)
- Simplely is like a reference on pointer
- Can be dereferenced

```lily
fun add(x trace *Int32) trace *Int32 = x;

fun main =
    val x *Int32 := Ptr.new(100);
    val y := add(trace x);
    // or
    val y2 := add('x);

    drop x;
end
```

### How to make pass by value safe?

- Move value

```lily
fun add(x Int32) = x;

fun main =
    val x Int32 := 30;
    val y Int32 := add(x); // the value of x is moved

    val z = x; // now you cannot move `x` in z variable because it has been move previously
end
```

## Cast

> The keyword cast is used to do 2 different actions. The first one is to be able to do type conversion on primaries. And the second is to access the parent or child class of a particular type.

```lily
val a := 30; // Int32
val b := a cast Int64;
```

```lily
object A class
    fun new() =
    end
end

object inherit A in B class
    fun new() =
    end
end

fun main =
    val b := B.new(); // B
    val a := b cast A; // A
    // or
    val a2 := b cast @B@A; // A
end
```

## Dereference

```lily
fun main =
    val a := 340;
    val b *Int32 := ref a;
    val c := b.*;
end
```

## Lib

You can create extern function.

```lily
@+Link("math")
lib Math =
    val PI Float64;
end
```

## Unsafe

- You can use raw pointer (*T)
- You can use nil value on pointer
- You can use Any data type
- `auto-drop` is disabled

## Safe mode

- All pointers are traced
- Pass by reference or by trace
- All values are moved
- All values are automatically drop

## Comptime mode

```
comptime <stmt|expr>
```

## Generic

> Normal generic param

```
T
```

> Constraint generic param

```
T: Abc
```

> List generic param

```
T(...)
```

> List generic param + constraint

```
T(...): Abc
```

> You can also add a minimum of given data type

```
T(1...2)
T(...4)
T(4...)
```

## Operators

```
+
and
+=
&=
<<=
|=
>>=
/=
**=
%=
*=
-=
xor=
=
&
|
|>
/
==
**
>=
>
<<
<=
<
%
*
not=
or
>>
..
-
xor
->
<-
[n]
[n..n]
[n..]
[..n]
```

## @sys

```lily
@sys.write(1, "Hello\n", 6)
```

## @builtin

```lily
@builtin.max(2, 30)
```

## @len

```lily
@len("Hello")
@len(b"Hello")
@len([1, 2, 3])
@len({1, 2, 3})
```

## @hide, @hideout

> This is used to add more security at analysis time for dropping the pointer. For example, when we use the drop operator, the pointer variable is `@hideout`, so the variable is removed from scope, so analysis will report an error because it's not found in scope (this is to add more security to memory at compile-time like the borrow checker (cheaper), but it's optional). 

```
ms: Memory security
options e.g. --ms-0, --ms-1, --ms-2, ...
```

```lily
val x := 30;
// we can use @hideout for use hide outsite of the scope of the function (for ref or ptr).
@hideout(x)
// by default that's a local hide
@hide(x)

// relaod variable, at level 0, this is technically an assignment 
x <~ 30
```

## Garbage collector (interpreter)

- Concurrent
- Mark and sweep

## C mode

```lily
@cc =
    int add(int x, int y) {
        return x + y;
    }
@end
```

## C++ mode

```lily
@cpp =
    auto add(int y, int y) -> int {
        return x + y;
    }
@end
```

## Library organization

### Std

- Alloc
- Arch
- Collections
- Env
- Ffi
- Fmt
- Fs
- Future
- Io
- Net
- Ops
- Os
- Panic
- Path
- Process
- String
- Thread
- Time
- Vector

### Core

- Cmp
- Copy
- Int8
- Int16
- Int32
- Int64
- Isize
- Uint8
- Uint16
- Uint32
- Uint64
- Usize
- Float32
- Float64
- Fun
- Str
- Char
- BitStr
- BitChar
- Tuple
- Array
- Unit
- Pointer
- Ref
- Slice
- Bool
- Exception
- Optional
- Never

## List of at keywords

```
@cc
@cpp
@sys
@builtin
@hide
@hideout
@len
```

## List of keywords

```
alias
and
as
asm
async
await
begin
break
cast
catch
class
close
comptime
defer
do
drop
elif
else
end
enum
error
false
for
fun
get
global
if
impl
include
inherit
is
lib
macro
match
module
mut
next
nil
none
not
Object
object
or
package
pub
raise
record
ref
req
return
Self
self
set
test
threadlocal
trace
trait
true
try
type
undef
unsafe
use
val
when
while
xor
```
