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
 * File: pins/pins.h
 *
 * Include pins definitions
 *
 * Pins numbering schemes:
 *
 *  - Digital I/O pin number if used by READ/WRITE macros. (e.g., X_STEP_DIR)
 *    The FastIO headers map digital pins to their ports and functions.
 *
 *  - Analog Input number if used by analogRead or DAC. (e.g., TEMP_n_PIN)
 *    These numbers are the same in any pin mapping.
 */

#if HAS_SMUFF
  #define MAX_EXTRUDERS 12
#else
  #define MAX_EXTRUDERS 8
#endif
#define MAX_E_STEPPERS 8

#if   MB(RAMPS_13_EFB, RAMPS_14_EFB, RAMPS_PLUS_EFB, RAMPS_14_RE_ARM_EFB, RAMPS_SMART_EFB, RAMPS_DUO_EFB, RAMPS4DUE_EFB)
  #define IS_RAMPS_EFB
#elif MB(RAMPS_13_EEB, RAMPS_14_EEB, RAMPS_PLUS_EEB, RAMPS_14_RE_ARM_EEB, RAMPS_SMART_EEB, RAMPS_DUO_EEB, RAMPS4DUE_EEB)
  #define IS_RAMPS_EEB
#elif MB(RAMPS_13_EFF, RAMPS_14_EFF, RAMPS_PLUS_EFF, RAMPS_14_RE_ARM_EFF, RAMPS_SMART_EFF, RAMPS_DUO_EFF, RAMPS4DUE_EFF)
  #define IS_RAMPS_EFF
#elif MB(RAMPS_13_EEF, RAMPS_14_EEF, RAMPS_PLUS_EEF, RAMPS_14_RE_ARM_EEF, RAMPS_SMART_EEF, RAMPS_DUO_EEF, RAMPS4DUE_EEF)
  #define IS_RAMPS_EEF
#elif MB(RAMPS_13_SF,  RAMPS_14_SF,  RAMPS_PLUS_SF,  RAMPS_14_RE_ARM_SF,  RAMPS_SMART_SF,  RAMPS_DUO_SF,  RAMPS4DUE_SF)
  #define IS_RAMPS_SF
#endif

#if !(BOTH(IS_ULTRA_LCD, IS_NEWPANEL) && ANY(PANEL_ONE, VIKI2, miniVIKI, MINIPANEL, REPRAPWORLD_KEYPAD))
  #define HAS_FREE_AUX2_PINS 1
#endif

// Test the target within the included pins file
#ifdef __MARLIN_DEPS__
  #define NOT_TARGET(V...) 0
#else
  #define NOT_TARGET(V...) NONE(V)
#endif

//
// RAMPS 1.3 / 1.4 - ATmega1280, ATmega2560
//

#if MB(RAMPS_OLD)
  #include "ramps/pins_RAMPS_OLD.h"             // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(RAMPS_13_EFB, RAMPS_13_EEB, RAMPS_13_EFF, RAMPS_13_EEF, RAMPS_13_SF)
  #include "ramps/pins_RAMPS_13.h"              // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(RAMPS_14_EFB, RAMPS_14_EEB, RAMPS_14_EFF, RAMPS_14_EEF, RAMPS_14_SF)
  #include "ramps/pins_RAMPS.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(RAMPS_PLUS_EFB, RAMPS_PLUS_EEB, RAMPS_PLUS_EFF, RAMPS_PLUS_EEF, RAMPS_PLUS_SF)
  #include "ramps/pins_RAMPS_PLUS.h"            // ATmega2560, ATmega1280                 env:mega2560 env:mega1280

//
// RAMPS Derivatives - ATmega1280, ATmega2560
//

#elif MB(3DRAG)
  #include "ramps/pins_3DRAG.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(K8200)
  #include "ramps/pins_K8200.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(K8400)
  #include "ramps/pins_K8400.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(K8600)
  #include "ramps/pins_K8600.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(K8800)
  #include "ramps/pins_K8800.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(BAM_DICE)
  #include "ramps/pins_RAMPS.h"                 // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(BAM_DICE_DUE)
  #include "ramps/pins_BAM_DICE_DUE.h"          // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(MKS_BASE)
  #include "ramps/pins_MKS_BASE_10.h"           // ATmega2560                             env:mega2560
#elif MB(MKS_BASE_14)
  #include "ramps/pins_MKS_BASE_14.h"           // ATmega2560                             env:mega2560
#elif MB(MKS_BASE_15)
  #include "ramps/pins_MKS_BASE_15.h"           // ATmega2560                             env:mega2560
#elif MB(MKS_BASE_16)
  #include "ramps/pins_MKS_BASE_16.h"           // ATmega2560                             env:mega2560
#elif MB(MKS_BASE_HEROIC)
  #include "ramps/pins_MKS_BASE_HEROIC.h"       // ATmega2560                             env:mega2560
#elif MB(MKS_GEN_13)
  #include "ramps/pins_MKS_GEN_13.h"            // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(MKS_GEN_L)
  #include "ramps/pins_MKS_GEN_L.h"             // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(KFB_2)
  #include "ramps/pins_BIQU_KFB_2.h"            // ATmega2560                             env:mega2560
#elif MB(ZRIB_V20)
  #include "ramps/pins_ZRIB_V20.h"              // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(ZRIB_V52)
  #include "ramps/pins_ZRIB_V52.h"              // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(FELIX2)
  #include "ramps/pins_FELIX2.h"                // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(RIGIDBOARD)
  #include "ramps/pins_RIGIDBOARD.h"            // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(RIGIDBOARD_V2)
  #include "ramps/pins_RIGIDBOARD_V2.h"         // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(SAINSMART_2IN1)
  #include "ramps/pins_SAINSMART_2IN1.h"        // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(ULTIMAKER)
  #include "ramps/pins_ULTIMAKER.h"             // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(ULTIMAKER_OLD)
  #include "ramps/pins_ULTIMAKER_OLD.h"         // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(AZTEEG_X3)
  #include "ramps/pins_AZTEEG_X3.h"             // ATmega2560                             env:mega2560
