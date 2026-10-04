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
 * lcd/extui/elegoo_neptune_3/neptune3_tft.cpp
 *
 * Elegoo Neptune 3 Pro / Plus / Max TJC touch screen
 *
 * The screen sends DGUS-style frames when a control is touched:
 *   5A A5 <len> 83 <addrH> <addrL> <words> <valueH> <valueL> ...
 * and is driven with TJC (Nextion) text commands, each ending in FF FF FF.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ELEGOO_NEPTUNE_3_TFT)

#include "neptune3_tft.h"

#include "../../../MarlinCore.h"
#include "../../../gcode/queue.h"
#include "../../../module/planner.h"
#include "../../../module/temperature.h"
#include "../../../sd/cardreader.h"

#if HAS_LEVELING
  #include "../../../feature/bedlevel/bedlevel.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#include <stdarg.h>

using namespace ExtUI;

Neptune3TFT neptune3;

// The model is identified to the screen by its mesh size, which also names the mesh pages
#if GRID_MAX_POINTS_X == 6 && GRID_MAX_POINTS_Y == 6
  #define N3_MODEL        1   // Neptune 3 Pro
  #define N3_MESH_DATA    "leveldata_36"
  #define N3_MESH_PROBE   "leveling_36"
  #define N3_MESH_PICC    167
#elif GRID_MAX_POINTS_X == 7 && GRID_MAX_POINTS_Y == 7
  #define N3_MODEL        2   // Neptune 3 Plus
  #define N3_MESH_DATA    "aux49_data"
  #define N3_MESH_PROBE   "leveling_49"
  #define N3_MESH_PICC    170
#elif GRID_MAX_POINTS_X == 7 && GRID_MAX_POINTS_Y == 9
  #define N3_MODEL        3   // Neptune 3 Max
  #define N3_MESH_DATA    "aux63_data"
  #define N3_MESH_PROBE   "leveling_63"
  #define N3_MESH_PICC    191
#else
  #error "ELEGOO_NEPTUNE_3_TFT requires a 6x6 (Pro), 7x7 (Plus), or 7x9 (Max) bed leveling grid."
#endif

#define SEND(S)       send(F(S))
#define SENDF(S, V...) sendf(PSTR(S), V)

#define N3_FILE_COUNT     25    // Five pages of five files
#define N3_NAME_LENGTH    40
#define N3_UPDATE_MS    2000
#define N3_BOOT_STEP_MS   30
#define N3_PREVIEW_MS    200
#define N3_PREVIEW_MAX   100    // Records of 1024 bytes
#define N3_LEVEL_INSET  37.5f   // Inset of the manual leveling points

#define COLOR_ON         1024   // Hardware test page colors
#define COLOR_OFF       50712

// Screen control addresses
enum N3Key : uint16_t {
  KEY_MAIN_PAGE       = 0x1002,
  KEY_ADJUSTMENT      = 0x1004,
  KEY_PRINT_SPEED     = 0x1006,
  KEY_STOP_PRINT      = 0x1008,
  KEY_PAUSE_PRINT     = 0x100A,
  KEY_RESUME_PRINT    = 0x100C,
  KEY_Z_OFFSET        = 0x1026,
  KEY_TEMP_SCREEN     = 0x1030,
  KEY_COOL_SCREEN     = 0x1032,
  KEY_HOTEND_TEMP     = 0x1034,
  KEY_BED_TEMP        = 0x103A,
  KEY_SETTINGS        = 0x103E,
  KEY_SETTINGS_BACK   = 0x1040,
  KEY_BED_LEVEL       = 0x1044,
  KEY_AXIS_PAGE       = 0x1046,
  KEY_X_MOVE          = 0x1048,
  KEY_Y_MOVE          = 0x104A,
  KEY_Z_MOVE          = 0x104C,
  KEY_FILAMENT_LENGTH = 0x1054,
  KEY_FILAMENT_LOAD   = 0x1056,
  KEY_FILAMENT_SPEED  = 0x1058,
  KEY_FILAMENT_CHECK  = 0x105E,
  KEY_POWER_CONTINUE  = 0x105F,
  KEY_STORE_MEMORY    = 0x1098,
  KEY_CHANGE_PAGE     = 0x110E,
  KEY_PRINT_FILE      = 0x2198,
  KEY_SELECT_FILE     = 0x2199,
  KEY_PRESET_NOZZLE   = 0x2200,
  KEY_PRESET_BED      = 0x2201,
  KEY_HARDWARE_TEST   = 0x2202
};

// An operation whose completion switches the screen to another page
enum N3Wait : uint8_t { WAIT_NONE, WAIT_PAUSE, WAIT_STOP, WAIT_HOME_MOVE, WAIT_HOME_LEVEL, WAIT_LEVEL };

enum N3Speed : uint8_t { SPEED_FEEDRATE = 1, SPEED_FLOW, SPEED_FAN };
enum N3Limit : uint8_t { LIMIT_NONE, LIMIT_FEEDRATE, LIMIT_ACCEL };

// Material preheat presets (PLA, ABS, PETG, TPU), saved with M500
typedef struct { celsius_t hotend, bed; } n3_preset_t;
typedef struct {
  n3_preset_t material[4];
} n3_settings_t;

static n3_settings_t n3_settings;

static uint8_t rx_state, rx_len, rx_count, rx_buf[26];
static millis_t rx_ms;

static N3Wait wait_for = WAIT_NONE;
static millis_t wait_ms;
static bool auto_confirm, auto_resume;  // Answer a prompt once Marlin is waiting for it
static uint8_t boot_step;   // 0 = not started, 1-101 = animating, 102 = done
static millis_t next_boot_ms, next_update_ms;
static bool homing, leveling, print_started, heat_wait;
TERN_(POWER_LOSS_RECOVERY, static bool plr_pending);

static uint8_t unit = 10, temp_select, speed_select = SPEED_FEEDRATE, limit_select;
static bool temp_is_bed;
static float axis_unit = 1.0f, zoffset_unit = 0.1f;
static int16_t filament_length = 10, filament_speed = manual_feedrate_mm_m.e;  // (mm/min)
static bool status_led1, status_led2;

static char file_short[N3_FILE_COUNT][FILENAME_LENGTH], file_name[N3_FILE_COUNT][N3_NAME_LENGTH],
            printed_short[FILENAME_LENGTH];
