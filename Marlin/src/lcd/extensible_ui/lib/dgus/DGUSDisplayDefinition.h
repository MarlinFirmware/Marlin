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

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "DGUSVPVariable.h"

// This file defines the interaction between Marlin and the display firmware.

// information on which screen which VP is displayed
// As this is a sparse table, two arrays are needed:
// one to list the VPs of one screen and one to map screens to the lists.
// (Strictly this would not be necessary, but allows to only send data the display needs and reducing load on Marlin)
struct VPMapping {
  const uint8_t screen;
  const uint16_t *VPList;  // The list is null-terminated.
};

extern const struct VPMapping VPMap[];

enum DGUSLCD_Screens : uint8_t {
  DGUSLCD_SCREEN_BOOT = 0,
  DGUSLCD_SCREEN_MAIN = 10,
  DGUSLCD_SCREEN_TEMPERATURE = 20,
  DGUSLCD_SCREEN_STATUS = 30,
  DGUSLCD_SCREEN_STATUS2 = 32,
  DGUSLCD_SCREEN_MANUALMOVE = 40,
  DGUSLCD_SCREEN_MANUALEXTRUDE=42,
  DGUSLCD_SCREEN_FANANDFEEDRATE = 44,
  DGUSLCD_SCREEN_FLOWRATES = 46,
  DGUSLCD_SCREEN_SDFILELIST = 50,
  DGUSLCD_SCREEN_SDPRINTMANIPULATION = 52,
  DGUSLCD_SCREEN_CONFIRM = 240,
  DGUSLCD_SCREEN_KILL = 250, ///< Kill Screen. Must always be 250 (to be able to display "Error wrong LCD Version")
  DGUSLCD_SCREEN_POPUP = 252,  ///< special target, popup screen will also return this code to say "return to previous screen"
  DGUSLDC_SCREEN_UNUSED = 255
};

// Display Memory layout used (T5UID)
// Except system variables this is arbitrary, just to organize stuff....

// 0x0000 .. 0x0FFF  -- System variables and reserved by the display
// 0x1000 .. 0x1FFF  -- Variables to never change location, regardless of UI Version
// 0x2000 .. 0x2FFF  -- Controls (VPs that will trigger some action)
// 0x3000 .. 0x4FFF  -- Marlin Data to be displayed
// 0x5000 ..         -- SPs (if we want to modify display elements, e.g change color or like) -- currently unused

// As there is plenty of space (at least most displays have >8k RAM), we do not pack them too tight,
// so that we can keep variables nicely together in the address space.

// UI Version always on 0x1000...0x1002 so that the firmware can check this and bail out.
constexpr uint16_t VP_UI_VERSION_MAJOR = 0x1000;  // Major -- incremented when incompatible
constexpr uint16_t VP_UI_VERSION_MINOR = 0x1001;  // Minor -- incremented on new features, but compatible
constexpr uint16_t VP_UI_VERSION_PATCH = 0x1002;  // Patch -- fixed which do not change functionality.
constexpr uint16_t VP_UI_FLAVOUR       = 0x1010;  // lets reserve 16 bytes here to determine if UI is suitable for this Marlin. tbd.

// Storage space for the Killscreen messages. 0x1100 - 0x1200 . Reused for the popup.
constexpr uint16_t VP_MSGSTR1 = 0x1100;
constexpr uint8_t VP_MSGSTR1_LEN = 0x20;  // might be more place for it...
constexpr uint16_t VP_MSGSTR2 = 0x1140;
constexpr uint8_t VP_MSGSTR2_LEN = 0x20;
constexpr uint16_t VP_MSGSTR3 = 0x1180;
constexpr uint8_t VP_MSGSTR3_LEN = 0x20;
constexpr uint16_t VP_MSGSTR4 = 0x11C0;
constexpr uint8_t VP_MSGSTR4_LEN = 0x20;

// Screenchange request for screens that only make sense when printer is idle.
// e.g movement is only allowed if printer is not printing.
// Marlin must confirm by setting the screen manually.
constexpr uint16_t VP_SCREENCHANGE_ASK = 0x2000;
constexpr uint16_t VP_SCREENCHANGE = 0x2001;   // Key-Return button to new menu pressed. Data contains target screen in low byte and info in high byte.
constexpr uint16_t VP_TEMP_ALL_OFF = 0x2002;   // Turn all heaters off. Value arbitrary ;)=
constexpr uint16_t VP_SCREENCHANGE_WHENSD = 0x2003; // "Print" Button touched -- go only there if there is an SD Card.

constexpr uint16_t VP_CONFIRMED = 0x2010; // OK on confirm screen.

// Buttons on the SD-Card File listing.
constexpr uint16_t VP_SD_ScrollEvent = 0x2020; // Data: 0 for "up a directory", numbers are the amount to scroll, e.g -1 one up, 1 one down
constexpr uint16_t VP_SD_FileSelected = 0x2022; // Number of file field selected.
constexpr uint16_t VP_SD_FileSelectConfirm = 0x2024; // (This is a virtual VP and emulated by the Confirm Screen when a file has been confirmed)

constexpr uint16_t VP_SD_ResumePauseAbort = 0x2026; // Resume(Data=0), Pause(Data=1), Abort(Data=2) SD Card prints
constexpr uint16_t VP_SD_AbortPrintConfirmed = 0x2028; // Abort print confirmation (virtual, will be injected by the confirm dialog)

