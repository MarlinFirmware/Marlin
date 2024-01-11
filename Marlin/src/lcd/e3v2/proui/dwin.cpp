/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 4.6.3
 * Date: 2023/10/27
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_LCD_PROUI)

#if !HAS_BED_PROBE && ENABLED(BABYSTEPPING)
  #define JUST_BABYSTEP 1
#endif

#if ANY(BABYSTEPPING, HAS_BED_PROBE)
  #define HAS_ZOFFSET_ITEM 1
#endif

#if MB(CREALITY_V24S1_301, CREALITY_V24S1_301F4)
  #define DASH_REDRAW 1
#endif

#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"
#include "../../../gcode/gcode.h"
#include "../../../gcode/queue.h"

#if HAS_MEDIA
  #include "../../../sd/cardreader.h"
  #include "../../../prouiex/file_header.h"
#endif

#if NEED_HEX_PRINT
  #include "../../../libs/hex_print.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../feature/host_actions.h"
#endif

#if HAS_MESH || HAS_BED_PROBE
  #include "../../../feature/bedlevel/bedlevel.h"
  #include "../../../feature/bedlevel/bedlevel_tools.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../../feature/bltouch.h"
#endif

#if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
  #include "../../../feature/babystep.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#if ENABLED(PRINTCOUNTER)
  #include "printstats.h"
#endif

#if ENABLED(CASE_LIGHT_MENU)
  #include "../../../feature/caselight.h"
#endif

#if ENABLED(LED_CONTROL_MENU)
  #include "../../../feature/leds/leds.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "../../../feature/tmc_util.h"
#endif

#if HAS_GCODE_PREVIEW
  #include "gcode_preview.h"
#endif

#if HAS_TOOLBAR
  #include "toolbar.h"
#endif

#if HAS_ESDIAG
  #include "endstop_diag.h"
#endif

#if HAS_PLOT
  #include "plot.h"
#endif

#if ANY(HAS_MESH, HAS_TRAMMING_WIZARD)
  #include "meshviewer.h"
#endif

#if HAS_LOCKSCREEN
  #include "lockscreen.h"
#endif

#if ENABLED(LCD_BED_TRAMMING)
  #include "bed_tramming.h"
#endif

#include "../../utf8.h"
#include "../../marlinui.h"
#include "menus.h"
#include "dwin_popup.h"
#include "dwin.h"

#define DEBUG_OUT ENABLED(DEBUG_DWIN)
#include "../../../core/debug_out.h"

#define PAUSE_HEAT

// Load and Unload limits
#ifndef EXTRUDE_MAXLENGTH
  #ifdef FILAMENT_CHANGE_UNLOAD_LENGTH
    #define EXTRUDE_MAXLENGTH (FILAMENT_CHANGE_UNLOAD_LENGTH + 10)
  #else
    #define EXTRUDE_MAXLENGTH 500
  #endif
#endif

// Juntion deviation limits
#define MIN_JD_MM             0.001
#define MAX_JD_MM             TERN(LIN_ADVANCE, 0.3f, 0.5f)

#if HAS_TRINAMIC_CONFIG
  #define MIN_TMC_CURRENT 100
  #define MAX_TMC_CURRENT 3000
#endif

// Editable temperature limits
#define MIN_ETEMP  0
#define MAX_ETEMP  thermalManager.hotend_max_target(EXT)
#define MIN_BEDTEMP 0
#define MAX_BEDTEMP BED_MAX_TARGET

#define DWIN_VAR_UPDATE_INTERVAL         1024
#define DWIN_UPDATE_INTERVAL             1024

#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, hmiData.manualZOffset)

// Structs
hmi_value_t hmiValue;
hmi_flag_t hmiFlag{0};
hmi_data_t hmiData;

enum SelectItem : uint8_t {
  PAGE_FILES = 0,
  PAGE_PREPARE,
  PAGE_CONTROL,
  PAGE_ADVANCE,
#if HAS_TOOLBAR
  PAGE_TOOLBAR,
#endif
  PAGE_COUNT,

  PRINT_SETUP = 0,
  PRINT_PAUSE_RESUME,
  PRINT_STOP,
  PRINT_COUNT
};

typedef struct {
  uint8_t now, last;
  void set(uint8_t v) { now = last = v; }
  void reset() { set(0); }
  bool changed() { bool c = (now != last); if (c) last = now; return c; }
  bool dec() { if (now) now--; return changed(); }
  bool inc(uint8_t v) { if (now < (v - 1)) now++; else now = (v - 1); return changed(); }
} select_t;
select_t select_page{0}, select_print{0};

bool hash_changed = true; // Flag to know if message status was changed
bool blink = false;
uint8_t checkkey = 255, last_checkkey = ID_MainMenu;

char dateTime[16+1] =
{
  // YY year
  __DATE__[7], __DATE__[8],__DATE__[9], __DATE__[10],
  // First month letter, Oct Nov Dec = '1' otherwise '0'
  (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') ? '1' : '0',
  // Second month letter
  (__DATE__[0] == 'J') ? ( (__DATE__[1] == 'a') ? '1' :       // Jan, Jun or Jul
                          ((__DATE__[2] == 'n') ? '6' : '7') ) :
  (__DATE__[0] == 'F') ? '2' :                                // Feb
  (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? '3' : '5' :   // Mar or May
  (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? '4' : '8' :   // Apr or Aug
  (__DATE__[0] == 'S') ? '9' :                                // Sep
  (__DATE__[0] == 'O') ? '0' :                                // Oct
  (__DATE__[0] == 'N') ? '1' :                                // Nov
  (__DATE__[0] == 'D') ? '2' :                                // Dec
  0,
  // First day letter, replace space with digit
  __DATE__[4]==' ' ? '0' : __DATE__[4],
  // Second day letter
  __DATE__[5],
  // Separator
  ' ','-',' ',
  // Time
  __TIME__[0],__TIME__[1],__TIME__[2],__TIME__[3],__TIME__[4],
  '\0'
};

// New menu system pointers
Menu *fileMenu = nullptr;
Menu *prepareMenu = nullptr;
#if ENABLED(LCD_BED_TRAMMING)
  Menu *trammingMenu = nullptr;
#endif
Menu *moveMenu = nullptr;
Menu *controlMenu = nullptr;
Menu *advancedSettings = nullptr;
#if HAS_HOME_OFFSET
  Menu *homeOffMenu = nullptr;
#endif
#if HAS_BED_PROBE
  Menu *probeSetMenu = nullptr;
#endif
Menu *filSetMenu = nullptr;
Menu *selectColorMenu = nullptr;
Menu *getColorMenu = nullptr;
Menu *tuneMenu = nullptr;
Menu *motionMenu = nullptr;
Menu *filamentMenu = nullptr;
#if ENABLED(MESH_BED_LEVELING)
  Menu *manualMesh = nullptr;
#endif
#if HAS_PREHEAT
  Menu *preheatMenu = nullptr;
  Menu *preheatHotendMenu = nullptr;
#endif
Menu *temperatureMenu = nullptr;
Menu *maxSpeedMenu = nullptr;
Menu *maxAccelMenu = nullptr;
#if ENABLED(CLASSIC_JERK)
  Menu *maxJerkMenu = nullptr;
#endif
Menu *stepsMenu = nullptr;
#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  Menu *hotendMPCMenu = nullptr;
#endif
#if ENABLED(PIDTEMP) && ANY(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  Menu *hotendPIDMenu = nullptr;
#endif
#if ENABLED(PIDTEMPBED) && ANY(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  Menu *bedPIDMenu = nullptr;
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  Menu *caseLightMenu = nullptr;
#endif
#if ENABLED(LED_CONTROL_MENU)
  Menu *ledControlMenu = nullptr;
#endif
#if HAS_BED_PROBE
  Menu *zOffsetWizMenu = nullptr;
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  Menu *homingMenu = nullptr;
#endif
#if ENABLED(FWRETRACT)
  Menu *fwRetractMenu = nullptr;
#endif
#if HAS_MESH
  Menu *meshMenu = nullptr;
  #if ENABLED(MESH_EDIT_MENU)
    Menu *editMeshMenu = nullptr;
  #endif
#endif
#if PROUI_EX
  #if HAS_TOOLBAR
    Menu *tbSetupMenu = nullptr;
  #endif
  #if ENABLED(NOZZLE_PARK_FEATURE)
    Menu *parkPosMenu = nullptr;
  #endif
  Menu *phySetMenu = nullptr;
  #if HAS_MESH
    Menu *meshInsetMenu = nullptr;
  #endif
#endif
#if ENABLED(SHAPING_MENU)
  Menu *inputShapingMenu = nullptr;
#endif
#if HAS_TRINAMIC_CONFIG
  Menu *trinamicConfigMenu = nullptr;
#endif
#if ENABLED(CV_LASER_MODULE)
  Menu *laserSettings = nullptr;
  Menu *laserPrintMenu = nullptr;
#endif

// Updatable menuitems
int8_t hotendTargetItem;
int8_t bedTargetItem;
int8_t fanSpeedItem;
int8_t mMeshMoveZItem;
int8_t editZValueItem;

//-----------------------------------------------------------------------------
// Main Buttons
//-----------------------------------------------------------------------------

void iconButton(const bool selected, const int iconid, const frame_rect_t &ico, FSTR_P caption) {
  DWINUI::drawIconWB(iconid + selected, ico.x, ico.y);
  if (selected) DWINUI::drawBox(0, hmiData.colorHighlight, ico);
  const uint16_t x = ico.x + (ico.w - strlen_P(FTOP(caption)) * DWINUI::fontWidth()) / 2,
                 y = (ico.y + ico.h - 20) - DWINUI::fontHeight() / 2;
  DWINUI::drawString(x, y, caption);
}

//
// Main Menu: "Print"
//
void iconFiles() {
  constexpr frame_rect_t ico = { 17, 110 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_FILES, ICON_Print_0, ico, GET_TEXT_F(MSG_BUTTON_MEDIA));
}

//
// Main Menu: "Prepare"
//
void iconPrepare() {
  constexpr frame_rect_t ico = { 145, 110 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_PREPARE, ICON_Prepare_0, ico, GET_TEXT_F(MSG_PREPARE));
}

//
// Main Menu: "Control"
//
void iconControl() {
  constexpr frame_rect_t ico = { 17, 226 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_CONTROL, ICON_Control_0, ico, GET_TEXT_F(MSG_CONTROL));
}

//
// Main Menu: "Advanced Settings"
//
void iconAdvSettings() {
  constexpr frame_rect_t ico = { 145, 226 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_ADVANCE, ICON_Info_0, ico, GET_TEXT_F(MSG_BUTTON_ADVANCED));
}

//
// Printing: "Tune"
//
void iconTune() {
  constexpr frame_rect_t ico = { 8, 232, 80, 100 };
  iconButton(select_print.now == PRINT_SETUP, ICON_Setup_0, ico, GET_TEXT_F(MSG_TUNE));
}

//
// Printing: "Pause"
//
void iconPause() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  iconButton(select_print.now == PRINT_PAUSE_RESUME, ICON_Pause_0, ico, GET_TEXT_F(MSG_BUTTON_PAUSE));
}

//
// Printing: "Resume"
//
void iconResume() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  iconButton(select_print.now == PRINT_PAUSE_RESUME, ICON_Continue_0, ico, GET_TEXT_F(MSG_BUTTON_RESUME));
}

//
// Printing: "Stop"
//
void iconStop() {
  constexpr frame_rect_t ico = { 184, 232, 80, 100 };
  iconButton(select_print.now == PRINT_STOP, ICON_Stop_0, ico, GET_TEXT_F(MSG_BUTTON_STOP));
}

//
// PopUps
//
void popupPauseOrStop() {
  dwinPopupConfirmCancel(ICON_BLTouch, select_print.now == PRINT_PAUSE_RESUME ? GET_TEXT_F(MSG_PAUSE_PRINT) : GET_TEXT_F(MSG_STOP_PRINT));
}

#if HAS_HOTEND || HAS_HEATED_BED
  void dwinPopupTemperature(const int_fast8_t heater_id, const bool toohigh) {
    FSTR_P heaterstr = heater_id == H_BED ? GET_TEXT_F(MSG_BED_TEMPERATURE) : GET_TEXT_F(MSG_NOZZLE_TEMPERATURE);
    FSTR_P lowhighstr = toohigh ? GET_TEXT_F(MSG_TOO_HIGH) : GET_TEXT_F(MSG_TOO_LOW);
    hmiSaveProcessID(ID_WaitResponse);
    dwinShowPopup(toohigh ? ICON_TempTooHigh : ICON_TempTooLow, heaterstr, lowhighstr, BTN_Continue);
  }
#endif

//
// Draw status line
//
void dwinDrawStatusLine(const char *text) {
  dwinDrawRectangle(1, hmiData.colorStatusBg, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
  if (text) DWINUI::drawCenteredString(hmiData.colorStatusTxt, STATUS_Y + 2, text);
}

// Clear & reset status line
void dwinResetStatusLine() {
  ui.status_message.clear();
  dwinCheckStatusMessage();
}

// Check for a change in the status message
void dwinCheckStatusMessage() {
  static MString<>::hash_t old_hash = 0x0000;
  const MString<>::hash_t hash = ui.status_message.hash();
  hash_changed = hash != old_hash;
  old_hash = hash;
}

void dwinDrawStatusMessage() {
  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    // Get the UTF8 character count of the string
    uint8_t slen = ui.status_message.glyphs();

    // If the string fits the status line do not scroll it
    if (slen <= LCD_WIDTH) {
      if (hash_changed) {
        dwinDrawStatusLine(ui.status_message);
        hash_changed = false;
      }
    }
    else {
      // String is larger than the available line space

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
      const char *stat = ui.status_and_len(rlen);
      dwinDrawRectangle(1, hmiData.colorStatusBg, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
      DWINUI::moveTo(0, STATUS_Y + 2);
      DWINUI::drawString(hmiData.colorStatusTxt, stat, LCD_WIDTH);

      // If the string doesn't completely fill the line...
      if (rlen < LCD_WIDTH) {
        DWINUI::drawChar(hmiData.colorStatusTxt, '.');    // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;                  // Amount of space left in characters
        if (--chars) {                                     // Draw a second dot if there's space
          DWINUI::drawChar(hmiData.colorStatusTxt, '.');
          if (--chars)
            DWINUI::drawString(hmiData.colorStatusTxt, ui.status_message, chars); // Print a second copy of the message
        }
      }
      ui.advance_status_scroll();
    }

  #else

    if (hash_changed) {
      ui.status_message.trunc(LCD_WIDTH);
      dwinDrawStatusLine(ui.status_message);
      hash_changed = false;
    }

  #endif
}

void drawPrintLabels() {
  DWINUI::drawString( 46, 173, GET_TEXT_F(MSG_INFO_PRINT_TIME));
  DWINUI::drawString(181, 173, GET_TEXT_F(MSG_REMAINING_TIME));
}

void drawPrintProgressBar() {
  const uint8_t _percent_done = ui.get_progress_percent();
  DWINUI::drawIconWB(ICON_Bar, 15, 93);
  dwinDrawRectangle(1, hmiData.colorBarfill, 16 + (_percent_done * 240) / 100, 93, 256, 113);
  DWINUI::drawInt(hmiData.colorPercentTxt, hmiData.colorBackground, 3, 117, 133, _percent_done);
  DWINUI::drawString(hmiData.colorPercentTxt, 142, 133, F("%"));
}

void drawPrintProgressElapsed() {
  MString<12> buf;
  duration_t elapsed = print_job_timer.duration(); // Print timer
  buf.setf(F("%02i:%02i "), uint16_t(elapsed.value / 3600), (uint16_t(elapsed.value) % 3600) / 60);
  DWINUI::drawString(hmiData.colorText, hmiData.colorBackground, 47, 192, buf);
}

#if ENABLED(SHOW_REMAINING_TIME)
  void drawPrintProgressRemain() {
    const uint32_t _remain_time = ui.get_remaining_time();
    MString<12> buf;
    buf.setf(F("%02i:%02i "), _remain_time / 3600, (_remain_time % 3600) / 60);
    DWINUI::drawString(hmiData.colorText, hmiData.colorBackground, 181, 192, buf);
  }
#endif

void iconResumeOrPause() {
  if (checkkey == ID_PrintProcess) (print_job_timer.isPaused() || hmiFlag.pause_flag) ? iconResume() : iconPause();
}

// Update filename on print
void dwinPrintHeader(const char *text = nullptr) {
  static char headertxt[31] = "";  // Print header text
  if (text) {
    const int8_t size = _MIN(30U, strlen_P(text));
    for (uint8_t i = 0; i < size; ++i) headertxt[i] = text[i];
    headertxt[size] = '\0';
  }
  if (checkkey == ID_PrintProcess || checkkey == ID_PrintDone) {
    dwinDrawRectangle(1, hmiData.colorBackground, 0, 60, DWIN_WIDTH, 60+16);
    DWINUI::drawCenteredString(60, headertxt);
  }
}

void drawPrintProcess() {
  #if ENABLED(CV_LASER_MODULE)
    title.draw(laser_device.is_laser_device() ? GET_TEXT_F(MSG_ENGRAVING) : GET_TEXT_F(MSG_PRINTING));
  #else
    title.draw(GET_TEXT_F(MSG_PRINTING));
  #endif
  DWINUI::clearMainArea();
  dwinPrintHeader(nullptr);
  drawPrintLabels();
  DWINUI::drawIcon(ICON_PrintTime, 15, 173);
  DWINUI::drawIcon(ICON_RemainTime, 150, 171);
  drawPrintProgressBar();
  drawPrintProgressElapsed();
  TERN_(SHOW_REMAINING_TIME, drawPrintProgressRemain());
  iconTune();
  iconResumeOrPause();
  iconStop();
}

void gotoPrintProcess() {
  if (checkkey == ID_PrintProcess)
    iconResumeOrPause();
  else {
    checkkey = ID_PrintProcess;
    drawPrintProcess();
    TERN_(DASH_REDRAW, dwinRedrawDash());
  }
  dwinUpdateLCD();
}

void drawPrintDone() {
  TERN_(SET_PROGRESS_PERCENT, ui.set_progress_done());
  TERN_(SET_REMAINING_TIME, ui.reset_remaining_time());
  title.draw(GET_TEXT_F(MSG_PRINT_DONE));
  DWINUI::clearMainArea();
  dwinPrintHeader(nullptr);
  #if HAS_GCODE_PREVIEW
    const bool haspreview = gPreview.isValid();
    if (haspreview) {
      gPreview.show();
      DWINUI::drawButton(BTN_Continue, 86, 295);
    }
  #else
    constexpr bool haspreview = false;
  #endif

  if (!haspreview) {
    drawPrintProgressBar();
    drawPrintLabels();
    DWINUI::drawIcon(ICON_PrintTime, 15, 173);
    DWINUI::drawIcon(ICON_RemainTime, 150, 171);
    drawPrintProgressElapsed();
    TERN_(SHOW_REMAINING_TIME, drawPrintProgressRemain());
    DWINUI::drawButton(BTN_Continue, 86, 273);
  }
}

void gotoPrintDone() {
  DEBUG_ECHOLNPGM("gotoPrintDone");
  wait_for_user = true;
  if (checkkey != ID_PrintDone) {
    checkkey = ID_PrintDone;
    drawPrintDone();
    dwinUpdateLCD();
  }
}

void drawMainMenu() {
  DWINUI::clearMainArea();
  #if ENABLED(CV_LASER_MODULE)
    title.draw(laser_device.is_laser_device() ? GET_TEXT_F(MSG_LASER_ENGRAVING) : GET_TEXT_F(MSG_3D_PRINTER));
  #else
    title.draw(GET_TEXT_F(MSG_MAIN_MENU));
  #endif
  DWINUI::drawIcon(ICON_LOGO, 71, 52);   // CREALITY logo
  iconFiles();
  iconPrepare();
  iconControl();
  iconAdvSettings();
  TERN_(HAS_TOOLBAR, drawToolBar());
}

void gotoMainMenu() {
  if (checkkey == ID_MainMenu) return;
  checkkey = ID_MainMenu;
  drawMainMenu();
  dwinUpdateLCD();
}

// Draw X, Y, Z and blink if in an un-homed or un-trusted state
void _update_axis_value(const AxisEnum axis, const uint16_t x, const uint16_t y, const bool force) {
  const bool draw_qmark = axis_should_home(axis),
             draw_empty = NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING) && !draw_qmark && !axis_is_trusted(axis);

  // Check for a position change
  static xyz_pos_t oldpos = { -1, -1, -1 };

  #if ALL(IS_FULL_CARTESIAN, SHOW_REAL_POS)
    const float p = planner.get_axis_position_mm(axis);
  #else
    const float p = current_position[axis];
  #endif

  const bool changed = oldpos[axis] != p;
  if (changed) oldpos[axis] = p;

  if (force || changed || draw_qmark || draw_empty) {
    if (blink && draw_qmark)
      DWINUI::drawString(hmiData.colorCoordinate, hmiData.colorBackground, x, y, F("  - ? -"));
    else if (blink && draw_empty)
      DWINUI::drawString(hmiData.colorCoordinate, hmiData.colorBackground, x, y, F("       "));
    else
      DWINUI::drawSignedFloat(hmiData.colorCoordinate, hmiData.colorBackground, 3, 2, x, y, p);
  }
}

void _drawIconBlink(bool &flag, const bool sensor, const uint8_t icon1, const uint8_t icon2, const uint16_t x, const uint16_t y) {
  #if DISABLED(NO_BLINK_IND)
    if (flag != sensor) {
      flag = sensor;
      if (!flag) {
        dwinDrawBox(1, hmiData.colorBackground, x-1, y-1, 22, 22);
        DWINUI::drawIcon(icon1, x, y);
      }
    }
    if (flag) {
      dwinDrawBox(1, blink ? hmiData.colorSplitLine : hmiData.colorBackground, x-1, y-1, 22, 22);
      DWINUI::drawIcon(icon2, x, y);
    }
  #else
    if (flag != sensor) {
      flag = sensor;
      dwinDrawBox(1, hmiData.colorBackground, x, y, 20, 20);
      DWINUI::drawIcon(flag ? icon2 : icon1, x, y);
    }
  #endif
}

#if HAS_HOTEND
  void _drawHotendIcon() {
    static bool _isHeatingHotend = false;
    _drawIconBlink(_isHeatingHotend, thermalManager.isHeatingHotend(EXT), ICON_HotendTemp, ICON_SetEndTemp, 10, 383);
  }
#endif

#if HAS_HEATED_BED
  void _drawBedIcon() {
    static bool _isHeatingBed = false;
    _drawIconBlink(_isHeatingBed, thermalManager.isHeatingBed(), ICON_BedTemp, ICON_SetBedTemp, 10, 416);
  }
#endif

void _drawZOffsetIcon() {
  #if HAS_LEVELING
    static bool _leveling_active = false;
    _drawIconBlink(_leveling_active, planner.leveling_active, ICON_Zoffset, ICON_SetZOffset, 186, 416);
  #else
    DWINUI::drawIcon(ICON_Zoffset, 187, 416);
  #endif
}

#if HAS_PROUI_RUNOUT_SENSOR
  void _drawRunoutIcon() {
    static bool _runout_active = false;
    _drawIconBlink(_runout_active, runout.enabled && !!FilamentSensorBase::poll_runout_states(), ICON_StepE, ICON_FilRunOut, 112, 416);
  }
#endif

void _drawFeedrate() {
  #if ENABLED(SHOW_SPEED_IND)
    int16_t _value;
    if (blink) {
      _value = feedrate_percentage;
      DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 116 + 4 * STAT_CHR_W + 2, 384, F(" %"));
    }
    else {
      _value = round(feedrate_mm_s * feedrate_percentage / 100);
      dwinDrawBox(1, hmiData.colorBackground, 116 + 5 * STAT_CHR_W + 2, 384, 20, 20);
    }
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 384, _value);
  #else
    static int16_t _feedrate = 100;
    if (_feedrate != feedrate_percentage) {
      _feedrate = feedrate_percentage;
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
    }
  #endif
}

