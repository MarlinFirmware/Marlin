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

#ifndef BOARDS_H
#define BOARDS_H

#define BOARD_UNKNOWN -1

//
// RAMPS 1.3 / 1.4 - ATmega1280, ATmega2560
//

#define BOARD_RAMPS_OLD         3     // MEGA/RAMPS up to 1.2

#define BOARD_RAMPS_13_EFB      33    // RAMPS 1.3 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_13_EEB      34    // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_13_EFF      35    // RAMPS 1.3 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_13_EEF      36    // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_13_SF       38    // RAMPS 1.3 (Power outputs: Spindle, Controller Fan)

#define BOARD_RAMPS_14_EFB      43    // RAMPS 1.4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_14_EEB      44    // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_14_EFF      45    // RAMPS 1.4 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_14_EEF      46    // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_14_SF       48    // RAMPS 1.4 (Power outputs: Spindle, Controller Fan)

#define BOARD_RAMPS_PLUS_EFB   143    // RAMPS Plus 3DYMY (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_PLUS_EEB   144    // RAMPS Plus 3DYMY (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_PLUS_EFF   145    // RAMPS Plus 3DYMY (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_PLUS_EEF   146    // RAMPS Plus 3DYMY (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_PLUS_SF    148    // RAMPS Plus 3DYMY (Power outputs: Spindle, Controller Fan)

//
// RAMPS Derivatives - ATmega1280, ATmega2560
//

#define BOARD_3DRAG             77    // 3Drag Controller
#define BOARD_K8200             78    // Velleman K8200 Controller (derived from 3Drag Controller)
#define BOARD_K8400             79    // Velleman K8400 Controller (derived from 3Drag Controller)
#define BOARD_BAM_DICE          401   // 2PrintBeta BAM&DICE with STK drivers
#define BOARD_BAM_DICE_DUE      402   // 2PrintBeta BAM&DICE Due with STK drivers
#define BOARD_MKS_BASE          40    // MKS BASE v1.0
#define BOARD_MKS_BASE_15       405   // MKS v1.5 with Allegro A4982 stepper drivers
#define BOARD_MKS_BASE_HEROIC   41    // MKS BASE 1.0 with Heroic HR4982 stepper drivers
#define BOARD_MKS_GEN_13        47    // MKS GEN v1.3 or 1.4
#define BOARD_MKS_GEN_L         53    // MKS GEN L
#define BOARD_KFB_2             136   // Bigtreetech or BIQU KFB2.0
#define BOARD_ZRIB_V20          504   // zrib V2.0 control board (Chinese knock off RAMPS replica)
#define BOARD_FELIX2            37    // Felix 2.0+ Electronics Board (RAMPS like)
#define BOARD_RIGIDBOARD        42    // Invent-A-Part RigidBoard
#define BOARD_RIGIDBOARD_V2     52    // Invent-A-Part RigidBoard V2
#define BOARD_SAINSMART_2IN1    49    // Sainsmart 2-in-1 board
#define BOARD_ULTIMAKER         7     // Ultimaker
#define BOARD_ULTIMAKER_OLD     71    // Ultimaker (Older electronics. Pre 1.5.4. This is rare)
#define BOARD_AZTEEG_X3         67    // Azteeg X3
#define BOARD_AZTEEG_X3_PRO     68    // Azteeg X3 Pro
#define BOARD_ULTIMAIN_2        72    // Ultimainboard 2.x (Uses TEMP_SENSOR 20)
#define BOARD_RUMBA             80    // Rumba
#define BOARD_BQ_ZUM_MEGA_3D    503   // bq ZUM Mega 3D
#define BOARD_MAKEBOARD_MINI    431   // MakeBoard Mini v2.1.2 is a control board sold by MicroMake
#define BOARD_TRIGORILLA_13     343   // TriGorilla Anycubic version 1.3 based on RAMPS EFB
#define BOARD_TRIGORILLA_14     443   // TriGorilla Anycubic version 1.4 based on RAMPS EFB
#define BOARD_RAMPS_ENDER_4     243   // Creality: Ender-4, CR-8

//
// Other ATmega1280, ATmega2560
//