// Controls for movement (we can't use the incremental / decremental feature of the display at this feature works only with 16 bit values
// (which would limit us to 655.35mm, which is likely not a problem for common setups, but i don't want to rule out hangprinters support)
// A word about the coding: The VP will be per axis and the return code will be an signed 16 bit value in 0.01 mm resolution, telling us
// the relative travel amount t he user wants to do. So eg. if the display sends us VP=2100 with value 100, the user wants us to move X by +1 mm.
constexpr uint16_t VP_MOVE_X = 0x2100;
constexpr uint16_t VP_MOVE_Y = 0x2102;
constexpr uint16_t VP_MOVE_Z = 0x2104;
constexpr uint16_t VP_MOVE_E0 = 0x2110;
constexpr uint16_t VP_MOVE_E1 = 0x2112;
//constexpr uint16_t VP_MOVE_E2 = 0x2114;
//constexpr uint16_t VP_MOVE_E3 = 0x2116;
//constexpr uint16_t VP_MOVE_E4 = 0x2118;
//constexpr uint16_t VP_MOVE_E5 = 0x211A;
constexpr uint16_t VP_HOME_ALL = 0x2120;

// Firmware version on the boot screen.
constexpr uint16_t VP_MARLIN_VERSION = 0x3000;
constexpr uint8_t VP_MARLIN_VERSION_LEN = 16;   // there is more space on the display, if needed.

// Place for status messages.
constexpr uint16_t VP_M117 = 0x3020;
constexpr uint8_t VP_M117_LEN = 0x20;

// Temperatures.
constexpr uint16_t VP_T_E0_Is = 0x3060;  // 4 Byte Integer
constexpr uint16_t VP_T_E0_Set = 0x3062; // 2 Byte Integer
constexpr uint16_t VP_T_E1_Is = 0x3064;  // 4 Byte Integer

// reserved to support up to 6 Extruders:
//constexpr uint16_t VP_T_E1_Set = 0x3066; // 2 Byte Integer
//constexpr uint16_t VP_T_E2_Is = 0x3068;  // 4 Byte Integer
//constexpr uint16_t VP_T_E2_Set = 0x306A; // 2 Byte Integer
//constexpr uint16_t VP_T_E3_Is = 0x306C;  // 4 Byte Integer
//constexpr uint16_t VP_T_E3_Set = 0x306E; // 2 Byte Integer
//constexpr uint16_t VP_T_E4_Is = 0x3070;  // 4 Byte Integer
//constexpr uint16_t VP_T_E4_Set = 0x3072; // 2 Byte Integer
//constexpr uint16_t VP_T_E4_Is = 0x3074;  // 4 Byte Integer
//constexpr uint16_t VP_T_E4_Set = 0x3076; // 2 Byte Integer
//constexpr uint16_t VP_T_E5_Is = 0x3078;  // 4 Byte Integer
//constexpr uint16_t VP_T_E5_Set = 0x307A; // 2 Byte Integer

constexpr uint16_t VP_T_Bed_Is = 0x3080;  // 4 Byte Integer
constexpr uint16_t VP_T_Bed_Set = 0x3082; // 2 Byte Integer

constexpr uint16_t VP_Flowrate_E0 = 0x3090; // 2 Byte Integer
constexpr uint16_t VP_Flowrate_E1 = 0x3092; // 2 Byte Integer

// reserved for up to 6 Extruders:
//constexpr uint16_t VP_Flowrate_E2 = 0x3094;
//constexpr uint16_t VP_Flowrate_E3 = 0x3096;
//constexpr uint16_t VP_Flowrate_E4 = 0x3098;
//constexpr uint16_t VP_Flowrate_E5 = 0x309A;

constexpr uint16_t VP_Fan_Percentage = 0x3100;  // 2 Byte Integer (0..100)
constexpr uint16_t VP_Feedrate_Percentage = 0x3102; // 2 Byte Integer (0..100)
constexpr uint16_t VP_PrintProgress_Percentage = 0x3104; // 2 Byte Integer (0..100)

constexpr uint16_t VP_PrintTime = 0x3106;
constexpr uint16_t VP_PrintTime_LEN = 10;

// Actual Position
constexpr uint16_t VP_XPos = 0x3110;  // 4 Byte Fixed point number; format xxx.yy
constexpr uint16_t VP_YPos = 0x3112;  // 4 Byte Fixed point number; format xxx.yy
constexpr uint16_t VP_ZPos = 0x3114;  // 4 Byte Fixed point number; format xxx.yy

constexpr uint16_t VP_EPos = 0x3120;  // 4 Byte Fixed point number; format xxx.yy

// SDCard File Listing
constexpr uint16_t VP_SD_FileName_LEN = 32; // LEN is shared for all entries.
constexpr uint16_t DGUS_SD_FILESPERSCREEN = 5; // FIXME move that info to the display and read it from there.
constexpr uint16_t VP_SD_FileName0 = 0x3200;
constexpr uint16_t VP_SD_FileName1 = 0x3220;
constexpr uint16_t VP_SD_FileName2 = 0x3240;
constexpr uint16_t VP_SD_FileName3 = 0x3260;
constexpr uint16_t VP_SD_FileName4 = 0x3280;

// SPs for certain variables...
// located at 0x5000 and up
// Not used yet!
// This can be used e.g to make controls / data display invisible
constexpr uint16_t SP_T_E0_Is = 0x5000;
constexpr uint16_t SP_T_E0_Set = 0x5010;
constexpr uint16_t SP_T_E1_Is = 0x5020;
constexpr uint16_t SP_T_Bed_Is = 0x5030;
constexpr uint16_t SP_T_Bed_Set = 0x5040;

// List of VPs handled by Marlin / The Display.
extern const struct DGUS_VP_Variable ListOfVP[];