#elif MB(AZTEEG_X3_PRO)
  #include "ramps/pins_AZTEEG_X3_PRO.h"         // ATmega2560                             env:mega2560
#elif MB(ULTIMAIN_2)
  #include "ramps/pins_ULTIMAIN_2.h"            // ATmega2560                             env:mega2560ext
#elif MB(FORMBOT_RAPTOR)
  #include "ramps/pins_FORMBOT_RAPTOR.h"        // ATmega2560                             env:mega2560
#elif MB(FORMBOT_RAPTOR2)
  #include "ramps/pins_FORMBOT_RAPTOR2.h"       // ATmega2560                             env:mega2560
#elif MB(FORMBOT_TREX2PLUS)
  #include "ramps/pins_FORMBOT_TREX2PLUS.h"     // ATmega2560                             env:mega2560
#elif MB(FORMBOT_TREX3)
  #include "ramps/pins_FORMBOT_TREX3.h"         // ATmega2560                             env:mega2560
#elif MB(RUMBA)
  #include "ramps/pins_RUMBA.h"                 // ATmega2560                             env:mega2560
#elif MB(RUMBA_RAISE3D)
  #include "ramps/pins_RUMBA_RAISE3D.h"         // ATmega2560                             env:mega2560
#elif MB(RL200)
  #include "ramps/pins_RL200.h"                 // ATmega2560                             env:mega2560
#elif MB(BQ_ZUM_MEGA_3D)
  #include "ramps/pins_BQ_ZUM_MEGA_3D.h"        // ATmega2560                             env:mega2560ext
#elif MB(MAKEBOARD_MINI)
  #include "ramps/pins_MAKEBOARD_MINI.h"        // ATmega2560                             env:mega2560
#elif MB(TRIGORILLA_13)
  #include "ramps/pins_TRIGORILLA_13.h"         // ATmega2560                             env:mega2560
#elif MB(TRIGORILLA_14, TRIGORILLA_14_11)
  #include "ramps/pins_TRIGORILLA_14.h"         // ATmega2560                             env:mega2560
#elif MB(RAMPS_ENDER_4)
  #include "ramps/pins_RAMPS_ENDER_4.h"         // ATmega2560                             env:mega2560
#elif MB(RAMPS_CREALITY)
  #include "ramps/pins_RAMPS_CREALITY.h"        // ATmega2560                             env:mega2560
#elif MB(DAGOMA_F5)
  #include "ramps/pins_DAGOMA_F5.h"             // ATmega2560                             env:mega2560
#elif MB(FYSETC_F6_13)
  #include "ramps/pins_FYSETC_F6_13.h"          // ATmega2560                             env:FYSETC_F6
#elif MB(FYSETC_F6_14)
  #include "ramps/pins_FYSETC_F6_14.h"          // ATmega2560                             env:FYSETC_F6
#elif MB(DUPLICATOR_I3_PLUS)
  #include "ramps/pins_DUPLICATOR_I3_PLUS.h"    // ATmega2560                             env:mega2560
#elif MB(VORON)
  #include "ramps/pins_VORON.h"                 // ATmega2560                             env:mega2560
#elif MB(TRONXY_V3_1_0)
  #include "ramps/pins_TRONXY_V3_1_0.h"         // ATmega2560                             env:mega2560
#elif MB(Z_BOLT_X_SERIES)
  #include "ramps/pins_Z_BOLT_X_SERIES.h"       // ATmega2560                             env:mega2560
#elif MB(TT_OSCAR)
  #include "ramps/pins_TT_OSCAR.h"              // ATmega2560                             env:mega2560
#elif MB(TANGO)
  #include "ramps/pins_TANGO.h"                 // ATmega2560                             env:mega2560
#elif MB(MKS_GEN_L_V2)
  #include "ramps/pins_MKS_GEN_L_V2.h"          // ATmega2560                             env:mega2560
#elif MB(COPYMASTER_3D)
  #include "ramps/pins_COPYMASTER_3D.h"         // ATmega2560                             env:mega2560
#elif MB(ORTUR_4)
  #include "ramps/pins_ORTUR_4.h"               // ATmega2560                             env:mega2560
#elif MB(TENLOG_D3_HERO)
  #include "ramps/pins_TENLOG_D3_HERO.h"        // ATmega2560                             env:mega2560
#elif MB(MKS_GEN_L_V21)
  #include "ramps/pins_MKS_GEN_L_V21.h"         // ATmega2560                             env:mega2560
#elif MB(RAMPS_S_12_EEFB, RAMPS_S_12_EEEB, RAMPS_S_12_EFFB)
  #include "ramps/pins_RAMPS_S_12.h"            // ATmega2560                             env:mega2560
#elif MB(LONGER3D_LK1_PRO, LONGER3D_LKx_PRO)
  #include "ramps/pins_LONGER3D_LKx_PRO.h"      // ATmega2560                             env:mega2560

//
// RAMBo and derivatives
//

#elif MB(RAMBO)
  #include "rambo/pins_RAMBO.h"                 // ATmega2560                             env:rambo
#elif MB(MINIRAMBO, MINIRAMBO_10A)
  #include "rambo/pins_MINIRAMBO.h"             // ATmega2560                             env:rambo
#elif MB(EINSY_RAMBO)
  #include "rambo/pins_EINSY_RAMBO.h"           // ATmega2560                             env:rambo
#elif MB(EINSY_RETRO)
  #include "rambo/pins_EINSY_RETRO.h"           // ATmega2560                             env:rambo
#elif MB(SCOOVO_X9H)
  #include "rambo/pins_SCOOVO_X9H.h"            // ATmega2560                             env:rambo
#elif MB(RAMBO_THINKERV2)
  #include "rambo/pins_RAMBO_THINKERV2.h"       // ATmega2560                             env:rambo

//
// Other ATmega1280, ATmega2560
//