#define BOARD_CNCONTROLS_11     111   // Cartesio CN Controls V11
#define BOARD_CNCONTROLS_12     112   // Cartesio CN Controls V12
#define BOARD_CHEAPTRONIC       2     // Cheaptronic v1.0
#define BOARD_CHEAPTRONIC_V2    21    // Cheaptronic v2.0
#define BOARD_MIGHTYBOARD_REVE  200   // Makerbot Mightyboard Revision E
#define BOARD_MEGATRONICS       70    // Megatronics
#define BOARD_MEGATRONICS_2     701   // Megatronics v2.0
#define BOARD_MEGATRONICS_3     703   // Megatronics v3.0
#define BOARD_MEGATRONICS_31    704   // Megatronics v3.1
#define BOARD_RAMBO             301   // Rambo
#define BOARD_MINIRAMBO         302   // Mini-Rambo
#define BOARD_MINIRAMBO_10A     303   // Mini-Rambo 1.0a
#define BOARD_EINSY_RAMBO       304   // Einsy Rambo
#define BOARD_EINSY_RETRO       305   // Einsy Retro
#define BOARD_ELEFU_3           21    // Elefu Ra Board (v3)
#define BOARD_LEAPFROG          999   // Leapfrog
#define BOARD_MEGACONTROLLER    310   // Mega controller
#define BOARD_SCOOVO_X9H        321   // abee Scoovo X9H
#define BOARD_GT2560_REV_A      74    // Geeetech GT2560 Rev. A
#define BOARD_GT2560_REV_A_PLUS 75    // Geeetech GT2560 Rev. A+ (with auto level probe)
#define BOARD_EINSTART_S        666   // Einstart retrofit

//
// ATmega1281, ATmega2561
//

#define BOARD_MINITRONICS       702   // Minitronics v1.0/1.1
#define BOARD_SILVER_GATE       25    // Silvergate v1.0

//
// Sanguinololu and Derivatives - ATmega644P, ATmega1284P
//

#define BOARD_SANGUINOLOLU_11   6     // Sanguinololu < 1.2
#define BOARD_SANGUINOLOLU_12   62    // Sanguinololu 1.2 and above
#define BOARD_MELZI             63    // Melzi
#define BOARD_MELZI_MAKR3D      66    // Melzi with ATmega1284 (MaKr3d version)
#define BOARD_MELZI_CREALITY    89    // Melzi Creality3D board (for CR-10 etc)
#define BOARD_MELZI_MALYAN      92    // Melzi Malyan M150 board
#define BOARD_MELZI_TRONXY      505   // Tronxy X5S
#define BOARD_STB_11            64    // STB V1.1
#define BOARD_AZTEEG_X1         65    // Azteeg X1
#define BOARD_ANET_10           69    // Anet 1.0 (Melzi clone)

//
// Other ATmega644P, ATmega644, ATmega1284P
//

#define BOARD_GEN3_MONOLITHIC   22    // Gen3 Monolithic Electronics
#define BOARD_GEN3_PLUS         9     // Gen3+
#define BOARD_GEN6              5     // Gen6
#define BOARD_GEN6_DELUXE       51    // Gen6 deluxe
#define BOARD_GEN7_CUSTOM       10    // Gen7 custom (Alfons3 Version) "https://github.com/Alfons3/Generation_7_Electronics"
#define BOARD_GEN7_12           11    // Gen7 v1.1, v1.2
#define BOARD_GEN7_13           12    // Gen7 v1.3
#define BOARD_GEN7_14           13    // Gen7 v1.4
#define BOARD_OMCA_A            90    // Alpha OMCA board
#define BOARD_OMCA              91    // Final OMCA board
#define BOARD_SETHI             20    // Sethi 3D_1

//
// Teensyduino - AT90USB1286, AT90USB1286P
//

#define BOARD_TEENSYLU          8     // Teensylu
#define BOARD_PRINTRBOARD       81    // Printrboard (AT90USB1286)
#define BOARD_PRINTRBOARD_REVF  811   // Printrboard Revision F (AT90USB1286)
#define BOARD_BRAINWAVE         82    // Brainwave (AT90USB646)
#define BOARD_BRAINWAVE_PRO     85    // Brainwave Pro (AT90USB1286)
#define BOARD_SAV_MKI           83    // SAV Mk-I (AT90USB1286)
#define BOARD_TEENSY2           84    // Teensy++2.0 (AT90USB1286)
#define BOARD_5DPRINT           88    // 5DPrint D8 Driver Board

//
// LPC1768 ARM Cortex M3
//

