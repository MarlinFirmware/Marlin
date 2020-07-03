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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../core/boards.h"

/**
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

#define MAX_EXTRUDERS 8

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

#define HAS_FREE_AUX2_PINS !(BOTH(ULTRA_LCD, NEWPANEL) && ANY(PANEL_ONE, VIKI2, miniVIKI, MINIPANEL, REPRAPWORLD_KEYPAD))

//
// RAMPS 1.3 / 1.4 - ATmega1280, ATmega2560
//

#if MB(RAMPS_OLD)
  #include "ramps/pins_RAMPS_OLD.h"             // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_13_EFB)
  #include "ramps/pins_RAMPS_13.h"              // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_13_EEB)
  #include "ramps/pins_RAMPS_13.h"              // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_13_EFF)
  #include "ramps/pins_RAMPS_13.h"              // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_13_EEF)
  #include "ramps/pins_RAMPS_13.h"              // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_13_SF)
  #include "ramps/pins_RAMPS_13.h"              // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_14_EFB)
  #include "ramps/pins_RAMPS.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_14_EEB)
  #include "ramps/pins_RAMPS.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_14_EFF)
  #include "ramps/pins_RAMPS.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_14_EEF)
  #include "ramps/pins_RAMPS.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_14_SF)
  #include "ramps/pins_RAMPS.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_PLUS_EFB)
  #include "ramps/pins_RAMPS_PLUS.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_PLUS_EEB)
  #include "ramps/pins_RAMPS_PLUS.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_PLUS_EFF)
  #include "ramps/pins_RAMPS_PLUS.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_PLUS_EEF)
  #include "ramps/pins_RAMPS_PLUS.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RAMPS_PLUS_SF)
  #include "ramps/pins_RAMPS_PLUS.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560

//
// RAMPS Derivatives - ATmega1280, ATmega2560
//

#elif MB(3DRAG)
  #include "ramps/pins_3DRAG.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(K8200)
  #include "ramps/pins_K8200.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560 (3DRAG)
#elif MB(K8400)
  #include "ramps/pins_K8400.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560 (3DRAG)
#elif MB(K8800)
  #include "ramps/pins_K8800.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560 (3DRAG)
#elif MB(BAM_DICE)
  #include "ramps/pins_RAMPS.h"                 // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(BAM_DICE_DUE)
  #include "ramps/pins_BAM_DICE_DUE.h"          // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
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
  #include "ramps/pins_MKS_GEN_13.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(MKS_GEN_L)
  #include "ramps/pins_MKS_GEN_L.h"             // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(KFB_2)
  #include "ramps/pins_BIQU_KFB_2.h"            // ATmega2560                             env:mega2560
#elif MB(ZRIB_V20)
  #include "ramps/pins_ZRIB_V20.h"              // ATmega1280, ATmega2560                 env:mega1280 env:mega2560 (MKS_GEN_13)
#elif MB(FELIX2)
  #include "ramps/pins_FELIX2.h"                // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RIGIDBOARD)
  #include "ramps/pins_RIGIDBOARD.h"            // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(RIGIDBOARD_V2)
  #include "ramps/pins_RIGIDBOARD_V2.h"         // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(SAINSMART_2IN1)
  #include "ramps/pins_SAINSMART_2IN1.h"        // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(ULTIMAKER)
  #include "ramps/pins_ULTIMAKER.h"             // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(ULTIMAKER_OLD)
  #include "ramps/pins_ULTIMAKER_OLD.h"         // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(AZTEEG_X3)
  #include "ramps/pins_AZTEEG_X3.h"             // ATmega2560                             env:mega2560
#elif MB(AZTEEG_X3_PRO)
  #include "ramps/pins_AZTEEG_X3_PRO.h"         // ATmega2560                             env:mega2560
#elif MB(ULTIMAIN_2)
  #include "ramps/pins_ULTIMAIN_2.h"            // ATmega2560                             env:mega2560
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
  #include "ramps/pins_BQ_ZUM_MEGA_3D.h"        // ATmega2560                             env:mega2560
#elif MB(MAKEBOARD_MINI)
  #include "ramps/pins_MAKEBOARD_MINI.h"        // ATmega2560                             env:mega2560
#elif MB(TRIGORILLA_13)
  #include "ramps/pins_TRIGORILLA_13.h"         // ATmega2560                             env:mega2560
#elif MB(TRIGORILLA_14)
  #include "ramps/pins_TRIGORILLA_14.h"         // ATmega2560                             env:mega2560
#elif MB(TRIGORILLA_14_11)
  #include "ramps/pins_TRIGORILLA_14.h"         // ATmega2560                             env:mega2560
#elif MB(RAMPS_ENDER_4)
  #include "ramps/pins_RAMPS_ENDER_4.h"         // ATmega2560                             env:mega2560
#elif MB(RAMPS_CREALITY)
  #include "ramps/pins_RAMPS_CREALITY.h"        // ATmega2560                             env:mega2560
#elif MB(RAMPS_DAGOMA)
  #include "ramps/pins_RAMPS_DAGOMA.h"          // ATmega2560                             env:mega2560
#elif MB(FYSETC_F6_13)
  #include "ramps/pins_FYSETC_F6_13.h"          // ATmega2560                             env:FYSETC_F6_13
#elif MB(FYSETC_F6_14)
  #include "ramps/pins_FYSETC_F6_14.h"          // ATmega2560                             env:FYSETC_F6_14
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

//
// Other ATmega1280, ATmega2560
//

#elif MB(CNCONTROLS_11)
  #include "mega/pins_CNCONTROLS_11.h"          // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(CNCONTROLS_12)
  #include "mega/pins_CNCONTROLS_12.h"          // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(CNCONTROLS_15)
  #include "mega/pins_CNCONTROLS_15.h"          // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(MIGHTYBOARD_REVE)
  #include "mega/pins_MIGHTYBOARD_REVE.h"       // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
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
  #include "mega/pins_LEAPFROG.h"               // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(MEGACONTROLLER)
  #include "mega/pins_MEGACONTROLLER.h"         // ATmega2560                             env:mega2560
#elif MB(GT2560_REV_A)
  #include "mega/pins_GT2560_REV_A.h"           // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(GT2560_REV_A_PLUS)
  #include "mega/pins_GT2560_REV_A_PLUS.h"      // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(GT2560_V3)
  #include "mega/pins_GT2560_V3.h"              // ATmega2560                             env:mega2560
#elif MB(GT2560_V3_MC2)
  #include "mega/pins_GT2560_V3_MC2.h"          // ATmega2560                             env:mega2560
#elif MB(GT2560_V3_A20)
  #include "mega/pins_GT2560_V3_A20.h"          // ATmega2560                             env:mega2560
#elif MB(EINSTART_S)
  #include "mega/pins_EINSTART-S.h"             // ATmega1280, ATmega2560                 env:mega1280 env:mega2560
#elif MB(WANHAO_ONEPLUS)
  #include "mega/pins_WANHAO_ONEPLUS.h"         // ATmega2560                             env:mega2560
#elif MB(OVERLORD)
  #include "mega/pins_OVERLORD.h"               // ATmega2560                             env:mega2560
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
  #include "sanguino/pins_SANGUINOLOLU_11.h"    // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(SANGUINOLOLU_12)
  #include "sanguino/pins_SANGUINOLOLU_12.h"    // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(MELZI)
  #include "sanguino/pins_MELZI.h"              // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(MELZI_MAKR3D)
  #include "sanguino/pins_MELZI_MAKR3D.h"       // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(MELZI_CREALITY)
  #include "sanguino/pins_MELZI_CREALITY.h"     // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(MELZI_MALYAN)
  #include "sanguino/pins_MELZI_MALYAN.h"       // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(MELZI_TRONXY)
  #include "sanguino/pins_MELZI_TRONXY.h"       // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(STB_11)
  #include "sanguino/pins_STB_11.h"             // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(AZTEEG_X1)
  #include "sanguino/pins_AZTEEG_X1.h"          // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p

//
// Other ATmega644P, ATmega644, ATmega1284P
//

#elif MB(GEN3_MONOLITHIC)
  #include "sanguino/pins_GEN3_MONOLITHIC.h"    // ATmega644P                             env:sanguino644p
#elif MB(GEN3_PLUS)
  #include "sanguino/pins_GEN3_PLUS.h"          // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(GEN6)
  #include "sanguino/pins_GEN6.h"               // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(GEN6_DELUXE)
  #include "sanguino/pins_GEN6_DELUXE.h"        // ATmega644P, ATmega1284P                env:sanguino644p env:sanguino1284p
#elif MB(GEN7_CUSTOM)
  #include "sanguino/pins_GEN7_CUSTOM.h"        // ATmega644P, ATmega644, ATmega1284P     env:sanguino644p env:sanguino1284p
#elif MB(GEN7_12)
  #include "sanguino/pins_GEN7_12.h"            // ATmega644P, ATmega644, ATmega1284P     env:sanguino644p env:sanguino1284p
#elif MB(GEN7_13)
  #include "sanguino/pins_GEN7_13.h"            // ATmega644P, ATmega644, ATmega1284P     env:sanguino644p env:sanguino1284p
#elif MB(GEN7_14)
  #include "sanguino/pins_GEN7_14.h"            // ATmega644P, ATmega644, ATmega1284P     env:sanguino644p env:sanguino1284p
#elif MB(OMCA_A)
  #include "sanguino/pins_OMCA_A.h"             // ATmega644                              env:sanguino644p
#elif MB(OMCA)
  #include "sanguino/pins_OMCA.h"               // ATmega644P, ATmega644                  env:sanguino644p
#elif MB(ANET_10)
  #include "sanguino/pins_ANET_10.h"            // ATmega1284P                            env:sanguino1284p
#elif MB(SETHI)
  #include "sanguino/pins_SETHI.h"              // ATmega644P, ATmega644, ATmega1284P     env:sanguino644p env:sanguino1284p

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

#elif MB(RAMPS_14_RE_ARM_EFB)
  #include "lpc1768/pins_RAMPS_RE_ARM.h"        // LPC1768                                env:LPC1768
#elif MB(RAMPS_14_RE_ARM_EEB)
  #include "lpc1768/pins_RAMPS_RE_ARM.h"        // LPC1768                                env:LPC1768
#elif MB(RAMPS_14_RE_ARM_EFF)
  #include "lpc1768/pins_RAMPS_RE_ARM.h"        // LPC1768                                env:LPC1768
#elif MB(RAMPS_14_RE_ARM_EEF)
  #include "lpc1768/pins_RAMPS_RE_ARM.h"        // LPC1768                                env:LPC1768
#elif MB(RAMPS_14_RE_ARM_SF)
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
#elif MB(RAMPS_SMART_EFB)
  #include "sam/pins_RAMPS_SMART.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_SMART_EEB)
  #include "sam/pins_RAMPS_SMART.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_SMART_EFF)
  #include "sam/pins_RAMPS_SMART.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_SMART_EEF)
  #include "sam/pins_RAMPS_SMART.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_SMART_SF)
  #include "sam/pins_RAMPS_SMART.h"             // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_DUO_EFB)
  #include "sam/pins_RAMPS_DUO.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_DUO_EEB)
  #include "sam/pins_RAMPS_DUO.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_DUO_EFF)
  #include "sam/pins_RAMPS_DUO.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_DUO_EEF)
  #include "sam/pins_RAMPS_DUO.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS_DUO_SF)
  #include "sam/pins_RAMPS_DUO.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS4DUE_EFB)
  #include "sam/pins_RAMPS4DUE.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS4DUE_EEB)
  #include "sam/pins_RAMPS4DUE.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS4DUE_EFF)
  #include "sam/pins_RAMPS4DUE.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS4DUE_EEF)
  #include "sam/pins_RAMPS4DUE.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(RAMPS4DUE_SF)
  #include "sam/pins_RAMPS4DUE.h"               // SAM3X8E                                env:DUE env:DUE_USB env:DUE_debug
#elif MB(ULTRATRONICS_PRO)
  #include "sam/pins_ULTRATRONICS_PRO.h"        // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(ARCHIM1)
  #include "sam/pins_ARCHIM1.h"                 // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(ARCHIM2)
  #include "sam/pins_ARCHIM2.h"                 // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(ALLIGATOR)
  #include "sam/pins_ALLIGATOR_R2.h"            // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(ADSK)
  #include "sam/pins_ADSK.h"                    // SAM3X8E                                env:DUE env:DUE_debug
#elif MB(PRINTRBOARD_G2)
  #include "sam/pins_PRINTRBOARD_G2.h"          // SAM3X8C                                env:DUE_USB
#elif MB(CNCONTROLS_15D)
  #include "sam/pins_CNCONTROLS_15D.h"          // SAM3X8E                                env:DUE env:DUE_USB

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
#elif MB(GTM32_MINI_A30)
  #include "stm32f1/pins_GTM32_MINI_A30.h"      // STM32F1                                env:STM32F103RE
#elif MB(GTM32_MINI)
  #include "stm32f1/pins_GTM32_MINI.h"          // STM32F1                                env:STM32F103RE
#elif MB(GTM32_REV_B)
  #include "stm32f1/pins_GTM32_REV_B.h"         // STM32F1                                env:STM32F103RE
#elif MB(MORPHEUS)
  #include "stm32f1/pins_MORPHEUS.h"            // STM32F1                                env:STM32F103RE
#elif MB(CHITU3D)
  #include "stm32f1/pins_CHITU3D.h"             // STM32F1                                env:STM32F103RE
#elif MB(MKS_ROBIN)
  #include "stm32f1/pins_MKS_ROBIN.h"           // STM32F1                                env:mks_robin
#elif MB(MKS_ROBIN_MINI)
  #include "stm32f1/pins_MKS_ROBIN_MINI.h"      // STM32F1                                env:mks_robin_mini
#elif MB(MKS_ROBIN_NANO)
  #include "stm32f1/pins_MKS_ROBIN_NANO.h"      // STM32F1                                env:mks_robin_nano
#elif MB(MKS_ROBIN_LITE)
  #include "stm32f1/pins_MKS_ROBIN_LITE.h"      // STM32F1                                env:mks_robin_lite
#elif MB(BTT_SKR_MINI_V1_1)
  #include "stm32f1/pins_BTT_SKR_MINI_V1_1.h"   // STM32F1                                env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_MINI_E3_V1_0)
  #include "stm32f1/pins_BTT_SKR_MINI_E3_V1_0.h"  // STM32F1                              env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_MINI_E3_V1_2)
  #include "stm32f1/pins_BTT_SKR_MINI_E3_V1_2.h"  // STM32F1                              env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
#elif MB(BTT_SKR_E3_DIP)
  #include "stm32f1/pins_BTT_SKR_E3_DIP.h"      // STM32F1                                env:STM32F103RE_btt env:STM32F103RE_btt_USB env:STM32F103RC_btt env:STM32F103RC_btt_512K env:STM32F103RC_btt_USB env:STM32F103RC_btt_512K_USB
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
#elif MB(MKS_ROBIN_LITE3)
  #include "stm32f1/pins_MKS_ROBIN_LITE3.h"     // STM32F1                                env:mks_robin_lite3
#elif MB(MKS_ROBIN_PRO)
  #include "stm32f1/pins_MKS_ROBIN_PRO.h"       // STM32F1                                env:mks_robin_pro

//
// ARM Cortex-M4F
//

#elif MB(TEENSY31_32)
  #include "teensy3/pins_TEENSY31_32.h"         // TEENSY31_32                            env:teensy31
#elif MB(TEENSY35_36)
  #include "teensy3/pins_TEENSY35_36.h"         // TEENSY35_36                            env:teensy35

//
// STM32 ARM Cortex-M4F
//

#elif MB(BEAST)
  #include "stm32f4/pins_BEAST.h"               // STM32F4                                env:STM32F4
#elif MB(GENERIC_STM32F4)
  #include "stm32f4/pins_GENERIC_STM32F4.h"     // STM32F4                                env:STM32F4
#elif MB(ARMED)
  #include "stm32f4/pins_ARMED.h"               // STM32F4                                env:ARMED
#elif MB(RUMBA32_AUS3D)
  #include "stm32f4/pins_RUMBA32_AUS3D.h"       // STM32F4                                env:rumba32_f446ve
#elif MB(RUMBA32_MKS)
  #include "stm32f4/pins_RUMBA32_MKS.h"         // STM32F4                                env:rumba32_mks
#elif MB(BLACK_STM32F407VE)
  #include "stm32f4/pins_BLACK_STM32F407VE.h"   // STM32F4                                env:STM32F407VE_black
#elif MB(STEVAL_3DP001V1)
  #include "stm32f4/pins_STEVAL_3DP001V1.h"     // STM32F4                                env:STM32F401VE_STEVAL
#elif MB(BTT_SKR_PRO_V1_1)
  #include "stm32f4/pins_BTT_SKR_PRO_V1_1.h"    // STM32F4                                env:BIGTREE_SKR_PRO
#elif MB(BTT_GTR_V1_0)
  #include "stm32f4/pins_BTT_GTR_V1_0.h"        // STM32F4                                env:BIGTREE_GTR_V1_0
#elif MB(BTT_BTT002_V1_0)
  #include "stm32f4/pins_BTT_BTT002_V1_0.h"     // STM32F4                                env:BIGTREE_BTT002
#elif MB(LERDGE_K)
  #include "stm32f4/pins_LERDGE_K.h"            // STM32F4                                env:STM32F4
#elif MB(LERDGE_X)
  #include "stm32f4/pins_LERDGE_X.h"            // STM32F4                                env:STM32F4
#elif MB(VAKE403D)
  #include "stm32f4/pins_VAKE403D.h"            // STM32F4                                env:STM32F4
#elif MB(FYSETC_S6)
  #include "stm32f4/pins_FYSETC_S6.h"           // STM32F4                                env:FYSETC_S6
#elif MB(FLYF407ZG)
  #include "stm32f4/pins_FLYF407ZG.h"           // STM32F4                                env:FLYF407ZG
#elif MB(MKS_ROBIN2)
  #include "stm32f4/pins_MKS_ROBIN2.h"          // STM32F4                                env:MKS_ROBIN2

//
// ARM Cortex M7
//

#elif MB(THE_BORG)
  #include "stm32f7/pins_THE_BORG.h"            // STM32F7                                env:STM32F7
#elif MB(REMRAM_V1)
  #include "stm32f7/pins_REMRAM_V1.h"           // STM32F7                                env:STM32F7

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

//
// Adafruit Grand Central M4 (SAMD51 ARM Cortex-M4)
//

#elif MB(AGCM4_RAMPS_144)
  #include "samd/pins_RAMPS_144.h"              // SAMD51                                 env:SAMD51_grandcentral_m4

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
    #error "BOARD_RUMBA32 is now BOARD_RUMBA32_MKS or BOARD_RUMBA32_AUS3D. Please update your configuration."
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

#endif

// Define certain undefined pins
#ifndef X_MS1_PIN
  #define X_MS1_PIN -1
#endif
#ifndef X_MS2_PIN
  #define X_MS2_PIN -1
#endif
#ifndef X_MS3_PIN
  #define X_MS3_PIN -1
#endif
#ifndef Y_MS1_PIN
  #define Y_MS1_PIN -1
#endif
#ifndef Y_MS2_PIN
  #define Y_MS2_PIN -1
#endif
#ifndef Y_MS3_PIN
  #define Y_MS3_PIN -1
#endif
#ifndef Z_MS1_PIN
  #define Z_MS1_PIN -1
#endif
#ifndef Z_MS2_PIN
  #define Z_MS2_PIN -1
#endif
#ifndef Z_MS3_PIN
  #define Z_MS3_PIN -1
#endif
#ifndef E0_MS1_PIN
  #define E0_MS1_PIN -1
#endif
#ifndef E0_MS2_PIN
  #define E0_MS2_PIN -1
#endif
#ifndef E0_MS3_PIN
  #define E0_MS3_PIN -1
#endif
#ifndef E1_MS1_PIN
  #define E1_MS1_PIN -1
#endif
#ifndef E1_MS2_PIN
  #define E1_MS2_PIN -1
#endif
#ifndef E1_MS3_PIN
  #define E1_MS3_PIN -1
#endif
#ifndef E2_MS1_PIN
  #define E2_MS1_PIN -1
#endif
#ifndef E2_MS2_PIN
  #define E2_MS2_PIN -1
#endif
#ifndef E2_MS3_PIN
  #define E2_MS3_PIN -1
#endif
#ifndef E3_MS1_PIN
  #define E3_MS1_PIN -1
#endif
#ifndef E3_MS2_PIN
  #define E3_MS2_PIN -1
#endif
#ifndef E3_MS3_PIN
  #define E3_MS3_PIN -1
#endif
#ifndef E4_MS1_PIN
  #define E4_MS1_PIN -1
#endif
#ifndef E4_MS2_PIN
  #define E4_MS2_PIN -1
#endif
#ifndef E4_MS3_PIN
  #define E4_MS3_PIN -1
#endif
#ifndef E5_MS1_PIN
  #define E5_MS1_PIN -1
#endif
#ifndef E5_MS2_PIN
  #define E5_MS2_PIN -1
#endif
#ifndef E5_MS3_PIN
  #define E5_MS3_PIN -1
#endif
#ifndef E6_MS1_PIN
  #define E6_MS1_PIN -1
#endif
#ifndef E6_MS2_PIN
  #define E6_MS2_PIN -1
#endif
#ifndef E6_MS3_PIN
  #define E6_MS3_PIN -1
#endif
#ifndef E7_MS1_PIN
  #define E7_MS1_PIN -1
#endif
#ifndef E7_MS2_PIN
  #define E7_MS2_PIN -1
#endif
#ifndef E7_MS3_PIN
  #define E7_MS3_PIN -1
#endif

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN -1
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN -1
#endif
#ifndef E0_ENABLE_PIN
  #define E0_ENABLE_PIN -1
#endif
#ifndef E1_STEP_PIN
  #define E1_STEP_PIN -1
#endif
#ifndef E1_DIR_PIN
  #define E1_DIR_PIN -1
#endif
#ifndef E1_ENABLE_PIN
  #define E1_ENABLE_PIN -1
#endif
#ifndef E2_STEP_PIN
  #define E2_STEP_PIN -1
#endif
#ifndef E2_DIR_PIN
  #define E2_DIR_PIN -1
#endif
#ifndef E2_ENABLE_PIN
  #define E2_ENABLE_PIN -1
#endif
#ifndef E3_STEP_PIN
  #define E3_STEP_PIN -1
#endif
#ifndef E3_DIR_PIN
  #define E3_DIR_PIN -1
#endif
#ifndef E3_ENABLE_PIN
  #define E3_ENABLE_PIN -1
#endif
#ifndef E4_STEP_PIN
  #define E4_STEP_PIN -1
#endif
#ifndef E4_DIR_PIN
  #define E4_DIR_PIN -1
#endif
#ifndef E4_ENABLE_PIN
  #define E4_ENABLE_PIN -1
#endif
#ifndef E5_STEP_PIN
  #define E5_STEP_PIN -1
#endif
#ifndef E5_DIR_PIN
  #define E5_DIR_PIN -1
#endif
#ifndef E5_ENABLE_PIN
  #define E5_ENABLE_PIN -1
#endif
#ifndef E6_STEP_PIN
  #define E6_STEP_PIN -1
#endif
#ifndef E6_DIR_PIN
  #define E6_DIR_PIN -1
#endif
#ifndef E6_ENABLE_PIN
  #define E6_ENABLE_PIN -1
#endif
#ifndef E7_STEP_PIN
  #define E7_STEP_PIN -1
#endif
#ifndef E7_DIR_PIN
  #define E7_DIR_PIN -1
#endif
#ifndef E7_ENABLE_PIN
  #define E7_ENABLE_PIN -1
#endif

//
// Destroy unused CS pins
//
#if !AXIS_HAS_SPI(X)
  #undef X_CS_PIN
#endif
#if !AXIS_HAS_SPI(Y)
  #undef Y_CS_PIN
#endif
#if !AXIS_HAS_SPI(Z)
  #undef Z_CS_PIN
#endif
#if E_STEPPERS && !AXIS_HAS_SPI(E0)
  #undef E0_CS_PIN
#endif
#if E_STEPPERS > 1 && !AXIS_HAS_SPI(E1)
  #undef E1_CS_PIN
#endif
#if E_STEPPERS > 2 && !AXIS_HAS_SPI(E2)
  #undef E2_CS_PIN
#endif
#if E_STEPPERS > 3 && !AXIS_HAS_SPI(E3)
  #undef E3_CS_PIN
#endif
#if E_STEPPERS > 4 && !AXIS_HAS_SPI(E4)
  #undef E4_CS_PIN
#endif
#if E_STEPPERS > 5 && !AXIS_HAS_SPI(E5)
  #undef E5_CS_PIN
#endif
#if E_STEPPERS > 6 && !AXIS_HAS_SPI(E6)
  #undef E6_CS_PIN
#endif
#if E_STEPPERS > 7 && !AXIS_HAS_SPI(E7)
  #undef E7_CS_PIN
#endif

#ifndef X_CS_PIN
  #define X_CS_PIN -1
#endif
#ifndef Y_CS_PIN
  #define Y_CS_PIN -1
#endif
#ifndef Z_CS_PIN
  #define Z_CS_PIN -1
#endif
#ifndef E0_CS_PIN
  #define E0_CS_PIN -1
#endif
#ifndef E1_CS_PIN
  #define E1_CS_PIN -1
#endif
#ifndef E2_CS_PIN
  #define E2_CS_PIN -1
#endif
#ifndef E3_CS_PIN
  #define E3_CS_PIN -1
#endif
#ifndef E4_CS_PIN
  #define E4_CS_PIN -1
#endif
#ifndef E5_CS_PIN
  #define E5_CS_PIN -1
#endif
#ifndef E6_CS_PIN
  #define E6_CS_PIN -1
#endif
#ifndef E7_CS_PIN
  #define E7_CS_PIN -1
#endif

#ifndef FAN_PIN
  #define FAN_PIN -1
#endif
#define FAN0_PIN FAN_PIN
#ifndef FAN1_PIN
  #define FAN1_PIN -1
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN -1
#endif
#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN  -1
#endif

#ifndef FANMUX0_PIN
  #define FANMUX0_PIN -1
#endif
#ifndef FANMUX1_PIN
  #define FANMUX1_PIN -1
#endif
#ifndef FANMUX2_PIN
  #define FANMUX2_PIN -1
#endif

#ifndef HEATER_0_PIN
  #define HEATER_0_PIN -1
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN -1
#endif
#ifndef HEATER_2_PIN
  #define HEATER_2_PIN -1
#endif
#ifndef HEATER_3_PIN
  #define HEATER_3_PIN -1
#endif
#ifndef HEATER_4_PIN
  #define HEATER_4_PIN -1
#endif
#ifndef HEATER_5_PIN
  #define HEATER_5_PIN -1
#endif
#ifndef HEATER_6_PIN
  #define HEATER_6_PIN -1
#endif
#ifndef HEATER_7_PIN
  #define HEATER_7_PIN -1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN -1
#endif

#ifndef TEMP_0_PIN
  #define TEMP_0_PIN -1
#endif
#ifndef TEMP_1_PIN
  #define TEMP_1_PIN -1
#endif
#ifndef TEMP_2_PIN
  #define TEMP_2_PIN -1
#endif
#ifndef TEMP_3_PIN
  #define TEMP_3_PIN -1
#endif
#ifndef TEMP_4_PIN
  #define TEMP_4_PIN -1
#endif
#ifndef TEMP_5_PIN
  #define TEMP_5_PIN -1
#endif
#ifndef TEMP_6_PIN
  #define TEMP_6_PIN -1
#endif
#ifndef TEMP_7_PIN
  #define TEMP_7_PIN -1
#endif
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN -1
#endif

#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN -1
#endif
#ifndef SDPOWER_PIN
  #define SDPOWER_PIN -1
#endif
#ifndef SDSS
  #define SDSS -1
#endif
#ifndef LED_PIN
  #define LED_PIN -1
#endif
#if DISABLED(PSU_CONTROL) || !defined(PS_ON_PIN)
  #undef PS_ON_PIN
  #define PS_ON_PIN -1
#endif
#ifndef KILL_PIN
  #define KILL_PIN -1
#endif
#ifndef SUICIDE_PIN
  #define SUICIDE_PIN -1
#endif
#ifndef SUICIDE_PIN_INVERTING
  #define SUICIDE_PIN_INVERTING false
#endif

#ifndef NUM_SERVO_PLUGS
  #define NUM_SERVO_PLUGS 4
#endif

//
// Assign auto fan pins if needed
//
#ifndef E0_AUTO_FAN_PIN
  #ifdef ORIG_E0_AUTO_FAN_PIN
    #define E0_AUTO_FAN_PIN ORIG_E0_AUTO_FAN_PIN
  #else
    #define E0_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E1_AUTO_FAN_PIN
  #ifdef ORIG_E1_AUTO_FAN_PIN
    #define E1_AUTO_FAN_PIN ORIG_E1_AUTO_FAN_PIN
  #else
    #define E1_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E2_AUTO_FAN_PIN
  #ifdef ORIG_E2_AUTO_FAN_PIN
    #define E2_AUTO_FAN_PIN ORIG_E2_AUTO_FAN_PIN
  #else
    #define E2_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E3_AUTO_FAN_PIN
  #ifdef ORIG_E3_AUTO_FAN_PIN
    #define E3_AUTO_FAN_PIN ORIG_E3_AUTO_FAN_PIN
  #else
    #define E3_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E4_AUTO_FAN_PIN
  #ifdef ORIG_E4_AUTO_FAN_PIN
    #define E4_AUTO_FAN_PIN ORIG_E4_AUTO_FAN_PIN
  #else
    #define E4_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E5_AUTO_FAN_PIN
  #ifdef ORIG_E5_AUTO_FAN_PIN
    #define E5_AUTO_FAN_PIN ORIG_E5_AUTO_FAN_PIN
  #else
    #define E5_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E6_AUTO_FAN_PIN
  #ifdef ORIG_E6_AUTO_FAN_PIN
    #define E6_AUTO_FAN_PIN ORIG_E6_AUTO_FAN_PIN
  #else
    #define E6_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef E7_AUTO_FAN_PIN
  #ifdef ORIG_E7_AUTO_FAN_PIN
    #define E7_AUTO_FAN_PIN ORIG_E7_AUTO_FAN_PIN
  #else
    #define E7_AUTO_FAN_PIN -1
  #endif
#endif
#ifndef CHAMBER_AUTO_FAN_PIN
  #ifdef ORIG_CHAMBER_AUTO_FAN_PIN
    #define CHAMBER_AUTO_FAN_PIN ORIG_CHAMBER_AUTO_FAN_PIN
  #else
    #define CHAMBER_AUTO_FAN_PIN -1
  #endif
#endif

//
// Assign endstop pins for boards with only 3 connectors
//
#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
    #define X_MIN_PIN X_STOP_PIN
    #ifndef X_MAX_PIN
      #define X_MAX_PIN -1
    #endif
  #else
    #define X_MAX_PIN X_STOP_PIN
    #ifndef X_MIN_PIN
      #define X_MIN_PIN -1
    #endif
  #endif
#elif X_HOME_DIR < 0
  #define X_STOP_PIN X_MIN_PIN
#else
  #define X_STOP_PIN X_MAX_PIN
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
    #define Y_MIN_PIN Y_STOP_PIN
    #ifndef Y_MAX_PIN
      #define Y_MAX_PIN -1
    #endif
  #else
    #define Y_MAX_PIN Y_STOP_PIN
    #ifndef Y_MIN_PIN
      #define Y_MIN_PIN -1
    #endif
  #endif
#elif Y_HOME_DIR < 0
  #define Y_STOP_PIN Y_MIN_PIN
#else
  #define Y_STOP_PIN Y_MAX_PIN
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
    #define Z_MIN_PIN Z_STOP_PIN
    #ifndef Z_MAX_PIN
      #define Z_MAX_PIN -1
    #endif
  #else
    #define Z_MAX_PIN Z_STOP_PIN
    #ifndef Z_MIN_PIN
      #define Z_MIN_PIN -1
    #endif
  #endif
#elif Z_HOME_DIR < 0
  #define Z_STOP_PIN Z_MIN_PIN
#else
  #define Z_STOP_PIN Z_MAX_PIN
#endif

//
// Disable unused endstop / probe pins
//
#if !HAS_CUSTOM_PROBE_PIN
  #undef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN    -1
#endif

#if DISABLED(USE_XMAX_PLUG)
  #undef X_MAX_PIN
  #define X_MAX_PIN          -1
#endif

#if DISABLED(USE_YMAX_PLUG)
  #undef Y_MAX_PIN
  #define Y_MAX_PIN          -1
#endif

#if DISABLED(USE_ZMAX_PLUG)
  #undef Z_MAX_PIN
  #define Z_MAX_PIN          -1
#endif

#if DISABLED(USE_XMIN_PLUG)
  #undef X_MIN_PIN
  #define X_MIN_PIN          -1
#endif

#if DISABLED(USE_YMIN_PLUG)
  #undef Y_MIN_PIN
  #define Y_MIN_PIN          -1
#endif

#if DISABLED(USE_ZMIN_PLUG)
  #undef Z_MIN_PIN
  #define Z_MIN_PIN          -1
#endif

#if HAS_FILAMENT_SENSOR
  #define FIL_RUNOUT1_PIN FIL_RUNOUT_PIN
#else
  #undef FIL_RUNOUT_PIN
  #undef FIL_RUNOUT1_PIN
#endif

#ifndef LCD_PINS_D4
  #define LCD_PINS_D4 -1
#endif

#if HAS_CHARACTER_LCD || TOUCH_UI_ULTIPANEL
  #ifndef LCD_PINS_D5
    #define LCD_PINS_D5 -1
  #endif
  #ifndef LCD_PINS_D6
    #define LCD_PINS_D6 -1
  #endif
  #ifndef LCD_PINS_D7
    #define LCD_PINS_D7 -1
  #endif
#endif

/**
 * Auto-Assignment for Dual X, Dual Y, Multi-Z Steppers
 *
 * By default X2 is assigned to the next open E plug
 * on the board, then in order, Y2, Z2, Z3. These can be
 * overridden in Configuration.h or Configuration_adv.h.
 */

