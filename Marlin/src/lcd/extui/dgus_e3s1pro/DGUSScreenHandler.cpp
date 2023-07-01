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

#include "DGUSDisplay.h"
#include "DGUSScreenHandler.h"
#include "DGUSSDCardHandler.h"

#include "definition/DGUS_ScreenAddrList.h"
#include "definition/DGUS_ScreenSetup.h"

#include "../../../gcode/queue.h"

#define DGUS_SCREEN_PAGE_DEBUG // uncomment to debug page changes

DGUSScreenHandler::eeprom_data_t DGUSScreenHandler::config = {};
uint16_t DGUSScreenHandler::currentMeshPointIndex = 0;
bool DGUSScreenHandler::isLeveling = false;
char DGUSScreenHandler::homeStatusMessage[128];

bool DGUSScreenHandler::settings_ready = false;
bool DGUSScreenHandler::booted = false;

DGUS_ScreenID DGUSScreenHandler::current_screen = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::new_screen = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::wait_return_screen = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::confirm_return_screen = DGUS_ScreenID::BOOT;
bool DGUSScreenHandler::full_update = false;
uint8_t DGUSScreenHandler::angry_beeps = 0;

#if HAS_MEDIA
  static const char* const noFileSelected = "";
  const char* DGUSScreenHandler::sdPrintFilename = noFileSelected;
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  bool DGUSScreenHandler::powerLossRecoveryAvailable = false;
#endif

#if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
  ssize_t DGUSScreenHandler::currentScrollIndex = 0;
  size_t DGUSScreenHandler::pageMaxStringLen = 0;
  size_t DGUSScreenHandler::pageMaxControlLen = 0;
#endif

millis_t DGUSScreenHandler::status_expire = 0;
millis_t DGUSScreenHandler::eeprom_save = 0;

void DGUSScreenHandler::init() {
  dgus.init();
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

    #if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
      currentScrollIndex = -DGUS_AUTOSCROLL_START_CYCLES;
      pageMaxStringLen = 0;
      pageMaxControlLen = 0;
    #endif

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

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (booted && powerLossRecoveryAvailable)
    {
      triggerScreenChange(DGUS_ScreenID::POWERCONTINUE);
      powerLossRecoveryAvailable = false;
    }
  #endif

  if (ELAPSED(ms, next_event_ms) || full_update) {
    next_event_ms = ms + (booted ? DGUS_UPDATE_INTERVAL_MS : 50);

    #if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
      currentScrollIndex += 1;
      if (currentScrollIndex > (ssize_t)(pageMaxStringLen - pageMaxControlLen) + DGUS_AUTOSCROLL_END_CYCLES)
        currentScrollIndex = -DGUS_AUTOSCROLL_START_CYCLES;
    #endif

    if (angry_beeps) {
      --angry_beeps;
      dgus.playSound(3);
    }

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

    config.levelingEnabled = ExtUI::getLevelingIsValid();
    ExtUI::setLevelingActive(config.levelingEnabled);
    triggerEEPROMSave();
  }
  wasLeveling = isLeveling;

  if (!settings_ready && booted) return;

  if (status_expire > 0 && ELAPSED(ms, status_expire)) {
    setStatusMessage(FPSTR(NUL_STR), 0);
    return;
  }

  if (eeprom_save > 0 && ELAPSED(ms, eeprom_save) && isPrinterIdle()) {
    eeprom_save = 0;
    ExtUI::injectCommands(F("M500"));
    return;
  }

  dgus.loop();
}

void DGUSScreenHandler::printerKilled(FSTR_P const error, FSTR_P const component) {
  dgus.playSound(0, (uint8_t)(3000/8), 100);
  setStatusMessage(error);
  moveToScreen(DGUS_ScreenID::ABNORMAL);
}

