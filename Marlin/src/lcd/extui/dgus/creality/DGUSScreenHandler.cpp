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

/**
 * lcd/extui/dgus/creality/DGUSScreenHandler.cpp
 *
 * Screen handler for the Creality CR-6 touch screen, ported from the
 * CR6Community firmware (DGUS implementation by Sebastiaan Dammann).
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CREALITY_TOUCH

#include "../DGUSScreenHandler.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../module/settings.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"
#include "../../../../feature/caselight.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if HAS_FAN
  #include "../../../../module/fans.h"
#endif

#ifndef BOOTSCREEN_TIMEOUT
  #define BOOTSCREEN_TIMEOUT 3000
#endif

// Storage initialization
constexpr uint8_t dwin_settings_version = 6; // Increase when properties are added or removed
creality_dwin_settings_t DGUSScreenHandlerCreality::lcd_settings = { .settings_size = sizeof(creality_dwin_settings_t) };

bool DGUSScreenHandlerCreality::hasSynchronousOperation;
bool DGUSScreenHandlerCreality::printJobScreens;
DGUSScreenHandlerCreality::queue_done_func_t DGUSScreenHandlerCreality::queueDoneFunc = nullptr;
bool DGUSScreenHandlerCreality::hasScreenVersionMismatch;
int8_t DGUSScreenHandlerCreality::meshLevelIndex = -1;
uint8_t DGUSScreenHandlerCreality::meshLevelIconIndex = 0;
bool DGUSScreenHandlerCreality::hasRGBSettings = ENABLED(HAS_COLOR_LEDS);

#if HAS_MESH
  constexpr uint16_t SkipMeshPoint = GRID_MAX_POINTS_X > MESH_LEVEL_EDGE_MAX_POINTS ? ((GRID_MAX_POINTS_X - 1) / (GRID_MAX_POINTS_X - MESH_LEVEL_EDGE_MAX_POINTS)) : 1;
#endif

#if HAS_MEDIA
  extern ExtUI::FileList filelist;
#endif

//
// Touch screen settings, stored in the ExtUI area of the EEPROM
//

void DGUSScreenHandlerCreality::defaultSettings() {
  lcd_settings.settings_size = sizeof(creality_dwin_settings_t);
  lcd_settings.settings_version = dwin_settings_version;

  lcd_settings.led_state = false;

  lcd_settings.display_standby = true;
  lcd_settings.display_sound = true;

  lcd_settings.standby_screen_brightness = 10;
  lcd_settings.screen_brightness = 100;
  lcd_settings.standby_time_seconds = 60;

  #if HAS_COLOR_LEDS_PREFERENCES
    lcd_settings.last_led_color = leds.defaultLEDColor;
  #endif
}

static_assert(ExtUI::eeprom_data_size >= sizeof(creality_dwin_settings_t), "Insufficient space in EEPROM for UI parameters");

void DGUSScreenHandlerCreality::loadSettings(const char *buff) {
  creality_dwin_settings_t eepromSettings;
  memcpy(&eepromSettings, buff, sizeof(creality_dwin_settings_t));

  // If size is not the same, discard settings
  if (eepromSettings.settings_size != sizeof(creality_dwin_settings_t)) {
    SERIAL_ECHOLNPGM("Discarding DWIN LCD setting from EEPROM - size incorrect");
    defaultSettings();
    return;
  }

  if (eepromSettings.settings_version != dwin_settings_version) {
    SERIAL_ECHOLNPGM("Discarding DWIN LCD setting from EEPROM - settings version incorrect");
    defaultSettings();
    return;
  }

  // Copy into final location
  SERIAL_ECHOLNPGM("Loading DWIN LCD setting from EEPROM");
  memcpy(&lcd_settings, &eepromSettings, sizeof(creality_dwin_settings_t));

  // Apply settings
  caselight.on = lcd_settings.led_state;
  caselight.update_enabled();

  #if HAS_COLOR_LEDS_PREFERENCES
    leds.set_color(lcd_settings.last_led_color);
  #endif

  setTouchScreenConfiguration();
}

void DGUSScreenHandlerCreality::storeSettings(char *buff) {
  // Update settings from Marlin state, if necessary
  lcd_settings.led_state = caselight.on;

  #if HAS_COLOR_LEDS_PREFERENCES
    lcd_settings.last_led_color = leds.color;
  #endif

  // Write to buffer
  SERIAL_ECHOLNPGM("Saving DWIN LCD setting to EEPROM");
  memcpy(buff, &lcd_settings, sizeof(creality_dwin_settings_t));
}

void DGUSScreenHandlerCreality::setTouchScreenConfiguration() {
  LIMIT(lcd_settings.screen_brightness, 10, 100);     // Prevent a possible all-dark screen
  LIMIT(lcd_settings.standby_time_seconds, 10, 655);  // Prevent a possible all-dark screen for standby, yet also don't go higher than the DWIN limitation

  dgus.setTouchScreenConfiguration(lcd_settings.display_standby, lcd_settings.display_sound, lcd_settings.standby_screen_brightness, lcd_settings.screen_brightness, lcd_settings.standby_time_seconds);
}

void DGUSScreenHandlerCreality::killScreenCalled() {
  // If killed, always fully wake up
  dgus.setTouchScreenConfiguration(false, true, 100, 100, 100 /*Doesn't really matter*/);

  // Hey! Something is going on!
  buzzer(1000 /*ignored*/, 880);
}

