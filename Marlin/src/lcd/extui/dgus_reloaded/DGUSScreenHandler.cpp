/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if DGUS_LCD_UI_RELOADED

#include "DGUSScreenHandler.h"

#include "DGUSDisplay.h"
#include "definition/DGUS_ScreenAddrList.h"
#include "definition/DGUS_ScreenSetup.h"

#include "../../../gcode/queue.h"

uint8_t DGUSScreenHandler::debug_count = 0;

#if HAS_MEDIA
  ExtUI::FileList DGUSScreenHandler::filelist;
  uint16_t DGUSScreenHandler::filelist_offset = 0;
  int16_t DGUSScreenHandler::filelist_selected = -1;
#endif

DGUS_Data::StepSize DGUSScreenHandler::offset_steps = DGUS_Data::StepSize::MMP1;
DGUS_Data::StepSize DGUSScreenHandler::move_steps = DGUS_Data::StepSize::MM10;

uint16_t DGUSScreenHandler::probing_icons[] = { 0, 0 };

DGUS_Data::Extruder DGUSScreenHandler::filament_extruder = DGUS_Data::Extruder::CURRENT;
uint16_t DGUSScreenHandler::filament_length = DGUS_DEFAULT_FILAMENT_LEN;

char DGUSScreenHandler::gcode[] = "";

DGUS_Data::Heater DGUSScreenHandler::pid_heater = DGUS_Data::Heater::H0;
uint16_t DGUSScreenHandler::pid_temp = DGUS_PLA_TEMP_HOTEND;
uint8_t DGUSScreenHandler::pid_cycles = 5;

bool DGUSScreenHandler::settings_ready = false;
bool DGUSScreenHandler::booted = false;

DGUS_ScreenID DGUSScreenHandler::current_screenID = DGUS_ScreenID::BOOT;
DGUS_ScreenID DGUSScreenHandler::new_screenID = DGUS_ScreenID::BOOT;
bool DGUSScreenHandler::full_update = false;

DGUS_ScreenID DGUSScreenHandler::wait_return_screenID = DGUS_ScreenID::HOME;
bool DGUSScreenHandler::wait_continue = false;

bool DGUSScreenHandler::leveling_active = false;

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
  if (!settings_ready || current_screenID == DGUS_ScreenID::KILL)
    return;

  const millis_t ms = ExtUI::safe_millis();
  static millis_t next_event_ms = 0;

  if (new_screenID != DGUS_ScreenID::BOOT) {
    const DGUS_ScreenID screenID = new_screenID;
    new_screenID = DGUS_ScreenID::BOOT;
    if (current_screenID == screenID)
      triggerFullUpdate();
    else
      moveToScreen(screenID);
    return;
  }

  if (!booted && ELAPSED(ms, 3000)) {
    booted = true;

    dgus.readVersions();

    if (current_screenID == DGUS_ScreenID::BOOT)
      moveToScreen(DGUS_ScreenID::HOME);

    return;
  }

  if (ELAPSED(ms, next_event_ms) || full_update) {
    next_event_ms = ms + DGUS_UPDATE_INTERVAL_MS;

    if (!sendScreenVPData(current_screenID, full_update))
      DEBUG_ECHOLNPGM("sendScreenVPData failed");

    return;
  }

  if (current_screenID == DGUS_ScreenID::WAIT
      && ((wait_continue && !wait_for_user) || (!wait_continue && isPrinterIdle()))
  ) {
    moveToScreen(wait_return_screenID, true);
    return;
  }

  if (current_screenID == DGUS_ScreenID::LEVELING_PROBING && isPrinterIdle()) {
    dgus.playSound(3);

    setStatusMessage(ExtUI::getLevelingIsValid() ? GET_TEXT_F(DGUS_MSG_PROBING_SUCCESS) : GET_TEXT_F(DGUS_MSG_PROBING_FAILED));

    moveToScreen(DGUS_ScreenID::LEVELING_AUTOMATIC);
    return;
  }

  if (status_expire > 0 && ELAPSED(ms, status_expire)) {
    setStatusMessage(FPSTR(NUL_STR), 0);
    return;
  }

  if (eeprom_save > 0 && ELAPSED(ms, eeprom_save) && isPrinterIdle()) {
    eeprom_save = 0;
    queue.enqueue_now(F("M500"));
    return;
  }

  dgus.loop();
}