void _drawXYZPosition(const bool force) {
  _update_axis_value(X_AXIS,  27, 459, force);
  _update_axis_value(Y_AXIS, 112, 459, force);
  _update_axis_value(Z_AXIS, 197, 459, force);
}

void updateVariable() {
  TERN_(DEBUG_DWIN,DWINUI::drawInt(COLOR_YELLOW, COLOR_BG_BLACK, 2, DWIN_WIDTH-6*DWINUI::fontWidth(), 6, checkkey));
  TERN_(DEBUG_DWIN,DWINUI::drawInt(COLOR_YELLOW, COLOR_BG_BLACK, 2, DWIN_WIDTH-3*DWINUI::fontWidth(), 6, last_checkkey));

  _drawXYZPosition(false);

  TERN_(CV_LASER_MODULE, if (laser_device.is_laser_device()) return);

  #if HAS_HOTEND
    static celsius_t _hotendtemp = 0, _hotendtarget = 0;
    const celsius_t hc = thermalManager.wholeDegHotend(EXT),
                    ht = thermalManager.degTargetHotend(EXT);
    const bool _new_hotend_temp = _hotendtemp != hc,
               _new_hotend_target = _hotendtarget != ht;
    if (_new_hotend_temp) _hotendtemp = hc;
    if (_new_hotend_target) _hotendtarget = ht;
  #endif
  #if HAS_HEATED_BED
    static celsius_t _bedtemp = 0, _bedtarget = 0;
    const celsius_t bc = thermalManager.wholeDegBed(),
                    bt = thermalManager.degTargetBed();
    const bool _new_bed_temp = _bedtemp != bc,
               _new_bed_target = _bedtarget != bt;
    if (_new_bed_temp) _bedtemp = bc;
    if (_new_bed_target) _bedtarget = bt;
  #endif
  #if HAS_FAN
    static uint8_t _fanspeed = 0;
    const bool _new_fanspeed = _fanspeed != thermalManager.fan_speed[FAN];
    if (_new_fanspeed) _fanspeed = thermalManager.fan_speed[FAN];
  #endif

  if (isMenu(tuneMenu) || isMenu(temperatureMenu)) {
    // Tune page temperature update
    TERN_(HAS_HOTEND, if (_new_hotend_target) redrawItem(hotendTargetItem));
    TERN_(HAS_HEATED_BED, if (_new_bed_target) redrawItem(bedTargetItem));
    TERN_(HAS_FAN, if (_new_fanspeed) redrawItem(fanSpeedItem));
  }

  // Bottom temperature update

  #if HAS_HOTEND
    if (_new_hotend_temp)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 384, _hotendtemp);
    if (_new_hotend_target)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);
    _drawHotendIcon();

    static int16_t _flow = planner.flow_percentage[EXT];
    if (_flow != planner.flow_percentage[EXT]) {
      _flow = planner.flow_percentage[EXT];
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 417, _flow);
    }
  #endif

  #if HAS_HEATED_BED
    if (_new_bed_temp)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 417, _bedtemp);
    if (_new_bed_target)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
    _drawBedIcon();
  #endif

  _drawFeedrate();

  #if HAS_FAN
    if (_new_fanspeed)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    DWINUI::drawSignedFloat(DWIN_FONT_STAT, hmiData.colorIndicator,  hmiData.colorBackground, 2, 2, 204, 417, _offset);
  }

  #if HAS_PROUI_RUNOUT_SENSOR
    _drawRunoutIcon();
  #endif

  _drawZOffsetIcon();
}

/**
 * Memory card and file management
 */

bool DWIN_lcd_sd_status = false;

#if ENABLED(MEDIASORT_MENU_ITEM)
  void setMediaSort() {
    toggleCheckboxLine(hmiData.mediaSort);
    card.setSortOn(hmiData.mediaSort ? TERN(SDSORT_REVERSE, AS_REV, AS_FWD) : AS_OFF);
  }
#endif

#if DISABLED(HAS_SD_EXTENDER)
  void setMediaAutoMount() { toggleCheckboxLine(hmiData.mediaAutoMount); }
#endif

inline uint16_t nr_sd_menu_items() {
  return _MIN(card.get_num_items() + !card.flag.workDirIsRoot, MENU_MAX_ITEMS);
}

void makeNameWithoutExt(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
  size_t pos = strlen(src);  // index of ending nul

  // For files, remove the extension
  // which may be .gcode, .gco, or .g
  if (!card.flag.filenameIsDir)
    while (pos && src[pos] != '.') pos--; // find last '.' (stop at 0)

  if (!pos) pos = strlen(src);  // pos = 0 ('.' not found) restore pos

  size_t len = pos;   // nul or '.'
  if (len > maxlen) { // Keep the name short
    pos        = len = maxlen; // move nul down
    dst[--pos] = '.'; // insert dots
    dst[--pos] = '.';
    dst[--pos] = '.';
  }

  dst[len] = '\0';    // end it

  // Copy down to 0
  while (pos--) dst[pos] = src[pos];
}

void sdCardUp() {
  card.cdup();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

void sdCardFolder(char * const dirname) {
  card.cd(dirname);
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

void onClickSDItem() {
  const uint16_t hasUpDir = !card.flag.workDirIsRoot;
  if (hasUpDir && currentMenu->selected == 1) return sdCardUp();
  else {
    const uint16_t filenum = currentMenu->selected - 1 - hasUpDir;
    card.selectFileByIndexSorted(filenum);

    // Enter that folder!
    if (card.flag.filenameIsDir) return sdCardFolder(card.filename);

    if (card.fileIsBinary())
      return dwinPopupContinue(ICON_Error, GET_TEXT_F(MSG_CHECK_FILENAME), GET_TEXT_F(MSG_ONLY_GCODE));
    else
      return gotoConfirmToPrint();
  }
}

#if ENABLED(SCROLL_LONG_FILENAMES)
  char shift_name[LONG_FILENAME_LENGTH + 1] = "";

  void drawSDItemShifted(uint8_t &shift) {
    // Shorten to the available space
    const size_t lastchar = shift + MENU_CHAR_LIMIT;
    const char c = shift_name[lastchar];
    shift_name[lastchar] = '\0';

    const uint8_t row = fileMenu->line();
    eraseMenuText(row);
    drawMenuItem(row, 0, &shift_name[shift]);

    shift_name[lastchar] = c;
  }

  void fileMenuIdle(bool reset=false) {
    static bool hasUpDir = false;
    static uint8_t last_itemselected = 0;
    static int8_t shift_amt = 0, shift_len = 0;
    if (reset) {
      last_itemselected = 0;
      hasUpDir = !card.flag.workDirIsRoot; // is a SubDir
      return;
    }
    const uint8_t selected = fileMenu->selected;
    if (last_itemselected != selected) {
      if (last_itemselected >= 1 + hasUpDir) redrawItem(last_itemselected, true);
      last_itemselected = selected;
      if (selected >= 1 + hasUpDir) {
        const int8_t filenum = selected - 1 - hasUpDir; // Skip "Back" and ".."
        card.selectFileByIndexSorted(filenum);
        makeNameWithoutExt(shift_name, card.longest_filename(), LONG_FILENAME_LENGTH);
        shift_len = strlen(shift_name);
        shift_amt = 0;
      }
    }
    else if ((selected >= 1 + hasUpDir) && (shift_len > MENU_CHAR_LIMIT)) {
      uint8_t shift_new = _MIN(shift_amt + 1, shift_len - MENU_CHAR_LIMIT); // Try to shift by...
      drawSDItemShifted(shift_new);             // Draw the item
      if (shift_new == shift_amt)                 // Scroll reached the end
        shift_new = -1;                           // Reset
      shift_amt = shift_new;                      // Set new scroll
    }
  }
#else
  char shift_name[FILENAME_LENGTH + 1] = "";
#endif

void onDrawFileName(int8_t pos, int8_t line) {
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && pos == 1) {
    drawMenuItem(line, ICON_Folder, "..");
  }
  else {
    uint8_t icon;
    card.selectFileByIndexSorted(pos - is_subdir - 1);
    makeNameWithoutExt(shift_name, card.longest_filename());
    icon = card.flag.filenameIsDir ? ICON_Folder : card.fileIsBinary() ? ICON_Binary : ICON_File;
    drawMenuItem(line, icon, shift_name);
  }
}

void drawFileMenu() {
  if (notCurrentMenu(fileMenu)) {
    BACK_ITEM(gotoMainMenu);
    if (card.isMounted())
      for (uint8_t i = 0; i < nr_sd_menu_items(); ++i) menuItemAdd(onDrawFileName, onClickSDItem);
  }
  SET_MENU(fileMenu, MSG_MEDIA_MENU);
  if (!card.isMounted()) {
    dwinDrawRectangle(1, hmiData.colorAlertBg, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::drawCenteredString(font12x24, hmiData.colorAlertTxt, MBASE(3), GET_TEXT_F(MSG_MEDIA_NOT_INSERTED));
  }
  TERN_(DASH_REDRAW, dwinRedrawDash());
  TERN_(SCROLL_LONG_FILENAMES, fileMenuIdle(true));
}

//
// Watch for media mount / unmount
//
void hmiSDCardUpdate() {
  if (checkkey == ID_Homing) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    resetMenu(fileMenu);
    if (isMenu(fileMenu)) {
      currentMenu = nullptr;
      drawFileMenu();
    }
    if (!DWIN_lcd_sd_status && sdPrinting()) ui.abort_print();  // Media removed while printing
  }
}

/**
 * Dash board and indicators
 */

void dwinDrawDashboard() {

  dwinDrawRectangle(1, hmiData.colorBackground, 0, STATUS_Y + 21, DWIN_WIDTH, DWIN_HEIGHT - 1);

  dwinDrawRectangle(1, hmiData.colorSplitLine, 0, 449, DWIN_WIDTH, 451);

  DWINUI::drawIcon(ICON_MaxSpeedX,  10, 456);
  DWINUI::drawIcon(ICON_MaxSpeedY,  95, 456);
  DWINUI::drawIcon(ICON_MaxSpeedZ, 180, 456);
  _drawXYZPosition(true);

  TERN_(CV_LASER_MODULE, if (laser_device.is_laser_device()) return);

  #if HAS_HOTEND
    DWINUI::drawIcon(ICON_HotendTemp, 10, 383);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 384, thermalManager.wholeDegHotend(EXT));
    DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(EXT));

    DWINUI::drawIcon(ICON_StepE, 112, 417);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[EXT]);
    DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWINUI::drawIcon(ICON_BedTemp, 10, 416);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 417, thermalManager.wholeDegBed());
    DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWINUI::drawIcon(ICON_Speed, 113, 383);
  DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  IF_DISABLED(SHOW_SPEED_IND, DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 116 + 5 * STAT_CHR_W + 2, 384, F("%")));

  #if HAS_FAN
    DWINUI::drawIcon(ICON_FanSpeed, 186, 383);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[FAN]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWINUI::drawIcon(planner.leveling_active ? ICON_SetZOffset : ICON_Zoffset, 186, 416);
    DWINUI::drawSignedFloat(DWIN_FONT_STAT, hmiData.colorIndicator,  hmiData.colorBackground, 2, 2, 204, 417, BABY_Z_VAR);
  #endif

}

void drawInfoMenu() {
  DWINUI::clearMainArea();
  title.draw(GET_TEXT_F(MSG_INFO_SCREEN));
  drawMenuItem(0, ICON_Back, GET_TEXT_F(MSG_BACK), false, true);
  char machine_size[21];
  machine_size[0] = '\0';
  sprintf_P(machine_size, PSTR("%ix%ix%i"), (int16_t)X_BED_SIZE, (int16_t)Y_BED_SIZE, (int16_t)Z_MAX_POS);

  DWINUI::drawCenteredString(92,  GET_TEXT_F(MSG_INFO_MACHINENAME));
  DWINUI::drawCenteredString(112, F(MACHINE_NAME));
  DWINUI::drawCenteredString(145, GET_TEXT_F(MSG_INFO_SIZE));
  DWINUI::drawCenteredString(165, machine_size);

  for (uint8_t i = 0; i < 4 - 2 * ENABLED(PROUI_EX); ++i) {
    DWINUI::drawIcon(ICON_Step + i, ICOX, 90 + i * MLINE);
    dwinDrawHLine(hmiData.colorSplitLine, 16, MYPOS(i + 2), 240);
  }

  #if PROUI_EX
    proUIEx.init();
  #else
    DWINUI::drawCenteredString(198, GET_TEXT_F(MSG_INFO_FWVERSION));
    DWINUI::drawCenteredString(218, F(SHORT_BUILD_VERSION));
    DWINUI::drawCenteredString(251, GET_TEXT_F(MSG_INFO_BUILD));
    DWINUI::drawCenteredString(271, F(dateTime));
  #endif

}

// Main Process
void hmiMainMenu() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_page.inc(PAGE_COUNT)) {
      switch (select_page.now) {
        case PAGE_FILES: iconFiles(); break;
        case PAGE_PREPARE: iconFiles(); iconPrepare(); break;
        case PAGE_CONTROL: iconPrepare(); iconControl(); break;
        case PAGE_ADVANCE: iconControl(); iconAdvSettings(); break;
        TERN_(HAS_TOOLBAR, case PAGE_TOOLBAR: iconAdvSettings(); gotoToolBar();  break);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_page.dec()) {
      switch (select_page.now) {
        case PAGE_FILES: iconFiles(); iconPrepare(); break;
        case PAGE_PREPARE: iconPrepare(); iconControl(); break;
        case PAGE_CONTROL: iconControl(); iconAdvSettings(); break;
        case PAGE_ADVANCE: iconAdvSettings(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_page.now) {
      case PAGE_FILES:
        if (ENABLED(HAS_SD_EXTENDER) || hmiData.mediaAutoMount) {
          card.mount();
          safe_delay(800);
        };
        drawFileMenu();
        break;
      case PAGE_PREPARE: drawPrepareMenu(); break;
      case PAGE_CONTROL: drawControlMenu(); break;
      case PAGE_ADVANCE: drawAdvancedSettingsMenu(); break;
    }
  }
  dwinUpdateLCD();
}

// Pause or Stop popup
void onClickPauseOrStop() {
  switch (select_print.now) {
    case PRINT_PAUSE_RESUME: if (hmiFlag.select_flag) ui.pause_print(); break; // confirm pause
    case PRINT_STOP: if (hmiFlag.select_flag) ui.abort_print(); break; // stop confirmed then abort print
    default: break;
  }
  return gotoPrintProcess();
}

// Printing
void hmiPrinting() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_print.inc(PRINT_COUNT)) {
      switch (select_print.now) {
        case PRINT_SETUP: iconTune(); break;
        case PRINT_PAUSE_RESUME: iconTune(); iconResumeOrPause(); break;
        case PRINT_STOP: iconResumeOrPause(); iconStop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_print.dec()) {
      switch (select_print.now) {
        case PRINT_SETUP: iconTune(); iconResumeOrPause(); break;
        case PRINT_PAUSE_RESUME: iconResumeOrPause(); iconStop(); break;
        case PRINT_STOP: iconStop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_print.now) {
      case PRINT_SETUP: drawTuneMenu(); break;
      case PRINT_PAUSE_RESUME:
        if (print_job_timer.isPaused()) {  // if printer is already in pause
          ui.resume_print();
          break;
        }
        else
          return gotoPopup(popupPauseOrStop, onClickPauseOrStop);
      case PRINT_STOP:
        return gotoPopup(popupPauseOrStop, onClickPauseOrStop);
      default: break;
    }
  }
  dwinUpdateLCD();
}

#include "../../../libs/buzzer.h"

void drawMainArea() {
  switch (checkkey) {
    case ID_MainMenu:               drawMainMenu(); break;
    case ID_PrintProcess:           drawPrintProcess(); break;
    case ID_PrintDone:              drawPrintDone(); break;
    #if HAS_ESDIAG
      case ID_ESDiagProcess:        drawEndStopDiag(); break;
    #endif
    case ID_Popup:                  popupDraw(); break;
    #if HAS_LOCKSCREEN
      case ID_Locked:               lockScreen.draw(); break;
    #endif
    case ID_Menu:
      #if HAS_TOOLBAR
        if (currentMenu == &toolBar) drawMainMenu(); else redrawMenu();
      #else
        redrawMenu();
      #endif
      break;
    case ID_SetInt:
    case ID_SetPInt:
    case ID_SetIntNoDraw:
    case ID_SetFloat:
    case ID_SetPFloat:              redrawMenu(true); break;
    default: break;
  }
}