#elif MB(CNCONTROLS_11)
  #include "mega/pins_CNCONTROLS_11.h"          // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(CNCONTROLS_12)
  #include "mega/pins_CNCONTROLS_12.h"          // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(CNCONTROLS_15)
  #include "mega/pins_CNCONTROLS_15.h"          // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(MIGHTYBOARD_REVE)
  #include "mega/pins_MIGHTYBOARD_REVE.h"       // ATmega2560, ATmega1280                 env:mega2560ext env:mega1280
#elif MB(CHEAPTRONIC)
  #include "mega/pins_CHEAPTRONIC.h"            // ATmega2560                             env:mega2560
#elif MB(CHEAPTRONIC_V2)
  #include "mega/pins_CHEAPTRONICv2.h"          // ATmega2560                             env:mega2560
#elif MB(MEGATRONICS)
  #include "mega/pins_MEGATRONICS.h"            // ATmega2560                             env:mega2560
#elif MB(MEGATRONICS_2)
  #include "mega/pins_MEGATRONICS_2.h"          // ATmega2560                             env:mega2560
#elif MB(MEGATRONICS_3, MEGATRONICS_31, MEGATRONICS_32)
  #include "mega/pins_MEGATRONICS_3.h"          // ATmega2560                             env:mega2560
#elif MB(ELEFU_3)
  #include "mega/pins_ELEFU_3.h"                // ATmega2560                             env:mega2560
#elif MB(LEAPFROG)
  #include "mega/pins_LEAPFROG.h"               // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(MEGACONTROLLER)
  #include "mega/pins_MEGACONTROLLER.h"         // ATmega2560                             env:mega2560
#elif MB(GT2560_REV_A)
  #include "mega/pins_GT2560_REV_A.h"           // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(GT2560_REV_A_PLUS)
  #include "mega/pins_GT2560_REV_A_PLUS.h"      // ATmega2560, ATmega1280                 env:mega2560 env:mega1280
#elif MB(GT2560_V3)
  #include "mega/pins_GT2560_V3.h"              // ATmega2560                             env:mega2560
#elif MB(GT2560_V3_MC2)
  #include "mega/pins_GT2560_V3_MC2.h"          // ATmega2560                             env:mega2560
#elif MB(GT2560_V3_A20)
  #include "mega/pins_GT2560_V3_A20.h"          // ATmega2560                             env:mega2560
#elif MB(EINSTART_S)
  #include "mega/pins_EINSTART-S.h"             // ATmega2560, ATmega1280                 env:mega2560ext env:mega1280
#elif MB(WANHAO_ONEPLUS)
  #include "mega/pins_WANHAO_ONEPLUS.h"         // ATmega2560                             env:mega2560
#elif MB(OVERLORD)
  #include "mega/pins_OVERLORD.h"               // ATmega2560                             env:mega2560
#elif MB(HJC2560C_REV1)
  #include "mega/pins_HJC2560C_REV2.h"          // ATmega2560                             env:mega2560
#elif MB(HJC2560C_REV2)
  #include "mega/pins_HJC2560C_REV2.h"          // ATmega2560                             env:mega2560
#elif MB(LEAPFROG_XEED2015)
  #include "mega/pins_LEAPFROG_XEED2015.h"      // ATmega2560                             env:mega2560
#elif MB(PICA)
  #include "mega/pins_PICA.h"                   // ATmega2560                             env:mega2560
#elif MB(PICA_REVB)
  #include "mega/pins_PICAOLD.h"                // ATmega2560                             env:mega2560
#elif MB(INTAMSYS40)
  #include "mega/pins_INTAMSYS40.h"             // ATmega2560                             env:mega2560

//
// ATmega1281, ATmega2561
//

#elif MB(MINITRONICS)
  #include "mega/pins_MINITRONICS.h"            // ATmega1281                             env:mega1280
#elif MB(SILVER_GATE)
  #include "mega/pins_SILVER_GATE.h"            // ATmega2561                             env:mega2560

//
// Sanguinololu and Derivatives - ATmega644P, ATmega1284P
//

#elif MB(SANGUINOLOLU_11)
  #include "sanguino/pins_SANGUINOLOLU_11.h"    // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(SANGUINOLOLU_12)
  #include "sanguino/pins_SANGUINOLOLU_12.h"    // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(MELZI)
  #include "sanguino/pins_MELZI.h"              // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(MELZI_V2)
  #include "sanguino/pins_MELZI_V2.h"           // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(MELZI_MAKR3D)
  #include "sanguino/pins_MELZI_MAKR3D.h"       // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(MELZI_CREALITY)
  #include "sanguino/pins_MELZI_CREALITY.h"     // ATmega1284P                            env:melzi_optiboot_optimized env:melzi_optiboot env:melzi_optimized env:melzi
#elif MB(MELZI_MALYAN)
  #include "sanguino/pins_MELZI_MALYAN.h"       // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(MELZI_TRONXY)
  #include "sanguino/pins_MELZI_TRONXY.h"       // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(STB_11)
  #include "sanguino/pins_STB_11.h"             // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(AZTEEG_X1)
  #include "sanguino/pins_AZTEEG_X1.h"          // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(ZMIB_V2)
  #include "sanguino/pins_ZMIB_V2.h"            // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p

//
// Other ATmega644P, ATmega644, ATmega1284P
//

#elif MB(GEN3_MONOLITHIC)
  #include "sanguino/pins_GEN3_MONOLITHIC.h"    // ATmega644P                             env:sanguino644p
#elif MB(GEN3_PLUS)
  #include "sanguino/pins_GEN3_PLUS.h"          // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(GEN6)
  #include "sanguino/pins_GEN6.h"               // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(GEN6_DELUXE)
  #include "sanguino/pins_GEN6_DELUXE.h"        // ATmega644P, ATmega1284P                env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(GEN7_CUSTOM)
  #include "sanguino/pins_GEN7_CUSTOM.h"        // ATmega644P, ATmega644, ATmega1284P     env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(GEN7_12)
  #include "sanguino/pins_GEN7_12.h"            // ATmega644P, ATmega644, ATmega1284P     env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(GEN7_13)
  #include "sanguino/pins_GEN7_13.h"            // ATmega644P, ATmega644, ATmega1284P     env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(GEN7_14)
  #include "sanguino/pins_GEN7_14.h"            // ATmega644P, ATmega644, ATmega1284P     env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p