#define __PEXI(p,q) PIN_EXISTS(E##p##_##q)
#define _PEXI(p,q) __PEXI(p,q)
#define __EPIN(p,q) E##p##_##q##_PIN
#define _EPIN(p,q) __EPIN(p,q)
#define DIAG_REMAPPED(p,q) (PIN_EXISTS(q) && _EPIN(p##_E_INDEX, DIAG) == q##_PIN)

// The X2 axis, if any, should be the next open extruder port
#define X2_E_INDEX E_STEPPERS

#if EITHER(DUAL_X_CARRIAGE, X_DUAL_STEPPER_DRIVERS)
  #ifndef X2_STEP_PIN
    #define X2_STEP_PIN   _EPIN(X2_E_INDEX, STEP)
    #define X2_DIR_PIN    _EPIN(X2_E_INDEX, DIR)
    #define X2_ENABLE_PIN _EPIN(X2_E_INDEX, ENABLE)
    #if X2_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(X2_STEP)
      #error "No E stepper plug left for X2!"
    #endif
  #endif
  #ifndef X2_MS1_PIN
    #define X2_MS1_PIN    _EPIN(X2_E_INDEX, MS1)
  #endif
  #ifndef X2_MS2_PIN
    #define X2_MS2_PIN    _EPIN(X2_E_INDEX, MS2)
  #endif
  #ifndef X2_MS3_PIN
    #define X2_MS3_PIN    _EPIN(X2_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_SPI(X2) && !defined(X2_CS_PIN)
    #define X2_CS_PIN     _EPIN(X2_E_INDEX, CS)
  #endif
  #if AXIS_HAS_UART(X2)
    #ifndef X2_SERIAL_TX_PIN
      #define X2_SERIAL_TX_PIN _EPIN(X2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef X2_SERIAL_RX_PIN
      #define X2_SERIAL_RX_PIN _EPIN(X2_E_INDEX, SERIAL_RX)
    #endif
  #endif

  //
  // Auto-assign pins for stallGuard sensorless homing
  //
  #if X2_STALL_SENSITIVITY && ENABLED(X_DUAL_ENDSTOPS) && _PEXI(X2_E_INDEX, DIAG)
    #define X2_DIAG_PIN _EPIN(X2_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(X2, X_MIN)      // If already remapped in the pins file...
      #define X2_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(X2, Y_MIN)
      #define X2_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(X2, Z_MIN)
      #define X2_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(X2, X_MAX)
      #define X2_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(X2, Y_MAX)
      #define X2_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(X2, Z_MAX)
      #define X2_USE_ENDSTOP _ZMAX_
    #else                               // Otherwise use the driver DIAG_PIN directly
      #define _X2_USE_ENDSTOP(P) _E##P##_DIAG_
      #define X2_USE_ENDSTOP _X2_USE_ENDSTOP(X2_E_INDEX)
    #endif
    #undef X2_DIAG_PIN
  #endif

  #define Y2_E_INDEX INCREMENT(X2_E_INDEX)