static uint8_t file_count;
static int8_t file_selected = -1, file_printed = -1;

static struct {
  MediaFile file;
  bool active;
  uint8_t records;
  millis_t next_ms;
  char buf[1024];
} preview;

//
// Screen output
//

void Neptune3TFT::sendEnd() { for (uint8_t i = 0; i < 3; ++i) LCD_SERIAL.write(0xFF); }

void Neptune3TFT::sendRaw(FSTR_P const fstr) {
  PGM_P str = FTOP(fstr);
  while (const char c = pgm_read_byte(str++)) LCD_SERIAL.write(c);
}

void Neptune3TFT::send(FSTR_P const fstr) { sendRaw(fstr); sendEnd(); }

void Neptune3TFT::sendf(PGM_P const fmt, ...) {
  char buf[96];
  va_list args;
  va_start(args, fmt);
  vsnprintf_P(buf, sizeof(buf), fmt, args);
  va_end(args);
  LCD_SERIAL.print(buf);
  sendEnd();
}

void Neptune3TFT::sendModel() { SENDF("main.va0.val=%d", N3_MODEL); }

void Neptune3TFT::sendZOffset(FSTR_P const obj) {
  sendRaw(obj);
  sendRaw(F(".z_offset.val="));
  LCD_SERIAL.print(int(LROUND(getZOffset_mm() * 100)));
  sendEnd();
}

// Index of a mesh point in probing order, which is how the screen numbers its fields
static uint8_t meshIndex(const int8_t x, const int8_t y) {
  const bool ascending = ((GRID_MAX_POINTS_Y) & 1) ^ (y & 1);
  return y * (GRID_MAX_POINTS_X) + (ascending ? x : (GRID_MAX_POINTS_X) - 1 - x);
}

void Neptune3TFT::sendMesh() {
  #if HAS_MESH
    for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; ++y) for (uint8_t x = 0; x < GRID_MAX_POINTS_X; ++x) {
      const xy_uint8_t pos = { x, y };
      const float z = getMeshPoint(pos);
      SENDF(N3_MESH_DATA ".x%d.val=%d", meshIndex(x, y), isnan(z) ? 0 : int(LROUND(z * 100)));
    }
  #endif
}