void DGUSScreenHandler::userConfirmRequired(const char * const msg) {
  #if ENABLED(DGUS_USERCONFIRM)
    if (confirm_return_screen == DGUS_ScreenID::BOOT)
      confirm_return_screen = getCurrentScreen();

    strcpy(dgus_sdcard_handler.filenames[0], msg);
    dgus_sdcard_handler.filenames[1][0] = '\0';
    dgus_sdcard_handler.filenames[2][0] = '\0';
    dgus_sdcard_handler.filenames[3][0] = '\0';

    strcpy(dgus_sdcard_handler.filenames[4], "[");
    strcat(dgus_sdcard_handler.filenames[4], GET_TEXT(MSG_BUTTON_CONFIRM));
    strcat(dgus_sdcard_handler.filenames[4], "]");

    new_screen = DGUS_ScreenID::FILE1;
    #if ALL(DEBUG_OUT, DGUS_SCREEN_PAGE_DEBUG)
      DEBUG_ECHOLNPGM("trig confirm: ", msg, ", ret: ", (uint16_t)confirm_return_screen);
    #endif
  #else
    UNUSED(msg);
  #endif
}

void DGUSScreenHandler::userConfirmation() {
  #if ENABLED(DGUS_USERCONFIRM)
    if (confirm_return_screen == DGUS_ScreenID::BOOT) {
      DEBUG_ECHOLNPGM("DGUS: User confirmation triggered but no return screen");
      return;
    }

    if (confirm_return_screen >= DGUS_ScreenID::FILE1 && confirm_return_screen <= DGUS_ScreenID::FILE4)
      dgus_sdcard_handler.onPageLoad(DGUS_SCREEN_TO_PAGE(confirm_return_screen));

    #ifdef DEBUG_DGUSLCD
      DEBUG_ECHOLNPGM("trig confirmed, ret:", (uint16_t)confirm_return_screen);
    #endif

    new_screen = confirm_return_screen;
    confirm_return_screen = DGUS_ScreenID::BOOT;
    ExtUI::setUserConfirmed();
  #endif
}

void DGUSScreenHandler::settingsReset() {
  config.initialized = true;
  config.volume = DGUS_DEFAULT_VOLUME;
  config.brightness = DGUS_DEFAULT_BRIGHTNESS;
  config.language = DGUS_Data::Language::Default;
  config.jogLength = DGUS_Data::AxisControlCommand::Jog_1mm;
  config.plaExtruderTemp = PREHEAT_1_TEMP_HOTEND;
  config.plaBedTemp = PREHEAT_1_TEMP_BED;
  config.absExtruderTemp = PREHEAT_2_TEMP_HOTEND;
  config.absBedTemp = PREHEAT_2_TEMP_BED;
  config.levelingEnabled = ExtUI::getLevelingActive();

  if (!settings_ready) {
    settings_ready = true;
    ready();
  }

  setStatusMessage(GET_TEXT_F(DGUS_MSG_RESET_EEPROM));
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
    || config.language > DGUS_Data::Language::Turkish
    || config.jogLength < DGUS_Data::AxisControlCommand::Jog_10mm
    || config.jogLength > DGUS_Data::AxisControlCommand::Jog_0_1mm) {
    DEBUG_ECHOLNPGM("invalid DGUS settings, resetting");
    settingsReset();
  }

  ExtUI::setLevelingActive(config.levelingEnabled);
  dgus.setVolume(config.volume);
  dgus.setBrightness(config.brightness);
}

void DGUSScreenHandler::configurationStoreWritten(bool success) {
  if (!success)
    setStatusMessage(GET_TEXT_F(DGUS_MSG_WRITE_EEPROM_FAILED));
}

void DGUSScreenHandler::configurationStoreRead(bool success) {
  if (!success) {
    setStatusMessage(GET_TEXT_F(DGUS_MSG_READ_EEPROM_FAILED));
  }
  else if (!settings_ready) {
    settings_ready = true;
    ready();
  }
}

void DGUSScreenHandler::playTone(const uint16_t frequency, const uint16_t duration) {
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
  triggerFullUpdate();
}