#else
  #define Y2_E_INDEX X2_E_INDEX
#endif

#ifndef X2_CS_PIN
  #define X2_CS_PIN  -1
#endif
#ifndef X2_MS1_PIN
  #define X2_MS1_PIN -1
#endif
#ifndef X2_MS2_PIN
  #define X2_MS2_PIN -1
#endif
#ifndef X2_MS3_PIN
  #define X2_MS3_PIN -1
#endif

// The Y2 axis, if any, should be the next open extruder port
#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #ifndef Y2_STEP_PIN
    #define Y2_STEP_PIN   _EPIN(Y2_E_INDEX, STEP)
    #define Y2_DIR_PIN    _EPIN(Y2_E_INDEX, DIR)
    #define Y2_ENABLE_PIN _EPIN(Y2_E_INDEX, ENABLE)
    #if Y2_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Y2_STEP)
      #error "No E stepper plug left for Y2!"
    #endif
  #endif
  #ifndef Y2_MS1_PIN
    #define Y2_MS1_PIN    _EPIN(Y2_E_INDEX, MS1)
  #endif
  #ifndef Y2_MS2_PIN
    #define Y2_MS2_PIN    _EPIN(Y2_E_INDEX, MS2)
  #endif
  #ifndef Y2_MS3_PIN
    #define Y2_MS3_PIN    _EPIN(Y2_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_SPI(Y2) && !defined(Y2_CS_PIN)
    #define Y2_CS_PIN     _EPIN(Y2_E_INDEX, CS)
  #endif
  #if AXIS_HAS_UART(Y2)
    #ifndef Y2_SERIAL_TX_PIN
      #define Y2_SERIAL_TX_PIN _EPIN(Y2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Y2_SERIAL_RX_PIN
      #define Y2_SERIAL_RX_PIN _EPIN(Y2_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if Y2_STALL_SENSITIVITY && ENABLED(Y_DUAL_ENDSTOPS) && _PEXI(Y2_E_INDEX, DIAG)
    #define Y2_DIAG_PIN _EPIN(Y2_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Y2, X_MIN)
      #define Y2_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Y2, Y_MIN)
      #define Y2_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Y2, Z_MIN)
      #define Y2_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Y2, X_MAX)
      #define Y2_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Y2, Y_MAX)
      #define Y2_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Y2, Z_MAX)
      #define Y2_USE_ENDSTOP _ZMAX_
    #else
      #define _Y2_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Y2_USE_ENDSTOP _Y2_USE_ENDSTOP(Y2_E_INDEX)
    #endif
    #undef Y2_DIAG_PIN
  #endif
  #define Z2_E_INDEX INCREMENT(Y2_E_INDEX)
