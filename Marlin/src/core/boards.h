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
#pragma once

#define BOARD_UNKNOWN -1

//
// RAMPS 1.3 / 1.4 - ATmega1280, ATmega2560
//

#define BOARD_RAMPS_OLD               1000  // MEGA/RAMPS up to 1.2

#define BOARD_RAMPS_13_EFB            1010  // RAMPS 1.3 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_13_EEB            1011  // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_13_EFF            1012  // RAMPS 1.3 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_13_EEF            1013  // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_13_SF             1014  // RAMPS 1.3 (Power outputs: Spindle, Controller Fan)

#define BOARD_RAMPS_14_EFB            1020  // RAMPS 1.4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_14_EEB            1021  // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_14_EFF            1022  // RAMPS 1.4 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_14_EEF            1023  // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_14_SF             1024  // RAMPS 1.4 (Power outputs: Spindle, Controller Fan)

#define BOARD_RAMPS_PLUS_EFB          1030  // RAMPS Plus 3DYMY (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_PLUS_EEB          1031  // RAMPS Plus 3DYMY (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_PLUS_EFF          1032  // RAMPS Plus 3DYMY (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_PLUS_EEF          1033  // RAMPS Plus 3DYMY (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_PLUS_SF           1034  // RAMPS Plus 3DYMY (Power outputs: Spindle, Controller Fan)

//
// RAMPS Derivatives - ATmega1280, ATmega2560
//

#define BOARD_3DRAG                   1100  // 3Drag Controller
#define BOARD_K8200                   1101  // Velleman K8200 Controller (derived from 3Drag Controller)
#define BOARD_K8400                   1102  // Velleman K8400 Controller (derived from 3Drag Controller)
#define BOARD_BAM_DICE                1103  // 2PrintBeta BAM&DICE with STK drivers
#define BOARD_BAM_DICE_DUE            1104  // 2PrintBeta BAM&DICE Due with STK drivers
#define BOARD_MKS_BASE                1105  // MKS BASE v1.0
#define BOARD_MKS_BASE_14             1106  // MKS v1.4 with A4982 stepper drivers
#define BOARD_MKS_BASE_15             1107  // MKS v1.5 with Allegro A4982 stepper drivers
#define BOARD_MKS_BASE_HEROIC         1108  // MKS BASE 1.0 with Heroic HR4982 stepper drivers
#define BOARD_MKS_GEN_13              1109  // MKS GEN v1.3 or 1.4
#define BOARD_MKS_GEN_L               1110  // MKS GEN L
#define BOARD_KFB_2                   1111  // Bigtreetech or BIQU KFB2.0
#define BOARD_ZRIB_V20                1112  // zrib V2.0 control board (Chinese knock off RAMPS replica)
#define BOARD_FELIX2                  1113  // Felix 2.0+ Electronics Board (RAMPS like)
#define BOARD_RIGIDBOARD              1114  // Invent-A-Part RigidBoard
#define BOARD_RIGIDBOARD_V2           1115  // Invent-A-Part RigidBoard V2
#define BOARD_SAINSMART_2IN1          1116  // Sainsmart 2-in-1 board
#define BOARD_ULTIMAKER               1117  // Ultimaker
#define BOARD_ULTIMAKER_OLD           1118  // Ultimaker (Older electronics. Pre 1.5.4. This is rare)
#define BOARD_AZTEEG_X3               1119  // Azteeg X3
#define BOARD_AZTEEG_X3_PRO           1120  // Azteeg X3 Pro
#define BOARD_ULTIMAIN_2              1121  // Ultimainboard 2.x (Uses TEMP_SENSOR 20)
#define BOARD_RUMBA                   1122  // Rumba
#define BOARD_RUMBA_RAISE3D           1123  // Raise3D N series Rumba derivative
#define BOARD_RL200                   1124  // Rapide Lite 200 (v1, low-cost RUMBA clone with drv)
#define BOARD_FORMBOT_TREX2PLUS       1125  // Formbot T-Rex 2 Plus
#define BOARD_FORMBOT_TREX3           1126  // Formbot T-Rex 3
#define BOARD_FORMBOT_RAPTOR          1127  // Formbot Raptor
#define BOARD_FORMBOT_RAPTOR2         1128  // Formbot Raptor 2
#define BOARD_BQ_ZUM_MEGA_3D          1129  // bq ZUM Mega 3D
#define BOARD_MAKEBOARD_MINI          1130  // MakeBoard Mini v2.1.2 is a control board sold by MicroMake
#define BOARD_TRIGORILLA_13           1131  // TriGorilla Anycubic version 1.3-based on RAMPS EFB
#define BOARD_TRIGORILLA_14           1132  //   ... Ver 1.4
#define BOARD_TRIGORILLA_14_11        1133  //   ... Rev 1.1 (new servo pin order)
#define BOARD_RAMPS_ENDER_4           1134  // Creality: Ender-4, CR-8
#define BOARD_RAMPS_CREALITY          1135  // Creality: CR10S, CR20, CR-X
#define BOARD_RAMPS_DAGOMA            1136  // Dagoma F5
#define BOARD_FYSETC_F6_13            1137  // Fysetc F6
#define BOARD_DUPLICATOR_I3_PLUS      1138  // Wanhao Duplicator i3 Plus
#define BOARD_VORON                   1139  // VORON Design
#define BOARD_TRONXY_V3_1_0           1140  // Tronxy TRONXY-V3-1.0
#define BOARD_Z_BOLT_X_SERIES         1141  // Z-Bolt X Series

