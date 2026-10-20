/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/sermoon_d1/sermoon_d1_rts.cpp
 *
 * Creality Sermoon D1 stock DWIN touchscreen (factory DWIN_SET).
 * Ported from Creality's Marlin 2.0.1 LCD_RTS (Sermoon D1 V1.1.16, screen DWIN 1.1.14).
 */

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_SERMOON_D1

#include "sermoon_d1_rts.h"

#include "../../../MarlinCore.h"
#include "../../../core/mstring.h"
#include "../../../module/motion.h"
#include "../../../module/stepper.h"

#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

using namespace ExtUI;

RTS rts;
RTS::settings_t RTS::settings;

#define RTS_ZOFFSET_STEP 0.1f   // (mm) Z offset change per button press

namespace {

  enum PrintState : uint8_t { PS_IDLE, PS_WARMUP, PS_PRINTING, PS_PAUSED, PS_STOPPING, PS_DONE };
  enum HeatState : uint8_t { HEAT_IDLE, HEAT_HEATING, HEAT_COOLING };
  enum FilamentCheck : uint8_t { FC_NONE, FC_START, FC_RESUME, FC_CHANGE };
  enum HomeReturn : uint8_t { HR_NONE, HR_MOVE, HR_LEVEL };

  PrintState print_state = PS_IDLE;
  HeatState heat_state = HEAT_IDLE;
  RTS::StatusMsg status_msg = RTS::MSG_READY;

  bool booted = false, recovery_pending = false;
  uint8_t boot_step = 0;
  millis_t next_boot_ms = 0, next_update_ms = 0;

  bool fan_on = false;
  celsius_t last_target_hotend = 0, last_target_bed = 0;
  uint8_t last_percent = 0xFF;

  bool homing = false;
  uint8_t homing_icon = 1;
  HomeReturn home_return = HR_NONE;
  uint8_t axis_page = 2;          // 0 = 0.1mm, 1 = 1mm, 2 = 10mm

  #if HAS_BED_PROBE
    bool auto_leveling = false;
  #endif
  #if HAS_MESH
    uint8_t probe_count = 0;
  #endif

  float filament_len = 10;
  float pending_e = 0;            // Filament move waiting for the nozzle to heat
  bool fil_heating = false;
  celsius_t fil_temp = PREHEAT_1_TEMP_HOTEND;
  FilamentCheck fil_check = FC_NONE;

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    PauseMessage pause_msg = PAUSE_MESSAGE_STATUS;
  #endif

  int8_t selected_file = -1;
  uint8_t file_count = 0;
  uint16_t file_index[RTS_FILE_SLOTS];

  uint8_t pending_language = 0;   // Chosen on the language page, applied on confirm
  bool zoffset_dirty = false;

  // Strip the extension and fit a filename into a screen field
  void makeDisplayName(char * const out, const char * const name) {
    const char * const dot = strrchr(name, '.');
    size_t len = (dot && dot != name) ? size_t(dot - name) : strlen(name);
    if (len >= RTS_FILENAME_LEN) {
      len = RTS_FILENAME_LEN - 1;
      memcpy(out, name, len - 2);
      out[len - 2] = out[len - 1] = '~';
    }
    else
      memcpy(out, name, len);
    out[len] = '\0';
  }

  bool nozzleAtTemp() {
    const celsius_t t = getTargetTemp_celsius(E0);
    return t && getActualTemp_celsius(E0) >= t - (TEMP_WINDOW);
  }

  bool bedAtTemp() {
    const celsius_t t = getTargetTemp_celsius(BED);
    return !t || getActualTemp_celsius(BED) >= t - (TEMP_BED_WINDOW);
  }

  bool steppersEnabled() {
    return stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
  }

} // namespace

//
// Serial protocol
//

void RTS::writeWord(const uint16_t vp, const uint16_t value) {
  const uint8_t frame[] = { RTS_FRAME_H1, RTS_FRAME_H2, 5, RTS_CMD_WRITE_VAR, uint8_t(vp >> 8), uint8_t(vp), uint8_t(value >> 8), uint8_t(value) };
  for (const uint8_t b : frame) RTS_SERIAL.write(b);
}

void RTS::writeLong(const uint16_t vp, const uint32_t value) {
  const uint8_t frame[] = { RTS_FRAME_H1, RTS_FRAME_H2, 7, RTS_CMD_WRITE_VAR, uint8_t(vp >> 8), uint8_t(vp),
                            uint8_t(value >> 24), uint8_t(value >> 16), uint8_t(value >> 8), uint8_t(value) };
  for (const uint8_t b : frame) RTS_SERIAL.write(b);
}