void DGUSScreenHandler::levelingEnd() {
  if (!isLeveling) return;

  #if ENABLED(DEBUG_DGUSLCD)
    DEBUG_ECHOLNPGM("levelingEnd(), valid=", ExtUI::getLevelingIsValid());
  #endif

  isLeveling = false;
  currentMeshPointIndex = 0;
  triggerFullUpdate();
}

void DGUSScreenHandler::meshUpdate(const int8_t xpos, const int8_t ypos) {
  if (!isLeveling) return;

  currentMeshPointIndex++;
  triggerFullUpdate();
}

void DGUSScreenHandler::printTimerStarted() {
  TERN_(HAS_FILAMENT_SENSOR, ExtUI::setFilamentRunoutState(false));
  triggerScreenChange(DGUS_ScreenID::PAUSE);
}

void DGUSScreenHandler::printTimerPaused() {
  dgus.playSound(3);
  triggerScreenChange(DGUS_ScreenID::RESUME);
}

void DGUSScreenHandler::printTimerStopped() {
  dgus.playSound(3);
  triggerScreenChange(DGUS_ScreenID::FINISH);
}

void DGUSScreenHandler::filamentRunout(const ExtUI::extruder_t extruder) {
  triggerScreenChange(DGUS_ScreenID::FILAMENTUSEUP);
  dgus.playSound(3);
}

ssize_t DGUSScreenHandler::getScrollIndex() {
  return currentScrollIndex;
}

void DGUSScreenHandler::addCurrentPageStringLength(size_t stringLength, size_t textControlLength) {
  NOLESS(pageMaxStringLen, stringLength);
  NOLESS(pageMaxControlLen, textControlLength);
}

#if HAS_MEDIA

  void DGUSScreenHandler::sdCardInserted() {}

  void DGUSScreenHandler::sdCardRemoved() {
    sdPrintFilename = noFileSelected;

    if (getCurrentScreen() >= DGUS_ScreenID::FILE1
      && getCurrentScreen() <= DGUS_ScreenID::FILE4) {
      triggerTempScreenChange(DGUS_ScreenID::SDCARDCHECK, DGUS_ScreenID::HOME);
    }
  }

  void DGUSScreenHandler::sdCardError() {}

#endif // HAS_MEDIA

#if ENABLED(POWER_LOSS_RECOVERY)
  void DGUSScreenHandler::powerLossResume() {
    powerLossRecoveryAvailable = true;
  }
#endif

#if HAS_PID_HEATING
  void DGUSScreenHandler::pidTuning(const ExtUI::result_t rst) {
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

void DGUSScreenHandler::startPrintFromSD(const char* const filename) {
  triggerScreenChange(DGUS_ScreenID::HOME);
  sdPrintFilename = filename;
  setStatusMessage(sdPrintFilename, 0);
}

void DGUSScreenHandler::setStatusMessage(FSTR_P msg, const millis_t duration) {
  setStatusMessage(FTOP(msg), duration);
}

void DGUSScreenHandler::setStatusMessage(const char* msg, const millis_t duration) {
  homeStatusMessage[0] = '\0';
  strncat(homeStatusMessage, msg, sizeof(homeStatusMessage) / sizeof(char) - 1);
  status_expire = duration ? ExtUI::safe_millis() + duration : 0;
}

DGUS_ScreenID DGUSScreenHandler::getCurrentScreen() { return current_screen; }

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
    if (!complete_update
      && !(vp.flags & VPFLAG_AUTOUPLOAD)
      && TERN1(DGUS_SOFTWARE_AUTOSCROLL, !(vp.flags & VPFLAG_TXSTRING_AUTOSCROLL))
    ) continue; // Unnecessary VP

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

  while (list && (uint16_t)*list) {
    if (*list == vpAddr) {
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

#endif // DGUS_LCD_UI_E3S1PRO