void hmiWaitForUser() {
  encoderReceiveAnalyze();
  if (!wait_for_user) {
    switch (checkkey) {
      case ID_PrintDone:
        select_page.reset();
        gotoMainMenu();
        break;
      #if HAS_ABL_OR_UBL
        case ID_Leveling:
          #if PROUI_EX
            proUIEx.stopLeveling();
          #else
            hmiReturnScreen();
          #endif
          break;
      #endif
      default:
        hmiReturnScreen();
        break;
    }
  }
}

#if DISABLED(PROUI_EX)
  // Draws boot screen
  void hmiInit() {
    #ifndef BOOTSCREEN_TIMEOUT
      #define BOOTSCREEN_TIMEOUT 1100
    #endif
    DWINUI::drawBox(1, COLOR_BLACK, { 5, 220, DWIN_WIDTH - 5, DWINUI::fontHeight() });
    DWINUI::drawCenteredString(COLOR_WHITE, 220, F("Professional Firmware "));
    for (uint16_t t = 15; t <= 257; t += 11) {
      DWINUI::drawIcon(ICON_Bar, 15, 260);
      dwinDrawRectangle(1, hmiData.colorBackground, t, 260, 257, 280);
      dwinUpdateLCD();
      safe_delay((BOOTSCREEN_TIMEOUT) / 22);
    }
  }
#endif

void eachMomentUpdate() {
  static millis_t next_var_update_ms = 0, next_rts_update_ms = 0, next_status_update_ms = 0;
  const millis_t ms = millis();

  if (ELAPSED(ms, next_var_update_ms)) {
    next_var_update_ms = ms + DWIN_VAR_UPDATE_INTERVAL;
    blink = !blink;
    updateVariable();
    #if HAS_ESDIAG
      if (checkkey == ID_ESDiagProcess) esDiag.update();
    #endif
    #if HAS_PLOT
      if (checkkey == ID_PIDProcess) {
        TERN_(PIDTEMP, if (hmiValue.tempControl == PIDTEMP_START) plot.update(thermalManager.wholeDegHotend(EXT)));
        TERN_(PIDTEMPBED, if (hmiValue.tempControl == PIDTEMPBED_START) plot.update(thermalManager.wholeDegBed()));
      }
      TERN_(MPCTEMP, if (checkkey == ID_MPCProcess) plot.update(thermalManager.wholeDegHotend(EXT)));
    #endif
  }

  #if HAS_STATUS_MESSAGE_TIMEOUT
    bool did_expire = ui.status_reset_callback && (*ui.status_reset_callback)();
    did_expire |= ui.status_message_expire_ms && ELAPSED(ms, ui.status_message_expire_ms);
    if (did_expire) ui.reset_status();
  #endif

  if (ELAPSED(ms, next_status_update_ms)) {
    next_status_update_ms = ms + 500;
    dwinDrawStatusMessage();
    #if ENABLED(SCROLL_LONG_FILENAMES)
      if (isMenu(fileMenu)) fileMenuIdle();
    #endif
  }

  #if LCD_BACKLIGHT_TIMEOUT_MINS
    if (ui.backlight_off_ms && ELAPSED(ms, ui.backlight_off_ms)) {
      #ifdef NEOPIXEL_BKGD_INDEX_FIRST
        neo.set_background_off();
        neo.show();
      #endif
      dimBacklight();
      ui.backlight_off_ms = 0;
    }
  #endif

  if (!PENDING(ms, next_rts_update_ms)) {
    next_rts_update_ms = ms + DWIN_UPDATE_INTERVAL;

    if ((hmiFlag.printing_flag != isPrinting()) && (checkkey != ID_Homing) && (checkkey != ID_Leveling)) {
      hmiFlag.printing_flag = isPrinting();
      DEBUG_ECHOLNPGM("printing_flag: ", hmiFlag.printing_flag);
      if (hmiFlag.printing_flag)
        dwinPrintStarted();
      else if (hmiFlag.abort_flag)
        dwinPrintAborted();
      else
        dwinPrintFinished();
    }

    if ((print_job_timer.isPaused() != hmiFlag.pause_flag) && (checkkey != ID_Homing)) {
      hmiFlag.pause_flag = print_job_timer.isPaused();
      DEBUG_ECHOLNPGM("pause_flag: ", hmiFlag.pause_flag);
      if (hmiFlag.pause_flag)
        dwinPrintPause();
      else if (hmiFlag.abort_flag)
        dwinPrintAborted();
      else
        dwinPrintResume();
    }

    if (checkkey == ID_PrintProcess) { // print process

      // Progress percent
      static uint8_t _percent_done = 255;
      if (_percent_done != ui.get_progress_percent()) {
        _percent_done = ui.get_progress_percent();
        drawPrintProgressBar();
      }

      // Remaining time
      #if ENABLED(SHOW_REMAINING_TIME)
        static uint32_t _remain_time = 0;
        if (_remain_time != ui.get_remaining_time()) {
          _remain_time = ui.get_remaining_time();
          drawPrintProgressRemain();
        }
      #endif

      // Elapse print time
      static uint16_t _printtime = 0;
      const uint16_t min = (print_job_timer.duration() % 3600) / 60;
      if (_printtime != min) { // 1 minute update
        _printtime = min;
        drawPrintProgressElapsed();
      }

    }
    #if ENABLED(POWER_LOSS_RECOVERY)
      else if (DWIN_lcd_sd_status && recovery.dwin_flag) { // resume print before power off
        return gotoPowerLossRecovery();
      }
    #endif // POWER_LOSS_RECOVERY
  }
  dwinUpdateLCD();
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void popupPowerLossRecovery() {
    DWINUI::clearMainArea();
    drawPopupBkgd();
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 70, GET_TEXT_F(MSG_OUTAGE_RECOVERY));
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 147, GET_TEXT_F(MSG_OUTAGE_RECV_1));
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 167, GET_TEXT_F(MSG_OUTAGE_RECV_2));
    DWINUI::drawButton(BTN_Cancel,    26, 280);
    DWINUI::drawButton(BTN_Continue, 146, 280);
    MediaFile *dir = nullptr;
    const char * const filename = card.diveToFile(true, dir, recovery.info.sd_filename);
    card.selectFileByName(filename);
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 207, card.longest_filename());
    dwinPrintHeader(card.longest_filename()); // Save filename
    drawSelectHighlight(hmiFlag.select_flag);
    dwinUpdateLCD();
  }

  void onClickPowerLossRecovery() {
    if (hmiFlag.select_flag) {
      queue.inject(F("M1000C"));
      select_page.reset();
      return gotoMainMenu();
    }
    else {
      hmiSaveProcessID(ID_NothingToDo);
      select_print.set(PRINT_SETUP);
      queue.inject(F("M1000"));
    }
  }

  void gotoPowerLossRecovery() {
    recovery.dwin_flag = false;
    LCD_MESSAGE(MSG_CONTINUE_PRINT_JOB);
    gotoPopup(popupPowerLossRecovery, onClickPowerLossRecovery);
  }

#endif // POWER_LOSS_RECOVERY


void dwinHandleScreen() {
  switch (checkkey) {
    case ID_MainMenu:        hmiMainMenu(); break;
    case ID_Menu:            hmiMenu(); break;
    case ID_SetInt:          hmiSetDraw(); break;
    case ID_SetFloat:        hmiSetDraw(); break;
    case ID_SetPInt:         hmiSetPInt(); break;
    case ID_SetPFloat:       hmiSetPFloat(); break;
    case ID_SetIntNoDraw:    hmiSetNoDraw(); break;
    case ID_PrintProcess:    hmiPrinting(); break;
    case ID_Popup:           hmiPopup(); break;
    case ID_Leveling:        TERN_(PROUI_EX, hmiWaitForUser();) break;
    #if HAS_LOCKSCREEN
      case ID_Locked:        hmiLockScreen(); break;
    #endif
    case ID_PrintDone:
    TERN_(HAS_ESDIAG, case ID_ESDiagProcess:)
    case ID_WaitResponse:    hmiWaitForUser(); break;
    case ID_Homing:
    case ID_PIDProcess:
    case ID_NothingToDo:     break;
    default: break;
  }
}

bool idIsPopUp() {    // If ID is popup...
  switch (checkkey) {
    case ID_NothingToDo:
    case ID_WaitResponse:
    case ID_Popup:
    case ID_Homing:
    case ID_Leveling:
    case ID_PIDProcess:
    TERN_(HAS_ESDIAG, case ID_ESDiagProcess:)
      return true;
    default: break;
  }
  return false;
}

void hmiSaveProcessID(const uint8_t id) {
  if (checkkey == id) return;
  if (!idIsPopUp()) last_checkkey = checkkey; // if previous is not a popup
  checkkey = id;
  switch (id) {
    case ID_Popup:
    case ID_WaitResponse:
    case ID_PrintDone:
    case ID_Leveling:
    TERN_(HAS_ESDIAG, case ID_ESDiagProcess:)
      wait_for_user = true;
    default: break;
  }
}

void hmiReturnScreen() {
  checkkey = last_checkkey;
  wait_for_user = false;
  drawMainArea();
}

void dwinHomingStart() {
  DEBUG_ECHOLNPGM("dwinHomingStart");
  hmiSaveProcessID(ID_Homing);
  title.draw(GET_TEXT_F(MSG_HOMING));
  dwinShowPopup(ICON_BLTouch, GET_TEXT_F(MSG_HOMING), GET_TEXT_F(MSG_PLEASE_WAIT));
}

void dwinHomingDone() {
  DEBUG_ECHOLNPGM("dwinHomingDone");
  #if DISABLED(HAS_BED_PROBE) && ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
    planner.synchronize();
    babystep.add_mm(Z_AXIS, hmiData.manualZOffset);
  #endif
  #if ENABLED(CV_LASER_MODULE)
    if (laser_device.is_laser_device()) laser_device.laser_home();
  #endif
  if (last_checkkey == ID_PrintDone)
    gotoPrintDone();
  else
    hmiReturnScreen();
}

#if HAS_LEVELING

  void dwinLevelingStart() {
    DEBUG_ECHOLNPGM("dwinLevelingStart");
    #if HAS_BED_PROBE
      hmiSaveProcessID(ID_Leveling);
      TERN_(PROUI_EX,proUIEx.cancel_lev = 0);
      title.draw(GET_TEXT_F(MSG_BED_LEVELING));
      #if PROUI_EX
        meshViewer.drawBackground(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y);
        DWINUI::drawButton(BTN_Cancel, 86, 305);
      #else
        dwinShowPopup(ICON_AutoLeveling, GET_TEXT_F(MSG_BED_LEVELING), GET_TEXT_F(MSG_PLEASE_WAIT), TERN(PROUI_EX, BTN_Cancel, 0));
      #endif
      #if ALL(AUTO_BED_LEVELING_UBL, PREHEAT_BEFORE_LEVELING)
        #if HAS_BED_PROBE
          if (!DEBUGGING(DRYRUN)) probe.preheat_for_probing(LEVELING_NOZZLE_TEMP, hmiData.bedLevT);
        #else
          #if HAS_HOTEND
            if (!DEBUGGING(DRYRUN) && thermalManager.degTargetHotend(EXT) < LEVELING_NOZZLE_TEMP) {
              thermalManager.setTargetHotend(LEVELING_NOZZLE_TEMP, EXT);
              thermalManager.wait_for_hotend(EXT);
            }
          #endif
          #if HAS_HEATED_BED
            if (!DEBUGGING(DRYRUN) && thermalManager.degTargetBed() < hmiData.bedLevT) {
              thermalManager.setTargetBed(hmiData.bedLevT);
              thermalManager.wait_for_bed_heating();
            }
          #endif
        #endif
      #endif
    #elif ENABLED(MESH_BED_LEVELING)
      drawManualMeshMenu();
    #endif
  }

  void dwinLevelingDone() {
    DEBUG_ECHOLNPGM("dwinLevelingDone");
    #if HAS_MESH
      #if PROUI_EX && HAS_BED_PROBE
        proUIEx.levelingDone();
      #else
        gotoMeshViewer(true);
      #endif
    #endif
  }

#endif // HAS_LEVELING

#if HAS_MESH
  void dwinMeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval) {
    ui.set_status(
      &MString<32>(GET_TEXT_F(MSG_PROBING_POINT), ' ', cpos, '/', tpos, F(" Z="), p_float_t(zval, 2))
    );
  }
#endif

// PID/MPC process

#if HAS_PLOT && ANY(HAS_PID_HEATING, MPC_AUTOTUNE)
  celsius_t _maxtemp, _target;
  void dwinDrawPIDMPCPopup() {
    constexpr frame_rect_t gfrm = { 40, 180, DWIN_WIDTH - 80, 120 };
    DWINUI::clearMainArea();
    drawPopupBkgd();
    // Draw labels
    switch (hmiValue.tempControl) {
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 100, GET_TEXT_F(MSG_MPC_AUTOTUNE));
          DWINUI::drawString(hmiData.colorPopupTxt, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, GET_TEXT_F(MSG_MPC_TARGET));
          break;
      #endif
      #if ANY(PIDTEMP, PIDTEMPBED)
        TERN_(PIDTEMP,    case PIDTEMP_START:)
        TERN_(PIDTEMPBED, case PIDTEMPBED_START:)
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 100, GET_TEXT_F(MSG_PID_AUTOTUNE));
          DWINUI::drawString(hmiData.colorPopupTxt, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, GET_TEXT_F(MSG_PID_TARGET));
          break;
      #endif
      default: break;
    }
    switch (hmiValue.tempControl) {
      #if ANY(PIDTEMP, MPC_AUTOTUNE)
        TERN_(PIDTEMP,      case PIDTEMP_START:)
        TERN_(MPC_AUTOTUNE, case MPCTEMP_START:)
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 120, GET_TEXT_F(MSG_NOZZLE_IS_RUN));
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 120, GET_TEXT_F(MSG_BED_IS_RUN));
          break;
      #endif
      default: break;
    }
    // Set values
    switch (hmiValue.tempControl) {
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[EXT];
          _target = 200;
          break;
      #endif
      #if ENABLED(PIDTEMP)
        case PIDTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[EXT];
          _target = hmiData.hotendPidT;
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
          _maxtemp = BED_MAXTEMP;
          _target = hmiData.bedPidT;
          break;
      #endif
      default: break;
    }
    plot.draw(gfrm, _maxtemp, _target);
    DWINUI::drawInt(hmiData.colorPopupTxt, 3, gfrm.x + 90, gfrm.y - DWINUI::fontHeight() - 4, _target);
  }
#endif // HAS_PLOT && ANY(HAS_PID_HEATING, MPC_AUTOTUNE)

#if HAS_PID_HEATING

  void dwinM303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp){
    if (seenC) hmiData.pidCycles = c;
    if (seenS) {
      switch (hid) {
        OPTCODE(PIDTEMP,    case 0 ... HOTENDS - 1: hmiData.hotendPidT = temp; break)
        OPTCODE(PIDTEMPBED, case H_BED:             hmiData.bedPidT = temp;    break)
        default: break;
      }
    }
  }

  void dwinPidTuning(tempcontrol_t result) {
    hmiValue.tempControl = result;
    switch (result) {
      case PIDTEMPBED_START:
        hmiSaveProcessID(ID_PIDProcess);
        #if HAS_PLOT
          dwinDrawPIDMPCPopup();
        #else
          dwinDrawPopup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_BED_IS_RUN));
        #endif
        break;
      case PIDTEMP_START:
        hmiSaveProcessID(ID_PIDProcess);
        #if HAS_PLOT
          dwinDrawPIDMPCPopup();
        #else
          dwinDrawPopup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_NOZZLE_IS_RUN));
        #endif
        break;
      case PID_BAD_HEATER_ID:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_BAD_HEATER_ID));
        break;
      case PID_TUNING_TIMEOUT:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), GET_TEXT_F(MSG_PID_TIMEOUT));
        break;
      case PID_TEMP_TOO_HIGH:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_TEMP_TOO_HIGH));
        break;
      case AUTOTUNE_DONE:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
        break;
      default:
        checkkey = last_checkkey;
        break;
    }
  }

#endif // HAS_PID_HEATING

#if ENABLED(MPC_AUTOTUNE)

  void dwinMPCTuning(tempcontrol_t result) {
    hmiValue.tempControl = result;
    switch (result) {
      case MPCTEMP_START:
        hmiSaveProcessID(ID_MPCProcess);
        #if HAS_PLOT
          dwinDrawPIDMPCPopup();
        #else
          dwinDrawPopup(ICON_TempTooHigh, GET_TEXT_F(MSG_MPC_AUTOTUNE), GET_TEXT_F(MSG_NOZZLE_IS_RUN));
        #endif
        break;
      case MPC_TEMP_ERROR:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), F(STR_MPC_TEMPERATURE_ERROR));
        ui.reset_alert_level();
        break;
      case MPC_INTERRUPTED:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), F(STR_MPC_AUTOTUNE_INTERRUPTED));
        ui.reset_alert_level();
        break;
      case AUTOTUNE_DONE:
        checkkey = last_checkkey;
        dwinPopupContinue(ICON_TempTooLow, GET_TEXT_F(MSG_MPC_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
        ui.reset_alert_level();
        break;
      default:
        checkkey = last_checkkey;
        ui.reset_alert_level();
        break;
    }
  }

#endif // MPC_AUTOTUNE

// Started a Print Job
void dwinPrintStarted() {
  DEBUG_ECHOLNPGM("dwinPrintStarted: ", sdPrinting());
  TERN_(SET_PROGRESS_PERCENT, ui.progress_reset());
  TERN_(SET_REMAINING_TIME, ui.reset_remaining_time());
  hmiFlag.pause_flag = false;
  hmiFlag.abort_flag = false;
  select_print.reset();
  #if PROUI_EX
    if (!fileprop.isConfig) gotoPrintProcess();
  #else
    gotoPrintProcess();
  #endif
}

// Pause a print job
void dwinPrintPause() {
  DEBUG_ECHOLNPGM("dwinPrintPause");
  iconResumeOrPause();
}

// Resume print job
void dwinPrintResume() {
  DEBUG_ECHOLNPGM("dwinPrintResume");
  iconResumeOrPause();
  LCD_MESSAGE(MSG_RESUME_PRINT);
}

// Ended print job
void dwinPrintFinished() {
  DEBUG_ECHOLNPGM("dwinPrintFinished");
  hmiFlag.abort_flag = false;
  hmiFlag.pause_flag = false;
  wait_for_heatup = false;
  #if PROUI_EX
    if (!fileprop.isConfig) gotoPrintDone();
    fileprop.clear();
  #else
    gotoPrintDone();
  #endif
}

// Print was aborted
void dwinPrintAborted() {
  DEBUG_ECHOLNPGM("dwinPrintAborted");
  #ifdef SD_FINISHED_RELEASECOMMAND
    queue.inject(SD_FINISHED_RELEASECOMMAND);
  #endif
  TERN_(HOST_PROMPT_SUPPORT, hostui.notify(GET_TEXT_F(MSG_PRINT_ABORTED)));
  dwinPrintFinished();
}

void dwinSetColorDefaults() {
  hmiData.colorBackground = defColorBackground;
  hmiData.colorCursor     = defColorCursor;
  hmiData.colorTitleBg    = defColorTitleBg;
  hmiData.colorTitleTxt   = defColorTitleTxt;
  hmiData.colorText       = defColorText;
  hmiData.colorSelected   = defColorSelected;
  hmiData.colorSplitLine  = defColorSplitLine;
  hmiData.colorHighlight  = defColorHighlight;
  hmiData.colorStatusBg   = defColorStatusBg;
  hmiData.colorStatusTxt  = defColorStatusTxt;
  hmiData.colorPopupBg    = defColorPopupBg;
  hmiData.colorPopupTxt   = defColorPopupTxt;
  hmiData.colorAlertBg    = defColorAlertBg;
  hmiData.colorAlertTxt   = defColorAlertTxt;
  hmiData.colorPercentTxt = defColorPercentTxt;
  hmiData.colorBarfill    = defColorBarfill;
  hmiData.colorIndicator  = defColorIndicator;
  hmiData.colorCoordinate = defColorCoordinate;
}

void dwinSetColors() {
  title.textColor = hmiData.colorTitleTxt;
  title.backColor = hmiData.colorTitleBg;
  DWINUI::setColors(hmiData.colorText, hmiData.colorBackground, hmiData.colorStatusBg);
}

