/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "DGUSDisplayDef.h"
#include "../DGUSDisplay.h"
#include "../DGUSScreenHandler.h"
#include "PageHandlers.h"
#include "FilamentLoadUnloadHandler.h"

#include "../../../../module/temperature.h"
#include "../../../../module/settings.h"
#include "../../../../sd/cardreader.h"

#if HAS_FILAMENT_SENSOR
  #include "../../../../feature/runout.h"
#endif

#include "../../../marlinui.h" // for material presets
#include "../../ui_api.h"

// Definitions of page handlers

void MainMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_MAINENTERKEY:
      switch (buttonValue) {
        case 1:
          #if HAS_MEDIA
            // Try to mount an unmounted card (BTT SKR board has especially some trouble sometimes)
            card.mount();
            screen.sdCardInserted();
          #endif
          break;

        case 2: screen.gotoScreen(DGUS_SCREEN_PREPARE); break;
        case 3: screen.gotoScreen(DGUS_SCREEN_SETUP); break;
        case 4: screen.gotoScreen(DGUS_SCREEN_CALIBRATE); break;
      }
      break;
  }
}

void SetupMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_PREPAREENTERKEY:
      switch (buttonValue) {
        case 5: // About
          screen.gotoScreen(DGUS_SCREEN_INFO);
          break;

        case 7: // Reset to factory settings
          settings.reset();
          settings.save();

          ExtUI::injectCommands(F("M300"));

          screen.gotoScreen(DGUS_SCREEN_MAIN, true);
          screen.setStatusMessage(F("Restored default settings. Please turn your printer off and then on to complete the reset"));
          break;
      }
      break;

    case VP_BUTTON_TEMPCONTROL:
      if (buttonValue == 2) screen.gotoScreen(DGUS_SCREEN_TEMP);
      break;

    case VP_BUTTON_ADJUSTENTERKEY:
      screen.handleLEDToggle();
      break;
  }
}

void LevelingModeHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_BEDLEVELKEY:
      switch (buttonValue) {
        case 1:
          ExtUI::injectCommands(F("G28\nG0 Z0"));
          break;

        case 2 ... 3: {
          // Increase / Decrease Z-offset
          const int16_t steps = ExtUI::mmToWholeSteps(0.01f, ExtUI::axis_t::Z);
          ExtUI::smartAdjustAxis_steps(buttonValue == 2 ? steps : -steps, ExtUI::axis_t::Z, true);
          screen.forceCompleteUpdate();
          screen.requestSaveSettings();
        } break;
      }
      break;

    case VP_BUTTON_PREPAREENTERKEY:
      if (buttonValue == 9) {
        #if DISABLED(HOTEND_IDLE_TIMEOUT)
          thermalManager.disable_all_heaters();
        #endif
        screen.gotoScreen(DGUS_SCREEN_MAIN, true);
      }
      #if HAS_MESH
        if (buttonValue == 1) {
          // View the leveling mesh
          screen.setViewMeshLevelState();
          screen.initMeshValues();
          screen.gotoScreen(DGUS_SCREEN_LEVELING);
        }
      #endif
      break;

    case VP_BUTTON_MAINENTERKEY:
      // Go to leveling screen
      ExtUI::injectCommands(F("G28\nG29"));
      TERN_(HAS_MESH, screen.resetMeshValues());
      dgus.writeVariable(VP_MESH_SCREEN_MESSAGE_ICON, MESH_SCREEN_MESSAGE_ICON_LEVELING);
      screen.gotoScreen(DGUS_SCREEN_LEVELING);
      break;
  }
}

void LevelingHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_BEDLEVELKEY:
      if (!screen.hasCurrentSynchronousOperation())
        screen.popToOldScreen();
      else
        screen.setStatusMessage(F("Wait for leveling completion..."));
      break;
  }
}

void TempMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_ADJUSTENTERKEY:
      switch (buttonValue) {
        case 3: screen.handleFanToggle(); break;
      }
      break;

    case VP_BUTTON_TEMPCONTROL:
      switch (buttonValue) {
        case 3: screen.gotoScreen(DGUS_SCREEN_TEMP_PLA); break;
        case 4: screen.gotoScreen(DGUS_SCREEN_TEMP_ABS); break;
      }
      break;
  }
}

void PrepareMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_PREPAREENTERKEY:
      switch (buttonValue) {
        case 3:
          screen.gotoScreen(DGUS_SCREEN_MOVE10MM);
          break;

        case 6: // Disable steppers
          screen.handleMotorLockUnlock(var, &buttonValue);
          break;
      }
      break;

    case VP_BUTTON_HEATLOADSTARTKEY:
      screen.gotoScreen(DGUS_SCREEN_FEED);
      TERN_(HAS_EXTRUDERS, FilamentLoadUnloadHandler::init());
      break;

    case VP_BUTTON_COOLDOWN:
      screen.handleAllHeatersOff(var, &buttonValue);
      break;

    case VP_BUTTON_TEMPCONTROL:
      #if PREHEAT_COUNT
        switch (buttonValue) {
          case 5:
            thermalManager.setTargetHotend(ui.material_preset[0].hotend_temp, 0);
            thermalManager.setTargetBed(ui.material_preset[0].bed_temp);
            break;
          case 6:
            thermalManager.setTargetHotend(ui.material_preset[1].hotend_temp, 0);
            thermalManager.setTargetBed(ui.material_preset[1].bed_temp);
            break;
        }
      #endif
      break;
  }

  screen.forceCompleteUpdate();
}

void TuneMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_ADJUSTENTERKEY:
      switch (buttonValue) {
        case 2: screen.gotoScreen(ExtUI::isPrintingPaused() ? DGUS_SCREEN_PRINT_PAUSED : DGUS_SCREEN_PRINT_RUNNING, true); break;
        case 3: screen.handleFanToggle(); break;
        case 4: screen.handleLEDToggle(); break;
      }
      break;
  }
}

void PrintRunningMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_ADJUSTENTERKEY: screen.gotoScreen(DGUS_SCREEN_TUNING); break;
    case VP_BUTTON_PAUSEPRINTKEY:  screen.gotoScreen(DGUS_SCREEN_DIALOG_PAUSE); break;
    case VP_BUTTON_STOPPRINTKEY:   screen.gotoScreen(DGUS_SCREEN_DIALOG_STOP); break;
  }
}

void PrintPausedMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_RESUMEPRINTKEY:
      TERN_(HAS_FILAMENT_SENSOR, runout.reset());

      if (!screen.handlePendingUserConfirmation()) {
        ExtUI::resumePrint();
        screen.gotoScreen(DGUS_SCREEN_PRINT_RUNNING);
      }
      break;

    case VP_BUTTON_ADJUSTENTERKEY: screen.gotoScreen(DGUS_SCREEN_TUNING); break;
    case VP_BUTTON_STOPPRINTKEY:   screen.gotoScreen(DGUS_SCREEN_DIALOG_STOP); break;
  }
}

void PrintPauseDialogHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_PAUSEPRINTKEY:
      switch (buttonValue) {
        case 2:
          screen.gotoScreen(DGUS_SCREEN_PRINT_RUNNING);
          screen.setStatusMessage(F("Pausing print - please wait..."));
          ExtUI::pausePrint();
          break;

        case 3:
          screen.gotoScreen(DGUS_SCREEN_PRINT_RUNNING);
          break;
      }
      break;
  }
}

void PrintFinishMenuHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_MAINENTERKEY:
      if (buttonValue == 5) screen.gotoScreen(DGUS_SCREEN_MAIN);
      break;
  }
}

void FilamentRunoutHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_RESUMEPRINTKEY:
      ExtUI::resumePrint();
      screen.gotoScreen(DGUS_SCREEN_PRINT_RUNNING);
      break;

    case VP_BUTTON_STOPPRINTKEY:
      ExtUI::stopPrint();
      screen.gotoScreen(DGUS_SCREEN_MAIN);
      break;
  }
}

void StopConfirmScreenHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_STOPPRINTKEY:
      switch (buttonValue) {
        case 2:
          ExtUI::stopPrint();
          screen.gotoScreen(DGUS_SCREEN_MAIN);
          break;

        case 3:
          screen.gotoScreen(ExtUI::isPrintingPaused() ? DGUS_SCREEN_PRINT_PAUSED : DGUS_SCREEN_PRINT_RUNNING);
          break;
      }
      break;
  }
}

void PreheatSettingsScreenHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  switch (var.VP) {
    case VP_BUTTON_PREPAREENTERKEY:
      // Save button, save settings and go back
      screen.requestSaveSettings();
      screen.popToOldScreen();
      break;

    case VP_BUTTON_COOLDOWN: // You can't make this up
      // Back button, discard settings
      settings.load();
      screen.popToOldScreen();
      break;
  }
}

void MoveHandler(DGUS_VP_Variable &var, unsigned short buttonValue) {
  if (var.VP != VP_BUTTON_MOVEKEY) return;
  switch (buttonValue) {
    case 1: screen.gotoScreen(DGUS_SCREEN_MOVE10MM, true); break;
    case 2: screen.gotoScreen(DGUS_SCREEN_MOVE1MM, true); break;
    case 3: screen.gotoScreen(DGUS_SCREEN_MOVE01MM, true); break;
    case 4: ExtUI::injectCommands(F("G28")); break;
  }
}

// Register the page handlers
#define PAGE_HANDLER(SCRID, HDLRPTR) { .ScreenID = SCRID, .Handler = HDLRPTR },

const struct PageHandler PageHandlers[] PROGMEM = {
  PAGE_HANDLER(DGUS_SCREEN_MAIN, MainMenuHandler)

  PAGE_HANDLER(DGUS_SCREEN_SETUP, SetupMenuHandler)

  PAGE_HANDLER(DGUS_SCREEN_ZOFFSET_LEVEL, LevelingModeHandler)
  PAGE_HANDLER(DGUS_SCREEN_LEVELING, LevelingHandler)

  PAGE_HANDLER(DGUS_SCREEN_TEMP, TempMenuHandler)
  PAGE_HANDLER(DGUS_SCREEN_TEMP_PLA, PreheatSettingsScreenHandler)
  PAGE_HANDLER(DGUS_SCREEN_TEMP_ABS, PreheatSettingsScreenHandler)

  PAGE_HANDLER(DGUS_SCREEN_TUNING, TuneMenuHandler)
  PAGE_HANDLER(DGUS_SCREEN_MOVE01MM, MoveHandler)
  PAGE_HANDLER(DGUS_SCREEN_MOVE1MM, MoveHandler)
  PAGE_HANDLER(DGUS_SCREEN_MOVE10MM, MoveHandler)

  PAGE_HANDLER(DGUS_SCREEN_FILAMENTRUNOUT1, FilamentRunoutHandler)
  PAGE_HANDLER(DGUS_SCREEN_FILAMENTRUNOUT2, FilamentRunoutHandler)

  PAGE_HANDLER(DGUS_SCREEN_DIALOG_STOP, StopConfirmScreenHandler)

  PAGE_HANDLER(DGUS_SCREEN_PRINT_RUNNING, PrintRunningMenuHandler)
  PAGE_HANDLER(DGUS_SCREEN_PRINT_PAUSED, PrintPausedMenuHandler)
  PAGE_HANDLER(DGUS_SCREEN_PRINT_FINISH, PrintFinishMenuHandler)

  PAGE_HANDLER(DGUS_SCREEN_DIALOG_PAUSE, PrintPauseDialogHandler)

  PAGE_HANDLER(DGUS_SCREEN_PREPARE, PrepareMenuHandler)

  // Terminating
  PAGE_HANDLER(static_cast<DGUS_ScreenID>(0), 0)
};

void DGUSCrealityDisplay_HandleReturnKeyEvent(DGUS_VP_Variable &var, void *val_ptr) {
  const struct PageHandler *map = PageHandlers;
  const DGUS_ScreenID current_screenID = screen.getCurrentScreen();

  while (pgm_read_ptr(&map->Handler)) {
    if (DGUS_ScreenID(pgm_read_byte(&map->ScreenID)) == current_screenID) {
      const unsigned short button_value = BE16_P(val_ptr);

      DEBUG_ECHOLNPGM("Invoking handler for screen ", current_screenID, " with VP=", var.VP, " value=", button_value);

      const DGUS_CREALITY_SCREEN_BUTTON_HANDLER handler = (DGUS_CREALITY_SCREEN_BUTTON_HANDLER)pgm_read_ptr(&map->Handler);
      handler(var, button_value);
      return;
    }
    map++;
  }
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH
