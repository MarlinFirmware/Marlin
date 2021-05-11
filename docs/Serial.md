# Serial port architecture in Marlin

Marlin is targeting a plethora of different CPU architectures and platforms. Each of these platforms has its own serial interface.
While many provide a Arduino-like Serial class, it's not all of them, and the differences in the existing API create a very complex brain teaser for writing code that works more or less on each platform.

Moreover, many platform have intrinsic needs about serial port (like forwarding the output on multiple serial port, providing a *serial-like* telnet server, mixing USB-based serial port with SD card emulation) that are difficult to handle cleanly in the other platform serial logic.

Starting with version `2.0.9`, Marlin provides a common interface for its serial needs.

## Common interface

This interface is declared in `Marlin/src/core/serial_base.h`
Any implementation will need to follow this interface for being used transparently in Marlin's codebase.

The implementation was written to prioritize performance over abstraction, so the base interface is not using virtual inheritance to avoid the cost of virtual dispatching while calling methods.
Instead, the Curiously Recurring Template Pattern (**CRTP**) is used so that, upon compilation, the interface abstraction does not incur a performance cost.

Because some platform do not follow the same interface, the missing method in the actual low-level implementation are detected via SFINAE and a wrapper is generated when such method are missing. See the `CALL_IF_EXISTS` macro in `Marlin/src/core/macros.h` for documentation of this technique.

