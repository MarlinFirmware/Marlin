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

#if ENABLED(DGUS_LCD_UI_E3V2_TOUCH)

#include "DGUSReturnKeyCodeHandler.h"

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"
#include "DGUSSDCardHandler.h"
#include "config/DGUS_Screen.h"

#include "../ui_api.h"
#include "../../../module/stepper.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

/**
 * Many buttons on the stock screen change page by themselves as well as
 * sending a key. Each handler moves to the same page so the screen handler
 * keeps refreshing the page that is actually showing.
 */

//#define DGUS_UNKNOWN_COMMAND_DEBUG // uncomment to debug unknown commands

#if ALL(DEBUG_DGUSLCD, DGUS_UNKNOWN_COMMAND_DEBUG)
  #define UNKNOWN_COMMAND(NAME, V) DEBUG_ECHOLNPGM(NAME ": unknown id ", (uint16_t)V)
#else
  #define UNKNOWN_COMMAND(NAME, V) NOOP
#endif

static bool filamentMissing() {
  #if HAS_FILAMENT_SENSOR
    return ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE;
  #else
    return false;
  #endif
}

static void preheat(const uint16_t hotend, const uint16_t bed, const uint16_t fan) {
  ExtUI::setTargetTemp_celsius(hotend, ExtUI::H0);
  ExtUI::setTargetTemp_celsius(bed, ExtUI::BED);
  TERN_(HAS_FAN, ExtUI::setTargetFan_percent(ui8_to_percent(fan), ExtUI::FAN0));
}

// The stock firmware limits the offset to ±10mm
#if HAS_BED_PROBE
  #define DGUS_ZOFFSET_MIN PROBE_OFFSET_ZMIN
  #define DGUS_ZOFFSET_MAX PROBE_OFFSET_ZMAX
#else
  #define DGUS_ZOFFSET_MIN -10
  #define DGUS_ZOFFSET_MAX  10
#endif

static void adjustZOffset(const float delta) {
  const float zoffset = constrain(ExtUI::getZOffset_mm() + delta, DGUS_ZOFFSET_MIN, DGUS_ZOFFSET_MAX);
  #if HAS_BED_PROBE
    ExtUI::babystepAxis_steps(ExtUI::getAxisSteps_per_mm(ExtUI::Z) * (zoffset - ExtUI::getZOffset_mm()), ExtUI::Z);
  #endif
  ExtUI::setZOffset_mm(zoffset);
  screen.triggerEEPROMSave();
  screen.refreshVP(DGUS_Addr::MAIN_ZOffset);
}

// 1002
void DGUSReturnKeyCodeHandler::Command_MenuSelect(DGUS_VP &vp, void *data) {
  const DGUS_Data::MenuSelectCommand command = Endianness::fromBE_P<DGUS_Data::MenuSelectCommand>(data);

  switch (command) {
    case DGUS_Data::MenuSelectCommand::Print:
      dgus_sdcard_handler.reset();
      screen.triggerScreenChange(DGUS_ScreenID::FILE1);
      break;

    case DGUS_Data::MenuSelectCommand::Prepare:
      screen.triggerScreenChange(DGUS_ScreenID::PREPARE);
      break;

    case DGUS_Data::MenuSelectCommand::Control:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL);
      break;

    case DGUS_Data::MenuSelectCommand::Level:
      screen.triggerScreenChange(TERN(HAS_BED_PROBE, DGUS_ScreenID::LEVELINGMODE, DGUS_ScreenID::NO_LEVEL));
      break;

    case DGUS_Data::MenuSelectCommand::PrintFinished:
      screen.triggerScreenChange(DGUS_ScreenID::MAIN);
      break;

    case DGUS_Data::MenuSelectCommand::StartAutoLevel:
      #if HAS_BED_PROBE
        screen.triggerScreenChange(DGUS_ScreenID::LEVELING);
        ExtUI::injectCommands(F("G28\nM420 S0\n" TERN(AUTO_BED_LEVELING_UBL, "G29 P1", "G29")));
      #else
        screen.triggerScreenChange(DGUS_ScreenID::NO_LEVEL);
      #endif
      break;

    default: UNKNOWN_COMMAND("Command_MenuSelect", command); break;
  }
}

// 1004
void DGUSReturnKeyCodeHandler::Command_Adjust(DGUS_VP &vp, void *data) {
  const DGUS_Data::AdjustCommand command = Endianness::fromBE_P<DGUS_Data::AdjustCommand>(data);

  switch (command) {
    case DGUS_Data::AdjustCommand::Show_Adjust:
      screen.triggerScreenChange(DGUS_ScreenID::ADJUST);
      break;

    case DGUS_Data::AdjustCommand::Exit_Adjust:
      screen.triggerScreenChange(screen.getPrintScreen());
      break;

    case DGUS_Data::AdjustCommand::Toggle_Fan:
      #if HAS_FAN
        ExtUI::setTargetFan_percent(ExtUI::getTargetFan_percent(ExtUI::FAN0) ? 0 : 100, ExtUI::FAN0);
        screen.refreshVP(DGUS_Addr::ADJUST_Icon_Fan);
      #endif
      break;

    default: UNKNOWN_COMMAND("Command_Adjust", command); break;
  }
}

