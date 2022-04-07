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
#pragma once

/**
 * No official schematics have been found.
 * But these differences where noted in https://github.com/bigtreetech/Rumba32/issues/1
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "TRONXY CXY-V10"

#if NO_EEPROM_SELECTED
  #define I2C_EEPROM
    #define MARLIN_EEPROM_SIZE            0x800  
#endif

#if ENABLED(FLASH_EEPROM_EMULATION)
  // Decrease delays and flash wear by spreading writes across the
  // 128 kB sector allocated for EEPROM emulation.
  #define FLASH_EEPROM_LEVELING
#endif

#if HOTENDS > 3 || E_STEPPERS > 3
  #error "RUMBA32 boards support up to 3 hotends / E-steppers."
#endif

#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

// Use soft PWM for fans - PWM is not working properly when paired with STM32 Arduino Core v1.7.0
// This can be removed when Core version is updated and PWM behaviour is fixed.
#define FAN_SOFT_PWM_REQUIRED

//
// Configure Timers
// TIM6 is used for TONE
// TIM7 is used for SERVO
// TIMER_SERIAL defaults to TIM7 and must be overridden in the platformio.h file if SERVO will also be used.
//              This will be difficult to solve from the Arduino IDE, without modifying the RUMBA32 variant
//              included with the STM32 framework.

#define STEP_TIMER 10
#define TEMP_TIMER 14

//
// Limit Switches
//
#define X_MIN_PIN                           PC15
//#define X_MAX_PIN                           PB13
#define Y_MIN_PIN                           PC14
//#define Y_MAX_PIN                           PD8
#define Z_MIN_PIN                           PC13
#define Z_MAX_PIN                           PF11  //Use for secondary endstop
//#define Z_MAX_PIN                           PD10

//
// Steppers
//
#define X_STEP_PIN                          PE5
#define X_DIR_PIN                           PF1
#define X_ENABLE_PIN                        PF0
//#define X_CS_PIN                            PC14

#define Y_STEP_PIN                          PF9
#define Y_DIR_PIN                           PF3
#define Y_ENABLE_PIN                        PF5
//#define Y_CS_PIN                            PE4


#define Z_STEP_PIN                          PA6
#define Z_DIR_PIN                           PF15
#define Z_ENABLE_PIN                        PA5
//#define Z_CS_PIN                            PE0

#define Z2_STEP_PIN                          PF6
#define Z2_DIR_PIN                           PF4
#define Z2_ENABLE_PIN                        PF7
//#define Z_CS_PIN                            PE0


#define E0_STEP_PIN                         PB1
#define E0_DIR_PIN                          PF13
#define E0_ENABLE_PIN                       PF14
//#define E0_CS_PIN                           PC13


//Filament runout is of no use without the LCD working.
//#define FIL_RUNOUT1_PIN                     PE6
//#define FIL_RUNOUT2_PIN                     PF12
//#define FIL_RUNOUT_PIN FIL_RUNOUT1_PIN

//I used the the filament runout pin to control the 3D touch as its on the ribbon/daughterboard
//Please note first resistor of RN4 must be bridged for this to work.
#define SERVO0_PIN PE6


#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI                     PA7
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO                     PA6
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK                      PA5
  #endif
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC3
//#define TEMP_1_PIN                          PC3
//#define TEMP_2_PIN                          PC2
//#define TEMP_3_PIN                          PC1
#define TEMP_BED_PIN                        PC2

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PG7
//#define HEATER_1_PIN                        PC7
//#define HEATER_2_PIN                        PC8
#define HEATER_BED_PIN                      PE2

#define FAN_PIN                             PG0
#define FAN1_PIN                            PG9
#define FAN2_PIN                            PD7

//
// SPI / SD CARD
#define SDCARD_CONNECTION            ONBOARD


//
// Misc. Functions
//
#define LED_PIN                             PG10
#define PS_ON_PIN                           PE1
//#define KILL_PIN                            PC5

#define BEEPER_PIN                          PA8

//
// LCD / Controller
//
#if HAS_WIRED_LCD

  #define BTN_EN1                           PB2
  #define BTN_EN2                           PB1
  #define BTN_ENC                           PE7

  #define LCD_PINS_RS                       PE10
  #define LCD_PINS_ENABLE                   PE9
  #define LCD_PINS_D4                       PE12

  #if ENABLED(MKS_MINI_12864)
    #define DOGLCD_CS                       PE13
    #define DOGLCD_A0                       PE14
  #endif

  #if IS_ULTIPANEL
    #define LCD_PINS_D5                     PE13
    #define LCD_PINS_D6                     PE14
    #define LCD_PINS_D7                     PE15

    #if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
      #define BTN_ENC_EN             LCD_PINS_D7  // Detect the presence of the encoder
    #endif
  #endif

#endif // HAS_WIRED_LCD

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                96
  #define BOARD_ST7920_DELAY_2                48
  #define BOARD_ST7920_DELAY_3               640
#endif

//#define SERVO0_PIN                          PA15  // Pin is not broken out, is a test point only.

#if HAS_TMC_UART
  //
  // TMC2208/TMC2209 Software Serial
  //
  #define X_SERIAL_TX_PIN                   PC14  // BTT Rumba32 only uses 1 pin for UART
  #define X_SERIAL_RX_PIN        X_SERIAL_TX_PIN

  #define Y_SERIAL_TX_PIN                   PE4
  #define Y_SERIAL_RX_PIN        Y_SERIAL_TX_PIN

  #define Z_SERIAL_TX_PIN                   PE0
  #define Z_SERIAL_RX_PIN        Z_SERIAL_TX_PIN

  #define E0_SERIAL_TX_PIN                  PC13
  #define E0_SERIAL_RX_PIN      E0_SERIAL_TX_PIN

  #define E1_SERIAL_TX_PIN                  PD5
  #define E1_SERIAL_RX_PIN      E1_SERIAL_TX_PIN

  #define E2_SERIAL_TX_PIN                  PD1
  #define E2_SERIAL_RX_PIN      E2_SERIAL_TX_PIN
#endif

#define TOUCH_MISO_PIN PB14
#define TOUCH_CS_PIN PD11
#define TOUCH_MOSI_PIN PB15
#define TOUCH_SCK_PIN PB13

// SPI Flash
#define HAS_SPI_FLASH                          1
#if HAS_SPI_FLASH
  #define SPI_FLASH_SIZE                0x200000  // 2MB
#endif

// SPI 2
#define SPI_FLASH_CS_PIN                    PG15
#define SPI_FLASH_MOSI_PIN                  PB5
#define SPI_FLASH_MISO_PIN                  PB4
#define SPI_FLASH_SCK_PIN                   PB3


//#define TFT_BUFFER_SIZE                  14400

//#define FSMC_DMA_DEV                      DMA2
//#define FSMC_DMA_CHANNEL               DMA_CH5