#elif MB(OMCA_A)
  #include "sanguino/pins_OMCA_A.h"             // ATmega644                              env:sanguino644p
#elif MB(OMCA)
  #include "sanguino/pins_OMCA.h"               // ATmega644P, ATmega644                  env:sanguino644p
#elif MB(ANET_10)
  #include "sanguino/pins_ANET_10.h"            // ATmega1284P                            env:sanguino1284p env:sanguino1284p_optimized
#elif MB(SETHI)
  #include "sanguino/pins_SETHI.h"              // ATmega644P, ATmega644, ATmega1284P     env:sanguino1284p_optimized env:sanguino1284p env:sanguino644p

//
// Teensyduino - AT90USB1286, AT90USB1286P
//

#elif MB(TEENSYLU)
  #include "teensy2/pins_TEENSYLU.h"            // AT90USB1286, AT90USB1286P              env:at90usb1286_cdc
#elif MB(PRINTRBOARD)
  #include "teensy2/pins_PRINTRBOARD.h"         // AT90USB1286                            env:at90usb1286_dfu
#elif MB(PRINTRBOARD_REVF)
  #include "teensy2/pins_PRINTRBOARD_REVF.h"    // AT90USB1286                            env:at90usb1286_dfu
#elif MB(BRAINWAVE)
  #include "teensy2/pins_BRAINWAVE.h"           // AT90USB646                             env:at90usb1286_cdc
#elif MB(BRAINWAVE_PRO)
  #include "teensy2/pins_BRAINWAVE_PRO.h"       // AT90USB1286                            env:at90usb1286_cdc
#elif MB(SAV_MKI)
  #include "teensy2/pins_SAV_MKI.h"             // AT90USB1286                            env:at90usb1286_cdc
#elif MB(TEENSY2)
  #include "teensy2/pins_TEENSY2.h"             // AT90USB1286                            env:teensy20
#elif MB(5DPRINT)
  #include "teensy2/pins_5DPRINT.h"             // AT90USB1286                            env:at90usb1286_dfu

//
// LPC1768 ARM Cortex M3
//

#elif MB(RAMPS_14_RE_ARM_EFB, RAMPS_14_RE_ARM_EEB, RAMPS_14_RE_ARM_EFF, RAMPS_14_RE_ARM_EEF, RAMPS_14_RE_ARM_SF)
  #include "lpc1768/pins_RAMPS_RE_ARM.h"        // LPC1768                                env:LPC1768
#elif MB(MKS_SBASE)
  #include "lpc1768/pins_MKS_SBASE.h"           // LPC1768                                env:LPC1768
#elif MB(MKS_SGEN_L)
  #include "lpc1768/pins_MKS_SGEN_L.h"          // LPC1768                                env:LPC1768
#elif MB(AZSMZ_MINI)
  #include "lpc1768/pins_AZSMZ_MINI.h"          // LPC1768                                env:LPC1768
#elif MB(BIQU_BQ111_A4)
  #include "lpc1768/pins_BIQU_BQ111_A4.h"       // LPC1768                                env:LPC1768
#elif MB(SELENA_COMPACT)
  #include "lpc1768/pins_SELENA_COMPACT.h"      // LPC1768                                env:LPC1768
#elif MB(BIQU_B300_V1_0)
  #include "lpc1768/pins_BIQU_B300_V1.0.h"      // LPC1768                                env:LPC1768
#elif MB(GMARSH_X6_REV1)
  #include "lpc1768/pins_GMARSH_X6_REV1.h"      // LPC1768                                env:LPC1768
#elif MB(BTT_SKR_V1_1)
  #include "lpc1768/pins_BTT_SKR_V1_1.h"        // LPC1768                                env:LPC1768
#elif MB(BTT_SKR_V1_3)
  #include "lpc1768/pins_BTT_SKR_V1_3.h"        // LPC1768                                env:LPC1768
#elif MB(BTT_SKR_V1_4)
  #include "lpc1768/pins_BTT_SKR_V1_4.h"        // LPC1768                                env:LPC1768

//
// LPC1769 ARM Cortex M3
//

#elif MB(MKS_SGEN)
  #include "lpc1769/pins_MKS_SGEN.h"            // LPC1769                                env:LPC1769
#elif MB(AZTEEG_X5_GT)
  #include "lpc1769/pins_AZTEEG_X5_GT.h"        // LPC1769                                env:LPC1769
#elif MB(AZTEEG_X5_MINI)
  #include "lpc1769/pins_AZTEEG_X5_MINI.h"      // LPC1769                                env:LPC1769
#elif MB(AZTEEG_X5_MINI_WIFI)
  #include "lpc1769/pins_AZTEEG_X5_MINI_WIFI.h" // LPC1769                                env:LPC1769
#elif MB(COHESION3D_REMIX)
  #include "lpc1769/pins_COHESION3D_REMIX.h"    // LPC1769                                env:LPC1769
#elif MB(COHESION3D_MINI)
  #include "lpc1769/pins_COHESION3D_MINI.h"     // LPC1769                                env:LPC1769
#elif MB(SMOOTHIEBOARD)
  #include "lpc1769/pins_SMOOTHIEBOARD.h"       // LPC1769                                env:LPC1769
#elif MB(TH3D_EZBOARD)
  #include "lpc1769/pins_TH3D_EZBOARD.h"        // LPC1769                                env:LPC1769
#elif MB(BTT_SKR_V1_4_TURBO)
  #include "lpc1769/pins_BTT_SKR_V1_4_TURBO.h"  // LPC1769                                env:LPC1769
#elif MB(MKS_SGEN_L_V2)
  #include "lpc1769/pins_MKS_SGEN_L_V2.h"       // LPC1769                                env:LPC1769
#elif MB(BTT_SKR_E3_TURBO)
  #include "lpc1769/pins_BTT_SKR_E3_TURBO.h"    // LPC1769                                env:LPC1769
#elif MB(FLY_CDY)
  #include "lpc1769/pins_FLY_CDY.h"             // LPC1769                                env:LPC1769

