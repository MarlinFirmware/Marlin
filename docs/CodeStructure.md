# Marlin Code Structure

Marlin is a pretty large embedded C++ project with many moving parts.

## Project Root Folder

The project root folder contains only those files which are _required_ to be at the project root. This includes such files as `.editorconfig` (for EditorConfig), `.gitignore`, `Makefile`, and the project `README.md`.

We recommend PlatformIO IDE for the best build experience. The `platformio.ini` file configures PlatformIO to build Marlin when the project is opened up in Visual Studio Code (PlatformIO IDE) or when PlatformIO (i.e., `pio`) is invoked from the command-line. The `ini` folder contains platform-specific INI files for PlatformIO.

## `buildroot` Folder

This folder contains scripts, editor support, useful information, and helpful items of various kinds. This folder must be kept with the project for PlatformIO build support.

## `Marlin` Folder

This folder is named 'Marlin' for the benefit of Arduino IDE. The `Marlin.ino` project file for Arduino IDE is stored here.

The most important files here are the Marlin configuration files `Configuration.h` and `Configuration_adv.h` that configure the firmware kernel and default settings.

The `config.ini` file is parsed and applied to `Configuration.h` and `Configuration_adv.h` at the start of PlatformIO build as an alternative way to configure the build. By default it does nothing.

### `config` Folder

Starting with Marlin 2.2 the `config` folder contains sub-configuration files organized by category / feature.

### `src` Folder

All the firmware code lives in the `src` folder. This folder is itself organized into several sub-folders:

- `core` : Core headers containing essential types, macros, and defines (such as boards IDs).
- `feature` : Individual features source code, mostly optional.
- `gcode` : G-code hooks for both core functionality and optional features.
- `HAL` : Platform-specific interfaces to the board pins, ports, and hardware.
- `inc` : Essentials to configure, set conditionals, and include required headers.
- `lcd` : LCD / Controller source code.
- `libs` : Useful types for general usage, both CPP and H files. (Not pre-built like `lib`.)
- `module` : Essential Marlin code for motion, temperature, kinematics, etc.
- `pins` : Pins files associate board pins with firmware functions.
- `sd` : Code for the SD card and other storage devices.
- `tests` : Code for tests used in development.
