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

enum DGUSLCD_Screens : uint8_t {
  DGUSLCD_SCREEN_BOOT = 0,

  DGUSLCD_SCREEN_MAIN = 28,

  DGUSLCD_SCREEN_CONFIRM = 66,
  DGUSLCD_SCREEN_SDPRINTMANIPULATION = 37 ,
  DGUSLCD_SCREEN_SDPRINTTUNE = 41,
  DGUSLCD_SCREEN_SDFILELIST = 67,

  DGUSLCD_SCREEN_FILAMENTRUNOUT1 = 34, // DWINTouchPage::ERR_FILAMENTRUNOUT_HOTEND_COLD
  DGUSLCD_SCREEN_FILAMENTRUNOUT2 = 35, // DWINTouchPage::ERR_FILAMENTRUNOUT_FILAMENT_LOADED

  DGUSLCD_SCREEN_PRINT_FINISH = 36,    // DWINTouchPage::PRINT_FINISHED
  DGUSLCD_SCREEN_PRINT_RUNNING = 37,   // DWINTouchPage::PRINT_PROGRESS_RUNNING
  DGUSLCD_SCREEN_PRINT_PAUSED = 39,    // DWINTouchPage::PRINT_PROGRESS_PAUSED

  DGUSLCD_SCREEN_DIALOG_PAUSE = 38,    // DWINTouchPage::DIALOG_PAUSE_PRINTING
  DGUSLCD_SCREEN_DIALOG_STOP = 40,     // DWINTouchPage::DIALOG_STOP_PRINTING

  DGUSLCD_SCREEN_TUNING = 41,          // DWINTouchPage::MENU_TUNING
  DGUSLCD_SCREEN_TUNING_EXTRA = 79,    // (this is a new page)
  DGUSLCD_SCREEN_PREPARE = 42,         // DWINTouchPage::MENU_PREPARE

  DGUSLCD_SCREEN_MOVE10MM = 43,         // DWINTouchPage::MOVE_1MM
  DGUSLCD_SCREEN_MOVE1MM = 44,          // DWINTouchPage::MOVE_10MM
  DGUSLCD_SCREEN_MOVE01MM = 45,         // DWINTouchPage::MOVE_01MM

  DGUSLCD_SCREEN_FEED = 46,            // DWINTouchPage::FEED
  DGUSLCD_SCREEN_SETUP = 47,         // DWINTouchPage::MENU_CONTROL

  DGUSLCD_SCREEN_TEMP = 48,            // DWINTouchPage::MENU_TEMP
  DGUSLCD_SCREEN_TEMP_PLA = 49,        // DWINTouchPage::MENU_PLA_TEMP
  DGUSLCD_SCREEN_TEMP_ABS = 50,        // DWINTouchPage::MENU_ABS_TEMP

  DGUSLCD_SCREEN_INFO = 51,            // DWINTouchPage::MENU_ABOUT

  DGUSLCD_SCREEN_ZOFFSET_LEVEL = 52,   // DWINTouchPage::MENU_ZOFFSET_LEVELING
  DGUSLCD_SCREEN_LEVELING = 53,        // DWINTouchPage::LEVELING

  DGUSLCD_SCREEN_POWER_LOSS = 54,       // DWINTouchPage::DIALOG_POWER_FAILURE
  DGUSLCD_SCREEN_THERMAL_RUNAWAY = 57,  // DWINTouchPage::ERR_THERMAL_RUNAWAY
  DGUSLCD_SCREEN_HEATING_FAILED = 58,   // DWINTouchPage::ERR_HEATING_FAILED
  DGUSLCD_SCREEN_THERMISTOR_ERROR = 59, // DWINTouchPage::ERR_THERMISTOR

  DGUSLCD_SCREEN_AUTOHOME = 61,         // DWINTouchPage::AUTOHOME_IN_PROGRESS

  DGUSLCD_SCREEN_POPUP = 63,           // NEW - does not exist in original display
  DGUSLCD_SCREEN_KILL = 64,            // NEW - does not exist in original display

  DGUSLCD_SCREEN_PIDTUNE_CALIBRATION = 68,
  DGUSLCD_SCREEN_ESTEPS_CALIBRATION = 69,

  DGUSLCD_SCREEN_TUNEFWRETRACT = 70,

  DGUSLCD_SCREEN_ESTEPS_CALIBRATION_RESULTS = 71,
  DGUSLCD_SCREEN_LEVELING_SETTINGS = 72,

