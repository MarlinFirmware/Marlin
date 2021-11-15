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

#include "../../../inc/MarlinConfigPre.h"

#define DEBUG_ECHOLNPAIR DEBUG_ECHOLNPAIR

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#include "DGUSScreenHandler.h"
#include "DGUSDisplay.h"
#include "DGUSVPVariable.h"
#include "DGUSDisplayDef.h"

#include "../ui_api.h"
#include "../../../MarlinCore.h"
#include "../../../module/temperature.h"
#include "../../../module/motion.h"
#include "../../../module/settings.h"
#include "../../../gcode/queue.h"
#include "../../../module/planner.h"
#include "../../../sd/cardreader.h"
#include "../../../libs/duration_t.h"
#include "../../../module/printcounter.h"
#include "../../../feature/caselight.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#if HAS_COLOR_LEDS
  #include "../../../feature/leds/leds.h"
#endif

uint16_t DGUSScreenHandler::ConfirmVP;

#if ENABLED(SDSUPPORT)
  int16_t DGUSScreenHandler::top_file = 0;
  int16_t DGUSScreenHandler::file_to_print = 0;
  static ExtUI::FileList filelist;
#endif

// Storage initialization
constexpr uint8_t dwin_settings_version = 6; // Increase when properties are added or removed
creality_dwin_settings_t DGUSScreenHandler::Settings = {.settings_size = sizeof(creality_dwin_settings_t)};
DGUSLCD_Screens DGUSScreenHandler::current_screen;
DGUSLCD_Screens DGUSScreenHandler::past_screens[NUM_PAST_SCREENS] = {DGUSLCD_SCREEN_MAIN};
uint8_t DGUSScreenHandler::update_ptr;
uint16_t DGUSScreenHandler::skipVP;
bool DGUSScreenHandler::ScreenComplete;
bool DGUSScreenHandler::SaveSettingsRequested;
bool DGUSScreenHandler::HasSynchronousOperation;
bool DGUSScreenHandler::HasScreenVersionMismatch;
uint8_t DGUSScreenHandler::MeshLevelIndex = -1;
uint8_t DGUSScreenHandler::MeshLevelIconIndex = -1;
bool DGUSScreenHandler::fwretract_available = TERN(FWRETRACT,  true, false);
bool DGUSScreenHandler::HasRGBSettings = TERN(HAS_COLOR_LEDS, true, false);

static_assert(GRID_MAX_POINTS_X == GRID_MAX_POINTS_Y, "Assuming bed leveling points is square");

constexpr uint16_t SkipMeshPoint = GRID_MAX_POINTS_X > MESH_LEVEL_EDGE_MAX_POINTS ? ((GRID_MAX_POINTS_X - 1) / (GRID_MAX_POINTS_X - MESH_LEVEL_EDGE_MAX_POINTS)) : 1;

void DGUSScreenHandler::sendinfoscreen(const char* line1, const char* line2, const char* line3, const char* line4, bool l1inflash, bool l2inflash, bool l3inflash, bool l4inflash) {
  DGUS_VP_Variable ramcopy;
  if (populate_VPVar(VP_MSGSTR1, &ramcopy)) {
    ramcopy.memadr = (void*) line1;
    l1inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR2, &ramcopy)) {
    ramcopy.memadr = (void*) line2;
    l2inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  if (populate_VPVar(VP_MSGSTR3, &ramcopy)) {
    ramcopy.memadr = (void*) line3;
    l3inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }
  //if (populate_VPVar(VP_MSGSTR4, &ramcopy)) {
  //  ramcopy.memadr = (void*) line4;
  //  l4inflash ? DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(ramcopy) : DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  //}
}


void DGUSScreenHandler::Init() {
  dgusdisplay.InitDisplay();
}

void DGUSScreenHandler::RequestSaveSettings() {
  SaveSettingsRequested = true;
}

void DGUSScreenHandler::DefaultSettings() {
  Settings.settings_size = sizeof(creality_dwin_settings_t);
  Settings.settings_version = dwin_settings_version;

  Settings.led_state = false;

  Settings.display_standby = true;
  Settings.display_sound = true;

  Settings.standby_screen_brightness = 10;
  Settings.screen_brightness = 100;
  Settings.standby_time_seconds = 60;
  
  #if ENABLED(LED_COLOR_PRESETS)
  Settings.LastLEDColor = LEDLights::defaultLEDColor;
  #endif
}

void DGUSScreenHandler::LoadSettings(const char* buff) {
  static_assert(
    ExtUI::eeprom_data_size >= sizeof(creality_dwin_settings_t),
    "Insufficient space in EEPROM for UI parameters"
  );

  creality_dwin_settings_t eepromSettings;
  memcpy(&eepromSettings, buff, sizeof(creality_dwin_settings_t));

  // If size is not the same, discard settings
  if (eepromSettings.settings_size != sizeof(creality_dwin_settings_t)) {
    SERIAL_ECHOLNPGM("Discarding DWIN LCD setting from EEPROM - size incorrect");

    ScreenHandler.DefaultSettings();
    return;
  } 

  if (eepromSettings.settings_version != dwin_settings_version) {
    SERIAL_ECHOLNPGM("Discarding DWIN LCD setting from EEPROM - settings version incorrect");

    ScreenHandler.DefaultSettings();
    return;
  }
  
  // Copy into final location
  SERIAL_ECHOLNPGM("Loading DWIN LCD setting from EEPROM");
  memcpy(&Settings, &eepromSettings, sizeof(creality_dwin_settings_t));

  // Apply settings
  caselight.on = Settings.led_state;
  caselight.update(Settings.led_state);

  #if HAS_COLOR_LEDS_PREFERENCES
  leds.set_color(Settings.LastLEDColor);
  #endif

  ScreenHandler.SetTouchScreenConfiguration();
}

void DGUSScreenHandler::StoreSettings(char* buff) {
  static_assert(
    ExtUI::eeprom_data_size >= sizeof(creality_dwin_settings_t),
    "Insufficient space in EEPROM for UI parameters"
  );

  // Update settings from Marlin state, if necessary
  Settings.led_state = caselight.on;

  #if HAS_COLOR_LEDS_PREFERENCES
  Settings.LastLEDColor = leds.color;
  #endif

  // Write to buffer
  SERIAL_ECHOLNPGM("Saving DWIN LCD setting from EEPROM");
  memcpy(buff, &Settings, sizeof(creality_dwin_settings_t));
}

void DGUSScreenHandler::SetTouchScreenConfiguration() {
  LIMIT(Settings.screen_brightness, 10, 100); // Prevent a possible all-dark screen
  LIMIT(Settings.standby_time_seconds, 10, 655); // Prevent a possible all-dark screen for standby, yet also don't go higher than the DWIN limitation

  dgusdisplay.SetTouchScreenConfiguration(Settings.display_standby, Settings.display_sound, Settings.standby_screen_brightness, Settings.screen_brightness, Settings.standby_time_seconds);
}

void DGUSScreenHandler::KillScreenCalled() {
  // If killed, always fully wake up
  dgusdisplay.SetTouchScreenConfiguration(false, true, 100, 100, 100 /*Doesn't really matter*/);

  // Hey! Something is going on!
  Buzzer(1000 /*ignored*/, 880);
}

void DGUSScreenHandler::OnPowerlossResume() {
  GotoScreen(DGUSLCD_SCREEN_POWER_LOSS);

  // Send print filename
  dgusdisplay.WriteVariable(VP_SD_Print_Filename, PrintJobRecovery::info.sd_filename, VP_SD_FileName_LEN, true);
}

void DGUSScreenHandler::HandleUserConfirmationPopUp(uint16_t VP, const char* line1, const char* line2, const char* line3, const char* line4, bool l1, bool l2, bool l3, bool l4) {
  if (current_screen == DGUSLCD_SCREEN_CONFIRM) {
    // Already showing a pop up, so we need to cancel that first.
    PopToOldScreen();
  }

  ConfirmVP = VP;
  sendinfoscreen(line1, line2, line3, line4, l1, l2, l3, l4);
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_CONFIRM);
}

