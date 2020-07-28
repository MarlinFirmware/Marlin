/***************
 * screens.cpp *
 ***************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)
#include "screens.h"
#include "screen_data.h"

tiny_timer_t refresh_timer;
screen_data_t screen_data;

SCREEN_TABLE {
  DECL_SCREEN(BootScreen),
  #if NUM_LANGUAGES > 1
    DECL_SCREEN(LanguageMenu),
  #endif
  DECL_SCREEN(TouchCalibrationScreen),
  DECL_SCREEN(StatusScreen),
  DECL_SCREEN(MainMenu),
  DECL_SCREEN(TuneMenu),
  DECL_SCREEN(AdvancedSettingsMenu),
  DECL_SCREEN(AlertDialogBox),
  DECL_SCREEN(ConfirmUserRequestAlertBox),
  DECL_SCREEN(RestoreFailsafeDialogBox),
  DECL_SCREEN(SaveSettingsDialogBox),
  DECL_SCREEN(ConfirmStartPrintDialogBox),
  DECL_SCREEN(ConfirmAbortPrintDialogBox),
#if ENABLED(CALIBRATION_GCODE)
  DECL_SCREEN(ConfirmAutoCalibrationDialogBox),
#endif
  DECL_SCREEN(SpinnerDialogBox),
  DECL_SCREEN(AboutScreen),
#if ENABLED(PRINTCOUNTER)
  DECL_SCREEN(StatisticsScreen),
#endif
#if ENABLED(BABYSTEPPING)
  DECL_SCREEN(NudgeNozzleScreen),
#endif
#if HAS_MESH
  DECL_SCREEN(BedMeshScreen),
#endif
  DECL_SCREEN(MoveAxisScreen),
  DECL_SCREEN(StepsScreen),
#if HAS_TRINAMIC_CONFIG
  DECL_SCREEN(StepperCurrentScreen),
  DECL_SCREEN(StepperBumpSensitivityScreen),
#endif
#if HAS_BED_PROBE
  DECL_SCREEN(ZOffsetScreen),
#endif
#if HAS_MULTI_HOTEND
  DECL_SCREEN(NozzleOffsetScreen),
#endif
#if ENABLED(BACKLASH_GCODE)
  DECL_SCREEN(BacklashCompensationScreen),
#endif
  DECL_SCREEN(FeedratePercentScreen),
  DECL_SCREEN(MaxVelocityScreen),
  DECL_SCREEN(MaxAccelerationScreen),
  DECL_SCREEN(DefaultAccelerationScreen),
#if HAS_JUNCTION_DEVIATION
  DECL_SCREEN(JunctionDeviationScreen),
#else
  DECL_SCREEN(JerkScreen),
#endif
#if HAS_CASE_LIGHT
  DECL_SCREEN(CaseLightScreen),
#endif
#if EITHER(LIN_ADVANCE, FILAMENT_RUNOUT_SENSOR)
  DECL_SCREEN(FilamentMenu),
#endif
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  DECL_SCREEN(FilamentRunoutScreen),
#endif
#if ENABLED(LIN_ADVANCE)
  DECL_SCREEN(LinearAdvanceScreen),
#endif
  DECL_SCREEN(TemperatureScreen),
  DECL_SCREEN(ChangeFilamentScreen),
  DECL_SCREEN(InterfaceSettingsScreen),
  DECL_SCREEN(InterfaceSoundsScreen),
  DECL_SCREEN(LockScreen),
  DECL_SCREEN(FilesScreen),
  DECL_SCREEN(EndstopStatesScreen),
#if ENABLED(TOUCH_UI_LULZBOT_BIO)
  DECL_SCREEN(BioPrintingDialogBox),
  DECL_SCREEN(BioConfirmHomeXYZ),
  DECL_SCREEN(BioConfirmHomeE),
#endif
#if ENABLED(TOUCH_UI_COCOA_PRESS)
  DECL_SCREEN(PreheatMenu),
  DECL_SCREEN(PreheatTimerScreen),
#endif
#if ENABLED(TOUCH_UI_DEVELOPER_MENU)
  DECL_SCREEN(DeveloperMenu),
  DECL_SCREEN(ConfirmEraseFlashDialogBox),
  DECL_SCREEN(WidgetsScreen),
  DECL_SCREEN(TouchRegistersScreen),
  DECL_SCREEN(StressTestScreen),
#endif
  DECL_SCREEN(MediaPlayerScreen),
  DECL_SCREEN(DisplayTuningScreen)
};

SCREEN_TABLE_POST

#endif // TOUCH_UI_FTDI_EVE
