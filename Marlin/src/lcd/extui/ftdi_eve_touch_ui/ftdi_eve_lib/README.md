FTDI EVE Library
----------------

The FTDI EVE Library is a fully open-source library and UI framework for the FTDI
FT800 and FT810 graphics processor.

Although the library has been developed within Lulzbot for providing a user interface
for Marlin, the library has been written so that it can be used in any Arduino sketch.

The library is split into two parts. The "basic" API provides a shallow interface to
the underlying FTDI hardware and command FIFO and provides low-level access to the
hardware as closely as possible to the API described in the FTDI Programmer's Guide.

The "extended" API builds on top of the "basic" API to provide a GUI framework for
handling common challenges in building a usable GUI. The GUI framework provides the
following features:

- Macros for a resolution-independent placement of widgets based on a grid.
- Class-based UI screens, with press and unpress touch events, as well as touch repeat.
- Event loop with button debouncing and button push visual and auditory feedback.
- Easy screen-to-screen navigation including a navigation stack for going backwards.
- Visual feedback for disabled vs enabled buttons, and custom button styles.
- A sound player class for playing individual notes or complete sound sequences.
- Display list caching, for storing static background elements of a screen in RAM_G.

See the "examples" folder for Arduino sketches. Modify the "src/config.h" file in
each to suit your particular setup. The "sample_configs" contain sample configuration
files for running the sketches on our 3D printer boards.
