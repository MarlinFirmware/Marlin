/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// Source: https://github.com/stm32duino/Arduino_Core_STM32/blob/master/variants/ST3DP001_EVAL/variant.cpp

/**
 * HOW TO COMPILE
 *
 * PlatformIO - Use the STM32F401VE_STEVAL environment (or the "Auto Build Marlin" extension).
 *
 * Arduino - Tested with 1.8.10
 *      Install library per https://github.com/stm32duino/Arduino_Core_STM32
 *      Make the following selections under the TOOL menu in the Arduino IDE
 *          Board: "3D printer boards"
 *          Board part number: "STEVAL-3DP001V1"
 *          U(S)ART support: "Enabled (generic "Serial")"
 *          USB support (if available): "CDC (no generic "Serial")"
 *          Optimize: "Smallest (-Os default)"
 *          C Runtime Library: "newlib Nano (default)"
 */

#pragma once

#ifndef STM32F4
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#endif

#ifndef MACHINE_NAME
  #define MACHINE_NAME "STEVAL-3DP001V1"
#endif

//
// Limit Switches
//
#define X_MIN_PIN          39   // PD8   X_STOP
#define Y_MIN_PIN          40   // PD9   Y_STOP
#define Z_MIN_PIN          41   // PD10  Z_STOP

#define X_MAX_PIN          44   // PD0   W_STOP
#define Y_MAX_PIN          43   // PA8   V_STOP
#define Z_MAX_PIN          42   // PD11  U_STOP

//
// Z Probe (when not Z_MIN_PIN)
//
//#ifndef Z_MIN_PROBE_PIN
//  #define Z_MIN_PROBE_PIN  16  // PA4
//#endif

//
// Filament runout
//
//#define FIL_RUNOUT_PIN     53   // PA3    BED_THE

//
// Steppers
//

#define X_STEP_PIN         61  // PE14    X_PWM
#define X_DIR_PIN          62  // PE15    X_DIR
#define X_ENABLE_PIN       60  // PE13    X_RES
#define X_CS_PIN           16  // PA4     SPI_CS

#define Y_STEP_PIN         64  // PB10    Y_PWM
#define Y_DIR_PIN          65  // PE9     Y_DIR
#define Y_ENABLE_PIN       63  // PE10    Y_RES
#define Y_CS_PIN           16  // PA4     SPI_CS

#define Z_STEP_PIN         67  // PC6     Z_PWM
#define Z_DIR_PIN          68  // PC0     Z_DIR
#define Z_ENABLE_PIN       66  // PC15    Z_RES
#define Z_CS_PIN           16  // PA4     SPI_CS

#define E0_STEP_PIN        71  // PD12    E1_PW
#define E0_DIR_PIN         70  // PC13    E1_DIR
#define E0_ENABLE_PIN      69  // PC14    E1_RE
#define E0_CS_PIN          16  // PA4     SPI_CS

#define E1_STEP_PIN        73  // PE5     E2_PWM
#define E1_DIR_PIN         74  // PE6     E2_DIR
#define E1_ENABLE_PIN      72  // PE4     E2_RESE
#define E1_CS_PIN          16  // PA4     SPI_CS

#define E2_STEP_PIN        77  // PB8     E3_PWM
#define E2_DIR_PIN         76  // PE2     E3_DIR
#define E2_ENABLE_PIN      75  // PE3     E3_RESE
#define E2_CS_PIN          16  // PA4     SPI_CS

// needed to pass a sanity check
#define X2_CS_PIN          16  // PA4     SPI_CS
#define Y2_CS_PIN          16  // PA4     SPI_CS
#define Z2_CS_PIN          16  // PA4     SPI_CS
#define Z3_CS_PIN          16  // PA4     SPI_CS
#define E3_CS_PIN          16  // PA4     SPI_CS
#define E4_CS_PIN          16  // PA4     SPI_CS
#define E5_CS_PIN          16  // PA4     SPI_CS

#if HAS_L64XX
  #define L6470_CHAIN_SCK_PIN  17   // PA5
  #define L6470_CHAIN_MISO_PIN 18   // PA6
  #define L6470_CHAIN_MOSI_PIN 19   // PA7
  #define L6470_CHAIN_SS_PIN   16   // PA4

  //#define SCK_PIN        L6470_CHAIN_SCK_PIN
  //#define MISO_PIN       L6470_CHAIN_MISO_PIN
  //#define MOSI_PIN       L6470_CHAIN_MOSI_PIN
#else
  //#define SCK_PIN        13   // PB13    SPI_S
  //#define MISO_PIN       12   // PB14    SPI_M
  //#define MOSI_PIN       11   // PB15    SPI_M
#endif

/**
 * Macro to reset/enable L6474 stepper drivers
 *
 * IMPORTANT - To disable (bypass) L6474s, install the corresponding
 *             resistors (R11 - R17) and change the "V" to "0" for the
 *             corresponding pins here:
 */
#define ENABLE_RESET_L64XX_CHIPS(V)   do{ OUT_WRITE(X_ENABLE_PIN, V); \
                                          OUT_WRITE(Y_ENABLE_PIN, V); \
                                          OUT_WRITE(Z_ENABLE_PIN, V); \
                                          OUT_WRITE(E0_ENABLE_PIN,V); \
                                          OUT_WRITE(E1_ENABLE_PIN,V); \
                                          OUT_WRITE(E2_ENABLE_PIN,V); \
                                        }while(0)

