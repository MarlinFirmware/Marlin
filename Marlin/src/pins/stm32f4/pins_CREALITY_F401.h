/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "env_validate.h"

#define BOARD_INFO_NAME      "STM32F401RET6"
#define DEFAULT_MACHINE_NAME "Ender-5 S1"

#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0 used only for display settings
  //#define SDCARD_EEPROM_EMULATION               // SD EEPROM until all EEPROM is BL24CXX
#endif
#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA11
  #define IIC_EEPROM_SCL                    PA12
  #define MARLIN_EEPROM_SIZE              0x800U  // 2K (24C16)
  #define EEPROM_EXCL_ZONE                916,926 // Creality bootloader EEPROM exclusion zone
#elif ENABLED(EEPROM_SETTINGS)
  #define MARLIN_EEPROM_SIZE              0x800U  // 2K
#endif

// SPI
//#define SPI_EEPROM                              // EEPROM on SPI-0
//#define SPI_CHAN_EEPROM1  ?
//#define SPI_EEPROM1_CS    ?

// 2K EEPROM
//#define SPI_EEPROM2_CS    ?

// 32Mb FLASH
//#define SPI_FLASH_CS      ?

#define BOARD_LCD_SERIAL_PORT               2

//
// Servos
//
#define SERVO0_PIN                          PC13  // BLTouch OUT

//
// Limit Switches
//
#define X_STOP_PIN                          PA5
#define Y_STOP_PIN                          PA6

#if ENABLED(BLTOUCH)
  #define Z_MAX_PIN                         PA15  // ROCK_20220730
  #define Z_MIN_PIN                         PC14
  //#define Z_STOP_PIN_NADD                 PA15  // Added z-axis limit switch
#elif ENABLED(PROBE_ACTIVATION_SWITCH)
  #define Z_MIN_PIN                         PA15
  #define PROBE_TARE_PIN                    PC14
  #define PROBE_ACTIVATION_SWITCH_PIN       PB2
#else
  #define Z_MIN_PIN                         PA15
#endif

//
// Filament runout
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PC15
#endif

//
// Steppers Motor drive chip power control
//
#define X_ENABLE_PIN                        PC3
#define X_STEP_PIN                          PC2
#define X_DIR_PIN                           PB9

#define Y_ENABLE_PIN                        PC3
#define Y_STEP_PIN                          PB8
#define Y_DIR_PIN                           PB7

#define Z_ENABLE_PIN                        PC3
#define Z_STEP_PIN                          PB6
#define Z_DIR_PIN                           PB5

#define E0_ENABLE_PIN                       PC3
#define E0_STEP_PIN                         PB4
#define E0_DIR_PIN                          PB3

//
// Temperature Sensors
//
#ifndef TEMP_0_PIN
  #define TEMP_0_PIN                        PC5   // E1_THERMISTOR PA0 -> PT100
#endif
#define TEMP_BED_PIN                        PC4   // BED_THERMISTOR_1

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PA1   // E1_HEAT_PWM
#define HEATER_BED_PIN                      PA7   // BED_HEAT_1 FET

#define FAN0_PIN                            PA0   // Part cooling fan FET
//#define FAN1_PIN                          PE11  // Extruder fan FET
//#define FAN2_PIN                          PE1   // Controller fan FET

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                   PC0   // Heatbreak fan FET (PA1 is the hotend heater)
#endif

//
// SD Card
//
#define SD_DETECT_PIN                       PC7
#define SDCARD_CONNECTION               ONBOARD
#define ONBOARD_SPI_DEVICE                     1
#define ONBOARD_SD_CS_PIN                   PC12  // SDSS
#define ONBOARD_SDIO
#define BOARD_NO_HOST_DRIVE                       // SD is only seen by the printer

//
// LCD / Controller
//
// The "LCD" header is the same 10-pin EXP3 found on the Creality V4.x and Ender-3 S1 boards.
// The stock touch screen uses UART2 on pins 3 and 4 (PA2/PA3), so it is not available when
// a 12864 display is used on this header.
//
/**
 *                 ------
 *           PC6  | 1  2 | PB2
 *  UART2_TX PA2  | 3  4 | PA3  UART2_RX
 *           PB14   5  6 | PB13
 *           PB12 | 7  8 | PB15
 *            GND | 9 10 | 5V
 *                 ------
 *                  EXP3
 */