#if ENABLED(POWER_LOSS_RECOVERY)

  void DGUSScreenHandlerCreality::onPowerLossResume() {
    gotoScreen(DGUS_SCREEN_POWER_LOSS);

    // Send print filename
    dgus.writeVariable(VP_SD_Print_Filename, recovery.info.sd_filename, VP_SD_FileName_LEN, true);
  }

  void DGUSScreenHandlerCreality::togglePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr) {
    recovery.enable(!recovery.enabled);
  }

  void DGUSScreenHandlerCreality::handlePowerLossRecovery(DGUS_VP_Variable &var, void *val_ptr) {
    const uint16_t value = BE16_P(val_ptr);
    if (value) {
      queue.inject(F("M1000"));
      gotoScreen(PLR_SCREEN_RECOVER, true);
    }
    else {
      recovery.cancel();
      gotoScreen(PLR_SCREEN_CANCEL, true);
    }
  }

#endif // POWER_LOSS_RECOVERY

void DGUSScreenHandlerCreality::handleUserConfirmationPopUp(uint16_t VP, PGM_P const line1, PGM_P const line2, PGM_P const line3, PGM_P const line4, bool l1, bool l2, bool l3, bool l4) {
  if (current_screenID == DGUS_SCREEN_CONFIRM) // Already showing a pop up, so we need to cancel that first.
    popToOldScreen();

  confirmVP = VP;
  sendInfoScreen_P(line1, line2, line3, line4, l1, l2, l3, l4);
  gotoScreen(DGUS_SCREEN_CONFIRM);
}

void DGUSScreenHandlerCreality::handleDevelopmentTestButton(DGUS_VP_Variable &var, void *val_ptr) {
  // Handle the button press only after 3 taps, so that a regular user won't tap it by accident
  static uint8_t tap_count = 0;
  if (++tap_count <= 3) return;

  switch (BE16_P(val_ptr)) {
    case VP_DEVELOPMENT_HELPER_BUTTON_ACTION_FIRMWARE_UPDATE:
      ExtUI::injectCommands(F("M997"));
      break;

    case VP_DEVELOPMENT_HELPER_BUTTON_ACTION_TO_MAIN_MENU:
      setStatusMessage(F("Dev action: main menu"));
      gotoScreen(DGUS_SCREEN_MAIN, true);
      break;

    case VP_DEVELOPMENT_HELPER_BUTTON_ACTION_RESET_DISPLAY:
      setStatusMessage(F("Dev action: reset DGUS"));
      dgus.resetDisplay();
      break;

    default:
      setStatusMessage(F("Dev action: unknown"));
      break;
  }
}

//
// Status message
//

void DGUSScreenHandlerCreality::setStatusMessage(const char *msg, const bool forceScrolling/*=false*/) {
  const bool needs_scrolling = forceScrolling || strlen(msg) > M117_STATIC_DISPLAY_LEN;

  DGUS_VP_Variable ramcopy;

  // Update static message to either NULL or the value
  if (populate_VPVar(VP_M117_STATIC, &ramcopy)) {
    ramcopy.memadr = (void*)(needs_scrolling ? NUL_STR : msg);
    sendStringToDisplay(ramcopy);
  }

  // Update scrolling message to either NULL or the value
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*)(needs_scrolling ? msg : NUL_STR);
    sendScrollingStringToDisplay(ramcopy);
  }
}

void DGUSScreenHandlerCreality::setStatusMessage_P(PGM_P const msg) {
  const bool needs_scrolling = msg && strlen_P(msg) > M117_STATIC_DISPLAY_LEN;

  DGUS_VP_Variable ramcopy;

  // Update static message to either NULL or the value
  if (populate_VPVar(VP_M117_STATIC, &ramcopy)) {
    ramcopy.memadr = (void*)(needs_scrolling ? nullptr : msg);
    sendStringToDisplayPGM(ramcopy);
  }

  // Update scrolling message to either NULL or the value
  if (populate_VPVar(VP_M117, &ramcopy)) {
    ramcopy.memadr = (void*)(needs_scrolling ? msg : nullptr);
    sendScrollingStringToDisplayPGM(ramcopy);
  }
}

//
// Data sent to the display
//

// Send the current print time to the display, if there's no remaining time to show.
void DGUSScreenHandlerCreality::sendPrintTimeToDisplay(DGUS_VP_Variable &var) {
  // Clear if changed and we shouldn't display
  static bool last_shouldDisplay = true;
  const bool shouldDisplay = TERN1(SHOW_REMAINING_TIME, ExtUI::getProgress_seconds_remaining() == 0);
  if (last_shouldDisplay != shouldDisplay && !shouldDisplay)
    dgus.writeVariable(VP_PrintTime, nullptr, var.size, true);

  last_shouldDisplay = shouldDisplay;
  if (!shouldDisplay) return;

  // Write if changed
  const duration_t elapsed = print_job_timer.duration();

  static uint32_t last_elapsed;
  if (elapsed == last_elapsed) return;

  char buf[32];
  elapsed.toString(buf);
  dgus.writeVariable(VP_PrintTime, buf, var.size, true);

  last_elapsed = elapsed.second();
}

// Send the current print time to the small field shown together with the remaining time
void DGUSScreenHandlerCreality::sendPrintTimeWithRemainingToDisplay(DGUS_VP_Variable &var) {
  // Clear if changed and we shouldn't display
  static bool last_shouldDisplay = true;
  const bool shouldDisplay = TERN0(SHOW_REMAINING_TIME, ExtUI::getProgress_seconds_remaining() != 0);
  if (last_shouldDisplay != shouldDisplay && !shouldDisplay)
    dgus.writeVariable(VP_PrintTimeWithRemainingVisible, nullptr, var.size, true);

  last_shouldDisplay = shouldDisplay;
  if (!shouldDisplay) return;

  // Write if changed
  const duration_t elapsed = print_job_timer.duration();

  static uint32_t last_elapsed;
  if (elapsed == last_elapsed) return;

  char buf[32];
  elapsed.toString(buf);
  dgus.writeVariable(VP_PrintTimeWithRemainingVisible, buf, var.size, true);

  last_elapsed = elapsed.second();
}