void DGUSScreenHandler::HandleDevelopmentTestButton(DGUS_VP_Variable &var, void *val_ptr) {
  // Handle the button press only after 3 taps, so that a regular user won't tap it by accident
  static uint8_t tap_count = 0;

  if (++tap_count <= 3) return;

  // Get button value
  uint16_t button_value = swap16(*static_cast<uint16_t*>(val_ptr));

  // Act on it
  switch (button_value) {
    case VP_DEVELOPMENT_HELPER_BUTTON_ACTION_FIRMWARE_UPDATE:
      ExtUI::injectCommands_P(PSTR("M997"));
    break;

    case VP_DEVELOPMENT_HELPER_BUTTON_ACTION_TO_MAIN_MENU:
      setstatusmessagePGM(PSTR("Dev action: main menu"));
      GotoScreen(DGUSLCD_SCREEN_MAIN, false);
    break;

    case VP_DEVELOPMENT_HELPER_BUTTON_ACTION_RESET_DISPLAY:
      setstatusmessagePGM(PSTR("Dev action: reset DGUS"));
      dgusdisplay.ResetDisplay();
    break;

    default:
      setstatusmessagePGM(PSTR("Dev action: unknown"));
    break;
  }
}

void setStatusMessage(const char *msg, bool forceScrolling) {
  const bool needs_scrolling = forceScrolling || strlen(msg) > M117_STATIC_DISPLAY_LEN;

  DGUS_VP_Variable ramcopy;

  // Update static message to either NULL or the value
  if (populate_VPVar(VP_M117_STATIC, &ramcopy)) {
    ramcopy.memadr = (void*) (needs_scrolling ? NUL_STR : msg);
    DGUSScreenHandler::DGUSLCD_SendStringToDisplay(ramcopy);
  }

  // Update scrolling message to either NULL or the value
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) (needs_scrolling ? msg : NUL_STR);
    DGUSScreenHandler::DGUSLCD_SendScrollingStringToDisplay(ramcopy);
  }
}

void DGUSScreenHandler::setstatusmessage(const char *msg) {
  setStatusMessage(msg, false);
}

void DGUSScreenHandler::setstatusmessagePGM(PGM_P const msg) {
  const bool needs_scrolling = strlen_P(msg) > M117_STATIC_DISPLAY_LEN;

  DGUS_VP_Variable ramcopy;

   // Update static message to either NULL or the value
  if (populate_VPVar(VP_M117_STATIC, &ramcopy)) {
    ramcopy.memadr = (void*) (needs_scrolling ? nullptr : msg);
    DGUSLCD_SendStringToDisplayPGM(ramcopy);
  }
  
  // Update scrolling message to either NULL or the value
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*) (needs_scrolling ? msg : nullptr);
    DGUSLCD_SendScrollingStringToDisplayPGM(ramcopy);
  }
}

// Send an 8 bit or 16 bit value to the display.
void DGUSScreenHandler::DGUSLCD_SendWordValueToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    if (var.size > 1)
      dgusdisplay.WriteVariable(var.VP, *(int16_t*)var.memadr);
    else
      dgusdisplay.WriteVariable(var.VP, *(int8_t*)var.memadr);
  }
}

// Send an uint8_t between 0 and 255 to the display, but scale to a percentage (0..100)
void DGUSScreenHandler::DGUSLCD_SendPercentageToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    //DEBUG_ECHOPAIR(" DGUS_LCD_SendWordValueToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", *(uint16_t *)var.memadr);
    uint16_t tmp = *(uint8_t *) var.memadr +1 ; // +1 -> avoid rounding issues for the display.
    tmp = map(tmp, 0, 255, 0, 100);
    dgusdisplay.WriteVariable(var.VP, tmp);
  }
}

// Send the current print progress to the display.
void DGUSScreenHandler::DGUSLCD_SendPrintProgressToDisplay(DGUS_VP_Variable &var) {
  uint16_t tmp = ExtUI::getProgress_percent();
  dgusdisplay.WriteVariable(var.VP, tmp);
}

// Send the current print time to the display.
// It is using a hex display for that: It expects BSD coded data in the format xxyyzz
void DGUSScreenHandler::DGUSLCD_SendPrintTimeToDisplay(DGUS_VP_Variable &var) {
  // Clear if changed and we shouldn't display
  static bool last_shouldDisplay = true;
  bool shouldDisplay = ui.remaining_time == 0;
  if (last_shouldDisplay != shouldDisplay) {
    if (!shouldDisplay) {
      dgusdisplay.WriteVariable(VP_PrintTime, nullptr, var.size, true);
    }
  }

  last_shouldDisplay = shouldDisplay;
  if (!shouldDisplay) return;

  // Write if changed
  duration_t elapsed = print_job_timer.duration();

  static uint32_t last_elapsed;
  if (elapsed == last_elapsed) {
    return;
  }

  char buf[32];
  elapsed.toString(buf);
  dgusdisplay.WriteVariable(VP_PrintTime, buf, var.size, true);

  last_elapsed = elapsed.second();
}

void DGUSScreenHandler::DGUSLCD_SendPrintTimeWithRemainingToDisplay(DGUS_VP_Variable &var) {
  // Clear if changed and we shouldn't display
  static bool last_shouldDisplay = true;
  bool shouldDisplay = ui.remaining_time != 0;
  if (last_shouldDisplay != shouldDisplay) {
    if (!shouldDisplay) {
      dgusdisplay.WriteVariable(VP_PrintTimeWithRemainingVisible, nullptr, var.size, true);
    }
  }

  last_shouldDisplay = shouldDisplay;
  if (!shouldDisplay) return;

  // Write if changed
  duration_t elapsed = print_job_timer.duration();

  static uint32_t last_elapsed;
  if (elapsed == last_elapsed) {
    return;
  }

  char buf[32];
  elapsed.toString(buf);
  dgusdisplay.WriteVariable(VP_PrintTimeWithRemainingVisible, buf, var.size, true);

  last_elapsed = elapsed.second();
}

// Send the current print time to the display.
// It is using a hex display for that: It expects BSD coded data in the format xxyyzz
void DGUSScreenHandler::DGUSLCD_SendPrintTimeRemainingToDisplay(DGUS_VP_Variable &var) { 
#if ENABLED(SHOW_REMAINING_TIME)
  static uint32_t lastRemainingTime = -1;
  uint32_t remaining_time = ui.remaining_time;
  if (lastRemainingTime == remaining_time) {
    return;
  }

  bool has_remaining_time = remaining_time != 0;

  // Update display of SPs (toggle between large and small print timer)
  if (has_remaining_time) {
    dgusdisplay.WriteVariable(VP_HideRemainingTime_Ico, ICON_REMAINING_VISIBLE);
  } else {
    dgusdisplay.WriteVariable(VP_HideRemainingTime_Ico, ICON_REMAINING_HIDDEN);
  }

  if (!has_remaining_time) {
    // Clear remaining time
    dgusdisplay.WriteVariable(VP_PrintTimeRemaining, nullptr, var.size, true);
    lastRemainingTime = remaining_time;
    return;
  }

  // Send a progress update to the display if anything is different.
  // This allows custom M117 commands to override the displayed string if desired.

  // Remaining time is seconds. When Marlin accepts a M73 R[minutes] command, it multiplies
  // the R value by 60 to make a number of seconds. But... Marlin can also predict time
  // if the M73 R command has not been used. So we should be good either way.
  duration_t remaining(remaining_time);
  constexpr size_t buffer_size = 21;

  // Write the duration
  char buffer[buffer_size] = {0};
  remaining.toString(buffer);

  dgusdisplay.WriteVariable(VP_PrintTimeRemaining, buffer, var.size, true);

  lastRemainingTime = remaining_time;
#endif
}