void dwinSetDataDefaults() {
  DEBUG_ECHOLNPGM("dwinSetDataDefaults");
  dwinSetColorDefaults();
  dwinSetColors();
  TERN_(PIDTEMP, hmiData.hotendPidT = DEF_HOTENDPIDT);
  TERN_(PIDTEMPBED, hmiData.bedPidT = DEF_BEDPIDT);
  TERN_(HAS_PID_HEATING, hmiData.pidCycles = DEF_PIDCYCLES);
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    hmiData.extMinT = EXTRUDE_MINTEMP;
    applyExtMinT();
  #endif
  #if ALL(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    hmiData.bedLevT = LEVELING_BED_TEMP;
  #endif
  TERN_(BAUD_RATE_GCODE, hmiData.baud115K = (BAUDRATE == 115200));
  #if ALL(LCD_BED_TRAMMING, HAS_BED_PROBE)
    hmiData.fullManualTramming = DISABLED(BED_TRAMMING_USE_PROBE);
  #endif
  #if ENABLED(MEDIASORT_MENU_ITEM)
    hmiData.mediaSort = true;
    card.setSortOn(TERN(SDSORT_REVERSE, AS_REV, AS_FWD));
  #endif
  hmiData.mediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  #if ALL(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    hmiData.zAfterHoming = DEF_Z_AFTER_HOMING;
  #endif
  #if DISABLED(HAS_BED_PROBE)
    hmiData.manualZOffset = 0;
  #endif
  #if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    #if ENABLED(LED_COLOR_PRESETS)
      leds.set_default();
      applyLEDColor();
    #else
      hmiData.ledColor = Def_Leds_Color;
      leds.set_color(
        (hmiData.ledColor >> 16) & 0xFF,
        (hmiData.ledColor >>  8) & 0xFF,
        (hmiData.ledColor >>  0) & 0xFF
        OPTARG(HAS_WHITE_LED, (hmiData.ledColor >> 24) & 0xFF)
      );
    #endif
  #endif
  TERN_(ADAPTIVE_STEP_SMOOTHING, hmiData.adaptiveStepSmoothing = true);
  #if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
    hmiData.enablePreview = true;
  #endif
}

void dwinCopySettingsTo(char * const buff) {
  DEBUG_ECHOLNPGM("dwinCopySettingsTo");
  DEBUG_ECHOLNPGM("hmiData: ", sizeof(hmi_data_t));
  memcpy(buff, &hmiData, sizeof(hmi_data_t));
}

void dwinCopySettingsFrom(const char * const buff) {
  DEBUG_ECHOLNPGM("dwinCopySettingsFrom");
  memcpy(&hmiData, buff, sizeof(hmi_data_t));
  if (hmiData.colorText == hmiData.colorBackground) dwinSetColorDefaults();
  dwinSetColors();
  TERN_(PREVENT_COLD_EXTRUSION, applyExtMinT());
  feedrate_percentage = 100;
  TERN_(BAUD_RATE_GCODE, if (hmiData.baud115K) setBaud115K(); else setBaud250K());
  TERN_(MEDIASORT_MENU_ITEM, card.setSortOn(hmiData.mediaSort ? TERN(SDSORT_REVERSE, AS_REV, AS_FWD) : AS_OFF));
  #if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_color(
      (hmiData.ledColor >> 16) & 0xFF,
      (hmiData.ledColor >>  8) & 0xFF,
      (hmiData.ledColor >>  0) & 0xFF
      OPTARG(HAS_WHITE_LED, (hmiData.ledColor >> 24) & 0xFF)
    );
  #endif
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {
  DEBUG_ECHOLNPGM("MarlinUI::init_lcd");
  delay(750);   // wait to wakeup screen
  const bool hs = dwinHandshake(); UNUSED(hs);
  #if ENABLED(DEBUG_DWIN)
    SERIAL_ECHOPGM("dwinHandshake ");
    SERIAL_ECHOLN(hs ? F("ok.") : F("error."));
  #endif
  dwinFrameSetDir(1);
  dwinUpdateLCD();
  encoderConfiguration();
}

void dwinInitScreen() {
  DEBUG_ECHOLNPGM("dwinInitScreen");
  DWINUI::init();
  dwinSetColors();
  #if !PROUI_EX
    hmiInit();
  #endif
  initMenu();
  checkkey = 255;
  hash_changed = true;
  dwinDrawStatusLine("");
  dwinDrawDashboard();
  gotoMainMenu();
}

void MarlinUI::update() {
  hmiSDCardUpdate();   // SD card update
  eachMomentUpdate();  // Status update
  dwinHandleScreen();  // Rotary encoder update
}

void MarlinUI::refresh() { /* Nothing to see here */ }

#if HAS_LCD_BRIGHTNESS
  void MarlinUI::_set_brightness() { dwinLCDBrightness(backlight ? brightness : 0); }
#endif

void MarlinUI::kill_screen(FSTR_P const lcd_error, FSTR_P const) {
  dwinDrawPopup(ICON_BLTouch, GET_TEXT_F(MSG_PRINTER_KILLED), lcd_error);
  DWINUI::drawCenteredString(hmiData.colorPopupTxt, 270, GET_TEXT_F(MSG_TURN_OFF));
  dwinUpdateLCD();
}

void dwinRebootScreen() {
  dwinFrameClear(COLOR_BG_BLACK);
  dwinJPGShowAndCache(0);
  DWINUI::drawCenteredString(COLOR_WHITE, 220, GET_TEXT_F(MSG_PLEASE_WAIT_REBOOT));
  dwinUpdateLCD();
  safe_delay(500);
}

void dwinRedrawDash() {
  hash_changed = true;
  dwinDrawStatusMessage();
  dwinDrawDashboard();
}

void dwinRedrawScreen() {
  drawMainArea();
  dwinRedrawDash();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void dwinPopupPause(FSTR_P const fmsg, uint8_t button/*=0*/) {
    hmiSaveProcessID(button ? ID_WaitResponse : ID_NothingToDo);
    dwinShowPopup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), fmsg, button);
  }

  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t extruder/*=active_extruder*/) {
    //if (mode == PAUSE_MODE_SAME) return;
    pause_mode = mode;
    switch (message) {
      case PAUSE_MESSAGE_PARKING:  dwinPopupPause(GET_TEXT_F(MSG_PAUSE_PRINT_PARKING));    break;                // M125
      case PAUSE_MESSAGE_CHANGING: dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INIT));   break;                // pause_print (M125, M600)
      case PAUSE_MESSAGE_WAITING:  dwinPopupPause(GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING), BTN_Continue); break;
      case PAUSE_MESSAGE_INSERT:   dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INSERT), BTN_Continue); break;
      case PAUSE_MESSAGE_LOAD:     dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_LOAD));   break;
      case PAUSE_MESSAGE_UNLOAD:   dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_UNLOAD)); break;                // Unload of pause and Unload of M702
      case PAUSE_MESSAGE_PURGE:
        #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
          dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_CONT_PURGE));
        #else
          dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE));
        #endif
        break;
      case PAUSE_MESSAGE_OPTION:   gotoFilamentPurge(); break;
      case PAUSE_MESSAGE_RESUME:   dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT), BTN_Continue);   break;
      case PAUSE_MESSAGE_HEATING:  dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_STATUS:   hmiReturnScreen(); break;                                                      // Exit from Pause, Load and Unload
      default: break;
    }
  }

  void drawPopupFilamentPurge() {
    dwinDrawPopup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE_CONTINUE));
    DWINUI::drawButton(BTN_Purge, 26, 280);
    DWINUI::drawButton(BTN_Continue, 146, 280);
    drawSelectHighlight(true);
  }

  void onClickFilamentPurge() {
    if (hmiFlag.select_flag)
      pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // "Purge More" button
    else {
      hmiSaveProcessID(ID_NothingToDo);
      pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;  // "Continue" button
    }
  }

  void gotoFilamentPurge() {
    pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
    gotoPopup(drawPopupFilamentPurge, onClickFilamentPurge);
  }

#endif // ADVANCED_PAUSE_FEATURE

#if HAS_MESH
  void dwinMeshViewer() {
    hmiSaveProcessID(ID_WaitResponse);
    meshViewer.drawViewer(false, true);
  }
#endif // HAS_MESH


#if HAS_LOCKSCREEN

  void dwinLockScreen() {
    if (checkkey != ID_Locked) {
      lockScreen.rprocess = checkkey;
      checkkey = ID_Locked;
      lockScreen.init();
    }
  }

  void dwinUnLockScreen() {
    if (checkkey == ID_Locked) {
      checkkey = lockScreen.rprocess;
      drawMainArea();
    }
  }

  void hmiLockScreen() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    lockScreen.onEncoder(encoder_diffState);
    if (lockScreen.isUnlocked()) dwinUnLockScreen();
  }

#endif // HAS_LOCKSCREEN

#if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
  void setPreview() { toggleCheckboxLine(hmiData.enablePreview); }
#endif

#if ANY(HAS_GCODE_PREVIEW, ONE_CLICK_PRINT)
  void onClickConfirmToPrint() {
    dwinResetStatusLine();
    if (hmiFlag.select_flag) {     // Confirm
      gotoMainMenu();
      return card.openAndPrintFile(card.filename);
    }
    else
      hmiReturnScreen();
  }
#endif

#if ENABLED(ONE_CLICK_PRINT)
  void confirmToPrintPopup() {
    dwinPopupConfirmCancel(ICON_Info_0, GET_TEXT_F(MSG_START_PRINT));
    dwinResetStatusLine();
    ui.set_status_P(card.longest_filename(), true);
  }
#endif

void gotoConfirmToPrint() {
  #if PROUI_EX
    fileprop.clear();
    fileprop.setname(card.filename);
    card.openFileRead(fileprop.name, 100);
    getFileHeader();
    card.closefile();
    if (fileprop.isConfig) return card.openAndPrintFile(card.filename);
  #endif
  #if ENABLED(CV_LASER_MODULE)
    if(fileprop.isLaser)
      if (laser_device.is_laser_device()) return drawLaserPrintMenu(); else return drawLaserSettingsMenu();
    else
      laserOn(false); // If it is not laser file turn off laser mode
  #endif
  #if HAS_GCODE_PREVIEW
    if (TERN1(PREVIEW_MENU_ITEM, hmiData.enablePreview)) return gotoPopup(gPreview.draw, onClickConfirmToPrint);
  #endif
  #if ENABLED(ONE_CLICK_PRINT)
    return gotoPopup(confirmToPrintPopup, onClickConfirmToPrint);
  #endif
  card.openAndPrintFile(card.filename); // Direct print SD file
}

#if HAS_ESDIAG
  void drawEndStopDiag() {
    hmiSaveProcessID(ID_ESDiagProcess);
    esDiag.draw();
  }
#endif

//=============================================================================
// MENU SUBSYSTEM
//=============================================================================

// Tool functions

#if ENABLED(EEPROM_SETTINGS)

  void writeEeprom() {
    dwinDrawStatusLine(GET_TEXT_F(MSG_STORE_EEPROM));
    dwinUpdateLCD();
    DONE_BUZZ(settings.save());
  }

  void readEeprom() {
    const bool success = settings.load();
    dwinRedrawScreen();
    DONE_BUZZ(success);
  }

  void resetEeprom() {
    settings.reset();
    dwinRedrawScreen();
    DONE_BUZZ(true);
  }

  #if HAS_MESH
    void saveMesh() { TERN(AUTO_BED_LEVELING_UBL, ublMeshSave(), writeEeprom()); }
  #endif

#endif // EEPROM_SETTINGS

// Reset Printer
void rebootPrinter() {
  wait_for_heatup = wait_for_user = false;    // Stop waiting for heating/user
  thermalManager.disable_all_heaters();
  planner.finish_and_disable();
  dwinRebootScreen();
  hal.reboot();
}

void gotoInfoMenu() {
  drawInfoMenu();
  dwinUpdateLCD();
  hmiSaveProcessID(ID_WaitResponse);
}

void disableMotors() { queue.inject(F("M84")); }

void autoLev() {   // Always reacquire the Z "home" position
  queue.inject(F(TERN(AUTO_BED_LEVELING_UBL, "G29P1", "G29")));
}

void autoHome() { queue.inject_P(G28_STR); }

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void homeX() { queue.inject(F("G28X")); }
  void homeY() { queue.inject(F("G28Y")); }
  void homeZ() { queue.inject(F("G28Z")); }
  #if ALL(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    void applyZAfterHoming() { hmiData.zAfterHoming = menuData.value; };
    void setZAfterHoming() { setIntOnClick(0, 20, hmiData.zAfterHoming, applyZAfterHoming); }
  #endif
#endif

#if HAS_ZOFFSET_ITEM

  #if ALL(EEPROM_SETTINGS, ZOFFSET_SAVE_SETTINGS)
    void applyZOffset() {
      settings.save();
    }
  #endif
  void liveZOffset() {
    #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      const_float_t step_zoffset = round((menuData.value / 100.0f) * planner.settings.axis_steps_per_mm[Z_AXIS]) - babystep.accum;
      if (BABYSTEP_ALLOWED()) babystep.add_steps(Z_AXIS, step_zoffset);
      //SERIAL_ECHOLN(F("BB Steps: "), step_zoffset);
    #endif
  }
  void setZOffset() {
    #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      babystep.accum = round(planner.settings.axis_steps_per_mm[Z_AXIS] * BABY_Z_VAR);
    #endif
    #if ALL(EEPROM_SETTINGS, ZOFFSET_SAVE_SETTINGS)
      setPFloatOnClick(OFFSET_ZMIN, OFFSET_ZMAX, 2, applyZOffset, liveZOffset);
    #else
      setPFloatOnClick(OFFSET_ZMIN, OFFSET_ZMAX, 2, nullptr, liveZOffset);
    #endif
  }

  void setMoveZto0() {
    #if HAS_LEVELING && ANY(RESTORE_LEVELING_AFTER_G28, ENABLE_LEVELING_AFTER_G28)
      set_bed_leveling_enabled(false);
    #endif
    #if ENABLED(Z_SAFE_HOMING)
      gcode.process_subcommands_now(TS(F("G28Z\nG0F5000X"), Z_SAFE_HOMING_X_POINT, F("Y"), Z_SAFE_HOMING_Y_POINT, F("\nG0Z0F300\nM400")));
    #else
      gcode.process_subcommands_now(F("G28Z\nG0Z0F300\nM400"));
    #endif
    ui.reset_status();
    DONE_BUZZ(true);
  }

  #if !HAS_BED_PROBE
    void homeZandDisable() {
      setMoveZto0();
      disableMotors();
    }
  #endif

#endif // HAS_ZOFFSET_ITEM

#if HAS_PREHEAT
  #define _doPreheat(N) void DoPreheat##N() { ui.preheat_all(N-1); }\
                        void DoPreheatHotend##N() { ui.preheat_hotend(N-1); }
  REPEAT_1(PREHEAT_COUNT, _doPreheat)
#endif

void doCoolDown() { thermalManager.cooldown(); }

bool enableLiveMove = false;
void setLiveMove() { toggleCheckboxLine(enableLiveMove); }
void axisMove(uint8_t axis) {
  #if HAS_HOTEND
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(EXT)) {
      gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
      return dwinPopupContinue(ICON_TempTooLow, GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT));
    }
  #endif
  planner.synchronize();
  if (!planner.is_full()) planner.buffer_line(current_position, manual_feedrate_mm_s[axis]);
}
void liveMove() {
  if (!enableLiveMove) return;
  *menuData.floatPtr = menuData.value / MINUNITMULT;
  axisMove(hmiValue.select);
}
void applyMove() {
  if (enableLiveMove) return;
  axisMove(hmiValue.select);
}