// Send the remaining print time to the display
void DGUSScreenHandlerCreality::sendPrintTimeRemainingToDisplay(DGUS_VP_Variable &var) {
  #if ENABLED(SHOW_REMAINING_TIME)
    static uint32_t lastRemainingTime = -1;
    const uint32_t remaining_time = ExtUI::getProgress_seconds_remaining();
    if (lastRemainingTime == remaining_time) return;

    const bool has_remaining_time = remaining_time != 0;

    // Update display of SPs (toggle between large and small print timer)
    dgus.writeVariable(VP_HideRemainingTime_Ico, has_remaining_time ? ICON_REMAINING_VISIBLE : ICON_REMAINING_HIDDEN);

    if (!has_remaining_time) {
      // Clear remaining time
      dgus.writeVariable(VP_PrintTimeRemaining, nullptr, var.size, true);
      lastRemainingTime = remaining_time;
      return;
    }

    // Send a progress update to the display if anything is different.
    // This allows custom M117 commands to override the displayed string if desired.
    const duration_t remaining(remaining_time);
    char buffer[21] = { 0 };
    remaining.toString(buffer);
    dgus.writeVariable(VP_PrintTimeRemaining, buffer, var.size, true);

    lastRemainingTime = remaining_time;
  #else
    UNUSED(var);
  #endif
}

void DGUSScreenHandlerCreality::sendAboutFirmwareWebsite(DGUS_VP_Variable &var) {
  dgus.writeVariable_P(var.VP, PSTR(WEBSITE_URL), var.size, true);
}

void DGUSScreenHandlerCreality::sendAboutFirmwareVersion(DGUS_VP_Variable &var) {
  dgus.writeVariable_P(var.VP, PSTR(SHORT_BUILD_VERSION), var.size, true);
}

void DGUSScreenHandlerCreality::sendAboutPrintSize(DGUS_VP_Variable &var) {
  char PRINTSIZE[VP_PRINTER_BEDSIZE_LEN] = { 0 };
  sprintf_P(PRINTSIZE, PSTR("%dx%dx%d"), X_BED_SIZE, Y_BED_SIZE, Z_MAX_POS);
  dgus.writeVariable(var.VP, PRINTSIZE, sizeof(PRINTSIZE), true);
}

// Sends a (RAM located) string to a scrolling text field
void DGUSScreenHandlerCreality::sendScrollingStringToDisplay(DGUS_VP_Variable &var) {
  dgus.writeVariable(var.VP, (char*)var.memadr, var.size, true, DWIN_SCROLLER_FILLER_CHAR);
}

// Sends a (flash located) string to a scrolling text field
void DGUSScreenHandlerCreality::sendScrollingStringToDisplayPGM(DGUS_VP_Variable &var) {
  dgus.writeVariable_P(var.VP, (char*)var.memadr, var.size, true, DWIN_SCROLLER_FILLER_CHAR);
}

#if HAS_FAN

  // Send the fan status as a toggle icon
  void DGUSScreenHandlerCreality::sendFanStatusToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) {
      const uint16_t data_to_send = *(uint8_t *)var.memadr ? ICON_TOGGLE_ON : ICON_TOGGLE_OFF;
      dgus.writeVariable(var.VP, data_to_send);
    }
  }

  void DGUSScreenHandlerCreality::sendFanSpeedToDisplay(DGUS_VP_Variable &var) {
    if (var.memadr) dgus.writeVariable(var.VP, int16_t(ExtUI::getTargetFan_percent(ExtUI::fan_t::FAN0)));
  }

  void DGUSScreenHandlerCreality::handleFanSpeedChanged(DGUS_VP_Variable &var, void *val_ptr) {
    const uint16_t newValue = BE16_P(val_ptr);
    DEBUG_ECHOLNPGM("Fan speed changed: ", newValue);
    ExtUI::setTargetFan_percent(newValue, ExtUI::fan_t::FAN0);
    skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }

#endif // HAS_FAN

//
// SD Card
//