#define EXP3_01_PIN                         PC6
#define EXP3_02_PIN                         PB2
#define EXP3_03_PIN                         PA2
#define EXP3_04_PIN                         PA3
#define EXP3_05_PIN                         PB14
#define EXP3_06_PIN                         PB13
#define EXP3_07_PIN                         PB12
#define EXP3_08_PIN                         PB15

#if ENABLED(CR10_STOCKDISPLAY)
  #define LCD_PINS_RS                EXP3_07_PIN
  #define LCD_PINS_EN                EXP3_08_PIN
  #define LCD_PINS_D4                EXP3_06_PIN

  #define BTN_ENC                    EXP3_02_PIN
  #define BTN_EN1                    EXP3_03_PIN
  #define BTN_EN2                    EXP3_05_PIN

  #define BEEPER_PIN                 EXP3_01_PIN

  // This MCU needs slower ST7920 serial timing than the 125 ns default for the CR-10 display.
  // (Without these the effective delays are 300 / 125 / 340 ns, and the display stays blank.)
  #define BOARD_ST7920_DELAY_1               600
  #define BOARD_ST7920_DELAY_2               600
  #define BOARD_ST7920_DELAY_3               600
#endif

//#define LED_CONTROL_PIN                   PA7

//
// WiFi Reset
//
#ifdef MENU_RESET_WIFI
  #define RESET_WIFI_PIN                    PB12
#endif

//
// Backpower
//
#ifndef BACKPOWER_CTRL_PIN
  #define BACKPOWER_CTRL_PIN                PA4   // Pull down on the electricity, When the ADC detects that the 24V power supply is below 20V, Save the continued call data first, PA4 = 1
#endif

#ifndef CHECK_24V_PIN
  #define POWER_DETECTION_PIN               PB0   // Detect ADC input pin of 24V power supply
#endif