#else
  #define Z2_E_INDEX Y2_E_INDEX
#endif

#ifndef Y2_CS_PIN
  #define Y2_CS_PIN  -1
#endif
#ifndef Y2_MS1_PIN
  #define Y2_MS1_PIN -1
#endif
#ifndef Y2_MS2_PIN
  #define Y2_MS2_PIN -1
#endif
#ifndef Y2_MS3_PIN
  #define Y2_MS3_PIN -1
#endif

// The Z2 axis, if any, should be the next open extruder port
#if NUM_Z_STEPPER_DRIVERS >= 2
  #ifndef Z2_STEP_PIN
    #define Z2_STEP_PIN   _EPIN(Z2_E_INDEX, STEP)
    #define Z2_DIR_PIN    _EPIN(Z2_E_INDEX, DIR)
    #define Z2_ENABLE_PIN _EPIN(Z2_E_INDEX, ENABLE)
    #if Z2_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Z2_STEP)
      #error "No E stepper plug left for Z2!"
    #endif
  #endif
  #ifndef Z2_MS1_PIN
    #define Z2_MS1_PIN    _EPIN(Z2_E_INDEX, MS1)
  #endif
  #ifndef Z2_MS2_PIN
    #define Z2_MS2_PIN    _EPIN(Z2_E_INDEX, MS2)
  #endif
  #ifndef Z2_MS3_PIN
    #define Z2_MS3_PIN    _EPIN(Z2_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_SPI(Z2) && !defined(Z2_CS_PIN)
    #define Z2_CS_PIN     _EPIN(Z2_E_INDEX, CS)
  #endif
  #if AXIS_HAS_UART(Z2)
    #ifndef Z2_SERIAL_TX_PIN
      #define Z2_SERIAL_TX_PIN _EPIN(Z2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z2_SERIAL_RX_PIN
      #define Z2_SERIAL_RX_PIN _EPIN(Z2_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if Z2_STALL_SENSITIVITY && ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 2 && _PEXI(Z2_E_INDEX, DIAG)
    #define Z2_DIAG_PIN _EPIN(Z2_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Z2, X_MIN)
      #define Z2_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Z2, Y_MIN)
      #define Z2_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Z2, Z_MIN)
      #define Z2_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Z2, X_MAX)
      #define Z2_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Z2, Y_MAX)
      #define Z2_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Z2, Z_MAX)
      #define Z2_USE_ENDSTOP _ZMAX_
    #else
      #define _Z2_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Z2_USE_ENDSTOP _Z2_USE_ENDSTOP(Z2_E_INDEX)
    #endif
    #undef Z2_DIAG_PIN
  #endif
  #define Z3_E_INDEX INCREMENT(Z2_E_INDEX)
