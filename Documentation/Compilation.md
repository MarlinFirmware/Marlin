Configuring and compilation:

Install the latest non-beta arduino software IDE/toolset
   http://www.arduino.cc/en/Main/Software

Download the Marlin firmware
   https://github.com/MarlinFirmware/Marlin/tree/development
   
   For the latest development, or 
   
   
   
   For the latest stable release
   
   In both cases use the "Download Zip" button on the right.

For some spec. boards a spec. dir in the ArduinoAddons directory in the Marlin dir needs to be copied to the arduino environment. <arduino home>\hardware\

Start the arduino IDE.
Select Tools -> Board -> Arduino Mega 2560    or your microcontroller
Select the correct serial port in Tools ->Serial Port
Open Marlin.pde or .ino

Click the Verify/Compile button

Click the Upload button
If all goes well the firmware is uploading

That's ok.  Enjoy Silky Smooth Printing.