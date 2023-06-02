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

bool DGUSScreenHandler::settings_ready = false;
bool DGUSScreenHandler::booted = false;

DGUS_Screen DGUSScreenHandler::current_screen = DGUS_Screen::BOOT;
DGUS_Screen DGUSScreenHandler::new_screen = DGUS_Screen::BOOT;
DGUS_Screen DGUSScreenHandler::wait_return_screen = DGUS_Screen::BOOT;
DGUS_Screen DGUSScreenHandler::confirm_return_screen = DGUS_Screen::BOOT;
bool DGUSScreenHandler::full_update = false;
uint8_t DGUSScreenHandler::angry_beeps = 0;

const char* DGUSScreenHandler::sdPrintFilename = nullptr;
char DGUSScreenHandler::homeStatusMessage[128];

#if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
  ssize_t DGUSScreenHandler::currentScrollIndex = 0;
  size_t DGUSScreenHandler::pageMaxStringLen = 0;
  size_t DGUSScreenHandler::pageMaxControlLen = 0;
#endif

DGUSScreenHandler::eeprom_data_t DGUSScreenHandler::config = {};

uint16_t DGUSScreenHandler::currentMeshPointIndex = 0;
bool DGUSScreenHandler::isLeveling = false;

millis_t DGUSScreenHandler::status_expire = 0;
millis_t DGUSScreenHandler::eeprom_save = 0;

void DGUSScreenHandler::init() {
  dgus_display.init();
  moveToScreen(DGUS_Screen::BOOT, true);
}

void DGUSScreenHandler::ready() {
  dgus_display.playSound(1);
}

