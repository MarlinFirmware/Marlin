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

#ifndef __PINMAP_RE_ARM_H__
#define __PINMAP_RE_ARM_H__

// ******************
// Runtime pinmapping
// ******************

#define NUM_ANALOG_INPUTS 8

const adc_pin_data adc_pin_map[] = {
  {0, 23, 0},         //A0 (T0) - D67 - TEMP_0_PIN
  {0, 24, 1},         //A1 (T1) - D68 - TEMP_BED_PIN
  {0, 25, 2},         //A2 (T2) - D69 - TEMP_1_PIN
  {0, 26, 3},         //A3 - D63
  {1, 30, 4},         //A4 - D37 - BUZZER_PIN
  {1, 31, 5},         //A5 - D49 - SD_DETECT_PIN
  {0,  3, 6},         //A6 - D0  - RXD0
  {0,  2, 7}          //A7 - D1  - TXD0
};

#define analogInputToDigitalPin(p) (p == 0 ? 67: \
                                    p == 1 ? 68: \
                                    p == 2 ? 69: \
                                    p == 3 ? 63: \
                                    p == 4 ? 37: \
                                    p == 5 ? 49: \
                                    p == 6 ?  0: \
                                    p == 7 ?  1: -1)

#define DIGITAL_PIN_TO_ANALOG_PIN(p) (p == 67 ? 0: \
                                      p == 68 ? 1: \
                                      p == 69 ? 2: \
                                      p == 63 ? 3: \
                                      p == 37 ? 4: \
                                      p == 49 ? 5: \
                                      p == 0  ? 6: \
                                      p == 1  ? 7: -1)

#define NUM_DIGITAL_PINS 80

#define VALID_PIN(r)  (r <   0 ? 0 :\
                       r ==  7 ? 0 :\
                       r == 17 ? 0 :\
                       r == 22 ? 0 :\
                       r == 23 ? 0 :\
                       r == 25 ? 0 :\
                       r == 27 ? 0 :\
                       r == 29 ? 0 :\
                       r == 32 ? 0 :\
                       r == 39 ? 0 :\
                       r == 40 ? 0 :\
                       r == 42 ? 0 :\
                       r == 43 ? 0 :\
                       r == 44 ? 0 :\
                       r == 45 ? 0 :\
                       r == 47 ? 0 :\
                       r == 64 ? 0 :\
                       r == 65 ? 0 :\
                       r == 66 ? 0 :\
                       r >= NUM_DIGITAL_PINS ? 0 : 1)

#define PWM_PIN(r)  (r <   0 ? 0 :\
                       r ==  3 ? 1 :\
                       r ==  4 ? 1 :\
                       r ==  6 ? 1 :\
                       r ==  9 ? 1 :\
                       r == 10 ? 1 :\
                       r == 11 ? 1 :\
                       r == 14 ? 1 :\
                       r == 26 ? 1 :\
                       r == 46 ? 1 :\
                       r == 53 ? 1 :\
                       r == 54 ? 1 :\
                       r == 60 ? 1 : 0)

#define NUM_INTERRUPT_PINS 35

#define INTERRUPT_PIN(r)  ( r< 0  ? 0 :\
                        r == 0  ? 1 :\
                        r == 1  ? 1 :\
                        r == 8  ? 1 :\
                        r == 9  ? 1 :\
                        r == 10 ? 1 :\
                        r == 12 ? 1 :\
                        r == 16 ? 1 :\
                        r == 20 ? 1 :\
                        r == 21 ? 1 :\
                        r == 24 ? 1 :\
                        r == 25 ? 1 :\
                        r == 26 ? 1 :\
                        r == 28 ? 1 :\
                        r == 34 ? 1 :\
                        r == 35 ? 1 :\
                        r == 36 ? 1 :\
                        r == 38 ? 1 :\
                        r == 46 ? 1 :\
                        r == 48 ? 1 :\
                        r == 50 ? 1 :\
                        r == 51 ? 1 :\
                        r == 52 ? 1 :\
                        r == 54 ? 1 :\
                        r == 55 ? 1 :\
                        r == 56 ? 1 :\
                        r == 57 ? 1 :\
                        r == 58 ? 1 :\
                        r == 59 ? 1 :\
                        r == 60 ? 1 :\
                        r == 61 ? 1 :\
                        r == 62 ? 1 :\
                        r == 63 ? 1 :\
                        r == 67 ? 1 :\
                        r == 68 ? 1 :\
                        r == 69 ? 1 : 0)
                        /*Internal SD Card */
                        /*r == 80 ? 1 :\
                        r == 81 ? 1 :\
                        r == 82 ? 1 :\
                        r == 83 ? 1 :\*/