#else
  #define Z3_E_INDEX Z2_E_INDEX
#endif

#ifndef Z2_CS_PIN
  #define Z2_CS_PIN  -1
#endif
#ifndef Z2_MS1_PIN
  #define Z2_MS1_PIN -1
#endif
#ifndef Z2_MS2_PIN
  #define Z2_MS2_PIN -1
#endif
#ifndef Z2_MS3_PIN
  #define Z2_MS3_PIN -1
#endif

#if NUM_Z_STEPPER_DRIVERS >= 3
  #ifndef Z3_STEP_PIN
    #define Z3_STEP_PIN   _EPIN(Z3_E_INDEX, STEP)
    #define Z3_DIR_PIN    _EPIN(Z3_E_INDEX, DIR)
    #define Z3_ENABLE_PIN _EPIN(Z3_E_INDEX, ENABLE)
    #if Z3_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Z3_STEP)
      #error "No E stepper plug left for Z3!"
    #endif
  #endif
  #if AXIS_HAS_SPI(Z3)
    #ifndef Z3_CS_PIN
      #define Z3_CS_PIN   _EPIN(Z3_E_INDEX, CS)
    #endif
  #endif
  #ifndef Z3_MS1_PIN
    #define Z3_MS1_PIN    _EPIN(Z3_E_INDEX, MS1)
  #endif
  #ifndef Z3_MS2_PIN
    #define Z3_MS2_PIN    _EPIN(Z3_E_INDEX, MS2)
  #endif
  #ifndef Z3_MS3_PIN
    #define Z3_MS3_PIN    _EPIN(Z3_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_UART(Z3)
    #ifndef Z3_SERIAL_TX_PIN
      #define Z3_SERIAL_TX_PIN _EPIN(Z3_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z3_SERIAL_RX_PIN
      #define Z3_SERIAL_RX_PIN _EPIN(Z3_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if Z3_STALL_SENSITIVITY && ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 3 && _PEXI(Z3_E_INDEX, DIAG)
    #define Z3_DIAG_PIN _EPIN(Z3_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Z3, X_MIN)
      #define Z3_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Z3, Y_MIN)
      #define Z3_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Z3, Z_MIN)
      #define Z3_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Z3, X_MAX)
      #define Z3_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Z3, Y_MAX)
      #define Z3_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Z3, Z_MAX)
      #define Z3_USE_ENDSTOP _ZMAX_
    #else
      #define _Z3_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Z3_USE_ENDSTOP _Z3_USE_ENDSTOP(Z3_E_INDEX)
    #endif
    #undef Z3_DIAG_PIN
  #endif
  #define Z4_E_INDEX INCREMENT(Z3_E_INDEX)