#if ENABLED(CV_LASER_MODULE)
  void setMoveX() {
    hmiValue.select = X_AXIS;
    if (!laser_device.is_laser_device()) { setPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
    else setPFloatOnClick(X_MIN_POS - laser_device.homepos.x, X_MAX_POS - laser_device.homepos.x, UNITFDIGITS, applyMove, liveMove);
  }
  void setMoveY() {
    hmiValue.select = Y_AXIS;
    if (!laser_device.is_laser_device()) setPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, applyMove, liveMove);
    else setPFloatOnClick(Y_MIN_POS - laser_device.homepos.y, Y_MAX_POS - laser_device.homepos.y, UNITFDIGITS, applyMove, liveMove);
  }
  void setMoveZ() { hmiValue.select = Z_AXIS; setPFloatOnClick(laser_device.is_laser_device() ? -Z_MAX_POS : Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
#else
  void setMoveX() { hmiValue.select = X_AXIS; setPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
  void setMoveY() { hmiValue.select = Y_AXIS; setPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
  void setMoveZ() { hmiValue.select = Z_AXIS; setPFloatOnClick(Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
#endif

#if HAS_HOTEND
  void setMoveE() {
    #define E_MIN_POS (current_position.e - (EXTRUDE_MAXLENGTH))
    #define E_MAX_POS (current_position.e + (EXTRUDE_MAXLENGTH))
    hmiValue.select = E_AXIS; setPFloatOnClick(E_MIN_POS, E_MAX_POS, UNITFDIGITS, applyMove, liveMove);
  }
  void moveE100() {
    current_position.e+=100;
    axisMove(E_AXIS);
  }
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  void setPwrLossr() {
    toggleCheckboxLine(recovery.enabled);
    recovery.changed();
  }
#endif

#if ENABLED(BAUD_RATE_GCODE)
  void setBaudRate() {
    toggleCheckboxLine(hmiData.baud115K);
    if (hmiData.baud115K) setBaud115K(); else setBaud250K();
  }
  void setBaud115K() { queue.inject(F("M575B115")); }
  void setBaud250K() { queue.inject(F("M575B250")); }
#endif

#if HAS_LCD_BRIGHTNESS
  void applyBrightness() { ui.set_brightness(menuData.value); }
  void liveBrightness() { dwinLCDBrightness(menuData.value); }
  void setBrightness() { setIntOnClick(LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, ui.brightness, applyBrightness, liveBrightness); }
  void turnOffBacklight() { if (ui.backlight) ui.set_brightness(0); }
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    void dimBacklight() { if (ui.backlight) { dwinLCDBrightness(LCD_BRIGHTNESS_DIM); ui.backlight = false; } }
  #endif
#endif

#if ENABLED(CASE_LIGHT_MENU)
  void setCaseLight() {
    toggleCheckboxLine(caselight.on);
    caselight.update_enabled();
  }
  #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
    void liveCaseLightBrightness() { caselight.brightness = menuData.value; caselight.update_brightness(); }
    void setCaseLightBrightness() { setIntOnClick(0, 255, caselight.brightness, nullptr, liveCaseLightBrightness); }
  #endif
#endif

#if ENABLED(LED_CONTROL_MENU)
  #if !ALL(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
    void setLedStatus() {
      leds.toggle();
      showChkbLine(leds.lights_on);
    }
  #endif
  #if HAS_COLOR_LEDS
    void applyLEDColor() { hmiData.ledColor = TERN0(HAS_WHITE_LED, (leds.color.w << 24)) | (leds.color.r << 16) | (leds.color.g << 8) | leds.color.b; }
    void liveLEDColor(uint8_t *color) { *color = menuData.value; leds.update(); }
    void liveLEDColorR() { liveLEDColor(&leds.color.r); }
    void liveLEDColorG() { liveLEDColor(&leds.color.g); }
    void liveLEDColorB() { liveLEDColor(&leds.color.b); }
    void setLEDColorR() { setIntOnClick(0, 255, leds.color.r, applyLEDColor, liveLEDColorR); }
    void setLEDColorG() { setIntOnClick(0, 255, leds.color.g, applyLEDColor, liveLEDColorG); }
    void setLEDColorB() { setIntOnClick(0, 255, leds.color.b, applyLEDColor, liveLEDColorB); }
    #if HAS_WHITE_LED
      void liveLEDColorW() { liveLEDColor(&leds.color.w); }
      void setLEDColorW() { setIntOnClick(0, 255, leds.color.w, applyLEDColor, liveLEDColorW); }
    #endif
  #endif
#endif

#if ENABLED(SOUND_MENU_ITEM)
  void setEnableSound() {
    toggleCheckboxLine(ui.sound_on);
  }
#endif

#if HAS_HOME_OFFSET
  void applyHomeOffset() { set_home_offset(hmiValue.select, menuData.value / MINUNITMULT); }
  void setHomeOffsetX() { hmiValue.select = X_AXIS; setPFloatOnClick(-50, 50, UNITFDIGITS, applyHomeOffset); }
  void setHomeOffsetY() { hmiValue.select = Y_AXIS; setPFloatOnClick(-50, 50, UNITFDIGITS, applyHomeOffset); }
  void setHomeOffsetZ() { hmiValue.select = Z_AXIS; setPFloatOnClick( -2,  2, UNITFDIGITS, applyHomeOffset); }
#endif

#if HAS_BED_PROBE
  void setProbeOffsetX() { setPFloatOnClick(-60, 60, UNITFDIGITS, TERN(PROUI_EX, proUIEx.applyPhySet, nullptr)); }
  void setProbeOffsetY() { setPFloatOnClick(-60, 60, UNITFDIGITS, TERN(PROUI_EX, proUIEx.applyPhySet, nullptr)); }
  void setProbeOffsetZ() { setPFloatOnClick(-10, 10, 2); }

  #if PROUI_EX
    void setProbeZSpeed()  { setPIntOnClick(60, Z_PROBE_FEEDRATE_FAST); }
    void applyProbeMultiple() { PRO_data.multiple_probing = (menuData.value > 1) ? menuData.value : 0; }
    void setProbeMultiple()  { setIntOnClick(0, 5, PRO_data.multiple_probing, applyProbeMultiple); }
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    void probeTest() {
      LCD_MESSAGE(MSG_M48_TEST);
      queue.inject(F("M48 P10"));
    }
  #endif

  void probeStow() { probe.stow(); }
  void probeDeploy() { probe.deploy(); }

  #if HAS_BLTOUCH_HS_MODE
    void setHSMode() { toggleCheckboxLine(bltouch.high_speed_mode); }
  #endif

#endif

#if PROUI_EX && ENABLED(NOZZLE_PARK_FEATURE)
  void setParkPosX()   { setPIntOnClick(X_MIN_POS, X_MAX_POS); }
  void setParkPosY()   { setPIntOnClick(Y_MIN_POS, Y_MAX_POS); }
  void setParkZRaise() { setPIntOnClick(Z_MIN_POS, 50); }
#endif

#if HAS_FILAMENT_SENSOR
  void setRunoutEnable() {
    runout.reset();
    toggleCheckboxLine(runout.enabled);
  }

  #if HAS_PROUI_RUNOUT_SENSOR
    void liveRunoutActive() { proUIEx.drawRunoutActive(true); }
    void setRunoutActive() {
      uint8_t val;
      val = PRO_data.FilamentMotionSensor ? 2 : PRO_data.Runout_active_state ? 1 : 0;
      setOnClick(ID_SetIntNoDraw, 0, 2, 0, val, proUIEx.applyRunoutActive, liveRunoutActive);
      proUIEx.drawRunoutActive(true);
    }
  #endif

  #if HAS_FILAMENT_RUNOUT_DISTANCE
    void applyRunoutDistance() { runout.set_runout_distance(menuData.value / MINUNITMULT); }
    void setRunoutDistance() { setFloatOnClick(0, 999, UNITFDIGITS, runout.runout_distance(), applyRunoutDistance); }
  #endif
#endif

#if ENABLED(CONFIGURE_FILAMENT_CHANGE)
  void setFilLoad()   { setPFloatOnClick(0, EXTRUDE_MAXLENGTH, UNITFDIGITS); }
  void setFilUnload() { setPFloatOnClick(0, EXTRUDE_MAXLENGTH, UNITFDIGITS); }
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  void applyExtMinT() { thermalManager.extrude_min_temp = hmiData.extMinT; thermalManager.allow_cold_extrude = (hmiData.extMinT == 0); }
  void setExtMinT() { setPIntOnClick(MIN_ETEMP, MAX_ETEMP, applyExtMinT); }
#endif

#if HAS_FEEDRATE_EDIT
  void setSpeed() { setPIntOnClick(SPEED_EDIT_MIN, SPEED_EDIT_MAX); }
#endif

#if HAS_HOTEND
  void applyHotendTemp() { thermalManager.setTargetHotend(menuData.value, 0); }
  void setHotendTemp() { setIntOnClick(MIN_ETEMP, MAX_ETEMP, thermalManager.degTargetHotend(EXT), applyHotendTemp); }
#endif

#if HAS_HEATED_BED
  void applyBedTemp() { thermalManager.setTargetBed(menuData.value); }
  void setBedTemp() { setIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP, thermalManager.degTargetBed(), applyBedTemp); }
#endif

#if HAS_FAN
  void applyFanSpeed() { thermalManager.set_fan_speed(0, menuData.value); }
  void setFanSpeed() { setIntOnClick(0, 255, thermalManager.fan_speed[FAN], applyFanSpeed); }
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void changeFilament() {
    hmiSaveProcessID(ID_NothingToDo);
    queue.inject(F("M600 B2"));
  }

  #if ENABLED(NOZZLE_PARK_FEATURE)
    void parkHead() {
      LCD_MESSAGE(MSG_FILAMENT_PARK_ENABLED);
      queue.inject(F("G28O\nG27"));
    }
  #endif

  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    void unloadFilament() {
      LCD_MESSAGE(MSG_FILAMENTUNLOAD);
      queue.inject(F("M702 Z20"));
    }

    void loadFilament() {
      LCD_MESSAGE(MSG_FILAMENTLOAD);
      queue.inject(F("M701 Z20"));
    }
  #endif

#endif // ADVANCED_PAUSE_FEATURE

#if HAS_FLOW_EDIT
  void setFlow() { setPIntOnClick(FLOW_EDIT_MIN, FLOW_EDIT_MAX, []{ planner.refresh_e_factor(EXT); }); }
#endif

#if ENABLED(MESH_BED_LEVELING)

  void manualMeshStart() {
    LCD_MESSAGE(MSG_UBL_BUILD_MESH_MENU);
    gcode.process_subcommands_now(F("G28XYO\nG28Z\nM211S0\nG29S1"));
    #ifdef MANUAL_PROBE_START_Z
      const int8_t line = currentMenu->line(mMeshMoveZItem);
      DWINUI::drawSignedFloat(hmiData.colorText, hmiData.colorBackground, 3, 2, VALX - 2 * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(line), MANUAL_PROBE_START_Z);
    #endif
  }

  void liveMeshMoveZ() {
    *menuData.floatPtr = menuData.value / POW(10, 2);
    if (!planner.is_full()) {
      planner.synchronize();
      planner.buffer_line(current_position, manual_feedrate_mm_s[Z_AXIS]);
    }
  }
  void setMMeshMoveZ() { setPFloatOnClick(-1, 1, 2, planner.synchronize, liveMeshMoveZ); }

  void manualMeshContinue() {
    gcode.process_subcommands_now(F("G29S2"));
    redrawItem(mMeshMoveZItem);
  }

  void manualMeshSave() {
    LCD_MESSAGE(MSG_UBL_STORAGE_MESH_MENU);
    queue.inject(F("M211S1\nM500"));
  }

#endif // MESH_BED_LEVELING

#if HAS_PREHEAT
  #if HAS_HOTEND
    void setPreheatEndTemp() { setPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif
  #if HAS_HEATED_BED
    void setPreheatBedTemp() { setPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif
  #if HAS_FAN
    void setPreheatFanSpeed() { setPIntOnClick(0, 255); }
  #endif
#endif

void applyMaxSpeed() { planner.set_max_feedrate((AxisEnum)hmiValue.select, menuData.value / MINUNITMULT); }
#if HAS_X_AXIS
  void setMaxSpeedX() { hmiValue.select = X_AXIS, setFloatOnClick(min_feedrate_edit_values.x, max_feedrate_edit_values.x, UNITFDIGITS, planner.settings.max_feedrate_mm_s[X_AXIS], applyMaxSpeed); }
#endif
#if HAS_Y_AXIS
  void setMaxSpeedY() { hmiValue.select = Y_AXIS, setFloatOnClick(min_feedrate_edit_values.y, max_feedrate_edit_values.y, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Y_AXIS], applyMaxSpeed); }
#endif
#if HAS_Z_AXIS
  void setMaxSpeedZ() { hmiValue.select = Z_AXIS, setFloatOnClick(min_feedrate_edit_values.z, max_feedrate_edit_values.z, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Z_AXIS], applyMaxSpeed); }
#endif
#if HAS_HOTEND
  void setMaxSpeedE() { hmiValue.select = E_AXIS; setFloatOnClick(min_feedrate_edit_values.e, max_feedrate_edit_values.e, UNITFDIGITS, planner.settings.max_feedrate_mm_s[E_AXIS], applyMaxSpeed); }
#endif

void applyMaxAccel() { planner.set_max_acceleration((AxisEnum)hmiValue.select, menuData.value); }
#if HAS_X_AXIS
  void setMaxAccelX() { hmiValue.select = X_AXIS, setIntOnClick(min_acceleration_edit_values.x, max_acceleration_edit_values.x, planner.settings.max_acceleration_mm_per_s2[X_AXIS], applyMaxAccel); }
#endif
#if HAS_Y_AXIS
  void setMaxAccelY() { hmiValue.select = Y_AXIS, setIntOnClick(min_acceleration_edit_values.y, max_acceleration_edit_values.y, planner.settings.max_acceleration_mm_per_s2[Y_AXIS], applyMaxAccel); }
#endif
#if HAS_Z_AXIS
  void setMaxAccelZ() { hmiValue.select = Z_AXIS, setIntOnClick(min_acceleration_edit_values.z, max_acceleration_edit_values.z, planner.settings.max_acceleration_mm_per_s2[Z_AXIS], applyMaxAccel); }
#endif
#if HAS_HOTEND
  void setMaxAccelE() { hmiValue.select = E_AXIS; setIntOnClick(min_acceleration_edit_values.e, max_acceleration_edit_values.e, planner.settings.max_acceleration_mm_per_s2[E_AXIS], applyMaxAccel); }
#endif

#if ENABLED(CLASSIC_JERK)
  void applyMaxJerk() { planner.set_max_jerk((AxisEnum)hmiValue.select, menuData.value / MINUNITMULT); }
  #if HAS_X_AXIS
    void setMaxJerkX() { hmiValue.select = X_AXIS, setFloatOnClick(min_jerk_edit_values.x, max_jerk_edit_values.x, UNITFDIGITS, planner.max_jerk.x, applyMaxJerk); }
  #endif
  #if HAS_Y_AXIS
    void setMaxJerkY() { hmiValue.select = Y_AXIS, setFloatOnClick(min_jerk_edit_values.y, max_jerk_edit_values.y, UNITFDIGITS, planner.max_jerk.y, applyMaxJerk); }
  #endif
  #if HAS_Z_AXIS
    void setMaxJerkZ() { hmiValue.select = Z_AXIS, setFloatOnClick(min_jerk_edit_values.z, max_jerk_edit_values.z, UNITFDIGITS, planner.max_jerk.z, applyMaxJerk); }
  #endif
  #if HAS_HOTEND
    void setMaxJerkE() { hmiValue.select = E_AXIS; setFloatOnClick(min_jerk_edit_values.e, max_jerk_edit_values.e, UNITFDIGITS, planner.max_jerk.e, applyMaxJerk); }
  #endif
#elif HAS_JUNCTION_DEVIATION
  void applyJDmm() { TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk()); }
  void setJDmm() { setPFloatOnClick(MIN_JD_MM, MAX_JD_MM, 3, applyJDmm); }
#endif

#if ENABLED(LIN_ADVANCE)
  void setLA_K() { setPFloatOnClick(0, 10, 3); }
#endif

#if HAS_X_AXIS
  void setStepsX() { hmiValue.select = X_AXIS, setPFloatOnClick( min_steps_edit_values.x, max_steps_edit_values.x, UNITFDIGITS); }
#endif
#if HAS_Y_AXIS
  void setStepsY() { hmiValue.select = Y_AXIS, setPFloatOnClick( min_steps_edit_values.y, max_steps_edit_values.y, UNITFDIGITS); }
#endif
#if HAS_Z_AXIS
  void setStepsZ() { hmiValue.select = Z_AXIS, setPFloatOnClick( min_steps_edit_values.z, max_steps_edit_values.z, UNITFDIGITS); }
#endif
#if HAS_HOTEND
  void setStepsE() { hmiValue.select = E_AXIS; setPFloatOnClick( min_steps_edit_values.e, max_steps_edit_values.e, UNITFDIGITS); }
#endif

#if PROUI_EX
  void setBedSizeX() { hmiValue.select = NO_AXIS_ENUM, setPIntOnClick(X_BED_MIN, X_MAX_POS, proUIEx.applyPhySet); }
  void setBedSizeY() { hmiValue.select = NO_AXIS_ENUM, setPIntOnClick(Y_BED_MIN, Y_MAX_POS, proUIEx.applyPhySet); }
  void setMinPosX()  { hmiValue.select = X_AXIS,       setPIntOnClick(     -100,       100, proUIEx.applyPhySet); }
  void setMinPosY()  { hmiValue.select = Y_AXIS,       setPIntOnClick(     -100,       100, proUIEx.applyPhySet); }
  void setMaxPosX()  { hmiValue.select = X_AXIS,       setPIntOnClick(X_BED_MIN,       999, proUIEx.applyPhySet); }
  void setMaxPosY()  { hmiValue.select = Y_AXIS,       setPIntOnClick(Y_BED_MIN,       999, proUIEx.applyPhySet); }
  void setMaxPosZ()  { hmiValue.select = Z_AXIS,       setPIntOnClick(      100,       999, proUIEx.applyPhySet); }
#endif

#if ALL(PROUI_EX, HAS_EXTRUDERS)
  void setInvertE0() {
    stepper.disable_e_steppers();
    toggleCheckboxLine(PRO_data.Invert_E0);
    current_position.e = 0;
    sync_plan_position_e();
  }
#endif

#if ENABLED(FWRETRACT)
  void doRetract() { 
    current_position.e-=fwretract.settings.retract_length;
    axisMove(E_AXIS);
  }
  void setRetractLength() { setPFloatOnClick( 0, 10, UNITFDIGITS); }
  void setRetractSpeed() { setPFloatOnClick( 1, 90, UNITFDIGITS); }
  void setZRaise() { setPFloatOnClick( 0, 2, 2); }
  void setRecoverSpeed() { setPFloatOnClick( 1, 90, UNITFDIGITS); }
  void setAddRecover() { setPFloatOnClick(-5, 5, UNITFDIGITS); }
#endif

#if HAS_TOOLBAR
  void liveTBSetupItem() {
    updateTBSetupItem(currentMenu->selected, menuData.value);
    drawTBSetupItem(true);
  }
  void applyTBSetupItem() {
    drawTBSetupItem(false);
    if (currentMenu->selectedItem().icon) {
      uint8_t *Pint = (uint8_t *)currentMenu->selectedItem().value;
      *Pint = menuData.value;
    }
  }
  void setTBSetupItem() {
    const uint8_t val = *(uint8_t *)currentMenu->selectedItem().value;
    setOnClick(ID_SetIntNoDraw, 0, toolBar.OptCount() - 1, 0, val, applyTBSetupItem, liveTBSetupItem);
    drawTBSetupItem(true);
  }
  void onDrawTBSetupItem(int8_t pos, int8_t line) {
    uint8_t val = *(uint8_t *)getMenuItem(pos)->value;
    updateTBSetupItem(pos, val);
    onDrawMenuItem(pos, line);
  }
#endif // HAS_TOOLBAR

// Special Menuitem Drawing functions =================================================

void onDrawSelColorItem(int8_t pos, int8_t line) {
  const uint16_t color = *(uint16_t*)getMenuItem(pos)->value;
  dwinDrawRectangle(0, hmiData.colorHighlight, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  dwinDrawRectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  onDrawMenuItem(pos, line);
}

void onDrawGetColorItem(int8_t pos, int8_t line) {
  const uint8_t i = getMenuItem(pos)->icon;
  uint16_t color;
  switch (i) {
    case 0: color = RGB(31, 0, 0); break; // Red
    case 1: color = RGB(0, 63, 0); break; // Green
    case 2: color = RGB(0, 0, 31); break; // Blue
    default: color = 0; break;
  }
  dwinDrawRectangle(0, hmiData.colorHighlight, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  dwinDrawRectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  DWINUI::drawString(LBLX, MBASE(line) - 1, getMenuItem(pos)->caption);
  drawMenuIntValue(hmiData.colorBackground, line, 4, hmiValue.Color[i]);
  dwinDrawHLine(hmiData.colorSplitLine, 16, MYPOS(line + 1), 240);
}

#if HAS_PROUI_RUNOUT_SENSOR
  void ondrawRunoutActive(int8_t pos, int8_t line) {
    onDrawMenuItem(pos, line);
    if (PRO_data.FilamentMotionSensor)
      DWINUI::drawString(VALX - 8, MBASE(line), GET_TEXT_F(MSG_MOTION));
    else
      DWINUI::drawString(VALX + 8, MBASE(line), PRO_data.Runout_active_state ? GET_TEXT_F(MSG_HIGH) : GET_TEXT_F(MSG_LOW));
  }
#endif

// Menu Creation and Drawing functions ======================================================

void returnToPreviousMenu() {
  #if ENABLED(CV_LASER_MODULE)
    if (previousMenu == laserPrintMenu) return drawLaserPrintMenu();
  #endif
  if (previousMenu == advancedSettings) return drawAdvancedSettingsMenu();
  if (previousMenu == filSetMenu) return drawFilSetMenu();
  if (previousMenu == tuneMenu) return drawTuneMenu();
  if (previousMenu == fileMenu) return drawFileMenu();
}

void drawPrepareMenu() {
  if (notCurrentMenu(prepareMenu)) {
    BACK_ITEM(gotoMainMenu);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENT_MAN, onDrawSubMenu, drawFilamentManMenu);
    #endif
    MENU_ITEM(ICON_Axis, MSG_MOVE_AXIS, onDrawSubMenu, drawMoveMenu);
    #if ENABLED(LCD_BED_TRAMMING)
      MENU_ITEM(ICON_Tram, MSG_BED_TRAMMING, onDrawSubMenu, drawTrammingMenu);
    #endif
    MENU_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, onDrawMenuItem, disableMotors);
    #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
      MENU_ITEM(ICON_Homing, MSG_HOMING, onDrawSubMenu, drawHomingMenu);
    #else
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
    #endif
    #if ENABLED(MESH_BED_LEVELING)
      MENU_ITEM(ICON_ManualMesh, MSG_MANUAL_MESH, onDrawSubMenu, drawManualMeshMenu);
    #elif HAS_BED_PROBE
      MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, autoLev);
    #endif
    #if HAS_ZOFFSET_ITEM
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, onDrawSubMenu, drawZOffsetWizMenu);
      #elif ENABLED(BABYSTEPPING)
        EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
      #endif
    #endif
    #if HAS_PREHEAT
      #define _ITEM_PREHEAT(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheat##N);
      REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT)
    #endif
    MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawMenuItem, doCoolDown);
  }
  SET_MENU(prepareMenu, MSG_PREPARE);
  ui.reset_status(true);
}

#if ENABLED(LCD_BED_TRAMMING)

  void setManualTramming() {
    toggleCheckboxLine(hmiData.fullManualTramming);
  }

  #if ALL(HAS_BED_PROBE, HAS_TRAMMING_WIZARD)
    void runTrammingWizard() {
      meshViewer.meshfont = font8x16;
      trammingWizard();
      meshViewer.meshfont = TERN(TJC_DISPLAY, font8x16, font6x12);
    }
  #endif

  void drawTrammingMenu() {
    if (notCurrentMenu(trammingMenu)) {
      BACK_ITEM(drawPrepareMenu);
      #if HAS_BED_PROBE
        #if HAS_TRAMMING_WIZARD
          MENU_ITEM(ICON_ProbeSet, MSG_TRAMMING_WIZARD, onDrawMenuItem, runTrammingWizard);
        #endif
        EDIT_ITEM(ICON_ProbeSet, MSG_BED_TRAMMING_MANUAL, onDrawChkbMenu, setManualTramming, &hmiData.fullManualTramming);
      #else
        MENU_ITEM(ICON_MoveZ0, MSG_HOME_Z_AND_DISABLE, onDrawMenuItem, homeZandDisable);
      #endif
      MENU_ITEM(ICON_Axis, MSG_TRAM_FL, onDrawMenuItem, []{ tram(LF); });
      MENU_ITEM(ICON_Axis, MSG_TRAM_FR, onDrawMenuItem, []{ tram(RF); });
      MENU_ITEM(ICON_Axis, MSG_TRAM_BR, onDrawMenuItem, []{ tram(RB); });
      MENU_ITEM(ICON_Axis, MSG_TRAM_BL, onDrawMenuItem, []{ tram(LB); });
      #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
        MENU_ITEM(ICON_Axis, MSG_TRAM_C , onDrawMenuItem, []{ tram(TRAM_C); } );
      #endif
    }
    SET_MENU(trammingMenu, MSG_BED_TRAMMING);
  }