  DGUSLCD_SCREEN_AXIS_SETTINGS_NAV = 73,
  DGUSLCD_SCREEN_AXIS_SETTINGS_AXIS = 74,
  DGUSLCD_SCREEN_AXIS_SETTINGS_TMC = 75,

  DGUSLCD_SCREEN_ADV_MOV_SETTINGS = 76,
  DGUSLCD_SCREEN_MISC_SETTINGS = 77,
  DGUSLCD_SCREEN_MESH_VALIDATION = 78,

  DGUSLCD_SCREEN_CALIBRATE = 80,
  DGUSLCD_SCREEN_RGB = 81
};

// Version checks
constexpr uint16_t VP_UI_VERSION_MAJOR = 0xFFFA;
constexpr uint16_t EXPECTED_UI_VERSION_MAJOR = 61;
constexpr uint16_t VERSION_MISMATCH_BUZZ_AMOUNT = 5;
constexpr uint16_t VERSION_MISMATCH_LED_FLASH_DELAY = 1000;

#define VP_STARTPROGRESSBAR           0x1000

// // Storage space for the Killscreen messages. Reused for the popup.
constexpr uint16_t VP_MSGSTR1 = 0x2010;
constexpr uint8_t VP_MSGSTR1_LEN = 0x20;  // might be more place for it...
constexpr uint16_t VP_MSGSTR2 = 0x2030;
constexpr uint8_t VP_MSGSTR2_LEN = 0x40;
constexpr uint16_t VP_MSGSTR3 = 0x2070;
constexpr uint8_t VP_MSGSTR3_LEN = 0x40;
constexpr uint16_t VP_MSGSTR4 = 0x2080;
constexpr uint8_t VP_MSGSTR4_LEN = 0x20;

// // Screenchange request for screens that only make sense when printer is idle.
// // e.g movement is only allowed if printer is not printing.
// // Marlin must confirm by setting the screen manually.
// constexpr uint16_t VP_SCREENCHANGE_ASK = 0x2000;
constexpr uint16_t VP_SCREENCHANGE = 0x219f;   // Key-Return button to new menu pressed. Data contains target screen in low byte and info in high byte.
// constexpr uint16_t VP_TEMP_ALL_OFF = 0x2002;   // Turn all heaters off. Value arbitrary ;)=
// constexpr uint16_t VP_SCREENCHANGE_WHENSD = 0x2003; // "Print" Button touched -- go only there if there is an SD Card.

constexpr uint16_t VP_CONFIRMED = 0x219E; // OK on confirm screen.

// // Buttons on the SD-Card File listing.
// constexpr uint16_t VP_SD_ScrollEvent = 0x2020; // Data: 0 for "up a directory", numbers are the amount to scroll, e.g -1 one up, 1 one down
// constexpr uint16_t VP_SD_FileSelected = 0x2022; // Number of file field selected.
constexpr uint16_t VP_SD_FileSelectConfirm = 0x2024; // (This is a virtual VP and emulated by the Confirm Screen when a file has been confirmed)

// constexpr uint16_t VP_SD_ResumePauseAbort = 0x2026; // Resume(Data=0), Pause(Data=1), Abort(Data=2) SD Card prints
constexpr uint16_t VP_SD_AbortPrintConfirmed = 0x2028; // Abort print confirmation (virtual, will be injected by the confirm dialog)
// constexpr uint16_t VP_SD_Print_Setting = 0x2040;
// constexpr uint16_t VP_SD_Print_LiveAdjustZ = 0x2050; // Data: 0 down, 1 up

// // Controls for movement (we can't use the incremental / decremental feature of the display at this feature works only with 16 bit values
// // (which would limit us to 655.35mm, which is likely not a problem for common setups, but i don't want to rule out hangprinters support)
// // A word about the coding: The VP will be per axis and the return code will be an signed 16 bit value in 0.01 mm resolution, telling us
// // the relative travel amount t he user wants to do. So eg. if the display sends us VP=2100 with value 100, the user wants us to move X by +1 mm.
constexpr uint16_t VP_MOVE_X = 0x2100;
constexpr uint16_t VP_MOVE_Y = 0x2102;
constexpr uint16_t VP_MOVE_Z = 0x2104;
constexpr uint16_t VP_MOVE_E0 = 0x2110;
// constexpr uint16_t VP_MOVE_E1 = 0x2112;
// //constexpr uint16_t VP_MOVE_E2 = 0x2114;
// //constexpr uint16_t VP_MOVE_E3 = 0x2116;
// //constexpr uint16_t VP_MOVE_E4 = 0x2118;
// //constexpr uint16_t VP_MOVE_E5 = 0x211A;
constexpr uint16_t VP_HOME_ALL = 0x2120;
// constexpr uint16_t VP_MOTOR_LOCK_UNLOK = 0x2130;

