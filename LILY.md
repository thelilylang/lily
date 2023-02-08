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

Phylosophy:
- Simplicity
- Two ways to program (Object v Functional) (e.g. you cannot use `.` (reserved to object programming) to chain function in Functional Lily, you must only use `|>`)
- Performance
- Low level programming access
- Safe memory and safe type
- No (explicit) lifetime

## Function

```lily
fun add(x, y) = x + y;
fun sub(x, y) = x - y;
fun mul(x, y) = x * y;
fun div(x, y) = x / y;
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
    name Str,
    age Uint8
end
```

### Enum

```lily
type Level enum =
    A,
    B,
    C
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
    @name Str;
    @age Uint8;

    pub fun get_name(self) = self.name;
    pub fun get_age(self) = self.age;
    pub fun set_name(mut self, name Str) = self.name = name;
    pub fun set_age(mut self, age Uint8) = self.age = age;

    pub fun new(name Str, age Uint8) =
        @.name = name
        @.age = age
    end
end
```

Create static property with `@@`.

```lily
@@name Str
```

### Object type

> The `Object` type is used to talk about child definition. That's very useful on specific trait. NOTE: In general trait Object is not recommented to use.

### Self type

> The `Self` type is used to talk about self definition.

### Trait

```lily
object Sum trait =
    fun sum(Int32) Int32;
end
```

### Record Object

```lily
object Human trait =
    @name Str;

    fun get_name() Str;
end

object impl Human in Work record =
    name Str
end

// Implements some function
pub fun@Person get_name(self) = self.name;

pub fun@Person get_age(self) = self.age;

pub fun@Person set_name(mut self, name) = self.name = name;

pub fun@Person set_age(mut self, age) = self.age = age;

pub fun@Person@Human get_name(self) = self.@Human.name; // can use object accessor in name

pub fun@Person get_name(self) = self.name;

pub fun@Person new(name Str, work_name Str) =
    .@Human.name = name
    .name = work_name
end
```

## Enum object

```lily
object Letter enum =
    A,
    B,
    C,
    D
end

pub fun@Letter to_string(self) =
    match self do
        A => "A",
        B => "B",
        C => "C",
        D => "D",
    end
end
```

## What is the difference between Class and Record Object?

The class can take an inheritance and an other class can take this class in inheritance, but the record cannot inherit of anything, but in other class can take it in inheritance. Can only take a trait as implemenetation.

## Implements a way to `derive` an enum or struct like in Rust, OCaml or Haskell

I think it's important to implement that in record object, enum object or class, because it's an avandage to implement that in a programming language.

```lily
@[derive(Eq)]
object App class =
end
```

## Error

```lily
pub error BadArgument(Str);

pub fun err() = raise BadArgument("failed");

fun main =
    try err() do
        ()
    // or
    // catch err do
    //  ()
    // end
    catch err => do
        BadArgument(s) => (),
        _ => ()
    end
end
```

## Macro

```lily
pub macro create_a_function($name id, $d dt) =
    pub fun {{name}}(x {{d}}) = x;
end

#create_a_function(add1, Int32)
#create_a_function(add2, Int32)
#create_a_function(add3, Int32)
```

### Macro param types

- id -> identifier: `x`
- dt -> data type: `Int32`
- tt -> token
- stmt -> statement: `val x := 32`
- expr -> expression: `x + 230`
- path -> path: `X.Y.Z`
- patt -> pattern: `_`
- block -> block: `{ () }`

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
import "std"
import "std.io.*" as io
```

## Package

```lily
// By default the compiler search in `.` path
package lily

package .abc // create a sub-package
```

## Constant

```lily
A Int32 := 30;
```

## Contract

### When contract

#### On function

```lily
fun add(x, y) when x < 0 = x + y;
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
fun div(x, y) req y not= 0 = x / y;
// or
// fun div(x, y) req [y not= 0] = x / y;
fun add(x, y) req x < 0 = x + y;
// or
// fun add(x, y) req [x < 0] = x + y;
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
    val x Int32 := 30
    val y Int32 := 100