#if HAS_MEDIA

  void DGUSScreenHandlerCreality::sdScrollFilelist(DGUS_VP_Variable& var, void *val_ptr) {
    auto old_top = top_file;
    const int16_t scroll = (int16_t)BE16_P(val_ptr);
    if (scroll) {
      top_file += scroll;
      if (top_file < 0)
        top_file = 0;
      else {
        int16_t max_top = filelist.count() - DGUS_SD_FILESPERSCREEN;
        NOLESS(max_top, 0);
        NOMORE(top_file, max_top);
      }
    }
    else if (!filelist.isAtRootDir()) {
      filelist.upDir();
      top_file = 0;
      forceCompleteUpdate();
    }
    else {
      // Navigate back to home
      gotoScreen(DGUS_SCREEN_MAIN);
    }

    if (old_top != top_file) forceCompleteUpdate();
  }

  void DGUSScreenHandler::sdFileSelected(DGUS_VP_Variable &var, void *val_ptr) {
    const uint16_t touched_nr = (int16_t)BE16_P(val_ptr) + top_file;

    if (touched_nr > filelist.count()) return;
    if (!filelist.seek(touched_nr)) return;
    if (filelist.isDir()) {
      filelist.changeDir(filelist.shortFilename());
      top_file = 0;
      forceCompleteUpdate();
      return;
    }

    // Send print filename
    dgus.writeVariable(VP_SD_Print_Filename, filelist.filename(), VP_SD_FileName_LEN, true);

    // Setup Confirmation screen
    file_to_print = touched_nr;
    screen.handleUserConfirmationPopUp(VP_SD_FileSelectConfirm, PSTR("Print file"), filelist.filename(), PSTR("from SD Card?"), nullptr, true, false, true, true);
  }

  void DGUSScreenHandlerCreality::setPrintingFromHost() {
    dgus.writeVariable_P(VP_SD_Print_Filename, PSTR("Printing from host"), VP_SD_FileName_LEN, true);
  }

  void DGUSScreenHandler::sdStartPrint(DGUS_VP_Variable &var, void *val_ptr) {
    if (!filelist.seek(file_to_print)) return;
    ExtUI::printFile(filelist.shortFilename());
    screen.gotoScreen(DGUS_SCREEN_SDPRINTMANIPULATION);
  }

  void DGUSScreenHandler::sdSendFilename(DGUS_VP_Variable& var) {
    const uint16_t target_line = (var.VP - VP_SD_FileName0) / VP_SD_FileName_LEN;
    if (target_line >= DGUS_SD_FILESPERSCREEN) return;
    char tmpfilename[VP_SD_FileName_LEN + 1] = "";
    var.memadr = (void*)tmpfilename;
    if (filelist.seek(top_file + target_line))
      snprintf_P(tmpfilename, VP_SD_FileName_LEN, PSTR("%s%c"), filelist.filename(), filelist.isDir() ? '/' : 0);
    sendStringToDisplay(var);
  }

  void DGUSScreenHandler::sdCardInserted() {
    top_file = 0;
    filelist.refresh();
    const DGUS_ScreenID cs = getCurrentScreen();
    if (cs == DGUS_SCREEN_MAIN || cs == DGUS_SCREEN_SETUP)
      screen.gotoScreen(DGUS_SCREEN_SDFILELIST);
  }

  void DGUSScreenHandler::sdCardRemoved() {
    if (!card.isStillPrinting()) return;

    if (current_screenID == DGUS_SCREEN_SDFILELIST
        || (current_screenID == DGUS_SCREEN_CONFIRM && (confirmVP == VP_SD_AbortPrintConfirmed || confirmVP == VP_SD_FileSelectConfirm))
        || current_screenID == DGUS_SCREEN_SDPRINTMANIPULATION
    ) screen.gotoScreen(DGUS_SCREEN_MAIN, true);
  }

  void DGUSScreenHandlerCreality::sdCardError() {
    sdCardRemoved();
    sendInfoScreen(F("NOTICE"), nullptr, F("SD card error"), nullptr);
    gotoScreen(DGUS_SCREEN_POPUP);
  }

#endif // HAS_MEDIA

void DGUSScreenHandlerCreality::filamentRunout() {
  gotoScreen(DGUS_SCREEN_FILAMENTRUNOUT1);
}

void DGUSScreenHandlerCreality::onFactoryReset() {
  defaultSettings();
  gotoScreen(DGUS_SCREEN_MAIN);
}

#if HAS_SOUND
  void DGUSScreenHandlerCreality::buzzer(const uint16_t frequency, const uint16_t duration/*=0*/) {
    // Frequency is fixed - duration is not but in 8 ms steps
    const uint8_t durationUnits = static_cast<uint8_t>(duration / 8);
    const unsigned char buzzerCommand[] = { 0x00, durationUnits, 0x40 /*Volume*/, 0x02 };

    // WAE_Music_Play_Set
    dgus.writeVariable(0xA0, buzzerCommand, sizeof(buzzerCommand));
  }
#endif

bool DGUSScreenHandlerCreality::handlePendingUserConfirmation() {
  if (!ExtUI::awaitingUserConfirm()) return false;

  // Switch to the resume screen
  gotoScreen(DGUS_SCREEN_PRINT_RUNNING, true);

  // We might be re-entrant here
  ExtUI::setUserConfirmed();

  return true;
}