void RTS::writeText(const uint16_t vp, const char *str, const uint8_t maxlen/*=RTS_FILENAME_LEN*/) {
  const uint8_t len = _MIN(strlen(str), size_t(maxlen));
  if (!len) return;
  const uint8_t head[] = { RTS_FRAME_H1, RTS_FRAME_H2, uint8_t(3 + len), RTS_CMD_WRITE_VAR, uint8_t(vp >> 8), uint8_t(vp) };
  for (const uint8_t b : head) RTS_SERIAL.write(b);
  for (uint8_t i = 0; i < len; ++i) RTS_SERIAL.write(uint8_t(str[i]));
}

void RTS::clearWords(const uint16_t vp, const uint8_t words) {
  const uint8_t head[] = { RTS_FRAME_H1, RTS_FRAME_H2, uint8_t(3 + 2 * words), RTS_CMD_WRITE_VAR, uint8_t(vp >> 8), uint8_t(vp) };
  for (const uint8_t b : head) RTS_SERIAL.write(b);
  for (uint8_t i = 0; i < 2 * words; ++i) RTS_SERIAL.write(uint8_t(0));
}

void RTS::gotoPage(const uint8_t page) {
  writeLong(VP_PAGE, RTS_PAGE_BASE + page);
}

// Each status message has an icon per language
void RTS::setStatus(const StatusMsg msg) {
  status_msg = msg;
  writeWord(VP_STATUS_ICON, 27 + 9 * msg + settings.language - 1);
}

// Frames from the screen: 5A A5 len 83 vpH vpL words dataH dataL ...
void RTS::receive() {
  static uint8_t buf[RTS_RX_BUFFER_SIZE], len, pos, state;
  while (RTS_SERIAL.available()) {
    const uint8_t c = RTS_SERIAL.read();
    switch (state) {
      case 0: if (c == RTS_FRAME_H1) state = 1; break;
      case 1: state = c == RTS_FRAME_H2 ? 2 : (c == RTS_FRAME_H1 ? 1 : 0); break;
      case 2:
        if (WITHIN(c, 3, RTS_RX_BUFFER_SIZE)) { len = c; pos = 0; state = 3; }
        else state = 0;
        break;
      case 3:
        buf[pos++] = c;
        if (pos < len) break;
        state = 0;
        if (buf[0] == RTS_CMD_READ_VAR && len >= 6)
          handle(uint16_t(buf[1] << 8) | buf[2], uint16_t(buf[4] << 8) | buf[5]);
        break;
    }
  }
}

//
// Display helpers
//

// Labels are drawn as icons, one per language: write the language number to each
void RTS::sendLanguage() {
  static constexpr uint16_t label_ranges[][2] PROGMEM = {
    { 0x1001, 0x100C }, { 0x1020, 0x1025 }, { 0x1027, 0x1029 }, { 0x1030, 0x1035 },
    { 0x1040, 0x1043 }, { 0x1050, 0x1054 }, { 0x1060, 0x1065 }, { 0x1080, 0x1085 },
    { 0x1090, 0x1095 }, { 0x10A0, 0x10A2 }, { 0x10B0, 0x10B3 }, { 0x1100, 0x110D },
    { 0x1110, 0x1114 }
  };
  for (const auto &r : label_ranges)
    for (uint16_t vp = pgm_read_word(&r[0]); vp <= pgm_read_word(&r[1]); ++vp)
      writeWord(vp, settings.language);
  setStatus(status_msg);
}

void RTS::selectLanguage(const uint8_t lang) {
  pending_language = lang;
  for (uint8_t i = 1; i <= RTS_LANGUAGES; ++i)
    writeWord(VP_LANGUAGE_ICON + i - 1, i == lang);
}

void RTS::sendZOffset() { writeWord(VP_ZOFFSET, int16_t(LROUND(getZOffset_mm() * 100))); }

// The screen lists mesh values row by row, reversing every other row
void RTS::sendMeshPoint(const int8_t x, const int8_t y, const float z) {
  if (!WITHIN(x, 0, RTS_MESH_SIZE - 1) || !WITHIN(y, 0, RTS_MESH_SIZE - 1)) return;
  const uint8_t i = y * RTS_MESH_SIZE + ((y & 1) ? RTS_MESH_SIZE - 1 - x : x);
  writeWord(VP_MESH_VALUES + i * 2, isnan(z) ? 0 : int16_t(LROUND(z * 1000)));
}

void RTS::sendMesh() {
  #if HAS_MESH
    for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; ++y)
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; ++x)
        sendMeshPoint(x, y, getMeshPoint({ x, y }));
  #endif
  writeWord(VP_LEVELING_ICON, TERN0(HAS_LEVELING, getLevelingActive()) ? 3 : 2);
}