end
```

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

### Check property conflict

```lily
object Foo class =
    @name Str;
//   ^^^^ -> check name
    @name Int32;
//   ^^^^ -> check name
end
```

### Check property data type

```lily
object Foo class =
    @name AA;
//        ^^ -> check data type
end
```

### Check method

```lily
object Foo class =
    @name Str;

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
    val a Foo := Foo.new()
    drop val b Foo* := Foo.new()
end
```

### Loop

```lily
fun main =
    for i in 0..10 do ();

    mut i := 0
    while i < 10 do i += 1;
end
```

### Condition

```lily
fun main =
    mut a := 30

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
    val x := true
    val y := 10

    match x do
        true ? y > 10 => true,
        false ? y < 10 => true,
        _ => false
    end
end
```

### Array

Static array

```lily
fun main =
    val arr [3]Int32 := [1, 2, 3]
end
```

Undetermined array size.

- Variant size at compile time.
- Static size at runtime.

NOTE: It's like `va_arg` in C.

```lily
fun main =
    val arr [?]Int32 := [1, 2, 3, 4]
end
```

Dynamic array

```lily
fun main =
    drop val arr [_]Int32 := []
    Array.append(ref arr, [1, 2, 3]) // append new elements
end
```

Multi Pointers (array)

- Cannot dereference that

NOTE: only available in unsafe mode

```lily
fun main =
    val arr [*]Int32 := [1, 2, 3, 4, 5] // [*]Int32 it same than Int32* in C
end
```

### Tuple

```lily
fun main =
    val tuple (Int32, Int32) := (1, 2)
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
    drop val x Int32* := Ptr.new(20) // you must specifie drop except when you precise to the compiler that the drops are automaticly manage by the compiler
    val y Int32* := nil // error in safe mode

    unsafe =
        val y Int32* := nil // ok in unsafe mode
        // The compiler emit an error if the Pointer is used with nil value
        // In the runtime the compiler verify if the pointer is null 
    end

    begin =
        val y Int32* := Ptr.new(100)
        drop y // drop at this point
        // after this point the compiler shadows the reference of y in this scope
        drop y <- Ptr.new(200) // reassign a drop ptr value
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
    val x Int32 := 20
    ref val y Int32 := add(ref x) // Int32 -> ref Int32 = Int32* in C
    // the value (x) is available in this scope, so the value (y) is available
    // or
    // val y ref Int32 := add(&x)
end
```

This syntax is better than C because the embiguity with the Pointer type is very dangerous, because the difference between `int*` (pass by reference) and `int*` (pass by pointer). The solution of the Lily language  permits to make difference between (pass by reference and pass by pointer).

### How to check if a ref is alive ?

- You cannot return a reference to a local variable.

```lily
fun add() ref Int32 =
    val x := 30
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
    name Str,
    age Uint8
end

fun get_name(p ref Person) ref Str =
    ref p.name
end

fun main =
    val p := Person { name: "John", age: 20 }
    ref val name := ref p |> get_name 
    // both reference values are available in the actual scope
end
```

### Tracing pointer ?

- Tracing pointer cannot be droped (or in other words if the pointer is already tracing you can't drop it)
- Simplely is like a reference on pointer
- Can be dereferenced

```lily
fun add(x trace Int32*) trace Int32* = x;

fun main =
    val x Int32* := Ptr.new(100)
    trace val y Int32* := add(trace x)
end
```

### How to make pass by value safe?

- Move value

```lily
fun add(x Int32) = x;

fun main =
    val x Int32 := 30
    val y Int32 := add(x) // the value of x is moved

    val z = x // now you cannot move `x` in z variable because it has been move previously
end
```

## Auto-drop

Compiler option:
- allow-auto-drop

When you pass `allow-auto-drop` as compile option, you can disable auto drop in a specific section of code with preprocess `disable_auto_drop`.

```lily
@[disable_auto_drop]
fun main =
end
@[end]
```

## Garbage collector (interpreter)

- Concurrent
- Mark and sweep

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
