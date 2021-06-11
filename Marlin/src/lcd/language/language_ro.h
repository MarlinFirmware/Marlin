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
  * Romanian
  *
  * LCD Menu Messages
  * See also https://marlinfw.org/docs/development/lcd_language.html
  *
  * Translation by cristyanul
  */
namespace Language_ro {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t    CHARSIZE                            = 2;
  PROGMEM Language_Str LANGUAGE                            = _UxGT("Romanian");

  PROGMEM Language_Str WELCOME_MSG                         = MACHINE_NAME _UxGT(" Pregatit.");
  PROGMEM Language_Str MSG_MARLIN                          = _UxGT("Marlin");
  PROGMEM Language_Str MSG_YES                             = _UxGT("DA");
  PROGMEM Language_Str MSG_NO                              = _UxGT("NU");
  PROGMEM Language_Str MSG_BACK                            = _UxGT("Inapoi");
  PROGMEM Language_Str MSG_MEDIA_ABORTING                  = _UxGT("Abandon...");
  PROGMEM Language_Str MSG_MEDIA_INSERTED                  = _UxGT("Media Introdus");
  PROGMEM Language_Str MSG_MEDIA_REMOVED                   = _UxGT("Media Inlaturat");
  PROGMEM Language_Str MSG_MEDIA_WAITING                   = _UxGT("Astept Media");
  PROGMEM Language_Str MSG_MEDIA_READ_ERROR                = _UxGT("Eroare Citire Media");
  PROGMEM Language_Str MSG_MEDIA_USB_REMOVED               = _UxGT("Dispozitiv USB Inlaturat");
  PROGMEM Language_Str MSG_MEDIA_USB_FAILED                = _UxGT("Pornire USB Esuata");
  PROGMEM Language_Str MSG_KILL_SUBCALL_OVERFLOW           = _UxGT("Eroare:Subcall Overflow");
  PROGMEM Language_Str MSG_LCD_ENDSTOPS                    = _UxGT("Endstops"); // Max length 8 characters
  PROGMEM Language_Str MSG_LCD_SOFT_ENDSTOPS               = _UxGT("Soft Endstops");
  PROGMEM Language_Str MSG_MAIN                            = _UxGT("Principal");
  PROGMEM Language_Str MSG_ADVANCED_SETTINGS               = _UxGT("Setari Avansate");
  PROGMEM Language_Str MSG_CONFIGURATION                   = _UxGT("Configurare");
  PROGMEM Language_Str MSG_RUN_AUTO_FILES                  = _UxGT("Autostart");
  PROGMEM Language_Str MSG_DISABLE_STEPPERS                = _UxGT("Dezactivare Motoare");
  PROGMEM Language_Str MSG_DEBUG_MENU                      = _UxGT("Meniu Debug");
  PROGMEM Language_Str MSG_PROGRESS_BAR_TEST               = _UxGT("Test Bara Progres");
  PROGMEM Language_Str MSG_AUTO_HOME                       = _UxGT("Auto Acasa");
  PROGMEM Language_Str MSG_AUTO_HOME_X                     = _UxGT("Acasa X");
  PROGMEM Language_Str MSG_AUTO_HOME_Y                     = _UxGT("Acasa Y");
  PROGMEM Language_Str MSG_AUTO_HOME_Z                     = _UxGT("Acasa Z");
  PROGMEM Language_Str MSG_AUTO_Z_ALIGN                    = _UxGT("Auto Aliniere-Z");
  PROGMEM Language_Str MSG_LEVEL_BED_HOMING                = _UxGT("Acasa XYZ");
  PROGMEM Language_Str MSG_LEVEL_BED_WAITING               = _UxGT("Click pentru a incepe");
  PROGMEM Language_Str MSG_LEVEL_BED_NEXT_POINT            = _UxGT("Urmatorul Punct");
  PROGMEM Language_Str MSG_LEVEL_BED_DONE                  = _UxGT("Nivelare Terminata!");
  PROGMEM Language_Str MSG_Z_FADE_HEIGHT                   = _UxGT("Fade Inaltime");
  PROGMEM Language_Str MSG_SET_HOME_OFFSETS                = _UxGT("Seteaza Offseturile Acasa");
  PROGMEM Language_Str MSG_HOME_OFFSETS_APPLIED            = _UxGT("Offseturi Aplicate");
  PROGMEM Language_Str MSG_SET_ORIGIN                      = _UxGT("Seteaza Originea");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_PREHEAT_1                     = _UxGT("Preincalzeste ") PREHEAT_1_LABEL;
    PROGMEM Language_Str MSG_PREHEAT_1_H                   = _UxGT("Preincalzeste ") PREHEAT_1_LABEL " ~";
    PROGMEM Language_Str MSG_PREHEAT_1_END                 = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Capatul");
    PROGMEM Language_Str MSG_PREHEAT_1_END_E               = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Capatul ~");
    PROGMEM Language_Str MSG_PREHEAT_1_ALL                 = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Tot");
    PROGMEM Language_Str MSG_PREHEAT_1_BEDONLY             = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Patul");
    PROGMEM Language_Str MSG_PREHEAT_1_SETTINGS            = _UxGT("Preincalzeste ") PREHEAT_1_LABEL _UxGT(" Conf");