// 1008
void DGUSReturnKeyCodeHandler::Command_Stop(DGUS_VP &vp, void *data) {
  const DGUS_Data::StopCommand command = Endianness::fromBE_P<DGUS_Data::StopCommand>(data);

  switch (command) {
    case DGUS_Data::StopCommand::Show_Confirm:
      screen.triggerScreenChange(DGUS_ScreenID::STOP_CONFIRM);
      break;

    case DGUS_Data::StopCommand::Confirm:
      screen.stopPrint();
      break;

    default: UNKNOWN_COMMAND("Command_Stop", command); break;
  }
}

// 100A
void DGUSReturnKeyCodeHandler::Command_Pause(DGUS_VP &vp, void *data) {
  const DGUS_Data::PauseCommand command = Endianness::fromBE_P<DGUS_Data::PauseCommand>(data);

  switch (command) {
    case DGUS_Data::PauseCommand::Show_Confirm:
      screen.triggerScreenChange(DGUS_ScreenID::PAUSE_CONFIRM);
      break;

    case DGUS_Data::PauseCommand::Confirm:
      ExtUI::pausePrint();
      screen.triggerScreenChange(DGUS_ScreenID::PAUSED);
      break;

    case DGUS_Data::PauseCommand::Cancel:
      screen.triggerScreenChange(screen.getPrintScreen());
      break;

    default: UNKNOWN_COMMAND("Command_Pause", command); break;
  }
}

// 100C
void DGUSReturnKeyCodeHandler::Command_Resume(DGUS_VP &vp, void *data) {
  const DGUS_Data::ResumeCommand command = Endianness::fromBE_P<DGUS_Data::ResumeCommand>(data);

  switch (command) {
    case DGUS_Data::ResumeCommand::Resume:
      if (filamentMissing()) {
        screen.triggerScreenChange(DGUS_ScreenID::FILAMENT_RUNOUT);
        break;
      }
      TERN_(HAS_FILAMENT_SENSOR, ExtUI::setFilamentRunoutState(false));
      ExtUI::resumePrint();
      screen.triggerScreenChange(DGUS_ScreenID::PRINTING);
      break;

    case DGUS_Data::ResumeCommand::Reheat:
      if (ExtUI::getTargetTemp_celsius(ExtUI::H0) < EXTRUDE_MINTEMP)
        ExtUI::setTargetTemp_celsius(EXTRUDE_MINTEMP, ExtUI::H0);
      if (!filamentMissing()) screen.triggerScreenChange(DGUS_ScreenID::FILAMENT_INSERT);
      break;

    default: UNKNOWN_COMMAND("Command_Resume", command); break;
  }
}

// 1030
void DGUSReturnKeyCodeHandler::Command_TemperatureMenu(DGUS_VP &vp, void *data) {
  const DGUS_Data::TemperatureMenuCommand command = Endianness::fromBE_P<DGUS_Data::TemperatureMenuCommand>(data);

  switch (command) {
    case DGUS_Data::TemperatureMenuCommand::Show_Temp:
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    case DGUS_Data::TemperatureMenuCommand::Show_PLA:
      screen.triggerScreenChange(DGUS_ScreenID::PLA_TEMP);
      break;

    case DGUS_Data::TemperatureMenuCommand::Show_ABS:
      screen.triggerScreenChange(DGUS_ScreenID::ABS_TEMP);
      break;

    case DGUS_Data::TemperatureMenuCommand::Preheat_PLA:
      preheat(screen.config.plaExtruderTemp, screen.config.plaBedTemp, screen.config.plaFanSpeed);
      break;

    case DGUS_Data::TemperatureMenuCommand::Preheat_ABS:
      preheat(screen.config.absExtruderTemp, screen.config.absBedTemp, screen.config.absFanSpeed);
      break;

    case DGUS_Data::TemperatureMenuCommand::Show_Control:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL);
      break;

    default: UNKNOWN_COMMAND("Command_TemperatureMenu", command); break;
  }
}

// 1032
void DGUSReturnKeyCodeHandler::Command_Cooldown(DGUS_VP &vp, void *data) {
  const DGUS_Data::CooldownCommand command = Endianness::fromBE_P<DGUS_Data::CooldownCommand>(data);

  switch (command) {
    case DGUS_Data::CooldownCommand::Cooldown:
      ExtUI::coolDown();
      break;

    case DGUS_Data::CooldownCommand::Show_Temp:
      screen.triggerScreenChange(DGUS_ScreenID::TEMP);
      break;

    default: UNKNOWN_COMMAND("Command_Cooldown", command); break;
  }
}

