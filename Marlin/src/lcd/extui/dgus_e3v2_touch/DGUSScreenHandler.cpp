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

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"
#include "DGUSSDCardHandler.h"

#include "definition/DGUS_ScreenAddrList.h"
#include "definition/DGUS_ScreenSetup.h"

#include "../../../gcode/queue.h"

//#define DGUS_SCREEN_PAGE_DEBUG // uncomment to debug page changes

DGUSScreenHandler::eeprom_data_t DGUSScreenHandler::config = {};
uint16_t DGUSScreenHandler::currentMeshPointIndex = 0;
bool DGUSScreenHandler::isLeveling = false;
float DGUSScreenHandler::filamentLength = 10;
char DGUSScreenHandler::statusMessage[DGUS_ERRORSTRING_LEN + 1];

bool DGUSScreenHandler::settings_ready = false;
bool DGUSScreenHandler::booted = false;
bool DGUSScreenHandler::stop_requested = false;

DGUS_ScreenID DGUSScreenHandler::current_screen = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::new_screen = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::wait_return_screen = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::confirm_return_screen = DGUS_ScreenID::BOOT;
bool DGUSScreenHandler::full_update = false;
uint8_t DGUSScreenHandler::angry_beeps = 0;

#if ENABLED(POWER_LOSS_RECOVERY)
  bool DGUSScreenHandler::powerLossRecoveryAvailable = false;
#endif

millis_t DGUSScreenHandler::eeprom_save = 0;

void DGUSScreenHandler::init() {
  dgus.init();
  if (booted)
    triggerFullUpdate(); // Reinit LCD hardware then refresh current screen
  else
    moveToScreen(DGUS_ScreenID::BOOT, true);
}

void DGUSScreenHandler::ready() {
  dgus.playSound(1);
}

void DGUSScreenHandler::loop() {
  const millis_t ms = ExtUI::safe_millis();
  static millis_t next_event_ms = 0, next_beep_ms = 0;
  static bool wasLeveling = isLeveling;

  if (new_screen != DGUS_ScreenID::BOOT) {
    const DGUS_ScreenID screen = new_screen;
    new_screen = DGUS_ScreenID::BOOT;

    if (current_screen == screen)
      triggerFullUpdate();
    else
      moveToScreen(screen);
    return;
  }

  if (!booted && current_screen == DGUS_ScreenID::HOME) {
    // Boot complete
    booted = true;
    dgus.readVersions();
    return;
  }

  #if HAS_FILAMENT_SENSOR
    // Like the stock firmware, offer to resume once filament is back in
    if (current_screen == DGUS_ScreenID::FILAMENT_RUNOUT
      && !(ExtUI::getFilamentRunoutEnabled() && READ(FIL_RUNOUT1_PIN) == FIL_RUNOUT1_STATE)
    ) triggerScreenChange(DGUS_ScreenID::FILAMENT_INSERT);
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (booted && powerLossRecoveryAvailable) {
      triggerScreenChange(DGUS_ScreenID::POWERCONTINUE);
      powerLossRecoveryAvailable = false;
    }
  #endif

  if (ELAPSED(ms, next_event_ms) || full_update) {
    next_event_ms = ms + (booted ? DGUS_UPDATE_INTERVAL_MS : 30);

    if (!sendScreenVPData(current_screen, full_update))
      DEBUG_ECHOLNPGM("SendScreenVPData failed");

    return;
  }

  if (ELAPSED(ms, next_beep_ms)) {
    next_beep_ms = ms + 300;

    if (angry_beeps) {
      --angry_beeps;
      dgus.playSound(0, 500/8, 100);
    }
  }

  if (wasLeveling && !isLeveling) {
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      ExtUI::injectCommands(ExtUI::getLevelingIsValid() ? F("G29 S0") : F("G29 S1\nG29 P3\nG29 S0"));
    #endif

    #if HAS_LEVELING
      config.levelingEnabled = ExtUI::getLevelingIsValid();
      ExtUI::setLevelingActive(config.levelingEnabled);
    #endif
    triggerEEPROMSave();
  }
  wasLeveling = isLeveling;

  if (!settings_ready && booted) return;

  if (eeprom_save > 0 && ELAPSED(ms, eeprom_save) && isPrinterIdle()) {
    eeprom_save = 0;
    ExtUI::injectCommands(F("M500"));
    return;
  }

  dgus.loop();
}