void DGUSScreenHandler::printerKilled(FSTR_P const error, FSTR_P const component) {
  setMessageLine(error, 1);
  setMessageLine(component, 2);
  setMessageLinePGM(NUL_STR, 3);
  setMessageLine(GET_TEXT_F(MSG_PLEASE_RESET), 4);

  dgus.playSound(3, 1, 200);

  moveToScreen(DGUS_ScreenID::KILL, true);
}

void DGUSScreenHandler::userConfirmRequired(const char * const msg) {
  setMessageLinePGM(NUL_STR, 1);
  setMessageLine(msg, 2);
  setMessageLinePGM(NUL_STR, 3);
  setMessageLinePGM(NUL_STR, 4);

  dgus.playSound(3);

  showWaitScreen(current_screenID, true);
}

void DGUSScreenHandler::settingsReset() {
  dgus.setVolume(DGUS_DEFAULT_VOLUME);
  dgus.setBrightness(DGUS_DEFAULT_BRIGHTNESS);

  if (!settings_ready) {
    settings_ready = true;
    ready();
  }

  setStatusMessage(GET_TEXT_F(DGUS_MSG_RESET_EEPROM));
}

void DGUSScreenHandler::storeSettings(char *buff) {
  eeprom_data_t data;

  static_assert(sizeof(data) <= ExtUI::eeprom_data_size, "sizeof(eeprom_data_t) > eeprom_data_size.");

  data.initialized = true;
  data.volume = dgus.getVolume();
  data.brightness = dgus.getBrightness();
  data.abl_okay = (ExtUI::getLevelingActive() && ExtUI::getLevelingIsValid());

  memcpy(buff, &data, sizeof(data));
}