#endif

void drawControlMenu() {
  if (notCurrentMenu(controlMenu)) {
    BACK_ITEM(gotoMainMenu);
    MENU_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawSubMenu, drawTemperatureMenu);
    MENU_ITEM(ICON_Motion, MSG_MOTION, onDrawSubMenu, drawMotionMenu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      MENU_ITEM(ICON_ReadEEPROM, MSG_LOAD_EEPROM, onDrawMenuItem, readEeprom);
      MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawMenuItem, resetEeprom);
    #endif
    MENU_ITEM(ICON_Reboot, MSG_RESET_PRINTER, onDrawMenuItem, rebootPrinter);
    MENU_ITEM(ICON_Info, MSG_INFO_SCREEN, onDrawSubMenu, gotoInfoMenu);
  }
  SET_MENU(controlMenu, MSG_CONTROL);
  ui.reset_status(true);
}

void drawAdvancedSettingsMenu() {
  if (notCurrentMenu(advancedSettings)) {
    BACK_ITEM(gotoMainMenu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
    #endif
    #if ENABLED(CV_LASER_MODULE)
      MENU_ITEM(ICON_LaserSet, MSG_LASER_MENU, onDrawSubMenu, drawLaserSettingsMenu);
    #endif
    #if HAS_MESH
      MENU_ITEM(ICON_ProbeSet, MSG_MESH_LEVELING, onDrawSubMenu, drawMeshSetMenu);
    #endif
    #if HAS_BED_PROBE
      MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, drawProbeSetMenu);
    #endif
    MENU_ITEM(ICON_FilSet, MSG_FILAMENT_SET, onDrawSubMenu, drawFilSetMenu);
    #if ENABLED(PIDTEMP) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM(ICON_PIDNozzle, MSG_HOTEND_PID_SETTINGS, onDrawSubMenu, drawHotendPIDMenu);
    #endif
    #if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
      MENU_ITEM(ICON_MPCNozzle, MSG_MPC_SETTINGS, onDrawSubMenu, drawHotendMPCMenu);
    #endif
    #if ENABLED(PIDTEMPBED) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM(ICON_PIDBed, MSG_BED_PID_SETTINGS, onDrawSubMenu, drawBedPIDMenu);
    #endif
    #if PROUI_EX
      MENU_ITEM(ICON_PhySet, MSG_PHY_SET, onDrawSubMenu, drawPhySetMenu);
    #endif
    #if HAS_TRINAMIC_CONFIG
      MENU_ITEM(ICON_TMCSet, MSG_TMC_DRIVERS, onDrawSubMenu, drawTrinamicConfigMenu);
    #endif
    #if HAS_TOOLBAR
      MENU_ITEM(ICON_TBSetup, MSG_TOOLBAR_SETUP, onDrawSubMenu, drawTBSetupMenu);
    #endif
    #if HAS_ESDIAG
      MENU_ITEM(ICON_ESDiag, MSG_ENDSTOP_DIAGNOSTICS, onDrawSubMenu, drawEndStopDiag);
    #endif
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(ICON_PrintStats, MSG_INFO_STATS_MENU, onDrawSubMenu, gotoPrintStats);
      MENU_ITEM(ICON_PrintStatsReset, MSG_INFO_PRINT_COUNT_RESET, onDrawSubMenu, printStatsReset);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, dwinLockScreen);
    #endif
    #if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
      MENU_ITEM(ICON_Host, MSG_HOST_SHUTDOWN, onDrawMenuItem, hostShutDown);
    #endif
    #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_SOUND_ENABLE, onDrawChkbMenu, setEnableSound, &ui.sound_on);
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, setPwrLossr, &recovery.enabled);
    #endif
    #if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
      EDIT_ITEM(ICON_File, MSG_HAS_PREVIEW, onDrawChkbMenu, setPreview, &hmiData.enablePreview);
    #endif
    #if ENABLED(MEDIASORT_MENU_ITEM)
      EDIT_ITEM(ICON_File, MSG_MEDIA_SORT, onDrawChkbMenu, setMediaSort, &hmiData.mediaSort);
    #endif
    #if DISABLED(HAS_SD_EXTENDER)
      EDIT_ITEM(ICON_File, MSG_MEDIA_UPDATE, onDrawChkbMenu, setMediaAutoMount, &hmiData.mediaAutoMount);
    #endif
    #if ENABLED(BAUD_RATE_GCODE)
      EDIT_ITEM(ICON_SetBaudRate, MSG_115K_BAUD, onDrawChkbMenu, setBaudRate, &hmiData.baud115K);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, setBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, turnOffBacklight);
    #endif
    #if ALL(HAS_CUSTOM_COLORS , HAS_CUSTOM_COLORS_MENU)
      MENU_ITEM(ICON_Scolor, MSG_COLORS_SELECT, onDrawSubMenu, drawSelectColorsMenu);
    #endif
  }
  SET_MENU(advancedSettings, MSG_ADVANCED_SETTINGS);
  ui.reset_status(true);
}

void drawMoveMenu() {
  if (notCurrentMenu(moveMenu)) {
    BACK_ITEM(drawPrepareMenu);
    EDIT_ITEM(ICON_Axis, MSG_LIVE_MOVE, onDrawChkbMenu, setLiveMove, &enableLiveMove);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, setMoveX, &current_position.x);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, setMoveY, &current_position.y);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MoveZ, MSG_MOVE_Z, onDrawPFloatMenu, setMoveZ, &current_position.z);
    #endif
    #if HAS_HOTEND
      gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
      EDIT_ITEM(ICON_Extruder, MSG_MOVE_E, onDrawPFloatMenu, setMoveE, &current_position.e);
      MENU_ITEM(ICON_Extruder, MSG_MOVE_E_100, onDrawMenuItem, moveE100);
    #endif
  }
  SET_MENU(moveMenu, MSG_MOVE_AXIS);
  if (!all_axes_trusted()) LCD_MESSAGE(MSG_POSITION_IS_UNKNOWN);
}

#if HAS_HOME_OFFSET

  void drawHomeOffsetMenu() {
    if (notCurrentMenu(homeOffMenu)) {
      BACK_ITEM(drawPhySetMenu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_HomeOffsetX, MSG_HOME_OFFSET_X, onDrawPFloatMenu, setHomeOffsetX, &home_offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_HomeOffsetY, MSG_HOME_OFFSET_Y, onDrawPFloatMenu, setHomeOffsetY, &home_offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_HomeOffsetZ, MSG_HOME_OFFSET_Z, onDrawPFloatMenu, setHomeOffsetZ, &home_offset.z);
      #endif
    }
    SET_MENU(homeOffMenu, MSG_SET_HOME_OFFSETS);
  }

#endif // HAS_HOME_OFFSET

#if HAS_BED_PROBE

  void drawProbeSetMenu() {
    if (notCurrentMenu(probeSetMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_ProbeOffsetX, MSG_ZPROBE_XOFFSET, onDrawPFloatMenu, setProbeOffsetX, &probe.offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_ProbeOffsetY, MSG_ZPROBE_YOFFSET, onDrawPFloatMenu, setProbeOffsetY, &probe.offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_ProbeOffsetZ, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, setProbeOffsetZ, &probe.offset.z);
      #endif
      #if PROUI_EX
        EDIT_ITEM(ICON_ProbeZSpeed, MSG_Z_FEED_RATE, onDrawPIntMenu, setProbeZSpeed, &PRO_data.zprobefeedslow);
        EDIT_ITEM(ICON_ProbeMultiple, MSG_ZPROBE_MULTIPLE, onDrawPInt8Menu, setProbeMultiple, &PRO_data.multiple_probing);
      #endif
      #if ENABLED(BLTOUCH)
        MENU_ITEM(ICON_ProbeStow, MSG_MANUAL_STOW, onDrawMenuItem, probeStow);
        MENU_ITEM(ICON_ProbeDeploy, MSG_MANUAL_DEPLOY, onDrawMenuItem, probeDeploy);
        MENU_ITEM(ICON_BltouchReset, MSG_BLTOUCH_RESET, onDrawMenuItem, bltouch._reset);
        #if HAS_BLTOUCH_HS_MODE
          EDIT_ITEM(ICON_HSMode, MSG_ENABLE_HS_MODE, onDrawChkbMenu, setHSMode, &bltouch.high_speed_mode);
        #endif
      #endif
      #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
        MENU_ITEM(ICON_ProbeTest, MSG_M48_TEST, onDrawMenuItem, probeTest);
      #endif
    }
    SET_MENU(probeSetMenu, MSG_ZPROBE_SETTINGS);
  }

#endif // HAS_BED_PROBE

void drawFilSetMenu() {
  if (notCurrentMenu(filSetMenu)) {
    BACK_ITEM(drawAdvancedSettingsMenu);
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, setRunoutEnable, &runout.enabled);
      #if HAS_PROUI_RUNOUT_SENSOR
        MENU_ITEM(ICON_Runout, MSG_RUNOUT_ACTIVE, ondrawRunoutActive, setRunoutActive);
      #endif
      #if HAS_FILAMENT_RUNOUT_DISTANCE
        EDIT_ITEM(ICON_Runout, MSG_RUNOUT_DISTANCE_MM, onDrawPFloatMenu, setRunoutDistance, &runout.runout_distance());
      #endif
    #endif
    #if ALL(PROUI_EX, HAS_EXTRUDERS)
      EDIT_ITEM(ICON_InvertE0, MSG_INVERT_EXTRUDER, onDrawChkbMenu, setInvertE0, &PRO_data.Invert_E0);
    #endif
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      EDIT_ITEM(ICON_ExtrudeMinT, MSG_EXTRUDER_MIN_TEMP, onDrawPIntMenu, setExtMinT, &hmiData.extMinT);
    #endif
    #if ENABLED(CONFIGURE_FILAMENT_CHANGE)
      EDIT_ITEM(ICON_FilLoad, MSG_FILAMENT_LOAD, onDrawPFloatMenu, setFilLoad, &fc_settings[EXT].load_length);
      EDIT_ITEM(ICON_FilUnload, MSG_FILAMENT_UNLOAD, onDrawPFloatMenu, setFilUnload, &fc_settings[EXT].unload_length);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, drawFWRetractMenu);
    #endif
  }
  SET_MENU(filSetMenu, MSG_FILAMENT_SET);
}

#if PROUI_EX
  #if ENABLED(NOZZLE_PARK_FEATURE)
    void drawParkPosMenu() {
      if (notCurrentMenu(parkPosMenu)) {
        BACK_ITEM(drawPhySetMenu);
        EDIT_ITEM(ICON_ParkPosX, MSG_PARK_XPOSITION, onDrawPIntMenu, setParkPosX, &PRO_data.Park_point.x);
        EDIT_ITEM(ICON_ParkPosY, MSG_PARK_YPOSITION, onDrawPIntMenu, setParkPosY, &PRO_data.Park_point.y);
        EDIT_ITEM(ICON_ParkPosZ, MSG_PARK_ZRAISE, onDrawPIntMenu, setParkZRaise, &PRO_data.Park_point.z);
      }
      SET_MENU(parkPosMenu, MSG_FILAMENT_PARK_ENABLED);
    }
  #endif

  void drawPhySetMenu() {
    if (notCurrentMenu(phySetMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMINPOS, onDrawPIntMenu, setMinPosX, &PRO_data.x_min_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMINPOS, onDrawPIntMenu, setMinPosY, &PRO_data.y_min_pos);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMAXPOS, onDrawPIntMenu, setMaxPosX, &PRO_data.x_max_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMAXPOS, onDrawPIntMenu, setMaxPosY, &PRO_data.y_max_pos);
      EDIT_ITEM(ICON_MaxPosZ, MSG_PHY_ZMAXPOS, onDrawPIntMenu, setMaxPosZ, &PRO_data.z_max_pos);
      EDIT_ITEM(ICON_BedSizeX, MSG_PHY_XBEDSIZE, onDrawPIntMenu, setBedSizeX, &PRO_data.x_bed_size);
      EDIT_ITEM(ICON_BedSizeY, MSG_PHY_YBEDSIZE, onDrawPIntMenu, setBedSizeY, &PRO_data.y_bed_size);
      #if HAS_HOME_OFFSET
        MENU_ITEM(ICON_HomeOffset, MSG_SET_HOME_OFFSETS, onDrawSubMenu, drawHomeOffsetMenu);
      #endif
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_ParkPos, MSG_FILAMENT_PARK_ENABLED, onDrawSubMenu, drawParkPosMenu);
      #endif
    }
    SET_MENU(phySetMenu, MSG_PHY_SET);
  }
#endif

#if ALL(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)

  void drawCaseLightMenu() {
    if (notCurrentMenu(caseLightMenu)) {
      BACK_ITEM(drawControlMenu);
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, setCaseLight, &caselight.on);
      EDIT_ITEM(ICON_Brightness, MSG_CASE_LIGHT_BRIGHTNESS, onDrawPInt8Menu, setCaseLightBrightness, &caselight.brightness);
    }
    SET_MENU(caseLightMenu, MSG_CASE_LIGHT);
  }

#endif

#if ENABLED(LED_CONTROL_MENU)

  void drawLedControlMenu () {
    if (notCurrentMenu(ledControlMenu)) {
      BACK_ITEM(drawControlMenu);
      #if !ALL(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
        EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, setLedStatus, &leds.lights_on);
      #endif
      #if HAS_COLOR_LEDS
        #if ENABLED(LED_COLOR_PRESETS)
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_WHITE, onDrawMenuItem,  leds.set_white);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_RED, onDrawMenuItem,    leds.set_red);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_ORANGE, onDrawMenuItem, leds.set_orange);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_YELLOW, onDrawMenuItem, leds.set_yellow);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_GREEN, onDrawMenuItem,  leds.set_green);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_BLUE, onDrawMenuItem,   leds.set_blue);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_INDIGO, onDrawMenuItem, leds.set_indigo);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_VIOLET, onDrawMenuItem, leds.set_violet);
        #else
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_RED, onDrawPInt8Menu, setLEDColorR, &leds.color.r);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_GREEN, onDrawPInt8Menu, setLEDColorG, &leds.color.g);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_BLUE, onDrawPInt8Menu, setLEDColorB, &leds.color.b);
          #if ENABLED(HAS_WHITE_LED)
            EDIT_ITEM(ICON_LedControl, MSG_COLORS_WHITE, onDrawPInt8Menu, setLEDColorW, &leds.color.w);
          #endif
        #endif
      #endif
    }
    SET_MENU(ledControlMenu, MSG_LED_CONTROL);
  }

#endif // LED_CONTROL_MENU

void drawTuneMenu() {
  #if ENABLED(CV_LASER_MODULE)
    if (laser_device.is_laser_device()) return LCD_MESSAGE(MSG_LASER_NOT_AVAILABLE);
  #endif
  if (notCurrentMenu(tuneMenu)) {
    BACK_ITEM(gotoPrintProcess);
    #if HAS_FEEDRATE_EDIT
      EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, setSpeed, &feedrate_percentage);
    #endif
    #if HAS_HOTEND
      hotendTargetItem = EDIT_ITEM(ICON_HotendTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, setHotendTemp, &thermalManager.temp_hotend[EXT].target);
    #endif
    #if HAS_HEATED_BED
      bedTargetItem = EDIT_ITEM(ICON_BedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      fanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, setFanSpeed, &thermalManager.fan_speed[FAN]);
    #endif
    #if ALL(HAS_ZOFFSET_ITEM, HAS_BED_PROBE, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
    #elif ALL(HAS_ZOFFSET_ITEM, MESH_BED_LEVELING, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
    #endif
    #if HAS_FLOW_EDIT
      EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, setFlow, &planner.flow_percentage[EXT]);
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, changeFilament);
    #endif
    #if ENABLED(RUNOUT_TUNE_ITEM)
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, setRunoutEnable, &runout.enabled);
    #endif
    #if ENABLED(PLR_TUNE_ITEM)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, setPwrLossr, &recovery.enabled);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, drawFWRetractMenu);
    #endif
    #if ENABLED(JD_TUNE_ITEM)
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, setJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(ADVK_TUNE_ITEM)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, setLA_K, &planner.extruder_advance_K[EXT]);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, dwinLockScreen);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, setBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, turnOffBacklight);
    #endif
    #if ENABLED(CASE_LIGHT_MENU)
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, setCaseLight, &caselight.on);
    #elif ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)
      EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, setLedStatus, &leds.lights_on);
    #endif
  }
  SET_MENU(tuneMenu, MSG_TUNE);
}

#if ENABLED(ADAPTIVE_STEP_SMOOTHING)
  void setAdaptiveStepSmoothing() {
    toggleCheckboxLine(hmiData.adaptiveStepSmoothing);
  }
#endif

#if ENABLED(SHAPING_MENU)
  void applyShapingFreq() { stepper.set_shaping_frequency((AxisEnum)hmiValue.select, menuData.value * 0.01); }
  void applyShapingZeta() { stepper.set_shaping_damping_ratio((AxisEnum)hmiValue.select, menuData.value * 0.01); }

  #if ENABLED(INPUT_SHAPING_X)
    void onDrawShapingXFreq(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 2, stepper.get_shaping_frequency(X_AXIS)); }
    void onDrawShapingXZeta(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 2, stepper.get_shaping_damping_ratio(X_AXIS)); }
    void setShapingXFreq() { hmiValue.select = X_AXIS; setFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(X_AXIS), applyShapingFreq); }
    void setShapingXZeta() { hmiValue.select = X_AXIS; setFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(X_AXIS), applyShapingZeta); }
  #endif

  #if ENABLED(INPUT_SHAPING_Y)
    void onDrawShapingYFreq(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 2, stepper.get_shaping_frequency(Y_AXIS)); }
    void onDrawShapingYZeta(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 2, stepper.get_shaping_damping_ratio(Y_AXIS)); }
    void setShapingYFreq() { hmiValue.select = Y_AXIS; setFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(Y_AXIS), applyShapingFreq); }
    void setShapingYZeta() { hmiValue.select = Y_AXIS; setFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(Y_AXIS), applyShapingZeta); }
  #endif

  void drawInputShaping_menu() {
    if (notCurrentMenu(inputShapingMenu)) {
      BACK_ITEM(drawMotionMenu);
      #if ENABLED(INPUT_SHAPING_X)
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_FREQ, onDrawShapingXFreq, setShapingXFreq);
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_ZETA, onDrawShapingXZeta, setShapingXZeta);
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_FREQ, onDrawShapingYFreq, setShapingYFreq);
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_ZETA, onDrawShapingYZeta, setShapingYZeta);
      #endif
    }
    SET_MENU(inputShapingMenu, MSG_INPUT_SHAPING);
  }
#endif