void DGUSScreenHandlerCreality::setSynchronousOperationStart() {
  hasSynchronousOperation = true;
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::setSynchronousOperationFinish() {
  hasSynchronousOperation = false;
}

void DGUSScreenHandlerCreality::sendBusyState(DGUS_VP_Variable &var) {
  dgus.writeVariable(VP_BACK_BUTTON_STATE, hasSynchronousOperation ? ICON_BACK_BUTTON_DISABLED : ICON_BACK_BUTTON_ENABLED);
  dgus.writeVariable(VP_BUSY_ANIM_STATE, hasSynchronousOperation ? ICON_THROBBER_ANIM_ON : ICON_THROBBER_ANIM_OFF);
}

void DGUSScreenHandlerCreality::onHomingStart() {
  setSynchronousOperationStart();
  gotoScreen(DGUS_SCREEN_AUTOHOME);
}

void DGUSScreenHandlerCreality::onHomingDone() {
  setSynchronousOperationFinish();
  popToOldScreen();
}

void DGUSScreenHandlerCreality::onPrintDone() {
  gotoScreen(DGUS_SCREEN_PRINT_FINISH, true);
}

void DGUSScreenHandlerCreality::setViewMeshLevelState() {
  dgus.writeVariable(VP_MESH_SCREEN_MESSAGE_ICON, MESH_SCREEN_MESSAGE_ICON_VIEWING);
}

//
// Mesh view and edit
//

#if HAS_MESH

  void DGUSScreenHandlerCreality::onLevelingStart() {
    gotoScreen(DGUS_SCREEN_LEVELING);
    dgus.writeVariable(VP_MESH_SCREEN_MESSAGE_ICON, MESH_SCREEN_MESSAGE_ICON_LEVELING);

    resetMeshValues();
    setSynchronousOperationStart();

    meshLevelIndex = 0;
    meshLevelIconIndex = 0;
  }

  void DGUSScreenHandlerCreality::onLevelingUpdate(const int8_t x, const int8_t y, const float z) {
    DEBUG_ECHOLNPGM("X: ", x, "; Y: ", y, "; Index ", meshLevelIndex, "; Icon ", meshLevelIconIndex);

    updateMeshValue(x, y, z);

    if (meshLevelIndex < 0) return; // We're not leveling

    meshLevelIndex++;
    meshLevelIconIndex++;

    // Update icon
    dgus.writeVariable(VP_MESH_LEVEL_STATUS, uint16_t(meshLevelIconIndex + DGUS_GRID_VISUALIZATION_START_ID));

    if (meshLevelIndex == GRID_MAX_POINTS) {
      // Done
      meshLevelIndex = -1;

      requestSaveSettings();

      if (getPreviousScreen() == DGUS_SCREEN_ZOFFSET_LEVEL) {
        // If the user is in the leveling workflow (not printing), get that hotend out of the way
        char gcodeBuffer[50] = { 0 };
        sprintf_P(gcodeBuffer, PSTR("G0 F3500 X%d\nG0 Y%d\nG0 Z%d\nM84"), (X_BED_SIZE / 2), (Y_BED_SIZE / 2), 35);
        queue.inject(gcodeBuffer);

        // Change text at the top
        setViewMeshLevelState();
      }
      else {
        // When leveling from anywhere but the Z-offset/level screen, automatically pop back to the previous screen
        popToOldScreen();
      }

      setSynchronousOperationFinish();
    }
  }

  void DGUSScreenHandlerCreality::initMeshValues() {
    if (ExtUI::getLevelingIsValid()) {
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
          updateMeshValue(x, y, ExtUI::getMeshPoint({ x, y }));
        safe_delay(100);
      }
      dgus.writeVariable(VP_MESH_LEVEL_STATUS, uint16_t(DGUS_GRID_VISUALIZATION_START_ID + GRID_MAX_POINTS));
    }
    else
      resetMeshValues();
  }

  void DGUSScreenHandlerCreality::resetMeshValues() {
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        updateMeshValue(x, y, 0);
      safe_delay(100);
    }
    dgus.writeVariable(VP_MESH_LEVEL_STATUS, uint16_t(DGUS_GRID_VISUALIZATION_START_ID));
  }

  // Convert HSV (h: 0..360, s,v: 0..1) to a 16-bit RGB565 color
  static uint16_t createRgb(float h, const float s, const float v) {
    if (s <= 0) return 0;

    if (h >= 360) h = 0;
    h /= 60;
    const long i = (long)h;
    const float ff = h - i,
                p = v * (1 - s),
                q = v * (1 - (s * ff)),
                t = v * (1 - (s * (1 - ff)));

    float r, g, b;
    switch (i) {
      case 0:  r = v; g = t; b = p; break;
      case 1:  r = q; g = v; b = p; break;
      case 2:  r = p; g = v; b = t; break;
      case 3:  r = p; g = q; b = v; break;
      case 4:  r = t; g = p; b = v; break;
      case 5:
      default: r = v; g = p; b = q; break;
    }

    return ((uint8_t(r * 255) & 0xF8) << 8) + ((uint8_t(g * 255) & 0xFC) << 3) + (uint8_t(b * 255) >> 3);
  }

  void DGUSScreenHandlerCreality::updateMeshValue(const int8_t x, const int8_t y, const float z) {
    DEBUG_ECHOLNPGM("X", x, " Y", y, " Z", p_float_t(z, 4));

    // Determine the screen X and Y value
    if (x % SkipMeshPoint != 0 || y % SkipMeshPoint != 0) return; // Skip this point

    const uint8_t scrX = x / SkipMeshPoint, scrY = y / SkipMeshPoint;

    // Each Y is a full edge of X values
    const uint16_t vpAddr = VP_MESH_LEVEL_X0_Y0 + (scrY * MESH_LEVEL_VP_SIZE) + (scrX * MESH_LEVEL_VP_EDGE_SIZE);

    // DWIN is inconsistently truncating floats. Examples: 0.1811 becomes 0.181, 0.1810 becomes 0.180. But 0.1800 is not 0.179
    // so we need to calculate a good number here that will not overflow
    float displayZ = z;
    constexpr float correctionFactor = 0.0001f;
    // If we don't accidentally overshoot to the next number, trick the display by upping the number 0.0001
    if (roundf(z * 1000) == roundf((z + correctionFactor) * 1000)) displayZ += correctionFactor;

    dgus.writeVariable(vpAddr, displayZ);

    // Set color
    const uint16_t spAddr = SP_MESH_LEVEL_X0_Y0 + (scrY * MESH_LEVEL_SP_SIZE) + (scrX * MESH_LEVEL_SP_EDGE_SIZE);

    uint16_t color = MESH_COLOR_NOT_MEASURED;

    // Only calculate if set
    if (ABS(z) > MESH_UNSET_EPSILON) {
      // Determine color scale
      const float clampedZ = -constrain(z, -0.5f, 0.5f),
                  h = (clampedZ + 0.5f) * 240;
      color = createRgb(h, 1, 0.75f);
    }

    dgus.setVariableDisplayColor(spAddr, color);
  }

  void DGUSScreenHandlerCreality::handleMeshPoint(DGUS_VP_Variable &var, void *val_ptr) {
    // Determine the X and Y for this mesh point
    // We can do this because we assume MESH_INPUT_SUPPORTED_X_SIZE and MESH_INPUT_SUPPORTED_Y_SIZE with MESH_INPUT_DATA_SIZE.
    // So each VP is MESH_INPUT_DATA_SIZE apart

    if (hasSynchronousOperation) {
      setStatusMessage(F("Wait for leveling to complete"));
      return;
    }

    const uint16_t probe_point = var.VP - VP_MESH_INPUT_X0_Y0;
    constexpr uint16_t col_size = MESH_INPUT_SUPPORTED_Y_SIZE * MESH_INPUT_DATA_SIZE;

    const uint8_t x = probe_point / col_size, // Will be 0 to 3 inclusive
                  y = (probe_point - (x * col_size)) / MESH_INPUT_DATA_SIZE;

    const float z = int16_t(BE16_P(val_ptr)) * 0.001f;

    DEBUG_ECHOLNPGM("Overriding mesh value. X:", x, " Y:", y, " Z:", p_float_t(z, 4), " [point ", probe_point, "] [VP: ", var.VP, "]");

    updateMeshValue(x, y, z);
    ExtUI::setMeshPoint({ x, y }, z);

    requestSaveSettings();
  }

