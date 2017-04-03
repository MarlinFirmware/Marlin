# Development on Windows
## Configurating the Enviroment

Before attempting to compile the software, be sure you have installed the following tools.

### Make (GnuWin32 - Make)
Make is a utility that automatically builds executable programs and libraries from source code. It controls the generation of executables and other non-source files of a program from the program's source files.

The software uses this tool to know how to compile and how to flash the binary.

Make is a Unix tool, so a port of this utility for Windows must be installed.

##### How to install it?
1. Download the [Make](http://sourceforge.net/projects/gnuwin32/files/make/3.81/make-3.81.exe/download) executable.

2. Execute it and follow the assistant.
Make must be installed on the following path:

```
C:\Program Files (x86)\GnuWin32
```

Make sure that make.exe exists on the following path:

```
C:\Program Files (x86)\GnuWin32\bin
```

### Arduino 1.6.7
The compilation and flash processes use the AVR toolchain. The easy way to get these tools is to install the Arduino SDK (version 1.6.7).

The Arduino files should be installed on the root folder of your sistem (C:\Arduino)

##### How to install it?
1. Download the [installer](https://www.arduino.cc/download_handler.php?f=/arduino-1.6.7-windows.exe) from Arduino web page.

2. Execute it and follow the assistant. Arduino must be installed on the following path:

  ```
  C:\Arduino
  ```

## Getting the Source Code
[Clone](https://help.github.com/articles/cloning-a-repository/) or download the source code from the following locations:

* Master at [bq/Marlin](https://github.com/bq/Marlin)
* Previous releases at [bq/Marlin/Releases](https://github.com/bq/Marlin/releases), including prebuilt binaries and source code for those releases.

Avoid using whitespaces for firmware path, for example.

  ```
  C:\witbox-fw
  ```

## Build Process
On Windows, the building and flashing processes have been automatized on a batch file.

1. Open a new Command Prompt. Press "WinKey + R" and type "cmd" on the recently open window.

2. Browse to your root project folder.

  ```
  cd C:\your_folder_path\witbox-fw
  ```

  If the firmware code was downloaded to the previously suggested path.

  ```
  cd C:\witbox-fw
  ```

3. Connect your computer to the printer and identify the COM port assigned.

4. Launch the script.

  ```
  make.cmd
  ```

5. Follow the wizard's instructions to select your device configuration and the COM port used by the printer.

6. Wait until the script finishes.