void DGUSScreenHandler::DGUSLCD_SendAboutFirmwareWebsite(DGUS_VP_Variable &var) {
  const char* websiteUrl = PSTR(WEBSITE_URL);

  dgusdisplay.WriteVariablePGM(var.VP, websiteUrl, var.size, true);
}

void DGUSScreenHandler::DGUSLCD_SendAboutFirmwareVersion(DGUS_VP_Variable &var) {
  const char* fwVersion = PSTR(SHORT_BUILD_VERSION);

  dgusdisplay.WriteVariablePGM(var.VP, fwVersion, var.size, true);
}

void DGUSScreenHandler::DGUSLCD_SendAboutPrintSize(DGUS_VP_Variable &var) {
  char PRINTSIZE[VP_PRINTER_BEDSIZE_LEN] = {0};
  sprintf(PRINTSIZE,"%dx%dx%d", X_BED_SIZE, Y_BED_SIZE, Z_MAX_POS);

  dgusdisplay.WriteVariablePGM(var.VP, &PRINTSIZE, sizeof(PRINTSIZE), true);
}


// Send an uint8_t between 0 and 100 to a variable scale to 0..255
void DGUSScreenHandler::DGUSLCD_PercentageToUint8(DGUS_VP_Variable &var, void *val_ptr) {
  if (var.memadr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    *(uint8_t*)var.memadr = map(constrain(value, 0, 100), 0, 100, 0, 255);
  }
}

// Sends a (RAM located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenHandler::DGUSLCD_SendStringToDisplay(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariable(var.VP, tmp, var.size, true, DWIN_DEFAULT_FILLER_CHAR);
}

// Sends a (RAM located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenHandler::DGUSLCD_SendScrollingStringToDisplay(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariable(var.VP, tmp, var.size, true, DWIN_SCROLLER_FILLER_CHAR);
}

// Sends a (flash located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenHandler::DGUSLCD_SendStringToDisplayPGM(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariablePGM(var.VP, tmp, var.size, true, DWIN_DEFAULT_FILLER_CHAR);
}


// Sends a (flash located) string to the DGUS Display
// (Note: The DGUS Display does not clear after the \0, you have to
// overwrite the remainings with spaces.// var.size has the display buffer size!
void DGUSScreenHandler::DGUSLCD_SendScrollingStringToDisplayPGM(DGUS_VP_Variable &var) {
  char *tmp = (char*) var.memadr;
  dgusdisplay.WriteVariablePGM(var.VP, tmp, var.size, true, DWIN_SCROLLER_FILLER_CHAR);
}

#if HAS_PID_HEATING
  void DGUSScreenHandler::DGUSLCD_SendTemperaturePID(DGUS_VP_Variable &var) {
    float value = *(float *)var.memadr;
    float valuesend = 0;
    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_E0_PID_P: valuesend = value; break;
        case VP_E0_PID_I: valuesend = unscalePID_i(value); break;
        case VP_E0_PID_D: valuesend = unscalePID_d(value); break;
      #endif
      #if HAS_HEATED_BED
        case VP_BED_PID_P: valuesend = value; break;
        case VP_BED_PID_I: valuesend = unscalePID_i(value); break;
        case VP_BED_PID_D: valuesend = unscalePID_d(value); break;
      #endif
    }

    valuesend *= cpow(10, 1);
    union { int16_t i; char lb[2]; } endian;

    char tmp[2];
    endian.i = valuesend;
    tmp[0] = endian.lb[1];
    tmp[1] = endian.lb[0];
    dgusdisplay.WriteVariable(var.VP, tmp, 2);
  }
#endif

// Send fan status value to the display.
#if HAS_FAN
  void DGUSScreenHandler::DGUSLCD_SendFanStatusToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      DEBUG_ECHOPAIR(" DGUSLCD_SendFanStatusToDisplay ", var.VP);
      DEBUG_ECHOLNPAIR(" data ", *(uint8_t *)var.memadr);
      uint16_t data_to_send = ICON_TOGGLE_OFF;
      if (*(uint8_t *) var.memadr) data_to_send = ICON_TOGGLE_ON;
      dgusdisplay.WriteVariable(var.VP, data_to_send);
    }
  }

  void DGUSScreenHandler::DGUSLCD_SendFanSpeedToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      int16_t data_to_send = static_cast<int16_t>(round(ExtUI::getTargetFan_percent(ExtUI::fan_t::FAN0)));
      dgusdisplay.WriteVariable(var.VP, data_to_send);
    }
  }
#endif

// Send heater status value to the display.
void DGUSScreenHandler::DGUSLCD_SendHeaterStatusToDisplay(DGUS_VP_Variable &var) {
  if (var.memadr) {
    DEBUG_ECHOPAIR(" DGUSLCD_SendHeaterStatusToDisplay ", var.VP);
    DEBUG_ECHOLNPAIR(" data ", *(int16_t *)var.memadr);
    uint16_t data_to_send = 0;
    if (*(int16_t *) var.memadr) data_to_send = 1;
    dgusdisplay.WriteVariable(var.VP, data_to_send);
  }
}

#if ENABLED(DGUS_UI_WAITING)
  void DGUSScreenHandler::DGUSLCD_SendWaitingStatusToDisplay(DGUS_VP_Variable &var) {
    // In FYSETC UI design there are 10 statuses to loop
    static uint16_t period = 0;
    static uint16_t index = 0;
    //DEBUG_ECHOPAIR(" DGUSLCD_SendWaitingStatusToDisplay ", var.VP);
    //DEBUG_ECHOLNPAIR(" data ", swap16(index));
    if (period++ > DGUS_UI_WAITING_STATUS_PERIOD) {
      dgusdisplay.WriteVariable(var.VP, index);
      //DEBUG_ECHOLNPAIR(" data ", swap16(index));
      if (++index >= DGUS_UI_WAITING_STATUS) index = 0;
      period = 0;
    }
  }
#endif

