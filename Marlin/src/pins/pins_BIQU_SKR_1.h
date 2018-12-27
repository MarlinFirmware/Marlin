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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * BIGTREETECH - BIQU SKR V1.1 pin assignments
 * 
 * pins_BIQU_SKR_1 (Hotend, Fan, Bed)
 *
 */

#ifndef TARGET_LPC1768
  #error "Oops! Make sure you have the LPC1768 environment selected in your IDE."
#endif

#define BOARD_NAME "BIQU SKR V1.1"

//
// Limit Switches
//
#define X_MIN_PIN          P1_29
#define X_MAX_PIN          P1_28
#define Y_MIN_PIN          P1_27
#define Y_MAX_PIN          P1_26
#define Z_MIN_PIN          P1_25
#define Z_MAX_PIN          P1_24

//
// Steppers
//
#define X_STEP_PIN         P0_04
#define X_DIR_PIN          P0_05
#define X_ENABLE_PIN       P4_28

#define Y_STEP_PIN         P2_01
#define Y_DIR_PIN          P2_02
#define Y_ENABLE_PIN       P2_00

#define Z_STEP_PIN         P0_20
#define Z_DIR_PIN          P0_21
#define Z_ENABLE_PIN       P0_19

#define E0_STEP_PIN        P0_11
#define E0_DIR_PIN         P2_13
#define E0_ENABLE_PIN      P2_12

#define E1_STEP_PIN        P0_01
#define E1_DIR_PIN         P0_00
#define E1_ENABLE_PIN      P0_10

//
// Temperature Sensors
//
#define TEMP_0_PIN          1   // A0 (P0_24)   Analog
#define TEMP_1_PIN          2   // A2 (P0_25)   Analog
#define TEMP_BED_PIN        0   // A1 (P0_23)   Analog

//
// Heaters / Fans
//
#define HEATER_0_PIN       P2_07
#define HEATER_1_PIN       P2_04
#define HEATER_BED_PIN     P2_05
#ifndef FAN_PIN
  #define FAN_PIN          P2_03
#endif

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD)

  #define BEEPER_PIN          P1_31   // EXP2-4

  #define BTN_EN1             P3_26   // EXP2-8
  #define BTN_EN2             P3_25   // EXP2-6
  #define BTN_ENC             P1_23   // EXP2-7

  #define SD_DETECT_PIN       P0_27   // EXP2-7
  #define LCD_PINS_RS         P0_16   // EXP1-7
  #define LCD_PINS_ENABLE     P0_18   // (MOSI) EXP1-8
  #define LCD_PINS_D4         P0_15   // (SCK)  EXP1-6

#endif // ULTRA_LCD

/**
 *   == EXP 1 ==                *#*   == EXP 2 ==
 *                              *#*
 *    #-------#                 *#*    #-------#
 *  1 | °   ° |  2              *#*  1 | °   ° |  2
 *  3 | °   ° |  4              *#*  3 | °   ° |  4
 *  5 | °   ° [  6              *#*  5 | °   ° [  6
 *  7 | °   ° |  8              *#*  7 | °   ° |  8
 *  9 | °   ° | 10              *#*  9 | °   ° | 10
 *    #-------#                 *#*    #-------#
 *                              *#*
 * ############################     ############################
 * # Pin | LPC1768 Pin | Func #     # Pin | LPC1768 Pin | Func #
 * ############################     ############################
 * #  1  |      VCC    |    + #     #  1  |       NC    |      #
 * #  2  |      GND    |    - #     #  2  |      GND    |   -  #
 * #  3  |       NC    |      #     #  3  |    RESET    |      #
 * #  4  |       NC    |      #     #  4  |    P1_31    |   A5 #
 * #  5  |       NC    |      #     #  5  |    P0_18    | MOSI #
 * #  6  |    P0_15    |  SCK #     #  6  |    P3_25    |      #
 * #  7  |    P0_16    | SSEL #     #  7  |    P1_23    |      #
 * #  8  |    P0_18    | MOSI #     #  8  |    P3_26    |      #
 * #  9  |    P2_11    |      #     #  9  |    P0_15    |  SCK #
 * # 10  |    P1_30    |   A4 #     # 10  |    P0_17    | MISO #
 * ############################     ############################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *   == AUX 1 ==
 *
 *    1  3  5  7
 *  #------------#
 *  | °  °  °  ° |
 *  | °  °  °  ° |
 *  #-----||-----#
 *    2  4  6  8
 *
 * ############################
 * # Pin | LPC1768 Pin | Func #
 * ############################
 * #  1  |      VCC    |   +  #
 * #  2  |      VCC    |   +  #
 * #  3  |      GND    |   -  #
 * #  4  |      GND    |   -  #
 * #  5  |    P0_02    |  TX0 #
 * #  6  |       NC    |      #
 * #  7  |    P0_03    |  RX0 #
 * #  8  |       NC    |      #
 * ############################
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * == LCD/SD/SPI ==
 *
 *    1   3   5   7   9  11
 *  | ° | ° | ° | ° | ° | ° |
 *  | ° | ° | ° | ° | ° | ° |
 *    2   4   6   8  10  12
 *
 * ############################
 * # Pin | LPC1768 Pin | Func #
 * ############################
 * #  1  |      GND    |   -  #
 * #  2  |      VCC    |   +  #
 * #  3  |    P0_15    |  SCK #
 * #  4  |    P0_18    | MOSI #
 * #  5  |    P0_16    | SSEL #
 * #  6  |    P2_06    |      #
 * #  7  |    P1_23    |      #
 * #  8  |    P0_17    | MISO #
 * #  9  |    P2_11    |      #
 * # 10  |    P3_25    |      #
 * # 11  |    P1_31    |   A5 #
 * # 12  |    P3_26    |      #
 * ############################
 *
 * Pictogram by Ludy https://github.com/Ludy87
 * See: https://www.biqu.equipment/collections/control-board-kits/products/bigtreetech-skr-v1-1-motherboard-32-bit-smoothieboard-arm-cpu-control-board-open-source-for-tft3-5-printer-pannel
 */
