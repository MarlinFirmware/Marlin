/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_E3S1PRO)

#include "DGUSReturnKeyCodeHandler.h"

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"
#include "config/DGUS_Screen.h"

#include "../ui_api.h"
#include "../../../core/language.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/stepper.h"
#include "../../../gcode/queue.h"
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

static uint16_t plaExtruderTempSave = 0;
static uint16_t plaBedTempSave = 0;
static uint16_t absExtruderTempSave = 0;
static uint16_t absBedTempSave = 0;

static DGUS_Screen GetJogScreenForSavedJogLength() {
  DGUS_Screen jogscreen = DGUS_Screen::MOVEAXIS_10;
  switch (dgus_screen_handler.config.jogLength) {
    case DGUS_Data::AxisControlCommand::Jog_10mm:
      jogscreen = DGUS_Screen::MOVEAXIS_10;
      break;
    case DGUS_Data::AxisControlCommand::Jog_1mm:
      jogscreen = DGUS_Screen::MOVEAXIS_1;
      break;
    case DGUS_Data::AxisControlCommand::Jog_0_1mm:
      jogscreen = DGUS_Screen::MOVEAXIS_01;
      break;
    default:
      break;
  }

  return jogscreen;
}

// 1002
void DGUSReturnKeyCodeHandler::Command_MenuSelect(DGUS_VP &vp, void *data) {
  DGUS_Data::MenuSelectCommand submenu = (DGUS_Data::MenuSelectCommand)BE16_P(data);

  switch (submenu) {
    case DGUS_Data::MenuSelectCommand::Main:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::MAIN);
      break;

    case DGUS_Data::MenuSelectCommand::Print:
      if (ExtUI::isMediaInserted()) {
        dgus_sdcard_handler.Reset();
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FILE1);
      }
      else
        dgus_screen_handler.TriggerTempScreenChange(DGUS_Screen::SDCARDCHECK, dgus_screen_handler.GetCurrentScreen());
      break;

    case DGUS_Data::MenuSelectCommand::Ready: {
      DGUS_Screen jogscreen = GetJogScreenForSavedJogLength();
      if (ExtUI::isPositionKnown())
        dgus_screen_handler.TriggerScreenChange(jogscreen);
      else
        dgus_screen_handler.HomeThenChangeScreen(jogscreen);
    } break;

    case DGUS_Data::MenuSelectCommand::Settings:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::TEMP);
      break;

    case DGUS_Data::MenuSelectCommand::StartAutoLevel:
      if (!ExtUI::isPositionKnown())
        dgus_screen_handler.HomeThenChangeScreen(dgus_screen_handler.GetCurrentScreen());

      ExtUI::injectCommands(F("M420 S0\n" TERN(AUTO_BED_LEVELING_UBL, "G29 P1", "G29")));
      break;

    case DGUS_Data::MenuSelectCommand::PrintFinished:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::HOME);
      break;

    case DGUS_Data::MenuSelectCommand::PausePrint:
      dgus_screen_handler.TriggerTempScreenChange(DGUS_Screen::PAUSE_STOP, DGUS_Screen::PAUSE);
      break;

    case DGUS_Data::MenuSelectCommand::PauseStopPrint:
      dgus_screen_handler.TriggerTempScreenChange(DGUS_Screen::CONTINUE_STOP, DGUS_Screen::RESUME);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_MenuSelect: unknown id "); DEBUG_DECIMAL((uint16_t)submenu);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1004
void DGUSReturnKeyCodeHandler::Command_Adjust(DGUS_VP &vp, void *data) {
  DGUS_Data::AdjustCommand command = (DGUS_Data::AdjustCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::AdjustCommand::Show_Adjust:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::ADJUST);
      break;

    case DGUS_Data::AdjustCommand::Show_PrintStatus:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PAUSE);
      break;

    case DGUS_Data::AdjustCommand::Show_Adjust_ZOffset:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PREPARE);
      break;

    case DGUS_Data::AdjustCommand::Validate_ZOffset:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::ADJUST);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_Adjust: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1008