#if ENABLED(SDSUPPORT)

  void DGUSScreenHandler::ScreenChangeHookIfSD(DGUS_VP_Variable &var, void *val_ptr) {
    // default action executed when there is a SD card, but not printing
    if (ExtUI::isMediaInserted() && !ExtUI::isPrintingFromMedia()) {
      ScreenChangeHook(var, val_ptr);
      GotoScreen(current_screen);
      return;
    }

    // if we are printing, we jump to two screens after the requested one.
    // This should host e.g a print pause / print abort / print resume dialog.
    // This concept allows to recycle this hook for other file
    if (ExtUI::isPrintingFromMedia() && !card.flag.abort_sd_printing) {
      GotoScreen(DGUSLCD_SCREEN_SDPRINTMANIPULATION);
      return;
    }

    // Don't let the user in the dark why there is no reaction.
    if (!ExtUI::isMediaInserted()) {
      setstatusmessagePGM(GET_TEXT(MSG_NO_MEDIA));
      return;
    }
    if (card.flag.abort_sd_printing) {
      setstatusmessagePGM(GET_TEXT(MSG_MEDIA_ABORTING));
      return;
    }
  }

  void DGUSScreenHandler::DGUSLCD_SD_ScrollFilelist(DGUS_VP_Variable& var, void *val_ptr) {
    auto old_top = top_file;
    const int16_t scroll = (int16_t)swap16(*(uint16_t*)val_ptr);
    if (scroll) {
      top_file += scroll;
      DEBUG_ECHOPAIR("new topfile calculated:", top_file);
      if (top_file < 0) {
        top_file = 0;
        DEBUG_ECHOLNPGM("Top of filelist reached");
      }
      else {
        int16_t max_top = filelist.count() -  DGUS_SD_FILESPERSCREEN;
        NOLESS(max_top, 0);
        NOMORE(top_file, max_top);
      }
      DEBUG_ECHOPAIR("new topfile adjusted:", top_file);
    }
    else {
      if (!filelist.isAtRootDir()) {
        filelist.upDir();
        top_file = 0;
        ForceCompleteUpdate();
      } else {
        // Navigate back to home
        GotoScreen(DGUSLCD_SCREEN_MAIN);
      }
    }

    if (old_top != top_file) ForceCompleteUpdate();
  }

  void DGUSScreenHandler::DGUSLCD_SD_FileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t touched_nr = (int16_t)swap16(*(uint16_t*)val_ptr) + top_file;

    DEBUG_ECHOLNPAIR("Selected file: ", touched_nr);

    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;
    if (filelist.isDir()) {
      filelist.changeDir(filelist.shortFilename());
      top_file = 0;
      ForceCompleteUpdate();
      return;
    }

    // Send print filename
    dgusdisplay.WriteVariable(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN, true);

    // Setup Confirmation screen
    file_to_print = touched_nr;
    HandleUserConfirmationPopUp(VP_SD_FileSelectConfirm, PSTR("Print file"), filelist.filename(), PSTR("from SD Card?"), nullptr, true, false, true, true);
  }

  void DGUSScreenHandler::SetPrintingFromHost() {
    const char* printFromHostString = PSTR("Printing from host");
    dgusdisplay.WriteVariablePGM(VP_SD_Print_Filename, printFromHostString, VP_SD_FileName_LEN, true);
  }

  void DGUSScreenHandler::DGUSLCD_SD_StartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    ScreenHandler.GotoScreen(
      DGUSLCD_SCREEN_SDPRINTMANIPULATION
    );
  }

  void DGUSScreenHandler::DGUSLCD_SD_SendFilename(DGUS_VP_Variable& var) {
    uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line > DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;
    if (filelist.seek(top_file + target_line))
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0);
    DGUSLCD_SendStringToDisplay(var);
  }

  void DGUSScreenHandler::SDCardInserted() {
    top_file = 0;
    filelist.refresh();
    auto cs = ScreenHandler.getCurrentScreen();
    if (cs == DGUSLCD_SCREEN_MAIN || cs == DGUSLCD_SCREEN_SETUP)
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_SDFILELIST);
  }

  void DGUSScreenHandler::SDCardRemoved() {
    if (!IS_SD_PRINTING()) {
      return;
    }

    if (current_screen == DGUSLCD_SCREEN_SDFILELIST
        || (current_screen == DGUSLCD_SCREEN_CONFIRM && (ConfirmVP == VP_SD_AbortPrintConfirmed || ConfirmVP == VP_SD_FileSelectConfirm))
        || current_screen == DGUSLCD_SCREEN_SDPRINTMANIPULATION
    ) ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN, false);
  }

  void DGUSScreenHandler::SDCardError() {
    DGUSScreenHandler::SDCardRemoved();
    ScreenHandler.sendinfoscreen(PSTR("NOTICE"), nullptr, PSTR("SD card error"), nullptr, true, true, true, true);
    ScreenHandler.GotoScreen(DGUSLCD_SCREEN_POPUP);
  }

#endif // SDSUPPORT

void DGUSScreenHandler::FilamentRunout() {
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_FILAMENTRUNOUT1);
}

void DGUSScreenHandler::OnFactoryReset() {
  ScreenHandler.DefaultSettings();
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN);
}

#if HAS_BUZZER
void DGUSScreenHandler::Buzzer(const uint16_t frequency, const uint16_t duration) {
  // Frequency is fixed - duration is not but in 8 ms steps
  const uint8_t durationUnits = static_cast<uint8_t>(duration / 8);

  DEBUG_ECHOLNPAIR("Invoking buzzer with units: ", durationUnits);
  const unsigned char buzzerCommand[] = { 0x00, durationUnits, 0x40 /*Volume*/, 0x02 };

  // WAE_Music_Play_Set
  dgusdisplay.WriteVariable(0xA0, buzzerCommand, sizeof(buzzerCommand));
}
#endif

bool DGUSScreenHandler::HandlePendingUserConfirmation() {
  if (!ExtUI::isWaitingOnUser()) {
    return false;
  }

  // Switch to the resume screen
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_RUNNING, false);

  // We might be re-entrant here
  ExtUI::setUserConfirmed();

  return true;
}

void DGUSScreenHandler::SetSynchronousOperationStart() {
  HasSynchronousOperation = true;
  ForceCompleteUpdate();
}

void DGUSScreenHandler::SetSynchronousOperationFinish() {
  HasSynchronousOperation = false;
}

void DGUSScreenHandler::SendBusyState(DGUS_VP_Variable &var) {
  dgusdisplay.WriteVariable(VP_BACK_BUTTON_STATE, HasSynchronousOperation ? ICON_BACK_BUTTON_DISABLED : ICON_BACK_BUTTON_ENABLED);
  dgusdisplay.WriteVariable(VP_BUSY_ANIM_STATE, HasSynchronousOperation ? ICON_THROBBER_ANIM_ON : ICON_THROBBER_ANIM_OFF);
}

void DGUSScreenHandler::OnHomingStart() {
  ScreenHandler.SetSynchronousOperationStart();
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_AUTOHOME);
}

void DGUSScreenHandler::OnHomingComplete() {
  ScreenHandler.SetSynchronousOperationFinish();
  ScreenHandler.PopToOldScreen();
}

void DGUSScreenHandler::OnPrintFinished() {
  ScreenHandler.GotoScreen(DGUSLCD_SCREEN_PRINT_FINISH, false);
}

void DGUSScreenHandler::ScreenConfirmedOK(DGUS_VP_Variable &var, void *val_ptr) {
  DGUS_VP_Variable ramcopy;
  if (!populate_VPVar(ConfirmVP, &ramcopy)) return;
  if (ramcopy.set_by_display_handler) ramcopy.set_by_display_handler(ramcopy, val_ptr);
}

#if HAS_MESH
void DGUSScreenHandler::OnMeshLevelingStart() {
  GotoScreen(DGUSLCD_SCREEN_LEVELING);
  dgusdisplay.WriteVariable(VP_MESH_SCREEN_MESSAGE_ICON, static_cast<uint16_t>(MESH_SCREEN_MESSAGE_ICON_LEVELING));

  ResetMeshValues();
  SetSynchronousOperationStart();

  MeshLevelIndex = 0;
  MeshLevelIconIndex = 0;
}

void DGUSScreenHandler::OnMeshLevelingUpdate(const int8_t x, const int8_t y, const float z) {
  SERIAL_ECHOPAIR("X: ", x);
  SERIAL_ECHOPAIR("; Y: ", y);
  SERIAL_ECHOPAIR("; Index ", MeshLevelIndex);
  SERIAL_ECHOLNPAIR("; Icon ", MeshLevelIconIndex);

  UpdateMeshValue(x, y, z);

  if (MeshLevelIndex < 0) {
    // We're not leveling
    return;
  }

  MeshLevelIndex++;
  MeshLevelIconIndex++;

  // Update icon
  dgusdisplay.WriteVariable(VP_MESH_LEVEL_STATUS, static_cast<uint16_t>(MeshLevelIconIndex + DGUS_GRID_VISUALIZATION_START_ID));

  if (MeshLevelIndex == GRID_MAX_POINTS) {
    // Done
    MeshLevelIndex = -1;

    RequestSaveSettings();
    
    if (GetPreviousScreen() == DGUSLCD_SCREEN_ZOFFSET_LEVEL) {
      // If the user is in the leveling workflow (not printing), get that hotend out of the way
      char gcodeBuffer[50] = {0};
      sprintf_P(gcodeBuffer, PSTR("G0 F3500 X%d\nG0 Y%d\nG0 Z%d\nM84"), (X_BED_SIZE / 2), (Y_BED_SIZE / 2), 35);
      queue.inject(gcodeBuffer);

      // Change text at the top
      ScreenHandler.SetViewMeshLevelState();
    } else {
      // When leveling from anywhere but the Z-offset/level screen, automatically pop back to the previous screen
      PopToOldScreen();
    }

    SetSynchronousOperationFinish();
  } else {
    // We've already updated the icon, so nothing left
  }
}
#endif
void DGUSScreenHandler::SetViewMeshLevelState() {
  dgusdisplay.WriteVariable(VP_MESH_SCREEN_MESSAGE_ICON, static_cast<uint16_t>(MESH_SCREEN_MESSAGE_ICON_VIEWING));
}
#if HAS_MESH
void DGUSScreenHandler::InitMeshValues() {
  if (ExtUI::getMeshValid()) {
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) {
          float z = ExtUI::getMeshPoint({ x, y });
          UpdateMeshValue(x, y, z);
      }

      safe_delay(100);
    }

    dgusdisplay.WriteVariable(VP_MESH_LEVEL_STATUS, static_cast<uint16_t>(DGUS_GRID_VISUALIZATION_START_ID + GRID_MAX_POINTS));
  } else {
    ResetMeshValues();
  }
}