void DGUSScreenHandler::loop() {
  const millis_t ms = ExtUI::safe_millis();
  static millis_t next_event_ms = 0, next_beep_ms = 0;
  static bool wasLeveling = isLeveling;

  if (new_screen != DGUS_Screen::BOOT) {
    const DGUS_Screen screen = new_screen;
    new_screen = DGUS_Screen::BOOT;

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

  if (!booted && current_screen == DGUS_Screen::HOME) {
    // Boot complete
    booted = true;
    dgus_display.readVersions();
    return;
  }

  if (ELAPSED(ms, next_event_ms) || full_update) {
    next_event_ms = ms + (booted ? DGUS_UPDATE_INTERVAL_MS : 50);

    #if ENABLED(DGUS_SOFTWARE_AUTOSCROLL)
      currentScrollIndex += 1;
      if (currentScrollIndex > (ssize_t)(pageMaxStringLen - pageMaxControlLen) + DGUS_AUTOSCROLL_END_CYCLES)
        currentScrollIndex = -DGUS_AUTOSCROLL_START_CYCLES;
    #endif

    if (angry_beeps) {
      --angry_beeps;
      dgus_display.playSound(3);
    }

    if (!sendScreenVPData(current_screen, full_update))
      DEBUG_ECHOLNPGM("SendScreenVPData failed");

    return;
  }

  if (ELAPSED(ms, next_beep_ms)) {
    next_beep_ms = ms + 300;

    if (angry_beeps) {
      --angry_beeps;
      dgus_display.playSound(0, 500/8, 100);
    }
  }

  if (wasLeveling && !isLeveling) {
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      if (!ExtUI::getMeshValid())
        ExtUI::injectCommands(F("G29 S1\nG29 P3\nG29 S0"));
      else
        ExtUI::injectCommands(F("G29 S0"));
    #endif

    config.levelingEnabled = ExtUI::getMeshValid();
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

  dgus_display.loop();
}

void DGUSScreenHandler::printerKilled(FSTR_P const error, FSTR_P const component) {
  dgus_display.playSound(0, (uint8_t)(3000/8), 100);
  setStatusMessage(error);
  moveToScreen(DGUS_Screen::ABNORMAL);
}

void DGUSScreenHandler::userConfirmRequired(const char * const msg) {
  #if ENABLED(DGUS_USERCONFIRM)
    if (confirm_return_screen == DGUS_Screen::BOOT)
      confirm_return_screen = getCurrentScreen();

    strcpy(dgus_sdcard_handler.filenames[0], msg);
    strcpy(dgus_sdcard_handler.filenames[1], "");
    strcpy(dgus_sdcard_handler.filenames[2], "");
    strcpy(dgus_sdcard_handler.filenames[3], "");

    strcpy(dgus_sdcard_handler.filenames[4], "[");
    strcat(dgus_sdcard_handler.filenames[4], GET_TEXT(MSG_BUTTON_CONFIRM));
    strcat(dgus_sdcard_handler.filenames[4], "]");

    new_screen = DGUS_Screen::FILE1;
    #ifdef DEBUG_DGUSLCD
      DEBUG_ECHOPGM("trig confirm: ");
      DEBUG_ECHO(msg);
      DEBUG_ECHOPGM(", ret: ");
      DEBUG_DECIMAL((uint8_t)confirm_return_screen);
      DEBUG_EOL();
    #endif
  #else
    UNUSED(msg);
  #endif
}

void DGUSScreenHandler::userConfirmation() {
  #if ENABLED(DGUS_USERCONFIRM)
    if (confirm_return_screen == DGUS_Screen::BOOT) {
      DEBUG_ECHOLNPGM("DGUS: User confirmation triggered but no return screen");
      return;
    }

    if (confirm_return_screen >= DGUS_Screen::FILE1 && confirm_return_screen <= DGUS_Screen::FILE4)
      dgus_sdcard_handler.onPageLoad(DGUS_SCREEN_TO_PAGE(confirm_return_screen));

    #ifdef DEBUG_DGUSLCD
      DEBUG_ECHOPGM("trig confirmed, ret:");
      DEBUG_DECIMAL((uint8_t)confirm_return_screen);
      DEBUG_EOL();
    #endif

    new_screen = confirm_return_screen;
    confirm_return_screen = DGUS_Screen::BOOT;
    ExtUI::setUserConfirmed();
  #endif
}

void DGUSScreenHandler::settingsReset() {
  config.initialized = true;
  config.volume = DGUS_DEFAULT_VOLUME;
  config.brightness = DGUS_DEFAULT_BRIGHTNESS;
  config.language = DGUS_Data::Language::Default;
  config.jogLength = DGUS_Data::AxisControlCommand::Jog_1mm;
  config.plaExtruderTemp = 200;
  config.plaBedTemp = 60;
  config.absExtruderTemp = 210;
  config.absBedTemp = 80;
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
  config.volume = dgus_display.getVolume();
  config.brightness = dgus_display.getBrightness();

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
  dgus_display.setVolume(config.volume);
  dgus_display.setBrightness(config.brightness);
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
  UNUSED(duration);

  if (frequency >= 1 && frequency <= 255) {
    if (duration >= 1 && duration <= 255)
      dgus_display.playSound((uint8_t)frequency, (uint8_t)duration);
    else
      dgus_display.playSound((uint8_t)frequency);
  }
}

void DGUSScreenHandler::angryBeeps(const uint8_t beepCount) {
  angry_beeps = beepCount;
}

void DGUSScreenHandler::levelingStart() {
  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOLNPGM("levelingStart()");
  #endif
  isLeveling = true;
  currentMeshPointIndex = 0;
  triggerFullUpdate();
}

void DGUSScreenHandler::levelingEnd() {
  if (!isLeveling) return;

  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOPGM("levelingEnd(), valid=");
    DEBUG_DECIMAL(ExtUI::getMeshValid());
    DEBUG_EOL();
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
  triggerScreenChange(DGUS_Screen::PAUSE);
}

void DGUSScreenHandler::printTimerPaused() {
  dgus_display.playSound(3);
  triggerScreenChange(DGUS_Screen::RESUME);
}

void DGUSScreenHandler::printTimerStopped() {
  dgus_display.playSound(3);
  triggerScreenChange(DGUS_Screen::FINISH);
}

void DGUSScreenHandler::filamentRunout(const ExtUI::extruder_t extruder) {
  triggerScreenChange(DGUS_Screen::FILAMENTUSEUP);
  dgus_display.playSound(3);
}

ssize_t DGUSScreenHandler::getScrollIndex() {
  return currentScrollIndex;
}

void DGUSScreenHandler::addCurrentPageStringLength(size_t stringLength, size_t textControlLength) {
  if (stringLength > pageMaxStringLen)
    pageMaxStringLen = stringLength;
  if (textControlLength > pageMaxControlLen)
    pageMaxControlLen = textControlLength;
}

#if HAS_MEDIA
  void DGUSScreenHandler::sdCardInserted() {
  }

  void DGUSScreenHandler::sdCardRemoved() {
    if (getCurrentScreen() >= DGUS_Screen::FILE1
      && getCurrentScreen() <= DGUS_Screen::FILE4) {
      triggerTempScreenChange(DGUS_Screen::SDCARDCHECK, DGUS_Screen::HOME);
    }
  }

  void DGUSScreenHandler::sdCardError() {
  }
#endif // HAS_MEDIA

#if ENABLED(POWER_LOSS_RECOVERY)
  void DGUSScreenHandler::powerLossResume() {
    //moveToScreen(DGUS_Screen::POWERLOSS, true);
  }
#endif

#if HAS_PID_HEATING
  void DGUSScreenHandler::pidTuning(const ExtUI::result_t rst) {
    dgus_display.playSound(3);
  }
#endif

void DGUSScreenHandler::steppersStatusChanged(bool steppersEnabled) {
  refreshVP(DGUS_Addr::AXIS_StepperStatus);
}

void DGUSScreenHandler::homingDone() {
  if (isOnTempScreen(DGUS_Screen::AUTOHOME))
    triggerReturnScreen();
}

void DGUSScreenHandler::startPrintFromSD(const char* const filename) {
  triggerScreenChange(DGUS_Screen::HOME);
  sdPrintFilename = filename;
  setStatusMessage(sdPrintFilename, 0);
}

void DGUSScreenHandler::setStatusMessage(FSTR_P msg, const millis_t duration) {
  setStatusMessage(FTOP(msg), duration);
}

void DGUSScreenHandler::setStatusMessage(const char* msg, const millis_t duration) {
  homeStatusMessage[0] = '\0';
  strncat(homeStatusMessage, msg, sizeof(homeStatusMessage)/sizeof(char)-1);

  if (duration)
    status_expire = ExtUI::safe_millis() + duration;
  else
    status_expire = 0;
}

DGUS_Screen DGUSScreenHandler::getCurrentScreen() {
  return current_screen;
}

void DGUSScreenHandler::homeThenChangeScreen(DGUS_Screen screen) {
  triggerTempScreenChange(DGUS_Screen::AUTOHOME, screen);
  ExtUI::injectCommands(F("G28"));
}

void DGUSScreenHandler::triggerScreenChange(DGUS_Screen screen) {
  if (confirm_return_screen != DGUS_Screen::BOOT)
    confirm_return_screen = screen;
  else
    new_screen = screen;
  wait_return_screen = DGUS_Screen::BOOT; // cancel temp screen

  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOPGM("trig scr: ");
    DEBUG_DECIMAL((uint8_t)screen);
    DEBUG_EOL();
  #endif
}

void DGUSScreenHandler::triggerTempScreenChange(DGUS_Screen screen, DGUS_Screen returnScreen) {
  if (confirm_return_screen != DGUS_Screen::BOOT)
    confirm_return_screen = screen;
  else
    new_screen = screen;
  wait_return_screen = returnScreen;

  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOPGM("trig tmp: ");
    DEBUG_DECIMAL((uint8_t)screen);
    DEBUG_ECHOPGM(" ret: ");
    DEBUG_DECIMAL((uint8_t)returnScreen);
    DEBUG_EOL();
  #endif
}

void DGUSScreenHandler::triggerReturnScreen() {
  new_screen = wait_return_screen;
  wait_return_screen = DGUS_Screen::BOOT;
  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOPGM("trig ret scr");
    DEBUG_EOL();
  #endif
}

bool DGUSScreenHandler::isOnTempScreen(DGUS_Screen screen) {
  return wait_return_screen != DGUS_Screen::BOOT
    && (screen == DGUS_Screen::BOOT || current_screen == screen);
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

const DGUS_Addr* DGUSScreenHandler::findScreenAddrList(DGUS_Screen screen) {
  DGUS_ScreenAddrList list;
  const DGUS_ScreenAddrList *map = screen_addr_list_map;

  do {
    memcpy_P(&list, map, sizeof(*map));
    if (!list.addr_list) break;
    if (list.screen == screen) {
      return list.addr_list;
    }
  } while (++map);

  return nullptr;
}

bool DGUSScreenHandler::callScreenSetup(DGUS_Screen screen) {
  DGUS_ScreenSetup setup;
  const DGUS_ScreenSetup *list = screen_setup_list;

  do {
    memcpy_P(&setup, list, sizeof(*list));
    if (!setup.setup_fn) break;
    if (setup.screen == screen) return setup.setup_fn();
  } while (++list);

  return true;
}

void DGUSScreenHandler::moveToScreen(DGUS_Screen screen, bool abort_wait) {
  current_screen = screen;

  if (!callScreenSetup(screen)) return;
  if (!sendScreenVPData(screen, true)) return;

  dgus_display.switchScreen(current_screen);
}

bool DGUSScreenHandler::sendScreenVPData(DGUS_Screen screen, bool complete_update) {
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

    while (expected_tx > dgus_display.getFreeTxBuffer()) {
      if (ELAPSED(ExtUI::safe_millis(), try_until)) return false; // Stop trying after 1 second

      dgus_display.flushTx(); // Flush the TX buffer
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

#endif // DGUS_LCD_UI_RELOADED