//
// Due (ATSAM) boards
//

#elif MB(DUE3DOM)
  #include "sam/pins_DUE3DOM.h"                 // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(DUE3DOM_MINI)
  #include "sam/pins_DUE3DOM_MINI.h"            // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RADDS)
  #include "sam/pins_RADDS.h"                   // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RURAMPS4D_11)
  #include "sam/pins_RURAMPS4D_11.h"            // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RURAMPS4D_13)
  #include "sam/pins_RURAMPS4D_13.h"            // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_FD_V1)
  #include "sam/pins_RAMPS_FD_V1.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_FD_V2)
  #include "sam/pins_RAMPS_FD_V2.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_SMART_EFB, RAMPS_SMART_EEB, RAMPS_SMART_EFF, RAMPS_SMART_EEF, RAMPS_SMART_SF)
  #include "sam/pins_RAMPS_SMART.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_DUO_EFB, RAMPS_DUO_EEB, RAMPS_DUO_EFF, RAMPS_DUO_EEF, RAMPS_DUO_SF)
  #include "sam/pins_RAMPS_DUO.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS4DUE_EFB, RAMPS4DUE_EEB, RAMPS4DUE_EFF, RAMPS4DUE_EEF, RAMPS4DUE_SF)
  #include "sam/pins_RAMPS4DUE.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(ULTRATRONICS_PRO)
  #include "sam/pins_ULTRATRONICS_PRO.h"        // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(ARCHIM1)
  #include "sam/pins_ARCHIM1.h"                 // SAM3X8E                                env:DUE_archim env:DUE_archim_debug
#elif MB(ARCHIM2)
  #include "sam/pins_ARCHIM2.h"                 // SAM3X8E                                env:DUE_archim env:DUE_archim_debug
#elif MB(ALLIGATOR)
  #include "sam/pins_ALLIGATOR_R2.h"            // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(ADSK)
  #include "sam/pins_ADSK.h"                    // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(PRINTRBOARD_G2)
  #include "sam/pins_PRINTRBOARD_G2.h"          // SAM3X8C                                env:DUE_USB
#elif MB(CNCONTROLS_15D)
  #include "sam/pins_CNCONTROLS_15D.h"          // SAM3X8E                                env:DUE env:DUE_USB

//
// STM32 ARM Cortex-M0
//
#elif MB(MALYAN_M200_V2)
  #include "stm32f0/pins_MALYAN_M200_V2.h"      // STM32F0                                env:STM32F070RB_malyan env:STM32F070CB_malyan
#elif MB(MALYAN_M300)
  #include "stm32f0/pins_MALYAN_M300.h"         // STM32F070                              env:malyan_M300

//
// STM32 ARM Cortex-M3
//

#elif MB(STM32F103RE)
  #include "stm32f1/pins_STM32F1R.h"            // STM32F1                                env:STM32F103RE
#elif MB(MALYAN_M200)
  #include "stm32f1/pins_MALYAN_M200.h"         // STM32F1                                env:STM32F103CB_malyan
#elif MB(STM3R_MINI)
  #include "stm32f1/pins_STM3R_MINI.h"          // STM32F1                                env:STM32F103RE
#elif MB(GTM32_PRO_VB)
  #include "stm32f1/pins_GTM32_PRO_VB.h"        // STM32F1                                env:STM32F103RE
#elif MB(GTM32_MINI)
  #include "stm32f1/pins_GTM32_MINI.h"          // STM32F1                                env:STM32F103RE
#elif MB(GTM32_MINI_A30)
  #include "stm32f1/pins_GTM32_MINI_A30.h"      // STM32F1                                env:STM32F103RE
#elif MB(GTM32_REV_B)
  #include "stm32f1/pins_GTM32_REV_B.h"         // STM32F1                                env:STM32F103RE
#elif MB(MORPHEUS)
  #include "stm32f1/pins_MORPHEUS.h"            // STM32F1                                env:STM32F103RE
#elif MB(CHITU3D)
  #include "stm32f1/pins_CHITU3D.h"             // STM32F1                                env:STM32F103RE
#elif MB(MKS_ROBIN)
  #include "stm32f1/pins_MKS_ROBIN.h"           // STM32F1                                env:mks_robin env:mks_robin_stm32
#elif MB(MKS_ROBIN_MINI)
  #include "stm32f1/pins_MKS_ROBIN_MINI.h"      // STM32F1                                env:mks_robin_mini
#elif MB(MKS_ROBIN_NANO)
  #include "stm32f1/pins_MKS_ROBIN_NANO.h"      // STM32F1                                env:mks_robin_nano35 env:mks_robin_nano35_stm32
#elif MB(MKS_ROBIN_NANO_V2)
  #include "stm32f1/pins_MKS_ROBIN_NANO_V2.h"   // STM32F1                                env:mks_robin_nano35 env:mks_robin_nano35_stm32
#elif MB(MKS_ROBIN_LITE)
  #include "stm32f1/pins_MKS_ROBIN_LITE.h"      // STM32F1                                env:mks_robin_lite
#elif MB(MKS_ROBIN_LITE3)
  #include "stm32f1/pins_MKS_ROBIN_LITE3.h"     // STM32F1                                env:mks_robin_lite3
#elif MB(MKS_ROBIN_PRO)
  #include "stm32f1/pins_MKS_ROBIN_PRO.h"       // STM32F1                                env:mks_robin_pro
#elif MB(MKS_ROBIN_E3)
  #include "stm32f1/pins_MKS_ROBIN_E3.h"        // STM32F1                                env:mks_robin_e3
#elif MB(MKS_ROBIN_E3_V1_1)
  #include "stm32f1/pins_MKS_ROBIN_E3_V1_1.h"   // STM32F1                                env:mks_robin_e3
#elif MB(MKS_ROBIN_E3D)
  #include "stm32f1/pins_MKS_ROBIN_E3D.h"       // STM32F1                                env:mks_robin_e3
#elif MB(MKS_ROBIN_E3D_V1_1)
  #include "stm32f1/pins_MKS_ROBIN_E3D_V1_1.h"  // STM32F1                                env:mks_robin_e3
