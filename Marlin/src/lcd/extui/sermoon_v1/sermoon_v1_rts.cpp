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
 * lcd/extui/sermoon_v1/sermoon_v1_rts.cpp
 *
 * Creality Sermoon V1 / V1 Pro stock DWIN touchscreen (factory DWIN_SET, screen firmware 1.0.13).
 * Ported from Creality's Marlin 2.0.6.1 lcdAutoUI (Sermoon V1 V1.0.33).
 */

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_SERMOON_V1

#include "sermoon_v1_rts.h"

#include "../../../MarlinCore.h"
#include "../../../core/mstring.h"
#include "../../../module/motion.h"
#include "../../../gcode/gcode.h"

#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#include "../../../module/printcounter.h"

using namespace ExtUI;

RTS rts;
RTS::settings_t RTS::settings;

namespace {

  enum PrintState : uint8_t { PS_IDLE, PS_WARMUP, PS_PRINTING, PS_PAUSED, PS_STOPPING, PS_DONE };
  enum FilamentOp : uint8_t { FO_NONE, FO_LOAD_HEAT, FO_LOADING, FO_UNLOAD_HEAT, FO_UNLOADING };
  enum CaliState : uint8_t { CAL_NONE, CAL_CENTER_MOVE, CAL_CENTER, CAL_MESH_MOVE, CAL_MESH, CAL_FINISH };

  PrintState print_state = PS_IDLE;
  uint8_t current_page = RTS::PAGE_BOOT;

  bool booted = false, recovery_pending = false;
  uint8_t boot_step = 0;
  millis_t next_boot_ms = 0, next_update_ms = 0, next_position_ms = 0;

  celsius_t last_target_hotend = -1, last_target_bed = -1;
  int16_t last_feedrate = -1;
  uint8_t last_percent = 0xFF;
  int8_t last_box_fan = -1, last_box_led = -1;

  bool move_after_home = false;
  uint8_t step_index = 0;         // 0 = 10mm, 1 = 1mm, 2 = 0.1mm

  FilamentOp fil_op = FO_NONE;
  celsius_t fil_saved_target = 0;

  bool runout_paused = false;

  CaliState cal_state = CAL_NONE;
  uint8_t cal_point = 0;
  float cal_offset = 0;

  int8_t selected_file = -1;
  uint8_t file_count = 0;
  uint16_t file_index[RTS_FILE_SLOTS];

  // Creality Cloud: the WiFi board streams the job over the host serial port and reports with M79
  enum WifiState : uint8_t { WIFI_START = 1, WIFI_PAUSE, WIFI_RESUME, WIFI_STOP, WIFI_FINISH, WIFI_POWER_LOSS };
  bool app_print = false, app_recovery = false, recovery_prompt = false;
  bool wifi_linked = false;       // The WiFi board sent its MAC since its last reset
  int8_t last_wifi_icon = -1;
  uint8_t net_reset_secs = 0;
  float app_pause_z = NAN;
  uint8_t wifi_return_page = RTS::PAGE_MAIN;
  char wifi_mac[RTS_TEXT_LEN + 1] = "";

  // Tell the WiFi board about a print event, as the stock firmware does
  void notifyWifi(const WifiState s) { SERIAL_ECHOLNPGM("M79 S", int(s)); }

  // M115 also reports this; the power-loss popup sends it straight away
  void reportRecoveryPrompt(const bool on) { recovery_prompt = on; SERIAL_ECHOLNPGM("Cap:IS_PLR:", int(on)); }