//
// Other ATmega1280, ATmega2560
//

#define BOARD_CNCONTROLS_11           1200  // Cartesio CN Controls V11
#define BOARD_CNCONTROLS_12           1201  // Cartesio CN Controls V12
#define BOARD_CHEAPTRONIC             1202  // Cheaptronic v1.0
#define BOARD_CHEAPTRONIC_V2          1203  // Cheaptronic v2.0
#define BOARD_MIGHTYBOARD_REVE        1204  // Makerbot Mightyboard Revision E
#define BOARD_MEGATRONICS             1205  // Megatronics
#define BOARD_MEGATRONICS_2           1206  // Megatronics v2.0
#define BOARD_MEGATRONICS_3           1207  // Megatronics v3.0
#define BOARD_MEGATRONICS_31          1208  // Megatronics v3.1
#define BOARD_MEGATRONICS_32          1209  // Megatronics v3.2
#define BOARD_RAMBO                   1210  // Rambo
#define BOARD_MINIRAMBO               1211  // Mini-Rambo
#define BOARD_MINIRAMBO_10A           1212  // Mini-Rambo 1.0a
#define BOARD_EINSY_RAMBO             1213  // Einsy Rambo
#define BOARD_EINSY_RETRO             1214  // Einsy Retro
#define BOARD_ELEFU_3                 1215  // Elefu Ra Board (v3)
#define BOARD_LEAPFROG                1216  // Leapfrog
#define BOARD_MEGACONTROLLER          1217  // Mega controller
#define BOARD_SCOOVO_X9H              1218  // abee Scoovo X9H
#define BOARD_GT2560_REV_A            1219  // Geeetech GT2560 Rev. A
#define BOARD_GT2560_REV_A_PLUS       1220  // Geeetech GT2560 Rev. A+ (with auto level probe)
#define BOARD_GT2560_V3               1221  // Geeetech GT2560 Rev B for A10(M/D)
#define BOARD_GT2560_V3_MC2           1222  // Geeetech GT2560 Rev B for Mecreator2
#define BOARD_GT2560_V3_A20           1223  // Geeetech GT2560 Rev B for A20(M/D)
#define BOARD_EINSTART_S              1224  // Einstart retrofit

//
// ATmega1281, ATmega2561
//

#define BOARD_MINITRONICS             1300  // Minitronics v1.0/1.1
#define BOARD_SILVER_GATE             1301  // Silvergate v1.0

//
// Sanguinololu and Derivatives - ATmega644P, ATmega1284P
//

#define BOARD_SANGUINOLOLU_11         1400  // Sanguinololu < 1.2
#define BOARD_SANGUINOLOLU_12         1401  // Sanguinololu 1.2 and above
#define BOARD_MELZI                   1402  // Melzi
#define BOARD_MELZI_MAKR3D            1403  // Melzi with ATmega1284 (MaKr3d version)
#define BOARD_MELZI_CREALITY          1404  // Melzi Creality3D board (for CR-10 etc)
#define BOARD_MELZI_MALYAN            1405  // Melzi Malyan M150 board
#define BOARD_MELZI_TRONXY            1406  // Tronxy X5S
#define BOARD_STB_11                  1407  // STB V1.1
#define BOARD_AZTEEG_X1               1408  // Azteeg X1
#define BOARD_ANET_10                 1409  // Anet 1.0 (Melzi clone)
#define BOARD_DERANDERE_PB_1          1410  // Repurposed Anet 1.0 (Melzi clone)

//
// Other ATmega644P, ATmega644, ATmega1284P
//