    PROGMEM Language_Str MSG_PREHEAT_M                     = _UxGT("Preincalzeste $");
    PROGMEM Language_Str MSG_PREHEAT_M_H                   = _UxGT("Preincalzeste $ ~");
    PROGMEM Language_Str MSG_PREHEAT_M_END                 = _UxGT("Preincalzeste $ Capatul");
    PROGMEM Language_Str MSG_PREHEAT_M_END_E               = _UxGT("Preincalzeste $ Capatul ~");
    PROGMEM Language_Str MSG_PREHEAT_M_ALL                 = _UxGT("Preincalzeste $ Tot");
    PROGMEM Language_Str MSG_PREHEAT_M_BEDONLY             = _UxGT("Preincalzeste $ Patul");
    PROGMEM Language_Str MSG_PREHEAT_M_SETTINGS            = _UxGT("Preincalzeste $ Conf");
  #endif
  PROGMEM Language_Str MSG_PREHEAT_CUSTOM                  = _UxGT("Preincalzeste Personalizat");
  PROGMEM Language_Str MSG_COOLDOWN                        = _UxGT("Racire");
  PROGMEM Language_Str MSG_CUTTER_FREQUENCY                = _UxGT("Frecventa");
  PROGMEM Language_Str MSG_LASER_MENU                      = _UxGT("Control Laser");
  PROGMEM Language_Str MSG_LASER_POWER                     = _UxGT("Puterea Laserului");
  PROGMEM Language_Str MSG_SPINDLE_MENU                    = _UxGT("Controlul Spindle");
  PROGMEM Language_Str MSG_SPINDLE_POWER                   = _UxGT("Puterea Spindle");
  PROGMEM Language_Str MSG_SPINDLE_REVERSE                 = _UxGT("Spindle Invers");
  PROGMEM Language_Str MSG_SWITCH_PS_ON                    = _UxGT("Porneste");
  PROGMEM Language_Str MSG_SWITCH_PS_OFF                   = _UxGT("Opreste");
  PROGMEM Language_Str MSG_EXTRUDE                         = _UxGT("Extrudeaza");
  PROGMEM Language_Str MSG_RETRACT                         = _UxGT("Retracteaza");
  PROGMEM Language_Str MSG_MOVE_AXIS                       = _UxGT("Muta Axa");
  PROGMEM Language_Str MSG_BED_LEVELING                    = _UxGT("Nivelarea Patului");
  PROGMEM Language_Str MSG_LEVEL_BED                       = _UxGT("Niveleaza Patul");
  PROGMEM Language_Str MSG_LEVEL_CORNERS                   = _UxGT("Niveleaza Colturile");
  PROGMEM Language_Str MSG_NEXT_CORNER                     = _UxGT("Urmatorul Colt");
  PROGMEM Language_Str MSG_MESH_EDITOR                     = _UxGT("Editor Mesh");
  PROGMEM Language_Str MSG_EDIT_MESH                       = _UxGT("Editeaza Mesh");
  PROGMEM Language_Str MSG_EDITING_STOPPED                 = _UxGT("Editarea Meshului Oprita");
  PROGMEM Language_Str MSG_PROBING_MESH                    = _UxGT("Punctul de Probare");
  PROGMEM Language_Str MSG_MESH_X                          = _UxGT("Index X");
  PROGMEM Language_Str MSG_MESH_Y                          = _UxGT("Index Y");
  PROGMEM Language_Str MSG_MESH_EDIT_Z                     = _UxGT("Valoare Z");
  PROGMEM Language_Str MSG_CUSTOM_COMMANDS                 = _UxGT("Comenzi Personalizate");
  PROGMEM Language_Str MSG_M48_TEST                        = _UxGT("M48 Probe Test");
  PROGMEM Language_Str MSG_M48_POINT                       = _UxGT("M48 Point");
  PROGMEM Language_Str MSG_M48_DEVIATION                   = _UxGT("Deviation");
  PROGMEM Language_Str MSG_IDEX_MENU                       = _UxGT("IDEX Mode");
  PROGMEM Language_Str MSG_OFFSETS_MENU                    = _UxGT("Tool Offsets");
  PROGMEM Language_Str MSG_IDEX_MODE_AUTOPARK              = _UxGT("Auto-Park");
  PROGMEM Language_Str MSG_IDEX_MODE_DUPLICATE             = _UxGT("Duplication");
  PROGMEM Language_Str MSG_IDEX_MODE_MIRRORED_COPY         = _UxGT("Mirrored Copy");
  PROGMEM Language_Str MSG_IDEX_MODE_FULL_CTRL             = _UxGT("Full Control");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_X                 = _UxGT("2nd Nozzle X");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Y                 = _UxGT("2nd Nozzle Y");
  PROGMEM Language_Str MSG_HOTEND_OFFSET_Z                 = _UxGT("2nd Nozzle Z");
  PROGMEM Language_Str MSG_UBL_DOING_G29                   = _UxGT("Doing G29");
  PROGMEM Language_Str MSG_UBL_TOOLS                       = _UxGT("UBL Tools");
  PROGMEM Language_Str MSG_UBL_LEVEL_BED                   = _UxGT("Unified Bed Leveling");
  PROGMEM Language_Str MSG_LCD_TILTING_MESH                = _UxGT("Tilting Point");
  PROGMEM Language_Str MSG_UBL_MANUAL_MESH                 = _UxGT("Manually Build Mesh");
  PROGMEM Language_Str MSG_UBL_BC_INSERT                   = _UxGT("Place Shim & Measure");
  PROGMEM Language_Str MSG_UBL_BC_INSERT2                  = _UxGT("Measure");
  PROGMEM Language_Str MSG_UBL_BC_REMOVE                   = _UxGT("Remove & Measure Bed");
  PROGMEM Language_Str MSG_UBL_MOVING_TO_NEXT              = _UxGT("Moving to next");
  PROGMEM Language_Str MSG_UBL_ACTIVATE_MESH               = _UxGT("Activate UBL");
  PROGMEM Language_Str MSG_UBL_DEACTIVATE_MESH             = _UxGT("Deactivate UBL");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_BED                = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_BED_TEMP_CUSTOM             = _UxGT("Bed Temp");
  PROGMEM Language_Str MSG_UBL_SET_TEMP_HOTEND             = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_HOTEND_TEMP_CUSTOM          = _UxGT("Hotend Temp");
  PROGMEM Language_Str MSG_UBL_MESH_EDIT                   = _UxGT("Mesh Edit");
  PROGMEM Language_Str MSG_UBL_EDIT_CUSTOM_MESH            = _UxGT("Edit Custom Mesh");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_MESH              = _UxGT("Fine Tuning Mesh");
  PROGMEM Language_Str MSG_UBL_DONE_EDITING_MESH           = _UxGT("Done Editing Mesh");
  PROGMEM Language_Str MSG_UBL_BUILD_CUSTOM_MESH           = _UxGT("Build Custom Mesh");
  PROGMEM Language_Str MSG_UBL_BUILD_MESH_MENU             = _UxGT("Build Mesh");
  #if PREHEAT_COUNT
    PROGMEM Language_Str MSG_UBL_BUILD_MESH_M              = _UxGT("Build Mesh ($)");
    PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_M           = _UxGT("Validate Mesh ($)");
  #endif
  PROGMEM Language_Str MSG_UBL_BUILD_COLD_MESH             = _UxGT("Build Cold Mesh");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_ADJUST          = _UxGT("Adjust Mesh Height");
  PROGMEM Language_Str MSG_UBL_MESH_HEIGHT_AMOUNT          = _UxGT("Height Amount");
  PROGMEM Language_Str MSG_UBL_VALIDATE_MESH_MENU          = _UxGT("Validate Mesh");
  PROGMEM Language_Str MSG_UBL_VALIDATE_CUSTOM_MESH        = _UxGT("Validate Custom Mesh");
  PROGMEM Language_Str MSG_G26_HEATING_BED                 = _UxGT("G26 Heating Bed");
  PROGMEM Language_Str MSG_G26_HEATING_NOZZLE              = _UxGT("G26 Heating Nozzle");
  PROGMEM Language_Str MSG_G26_MANUAL_PRIME                = _UxGT("Manual priming...");
  PROGMEM Language_Str MSG_G26_FIXED_LENGTH                = _UxGT("Fixed Length Prime");
  PROGMEM Language_Str MSG_G26_PRIME_DONE                  = _UxGT("Done Priming");
  PROGMEM Language_Str MSG_G26_CANCELED                    = _UxGT("G26 Canceled");
  PROGMEM Language_Str MSG_G26_LEAVING                     = _UxGT("Leaving G26");
  PROGMEM Language_Str MSG_UBL_CONTINUE_MESH               = _UxGT("Continue Bed Mesh");
  PROGMEM Language_Str MSG_UBL_MESH_LEVELING               = _UxGT("Mesh Leveling");
  PROGMEM Language_Str MSG_UBL_3POINT_MESH_LEVELING        = _UxGT("3-Point Leveling");
  PROGMEM Language_Str MSG_UBL_GRID_MESH_LEVELING          = _UxGT("Grid Mesh Leveling");
  PROGMEM Language_Str MSG_UBL_MESH_LEVEL                  = _UxGT("Level Mesh");
  PROGMEM Language_Str MSG_UBL_SIDE_POINTS                 = _UxGT("Side Points");
  PROGMEM Language_Str MSG_UBL_MAP_TYPE                    = _UxGT("Map Type");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP                  = _UxGT("Output Mesh Map");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_HOST             = _UxGT("Output for Host");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_CSV              = _UxGT("Output for CSV");
  PROGMEM Language_Str MSG_UBL_OUTPUT_MAP_BACKUP           = _UxGT("Off Imprimanta Backup");
  PROGMEM Language_Str MSG_UBL_INFO_UBL                    = _UxGT("Output UBL Info");
  PROGMEM Language_Str MSG_UBL_FILLIN_AMOUNT               = _UxGT("Fill-in Amount");
  PROGMEM Language_Str MSG_UBL_MANUAL_FILLIN               = _UxGT("Manual Fill-in");
  PROGMEM Language_Str MSG_UBL_SMART_FILLIN                = _UxGT("Smart Fill-in");
  PROGMEM Language_Str MSG_UBL_FILLIN_MESH                 = _UxGT("Fill-in Mesh");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_ALL              = _UxGT("Invalidate All");
  PROGMEM Language_Str MSG_UBL_INVALIDATE_CLOSEST          = _UxGT("Invalidate Closest");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_ALL               = _UxGT("Fine Tune All");
  PROGMEM Language_Str MSG_UBL_FINE_TUNE_CLOSEST           = _UxGT("Fine Tune Closest");
  PROGMEM Language_Str MSG_UBL_STORAGE_MESH_MENU           = _UxGT("Mesh Storage");
  PROGMEM Language_Str MSG_UBL_STORAGE_SLOT                = _UxGT("Memory Slot");
  PROGMEM Language_Str MSG_UBL_LOAD_MESH                   = _UxGT("Load Bed Mesh");
  PROGMEM Language_Str MSG_UBL_SAVE_MESH                   = _UxGT("Save Bed Mesh");
  PROGMEM Language_Str MSG_MESH_LOADED                     = _UxGT("Mesh %i Loaded");
  PROGMEM Language_Str MSG_MESH_SAVED                      = _UxGT("Mesh %i Saved");
  PROGMEM Language_Str MSG_UBL_NO_STORAGE                  = _UxGT("No Storage");
  PROGMEM Language_Str MSG_UBL_SAVE_ERROR                  = _UxGT("Err: UBL Save");
  PROGMEM Language_Str MSG_UBL_RESTORE_ERROR               = _UxGT("Err: UBL Restore");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET                    = _UxGT("Z-Offset: ");
  PROGMEM Language_Str MSG_UBL_Z_OFFSET_STOPPED            = _UxGT("Z-Offset Stopped");
  PROGMEM Language_Str MSG_UBL_STEP_BY_STEP_MENU           = _UxGT("Step-By-Step UBL");
  PROGMEM Language_Str MSG_UBL_1_BUILD_COLD_MESH           = _UxGT("1. Build Cold Mesh");
  PROGMEM Language_Str MSG_UBL_2_SMART_FILLIN              = _UxGT("2. Smart Fill-in");
  PROGMEM Language_Str MSG_UBL_3_VALIDATE_MESH_MENU        = _UxGT("3. Validate Mesh");
  PROGMEM Language_Str MSG_UBL_4_FINE_TUNE_ALL             = _UxGT("4. Fine Tune All");
  PROGMEM Language_Str MSG_UBL_5_VALIDATE_MESH_MENU        = _UxGT("5. Validate Mesh");
  PROGMEM Language_Str MSG_UBL_6_FINE_TUNE_ALL             = _UxGT("6. Fine Tune All");
  PROGMEM Language_Str MSG_UBL_7_SAVE_MESH                 = _UxGT("7. Save Bed Mesh");