// // Power loss recovery
// constexpr uint16_t VP_POWER_LOSS_RECOVERY = 0x2180;

// // Fan Control Buttons , switch between "off" and "on"
// constexpr uint16_t VP_FAN0_CONTROL = 0x2200;
// constexpr uint16_t VP_FAN1_CONTROL = 0x2202;
// //constexpr uint16_t VP_FAN2_CONTROL = 0x2204;
// //constexpr uint16_t VP_FAN3_CONTROL = 0x2206;

// // Heater Control Buttons , triged between "cool down" and "heat PLA" state
constexpr uint16_t VP_E0_CONTROL = 0x2210;
// constexpr uint16_t VP_E1_CONTROL = 0x2212;
// //constexpr uint16_t VP_E2_CONTROL = 0x2214;
// //constexpr uint16_t VP_E3_CONTROL = 0x2216;
// //constexpr uint16_t VP_E4_CONTROL = 0x2218;
// //constexpr uint16_t VP_E5_CONTROL = 0x221A;
constexpr uint16_t VP_BED_CONTROL = 0x221C;

// // Preheat
// constexpr uint16_t VP_E0_BED_PREHEAT = 0x2220;
// constexpr uint16_t VP_E1_BED_CONTROL = 0x2222;
// //constexpr uint16_t VP_E2_BED_CONTROL = 0x2224;
// //constexpr uint16_t VP_E3_BED_CONTROL = 0x2226;
// //constexpr uint16_t VP_E4_BED_CONTROL = 0x2228;
// //constexpr uint16_t VP_E5_BED_CONTROL = 0x222A;

// // Filament load and unload
// constexpr uint16_t VP_E0_FILAMENT_LOAD_UNLOAD = 0x2300;
// constexpr uint16_t VP_E1_FILAMENT_LOAD_UNLOAD = 0x2302;

// // Settings store , reset
// constexpr uint16_t VP_SETTINGS = 0x2400;

// // PID autotune
constexpr uint16_t VP_PID_AUTOTUNE_E0 = 0x2410;
// //constexpr uint16_t VP_PID_AUTOTUNE_E1 = 0x2412;
// //constexpr uint16_t VP_PID_AUTOTUNE_E2 = 0x2414;
// //constexpr uint16_t VP_PID_AUTOTUNE_E3 = 0x2416;
// //constexpr uint16_t VP_PID_AUTOTUNE_E4 = 0x2418;
// //constexpr uint16_t VP_PID_AUTOTUNE_E5 = 0x241A;
constexpr uint16_t VP_PID_AUTOTUNE_BED = 0x2420;

// // Firmware version on the boot screen.
constexpr uint16_t VP_PRINTER_BEDSIZE = 0x1074;
constexpr uint16_t VP_PRINTER_BEDSIZE_LEN = 12;
constexpr uint16_t VP_MARLIN_VERSION = 0x2222;
constexpr uint8_t VP_MARLIN_VERSION_LEN = 32;

constexpr uint16_t VP_MARLIN_WEBSITE = 0x2242;
constexpr uint8_t VP_MARLIN_WEBSITE_LEN = 32;  

constexpr uint16_t VP_STANDBY_BACKLIGHT_ICON = 0x2280;
constexpr uint16_t VP_STANDBY_BACKLIGHT_TOGGLE = 0x2282;

constexpr uint16_t VP_MUTE_ICON = 0x2284;
constexpr uint16_t VP_MUTE_TOGGLE = 0x2286;

constexpr uint16_t VP_SCREEN_BACKLIGHT_STANDBY = 0x228D;
constexpr uint16_t VP_SCREEN_BACKLIGHT = 0x2384;

constexpr uint16_t VP_SCREEN_STANDBY_TIME = 0x2386;

// Material preheat settings
constexpr uint16_t VP_PREHEAT_PLA_HOTEND_TEMP = 0x1102;
constexpr uint16_t VP_PREHEAT_PLA_BED_TEMP = 0x1104;

constexpr uint16_t VP_PREHEAT_ABS_HOTEND_TEMP = 0x1108;
constexpr uint16_t VP_PREHEAT_ABS_BED_TEMP = 0x110a;

// Place for status messages.

// ... We have memory space for scrolling messages
constexpr uint16_t VP_M117 = 0x3000 + (3 * 1);  // Text Variable Pointer. First three VP must be reserved [a VP is two bytes red.]. Text is saved after the 3rd VP and ended with 0x00 or 0x0F. 
constexpr uint8_t VP_M117_LEN = 100;