void DGUSScreenHandler::ResetMeshValues() {
  for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
    for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) {
        UpdateMeshValue(x, y, 0);
    }

    safe_delay(100);
  }

  dgusdisplay.WriteVariable(VP_MESH_LEVEL_STATUS, static_cast<uint16_t>(DGUS_GRID_VISUALIZATION_START_ID));
}
#endif
uint16_t CreateRgb(double h, double s, double v) {
    struct {
      double h;       // angle in degrees
      double s;       // a fraction between 0 and 1
      double v;       // a fraction between 0 and 1
    } in = { h, s, v};

    double      hh, p, q, t, ff;
    long        i;
    struct {
      double r;       // a fraction between 0 and 1
      double g;       // a fraction between 0 and 1
      double b;       // a fraction between 0 and 1
      } out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return 0;
    }

    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
  
  return (((static_cast<uint8_t>(out.r * 255) & 0xf8)<<8) + ((static_cast<uint8_t>(out.g * 255) & 0xfc)<<3) + (static_cast<uint8_t>(out.b * 255)>>3));
}

#if HAS_MESH
void DGUSScreenHandler::UpdateMeshValue(const int8_t x, const int8_t y, const float z) {
  SERIAL_ECHOPAIR("X", x);
  SERIAL_ECHOPAIR(" Y", y);
  SERIAL_ECHO(" Z");
  SERIAL_ECHO_F(z, 4);

  // Determine the screen X and Y value
  if (x % SkipMeshPoint != 0 || y % SkipMeshPoint != 0) {
    // Skip this point
    SERIAL_ECHOLN("");
    return;
  }

  const uint8_t scrX = x / SkipMeshPoint;
  const uint8_t scrY = y / SkipMeshPoint;

  // Each Y is a full edge of X values
  const uint16_t vpAddr = VP_MESH_LEVEL_X0_Y0 + (scrY * MESH_LEVEL_VP_SIZE) + (scrX * MESH_LEVEL_VP_EDGE_SIZE);

  // ... DWIN is inconsistently truncating floats. Examples: 0.1811 becomes 0.181, 0.1810 becomes 0.180. But 0.1800 is not 0.179
  //     so we need to calculate a good number here that will not overflow
  float displayZ = z;
  
  {
    constexpr float correctionFactor = 0.0001;

    if (round(z * cpow(10,3)) == round((z + correctionFactor) * cpow(10,3))) {
      // If we don't accidently overshoot to the next number, trick the display by upping the number 0.0001 💩
      displayZ += correctionFactor;

      SERIAL_ECHO(" displayZ: ");
      SERIAL_ECHO_F(z, 4);
    }
  }

  SERIAL_ECHOLN("");

  dgusdisplay.WriteVariable(vpAddr, displayZ);

  // Set color
  const uint16_t spAddr = SP_MESH_LEVEL_X0_Y0 + (scrY * MESH_LEVEL_SP_SIZE) + (scrX * MESH_LEVEL_SP_EDGE_SIZE);

  uint16_t color = MESH_COLOR_NOT_MEASURED;

  // ... Only calculate if set
  if (abs(z) > MESH_UNSET_EPSILON) {
    // Determine color scale
    float clampedZ = max(min(z, 0.5f),-0.5f) * -1;
    float h = (clampedZ + 0.5f) * 240;

    // Convert to RGB
    color = CreateRgb(h, 1, 0.75);
  }

  dgusdisplay.SetVariableDisplayColor(spAddr, color);
}

void DGUSScreenHandler::HandleMeshPoint(DGUS_VP_Variable &var, void *val_ptr) {
  // Determine the X and Y for this mesh point
  // We can do this because we assume MESH_INPUT_SUPPORTED_X_SIZE and MESH_INPUT_SUPPORTED_Y_SIZE with MESH_INPUT_DATA_SIZE.
  // So each VP is MESH_INPUT_DATA_SIZE apart

  if (HasSynchronousOperation) {
    setstatusmessagePGM(PSTR("Wait for leveling to complete"));
    return;
  }

  const uint16_t probe_point = var.VP - VP_MESH_INPUT_X0_Y0;
  constexpr uint16_t col_size = MESH_INPUT_SUPPORTED_Y_SIZE * MESH_INPUT_DATA_SIZE;

  const uint8_t x = probe_point / col_size; // Will be 0 to 3 inclusive
  const uint8_t y = (probe_point - (x * col_size)) / MESH_INPUT_DATA_SIZE;

  int16_t rawZ = *(int16_t*)val_ptr;
  float z = swap16(rawZ) * 0.001;

  SERIAL_ECHOPAIR("Overriding mesh value. X:", x);
  SERIAL_ECHOPAIR(" Y:", y);
  SERIAL_ECHO(" Z:");
  SERIAL_ECHO_F(z, 4);
  SERIAL_ECHOPAIR(" [raw: ", rawZ);
  SERIAL_ECHOPAIR("] [point ", probe_point, "] ");
  SERIAL_ECHOPAIR(" [VP: ", var.VP);
  SERIAL_ECHOLN("]");

  UpdateMeshValue(x, y, z);
  ExtUI::setMeshPoint({ x, y }, z);

  RequestSaveSettings();
}
#endif
#if HAS_COLOR_LEDS
void DGUSScreenHandler::HandleLED(DGUS_VP_Variable &var, void *val_ptr) {
  // The display returns a 16-bit integer
  uint16_t newValue = swap16(*(uint16_t*)val_ptr);
  
  NOLESS(newValue, 0);
  NOMORE(newValue, 255);

  (*(uint8_t*)var.memadr) = static_cast<uint8_t>(newValue);
  leds.set_color(leds.color);

  SERIAL_ECHOLNPAIR("HandleLED ", newValue);
  RequestSaveSettings();

  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::SendLEDToDisplay(DGUS_VP_Variable &var) {
  DGUS_VP_Variable rcpy;
  if (!populate_VPVar(var.VP, &rcpy)) {
    return;
  }

  // The display wants a 16-bit integer
  uint16_t val = *(uint8_t*)var.memadr;
  rcpy.memadr = &val;

  DGUSLCD_SendWordValueToDisplay(rcpy);
}
#endif

const uint16_t* DGUSLCD_FindScreenVPMapList(uint8_t screen) {
  const uint16_t *ret;
  const struct VPMapping *map = VPMap;
  while ((ret = (uint16_t*) pgm_read_ptr(&(map->VPList)))) {
    if (pgm_read_byte(&(map->screen)) == screen) return ret;
    map++;
  }
  return nullptr;
}

const DGUS_VP_Variable* DGUSLCD_FindVPVar(const uint16_t vp) {
  const DGUS_VP_Variable *ret = ListOfVP;
  do {
    const uint16_t vpcheck = pgm_read_word(&(ret->VP));
    if (vpcheck == 0) break;
    if (vpcheck == vp) return ret;
    ++ret;
  } while (1);

  DEBUG_ECHOLNPAIR("FindVPVar NOT FOUND ", vp);
  return nullptr;
}

void DGUSScreenHandler::ScreenChangeHookIfIdle(DGUS_VP_Variable &var, void *val_ptr) {
  if (!ExtUI::isPrinting()) {
    ScreenChangeHook(var, val_ptr);
    GotoScreen(current_screen);
  }
}

void DGUSScreenHandler::ScreenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUSLCD_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  DGUSLCD_Screens target = (DGUSLCD_Screens)tmp[1];

  DEBUG_ECHOLNPAIR("Current screen:", current_screen);
  DEBUG_ECHOLNPAIR("Cancel target:", target);

  if (ExtUI::isWaitingOnUser() && current_screen == DGUSLCD_SCREEN_POPUP) {
    DEBUG_ECHOLN("Executing confirmation action");
    ExtUI::setUserConfirmed();
    PopToOldScreen();
    return;
  }

  if (target == DGUSLCD_SCREEN_POPUP || target == DGUSLCD_SCREEN_CONFIRM || target == 0 || target == 255 /*Buggy DWIN screen sometimes just returns 255*/) {
    PopToOldScreen();
    return;
  }

  UpdateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!DGUSLCD_FindScreenVPMapList(target)) DEBUG_ECHOLNPAIR("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandler::HandleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr) {
  ExtUI::coolDown();
  ScreenHandler.ForceCompleteUpdate(); // hint to send all data.
}