void DGUSScreenHandler::printerKilled(FSTR_P const error, FSTR_P const component) {
  dgus.playSound(0, (uint8_t)(3000/8), 100);

  DGUS_ScreenID errorScreen = DGUS_ScreenID::ABNORMAL;
  if (error == GET_TEXT_F(MSG_ERR_THERMAL_RUNAWAY))
    errorScreen = DGUS_ScreenID::THERMAL_RUNAWAY;
  else if (error == GET_TEXT_F(MSG_ERR_HEATING_FAILED))
    errorScreen = DGUS_ScreenID::HEATING_FAILED;
  else if (error == GET_TEXT_F(MSG_ERR_MAXTEMP) || error == GET_TEXT_F(MSG_ERR_MINTEMP))
    errorScreen = DGUS_ScreenID::THERMISTOR_ERROR;

  setStatusMessage(error);
  moveToScreen(errorScreen);
}

/**
 * The stock firmware has no prompt page, so reuse the "abnormal" page:
 * the message goes in its text box and its OK button confirms.
 */
void DGUSScreenHandler::userConfirmRequired(const char * const msg) {
  if (confirm_return_screen == DGUS_ScreenID::BOOT)
    confirm_return_screen = getCurrentScreen();

  setStatusMessage(msg);
  new_screen = DGUS_ScreenID::ABNORMAL;

  #if ALL(DEBUG_OUT, DGUS_SCREEN_PAGE_DEBUG)
    DEBUG_ECHOLNPGM("trig confirm: ", msg, ", ret: ", (uint16_t)confirm_return_screen);
  #endif
}

void DGUSScreenHandler::userConfirmation() {
  if (confirm_return_screen == DGUS_ScreenID::BOOT) {
    DEBUG_ECHOLNPGM("DGUS: User confirmation triggered but no return screen");
    return;
  }

  #if ENABLED(DEBUG_DGUSLCD)
    DEBUG_ECHOLNPGM("trig confirmed, ret:", (uint16_t)confirm_return_screen);
  #endif

  new_screen = confirm_return_screen;
  confirm_return_screen = DGUS_ScreenID::BOOT;
  ExtUI::setUserConfirmed();
}

void DGUSScreenHandler::settingsReset() {
  config.initialized = true;
  config.volume = DGUS_DEFAULT_VOLUME;
  config.brightness = DGUS_DEFAULT_BRIGHTNESS;
  config.language = DGUS_Data::Language::Default;
  config.plaExtruderTemp = PREHEAT_1_TEMP_HOTEND;
  config.plaBedTemp = PREHEAT_1_TEMP_BED;
  config.plaFanSpeed = PREHEAT_1_FAN_SPEED;
  config.absExtruderTemp = PREHEAT_2_TEMP_HOTEND;
  config.absBedTemp = PREHEAT_2_TEMP_BED;
  config.absFanSpeed = PREHEAT_2_FAN_SPEED;
  config.levelingEnabled = TERN0(HAS_LEVELING, ExtUI::getLevelingActive());

  if (!settings_ready) {
    settings_ready = true;
    ready();
  }
}

void DGUSScreenHandler::storeSettings(char *buff) {
  static_assert(sizeof(config) <= ExtUI::eeprom_data_size, "sizeof(eeprom_data_t) > eeprom_data_size.");

  config.initialized = true;
  config.volume = dgus.getVolume();
  config.brightness = dgus.getBrightness();

  memcpy(buff, &config, sizeof(config));
}

void DGUSScreenHandler::loadSettings(const char *buff) {
  static_assert(sizeof(config) <= ExtUI::eeprom_data_size, "sizeof(eeprom_data_t) > eeprom_data_size.");
  memcpy(&config, buff, sizeof(config));

  if (!config.initialized
    || config.language < DGUS_Data::Language::Chinese_Simplified
    || config.language > DGUS_Data::Language::Turkish) {
    DEBUG_ECHOLNPGM("invalid DGUS settings, resetting");
    settingsReset();
  }

  TERN_(HAS_LEVELING, ExtUI::setLevelingActive(config.levelingEnabled));
  dgus.setVolume(config.volume);
  dgus.setBrightness(config.brightness);
}

void DGUSScreenHandler::configurationStoreWritten(bool success) {
  if (!success) angryBeeps(2);
}

void DGUSScreenHandler::configurationStoreRead(bool success) {
  if (!success)
    angryBeeps(2);
  else if (!settings_ready) {
    settings_ready = true;
    ready();
  }
}

void DGUSScreenHandler::playTone(const uint16_t frequency, const uint16_t duration/*=0*/) {
  if (WITHIN(frequency, 1, 255)) {
    if (WITHIN(duration, 1, 255))
      dgus.playSound((uint8_t)frequency, (uint8_t)duration);
    else
      dgus.playSound((uint8_t)frequency);
  }
}

void DGUSScreenHandler::angryBeeps(const uint8_t beepCount) {
  angry_beeps = beepCount;
}

void DGUSScreenHandler::levelingStart() {
  isLeveling = true;
  currentMeshPointIndex = 0;
  triggerScreenChange(DGUS_ScreenID::LEVELING);
}

