# Felix 2.0/3.0 Configuration for Marlin Firmware

Bringing silky smooth prints to Felix.

## Build HOWTO

  - Install the latest non-beta Arduino software IDE/toolset: http://www.arduino.cc/en/Main/Software
  - Download the Marlin firmware
    - [Latest developement version](https://github.com/MarlinFirmware/Marlin/tree/Development)
    - [Stable version](https://github.com/MarlinFirmware/Marlin/tree/Development)
  - In both cases use the "Download Zip" button on the right.

```
cd Marlin/Marlin
cp example_configurations/Felix/Configuration_adv.h .
```

The next step depends on your setup:

### Single Extruder Configuration

    cp example_configurations/Felix/Configuration.h .

### Dual Extruder Configuration

    cp example_configurations/Felix/Configuration_DUAL.h Configuration.h

### Compile Firmware

  - Start the Arduino IDE.
  - Select Tools -> Board -> Arduino Mega 2560
  - Select the correct serial port in Tools -> Serial Port (usually /dev/ttyUSB0)
  - Open Marlin.pde or .ino
  - Click the Verify/Compile button

### Flash Firmware

#### Connected directly via USB

  - Click the Upload button. If all goes well the firmware is uploading

#### Remote update

Find the latest Arduino build:

    ls -altr /tmp/
    drwxr-xr-x 5 chrono users 12288 Mar 3 21:41 build6072035599686630843.tmp

Copy the firmware to your printer host:

    scp /tmp/build6072035599686630843.tmp/Marlin.cpp.hex a.b.c.d:/tmp/

Connect to your printer host via ssh, stop Octoprint or any other service that may block your USB device and make sure you have avrdude installed, then run:

    avrdude -C/etc/avrdude.conf -v -v -v -patmega2560 -cwiring -P/dev/ttyUSB0 \
    -b115200 -D -Uflash:w:/tmp/Marlin.cpp.hex:i

## Acknowledgements

Mashed together and tested on https://apollo.open-resource.org/mission:resources:picoprint based on collaborative teamwork of @andrewsil1 and @thinkyhead.
