# Configuring and compilation

  1. Install the latest non-beta arduino software IDE/toolset: http://www.arduino.cc/en/Main/Software
  2. Download the Marlin firmware
    - [Latest developement version](https://github.com/MarlinFirmware/Marlin/tree/Development)
    - [Stable version](https://github.com/MarlinFirmware/Marlin/tree/Development)
  3. In both cases use the "Download Zip" button on the right.
  4. Some boards require special files and/or libraries from the ArduinoAddons directory. Take a look at the dedicated [README](/ArduinoAddons/README.md) for details.
  5. Start the arduino IDE.
  6. Select Tools -> Board -> Arduino Mega 2560    or your microcontroller
  7. Select the correct serial port in Tools ->Serial Port
  8. Open Marlin.pde or .ino
  9. Click the Verify/Compile button
  10. Click the Upload button. If all goes well the firmware is uploading

That's ok.  Enjoy Silky Smooth Printing.