void DGUSScreenHandler::levelingEnd() {
  if (!isLeveling) return;

  #if ENABLED(DEBUG_DGUSLCD)
    DEBUG_ECHOLNPGM("levelingEnd(), valid=", ExtUI::getLevelingIsValid());
  #endif

  isLeveling = false;
  triggerFullUpdate();
}

void DGUSScreenHandler::meshUpdate(const int8_t xpos, const int8_t ypos) {
  if (!isLeveling) return;

  currentMeshPointIndex++;
  refreshVP(DGUS_Addr::LEVELING_Progress_Icon);
}

void DGUSScreenHandler::printTimerStarted() {
  stop_requested = false;
  TERN_(HAS_FILAMENT_SENSOR, ExtUI::setFilamentRunoutState(false));
  triggerScreenChange(DGUS_ScreenID::PRINTING);
}

void DGUSScreenHandler::printTimerPaused() {
  dgus.playSound(3);
  if (!isOnUserConfirmationScreen() && getCurrentScreen() != DGUS_ScreenID::FILAMENT_RUNOUT)
    triggerScreenChange(DGUS_ScreenID::PAUSED);
}

void DGUSScreenHandler::printTimerStopped() {
  dgus.playSound(3);
  triggerScreenChange(stop_requested ? DGUS_ScreenID::MAIN : DGUS_ScreenID::FINISH);
  stop_requested = false;
}

void DGUSScreenHandler::stopPrint() {
  stop_requested = true;
  ExtUI::stopPrint();
  TERN_(HAS_FILAMENT_SENSOR, ExtUI::setFilamentRunoutState(false));
  triggerScreenChange(DGUS_ScreenID::MAIN);
}

void DGUSScreenHandler::filamentRunout(const ExtUI::extruder_t extruder) {
  dgus.playSound(3);
  // With ADVANCED_PAUSE_FEATURE the runout script (M600) prompts through userConfirmRequired
  #if DISABLED(ADVANCED_PAUSE_FEATURE)
    triggerScreenChange(DGUS_ScreenID::FILAMENT_RUNOUT);
  #endif
}

#if HAS_MEDIA

  void DGUSScreenHandler::sdCardInserted() {}

  void DGUSScreenHandler::sdCardRemoved() {
    dgus_sdcard_handler.reset();
    if (WITHIN(getCurrentScreen(), DGUS_ScreenID::FILE1, DGUS_ScreenID::FILE5))
      triggerScreenChange(DGUS_ScreenID::MAIN);
  }

  void DGUSScreenHandler::sdCardError() {}

#endif // HAS_MEDIA

#if ENABLED(POWER_LOSS_RECOVERY)
  void DGUSScreenHandler::powerLossResume() {
    powerLossRecoveryAvailable = true;
  }
#endif

#if HAS_PID_HEATING
  void DGUSScreenHandler::pidTuning(const ExtUI::pidresult_t rst) {
    dgus.playSound(3);
  }
#endif

void DGUSScreenHandler::steppersStatusChanged(bool steppersEnabled) {
  refreshVP(DGUS_Addr::AXIS_StepperStatus);
}

void DGUSScreenHandler::homingDone() {
  if (isOnTempScreen(DGUS_ScreenID::AUTOHOME))
    triggerReturnScreen();
}

// The status text is shown only on the "abnormal" page (errors and prompts)
void DGUSScreenHandler::setStatusMessage(FSTR_P msg) {
  statusMessage[0] = '\0';
  if (msg) strncat_P(statusMessage, FTOP(msg), sizeof(statusMessage) - 1);
  refreshVP(DGUS_Addr::ABNORMAL_StatusMessage);
}

void DGUSScreenHandler::setStatusMessage(const char* msg) {
  statusMessage[0] = '\0';
  if (msg) strncat(statusMessage, msg, sizeof(statusMessage) - 1);
  refreshVP(DGUS_Addr::ABNORMAL_StatusMessage);
}

DGUS_ScreenID DGUSScreenHandler::getCurrentScreen() { return current_screen; }

// The screen shown for a running job: printing or paused
DGUS_ScreenID DGUSScreenHandler::getPrintScreen() {
  return ExtUI::isPrintingPaused() ? DGUS_ScreenID::PAUSED : DGUS_ScreenID::PRINTING;
}

void DGUSScreenHandler::homeThenChangeScreen(DGUS_ScreenID screen) {
  triggerTempScreenChange(DGUS_ScreenID::AUTOHOME, screen);
  ExtUI::injectCommands(F("G28"));
}