## Composing the desired feature
The different specificities for each architecture are provided by composing the serial type based on desired functionality.
In the `Marlin/src/core/serial_hook.h` file, the different serial feature are declared and defined in each templated type:
1. `BaseSerial` is a simple 1:1 wrapper to the underlying, Arduino compatible, `Serial`'s class. It derives from it. You'll use this if the platform does not do anything specific for the `Serial` object (for example, if an interrupt callback calls directly the serial **instance** in the platform's framework code, this is not the right class to use). This wrapper is completely inlined so that it does not generate any code upon compilation. `BaseSerial` constructor forwards any parameter to the platform's `Serial`'s constructor.
2. `ForwardSerial` is a composing wrapper. It references an actual Arduino compatible `Serial` instance. You'll use this if the instance is declared in the platform's framework and is being referred directly in the framework. This is not as efficient as the `BaseSerial` implementation since static dereferencing is done for each method call (it'll still be faster than virtual dispatching)
3. `ConditionalSerial` is working a bit like the `ForwardSerial` interface, but it checks a boolean condition before calling the referenced instance. You'll use it when the serial output can be switch off at runtime, for example in a *telnet* like serial output that should not emit any packet if no client is connected.
4. `RuntimeSerial` is providing a runtime-modifiable hooking method for its `write` and `msgDone` method. You'll use it if you need to capture the serial output of Marlin, for example to display the G-Code parser's output on a GUI interface. The hooking interface is setup via the `setHook` method.
5. `MultiSerial` is a runtime modifiable serial output multiplexer. It can output (*respectively input*) to 2 different interface based on a port *mask*. You'll use this if you need to output the same serial stream to multiple port. You can plug a `MultiSerial` to itself to duplicate to more than 2 ports.

## Plumbing
Since all the types above are using CRTP, it's possible to combine them to get the appropriate functionality.
This is easily done via type definition of the feature.

For example, to create a single serial interface with 2 serial outputs (one enabled at runtime and the other switchable):
```cpp
typedef MultiSerial< RuntimeSerial<Serial>, ConditionalSerial<TelnetClient> > Serial1Class;
```

To send the same output to 4 serial ports you could nest `MultiSerial` like this:
```cpp
typedef MultiSerial< MultiSerial< BaseSerial<Serial>, BaseSerial<Serial1> >, MultiSerial< BaseSerial<Serial2>, BaseSerial<Serial3>, 2, 1>, 0, 2> Serial1Class;
```
The magical numbers here are the step and offset for computing the serial port. Simplifying the above monster a bit:
```cpp
MS< A = MS<a, b, offset=0, step=1>, B=MS<c, d, offset=2, step=1>, offset=0, step=2>
```
This means that the underlying multiserial A (with output to `a,b`) is available from offset = 0 to offset + step = 1 (default value).
The multiserial B (with output to `c,d`) is available from offset = 2 (the next step from the root multiserial) to offset + step = 3.
In practice, the root multiserial will redirect any index/mask `offset` to `offset + step - 1` to its first leaf, and any index/mask `offset + step` to `offset + 2*step - 1` to its second leaf.

## Emergency parser
By default, the serial base interface provide an emergency parser that's only enable for serial classes that support it. Because of this condition, all underlying types take a first `bool emergencyParserEnabled` argument to their constructor. You must take into account this parameter when defining the actual type used.

## SERIAL macros
The following macros are defined (in `serial.h`) to output data to the serial ports:

| MACRO | Parameters | Usage | Example | Expected output |
|-------|------------|-------|---------|-----------------|
| `SERIAL_ECHO` | Any basic type is supported (`char`, `uint8_t`, `int16_t`, `int32_t`, `float`, `long`, `const char*`, ...). | For a numeric type it prints the number in decimal. A string is output as a string. | `uint8_t a = 123; SERIAL_ECHO(a); SERIAL_CHAR(' '); SERIAL_ECHO(' '); ` | `123 32` |
| `SERIAL_ECHOLN` | Same as `SERIAL_ECHO` | Do `SERIAL_ECHO`, adding a newline | `int a = 456; SERIAL_ECHOLN(a);` | `456\n` |
| `SERIAL_ECHO_F` | `float` or `double` | Print a decimal value with a given precision (default 2) | `float a = 3.1415; SERIAL_ECHO_F(a); SERIAL_CHAR(' '); SERIAL_ECHO_F(a, 4);` | `3.14 3.1415`|
| `SERIAL_ECHOPAIR` | String / Value pairs | Print a series of string literals and values alternately | `SERIAL_ECHOPAIR("Bob", 34);` | `Bob34` |
| `SERIAL_ECHOLNPAIR` | Same as `SERIAL_ECHOPAIR` | Do `SERIAL_ECHOPAIR`, adding a newline | `SERIAL_ECHOPAIR("Alice", 56);` | `alice56` |
| `SERIAL_ECHOPAIR_P` | Like `SERIAL_ECHOPAIR` but takes PGM strings | Print a series of PGM strings and values alternately | `SERIAL_ECHOPAIR_P(GET_TEXT(MSG_HELLO), 123);` | `Hello123` |
| `SERIAL_ECHOLNPAIR_P` | Same as `SERIAL_ECHOPAIR_P` | Do `SERIAL_ECHOPAIR_P`, adding a newline | `SERIAL_ECHOLNPAIR_P(PSTR("Alice"), 78);` | `alice78\n` |
| `SERIAL_ECHOLIST` | String literal, values | Print a string literal and a list of values | `SERIAL_ECHOLIST("Key ", 1, 2, 3);` | `Key 1, 2, 3` |
| `SERIAL_ECHO_START` | None | Prefix an echo line | `SERIAL_ECHO_START();` | `echo:` |
| `SERIAL_ECHO_MSG` | Same as `SERIAL_ECHOLN_PAIR` | Print a full echo line | `SERIAL_ECHO_MSG("Count is ", count);` | `echo:Count is 3` |
| `SERIAL_ERROR_START`| None | Prefix an error line | `SERIAL_ERROR_START();` | `Error:` |
| `SERIAL_ERROR_MSG` | Same as `SERIAL_ECHOLN_PAIR` | Print a full error line | `SERIAL_ERROR_MSG("Not found");` | `Error:Not found` |
| `SERIAL_ECHO_SP` | Number of spaces | Print one or more spaces | `SERIAL_ECHO_SP(3)` | `   ` |
| `SERIAL_EOL` | None | Print an end of line | `SERIAL_EOL();` | `\n` |
| `SERIAL_OUT` | `SERIAL_OUT(myMethod)` | Call a custom serial method | `SERIAL_OUT(msgDone);` | ... |

*This document was written by [X-Ryl669](https://blog.cyril.by) and is under [CC-SA license](https://creativecommons.org/licenses/by-sa)*
