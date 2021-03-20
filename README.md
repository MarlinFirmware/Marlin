# Marlin_2.0.X_Beta_Alfawise_Ux0

This is the initial commit of the Marlin 2.0.X code port to LONGER_LKx boards(Its MCU is STM32F103VE). ONLY COMPILES with STM32 5.3.0 Libs or above.

This is based on a Marlin2.0.x_bugfix pull 14th April 2019 + LONGER、JMZ52 and Hobi code specific for LONGER_LKx and Alfawise. 

To build the code, you need to install Vscode + Platformio. 

## To have done

- Support screen calibration with M995
- Store the calibration results directly in Flash when you have defined LONGER3D
- Support TFT 320x240 color display by default, UI is prettier and more frendly than default Marlin UI

## Configuration

- 1 extruder with 1 temperature probe
- 1 heater bed with 1 temperature probe. U20 or U30 flag to be set at the beginning of config.h
- 4 drivers type A4998 for the 3 axis + extruder
- Homing on X,Y,Z on (-) direction. No Xmax,Ymax and Zmax declared.
- Filament detection has been assigned to Xmax pin, like the  LONGER_LKx or alfawise fw. 
- SD-Card support for the code.
- 2 serial ports running at 250,000 baud (on the LONGER_LKx USB port, which is directly connected to STM32 CPU)
- Support in theory only 2 types of LCD screen : ILI9341, ST7798V. The LCD screen types of LONGER_LKx and Alfawise Ux0 printers are ILI9341.

ILI9341 can have its ID in register 0x04 sometimes or in alternate ID4 register 0xD3. Returned value for 0xD3 is 0x9341.
The LCD register to be checked for ID is configurable in `Configuration.h`.

If you want to use the UI of Marlin, you need to Comment #define LONGER3D, like //#define XXXX

This code supports the LK4 printer by default. It also supports LK1,LK1_PLUS,LK2,U20,U30,U20_PLUS printer. If you are using one of them, you need to Comment it and select your pinter in `Configuration.h`.
