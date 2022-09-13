/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * CREALITY (STM32F103) board pin assignments
 */

#include "env_validate.h"

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "CREALITY supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "Creality V521"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Creality V5.2.1"
#endif

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  // FLASH
  //#define FLASH_EEPROM_EMULATION

  // I2C
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0 used only for display settings
  #if ENABLED(IIC_BL24CXX_EEPROM)
    #define IIC_EEPROM_SDA       PC2
    #define IIC_EEPROM_SCL       PC3
    #define MARLIN_EEPROM_SIZE             0x800  // 2Kb (24C16)
  #else
    #define SDCARD_EEPROM_EMULATION               // SD EEPROM until all EEPROM is BL24CXX
    #define MARLIN_EEPROM_SIZE             0x800  // 2Kb
  #endif

  // SPI
  //#define SPI_EEPROM                            // EEPROM on SPI-0
  //#define SPI_CHAN_EEPROM1  ?
  //#define SPI_EEPROM1_CS    ?

  // 2K EEPROM
  //#define SPI_EEPROM2_CS    ?

  // 32Mb FLASH
  //#define SPI_FLASH_CS      ?
#endif

//
// Servos
//
#ifdef BLTOUCH
  #define SERVO0_PIN       PD13  // BLTouch OUT
  #define Z_MIN_PIN        PD12
#else
  #define Z_MIN_PIN        PE1
#endif

//
// Limit Switches
//
#define X_MIN_PIN          PD10
#define X_MAX_PIN          PE15
#define Y_MIN_PIN          PE0
// #define Z_MIN_PIN       PE1
#define Z2_MIN_PIN         PE2

//
// Filament Runout Sensor
//
  #define FIL_RUNOUT_PIN                  PE5   // "Pulled-high"
  #define FIL_RUNOUT2_PIN                 PE6   // "Pulled-high"

//
// Steppers
//
#define X_ENABLE_PIN                        PC7
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PD15
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PD14
#endif

#define X2_ENABLE_PIN                       PE11
#ifndef X2_STEP_PIN
  #define X2_STEP_PIN                       PE9
#endif
#ifndef X2_DIR_PIN
  #define X2_DIR_PIN                        PE8
#endif

#define Y_ENABLE_PIN                        PB9
#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB7
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PB6
#endif

#define Z_ENABLE_PIN                        PB5
#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB3
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PD7
#endif

#define Z2_ENABLE_PIN                       PC5
#ifndef Z2_STEP_PIN
  #define Z2_STEP_PIN                       PA7
#endif
#ifndef Z2_DIR_PIN
  #define Z2_DIR_PIN                        PA6
#endif

#define E0_ENABLE_PIN                       PD4
#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                       PD1
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                        PD0
#endif

#define E1_ENABLE_PIN                       PE7
#ifndef E1_STEP_PIN
  #define E1_STEP_PIN                       PB1
#endif
#ifndef E1_DIR_PIN
  #define E1_DIR_PIN                        PB0
#endif

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_JTAG

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA4   // TH0
#define TEMP_1_PIN                          PA5   // TH1
#define TEMP_BED_PIN                        PA3   // TB1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1   // HEATER0
#define HEATER_1_PIN                        PA0   // HEATER1
#define HEATER_BED_PIN                      PA2   // HOT BED

#define FAN_PIN                             PB14   // FAN
#define FAN1_PIN                            PB12   // FAN
#define FAN_SOFT_PWM

//
// SD Card
//
#define SD_DETECT_PIN                       PA8
#define SDCARD_CONNECTION                ONBOARD
#define ONBOARD_SPI_DEVICE                     1
#define ONBOARD_SD_CS_PIN                   PC11   // SDSS
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE                           // This board's SD is only seen by the printer

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

  #define BEEPER_PIN                        PC6

#elif ENABLED(VET6_12864_LCD)

  // VET6 12864 LCD
  #define LCD_PINS_RS                       PA4
  #define LCD_PINS_ENABLE                   PA7
  #define LCD_PINS_D4                       PA5

  #define BTN_ENC                           PC5
  #define BTN_EN1                           PB10
  #define BTN_EN2                           PA6

#elif ENABLED(DWIN_CREALITY_LCD)

  // RET6 DWIN ENCODER LCD
  #define BTN_ENC                           PB14
  #define BTN_EN1                           PB15
  #define BTN_EN2                           PB12

  //#define LCD_LED_PIN                     PB2
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                      PB13
    #undef SPEAKER
  #endif

#elif ENABLED(DWIN_VET6_CREALITY_LCD)

  // VET6 DWIN ENCODER LCD
  #define BTN_ENC                           PA6
  #define BTN_EN1                           PA7
  #define BTN_EN2                           PA4

  #define BEEPER_PIN                        PA5

#endif

//  DGUS LCDs
#if HAS_DGUS_LCD
  #define LCD_SERIAL_PORT 3
#endif // HAS_DGUS_LCD