#if HAS_TRINAMIC_CONFIG
  #if AXIS_IS_TMC(X)
    void setXTMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperX.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Y)
    void setYTMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperY.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Z)
    void setZTMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperZ.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(E0)
    void setETMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperE0.refresh_stepper_current(); }); }
  #endif

  void drawTrinamicConfigMenu() {
    if (notCurrentMenu(trinamicConfigMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if AXIS_IS_TMC(X)
        EDIT_ITEM(ICON_TMCXSet, MSG_TMC_ACURRENT, onDrawPIntMenu, setXTMCCurrent, &stepperX.val_mA);
      #endif
      #if AXIS_IS_TMC(Y)
        EDIT_ITEM(ICON_TMCYSet, MSG_TMC_BCURRENT, onDrawPIntMenu, setYTMCCurrent, &stepperY.val_mA);
      #endif
      #if AXIS_IS_TMC(Z)
        EDIT_ITEM(ICON_TMCZSet, MSG_TMC_CCURRENT, onDrawPIntMenu, setZTMCCurrent, &stepperZ.val_mA);
      #endif
      #if AXIS_IS_TMC(E0)
        EDIT_ITEM(ICON_TMCESet, MSG_TMC_ECURRENT, onDrawPIntMenu, setETMCCurrent, &stepperE0.val_mA);
      #endif
    }
    SET_MENU(trinamicConfigMenu, MSG_TMC_DRIVERS);
  }
#endif

void drawMotionMenu() {
  if (notCurrentMenu(motionMenu)) {
    BACK_ITEM(drawControlMenu);
    MENU_ITEM(ICON_MaxSpeed, MSG_SPEED, onDrawSubMenu, drawMaxSpeedMenu);
    MENU_ITEM(ICON_MaxAccelerated, MSG_ACCELERATION, onDrawSubMenu, drawMaxAccelMenu);
    #if ENABLED(CLASSIC_JERK)
      MENU_ITEM(ICON_MaxJerk, MSG_JERK, onDrawSubMenu, drawMaxJerkMenu);
    #elif HAS_JUNCTION_DEVIATION
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, setJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(LIN_ADVANCE)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, setLA_K, &planner.extruder_advance_K[EXT]);
    #endif
    #if ENABLED(SHAPING_MENU)
      MENU_ITEM(ICON_InputShaping, MSG_INPUT_SHAPING, onDrawSubMenu, drawInputShaping_menu);
    #endif
    #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
      EDIT_ITEM(ICON_UBLActive, MSG_STEP_SMOOTHING, onDrawChkbMenu, setAdaptiveStepSmoothing, &hmiData.adaptiveStepSmoothing);
    #endif
    MENU_ITEM(ICON_Step, MSG_STEPS_PER_MM, onDrawSubMenu, drawStepsMenu);
    #if HAS_FLOW_EDIT
      EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, setFlow, &planner.flow_percentage[EXT]);
    #endif
    #if HAS_FEEDRATE_EDIT
      EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, setSpeed, &feedrate_percentage);
    #endif
  }
  SET_MENU(motionMenu, MSG_MOTION);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  #if HAS_PREHEAT
    void drawPreheatHotendMenu() {
      if (notCurrentMenu(preheatHotendMenu)) {
        BACK_ITEM(drawFilamentManMenu);
        #define _ITEM_PREHEAT_HE(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheatHotend##N);
        REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT_HE)
        MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawMenuItem, []{thermalManager.setTargetHotend(0, 0);});
      }
      SET_MENU(preheatHotendMenu, MSG_PREHEAT_HOTEND);
    }
  #endif

  void drawFilamentManMenu() {
    if (notCurrentMenu(filamentMenu)) {
      BACK_ITEM(drawPrepareMenu);
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_Park, MSG_FILAMENT_PARK_ENABLED, onDrawMenuItem, parkHead);
      #endif
      #if HAS_PREHEAT
        MENU_ITEM(ICON_SetEndTemp, MSG_PREHEAT_HOTEND, onDrawSubMenu, drawPreheatHotendMenu);
      #endif
      #if ENABLED(FWRETRACT)
        MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawMenuItem, doRetract);
      #endif
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, changeFilament);
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        MENU_ITEM(ICON_FilUnload, MSG_FILAMENTUNLOAD, onDrawMenuItem, unloadFilament);
        MENU_ITEM(ICON_FilLoad, MSG_FILAMENTLOAD, onDrawMenuItem, loadFilament);
      #endif
    }
    SET_MENU(filamentMenu, MSG_FILAMENT_MAN);
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(MESH_BED_LEVELING)

  void drawManualMeshMenu() {
    if (notCurrentMenu(manualMesh)) {
      BACK_ITEM(drawPrepareMenu);
      MENU_ITEM(ICON_ManualMesh, MSG_LEVEL_BED, onDrawMenuItem, manualMeshStart);
      mMeshMoveZItem = EDIT_ITEM(ICON_Zoffset, MSG_MOVE_Z, onDrawPFloat2Menu, setMMeshMoveZ, &current_position.z);
      MENU_ITEM(ICON_Axis, MSG_UBL_CONTINUE_MESH, onDrawMenuItem, manualMeshContinue);
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, dwinMeshViewer);
      MENU_ITEM(ICON_MeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, manualMeshSave);
    }
    SET_MENU(manualMesh, MSG_UBL_MANUAL_MESH);
  }

#endif // MESH_BED_LEVELING

#if HAS_PREHEAT

  void drawPreheatMenu(uint8_t N) {
    if (notCurrentMenu(preheatMenu)) {
      BACK_ITEM(drawTemperatureMenu);
      #if HAS_HOTEND
        EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, setPreheatEndTemp, &ui.material_preset[N].hotend_temp);
      #endif
      #if HAS_HEATED_BED
        EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setPreheatBedTemp, &ui.material_preset[N].bed_temp);
      #endif
      #if HAS_FAN
        EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPIntMenu, setPreheatFanSpeed, &ui.material_preset[N].fan_speed);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      #endif
    }
  }

  #define _PreheatMenu(N) \
    void drawPreheat## N ##Menu() { \
      drawPreheatMenu((N) - 1); \
      SET_MENU(preheatMenu, MSG_PREHEAT_## N ##_SETTINGS); \
    }
  REPEAT_1(PREHEAT_COUNT, _PreheatMenu)

#endif // HAS_PREHEAT

void drawTemperatureMenu() {
  if (notCurrentMenu(temperatureMenu)) {
    BACK_ITEM(drawControlMenu);
    #if HAS_HOTEND
      hotendTargetItem = EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, setHotendTemp, &thermalManager.temp_hotend[EXT].target);
    #endif
    #if HAS_HEATED_BED
      bedTargetItem = EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      fanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, setFanSpeed, &thermalManager.fan_speed[FAN]);
    #endif
    #if HAS_PREHEAT
      #define _ITEM_SETPREHEAT(N) MENU_ITEM(ICON_SetPreheat##N, MSG_PREHEAT_## N ##_SETTINGS, onDrawSubMenu, drawPreheat## N ##Menu);
      REPEAT_1(PREHEAT_COUNT, _ITEM_SETPREHEAT)
    #endif
  }
  SET_MENU(temperatureMenu, MSG_TEMPERATURE);
}