// ... And memory space for static (short) messages. Note this VPAddr is also the VP of the 5 beta and alpha 4 touch screens. 
constexpr uint8_t M117_STATIC_DISPLAY_LEN = 28; // Fits "TFT flashed incorrectly v0" exactly
constexpr uint16_t VP_M117_STATIC = 0x21B3;
constexpr uint8_t VP_M117_STATIC_LEN = 70;

// // Temperatures.
constexpr uint16_t VP_T_E0_Is = 0x1036;  // 4 Byte Integer - HEAD_CURRENT_TEMP_VP
constexpr uint16_t VP_T_E0_Set = 0x1034; // 2 Byte Integer - HEAD_SET_TEMP_VP

constexpr uint16_t VP_T_Bed_Is = 0x103c;  // 4 Byte Integer - BED_SET_TEMP_VP
constexpr uint16_t VP_T_Bed_Set = 0x103A; // 2 Byte Integer - BED_CURRENT_TEMP_VP

constexpr uint16_t VP_Flowrate_E0 = 0x228A; // 2 Byte Integer

constexpr uint16_t VP_Fan0_Percentage = 0x228F;  // 2 Byte Integer (0..100)

constexpr uint16_t VP_Feedrate_Percentage = 0x1006; // 2 Byte Integer (0..100) - PRINT_SPEED_RATE_VP
constexpr uint16_t VP_PrintProgress_Percentage = 0x1016; // 2 Byte Integer (0..100)

constexpr uint16_t VP_PrintTimeProgressBar = 0x100E;

constexpr uint16_t VP_PrintTime = 0x21a0;
constexpr uint16_t VP_PrintTimeWithRemainingVisible = 0x2335;
constexpr uint16_t VP_PrintTime_LEN = 19;

constexpr uint16_t VP_PrintTimeRemaining = 0x231f;
constexpr uint16_t VP_PrintTimeRemaining_LEN = 21;

constexpr uint16_t VP_HideRemainingTime_Ico = 0x2380;
constexpr uint16_t ICON_REMAINING_VISIBLE = 26;
constexpr uint16_t ICON_REMAINING_HIDDEN = 27;

constexpr uint16_t VP_Z_OFFSET = 0x1026;

// // SDCard File Listing
constexpr uint16_t VP_SD_ScrollEvent = 0x20D4; // Data: 0 for "up a directory", numbers are the amount to scroll, e.g -1 one up, 1 one down
constexpr uint16_t VP_SD_FileSelected = 0x2200; // Number of file field selected.
constexpr uint16_t VP_SD_FileName_LEN = 21; // LEN is shared for all entries.
constexpr uint16_t VP_SD_FileName_CNT = 5; // LEN is shared for all entries.
constexpr uint16_t DGUS_SD_FILESPERSCREEN = VP_SD_FileName_CNT; // FIXME move that info to the display and read it from there.
constexpr uint16_t VP_SD_FileName0 = 0x20D5;
constexpr uint16_t VP_SD_FileName1 = VP_SD_FileName0 + VP_SD_FileName_LEN;
constexpr uint16_t VP_SD_FileName2 = VP_SD_FileName1 + VP_SD_FileName_LEN;
constexpr uint16_t VP_SD_FileName3 = VP_SD_FileName2 + VP_SD_FileName_LEN;
constexpr uint16_t VP_SD_FileName4 = VP_SD_FileName3 + VP_SD_FileName_LEN;
constexpr uint16_t VP_SD_FileName5 = VP_SD_FileName4 + VP_SD_FileName_LEN;

constexpr uint16_t VP_SD_Print_ProbeOffsetZ = 0x32A0; //
constexpr uint16_t VP_SD_Print_Filename = 0x2000; //

constexpr uint16_t VP_ICON_OVERLAY_CLEAR = 10;
constexpr uint16_t VP_ICON_OVERLAY_SELECTED = 6;

// // Step per mm
constexpr uint16_t VP_X_STEP_PER_MM = 0x3600;
constexpr uint16_t VP_Y_STEP_PER_MM = 0x3604;
constexpr uint16_t VP_Z_STEP_PER_MM = 0x3608;
constexpr uint16_t VP_E0_STEP_PER_MM = 0x3610;
// // PIDs
constexpr uint16_t VP_E0_PID_P = 0x3700;
constexpr uint16_t VP_E0_PID_I = 0x3702;
constexpr uint16_t VP_E0_PID_D = 0x3704;
constexpr uint16_t VP_BED_PID_P = 0x3710;
constexpr uint16_t VP_BED_PID_I = 0x3712;
constexpr uint16_t VP_BED_PID_D = 0x3714;