void DGUSScreenHandler::loadSettings(const char *buff) {
  eeprom_data_t data;

  static_assert(sizeof(data) <= ExtUI::eeprom_data_size, "sizeof(eeprom_data_t) > eeprom_data_size.");

  memcpy(&data, buff, sizeof(data));

  dgus.setVolume(data.initialized ? data.volume : DGUS_DEFAULT_VOLUME);
  dgus.setBrightness(data.initialized ? data.brightness : DGUS_DEFAULT_BRIGHTNESS);

  if (data.initialized) {
    leveling_active = (data.abl_okay && ExtUI::getLevelingIsValid());
    ExtUI::setLevelingActive(leveling_active);
  }
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

void DGUSScreenHandler::meshUpdate(const int8_t xpos, const int8_t ypos) {
  if (current_screenID != DGUS_ScreenID::LEVELING_PROBING) {
    if (current_screenID == DGUS_ScreenID::LEVELING_AUTOMATIC)
      triggerFullUpdate();
    return;
  }

  uint8_t point = ypos * GRID_MAX_POINTS_X + xpos;
  probing_icons[point < 16 ? 0 : 1] |= (1U << (point % 16));

  if (xpos >= GRID_MAX_POINTS_X - 1 && ypos >= GRID_MAX_POINTS_Y - 1 && !ExtUI::getLevelingIsValid())
    probing_icons[0] = probing_icons[1] = 0;

  triggerFullUpdate();
}

void DGUSScreenHandler::printTimerStarted() {
  triggerScreenChange(DGUS_ScreenID::PRINT_STATUS);
}

void DGUSScreenHandler::printTimerPaused() {
  dgus.playSound(3);
  triggerFullUpdate();
}

void DGUSScreenHandler::printTimerStopped() {
  if (current_screenID != DGUS_ScreenID::PRINT_STATUS && current_screenID != DGUS_ScreenID::PRINT_ADJUST)
    return;

  dgus.playSound(3);

  triggerScreenChange(DGUS_ScreenID::PRINT_FINISHED);
}

void DGUSScreenHandler::filamentRunout(const ExtUI::extruder_t extruder) {
  char buffer[21];

  snprintf_P(buffer, sizeof(buffer), GET_TEXT(DGUS_MSG_FILAMENT_RUNOUT), extruder);

  setStatusMessage(buffer);

  dgus.playSound(3);
}

#if HAS_MEDIA

  void DGUSScreenHandler::sdCardInserted() {
    if (current_screenID == DGUS_ScreenID::HOME)
      triggerScreenChange(DGUS_ScreenID::PRINT);
  }

  void DGUSScreenHandler::sdCardRemoved() {
    if (current_screenID == DGUS_ScreenID::PRINT)
      triggerScreenChange(DGUS_ScreenID::HOME);
  }

  void DGUSScreenHandler::sdCardError() {
    setStatusMessage(GET_TEXT_F(MSG_MEDIA_READ_ERROR));
    if (current_screenID == DGUS_ScreenID::PRINT)
      triggerScreenChange(DGUS_ScreenID::HOME);
  }

#endif // HAS_MEDIA

#if ENABLED(POWER_LOSS_RECOVERY)
  void DGUSScreenHandler::powerLossResume() {
    moveToScreen(DGUS_ScreenID::POWERLOSS, true);
  }
#endif

#if HAS_PID_HEATING

  void DGUSScreenHandler::pidTuning(const ExtUI::result_t rst) {
    switch (rst) {
      case ExtUI::PID_STARTED:
        setStatusMessage(GET_TEXT_F(MSG_PID_AUTOTUNE));
        break;
      case ExtUI::PID_BAD_HEATER_ID:
        setStatusMessage(GET_TEXT_F(MSG_PID_BAD_HEATER_ID));
        break;
      case ExtUI::PID_TEMP_TOO_HIGH:
        setStatusMessage(GET_TEXT_F(MSG_PID_TEMP_TOO_HIGH));
        break;
      case ExtUI::PID_TUNING_TIMEOUT:
        setStatusMessage(GET_TEXT_F(MSG_PID_TIMEOUT));
        break;
      case ExtUI::PID_DONE:
        setStatusMessage(GET_TEXT_F(MSG_PID_AUTOTUNE_DONE));
        break;
      default:
        return;
    }

    dgus.playSound(3);
  }

#endif // HAS_PID_HEATING

void DGUSScreenHandler::setMessageLine(const char * const msg, const uint8_t line) {
  switch (line) {
    default: return;
    case 1:
      dgus.writeString((uint16_t)DGUS_Addr::MESSAGE_Line1, msg, DGUS_LINE_LEN, true, true);
      break;
    case 2:
      dgus.writeString((uint16_t)DGUS_Addr::MESSAGE_Line2, msg, DGUS_LINE_LEN, true, true);
      break;
    case 3:
      dgus.writeString((uint16_t)DGUS_Addr::MESSAGE_Line3, msg, DGUS_LINE_LEN, true, true);
      break;
    case 4:
      dgus.writeString((uint16_t)DGUS_Addr::MESSAGE_Line4, msg, DGUS_LINE_LEN, true, true);
      break;
  }
}

void DGUSScreenHandler::setMessageLinePGM(PGM_P const msg, const uint8_t line) {
  switch (line) {
    default: return;
    case 1:
      dgus.writeStringPGM((uint16_t)DGUS_Addr::MESSAGE_Line1, msg, DGUS_LINE_LEN, true, true);
      break;
    case 2:
      dgus.writeStringPGM((uint16_t)DGUS_Addr::MESSAGE_Line2, msg, DGUS_LINE_LEN, true, true);
      break;
    case 3:
      dgus.writeStringPGM((uint16_t)DGUS_Addr::MESSAGE_Line3, msg, DGUS_LINE_LEN, true, true);
      break;
    case 4:
      dgus.writeStringPGM((uint16_t)DGUS_Addr::MESSAGE_Line4, msg, DGUS_LINE_LEN, true, true);
      break;
  }
}

void DGUSScreenHandler::setStatusMessage(const char* msg, const millis_t duration) {
  dgus.writeString((uint16_t)DGUS_Addr::MESSAGE_Status, msg, DGUS_STATUS_LEN, false, true);
  status_expire = (duration > 0 ? ExtUI::safe_millis() + duration : 0);
}

void DGUSScreenHandler::setStatusMessage(FSTR_P const fmsg, const millis_t duration) {
  dgus.writeString((uint16_t)DGUS_Addr::MESSAGE_Status, fmsg, DGUS_STATUS_LEN, false, true);
  status_expire = (duration > 0 ? ExtUI::safe_millis() + duration : 0);
}

void DGUSScreenHandler::showWaitScreen(const DGUS_ScreenID return_screenID, const bool has_continue/*=false*/) {
  if (return_screenID != DGUS_ScreenID::WAIT)
    wait_return_screenID = return_screenID;

  wait_continue = has_continue;

  triggerScreenChange(DGUS_ScreenID::WAIT);
}

void DGUSScreenHandler::showWaitScreen(FSTR_P const msg, const DGUS_ScreenID return_screenID, const bool has_continue/*=false*/) {
  setMessageLinePGM(NUL_STR, 1);
  setMessageLine(msg, 2);
  setMessageLinePGM(NUL_STR, 3);
  setMessageLinePGM(NUL_STR, 4);
  showWaitScreen(return_screenID, has_continue);
}

DGUS_ScreenID DGUSScreenHandler::getCurrentScreen() {
  return current_screenID;
}

void DGUSScreenHandler::triggerScreenChange(const DGUS_ScreenID screenID) {
  new_screenID = screenID;
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

const DGUS_Addr* DGUSScreenHandler::findScreenAddrList(const DGUS_ScreenID screenID) {
  DGUS_ScreenAddrList list;
  const DGUS_ScreenAddrList *map = screen_addr_list_map;

  do {
    memcpy_P(&list, map, sizeof(*map));
    if (!list.addr_list) break;
    if (list.screenID == screenID) return list.addr_list;
  } while (++map);

  return nullptr;
}

bool DGUSScreenHandler::callScreenSetup(const DGUS_ScreenID screenID) {
  DGUS_ScreenSetup setup;
  const DGUS_ScreenSetup *list = screen_setup_list;

  do {
    memcpy_P(&setup, list, sizeof(*list));
    if (!setup.setup_fn) break;
    if (setup.screenID == screenID) {
      return setup.setup_fn();
    }
  } while (++list);

  return true;
}

void DGUSScreenHandler::moveToScreen(const DGUS_ScreenID screenID, bool abort_wait) {
  if (current_screenID == DGUS_ScreenID::KILL) return;

  if (current_screenID == DGUS_ScreenID::WAIT) {
    if (screenID != DGUS_ScreenID::WAIT)
      wait_return_screenID = screenID;

    if (!abort_wait) return;

    if (wait_continue && wait_for_user)
      ExtUI::setUserConfirmed();
  }

  if (!callScreenSetup(screenID)) return;

  if (!sendScreenVPData(screenID, true)) return;

  current_screenID = screenID;
  dgus.switchScreen(current_screenID);
}

bool DGUSScreenHandler::sendScreenVPData(const DGUS_ScreenID screenID, bool complete_update) {
  if (complete_update) full_update = false;

  const DGUS_Addr *list = findScreenAddrList(screenID);

  while (true) {
    if (!list) return true; // Nothing left to send

    const uint16_t addr = pgm_read_word(list++);
    if (!addr) return true; // Nothing left to send

    DGUS_VP vp;
    if (!populateVP((DGUS_Addr)addr, &vp)) continue; // Invalid VP
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

#endif // DGUS_LCD_UI_RELOADED