void DGUSReturnKeyCodeHandler::Command_CheckKO(DGUS_VP &vp, void *data) {
  DGUS_Data::CheckKOCommand command = (DGUS_Data::CheckKOCommand)BE16_P(data);

  if (command != DGUS_Data::CheckKOCommand::KO
    && command != DGUS_Data::CheckKOCommand::SDCard_No
  ) {
    #ifdef DEBUG_DGUSLCD
      DEBUG_ECHOPGM("Command_CheckKO: unknown id "); DEBUG_DECIMAL((uint16_t)command);
      DEBUG_EOL();
    #endif
    return;
  }

  switch (dgus_screen_handler.GetCurrentScreen()) {
    case DGUS_Screen::CONTINUE_STOP:
    case DGUS_Screen::STOP_CONFIRM:
    case DGUS_Screen::PAUSE_STOP:
      ExtUI::stopPrint();
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FINISH);
      break;

    case DGUS_Screen::FILAMENTUSEUP:
    case DGUS_Screen::FILAMENTLOAD:
    case DGUS_Screen::SDCARDCHECK:
    case DGUS_Screen::PAUSE_LASER:
    break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_CheckKO: unknown src screen "); DEBUG_DECIMAL((uint8_t)dgus_screen_handler.GetCurrentScreen());
        DEBUG_EOL();
      #endif
      return;
  }

  dgus_screen_handler.TriggerScreenChange(DGUS_Screen::HOME);
}

// 100A
void DGUSReturnKeyCodeHandler::Command_StopPause(DGUS_VP &vp, void *data) {
  DGUS_Data::StopPauseCommand command = (DGUS_Data::StopPauseCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::StopPauseCommand::Pause:
      ExtUI::pausePrint();
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::RESUME);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_StopPause: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 100C
void DGUSReturnKeyCodeHandler::Command_CheckOK(DGUS_VP &vp, void *data) {
  DGUS_Data::CheckOKCommand command = (DGUS_Data::CheckOKCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::CheckOKCommand::ContinueStop_Continue:
      ExtUI::resumePrint();
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PAUSE);
      break;

    #if HAS_FILAMENT_SENSOR
      case DGUS_Data::CheckOKCommand::FilamentRunout_Yes:
        if (ExtUI::getTargetTemp_celsius(ExtUI::E0) < EXTRUDE_MINTEMP)
          ExtUI::setTargetTemp_celsius(EXTRUDE_MINTEMP, ExtUI::E0);

        if (ExtUI::getFilamentRunoutEnabled() && ExtUI::getFilamentRunoutState())
          dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FILAMENTUSEUP);
        else
          dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FILAMENTLOAD);
        break;

      case DGUS_Data::CheckOKCommand::FilamentLoad_Yes:
        if (ExtUI::getFilamentRunoutEnabled() && ExtUI::getFilamentRunoutState())
          dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FILAMENTLOAD);
        else {
          ExtUI::resumePrint();
        }
        break;
    #endif // HAS_FILAMENT_SENSOR

    case DGUS_Data::CheckOKCommand::SDCardCheck_Yes:
      if (ExtUI::isMediaInserted()) {
        dgus_sdcard_handler.Reset();
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FILE1);
      }
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_CheckOK: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1030
void DGUSReturnKeyCodeHandler::Command_PresetControl(DGUS_VP &vp, void *data) {
  DGUS_Data::PresetControlCommand command = (DGUS_Data::PresetControlCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::PresetControlCommand::Show_Ready_Manual:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::CONTROL);
      break;

    case DGUS_Data::PresetControlCommand::Show_Settings_PLA_Settings:
      plaExtruderTempSave = dgus_screen_handler.config.plaExtruderTemp;
      plaBedTempSave = dgus_screen_handler.config.plaBedTemp;
      absExtruderTempSave = dgus_screen_handler.config.absExtruderTemp;
      absBedTempSave = dgus_screen_handler.config.absBedTemp;
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PLA_TEMP);
      break;

    case DGUS_Data::PresetControlCommand::Show_Settings_ABS_Settings:
      plaExtruderTempSave = dgus_screen_handler.config.plaExtruderTemp;
      plaBedTempSave = dgus_screen_handler.config.plaBedTemp;
      absExtruderTempSave = dgus_screen_handler.config.absExtruderTemp;
      absBedTempSave = dgus_screen_handler.config.absBedTemp;
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::ABS_TEMP);
      break;

    case DGUS_Data::PresetControlCommand::Apply_PLA_Settings:
      ExtUI::setTargetTemp_celsius(dgus_screen_handler.config.plaExtruderTemp, ExtUI::E0);
      ExtUI::setTargetTemp_celsius(dgus_screen_handler.config.plaBedTemp, ExtUI::BED);
      break;

    case DGUS_Data::PresetControlCommand::Apply_ABS_Settings:
      ExtUI::setTargetTemp_celsius(dgus_screen_handler.config.absExtruderTemp, ExtUI::E0);
      ExtUI::setTargetTemp_celsius(dgus_screen_handler.config.absBedTemp, ExtUI::BED);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Settings_Submenu: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1032