#elif MB(MKS_ROBIN_E3P)
  #include "stm32f1/pins_MKS_ROBIN_E3P.h"       // STM32F1                                env:mks_robin_e3p
#elif MB(BTT_SKR_MINI_V1_1)
  #include "stm32f1/pins_BTT_SKR_MINI_V1_1.h"   // STM32F1                                env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_MINI_E3_V1_0)
  #include "stm32f1/pins_BTT_SKR_MINI_E3_V1_0.h"  // STM32F1                              env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_MINI_E3_V1_2)
  #include "stm32f1/pins_BTT_SKR_MINI_E3_V1_2.h"  // STM32F1                              env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_MINI_E3_V2_0)
  #include "stm32f1/pins_BTT_SKR_MINI_E3_V2_0.h"  // STM32F1                              env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_MINI_MZ_V1_0)
  #include "stm32f1/pins_BTT_SKR_MINI_MZ_V1_0.h"  // STM32F1                              env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_E3_DIP)
  #include "stm32f1/pins_BTT_SKR_E3_DIP.h"      // STM32F1                                env:STM32F103RE_btt env:STM32F103RE_btt_USB env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_CR6)
  #include "stm32f1/pins_BTT_SKR_CR6.h"         // STM32F1                                env:STM32F103RE_btt env:STM32F103RE_btt_USB
#elif MB(JGAURORA_A5S_A1)
  #include "stm32f1/pins_JGAURORA_A5S_A1.h"     // STM32F1                                env:jgaurora_a5s_a1
#elif MB(FYSETC_AIO_II)
  #include "stm32f1/pins_FYSETC_AIO_II.h"       // STM32F1                                env:STM32F103RC_fysetc
#elif MB(FYSETC_CHEETAH)
  #include "stm32f1/pins_FYSETC_CHEETAH.h"      // STM32F1                                env:STM32F103RC_fysetc
#elif MB(FYSETC_CHEETAH_V12)
  #include "stm32f1/pins_FYSETC_CHEETAH_V12.h"  // STM32F1                                env:STM32F103RC_fysetc
#elif MB(LONGER3D_LK)
  #include "stm32f1/pins_LONGER3D_LK.h"         // STM32F1                                env:STM32F103VE_longer
#elif MB(CCROBOT_MEEB_3DP)
  #include "stm32f1/pins_CCROBOT_MEEB_3DP.h"    // STM32F1                                env:STM32F103RC_meeb
#elif MB(CHITU3D_V5)
  #include "stm32f1/pins_CHITU3D_V5.h"          // STM32F1                                env:chitu_f103 env:chitu_v5_gpio_init
#elif MB(CHITU3D_V6)
  #include "stm32f1/pins_CHITU3D_V6.h"          // STM32F1                                env:chitu_f103
#elif MB(CREALITY_V4)
  #include "stm32f1/pins_CREALITY_V4.h"         // STM32F1                                env:STM32F103RET6_creality
#elif MB(CREALITY_V4210)
  #include "stm32f1/pins_CREALITY_V4210.h"      // STM32F1                                env:STM32F103RET6_creality
#elif MB(CREALITY_V427)
  #include "stm32f1/pins_CREALITY_V427.h"       // STM32F1                                env:STM32F103RET6_creality
#elif MB(CREALITY_V431)
  #include "stm32f1/pins_CREALITY_V431.h"       // STM32F1                                env:STM32F103RET6_creality
#elif MB(CREALITY_V452)
  #include "stm32f1/pins_CREALITY_V452.h"       // STM32F1                                env:STM32F103RET6_creality
#elif MB(CREALITY_V453)
  #include "stm32f1/pins_CREALITY_V453.h"       // STM32F1                                env:STM32F103RET6_creality
#elif MB(TRIGORILLA_PRO)
  #include "stm32f1/pins_TRIGORILLA_PRO.h"      // STM32F1                                env:trigorilla_pro
#elif MB(FLY_MINI)
  #include "stm32f1/pins_FLY_MINI.h"            // STM32F1                                env:FLY_MINI
#elif MB(FLSUN_HISPEED)
  #include "stm32f1/pins_FLSUN_HISPEED.h"       // STM32F1                                env:flsun_hispeed
#elif MB(BEAST)
  #include "stm32f1/pins_BEAST.h"               // STM32F1                                env:STM32F103RE
#elif MB(MINGDA_MPX_ARM_MINI)
  #include "stm32f1/pins_MINGDA_MPX_ARM_MINI.h" // STM32F1                                env:STM32F103RE

//
// ARM Cortex-M4F
//

#elif MB(TEENSY31_32)
  #include "teensy3/pins_TEENSY31_32.h"         // TEENSY31_32                            env:teensy31
#elif MB(TEENSY35_36)
  #include "teensy3/pins_TEENSY35_36.h"         // TEENSY35_36                            env:teensy35 env:teensy36

//
// STM32 ARM Cortex-M4F
//

#elif MB(ARMED)
  #include "stm32f4/pins_ARMED.h"               // STM32F4                                env:ARMED
#elif MB(RUMBA32_V1_0, RUMBA32_V1_1)
  #include "stm32f4/pins_RUMBA32_AUS3D.h"       // STM32F4                                env:rumba32
#elif MB(RUMBA32_MKS)
  #include "stm32f4/pins_RUMBA32_MKS.h"         // STM32F4                                env:rumba32
#elif MB(BLACK_STM32F407VE)
  #include "stm32f4/pins_BLACK_STM32F407VE.h"   // STM32F4                                env:STM32F407VE_black
#elif MB(STEVAL_3DP001V1)
  #include "stm32f4/pins_STEVAL_3DP001V1.h"     // STM32F4                                env:STM32F401VE_STEVAL
#elif MB(BTT_SKR_PRO_V1_1)
  #include "stm32f4/pins_BTT_SKR_PRO_V1_1.h"    // STM32F4                                env:BIGTREE_SKR_PRO env:BIGTREE_SKR_PRO_usb_flash_drive