#define BOARD_GEN3_MONOLITHIC         1500  // Gen3 Monolithic Electronics
#define BOARD_GEN3_PLUS               1501  // Gen3+
#define BOARD_GEN6                    1502  // Gen6
#define BOARD_GEN6_DELUXE             1503  // Gen6 deluxe
#define BOARD_GEN7_CUSTOM             1504  // Gen7 custom (Alfons3 Version) "https://github.com/Alfons3/Generation_7_Electronics"
#define BOARD_GEN7_12                 1505  // Gen7 v1.1, v1.2
#define BOARD_GEN7_13                 1506  // Gen7 v1.3
#define BOARD_GEN7_14                 1507  // Gen7 v1.4
#define BOARD_OMCA_A                  1508  // Alpha OMCA board
#define BOARD_OMCA                    1509  // Final OMCA board
#define BOARD_SETHI                   1510  // Sethi 3D_1

//
// Teensyduino - AT90USB1286, AT90USB1286P
//

#define BOARD_TEENSYLU                1600  // Teensylu
#define BOARD_PRINTRBOARD             1601  // Printrboard (AT90USB1286)
#define BOARD_PRINTRBOARD_REVF        1602  // Printrboard Revision F (AT90USB1286)
#define BOARD_BRAINWAVE               1603  // Brainwave (AT90USB646)
#define BOARD_BRAINWAVE_PRO           1604  // Brainwave Pro (AT90USB1286)
#define BOARD_SAV_MKI                 1605  // SAV Mk-I (AT90USB1286)
#define BOARD_TEENSY2                 1606  // Teensy++2.0 (AT90USB1286)
#define BOARD_5DPRINT                 1607  // 5DPrint D8 Driver Board

//
// LPC1768 ARM Cortex M3
//

#define BOARD_RAMPS_14_RE_ARM_EFB     2000  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_14_RE_ARM_EEB     2001  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_14_RE_ARM_EFF     2002  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_14_RE_ARM_EEF     2003  // Re-ARM with RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_14_RE_ARM_SF      2004  // Re-ARM with RAMPS 1.4 (Power outputs: Spindle, Controller Fan)
#define BOARD_MKS_SBASE               2005  // MKS-Sbase (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_AZSMZ_MINI              2006  // AZSMZ Mini
#define BOARD_AZTEEG_X5_GT            2007  // Azteeg X5 GT (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_BIQU_BQ111_A4           2008  // BIQU BQ111-A4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_SELENA_COMPACT          2009  // Selena Compact (Power outputs: Hotend0, Hotend1, Bed0, Bed1, Fan0, Fan1)
#define BOARD_COHESION3D_REMIX        2010  // Cohesion3D ReMix
#define BOARD_COHESION3D_MINI         2011  // Cohesion3D Mini
#define BOARD_SMOOTHIEBOARD           2012  // Smoothieboard
#define BOARD_AZTEEG_X5_MINI_WIFI     2013  // Azteeg X5 Mini Wifi (Power outputs: Hotend0, Bed, Fan)
#define BOARD_BIQU_SKR_V1_1           2014  // BIQU SKR_V1.1 (Power outputs: Hotend0,Hotend1, Fan, Bed)
#define BOARD_BIQU_B300_V1_0          2015  // BIQU B300_V1.0 (Power outputs: Hotend0, Fan, Bed, SPI Driver)
#define BOARD_BIGTREE_SKR_V1_3        2016  // BIGTREE SKR_V1.3 (Power outputs: Hotend0, Hotend1, Fan, Bed)
#define BOARD_AZTEEG_X5_MINI          2017  // Azteeg X5 Mini (Power outputs: Hotend0, Bed, Fan)
#define BOARD_MKS_SGEN                2018  // MKS-SGen (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_MKS_SGEN_L              2019  // MKS-SGen-L (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_TH3D_EZBOARD            2020  // TH3D EZBoard v1.0

//
// SAM3X8E ARM Cortex M3
//

