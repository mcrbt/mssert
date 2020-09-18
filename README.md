# mssert


## Description

`mssert` is a single ANSI C header file basically defining an *assert* macro.
Though working equally on modern desktop machines, it is intended to allow
*unit testing* for embedded systems.

The header file is not only ANSI C compliant, but does not have any
dependencies, not even a C standard library implementation. `mssert.h` itself
does not include anything.

`mssert` tries to find a good trade-off between small code footprint, small
compiled object code size, minimal runtime space requirements, while being as
fast as possible. One reason for its efficiency is doing as few things as
possible.

As each system and embedded device toolchain offers a different set of output
and debugging options, the
**way of handling an assertion failure is left to the developer**.

`mssert` can be utilized by both, C and C++ projects.


## Usage

For using `mssert` the preprocessor macro `MSSERT` *must* be defined, and
the macro `NDEBUG` *must not* be defined at compile time.

There are four relevant macros defined for use by the (embedded) target
software:

- `ms_assert`
- `ms_success`
- `ms_abort`
- `ms_reset`

In addition, a message buffer `mssert_buffer` is available to the developer.


### `ms_assert`

`ms_assert(E)` is the *assertion macro* of `mssert`. It takes an arbitrary
expression `E` as argument. If the assertion succeeds, i.e. `E` evaluates to
`true`, or unequals `0`, respectively, the only overhead of calling `ms_assert`
is *one comparison*, but not even a function call.

The overhead is much bigger if the assertion fails. In that case

1. the *fail counter* is incremented,
2. the message buffer is filled with the source code line of the failed
   assertion and the verbal representation of the passed expression, and
3. whatever is defined for the `ms_abort` macro is executed.

(For details on the message buffer see section
[`mssert_buffer`](https://github.com/mcrbt/mssert#mssert_buffer)
below.)


### `ms_success`

The `ms_success` macro can be considered and use as a boolean variable that is
`false` (`0`) if at least one assertion failed by the time of using the macro.

`ms_success` simply negates the *fail counter*.

For instance, `ms_success` can be used like this in C++ code:

```c++
std::cout << (ms_success ? "success" : "failure") << std::endl;
```


### `ms_abort`

In order to provide the necessary degree of flexibility it is completely up to
the developer how to handle failed asserts: The `ms_abort` macro can be
(*and has to be*(!)) freely defined **before including the header file**
`mssert.h`. If the macro terminates the execution its use is related to that of
an *exit handler*.

In C code for desktop machines `ms_abort` could be something like:

```c
#include <stdio.h>
#include <stdlib.h>

#define ms_abort fprintf(stderr, "%s\n", mssert_buffer); exit(EXIT_FAILURE);
#include "include/mssert.h"
```

For embedded applications it may be sufficient to define `ms_abort` like this:

```c
#define ms_abort while(1);
#include <mssert.h>
```

Once the program stopped to continue the current *program counter* value may
give an indication of which assertion failed. In addition, an error LED may be
turned on and/or interrupts may be disabled, for instance.


### `ms_reset`

For convenience, `mssert` provides the macro `ms_reset` to reset the
*fail counter* and the message buffer. After calling `ms_reset` the `ms_success`
macro will evaluate to `true`.


### `mssert_buffer`

`mssert` defines a global `char` array `mssert_buffer` that can be used as
message buffer by the developer. Though providing only sparse information it
should give an idea of which assertion failed.

**The content of the message buffer is undefined until the first assertion fails.**

Once an assertion fails information regarding that assertion is written to the
buffer, in the form:

```
<line>: <expression>
```

where `<line>` is the source code location where the `ms_assert` macro was
called and `<expression>` is the verbal (string) representation of the
expression passed to `ms_assert`. Even if several source code files are compiled
the combination of line number and passed expression should be sufficient to
determine the correct input file in most cases.

The size of the buffer can be set at compile time by defining the macro
`MSSERT_BUFFER_SIZE` (the default is 32 bytes).
A buffer size of `0` is absolutely valid, in which case writing to the buffer is
never attempted.

No matter how long the generated information about the failed assertion is,
`mssert` will write **at most** `MSSERT_BUFFER_SIZE` characters to the buffer.


### `MSSERT_VERSION`

The `MSSERT_VERSION` macro provides the version of `mssert` as C-string. It can
be access by the target code once the header `mssert.h` is included.


## Implementation

To void the need of *any* dependencies, including a standard library
implementation (which may vary for embedded devices), two functions from the
string header `string.h` are "overridden":

- `strlen()`
- `strncpy()`

While the `strlen` implementation should be almost identical to those provided
in most standard library implementations, the `mssert` implemenation of
`strncpy` differs from others primarily by not filling up the buffer with `0`'s
if the source string is shorter than the buffer. Anyway a terminating `0` is
always written (though only one).
**Even the `ms_reset` macro writes only the first byte of the buffer.**

If the content of the buffer is always treated as C-string (i.e. `char *`) then
there is no drawback in this implementation but the advantage of letting the
function consume less CPU cycles.


## Example

The `example` subdirectory contains both, a C and a C++ source file providing a
usage example for `mssert`.

The sample sources can be built using the *Makefile*. The default target
compiles both examples producing the executables `cmssert` and `cppmssert` in
the root directory of the repository:

```
$ make
```

To remove the generated binary files the `clean` target can be run:

```
$ make clean
```


## Copyright

`mssert` - assertion header for embedded systems

Copyright &copy; 2020 Daniel Haase

Distributed under the **Boost Software License**, Version 1.0.


## License

```
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```