#elif MB(BTT_SKR_PRO_V1_2)
  #include "stm32f4/pins_BTT_SKR_PRO_V1_2.h"    // STM32F4                                env:BIGTREE_SKR_PRO env:BIGTREE_SKR_PRO_usb_flash_drive
#elif MB(BTT_GTR_V1_0)
  #include "stm32f4/pins_BTT_GTR_V1_0.h"        // STM32F4                                env:BIGTREE_GTR_V1_0 env:BIGTREE_GTR_V1_0_usb_flash_drive
#elif MB(BTT_BTT002_V1_0)
  #include "stm32f4/pins_BTT_BTT002_V1_0.h"     // STM32F4                                env:BIGTREE_BTT002
#elif MB(LERDGE_K)
  #include "stm32f4/pins_LERDGE_K.h"            // STM32F4                                env:LERDGEK env:LERDGEK_usb_flash_drive
#elif MB(LERDGE_S)
  #include "stm32f4/pins_LERDGE_S.h"            // STM32F4                                env:LERDGES env:LERDGES_usb_flash_drive
#elif MB(LERDGE_X)
  #include "stm32f4/pins_LERDGE_X.h"            // STM32F4                                env:LERDGEX env:LERDGEX_usb_flash_drive
#elif MB(VAKE403D)
  #include "stm32f4/pins_VAKE403D.h"            // STM32F4
#elif MB(FYSETC_S6)
  #include "stm32f4/pins_FYSETC_S6.h"           // STM32F4                                env:FYSETC_S6
#elif MB(FYSETC_S6_V2_0)
  #include "stm32f4/pins_FYSETC_S6_V2_0.h"      // STM32F4                                env:FYSETC_S6
#elif MB(FLYF407ZG)
  #include "stm32f4/pins_FLYF407ZG.h"           // STM32F4                                env:FLYF407ZG
#elif MB(MKS_ROBIN2)
  #include "stm32f4/pins_MKS_ROBIN2.h"          // STM32F4                                env:MKS_ROBIN2
#elif MB(MKS_ROBIN_PRO_V2)
  #include "stm32f4/pins_MKS_ROBIN_PRO_V2.h"    // STM32F4                                env:mks_robin_pro2
#elif MB(MKS_ROBIN_NANO_V3)
  #include "stm32f4/pins_MKS_ROBIN_NANO_V3.h"   // STM32F4                                env:mks_robin_nano_v3 env:mks_robin_nano_v3_usb_flash_drive env:mks_robin_nano_v3_usb_flash_drive_msc
#elif MB(ANET_ET4)
  #include "stm32f4/pins_ANET_ET4.h"            // STM32F4                                env:Anet_ET4_OpenBLT
#elif MB(ANET_ET4P)
  #include "stm32f4/pins_ANET_ET4P.h"           // STM32F4                                env:Anet_ET4_OpenBLT
#elif MB(FYSETC_CHEETAH_V20)
  #include "stm32f4/pins_FYSETC_CHEETAH_V20.h"  // STM32F4                                env:FYSETC_CHEETAH_V20

//
// ARM Cortex M7
//

#elif MB(REMRAM_V1)
  #include "stm32f7/pins_REMRAM_V1.h"           // STM32F7                                env:REMRAM_V1
#elif MB(NUCLEO_F767ZI)
  #include "stm32f7/pins_NUCLEO_F767ZI.h"       // STM32F7                                env:NUCLEO_F767ZI
#elif MB(TEENSY41)
  #include "teensy4/pins_TEENSY41.h"            // Teensy-4.x                             env:teensy41
#elif MB(T41U5XBB)
  #include "teensy4/pins_T41U5XBB.h"            // Teensy-4.x                             env:teensy41

//
// Espressif ESP32
//

#elif MB(ESPRESSIF_ESP32)
  #include "esp32/pins_ESP32.h"                 // ESP32                                  env:esp32
#elif MB(MRR_ESPA)
  #include "esp32/pins_MRR_ESPA.h"              // ESP32                                  env:esp32
#elif MB(MRR_ESPE)
  #include "esp32/pins_MRR_ESPE.h"              // ESP32                                  env:esp32
#elif MB(E4D_BOX)
  #include "esp32/pins_E4D.h"                   // ESP32                                  env:esp32
#elif MB(FYSETC_E4)
  #include "esp32/pins_FYSETC_E4.h"             // ESP32                                  env:FYSETC_E4

//
// Adafruit Grand Central M4 (SAMD51 ARM Cortex-M4)
//

#elif MB(AGCM4_RAMPS_144)
  #include "samd/pins_RAMPS_144.h"              // SAMD51                                 env:SAMD51_grandcentral_m4

//
// Custom board (with custom PIO env)
//

#elif MB(CUSTOM)
  #include "pins_custom.h"                      //                                        env:custom

//
// Linux Native Debug board
//

#elif MB(LINUX_RAMPS)
  #include "linux/pins_RAMPS_LINUX.h"           // Linux                                  env:linux_native