/**
 * Logical pin vs. port/pin cross reference
 *
 * PA0  54   //  E1_THERMISTOR       PA9   0   //  TX
 * PA1  55   //  E2_THERMISTOR       PA10  1   //  RX
 * PA2  56   //  E3_THERMISTOR       PD3   2   //  CTS
 * PA3  53   //  BED_THERMISTOR_3    PD4   3   //  RTS
 * PA4  16   //  SPI_CS              PD5   4   //  TX
 * PA5  17   //  SPI_SCK             PD6   5   //  RX
 * PA6  18   //  SPI_MISO            PB5   6   //  WIFI_WAKEUP
 * PA7  19   //  SPI_MOSI            PE11  7   //  WIFI_RESET
 * PA8  43   //  V_STOP              PE12  8   //  WIFI_BOOT
 * PA9   0   //  TX                  PE1   9   //  STATUS_LED
 * PA10  1   //  RX                  PB12 10   //  SPI_CS
 * PA11 30   //  OTG_DM              PB15 11   //  SPI_MOSI
 * PA12 31   //  OTG_DP              PB14 12   //  SPI_MISO
 * PA13 20   //  JTAG_TMS/SWDIO      PB13 13   //  SPI_SCK
 * PA14 21   //  JTAG_TCK/SWCLK      PB7  14   //  SDA
 * PA15 25   //  SD_CARD_DETECT      PB6  15   //  SCL
 * PB0  49   //  E2_HEAT_PWM         PA4  16   //  SPI_CS
 * PB1  50   //  E3_HEAT_PWM         PA5  17   //  SPI_SCK
 * PB3  22   //  JTAG_TDO/SWO        PA6  18   //  SPI_MISO
 * PB4  37   //  USER4               PA7  19   //  SPI_MOSI
 * PB5   6   //  WIFI_WAKEUP         PA13 20   //  JTAG_TMS/SWDIO
 * PB6  15   //  SCL                 PA14 21   //  JTAG_TCK/SWCLK
 * PB7  14   //  SDA                 PB3  22   //  JTAG_TDO/SWO
 * PB8  77   //  E3_PWM              PC8  23   //  SDIO_D0
 * PB9  35   //  USER1               PC9  24   //  SDIO_D1
 * PB10 64   //  Y_PWM               PA15 25   //  SD_CARD_DETECT
 * PB12 10   //  SPI_CS              PC10 26   //  SDIO_D2
 * PB13 13   //  SPI_SCK             PC11 27   //  SDIO_D3
 * PB14 12   //  SPI_MISO            PC12 28   //  SDIO_CK
 * PB15 11   //  SPI_MOSI            PD2  29   //  SDIO_CMD
 * PC0  68   //  Z_DIR               PA11 30   //  OTG_DM
 * PC1  33   //  IR_ON               PA12 31   //  OTG_DP
 * PC2  51   //  BED_THERMISTOR_1    PD1  32   //  IR_OUT
 * PC3  52   //  BED_THERMISTOR_2    PC1  33   //  IR_ON
 * PC4  57   //  E1_FAN              PD7  34   //  USER3
 * PC5  58   //  E2_FAN              PB9  35   //  USER1
 * PC6  67   //  Z_PWM               PE0  36   //  USER2
 * PC7  48   //  E1_HEAT_PWM         PB4  37   //  USER4
 * PC8  23   //  SDIO_D0             PE7  38   //  USER_BUTTON
 * PC9  24   //  SDIO_D1             PD8  39   //  X_STOP
 * PC10 26   //  SDIO_D2             PD9  40   //  Y_STOP
 * PC11 27   //  SDIO_D3             PD10 41   //  Z_STOP
 * PC12 28   //  SDIO_CK             PD11 42   //  U_STOP
 * PC13 70   //  E1_DIR              PA8  43   //  V_STOP
 * PC14 69   //  E1_RESET            PD0  44   //  W_STOP
 * PC15 66   //  Z_RESET             PD13 45   //  BED_HEAT_2
 * PD0  44   //  W_STOP              PD14 46   //  BED_HEAT_1
 * PD1  32   //  IR_OUT              PD15 47   //  BED_HEAT_3
 * PD2  29   //  SDIO_CMD            PC7  48   //  E1_HEAT_PWM
 * PD3   2   //  CTS                 PB0  49   //  E2_HEAT_PWM
 * PD4   3   //  RTS                 PB1  50   //  E3_HEAT_PWM
 * PD5   4   //  TX                  PC2  51   //  BED_THERMISTOR_1
 * PD6   5   //  RX                  PC3  52   //  BED_THERMISTOR_2
 * PD7  34   //  USER3               PA3  53   //  BED_THERMISTOR_3
 * PD8  39   //  X_STOP              PA0  54   //  E1_THERMISTOR
 * PD9  40   //  Y_STOP              PA1  55   //  E2_THERMISTOR
 * PD10 41   //  Z_STOP              PA2  56   //  E3_THERMISTOR
 * PD11 42   //  U_STOP              PC4  57   //  E1_FAN
 * PD12 71   //  E1_PWM              PC5  58   //  E2_FAN
 * PD13 45   //  BED_HEAT_2          PE8  59   //  E3_FAN
 * PD14 46   //  BED_HEAT_1          PE13 60   //  X_RESET
 * PD15 47   //  BED_HEAT_3          PE14 61   //  X_PWM
 * PE0  36   //  USER2               PE15 62   //  X_DIR
 * PE1   9   //  STATUS_LED          PE10 63   //  Y_RESET
 * PE2  76   //  E3_DIR              PB10 64   //  Y_PWM
 * PE3  75   //  E3_RESET            PE9  65   //  Y_DIR
 * PE4  72   //  E2_RESET            PC15 66   //  Z_RESET
 * PE5  73   //  E2_PWM              PC6  67   //  Z_PWM
 * PE6  74   //  E2_DIR              PC0  68   //  Z_DIR
 * PE7  38   //  USER_BUTTON         PC14 69   //  E1_RESET
 * PE8  59   //  E3_FAN              PC13 70   //  E1_DIR
 * PE9  65   //  Y_DIR               PD12 71   //  E1_PWM
 * PE10 63   //  Y_RESET             PE4  72   //  E2_RESET
 * PE11  7   //  WIFI_RESET          PE5  73   //  E2_PWM
 * PE12  8   //  WIFI_BOOT           PE6  74   //  E2_DIR
 * PE13 60   //  X_RESET             PE3  75   //  E3_RESET
 * PE14 61   //  X_PWM               PE2  76   //  E3_DIR
 * PE15 62   //  X_DIR               PB8  77   //  E3_PWM
 */