void DGUSReturnKeyCodeHandler::Control_TemperatureCommand(DGUS_VP &vp, void *data) {
  DGUS_Data::TemperatureControlCommand command = (DGUS_Data::TemperatureControlCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::TemperatureControlCommand::Cooldown:
      ExtUI::coolDown();
      break;

    case DGUS_Data::TemperatureControlCommand::Exit_Temperature_Preset_And_Discard_Temperature:
      dgus_screen_handler.config.plaExtruderTemp = plaExtruderTempSave;
      dgus_screen_handler.config.plaBedTemp = plaBedTempSave;
      dgus_screen_handler.config.absExtruderTemp = absExtruderTempSave;
      dgus_screen_handler.config.absBedTemp = absBedTempSave;
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::TEMP);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Control_TemperatureCommand: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 103E
void DGUSReturnKeyCodeHandler::Command_SettingsMenu(DGUS_VP &vp, void *data) {
  DGUS_Data::SettingsMenuCommand command = (DGUS_Data::SettingsMenuCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::SettingsMenuCommand::DisableStepperMotors: {
      const bool areSteppersEnabled = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
      if (areSteppersEnabled)
        stepper.disable_all_steppers();
      else
        stepper.enable_all_steppers();
    } break;

    case DGUS_Data::SettingsMenuCommand::Reset_All_Settings:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FACTORYRESET_CONFIRM);
      break;

    case DGUS_Data::SettingsMenuCommand::FactoryReset_Validate:
      ExtUI::injectCommands(F("M502"));

    case DGUS_Data::SettingsMenuCommand::FactoryReset_Cancel:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::CONTROL_DEVICE);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Device_and_Save_Temperatures:
      dgus_screen_handler.TriggerEEPROMSave();
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::TEMP);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Device_and_Discard_Temperatures:
      dgus_screen_handler.config.plaExtruderTemp = plaExtruderTempSave;
      dgus_screen_handler.config.plaBedTemp = plaBedTempSave;
      dgus_screen_handler.config.absExtruderTemp = absExtruderTempSave;
      dgus_screen_handler.config.absBedTemp = absBedTempSave;
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::TEMP);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Device_Language:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::LANGUAGE);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_About:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::INFORMATION);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Advanced:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::CONTROL_DEVICE);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Ready_Jog:
      dgus_screen_handler.TriggerScreenChange(GetJogScreenForSavedJogLength());
      break;

    case DGUS_Data::SettingsMenuCommand::Exit_Settings_Tramming:
      dgus_screen_handler.TriggerEEPROMSave();
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::TEMP);
      break;

    case DGUS_Data::SettingsMenuCommand::Exit_Settings_Leveling:
      dgus_screen_handler.TriggerEEPROMSave();
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::TEMP);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_SettingsMenu: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

