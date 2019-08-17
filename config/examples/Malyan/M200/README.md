### Malyan M200 Build Instructions

Malyan M200 series firmware currently builds using the Arduino IDE. These instructions should 
guide you through the configuration and compilation.

1. Install the Arduino IDE from your favorite source (arduino.cc, windows store, app store)
2. Launch the IDE to add the ST boards manager:
   - Open the **Preferences** dialog.
   - Add this link in the "*Additional Boards Managers URLs*" field:
      https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json
   - Select "**Show verbose ouptut during: compilation**."
3. Select **Tools** > **Board** > **Boards Manager**.
4. Type "Malyan" into the Search field.
5. The only board listed will be "**STM32 Cores by STMicroelectronics**." Any version from 1.6.0 up is fine. Choose install. This will download many tools and packages, be patient.
6. Open the **Tools** > **Board** submenu, scroll all the way down, and select **3D Printer Boards**.
7. From the **Tools** menu, select a board part number:
   - If you own a M200 V1 or early run (black V2), choose **Malyan M200 V1**.
   - If you own a M200 V2 later run (white/black) or V3 (Pro), choose **Malyan M200 V2** (The V2 and V3 both share an STM32F070 MCU). Note that the V3 pinout is not complete (autolevel doesn't work as of this writing).
8. From the **Tools** menu, choose **USB Support** > **CDC No Generic Serial**.
9. Download the latest Marlin source (from the [bugfix-2.0.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.0.x) branch) and unzip it.
10. Look in the `Marlin` subdirectory for the `Configuration.h` and `Configuration_adv.h` files. Replace these files with the configurations in the `config\examples\Malyan\M200` folder.
11. If you have an early-run V2, the steps-per-mm are roughly half. Consult the [mpminipro.com wiki](https://mpminipro.com/) for the steps that apply to your unit. Modify `Configuration.h`.
12. Inverting Axis. There's no pattern to axes will need to be inverted. The only way to know is to test your particular printer. If you *do* know, go ahead and invert the correct axes.
13. Open the `Marlin/Marlin.ino` file in Arduino IDE.
14. From the **Sketch** menu, select **File** > **Export Compiled Binary**.
15. When compilation is done you've built the firmware. The next stage is to flash it to the board. To do this look for a line like this: `"path/to/bin/arm-none-eabi-objcopy" -O binary "/path/to/Marlin.ino.elf" "/path/to/Marlin.ino.bin"`
  The file `Marlin.ino.bin` is your firmware binary. M200 (v1-3) and M300 printers require flashing via SD card. Use the SD card that came with the printer if possible. The bootloader is very picky about SD cards. Copy `Marlin.ino.bin` to your SD card under three names: `firmware.bin`, `update.bin`, and `fcupdate.flg`.
16. Insert the SD card into your printer. Make sure the X and Y axes are centered in the middle of the bed. (When X and Y endstops are closed this signals a UI upgrade to the bootloader.)
17. Power-cycle the printer. The first flash may take longer. Don't be surprised if the .99 version number doesn't show up until after the UI has launched the default screen.
18. Remove the SD card and delete the `fcupdate.flg` file from the card to prevent an accidental re-flash. 
19. Test the endstops and homing directions, run M303 PID autotune, and verify all features are working correctly.

Welcome to Marlin 2.x...
