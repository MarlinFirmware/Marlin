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
 * Finnish
 *
 * LCD Menu Messages
 * See also https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language
 *
 */
#ifndef LANGUAGE_FI_H
#define LANGUAGE_FI_H

#define MAPPER_C2C3
#define DISPLAY_CHARSET_ISO10646_1

#define WELCOME_MSG                         MACHINE_NAME " valmis."
#define MSG_SD_INSERTED                     "Kortti asetettu"
#define MSG_SD_REMOVED                      "Kortti poistettu"
#define MSG_MAIN                            "Palaa"
#define MSG_AUTOSTART                       "Automaatti"
#define MSG_DISABLE_STEPPERS                "Vapauta moottorit"
#define MSG_AUTO_HOME                       "Aja referenssiin"
#define MSG_LEVEL_BED_HOMING                "Homing XYZ"
#define MSG_LEVEL_BED_WAITING               "Click to Begin"
#define MSG_LEVEL_BED_DONE                  "Leveling Done!"
#define MSG_LEVEL_BED_CANCEL                "Cancel"
#define MSG_SET_HOME_OFFSETS                "Set home offsets"
#define MSG_HOME_OFFSETS_APPLIED            "Offsets applied"
#define MSG_SET_ORIGIN                      "Aseta origo"
#define MSG_PREHEAT_1                       "Esilämmitä PLA"
#define MSG_PREHEAT_1_N                     "Esilämmitä PLA "
#define MSG_PREHEAT_1_ALL                   "Esilä. PLA Kaikki"
#define MSG_PREHEAT_1_BEDONLY               "Esilä. PLA Alusta"
#define MSG_PREHEAT_1_SETTINGS              "Esilämm. PLA konf"
#define MSG_PREHEAT_2                       "Esilämmitä ABS"
#define MSG_PREHEAT_2_N                     "Esilämmitä ABS "
#define MSG_PREHEAT_2_ALL                   "Esilä. ABS Kaikki"
#define MSG_PREHEAT_2_BEDONLY               "Esilä. ABS Alusta"
#define MSG_PREHEAT_2_SETTINGS              "Esilämm. ABS konf"
#define MSG_COOLDOWN                        "Jäähdytä"
#define MSG_SWITCH_PS_ON                    "Virta päälle"
#define MSG_SWITCH_PS_OFF                   "Virta pois"
#define MSG_EXTRUDE                         "Pursota"
#define MSG_RETRACT                         "Vedä takaisin"
#define MSG_MOVE_AXIS                       "Liikuta akseleita"
#define MSG_MOVE_X                          "Liikuta X"
#define MSG_MOVE_Y                          "Liikuta Y"
#define MSG_MOVE_Z                          "Liikuta Z"
#define MSG_MOVE_E                          "Extruder"
#define MSG_MOVE_01MM                       "Liikuta 0.1mm"
#define MSG_MOVE_1MM                        "Liikuta 1mm"
#define MSG_MOVE_10MM                       "Liikuta 10mm"
#define MSG_SPEED                           "Nopeus"
#define MSG_NOZZLE                          "Suutin"
#define MSG_BED                             "Alusta"
#define MSG_FAN_SPEED                       "Tuul. nopeus"
#define MSG_FLOW                            "Virtaus"
#define MSG_CONTROL                         "Kontrolli"
#define MSG_MIN                             LCD_STR_THERMOMETER " Min"
#define MSG_MAX                             LCD_STR_THERMOMETER " Max"
#define MSG_FACTOR                          LCD_STR_THERMOMETER " Kerr"
#define MSG_AUTOTEMP                        "Autotemp"
#define MSG_ON                              "On "
#define MSG_OFF                             "Off"
#define MSG_PID_P                           "PID-P"
#define MSG_PID_I                           "PID-I"
#define MSG_PID_D                           "PID-D"
#define MSG_PID_C                           "PID-C"
#define MSG_ACC                             "Kiihtyv"
#define MSG_VXY_JERK                        "Vxy-jerk"
#define MSG_VZ_JERK                         "Vz-jerk"
#define MSG_VE_JERK                         "Ve-jerk"
#define MSG_VMAX                            "Vmax "
#define MSG_VMIN                            "Vmin"
#define MSG_VTRAV_MIN                       "VLiike min"
#define MSG_AMAX                            "Amax "
#define MSG_A_RETRACT                       "A-peruuta"
#define MSG_XSTEPS                          "Xsteps/mm"
#define MSG_YSTEPS                          "Ysteps/mm"
#define MSG_ZSTEPS                          "Zsteps/mm"
#define MSG_ESTEPS                          "Esteps/mm"
#define MSG_TEMPERATURE                     "Lämpötila"
#define MSG_MOTION                          "Liike"
#define MSG_VOLUMETRIC                      "Filament"
#define MSG_VOLUMETRIC_ENABLED              "E in mm³"
#define MSG_FILAMENT_DIAM                   "Fil. Dia."
#define MSG_CONTRAST                        "LCD kontrasti"
#define MSG_STORE_EPROM                     "Tallenna muistiin"
#define MSG_LOAD_EPROM                      "Lataa muistista"
#define MSG_RESTORE_FAILSAFE                "Palauta oletus"
#define MSG_REFRESH                         "Päivitä"
#define MSG_WATCH                           "Seuraa"
#define MSG_PREPARE                         "Valmistele"
#define MSG_TUNE                            "Säädä"
#define MSG_PAUSE_PRINT                     "Keskeytä tulostus"
#define MSG_RESUME_PRINT                    "Jatka tulostusta"
#define MSG_STOP_PRINT                      "Pysäytä tulostus"
#define MSG_CARD_MENU                       "Korttivalikko"
#define MSG_NO_CARD                         "Ei korttia"
#define MSG_DWELL                           "Nukkumassa..."
#define MSG_USERWAIT                        "Odotet. valintaa"
#define MSG_RESUMING                        "Jatke. tulostusta"
#define MSG_PRINT_ABORTED                   "Print aborted"
#define MSG_NO_MOVE                         "Ei liiketta."
#define MSG_KILLED                          "KILLED. "
#define MSG_STOPPED                         "STOPPED. "
#define MSG_CONTROL_RETRACT                 "Vedä mm"
#define MSG_CONTROL_RETRACT_SWAP            "Va. Vedä mm"
#define MSG_CONTROL_RETRACTF                "Vedä V"
#define MSG_CONTROL_RETRACT_ZLIFT           "Z mm"
#define MSG_CONTROL_RETRACT_RECOVER         "UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVER_SWAP    "Va. UnRet +mm"
#define MSG_CONTROL_RETRACT_RECOVERF        "UnRet  V"
#define MSG_AUTORETRACT                     "AutoVeto."
#define MSG_FILAMENTCHANGE                  "Change filament"
#define MSG_INIT_SDCARD                     "Init. SD-Card"
#define MSG_CNG_SDCARD                      "Change SD-Card"
#define MSG_ZPROBE_OUT                      "Z probe out. bed"
#define MSG_HOME                            "Home"  // Used as MSG_HOME " " MSG_X MSG_Y MSG_Z " " MSG_FIRST
#define MSG_FIRST                           "first"
#define MSG_ZPROBE_ZOFFSET                  "Z Offset"
#define MSG_BABYSTEP_X                      "Babystep X"
#define MSG_BABYSTEP_Y                      "Babystep Y"
#define MSG_BABYSTEP_Z                      "Babystep Z"
#define MSG_ENDSTOP_ABORT                   "Endstop abort"
#define MSG_DELTA_CALIBRATE                 "Delta Kalibrointi"
#define MSG_DELTA_CALIBRATE_X               "Kalibroi X"
#define MSG_DELTA_CALIBRATE_Y               "Kalibroi Y"
#define MSG_DELTA_CALIBRATE_Z               "Kalibroi Z"
#define MSG_DELTA_CALIBRATE_CENTER          "Kalibroi Center"

#endif // LANGUAGE_FI_H