// Power loss recovery
constexpr uint16_t VP_POWER_LOSS_RECOVERY = 0x105F;

// Buttons defined by Creality - Don't worry if you're confused by the naming, so am I
constexpr uint16_t VP_BUTTON_MAINENTERKEY = 0x1002;
constexpr uint16_t VP_BUTTON_ADJUSTENTERKEY = 0x1004;
constexpr uint16_t VP_BUTTON_PAUSEPRINTKEY = 0x100A;
constexpr uint16_t VP_BUTTON_TEMPCONTROL = 0x1030;
constexpr uint16_t VP_BUTTON_COOLDOWN = 0x1032;
constexpr uint16_t VP_BUTTON_PREPAREENTERKEY = 0x103E;

constexpr uint16_t VP_BUTTON_SELECTFILEKEY = 0x20D3;
constexpr uint16_t VP_BUTTON_STARTPRINTKEY = 0x20D2;
constexpr uint16_t VP_BUTTON_STOPPRINTKEY = 0x1008;
constexpr uint16_t VP_BUTTON_RESUMEPRINTKEY = 0x100C;
constexpr uint16_t VP_BUTTON_BEDLEVELKEY = 0x1044;

constexpr uint16_t VP_BUTTON_HEATLOADSTARTKEY = 0x1056;

// Additional stuff defined by Creality
constexpr uint16_t VP_FAN_TOGGLE = 0x101E;
constexpr uint16_t VP_LED_TOGGLE = 0x101F;

// Axis settings
constexpr uint16_t VP_AXIS_SETTINGS_NAV_BUTTON = 0x22D9;
constexpr uint16_t AXIS_SETTINGS_NAV_BUTTON_VAL_X = 1;
constexpr uint16_t AXIS_SETTINGS_NAV_BUTTON_VAL_Y = 2;
constexpr uint16_t AXIS_SETTINGS_NAV_BUTTON_VAL_Z = 3;
constexpr uint16_t AXIS_SETTINGS_NAV_BUTTON_VAL_E = 4;

constexpr uint16_t VP_AXIS_SETTINGS_TITLE_ICON = 0x22DB;
constexpr uint16_t ICON_AXIS_SETTINGS_TITLE_X = 20;
constexpr uint16_t ICON_AXIS_SETTINGS_TITLE_Y = 21;
constexpr uint16_t ICON_AXIS_SETTINGS_TITLE_Z = 22;
constexpr uint16_t ICON_AXIS_SETTINGS_TITLE_E = 23;

constexpr uint16_t VP_AXIS_SETTINGS_NAV_BACKBUTTON = 0x22DD;

constexpr uint16_t VP_AXIS_SETTINGS_AXIS_STEPSMM = 0x22DF; // 2-byte
constexpr uint16_t VP_AXIS_SETTINGS_AXIS_MAX_ACCEL = 0x22E1; // 4-byte (!)
constexpr uint16_t VP_AXIS_SETTINGS_AXIS_JERK = 0x22E5; // 2-byte
constexpr uint16_t VP_AXIS_SETTINGS_AXIS_FEEDRATE = 0x22E7; // 2-byte

constexpr uint16_t VP_AXIS_SETTINGS_AXIS_TMCCURRENT = 0x22E9; // 2-byte

constexpr uint16_t VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_BUTTON = 0x22EB; // 2-byte
constexpr uint16_t VP_AXIS_SETTINGS_AXIS_TMCSTEALTHCHOP_ICON = 0x22ED; // 2-byte
constexpr uint16_t VP_AXIS_SETTINGS_AXIS_TMCHYBRIDTHRESHOLD = 0x22EF; // 4-byte (!)

constexpr uint16_t VP_AXIS_TUNING_NAV_BUTTON = 0x22F5; 
constexpr uint16_t VP_AXIS_TMC_NAV_BUTTON = 0x22F7; 

constexpr uint16_t VP_AXIS_TMC_NAV_ICON = 0x22F3; // 2-byte
constexpr uint16_t AXIS_TMC_NAV_ICON_SHOWING = 10;
constexpr uint16_t AXIS_TMC_NAV_ICON_HIDING = 11;

// ... Advanced movement settings
constexpr uint16_t VP_MOV_NAV_BUTTON = 0x2305; 

constexpr uint16_t VP_MOV_MINIMUM_SEGMENT_TIME = 0x22F9; // uint 2-byte
constexpr uint16_t VP_MOV_MINIMUM_FEEDRATE = 0x22FB; // float 2-byte
constexpr uint16_t VP_MOV_NORMAL_ACCELERATION = 0x22FD; // float 2-byte

