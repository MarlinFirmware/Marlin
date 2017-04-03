# Development on Linux
## Configuring the system

Before trying to build the software, be sure you have installed the following tools.

### Make
Make is an utility that builds executable programs and libraries from source code using a Makefile template. The software uses this tool to know how to compile and how to flash the binary.

##### How to install it?
1. In Ubuntu and derived distributions, you can install the package `build-essential` which contains the Make utility among other required packages.

```
sudo apt-get install build-essential
```

### Arduino 1.6.7
The compilation and flash processes use the AVR toolchain. The easy way to get these tools is to install the Arduino SDK (version 1.6.7).

The Arduino files should be installed on your user home folder (/home/your_user_name/arduino).

##### How to install it?
1. Download the installer from Arduino web page:

    * [32-bits OS](https://www.arduino.cc/download_handler.php?f=/arduino-1.6.7-linux32.tar.xz)
    * [64-bits OS](https://www.arduino.cc/download_handler.php?f=/arduino-1.6.7-linux64.tar.xz)

2. Extract the compressed folder on your user home folder.
3. Rename the uncompressed folder to 'arduino'.

## Getting the Source Code
[Clone](https://help.github.com/articles/cloning-a-repository/) or download the source code from the following locations:

* Master at [bq/Marlin](https://github.com/bq/Marlin)
* Previous releases at [bq/Marlin/Releases](https://github.com/bq/Marlin/releases), including prebuilt binaries and source code for those releases.

## Compiling the Source Code
The compilation process has two stages:

1. Choose the target device.

  If the target device is Witbox, Hephestos or Hephestos XL, the firmware language can be set. For Witbox 2 and Hephestos 2, the firmware image is multilanguage and this option has no effect.

  ```
  make <target> [<language>]
  ```

  List of targets:

  * witbox
  * hephestos
  * hephestos_xl
  * witbox_2
  * hephestos_2

  List of languages:

  * DE: German
  * EN: English
  * ES: Spanish
  * FI: Finnish
  * FR: French
  * IT: Italian
  * NL: Dutch
  * PL: Polish
  * PT: Portuguese

2. Build the image for the selected target.

  ```
  make
  ```

## Flashing the Binary Image
For upload the generated image, connect your computer to the printer and type the next line:

  ```
  make upload
  ```