static void _GotoTrammingPoint(unsigned char point) {
  constexpr float lfrb[4] = BED_TRAMMING_INSET_LFRB;
  float x, y;

  switch (point) {
    default: return;
    case 1:
      x = (X_BED_SIZE/2);
      y = (X_BED_SIZE/2);
      break;
    case 2:
      x = X_MIN_POS + lfrb[0];
      y = Y_MIN_POS + lfrb[1];
      break;
    case 3:
      x = X_MAX_POS - lfrb[2];
      y = Y_MIN_POS + lfrb[1];
      break;
    case 4:
      x = X_MAX_POS - lfrb[2];
      y = Y_MAX_POS - lfrb[3];
      break;
    case 5:
      x = X_MIN_POS + lfrb[0];
      y = Y_MAX_POS - lfrb[3];
      break;
  }

  if (ExtUI::getAxisPosition_mm(ExtUI::Z) < Z_MIN_POS + BED_TRAMMING_Z_HOP) {
    ExtUI::setAxisPosition_mm(Z_MIN_POS + BED_TRAMMING_Z_HOP, ExtUI::Z);
  }

  ExtUI::setAxisPosition_mm(x, ExtUI::X);
  ExtUI::setAxisPosition_mm(y, ExtUI::Y);
  ExtUI::setAxisPosition_mm(Z_MIN_POS + BED_TRAMMING_HEIGHT, ExtUI::Z);
}

// 1044
void DGUSReturnKeyCodeHandler::Command_Leveling(DGUS_VP &vp, void *data) {
  DGUS_Data::LevelingCommand command = (DGUS_Data::LevelingCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::LevelingCommand::Show_AuxLeveling:
      if (ExtUI::isPositionKnown())
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::LEVELINGMODE);
      _GotoTrammingPoint(1);
      break;

    case DGUS_Data::LevelingCommand::Show_Settings_Leveling:
    case DGUS_Data::LevelingCommand::Show_AutoLeveling:
        dgus_screen_handler.HomeThenChangeScreen(DGUS_Screen::LEVELING);
      break;

    case DGUS_Data::LevelingCommand::Goto_Center:
      _GotoTrammingPoint(1);
      break;

    case DGUS_Data::LevelingCommand::Goto_LF:
      _GotoTrammingPoint(2);
      break;

    case DGUS_Data::LevelingCommand::Goto_RF:
      _GotoTrammingPoint(3);
      break;

    case DGUS_Data::LevelingCommand::Goto_RB:
      _GotoTrammingPoint(4);
      break;

    case DGUS_Data::LevelingCommand::Goto_LB:
      _GotoTrammingPoint(5);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_Leveling: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1046
void DGUSReturnKeyCodeHandler::Command_AxisControl(DGUS_VP &vp, void *data) {
  DGUS_Data::AxisControlCommand control = (DGUS_Data::AxisControlCommand)BE16_P(data);

  switch (control) {
    case DGUS_Data::AxisControlCommand::Jog_10mm:
    case DGUS_Data::AxisControlCommand::Jog_1mm:
    case DGUS_Data::AxisControlCommand::Jog_0_1mm:
      dgus_screen_handler.config.jogLength = control;
    break;

    case DGUS_Data::AxisControlCommand::Home_XY:
      ExtUI::injectCommands(F("G28XY"));
      break;

    case DGUS_Data::AxisControlCommand::Home_Z:
      ExtUI::injectCommands(F("G28Z"));
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_AxisControl: unknown id "); DEBUG_DECIMAL((uint16_t)control);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1056
void DGUSReturnKeyCodeHandler::Command_AxisIO(DGUS_VP &vp, void *data) {
  DGUS_Data::AxisIoCommand command = (DGUS_Data::AxisIoCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::AxisIoCommand::Show_Ready_IO:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::FEEDRETURN);
      break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_AxisIO: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 1098
void DGUSReturnKeyCodeHandler::Command_AdvancedSettings(DGUS_VP &vp, void *data) {
  DGUS_Data::AdvancedSettingsCommand command = (DGUS_Data::AdvancedSettingsCommand)BE16_P(data);

  switch (command) {
    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::MOTION);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_PID:
      #if !ENABLED(PIDTEMP) && !ENABLED(PIDBEDTEMP)
        dgus_screen_handler.AngryBeeps(2);
      #else
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PIDCONTROL);
      #endif
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_MaxFeedrate:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::MAX_FEEDRATE);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_Acceleration:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::ACCELERATION);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_Jerk:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::JERK);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_StepsPerMm:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::STEPSMM);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Exit_AdvSettings_Movement_Submenu:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::MOTION);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::CONTROL_DEVICE);
    break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_AdvancedSettings: unknown id "); DEBUG_DECIMAL((uint16_t)command);
        DEBUG_EOL();
      #endif
      break;
  }
}