#endif // HAS_MESH

#if HAS_COLOR_LEDS

  void DGUSScreenHandlerCreality::handleLED(DGUS_VP_Variable &var, void *val_ptr) {
    // The display returns a 16-bit integer
    const uint16_t newValue = _MIN(BE16_P(val_ptr), 255U);

    *(uint8_t*)var.memadr = uint8_t(newValue);
    leds.set_color(leds.color);

    DEBUG_ECHOLNPGM("handleLED ", newValue);
    requestSaveSettings();

    skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
  }

  void DGUSScreenHandlerCreality::sendLEDToDisplay(DGUS_VP_Variable &var) {
    DGUS_VP_Variable rcpy;
    if (!populate_VPVar(var.VP, &rcpy)) return;

    // The display wants a 16-bit integer
    uint16_t val = *(uint8_t*)var.memadr;
    rcpy.memadr = &val;

    sendWordValueToDisplay(rcpy);
  }

#endif // HAS_COLOR_LEDS

//
// Handlers for values and buttons from the display
//

void DGUSScreenHandler::screenChangeHook(DGUS_VP_Variable &var, void *val_ptr) {
  uint8_t *tmp = (uint8_t*)val_ptr;

  // The keycode in target is coded as <from-frame><to-frame>, so 0x0100A means
  // from screen 1 (main) to 10 (temperature). DGUS_SCREEN_POPUP is special,
  // meaning "return to previous screen"
  const DGUS_ScreenID target = (DGUS_ScreenID)tmp[1];

  DEBUG_ECHOLNPGM("Current screen:", current_screenID, " Cancel target:", target);

  if (ExtUI::awaitingUserConfirm() && current_screenID == DGUS_SCREEN_POPUP) {
    DEBUG_ECHOLNPGM("Executing confirmation action");
    ExtUI::setUserConfirmed();
    screen.popToOldScreen();
    return;
  }

  if (target == DGUS_SCREEN_POPUP || target == DGUS_SCREEN_CONFIRM || target == 0 || target == 255 /*Buggy DWIN screen sometimes just returns 255*/) {
    screen.popToOldScreen();
    return;
  }

  screen.updateNewScreen(target);

  #ifdef DEBUG_DGUSLCD
    if (!findScreenVPMapList(target)) DEBUG_ECHOLNPGM("WARNING: No screen Mapping found for ", target);
  #endif
}

void DGUSScreenHandlerCreality::handleAllHeatersOff(DGUS_VP_Variable &var, void *val_ptr) {
  ExtUI::coolDown();
  forceCompleteUpdate(); // hint to send all data.
}

void DGUSScreenHandlerCreality::handleScreenVersion(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t actualScreenVersion = BE16_P(val_ptr);

  SERIAL_ECHOLNPGM("DWIN version received: ", actualScreenVersion);
  SERIAL_ECHOLNPGM("We expected DWIN version: ", EXPECTED_UI_VERSION_MAJOR);

  if (actualScreenVersion == EXPECTED_UI_VERSION_MAJOR) {
    SERIAL_ECHOLNPGM("Screen version check passed.");
    return;
  }

  // Dump error to serial
  SERIAL_ECHOLNPGM("WARNING: Your screen is not flashed correctly.");
  SERIAL_ECHOLNPGM("We received version ", actualScreenVersion, " from the display");
  SERIAL_ECHOLNPGM("This firmware needs screen version ", EXPECTED_UI_VERSION_MAJOR);
  SERIAL_ECHOLNPGM("Please follow the release notes for flashing instructions.");

  // Will cause flashing in the loop()
  hasScreenVersionMismatch = true;

  // Show on display if user has M117 message
  char buffer[VP_M117_LEN] = { 0 };
  if (actualScreenVersion >= 6) {
    // We have a scrolling message so we can do something more complicated
    sprintf_P(buffer, PSTR("Please flash your TFT screen: version mismatch - build %d found but expected %d"), actualScreenVersion, EXPECTED_UI_VERSION_MAJOR);
    setStatusMessage(buffer, true);
  }
  else {
    sprintf_P(buffer, PSTR("Flash TFT please v%d<>v%d"), actualScreenVersion, EXPECTED_UI_VERSION_MAJOR);
    setStatusMessage(buffer);
  }

  // Audio buzzer
  buzzer(500, 500);
  for (uint8_t times = 0; times < VERSION_MISMATCH_BUZZ_AMOUNT; ++times) {
    safe_delay(750);
    buzzer(500, 500);
  }
}