constexpr uint16_t VP_MOV_MINIMUM_TRAVEL_FEEDRATE = 0x2301; // float 2-byte
constexpr uint16_t VP_MOV_MINIMUM_TRAVEL_ACCELERATION = 0x2303; // float 2-byte

constexpr uint16_t VP_MOV_RETRACT_ACCELERATION = 0x2307; // float 4-byte

// Misc settings
constexpr uint16_t VP_MISCSETTINGS_NAV_BUTTON = 0x2311; 

constexpr uint16_t VP_FILAMENTRUNOUT_SENSOR_TOGGLE_BUTTON = 0x2309;
constexpr uint16_t VP_FILAMENTRUNOUT_SENSOR_TOGGLE_ICON = 0x230b;

constexpr uint16_t VP_PLR_TOGGLE_BUTTON = 0x230d;
constexpr uint16_t VP_PLR_TOGGLE_ICON = 0x230F;

// Mesh leveling
constexpr uint16_t VP_LEVELING_NAV_BUTTON = 0x238a;
constexpr uint16_t VP_LEVELING_EDIT_NAV_BUTTON = 0x23A8;

constexpr uint16_t VP_MESH_SCREEN_MESSAGE_ICON = 0x22cb;
constexpr uint16_t MESH_SCREEN_MESSAGE_ICON_LEVELING = 5;
constexpr uint16_t MESH_SCREEN_MESSAGE_ICON_VIEWING = 6;

constexpr uint16_t VP_MESH_LEVEL_TEMP = 0x108A;
constexpr uint16_t VP_MESH_LEVEL_STATUS = 0x108D;

constexpr uint8_t DGUS_GRID_VISUALIZATION_START_ID = GRID_MAX_POINTS > (4*4) ? 30 : 1;

static_assert(
  (GRID_MAX_POINTS == 16 && DGUS_GRID_VISUALIZATION_START_ID == 1)||  // CR-6 SE
  (GRID_MAX_POINTS == 49 && DGUS_GRID_VISUALIZATION_START_ID == 30) || // CR-6 MAX
  (GRID_MAX_POINTS != 16 && GRID_MAX_POINTS != 49),                    // Custom Leveling
  "Incorrect offset selected for leveling config"
);
static_assert(GRID_MAX_POINTS_X == GRID_MAX_POINTS_Y, "Assuming bed leveling points is square");


constexpr uint16_t VP_MESH_LEVEL_X0_Y0 = 0x1350;
constexpr uint16_t SP_MESH_LEVEL_X0_Y0 = 0x5000;

constexpr uint16_t MESH_LEVEL_EDGE_MAX_POINTS = 4;

constexpr uint16_t MESH_LEVEL_VP_SIZE = 0x4; // 4-byte native float
constexpr uint16_t MESH_LEVEL_SP_SIZE = 0x10; // 0x10 distance

constexpr uint16_t MESH_LEVEL_VP_EDGE_SIZE = MESH_LEVEL_VP_SIZE * MESH_LEVEL_EDGE_MAX_POINTS;
constexpr uint16_t MESH_LEVEL_SP_EDGE_SIZE = MESH_LEVEL_SP_SIZE * MESH_LEVEL_EDGE_MAX_POINTS;

constexpr uint16_t MESH_LEVEL_MAX_POINTS = MESH_LEVEL_EDGE_MAX_POINTS * MESH_LEVEL_EDGE_MAX_POINTS;

// Mesh inputs
constexpr uint16_t VP_MESH_INPUT_X0_Y0 = 0x2360;

#define MESH_INPUT_SUPPORTED_X_SIZE 4
#define MESH_INPUT_SUPPORTED_Y_SIZE 4
#define MESH_INPUT_SUPPORTED_SIZE 16
#define MESH_INPUT_DATA_SIZE 2 // 2 byte integer

// Color table: https://stackoverflow.com/q/13720937/646215
// Color picker: https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
// Colors below were picked on bed leveling visualizer defaults

constexpr uint16_t MESH_COLOR_BELOW_ZERO = 0x899B;   // Blue
constexpr uint16_t MESH_COLOR_NEAR_ZERO = 0x1C80;    // Green
constexpr uint16_t MESH_COLOR_ABOVE_ZERO = 0xB800;    // Purple
constexpr uint16_t MESH_COLOR_NOT_MEASURED = 0xFFFF; // White (becomes invisible on white background)

constexpr float MESH_NEAR_ZERO = 0.020;
constexpr float MESH_UNSET_EPSILON = 0.001;