void drawMaxSpeedMenu() {
  checkkey = ID_Menu;
  if (notCurrentMenu(maxSpeedMenu)) {
    BACK_ITEM(drawMotionMenu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxSpeedX, MSG_VMAX_A, onDrawPFloatMenu, setMaxSpeedX, &planner.settings.max_feedrate_mm_s[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxSpeedY, MSG_VMAX_B, onDrawPFloatMenu, setMaxSpeedY, &planner.settings.max_feedrate_mm_s[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxSpeedZ, MSG_VMAX_C, onDrawPFloatMenu, setMaxSpeedZ, &planner.settings.max_feedrate_mm_s[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxSpeedE, MSG_VMAX_E, onDrawPFloatMenu, setMaxSpeedE, &planner.settings.max_feedrate_mm_s[E_AXIS]);
    #endif
  }
  SET_MENU(maxSpeedMenu, MSG_MAX_SPEED);
}

void drawMaxAccelMenu() {
  if (notCurrentMenu(maxAccelMenu)) {
    BACK_ITEM(drawMotionMenu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxAccX, MSG_AMAX_A, onDrawPInt32Menu, setMaxAccelX, &planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxAccY, MSG_AMAX_B, onDrawPInt32Menu, setMaxAccelY, &planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxAccZ, MSG_AMAX_C, onDrawPInt32Menu, setMaxAccelZ, &planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxAccE, MSG_AMAX_E, onDrawPInt32Menu, setMaxAccelE, &planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
    #endif
  }
  SET_MENU(maxAccelMenu, MSG_AMAX_EN);
}

#if ENABLED(CLASSIC_JERK)

  void drawMaxJerkMenu() {
    if (notCurrentMenu(maxJerkMenu)) {
      BACK_ITEM(drawMotionMenu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkX, MSG_VA_JERK, onDrawPFloatMenu, setMaxJerkX, &planner.max_jerk.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkY, MSG_VB_JERK, onDrawPFloatMenu, setMaxJerkY, &planner.max_jerk.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkZ, MSG_VC_JERK, onDrawPFloatMenu, setMaxJerkZ, &planner.max_jerk.z);
      #endif
      #if HAS_HOTEND
        EDIT_ITEM(ICON_MaxSpeedJerkE, MSG_VE_JERK, onDrawPFloatMenu, setMaxJerkE, &planner.max_jerk.e);
      #endif
    }
    SET_MENU(maxJerkMenu, MSG_JERK);
  }

#endif // ENABLED(CLASSIC_JERK)

void drawStepsMenu() {
  if (notCurrentMenu(stepsMenu)) {
    BACK_ITEM(drawMotionMenu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_StepX, MSG_A_STEPS, onDrawPFloatMenu, setStepsX, &planner.settings.axis_steps_per_mm[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_StepY, MSG_B_STEPS, onDrawPFloatMenu, setStepsY, &planner.settings.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_StepZ, MSG_C_STEPS, onDrawPFloatMenu, setStepsZ, &planner.settings.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_StepE, MSG_E_STEPS, onDrawPFloatMenu, setStepsE, &planner.settings.axis_steps_per_mm[E_AXIS]);
    #endif
  }
  SET_MENU(stepsMenu, MSG_STEPS_PER_MM);
}

//=============================================================================
// UI editable custom colors
//=============================================================================

#if HAS_CUSTOM_COLORS

  void dwinApplyColor(const int8_t element, const bool ldef /* = false*/) {
    const uint16_t color = RGB(hmiValue.Color[0], hmiValue.Color[1], hmiValue.Color[2]);
    switch (element) {
      case  2: hmiData.colorBackground = ldef ? defColorBackground : color; DWINUI::setBackgroundColor(hmiData.colorBackground); break;
      case  3: hmiData.colorCursor     = ldef ? defColorCursor     : color; break;
      case  4: hmiData.colorTitleBg    = ldef ? defColorTitleBg    : color; title.backColor = hmiData.colorTitleBg; break;
      case  5: hmiData.colorTitleTxt   = ldef ? defColorTitleTxt   : color; title.textColor = hmiData.colorTitleTxt; break;
      case  6: hmiData.colorText       = ldef ? defColorText       : color; DWINUI::setTextColor(hmiData.colorText); break;
      case  7: hmiData.colorSelected   = ldef ? defColorSelected   : color; break;
      case  8: hmiData.colorSplitLine  = ldef ? defColorSplitLine  : color; break;
      case  9: hmiData.colorHighlight  = ldef ? defColorHighlight  : color; break;
      case 10: hmiData.colorStatusBg   = ldef ? defColorStatusBg   : color; break;
      case 11: hmiData.colorStatusTxt  = ldef ? defColorStatusTxt  : color; break;
      case 12: hmiData.colorPopupBg    = ldef ? defColorPopupBg    : color; break;
      case 13: hmiData.colorPopupTxt   = ldef ? defColorPopupTxt   : color; break;
      case 14: hmiData.colorAlertBg    = ldef ? defColorAlertBg    : color; break;
      case 15: hmiData.colorAlertTxt   = ldef ? defColorAlertTxt   : color; break;
      case 16: hmiData.colorPercentTxt = ldef ? defColorPercentTxt : color; break;
      case 17: hmiData.colorBarfill    = ldef ? defColorBarfill    : color; break;
      case 18: hmiData.colorIndicator  = ldef ? defColorIndicator  : color; break;
      case 19: hmiData.colorCoordinate = ldef ? defColorCoordinate : color; break;
      default: break;
    }
  }

  #if HAS_CUSTOM_COLORS_MENU

    void dwinApplyColor() {
      *menuData.intPtr = RGB(hmiValue.Color[0], hmiValue.Color[1], hmiValue.Color[2]);
      dwinSetColors();
      drawSelectColorsMenu();
      hash_changed = true;
      LCD_MESSAGE(MSG_COLORS_APPLIED);
      dwinDrawDashboard();
    }

    void restoreDefaultColors() {
      dwinSetColorDefaults();
      dwinSetColors();
      dwinRedrawScreen();
    }

    void selColor() {
      menuData.intPtr = (int16_t*)currentMenu->selectedItem().value;
      hmiValue.Color[0] = GetRColor(*menuData.intPtr);  // Red
      hmiValue.Color[1] = GetGColor(*menuData.intPtr);  // Green
      hmiValue.Color[2] = GetBColor(*menuData.intPtr);  // Blue
      drawGetColorMenu();
    }

    void liveRGBColor() {
      hmiValue.Color[currentMenu->line() - 2] = menuData.value;
      uint16_t color = RGB(hmiValue.Color[0], hmiValue.Color[1], hmiValue.Color[2]);
      dwinDrawRectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
    }
    void setRGBColor() {
      const uint8_t color = currentMenu->selectedItem().icon;
      setIntOnClick(0, (color == 1) ? 63 : 31, hmiValue.Color[color], nullptr, liveRGBColor);
    }

    void drawSelectColorsMenu() {
      checkkey = ID_Menu;
      if (notCurrentMenu(selectColorMenu)) {
        BACK_ITEM(drawAdvancedSettingsMenu);
        MENU_ITEM(ICON_StockConfiguration, MSG_RESTORE_DEFAULTS, onDrawMenuItem, restoreDefaultColors);
        EDIT_ITEM_F(0, "Screen Background", onDrawSelColorItem, selColor, &hmiData.colorBackground);
        EDIT_ITEM_F(0, "Cursor", onDrawSelColorItem, selColor, &hmiData.colorCursor);
        EDIT_ITEM_F(0, "Title Background", onDrawSelColorItem, selColor, &hmiData.colorTitleBg);
        EDIT_ITEM_F(0, "Title Text", onDrawSelColorItem, selColor, &hmiData.colorTitleTxt);
        EDIT_ITEM_F(0, "Text", onDrawSelColorItem, selColor, &hmiData.colorText);
        EDIT_ITEM_F(0, "Selected", onDrawSelColorItem, selColor, &hmiData.colorSelected);
        EDIT_ITEM_F(0, "Split Line", onDrawSelColorItem, selColor, &hmiData.colorSplitLine);
        EDIT_ITEM_F(0, "Highlight", onDrawSelColorItem, selColor, &hmiData.colorHighlight);
        EDIT_ITEM_F(0, "Status Background", onDrawSelColorItem, selColor, &hmiData.colorStatusBg);
        EDIT_ITEM_F(0, "Status Text", onDrawSelColorItem, selColor, &hmiData.colorStatusTxt);
        EDIT_ITEM_F(0, "Popup Background", onDrawSelColorItem, selColor, &hmiData.colorPopupBg);
        EDIT_ITEM_F(0, "Popup Text", onDrawSelColorItem, selColor, &hmiData.colorPopupTxt);
        EDIT_ITEM_F(0, "Alert Background", onDrawSelColorItem, selColor, &hmiData.colorAlertBg);
        EDIT_ITEM_F(0, "Alert Text", onDrawSelColorItem, selColor, &hmiData.colorAlertTxt);
        EDIT_ITEM_F(0, "Percent Text", onDrawSelColorItem, selColor, &hmiData.colorPercentTxt);
        EDIT_ITEM_F(0, "Bar Fill", onDrawSelColorItem, selColor, &hmiData.colorBarfill);
        EDIT_ITEM_F(0, "Indicator value", onDrawSelColorItem, selColor, &hmiData.colorIndicator);
        EDIT_ITEM_F(0, "Coordinate value", onDrawSelColorItem, selColor, &hmiData.colorCoordinate);
      }
      SET_MENU(selectColorMenu, MSG_COLORS_SELECT);
    }

    void drawGetColorMenu() {
      if (notCurrentMenu(getColorMenu)) {
        BACK_ITEM(dwinApplyColor);
        MENU_ITEM(ICON_Cancel, MSG_BUTTON_CANCEL, onDrawMenuItem, drawSelectColorsMenu);
        MENU_ITEM(0, MSG_COLORS_RED, onDrawGetColorItem, setRGBColor);
        MENU_ITEM(1, MSG_COLORS_GREEN, onDrawGetColorItem, setRGBColor);
        MENU_ITEM(2, MSG_COLORS_BLUE, onDrawGetColorItem, setRGBColor);
      }
      SET_MENU(getColorMenu, MSG_COLORS_GET);
      dwinDrawRectangle(1, *menuData.intPtr, 20, 315, DWIN_WIDTH - 20, 335);
    }

  #endif // HAS_CUSTOM_COLORS_MENU

#endif // HAS_CUSTOM_COLORS

//=============================================================================
// Nozzle and Bed PID/MPC
//=============================================================================

#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)

  #if ENABLED(MPC_EDIT_MENU)
    void setHeaterPower() { setPFloatOnClick(1, 200, 1); }
    void setBlkHeatCapacity() { setPFloatOnClick(0, 40, 2); }
    void SetSensorResponse() { setPFloatOnClick(0, 1, 4); }
    void setAmbientXfer() { setPFloatOnClick(0, 1, 4); }
    #if ENABLED(MPC_INCLUDE_FAN)
      void onDrawFanAdj(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 4, thermalManager.temp_hotend[EXT].fanCoefficient()); }
      void applyFanAdj() { thermalManager.temp_hotend[EXT].applyFanAdjustment(menuData.value / POW(10, 4)); }
      void setFanAdj() { setFloatOnClick(0, 1, 4, thermalManager.temp_hotend[EXT].fanCoefficient(), applyFanAdj); }
    #endif
  #endif

  void drawHotendMPCMenu() {
    if (notCurrentMenu(hotendMPCMenu)) {
      MPC_t &mpc = thermalManager.temp_hotend[EXT].mpc;
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if ENABLED(MPC_AUTOTUNE_MENU)
        MENU_ITEM(ICON_MPCNozzle, MSG_MPC_AUTOTUNE, onDrawMenuItem, []{ thermalManager.MPC_autotune(active_extruder, Temperature::MPCTuningType::AUTO); });
      #endif
      #if ENABLED(MPC_EDIT_MENU)
        EDIT_ITEM(ICON_MPCHeater, MSG_MPC_POWER, onDrawPFloatMenu, setHeaterPower, &mpc.heater_power);
        EDIT_ITEM(ICON_MPCHeatCap, MSG_MPC_BLOCK_HEAT_CAPACITY, onDrawPFloat2Menu, setBlkHeatCapacity, &mpc.block_heat_capacity);
        EDIT_ITEM(ICON_MPCValue, MSG_SENSOR_RESPONSIVENESS, onDrawPFloat4Menu, SetSensorResponse, &mpc.sensor_responsiveness);
        EDIT_ITEM(ICON_MPCValue, MSG_MPC_AMBIENT_XFER_COEFF, onDrawPFloat4Menu, setAmbientXfer, &mpc.ambient_xfer_coeff_fan0);
        #if ENABLED(MPC_INCLUDE_FAN)
          EDIT_ITEM(ICON_MPCFan, MSG_MPC_AMBIENT_XFER_COEFF_FAN, onDrawFanAdj, setFanAdj, &mpc.fan255_adjustment);
        #endif
      #endif
    }
    SET_MENU(hotendMPCMenu, MSG_MPC_SETTINGS);
  }

#endif // MPC_EDIT_MENU || MPC_AUTOTUNE_MENU

#if HAS_PID_HEATING && ENABLED(PID_AUTOTUNE_MENU)
  void setPID(celsius_t t, heater_id_t h) {
    gcode.process_subcommands_now(TS(F("G28OXY\nG0Z5F300\nG0X"), X_CENTER, F("Y"), Y_CENTER, F("F5000\nM84\nM400")));
    thermalManager.PID_autotune(t, h, hmiData.pidCycles, true);
  }
  void setPidCycles() { setPIntOnClick(3, 50); }
#endif

#if HAS_PID_HEATING && ENABLED(PID_EDIT_MENU)

  void setKp() { setPFloatOnClick(0, 1000, 2); }
  void applyPIDi() {
    *menuData.floatPtr = scalePID_i(menuData.value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void applyPIDd() {
    *menuData.floatPtr = scalePID_d(menuData.value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void setKi() {
    menuData.floatPtr = (float*)currentMenu->selectedItem().value;
    const float value = unscalePID_i(*menuData.floatPtr);
    setFloatOnClick(0, 1000, 2, value, applyPIDi);
  }
  void setKd() {
    menuData.floatPtr = (float*)currentMenu->selectedItem().value;
    const float value = unscalePID_d(*menuData.floatPtr);
    setFloatOnClick(0, 1000, 2, value, applyPIDd);
  }
  void onDrawPIDi(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 2, unscalePID_i(*(float*)getMenuItem(pos)->value)); }
  void onDrawPIDd(int8_t pos, int8_t line) { onDrawFloatMenu(pos, line, 2, unscalePID_d(*(float*)getMenuItem(pos)->value)); }

#endif // HAS_PID_HEATING && PID_EDIT_MENU

#if ENABLED(PIDTEMP) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void hotendPID() { setPID(hmiData.hotendPidT, H_E0); }
    void setHotendPidT() { setPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif

  void drawHotendPIDMenu() {
    if (notCurrentMenu(hotendPIDMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM(ICON_PIDNozzle, MSG_PID_AUTOTUNE, onDrawMenuItem, hotendPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, setHotendPidT, &hmiData.hotendPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, setPidCycles, &hmiData.pidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM(ICON_PIDValue, MSG_PID_SET_KP, onDrawPFloat2Menu, setKp, &thermalManager.temp_hotend[EXT].pid.Kp);
        EDIT_ITEM(ICON_PIDValue, MSG_PID_SET_KI, onDrawPIDi, setKi, &thermalManager.temp_hotend[EXT].pid.Ki);
        EDIT_ITEM(ICON_PIDValue, MSG_PID_SET_KD, onDrawPIDd, setKd, &thermalManager.temp_hotend[EXT].pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      #endif
    }
    SET_MENU(hotendPIDMenu, MSG_HOTEND_PID_SETTINGS);
  }

#endif // PIDTEMP && (PID_AUTOTUNE_MENU || PID_EDIT_MENU)

#if ENABLED(PIDTEMPBED) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void bedPID() { setPID(hmiData.bedPidT, H_BED); }
    void setBedPidT() { setPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  void drawBedPIDMenu() {
    checkkey = ID_Menu;
    if (notCurrentMenu(bedPIDMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM(ICON_PIDBed, MSG_PID_AUTOTUNE, onDrawMenuItem,bedPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, setBedPidT, &hmiData.bedPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, setPidCycles, &hmiData.pidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM(ICON_PIDValue, MSG_PID_SET_KP, onDrawPFloat2Menu, setKp, &thermalManager.temp_bed.pid.Kp);
        EDIT_ITEM(ICON_PIDValue, MSG_PID_SET_KI, onDrawPIDi, setKi, &thermalManager.temp_bed.pid.Ki);
        EDIT_ITEM(ICON_PIDValue, MSG_PID_SET_KD, onDrawPIDd, setKd, &thermalManager.temp_bed.pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      #endif
    }
    SET_MENU(bedPIDMenu, MSG_BED_PID_SETTINGS);
  }

#endif // PIDTEMPBED && (PID_AUTOTUNE_MENU || PID_EDIT_MENU)

//=============================================================================
//
//=============================================================================

#if HAS_BED_PROBE

  void drawZOffsetWizMenu() {
    if (notCurrentMenu(zOffsetWizMenu)) {
      BACK_ITEM(drawPrepareMenu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
      MENU_ITEM(ICON_MoveZ0, MSG_MOVE_Z_HOME, onDrawMenuItem, setMoveZto0);
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
    }
    SET_MENU(zOffsetWizMenu, MSG_PROBE_WIZARD);
    if (!axis_is_trusted(Z_AXIS)) LCD_MESSAGE(MSG_Z_POSITION_IS_UNKNOWN);
  }

#endif

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)

  void drawHomingMenu() {
    if (notCurrentMenu(homingMenu)) {
      BACK_ITEM(drawPrepareMenu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
      #if HAS_X_AXIS
        MENU_ITEM(ICON_HomeX, MSG_AUTO_HOME_X, onDrawMenuItem, homeX);
      #endif
      #if HAS_Y_AXIS
        MENU_ITEM(ICON_HomeY, MSG_AUTO_HOME_Y, onDrawMenuItem, homeY);
      #endif
      #if HAS_Z_AXIS
        MENU_ITEM(ICON_HomeZ, MSG_AUTO_HOME_Z, onDrawMenuItem, homeZ);
      #endif
      #if ENABLED(MESH_BED_LEVELING)
        EDIT_ITEM(ICON_ZAfterHome, MSG_Z_AFTER_HOME, onDrawPInt8Menu, setZAfterHoming, &hmiData.zAfterHoming);
      #endif
    }
    SET_MENU(homingMenu, MSG_HOMING);
  }

#endif // INDIVIDUAL_AXIS_HOMING_SUBMENU

#if ENABLED(FWRETRACT)

  void drawFWRetractMenu() {
    if (notCurrentMenu(fwRetractMenu)) {
      BACK_ITEM(returnToPreviousMenu);
      EDIT_ITEM(ICON_FWRetLength, MSG_CONTROL_RETRACT, onDrawPFloatMenu, setRetractLength, &fwretract.settings.retract_length);
      EDIT_ITEM(ICON_FWRetSpeed, MSG_SINGLENOZZLE_RETRACT_SPEED, onDrawPFloatMenu, setRetractSpeed, &fwretract.settings.retract_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRetZRaise, MSG_CONTROL_RETRACT_ZHOP, onDrawPFloat2Menu, setZRaise, &fwretract.settings.retract_zraise);
      EDIT_ITEM(ICON_FWRecSpeed, MSG_SINGLENOZZLE_UNRETRACT_SPEED, onDrawPFloatMenu, setRecoverSpeed, &fwretract.settings.retract_recover_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRecExtra, MSG_CONTROL_RETRACT_RECOVER, onDrawPFloatMenu, setAddRecover, &fwretract.settings.retract_recover_extra);
    }
    SET_MENU(fwRetractMenu, MSG_FWRETRACT);
  }

#endif

//=============================================================================
// Mesh Bed Leveling
//=============================================================================

#if HAS_MESH

  #if PROUI_EX
    void applyMeshPoints() {
      uint8_t*uint8Ptr = (uint8_t*)currentMenu->selectedItem().value;
      *uint8Ptr = menuData.value;
      proUIEx.applyMeshLimits();
    }
    void setMeshPoints() { setIntOnClick(GRID_MIN, GRID_LIMIT, *(uint8_t*)currentMenu->selectedItem().value, applyMeshPoints); }
    void applyMeshLimits() { proUIEx.applyMeshLimits(); redrawItem(); }
    void setMeshInset() { setPIntOnClick(MIN_MESH_INSET, MAX_MESH_INSET, applyMeshLimits); }
  #endif

  void applyMeshFadeHeight() { set_z_fade_height(planner.z_fade_height); }
  void setMeshFadeHeight() { setPFloatOnClick(0, 100, 1, applyMeshFadeHeight); }

  #if ENABLED(ACTIVATE_MESH_ITEM)

    void setMeshActive() {
      const bool val = planner.leveling_active;
      set_bed_leveling_enabled(!planner.leveling_active);
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (!val) {
          if (planner.leveling_active && bedlevel.storage_slot >= 0)
            ui.status_printf(0, GET_TEXT_F(MSG_MESH_ACTIVE), bedlevel.storage_slot);
          else
            ui.set_status(GET_TEXT_F(MSG_UBL_MESH_INVALID));
        }
        else ui.reset_status(true);
      #else
        UNUSED(val);
      #endif
      showCheckboxLine(planner.leveling_active);
    }

  #endif

  #if ALL(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    void setBedLevT() { setPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  #if ENABLED(MESH_EDIT_MENU)
    bool autoMovToMesh = false;
    #define Z_OFFSET_MIN -3
    #define Z_OFFSET_MAX  3
    void applyEditMeshX() { bedLevelTools.mesh_x = menuData.value; if (autoMovToMesh) bedLevelTools.moveToXY(); }
    void applyEditMeshY() { bedLevelTools.mesh_y = menuData.value; if (autoMovToMesh) bedLevelTools.moveToXY(); }
    void liveEditMesh() { getMenuItem(editZValueItem)->value = &bedlevel.z_values[hmiValue.select ? bedLevelTools.mesh_x : menuData.value][hmiValue.select ? menuData.value : bedLevelTools.mesh_y]; redrawItem(editZValueItem); }
    void setEditMeshX() { hmiValue.select = 0; setIntOnClick(0, GRID_MAX_POINTS_X - 1, bedLevelTools.mesh_x, applyEditMeshX, liveEditMesh); }
    void setEditMeshY() { hmiValue.select = 1; setIntOnClick(0, GRID_MAX_POINTS_Y - 1, bedLevelTools.mesh_y, applyEditMeshY, liveEditMesh); }
    void liveEditMeshZ() { *menuData.floatPtr = menuData.value / POW(10, 2); if (autoMovToMesh) bedLevelTools.moveToZ(); }
    void setEditZValue() { setPFloatOnClick(Z_OFFSET_MIN, Z_OFFSET_MAX, 2, nullptr, liveEditMeshZ); if (autoMovToMesh) bedLevelTools.moveToXYZ(); }
    void resetMesh() { bedLevelTools.meshReset(); LCD_MESSAGE(MSG_MESH_RESET); }
    void setAutoMovToMesh() { toggleCheckboxLine(autoMovToMesh); }
  #endif

#endif // HAS_MESH

#if ENABLED(AUTO_BED_LEVELING_UBL)

  void applyUBLSlot() { bedlevel.storage_slot = menuData.value; }
  void setUBLSlot() { setIntOnClick(0, settings.calc_num_meshes() - 1, bedlevel.storage_slot, applyUBLSlot); }
  void onDrawUBLSlot(int8_t pos, int8_t line) {
    NOLESS(bedlevel.storage_slot, 0);
    onDrawIntMenu(pos, line, bedlevel.storage_slot);
  }

  void applyUBLTiltGrid() { bedLevelTools.tilt_grid = menuData.value; }
  void setUBLTiltGrid() { setIntOnClick(1, 3, bedLevelTools.tilt_grid, applyUBLTiltGrid); }

  void ublMeshTilt() {
    NOLESS(bedlevel.storage_slot, 0);
    gcode.process_subcommands_now(TS(F("G29J"), bedLevelTools.tilt_grid > 1 ? bedLevelTools.tilt_grid : 0));
    LCD_MESSAGE(MSG_UBL_MESH_TILTED);
  }

  void ublSmartFillMesh() {
    for (uint8_t i = 0; i < TERN(PROUI_EX, GRID_LIMIT, GRID_MAX_POINTS_X); ++i) bedlevel.smart_fill_mesh();
    LCD_MESSAGE(MSG_UBL_MESH_FILLED);
  }

  void ublMeshSave() {
    NOLESS(bedlevel.storage_slot, 0);
    settings.store_mesh(bedlevel.storage_slot);
    ui.status_printf(0, GET_TEXT_F(MSG_MESH_SAVED), bedlevel.storage_slot);
    DONE_BUZZ(true);
  }

  void ublMeshLoad() {
    NOLESS(bedlevel.storage_slot, 0);
    settings.load_mesh(bedlevel.storage_slot);
  }

#endif // AUTO_BED_LEVELING_UBL

#if HAS_MESH
  void drawMeshSetMenu() {
    if (notCurrentMenu(meshMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if PROUI_EX
        EDIT_ITEM(ICON_MeshPointsX, MSG_MESH_POINTS_X, onDrawPInt8Menu, setMeshPoints, &meshSet.grid_max_points_x);
        EDIT_ITEM(ICON_MeshPointsY, MSG_MESH_POINTS_Y, onDrawPInt8Menu, setMeshPoints, &meshSet.grid_max_points_y);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_INSET, onDrawSubMenu, drawMeshInsetMenu);
      #endif
      #if ALL(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
        EDIT_ITEM(ICON_Temperature, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setBedLevT, &hmiData.bedLevT);
      #endif
      EDIT_ITEM(ICON_SetZOffset, MSG_Z_FADE_HEIGHT, onDrawPFloatMenu, setMeshFadeHeight, &planner.z_fade_height);
      #if ENABLED(ACTIVATE_MESH_ITEM)
        EDIT_ITEM(ICON_UBLActive, MSG_ACTIVATE_MESH, onDrawChkbMenu, setMeshActive, &planner.leveling_active);
      #endif
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, autoLev);
      #endif
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        EDIT_ITEM(ICON_UBLSlot, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, setUBLSlot, &bedlevel.storage_slot);
        MENU_ITEM(ICON_UBLMeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, ublMeshSave);
        MENU_ITEM(ICON_UBLMeshLoad, MSG_UBL_LOAD_MESH, onDrawMenuItem, ublMeshLoad);
        EDIT_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILTING_GRID, onDrawPInt8Menu, setUBLTiltGrid, &bedLevelTools.tilt_grid);
        MENU_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILT_MESH, onDrawMenuItem, ublMeshTilt);
        MENU_ITEM(ICON_UBLSmartFill, MSG_UBL_SMART_FILLIN, onDrawMenuItem, ublSmartFillMesh);
      #endif
      #if ENABLED(MESH_EDIT_MENU)
        MENU_ITEM(ICON_MeshReset, MSG_MESH_RESET, onDrawMenuItem, resetMesh);
        MENU_ITEM(ICON_MeshEdit, MSG_EDIT_MESH, onDrawSubMenu, drawEditMeshMenu);
      #endif
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, dwinMeshViewer);
    }
    SET_MENU(meshMenu, MSG_MESH_LEVELING);
  }

  #if ENABLED(MESH_EDIT_MENU)
    void drawEditMeshMenu() {
      if (!leveling_is_valid()) { LCD_MESSAGE(MSG_UBL_MESH_INVALID); return; }
      set_bed_leveling_enabled(false);
      if (notCurrentMenu(editMeshMenu)) {
        bedLevelTools.mesh_x = bedLevelTools.mesh_y = 0;
        BACK_ITEM(drawMeshSetMenu);
        EDIT_ITEM(ICON_UBLActive, MSG_PROBE_WIZARD_MOVING, onDrawChkbMenu, setAutoMovToMesh, &autoMovToMesh);
        EDIT_ITEM(ICON_MeshEditX, MSG_MESH_X, onDrawPInt8Menu, setEditMeshX, &bedLevelTools.mesh_x);
        EDIT_ITEM(ICON_MeshEditY, MSG_MESH_Y, onDrawPInt8Menu, setEditMeshY, &bedLevelTools.mesh_y);
        editZValueItem = EDIT_ITEM(ICON_MeshEditZ, MSG_MESH_EDIT_Z, onDrawPFloat2Menu, setEditZValue, &bedlevel.z_values[bedLevelTools.mesh_x][bedLevelTools.mesh_y]);
        TERN_(HAS_BED_PROBE, MENU_ITEM(ICON_UBLActive, MSG_PROBE_WIZARD_PROBING, onDrawMenuItem, bedLevelTools.probeXY));
      }
      SET_MENU(editMeshMenu, MSG_EDIT_MESH);
    }
  #endif

  #if PROUI_EX
    void drawMeshInsetMenu() {
      checkkey = ID_Menu;
      if (notCurrentMenu(meshInsetMenu)) {
        BACK_ITEM(drawMeshSetMenu);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_X, onDrawPIntMenu, setMeshInset, &meshSet.mesh_min_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_X, onDrawPIntMenu, setMeshInset, &meshSet.mesh_max_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_Y, onDrawPIntMenu, setMeshInset, &meshSet.mesh_min_y);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_Y, onDrawPIntMenu, setMeshInset, &meshSet.mesh_max_y);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_AMAX, onDrawMenuItem, proUIEx.maxMeshArea);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_CENTER, onDrawMenuItem, proUIEx.centerMeshArea);
      }
      SET_MENU(meshInsetMenu, MSG_MESH_INSET);
    }
  #endif

#endif // HAS_MESH

//=============================================================================
// CV Laser Module support
//=============================================================================
#if ENABLED(CV_LASER_MODULE)

  // Make the current position 0,0,0
  void setHome() {
    laser_device.homepos += current_position;
    set_all_homed();
    gcode.process_subcommands_now(F("G92X0Y0Z0"));
    redrawMenu();
  }

  void laserOn(const bool turn_on) {
    laser_device.laser_set(turn_on);
    dwinDrawDashboard();
  }

  void laserToggle() {
    laserOn(!laser_device.is_laser_device());
    showCheckboxLine(laser_device.is_laser_device());
  }

  void laserPrint() {
    if (!laser_device.is_laser_device()) return;
    thermalManager.disable_all_heaters(); // 关闭加热107011 -20211012
    print_job_timer.reset();  //107011 -20211009 清除前一次的打印时间
    laser_device.laser_power_open(); // 打开激光, 以最弱的激光输出。专业固件是最好的
    // queue.inject_P(PSTR("M999\nG92.9Z0")); // 107011 -20211013
    card.openAndPrintFile(card.filename);
  }

  void laserRunRange() {
    if (!laser_device.is_laser_device()) return;
    if (!all_axes_trusted()) return LCD_MESSAGE(MSG_LASER_FIRST_HOME);
    dwinShowPopup(ICON_TempTooHigh, "LASER", "Run Range", BTN_Cancel);
    hmiSaveProcessID(ID_WaitResponse);
    laser_device.laser_range();
  }

  void drawLaserSettingsMenu() {
    enableLiveMove = true;
    if (notCurrentMenu(laserSettings)) {
      BACK_ITEM(returnToPreviousMenu);
      EDIT_ITEM(ICON_LaserToggle, MSG_LASER_TOGGLE, onDrawChkbMenu, laserToggle, &laser_device.laser_enabled);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
      EDIT_ITEM(ICON_LaserFocus, MSG_LASER_FOCUS, onDrawPFloatMenu, setMoveZ, &current_position.z);
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, setMoveX, &current_position.x);
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, setMoveY, &current_position.y);
      MENU_ITEM(ICON_SetHome, MSG_SET_AS_HOME, onDrawMenuItem, setHome);
    }
    SET_MENU(laserSettings, MSG_LASER_MENU);
  }

  void drawLaserPrintMenu() {
    if (!laser_device.is_laser_device()) return gotoMainMenu();
    if (notCurrentMenu(laserPrintMenu)) {
      BACK_ITEM(drawFileMenu);
      MENU_ITEM(ICON_SetHome, MSG_CONFIGURATION, onDrawSubMenu, drawLaserSettingsMenu);
      MENU_ITEM(ICON_LaserPrint, MSG_LASER_ENGRAVING, onDrawMenuItem, laserPrint);
      MENU_ITEM(ICON_LaserRunRange, MSG_LASER_RUN_RANGE, onDrawMenuItem, laserRunRange);
    }
    SET_MENU(laserPrintMenu, MSG_LASER_MENU);
    char buf[23], str_1[5], str_2[5];
    sprintf_P(buf, PSTR("XMIN: %s XMAX: %s"), dtostrf(LASER_XMIN, 1, 1, str_1), dtostrf(LASER_XMAX, 1, 1, str_2));
    DWINUI::drawString(LBLX, MBASE(4) + 10, buf);
    sprintf_P(buf, PSTR("YMIN: %s YMAX: %s"), dtostrf(LASER_YMIN, 1, 1, str_1), dtostrf(LASER_YMAX, 1, 1, str_2));
    DWINUI::drawString(LBLX, MBASE(5) - 10, buf);
  }

#endif // CV_LASER_MODULE

//=============================================================================
// toolBar
//=============================================================================
#if HAS_TOOLBAR

  void drawTBSetupMenu() {
    if (notCurrentMenu(tbSetupMenu)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      for (uint8_t i = 0; i < TBMaxOpt; ++i) EDIT_ITEM_F(0, "", onDrawTBSetupItem, setTBSetupItem, &PRO_data.TBopt[i]);
    }
    SET_MENU(tbSetupMenu, MSG_TOOLBAR_SETUP);
  }

  void exitToolBar() {
    select_page.set(PAGE_ADVANCE);
    iconAdvSettings();
    checkkey = ID_MainMenu;
    toolBar.draw();
    dwinUpdateLCD();
  }

  void gotoToolBar() {
    checkkey = ID_Menu;
    toolBar.draw();
  }

#endif  // HAS_TOOLBAR

//=============================================================================
// More Host support
//=============================================================================

#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)

  void popUpHostShutDown() { dwinPopupConfirmCancel(ICON_Info_1, GET_TEXT_F(MSG_HOST_SHUTDOWN)); }
  void onClickHostShutDown() {
    if (hmiFlag.select_flag) hostui.shutdown();
    hmiReturnScreen();
  }
  void hostShutDown() { gotoPopup(popUpHostShutDown, onClickHostShutDown); }

#endif

//=============================================================================

#if DEBUG_DWIN
  void dwinDebug(const char *msg) {
    SERIAL_ECHOLNPGM_P(msg);
    dwinShowPopup(0, "Debug", msg, BTN_Continue);
    wait_for_user_response();
    drawMainArea();
  }
#endif

#endif // DWIN_LCD_PROUI
