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

#define BOARD_GEN7_CUSTOM       10   // Gen7 custom (Alfons3 Version) "https://github.com/Alfons3/Generation_7_Electronics"
#define BOARD_GEN7_12           11   // Gen7 v1.1, v1.2
#define BOARD_GEN7_13           12   // Gen7 v1.3
#define BOARD_GEN7_14           13   // Gen7 v1.4
#define BOARD_CNCONTROLS_11     111  // Cartesio CN Controls V11
#define BOARD_CNCONTROLS_12     112  // Cartesio CN Controls V12
#define BOARD_CHEAPTRONIC       2    // Cheaptronic v1.0
#define BOARD_SETHI             20   // Sethi 3D_1
#define BOARD_RAMPS_OLD         3    // MEGA/RAMPS up to 1.2
#define BOARD_RAMPS_13_EFB      33   // RAMPS 1.3 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_13_EEB      34   // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_13_EFF      35   // RAMPS 1.3 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_13_EEF      36   // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_13_SF       38   // RAMPS 1.3 (Power outputs: Spindle, Controller Fan)
#define BOARD_FELIX2            37   // Felix 2.0+ Electronics Board (RAMPS like)
#define BOARD_RIGIDBOARD        42   // Invent-A-Part RigidBoard
#define BOARD_RIGIDBOARD_V2     52   // Invent-A-Part RigidBoard V2
#define BOARD_RAMPS_14_EFB      43   // RAMPS 1.4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_14_EEB      44   // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_14_EFF      45   // RAMPS 1.4 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_14_EEF      46   // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_14_SF       48   // RAMPS 1.4 (Power outputs: Spindle, Controller Fan)
#define BOARD_GEN6              5    // Gen6
#define BOARD_GEN6_DELUXE       51   // Gen6 deluxe
#define BOARD_SANGUINOLOLU_11   6    // Sanguinololu < 1.2
#define BOARD_SANGUINOLOLU_12   62   // Sanguinololu 1.2 and above
#define BOARD_MELZI             63   // Melzi
#define BOARD_STB_11            64   // STB V1.1
#define BOARD_AZTEEG_X1         65   // Azteeg X1
#define BOARD_MELZI_MAKR3D      66   // Melzi with ATmega1284 (MaKr3d version)
#define BOARD_AZTEEG_X3         67   // Azteeg X3
#define BOARD_AZTEEG_X3_PRO     68   // Azteeg X3 Pro
#define BOARD_ULTIMAKER         7    // Ultimaker
#define BOARD_ULTIMAKER_OLD     71   // Ultimaker (Older electronics. Pre 1.5.4. This is rare)
#define BOARD_ULTIMAIN_2        72   // Ultimainboard 2.x (Uses TEMP_SENSOR 20)
#define BOARD_3DRAG             77   // 3Drag Controller
#define BOARD_K8200             78   // Velleman K8200 Controller (derived from 3Drag Controller)
#define BOARD_K8400             79   // Velleman K8400 Controller (derived from 3Drag Controller)
#define BOARD_TEENSYLU          8    // Teensylu
#define BOARD_RUMBA             80   // Rumba
#define BOARD_PRINTRBOARD       81   // Printrboard (AT90USB1286)
#define BOARD_PRINTRBOARD_REVF  811  // Printrboard Revision F (AT90USB1286)
#define BOARD_BRAINWAVE         82   // Brainwave (AT90USB646)
#define BOARD_SAV_MKI           83   // SAV Mk-I (AT90USB1286)
#define BOARD_TEENSY2           84   // Teensy++2.0 (AT90USB1286) - CLI compile: DEFINES=AT90USBxx_TEENSYPP_ASSIGNMENTS HARDWARE_MOTHERBOARD=84  make
#define BOARD_BRAINWAVE_PRO     85   // Brainwave Pro (AT90USB1286)
#define BOARD_GEN3_PLUS         9    // Gen3+
#define BOARD_GEN3_MONOLITHIC   22   // Gen3 Monolithic Electronics
#define BOARD_MEGATRONICS       70   // Megatronics
#define BOARD_MEGATRONICS_2     701  // Megatronics v2.0
#define BOARD_MINITRONICS       702  // Minitronics v1.0/1.1
#define BOARD_MEGATRONICS_3     703  // Megatronics v3.0
#define BOARD_OMCA_A            90   // Alpha OMCA board
#define BOARD_OMCA              91   // Final OMCA board
#define BOARD_RAMBO             301  // Rambo
#define BOARD_MINIRAMBO         302  // Mini-Rambo
#define BOARD_AJ4P              303  // AJ4P
#define BOARD_MEGACONTROLLER    310  // Mega controller
#define BOARD_ELEFU_3           21   // Elefu Ra Board (v3)
#define BOARD_5DPRINT           88   // 5DPrint D8 Driver Board
#define BOARD_LEAPFROG          999  // Leapfrog
#define BOARD_MKS_BASE          40   // MKS BASE 1.0
#define BOARD_MKS_13            47   // MKS v1.3 or 1.4 (maybe higher)
#define BOARD_SAINSMART_2IN1    49   // Sainsmart 2-in-1 board
#define BOARD_BAM_DICE          401  // 2PrintBeta BAM&DICE with STK drivers
#define BOARD_BAM_DICE_DUE      402  // 2PrintBeta BAM&DICE Due with STK drivers
#define BOARD_BQ_ZUM_MEGA_3D    503  // bq ZUM Mega 3D

#define BOARD_99                99   // This is in pins.h but...?

#define MB(board) (MOTHERBOARD==BOARD_##board)

#endif //__BOARDS_H