// Mesh validation pattern
constexpr uint16_t VP_MESHPATTERN_NOZZLE_TEMP = 0x2313;
constexpr uint16_t VP_MESHPATTERN_BED_TEMP = 0x2315;

constexpr uint16_t VP_MESHPATTERN_START_BUTTON = 0x2317;
constexpr uint16_t VP_MESHPATTERN_BUTTON_ICON = 0x2319;

constexpr uint16_t VP_MESHPATTERN_NAV_BUTTON = 0x231B;

constexpr uint16_t MESHPATTERN_BUTTON_START = 24; // This icon does not exist, and will therefore not show
constexpr uint16_t MESHPATTERN_BUTTON_CANCEL = 25;


// Movement screen
constexpr uint16_t VP_X_POSITION = 0x1048;
constexpr uint16_t SP_X_POSITION = 0x4000;
constexpr uint16_t VP_Y_POSITION = 0x104A;
constexpr uint16_t SP_Y_POSITION = 0x4030;
constexpr uint16_t VP_Z_POSITION = 0x104C;
constexpr uint16_t VP_Z_POSITION_PRECISION = 0x108F;
constexpr uint16_t SP_Z_POSITION = 0x4060;
constexpr uint16_t VP_BUTTON_MOVEKEY = 0x1046;

// Buttons
constexpr uint16_t VP_ESTEP_NAV_BUTTON = 0x2291;
constexpr uint16_t VP_PIDTUNE_NAV_BUTTON = 0x2293;
constexpr uint16_t VP_GENERIC_BACK_BUTTON = 0x2295; // Generic button for popping back to the old display

constexpr uint16_t GENERIC_BACK_BUTTON_NEED_SAVE = 0x1;

// PID tuning
constexpr uint16_t VP_PIDTUNE_TARGET_TEMP = 0x2297;
constexpr uint16_t VP_PIDTUNE_CYCLES = 0x2299;
constexpr uint16_t VP_PIDTUNE_FAN_TOGGLE = 0x238C;
constexpr uint16_t VP_PIDTUNE_FAN_TOGGLE_ICON = 0x238E;
constexpr uint16_t VP_PIDTUNE_START_BUTTON = 0x229B;

// FWRetract
constexpr uint16_t VP_FWRETRACT_NAV_BUTTON = 0x22AD;

constexpr uint16_t VP_FWRETRACT_RETRACT_LENGTH = 0x22B1;
constexpr uint16_t VP_FWRETRACT_RETRACT_FEEDRATE = 0x22B3;
constexpr uint16_t VP_FWRETRACT_RETRACT_ZHOP = 0x22B5;

constexpr uint16_t VP_FWRETRACT_RESTART_LENGTH = 0x22B7;
constexpr uint16_t VP_FWRETRACT_RESTART_FEEDRATE = 0x22B9;

constexpr uint16_t VP_FWRETRACT_TOGGLE_BUTTON = 0x22BB;
constexpr uint16_t VP_FWRETRACT_TOGGLE_BUTTON_ICON = 0x22BD;
constexpr uint16_t VP_FWRETRACT_INDICATOR_ICON = 0x22BF;

// Other tuning
constexpr uint16_t VP_LINEAR_ADVANCE_FACTOR = 0x22AF;

constexpr uint16_t VP_OTHER_TUNE_NAV_BUTTON = 0x2382;

// Leveling settings
constexpr uint16_t VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_BUTTON = 0x22C1;
constexpr uint16_t VP_TOGGLE_PROBING_HEATERS_OFF_ONOFF_ICON = 0x22C3;

constexpr uint16_t VP_TOGGLE_PROBE_PREHEAT_HOTEND_TEMP = 0x22C5;
constexpr uint16_t VP_TOGGLE_PROBE_PREHEAT_BED_TEMP = 0x22C7;

constexpr uint16_t VP_TOGGLE_PROBE_SETTINGS_NAV_BUTTON = 0x22C9;

constexpr uint16_t VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_ICON = 0x22CD;
constexpr uint16_t VP_TOGGLE_POST_PROBING_TEMPERATURE_STABILIZATION_BUTTON = 0x22CF;

constexpr uint16_t VP_LEVELING_FADE_HEIGHT = 0x231D;

