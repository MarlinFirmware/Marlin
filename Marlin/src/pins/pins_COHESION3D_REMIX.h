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

/**
 * Cohesion3D ReMix pin assignments
 */

#ifndef LPC1769
  #error "Oops! Make sure you have the LPC1769 environment selected in your IDE."
#endif

#define BOARD_NAME "Cohesion3D ReMix"

//
// Servos
//
#define SERVO0_PIN         P2_04

//
// Limit Switches
//
#define X_MIN_PIN          P1_24   // 10k pullup to 3.3V
#define X_MAX_PIN          P1_25   // 10k pullup to 3.3V
#define Y_MIN_PIN          P1_26   // 10k pullup to 3.3V
#define Y_MAX_PIN          P1_27   // 10k pullup to 3.3V
#define Z_MIN_PIN          P1_28   // 10k pullup to 3.3V
#define Z_MAX_PIN          P1_29   // 10k pullup to 3.3V

//
// Steppers
//
#define X_STEP_PIN         P2_00
#define X_DIR_PIN          P0_05
#define X_ENABLE_PIN       P0_04
#define X_CS_PIN           P1_10   // Ethernet Expansion - Pin 9

#define Y_STEP_PIN         P2_01
#define Y_DIR_PIN          P0_11
#define Y_ENABLE_PIN       P0_10
#define Y_CS_PIN           P1_09   // Ethernet Expansion - Pin 10

#define Z_STEP_PIN         P2_02
#define Z_DIR_PIN          P0_20
#define Z_ENABLE_PIN       P0_19
#define Z_CS_PIN           P1_00   // Ethernet Expansion - Pin 11

#define E0_STEP_PIN        P2_03
#define E0_DIR_PIN         P0_22
#define E0_ENABLE_PIN      P0_21
#define E0_CS_PIN          P1_04   // Ethernet Expansion - Pin 12

#define E1_STEP_PIN        P2_08
#define E1_DIR_PIN         P2_13
#define E1_ENABLE_PIN      P4_29
#define E1_CS_PIN          P1_01   // Ethernet Expansion - Pin 14

#define E2_STEP_PIN        P1_20
#define E2_DIR_PIN         P1_19
#define E2_ENABLE_PIN      P1_21
#define E2_CS_PIN          P1_18   // FET 6

//
// Default pins for TMC software SPI
//
#if ENABLED(TMC_USE_SW_SPI)
  #ifndef TMC_SW_MOSI
    #define TMC_SW_MOSI    P1_16   // Ethernet Expansion - Pin 5
  #endif
  #ifndef TMC_SW_MISO
    #define TMC_SW_MISO    P1_17   // Ethernet Expansion - Pin 6
  #endif
  #ifndef TMC_SW_SCK
    #define TMC_SW_SCK     P1_08   // Ethernet Expansion - Pin 7
  #endif
#endif

//
// Analog Inputs
//  3.3V max when defined as an analog input
//
#define TEMP_0_PIN          0   // P0_23
#define TEMP_BED_PIN        1   // P0_24
#define TEMP_1_PIN          2   // P0_25
#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #define FILWIDTH_PIN      3   // P0_26
#else
  #define TEMP_2_PIN        3   // P0_26
#endif

//
// Heaters / Fans
//
#define HEATER_BED_PIN     P2_05
#define HEATER_0_PIN       P2_07   // FET 1
#define HEATER_1_PIN       P1_23   // FET 2
#define HEATER_2_PIN       P1_22   // FET 3
#ifndef FAN_PIN
  #define FAN_PIN          P2_06   // FET 4
#endif

//
// Auto fans
//
#if HOTENDS == 3
  #define AUTO_FAN_PIN     P1_18   // FET 6
#else
  #define AUTO_FAN_PIN     P1_22   // FET 3
#endif
#define ORIG_E0_AUTO_FAN_PIN  AUTO_FAN_PIN
#define ORIG_E1_AUTO_FAN_PIN  AUTO_FAN_PIN
#define ORIG_E2_AUTO_FAN_PIN  AUTO_FAN_PIN

//
// Misc. Functions
//
#define LED_PIN            P4_28   // Play LED

//
// M3/M4/M5 - Spindle/Laser Control
//
#if ENABLED(SPINDLE_LASER_ENABLE)
  #undef HEATER_0_PIN
  #define SPINDLE_LASER_ENA_PIN     P2_07   // FET 1
  #undef HEATER_BED_PIN
  #define SPINDLE_LASER_PWM_PIN     P2_05   // Bed FET
  #undef FAN_PIN
  #define SPINDLE_DIR_PIN           P2_06   // FET 4
#endif

//
// LCD / Controller
//
// LCD_PINS_D5, D6, and D7 are not present in the EXP1 connector, and will need to be
// defined to use the REPRAP_DISCOUNT_SMART_CONTROLLER.
//
// A remote SD card is currently not supported because the pins routed to the EXP2
// connector are shared with the onboard SD card, and Marlin does not support reading
// G-code files from the onboard SD card.
//
#if ENABLED(ULTRA_LCD)

  #define BEEPER_PIN       P1_31   // EXP1-1
  #define SD_DETECT_PIN    P0_27   // EXP2-7

  #define BTN_EN1          P3_26   // EXP2-5
  #define BTN_EN2          P3_25   // EXP2-3
  #define BTN_ENC          P1_30   // EXP1-2

  #define LCD_PINS_RS      P0_16   // EXP1-4
  #define LCD_SDSS         P0_28   // EXP2-4
  #define LCD_PINS_ENABLE  P0_18   // EXP1-3
  #define LCD_PINS_D4      P0_15   // EXP1-5

  #define KILL_PIN         P2_11   // EXP2-10

  #if ENABLED(SDSUPPORT)
    #error "SDSUPPORT is not currently supported by the Cohesion3D boards"
  #endif

#endif // ULTRA_LCD

//
// Ethernet pins
//
#define ENET_MDIO          P1_17
#define ENET_RX_ER         P1_14
#define ENET_RXD1          P1_10
#define ENET_MOC           P1_16
#define REF_CLK            P1_15
#define ENET_RXD0          P1_09
#define ENET_CRS           P1_08
#define ENET_TX_EN         P1_04
#define ENET_TXD0          P1_00
#define ENET_TXD1          P1_01
