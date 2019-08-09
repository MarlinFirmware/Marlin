### Malyan M200 Build Instructions

Malyan M200 series firmware currently builds using the Arduino IDE. These instructions should 
guide you through the configuration and compilation.

1. Install the Arduino IDE from your favorite source (arduino.cc, windows store, app store)
2. Launch the IDE to add the ST boards manager:
   a. From the file menu, select preferences
   b. Add this link in the "*Additional Boards Managers URLs*" field:
      https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json
   c. Select "Show verbose ouptut during: compilation"
3. From the tools menu, choose Board, and at the top, Boards Manager
4. Type Malyan into the search field.
5. The only boards shown will be STM32 Cords by STMicroelectronics. The version should be at least 1.6.0, but if it's later, that's fine. Choose install. This will download many tools and packages, be patient.
6. From the Tools Menu, choose board, and scroll all the way down to 3D Printer Boards. 
   Select 3D Printer Boards..
7. From the Tools Meny, choose board part number:
   a. If you own a M200 V1 or early run (black V2), choose Malyan M200 V1
   b. If you own a M200 V2 later run (white/black) or V3 (Pro), choose Malyan M200 V2 (The V2 and V3 both share an STM32F070 MCU). Note that the V3 pinout is not complete - autolevel does not work.
8. From the Tools Menu, choose USB Support, CDC No Generic Serial.
9. Download the Marlin Source code from the bugfix-2.0.x branch. Unzip it
10. In the \Marlin-bugfix-2.0.x\Marlin directory are configuration.h and configuration_adv.h. Copy the ones from ..\config\examples\Malyan\M200 over these.
11. If you have an early run V2, the steps per MM are roughly half. Consult the mpminipro.com wiki for steps for your unit - modify configuration.h.
12. Inverting Axis - there's no pattern to which axis will need to be inverted - the only way to know is to test your particular printer. If you DO know, go ahead and invert the right ones.
13. Open the marlin.ino in Arduino. (it's in marlin-bugfix-2.0.x\Marlin).
14. From the sketch menu, select File->Export Compiled Binary.
15. When compilation is done, you've built firmware. The next stage is to flash it on. To do this look for a line like:
"C:\\Users\\Administrator\\Documents\\ArduinoData\\packages\\STM32\\tools\\arm-none-eabi-gcc\\8.2.1-1.7/bin/arm-none-eabi-objcopy" -O binary "C:\\Users\\ADMINI~1\\AppData\\Local\\Temp\\arduino_build_335240/Marlin.ino.elf" "C:\\Users\\ADMINI~1\\AppData\\Local\\Temp\\arduino_build_335240/Marlin.ino.bin"
  The file "marlin.ino.bin" is your firmware. M200 (v1-3) and M300 printers require flashing via SD card. Use the SD card which came with the printer if possible - the bootloader is very picky about SD cards. Copy marlin.ino.bin to your SD card under three names: firmware.bin, update.bin, and fcupdate.flg.
16. Insert the SD card into your printer. Make sure the X axis is in the middle of the bed and the Y axis is as well (X and Y closed signals a UI upgrade to the bootloader).
17. Powercycle the printer. The first flash may take longer - don't be surprised if the .99 version number doesn't show up until after the UI has launched the default screen.
18. Remove the SD card and remove the fcupdate.flg file to prevent accidentally reflashing. 
19. Test endstops, homing directions, and run a PID autotune.

Welcome to Marlin 2.x...