// 2198
void DGUSReturnKeyCodeHandler::Command_FilelistControl(DGUS_VP &vp, void *data) {
  DGUS_Data::FilelistControlCommand control = (DGUS_Data::FilelistControlCommand)BE16_P(data);
  DGUS_SDCardHandler::page_t newPage;

  switch (control) {
    case DGUS_Data::FilelistControlCommand::Start_Print:
      if (!dgus_screen_handler.GetSDCardPrintFilename()[0]) {
        dgus_screen_handler.TriggerTempScreenChange(DGUS_Screen::PAUSE_STOP, DGUS_Screen::PAUSE);
      }

      ExtUI::printFile(dgus_screen_handler.GetSDCardPrintFilename());
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PAUSE);
      return;

    case DGUS_Data::FilelistControlCommand::F1_Up:
    //case DGUS_Data::FilelistControlCommand::F2_Up:
    case DGUS_Data::FilelistControlCommand::F3_Up:
    case DGUS_Data::FilelistControlCommand::F4_Up:
      newPage = dgus_sdcard_handler.onPreviousPage();
    break;

    case DGUS_Data::FilelistControlCommand::F1_Down:
    case DGUS_Data::FilelistControlCommand::F2_Down:
    case DGUS_Data::FilelistControlCommand::F3_Down:
    case DGUS_Data::FilelistControlCommand::F4_Down:
      newPage = dgus_sdcard_handler.onNextPage();
    break;

    case DGUS_Data::FilelistControlCommand::Begin:
      newPage = dgus_sdcard_handler.onFirstPage();
    break;

    case DGUS_Data::FilelistControlCommand::End:
      newPage = dgus_sdcard_handler.onLastPage();
    break;

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_FilelistControl: unknown id "); DEBUG_DECIMAL((uint16_t)control);
        DEBUG_EOL();
      #endif
      return;
  }

  dgus_screen_handler.TriggerScreenChange(DGUS_PAGE_TO_SCREEN(newPage));
}

// 2201
void DGUSReturnKeyCodeHandler::Command_LaserControl(DGUS_VP &vp, void *data) {
  DGUS_Data::LaserControlCommand control = (DGUS_Data::LaserControlCommand)BE16_P(data);

  switch (control) {
    case DGUS_Data::LaserControlCommand::Mode_FDM:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::SW_FDM_TIPS);
      break;
    case DGUS_Data::LaserControlCommand::Mode_FDM_Confirm:
      dgus_screen_handler.TriggerScreenChange(DGUS_Screen::SW_FDM_TIPS);
      break;

    #if HAS_CUTTER
      case DGUS_Data::LaserControlCommand::Mode_Cutter:
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::SW_LASER_TIPS);
      break;

      case DGUS_Data::LaserControlCommand::Mode_Cutter_Confirm:
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::SW_LASER_TIPS);
      break;
    #endif

    case DGUS_Data::LaserControlCommand::Mode_Change:
      #if HAS_CUTTER
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::LASER_FDM);
      #else
        dgus_screen_handler.AngryBeeps(2);
      #endif

    default:
      #ifdef DEBUG_DGUSLCD
        DEBUG_ECHOPGM("Command_LaserControl: unknown id "); DEBUG_DECIMAL((uint16_t)control);
        DEBUG_EOL();
      #endif
      return;
  }
}

#endif // DGUS_LCD_UI_RELOADED
