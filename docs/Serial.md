# Serial port architecture in Marlin

Marlin is targeting a pletora of different CPU architecture and platforms. Each of these platforms has its own serial interface.
While many provide a Arduino-like Serial class, it's not all of them, and the differences in the existing API create a very complex brain teaser for writing code that works more or less on each platform.

Moreover, many platform have intrinsic needs about serial port (like forwarding the output on multiple serial port, providing a *serial-like* telnet server, mixing USB-based serial port with SD card emulation) that are difficult to handle cleanly in the other platform serial logic.


Starting with version `2.0.9`, Marlin provides a common interface for its serial needs.

## Common interface

This interface is declared in `Marlin/src/core/serial_base.h` 
Any implementation will need to follow this interface for being used transparently in Marlin's codebase.

The implementation was written to prioritize performance over abstraction, so the base interface is not using virtual inheritance to avoid the cost of virtual dispatching while calling methods.
Instead, the Curiously Recurring Template Pattern (**CRTP**) is used so that, upon compilation, the interface abstraction does not incur a performance cost.

Because some platform do not follow the same interface, the missing method in the actual low-level implementation are detected via SFINAE and a wrapper is generated when such method are missing. See `CALL_IF_EXISTS` macro in `Marlin/src/core/macros.h` for the documentation of this technic.  

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

For example, to present a serial interface that's outputting to 2 serial port, the first one being hooked at runtime and the second one connected to a runtime switchable telnet client, you'll declare the type to use as:
```
typedef MultiSerial< RuntimeSerial<Serial>, ConditionalSerial<TelnetClient> > Serial0Type;
```

## Emergency parser
By default, the serial base interface provide an emergency parser that's only enable for serial classes that support it. 
Because of this condition, all underlying type takes a first `bool emergencyParserEnabled` argument to their constructor. You must take into account this parameter when defining the actual type used. 


*This document was written by [X-Ryl669](https://blog.cyril.by) and is under [CC-SA license](https://creativecommons.org/licenses/by-sa)*