//
// Temperature Sensors
//
#define TEMP_0_PIN          3   // Analog input 3,  digital pin 54   PA0     E1_THERMISTOR
#define TEMP_1_PIN          4   // Analog input 4,  digital pin 55   PA1     E2_THERMISTOR
#define TEMP_2_PIN          5   // Analog input 5,  digital pin 56   PA2     E3_THERMISTOR
#define TEMP_BED_PIN        0   // Analog input 0,  digital pin 51   PC2     BED_THERMISTOR_1
#define TEMP_BED_1_PIN      1   // Analog input 1,  digital pin 52   PC3     BED_THERMISTOR_2
#define TEMP_BED_2_PIN      2   // Analog input 2,  digital pin 53   PA3     BED_THERMISTOR_3

//
// Heaters / Fans
//
#define HEATER_0_PIN       48   // PC7   E1_HEAT_PWM
#define HEATER_1_PIN       49   // PB0   E2_HEAT_PWM
#define HEATER_2_PIN       50   // PB1   E3_HEAT_PWM
#define HEATER_BED_PIN     46   // PD14 (BED_HEAT_1 FET
#define HEATER_BED_1_PIN   45   // PD13 (BED_HEAT_2 FET
#define HEATER_BED_2_PIN   47   // PD15 (BED_HEAT_3 FET

#define FAN_PIN            57   // PC4   E1_FAN   PWM pin, Part cooling fan FET
#define FAN1_PIN           58   // PC5   E2_FAN   PWM pin, Extruder fan FET
#define ORIG_E0_AUTO_FAN_PIN  FAN1_PIN
#define FAN2_PIN           59   // PE8   E3_FAN   PWM pin, Controller fan FET

//
// Misc functions
//
#define SDSS               16   // PA4    SPI_CS
#define LED_PIN            -1   // 9 // PE1 green LED   Heart beat
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define POWER_LOSS_PIN     -1   // PWR_LOSS / nAC_FAULT

//
// LCD / Controller
//
//#define SD_DETECT_PIN      66   // PA15    SD_CA
//#define BEEPER_PIN         24   // PC9     SDIO_D1
//#define LCD_PINS_RS        65   // PE9     Y_DIR
//#define LCD_PINS_ENABLE    59   // PE8     E3_FAN
//#define LCD_PINS_D4        10   // PB12    SPI_C
//#define LCD_PINS_D5        13   // PB13    SPI_S
//#define LCD_PINS_D6        12   // PB14    SPI_M
//#define LCD_PINS_D7        11   // PB15    SPI_M
//#define BTN_EN1            57   // PC4     E1_FAN
//#define BTN_EN2            58   // PC5     E2_FAN
//#define BTN_ENC            52   // PC3     BED_THE

//
// Extension pins
//
//#define EXT0_PIN           49   // PB0     E2_HEAT
//#define EXT1_PIN           50   // PB1     E3_HEAT
//#define EXT2_PIN                // PB2    not used (tied to ground
//#define EXT3_PIN           39   // PD8     X_STOP
//#define EXT4_PIN           40   // PD9     Y_STOP
//#define EXT5_PIN           41   // PD10    Z_STOP
//#define EXT6_PIN           42   // PD11
//#define EXT7_PIN           71   // PD12    E1_PW
//#define EXT8_PIN           64   // PB10    Y_PWM

// WIFI
//  2   // PD3   CTS
//  3   // PD4   RTS
//  4   // PD5   TX
//  5   // PD6   RX
//  6   // PB5   WIFI_WAKEUP
//  7   // PE11  WIFI_RESET
//  8   // PE12  WIFI_BOOT


// I2C USER
// 14   // PB7   SDA
// 15   // PB6   SCL

// JTAG
// 20   // PA13  JTAG_TMS/SWDIO
// 21   // PA14  JTAG_TCK/SWCLK
// 22   // PB3   JTAG_TDO/SWO

//
// SD support
//
//#define SDIO_SUPPORT
// 23   // PC8   SDIO_D0
// 24   // PC9   SDIO_D1
// 25   // PA15  SD_CARD_DETECT
// 26   // PC10  SDIO_D2
// 27   // PC11  SDIO_D3
// 28   // PC12  SDIO_CK
// 29   // PD2   SDIO_CMD

#define SOFTWARE_SPI            // Use soft SPI for onboard SD
#define SDSS               27   // PC11  SDIO_D3
#define SCK_PIN            28   // PC12  SDIO_CK
#define MISO_PIN           23   // PC8   SDIO_D0
#define MOSI_PIN           29   // PD2   SDIO_CMD

// OTG
// 30   // PA11  OTG_DM
// 31   // PA12  OTG_DP

// USER_PINS
// 34   // PD7   USER3
// 35   // PB9   USER1
// 36   // PE0   USER2
// 37   // PB4   USER4

// USERKET
// 38   // PE7   USER_BUTTON


//  0   // PA9   TX
//  1   // PA10  RX


// IR/PROBE
// 32   // PD1   IR_OUT
// 33   // PC1   IR_ON


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