void DGUSScreenHandler::triggerScreenChange(DGUS_ScreenID screen) {
  if (confirm_return_screen != DGUS_ScreenID::BOOT)
    confirm_return_screen = screen;
  else
    new_screen = screen;
  wait_return_screen = DGUS_ScreenID::BOOT; // cancel temp screen

  #if ALL(DEBUG_OUT, DGUS_SCREEN_PAGE_DEBUG)
    DEBUG_ECHOLNPGM("trig scr: ", (uint16_t)screen);
  #endif
}

void DGUSScreenHandler::triggerTempScreenChange(DGUS_ScreenID screen, DGUS_ScreenID returnScreen) {
  if (confirm_return_screen != DGUS_ScreenID::BOOT)
    confirm_return_screen = screen;
  else
    new_screen = screen;
  wait_return_screen = returnScreen;

  #if ALL(DEBUG_OUT, DGUS_SCREEN_PAGE_DEBUG)
    DEBUG_ECHOLNPGM("trig tmp: ", (uint16_t)screen, " ret: ", (uint16_t)returnScreen);
  #endif
}

void DGUSScreenHandler::triggerReturnScreen() {
  new_screen = wait_return_screen;
  wait_return_screen = DGUS_ScreenID::BOOT;
  #if ALL(DEBUG_OUT, DGUS_SCREEN_PAGE_DEBUG)
    DEBUG_ECHOLNPGM("trig ret scr");
  #endif
}

bool DGUSScreenHandler::isOnTempScreen(DGUS_ScreenID screen) {
  return wait_return_screen != DGUS_ScreenID::BOOT
    && (screen == DGUS_ScreenID::BOOT || current_screen == screen);
}

void DGUSScreenHandler::triggerFullUpdate() {
  full_update = true;
}

void DGUSScreenHandler::triggerEEPROMSave() {
  eeprom_save = ExtUI::safe_millis() + 500;
}

bool DGUSScreenHandler::isPrinterIdle() {
  return (!ExtUI::commandsInQueue() && !ExtUI::isMoving());
}

const DGUS_Addr* DGUSScreenHandler::findScreenAddrList(DGUS_ScreenID screen) {
  DGUS_ScreenAddrList list;
  const DGUS_ScreenAddrList *map = screen_addr_list_map;

  do {
    memcpy_P(&list, map, sizeof(*map));
    if (!list.addr_list) break;
    if (list.screen == screen) return list.addr_list;
  } while (++map);

  return nullptr;
}

bool DGUSScreenHandler::callScreenSetup(DGUS_ScreenID screen) {
  DGUS_ScreenSetup setup;
  const DGUS_ScreenSetup *list = screen_setup_list;

  do {
    memcpy_P(&setup, list, sizeof(*list));
    if (!setup.setup_fn) break;
    if (setup.screen == screen) return setup.setup_fn();
  } while (++list);

  return true;
}

void DGUSScreenHandler::moveToScreen(DGUS_ScreenID screen, bool abort_wait) {
  current_screen = screen;

  if (!callScreenSetup(screen)) return;
  if (!sendScreenVPData(screen, true)) return;

  dgus.switchScreen(current_screen);
}

bool DGUSScreenHandler::sendScreenVPData(DGUS_ScreenID screen, bool complete_update) {
  if (complete_update) full_update = false;

  const DGUS_Addr *list = findScreenAddrList(screen);

  while (true) {
    if (!list) return true; // Nothing left to send

    const uint16_t addr = pgm_read_word(list++);
    if (!addr) return true; // Nothing left to send

    DGUS_VP vp;
    if (!DGUS_PopulateVP((DGUS_Addr)addr, &vp)) continue; // Invalid VP
    if (!vp.tx_handler) continue; // Nothing to send
    if (!complete_update && !(vp.flags & VPFLAG_AUTOUPLOAD)) continue; // Unnecessary VP

    uint8_t expected_tx = 6 + vp.size; // 6 bytes header + payload.
    const millis_t try_until = ExtUI::safe_millis() + 1000;

    while (expected_tx > dgus.getFreeTxBuffer()) {
      if (ELAPSED(ExtUI::safe_millis(), try_until)) return false; // Stop trying after 1 second

      dgus.flushTx(); // Flush the TX buffer
      delay(50);
    }

    vp.tx_handler(vp);
  }
}

bool DGUSScreenHandler::refreshVP(DGUS_Addr vpAddr) {
  const DGUS_Addr *list = findScreenAddrList(current_screen);

  while (list && (uint16_t)pgm_read_word(list)) {
    if ((DGUS_Addr)pgm_read_word(list) == vpAddr) {
      DGUS_VP vp;
      if (!DGUS_PopulateVP((DGUS_Addr)vpAddr, &vp) || !vp.tx_handler)
        return false;

      vp.tx_handler(vp);
      return true;
    }

    list++;
  }

  return false;
}

#endif // DGUS_LCD_UI_E3V2_TOUCH