  // Strip the extension; long names keep 22 characters and gain ".."
  void makeDisplayName(char * const out, const char * const name) {
    const char * const dot = strrchr(name, '.');
    size_t len = (dot && dot != name) ? size_t(dot - name) : strlen(name);
    if (len >= 25) {
      memcpy(out, name, 22);
      out[22] = out[23] = '.';
      len = 24;
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

  bool isIdleMachine() { return !commandsInQueue() && !isMoving(); }

  bool boxFanOn() { return TERN0(HAS_FAN1, getTargetFan_percent(FAN1) > 0); }
  void setBoxFan(const bool on) { TERN_(HAS_FAN1, setTargetFan_percent(on ? 100 : 0, FAN1)); UNUSED(on); }

  bool boxLedOn() { return TERN0(CASE_LIGHT_ENABLE, getCaseLightState()); }
  void setBoxLed(const bool on) { TERN_(CASE_LIGHT_ENABLE, setCaseLightState(on)); UNUSED(on); }

  constexpr float jog_steps[] = { 10, 1, 0.1f };

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
  const uint8_t len = _MIN(strlen(str), size_t(RTS_TEXT_LEN));
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
  current_page = page;
  writeLong(VP_PAGE, RTS_PAGE_BASE + page);
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

// Every page draws its labels from the language number
void RTS::sendLanguage() {
  writeWord(VP_LANGUAGE, settings.language);
  for (uint8_t i = 0; i < RTS_LANGUAGES; ++i)
    writeWord(VP_LANGUAGE_ICON + i, i == settings.language);
}

void RTS::sendPosition() {
  writeWord(VP_AXIS_X, int16_t(LROUND(getAxisPosition_mm(X) * 10)));
  writeWord(VP_AXIS_Y, int16_t(LROUND(getAxisPosition_mm(Y) * 10)));
  writeWord(VP_AXIS_Z, int16_t(LROUND(getAxisPosition_mm(Z) * 10)));
}

void RTS::showTextField(const uint16_t vp, const char *str) {
  clearWords(vp, TEXT_WORDS);
  writeText(vp, str);
}

void RTS::showError(FSTR_P const msg) {
  char buf[RTS_TEXT_LEN + 1];
  strlcpy_P(buf, FTOP(msg), sizeof(buf));
  showTextField(VP_ERROR_TEXT, buf);
  gotoPage(PAGE_ERROR);
}

void RTS::clearPrintInfo() {
  writeWord(VP_PROGRESS_ICON, 0);
  writeWord(VP_PERCENTAGE, 0);
  writeWord(VP_TIME_HOUR, 0);
  writeWord(VP_TIME_MIN, 0);
  clearWords(VP_PRINT_FILENAME, TEXT_WORDS);
  last_percent = 0xFF;
  // Total print time, in hours x 100
  #if ENABLED(PRINTCOUNTER)
    writeLong(VP_TOTAL_TIME, uint32_t(print_job_timer.getStats().printTime / 36));
  #endif
}

void RTS::clearFileList() {
  for (uint8_t i = 0; i < RTS_FILE_SLOTS; ++i) {
    clearWords(VP_FILE_NAMES + i * 0x20, TEXT_WORDS);
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
  char name[RTS_TEXT_LEN];
  for (uint16_t i = files.count(); i-- && file_count < RTS_FILE_SLOTS;) {
    if (!files.seek(i) || files.isDir()) continue;
    file_index[file_count] = i;
    makeDisplayName(name, files.longFilename());
    writeText(VP_FILE_NAMES + file_count * 0x20, name);
    ++file_count;
  }
}

void RTS::selectFile(const uint8_t n) {
  if (n >= file_count) return;
  selected_file = n;
  for (uint8_t i = 0; i < file_count; ++i) writeWord(VP_FILE_ICON + i, i == n);
}

void RTS::showPrintPage() {
  switch (print_state) {
    case PS_WARMUP: gotoPage(PAGE_HEATING); break;
    case PS_PAUSED: gotoPage(runout_paused ? PAGE_NO_FILAMENT : PAGE_PAUSED); break;
    default:        gotoPage(PAGE_PRINTING); break;
  }
}

// The filament menu's return key goes back to the paused print or the print mode menu
void RTS::showFilamentReturn() {
  gotoPage(print_state == PS_PAUSED ? PAGE_PAUSED : PAGE_MODE);
}

bool RTS::filamentMissing() {
  #if HAS_FILAMENT_SENSOR
    return getFilamentRunoutEnabled() && (FilamentSensorBase::poll_runout_states() & 1);
  #else
    return false;
  #endif
}

void RTS::saveSettings() { TERN_(EEPROM_SETTINGS, injectCommands(F("M500"))); }

void RTS::startSelectedFile() {
  if (selected_file < 0 || !isMediaMounted()) return;
  if (filamentMissing()) {
    gotoPage(PAGE_NO_FILAMENT);
    return;
  }
  FileList files;
  if (!files.seek(file_index[selected_file])) return;
  char name[RTS_TEXT_LEN];
  makeDisplayName(name, files.longFilename());
  showTextField(VP_PRINT_FILENAME, name);
  printFile(files.shortFilename());
  notifyWifi(WIFI_START);
  printStarted();
}

void RTS::resumeJob() {
  if (filamentMissing()) {
    gotoPage(PAGE_NO_FILAMENT);
    return;
  }
  runout_paused = false;
  gotoPage(PAGE_HEATING);
  if (awaitingUserConfirm()) {
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
    #endif
    setUserConfirmed();
  }
  else
    resumePrint();
}

void RTS::jog(const AxisEnum axis, const int8_t dir) {
  if (!isMachineHomed() || isMoving()) return;
  const axis_t ax = axis == X_AXIS ? X : axis == Y_AXIS ? Y : Z;
  setAxisPosition_mm(getAxisPosition_mm(ax) + dir * jog_steps[step_index], ax);
  sendPosition();
}

//
// Filament load / unload
//

void RTS::startLoad() {
  fil_saved_target = getTargetTemp_celsius(E0);
  setTargetTemp_celsius(_MAX(fil_saved_target, RTS_FEED_TEMP), E0);
  fil_op = FO_LOAD_HEAT;
}

void RTS::startUnload() {
  fil_saved_target = getTargetTemp_celsius(E0);
  setTargetTemp_celsius(_MAX(fil_saved_target, RTS_FEED_TEMP), E0);
  fil_op = FO_UNLOAD_HEAT;
  writeWord(VP_UNLOAD_RETURN, print_state != PS_PAUSED);
  gotoPage(PAGE_UNLOAD_HEAT);
}

// Stop feeding and put the hotend target back
void RTS::finishFilament() {
  if (fil_op == FO_NONE) return;
  fil_op = FO_NONE;
  injectCommands(F("M410"));
  setTargetTemp_celsius(fil_saved_target, E0);
}

//
// Bed calibration: set Z0 at the bed centre, then the manual mesh
//

void RTS::startCalibration() {
  cal_state = CAL_CENTER_MOVE;
  TERN_(HAS_LEVELING, setLevelingActive(false));
  TERN_(HAS_SOFTWARE_ENDSTOPS, setSoftEndstopState(false));
  setBoxLed(true);
  gotoPage(PAGE_BUSY);
  MString<64> cmd(F("G28\nG1X"), int(X_CENTER), 'Y', int(Y_CENTER), F("F3000\nG1Z0F"), int(motion.homing_feedrate_mm_m.z));
  injectCommands(cmd);
}

void RTS::calibrationNext() {
  switch (cal_state) {
    case CAL_CENTER: {
      // The nozzle touches the bed here: make this Z0 from now on
      const float z = getAxisPosition_mm(Z);
      #if HAS_HOME_OFFSET
        MString<40> cmd(F("M206Z"), p_float_t(motion.home_offset.z - z, 2), F("\nG92Z0"));
      #else
        MString<40> cmd(F("G92Z0"));
      #endif
      injectCommands(cmd);
      #if ENABLED(MESH_BED_LEVELING)
        injectCommands(F("G29S1"));
        cal_point = 1;
        cal_state = CAL_MESH_MOVE;
      #elif HAS_BED_PROBE
        injectCommands(F("G28\nG29"));
        cal_state = CAL_FINISH;
      #else
        cal_state = CAL_FINISH;
      #endif
      gotoPage(PAGE_BUSY);
    } break;

    #if ENABLED(MESH_BED_LEVELING)
      case CAL_MESH:
        injectCommands(F("G29S2"));
        cal_state = ++cal_point > GRID_MAX_POINTS ? CAL_FINISH : CAL_MESH_MOVE;
        gotoPage(PAGE_BUSY);
        break;
    #endif

    default: break;
  }
}

void RTS::endCalibration() {
  if (cal_state == CAL_NONE) return;
  cal_state = CAL_NONE;
  TERN_(HAS_SOFTWARE_ENDSTOPS, setSoftEndstopState(true));
  TERN_(HAS_LEVELING, setLevelingActive(getLevelingIsValid()));
  setBoxLed(false);
  saveSettings();
  gotoPage(PAGE_SETTINGS);
}

//
// Marlin events
//

void RTS::onStartup() {
  RTS_SERIAL.begin(LCD_BAUDRATE);

  char sizebuf[24];
  sprintf_P(sizebuf, PSTR("%d x %d x %d"), int(X_BED_SIZE), int(Y_BED_SIZE), int(Z_MAX_POS));
  showTextField(VP_MACHINE_NAME, MACHINE_NAME);
  showTextField(VP_FW_VERSION, SHORT_BUILD_VERSION);
  showTextField(VP_PRINTER_SIZE, sizebuf);
  clearWords(VP_WIFI_MAC, TEXT_WORDS);

  writeWord(VP_FEEDRATE, 100);
  writeWord(VP_PLA_ICON, 0);
  writeWord(VP_ABS_ICON, 0);
  writeWord(VP_UNLOAD_RETURN, 1);
  clearPrintInfo();
  clearFileList();
}

void RTS::onIdle() {
  receive();
  if (!booted) return bootAnimation();

  if (print_state == PS_STOPPING) {
    print_state = PS_IDLE;
    runout_paused = false;
    app_print = false;
    clearPrintInfo();
    writeWord(VP_FEEDRATE, 100);
    gotoPage(PAGE_MAIN);
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

  writeWord(VP_BOOT_PROGRESS, boot_step);
  if (++boot_step <= 100) return;

  booted = true;
  sendLanguage();
  writeWord(VP_WIFI_LED_ICON, settings.wifi_led);
  if (wifi_mac[0]) showTextField(VP_WIFI_MAC, wifi_mac);
  injectCommands(F("M115"));  // Cap:WIFI tells the WiFi board whether it is switched on
  if (recovery_pending) powerLossDetected(); else gotoPage(PAGE_MAIN);
}

void RTS::update() {
  // Status bar on most pages
  writeWord(VP_NOZZLE_TEMP, getActualTemp_celsius(E0));
  writeWord(VP_BED_TEMP, getActualTemp_celsius(BED));
  const celsius_t th = getTargetTemp_celsius(E0), tb = getTargetTemp_celsius(BED);
  if (th != last_target_hotend) {
    writeWord(VP_NOZZLE_TARGET, th);
    writeWord(VP_HEATING_NOZZLE, th > 0);
    last_target_hotend = th;
  }
  if (tb != last_target_bed) {
    writeWord(VP_BED_TARGET, tb);
    writeWord(VP_HEATING_BED, tb > 0);
    last_target_bed = tb;
  }
  const int16_t fr = getFeedrate_percent();
  if (fr != last_feedrate) {
    writeWord(VP_FEEDRATE, fr);
    if (last_feedrate >= 0) SERIAL_ECHOLNPGM("FR:", fr, "%");
    last_feedrate = fr;
  }

  // Enclosure switches, which G-code may also change
  const int8_t bf = boxFanOn(), bl = boxLedOn();
  if (bf != last_box_fan) { writeWord(VP_BOX_FAN_ICON, bf); last_box_fan = bf; }
  if (bl != last_box_led) { writeWord(VP_BOX_LED_ICON, bl); last_box_led = bl; }
  const int8_t wl = settings.wifi_led && wifi_linked;
  if (wl != last_wifi_icon) { writeWord(VP_WIFI_LINK_ICON, wl); last_wifi_icon = wl; }

  // Printing starts once the job's heat-up has finished
  if (print_state == PS_WARMUP && (app_print || isOngoingPrintJob()) && nozzleAtTemp() && bedAtTemp()) {
    print_state = PS_PRINTING;
    gotoPage(PAGE_PRINTING);
  }

  // Cloud jobs report their own time and progress
  if (!app_print && WITHIN(print_state, PS_WARMUP, PS_PAUSED)) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    writeWord(VP_TIME_HOUR, _MIN(elapsed / 3600, 99U));
    writeWord(VP_TIME_MIN, (elapsed % 3600) / 60);
    const uint8_t pct = getProgress_percent();
    if (pct != last_percent) {
      last_percent = pct;
      writeWord(VP_PROGRESS_ICON, pct);
      writeWord(VP_PERCENTAGE, pct);
    }
  }

  // Filament load / unload, once the nozzle is hot
  switch (fil_op) {
    case FO_LOAD_HEAT:
      if (nozzleAtTemp()) {
        fil_op = FO_LOADING;
        setAxisPosition_mm(getAxisPosition_mm(E0) + RTS_LOAD_LENGTH, E0, RTS_LOAD_SPEED);
      }
      break;
    case FO_UNLOAD_HEAT:
      if (nozzleAtTemp()) {
        fil_op = FO_UNLOADING;
        setAxisPosition_mm(getAxisPosition_mm(E0) + RTS_UNLOAD_PUSH, E0, RTS_UNLOAD_PUSH_SPEED);
        setAxisPosition_mm(getAxisPosition_mm(E0) - RTS_UNLOAD_PULL, E0, RTS_UNLOAD_PULL_SPEED);
        gotoPage(PAGE_UNLOAD_PULL);
      }
      break;
    default: break;
  }

  // Bed calibration waits for each move to finish
  if (isIdleMachine()) switch (cal_state) {
    case CAL_CENTER_MOVE:
    case CAL_MESH_MOVE:
      cal_state = cal_state == CAL_CENTER_MOVE ? CAL_CENTER : CAL_MESH;
      cal_offset = 0;
      writeWord(VP_CALI_POINT_ICON, cal_point);
      writeWord(VP_CALI_ZOFFSET, 0);
      writeWord(VP_CALI_ZHEIGHT, int16_t(LROUND(getAxisPosition_mm(Z) * 100)));
      gotoPage(PAGE_CALIBRATE);
      break;
    case CAL_FINISH: endCalibration(); break;
    default: break;
  }

  // The WiFi board resets its network while the screen counts down
  if (net_reset_secs) {
    writeWord(VP_NET_RESET_TIME, --net_reset_secs);
    if (!net_reset_secs) gotoPage(PAGE_MAIN);
  }

  // Keep the jog pages in step with moves made from elsewhere
  if (WITHIN(current_page, PAGE_MOVE, PAGE_MOVE + 2) && ELAPSED(millis(), next_position_ms) && !isMoving()) {
    next_position_ms = millis() + 2000;
    sendPosition();
  }
}

void RTS::printStarted() {
  if (print_state == PS_PAUSED) {
    print_state = PS_PRINTING;
    runout_paused = false;
    gotoPage(PAGE_PRINTING);
    return;
  }
  if (WITHIN(print_state, PS_WARMUP, PS_PRINTING)) return;

  // A new job, from the screen or from a host
  print_state = PS_WARMUP;
  last_percent = 0xFF;
  writeWord(VP_PROGRESS_ICON, 0);
  writeWord(VP_PERCENTAGE, 0);
  gotoPage(PAGE_HEATING);
}

void RTS::printPaused() {
  if (!WITHIN(print_state, PS_WARMUP, PS_PRINTING)) return;
  print_state = PS_PAUSED;
  showPrintPage();
}

// Finished jobs report onPrintDone right after the timer stops, so decide in onIdle
void RTS::printStopped() {
  if (WITHIN(print_state, PS_WARMUP, PS_PAUSED)) print_state = PS_STOPPING;
  if (fil_op != FO_NONE) finishFilament();
}

void RTS::printFinished() {
  if (!app_print) notifyWifi(WIFI_FINISH);
  print_state = PS_DONE;
  writeWord(VP_PROGRESS_ICON, 100);
  writeWord(VP_PERCENTAGE, 100);
  gotoPage(PAGE_PRINT_DONE);
}

void RTS::homingFinished() {
  sendPosition();
  if (move_after_home) {
    move_after_home = false;
    gotoPage(PAGE_MOVE + step_index);
  }
}

void RTS::powerLossDetected() {
  recovery_pending = true;
  if (!booted) return;
  recovery_pending = false;
  if (app_recovery)
    showTextField(VP_RECOVERY_FILENAME, "CLOUDPRINT.gcode");
  else {
    #if ENABLED(POWER_LOSS_RECOVERY)
      const char * const path = recovery.info.sd_filename, * const slash = strrchr(path, '/');
      char name[RTS_TEXT_LEN];
      makeDisplayName(name, slash ? slash + 1 : path);
      showTextField(VP_RECOVERY_FILENAME, name);
    #endif
    notifyWifi(WIFI_POWER_LOSS);
  }
  reportRecoveryPrompt(true);
  gotoPage(PAGE_RECOVERY);
}

void RTS::mediaInserted() { refreshFileList(); }

void RTS::mediaRemoved() {
  clearFileList();
  if (WITHIN(current_page, PAGE_FILES, PAGE_FILES + 4)) gotoPage(PAGE_MAIN);
}

// Without ADVANCED_PAUSE_FEATURE the job just pauses; the screen then unloads and loads
void RTS::filamentRunout() {
  if (!app_print && !isOngoingPrintJob()) return;
  runout_paused = true;
  gotoPage(TERN(ADVANCED_PAUSE_FEATURE, PAGE_BUSY, PAGE_NO_FILAMENT));
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void RTS::pauseMessage(const PauseMessage message) {
    switch (message) {
      case PAUSE_MESSAGE_PARKING:
      case PAUSE_MESSAGE_CHANGING:
      case PAUSE_MESSAGE_UNLOAD:
      case PAUSE_MESSAGE_LOAD:
      case PAUSE_MESSAGE_PURGE:
      case PAUSE_MESSAGE_RESUME:
      case PAUSE_MESSAGE_HEATING:
        gotoPage(PAGE_BUSY);
        break;
      // "Insert filament" and "reheat": the load key confirms
      case PAUSE_MESSAGE_WAITING:
      case PAUSE_MESSAGE_INSERT:
      case PAUSE_MESSAGE_HEAT:
        gotoPage(PAGE_NO_FILAMENT);
        break;
      // Purge more or resume
      case PAUSE_MESSAGE_OPTION:
        gotoPage(PAGE_LOADED);
        break;
      case PAUSE_MESSAGE_STATUS:
      default:
        if (isPrinting()) showPrintPage();
        break;
    }
  }

#endif

//
// Creality Cloud WiFi board
//

void RTS::appStart() {
  app_print = true;
  app_pause_z = NAN;
  print_state = PS_WARMUP;
  last_percent = 0xFF;
  writeWord(VP_PROGRESS_ICON, 0);
  writeWord(VP_PERCENTAGE, 0);
  writeWord(VP_TIME_HOUR, 0);
  writeWord(VP_TIME_MIN, 0);
  gotoPage(PAGE_HEATING);
}

// The WiFi board stops streaming; end the job here
void RTS::appStop() {
  app_print = false;
  app_pause_z = NAN;
  runout_paused = false;
  stopPrint();
  coolDown();
  print_state = PS_STOPPING;
}

/**
 * M79 from the WiFi board:
 *   S1 cloud job starting   S2 pause   S3 resume   S4 stop   S5 finished   S6 cloud job lost to a power cut
 *   T<percent>  C<seconds elapsed>  D<seconds left>  A<MAC address>  B<1 = WiFi reset done>
 * S2, S3 and S4 also control a print started from the screen.
 */
void RTS::wifiCommand(const char code, const char *arg) {
  const long val = atol(arg);
  switch (code) {
    case 'S': switch (val) {
      case WIFI_START:
        // Also take over a host job whose heat-up started the print timer first
        if (print_state == PS_IDLE || print_state == PS_DONE || (print_state == PS_WARMUP && !app_print && !isPrintingFromMedia()))
          appStart();
        break;

      case WIFI_PAUSE:
        if (app_print) {
          if (print_state != PS_PRINTING) break;
          // Lift the nozzle off the print once the streamed moves are done
          app_pause_z = getAxisPosition_mm(Z);
          setAxisPosition_mm(_MIN(app_pause_z + RTS_APP_PAUSE_LIFT, Z_MAX_POS), Z);
          print_state = PS_PAUSED;
          gotoPage(PAGE_PAUSED);
        }
        else if (print_state == PS_PRINTING)
          handle(VP_KEY_PRINT_PAUSE, 0);
        break;

      case WIFI_RESUME:
        if (app_print) {
          if (print_state != PS_PAUSED || runout_paused) break;
          if (!isnan(app_pause_z)) setAxisPosition_mm(app_pause_z, Z);
          app_pause_z = NAN;
          print_state = PS_WARMUP;
          gotoPage(PAGE_HEATING);
        }
        else if (print_state == PS_PAUSED)
          handle(VP_KEY_PAUSE_RESUME, 0);
        else if (print_state == PS_IDLE || print_state == PS_DONE) {
          if (current_page == PAGE_RECOVERY && !app_recovery)
            handle(VP_KEY_RECOVER_YES, 0);
          else {
            app_recovery = false;
            appStart();
          }
        }
        break;

      case WIFI_STOP:
        if (app_print) appStop();
        else if (WITHIN(print_state, PS_WARMUP, PS_PAUSED))
          handle(VP_KEY_STOP_YES, 0);
        else if (current_page == PAGE_RECOVERY) {
          if (app_recovery) {
            app_recovery = false;
            reportRecoveryPrompt(false);
            gotoPage(PAGE_MAIN);
          }
          else
            handle(VP_KEY_RECOVER_NO, 0);
        }
        break;

      case WIFI_FINISH:
        if (!app_print && WITHIN(print_state, PS_WARMUP, PS_PAUSED)) break;
        print_job_timer.stop();
        printFinished();
        app_print = false;
        app_pause_z = NAN;
        // Lower the bed and bring the head out of the way
        if (isMachineHomed()) {
          MString<48> cmd(F("G90\nG1Z"), int(Z_MAX_POS), 'F', int(motion.homing_feedrate_mm_m.z), F("\nG1X0Y"), int(Y_MAX_POS), F("F3000"));
          injectCommands(cmd);
        }
        break;

      case WIFI_POWER_LOSS:
        app_recovery = true;
        powerLossDetected();
        break;

      default: break;
    } break;

    case 'T':
      if (app_print) {
        const uint8_t pct = constrain(val, 0, 100);
        if (pct != last_percent) {
          last_percent = pct;
          writeWord(VP_PROGRESS_ICON, pct);
          writeWord(VP_PERCENTAGE, pct);
        }
      }
      break;

    case 'C':
      if (app_print && WITHIN(print_state, PS_WARMUP, PS_PRINTING)) {
        const uint32_t s = _MIN(uint32_t(_MAX(val, 0L)), 359999UL);  // 99:59:59
        writeWord(VP_TIME_HOUR, s / 3600);
        writeWord(VP_TIME_MIN, (s % 3600) / 60);
      }
      break;

    case 'A':
      wifi_linked = true;
      strlcpy(wifi_mac, arg, sizeof(wifi_mac));
      if (booted) showTextField(VP_WIFI_MAC, wifi_mac);
      break;

    case 'B':
      if (val && !net_reset_secs) {
        wifi_linked = false;
        if (current_page != PAGE_WIFI_RESET) {
          wifi_return_page = current_page;
          gotoPage(PAGE_WIFI_RESET);
        }
      }
      break;

    default: break;
  }
}

bool RTS::recoveryPrompt() { return recovery_prompt; }

// M72: the cloud job's file name
void RTS::cloudJobName(const char *name) {
  char buf[RTS_TEXT_LEN];
  makeDisplayName(buf, name);
  showTextField(VP_PRINT_FILENAME, buf);
}

/**
 * M79: Creality Cloud print status, from the WiFi board on the host serial port
 *   M79 S<state> | T<percent> | C<seconds> | D<seconds> | A<MAC> | B<result>
 */
void GcodeSuite::M79() {
  const char *p = parser.string_arg;
  if (!p) return;
  while (*p == ' ') ++p;
  const char code = toupper(*p);
  if (!code) return;
  do ++p; while (*p == ' ');
  rts.wifiCommand(code, p);
}

/**
 * M72: Creality Cloud job name, from the WiFi board
 *   M72 <filename>
 */
void GcodeSuite::M72() {
  if (parser.string_arg) rts.cloudJobName(parser.string_arg);
}

//
// Touch events
//

void RTS::handle(const uint16_t vp, const uint16_t value) {
  switch (vp) {
    // Main page
    case VP_KEY_MAIN_FILES:    refreshFileList(); gotoPage(PAGE_FILES); break;
    case VP_KEY_MAIN_MODE:     gotoPage(PAGE_MODE); break;
    case VP_KEY_MAIN_SETTINGS: gotoPage(PAGE_SETTINGS); break;
    case VP_KEY_MAIN_INFO:     gotoPage(PAGE_INFO); break;
    case VP_KEY_RETURN:        onReturn(value); break;

    // Status bar keypads
    case VP_KEY_NOZZLE_TARGET: setTargetTemp_celsius(value, E0); break;
    case VP_KEY_BED_TARGET:    setTargetTemp_celsius(value, BED); break;
    case VP_KEY_FEEDRATE:      setFeedrate_percent(value); break;

    // File list
    case VP_KEY_FILE_SELECT:   selectFile(value); break;
    case VP_KEY_FILE_NEXT:     if (value < 4) gotoPage(PAGE_FILES + value + 1); break;
    case VP_KEY_FILE_PREV:     if (value < 4) gotoPage(PAGE_FILES + value); break;
    case VP_KEY_START_PRINT:   if (!app_print && !isPrinting()) startSelectedFile(); break;

    // Printing
    case VP_KEY_PRINT_ADJUST:  if (print_state != PS_PAUSED) gotoPage(PAGE_ADJUST); break;
    case VP_KEY_PRINT_PAUSE:
      if (app_print)
        notifyWifi(WIFI_PAUSE);   // The WiFi board pauses the job with M79 S2
      else if (print_state == PS_PRINTING) {
        notifyWifi(WIFI_PAUSE);
        gotoPage(PAGE_BUSY);
        pausePrint();
      }
      break;
    case VP_KEY_PRINT_STOP:    if (print_state != PS_PAUSED) gotoPage(PAGE_STOP_CONFIRM); break;
    case VP_KEY_PRINT_DONE:
      print_state = PS_IDLE;
      clearPrintInfo();
      gotoPage(PAGE_MAIN);
      break;
    case VP_KEY_PAUSE_STOP:    gotoPage(PAGE_STOP_CONFIRM); break;
    case VP_KEY_STOP_YES:
    case VP_KEY_RUNOUT_STOP:
    case VP_KEY_LOADED_STOP:
      notifyWifi(WIFI_STOP);
      finishFilament();
      if (app_print) appStop();
      else if (isPrinting()) { gotoPage(PAGE_BUSY); stopPrint(); }
      else gotoPage(PAGE_MAIN);
      break;
    case VP_KEY_STOP_NO:       showPrintPage(); break;
    case VP_KEY_PAUSE_RESUME:
      notifyWifi(WIFI_RESUME);
      // A cloud job paused by M79 S2 resumes with M79 S3
      if (!app_print || awaitingUserConfirm()) resumeJob();
      break;
    case VP_KEY_PAUSE_FILAMENT: gotoPage(PAGE_FILAMENT); break;

    // Enclosure switches
    case VP_KEY_BOX_FAN:       setBoxFan(!boxFanOn()); writeWord(VP_BOX_FAN_ICON, boxFanOn()); break;
    case VP_KEY_BOX_LED:       setBoxLed(!boxLedOn()); writeWord(VP_BOX_LED_ICON, boxLedOn()); break;
    case VP_KEY_WIFI_LED:
      settings.wifi_led = !settings.wifi_led;
      writeWord(VP_WIFI_LED_ICON, settings.wifi_led);
      saveSettings();
      injectCommands(F("M115"));
      break;

    // Print mode: preheat for PLA or ABS
    case VP_KEY_MODE_PLA:
      setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, E0);
      setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
      TERN_(HAS_FAN, setTargetFan_percent(ui8_to_percent(PREHEAT_1_FAN_SPEED), FAN0));
      writeWord(VP_PLA_ICON, 1);
      writeWord(VP_ABS_ICON, 0);
      break;
    case VP_KEY_MODE_ABS:
      setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, E0);
      setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
      TERN_(HAS_FAN, setTargetFan_percent(ui8_to_percent(PREHEAT_2_FAN_SPEED), FAN0));
      writeWord(VP_PLA_ICON, 0);
      writeWord(VP_ABS_ICON, 1);
      break;
    case VP_KEY_MODE_COOL:     gotoPage(PAGE_COOL_CONFIRM); break;
    case VP_KEY_COOL_YES:
      coolDown();
      TERN_(HAS_FAN, setTargetFan_percent(100, FAN0));
      setBoxFan(true);
      writeWord(VP_PLA_ICON, 0);
      writeWord(VP_ABS_ICON, 0);
      gotoPage(PAGE_MODE);
      break;
    case VP_KEY_COOL_NO:       gotoPage(PAGE_MODE); break;

    // Filament
    case VP_KEY_MODE_FILAMENT: gotoPage(PAGE_FILAMENT); break;
    case VP_KEY_LOAD:          writeWord(VP_LOADING, 1); gotoPage(PAGE_LOAD); break;
    case VP_KEY_LOAD_START:    current_page = PAGE_LOADING; startLoad(); break;
    case VP_KEY_LOAD_DONE:
      finishFilament();
      if (print_state == PS_PAUSED)
        gotoPage(filamentMissing() ? PAGE_NO_FILAMENT : PAGE_LOADED);
      else
        gotoPage(PAGE_FILAMENT);
      break;
    case VP_KEY_UNLOAD:        startUnload(); break;
    case VP_KEY_UNLOAD_DONE:
      finishFilament();
      if (print_state == PS_PAUSED) { writeWord(VP_LOADING, 1); gotoPage(PAGE_LOAD); }
      else gotoPage(PAGE_FILAMENT);
      break;
    case VP_KEY_RUNOUT_LOAD:
      if (awaitingUserConfirm())
        setUserConfirmed();       // M600 loads the new filament itself
      else if (print_state == PS_PAUSED)
        startUnload();
      else {
        writeWord(VP_LOADING, 1);
        gotoPage(PAGE_LOAD);
      }
      break;
    case VP_KEY_LOADED_RESUME: resumeJob(); break;

    // Settings
    case VP_KEY_CALIBRATE:     gotoPage(PAGE_CALI_CONFIRM); break;
    case VP_KEY_CALI_YES:      startCalibration(); break;
    case VP_KEY_CALI_NO:       gotoPage(PAGE_SETTINGS); break;
    case VP_KEY_CALI_UP:
    case VP_KEY_CALI_DOWN:
      if (WITHIN(cal_state, CAL_CENTER, CAL_MESH) && !isMoving()) {
        const float dz = vp == VP_KEY_CALI_UP ? -RTS_CALI_STEP : RTS_CALI_STEP;
        cal_offset += dz;
        setAxisPosition_mm(getAxisPosition_mm(Z) + dz, Z);
        writeWord(VP_CALI_ZOFFSET, int16_t(LROUND(cal_offset * 100)));
        writeWord(VP_CALI_ZHEIGHT, int16_t(LROUND(getAxisPosition_mm(Z) * 100)));
      }
      break;
    case VP_KEY_CALI_NEXT:     if (!isMoving()) calibrationNext(); break;

    // Move axes
    case VP_KEY_MOVE:
    case VP_KEY_HOME:
      if (vp == VP_KEY_MOVE && isMachineHomed()) {
        sendPosition();
        gotoPage(PAGE_MOVE + step_index);
        break;
      }
      move_after_home = true;
      injectCommands(F("G28"));
      gotoPage(PAGE_BUSY);
      break;
    case VP_KEY_STEP_10MM:     step_index = 0; gotoPage(PAGE_MOVE); break;
    case VP_KEY_STEP_1MM:      step_index = 1; gotoPage(PAGE_MOVE + 1); break;
    case VP_KEY_STEP_01MM:     step_index = 2; gotoPage(PAGE_MOVE + 2); break;
    case VP_KEY_X_MINUS:       jog(X_AXIS, -1); break;
    case VP_KEY_X_PLUS:        jog(X_AXIS,  1); break;
    case VP_KEY_Y_MINUS:       jog(Y_AXIS, -1); break;
    case VP_KEY_Y_PLUS:        jog(Y_AXIS,  1); break;
    case VP_KEY_Z_MINUS:       jog(Z_AXIS, -1); break;  // Bed up
    case VP_KEY_Z_PLUS:        jog(Z_AXIS,  1); break;
    case VP_AXIS_X:
    case VP_AXIS_Y:
    case VP_AXIS_Z:
      if (isMachineHomed()) {
        setAxisPosition_mm(value / 10.0f, vp == VP_AXIS_X ? X : vp == VP_AXIS_Y ? Y : Z);
        sendPosition();
      }
      break;

    // Information, language, factory reset
    case VP_KEY_INFO_LANGUAGE: gotoPage(PAGE_LANGUAGE); break;
    case VP_KEY_INFO_RESET:    gotoPage(PAGE_RESET_CONFIRM); break;
    case VP_KEY_LANGUAGE:
      settings.language = _MIN(value, RTS_LANGUAGES - 1);
      sendLanguage();
      saveSettings();
      break;
    case VP_KEY_RESET_YES:
      injectCommands(F("M502\nM500"));
      gotoPage(PAGE_INFO);
      break;
    case VP_KEY_RESET_NO:      gotoPage(PAGE_INFO); break;
    case VP_KEY_WIFI_RESET_OK: gotoPage(wifi_return_page); break;

    // Network reset: the WiFi board forgets its network, to pair again from the Creality Cloud app
    case VP_KEY_NET_RESET:     gotoPage(PAGE_NET_CONFIRM); break;
    case VP_KEY_NET_RESET_NO:  gotoPage(PAGE_INFO); break;
    case VP_KEY_NET_RESET_YES:
      SERIAL_ECHOLNPGM("M79 F");
      wifi_linked = false;
      net_reset_secs = RTS_NET_RESET_TIME;
      writeWord(VP_NET_RESET_TIME, net_reset_secs);
      gotoPage(PAGE_NET_RESET);
      break;

    // Power-loss recovery
    case VP_KEY_RECOVER_YES:
      notifyWifi(WIFI_RESUME);
      reportRecoveryPrompt(false);
      if (app_recovery) {         // The WiFi board resends the job
        app_recovery = false;
        appStart();
        break;
      }
      #if ENABLED(POWER_LOSS_RECOVERY)
        print_state = PS_WARMUP;
        gotoPage(PAGE_HEATING);
        injectCommands(F("M1000"));
      #endif
      break;
    case VP_KEY_RECOVER_NO:
      notifyWifi(WIFI_STOP);
      reportRecoveryPrompt(false);
      #if ENABLED(POWER_LOSS_RECOVERY)
        if (!app_recovery) injectCommands(F("M1000C"));
      #endif
      app_recovery = false;
      gotoPage(PAGE_MAIN);
      break;

    default: break;
  }
}

// One return key VP; the value says which page it is on
void RTS::onReturn(const uint16_t value) {
  switch (value) {
    case 0: case 1: case 2: case 3: case 4: // File list
    case 6:                                 // Print mode
    case 0x0B:                              // Settings
    case 0x0E:                              // Information
      gotoPage(PAGE_MAIN);
      break;
    case 5: showPrintPage(); break;         // Adjust
    case 7: showFilamentReturn(); break;    // Filament menu
    case 8:                                 // Unload, while heating
      finishFilament();
      gotoPage(PAGE_FILAMENT);
      break;
    case 0x0A: gotoPage(PAGE_MODE); break;  // User-defined mode
    case 0x0C:                              // Abandon bed calibration
      if (cal_state != CAL_NONE) {
        cal_state = CAL_NONE;
        TERN_(HAS_SOFTWARE_ENDSTOPS, setSoftEndstopState(true));
        TERN_(HAS_LEVELING, setLevelingActive(getLevelingIsValid()));
        setBoxLed(false);
      }
      gotoPage(PAGE_SETTINGS);
      break;
    case 0x0D: gotoPage(PAGE_SETTINGS); break; // Move axes
    case 0x0F: gotoPage(PAGE_INFO); break;     // Language
  }
}

#endif // DGUS_LCD_UI_SERMOON_V1