#endif

#ifndef Z3_CS_PIN
  #define Z3_CS_PIN  -1
#endif
#ifndef Z3_MS1_PIN
  #define Z3_MS1_PIN -1
#endif
#ifndef Z3_MS2_PIN
  #define Z3_MS2_PIN -1
#endif
#ifndef Z3_MS3_PIN
  #define Z3_MS3_PIN -1
#endif

#if NUM_Z_STEPPER_DRIVERS >= 4
  #ifndef Z4_STEP_PIN
    #define Z4_STEP_PIN   _EPIN(Z4_E_INDEX, STEP)
    #define Z4_DIR_PIN    _EPIN(Z4_E_INDEX, DIR)
    #define Z4_ENABLE_PIN _EPIN(Z4_E_INDEX, ENABLE)
    #if Z4_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Z4_STEP)
      #error "No E stepper plug left for Z4!"
    #endif
  #endif
  #if AXIS_HAS_SPI(Z4)
    #ifndef Z4_CS_PIN
      #define Z4_CS_PIN     _EPIN(Z4_E_INDEX, CS)
    #endif
  #endif
  #ifndef Z4_MS1_PIN
    #define Z4_MS1_PIN    _EPIN(Z4_E_INDEX, MS1)
  #endif
  #ifndef Z4_MS2_PIN
    #define Z4_MS2_PIN    _EPIN(Z4_E_INDEX, MS2)
  #endif
  #ifndef Z4_MS3_PIN
    #define Z4_MS3_PIN    _EPIN(Z4_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_UART(Z4)
    #ifndef Z4_SERIAL_TX_PIN
      #define Z4_SERIAL_TX_PIN _EPIN(Z4_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z4_SERIAL_RX_PIN
      #define Z4_SERIAL_RX_PIN _EPIN(Z4_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if Z4_STALL_SENSITIVITY && ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 4 && _PEXI(Z4_E_INDEX, DIAG)
    #define Z4_DIAG_PIN _EPIN(Z4_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Z4, X_MIN)
      #define Z4_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Z4, Y_MIN)
      #define Z4_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Z4, Z_MIN)
      #define Z4_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Z4, X_MAX)
      #define Z4_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Z4, Y_MAX)
      #define Z4_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Z4, Z_MAX)
      #define Z4_USE_ENDSTOP _ZMAX_
    #else
      #define _Z4_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Z4_USE_ENDSTOP _Z4_USE_ENDSTOP(Z4_E_INDEX)
    #endif
    #undef Z4_DIAG_PIN
  #endif
#endif

#ifndef Z4_CS_PIN
  #define Z4_CS_PIN  -1
#endif
#ifndef Z4_MS1_PIN
  #define Z4_MS1_PIN -1
#endif
#ifndef Z4_MS2_PIN
  #define Z4_MS2_PIN -1
#endif
#ifndef Z4_MS3_PIN
  #define Z4_MS3_PIN -1
#endif

#if HAS_GRAPHICAL_LCD
  #if !defined(ST7920_DELAY_1) && defined(BOARD_ST7920_DELAY_1)
    #define ST7920_DELAY_1 BOARD_ST7920_DELAY_1
  #endif
  #if !defined(ST7920_DELAY_2) && defined(BOARD_ST7920_DELAY_2)
    #define ST7920_DELAY_2 BOARD_ST7920_DELAY_2
  #endif
  #if !defined(ST7920_DELAY_3) && defined(BOARD_ST7920_DELAY_3)
    #define ST7920_DELAY_3 BOARD_ST7920_DELAY_3
  #endif
#else
  #undef ST7920_DELAY_1
  #undef ST7920_DELAY_2
  #undef ST7920_DELAY_3
#endif

#undef HAS_FREE_AUX2_PINS
#undef DIAG_REMAPPED