// 103E
void DGUSReturnKeyCodeHandler::Command_ControlMenu(DGUS_VP &vp, void *data) {
  const DGUS_Data::ControlMenuCommand command = Endianness::fromBE_P<DGUS_Data::ControlMenuCommand>(data);

  switch (command) {
    case DGUS_Data::ControlMenuCommand::Show_MoveAxis:
      if (ExtUI::isPositionKnown())
        screen.triggerScreenChange(DGUS_ScreenID::MOVEAXIS_10);
      else
        screen.homeThenChangeScreen(DGUS_ScreenID::MOVEAXIS_10);
      break;

    case DGUS_Data::ControlMenuCommand::Show_Info:
      screen.triggerScreenChange(DGUS_ScreenID::INFORMATION);
      break;

    case DGUS_Data::ControlMenuCommand::Disable_Steppers:
      stepper.disable_all_steppers();
      break;

    case DGUS_Data::ControlMenuCommand::Reset_Settings:
      ExtUI::injectCommands(F("M502\nM500"));
      break;

    case DGUS_Data::ControlMenuCommand::Save_Presets:
      screen.triggerEEPROMSave();
      break;

    case DGUS_Data::ControlMenuCommand::Show_Main:
      screen.triggerScreenChange(DGUS_ScreenID::MAIN);
      break;

    default: UNKNOWN_COMMAND("Command_ControlMenu", command); break;
  }
}

// 1044
void DGUSReturnKeyCodeHandler::Command_Leveling(DGUS_VP &vp, void *data) {
  const DGUS_Data::LevelingCommand command = Endianness::fromBE_P<DGUS_Data::LevelingCommand>(data);

  switch (command) {
    case DGUS_Data::LevelingCommand::Home:
      screen.homeThenChangeScreen(DGUS_ScreenID::LEVELINGMODE);
      break;

    case DGUS_Data::LevelingCommand::ZOffset_Up:
      adjustZOffset(0.05f);
      break;

    case DGUS_Data::LevelingCommand::ZOffset_Down:
      adjustZOffset(-0.05f);
      break;

    case DGUS_Data::LevelingCommand::Exit_Leveling:
      screen.triggerScreenChange(DGUS_ScreenID::LEVELINGMODE);
      break;

    default: UNKNOWN_COMMAND("Command_Leveling", command); break;
  }
}

// 1046
void DGUSReturnKeyCodeHandler::Command_AxisControl(DGUS_VP &vp, void *data) {
  const DGUS_Data::AxisControlCommand command = Endianness::fromBE_P<DGUS_Data::AxisControlCommand>(data);

  switch (command) {
    case DGUS_Data::AxisControlCommand::Jog_10mm:
      screen.triggerScreenChange(DGUS_ScreenID::MOVEAXIS_10);
      break;

    case DGUS_Data::AxisControlCommand::Jog_1mm:
      screen.triggerScreenChange(DGUS_ScreenID::MOVEAXIS_1);
      break;

    case DGUS_Data::AxisControlCommand::Jog_0_1mm:
      screen.triggerScreenChange(DGUS_ScreenID::MOVEAXIS_01);
      break;

    case DGUS_Data::AxisControlCommand::Home:
      screen.homeThenChangeScreen(screen.getCurrentScreen());
      break;

    default: UNKNOWN_COMMAND("Command_AxisControl", command); break;
  }
}

// 1056
void DGUSReturnKeyCodeHandler::Command_FilamentIO(DGUS_VP &vp, void *data) {
  const DGUS_Data::FilamentIoCommand command = Endianness::fromBE_P<DGUS_Data::FilamentIoCommand>(data);

  switch (command) {
    case DGUS_Data::FilamentIoCommand::Retract:
    case DGUS_Data::FilamentIoCommand::Extrude: {
      // Like the stock firmware, heat up first if the hotend is cold
      if (ExtUI::getTargetTemp_celsius(ExtUI::H0) < EXTRUDE_MINTEMP)
        ExtUI::setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, ExtUI::H0);
      if (ExtUI::getActualTemp_celsius(ExtUI::H0) < EXTRUDE_MINTEMP) {
        screen.angryBeeps(2);
        break;
      }
      const float length = command == DGUS_Data::FilamentIoCommand::Extrude ? screen.filamentLength : -screen.filamentLength;
      ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(ExtUI::E0) + length, ExtUI::E0);
    } break;

    case DGUS_Data::FilamentIoCommand::Show_Prepare:
      screen.triggerScreenChange(DGUS_ScreenID::PREPARE);
      break;

    case DGUS_Data::FilamentIoCommand::Show_FeedReturn:
      screen.triggerScreenChange(DGUS_ScreenID::FEEDRETURN);
      break;

    default: UNKNOWN_COMMAND("Command_FilamentIO", command); break;
  }
}

