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

#define DGUS_UNKNOWN_COMMAND_DEBUG // uncomment to debug unknown commands

static uint16_t plaExtruderTempSave = 0;
static uint16_t plaBedTempSave = 0;
static uint16_t absExtruderTempSave = 0;
static uint16_t absBedTempSave = 0;

static DGUS_ScreenID GetJogScreenForSavedJogLength() {
  DGUS_ScreenID jogscreen = DGUS_ScreenID::MOVEAXIS_10;
  switch (screen.config.jogLength) {
    case DGUS_Data::AxisControlCommand::Jog_10mm:  jogscreen = DGUS_ScreenID::MOVEAXIS_10; break;
    case DGUS_Data::AxisControlCommand::Jog_1mm:   jogscreen = DGUS_ScreenID::MOVEAXIS_1; break;
    case DGUS_Data::AxisControlCommand::Jog_0_1mm: jogscreen = DGUS_ScreenID::MOVEAXIS_01; break;
    default: break;
  }

  return jogscreen;
}

// 1002
void DGUSReturnKeyCodeHandler::Command_MenuSelect(DGUS_VP &vp, void *data) {
  DGUS_Data::MenuSelectCommand submenu = Endianness::fromBE_P<DGUS_Data::MenuSelectCommand>(data);

  switch (submenu) {
    case DGUS_Data::MenuSelectCommand::Main:
      screen.triggerScreenChange(DGUS_ScreenID::MAIN);
      break;

    case DGUS_Data::MenuSelectCommand::Print:
      if (ExtUI::isMediaInserted()) {
        dgus_sdcard_handler.Reset();
        screen.triggerScreenChange(DGUS_ScreenID::FILE1);
      }
      else
        screen.triggerTempScreenChange(DGUS_ScreenID::SDCARDCHECK, screen.getCurrentScreen());
      break;

    case DGUS_Data::MenuSelectCommand::Ready: {
      DGUS_ScreenID jogscreen = GetJogScreenForSavedJogLength();
      if (ExtUI::isPositionKnown())
        screen.triggerScreenChange(jogscreen);
      else
        screen.homeThenChangeScreen(jogscreen);
    } break;

    case DGUS_Data::MenuSelectCommand::Settings:
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    case DGUS_Data::MenuSelectCommand::StartAutoLevel:
      if (!ExtUI::isPositionKnown())
        screen.homeThenChangeScreen(screen.getCurrentScreen());

      ExtUI::injectCommands(F("M420 S0\n" TERN(AUTO_BED_LEVELING_UBL, "G29 P1", "G29")));
      break;

    case DGUS_Data::MenuSelectCommand::PrintFinished:
      screen.triggerScreenChange(DGUS_ScreenID::HOME);
      break;

    case DGUS_Data::MenuSelectCommand::PausePrint:
      screen.triggerTempScreenChange(DGUS_ScreenID::PAUSE_STOP, DGUS_ScreenID::PAUSE);
      break;

    case DGUS_Data::MenuSelectCommand::PauseStopPrint:
      screen.triggerTempScreenChange(DGUS_ScreenID::CONTINUE_STOP, DGUS_ScreenID::RESUME);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_MenuSelect: unknown id ", (uint16_t)submenu);
      #endif
      break;
  }
}

