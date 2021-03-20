/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef STM32F4
  #error "Oops! Select an STM32F4 board in 'Tools > Board.'"
#endif

#ifndef MACHINE_NAME
  #define MACHINE_NAME "STEVAL-3DP001V1"
#endif

#undef  TEMP_TIMER
#define TEMP_TIMER 9 // the Marlin default of timer 7 doesn't exist on the STM32MF401 series

//
// Limit Switches
//
#define X_MIN_PIN          38  // PD8  X_STOP
#define X_MAX_PIN          -1
#define Y_MIN_PIN          39  // PD9  Y_STOP
#define Y_MAX_PIN          -1
#define Z_MIN_PIN          40  // PD10  Z_STOP
#define Z_MAX_PIN          -1

//
// Z Probe (when not Z_MIN_PIN)
//
// #ifndef Z_MIN_PROBE_PIN
//   #define Z_MIN_PROBE_PIN  11  // PA4
// #endif

#define SCK_PIN            16  // PB13   SPI_S
#define MISO_PIN           17  // PB14   SPI_M
#define MOSI_PIN           18  // PB15   SPI_M

//
// Steppers
//

#define X_STEP_PIN         60  // PE14   X_PWM
#define X_DIR_PIN          61  // PE15   X_DIR
#define X_ENABLE_PIN       59  // PE13   X_RES
#define X_CS_PIN           11  // PA4    SPI_CS

#define Y_STEP_PIN         62  // PB10   Y_PWM
#define Y_DIR_PIN          63  // PE9    Y_DIR
#define Y_ENABLE_PIN       64  // PE10   Y_RES
#define Y_CS_PIN           11  // PA4    SPI_CS

#define Z_STEP_PIN         66  // PC6    Z_PWM
#define Z_DIR_PIN          67  // PC0    Z_DIR
#define Z_ENABLE_PIN       65  // PC15   Z_RES
#define Z_CS_PIN           11  // PA4    SPI_CS

#define E0_STEP_PIN        70  // PD12   E1_PW
#define E0_DIR_PIN         68  // PC13   E1_DIR
#define E0_ENABLE_PIN      69  // PC14   E1_RE
#define E0_CS_PIN          11  // PA4    SPI_CS

#define E1_STEP_PIN        72  // PE5    E2_PWM
#define E1_DIR_PIN         73  // PE6    E2_DIR
#define E1_ENABLE_PIN      71  // PE4    E2_RESE
#define E1_CS_PIN          11  // PA4    SPI_CS

#define E2_STEP_PIN        76  // PB8    E3_PWM
#define E2_DIR_PIN         74  // PE2    E3_DIR
#define E2_ENABLE_PIN      75  // PE3    E3_RESE
#define E2_CS_PIN          11  // PA4    SPI_CS


//
// Temperature Sensors
//
#define TEMP_0_PIN         52  // PA0    E1_THER
#define TEMP_1_PIN         53  // PA1    E2_THER
#define TEMP_BED_PIN       50  // PC2    BED_THE

//
// Heaters / Fans
//
#define HEATER_0_PIN       52  // PA0    E1_THER
#define HEATER_1_PIN       53  // PA1    E2_THER
#define HEATER_BED_PIN     50  // PC2    BED_THE

#define FAN_PIN            56  // PC4    E1_FAN   PWM pin, Part cooling fan
#define FAN1_PIN           57  // PC5    E2_FAN   PWM pin, Extruder fan
#define FAN2_PIN           58  // PE8    E3_FAN   PWM pin, Controller fan

//
// Misc functions
//
#define SDSS               11  // PA4    SPI_CS
#define LED_PIN            -1  // Heart beat
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define PWR_LOSS           -1  // Power loss / nAC_FAULT

//
// LCD / Controller
//
//#define SD_DETECT_PIN    24  // PA15   SD_CA
//#define BEEPER_PIN       23  // PC9    SDIO_D1
//#define LCD_PINS_RS      63  // PE9    Y_DIR
//#define LCD_PINS_ENABLE  58  // PE8    E3_FAN
//#define LCD_PINS_D4      15  // PB12   SPI_C
//#define LCD_PINS_D5      16  // PB13   SPI_S
//#define LCD_PINS_D6      17  // PB14   SPI_M
//#define LCD_PINS_D7      18  // PB15   SPI_M
//#define BTN_EN1          56  // PC4    E1_FAN
//#define BTN_EN2          57  // PC5    E2_FAN
//#define BTN_ENC          51  // PC3    BED_THE

//
// Filament runout detection
//
//#define FIL_RUNOUT_PIN   55  // PA3    BED_THE

//
// Extension pins
//
//#define EXT0_PIN         48  // PB0    E2_HEAT
//#define EXT1_PIN         49  // PB1    E3_HEAT
//#define EXT2_PIN             // PB2
//#define EXT3_PIN         38  // PD8    X_STOP
//#define EXT4_PIN         39  // PD9    Y_STOP
//#define EXT5_PIN         40  // PD10   Z_STOP
//#define EXT6_PIN         41  // PD11
//#define EXT7_PIN         70  // PD12   E1_PW
//#define EXT8_PIN         62  // PB10   Y_PWM
//#define EXT9_PIN             // PB11