void RTS::sendPosition() {
  writeWord(VP_AXIS_X, int16_t(LROUND(getAxisPosition_mm(X) * 10)));
  writeWord(VP_AXIS_Y, int16_t(LROUND(getAxisPosition_mm(Y) * 10)));
  writeWord(VP_AXIS_Z, int16_t(LROUND(getAxisPosition_mm(Z) * 10)));
}

void RTS::showFilename(const uint16_t vp, const char *name) {
  char buf[RTS_FILENAME_LEN + 1];
  const uint8_t len = strlen(name), pad = len < RTS_FILENAME_LEN ? (RTS_FILENAME_LEN - len) / 2 : 0;
  memset(buf, ' ', pad);
  strlcpy(buf + pad, name, sizeof(buf) - pad);
  clearWords(vp, FILENAME_WORDS);
  writeText(vp, buf);
}

void RTS::showError(FSTR_P const msg) {
  char buf[21];
  strlcpy_P(buf, FTOP(msg), sizeof(buf));
  clearWords(VP_ERROR_TEXT, 10);
  writeText(VP_ERROR_TEXT, buf, 20);
  gotoPage(PAGE_ERROR);
}

void RTS::clearPrintInfo() {
  writeWord(VP_BOOT_PROGRESS, 0);
  writeWord(VP_PERCENTAGE, 0);
  writeWord(VP_TIME_HOUR, 0);
  writeWord(VP_TIME_MIN, 0);
  clearWords(VP_PRINT_FILENAME, FILENAME_WORDS);
  last_percent = 0xFF;
}

void RTS::clearFileList() {
  for (uint8_t i = 0; i < RTS_FILE_SLOTS; ++i) {
    clearWords(VP_FILE_NAMES + i * 0x14, FILENAME_WORDS);
    writeWord(VP_FILE_ICON + i, 0);
  }
  file_count = 0;
  selected_file = -1;
}

// List up to RTS_FILE_SLOTS printable files from the current folder, newest first
void RTS::refreshFileList() {
  clearFileList();
  if (!isMediaMounted()) return;
  FileList files;
  char name[RTS_FILENAME_LEN];
  for (uint16_t i = files.count(); i-- && file_count < RTS_FILE_SLOTS;) {
    if (!files.seek(i) || files.isDir()) continue;
    file_index[file_count] = i;
    makeDisplayName(name, files.longFilename());
    writeText(VP_FILE_NAMES + file_count * 0x14, name);
    ++file_count;
  }
}

void RTS::showTempPage() { gotoPage(fan_on ? PAGE_TEMP_FAN_ON : PAGE_TEMP_FAN_OFF); }

void RTS::showPrintPage() {
  switch (print_state) {
    case PS_WARMUP: gotoPage(PAGE_PRINT_HEAT); break;
    case PS_PAUSED: gotoPage(PAGE_PAUSED); break;
    default:        gotoPage(PAGE_PRINTING); break;
  }
}

bool RTS::filamentMissing() {
  #if HAS_FILAMENT_SENSOR
    return getFilamentRunoutEnabled() && (FilamentSensorBase::poll_runout_states() & 1);
  #else
    return false;
  #endif
}

void RTS::saveSettings() { TERN_(EEPROM_SETTINGS, injectCommands(F("M500"))); }

void RTS::setZOffset(const float z) {
  #if HAS_BED_PROBE
    if (!WITHIN(z, PROBE_OFFSET_ZMIN, PROBE_OFFSET_ZMAX)) return;
  #endif
  babystepAxis_steps(mmToWholeSteps(z - getZOffset_mm(), Z), Z);
  TERN_(HAS_BED_PROBE, setZOffset_mm(z));
  zoffset_dirty = true;
  sendZOffset();
}

void RTS::moveToTrammingPoint(const float x, const float y) {
  const int fz = motion.homing_feedrate_mm_m.z;
  MString<64> cmd(F("G1Z3F"), fz, F("\nG1X"), int(x), 'Y', int(y), 'F', int(motion.homing_feedrate_mm_m.x), F("\nG1Z0F"), fz);
  injectCommands(cmd);
}

void RTS::moveFilament(const float mm) {
  setAxisPosition_mm(getAxisPosition_mm(E0) + mm, E0);
  writeWord(VP_FILAMENT_LEN, LROUND(filament_len * 10));
}

void RTS::startSelectedFile() {
  if (selected_file < 0 || !isMediaMounted()) return;
  if (filamentMissing()) {
    fil_check = FC_START;
    gotoPage(PAGE_NO_FILAMENT);
    return;
  }
  FileList files;
  if (!files.seek(file_index[selected_file])) return;
  char name[RTS_FILENAME_LEN];
  makeDisplayName(name, files.longFilename());
  showFilename(VP_PRINT_FILENAME, name);
  printFile(files.shortFilename());
  printStarted();
}