#define BOARD_DUE3DOM                 3000  // DUE3DOM for Arduino DUE
#define BOARD_DUE3DOM_MINI            3001  // DUE3DOM MINI for Arduino DUE
#define BOARD_RADDS                   3002  // RADDS
#define BOARD_RAMPS_FD_V1             3003  // RAMPS-FD v1
#define BOARD_RAMPS_FD_V2             3004  // RAMPS-FD v2
#define BOARD_RAMPS_SMART_EFB         3005  // RAMPS-SMART (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_SMART_EEB         3006  // RAMPS-SMART (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_SMART_EFF         3007  // RAMPS-SMART (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_SMART_EEF         3008  // RAMPS-SMART (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_SMART_SF          3009  // RAMPS-SMART (Power outputs: Spindle, Controller Fan)
#define BOARD_RAMPS_DUO_EFB           3010  // RAMPS Duo (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_DUO_EEB           3011  // RAMPS Duo (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_DUO_EFF           3012  // RAMPS Duo (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_DUO_EEF           3013  // RAMPS Duo (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_DUO_SF            3014  // RAMPS Duo (Power outputs: Spindle, Controller Fan)
#define BOARD_RAMPS4DUE_EFB           3015  // RAMPS4DUE (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS4DUE_EEB           3016  // RAMPS4DUE (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS4DUE_EFF           3017  // RAMPS4DUE (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS4DUE_EEF           3018  // RAMPS4DUE (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS4DUE_SF            3019  // RAMPS4DUE (Power outputs: Spindle, Controller Fan)
#define BOARD_RURAMPS4D_11            3020  // RuRAMPS4Duo v1.1 (Power outputs: Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
#define BOARD_RURAMPS4D_13            3021  // RuRAMPS4Duo v1.3 (Power outputs: Hotend0, Hotend1, Hotend2, Fan0, Fan1, Bed)
#define BOARD_ULTRATRONICS_PRO        3022  // ReprapWorld Ultratronics Pro V1.0
#define BOARD_ARCHIM1                 3023  // UltiMachine Archim1 (with DRV8825 drivers)
#define BOARD_ARCHIM2                 3024  // UltiMachine Archim2 (with TMC2130 drivers)
#define BOARD_ALLIGATOR               3025  // Alligator Board R2

//
// SAM3X8C ARM Cortex M3
//

#define BOARD_PRINTRBOARD_G2          3100  // PRINTRBOARD G2
#define BOARD_ADSK                    3101  // Arduino DUE Shield Kit (ADSK)

//
// STM32 ARM Cortex-M3
//

#define BOARD_STM32F1R                4000  // STM32R    Libmaple-based STM32F1 controller
#define BOARD_MALYAN_M200             4001  // STM32C8T6 Libmaple-based STM32F1 controller
#define BOARD_STM3R_MINI              4002  // STM32     Libmaple-based STM32F1 controller
#define BOARD_GTM32_PRO_VB            4003  // STM32F103VET6 controller
#define BOARD_MORPHEUS                4004  // STM32F103C8 / STM32F103CB  Libmaple-based STM32F1 controller
#define BOARD_MKS_ROBIN               4005  // MKS Robin (STM32F103ZET6)
#define BOARD_MKS_ROBIN_MINI          4006  // MKS Robin Mini (STM32F103VET6)
#define BOARD_MKS_ROBIN_NANO          4007  // MKS Robin Nano (STM32F103VET6)
#define BOARD_BIGTREE_SKR_MINI_V1_1   4008  // BigTreeTech SKR Mini v1.1 (STM32F103RC)
#define BOARD_BIGTREE_SKR_MINI_E3     4009  // BigTreeTech SKR Mini E3 (STM32F103RC)
#define BOARD_JGAURORA_A5S_A1         4010  // JGAurora A5S A1 (STM32F103ZET6)
#define BOARD_FYSETC_AIO_II           4011  // FYSETC AIO_II
#define BOARD_FYSETC_CHEETAH          4012  // FYSETC CHEETAH

//
// STM32 ARM Cortex-M4F
//

#define BOARD_TEENSY31_32             4100  // Teensy3.1 and Teensy3.2
#define BOARD_TEENSY35_36             4101  // Teensy3.5 and Teensy3.6
#define BOARD_BEAST                   4102  // STM32F4xxVxT6 Libmaple-based STM32F4 controller
#define BOARD_STM32F4                 4103  // STM32 STM32GENERIC-based STM32F4 controller
#define BOARD_ARMED                   4104  // Arm'ed STM32F4-based controller
#define BOARD_RUMBA32                 4105  // RUMBA32 STM32F4-based controller
#define BOARD_BLACK_STM32F407VE       4106  // BLACK_STM32F407VE
#define BOARD_BLACK_STM32F407ZE       4107  // BLACK_STM32F407ZE
#define BOARD_STEVAL                  4108  // STEVAL-3DP001V1 3D PRINTER BOARD

//
// ARM Cortex M7
//

#define BOARD_THE_BORG                5000  // THE-BORG (Power outputs: Hotend0, Hotend1, Bed, Fan)
#define BOARD_REMRAM_V1               5001  // RemRam v1

//
// Espressif ESP32 WiFi
//
#define BOARD_ESP32                   6000

//
// Simulations
//

#define BOARD_LINUX_RAMPS             9999

#define MB(board) (defined(BOARD_##board) && MOTHERBOARD==BOARD_##board)
