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
 * lcd/extui/cr10spro_v2/cr10spro_v2_rts.cpp
 *
 * Creality CR-10S Pro V2 stock DWIN touchscreen (factory DWIN_SET).
 * Ported from Creality's Marlin 1.1.6 LCD_RTS (CR-10S Pro V2 V1.70.0 BL).
 */

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_CR10SPROV2

#include "cr10spro_v2_rts.h"

#include "../../../MarlinCore.h"
#include "../../../core/mstring.h"
#include "../../../module/motion.h"

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
  RTS::StatusIcon status_icon = RTS::ICON_READY;

  bool booted = false, recovery_pending = false;
  uint8_t boot_step = 0;
  millis_t next_boot_ms = 0, next_update_ms = 0;

  bool fan_on = false;
  celsius_t last_target_hotend = 0, last_target_bed = 0;
  uint8_t last_percent = 0xFF;

  bool homing = false;
  uint8_t homing_icon = 0;
  HomeReturn home_return = HR_NONE;
  uint8_t axis_page = 0;          // 0 = 10mm, 1 = 1mm, 2 = 0.1mm

  bool auto_leveling = false;
  uint8_t probe_count = 0;
  uint32_t probed_points = 0;     // One bit per mesh point, to count each once

  float filament_len[2] = { 10, 10 };
  float pending_e = 0;            // Filament move waiting for the nozzle to heat
  bool fil_heating = false;
  celsius_t fil_temp = PREHEAT_1_TEMP_HOTEND;
  FilamentCheck fil_check = FC_NONE;

  bool reheating = false;
  uint8_t reheat_icon = 0;

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    PauseMessage pause_msg = PAUSE_MESSAGE_STATUS;
  #endif

  int8_t selected_file = -1;
  uint8_t file_count = 0;
  uint16_t file_index[RTS_FILE_SLOTS];

  bool energy_pending = false;    // Bed not yet turned off by energy saving
  uint32_t energy_start_s = 0;
  celsius_t energy_bed_temp = 0;

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

  // Target is 0 or set below current temp
  bool nozzleAtTemp() {
    const celsius_t t = getTargetTemp_celsius(E0);
    return !t || t <= getActualTemp_celsius(E0) + (TEMP_WINDOW);
  }

  // Target is 0 or set below current temp
  bool bedAtTemp() {
    const celsius_t t = getTargetTemp_celsius(BED);
    return !t || t <= getActualTemp_celsius(BED) + (TEMP_BED_WINDOW);
  }

  // Current temp percent of target
  uint8_t heatPercent() {
    const celsius_t t = getTargetTemp_celsius(E0),
                    c = getActualTemp_celsius(E0);
    return c >= t ? 100 : c * 100 / t;
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

void RTS::writeText(const uint16_t vp, const char *str) {
  const uint8_t len = _MIN(strlen(str), size_t(2 * FILENAME_WORDS));
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

void RTS::gotoPage(const page_t p, const uint8_t offset/*=0*/) {
  writeLong(VP_PAGE, RTS_PAGE_BASE + (settings.chinese ? p.cn : p.en) + offset);
}

void RTS::setStatusIcon(const StatusIcon icon) {
  status_icon = icon;
  writeWord(VP_STATUS_ICON, icon + (settings.chinese ? 0 : RTS_ICON_EN_OFFSET));
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

void RTS::sendLanguage() {
  setStatusIcon(status_icon);
  writeWord(VP_ENERGY_KEY_ICON, settings.energy_saving ? 3 : 2);
  writeWord(VP_ENERGY_ICON, (settings.chinese ? 7 : 9) + (settings.energy_saving ? 0 : 1));
  clearWords(VP_WEBSITE, 10);
  writeText(VP_WEBSITE, WEBSITE_URL);
}

void RTS::sendVolume() {
  const uint8_t v = settings.volume;
  writeWord(VP_VOLUME_ICON, v ? _MAX((v + 1) / 32, 1) - 1 : 0);
  writeWord(VP_SOUND_ICON, v ? 8 : 9);
  writeWord(VP_VOLUME, v);
  writeWord(VP_SOUND + 1, uint16_t(v) << 8);
}

void RTS::sendZOffset() { writeWord(VP_ZOFFSET, int16_t(LROUND(getZOffset_mm() * 100))); }

// The screen lists mesh values row by row, reversing every other row
void RTS::sendMeshPoint(const int8_t x, const int8_t y, const float z) {
  if (!WITHIN(x, 0, RTS_MESH_SIZE - 1) || !WITHIN(y, 0, RTS_MESH_SIZE - 1)) return;
  const uint8_t i = y * RTS_MESH_SIZE + ((y & 1) ? RTS_MESH_SIZE - 1 - x : x);
  writeWord(VP_MESH_VALUES + i * 2, isnan(z) ? 0 : int16_t(LROUND(z * 1000)));
}

void RTS::sendMesh() {
  for (uint8_t y = 0; y < RTS_MESH_SIZE; ++y)
    for (uint8_t x = 0; x < RTS_MESH_SIZE; ++x)
      sendMeshPoint(x, y, getMeshPoint({ x, y }));
  writeWord(VP_LEVELING_ICON, getLevelingActive() ? 3 : 2);
}

void RTS::sendPosition() {
  writeWord(VP_AXIS_X, LROUND(getAxisPosition_mm(X) * 10));
  writeWord(VP_AXIS_Y, LROUND(getAxisPosition_mm(Y) * 10));
  writeWord(VP_AXIS_Z, LROUND(getAxisPosition_mm(Z) * 10));
}

void RTS::showFilename(const uint16_t vp, const char *name) {
  char buf[RTS_FILENAME_LEN + 1];
  const uint8_t len = strlen(name), pad = len < RTS_FILENAME_LEN ? (RTS_FILENAME_LEN - len) / 2 : 0;
  memset(buf, ' ', pad);
  strlcpy(buf + pad, name, sizeof(buf) - pad);
  clearWords(vp, FILENAME_WORDS);
  writeText(vp, buf);
}

void RTS::clearPrintInfo() {
  writeWord(VP_FILE_ICON, 11);
  writeWord(VP_PROGRESS_BAR, 0);
  writeWord(VP_PROGRESS_BAR + 1, 0);
  writeWord(VP_PERCENTAGE, 0);
  writeWord(VP_TIME_HOUR, 0);
  writeWord(VP_TIME_MIN, 0);
  clearWords(VP_PRINT_FILENAME, FILENAME_WORDS);
  clearWords(VP_CHOSEN_FILENAME, FILENAME_WORDS);
  clearWords(VP_FILE_COUNT, 8);
  last_percent = 0xFF;
}

void RTS::clearFileList() {
  for (uint8_t i = 0; i < RTS_FILE_SLOTS; ++i) {
    clearWords(VP_FILE_NAMES + i * 10, 10);
    writeWord(SP_FILE_NAMES + (i + 1) * 16, 0xFFFF);
    writeWord(VP_FILE_ICON + 1 + i, 10);
    writeWord(VP_FILE_SELECT_ICON + 1 + i, 10);
  }
  clearWords(VP_CHOSEN_FILENAME, FILENAME_WORDS);
  clearWords(VP_FILE_COUNT, 8);
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
    makeDisplayName(name, files.filename());
    writeText(VP_FILE_NAMES + file_count * 10, name);
    writeWord(VP_FILE_ICON + 1 + file_count, 1);
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
  if (!WITHIN(z, PROBE_OFFSET_ZMIN, PROBE_OFFSET_ZMAX)) return;
  babystepAxis_steps(mmToWholeSteps(z - getZOffset_mm(), Z), Z);
  setZOffset_mm(z);
  zoffset_dirty = true;
  sendZOffset();
}

void RTS::moveToTrammingPoint(const float x, const float y) {
  const int fz = motion.homing_feedrate_mm_m.z;
  MString<64> cmd(F("G1Z3F"), fz, F("\nG1X"), int(x), 'Y', int(y), 'F', int(motion.homing_feedrate_mm_m.x), F("\nG1Z0F"), fz);
  injectCommands(cmd);
}

void RTS::extrudeFilament(const float mm) {
  setAxisPosition_mm(getAxisPosition_mm(E0) + mm, E0);
  writeWord(VP_FILAMENT_LEN1, LROUND(filament_len[0] * 10));
  writeWord(VP_FILAMENT_LEN2, LROUND(filament_len[1] * 10));
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
  makeDisplayName(name, files.filename());
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
  RTS_SERIAL.begin(115200);
  settings.volume = 0x80;

  writeWord(VP_FEEDRATE, 100);
  writeWord(VP_NOZZLE_TARGET, 0);
  writeWord(VP_BED_TARGET, 0);
  writeWord(VP_FAN_ICON, 2);

  char sizebuf[20];
  sprintf_P(sizebuf, PSTR("%d X %d X %d"), X_BED_SIZE, Y_BED_SIZE, Z_MAX_POS);
  clearWords(VP_MACHINE_NAME, 20);
  writeText(VP_MACHINE_NAME, MACHINE_NAME);
  writeText(VP_FIRMWARE_VERSION, SHORT_BUILD_VERSION);
  writeText(VP_PRINTER_SIZE, sizebuf);

  clearPrintInfo();
  clearFileList();
  sendLanguage();
}

void RTS::onIdle() {
  receive();
  if (!booted) return bootAnimation();

  if (print_state == PS_STOPPING) {
    print_state = PS_IDLE;
    clearPrintInfo();
    setStatusIcon(ICON_READY);
    gotoPage(PAGE_MAIN);
    if (zoffset_dirty) { zoffset_dirty = false; saveSettings(); }
  }

  const millis_t ms = millis();
  if (ELAPSED(ms, next_update_ms)) {
    next_update_ms = ms + RTS_UPDATE_INTERVAL;
    update();
  }
}

// The startup progress bar, then the main page or the power-loss prompt
void RTS::bootAnimation() {
  const millis_t ms = millis();
  if (PENDING(ms, next_boot_ms)) return;
  next_boot_ms = ms + RTS_BOOT_STEP_MS;

  if (boot_step == 0) {
    writeLong(VP_SOUND, RTS_START_SOUND);
    sendVolume();
  }
  if (boot_step <= 100)
    writeWord(VP_BOOT_PROGRESS, boot_step);
  else
    writeWord(VP_BOOT_PROGRESS + 1, boot_step - 100);

  if (++boot_step > 200) {
    booted = true;
    if (recovery_pending) powerLossDetected(); else gotoPage(PAGE_MAIN);
  }
}

void RTS::checkHeatDone() {
  if (isPrinting()) return;
  switch (heat_state) {
    case HEAT_HEATING:
      if (nozzleAtTemp() || (!getTargetTemp_celsius(E0) && bedAtTemp())) {
        heat_state = HEAT_IDLE;
        setStatusIcon(ICON_HEAT_DONE);
      }
      break;
    case HEAT_COOLING: {
      const celsius_t t = getTargetTemp_celsius(E0);
      if (getActualTemp_celsius(E0) <= (t ? t + 5 : 50)) {
        heat_state = HEAT_IDLE;
        setStatusIcon(ICON_COOL_DONE);
      }
    } break;
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
      setStatusIcon(rising ? ICON_HEATING : ICON_COOLING);
    }
    last_target_hotend = th;
    last_target_bed = tb;
  }
  checkHeatDone();

  const bool fan = getTargetFan_percent(FAN0) > 0;
  if (fan != fan_on) { fan_on = fan; writeWord(VP_FAN_ICON, fan ? 3 : 2); }

  // Printing starts once the job's heat-up has finished
  if (print_state == PS_WARMUP && isPrintingFromMedia() && nozzleAtTemp() && bedAtTemp()) {
    print_state = PS_PRINTING;
    energy_start_s = getProgress_seconds_elapsed();
    setStatusIcon(ICON_PRINTING);
    gotoPage(PAGE_PRINTING);
  }

  if (WITHIN(print_state, PS_WARMUP, PS_PAUSED)) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    writeWord(VP_TIME_HOUR, elapsed / 3600);
    writeWord(VP_TIME_MIN, (elapsed % 3600) / 60);

    const uint8_t pct = getProgress_percent();
    if (pct != last_percent) {
      last_percent = pct;
      const uint8_t bar = pct ? pct + 1 : 0;
      writeWord(VP_PROGRESS_BAR, _MIN(bar * 2, 100));
      writeWord(VP_PROGRESS_BAR + 1, bar > 50 ? bar * 2 - 100 : 0);
      writeWord(VP_PERCENTAGE, pct);
    }

    // Energy saving turns off the bed five minutes into the print
    if (settings.energy_saving && energy_pending && print_state == PS_PRINTING
      && getAxisPosition_mm(Z) > 2 && elapsed > energy_start_s + 5 * 60
    ) {
      energy_pending = false;
      energy_bed_temp = tb;
      setTargetTemp_celsius(0, BED);
    }
  }

  // Waiting to heat for a filament change
  if (fil_heating) {
    writeWord(VP_HEAT_PERCENT, heatPercent());
    if (nozzleAtTemp()) {
      fil_heating = false;
      gotoPage(PAGE_FILAMENT);
      if (pending_e) { extrudeFilament(pending_e); pending_e = 0; }
    }
  }

  if (reheating) writeWord(VP_REHEAT_ICON, reheat_icon++ % 5);

  if (homing) {
    writeWord(VP_HOMING_ICON, homing_icon);
    if (++homing_icon > 9) homing_icon = 0;
  }
}

void RTS::printStarted() {
  if (print_state == PS_PAUSED) {
    print_state = PS_PRINTING;
    reheating = false;
    setStatusIcon(ICON_PRINTING);
    gotoPage(PAGE_PRINTING);
    return;
  }
  if (WITHIN(print_state, PS_WARMUP, PS_PRINTING)) return;

  // A new job, from the screen or from a host
  print_state = PS_WARMUP;
  energy_pending = true;
  energy_bed_temp = 0;
  last_percent = 0xFF;
  writeWord(VP_FILE_ICON, 10);
  writeWord(VP_ENERGY_ICON, (settings.chinese ? 7 : 9) + (settings.energy_saving ? 0 : 1));
  setStatusIcon(ICON_HEATING);
  gotoPage(PAGE_PRINT_HEAT);
}

void RTS::printPaused() {
  if (!WITHIN(print_state, PS_WARMUP, PS_PRINTING)) return;
  print_state = PS_PAUSED;
  setStatusIcon(ICON_PAUSED);
  gotoPage(PAGE_PAUSED);
}

// Finished jobs report onPrintDone right after the timer stops, so decide in onIdle
void RTS::printStopped() {
  if (WITHIN(print_state, PS_WARMUP, PS_PAUSED)) print_state = PS_STOPPING;
  reheating = fil_heating = false;
}

void RTS::printFinished() {
  print_state = PS_DONE;
  writeWord(VP_PERCENTAGE, 100);
  writeWord(VP_PROGRESS_BAR, 100);
  writeWord(VP_PROGRESS_BAR + 1, 100);
  gotoPage(PAGE_PRINT_DONE);
  if (zoffset_dirty) { zoffset_dirty = false; saveSettings(); }
}

void RTS::homingStarted() {
  homing = true;
  homing_icon = 0;
}

void RTS::homingFinished() {
  homing = false;
  sendPosition();
  switch (home_return) {
    case HR_MOVE:  gotoPage(PAGE_MOVE, axis_page); break;
    case HR_LEVEL: gotoPage(PAGE_LEVELING); break;
    default: break;
  }
  home_return = HR_NONE;
}

void RTS::meshPointProbed(const int8_t x, const int8_t y, const float z) {
  sendMeshPoint(x, y, z);
  if (!auto_leveling || !WITHIN(x, 0, RTS_MESH_SIZE - 1) || !WITHIN(y, 0, RTS_MESH_SIZE - 1)) return;
  // A mesh reset reports every point as 0 after clearing it to NAN
  if (z == 0 && isnan(getMeshPoint({ uint8_t(x), uint8_t(y) }))) return;
  // G29 reports each probed point, then every point again when the new mesh is applied
  const uint8_t i = y * RTS_MESH_SIZE + x;
  if (TEST32(probed_points, i)) return;
  SBI32(probed_points, i);
  writeWord(VP_AUTOLEVEL_ICON, ++probe_count);
}

void RTS::levelingFinished() {
  sendMesh();
  if (auto_leveling) {
    auto_leveling = false;
    settings.leveling = getLevelingActive();
    if (getLevelingIsValid()) saveSettings();
    gotoPage(PAGE_LEVELING);
  }
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
  if (!isPrinting()) setStatusIcon(ICON_READY);
}

void RTS::mediaRemoved() {
  clearFileList();
  setStatusIcon(ICON_NO_CARD);
}

void RTS::filamentRunout() {
  if (isPrintingFromMedia()) gotoPage(PAGE_WAIT);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void RTS::pauseMessage(const PauseMessage message) {
    pause_msg = message;
    reheating = false;
    switch (message) {
      case PAUSE_MESSAGE_PARKING:
      case PAUSE_MESSAGE_CHANGING:
      case PAUSE_MESSAGE_UNLOAD:
      case PAUSE_MESSAGE_LOAD:
      case PAUSE_MESSAGE_RESUME:
        gotoPage(PAGE_WAIT);
        break;
      case PAUSE_MESSAGE_PURGE:
        gotoPage(TERN(ADVANCED_PAUSE_CONTINUOUS_PURGE, PAGE_PAUSED, PAGE_WAIT));
        break;
      case PAUSE_MESSAGE_WAITING:
      case PAUSE_MESSAGE_INSERT:
      case PAUSE_MESSAGE_OPTION:
        gotoPage(PAGE_PAUSED);
        break;
      case PAUSE_MESSAGE_HEAT:
        gotoPage(PAGE_RUNOUT_COLD);
        break;
      case PAUSE_MESSAGE_HEATING:
        reheating = true;
        gotoPage(PAGE_REHEATING);
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
    case VP_PRINT_FILE:   onPrintFileKey(value); break;
    case VP_ADJUST:       onAdjustKey(value); break;
    case VP_FEEDRATE:     setFeedrate_percent(value); break;
    case VP_STOP_PRINT:
    case VP_PAUSE_PRINT:
    case VP_RESUME_PRINT: onPrintChoice(vp, value); break;
    case VP_ZOFFSET:      setZOffset(int16_t(value) / 100.0f); break;
    case VP_TEMP_CONTROL: onTempControl(value); break;
    case VP_TEMP_SET:
    case VP_NOZZLE_TARGET:
    case VP_BED_TARGET:   onTempSet(vp, value); break;
    case VP_SETTING:      onSetting(value); break;
    case VP_RETURN_BACK:  onReturnBack(value); break;
    case VP_BED_LEVEL:    onBedLevel(value); writeWord(VP_FILE_ICON, 10); break;
    case VP_AUTOHOME:
    case VP_AXIS_X:
    case VP_AXIS_Y:
    case VP_AXIS_Z:       onAxis(vp, value); break;
    case VP_FILAMENT_LEN1:
    case VP_FILAMENT:
    case VP_FILAMENT_LEN2: onFilament(vp, value); break;
    case VP_LANGUAGE:
      settings.chinese = value != 0;
      sendLanguage();
      saveSettings();
      break;
    case VP_NO_FILAMENT:  onNoFilament(value); break;
    case VP_POWER_LOSS:   onPowerLoss(value); break;
    case VP_VOLUME:       onVolume(value); break;
    case VP_FILE_CHOOSE:  onFileChoose(value); break;
    case VP_FILE_PRINT:   onFilePrint(value); break;
    default: break;
  }
}

void RTS::onPrintFileKey(const uint16_t value) {
  switch (value) {
    case 1: // Open the file list
      refreshFileList();
      gotoPage(PAGE_FILES);
      break;
    case 2: // Leave the print-finished page
      print_state = PS_IDLE;
      clearPrintInfo();
      setStatusIcon(ICON_READY);
      gotoPage(PAGE_MAIN);
      break;
    case 3: showTempPage(); break;
  }
}

void RTS::onAdjustKey(const uint16_t value) {
  switch (value) {
    case 1: writeWord(VP_FAN_ICON, fan_on ? 3 : 2); break;
    case 2: showPrintPage(); break;
    case 3: // Fan toggle
      fan_on = !fan_on;
      setTargetFan_percent(fan_on ? 100 : 0, FAN0);
      writeWord(VP_FAN_ICON, fan_on ? 3 : 2);
      break;
    case 4: // Energy saving toggle
      settings.energy_saving = !settings.energy_saving;
      if (settings.energy_saving)
        energy_pending = true;
      else if (energy_bed_temp) {
        setTargetTemp_celsius(energy_bed_temp, BED);
        energy_bed_temp = 0;
      }
      sendLanguage();
      saveSettings();
      break;
  }
}

void RTS::onPrintChoice(const uint16_t vp, const uint16_t value) {
  switch (vp) {
    case VP_STOP_PRINT:
      if (value == 0xF0) { showPrintPage(); break; } // Stop dialog cancelled
      gotoPage(PAGE_WAIT);
      stopPrint();
      break;

    case VP_PAUSE_PRINT:
      if (value != 0xF1) break;
      gotoPage(PAGE_WAIT);
      pausePrint();
      break;

    case VP_RESUME_PRINT:
      if (value == 1)
        resumeJob();
      else if (value == 2) // Reheat after the heater timed out
        setUserConfirmed();
      break;
  }
}

void RTS::onTempControl(const uint16_t value) {
  switch (value) {
    case 3: // Fan toggle
      fan_on = !fan_on;
      setTargetFan_percent(fan_on ? 100 : 0, FAN0);
      showTempPage();
      break;
    case 5: // PLA preheat
      fil_temp = PREHEAT_1_TEMP_HOTEND;
      setTargetTemp_celsius(fil_temp, E0);
      setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
      break;
    case 6: // ABS preheat
      fil_temp = PREHEAT_2_TEMP_HOTEND;
      setTargetTemp_celsius(fil_temp, E0);
      setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
      break;
    case 0xF1: // Cool down
      coolDown();
      fan_on = true;
      setTargetFan_percent(100, FAN0);
      gotoPage(PAGE_TEMP_FAN_ON);
      break;
  }
}

void RTS::onTempSet(const uint16_t vp, const uint16_t value) {
  switch (vp) {
    case VP_TEMP_SET:
      if (value == 0) showTempPage();
      else if (value == 1) setTargetTemp_celsius(0, E0);
      else if (value == 2) setTargetTemp_celsius(0, BED);
      break;
    case VP_NOZZLE_TARGET: setTargetTemp_celsius(value, E0); break;
    case VP_BED_TARGET:    setTargetTemp_celsius(value, BED); break;
  }
}

void RTS::onSetting(const uint16_t value) {
  switch (value) {
    case 1: // Bed leveling: home, lower the nozzle to Z0
      writeWord(VP_LEVELING_ICON, getLevelingActive() ? 3 : 2);
      writeWord(VP_FILE_ICON, 10);
      home_return = HR_LEVEL;
      injectCommands(F("G28\nG1F200Z0"));
      gotoPage(PAGE_HOMING);
      break;
    case 2: // Filament change
      filament_len[0] = filament_len[1] = 10;
      writeWord(VP_FILAMENT_LEN1, 100);
      writeWord(VP_FILAMENT_LEN2, 100);
      writeWord(VP_NOZZLE_TEMP, getActualTemp_celsius(E0));
      writeWord(VP_NOZZLE_TARGET, getTargetTemp_celsius(E0));
      gotoPage(PAGE_FILAMENT);
      break;
    case 3: // Move axes
      axis_page = 0;
      sendPosition();
      gotoPage(PAGE_MOVE);
      break;
    case 5: // Printer information
      clearWords(VP_WEBSITE, 10);
      writeText(VP_WEBSITE, WEBSITE_URL);
      break;
    case 6: // Disable steppers
      injectCommands(F("M84"));
      writeWord(VP_FILE_ICON, 11);
      break;
  }
}

void RTS::onReturnBack(const uint16_t value) {
  if (value == 1) {
    if (zoffset_dirty) { zoffset_dirty = false; saveSettings(); }
    sendZOffset();
    gotoPage(PAGE_TOOLS);
  }
  else if (value == 2)
    gotoPage(PAGE_LEVELING);
}

void RTS::onBedLevel(const uint16_t value) {
  #if ENABLED(LCD_BED_TRAMMING)
    constexpr float lfrb[4] = BED_TRAMMING_INSET_LFRB;
  #else
    constexpr float lfrb[4] = { 30, 30, 30, 30 };
  #endif
  switch (value) {
    case 1: // Home Z and lower the nozzle for the paper test
      home_return = HR_LEVEL;
      injectCommands(isAxisPositionKnown(X) && isAxisPositionKnown(Y) ? F("G28Z\nG1F200Z0") : F("G28\nG1F200Z0"));
      break;
    case 2: setZOffset(getZOffset_mm() + RTS_ZOFFSET_STEP); break;
    case 3: setZOffset(getZOffset_mm() - RTS_ZOFFSET_STEP); break;
    case 4: // Manual tramming
      injectCommands(F("G28"));
      gotoPage(PAGE_TRAMMING);
      break;
    case 5: // Probe the mesh
      auto_leveling = true;
      probe_count = 0;
      probed_points = 0;
      writeWord(VP_AUTOLEVEL_ICON, 1);
      gotoPage(PAGE_AUTOLEVEL);
      injectCommands(isMachineHomed() ? F("G29") : F("G28\nG29"));
      break;
    case  6: moveToTrammingPoint(X_CENTER, Y_CENTER); break;
    case  7: moveToTrammingPoint(X_MIN_BED + lfrb[0], Y_MIN_BED + lfrb[1]); break;
    case  8: moveToTrammingPoint(X_MAX_BED - lfrb[2], Y_MIN_BED + lfrb[1]); break;
    case  9: moveToTrammingPoint(X_MAX_BED - lfrb[2], Y_MAX_BED - lfrb[3]); break;
    case 10: moveToTrammingPoint(X_MIN_BED + lfrb[0], Y_MAX_BED - lfrb[3]); break;
    case 11: // Leveling on/off
      setLevelingActive(!getLevelingActive());
      settings.leveling = getLevelingActive();
      writeWord(VP_LEVELING_ICON, getLevelingActive() ? 3 : 2);
      sendZOffset();
      saveSettings();
      break;
  }
}

void RTS::onAxis(const uint16_t vp, const uint16_t value) {
  axis_t axis;
  switch (vp) {
    case VP_AUTOHOME:
      if (value == 3) {
        home_return = HR_MOVE;
        injectCommands(F("G28"));
        writeWord(VP_FILE_ICON, 10);
        gotoPage(PAGE_HOMING);
      }
      else
        axis_page = value;
      return;
    case VP_AXIS_X: axis = X; break;
    case VP_AXIS_Y: axis = Y; break;
    default:        axis = Z; break;
  }
  setAxisPosition_mm(value / 10.0f, axis);
  sendPosition();
  writeWord(VP_FILE_ICON, 10);
}

void RTS::onFilament(const uint16_t vp, const uint16_t value) {
  if (vp == VP_FILAMENT_LEN1) { filament_len[0] = value / 10.0f; return; }
  if (vp == VP_FILAMENT_LEN2) { filament_len[1] = value / 10.0f; return; }

  switch (value) {
    case 1: case 2: case 3: case 4: { // Unload / load, length 1 or 2
      const bool load = !(value & 1);
      if (load && filamentMissing()) {
        fil_check = FC_CHANGE;
        gotoPage(PAGE_NO_FILAMENT);
        break;
      }
      const float mm = filament_len[value > 2] * (load ? 1 : -1);
      if (getActualTemp_celsius(E0) < fil_temp - 5) {
        pending_e = mm;
        writeWord(VP_HEAT_TARGET, fil_temp);
        gotoPage(PAGE_FIL_COLD);
      }
      else
        extrudeFilament(mm);
    } break;
    case 5: // Heat for the pending move
      setTargetTemp_celsius(_MAX(getTargetTemp_celsius(E0), fil_temp), E0);
      fil_heating = true;
      writeWord(VP_HEAT_PERCENT, heatPercent());
      writeWord(VP_NOZZLE_TARGET, getTargetTemp_celsius(E0));
      gotoPage(PAGE_FIL_HEATING);
      break;
    case 6:    // Don't heat
    case 0xF1: // Stop waiting to heat
      fil_heating = false;
      pending_e = 0;
      gotoPage(PAGE_FILAMENT);
      break;
  }
}

void RTS::onNoFilament(const uint16_t value) {
  if (value == 1) {
    if (filamentMissing()) return;
    switch (fil_check) {
      case FC_START:  startSelectedFile(); break;
      case FC_RESUME: resumeJob(); break;
      case FC_CHANGE: gotoPage(PAGE_FILAMENT); break;
      default: break;
    }
  }
  else if (value == 0) {
    switch (fil_check) {
      case FC_START:  gotoPage(PAGE_FILES); break;
      case FC_RESUME: gotoPage(PAGE_PAUSED); break;
      case FC_CHANGE: gotoPage(PAGE_FILAMENT); break;
      default: break;
    }
  }
  fil_check = FC_NONE;
}

void RTS::onPowerLoss(const uint16_t value) {
  #if ENABLED(POWER_LOSS_RECOVERY)
    if (value == 1) {
      print_state = PS_WARMUP;
      energy_pending = true;
      setStatusIcon(ICON_HEATING);
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

void RTS::onVolume(const uint16_t value) {
  settings.volume = _MIN(value, uint16_t(0xFF));
  sendVolume();
  saveSettings();
}

void RTS::onFileChoose(const uint16_t value) {
  if (!value || value > file_count) return;
  selected_file = value - 1;

  FileList files;
  if (!files.seek(file_index[selected_file])) return;
  char name[RTS_FILENAME_LEN];
  makeDisplayName(name, files.filename());
  showFilename(VP_CHOSEN_FILENAME, name);

  char buf[8];
  sprintf_P(buf, PSTR("%d/%d"), value, file_count);
  clearWords(VP_FILE_COUNT, 8);
  writeText(VP_FILE_COUNT, buf);

  for (uint8_t i = 1; i <= file_count; ++i) {
    writeWord(SP_FILE_NAMES + i * 16, 0xFFFF);
    writeWord(VP_FILE_SELECT_ICON + i, 10);
  }
  writeWord(SP_FILE_NAMES + value * 16, 0x87F0);
  writeWord(VP_FILE_SELECT_ICON + value, 6);
}

void RTS::onFilePrint(const uint16_t value) {
  if (value == 1) startSelectedFile();
}

#endif // DGUS_LCD_UI_CR10SPROV2