void RTS::resumeJob() {
  if (filamentMissing()) {
    fil_check = FC_RESUME;
    gotoPage(PAGE_NO_FILAMENT);
    return;
  }
  gotoPage(PAGE_WAIT);
  if (awaitingUserConfirm()) {
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      if (pause_msg == PAUSE_MESSAGE_OPTION) setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
    #endif
    setUserConfirmed();
  }
  else
    resumePrint();
}

//
// Marlin events
//

void RTS::onStartup() {
  RTS_SERIAL.begin(LCD_BAUDRATE);

  writeWord(VP_FEEDRATE, 100);
  writeWord(VP_NOZZLE_TARGET, 0);
  writeWord(VP_BED_TARGET, 0);
  writeWord(VP_FAN_ICON, 0);
  writeWord(VP_MOTOR_ICON, 0);

  char sizebuf[20];
  sprintf_P(sizebuf, PSTR("%d*%d*%d"), X_BED_SIZE, Y_BED_SIZE, Z_MAX_POS);
  writeText(VP_MACHINE_NAME, MACHINE_NAME, 20);
  writeText(VP_HW_VERSION, BOARD_INFO_NAME, 20);
  writeText(VP_FW_VERSION, SHORT_BUILD_VERSION, 20);
  writeText(VP_PRINTER_SIZE, sizebuf, 20);
  writeText(VP_WEBSITE, WEBSITE_URL, 20);

  clearPrintInfo();
  clearFileList();
  gotoPage(PAGE_BOOT);
}

void RTS::onIdle() {
  receive();
  if (!booted) return bootAnimation();

  if (print_state == PS_STOPPING) {
    print_state = PS_IDLE;
    clearPrintInfo();
    setStatus(MSG_STOPPED);
    gotoPage(PAGE_MAIN);
    if (zoffset_dirty) { zoffset_dirty = false; saveSettings(); }
  }

  const millis_t ms = millis();
  if (ELAPSED(ms, next_update_ms)) {
    next_update_ms = ms + RTS_UPDATE_INTERVAL;
    update();
  }
}

// The startup progress bar, then the language prompt, main page or power-loss prompt
void RTS::bootAnimation() {
  const millis_t ms = millis();
  if (PENDING(ms, next_boot_ms)) return;
  next_boot_ms = ms + RTS_BOOT_STEP_MS;

  writeWord(VP_BOOT_PROGRESS, boot_step);
  if (++boot_step <= 100) return;

  booted = true;
  writeWord(VP_BOOT_PROGRESS, 0);
  if (!WITHIN(settings.language, 1, RTS_LANGUAGES)) {
    selectLanguage(0);
    gotoPage(PAGE_FIRST_LANG);
    return;
  }
  sendLanguage();
  setStatus(isMediaMounted() ? MSG_READY : MSG_NO_CARD);
  if (recovery_pending) powerLossDetected(); else gotoPage(PAGE_MAIN);
}

void RTS::checkHeatDone() {
  if (isPrinting()) return;
  switch (heat_state) {
    case HEAT_HEATING:
      if (nozzleAtTemp() || (!getTargetTemp_celsius(E0) && bedAtTemp())) {
        heat_state = HEAT_IDLE;
        setStatus(MSG_HEAT_DONE);
      }
      break;
    case HEAT_COOLING:
      // Like the stock firmware, call it cool below 35°C
      if (getActualTemp_celsius(E0) < 35 && getActualTemp_celsius(BED) < 35) {
        heat_state = HEAT_IDLE;
        setStatus(MSG_COOL_DONE);
      }
      break;
    default: break;
  }
}

void RTS::update() {
  writeWord(VP_NOZZLE_TEMP, getActualTemp_celsius(E0));
  writeWord(VP_BED_TEMP, getActualTemp_celsius(BED));

  const celsius_t th = getTargetTemp_celsius(E0), tb = getTargetTemp_celsius(BED);
  if (th != last_target_hotend || tb != last_target_bed) {
    writeWord(VP_NOZZLE_TARGET, th);
    writeWord(VP_BED_TARGET, tb);
    if (!isPrinting()) {
      const bool rising = th > last_target_hotend || tb > last_target_bed;
      heat_state = rising ? HEAT_HEATING : HEAT_COOLING;
      setStatus(rising ? MSG_HEATING : MSG_COOLING);
    }
    last_target_hotend = th;
    last_target_bed = tb;
  }
  checkHeatDone();

  const bool fan = getTargetFan_percent(FAN0) > 0;
  if (fan != fan_on) { fan_on = fan; writeWord(VP_FAN_ICON, fan); }

  // Printing starts once the job's heat-up has finished
  if (print_state == PS_WARMUP && isPrintingFromMedia() && nozzleAtTemp() && bedAtTemp()) {
    print_state = PS_PRINTING;
    setStatus(MSG_PRINTING);
    gotoPage(PAGE_PRINTING);
  }

  if (WITHIN(print_state, PS_WARMUP, PS_PAUSED)) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    writeWord(VP_TIME_HOUR, elapsed / 3600);
    writeWord(VP_TIME_MIN, (elapsed % 3600) / 60);
    const uint8_t pct = getProgress_percent();
    if (pct != last_percent) {
      last_percent = pct;
      writeWord(VP_BOOT_PROGRESS, pct ? _MIN(pct + 1, 100) : 0);
      writeWord(VP_PERCENTAGE, pct);
    }
  }

  // Waiting to heat for a filament change
  if (fil_heating && nozzleAtTemp()) {
    fil_heating = false;
    gotoPage(PAGE_FILAMENT);
    if (pending_e) { moveFilament(pending_e); pending_e = 0; }
  }

  if (homing) {
    writeWord(VP_HOMING_ICON, homing_icon);
    if (++homing_icon > 9) homing_icon = 1;
  }
}