#define BOARD_RAMPS_14_RE_ARM_EFB 1743  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_14_RE_ARM_EEB 1744  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_14_RE_ARM_EFF 1745  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_14_RE_ARM_EEF 1746  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_14_RE_ARM_SF  1748  // Re-ARM with RAMPS 1.4 (Power outputs: Spindle, Controller Fan)
#define BOARD_MKS_SBASE           1750  // MKS-Sbase (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_AZSMZ_MINI          1751  // AZSMZ Mini
#define BOARD_AZTEEG_X5_GT        1752  // Azteeg X5 GT (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_BIQU_BQ111_A4       1753  // BIQU BQ111-A4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_SELENA_COMPACT      1754  // Selena Compact (Power outputs: Hotend0, Hotend1, Bed0, Bed1, Fan0, Fan1)
#define BOARD_COHESION3D_REMIX    1755  // Cohesion3D ReMix
#define BOARD_COHESION3D_MINI     1756  // Cohesion3D Mini
#define BOARD_SMOOTHIEBOARD       1757  // Smoothieboard

//
// SAM3X8E ARM Cortex M3
//

#define BOARD_DUE3DOM          1411   // DUE3DOM for Arduino DUE
#define BOARD_DUE3DOM_MINI     1412   // DUE3DOM MINI for Arduino DUE
#define BOARD_RADDS            1502   // RADDS
#define BOARD_RAMPS_FD_V1      1503   // RAMPS-FD v1
#define BOARD_RAMPS_FD_V2      1504   // RAMPS-FD v2
#define BOARD_RAMPS_SMART_EFB  1523   // RAMPS-SMART (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_SMART_EEB  1524   // RAMPS-SMART (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_SMART_EFF  1525   // RAMPS-SMART (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_SMART_EEF  1526   // RAMPS-SMART (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_SMART_SF   1528   // RAMPS-SMART (Power outputs: Spindle, Controller Fan)
#define BOARD_RAMPS_DUO_EFB    1533   // RAMPS Duo (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_DUO_EEB    1534   // RAMPS Duo (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_DUO_EFF    1535   // RAMPS Duo (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_DUO_EEF    1536   // RAMPS Duo (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_DUO_SF     1538   // RAMPS Duo (Power outputs: Spindle, Controller Fan)
#define BOARD_RAMPS4DUE_EFB    1543   // RAMPS4DUE (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS4DUE_EEB    1544   // RAMPS4DUE (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS4DUE_EFF    1545   // RAMPS4DUE (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS4DUE_EEF    1546   // RAMPS4DUE (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS4DUE_SF     1548   // RAMPS4DUE (Power outputs: Spindle, Controller Fan)
#define BOARD_RURAMPS4D_11     1550   // RuRAMPS4Duo v1.1 (Power outputs: Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
#define BOARD_RURAMPS4D_13     1551   // RuRAMPS4Duo v1.3 (Power outputs: Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
#define BOARD_ULTRATRONICS_PRO 1560   // ReprapWorld Ultratronics Pro V1.0
#define BOARD_ARCHIM2          1590   // UltiMachine Archim2 (with TMC2130 drivers)
#define BOARD_ALLIGATOR        1602   // Alligator Board R2

//
// STM32 ARM Cortex-M3
//

#define BOARD_STM32F1R         1800   // STM32R Libmaple based STM32F1 controller
#define BOARD_MALYAN_M200      1801   // STM32C8T6 Libmaple based stm32f1 controller
#define BOARD_STM3R_MINI       1803   // STM32 Libmaple based stm32f1 controller
#define BOARD_GTM32_PRO_VB     1805   // STM32f103VET6 controller

//
// STM32 ARM Cortex-M4F
//

#define BOARD_TEENSY35_36       841   // Teensy3.5 and Teensy3.6
#define BOARD_BEAST            1802   // STM32FxxxVxT6 Libmaple based stm32f4 controller
#define BOARD_STM32F4          1804   // STM32 STM32GENERIC based STM32F4 controller

//
// ARM Cortex M7
//

#define BOARD_THE_BORG         1860   // THE-BORG (Power outputs: Hotend0, Hotend1, Bed, Fan)

//
// Espressif ESP32 WiFi
//
#define BOARD_ESP32            1900

#define MB(board) (defined(BOARD_##board) && MOTHERBOARD==BOARD_##board)

#endif // __BOARDS_H