  PROGMEM Language_Str MSG_LED_CONTROL                     = _UxGT("LED Control");
  PROGMEM Language_Str MSG_LEDS                            = _UxGT("Lights");
  PROGMEM Language_Str MSG_LED_PRESETS                     = _UxGT("Light Presets");
  PROGMEM Language_Str MSG_SET_LEDS_RED                    = _UxGT("Red");
  PROGMEM Language_Str MSG_SET_LEDS_ORANGE                 = _UxGT("Orange");
  PROGMEM Language_Str MSG_SET_LEDS_YELLOW                 = _UxGT("Yellow");
  PROGMEM Language_Str MSG_SET_LEDS_GREEN                  = _UxGT("Green");
  PROGMEM Language_Str MSG_SET_LEDS_BLUE                   = _UxGT("Blue");
  PROGMEM Language_Str MSG_SET_LEDS_INDIGO                 = _UxGT("Indigo");
  PROGMEM Language_Str MSG_SET_LEDS_VIOLET                 = _UxGT("Violet");
  PROGMEM Language_Str MSG_SET_LEDS_WHITE                  = _UxGT("White");
  PROGMEM Language_Str MSG_SET_LEDS_DEFAULT                = _UxGT("Default");
  PROGMEM Language_Str MSG_CUSTOM_LEDS                     = _UxGT("Custom Lights");
  PROGMEM Language_Str MSG_INTENSITY_R                     = _UxGT("Red Intensity");
  PROGMEM Language_Str MSG_INTENSITY_G                     = _UxGT("Green Intensity");
  PROGMEM Language_Str MSG_INTENSITY_B                     = _UxGT("Blue Intensity");
  PROGMEM Language_Str MSG_INTENSITY_W                     = _UxGT("White Intensity");
  PROGMEM Language_Str MSG_LED_BRIGHTNESS                  = _UxGT("Brightness");

