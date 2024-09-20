/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Creality V3 (GD303RET6) board pin assignments
 */
#include "env_validate.h"

#if HOTENDS > 1 || E_STEPPERS > 1
  #error "Creality V3 only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "CR4NS200320C13"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender 3 V3 SE"
#endif

#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
//#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  // #define SDCARD_EEPROM_EMULATION
//#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA7
  #define IIC_EEPROM_SCL                    PA8
  #define MARLIN_EEPROM_SIZE                0x800  // 2Kb (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
  #define MARLIN_EEPROM_SIZE                0x800  // 2Kb
#endif


//
// Limit Switches
//
#define X_STOP_PIN                          PA5
#define Y_STOP_PIN                          PA6

#if ENABLED(BLTOUCH)
  #define Z_STOP_PIN                          PC14   // BLTouch IN
  #define SERVO0_PIN                          PC13   // BLTouch OUT
#elif ENABLED(PROBE_TARE)
  #define Z_STOP_PIN                          PC14
  #define PROBE_TARE_PIN                      PC13
  // #define PROBE_ACTIVATION_SWITCH_PIN      PB2
#else
  #define Z_STOP_PIN                          PA15
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15   // "Pulled-high"
#endif

// Filament check pins
#define HAS_CHECKFILAMENT
#if ENABLED(HAS_CHECKFILAMENT)
  #define CHECKFILAMENT_PIN                 PC15
#endif

// HX711 corresponding clock interface, this GPIO port do not parallel capacitor,
// And should be as short wire as possible (if too long, use shield wire).
#define HX711_SCK_PIN                       PA4
// HX711 corresponding data interface, this GPIO port should not parallel capacitor,
// And should be as short as possible wire (if too long, use shield wire).
#define HX711_SDO_PIN                       PC6

#ifndef CHECK_24V_PIN
  #define POWER_DETECTION_PIN               PB0   // Detect the ADC input pin of 24V power supply
#endif

//
// Steppers
//
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PC2
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PB9
#endif
#define X_ENABLE_PIN                        PC3

#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB8
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PB7
#endif
#define Y_ENABLE_PIN                        PC3

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB6
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB5
#endif
#define Z_ENABLE_PIN                        PC3

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                       PB4
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                        PB3
#endif
#define E0_ENABLE_PIN                       PC3

#if HAS_TMC_UART

  #define X_SERIAL_TX_PIN                     PB12
  #define X_SERIAL_RX_PIN                     PB12

  //#define Y_SERIAL_TX_PIN                   X_SERIAL_TX_PIN
  //#define Y_SERIAL_RX_PIN                   X_SERIAL_RX_PIN
  #define Y_SERIAL_TX_PIN                     PB13
  #define Y_SERIAL_RX_PIN                     PB13

  //#define Z_SERIAL_TX_PIN                   X_SERIAL_TX_PIN
  //#define Z_SERIAL_RX_PIN                   X_SERIAL_RX_PIN
  #define Z_SERIAL_TX_PIN                     PB14
  #define Z_SERIAL_RX_PIN                     PB14

  //#define E0_SERIAL_TX_PIN                  X_SERIAL_TX_PIN
  //#define E0_SERIAL_RX_PIN                  X_SERIAL_RX_PIN
  #define E0_SERIAL_TX_PIN                    PB15
  #define E0_SERIAL_RX_PIN                    PB15

  #define X_DIAG_PIN                          PB10
  #define Y_DIAG_PIN                          PB11
  //#define Z_DIAG_PIN                        PB14
  //#define E0_DIAG_PIN                       PB15

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                       19200 // 9600
#endif

// SPI 2
// #define W25QXX_CS_PIN                       -1   // PB12
// The current MOSI, MISO, SCK of HAL/STM32 are not defined by the following pins,
// Needs to be in buildroot\share\PlatformIO\variants\CREALITY_F107\variant.h modification
// #define W25QXX_MOSI_PIN                     PB15
// #define W25QXX_MISO_PIN                     PB14
// #define W25QXX_SCK_PIN                      -1   // PB13

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

// Temperature Sensors
//
#define TEMP_0_PIN                          PC5   // TH1
#define TEMP_BED_PIN                        PC4   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1   // HEATER1
#define HEATER_BED_PIN                      PB2   // HOT BED

#ifndef FAN0_PIN
  #define FAN0_PIN                           PA0   // FAN
#endif
#if PIN_EXISTS(FAN)
  #define FAN_SOFT_PWM
#endif

//
// SD Card
//
#define SD_DETECT_PIN                    PC7
#define SDCARD_CONNECTION                ONBOARD
#define ONBOARD_SPI_DEVICE               1
#define ONBOARD_SD_CS_PIN                PA4   // SDSS
#define ONBOARD_SDIO
#define NO_SD_HOST_DRIVE                       // This board's SD is only seen by the printer

#if ENABLED(CR10_STOCKDISPLAY) && NONE(RET6_12864_LCD, VET6_12864_LCD)
  #error "Define RET6_12864_LCD or VET6_12864_LCD to select pins for CR10_STOCKDISPLAY with the Creality V4 controller."
#endif

#if ENABLED(RET6_12864_LCD)

  // RET6 12864 LCD
  #define LCD_PINS_RS                       PB12
  #define LCD_PINS_ENABLE                   PB15
  #define LCD_PINS_D4                       PB13

  #define BTN_ENC                           PB2
  #define BTN_EN1                           PB10
  #define BTN_EN2                           PB14

  #ifndef HAS_PIN_27_BOARD
    #define BEEPER_PIN                      PC6
  #endif

#elif ENABLED(VET6_12864_LCD)

  // VET6 12864 LCD
  #define LCD_PINS_RS                       PA4
  //#define LCD_PINS_ENABLE                 PA7
  #define LCD_PINS_D4                       PA5

  #define BTN_ENC                           PC5
  #define BTN_EN1                           PB10
  #define BTN_EN2                           PA6

#elif ENABLED(DWIN_CREALITY_LCD)

  // RET6 DWIN ENCODER LCD
  #define BTN_ENC                           PB1
  #define BTN_EN1                           PA11
  #define BTN_EN2                           PA12

  // //#define LCD_LED_PIN                     PB2
  // // #if ENABLED(SPEAKER)
  //   #define BEEPER_PIN                      PB13
  // //   #undef SPEAKER
  // // #endif
 
 #ifndef BEEPER_PIN
    #define BEEPER_PIN                     PB0//PB13
    #undef SPEAKER
  #endif
  
#elif ENABLED(DWIN_VET6_CREALITY_LCD)

  // VET6 DWIN ENCODER LCD
  #define BTN_ENC                           PA6
  //#define BTN_EN1                         PA7
  #define BTN_EN2                           PA4

  #define BEEPER_PIN                        PA5

#endif