void RTS::printStarted() {
  if (print_state == PS_PAUSED) {
    print_state = PS_PRINTING;
    setStatus(MSG_PRINTING);
    gotoPage(PAGE_PRINTING);
    return;
  }
  if (WITHIN(print_state, PS_WARMUP, PS_PRINTING)) return;

  // A new job, from the screen or from a host
  print_state = PS_WARMUP;
  last_percent = 0xFF;
  writeWord(VP_FEEDRATE, getFeedrate_percent());
  sendZOffset();
  setStatus(MSG_HEATING);
  gotoPage(PAGE_PRINT_HEAT);
}

void RTS::printPaused() {
  if (!WITHIN(print_state, PS_WARMUP, PS_PRINTING)) return;
  print_state = PS_PAUSED;
  gotoPage(PAGE_PAUSED);
}

// Finished jobs report onPrintDone right after the timer stops, so decide in onIdle
void RTS::printStopped() {
  if (WITHIN(print_state, PS_WARMUP, PS_PAUSED)) print_state = PS_STOPPING;
  fil_heating = false;
}

void RTS::printFinished() {
  print_state = PS_DONE;
  writeWord(VP_PERCENTAGE, 100);
  writeWord(VP_BOOT_PROGRESS, 100);
  setStatus(MSG_PRINT_DONE);
  gotoPage(PAGE_PRINT_DONE);
  if (zoffset_dirty) { zoffset_dirty = false; saveSettings(); }
}

void RTS::homingStarted() {
  homing = true;
  homing_icon = 1;
}

void RTS::homingFinished() {
  homing = false;
  sendPosition();
  switch (home_return) {
    case HR_MOVE:  gotoPage(PAGE_MOVE + axis_page); break;
    case HR_LEVEL: gotoPage(PAGE_LEVELING); break;
    default: break;
  }
  home_return = HR_NONE;
}

void RTS::meshPointProbed(const int8_t x, const int8_t y, const float z) {
  sendMeshPoint(x, y, z);
  // A mesh reset reports every point, already cleared to NAN
  #if ALL(HAS_MESH, HAS_BED_PROBE)
    if (auto_leveling && probe_count < GRID_MAX_POINTS && !isnan(getMeshPoint({ uint8_t(x), uint8_t(y) })))
      writeWord(VP_PROBE_ICON, ++probe_count);
  #endif
}

void RTS::levelingFinished() {
  sendMesh();
  #if HAS_BED_PROBE
    if (auto_leveling) {
      auto_leveling = false;
      gotoPage(PAGE_LEVELING);
    }
  #endif
}

void RTS::powerLossDetected() {
  recovery_pending = true;
  if (!booted) return;
  recovery_pending = false;
  #if ENABLED(POWER_LOSS_RECOVERY)
    const char * const path = recovery.info.sd_filename, * const slash = strrchr(path, '/');
    char name[RTS_FILENAME_LEN];
    makeDisplayName(name, slash ? slash + 1 : path);
    showFilename(VP_PRINT_FILENAME, name);
  #endif
  gotoPage(PAGE_RECOVERY);
}

void RTS::mediaInserted() {
  refreshFileList();
  if (!isPrinting()) setStatus(MSG_READY);
}

void RTS::mediaRemoved() {
  clearFileList();
  setStatus(MSG_NO_CARD);
}