  PROGMEM Language_Str MSG_MOVING                          = _UxGT("Moving...");
  PROGMEM Language_Str MSG_FREE_XY                         = _UxGT("Free XY");
  PROGMEM Language_Str MSG_MOVE_X                          = _UxGT("Move X");
  PROGMEM Language_Str MSG_MOVE_Y                          = _UxGT("Move Y");
  PROGMEM Language_Str MSG_MOVE_Z                          = _UxGT("Move Z");
  PROGMEM Language_Str MSG_MOVE_E                          = _UxGT("Extruder");
  PROGMEM Language_Str MSG_MOVE_EN                         = _UxGT("Extruder *");
  PROGMEM Language_Str MSG_HOTEND_TOO_COLD                 = _UxGT("Capat Prea Rece");
  PROGMEM Language_Str MSG_MOVE_N_MM                       = _UxGT("Move %smm");
  PROGMEM Language_Str MSG_MOVE_01MM                       = _UxGT("Move 0.1mm");
  PROGMEM Language_Str MSG_MOVE_1MM                        = _UxGT("Move 1mm");
  PROGMEM Language_Str MSG_MOVE_10MM                       = _UxGT("Move 10mm");
  PROGMEM Language_Str MSG_SPEED                           = _UxGT("Speed");
  PROGMEM Language_Str MSG_BED_Z                           = _UxGT("Bed Z");
  PROGMEM Language_Str MSG_NOZZLE                          = _UxGT("Nozzle");
  PROGMEM Language_Str MSG_NOZZLE_N                        = _UxGT("Nozzle ~");
  PROGMEM Language_Str MSG_NOZZLE_PARKED                   = _UxGT("Nozzle Parked");
  PROGMEM Language_Str MSG_NOZZLE_STANDBY                  = _UxGT("Nozzle Standby");
  PROGMEM Language_Str MSG_BED                             = _UxGT("Bed");
  PROGMEM Language_Str MSG_CHAMBER                         = _UxGT("Enclosure");
  PROGMEM Language_Str MSG_FAN_SPEED                       = _UxGT("Fan Speed");
  PROGMEM Language_Str MSG_FAN_SPEED_N                     = _UxGT("Fan Speed ~");
  PROGMEM Language_Str MSG_STORED_FAN_N                    = _UxGT("Stored Fan ~");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED                 = _UxGT("Extra Fan Speed");
  PROGMEM Language_Str MSG_EXTRA_FAN_SPEED_N               = _UxGT("Extra Fan Speed ~");
  PROGMEM Language_Str MSG_CONTROLLER_FAN                  = _UxGT("Controller Fan");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_IDLE_SPEED       = _UxGT("Idle Speed");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_AUTO_ON          = _UxGT("Auto Mode");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_SPEED            = _UxGT("Active Speed");
  PROGMEM Language_Str MSG_CONTROLLER_FAN_DURATION         = _UxGT("Idle Period");
  PROGMEM Language_Str MSG_FLOW                            = _UxGT("Flow");
  PROGMEM Language_Str MSG_FLOW_N                          = _UxGT("Flow ~");
  PROGMEM Language_Str MSG_CONTROL                         = _UxGT("Control");
  PROGMEM Language_Str MSG_MIN                             = " " LCD_STR_THERMOMETER _UxGT(" Min");
  PROGMEM Language_Str MSG_MAX                             = " " LCD_STR_THERMOMETER _UxGT(" Max");
  PROGMEM Language_Str MSG_FACTOR                          = " " LCD_STR_THERMOMETER _UxGT(" Fact");
  PROGMEM Language_Str MSG_AUTOTEMP                        = _UxGT("Autotemp");
  PROGMEM Language_Str MSG_LCD_ON                          = _UxGT("On");
  PROGMEM Language_Str MSG_LCD_OFF                         = _UxGT("Off");
  PROGMEM Language_Str MSG_PID_AUTOTUNE                    = _UxGT("PID Autotune");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_E                  = _UxGT("PID Autotune *");
  PROGMEM Language_Str MSG_PID_AUTOTUNE_DONE               = _UxGT("PID tuning done");
  PROGMEM Language_Str MSG_PID_BAD_EXTRUDER_NUM            = _UxGT("Autotune failed. Bad extruder.");
  PROGMEM Language_Str MSG_PID_TEMP_TOO_HIGH               = _UxGT("Autotune failed. Temperature too high.");
  PROGMEM Language_Str MSG_PID_TIMEOUT                     = _UxGT("Autotune failed! Timeout.");
  PROGMEM Language_Str MSG_SELECT                          = _UxGT("Select");
  PROGMEM Language_Str MSG_SELECT_E                        = _UxGT("Select *");
  PROGMEM Language_Str MSG_ACC                             = _UxGT("Accel");
  PROGMEM Language_Str MSG_JERK                            = _UxGT("Jerk");
  PROGMEM Language_Str MSG_VA_JERK                         = _UxGT("V") LCD_STR_A _UxGT("-Jerk");
  PROGMEM Language_Str MSG_VB_JERK                         = _UxGT("V") LCD_STR_B _UxGT("-Jerk");
  PROGMEM Language_Str MSG_VC_JERK                         = _UxGT("V") LCD_STR_C _UxGT("-Jerk");
  PROGMEM Language_Str MSG_VE_JERK                         = _UxGT("Ve-Jerk");
  PROGMEM Language_Str MSG_JUNCTION_DEVIATION              = _UxGT("Junction Dev");
  PROGMEM Language_Str MSG_VELOCITY                        = _UxGT("Velocity");
  PROGMEM Language_Str MSG_VMAX_A                          = _UxGT("Vmax ") LCD_STR_A;
  PROGMEM Language_Str MSG_VMAX_B                          = _UxGT("Vmax ") LCD_STR_B;
  PROGMEM Language_Str MSG_VMAX_C                          = _UxGT("Vmax ") LCD_STR_C;
  PROGMEM Language_Str MSG_VMAX_E                          = _UxGT("Vmax ") LCD_STR_E;
  PROGMEM Language_Str MSG_VMAX_EN                         = _UxGT("Vmax *");
  PROGMEM Language_Str MSG_VMIN                            = _UxGT("Vmin");
  PROGMEM Language_Str MSG_VTRAV_MIN                       = _UxGT("VTrav Min");
  PROGMEM Language_Str MSG_ACCELERATION                    = _UxGT("Acceleration");
  PROGMEM Language_Str MSG_AMAX_A                          = _UxGT("Amax ") LCD_STR_A;
  PROGMEM Language_Str MSG_AMAX_B                          = _UxGT("Amax ") LCD_STR_B;
  PROGMEM Language_Str MSG_AMAX_C                          = _UxGT("Amax ") LCD_STR_C;
  PROGMEM Language_Str MSG_AMAX_E                          = _UxGT("Amax ") LCD_STR_E;
  PROGMEM Language_Str MSG_AMAX_EN                         = _UxGT("Amax *");
  PROGMEM Language_Str MSG_A_RETRACT                       = _UxGT("A-Retract");
  PROGMEM Language_Str MSG_A_TRAVEL                        = _UxGT("A-Travel");
  PROGMEM Language_Str MSG_XY_FREQUENCY_LIMIT              = _UxGT("Frequency max");
  PROGMEM Language_Str MSG_XY_FREQUENCY_FEEDRATE           = _UxGT("Feed min");
  PROGMEM Language_Str MSG_STEPS_PER_MM                    = _UxGT("Steps/mm");
  PROGMEM Language_Str MSG_A_STEPS                         = LCD_STR_A _UxGT("steps/mm");
  PROGMEM Language_Str MSG_B_STEPS                         = LCD_STR_B _UxGT("steps/mm");
  PROGMEM Language_Str MSG_C_STEPS                         = LCD_STR_C _UxGT("steps/mm");
  PROGMEM Language_Str MSG_E_STEPS                         = _UxGT("Esteps/mm");
  PROGMEM Language_Str MSG_EN_STEPS                        = _UxGT("*steps/mm");
  PROGMEM Language_Str MSG_TEMPERATURE                     = _UxGT("Temperature");
  PROGMEM Language_Str MSG_MOTION                          = _UxGT("Motion");
  PROGMEM Language_Str MSG_FILAMENT                        = _UxGT("Filament");
  PROGMEM Language_Str MSG_VOLUMETRIC_ENABLED              = _UxGT("E in mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT                = _UxGT("E Limit in mm") SUPERSCRIPT_THREE;
  PROGMEM Language_Str MSG_VOLUMETRIC_LIMIT_E              = _UxGT("E Limit *");
  PROGMEM Language_Str MSG_FILAMENT_DIAM                   = _UxGT("Fil. Dia.");
  PROGMEM Language_Str MSG_FILAMENT_DIAM_E                 = _UxGT("Fil. Dia. *");
  PROGMEM Language_Str MSG_FILAMENT_UNLOAD                 = _UxGT("Unload mm");
  PROGMEM Language_Str MSG_FILAMENT_LOAD                   = _UxGT("Load mm");
  PROGMEM Language_Str MSG_ADVANCE_K                       = _UxGT("Advance K");
  PROGMEM Language_Str MSG_ADVANCE_K_E                     = _UxGT("Advance K *");
  PROGMEM Language_Str MSG_CONTRAST                        = _UxGT("LCD Contrast");
  PROGMEM Language_Str MSG_STORE_EEPROM                    = _UxGT("Store Settings");
  PROGMEM Language_Str MSG_LOAD_EEPROM                     = _UxGT("Load Settings");
  PROGMEM Language_Str MSG_RESTORE_DEFAULTS                = _UxGT("Restore Defaults");
  PROGMEM Language_Str MSG_INIT_EEPROM                     = _UxGT("Initialize EEPROM");
  PROGMEM Language_Str MSG_ERR_EEPROM_CRC                  = _UxGT("EEPROM CRC Error");
  PROGMEM Language_Str MSG_ERR_EEPROM_INDEX                = _UxGT("EEPROM Index Error");
  PROGMEM Language_Str MSG_ERR_EEPROM_VERSION              = _UxGT("EEPROM Version Error");
  PROGMEM Language_Str MSG_SETTINGS_STORED                 = _UxGT("Settings Stored");
  PROGMEM Language_Str MSG_MEDIA_UPDATE                    = _UxGT("Media Update");
  PROGMEM Language_Str MSG_RESET_PRINTER                   = _UxGT("Reset Imprimanta");
  PROGMEM Language_Str MSG_REFRESH                         = LCD_STR_REFRESH  _UxGT("Refresh");
  PROGMEM Language_Str MSG_INFO_SCREEN                     = _UxGT("Info Screen");
  PROGMEM Language_Str MSG_PREPARE                         = _UxGT("Prepare");
  PROGMEM Language_Str MSG_TUNE                            = _UxGT("Tune");
  PROGMEM Language_Str MSG_POWER_MONITOR                   = _UxGT("Power monitor");
  PROGMEM Language_Str MSG_CURRENT                         = _UxGT("Intensitate");
  PROGMEM Language_Str MSG_VOLTAGE                         = _UxGT("Voltaj");
  PROGMEM Language_Str MSG_POWER                           = _UxGT("Putere");
  PROGMEM Language_Str MSG_START_PRINT                     = _UxGT("Start Imprimare");
  PROGMEM Language_Str MSG_BUTTON_NEXT                     = _UxGT("Urmatorul");
  PROGMEM Language_Str MSG_BUTTON_INIT                     = _UxGT("Initiere");
  PROGMEM Language_Str MSG_BUTTON_STOP                     = _UxGT("Stop");
  PROGMEM Language_Str MSG_BUTTON_PRINT                    = _UxGT("Imprimeaza");
  PROGMEM Language_Str MSG_BUTTON_RESET                    = _UxGT("Reseteaza");
  PROGMEM Language_Str MSG_BUTTON_IGNORE                   = _UxGT("Ignora");
  PROGMEM Language_Str MSG_BUTTON_CANCEL                   = _UxGT("Anuleaza");
  PROGMEM Language_Str MSG_BUTTON_DONE                     = _UxGT("OK");
  PROGMEM Language_Str MSG_BUTTON_BACK                     = _UxGT("Inapoi");
  PROGMEM Language_Str MSG_BUTTON_PROCEED                  = _UxGT("Proceed");
  PROGMEM Language_Str MSG_PAUSING                         = _UxGT("Pausing...");
  PROGMEM Language_Str MSG_PAUSE_PRINT                     = _UxGT("Pause Print");
  PROGMEM Language_Str MSG_RESUME_PRINT                    = _UxGT("Resume Print");
  PROGMEM Language_Str MSG_STOP_PRINT                      = _UxGT("Stop Print");
  PROGMEM Language_Str MSG_PRINTING_OBJECT                 = _UxGT("Printing Object");
  PROGMEM Language_Str MSG_CANCEL_OBJECT                   = _UxGT("Cancel Object");
  PROGMEM Language_Str MSG_CANCEL_OBJECT_N                 = _UxGT("Cancel Object =");
  PROGMEM Language_Str MSG_OUTAGE_RECOVERY                 = _UxGT("Outage Recovery");
  PROGMEM Language_Str MSG_MEDIA_MENU                      = _UxGT("Print from Media");
  PROGMEM Language_Str MSG_NO_MEDIA                        = _UxGT("No Media");
  PROGMEM Language_Str MSG_DWELL                           = _UxGT("Sleep...");
  PROGMEM Language_Str MSG_USERWAIT                        = _UxGT("Click to Resume...");
  PROGMEM Language_Str MSG_PRINT_PAUSED                    = _UxGT("Print Paused");
  PROGMEM Language_Str MSG_PRINTING                        = _UxGT("Printing...");
  PROGMEM Language_Str MSG_PRINT_ABORTED                   = _UxGT("Print Aborted");
  PROGMEM Language_Str MSG_PRINT_DONE                      = _UxGT("Print Done");
  PROGMEM Language_Str MSG_NO_MOVE                         = _UxGT("No Move.");
  PROGMEM Language_Str MSG_KILLED                          = _UxGT("KILLED. ");
  PROGMEM Language_Str MSG_STOPPED                         = _UxGT("STOPPED. ");
  PROGMEM Language_Str MSG_CONTROL_RETRACT                 = _UxGT("Retract mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_SWAP            = _UxGT("Swap Re.mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACTF                = _UxGT("Retract V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_ZHOP            = _UxGT("Hop mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER         = _UxGT("Unretr. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAP    = _UxGT("S Unretr. mm");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVERF        = _UxGT("Unretract V");
  PROGMEM Language_Str MSG_CONTROL_RETRACT_RECOVER_SWAPF   = _UxGT("S UnRet V");
  PROGMEM Language_Str MSG_AUTORETRACT                     = _UxGT("Auto-Retract");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_LENGTH            = _UxGT("Swap Length");
  PROGMEM Language_Str MSG_FILAMENT_SWAP_EXTRA             = _UxGT("Swap Extra");
  PROGMEM Language_Str MSG_FILAMENT_PURGE_LENGTH           = _UxGT("Purge Length");
  PROGMEM Language_Str MSG_TOOL_CHANGE                     = _UxGT("Tool Inlocuire");
  PROGMEM Language_Str MSG_TOOL_CHANGE_ZLIFT               = _UxGT("Z Raise");
  PROGMEM Language_Str MSG_SINGLENOZZLE_PRIME_SPEED        = _UxGT("Prime Speed");
  PROGMEM Language_Str MSG_SINGLENOZZLE_RETRACT_SPEED      = _UxGT("Retract Speed");
  PROGMEM Language_Str MSG_FILAMENT_PARK_ENABLED           = _UxGT("Park Head");
  PROGMEM Language_Str MSG_SINGLENOZZLE_UNRETRACT_SPEED    = _UxGT("Recover Speed");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_SPEED          = _UxGT("Fan Speed");
  PROGMEM Language_Str MSG_SINGLENOZZLE_FAN_TIME           = _UxGT("Fan Time");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_ON               = _UxGT("Auto Pornit");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_OFF              = _UxGT("Auto Oprit");
  PROGMEM Language_Str MSG_TOOL_MIGRATION                  = _UxGT("Tool Migration");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_AUTO             = _UxGT("Auto-migrare");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_END              = _UxGT("Last Extruder");
  PROGMEM Language_Str MSG_TOOL_MIGRATION_SWAP             = _UxGT("Migrate to *");
  PROGMEM Language_Str MSG_FILAMENTCHANGE                  = _UxGT("Inlocuire Filament");
  PROGMEM Language_Str MSG_FILAMENTCHANGE_E                = _UxGT("Inlocuire Filament *");
  PROGMEM Language_Str MSG_FILAMENTLOAD                    = _UxGT("Incarcare Filament");
  PROGMEM Language_Str MSG_FILAMENTLOAD_E                  = _UxGT("Incarcare Filament *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD                  = _UxGT("Scoatere Filament");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_E                = _UxGT("Scoatere Filament *");
  PROGMEM Language_Str MSG_FILAMENTUNLOAD_ALL              = _UxGT("Scoate Tot");
  PROGMEM Language_Str MSG_ATTACH_MEDIA                    = _UxGT("Atasare Media");
  PROGMEM Language_Str MSG_CHANGE_MEDIA                    = _UxGT("Inlocuire Media");
  PROGMEM Language_Str MSG_RELEASE_MEDIA                   = _UxGT("Eliberare Media");
  PROGMEM Language_Str MSG_ZPROBE_OUT                      = _UxGT("Z Probe Past Bed");
  PROGMEM Language_Str MSG_SKEW_FACTOR                     = _UxGT("Skew Factor");
  PROGMEM Language_Str MSG_BLTOUCH                         = _UxGT("BLTouch");
  PROGMEM Language_Str MSG_BLTOUCH_SELFTEST                = _UxGT("Self-Test");
  PROGMEM Language_Str MSG_BLTOUCH_RESET                   = _UxGT("Reset");
  PROGMEM Language_Str MSG_BLTOUCH_STOW                    = _UxGT("Stow");
  PROGMEM Language_Str MSG_BLTOUCH_DEPLOY                  = _UxGT("Deploy");
  PROGMEM Language_Str MSG_BLTOUCH_SW_MODE                 = _UxGT("SW-Mode");
  PROGMEM Language_Str MSG_BLTOUCH_5V_MODE                 = _UxGT("5V-Mode");
  PROGMEM Language_Str MSG_BLTOUCH_OD_MODE                 = _UxGT("OD-Mode");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE              = _UxGT("Mode-Store");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_5V           = _UxGT("Set BLTouch to 5V");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_STORE_OD           = _UxGT("Set BLTouch to OD");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_ECHO               = _UxGT("Report Drain");
  PROGMEM Language_Str MSG_BLTOUCH_MODE_CHANGE             = _UxGT("DANGER: Bad settings can cause damage! Proceed anyway?");
  PROGMEM Language_Str MSG_TOUCHMI_PROBE                   = _UxGT("TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_INIT                    = _UxGT("Init TouchMI");
  PROGMEM Language_Str MSG_TOUCHMI_ZTEST                   = _UxGT("Z Offset Test");
  PROGMEM Language_Str MSG_TOUCHMI_SAVE                    = _UxGT("Save");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY_TOUCHMI           = _UxGT("Deploy TouchMI");
  PROGMEM Language_Str MSG_MANUAL_DEPLOY                   = _UxGT("Deploy Z-Probe");
  PROGMEM Language_Str MSG_MANUAL_STOW                     = _UxGT("Stow Z-Probe");
  PROGMEM Language_Str MSG_HOME_FIRST                      = _UxGT("Home %s%s%s First");
  PROGMEM Language_Str MSG_ZPROBE_OFFSETS                  = _UxGT("Probe Offsets");
  PROGMEM Language_Str MSG_ZPROBE_XOFFSET                  = _UxGT("Probe X Offset");
  PROGMEM Language_Str MSG_ZPROBE_YOFFSET                  = _UxGT("Probe Y Offset");
  PROGMEM Language_Str MSG_ZPROBE_ZOFFSET                  = _UxGT("Probe Z Offset");
  PROGMEM Language_Str MSG_BABYSTEP_X                      = _UxGT("Babystep X");
  PROGMEM Language_Str MSG_BABYSTEP_Y                      = _UxGT("Babystep Y");
  PROGMEM Language_Str MSG_BABYSTEP_Z                      = _UxGT("Babystep Z");
  PROGMEM Language_Str MSG_BABYSTEP_TOTAL                  = _UxGT("Total");
  PROGMEM Language_Str MSG_ENDSTOP_ABORT                   = _UxGT("Endstop Abort");
  PROGMEM Language_Str MSG_HEATING_FAILED_LCD              = _UxGT("Heating Failed");
  PROGMEM Language_Str MSG_ERR_REDUNDANT_TEMP              = _UxGT("Err: REDUNDANT TEMP");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY                 = _UxGT("THERMAL RUNAWAY");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_BED             = _UxGT("BED THERMAL RUNAWAY");
  PROGMEM Language_Str MSG_THERMAL_RUNAWAY_CHAMBER         = _UxGT("CHAMBER T. RUNAWAY");
  PROGMEM Language_Str MSG_ERR_MAXTEMP                     = _UxGT("Err: MAXTEMP");
  PROGMEM Language_Str MSG_ERR_MINTEMP                     = _UxGT("Err: MINTEMP");
  PROGMEM Language_Str MSG_HALTED                          = _UxGT("PRINTER HALTED");
  PROGMEM Language_Str MSG_PLEASE_RESET                    = _UxGT("Please Reset");
  PROGMEM Language_Str MSG_SHORT_DAY                       = _UxGT("d"); // One character only
  PROGMEM Language_Str MSG_SHORT_HOUR                      = _UxGT("h"); // One character only
  PROGMEM Language_Str MSG_SHORT_MINUTE                    = _UxGT("m"); // One character only
  PROGMEM Language_Str MSG_HEATING                         = _UxGT("Heating...");
  PROGMEM Language_Str MSG_COOLING                         = _UxGT("Cooling...");
  PROGMEM Language_Str MSG_BED_HEATING                     = _UxGT("Bed Heating...");
  PROGMEM Language_Str MSG_BED_COOLING                     = _UxGT("Bed Cooling...");
  PROGMEM Language_Str MSG_CHAMBER_HEATING                 = _UxGT("Chamber Heating...");
  PROGMEM Language_Str MSG_CHAMBER_COOLING                 = _UxGT("Chamber Cooling...");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE                 = _UxGT("Delta Calibration");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_X               = _UxGT("Calibrate X");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Y               = _UxGT("Calibrate Y");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_Z               = _UxGT("Calibrate Z");
  PROGMEM Language_Str MSG_DELTA_CALIBRATE_CENTER          = _UxGT("Calibrate Center");
  PROGMEM Language_Str MSG_DELTA_SETTINGS                  = _UxGT("Delta Settings");
  PROGMEM Language_Str MSG_DELTA_AUTO_CALIBRATE            = _UxGT("Auto Calibration");
  PROGMEM Language_Str MSG_DELTA_HEIGHT_CALIBRATE          = _UxGT("Set Delta Height");
  PROGMEM Language_Str MSG_DELTA_Z_OFFSET_CALIBRATE        = _UxGT("Probe Z-offset");
  PROGMEM Language_Str MSG_DELTA_DIAG_ROD                  = _UxGT("Diag Rod");
  PROGMEM Language_Str MSG_DELTA_HEIGHT                    = _UxGT("Inaltime");
  PROGMEM Language_Str MSG_DELTA_RADIUS                    = _UxGT("Radius");
  PROGMEM Language_Str MSG_INFO_MENU                       = _UxGT("Despre Imprimanta");
  PROGMEM Language_Str MSG_INFO_PRINTER_MENU               = _UxGT("Info Imprimanta");
  PROGMEM Language_Str MSG_3POINT_LEVELING                 = _UxGT("Nivelare in 3 puncte");
  PROGMEM Language_Str MSG_LINEAR_LEVELING                 = _UxGT("Nivelare Lineara");
  PROGMEM Language_Str MSG_BILINEAR_LEVELING               = _UxGT("Nivelare Bilineara");
  PROGMEM Language_Str MSG_UBL_LEVELING                    = _UxGT("Nivelarea Patului Unificata");
  PROGMEM Language_Str MSG_MESH_LEVELING                   = _UxGT("Nivelare Mesh");
  PROGMEM Language_Str MSG_INFO_STATS_MENU                 = _UxGT("Status Imprimanta");
  PROGMEM Language_Str MSG_INFO_BOARD_MENU                 = _UxGT("Informatii Placa");
  PROGMEM Language_Str MSG_INFO_THERMISTOR_MENU            = _UxGT("Termistoare");
  PROGMEM Language_Str MSG_INFO_EXTRUDERS                  = _UxGT("Extrudere");
  PROGMEM Language_Str MSG_INFO_BAUDRATE                   = _UxGT("Baud");
  PROGMEM Language_Str MSG_INFO_PROTOCOL                   = _UxGT("Protocol");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_OFF                = _UxGT("Runaway Watch: OFF");
  PROGMEM Language_Str MSG_INFO_RUNAWAY_ON                 = _UxGT("Runaway Watch: ON");
  PROGMEM Language_Str MSG_HOTEND_IDLE_TIMEOUT             = _UxGT("Hotend Idle Timeout");

  PROGMEM Language_Str MSG_CASE_LIGHT                      = _UxGT("Case Light");
  PROGMEM Language_Str MSG_CASE_LIGHT_BRIGHTNESS           = _UxGT("Light Brightness");
  PROGMEM Language_Str MSG_KILL_EXPECTED_PRINTER           = _UxGT("INCORRECT PRINTER");

#if LCD_WIDTH >= 20
  PROGMEM Language_Str MSG_INFO_PRINT_COUNT                = _UxGT("Total Printuri");
  PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS           = _UxGT("Completat");
  PROGMEM Language_Str MSG_INFO_PRINT_TIME                 = _UxGT("Timp Imprimare Total");
  PROGMEM Language_Str MSG_INFO_PRINT_LONGEST              = _UxGT("Longest Job Time");
  PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT             = _UxGT("Total Extrudat");
#else
  PROGMEM Language_Str MSG_INFO_PRINT_COUNT                = _UxGT("Prints");
  PROGMEM Language_Str MSG_INFO_COMPLETED_PRINTS           = _UxGT("Completed");
  PROGMEM Language_Str MSG_INFO_PRINT_TIME                 = _UxGT("Total");
  PROGMEM Language_Str MSG_INFO_PRINT_LONGEST              = _UxGT("Longest");
  PROGMEM Language_Str MSG_INFO_PRINT_FILAMENT             = _UxGT("Extruded");
#endif

  PROGMEM Language_Str MSG_INFO_MIN_TEMP                   = _UxGT("Temperatura Minima");
  PROGMEM Language_Str MSG_INFO_MAX_TEMP                   = _UxGT("Temperatura Maxima");
  PROGMEM Language_Str MSG_INFO_PSU                        = _UxGT("PSU");
  PROGMEM Language_Str MSG_DRIVE_STRENGTH                  = _UxGT("Drive Strength");
  PROGMEM Language_Str MSG_DAC_PERCENT_X                   = _UxGT("X Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Y                   = _UxGT("Y Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_Z                   = _UxGT("Z Driver %");
  PROGMEM Language_Str MSG_DAC_PERCENT_E                   = _UxGT("E Driver %");
  PROGMEM Language_Str MSG_ERROR_TMC                       = _UxGT("TMC CONNECTION ERROR");
  PROGMEM Language_Str MSG_DAC_EEPROM_WRITE                = _UxGT("DAC EEPROM Write");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER          = _UxGT("FILAMENT CHANGE");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_PAUSE    = _UxGT("PRINT PAUSED");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_LOAD     = _UxGT("LOAD FILAMENT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEADER_UNLOAD   = _UxGT("UNLOAD FILAMENT");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_HEADER   = _UxGT("RESUME OPTIONS:");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_PURGE    = _UxGT("Purge more");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_OPTION_RESUME   = _UxGT("Continue");
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_NOZZLE          = _UxGT("  Nozzle: ");
  PROGMEM Language_Str MSG_RUNOUT_SENSOR                   = _UxGT("Runout Sensor");
  PROGMEM Language_Str MSG_RUNOUT_DISTANCE_MM              = _UxGT("Runout Dist mm");
  PROGMEM Language_Str MSG_KILL_HOMING_FAILED              = _UxGT("Homing Failed");
  PROGMEM Language_Str MSG_LCD_PROBING_FAILED              = _UxGT("Probing Failed");

  PROGMEM Language_Str MSG_MMU2_CHOOSE_FILAMENT_HEADER     = _UxGT("CHOOSE FILAMENT");
  PROGMEM Language_Str MSG_MMU2_MENU                       = _UxGT("MMU");
  PROGMEM Language_Str MSG_KILL_MMU2_FIRMWARE              = _UxGT("Update MMU Firmware!");
  PROGMEM Language_Str MSG_MMU2_NOT_RESPONDING             = _UxGT("MMU Needs Attention.");
  PROGMEM Language_Str MSG_MMU2_RESUME                     = _UxGT("MMU Resume");
  PROGMEM Language_Str MSG_MMU2_RESUMING                   = _UxGT("MMU Resuming...");
  PROGMEM Language_Str MSG_MMU2_LOAD_FILAMENT              = _UxGT("MMU Load");
  PROGMEM Language_Str MSG_MMU2_LOAD_ALL                   = _UxGT("MMU Load All");
  PROGMEM Language_Str MSG_MMU2_LOAD_TO_NOZZLE             = _UxGT("MMU Load to Nozzle");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT             = _UxGT("MMU Eject");
  PROGMEM Language_Str MSG_MMU2_EJECT_FILAMENT_N           = _UxGT("MMU Eject ~");
  PROGMEM Language_Str MSG_MMU2_UNLOAD_FILAMENT            = _UxGT("MMU Unload");
  PROGMEM Language_Str MSG_MMU2_LOADING_FILAMENT           = _UxGT("Loading Fil. %i...");
  PROGMEM Language_Str MSG_MMU2_EJECTING_FILAMENT          = _UxGT("Ejecting Fil. ...");
  PROGMEM Language_Str MSG_MMU2_UNLOADING_FILAMENT         = _UxGT("Unloading Fil....");
  PROGMEM Language_Str MSG_MMU2_ALL                        = _UxGT("All");
  PROGMEM Language_Str MSG_MMU2_FILAMENT_N                 = _UxGT("Filament ~");
  PROGMEM Language_Str MSG_MMU2_RESET                      = _UxGT("Reset MMU");
  PROGMEM Language_Str MSG_MMU2_RESETTING                  = _UxGT("MMU Resetting...");
  PROGMEM Language_Str MSG_MMU2_EJECT_RECOVER              = _UxGT("Remove, click");

  PROGMEM Language_Str MSG_MIX                             = _UxGT("Mix");
  PROGMEM Language_Str MSG_MIX_COMPONENT_N                 = _UxGT("Component =");
  PROGMEM Language_Str MSG_MIXER                           = _UxGT("Mixer");
  PROGMEM Language_Str MSG_GRADIENT                        = _UxGT("Gradient");
  PROGMEM Language_Str MSG_FULL_GRADIENT                   = _UxGT("Full Gradient");
  PROGMEM Language_Str MSG_TOGGLE_MIX                      = _UxGT("Toggle Mix");
  PROGMEM Language_Str MSG_CYCLE_MIX                       = _UxGT("Cycle Mix");
  PROGMEM Language_Str MSG_GRADIENT_MIX                    = _UxGT("Gradient Mix");
  PROGMEM Language_Str MSG_REVERSE_GRADIENT                = _UxGT("Reverse Gradient");
  PROGMEM Language_Str MSG_ACTIVE_VTOOL                    = _UxGT("Active V-tool");
  PROGMEM Language_Str MSG_START_VTOOL                     = _UxGT("Start V-tool");
  PROGMEM Language_Str MSG_END_VTOOL                       = _UxGT("  End V-tool");
  PROGMEM Language_Str MSG_GRADIENT_ALIAS                  = _UxGT("Alias V-tool");
  PROGMEM Language_Str MSG_RESET_VTOOLS                    = _UxGT("Reset V-tools");
  PROGMEM Language_Str MSG_COMMIT_VTOOL                    = _UxGT("Commit V-tool Mix");
  PROGMEM Language_Str MSG_VTOOLS_RESET                    = _UxGT("V-tools Were Reset");
  PROGMEM Language_Str MSG_START_Z                         = _UxGT("Start Z:");
  PROGMEM Language_Str MSG_END_Z                           = _UxGT("  End Z:");

  PROGMEM Language_Str MSG_GAMES                           = _UxGT("Jocuri");
  PROGMEM Language_Str MSG_BRICKOUT                        = _UxGT("Brickout");
  PROGMEM Language_Str MSG_INVADERS                        = _UxGT("Invaders");
  PROGMEM Language_Str MSG_SNAKE                           = _UxGT("Sn4k3");
  PROGMEM Language_Str MSG_MAZE                            = _UxGT("Maze");

  PROGMEM Language_Str MSG_BAD_PAGE                        = _UxGT("Bad page index");
  PROGMEM Language_Str MSG_BAD_PAGE_SPEED                  = _UxGT("Bad page speed");

  //
  // Filament Inlocuire screens show up to 3 lines on a 4-line display
  //                        ...or up to 2 lines on a 3-line display
  //
#if LCD_HEIGHT >= 4
  PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING          = _UxGT(MSG_2_LINE("Apasa Butonul", "pentru a reveni la print"));
  PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING             = _UxGT(MSG_1_LINE("Parcare..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT            = _UxGT(MSG_3_LINE("Astept ca", "inlocuirea filamentului", "sa inceapa"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT          = _UxGT(MSG_3_LINE("Insert filament", "and press button", "to continue"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT            = _UxGT(MSG_2_LINE("Press button", "to heat nozzle"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING         = _UxGT(MSG_2_LINE("Nozzle heating", "Please wait..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD          = _UxGT(MSG_2_LINE("Wait for", "filament unload"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD            = _UxGT(MSG_2_LINE("Wait for", "filament load"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE           = _UxGT(MSG_2_LINE("Wait for", "filament purge"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE      = _UxGT(MSG_2_LINE("Click to finish", "filament purge"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME          = _UxGT(MSG_2_LINE("Wait for print", "to resume..."));
#else
  PROGMEM Language_Str MSG_ADVANCED_PAUSE_WAITING          = _UxGT(MSG_1_LINE("Click to continue"));
  PROGMEM Language_Str MSG_PAUSE_PRINT_PARKING             = _UxGT(MSG_1_LINE("Parcare..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INIT            = _UxGT(MSG_1_LINE("Va rog asteptati..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_INSERT          = _UxGT(MSG_1_LINE("Insert and Click"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEAT            = _UxGT(MSG_1_LINE("Click pentru incalzire"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_HEATING         = _UxGT(MSG_1_LINE("Incalzire..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_UNLOAD          = _UxGT(MSG_1_LINE("Ejectare..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_LOAD            = _UxGT(MSG_1_LINE("Incarcare..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_PURGE           = _UxGT(MSG_1_LINE("Curatare..."));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_CONT_PURGE      = _UxGT(MSG_1_LINE("Click pentru a termina"));
  PROGMEM Language_Str MSG_FILAMENT_CHANGE_RESUME          = _UxGT(MSG_1_LINE("Se Reia..."));
#endif
  PROGMEM Language_Str MSG_TMC_DRIVERS                     = _UxGT("TMC Drivers");
  PROGMEM Language_Str MSG_TMC_CURRENT                     = _UxGT("Driver Current");
  PROGMEM Language_Str MSG_TMC_HYBRID_THRS                 = _UxGT("Hybrid Threshold");
  PROGMEM Language_Str MSG_TMC_HOMING_THRS                 = _UxGT("Sensorless Homing");
  PROGMEM Language_Str MSG_TMC_STEPPING_MODE               = _UxGT("Stepping Mode");
  PROGMEM Language_Str MSG_TMC_STEALTH_ENABLED             = _UxGT("StealthChop Enabled");
  PROGMEM Language_Str MSG_SERVICE_RESET                   = _UxGT("Reset");
  PROGMEM Language_Str MSG_SERVICE_IN                      = _UxGT(" in:");
  PROGMEM Language_Str MSG_BACKLASH                        = _UxGT("Backlash");
  PROGMEM Language_Str MSG_BACKLASH_A                      = LCD_STR_A;
  PROGMEM Language_Str MSG_BACKLASH_B                      = LCD_STR_B;
  PROGMEM Language_Str MSG_BACKLASH_C                      = LCD_STR_C;
  PROGMEM Language_Str MSG_BACKLASH_CORRECTION             = _UxGT("Corectare");
  PROGMEM Language_Str MSG_BACKLASH_SMOOTHING              = _UxGT("Smoothing");

  PROGMEM Language_Str MSG_LEVEL_X_AXIS                    = _UxGT("Nivelare Axa X");
  PROGMEM Language_Str MSG_AUTO_CALIBRATE                  = _UxGT("Auto Calibrare");
  PROGMEM Language_Str MSG_HEATER_TIMEOUT                  = _UxGT("Timeout Incalzitor");
  PROGMEM Language_Str MSG_REHEAT                          = _UxGT("Reincalzire");
  PROGMEM Language_Str MSG_REHEATING                       = _UxGT("Reincalzire...");
}

#if FAN_COUNT == 1
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED
  #define MSG_EXTRA_FIRST_FAN_SPEED MSG_EXTRA_FAN_SPEED
#else
  #define MSG_FIRST_FAN_SPEED       MSG_FAN_SPEED_N
  #define MSG_EXTRA_FIRST_FAN_SPEED MSG_EXTRA_FAN_SPEED_N
#endif