#else

  //
  // Obsolete or unknown board
  //

  #define BOARD_MKS_13                  -1000
  #define BOARD_TRIGORILLA              -1001
  #define BOARD_RURAMPS4D               -1002
  #define BOARD_FORMBOT_TREX2           -1003
  #define BOARD_BIQU_SKR_V1_1           -1004
  #define BOARD_STM32F1R                -1005
  #define BOARD_STM32F103R              -1006
  #define BOARD_ESP32                   -1007
  #define BOARD_STEVAL                  -1008
  #define BOARD_BIGTREE_SKR_V1_1        -1009
  #define BOARD_BIGTREE_SKR_V1_3        -1010
  #define BOARD_BIGTREE_SKR_V1_4        -1011
  #define BOARD_BIGTREE_SKR_V1_4_TURBO  -1012
  #define BOARD_BIGTREE_BTT002_V1_0     -1013
  #define BOARD_BIGTREE_SKR_PRO_V1_1    -1014
  #define BOARD_BIGTREE_SKR_MINI_V1_1   -1015
  #define BOARD_BIGTREE_SKR_MINI_E3     -1016
  #define BOARD_BIGTREE_SKR_E3_DIP      -1017
  #define BOARD_RUMBA32                 -1018
  #define BOARD_RUMBA32_AUS3D           -1019
  #define BOARD_RAMPS_DAGOMA            -1020
  #define BOARD_RAMPS_LONGER3D_LK4PRO   -1021

  #if MB(MKS_13)
    #error "BOARD_MKS_13 has been renamed BOARD_MKS_GEN_13. Please update your configuration."
  #elif MB(TRIGORILLA)
    #error "BOARD_TRIGORILLA has been renamed BOARD_TRIGORILLA_13. Please update your configuration."
  #elif MB(RURAMPS4D)
    #error "BOARD_RURAMPS4D has been renamed BOARD_RURAMPS4D_11. Please update your configuration."
  #elif MB(FORMBOT_TREX2)
    #error "FORMBOT_TREX2 has been renamed BOARD_FORMBOT_TREX2PLUS. Please update your configuration."
  #elif MB(BIQU_SKR_V1_1)
    #error "BOARD_BIQU_SKR_V1_1 has been renamed BOARD_BTT_SKR_V1_1. Please update your configuration."
  #elif MB(BIGTREE_SKR_V1_1)
    #error "BOARD_BIGTREE_SKR_V1_1 has been renamed BOARD_BTT_SKR_V1_1. Please update your configuration."
  #elif MB(BIGTREE_SKR_V2_2)
    #error "BOARD_BIGTREE_SKR_V1_2 has been renamed BOARD_BTT_SKR_V1_2. Please update your configuration."
  #elif MB(BIGTREE_SKR_V1_3)
    #error "BOARD_BIGTREE_SKR_V1_3 has been renamed BOARD_BTT_SKR_V1_3. Please update your configuration."
  #elif MB(BIGTREE_SKR_V1_4)
    #error "BOARD_BIGTREE_SKR_V1_4 has been renamed BOARD_BTT_SKR_V1_4. Please update your configuration."
  #elif MB(BIGTREE_SKR_V1_4_TURBO)
    #error "BOARD_BIGTREE_SKR_V1_4_TURBO has been renamed BOARD_BTT_SKR_V1_4_TURBO. Please update your configuration."
  #elif MB(BIGTREE_BTT002_V1_0)
    #error "BOARD_BIGTREE_BTT002_V1_0 has been renamed BOARD_BTT_BTT002_V1_0. Please update your configuration."
  #elif MB(BIGTREE_SKR_PRO_V1_1)
    #error "BOARD_BIGTREE_SKR_PRO_V1_1 has been renamed BOARD_BTT_SKR_PRO_V1_1. Please update your configuration."
  #elif MB(BIGTREE_SKR_MINI_V1_1)
    #error "BOARD_BIGTREE_SKR_MINI_V1_1 has been renamed BOARD_BTT_SKR_MINI_V1_1. Please update your configuration."
  #elif MB(BIGTREE_SKR_MINI_E3)
    #error "BOARD_BIGTREE_SKR_MINI_E3 has been renamed BOARD_BTT_SKR_MINI_E3_V1_0. Please update your configuration."
  #elif MB(BIGTREE_SKR_E3_DIP)
    #error "BOARD_BIGTREE_SKR_E3_DIP has been renamed BOARD_BTT_SKR_E3_DIP. Please update your configuration."
  #elif MB(STM32F1R)
    #error "BOARD_STM32F1R has been renamed BOARD_STM32F103RE. Please update your configuration."
  #elif MB(STM32F103R)
    #error "BOARD_STM32F103R has been renamed BOARD_STM32F103RE. Please update your configuration."
  #elif MOTHERBOARD == BOARD_ESP32
    #error "BOARD_ESP32 has been renamed BOARD_ESPRESSIF_ESP32. Please update your configuration."
  #elif MB(STEVAL)
    #error "BOARD_STEVAL has been renamed BOARD_STEVAL_3DP001V1. Please update your configuration."
  #elif MB(RUMBA32)
    #error "BOARD_RUMBA32 is now BOARD_RUMBA32_MKS or BOARD_RUMBA32_V1_0. Please update your configuration."
  #elif MB(RUMBA32_AUS3D)
    #error "BOARD_RUMBA32_AUS3D is now BOARD_RUMBA32_V1_0. Please update your configuration."
  #elif MB(RAMPS_DAGOMA)
    #error "BOARD_RAMPS_DAGOMA is now BOARD_DAGOMA_F5. Please update your configuration."
  #elif MB(RAMPS_LONGER3D_LK4PRO)
    #error "BOARD_RAMPS_LONGER3D_LK4PRO is now BOARD_LONGER3D_LKx_PRO. Please update your configuration."
  #else
    #error "Unknown MOTHERBOARD value set in Configuration.h"
  #endif

  #undef BOARD_MKS_13
  #undef BOARD_TRIGORILLA
  #undef BOARD_RURAMPS4D
  #undef BOARD_FORMBOT_TREX2
  #undef BOARD_BIQU_SKR_V1_1
  #undef BOARD_STM32F1R
  #undef BOARD_STM32F103R
  #undef BOARD_ESP32
  #undef BOARD_STEVAL
  #undef BOARD_BIGTREE_SKR_MINI_E3
  #undef BOARD_BIGTREE_SKR_V1_1
  #undef BOARD_BIGTREE_SKR_V1_3
  #undef BOARD_BIGTREE_SKR_V1_4
  #undef BOARD_BIGTREE_SKR_V1_4_TURBO
  #undef BOARD_BIGTREE_BTT002_V1_0
  #undef BOARD_BIGTREE_SKR_PRO_V1_1
  #undef BOARD_BIGTREE_SKR_MINI_V1_1
  #undef BOARD_BIGTREE_SKR_E3_DIP
  #undef BOARD_RUMBA32
  #undef BOARD_RUMBA32_AUS3D
  #undef BOARD_RAMPS_DAGOMA
  #undef BOARD_RAMPS_LONGER3D_LK4PRO

#endif

//
// Post-process pins according to configured settings
//
#include "pins_postprocess.h"