void DGUSScreenHandlerCreality::handleScreenVersionMismatchLEDFlash() {
  if (!hasScreenVersionMismatch) return;

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + VERSION_MISMATCH_LED_FLASH_DELAY;

    caselight.on = !caselight.on;
    caselight.update_enabled();

    #if HAS_COLOR_LEDS
      if (caselight.on)
        leds.set_color(LEDColorRed());
      else
        leds.set_off();
    #endif
  }
}

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  void DGUSScreenHandlerCreality::handleFadeHeight(DGUS_VP_Variable &var, void *val_ptr) {
    planner.set_z_fade_height(float(BE16_P(val_ptr)) / 10.0f);
    requestSaveSettings();
  }
#endif

void DGUSScreenHandlerCreality::handlePositionChange(DGUS_VP_Variable &var, void *val_ptr) {
  feedRate_t speed = motion.homing_feedrate_mm_m.x;
  const float target_position = float(BE16_P(val_ptr)) / 10.0f;

  switch (var.VP) {
    default: return;

    case VP_X_POSITION:
      if (!ExtUI::canMove(ExtUI::axis_t::X)) return;
      motion.position.x = _MIN(target_position, float(X_MAX_POS));
      break;

    case VP_Y_POSITION:
      if (!ExtUI::canMove(ExtUI::axis_t::Y)) return;
      motion.position.y = _MIN(target_position, float(Y_MAX_POS));
      break;

    case VP_Z_POSITION:
      if (!ExtUI::canMove(ExtUI::axis_t::Z)) return;
      speed = motion.homing_feedrate_mm_m.z;
      motion.position.z = _MIN(target_position, float(Z_MAX_POS));
      break;
  }

  motion.goto_current_position(MMM_TO_MMS(speed));

  forceCompleteUpdate();
}

// Minimum segment time is uint32 (µs) in the planner but a 16-bit int on the screen
void DGUSScreenHandlerCreality::handleMinSegmentTime(DGUS_VP_Variable&, void *val_ptr) {
  planner.settings.min_segment_time_us = _MAX(0, int16_t(BE16_P(val_ptr)));
}
void DGUSScreenHandlerCreality::sendMinSegmentTime(DGUS_VP_Variable &var) {
  dgus.writeVariable(var.VP, uint16_t(_MIN(planner.settings.min_segment_time_us, uint32_t(INT16_MAX))));
}

#if ENABLED(LIN_ADVANCE)
  // Linear Advance K with 2 decimals, through ExtUI so derived values are updated
  void DGUSScreenHandlerCreality::handleLinearAdvance(DGUS_VP_Variable&, void *val_ptr) {
    ExtUI::setLinearAdvance_mm_mm_s(float(int16_t(BE16_P(val_ptr))) / 100.0f, ExtUI::E0);
  }
  void DGUSScreenHandlerCreality::sendLinearAdvance(DGUS_VP_Variable &var) {
    dgus.writeVariable(var.VP, int16_t(LROUND(ExtUI::getLinearAdvance_mm_mm_s(ExtUI::E0) * 100.0f)));
  }
#endif