void DGUSScreenHandler::HandleTemperatureChanged(DGUS_VP_Variable &var, void *val_ptr) {
  celsius_t newvalue = swap16(*(uint16_t*)val_ptr);
  celsius_t acceptedvalue;

  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_T_E0_Set:
        NOMORE(newvalue, thermalManager.hotend_max_target(0));
        thermalManager.setTargetHotend(newvalue, 0);
        acceptedvalue = thermalManager.degTargetHotend(0);
        break;
    #endif
    #if HAS_HEATED_BED
      case VP_T_Bed_Set:
        NOMORE(newvalue, BED_MAXTEMP);
        thermalManager.setTargetBed(newvalue);
        acceptedvalue = thermalManager.degTargetBed();
        break;
    #endif
  }

  // reply to display the new value to update the view if the new value was rejected by the Thermal Manager.
  if (newvalue != acceptedvalue && var.send_to_display_handler) var.send_to_display_handler(var);
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleFanSpeedChanged(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newValue = swap16(*(uint16_t*)val_ptr);
    
    SERIAL_ECHOLNPAIR("Fan speed changed: ", newValue);
    ExtUI::setTargetFan_percent(newValue, ExtUI::fan_t::FAN0);

    ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandler::HandleFlowRateChanged(DGUS_VP_Variable &var, void *val_ptr) {
  #if EXTRUDERS
    uint16_t newValue = swap16(*(uint16_t*)val_ptr);
    
    SERIAL_ECHOLNPAIR("Flow rate changed: ", newValue);
    ExtUI::setFlow_percent(newValue, ExtUI::E0);

    ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  #else
    UNUSED(var); UNUSED(val_ptr);
  #endif
}

void DGUSScreenHandler::HandleManualExtrude(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleManualExtrude");

  int16_t movevalue = swap16(*(uint16_t*)val_ptr);
  float target = movevalue * 0.01f;
  ExtUI::extruder_t target_extruder;

  switch (var.VP) {
    #if HOTENDS >= 1
      case VP_MOVE_E0: target_extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >= 2
      case VP_MOVE_E1: target_extruder = ExtUI::extruder_t::E1; break;
    #endif
    default: return;
  }

  target += ExtUI::getAxisPosition_mm(target_extruder);
  ExtUI::setAxisPosition_mm(target, target_extruder);
  skipVP = var.VP;
}

void DGUSScreenHandler::HandleMotorLockUnlock(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleMotorLockUnlock");

  char buf[4];
  const int16_t lock = swap16(*(uint16_t*)val_ptr);
  strcpy_P(buf, lock ? PSTR("M18") : PSTR("M17"));

  //DEBUG_ECHOPAIR(" ", buf);
  queue.enqueue_one_now(buf);
}

#if ENABLED(POWER_LOSS_RECOVERY)

  void DGUSScreenHandler::TogglePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr) {
    PrintJobRecovery::enable(!PrintJobRecovery::enabled);
  }

  void DGUSScreenHandler::HandlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t value = swap16(*(uint16_t*)val_ptr);
    if (value) {
      queue.inject_P(PSTR("M1000"));
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_SDPRINTMANIPULATION, false);
    }
    else {
      recovery.cancel();
      ScreenHandler.GotoScreen(DGUSLCD_SCREEN_MAIN, false);
    }
  }

#endif



void DGUSScreenHandler::HandleScreenVersion(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleScreenVersion");
  
  uint16_t actualScreenVersion = swap16(*(uint16_t*)val_ptr);

  SERIAL_ECHOLNPAIR("DWIN version received: ", actualScreenVersion);
  SERIAL_ECHOLNPAIR("We expected DWIN version: ", EXPECTED_UI_VERSION_MAJOR);

  if (actualScreenVersion == EXPECTED_UI_VERSION_MAJOR) {
    SERIAL_ECHOLN("Screen version check passed.");
    return;
  }

  // Dump error to serial
  SERIAL_ECHOLN("WARNING: Your screen is not flashed correctly.");

  SERIAL_ECHOPAIR("We received version ", actualScreenVersion);
  SERIAL_ECHOLN("from the display");

  SERIAL_ECHOLNPAIR("This firmware needs screen version ", actualScreenVersion);
  SERIAL_ECHOLN("Please follow the release notes for flashing instructions.");

  // Will cause flashing in the loop()
  HasScreenVersionMismatch = true;

  // Show on display if user has M117 message
  if (actualScreenVersion >= 6) {
    // We have a scrolling message so we can do something more complicated
    char buffer[VP_M117_LEN] = {0};
    sprintf_P(buffer, "Please flash your TFT screen: version mismatch - build %d found but expected %d", actualScreenVersion, EXPECTED_UI_VERSION_MAJOR);
    setStatusMessage(buffer, true);
  } else {
    char buffer[VP_M117_LEN] = {0};
    sprintf_P(buffer, "Flash TFT please v%d<>v%d", actualScreenVersion, EXPECTED_UI_VERSION_MAJOR);
    setstatusmessage(buffer);
  }

  // Audio buzzer
  Buzzer(500, 500);
  for (int times=0;times<VERSION_MISMATCH_BUZZ_AMOUNT;times++) {
    safe_delay(750);
    Buzzer(500, 500);
  }
}

void DGUSScreenHandler::HandleScreenVersionMismatchLEDFlash() {
  if (!HasScreenVersionMismatch) return;

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + VERSION_MISMATCH_LED_FLASH_DELAY;

    caselight.on = !caselight.on;
    caselight.update(caselight.on);

    #if HAS_COLOR_LEDS
    if (caselight.on) {
      leds.set_color(LEDColorRed());
    } else {
      leds.set_color(LEDColorOff());
    }
    #endif
  }
}