const pin_data pin_map[] = { // pin map for variable pin function
  {0,3},        //  DIO0   RXD0             A6               J4-4                           AUX-1
  {0,2},        //  DIO1   TXD0             A7               J4-5                           AUX-1
  {1,25},       //  DIO2   X_MAX_PIN                          10K PULLUP TO 3.3v, 1K SERIES
  {1,24},       //  DIO3   X_MIN_PIN                          10K PULLUP TO 3.3v, 1K SERIES
  {1,18},       //  DIO4   SERVO3_PIN        FIL_RUNOUT_PIN   5V output, PWM
  {1,19},       //  DIO5   SERVO2_PIN
  {1,21},       //  DIO6   SERVO1_PIN       J5-1
  {0xFF,0xFF},  //  DIO7   N/C
  {2,7},        //  DIO8   RAMPS_D8_PIN
  {2,4},        //  DIO9   RAMPS_D9_PIN     PWM
  {2,5},        //  DIO10  RAMPS_D10_PIN    PWM
  {1,20},       //  DIO11  SERVO0_PIN
  {2,12},       //  DIO12  PS_ON_PIN
  {4,28},       //  DIO13  LED_PIN
  {1,26},       //  DIO14  Y_MIN_PIN                          10K PULLUP TO 3.3v, 1K SERIES
  {1,27},       //  DIO15  Y_MAX_PIN                          10K PULLUP TO 3.3v, 1K SERIES
  {0,16},       //  DIO16  LCD_PINS_RS      J3-7
  {0xFF,0xFF},  //  DIO17  LCD_PINS_ENABLE   MOSI_PIN(MOSI0)  J3-10                          AUX-3
  {1,29},       //  DIO18  Z_MIN_PIN                          10K PULLUP TO 3.3v, 1K SERIES
  {1,28},       //  DIO19  Z_MAX_PIN                          10K PULLUP TO 3.3v, 1K SERIES
  {0,0},        //  DIO20  SCA
  {0,1},        //  DIO21  SCL
  {0xFF,0xFF},  //  DIO22  N/C
  {0xFF,0xFF},  //  DIO23  LCD_PINS_D4       SCK_PIN(SCLK0)   J3-9                           AUX-3
  {0,4},        //  DIO24  E0_ENABLE_PIN
  {0xFF,0xFF},  //  DIO25  N/C
  {2,0},        //  DIO26  E0_STEP_PIN
  {0xFF,0xFF},  //  DIO27  N/C
  {0,5},        //  DIO28  E0_DIR_PIN
  {0xFF,0xFF},  //  DIO29  N/C
  {4,29},       //  DIO30  E1_ENABLE_PIN
  {3,26},       //  DIO31  BTN_EN1
  {0xFF,0xFF},  //  DIO32  N/C
  {3,25},       //  DIO33  BTN_EN2          J3-4
  {2,13},       //  DIO34  E1_DIR_PIN
  {2,11},       //  DIO35  BTN_ENC          J3-3
  {2,8},        //  DIO36  E1_STEP_PIN
  {1,30},       //  DIO37  BEEPER_PIN       A4                                              not 5V tolerant
  {0,10},       //  DIO38  X_ENABLE_PIN
  {0xFF,0xFF},  //  DIO39  N/C
  {0xFF,0xFF},  //  DIO40  N/C
  {1,22},       //  DIO41  KILL_PIN         J5-4
  {0xFF,0xFF},  //  DIO42  N/C
  {0xFF,0xFF},  //  DIO43  N/C
  {0xFF,0xFF},  //  DIO44  N/C
  {0xFF,0xFF},  //  DIO45  N/C
  {2,3},        //  DIO46  Z_STEP_PIN
  {0xFF,0xFF},  //  DIO47  N/C
  {0,22},       //  DIO48  Z_DIR_PIN
  {1,31},       //  DIO49  SD_DETECT_PIN    A5               J3-1                           not 5V tolerant
  {0,17},       //  DIO50  MISO_PIN(MISO0)                                                   AUX-3
  {0,18},       //  DIO51  MOSI_PIN(MOSI0)   LCD_PINS_ENABLE  J3-10                          AUX-3
  {0,15},       //  DIO52  SCK_PIN(SCLK0)    LCD_PINS_D4      J3-9                           AUX-3
  {1,23},       //  DIO53  SDSS(SSEL0)      J3-5                                             AUX-3
  {2,1},        //  DIO54  X_STEP_PIN
  {0,11},       //  DIO55  X_DIR_PIN
  {0,19},       //  DIO56  Y_ENABLE_PIN
  {0,27},       //  DIO57                   AUX-1             open collector
  {0,28},       //  DIO58                   AUX-1             open collector
  {2,6},        //  DIO59  LCD_A0           J3-8                                             AUX-2
  {2,2},        //  DIO60  Y_STEP_PIN
  {0,20},       //  DIO61  Y_DIR_PIN
  {0,21},       //  DIO62  Z_ENABLE_PIN
  {0,26},       //  DIO63  AUX-2             A3               J5-3                           AUX-2
  {0xFF,0xFF},  //  DIO64  N/C
  {0xFF,0xFF},  //  DIO65  N/C
  {0xFF,0xFF},  //  DIO66  N/C
  {0,23},       //  DIO67  TEMP_0_PIN       A0
  {0,24},       //  DIO68  TEMP_BED_PIN     A1
  {0,25},       //  DIO69  TEMP_1_PIN       A2
  {1,16},       //  DIO70                   J12-3             ENET_MOC
  {1,17},       //  DIO71                   J12-4             ENET_MDIO
  {1,15},       //  DIO72                   J12-5             REF_CLK
  {1,14},       //  DIO73                   J12-6             ENET_RX_ER
  {1,9},        //  DIO74                   J12-7             ENET_RXD0
  {1,10},       //  DIO75                   J12-8             ENET_RXD1
  {1,8},        //  DIO76                   J12-9             ENET_CRS
  {1,4},        //  DIO77                   J12-10            ENET_TX_EN
  {1,0},        //  DIO78                   J12-11            ENET_TXD0
  {1,1},        //  DIO79                   J12-12            ENET_TXD1
//  {0,6},        //  DIO80  SD-CS            on board SD card
//  {0,7},        //  DIO81  SD-SCK           on board SD card
//  {0,8},        //  DIO82  SD-MISO          on board SD card
//  {0,9},        //  DIO83  SD-MOSI          on board SD card

};