// 105C
void DGUSReturnKeyCodeHandler::Command_LanguageMenu(DGUS_VP &vp, void *data) {
  const DGUS_Data::LanguageMenuCommand command = Endianness::fromBE_P<DGUS_Data::LanguageMenuCommand>(data);

  switch (command) {
    case DGUS_Data::LanguageMenuCommand::Show_Language:
      screen.triggerScreenChange(DGUS_ScreenID::LANGUAGE);
      break;

    case DGUS_Data::LanguageMenuCommand::Show_Control:
      screen.triggerScreenChange(DGUS_ScreenID::CONTROL);
      break;

    default: UNKNOWN_COMMAND("Command_LanguageMenu", command); break;
  }
}

// 105F
void DGUSReturnKeyCodeHandler::Command_PowerLoss(DGUS_VP &vp, void *data) {
  const DGUS_Data::PowerLossCommand command = Endianness::fromBE_P<DGUS_Data::PowerLossCommand>(data);

  switch (command) {
    #if ENABLED(POWER_LOSS_RECOVERY)
      case DGUS_Data::PowerLossCommand::PowerLoss_Continue:
        if (!recovery.valid()) {
          screen.angryBeeps(2);
          screen.triggerScreenChange(DGUS_ScreenID::MAIN);
          break;
        }
        screen.triggerScreenChange(DGUS_ScreenID::PRINTING);
        ExtUI::injectCommands(F("M1000"));
        break;

      case DGUS_Data::PowerLossCommand::PowerLoss_No:
        screen.triggerScreenChange(DGUS_ScreenID::MAIN);
        ExtUI::injectCommands(F("M1000 C"));
        break;
    #endif

    default: UNKNOWN_COMMAND("Command_PowerLoss", command); break;
  }
}

// 111A
void DGUSReturnKeyCodeHandler::Command_Acknowledge(DGUS_VP &vp, void *data) {
  const DGUS_Data::AcknowledgeCommand command = Endianness::fromBE_P<DGUS_Data::AcknowledgeCommand>(data);

  switch (command) {
    case DGUS_Data::AcknowledgeCommand::Abnormal_OK:
      if (screen.isOnUserConfirmationScreen())
        screen.userConfirmation();
      else
        screen.triggerScreenChange(ExtUI::isPrinting() ? screen.getPrintScreen() : DGUS_ScreenID::MAIN);
      break;

    case DGUS_Data::AcknowledgeCommand::NoLevel_OK:
      screen.triggerScreenChange(DGUS_ScreenID::MAIN);
      break;

    default: UNKNOWN_COMMAND("Command_Acknowledge", command); break;
  }
}

// 20D2
void DGUSReturnKeyCodeHandler::Command_FilelistControl(DGUS_VP &vp, void *data) {
  const DGUS_Data::FilelistControlCommand command = Endianness::fromBE_P<DGUS_Data::FilelistControlCommand>(data);
  const DGUS_ScreenID current = screen.getCurrentScreen();

  switch (command) {
    case DGUS_Data::FilelistControlCommand::Start_Print: {
      const char * const filename = dgus_sdcard_handler.selectedShortName();
      if (!filename) {
        screen.angryBeeps(2);
        break;
      }
      if (filamentMissing()) {
        screen.triggerScreenChange(DGUS_ScreenID::FILAMENT_INSERT);
        break;
      }
      ExtUI::printFile(filename);
      screen.triggerScreenChange(DGUS_ScreenID::PRINTING);
    } break;

    case DGUS_Data::FilelistControlCommand::Prev_Page:
      if (current > DGUS_ScreenID::FILE1 && current <= DGUS_ScreenID::FILE5)
        screen.triggerScreenChange((DGUS_ScreenID)((uint8_t)current - 1));
      break;

    case DGUS_Data::FilelistControlCommand::Next_Page:
      if (current >= DGUS_ScreenID::FILE1 && current < DGUS_ScreenID::FILE5)
        screen.triggerScreenChange((DGUS_ScreenID)((uint8_t)current + 1));
      break;

    case DGUS_Data::FilelistControlCommand::Show_Main:
    default:
      screen.triggerScreenChange(DGUS_ScreenID::MAIN);
      break;
  }
}

// 20D3
void DGUSReturnKeyCodeHandler::Command_FileSelect(DGUS_VP &vp, void *data) {
  const uint16_t file = Endianness::fromBE_P<uint16_t>(data);
  if (!file || !dgus_sdcard_handler.select(file - 1)) return;
  screen.triggerFullUpdate();
}

#endif // DGUS_LCD_UI_E3V2_TOUCH