void DGUSScreenHandler::HandleStepPerMMChanged(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMChanged");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw/10;
  ExtUI::axis_t axis;
  switch (var.VP) {
    case VP_X_STEP_PER_MM: axis = ExtUI::axis_t::X; break;
    case VP_Y_STEP_PER_MM: axis = ExtUI::axis_t::Y; break;
    case VP_Z_STEP_PER_MM: axis = ExtUI::axis_t::Z; break;
    default: return;
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisSteps_per_mm(value, axis);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(axis));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

void DGUSScreenHandler::HandleStepPerMMExtruderChanged(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleStepPerMMExtruderChanged");

  uint16_t value_raw = swap16(*(uint16_t*)val_ptr);
  DEBUG_ECHOLNPAIR("value_raw:", value_raw);
  float value = (float)value_raw/10;
  ExtUI::extruder_t extruder;
  switch (var.VP) {
    default: return;
    #if HOTENDS >= 1
      case VP_E0_STEP_PER_MM: extruder = ExtUI::extruder_t::E0; break;
    #endif
    #if HOTENDS >= 2
      case VP_E1_STEP_PER_MM: extruder = ExtUI::extruder_t::E1; break;
    #endif
  }
  DEBUG_ECHOLNPAIR_F("value:", value);
  ExtUI::setAxisSteps_per_mm(value,extruder);
  DEBUG_ECHOLNPAIR_F("value_set:", ExtUI::getAxisSteps_per_mm(extruder));
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

#if HAS_PID_HEATING
  void DGUSScreenHandler::HandleTemperaturePIDChanged(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t rawvalue = swap16(*(uint16_t*)val_ptr);
    DEBUG_ECHOLNPAIR("V1:", rawvalue);
    float value = (float)rawvalue / 10;
    DEBUG_ECHOLNPAIR("V2:", value);
    float newvalue = 0;

    switch (var.VP) {
      default: return;
      #if HOTENDS >= 1
        case VP_E0_PID_P: newvalue = value; break;
        case VP_E0_PID_I: newvalue = scalePID_i(value); break;
        case VP_E0_PID_D: newvalue = scalePID_d(value); break;
      #endif
      #if HOTENDS >= 2
        case VP_E1_PID_P: newvalue = value; break;
        case VP_E1_PID_I: newvalue = scalePID_i(value); break;
        case VP_E1_PID_D: newvalue = scalePID_d(value); break;
      #endif
      #if HAS_HEATED_BED
        case VP_BED_PID_P: newvalue = value; break;
        case VP_BED_PID_I: newvalue = scalePID_i(value); break;
        case VP_BED_PID_D: newvalue = scalePID_d(value); break;
      #endif
    }

    DEBUG_ECHOLNPAIR_F("V3:", newvalue);
    *(float *)var.memadr = newvalue;
    ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }

  void DGUSScreenHandler::HandlePIDAutotune(DGUS_VP_Variable &var, void *val_ptr) {
    DEBUG_ECHOLNPGM("HandlePIDAutotune");

    char buf[32] = {0};

    switch (var.VP) {
      default: break;
      #if ENABLED(PIDTEMP)
        #if HOTENDS >= 1
          case VP_PID_AUTOTUNE_E0: // Autotune Extruder 0
            sprintf(buf, "M303 E%d C5 S210 U1", ExtUI::extruder_t::E0);
            break;
        #endif
        #if HOTENDS >= 2
          case VP_PID_AUTOTUNE_E1:
            sprintf(buf, "M303 E%d C5 S210 U1", ExtUI::extruder_t::E1);
            break;
        #endif
      #endif
      #if ENABLED(PIDTEMPBED)
        case VP_PID_AUTOTUNE_BED:
          sprintf(buf, "M303 E-1 C5 S70 U1");
          break;
      #endif
    }

    if (buf[0]) queue.enqueue_one_now(buf);

    #if ENABLED(DGUS_UI_WAITING)
      sendinfoscreen(PSTR("PID is autotuning"), PSTR("please wait"), NUL_STR, NUL_STR, true, true, true, true);
      GotoScreen(DGUSLCD_SCREEN_WAITING);
    #endif
  }
#endif

void DGUSScreenHandler::HandleFadeHeight(DGUS_VP_Variable &var, void *val_ptr) {
    DGUSLCD_SetFloatAsIntFromDisplay<1>(var, val_ptr);

    RequestSaveSettings();
    return;
}

void DGUSScreenHandler::HandlePositionChange(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandlePositionChange");

  unsigned int speed = homing_feedrate_mm_m.x;
  float target_position = ((float)swap16(*(uint16_t*)val_ptr)) / 10.0;

  switch (var.VP) {
    default: return;

    case VP_X_POSITION:
      if (!ExtUI::canMove(ExtUI::axis_t::X)) return;
      current_position.x = min(target_position, static_cast<float>(X_MAX_POS));
      break;

    case VP_Y_POSITION:
      if (!ExtUI::canMove(ExtUI::axis_t::Y)) return;
      current_position.y = min(target_position, static_cast<float>(Y_MAX_POS));
      break;

    case VP_Z_POSITION:
      if (!ExtUI::canMove(ExtUI::axis_t::Z)) return;
      speed = homing_feedrate_mm_m.z;
      current_position.z = min(target_position, static_cast<float>(Z_MAX_POS));
      break;
  }

  line_to_current_position(MMM_TO_MMS(speed));

  ScreenHandler.ForceCompleteUpdate();
  DEBUG_ECHOLNPGM("poschg done.");
}

void DGUSScreenHandler::HandleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr, const_float_t scalingFactor) {
  DEBUG_ECHOLNPGM("HandleLiveAdjustZ");

  float absoluteAmount = float(swap16(*(int16_t*)val_ptr))  / scalingFactor;
  float existingAmount = ExtUI::getZOffset_mm();
  float difference = (absoluteAmount - existingAmount) < 0 ? -0.01 : 0.01;

  int16_t steps = ExtUI::mmToWholeSteps(difference, ExtUI::axis_t::Z);

  ExtUI::smartAdjustAxis_steps(steps, ExtUI::axis_t::Z, true);
#if ENABLED(HAS_BED_PROBE) //  Without a probe the Z offset is applied using baby offsets, which aren't saved anyway.
  RequestSaveSettings();
#endif
  ScreenHandler.ForceCompleteUpdate();
  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  return;
}

// This wrapper function is needed to avoid pulling in ExtUI in DGUSScreenHandler.h
float DGUSScreenHandler::GetCurrentLifeAdjustZ() {
  return ExtUI::getZOffset_mm();
}

void DGUSScreenHandler::HandleHeaterControl(DGUS_VP_Variable &var, void *val_ptr) {
  DEBUG_ECHOLNPGM("HandleHeaterControl");

  uint8_t preheat_temp = 0;
  switch (var.VP) {
    #if HOTENDS >= 1
      case VP_E0_CONTROL:
    #endif
    #if HOTENDS >= 2
      case VP_E1_CONTROL:
    #endif
    #if HOTENDS >= 3
      case VP_E2_CONTROL:
    #endif
      preheat_temp = PREHEAT_1_TEMP_HOTEND;
      break;

    case VP_BED_CONTROL:
      preheat_temp = PREHEAT_1_TEMP_BED;
      break;
  }

  *(int16_t*)var.memadr = *(int16_t*)var.memadr > 0 ? 0 : preheat_temp;
}

void DGUSScreenHandler::HandleLEDToggle() {
  bool newState = !caselight.on;

  caselight.on = newState;
  caselight.update(newState);

  RequestSaveSettings();
  ForceCompleteUpdate();
}

void DGUSScreenHandler::HandleToggleTouchScreenMute(DGUS_VP_Variable &var, void *val_ptr) {
  Settings.display_sound = !Settings.display_sound;
  ScreenHandler.SetTouchScreenConfiguration();

  RequestSaveSettings();
  ForceCompleteUpdate();

  ScreenHandler.skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

#if ALL(HAS_PROBE_SETTINGS, HAS_BED_PROBE)
void DGUSScreenHandler::HandleToggleProbeHeaters(DGUS_VP_Variable &var, void *val_ptr) {
  probe.settings.turn_heaters_off = !probe.settings.turn_heaters_off;

  RequestSaveSettings();
}

void DGUSScreenHandler::HandleToggleProbeTemperatureStabilization(DGUS_VP_Variable &var, void *val_ptr) {
  probe.settings.stabilize_temperatures_after_probing = !probe.settings.stabilize_temperatures_after_probing;

  RequestSaveSettings();
}

void DGUSScreenHandler::HandleToggleProbePreheatTemp(DGUS_VP_Variable &var, void *val_ptr) {
  ScreenHandler.DGUSLCD_SetValueDirectly<uint16_t>(var, val_ptr);

  RequestSaveSettings();
} 
#endif

void DGUSScreenHandler::HandleTouchScreenBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);

  SERIAL_ECHOLNPAIR("HandleTouchScreenBrightnessSetting: ", newvalue);
  Settings.screen_brightness = newvalue;
  ScreenHandler.SetTouchScreenConfiguration();

  RequestSaveSettings();
  ForceCompleteUpdate();
}