void DGUSScreenHandlerCreality::handleLiveAdjustZ(DGUS_VP_Variable &var, void *val_ptr, const float scalingFactor) {
  // The display sends the new absolute value. Move one hundredth of a mm towards it.
  const float absoluteAmount = float(int16_t(BE16_P(val_ptr))) / scalingFactor,
              existingAmount = ExtUI::getZOffset_mm(),
              difference = (absoluteAmount - existingAmount) < 0 ? -0.01f : 0.01f;

  const int16_t steps = ExtUI::mmToWholeSteps(difference, ExtUI::axis_t::Z);

  ExtUI::smartAdjustAxis_steps(steps, ExtUI::axis_t::Z, true);

  // Without a probe the Z offset is applied using babystepping, which isn't saved
  TERN_(HAS_BED_PROBE, requestSaveSettings());

  forceCompleteUpdate();
  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

// This wrapper function is needed to avoid pulling in ExtUI in DGUSScreenHandler.h
float DGUSScreenHandlerCreality::getCurrentLiveAdjustZ() {
  return ExtUI::getZOffset_mm();
}

void DGUSScreenHandlerCreality::handleLEDToggle() {
  caselight.on = !caselight.on;
  caselight.update_enabled();

  requestSaveSettings();
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::handleToggleTouchScreenMute(DGUS_VP_Variable &var, void *val_ptr) {
  lcd_settings.display_sound = !lcd_settings.display_sound;
  setTouchScreenConfiguration();

  requestSaveSettings();
  forceCompleteUpdate();

  skipVP = var.VP; // don't overwrite value the next update time as the display might autoincrement in parallel
}

void DGUSScreenHandlerCreality::handleTouchScreenBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t newvalue = BE16_P(val_ptr);
  DEBUG_ECHOLNPGM("handleTouchScreenBrightnessSetting: ", newvalue);
  lcd_settings.screen_brightness = newvalue;
  setTouchScreenConfiguration();

  requestSaveSettings();
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::handleTouchScreenStandbyBrightnessSetting(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t newvalue = BE16_P(val_ptr);
  DEBUG_ECHOLNPGM("handleTouchScreenStandbyBrightnessSetting: ", newvalue);
  lcd_settings.standby_screen_brightness = newvalue;
  setTouchScreenConfiguration();

  requestSaveSettings();
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::handleTouchScreenStandbyTimeSetting(DGUS_VP_Variable &var, void *val_ptr) {
  const uint16_t newvalue = BE16_P(val_ptr);
  DEBUG_ECHOLNPGM("handleTouchScreenStandbyTimeSetting: ", newvalue);
  lcd_settings.standby_time_seconds = newvalue;
  setTouchScreenConfiguration();

  requestSaveSettings();
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::handleToggleTouchScreenStandbySetting(DGUS_VP_Variable &var, void *val_ptr) {
  lcd_settings.display_standby = !lcd_settings.display_standby;
  setTouchScreenConfiguration();

  requestSaveSettings();
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::handleFanToggle() {
  #if HAS_FAN
    fans[0].speed = fans[0].speed > 0 ? 0 : 255;
  #endif
  forceCompleteUpdate();
}

//
// Navigation
//

void DGUSScreenHandlerCreality::updateNewScreen(const DGUS_ScreenID screenID, const bool popup/*=false*/) {
  DEBUG_ECHOLNPGM("SetNewScreen: ", screenID);

  // Remember the current screen, unless it's a popup
  if (!popup && current_screenID != DGUS_SCREEN_POPUP && current_screenID != DGUS_SCREEN_CONFIRM) {
    memmove(&past_screenIDs[1], &past_screenIDs[0], sizeof(past_screenIDs) - 1);
    past_screenIDs[0] = current_screenID;
  }

  current_screenID = screenID;
  skipVP = 0;
  forceCompleteUpdate();
}

void DGUSScreenHandlerCreality::popToOldScreen() {
  DEBUG_ECHOLNPGM("popToOldScreen s=", past_screenIDs[0]);

  if (past_screenIDs[0] != 0) {
    gotoScreen(past_screenIDs[0], true);
    memmove(&past_screenIDs[0], &past_screenIDs[1], sizeof(past_screenIDs) - 1);
    past_screenIDs[sizeof(past_screenIDs) - 1] = DGUS_SCREEN_MAIN;
  }
  else
    gotoScreen(isPrintJobActive() ? DGUS_SCREEN_PRINT_RUNNING : DGUS_SCREEN_MAIN, true);
}

void DGUSScreenHandlerCreality::onBackButton(DGUS_VP_Variable &var, void *val_ptr) {
  // If we're busy: ignore
  if (hasSynchronousOperation) return;

  // Pop back
  const uint16_t button_value = BE16_P(val_ptr);

  popToOldScreen();

  // Handle optional save from back button
  if (button_value == GENERIC_BACK_BUTTON_NEED_SAVE) requestSaveSettings();
}

void DGUSScreenHandlerCreality::updateScreenVPData() {
  if (!dgus.isInitialized()) return;
  DGUSScreenHandler::updateScreenVPData();
}

void DGUSScreenHandlerCreality::gotoScreen(const DGUS_ScreenID screenID, const bool popup/*=false*/) {
  if (current_screenID == screenID) return; // Ignore this request

  DEBUG_ECHOLNPGM("Issuing command to go to screen: ", screenID);
  dgus.requestScreen(screenID);
  updateNewScreen(screenID, popup);
}

bool DGUSScreenHandlerCreality::loop() {
  dgus.loop();

  // Injected commands stay in the queue until the last one has finished
  if (queueDoneFunc && !ExtUI::commandsInQueue()) {
    const queue_done_func_t func = queueDoneFunc;
    queueDoneFunc = nullptr;
    func();
  }

  handleScreenVersionMismatchLEDFlash();

  const millis_t ms = millis();
  static millis_t next_event_ms = 0;

  if (!isScreenComplete() || ELAPSED(ms, next_event_ms)) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;
    updateScreenVPData();
  }

  if (dgus.isInitialized()) {
    static bool booted = false;

    if (!booted) {
      static int16_t last_percentage = -1;
      if (last_percentage < 0) setStatusMessage(GET_TEXT_F(WELCOME_MSG)); // Once, not on every loop

      const int16_t percentage = _MIN(100, int16_t(100.0f * ms / (BOOTSCREEN_TIMEOUT)));
      if (percentage != last_percentage) {
        last_percentage = percentage;
        dgus.writeVariable(VP_STARTPROGRESSBAR, percentage);
      }
    }

    if (!booted && TERN0(POWER_LOSS_RECOVERY, recovery.valid())) {
      booted = true;
      DEBUG_ECHOLNPGM("Power loss recovery...");
    }

    if (!booted && ELAPSED(ms, BOOTSCREEN_TIMEOUT)) {
      booted = true;

      #if HAS_COLOR_LEDS && !HAS_COLOR_LEDS_PREFERENCES
        leds.set_default();
      #endif

      // Ensure to pick up the settings
      setTouchScreenConfiguration();

      // Set initial leveling status
      TERN_(HAS_MESH, initMeshValues());

      // No disabled back button
      setSynchronousOperationFinish();

      // Ask for the screen version - handleScreenVersion will act
      dgus.readVariable(VP_UI_VERSION_MAJOR);

      // Main menu
      gotoScreen(DGUS_SCREEN_MAIN);
    }
  }

  return isScreenComplete();
}

#endif // DGUS_LCD_UI_CREALITY_TOUCH