// E-steps calibration
constexpr uint16_t VP_ESTEPS_CURRENT = 0x229d;
constexpr uint16_t VP_ESTEPS_CALIBRATION_TEMP = 0x229f;
constexpr uint16_t VP_ESTEPS_CALIBRATION_LENGTH = 0x22a1;
constexpr uint16_t VP_ESTEPS_CALIBRATION_MARK_LENGTH = 0x22ab;
constexpr uint16_t VP_ESTEPS_CALIBRATION_LEFTOVER_LENGTH = 0x22a3;
constexpr uint16_t VP_ESTEPS_CALCULATED_ESTEPS = 0x22a5;
constexpr uint16_t VP_ESTEPS_CALIBRATESTART_BUTTON = 0x22a7;
constexpr uint16_t VP_ESTEPS_APPLY_BUTTON = 0x22a9;
constexpr uint16_t VP_ESTEPS_BACK_BUTTON = 0x22D7;

// RGB
constexpr uint16_t VP_RGB_NAV_BUTTON = 0x2390;

constexpr uint16_t VP_RGB_CONTROL_R = 0x2392;
constexpr uint16_t VP_RGB_CONTROL_G = 0x2394;
constexpr uint16_t VP_RGB_CONTROL_B = 0x2396;
constexpr uint16_t VP_RGB_CONTROL_W = 0x2398;
constexpr uint16_t VP_RGB_CONTROL_I = 0x239A; // brightness

constexpr uint16_t VP_RGB_NAV_BUTTON_ICON = 0x239E;
constexpr uint16_t ICON_RGB_SETTINGS_AVAILABLE = 28;
constexpr uint16_t ICON_RGB_SETTINGS_UNAVAILABLE = 29;

// Filament load/unload
constexpr uint16_t VP_FILCHANGE_NAV_BUTTON = 0x23a6;

constexpr uint16_t VP_FILCHANGE_NOZZLE_TEMP = 0x23a0;
constexpr uint16_t VP_FILCHANGE_LENGTH = 0x23a2;

constexpr uint16_t VP_FILCHANGE_ACTION_BUTTON = 0x23a4;
constexpr uint16_t FILCHANGE_ACTION_UNLOAD_BUTTON = 1;
constexpr uint16_t FILCHANGE_ACTION_LOAD_BUTTON = 2;

// Icons
constexpr uint16_t ICON_TOGGLE_ON = 1;
constexpr uint16_t ICON_TOGGLE_OFF = 2;

constexpr uint16_t ICON_BACK_BUTTON_ENABLED = 7;
constexpr uint16_t ICON_BACK_BUTTON_DISABLED = 8;

constexpr uint16_t ICON_THROBBER_ANIM_OFF = 0;
constexpr uint16_t ICON_THROBBER_ANIM_ON = 1;

// Allow to visually disable/enable the back button
constexpr uint16_t VP_BACK_BUTTON_STATE = 0x22D1;

// Throbber animation
constexpr uint16_t VP_BUSY_ANIM_STATE = 0x22D3;

// Toggles
constexpr uint16_t ICON_FAN_TOGGLE_ON = 1;
constexpr uint16_t ICON_FAN_TOGGLE_OFF = 2;
constexpr uint16_t ICON_LED_TOGGLE_ON = 3;
constexpr uint16_t ICON_LED_TOGGLE_OFF = 4;
constexpr uint16_t ICON_SOUND_TOGGLE_ON = 5;
constexpr uint16_t ICON_SOUND_TOGGLE_OFF = 6;
constexpr uint16_t ICON_FWRETRACT_AUTO_TOGGLE_ON = 9;
constexpr uint16_t ICON_FWRETRACT_AUTO_TOGGLE_OFF = 10;
constexpr uint16_t ICON_ACCURACY_TOGGLE_ON = 11;
constexpr uint16_t ICON_ACCURACY_TOGGLE_OFF = 12;
constexpr uint16_t ICON_POST_PROBE_TEMP_STABILIZATION_TOGGLE_ON = 13;
constexpr uint16_t ICON_POST_PROBE_TEMP_STABILIZATION_TOGGLE_OFF = 14;

constexpr uint16_t ICON_FWRETRACT_AUTO_DISENGAGED = 4; // This icon deliberately does not exist
constexpr uint16_t ICON_FWRETRACT_AUTO_ENGAGED = 3;

// Development helper
constexpr uint16_t VP_DEVELOPMENT_HELPER_BUTTON = 0x22D5;
constexpr uint16_t VP_DEVELOPMENT_HELPER_BUTTON_ACTION_FIRMWARE_UPDATE = 2;
constexpr uint16_t VP_DEVELOPMENT_HELPER_BUTTON_ACTION_TO_MAIN_MENU = 4;
constexpr uint16_t VP_DEVELOPMENT_HELPER_BUTTON_ACTION_RESET_DISPLAY = 8;