void DGUSScreenHandler::HandleTouchScreenStandbyBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);

  SERIAL_ECHOLNPAIR("HandleTouchScreenStandbyBrightnessSetting: ", newvalue);
  Settings.standby_screen_brightness = newvalue;
  ScreenHandler.SetTouchScreenConfiguration();

  RequestSaveSettings();
  ForceCompleteUpdate();
}

void DGUSScreenHandler::HandleTouchScreenStandbyTimeSetting(DGUS_VP_Variable &var, void *val_ptr) {
  uint16_t newvalue = swap16(*(uint16_t*)val_ptr);

  SERIAL_ECHOLNPAIR("HandleTouchScreenStandbyTimeSetting: ", newvalue);
  Settings.standby_time_seconds = newvalue;
  ScreenHandler.SetTouchScreenConfiguration();

  RequestSaveSettings();
  ForceCompleteUpdate();
}

void DGUSScreenHandler::HandleToggleTouchScreenStandbySetting(DGUS_VP_Variable &var, void *val_ptr) {
  SERIAL_ECHOLNPAIR("HandleToggleTouchScreenStandbySetting");

  Settings.display_standby = !Settings.display_standby;
  ScreenHandler.SetTouchScreenConfiguration();

  RequestSaveSettings();
  ForceCompleteUpdate();
}

void DGUSScreenHandler::HandleFanToggle() {
  thermalManager.fan_speed[0] = (thermalManager.fan_speed[0] > 0) ? 0 : 255;

  ForceCompleteUpdate();
}

void DGUSScreenHandler::UpdateNewScreen(DGUSLCD_Screens newscreen, bool save_current_screen) {
  SERIAL_ECHOLNPAIR("SetNewScreen: ", newscreen);

  if (save_current_screen && current_screen != DGUSLCD_SCREEN_POPUP && current_screen != DGUSLCD_SCREEN_CONFIRM) {
    SERIAL_ECHOLNPAIR("SetNewScreen (saving): ", newscreen);
    memmove(&past_screens[1], &past_screens[0], sizeof(past_screens) - 1);
    past_screens[0] = current_screen;
  }

  current_screen = newscreen;
  skipVP = 0;
  ForceCompleteUpdate();
}

void DGUSScreenHandler::PopToOldScreen() {
  DEBUG_ECHOLNPAIR("PopToOldScreen s=", past_screens[0]);

  if(past_screens[0] != 0) {
    GotoScreen(past_screens[0], false);
    memmove(&past_screens[0], &past_screens[1], sizeof(past_screens) - 1);
    past_screens[sizeof(past_screens) - 1] = DGUSLCD_SCREEN_MAIN;
  } else {
    if(ExtUI::isPrinting()) {
      GotoScreen(DGUSLCD_SCREEN_PRINT_RUNNING, false);
    } else {
      GotoScreen(DGUSLCD_SCREEN_MAIN, false);
    }
  }
}

void DGUSScreenHandler::OnBackButton(DGUS_VP_Variable &var, void *val_ptr) {
  // If we're busy: ignore
  if (HasSynchronousOperation) return;

  // Pop back
  uint16_t button_value = uInt16Value(val_ptr);

  PopToOldScreen();

  // Handle optional save from back button
  if (button_value == GENERIC_BACK_BUTTON_NEED_SAVE) {
    RequestSaveSettings();
  }
}

void DGUSScreenHandler::UpdateScreenVPData() {
  if (!dgusdisplay.isInitialized()) {
    return;
  }

  //DEBUG_ECHOPAIR(" UpdateScreenVPData Screen: ", current_screen);

  const uint16_t *VPList = DGUSLCD_FindScreenVPMapList(current_screen);
  if (!VPList) {
    DEBUG_ECHOLNPAIR(" NO SCREEN FOR: ", current_screen);
    ScreenComplete = true;
    return;  // nothing to do, likely a bug or boring screen.
  }

  // Round-robin updating of all VPs.
  VPList += update_ptr;

  bool sent_one = false;
  do {
    uint16_t VP = pgm_read_word(VPList);
    DEBUG_ECHOPAIR(" VP: ", VP);
    if (!VP) {
      update_ptr = 0;
      DEBUG_ECHOLNPGM(" UpdateScreenVPData done");
      ScreenComplete = true;
      return;  // Screen completed.
    }

    if (VP == skipVP) { skipVP = 0; continue; }

    DGUS_VP_Variable rcpy;
    if (populate_VPVar(VP, &rcpy)) {
      uint8_t expected_tx = 6 + rcpy.size;  // expected overhead is 6 bytes + payload.
      // Send the VP to the display, but try to avoid overrunning the Tx Buffer.
      // But send at least one VP, to avoid getting stalled.
      if (rcpy.send_to_display_handler && (!sent_one || expected_tx <= dgusdisplay.GetFreeTxBuffer())) {
        DEBUG_ECHOPAIR(" calling handler for ", rcpy.VP);
        sent_one = true;
        rcpy.send_to_display_handler(rcpy);
      }
      else {
        auto x = dgusdisplay.GetFreeTxBuffer();
        DEBUG_ECHOLNPAIR(" tx almost full: ", x);
        UNUSED(x);
        //DEBUG_ECHOPAIR(" update_ptr ", update_ptr);
        ScreenComplete = false;
        return;  // please call again!
      }
    }

  } while (++update_ptr, ++VPList, true);
}

void DGUSScreenHandler::GotoScreen(DGUSLCD_Screens screen, bool save_current_screen) {
  if (current_screen == screen) {
     // Ignore this request
     return;
  }

  DEBUG_ECHOLNPAIR("Issuing command to go to screen: ", screen);
  dgusdisplay.RequestScreen(screen);
  UpdateNewScreen(screen, save_current_screen);
}

bool DGUSScreenHandler::loop() {
  dgusdisplay.loop();

  HandleScreenVersionMismatchLEDFlash();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (ELAPSED(ms, next_event_ms) && SaveSettingsRequested) {
    // Only save settings so many times in a second - otherwise the EEPROM chip gets overloaded and the watchdog reboots the CPU
    settings.save();
    SaveSettingsRequested = false;
  }

  if (!IsScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;

    UpdateScreenVPData();
  }

  if (dgusdisplay.isInitialized()) {
    static bool booted = false;

    if (!booted) {
      progmem_str message = GET_TEXT_F(WELCOME_MSG);
      char buff[strlen_P((const char * const)message)+1];
      strcpy_P(buff, (const char * const) message);
      ExtUI::onStatusChanged((const char *)buff);

      int16_t percentage = static_cast<int16_t>(((float) ms / (float)BOOTSCREEN_TIMEOUT) * 100);
      if (percentage > 100) percentage = 100;

      dgusdisplay.WriteVariable(VP_STARTPROGRESSBAR, percentage);
    }

    if (!booted && TERN0(POWER_LOSS_RECOVERY, recovery.valid())) {
      booted = true;
      DEBUG_ECHOLN("Power loss recovery...");
    }

    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;
      
      #if HAS_COLOR_LEDS && !HAS_COLOR_LEDS_PREFERENCES
      leds.set_default();
      #endif
      
      // Ensure to pick up the settings
      SetTouchScreenConfiguration();

#if HAS_MESH
      // Set initial leveling status
      InitMeshValues();
#endif

      // No disabled back button
      ScreenHandler.SetSynchronousOperationFinish();

      // Ask for the screen version - HandleScreenVersion will act
      dgusdisplay.ReadVariable(VP_UI_VERSION_MAJOR);

      // Main menu
      GotoScreen(DGUSLCD_SCREEN_MAIN);
    }
  }

  return IsScreenComplete();
}

#endif // HAS_DGUS_LCD