// 1004
void DGUSReturnKeyCodeHandler::Command_Adjust(DGUS_VP &vp, void *data) {
  DGUS_Data::AdjustCommand command = Endianness::fromBE_P<DGUS_Data::AdjustCommand>(data);

  switch (command) {
    case DGUS_Data::AdjustCommand::Show_Adjust:
      screen.triggerScreenChange(DGUS_ScreenID::ADJUST);
      break;

    case DGUS_Data::AdjustCommand::Show_PrintStatus:
      screen.triggerScreenChange(DGUS_ScreenID::PAUSE);
      break;

    case DGUS_Data::AdjustCommand::Show_Adjust_ZOffset:
      screen.triggerScreenChange(DGUS_ScreenID::PREPARE);
      break;

    case DGUS_Data::AdjustCommand::Validate_ZOffset:
      screen.triggerScreenChange(DGUS_ScreenID::ADJUST);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_Adjust: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 1008
void DGUSReturnKeyCodeHandler::Command_CheckKO(DGUS_VP &vp, void *data) {
  DGUS_Data::CheckKOCommand command = Endianness::fromBE_P<DGUS_Data::CheckKOCommand>(data);

  if (command != DGUS_Data::CheckKOCommand::KO && command != DGUS_Data::CheckKOCommand::SDCard_No) {
    #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
      DEBUG_ECHOLNPGM("Command_CheckKO: unknown id ", (uint16_t)command);
    #endif
    return;
  }

  switch (screen.getCurrentScreen()) {
    case DGUS_ScreenID::CONTINUE_STOP:
    case DGUS_ScreenID::STOP_CONFIRM:
    case DGUS_ScreenID::PAUSE_STOP:
    case DGUS_ScreenID::FILAMENTUSEUP:
    case DGUS_ScreenID::FILAMENTLOAD:
      ExtUI::stopPrint();
      TERN_(HAS_FILAMENT_SENSOR,ExtUI::setFilamentRunoutState(false));
      screen.triggerScreenChange(DGUS_ScreenID::FINISH);
      break;

    case DGUS_ScreenID::SDCARDCHECK:
    case DGUS_ScreenID::PAUSE_LASER:
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_CheckKO: unknown src screen ", (uint16_t)screen.getCurrentScreen());
      #endif
      return;
  }

  screen.triggerScreenChange(DGUS_ScreenID::HOME);
}

// 100A
void DGUSReturnKeyCodeHandler::Command_StopPause(DGUS_VP &vp, void *data) {
  DGUS_Data::StopPauseCommand command = Endianness::fromBE_P<DGUS_Data::StopPauseCommand>(data);

  switch (command) {
    case DGUS_Data::StopPauseCommand::Pause:
      ExtUI::pausePrint();
      screen.triggerScreenChange(DGUS_ScreenID::RESUME);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_StopPause: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 100C
void DGUSReturnKeyCodeHandler::Command_CheckOK(DGUS_VP &vp, void *data) {
  DGUS_Data::CheckOKCommand command = Endianness::fromBE_P<DGUS_Data::CheckOKCommand>(data);

  switch (command) {
    case DGUS_Data::CheckOKCommand::ContinueStop_Continue:
      #if HAS_FILAMENT_SENSOR
        if (ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE) {
          screen.triggerScreenChange(DGUS_ScreenID::FILAMENTUSEUP);
          break;
        }
      #endif
      ExtUI::resumePrint();
      screen.triggerScreenChange(DGUS_ScreenID::PAUSE);
      break;

    #if HAS_FILAMENT_SENSOR
      case DGUS_Data::CheckOKCommand::FilamentRunout_Yes:
        if (ExtUI::getTargetTemp_celsius(ExtUI::E0) < EXTRUDE_MINTEMP)
          ExtUI::setTargetTemp_celsius(EXTRUDE_MINTEMP, ExtUI::E0);

        if (ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE)
          screen.triggerScreenChange(DGUS_ScreenID::FILAMENTUSEUP);
        else
          screen.triggerScreenChange(DGUS_ScreenID::FILAMENTLOAD);
        break;

      case DGUS_Data::CheckOKCommand::FilamentLoad_Yes:
        if (ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE) {
          screen.triggerScreenChange(DGUS_ScreenID::FILAMENTLOAD);
          break;
        }

        ExtUI::setFilamentRunoutState(false);
        ExtUI::resumePrint();
        break;
    #endif // HAS_FILAMENT_SENSOR

    case DGUS_Data::CheckOKCommand::SDCardCheck_Yes:
      if (ExtUI::isMediaInserted()) {
        dgus_sdcard_handler.Reset();
        screen.triggerScreenChange(DGUS_ScreenID::FILE1);
      }
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_CheckOK: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 1030
void DGUSReturnKeyCodeHandler::Command_PresetControl(DGUS_VP &vp, void *data) {
  DGUS_Data::PresetControlCommand command = Endianness::fromBE_P<DGUS_Data::PresetControlCommand>(data);

  switch (command) {
    case DGUS_Data::PresetControlCommand::Show_Ready_Manual:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL);
      break;

    case DGUS_Data::PresetControlCommand::Show_Settings_PLA_Settings:
      plaExtruderTempSave = screen.config.plaExtruderTemp;
      plaBedTempSave = screen.config.plaBedTemp;
      absExtruderTempSave = screen.config.absExtruderTemp;
      absBedTempSave = screen.config.absBedTemp;
      screen.triggerScreenChange(DGUS_ScreenID::PLA_TEMP);
      break;

    case DGUS_Data::PresetControlCommand::Show_Settings_ABS_Settings:
      plaExtruderTempSave = screen.config.plaExtruderTemp;
      plaBedTempSave = screen.config.plaBedTemp;
      absExtruderTempSave = screen.config.absExtruderTemp;
      absBedTempSave = screen.config.absBedTemp;
      screen.triggerScreenChange(DGUS_ScreenID::ABS_TEMP);
      break;

    case DGUS_Data::PresetControlCommand::Apply_PLA_Settings:
      ExtUI::setTargetTemp_celsius(screen.config.plaExtruderTemp, ExtUI::E0);
      ExtUI::setTargetTemp_celsius(screen.config.plaBedTemp, ExtUI::BED);
      break;

    case DGUS_Data::PresetControlCommand::Apply_ABS_Settings:
      ExtUI::setTargetTemp_celsius(screen.config.absExtruderTemp, ExtUI::E0);
      ExtUI::setTargetTemp_celsius(screen.config.absBedTemp, ExtUI::BED);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Settings_Submenu: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 1032
void DGUSReturnKeyCodeHandler::Control_TemperatureCommand(DGUS_VP &vp, void *data) {
  DGUS_Data::TemperatureControlCommand command = Endianness::fromBE_P<DGUS_Data::TemperatureControlCommand>(data);

  switch (command) {
    case DGUS_Data::TemperatureControlCommand::Cooldown:
      ExtUI::coolDown();
      break;

    case DGUS_Data::TemperatureControlCommand::Exit_Temperature_Preset_And_Discard_Temperature:
      screen.config.plaExtruderTemp = plaExtruderTempSave;
      screen.config.plaBedTemp = plaBedTempSave;
      screen.config.absExtruderTemp = absExtruderTempSave;
      screen.config.absBedTemp = absBedTempSave;
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Control_TemperatureCommand: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 103E
void DGUSReturnKeyCodeHandler::Command_SettingsMenu(DGUS_VP &vp, void *data) {
  DGUS_Data::SettingsMenuCommand command = Endianness::fromBE_P<DGUS_Data::SettingsMenuCommand>(data);

  switch (command) {
    case DGUS_Data::SettingsMenuCommand::DisableStepperMotors: {
      const bool areSteppersEnabled = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
      if (areSteppersEnabled)
        stepper.disable_all_steppers();
      else
        stepper.enable_all_steppers();
    } break;

    case DGUS_Data::SettingsMenuCommand::Reset_All_Settings:
      screen.triggerScreenChange(DGUS_ScreenID::FACTORYRESET_CONFIRM);
      break;

    case DGUS_Data::SettingsMenuCommand::FactoryReset_Validate:
      ExtUI::injectCommands(F("M502"));

    case DGUS_Data::SettingsMenuCommand::FactoryReset_Cancel:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL_DEVICE);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Device_and_Save_Temperatures:
      screen.triggerEEPROMSave();
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Device_and_Discard_Temperatures:
      screen.config.plaExtruderTemp = plaExtruderTempSave;
      screen.config.plaBedTemp = plaBedTempSave;
      screen.config.absExtruderTemp = absExtruderTempSave;
      screen.config.absBedTemp = absBedTempSave;
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Device_Language:
      screen.triggerScreenChange(DGUS_ScreenID::LANGUAGE);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_About:
      screen.triggerScreenChange(DGUS_ScreenID::INFORMATION);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Settings_Advanced:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL_DEVICE);
      break;

    case DGUS_Data::SettingsMenuCommand::Show_Ready_Jog:
      screen.triggerScreenChange(GetJogScreenForSavedJogLength());
      break;

    case DGUS_Data::SettingsMenuCommand::Exit_Settings_Tramming:
      screen.triggerEEPROMSave();
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    case DGUS_Data::SettingsMenuCommand::Exit_Settings_Leveling:
      screen.triggerEEPROMSave();
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_SettingsMenu: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

static void _gotoTrammingPoint(unsigned char point) {
  constexpr float lfrb[4] = BED_TRAMMING_INSET_LFRB;
  float x, y;

  switch (point) {
    default: return;
    case 1: x = X_CENTER; y = Y_CENTER; break;
    case 2: x = X_MIN_POS + lfrb[0]; y = Y_MIN_POS + lfrb[1]; break;
    case 3: x = X_MAX_POS - lfrb[2]; y = Y_MIN_POS + lfrb[1]; break;
    case 4: x = X_MAX_POS - lfrb[2]; y = Y_MAX_POS - lfrb[3]; break;
    case 5: x = X_MIN_POS + lfrb[0]; y = Y_MAX_POS - lfrb[3]; break;
  }

  if (ExtUI::getAxisPosition_mm(ExtUI::Z) < (Z_MIN_POS) + (BED_TRAMMING_Z_HOP))
    ExtUI::setAxisPosition_mm((Z_MIN_POS) + (BED_TRAMMING_Z_HOP), ExtUI::Z);

  ExtUI::setAxisPosition_mm(x, ExtUI::X);
  ExtUI::setAxisPosition_mm(y, ExtUI::Y);
  ExtUI::setAxisPosition_mm((Z_MIN_POS) + (BED_TRAMMING_HEIGHT), ExtUI::Z);
}

// 1044
void DGUSReturnKeyCodeHandler::Command_Leveling(DGUS_VP &vp, void *data) {
  DGUS_Data::LevelingCommand command = Endianness::fromBE_P<DGUS_Data::LevelingCommand>(data);

  switch (command) {
    case DGUS_Data::LevelingCommand::Show_AuxLeveling:
      if (ExtUI::isPositionKnown())
        screen.triggerScreenChange(DGUS_ScreenID::LEVELINGMODE);
      _gotoTrammingPoint(1);
      break;

    case DGUS_Data::LevelingCommand::Show_Settings_Leveling:
    case DGUS_Data::LevelingCommand::Show_AutoLeveling:
      screen.homeThenChangeScreen(DGUS_ScreenID::LEVELING);
      break;

    case DGUS_Data::LevelingCommand::Goto_Center:
      _gotoTrammingPoint(1);
      break;

    case DGUS_Data::LevelingCommand::Goto_LF:
      _gotoTrammingPoint(2);
      break;

    case DGUS_Data::LevelingCommand::Goto_RF:
      _gotoTrammingPoint(3);
      break;

    case DGUS_Data::LevelingCommand::Goto_RB:
      _gotoTrammingPoint(4);
      break;

    case DGUS_Data::LevelingCommand::Goto_LB:
      _gotoTrammingPoint(5);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_Leveling: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 1046
void DGUSReturnKeyCodeHandler::Command_AxisControl(DGUS_VP &vp, void *data) {
  DGUS_Data::AxisControlCommand control = Endianness::fromBE_P<DGUS_Data::AxisControlCommand>(data);

  switch (control) {
    case DGUS_Data::AxisControlCommand::Jog_10mm:
    case DGUS_Data::AxisControlCommand::Jog_1mm:
    case DGUS_Data::AxisControlCommand::Jog_0_1mm:
      screen.config.jogLength = control;
    break;

    case DGUS_Data::AxisControlCommand::Home_XY:
      ExtUI::injectCommands(F("G28XY"));
      break;

    case DGUS_Data::AxisControlCommand::Home_Z:
      ExtUI::injectCommands(F("G28Z"));
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_AxisControl: unknown id ", (uint16_t)control);
      #endif
      break;
  }
}

// 1056
void DGUSReturnKeyCodeHandler::Command_FilamentIO(DGUS_VP &vp, void *data) {
  DGUS_Data::FilamentIoCommand command = Endianness::fromBE_P<DGUS_Data::FilamentIoCommand>(data);

  switch (command) {
    case DGUS_Data::FilamentIoCommand::FilamentCheck_Yes:
    case DGUS_Data::FilamentIoCommand::FilamentCheck_No:
      screen.triggerReturnScreen();
      break;

    case DGUS_Data::FilamentIoCommand::Show_Ready_IO:
      screen.triggerScreenChange(DGUS_ScreenID::FEEDRETURN);
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_FilamentIO: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 105F
void DGUSReturnKeyCodeHandler::Command_PowerLoss(DGUS_VP &vp, void *data) {
  DGUS_Data::PowerLossCommand command = Endianness::fromBE_P<DGUS_Data::PowerLossCommand>(data);

  switch (command) {
    case DGUS_Data::PowerLossCommand::PowerLoss_Continue:
      if (!recovery.valid()) {
        screen.setStatusMessage(GET_TEXT_F(DGUS_MSG_INVALID_RECOVERY_DATA));
        screen.triggerScreenChange(DGUS_ScreenID::HOME);
        return;
      }

      screen.triggerScreenChange(DGUS_ScreenID::PAUSE);
      ExtUI::injectCommands(F("M1000"));
      break;

    case DGUS_Data::PowerLossCommand::PowerLoss_No:
      screen.triggerScreenChange(DGUS_ScreenID::HOME);
      ExtUI::injectCommands(F("M1000 C"));
      break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_PowerLoss: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 1098
void DGUSReturnKeyCodeHandler::Command_AdvancedSettings(DGUS_VP &vp, void *data) {
  DGUS_Data::AdvancedSettingsCommand command = Endianness::fromBE_P<DGUS_Data::AdvancedSettingsCommand>(data);

  switch (command) {
    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement:
      screen.triggerScreenChange(DGUS_ScreenID::MOTION);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_PID:
      #if NONE(PIDTEMP, PIDTEMPBED)
        screen.angryBeeps(2);
        screen.triggerScreenChange(DGUS_ScreenID::CONTROL_DEVICE);
      #else
        screen.triggerScreenChange(DGUS_ScreenID::PIDCONTROL);
      #endif
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_MaxFeedrate:
      screen.triggerScreenChange(DGUS_ScreenID::MAX_FEEDRATE);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_Acceleration:
      screen.triggerScreenChange(DGUS_ScreenID::ACCELERATION);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_Jerk:
      screen.triggerScreenChange(DGUS_ScreenID::JERK);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings_Movement_StepsPerMm:
      screen.triggerScreenChange(DGUS_ScreenID::STEPSMM);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Exit_AdvSettings_Movement_Submenu:
      screen.triggerScreenChange(DGUS_ScreenID::MOTION);
    break;

    case DGUS_Data::AdvancedSettingsCommand::Show_AdvSettings:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL_DEVICE);
    break;

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_AdvancedSettings: unknown id ", (uint16_t)command);
      #endif
      break;
  }
}

// 2198
void DGUSReturnKeyCodeHandler::Command_FilelistControl(DGUS_VP &vp, void *data) {
  DGUS_Data::FilelistControlCommand control = Endianness::fromBE_P<DGUS_Data::FilelistControlCommand>(data);
  DGUS_SDCardHandler::page_t newPage;

  switch (control) {
    #if HAS_MEDIA
      case DGUS_Data::FilelistControlCommand::Start_Print:
        if (!screen.getSDCardPrintFilename()[0]) {
          screen.angryBeeps(2);
          return;
        }

        #if HAS_FILAMENT_SENSOR
          if (ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE) {
            screen.triggerTempScreenChange(DGUS_ScreenID::FILAMENTCHECK, DGUS_ScreenID::HOME);
            return;
          }
        #endif

        ExtUI::printFile(screen.getSDCardPrintFilename());
        screen.triggerScreenChange(DGUS_ScreenID::PAUSE);
        return;
    #endif

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
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_FilelistControl: unknown id ", (uint16_t)control);
      #endif
      return;
  }

  screen.triggerScreenChange(DGUS_PAGE_TO_SCREEN(newPage));
}

// 2201
void DGUSReturnKeyCodeHandler::Command_LaserControl(DGUS_VP &vp, void *data) {
  DGUS_Data::LaserControlCommand control = Endianness::fromBE_P<DGUS_Data::LaserControlCommand>(data);

  switch (control) {
    case DGUS_Data::LaserControlCommand::Mode_FDM:
      screen.triggerScreenChange(DGUS_ScreenID::SW_FDM_TIPS);
      break;
    case DGUS_Data::LaserControlCommand::Mode_FDM_Confirm:
      screen.triggerScreenChange(DGUS_ScreenID::SW_FDM_TIPS);
      break;

    #if HAS_CUTTER
      case DGUS_Data::LaserControlCommand::Mode_Cutter:
        screen.triggerScreenChange(DGUS_ScreenID::SW_LASER_TIPS);
      break;

      case DGUS_Data::LaserControlCommand::Mode_Cutter_Confirm:
        screen.triggerScreenChange(DGUS_ScreenID::SW_LASER_TIPS);
      break;
    #endif

    case DGUS_Data::LaserControlCommand::Mode_Change:
      #if HAS_CUTTER
        screen.triggerScreenChange(DGUS_ScreenID::LASER_FDM);
      #else
        screen.angryBeeps(2);
      #endif

    default:
      #if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
        DEBUG_ECHOLNPGM("Command_LaserControl: unknown id ", (uint16_t)control);
      #endif
      return;
  }
}

#endif // DGUS_LCD_UI_E3S1PRO