void RTS::filamentRunout() {
  if (isPrintingFromMedia()) gotoPage(PAGE_WAIT);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void RTS::pauseMessage(const PauseMessage message) {
    pause_msg = message;
    switch (message) {
      case PAUSE_MESSAGE_PARKING:
      case PAUSE_MESSAGE_CHANGING:
      case PAUSE_MESSAGE_UNLOAD:
      case PAUSE_MESSAGE_LOAD:
      case PAUSE_MESSAGE_RESUME:
      case PAUSE_MESSAGE_HEATING:
        gotoPage(PAGE_WAIT);
        break;
      case PAUSE_MESSAGE_PURGE:
        gotoPage(TERN(ADVANCED_PAUSE_CONTINUOUS_PURGE, PAGE_PAUSED, PAGE_WAIT));
        break;
      // Resume on the paused page continues, reheating first when needed
      case PAUSE_MESSAGE_WAITING:
      case PAUSE_MESSAGE_INSERT:
      case PAUSE_MESSAGE_OPTION:
      case PAUSE_MESSAGE_HEAT:
        gotoPage(PAGE_PAUSED);
        break;
      case PAUSE_MESSAGE_STATUS:
      default:
        if (isPrinting()) showPrintPage();
        break;
    }
  }

#endif

//
// Touch events
//

void RTS::handle(const uint16_t vp, const uint16_t value) {
  switch (vp) {
    case VP_KEY_MAIN:        onMainKey(value); break;
    case VP_KEY_FILES:       onFilesKey(value); break;
    case VP_KEY_PRINT:       onPrintKey(value); break;
    case VP_FEEDRATE:        setFeedrate_percent(value); break;
    case VP_ZOFFSET:         setZOffset(int16_t(value) / 100.0f); break;
    case VP_KEY_TEMP:        onTempKey(value); break;
    case VP_KEY_PREHEAT:     if (value == 1) showTempPage(); break;
    case VP_KEY_MANUAL_TEMP: onManualTempKey(value); break;
    case VP_NOZZLE_TARGET:   setTargetTemp_celsius(value, E0); break;
    case VP_BED_TARGET:      setTargetTemp_celsius(value, BED); break;
    case VP_KEY_SETTINGS:    onSettingsKey(value); break;
    case VP_KEY_LEVELING:    onLevelingKey(value); writeWord(VP_MOTOR_ICON, 1); break;
    case VP_KEY_HOME:
    case VP_AXIS_X:
    case VP_AXIS_Y:
    case VP_AXIS_Z:          onAxis(vp, value); break;
    case VP_KEY_MOVE:        onMoveKey(value); break;
    case VP_KEY_FILAMENT:    onFilamentKey(value); break;
    case VP_FILAMENT_LEN:    filament_len = value / 10.0f; break;
    case VP_KEY_LANGUAGE:    onLanguageKey(value); break;
    case VP_KEY_POWER_LOSS:  onPowerLossKey(value); break;
    case VP_KEY_NO_FILAMENT: onNoFilamentKey(value); break;
    default: break;
  }
}

void RTS::onMainKey(const uint16_t value) {
  switch (value) {
    case 1: // Open the file list
      refreshFileList();
      gotoPage(PAGE_FILES);
      break;
    case 2: // Leave the print-finished page
      print_state = PS_IDLE;
      clearPrintInfo();
      setStatus(MSG_READY);
      gotoPage(PAGE_MAIN);
      break;
    case 3: showTempPage(); break;
    case 4:
      writeWord(VP_MOTOR_ICON, steppersEnabled());
      gotoPage(PAGE_SETTINGS);
      break;
  }
}

void RTS::onFilesKey(const uint16_t value) {
  if (WITHIN(value, 0x21, 0x20 + RTS_FILE_SLOTS)) {
    const uint8_t n = value - 0x21;
    if (n >= file_count) return;
    selected_file = n;
    for (uint8_t i = 0; i < file_count; ++i) writeWord(VP_FILE_ICON + i, i == n);
  }
  else if (value == 2)
    startSelectedFile();
  else if (value == 1)
    gotoPage(PAGE_MAIN);
}

void RTS::onPrintKey(const uint16_t value) {
  switch (value) {
    case 0x01: gotoPage(PAGE_STOP_CONFIRM); break;
    case 0x02: writeWord(VP_FAN_ICON, fan_on); gotoPage(PAGE_ADJUST); break;
    case 0x03: gotoPage(PAGE_PAUSE_CONFIRM); break;
    case 0x05: resumeJob(); break;
    case 0x10: showPrintPage(); break;            // Leave the adjust page
    case 0x11:                                    // Fan toggle
      fan_on = !fan_on;
      setTargetFan_percent(fan_on ? 100 : 0, FAN0);
      writeWord(VP_FAN_ICON, fan_on);
      break;
    case 0xF1:                                    // Pause cancelled
    case 0xF3:                                    // Stop cancelled
      showPrintPage();
      break;
    case 0xF2:                                    // Pause confirmed
      gotoPage(PAGE_WAIT);
      pausePrint();
      break;
    case 0xF4:                                    // Stop confirmed
      gotoPage(PAGE_WAIT);
      stopPrint();
      break;
  }
}

void RTS::onTempKey(const uint16_t value) {
  switch (value) {
    case 0x00: gotoPage(PAGE_MAIN); break;
    case 0x03: // Fan toggle
      fan_on = !fan_on;
      setTargetFan_percent(fan_on ? 100 : 0, FAN0);
      showTempPage();
      break;
    case 0x05: // PLA preheat
      fil_temp = PREHEAT_1_TEMP_HOTEND;
      setTargetTemp_celsius(fil_temp, E0);
      setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
      showTempPage();
      break;
    case 0x06: // ABS preheat
      fil_temp = PREHEAT_2_TEMP_HOTEND;
      setTargetTemp_celsius(fil_temp, E0);
      setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
      showTempPage();
      break;
    case 0xF1: showTempPage(); break;             // Cool down cancelled
    case 0xF2:                                    // Cool down: heaters off, fan on
      coolDown();
      fan_on = true;
      setTargetFan_percent(100, FAN0);
      setStatus(MSG_COOLING);
      heat_state = HEAT_COOLING;
      gotoPage(PAGE_TEMP_FAN_ON);
      break;
  }
}

void RTS::onManualTempKey(const uint16_t value) {
  switch (value) {
    case 1: showTempPage(); break;
    case 3: setTargetTemp_celsius(0, E0); break;
    case 5: setTargetTemp_celsius(0, BED); break;
  }
}

void RTS::onSettingsKey(const uint16_t value) {
  switch (value) {
    case 1: gotoPage(PAGE_MAIN); break;
    case 2: // Leveling: home, lower the nozzle to Z0
      sendZOffset();
      sendMesh();
      home_return = HR_LEVEL;
      injectCommands(F("G28\nG1F200Z0"));
      gotoPage(PAGE_HOMING);
      break;
    case 3: // Filament change
      filament_len = 10;
      writeWord(VP_FILAMENT_LEN, 100);
      writeWord(VP_NOZZLE_TARGET, getTargetTemp_celsius(E0));
      gotoPage(PAGE_FILAMENT);
      break;
    case 4: // Move axes
      sendPosition();
      gotoPage(PAGE_MOVE + axis_page);
      break;
    case 5: // Motors on/off
      if (steppersEnabled()) {
        injectCommands(F("M84"));
        writeWord(VP_MOTOR_ICON, 0);
      }
      else {
        injectCommands(F("M17"));
        writeWord(VP_MOTOR_ICON, 1);
      }
      break;
    case 6: // Language
      selectLanguage(settings.language);
      gotoPage(PAGE_LANGUAGE);
      break;
    case 7: gotoPage(PAGE_ABOUT); break;
    case 0xF1:
    case 0xF2: gotoPage(PAGE_SETTINGS); break;
  }
}

void RTS::onLevelingKey(const uint16_t value) {
  #if ENABLED(LCD_BED_TRAMMING)
    constexpr float lfrb[4] = BED_TRAMMING_INSET_LFRB;
  #else
    constexpr float lfrb[4] = { 30, 30, 30, 30 };
  #endif
  switch (value) {
    case 1: // Leave leveling
      if (zoffset_dirty) { zoffset_dirty = false; saveSettings(); }
      gotoPage(PAGE_SETTINGS);
      break;
    case 2: setZOffset(getZOffset_mm() + RTS_ZOFFSET_STEP); break;
    case 3: // Home Z and lower the nozzle for the paper test
      home_return = HR_LEVEL;
      injectCommands(isAxisPositionKnown(X) && isAxisPositionKnown(Y) ? F("G28Z\nG1F200Z0") : F("G28\nG1F200Z0"));
      break;
    case 4: setZOffset(getZOffset_mm() - RTS_ZOFFSET_STEP); break;
    case 5: // Probe the mesh
      #if ALL(HAS_BED_PROBE, HAS_MESH)
        auto_leveling = true;
        probe_count = 0;
        writeWord(VP_PROBE_ICON, 0);
        gotoPage(PAGE_AUTOLEVEL);
        injectCommands(isMachineHomed() ? F("G29") : F("G28\nG29"));
      #endif
      break;
    case 6: // Manual tramming
      if (!isMachineHomed()) injectCommands(F("G28"));
      gotoPage(PAGE_TRAMMING);
      break;
    case 7: // Leveling on/off
      #if HAS_LEVELING
        setLevelingActive(!getLevelingActive());
        sendMesh();
        saveSettings();
      #endif
      break;
    case  8: moveToTrammingPoint(X_CENTER, Y_CENTER); break;
    case  9: moveToTrammingPoint(X_MIN_BED + lfrb[0], Y_MIN_BED + lfrb[1]); break;
    case 10: moveToTrammingPoint(X_MAX_BED - lfrb[2], Y_MIN_BED + lfrb[1]); break;
    case 11: moveToTrammingPoint(X_MAX_BED - lfrb[2], Y_MAX_BED - lfrb[3]); break;
    case 12: moveToTrammingPoint(X_MIN_BED + lfrb[0], Y_MAX_BED - lfrb[3]); break;
    case 13: gotoPage(PAGE_SETTINGS); break;
  }
}

void RTS::onAxis(const uint16_t vp, const uint16_t value) {
  axis_t axis;
  switch (vp) {
    case VP_KEY_HOME:
      if (value == 0x0D) {
        home_return = HR_MOVE;
        injectCommands(F("G28"));
        gotoPage(PAGE_HOMING);
      }
      return;
    case VP_AXIS_X: axis = X; break;
    case VP_AXIS_Y: axis = Y; break;
    default:        axis = Z; break;
  }
  setAxisPosition_mm(int16_t(value) / 10.0f, axis);
  sendPosition();
  writeWord(VP_MOTOR_ICON, 1);
}

void RTS::onMoveKey(const uint16_t value) {
  if (value == 1)
    gotoPage(PAGE_SETTINGS);
  else if (WITHIN(value, 2, 4))
    axis_page = value - 2;  // The screen changes page itself
}

void RTS::onFilamentKey(const uint16_t value) {
  switch (value) {
    case 1: gotoPage(PAGE_SETTINGS); break;
    case 3: case 4: { // Retract / extrude
      const bool load = value == 4;
      if (load && filamentMissing()) {
        fil_check = FC_CHANGE;
        gotoPage(PAGE_NO_FILAMENT);
        break;
      }
      const float mm = load ? filament_len : -filament_len;
      if (getActualTemp_celsius(E0) < fil_temp - 5) {
        pending_e = mm;
        writeWord(VP_FILAMENT_TEMP, fil_temp);
        gotoPage(PAGE_FIL_COLD);
      }
      else
        moveFilament(mm);
    } break;
    case 5: // Heat for the pending move
      setTargetTemp_celsius(_MAX(getTargetTemp_celsius(E0), fil_temp), E0);
      fil_heating = true;
      writeWord(VP_NOZZLE_TARGET, getTargetTemp_celsius(E0));
      gotoPage(PAGE_FIL_HEATING);
      break;
    case 6: // Don't heat
      fil_heating = false;
      pending_e = 0;
      gotoPage(PAGE_FILAMENT);
      break;
    case 7: // Stop heating and cool down
      fil_heating = false;
      pending_e = 0;
      coolDown();
      setStatus(MSG_COOLING);
      heat_state = HEAT_COOLING;
      gotoPage(PAGE_FILAMENT);
      break;
    case 9: gotoPage(PAGE_FIL_STOP_HEAT); break;
  }
}

void RTS::onLanguageKey(const uint16_t value) {
  if (WITHIN(value, 1, RTS_LANGUAGES)) {
    selectLanguage(value);
    return;
  }
  if (!WITHIN(pending_language, 1, RTS_LANGUAGES)) return;
  settings.language = pending_language;
  sendLanguage();
  saveSettings();
  if (value == 0x0A)
    gotoPage(PAGE_SETTINGS);
  else if (value == 0x0B) { // First boot
    setStatus(isMediaMounted() ? MSG_READY : MSG_NO_CARD);
    if (recovery_pending) powerLossDetected(); else gotoPage(PAGE_MAIN);
  }
}

void RTS::onPowerLossKey(const uint16_t value) {
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (value == 1) {
      print_state = PS_WARMUP;
      setStatus(MSG_HEATING);
      gotoPage(PAGE_PRINT_HEAT);
      injectCommands(F("M1000"));
    }
    else if (value == 2) {
      clearPrintInfo();
      gotoPage(PAGE_MAIN);
      injectCommands(F("M1000C"));
    }
  #else
    UNUSED(value);
  #endif
}

void RTS::onNoFilamentKey(const uint16_t value) {
  if (value == 1) {
    if (filamentMissing()) return;
    switch (fil_check) {
      case FC_START:  startSelectedFile(); break;
      case FC_RESUME: resumeJob(); break;
      case FC_CHANGE: gotoPage(PAGE_FILAMENT); break;
      default: break;
    }
  }
  else if (value == 2) {
    switch (fil_check) {
      case FC_START:  gotoPage(PAGE_FILES); break;
      case FC_RESUME: gotoPage(PAGE_PAUSED); break;
      case FC_CHANGE: gotoPage(PAGE_FILAMENT); break;
      default: break;
    }
  }
  fil_check = FC_NONE;
}

#endif // DGUS_LCD_UI_SERMOON_D1