// ***********************
// Preprocessor pinmapping
// ***********************

//#define RXD0                      0  //              A16               J4-4                           AUX-1
#define DIO0_PORT 0
#define DIO0_PIN 3
//#define TXD0                      1  //              A17               J4-5                           AUX-1
#define DIO1_PORT 0
#define DIO1_PIN 2
//#define X_MAX_PIN                 2  //                           10K PULLUP TO 3.3v, 1K SERIES
#define DIO2_PORT 1
#define DIO2_PIN 25
//#define X_MIN_PIN                 3  //                           10K PULLUP TO 3.3v, 1K SERIES
#define DIO3_PORT 1
#define DIO3_PIN 24
//#define SERVO3_PIN                4  //         FIL_RUNOUT_PIN   5V output, PWM
#define DIO4_PORT 1
#define DIO4_PIN 18
//#define SERVO2_PIN                5  //
#define DIO5_PORT 1
#define DIO5_PIN 19
//#define SERVO1_PIN                6  //        J5-1
#define DIO6_PORT 1
#define DIO6_PIN 21
//#define RAMPS_D8_PIN              8  //
#define DIO8_PORT 2
#define DIO8_PIN 7
//#define RAMPS_D9_PIN              9  //      PWM
#define DIO9_PORT 2
#define DIO9_PIN 4
//#define RAMPS_D10_PIN             10  //     PWM
#define DIO10_PORT 2
#define DIO10_PIN 5
//#define SERVO0_PIN                11  //
#define DIO11_PORT 1
#define DIO11_PIN 20
//#define PS_ON_PIN                 12  //
#define DIO12_PORT 2
#define DIO12_PIN 12
//#define LED_PIN                   13  //
#define DIO13_PORT 4
#define DIO13_PIN 28
//#define Y_MIN_PIN                 14  //                           10K PULLUP TO 3.3v, 1K SERIES
#define DIO14_PORT 1
#define DIO14_PIN 26
//#define Y_MAX_PIN                 15  //                           10K PULLUP TO 3.3v, 1K SERIES
#define DIO15_PORT 1
#define DIO15_PIN 27
//#define LCD_PINS_RS               16  //       J3-7
#define DIO16_PORT 0
#define DIO16_PIN 16
//#define Z_MIN_PIN                 18  //                           10K PULLUP TO 3.3v, 1K SERIES
#define DIO18_PORT 1
#define DIO18_PIN 29
//#define Z_MAX_PIN                 19  //                           10K PULLUP TO 3.3v, 1K SERIES
#define DIO19_PORT 1
#define DIO19_PIN 28
//#define SCA                       20  //
#define DIO20_PORT 0
#define DIO20_PIN 0
//#define SCL                       21  //
#define DIO21_PORT 0
#define DIO21_PIN 1
//#define E0_ENABLE_PIN             24  //
#define DIO24_PORT 0
#define DIO24_PIN 4
//#define E0_STEP_PIN               26  //
#define DIO26_PORT 2
#define DIO26_PIN 0
//#define E0_DIR_PIN                28  //
#define DIO28_PORT 0
#define DIO28_PIN 5
//#define E1_ENABLE_PIN             30  //
#define DIO30_PORT 4
#define DIO30_PIN 29
//#define BTN_EN1                   31  //
#define DIO31_PORT 3
#define DIO31_PIN 26
//#define BTN_EN2                   33  //           J3-4
#define DIO33_PORT 3
#define DIO33_PIN 25
//#define E1_DIR_PIN                34  //
#define DIO34_PORT 2
#define DIO34_PIN 13
//#define BTN_ENC                   35  //           J3-3
#define DIO35_PORT 2
#define DIO35_PIN 11
//#define E1_STEP_PIN               36  //
#define DIO36_PORT 2
#define DIO36_PIN 8
//#define BEEPER_PIN                37  //        A18                                              not 5V tolerant
#define DIO37_PORT 1
#define DIO37_PIN 30
//#define X_ENABLE_PIN              38  //
#define DIO38_PORT 0
#define DIO38_PIN 10
//#define KILL_PIN                  41  //          J5-4
#define DIO41_PORT 1
#define DIO41_PIN 22
//#define Z_STEP_PIN                46  //
#define DIO46_PORT 2
#define DIO46_PIN 3
//#define Z_DIR_PIN                 48  //
#define DIO48_PORT 0
#define DIO48_PIN 22
//#define SD_DETECT_PIN             49  //     A19               J3-1                           not 5V tolerant
#define DIO49_PORT 1
#define DIO49_PIN 31
//#define MISO_PIN(MISO0)           50  //                                                    AUX-3
#define DIO50_PORT 0
#define DIO50_PIN 17
//#define MOSI_PIN(MOSI0)           51  //    LCD_PINS_ENABLE  J3-10                          AUX-3
#define DIO51_PORT 0
#define DIO51_PIN 18
//#define SCK_PIN(SCLK0)            52  //     LCD_PINS_D4      J3-9                           AUX-3
#define DIO52_PORT 0
#define DIO52_PIN 15
//#define SDSS(SSEL0)               53  //       J3-5                                             AUX-3
#define DIO53_PORT 1
#define DIO53_PIN 23
//#define X_STEP_PIN                54  //
#define DIO54_PORT 2
#define DIO54_PIN 1
//#define X_DIR_PIN                 55  //
#define DIO55_PORT 0
#define DIO55_PIN 11
//#define Y_ENABLE_PIN              56  //
#define DIO56_PORT 0
#define DIO56_PIN 19
//#define AUX-1                     57  //              open collector
#define DIO57_PORT 0
#define DIO57_PIN 27
//#define AUX-1                     58  //              open collector
#define DIO58_PORT 0
#define DIO58_PIN 28
//#define LCD_A0                    59  //            J3-8                                             AUX-2
#define DIO59_PORT 2
#define DIO59_PIN 6
//#define Y_STEP_PIN                60  //
#define DIO60_PORT 2
#define DIO60_PIN 2
//#define Y_DIR_PIN                 61  //
#define DIO61_PORT 0
#define DIO61_PIN 20
//#define Z_ENABLE_PIN              62  //
#define DIO62_PORT 0
#define DIO62_PIN 21
//#define AUX-2                     63  //              A9               J5-3                           AUX-2
#define DIO63_PORT 0
#define DIO63_PIN 26
//#define TEMP_0_PIN                67  //        A13
#define DIO67_PORT 0
#define DIO67_PIN 23
//#define TEMP_BED_PIN              68  //      A14
#define DIO68_PORT 0
#define DIO68_PIN 24
//#define TEMP_1_PIN                69  //        A15
#define DIO69_PORT 0
#define DIO69_PIN 25
//#define J12-3                     70  //              ENET_MOC
#define DIO70_PORT 1
#define DIO70_PIN 16
//#define J12-4                     71  //              ENET_MDIO
#define DIO71_PORT 1
#define DIO71_PIN 17
//#define J12-5                     72  //              REF_CLK
#define DIO72_PORT 1
#define DIO72_PIN 15
//#define J12-6                     73  //              ENET_RX_ER
#define DIO73_PORT 1
#define DIO73_PIN 14
//#define J12-7                     74  //              ENET_RXD0
#define DIO74_PORT 1
#define DIO74_PIN 9
//#define J12-8                     75  //              ENET_RXD1
#define DIO75_PORT 1
#define DIO75_PIN 10
//#define J12-9                     76  //              ENET_CRS
#define DIO76_PORT 1
#define DIO76_PIN 8
//#define J12-10                    77  //             ENET_TX_EN
#define DIO77_PORT 1
#define DIO77_PIN 4
//#define J12-11                    78  //             ENET_TXD0
#define DIO78_PORT 1
#define DIO78_PIN 0
//#define J12-12                    79  //             ENET_TXD1
#define DIO79_PORT 1
#define DIO79_PIN 1
/**
//#define SD-CS                     80  //             on board SD card
#define DIO80_PORT 0
#define DIO80_PIN 6
//#define SD-SCK                    81  //            on board SD card
#define DIO81_PORT 0
#define DIO81_PIN 7
//#define SD-MISO                   82  //           on board SD card
#define DIO82_PORT 0
#define DIO82_PIN 8
//#define SD-MOSI                   83  //           on board SD card
#define DIO83_PORT 0
#define DIO83_PIN 9
*/

#endif //__PINMAP_RE_ARM_H__