void Neptune3TFT::sendPreTemps(const bool nozzle, const bool bed) {
  if (nozzle) SENDF("pretemp.nozzletemp.txt=\"%d / %d\"", thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
  if (bed) SENDF("pretemp.bedtemp.txt=\"%d / %d\"", thermalManager.wholeDegBed(), thermalManager.degTargetBed());
}

void Neptune3TFT::sendPrintInfo() {
  SENDF("printpause.printspeed.txt=\"%d\"", int(getFeedrate_percent()));
  SENDF("printpause.fanspeed.txt=\"%d\"", fans[0].speed);
  SENDF("printpause.zvalue.val=%d", int(getAxisPosition_mm(Z) * 10));
  const uint32_t elapsed = getProgress_seconds_elapsed();
  SENDF("printpause.printtime.txt=\"%d h %d min\"", int(elapsed / 3600), int((elapsed % 3600) / 60));
  const uint8_t progress = getProgress_percent();
  SENDF("printpause.printprocess.val=%d", progress);
  SENDF("printpause.printvalue.txt=\"%d\"", progress);
}

void Neptune3TFT::sendSpeedValue() {
  const int value = speed_select == SPEED_FLOW ? getFlow_percent(E0)
                  : speed_select == SPEED_FAN  ? fans[0].speed
                  : int(getFeedrate_percent());
  SENDF("adjustspeed.targetspeed.val=%d", value);
}

void Neptune3TFT::sendAdvancedValues() {
  if (limit_select == LIMIT_NONE) return;
  const bool fr = limit_select == LIMIT_FEEDRATE;
  #define _LIMIT_VALUE(A) int(fr ? getAxisMaxFeedrate_mm_s(A) : getAxisMaxAcceleration_mm_s2(A))
  SENDF("speedsetvalue.xaxis.val=%d", _LIMIT_VALUE(X));
  SENDF("speedsetvalue.yaxis.val=%d", _LIMIT_VALUE(Y));
  SENDF("speedsetvalue.zaxis.val=%d", _LIMIT_VALUE(Z));
  SENDF("speedsetvalue.eaxis.val=%d", _LIMIT_VALUE(E0));
}

// The preset being edited: a material, or Marlin's leveling temperatures
static n3_preset_t getPreset() {
  if (temp_select < 4) return n3_settings.material[temp_select];
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    return { getLevelingNozzleTemp(), getLevelingBedTemp() };
  #else
    return { 0, 0 };
  #endif
}

static void setPreset(const n3_preset_t &p) {
  if (temp_select < 4)
    n3_settings.material[temp_select] = p;
  #if HAS_LEVELING_TEMP_EDIT
    else {
      setLevelingNozzleTemp(p.hotend);
      setLevelingBedTemp(p.bed);
    }
  #endif
}

void Neptune3TFT::sendMaterialValues() {
  const n3_preset_t p = getPreset();
  SENDF("tempsetvalue.nozzletemp.val=%d", p.hotend);
  SENDF("tempsetvalue.bedtemp.val=%d", p.bed);
}

//
// Screen input
//

void Neptune3TFT::readData() {
  // Drop a partial frame that has stalled
  if (rx_state && ELAPSED(millis(), rx_ms + 100)) rx_state = 0;

  while (LCD_SERIAL.available()) {
    const uint8_t c = LCD_SERIAL.read();
    rx_ms = millis();
    switch (rx_state) {
      case 0: if (c == 0x5A) rx_state = 1; break;
      case 1: rx_state = c == 0xA5 ? 2 : c == 0x5A ? 1 : 0; break;
      case 2:
        rx_len = c;
        rx_count = 0;
        rx_state = WITHIN(rx_len, 3, sizeof(rx_buf)) ? 3 : 0;
        break;
      case 3:
        rx_buf[rx_count++] = c;
        if (rx_count == rx_len) { rx_state = 0; processFrame(); }
        break;
    }
  }
}

void Neptune3TFT::processFrame() {
  // Only variable reads carry a touch event; 82 4F 4B acknowledges a write
  if (rx_buf[0] != 0x83 || rx_len < 6) return;
  // Ignore the screen while an operation it started is still running
  if (wait_for != WAIT_NONE) return;
  handleKey((rx_buf[1] << 8) | rx_buf[2], (rx_buf[4] << 8) | rx_buf[5]);
}

//
// Lifecycle
//

void Neptune3TFT::startup() {
  #ifndef LCD_BAUDRATE
    #define LCD_BAUDRATE 115200
  #endif
  LCD_SERIAL.begin(LCD_BAUDRATE);

  SEND("page boot");
  sendModel();
  SENDF("information.sversion.txt=\"%s\"", SHORT_BUILD_VERSION);
  sendMesh();
  sendZOffset(F("leveldata"));

  #if HAS_MESH
    if (getLevelingIsValid()) setLevelingActive(true);
  #endif

  boot_step = 1;
  next_boot_ms = millis();
}

void Neptune3TFT::idleLoop() {
  readData();

  // Prompts are raised before Marlin starts waiting, so answer them from here
  if (auto_confirm && awaitingUserConfirm()) { auto_confirm = false; setUserConfirmed(); }
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    if (auto_resume && pause_menu_response == PAUSE_RESPONSE_WAIT_FOR) {
      auto_resume = false;
      setPauseMenuResponse(PAUSE_RESPONSE_RESUME_PRINT);
    }
  #endif

  const millis_t ms = millis();

  // Boot animation, then show the main page or offer to resume
  if (WITHIN(boot_step, 1, 101)) {
    if (ELAPSED(ms, next_boot_ms)) {
      SENDF("boot.j0.val=%d", boot_step - 1);
      next_boot_ms = ms + N3_BOOT_STEP_MS;
      if (++boot_step > 101) {
        #if ENABLED(POWER_LOSS_RECOVERY)
          if (plr_pending) powerLossResume(); else
        #endif
        SEND("page main");
        next_update_ms = ms;
      }
    }
    return;
  }

  previewStep();
  checkWaitState();

  if (ELAPSED(ms, next_update_ms)) {
    next_update_ms = ms + N3_UPDATE_MS;
    periodicUpdate();
  }
}

void Neptune3TFT::periodicUpdate() {
  #if HAS_FILAMENT_SENSOR
    SENDF("set.va1.val=%d", getFilamentRunoutEnabled());
  #endif
  SENDF("set.va0.val=%d", fans[0].speed ? 1 : 0);
  #if ENABLED(POWER_LOSS_RECOVERY)
    SENDF("multiset.plrbutton.val=%d", getPowerLossRecoveryEnabled());
  #endif
  SENDF("main.nozzletemp.txt=\"%d / %d\"", thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
  SENDF("main.bedtemp.txt=\"%d / %d\"", thermalManager.wholeDegBed(), thermalManager.degTargetBed());

  // Leave the "heating" page once the nozzle is up to temperature
  if (heat_wait && thermalManager.wholeDegHotend(0) >= thermalManager.degTargetHotend(0) - 5) {
    heat_wait = false;
    if (isPrintingPaused()) SEND("page adjusttemp"); else SEND("page prefilament");
  }
}

void Neptune3TFT::checkWaitState() {
  const bool busy = planner.has_blocks_queued() || queue.has_commands_queued();

  // Don't leave the screen locked if a homing or leveling command ends without completing
  if (wait_for != WAIT_NONE && (busy || homing || leveling)) wait_ms = millis();

  switch (wait_for) {
    case WAIT_PAUSE:
      if (!isPrinting()) wait_for = WAIT_NONE; // Nothing to pause
      else if (isPrintingPaused() && !busy) {
        wait_for = WAIT_NONE;
        SEND("page printpause");
      }
      break;
    case WAIT_STOP:
      if (!isPrinting() && !busy) {
        wait_for = WAIT_NONE;
        SEND("page main");
        injectCommands(F("M84"));
      }
      break;
    case WAIT_NONE: break;
    default:
      if (ELAPSED(millis(), wait_ms + 3000)) wait_for = WAIT_NONE;
      break;
  }

  // Once the print has heated up and started moving, enable its controls
  if (!print_started && isPrintingFromMedia() && !isPrintingFromMediaPaused()
      && !marlin.is_heating() && planner.has_blocks_queued()
  ) {
    print_started = true;
    SEND("restFlag1=0");
    SEND("restFlag2=1");
    SEND("page printpause");
  }
}

//
// Files
//

static bool isGcode(const char * const name) {
  const char * const dot = strrchr(name, '.');
  return dot && (!strcasecmp_P(dot, PSTR(".gcode")) || !strcasecmp_P(dot, PSTR(".gco")));
}

// Name without extension, shortened with ".." to fit the screen
static void displayName(char * const dst, const char * const src) {
  const char * const dot = strrchr(src, '.');
  size_t len = dot ? size_t(dot - src) : strlen(src);
  const bool shorten = len > N3_NAME_LENGTH - 1;
  if (shorten) len = N3_NAME_LENGTH - 3;
  for (size_t i = 0; i < len; ++i) dst[i] = src[i] == '"' ? '\'' : src[i];
  if (shorten) { dst[len++] = '.'; dst[len++] = '.'; }
  dst[len] = '\0';
}

void Neptune3TFT::clearFileList() {
  file_count = 0;
  file_selected = file_printed = -1;
  for (uint8_t i = 0; i < N3_FILE_COUNT; ++i) SENDF("file%d.t%d.txt=\"\"", i / 5 + 1, i);
}

void Neptune3TFT::refreshFileList() {
  clearFileList();
  if (!card.isMounted()) return;

  card.cdroot();
  const int16_t count = card.get_num_items();
  for (int16_t i = count - 1; i >= 0 && file_count < N3_FILE_COUNT; --i) {
    card.selectFileByIndex(i);
    if (card.flag.filenameIsDir || !isGcode(card.longest_filename())) continue;
    strcpy(file_short[file_count], card.filename);
    displayName(file_name[file_count], card.longest_filename());
    SENDF("file%d.t%d.txt=\"%s\"", file_count / 5 + 1, file_count, file_name[file_count]);
    if (!strcmp(file_short[file_count], printed_short)) file_printed = file_count; // For "Print again"
    ++file_count;
  }
}

void Neptune3TFT::mediaMounted() { refreshFileList(); }
void Neptune3TFT::mediaRemoved() { clearFileList(); }
void Neptune3TFT::mediaError() { SEND("page err_sdread"); }

void Neptune3TFT::startPrint(const int8_t index) {
  if (!WITHIN(index, 0, file_count - 1)) return;

  if (thermalManager.wholeDegHotend(0) < 0) { SEND("page err_nozzleunde"); return; }
  if (thermalManager.wholeDegBed() < 0) { SEND("page err_bedunder"); return; }

  SENDF("file%d.t%d.pco=65504", index / 5 + 1, index);

  if (!filamentPresent()) { SEND("page nofilament"); return; }

  file_printed = index;
  strcpy(printed_short, file_short[index]);
  print_started = false;
  setFeedrate_percent(100);

  SENDF("printpause.t0.txt=\"%s\"", file_name[index]);
  SEND("printpause.printvalue.txt=\"0\"");
  SEND("printpause.printprocess.val=0");
  sendZOffset(F("leveldata"));
  SEND("page printpause");
  SEND("restFlag2=0");

  startPreview(index);
  printFile(file_short[index]);
}

void Neptune3TFT::stopPrint() {
  wait_for = WAIT_STOP; wait_ms = millis();
  preview.active = false;
  preview.file.close();
  clearPreview();
  ExtUI::stopPrint();
}

//
// Print preview, from the ";simage:" thumbnail records at the start of the file
//

void Neptune3TFT::clearPreview() {
  SEND("printpause.cp0.close()");
  SEND("printpause.cp0.aph=0");
  SEND("printpause.va0.txt=\"\"");
  SEND("printpause.va1.txt=\"\"");
}

void Neptune3TFT::startPreview(const int8_t index) {
  clearPreview();
  preview.file.close();
  MediaFile root = card.getroot();
  preview.active = preview.file.open(&root, file_short[index], O_READ);
  preview.records = 0;
  preview.next_ms = millis();
}

void Neptune3TFT::previewStep() {
  if (!preview.active || PENDING(millis(), preview.next_ms)) return;

  const int16_t n = preview.file.read(preview.buf, sizeof(preview.buf));
  const bool last = n > 9 && !strncmp_P(preview.buf, PSTR(";;simage:"), 9),
             more = !last && n > 8 && !strncmp_P(preview.buf, PSTR(";simage:"), 8);

  if (last || more) {
    const uint8_t skip = last ? 9 : 8;
    int16_t len = n - skip;
    while (len && (preview.buf[skip + len - 1] == '\n' || preview.buf[skip + len - 1] == '\r')) --len;
    sendRaw(F("printpause.va0.txt=\""));
    LCD_SERIAL.write((const uint8_t *)preview.buf + skip, len);
    LCD_SERIAL.write('"');
    sendEnd();
    SEND("printpause.va1.txt+=printpause.va0.txt");
    ++preview.records;
  }

  if (more && preview.records < N3_PREVIEW_MAX) {
    preview.next_ms = millis() + N3_PREVIEW_MS;
    return;
  }

  // Done: show the image, or close the preview if there was none
  preview.active = false;
  preview.file.close();
  if (preview.records) {
    SEND("printpause.cp0.aph=127");
    SEND("printpause.cp0.write(printpause.va1.txt)");
  }
  else {
    SEND("printpause.cp0.aph=0");
    SEND("printpause.cp0.close()");
  }
}

//
// Helpers
//

bool Neptune3TFT::filamentPresent() {
  #if HAS_FILAMENT_SENSOR
    if (getFilamentRunoutEnabled()) return READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE;
  #endif
  return true;
}

void Neptune3TFT::toggleCaseLight() {
  status_led2 = !status_led2;
  SENDF("status_led2=%d", status_led2);
  TERN_(CASE_LIGHT_ENABLE, setCaseLightState(status_led2));
}

void Neptune3TFT::adjustZOffset(const float delta) {
  const float zoffs = getZOffset_mm(), newz = zoffs + delta;
  if (!WITHIN(newz, PROBE_OFFSET_ZMIN, PROBE_OFFSET_ZMAX)) return;
  TERN_(BABYSTEPPING, babystepAxis_steps(mmToWholeSteps(newz - zoffs, Z), Z));
  setZOffset_mm(newz);
}

void Neptune3TFT::moveAxis(const axis_t axis, const bool positive) {
  static constexpr float lo[] = { X_MIN_POS, Y_MIN_POS, Z_MIN_POS },
                         hi[] = { X_MAX_POS, Y_MAX_POS, Z_MAX_POS };
  const float pos = getAxisPosition_mm(axis) + (positive ? axis_unit : -axis_unit);
  setAxisPosition_mm(constrain(pos, lo[axis], hi[axis]), axis);
}

void Neptune3TFT::moveExtruder(const float distance) {
  setAxisPosition_mm(getAxisPosition_mm(E0) + distance, E0, MMM_TO_MMS(filament_speed));
}

//
// Touch events
//

void Neptune3TFT::handleKey(const uint16_t addr, const uint16_t value) {
  // Values typed on the screen's keypad arrive byte-swapped
  const uint16_t swapped = (value >> 8) | (value << 8);

  switch (addr) {

    case KEY_MAIN_PAGE:
      switch (value) {
        case 1:
          refreshFileList();
          if (card.isMounted()) SEND("page file1"); else SEND("page nosdcard");
          break;
        case 2: stopPrint(); break;
        case 6: refreshFileList(); break;
      }
      break;

    case KEY_ADJUSTMENT:
      switch (value) {
        case 1:
          temp_is_bed = false;
          unit = 10;
          SENDF("adjusttemp.targettemp.val=%d", getTargetTemp_celsius(H0));
          SEND("adjusttemp.va0.val=1");
          SEND("adjusttemp.va1.val=3");
          break;
        case 2: SEND("page printpause"); break;
        case 3: thermalManager.set_fan_speed(0, fans[0].speed ? 0 : 255); break;
        case 4: toggleCaseLight(); break;
        case 5: unit = 10; SEND("page adjusttemp"); break;
        case 6:
          unit = 10;
          speed_select = SPEED_FEEDRATE;
          sendSpeedValue();
          SEND("page adjustspeed");
          break;
        case 7:
          zoffset_unit = 0.1f;
          SEND("adjustzoffset.zoffset_value.val=2");
          sendZOffset(F("adjustzoffset"));
          SEND("page adjustzoffset");
          break;
        case 8: setFeedrate_percent(100); speed_select = SPEED_FEEDRATE; sendSpeedValue(); break;
        case 9: setFlow_percent(100, E0); speed_select = SPEED_FLOW; sendSpeedValue(); break;
        case 10: thermalManager.set_fan_speed(0, 255); speed_select = SPEED_FAN; sendSpeedValue(); break;
      }
      break;

    case KEY_PRINT_SPEED: setFeedrate_percent(value); break;

    case KEY_STOP_PRINT:
      if (value == 1 || value == 0xF1) {
        if (!homing) { SEND("page wait"); stopPrint(); }
      }
      else if (value == 0xF0)
        SEND("page printpause");
      break;

    case KEY_PAUSE_PRINT:
      if (value == 0xF1) {
        SEND("page wait");
        wait_for = WAIT_PAUSE; wait_ms = millis();
        pausePrint();
      }
      else if (value == 1 && isPrintingFromMedia())
        SEND("page pauseconfirm");
      break;

    case KEY_RESUME_PRINT:
      switch (value) {
        case 2: if (!filamentPresent()) break; // fall through
        case 1:
          if (awaitingUserConfirm()) { setUserConfirmed(); break; }
          SEND("page wait");
          if (isPrintingPaused()) resumePrint();
          SEND("page printpause");
          break;
        case 3:
          if (isPrinting())
            SEND("page filamentresume");
          else
            startPrint(file_printed); // Print the last file again
          break;
        case 4: if (card.isMounted()) refreshFileList(); break;
      }
      break;

    case KEY_Z_OFFSET: adjustZOffset(int16_t(value) / 100.0f - getZOffset_mm()); break;

    case KEY_TEMP_SCREEN: {
      const heater_t heater = temp_is_bed ? BED : H0;
      switch (value) {
        case 1: temp_is_bed = false; SENDF("adjusttemp.targettemp.val=%d", getTargetTemp_celsius(H0)); break;
        case 3: temp_is_bed = true; SENDF("adjusttemp.targettemp.val=%d", getTargetTemp_celsius(BED)); break;
        case 5: unit = 1; axis_unit = 0.1f; break;
        case 6: unit = 5; axis_unit = 1.0f; break;
        case 7: unit = 10; axis_unit = 10.0f; break;
        case 8: case 9: {
          const int16_t max = temp_is_bed ? BED_MAX_TARGET : thermalManager.hotend_max_target(0),
                        t = getTargetTemp_celsius(heater) + (value == 8 ? unit : -unit);
          setTargetTemp_celsius(constrain(t, 0, max), heater);
          SENDF("adjusttemp.targettemp.val=%d", getTargetTemp_celsius(heater));
        } break;
        case 10: speed_select = SPEED_FEEDRATE; sendSpeedValue(); break;
        case 11: speed_select = SPEED_FLOW; sendSpeedValue(); break;
        case 12: speed_select = SPEED_FAN; sendSpeedValue(); break;
        case 13: case 14: {
          const int16_t d = value == 13 ? unit : -unit;
          switch (speed_select) {
            case SPEED_FEEDRATE: setFeedrate_percent(constrain(getFeedrate_percent() + d, 10, 300)); break;
            case SPEED_FLOW: setFlow_percent(constrain(getFlow_percent(E0) + d, 100, 300), E0); break;
            case SPEED_FAN: thermalManager.set_fan_speed(0, constrain(fans[0].speed + d, 0, 255)); break;
          }
          sendSpeedValue();
        } break;
        case 15: limit_select = LIMIT_FEEDRATE; unit = 10; break;
        case 16: limit_select = LIMIT_ACCEL; unit = 10; break;
        case 17 ... 24: {
          // 17-20 decrease and 21-24 increase the X, Y, Z, E limits
          const bool up = value >= 21;
          const uint8_t a = (value - 17) % 4;
          const axis_t axis = a == 0 ? X : a == 1 ? Y : Z;
          if (limit_select == LIMIT_FEEDRATE) {
            static constexpr float lo[] = { 100, 100, 5, 10 }, hi[] = { 300, 300, 15, 25 };
            const float v = constrain((a == 3 ? getAxisMaxFeedrate_mm_s(E0) : getAxisMaxFeedrate_mm_s(axis)) + (up ? unit : -unit), lo[a], hi[a]);
            if (a == 3) setAxisMaxFeedrate_mm_s(v, E0); else setAxisMaxFeedrate_mm_s(v, axis);
          }
          else if (limit_select == LIMIT_ACCEL) {
            static constexpr float lo[] = { 100, 100, 50, 100 }, hi[] = { 3000, 3000, 150, 2000 };
            const float v = constrain((a == 3 ? getAxisMaxAcceleration_mm_s2(E0) : getAxisMaxAcceleration_mm_s2(axis)) + (up ? unit : -unit) * 10, lo[a], hi[a]);
            if (a == 3) setAxisMaxAcceleration_mm_s2(v, E0); else setAxisMaxAcceleration_mm_s2(v, axis);
          }
        } break;
        case 0xF1:
          thermalManager.set_fan_speed(0, 255);
          thermalManager.disable_all_heaters();
          break;
      }
      sendAdvancedValues();
    } break;

    case KEY_COOL_SCREEN:
      switch (value) {
        case 1: setTargetTemp_celsius(0, H0); sendPreTemps(true, true); break;
        case 2: setTargetTemp_celsius(0, BED); sendPreTemps(true, true); break;
        case 5: case 6: {
          const n3_preset_t &p = n3_settings.material[value - 5];
          setTargetTemp_celsius(p.hotend, H0);
          setTargetTemp_celsius(p.bed, BED);
        } break;
        case 9 ... 12: {
          const n3_preset_t &p = n3_settings.material[value - 9];
          setTargetTemp_celsius(p.hotend, H0);
          setTargetTemp_celsius(p.bed, BED);
          sendPreTemps(true, true);
          SENDF("pretemp.nozzle.txt=\"%d\"", thermalManager.degTargetHotend(0));
          SENDF("pretemp.bed.txt=\"%d\"", thermalManager.degTargetBed());
        } break;
        case 13 ... 17:
          temp_select = value - 13; // Materials, then the leveling temperatures
          unit = 10;
          sendMaterialValues();
          SEND("page tempsetvalue");
          break;
      }
      break;

    case KEY_HOTEND_TEMP: setTargetTemp_celsius(swapped, H0); sendPreTemps(true, false); break;
    case KEY_BED_TEMP: setTargetTemp_celsius(swapped, BED); sendPreTemps(false, true); break;

    case KEY_PRESET_NOZZLE:
    case KEY_PRESET_BED: {
      if (!WITHIN(value, 1, 2)) break;
      n3_preset_t p = getPreset();
      const int16_t d = value == 1 ? unit : -unit;
      if (addr == KEY_PRESET_NOZZLE)
        p.hotend = constrain(p.hotend + d, temp_select < 4 ? 160 : 140, _MIN(280, thermalManager.hotend_max_target(0)));
      else
        p.bed = constrain(p.bed + d, 50, _MIN(110, BED_MAX_TARGET));
      setPreset(p);
      sendMaterialValues();
    } break;

    case KEY_FILAMENT_LENGTH:
      filament_length = swapped;
      SENDF("prefilament.filamentlength.txt=\"%d\"", filament_length);
      break;

    case KEY_FILAMENT_SPEED:
      filament_speed = swapped;
      SENDF("prefilament.filamentspeed.txt=\"%d\"", filament_speed);
      break;

    case KEY_AXIS_PAGE:
      switch (value) {
        case 1: axis_unit = 0.1f; break;
        case 2: axis_unit = 1.0f; break;
        case 3: axis_unit = 10.0f; break;
        case 4:
          wait_for = WAIT_HOME_MOVE; wait_ms = millis();
          injectCommands(F("G28"));
          SEND("page autohome");
          break;
        case 5: injectCommands(F("G28X")); break;
        case 6: injectCommands(F("G28Y")); break;
        case 7: injectCommands(F("G28Z")); break;
      }
      break;

    case KEY_SETTINGS:
      switch (value) {
        case 1:
          wait_for = WAIT_HOME_LEVEL; wait_ms = millis();
          injectCommands(F("G28\nG1F200Z0"));
          SEND("page autohome");
          SENDF("leveling.va1.val=%d", N3_MODEL);
          break;
        case 2: filament_length = 10; SEND("page autohome"); break;
        case 3:
          axis_unit = 1.0f;
          SEND("page premove");
          SEND("premove.unit_move.val=2");
          break;
        case 6: injectCommands(F("M84")); break;
        case 7:
          thermalManager.set_fan_speed(0, fans[0].speed ? 0 : 255);
          SENDF("set.va0.val=%d", fans[0].speed ? 1 : 0);
          break;
        case 8:
          #if HAS_FILAMENT_SENSOR
            setFilamentRunoutEnabled(!getFilamentRunoutEnabled());
            SENDF("set.va1.val=%d", getFilamentRunoutEnabled());
          #endif
          break;
        case 9:
          SEND("page pretemp");
          if (thermalManager.wholeDegHotend(0) < 0) SEND("page err_nozzleunde");
          else if (thermalManager.wholeDegBed() < 0) SEND("page err_bedunder");
          break;
        case 10:
          SEND("page prefilament");
          SENDF("prefilament.filamentlength.txt=\"%d\"", filament_length);
          SENDF("prefilament.filamentspeed.txt=\"%d\"", filament_speed);
          break;
        case 11: SEND("page set"); break;
        case 12: SEND("page warn_rdlevel"); break;
        case 13:
          #if ENABLED(POWER_LOSS_RECOVERY)
            SENDF("multiset.plrbutton.val=%d", getPowerLossRecoveryEnabled());
            SEND("page multiset");
          #endif
          break;
      }
      break;

    case KEY_SETTINGS_BACK:
      switch (value) {
        case 1: injectCommands(F("M500\nG1F1000Z15")); break;
        case 2: if (!isMoving()) injectCommands(F("M420S1")); break;
        case 4: case 5: injectCommands(F("M500")); break;
        case 6: temp_select = 0; injectCommands(F("M500")); break;
      }
      break;

    case KEY_BED_LEVEL:
      switch (value) {
        case 1:
          wait_for = WAIT_HOME_LEVEL; wait_ms = millis();
          injectCommands(F("G28Z\nG1F200Z0"));
          break;
        case 2: case 3:
          adjustZOffset(value == 2 ? zoffset_unit : -zoffset_unit);
          sendZOffset(F("leveldata"));
          sendZOffset(F("adjustzoffset"));
          break;
        case 4: zoffset_unit = 0.01f; SEND("adjustzoffset.zoffset_value.val=1"); break;
        case 5: zoffset_unit = 0.1f;  SEND("adjustzoffset.zoffset_value.val=2"); break;
        case 6: zoffset_unit = 1.0f;  SEND("adjustzoffset.zoffset_value.val=3"); break;
        case 7: status_led1 = !status_led1; SENDF("status_led1=%d", status_led1); break;
        case 8: toggleCaseLight(); break;
        case 9:
          wait_for = WAIT_LEVEL; wait_ms = millis();
          injectCommands(F("G28\nG29"));
          break;
        case 10: sendPrintInfo(); break;
        case 11:
          SENDF("main.nozzletemp.txt=\"%d / %d\"", thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
          SENDF("main.bedtemp.txt=\"%d / %d\"", thermalManager.wholeDegBed(), thermalManager.degTargetBed());
          break;
        case 12: // The screen has restarted
          if (boot_step > 101) {
            SEND("tm0.en=0");
            SEND("va0.val=0");
            SEND("tm1.en=1");
            sendModel();
          }
          break;
        case 13 ... 19: {
          // Manual leveling: the center, then the left column front to back, then the right column back to front
          if (isMoving()) break;
          if (value == 13) { injectCommands(F("G28Z\nG1F200Z0")); break; }
          const uint8_t p = value - 14;
          const float x = p < 3 ? N3_LEVEL_INSET : X_BED_SIZE - (N3_LEVEL_INSET),
                      ys[] = { N3_LEVEL_INSET, Y_BED_SIZE / 2.0f, Y_BED_SIZE - (N3_LEVEL_INSET) },
                      y = ys[p < 3 ? p : 5 - p];
          char cmd[48];
          sprintf_P(cmd, PSTR("G1F600Z3\nG1X%i.%iY%i.%iF8000\nG1F200Z0"),
            int(x), int(x * 10) % 10, int(y), int(y * 10) % 10);
          injectCommands(cmd);
        } break;
        case 20: TERN_(HAS_LEVELING, reset_bed_level()); break;
        case 21: injectCommands(F("M500")); break;
        case 22: // The print page has been reopened
          sendModel();
          if (WITHIN(file_printed, 0, file_count - 1))
            SENDF("printpause.t0.txt=\"%s\"", file_name[file_printed]);
          SENDF("printpause.printprocess.val=%d", getProgress_percent());
          SENDF("printpause.printvalue.txt=\"%d\"", getProgress_percent());
          break;
      }
      break;

    case KEY_X_MOVE: moveAxis(X, value == 1); break;
    case KEY_Y_MOVE: moveAxis(Y, value == 1); break;
    case KEY_Z_MOVE: moveAxis(Z, value == 1); break;

    case KEY_FILAMENT_LOAD:
      switch (value) {
        case 1: case 2:
          if (isPrinting())
            SEND("page warn1_filament");
          else if (!isMoving()) {
            if (thermalManager.tooColdToExtrude(0))
              SEND("page warn2_filament");
            else
              moveExtruder(value == 1 ? -filament_length : filament_length);
          }
          break;
        case 5:
          if (isMoving()) break;
          setTargetTemp_celsius(n3_settings.material[0].hotend, H0);
          SEND("page heatfilament");
          heat_wait = true;
          break;
        case 6:
          if (isMoving()) break;
          filament_length = 10;
          SEND("page prefilament");
          heat_wait = true;
          break;
        case 8: filament_length = 10; break;
        case 9:
          SEND("page wait");
          wait_for = WAIT_PAUSE; wait_ms = millis();
          pausePrint();
          break;
        case 10: if (!isMoving()) SEND("page main"); break;
        case 11:
          unit = 10;
          SENDF("motorsetvalue.motorvalue.val=%d", int(getAxisSteps_per_mm(E0)));
          break;
        case 12: case 13:
          setAxisSteps_per_mm(_MAX(getAxisSteps_per_mm(E0) + (value == 12 ? unit : -unit), 1), E0);
          SENDF("motorsetvalue.motorvalue.val=%d", int(getAxisSteps_per_mm(E0)));
          break;
        case 14: moveExtruder(-filament_length); break;
        case 15: moveExtruder(filament_length); break;
        case 16: if (isPrintingPaused()) planner.quick_stop(); break;
        case 0xF1: setTargetTemp_celsius(0, H0); filament_length = 10; break;
      }
      break;

    case KEY_FILAMENT_CHECK:
      if (value == 1 && !filamentPresent()) SEND("page nofilament");
      else if (value == 2) filament_length = 10;
      break;

    case KEY_POWER_CONTINUE:
      #if ENABLED(POWER_LOSS_RECOVERY)
        switch (value) {
          case 1:
            plr_pending = false;
            print_started = false;
            SEND("restFlag2=0");
            SEND("page printpause");
            sendZOffset(F("leveldata"));
            injectCommands(F("M1000"));
            break;
          case 2:
            plr_pending = false;
            SEND("page main");
            injectCommands(F("M1000C"));
            break;
          case 3: setPowerLossRecoveryEnabled(!getPowerLossRecoveryEnabled()); break;
        }
      #endif
      break;

    case KEY_SELECT_FILE:
      if (!WITHIN(value, 1, file_count)) break;
      file_selected = value - 1;
      SENDF("askprint.t0.txt=\"%s\"", file_name[file_selected]);
      SENDF("printpause.t0.txt=\"%s\"", file_name[file_selected]);
      SEND("page askprint");
      break;

    case KEY_PRINT_FILE:
      if (value == 1) startPrint(file_selected);
      else if (value == 10) SEND("page main");
      break;

    case KEY_STORE_MEMORY:
      if (value == 0xF1) {
        injectCommands(F("M502\nM500"));
        sendModel();
      }
      break;

    case KEY_CHANGE_PAGE: sendZOffset(F("leveldata")); break;

    case KEY_HARDWARE_TEST:
      switch (value) {
        #define _TEST_PIN(N, P) SENDF(N ".bco=%u", READ(P) ? COLOR_ON : COLOR_OFF)
        #if PIN_EXISTS(X_STOP)
          case 0: _TEST_PIN("x", X_STOP_PIN); break;
        #endif
        #if PIN_EXISTS(Y_STOP)
          case 1: _TEST_PIN("y", Y_STOP_PIN); break;
        #endif
        #if PIN_EXISTS(Z_MIN_PROBE)
          case 2: _TEST_PIN("z", Z_MIN_PROBE_PIN); break;
        #endif
        #if PIN_EXISTS(FIL_RUNOUT1)
          case 3: _TEST_PIN("mtd", FIL_RUNOUT1_PIN); break;
        #endif
        case 4: setTargetTemp_celsius(260, H0); SENDF("nozzle.bco=%u", COLOR_ON); break;
        case 5: setTargetTemp_celsius(100, BED); SENDF("bed.bco=%u", COLOR_ON); break;
        case 6: thermalManager.set_fan_speed(0, 255); SENDF("fan.bco=%u", COLOR_ON); SEND("set.va0.val=1"); break;
        case 7:
          status_led2 = false; toggleCaseLight();
          SENDF("led.bco=%u", COLOR_ON);
          break;
        case 8: setTargetTemp_celsius(0, H0); SENDF("nozzle.bco=%u", COLOR_OFF); break;
        case 9: setTargetTemp_celsius(0, BED); SENDF("bed.bco=%u", COLOR_OFF); break;
        case 10: thermalManager.set_fan_speed(0, 0); SENDF("fan.bco=%u", COLOR_OFF); SEND("set.va0.val=0"); break;
        case 11:
          status_led2 = true; toggleCaseLight();
          SENDF("led.bco=%u", COLOR_OFF);
          break;
        case 12: case 13: {
          // Run each motor briefly forward or back
          if (isMoving()) break;
          injectCommands(value == 12 ? F("M302P1\nG91\nG1X5Y5Z5E5F3000\nG90") : F("G91\nG1X-5Y-5Z-5E-5F3000\nG90\nM302P0"));
          SENDF("motor1.bco=%u", value == 12 ? COLOR_ON : COLOR_OFF);
          SENDF("motor2.bco=%u", value == 12 ? COLOR_OFF : COLOR_ON);
        } break;
        case 15: {
          // The test page is only offered when the card has an MKS_TEST folder
          MediaFile root = card.getroot(), dir;
          if (dir.open(&root, "MKS_TEST", O_RDONLY)) { dir.close(); SEND("page hardwaretest"); }
        } break;
      }
      break;
  }
}

//
// Marlin events
//

void Neptune3TFT::printerKilled(FSTR_P const error) {
  if (error == GET_TEXT_F(MSG_KILL_HOMING_FAILED))
    SEND("page err_homefail");
  else if (error == GET_TEXT_F(MSG_ERR_THERMAL_RUNAWAY))
    SEND("page err_heatfail");
}

void Neptune3TFT::heatingError(const heater_id_t heater) {
  if (heater == H_BED) SEND("page err_bedheat"); else SEND("page err_nozzleheat");
}

void Neptune3TFT::minTempError(const heater_id_t heater) {
  if (heater == H_BED) SEND("page err_bedunder"); else SEND("page err_nozzleunde");
}

void Neptune3TFT::maxTempError(const heater_id_t heater) {
  if (heater == H_BED) SEND("page err_bedover"); else SEND("page err_nozzleover");
}

void Neptune3TFT::printTimerStarted() { print_started = false; }

void Neptune3TFT::printDone() {
  SEND("printpause.printprocess.val=100");
  SEND("printpause.printvalue.txt=\"100\"");
  SEND("page printfinish");
}

void Neptune3TFT::statusChanged(const char * const msg) {
  if (!strcmp_P(msg, GET_TEXT(MSG_LCD_PROBING_FAILED))) {
    wait_for = WAIT_NONE;
    SEND("page err_probefail");
  }
}

// M0 and other prompts continue straight away, as with the stock firmware.
// During a filament change the Resume keys answer them instead.
void Neptune3TFT::userConfirmRequired() {
  if (!did_pause_print) auto_confirm = true;
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void Neptune3TFT::pauseMode(const PauseMessage message) {
    pauseModeStatus = message;
    switch (message) {
      case PAUSE_MESSAGE_INSERT: SEND("page noFilamentPush"); break;             // Continue with a Resume key
      case PAUSE_MESSAGE_HEAT: auto_confirm = true; break;                       // Reheat the nozzle
      case PAUSE_MESSAGE_OPTION: auto_resume = true; break;                      // Don't purge more
      case PAUSE_MESSAGE_STATUS: SEND("page printpause"); break;
      default: SEND("page wait"); break;
    }
  }

#endif

void Neptune3TFT::homingStart() { homing = true; }

void Neptune3TFT::homingDone() {
  homing = false;
  switch (wait_for) {
    case WAIT_HOME_MOVE:
      wait_for = WAIT_NONE;
      SEND("page premove");
      break;
    case WAIT_HOME_LEVEL:
      wait_for = WAIT_NONE;
      SEND("page " N3_MESH_DATA);
      SEND(N3_MESH_PROBE ".tm0.en=0");
      SEND("leveling.tm0.en=0");
      break;
    default: break;
  }
}

void Neptune3TFT::levelingStart() { leveling = true; }

void Neptune3TFT::levelingDone() {
  leveling = false;
  if (wait_for != WAIT_LEVEL) return;
  wait_for = WAIT_NONE;
  SEND("page " N3_MESH_DATA);
  SEND(N3_MESH_PROBE ".tm0.en=0");
  SEND("leveling.tm0.en=0");
  SEND("page warn_zoffset");
}

void Neptune3TFT::meshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
  const uint8_t i = meshIndex(xpos, ypos);
  if (leveling) {
    SENDF(N3_MESH_PROBE ".q%d.picc=%d", i, N3_MESH_PICC);
    SEND(N3_MESH_PROBE ".tm0.en=1");
  }
  SENDF(N3_MESH_DATA ".x%d.val=%d", i, isnan(zval) ? 0 : int(LROUND(zval * 100)));
}

#if ENABLED(POWER_LOSS_RECOVERY)

  void Neptune3TFT::powerLossResume() {
    plr_pending = true;
    SEND("com_star");
    if (boot_step <= 101) return; // Offer to resume after the boot animation

    // Show the long name if the file is in the list
    const char * const sfn = recovery.info.sd_filename + (recovery.info.sd_filename[0] == '/');
    const char *name = sfn;
    for (uint8_t i = 0; i < file_count; ++i) if (!strcasecmp(file_short[i], sfn)) { name = file_name[i]; file_printed = i; strcpy(printed_short, file_short[i]); break; }

    SENDF("continueprint.t0.txt=\"%s\"", name);
    SENDF("printpause.t0.txt=\"%s\"", name);
    SEND("page continueprint");
  }

  void Neptune3TFT::setPowerLoss(const bool onoff) { SENDF("multiset.plrbutton.val=%d", onoff); }

#endif

//
// Settings
//

void Neptune3TFT::factoryReset() {
  #define _PRESET(N, H, B) n3_settings.material[N] = { H, B }
  #ifdef PREHEAT_1_TEMP_HOTEND
    _PRESET(0, PREHEAT_1_TEMP_HOTEND, PREHEAT_1_TEMP_BED);
  #else
    _PRESET(0, 200, 60);
  #endif
  #ifdef PREHEAT_2_TEMP_HOTEND
    _PRESET(1, PREHEAT_2_TEMP_HOTEND, PREHEAT_2_TEMP_BED);
  #else
    _PRESET(1, 240, 80);
  #endif
  #ifdef PREHEAT_3_TEMP_HOTEND
    _PRESET(2, PREHEAT_3_TEMP_HOTEND, PREHEAT_3_TEMP_BED);
  #else
    _PRESET(2, 220, 60);
  #endif
  #ifdef PREHEAT_4_TEMP_HOTEND
    _PRESET(3, PREHEAT_4_TEMP_HOTEND, PREHEAT_4_TEMP_BED);
  #else
    _PRESET(3, 200, 50);
  #endif
  #undef _PRESET
}

void Neptune3TFT::storeSettings(char *buff) {
  static_assert(sizeof(n3_settings) <= ExtUI::eeprom_data_size, "n3_settings is too large for ExtUI EEPROM data.");
  memcpy(buff, &n3_settings, sizeof(n3_settings));
}

void Neptune3TFT::loadSettings(const char *buff) {
  memcpy(&n3_settings, buff, sizeof(n3_settings));
}

void Neptune3TFT::postprocessSettings() {
  sendZOffset(F("leveldata"));
  sendMesh();
}

#endif // ELEGOO_NEPTUNE_3_TFT
