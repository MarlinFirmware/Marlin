# Setup for Linux
## Configurating the Enviroment

Before attempting to compile the software, be sure you have installed the following tools.

### Make
Make is a utility that automatically builds executable programs and libraries from source code. The software uses this tool to know how to compile and how to flash the binary.

##### How to install it?
1. Install the package `build-essential` which contains the Make utility.
```shell
sudo apt-get install build-essential
```

### Arduino 1.6.5
The compilation and flash processes use the AVR toolchain. The easy way to get these tools is to install the Arduino SDK (version 1.6.5).

The Arduino files should be installed on your user home folder (/home/your_user_name/arduino).

##### How to install it?
1. Download the installer from Arduino web page:

    * [32-bits OS](https://www.arduino.cc/download_handler.php?f=/arduino-1.6.5-r5-linux32.tar.xz)
    * [64-bits OS](https://www.arduino.cc/download_handler.php?f=/arduino-1.6.5-r5-linux64.tar.xz)

2. Extract the compressed folder on your user home folder.
3. Rename the uncompressed folder to 'arduino'.

## Getting the Source Code