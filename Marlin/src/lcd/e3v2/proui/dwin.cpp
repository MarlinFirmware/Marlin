/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.19.3
 * Date: 2022/09/25
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

#include "../../fontutils.h"
#include "../../marlinui.h"

#include "../../../sd/cardreader.h"

#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"

#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/stepper.h"
#include "../../../module/planner.h"

#include "../../../gcode/gcode.h"
#include "../../../gcode/queue.h"

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
  #include "bedlevel_tools.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#ifdef BLTOUCH
  #include "../../../feature/bltouch.h"
#endif

#if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
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

#include "dwin.h"
#include "dwinui.h"
#include "menus.h"
#include "dwin_popup.h"

#if HAS_GCODE_PREVIEW
  #include "gcode_preview.h"
#endif

#if HAS_TOOLBAR
  #include "toolbar.h"
#endif

#if HAS_ESDIAG
  #include "endstop_diag.h"
#endif

#if HAS_PIDPLOT
  #include "plot.h"
#endif

#if HAS_MESH
  #include "meshviewer.h"
#endif

#if HAS_LOCKSCREEN
  #include "lockscreen.h"
#endif

#define PAUSE_HEAT

// Print speed limit
#define MIN_PRINT_SPEED  10
#define MAX_PRINT_SPEED 999

// Print flow limit
#define MIN_PRINT_FLOW   10
#define MAX_PRINT_FLOW   299

// Load and Unload limits
#define MAX_LOAD_UNLOAD  500

// Feedspeed limit (max feedspeed = MAX_FEEDRATE_EDIT_VALUES)
#define MIN_MAXFEEDSPEED      1
#define MIN_MAXACCELERATION   1
#define MIN_MAXJERK           0.1
#define MIN_STEP              1
#define MAX_STEP              1999.9
#define MIN_JD_MM             0.001
#define MAX_JD_MM             TERN(LIN_ADVANCE, 0.3f, 0.5f)

// Editable temperature limits
#define MIN_ETEMP  0
#define MAX_ETEMP  (thermalManager.hotend_maxtemp[0] - (HOTEND_OVERSHOOT))
#define MIN_BEDTEMP 0
#define MAX_BEDTEMP BED_MAX_TARGET

#ifdef MANUAL_FEEDRATE
  #define FEEDRATE_E      (manual_feedrate_mm_s.e)
#else
  #define FEEDRATE_E      (1)
#endif

#define DWIN_VAR_UPDATE_INTERVAL         500
#define DWIN_UPDATE_INTERVAL             1000
#define DWIN_REMAIN_TIME_UPDATE_INTERVAL SEC_TO_MS(20)

#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, HMI_data.ManualZOffset)

// Structs
HMI_value_t HMI_value;
HMI_flag_t HMI_flag{0};
HMI_data_t HMI_data;

bool blink = false;
bool htemp = false;
millis_t dwin_heat_time = 0;

uint8_t checkkey = 255, last_checkkey = MainMenu;

enum SelectItem : uint8_t {
  PAGE_PRINT = 0,
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

constexpr float max_feedrate_edit_values[] =
  #ifdef MAX_FEEDRATE_EDIT_VALUES
    MAX_FEEDRATE_EDIT_VALUES
  #else
    { 1000, 1000, 40, 200 }
  #endif
;

constexpr float max_acceleration_edit_values[] =
  #ifdef MAX_ACCEL_EDIT_VALUES
    MAX_ACCEL_EDIT_VALUES
  #else
    { 3000, 3000, 300, 9999 }
  #endif
;

#if HAS_CLASSIC_JERK
  constexpr float max_jerk_edit_values[] =
    #ifdef MAX_JERK_EDIT_VALUES
      MAX_JERK_EDIT_VALUES
    #else
      { DEFAULT_XJERK * 2, DEFAULT_YJERK * 2, DEFAULT_ZJERK * 4, DEFAULT_EJERK * 4 }
    #endif
  ;
#endif

static uint8_t _percent_done = 0;
static uint32_t _remain_time = 0;

#if HAS_HOTEND
  float last_E = 0;
#endif

// New menu system pointers
MenuClass *FileMenu = nullptr;
MenuClass *PrepareMenu = nullptr;
MenuClass *TrammingMenu = nullptr;
MenuClass *MoveMenu = nullptr;
MenuClass *ControlMenu = nullptr;
MenuClass *AdvancedMenu = nullptr;
MenuClass *AdvancedSettings = nullptr;
#if HAS_HOME_OFFSET
  MenuClass *HomeOffMenu = nullptr;
#endif
#if HAS_BED_PROBE
  MenuClass *ProbeSetMenu = nullptr;
#endif
MenuClass *FilSetMenu = nullptr;
MenuClass *SelectColorMenu = nullptr;
MenuClass *GetColorMenu = nullptr;
MenuClass *TuneMenu = nullptr;
MenuClass *MotionMenu = nullptr;
MenuClass *FilamentMenu = nullptr;
#if ENABLED(MESH_BED_LEVELING)
  MenuClass *ManualMesh = nullptr;
#endif
#if HAS_HOTEND
  MenuClass *PreheatMenu = nullptr;
#endif
MenuClass *TemperatureMenu = nullptr;
MenuClass *MaxSpeedMenu = nullptr;
MenuClass *MaxAccelMenu = nullptr;
#if HAS_CLASSIC_JERK
  MenuClass *MaxJerkMenu = nullptr;
#endif
MenuClass *StepsMenu = nullptr;
MenuClass *PIDMenu = nullptr;
TERN_(PIDTEMP, MenuClass *HotendPIDMenu = nullptr);
TERN_(MPCTEMP, MenuClass *HotendMPCMenu = nullptr);
TERN_(PIDTEMPBED, MenuClass *BedPIDMenu = nullptr);
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  MenuClass *CaseLightMenu = nullptr;
#endif
#if ENABLED(LED_CONTROL_MENU)
  MenuClass *LedControlMenu = nullptr;
#endif
#if HAS_ZOFFSET_ITEM
  MenuClass *ZOffsetWizMenu = nullptr;
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  MenuClass *HomingMenu = nullptr;
#endif
#if ENABLED(FWRETRACT)
  MenuClass *FWRetractMenu = nullptr;
#endif
#if ProUIex
  #if ENABLED(NOZZLE_PARK_FEATURE)
     MenuClass *ParkPosMenu = nullptr;
  #endif
  MenuClass *PhySetMenu = nullptr;
#endif
#if HAS_TOOLBAR
  MenuClass *TBSetupMenu = nullptr;
#endif
#if HAS_MESH
  MenuClass *MeshMenu = nullptr;
  #if ENABLED(MESH_EDIT_MENU)
    MenuClass *EditMeshMenu = nullptr;
  #endif
  #if ProUIex
    MenuClass *MeshInsetMenu = nullptr;
  #endif
  #if ENABLED(SHAPING_MENU)
    MenuClass *InputShapingMenu = nullptr;
  #endif
#endif

// Updatable menuitems pointers
MenuItemClass *HotendTargetItem = nullptr;
MenuItemClass *BedTargetItem = nullptr;
MenuItemClass *FanSpeedItem = nullptr;
MenuItemClass *MMeshMoveZItem = nullptr;
MenuItemClass *EditZValueItem = nullptr;

bool Printing() {
  return (printingIsActive() || print_job_timer.isPaused());
}

bool SD_Printing() {
  return (Printing() && IS_SD_FILE_OPEN());
}

bool Host_Printing() {
  return (Printing() && !IS_SD_FILE_OPEN());
}

//-----------------------------------------------------------------------------
// Main Buttons
//-----------------------------------------------------------------------------

void ICON_Button(const bool selected, const int iconid, const frame_rect_t &ico, const frame_rect_t &ico2, FSTR_P caption) {
  DWINUI::Draw_IconWB(iconid + selected, ico.x, ico.y);
  if (selected) {
    DWINUI::Draw_Box(0, HMI_data.Cursor_color, ico);
    DWINUI::Draw_Box(0, HMI_data.Cursor_color, ico2);
  }
  const uint16_t x = ico.x + (ico.w - strlen_P(FTOP(caption)) * DWINUI::fontWidth()) / 2,
                 y = (ico.y + ico.h - 20) - DWINUI::fontHeight() / 2;
  DWINUI::Draw_String(x, y, caption);
}

//
// Main Menu: "Print"
//
void ICON_Print() {
  constexpr frame_rect_t ico = { 17, 110 - TERN0(HAS_TOOLBAR,TBOffset), 110, 100};
  constexpr frame_rect_t ico2 = { 18, 111 - TERN0(HAS_TOOLBAR,TBOffset), 108, 98};
  ICON_Button(select_page.now == PAGE_PRINT, ICON_Print_0, ico, ico2, GET_TEXT_F(MSG_BUTTON_PRINT));
}

//
// Main Menu: "Prepare"
//
void ICON_Prepare() {
  constexpr frame_rect_t ico = { 145, 110 - TERN0(HAS_TOOLBAR,TBOffset), 110, 100};
  constexpr frame_rect_t ico2 = { 146, 111 - TERN0(HAS_TOOLBAR,TBOffset), 108, 98};
  ICON_Button(select_page.now == PAGE_PREPARE, ICON_Prepare_0, ico, ico2, GET_TEXT_F(MSG_PREPARE));
}

//
// Main Menu: "Control"
//
void ICON_Control() {
  constexpr frame_rect_t ico = { 17, 226 - TERN0(HAS_TOOLBAR,TBOffset), 110, 100};
  constexpr frame_rect_t ico2 = { 18, 227 - TERN0(HAS_TOOLBAR,TBOffset), 108, 98};
  ICON_Button(select_page.now == PAGE_CONTROL, ICON_Control_0, ico, ico2, GET_TEXT_F(MSG_CONTROL));
}

//
// Main Menu: "Level" || "Advanced Settings" if no leveling
//
//
void ICON_AdvSettings() {
  constexpr frame_rect_t ico = { 145, 226 - TERN0(HAS_TOOLBAR,TBOffset), 110, 100};
  constexpr frame_rect_t ico2 = { 146, 227 - TERN0(HAS_TOOLBAR,TBOffset), 108, 98};
  #if ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_UBL, MESH_BED_LEVELING)
  ICON_Button(select_page.now == PAGE_ADVANCE, ICON_Leveling_0, ico, ico2, GET_TEXT_F(MSG_BUTTON_LEVEL));
  #else
  ICON_Button(select_page.now == PAGE_ADVANCE, ICON_Info_0, ico, ico2, GET_TEXT_F(MSG_BUTTON_ADVANCED));
  #endif
}

//
// Printing: "Tune"
//
void ICON_Tune() {
  constexpr frame_rect_t ico = { 8, 232, 80, 100 };
  constexpr frame_rect_t ico2 = { 9, 233, 78, 98 };
  ICON_Button(select_print.now == PRINT_SETUP, ICON_Setup_0, ico, ico2, GET_TEXT_F(MSG_TUNE));
}

//
// Printing: "Pause"
//
void ICON_Pause() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  constexpr frame_rect_t ico2 = { 97, 233, 78, 98 };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Pause_0, ico, ico2, GET_TEXT_F(MSG_BUTTON_PAUSE));
}

//
// Printing: "Resume"
//
void ICON_Resume() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  constexpr frame_rect_t ico2 = { 97, 233, 78, 98 };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Continue_0, ico, ico2, GET_TEXT_F(MSG_BUTTON_RESUME));
}

//
// Printing: "Stop"
//
void ICON_Stop() {
  constexpr frame_rect_t ico = { 184, 232, 80, 100 };
  constexpr frame_rect_t ico2 = { 185, 233, 78, 98 };
  ICON_Button(select_print.now == PRINT_STOP, ICON_Stop_0, ico, ico2, GET_TEXT_F(MSG_BUTTON_STOP));
}

//
//PopUps
//
void Popup_window_PauseOrStop() {
  DWIN_Popup_ConfirmCancel(ICON_BLTouch, select_print.now == PRINT_PAUSE_RESUME ? GET_TEXT_F(MSG_PAUSE_PRINT) : GET_TEXT_F(MSG_STOP_PRINT));
}

#if HAS_HOTEND || HAS_HEATED_BED
  void DWIN_Popup_Temperature(const bool toohigh) {
    HMI_SaveProcessID(WaitResponse);
    DWIN_Show_Popup(toohigh ? ICON_TempTooHigh : ICON_TempTooLow, F("Nozzle or Bed temperature"), toohigh ? F("is too high") : F("is too low"), BTN_Continue);
  }
#endif

//
// Draw status line
//
void DWIN_DrawStatusLine(const char *text) {
  DWIN_Draw_Rectangle(1, HMI_data.StatusBg_Color, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
  if (text) DWINUI::Draw_CenteredString(HMI_data.StatusTxt_Color, STATUS_Y + 2, text);
}
void DWIN_DrawStatusLine(FSTR_P fstr) { DWIN_DrawStatusLine(FTOP(fstr)); }

// Clear & reset status line
void DWIN_ResetStatusLine() {
  ui.status_message[0] = 0;
  DWIN_CheckStatusMessage();
}

// Djb2 hash algorithm
uint32_t GetHash(char * str) {
  uint32_t hash = 5381;
  char c;
  while ((c = *str++)) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

void DWIN_CheckStatusMessage() {
  static uint32_t old_hash = 0;
  uint32_t hash = GetHash(&ui.status_message[0]);
  hash_changed = hash != old_hash;
  old_hash = hash;
};

void DWIN_DrawStatusMessage() {
  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    // Get the UTF8 character count of the string
    uint8_t slen = utf8_strlen(ui.status_message);

    // If the string fits the status line do not scroll it
    if (slen <= LCD_WIDTH) {
       if (hash_changed) {
         DWIN_DrawStatusLine(ui.status_message);
         hash_changed = false;
       }
    }
    else {
      // String is larger than the available line space

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
      const char *stat = MarlinUI::status_and_len(rlen);
      DWIN_Draw_Rectangle(1, HMI_data.StatusBg_Color, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
      DWINUI::MoveTo(0, STATUS_Y + 2);
      DWINUI::Draw_String(HMI_data.StatusTxt_Color, stat, LCD_WIDTH);

      // If the string doesn't completely fill the line...
      if (rlen < LCD_WIDTH) {
        DWINUI::Draw_Char(HMI_data.StatusTxt_Color, '.');  // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;                  // Amount of space left in characters
        if (--chars) {                                     // Draw a second dot if there's space
          DWINUI::Draw_Char(HMI_data.StatusTxt_Color, '.');
          if (--chars)
            DWINUI::Draw_String(HMI_data.StatusTxt_Color, ui.status_message, chars); // Print a second copy of the message
        }
      }
      MarlinUI::advance_status_scroll();
    }

  #else

    if (hash_changed) {
      ui.status_message[LCD_WIDTH] = 0;
      DWIN_DrawStatusLine(ui.status_message);
      hash_changed = false;
    }

  #endif
}

void Draw_Print_Labels() {
  DWINUI::Draw_String( 46, 173, GET_TEXT_F(MSG_INFO_PRINT_TIME));
  DWINUI::Draw_String(181, 173, GET_TEXT_F(MSG_REMAINING_TIME));
}

void Draw_Print_ProgressBar() {
  DWINUI::Draw_IconWB(ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, HMI_data.Barfill_Color, 16 + _percent_done * 240 / 100, 93, 256, 113);
  DWINUI::Draw_Int(HMI_data.PercentTxt_Color, HMI_data.Background_Color, 3, 117, 133, _percent_done);
  DWINUI::Draw_String(HMI_data.PercentTxt_Color, 142, 133, F("%"));
}

void Draw_Print_ProgressElapsed() {
  char buf[10];
  duration_t elapsed = print_job_timer.duration(); // print timer
  sprintf_P(buf, PSTR("%02i:%02i "), (uint16_t)(elapsed.value / 3600), ((uint16_t)elapsed.value % 3600) / 60);
  DWINUI::Draw_String(HMI_data.Text_Color, HMI_data.Background_Color, 47, 192, buf);
}

void Draw_Print_ProgressRemain() {
  char buf[10];
  sprintf_P(buf, PSTR("%02i:%02i "), (uint16_t)(_remain_time / 3600), ((uint16_t)_remain_time % 3600) / 60);
  DWINUI::Draw_String(HMI_data.Text_Color, HMI_data.Background_Color, 181, 192, buf);
}

void ICON_ResumeOrPause() {
  if (checkkey == PrintProcess) printingIsPaused() ? ICON_Resume() : ICON_Pause();
}

// Update filename on print
void DWIN_Print_Header(const char *text = nullptr) {
  static char headertxt[31] = "";  // Print header text
  if (text) {
    const int8_t size = _MIN(30U, strlen_P(text));
    LOOP_L_N(i, size) headertxt[i] = text[i];
    headertxt[size] = '\0';
  }
  if (checkkey == PrintProcess || checkkey == PrintDone) {
    DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, 60, DWIN_WIDTH, 60+16);
    DWINUI::Draw_CenteredString(60, headertxt);
  }
}

void Draw_PrintProcess() {
  Title.ShowCaption(GET_TEXT_F(MSG_PRINTING));
  DWINUI::ClearMainArea();
  DWIN_Print_Header(SD_Printing() ? card.longest_filename() : nullptr);
  Draw_Print_Labels();
  DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
  DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
  ICON_Tune();
  ICON_ResumeOrPause();
  ICON_Stop();
}

void Goto_PrintProcess() {
  if (checkkey == PrintProcess)
    ICON_ResumeOrPause();
  else {
    checkkey = PrintProcess;
    Draw_PrintProcess();
    TERN_(DASH_REDRAW, DWIN_RedrawDash());
  }
  DWIN_UpdateLCD();
}

void Draw_PrintDone() {
  // show percent bar and value
  _percent_done = 100;
  _remain_time = 0;

  Title.ShowCaption(GET_TEXT_F(MSG_PRINT_DONE));
  DWINUI::ClearMainArea();
  DWIN_Print_Header(nullptr);
  if (TERN0(HAS_GCODE_PREVIEW, Preview_Valid())) {  // TODO: Revisar si se activa luego de ver un archivo en la SD y mandar a imprimir desde el host
    Preview_Show();
    DWINUI::Draw_Button(BTN_Continue, 86, 295);
    Draw_Select_Box(86, 295);
  }
  else {
    Draw_Print_ProgressBar();
    Draw_Print_Labels();
    DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
    DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
    Draw_Print_ProgressElapsed();
    Draw_Print_ProgressRemain();
    DWINUI::Draw_Button(BTN_Continue, 86, 273);
    Draw_Select_Box(86, 273);
  }
}

void Goto_PrintDone() {
  wait_for_user = true;
  if (checkkey != PrintDone) {
    checkkey = PrintDone;
    Draw_PrintDone();
    DWIN_UpdateLCD();
  }
}

void Draw_Main_Menu() {
  DWINUI::ClearMainArea();
  Title.ShowCaption(MACHINE_NAME);
  DWINUI::Draw_Icon(ICON_LOGO, 71, 52);   // CREALITY logo
  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  ICON_AdvSettings();
  TERN_(HAS_TOOLBAR, Draw_ToolBar());
}

void Goto_Main_Menu() {
  if (checkkey == MainMenu) return;
  checkkey = MainMenu;
  Draw_Main_Menu();
  DWIN_UpdateLCD();
}

// Draw X, Y, Z and blink if in an un-homed or un-trusted state
void _update_axis_value(const AxisEnum axis, const uint16_t x, const uint16_t y, const bool force) {
  const bool draw_qmark = axis_should_home(axis),
             draw_empty = NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING) && !draw_qmark && !axis_is_trusted(axis);

  // Check for a position change
  static xyz_pos_t oldpos = { -1, -1, -1 };

#define SHOW_REAL_POS 1

  #if ENABLED(SHOW_REAL_POS)
    const float p = stepper.position(axis) / planner.settings.axis_steps_per_mm[axis];
  #else
    const float p = current_position[axis];
  #endif

  const bool changed = oldpos[axis] != p;
  if (changed) oldpos[axis] = p;

  if (force || changed || draw_qmark || draw_empty) {
    if (blink && draw_qmark)
      DWINUI::Draw_String(HMI_data.Coordinate_Color, HMI_data.Background_Color, x, y, F("  - ? -"));
    else if (blink && draw_empty)
      DWINUI::Draw_String(HMI_data.Coordinate_Color, HMI_data.Background_Color, x, y, F("       "));
    else
      DWINUI::Draw_Signed_Float(HMI_data.Coordinate_Color, HMI_data.Background_Color, 3, 2, x, y, p);
  }
}

void _draw_ZOffsetIcon() {
  #if HAS_LEVELING
    if (planner.leveling_active) {
      DWIN_Draw_Rectangle(1, blink ? HMI_data.Selected_Color : HMI_data.Background_Color, 186, 415, 205, 436);
      DWINUI::Draw_Icon(ICON_Zoffset, 186, 416);
    }
    static bool _leveling_active = false;
    if (_leveling_active != planner.leveling_active) {
      _leveling_active = planner.leveling_active;
      if (!_leveling_active) {
        DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 186, 415, 205, 436);
        DWINUI::Draw_Icon(ICON_Zoffset, 186, 416);
      }
    }
  #else
    DWINUI::Draw_Icon(ICON_SetZOffset, 186, 416);
  #endif
}

void _draw_xyz_position(const bool force) {
  _update_axis_value(X_AXIS,  27, 457, force);
  _update_axis_value(Y_AXIS, 112, 457, force);
  _update_axis_value(Z_AXIS, 197, 457, force);
}

void update_variable() {
  TERN_(DEBUG_DWIN,DWINUI::Draw_Int(Color_Yellow, Color_Bg_Black, 2, DWIN_WIDTH-6*DWINUI::fontWidth(), 6, checkkey));
  TERN_(DEBUG_DWIN,DWINUI::Draw_Int(Color_Yellow, Color_Bg_Black, 2, DWIN_WIDTH-3*DWINUI::fontWidth(), 6, last_checkkey));

  #if HAS_HOTEND
    static celsius_t _hotendtemp = 0, _hotendtarget = 0;
    const celsius_t hc = thermalManager.wholeDegHotend(0),
                    ht = thermalManager.degTargetHotend(0);
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
    const bool _new_fanspeed = _fanspeed != thermalManager.fan_speed[0];
    if (_new_fanspeed) _fanspeed = thermalManager.fan_speed[0];
  #endif

  if (IsMenu(TuneMenu) || IsMenu(TemperatureMenu)) {
    // Tune page temperature update
    #if HAS_HOTEND
      if (_new_hotend_target) HotendTargetItem->redraw();
    #endif
    #if HAS_HEATED_BED
      if (_new_bed_target) BedTargetItem->redraw();
    #endif
    #if HAS_FAN
      if (_new_fanspeed) FanSpeedItem->redraw();
    #endif
  }

  // Bottom temperature update

  #if HAS_HOTEND
    if (_new_hotend_temp)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, _hotendtemp);
    if (_new_hotend_target)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);

    static int16_t _flow = planner.flow_percentage[0];
    if (_flow != planner.flow_percentage[0]) {
      _flow = planner.flow_percentage[0];
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, _flow);
    }
  #endif

  #if HAS_HEATED_BED
    if (_new_bed_temp)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, _bedtemp);
    if (_new_bed_target)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
  #endif

  static int16_t _feedrate = 100;
  if (_feedrate != feedrate_percentage) {
    _feedrate = feedrate_percentage;
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
  }

  #if HAS_FAN
    if (_new_fanspeed) {
      _fanspeed = thermalManager.fan_speed[0];
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
    }
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 204, 417, _offset);
  }

  _draw_ZOffsetIcon();
  _draw_xyz_position(false);
}

//void update_variable2() {

//}

/**
 * Memory card and file management
 */

bool DWIN_lcd_sd_status = false;

void SetMediaAutoMount() {
  HMI_data.MediaAutoMount = !HMI_data.MediaAutoMount;
  Show_Chkb_Line(CurrentMenu->line(), HMI_data.MediaAutoMount);
}

inline uint16_t nr_sd_menu_items() {
  return _MIN(card.get_num_Files() + !card.flag.workDirIsRoot, MENU_MAX_ITEMS);
}

void make_name_without_ext(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
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

void Redraw_SD_List() {
  InvalidateMenu();
  Draw_Print_File_Menu();
}

void SDCard_Up() {
  card.cdup();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

void SDCard_Folder(char * const dirname) {
  card.cd(dirname);
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

void onClickSDItem() {
  const uint16_t hasUpDir = !card.flag.workDirIsRoot;
  if (hasUpDir && CurrentMenu->selected == 1) return SDCard_Up();
  else {
    const uint16_t filenum = CurrentMenu->selected - 1 - hasUpDir;
    card.getfilename_sorted(SD_ORDER(filenum, card.get_num_Files()));

    // Enter that folder!
    if (card.flag.filenameIsDir) return SDCard_Folder(card.filename);

    if (card.fileIsBinary())
      return DWIN_Popup_Confirm(ICON_Error, F("Please check filenames"), F("Only G-code can be printed"));
    else
      return Goto_ConfirmToPrint();
  }
}

#if ENABLED(SCROLL_LONG_FILENAMES)
  char shift_name[LONG_FILENAME_LENGTH + 1] = "";

  void Draw_SDItem_Shifted(uint8_t &shift) {
    // Shorten to the available space
    const size_t lastchar = shift + MENU_CHAR_LIMIT;
    const char c = shift_name[lastchar];
    shift_name[lastchar] = '\0';

    const uint8_t row = FileMenu->line();
    Erase_Menu_Text(row);
    Draw_Menu_Line(row, 0, &shift_name[shift]);

    shift_name[lastchar] = c;
  }

  void FileMenuIdle(bool reset=false) {
    static bool hasUpDir = false;
    static uint8_t last_itemselected = 0;
    static int8_t shift_amt = 0;
    static int8_t shift_len = 0;
    if (reset) {
      last_itemselected = 0;
      hasUpDir = !card.flag.workDirIsRoot; // is a SubDir
      return;
    }
    const uint8_t selected = FileMenu->selected;
    if (last_itemselected != selected) {
      if (last_itemselected >= 1 + hasUpDir) FileMenu->Items()[last_itemselected]->redraw(true);
      last_itemselected = selected;
      if (selected >= 1 + hasUpDir) {
        const int8_t filenum = selected - 1 - hasUpDir; // Skip "Back" and ".."
        card.getfilename_sorted(SD_ORDER(filenum, card.get_num_Files()));
        make_name_without_ext(shift_name, card.longest_filename(), LONG_FILENAME_LENGTH);
        shift_len = strlen(shift_name);
        shift_amt = 0;
      }
    }
    else if ((selected >= 1 + hasUpDir) && (shift_len > MENU_CHAR_LIMIT)) {
      uint8_t shift_new = _MIN(shift_amt + 1, shift_len - MENU_CHAR_LIMIT); // Try to shift by...
      Draw_SDItem_Shifted(shift_new);             // Draw the item
      if (shift_new == shift_amt)                 // Scroll reached the end
        shift_new = -1;                           // Reset
      shift_amt = shift_new;                      // Set new scroll
    }
  }
#else
  char shift_name[FILENAME_LENGTH + 1] = "";
#endif

void onDrawFileName(MenuItemClass* menuitem, int8_t line) {
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && menuitem->pos == 1) {
    Draw_Menu_Line(line, ICON_Folder, "..");
  }
  else {
    uint8_t icon;
    card.getfilename_sorted(SD_ORDER(menuitem->pos - is_subdir - 1, card.get_num_Files()));
    make_name_without_ext(shift_name, card.longest_filename());
    icon = card.flag.filenameIsDir ? ICON_Folder : card.fileIsBinary() ? ICON_Binary : ICON_File;
    Draw_Menu_Line(line, icon, shift_name);
  }
}

void Draw_Print_File_Menu() {
  checkkey = Menu;
  if (card.isMounted()) {
    if (SET_MENU(FileMenu, MSG_MEDIA_MENU, nr_sd_menu_items() + 1)) {
      BACK_ITEM(Goto_Main_Menu);
      LOOP_L_N(i, nr_sd_menu_items()) {
        MenuItemAdd(onDrawFileName, onClickSDItem);
      }
    }
    UpdateMenu(FileMenu);
    TERN_(DASH_REDRAW, DWIN_RedrawDash());
  }
  else {
    if (SET_MENU(FileMenu, MSG_MEDIA_MENU, 1)) BACK_ITEM(Goto_Main_Menu);
    UpdateMenu(FileMenu);
    DWIN_Draw_Rectangle(1, HMI_data.AlertBg_Color, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::Draw_CenteredString(font12x24, HMI_data.AlertTxt_Color, MBASE(3), GET_TEXT_F(MSG_MEDIA_NOT_INSERTED));
  }
  TERN_(SCROLL_LONG_FILENAMES, FileMenuIdle(true));
}

//
// Watch for media mount / unmount
//
void HMI_SDCardUpdate() {
  if (HMI_flag.home_flag) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    if (IsMenu(FileMenu)) Redraw_SD_List();
    if (!DWIN_lcd_sd_status && SD_Printing()) ui.abort_print();  // Media removed while printing
    DWIN_UpdateLCD();
  }
}

/**
 * Dash board and indicators
 */

void DWIN_Draw_Dashboard() {

  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, STATUS_Y + 21, DWIN_WIDTH, DWIN_HEIGHT - 1);

  #if HAS_HOTEND
    DWINUI::Draw_Icon(ICON_HotendTemp, 10, 384);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, thermalManager.wholeDegHotend(0));
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(0));

    DWINUI::Draw_Icon(ICON_StepE, 112, 417);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWINUI::Draw_Icon(ICON_BedTemp, 10, 417);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, thermalManager.wholeDegBed());
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWINUI::Draw_Icon(ICON_Speed, 113, 384);
  DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, F("%"));

  #if HAS_FAN
    DWINUI::Draw_Icon(ICON_FanSpeed, 188, 384);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWINUI::Draw_Icon(planner.leveling_active ? ICON_SetZOffset : ICON_Zoffset, 188, 417);
  #endif

  DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 204, 417, BABY_Z_VAR);

  DWIN_Draw_Rectangle(1, HMI_data.Bottom_Color, 0, 449, DWIN_WIDTH, 450);

  DWINUI::Draw_Icon(ICON_MaxSpeedX,  10, 454);
  DWINUI::Draw_Icon(ICON_MaxSpeedY,  95, 454);
  DWINUI::Draw_Icon(ICON_MaxSpeedZ, 180, 454);
  _draw_xyz_position(true);

DWIN_Draw_Rectangle(1, HMI_data.Bottom_Color, 0, 478, DWIN_WIDTH, 479); //changed added 2nd line

}

void Draw_Info_Menu() {
  DWINUI::ClearMainArea();
  Title.ShowCaption(GET_TEXT_F(MSG_INFO_SCREEN));
  Draw_Menu_Line(0, ICON_Back, GET_TEXT_F(MSG_BACK), false, true);
  char machine_size[21];
  machine_size[0] = '\0';
  sprintf_P(machine_size, PSTR("%ix%ix%i"), (int16_t)X_BED_SIZE, (int16_t)Y_BED_SIZE, (int16_t)Z_MAX_POS);

  DWINUI::Draw_CenteredString(92,  GET_TEXT_F(MSG_INFO_MACHINENAME));
  DWINUI::Draw_CenteredString(112, F(MACHINE_NAME));
  DWINUI::Draw_CenteredString(145, GET_TEXT_F(MSG_INFO_SIZE));
  DWINUI::Draw_CenteredString(165, machine_size);

  #if ProUIex
    ProEx.Init();
  #else
    DWINUI::Draw_CenteredString(198, GET_TEXT_F(MSG_INFO_FWVERSION));
    DWINUI::Draw_CenteredString(218, F(SHORT_BUILD_VERSION));
    DWINUI::Draw_CenteredString(251, GET_TEXT_F(MSG_INFO_BUILD));
    DWINUI::Draw_CenteredString(271, F(DateTime));
  #endif

    LOOP_L_N(i, TERN(ProUIex, 2, 4)) {
    DWINUI::Draw_Icon(ICON_Step + i, ICOX, 90 + i * MLINE);
    DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(i + 2), 240);
  }
}

// Main Process
void HMI_MainMenu() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_page.inc(PAGE_COUNT)) {
      switch (select_page.now) {
        case PAGE_PRINT: ICON_Print(); break;
        case PAGE_PREPARE: ICON_Print(); ICON_Prepare(); break;
        case PAGE_CONTROL: ICON_Prepare(); ICON_Control(); break;
        case PAGE_ADVANCE: ICON_Control(); ICON_AdvSettings(); break;
        TERN_(HAS_TOOLBAR, case PAGE_TOOLBAR: ICON_AdvSettings(); Goto_ToolBar();  break);
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_page.dec()) {
      switch (select_page.now) {
        case PAGE_PRINT: ICON_Print(); ICON_Prepare(); break;
        case PAGE_PREPARE: ICON_Prepare(); ICON_Control(); break;
        case PAGE_CONTROL: ICON_Control(); ICON_AdvSettings(); break;
        case PAGE_ADVANCE: ICON_AdvSettings(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_page.now) {
      case PAGE_PRINT:
        if (HMI_data.MediaAutoMount) {
          card.mount();
          safe_delay(800);
        };
        Draw_Print_File_Menu();
        break;
      case PAGE_PREPARE: Draw_Prepare_Menu(); break;
      case PAGE_CONTROL: Draw_Control_Menu(); break;
      case PAGE_ADVANCE: Draw_AdvancedSettings_Menu(); break;
    }
  }
  DWIN_UpdateLCD();
}

// Pause or Stop popup
void onClick_PauseOrStop() {
  switch (select_print.now) {
    case PRINT_PAUSE_RESUME: if (HMI_flag.select_flag) ui.pause_print(); break; // confirm pause
    case PRINT_STOP: if (HMI_flag.select_flag) ui.abort_print(); break; // stop confirmed then abort print
    default: break;
  }
  return Goto_PrintProcess();
}

// Printing
void HMI_Printing() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_print.inc(PRINT_COUNT)) {
      switch (select_print.now) {
        case PRINT_SETUP: ICON_Tune(); break;
        case PRINT_PAUSE_RESUME: ICON_Tune(); ICON_ResumeOrPause(); break;
        case PRINT_STOP: ICON_ResumeOrPause(); ICON_Stop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_print.dec()) {
      switch (select_print.now) {
        case PRINT_SETUP: ICON_Tune(); ICON_ResumeOrPause(); break;
        case PRINT_PAUSE_RESUME: ICON_ResumeOrPause(); ICON_Stop(); break;
        case PRINT_STOP: ICON_Stop(); break;
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_print.now) {
      case PRINT_SETUP: Draw_Tune_Menu(); break;
      case PRINT_PAUSE_RESUME:
        if (printingIsPaused()) {  // if printer is already in pause
          ui.resume_print();
          break;
        }
        else
          return Goto_Popup(Popup_window_PauseOrStop, onClick_PauseOrStop);
      case PRINT_STOP:
        return Goto_Popup(Popup_window_PauseOrStop, onClick_PauseOrStop);
      default: break;
    }
  }
  DWIN_UpdateLCD();
}

#include "../../../libs/buzzer.h"

void Draw_Main_Area() {
  switch (checkkey) {
    case MainMenu:               Draw_Main_Menu(); break;
    case PrintProcess:           Draw_PrintProcess(); break;
    case PrintDone:              Draw_PrintDone(); break;
    #if HAS_ESDIAG
      case ESDiagProcess:        Draw_EndStopDiag(); break;
    #endif
    case Popup:                  Draw_Popup(); break;
    #if HAS_LOCKSCREEN
      case Locked:               lockScreen.draw(); break;
    #endif
    case Menu:
      #if HAS_TOOLBAR
        if (CurrentMenu == &ToolBar) Draw_Main_Menu(); else ReDrawMenu();
      #else
        ReDrawMenu();
      #endif
      break;
    case SetInt:
    case SetPInt:
    case SetIntNoDraw:
    case SetFloat:
    case SetPFloat:              ReDrawMenu(true); break;
    default: break;
  }
}

void HMI_ReturnScreen() {
  checkkey = last_checkkey;
  wait_for_user = false;
  Draw_Main_Area();
}

void HMI_WaitForUser() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState != ENCODER_DIFF_NO && !ui.backlight) {
    if (checkkey == WaitResponse) HMI_ReturnScreen();
    return ui.refresh_brightness();
  }
  if (!wait_for_user) {
    switch (checkkey) {
      case PrintDone:
        select_page.reset();
        Goto_Main_Menu();
        break;
      #if HAS_BED_PROBE
        case Leveling:
          #if ProUIex
            ProEx.StopLeveling();
          #else
            HMI_ReturnScreen();
          #endif
          break;
      #endif
      default:
        HMI_ReturnScreen();
        break;
    }
  }
}

// Draws boot screen
void HMI_Init() {
  DWINUI::Draw_Box(1, Color_Black, {5, 220, DWIN_WIDTH-5, DWINUI::fontHeight()});
  DWINUI::Draw_CenteredString(3, Color_White, 220, F(BOOT_MACHINE_NAME));
  for (uint16_t t = 15; t <= 257; t += 10) {
    DWINUI::Draw_Icon(ICON_Bar, 15, 260);
    DWIN_Draw_Rectangle(1, HMI_data.Background_Color, t, 260, 257, 280);
    DWIN_UpdateLCD();
    delay(50);
  }
}

void EachMomentUpdate() {
  static millis_t next_var_update_ms = 0, next_rts_update_ms = 0, next_status_update_ms = 0;
  const millis_t ms = millis();

  #if LCD_BACKLIGHT_TIMEOUT_MINS
    if (ui.backlight_off_ms && ELAPSED(ms, ui.backlight_off_ms)) {
      TurnOffBacklight(); ; // Backlight off
      ui.backlight_off_ms = 0;
    }
  #endif

  if (ELAPSED(ms, next_var_update_ms)) {
    next_var_update_ms = ms + DWIN_VAR_UPDATE_INTERVAL;
    blink = !blink;
    update_variable();
    #if HAS_ESDIAG
      if (checkkey == ESDiagProcess) ESDiag.Update();
    #endif
    #if HAS_PIDPLOT
      if (checkkey == PidProcess) Plot.Update((HMI_value.pidresult == PID_EXTR_START) ? thermalManager.wholeDegHotend(0) : thermalManager.wholeDegBed());
      if (checkkey == MPCProcess) Plot.Update(thermalManager.wholeDegHotend(0));
      if (checkkey == PlotProcess) Plot.Update(htemp ? thermalManager.wholeDegHotend(0) : thermalManager.wholeDegBed());
    #endif
  }

  #if HAS_STATUS_MESSAGE_TIMEOUT
    bool did_expire = ui.status_reset_callback && (*ui.status_reset_callback)();
    did_expire |= ui.status_message_expire_ms && ELAPSED(ms, ui.status_message_expire_ms);
    if (did_expire) ui.reset_status();
  #endif

  if (ELAPSED(ms, next_status_update_ms)) {
    next_status_update_ms = ms + DWIN_VAR_UPDATE_INTERVAL;
    DWIN_DrawStatusMessage();
    #if ENABLED(SCROLL_LONG_FILENAMES)
      if (IsMenu(FileMenu)) FileMenuIdle();
    #endif
  }

  if (!PENDING(ms, next_rts_update_ms)) {
    next_rts_update_ms = ms + DWIN_UPDATE_INTERVAL;

    if ((Printing() != HMI_flag.printing_flag) && !HMI_flag.home_flag) {
      HMI_flag.printing_flag = Printing();
      TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("printing_flag: ", HMI_flag.printing_flag));
      if (HMI_flag.printing_flag)
        DWIN_Print_Started();
      else if (HMI_flag.abort_flag)
        DWIN_Print_Aborted();
      else
        DWIN_Print_Finished();
    }

    if ((printingIsPaused() != HMI_flag.pause_flag) && !HMI_flag.home_flag) {
      HMI_flag.pause_flag = printingIsPaused();
      TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("pause_flag: ", HMI_flag.pause_flag));
      if (HMI_flag.pause_flag)
        DWIN_Print_Pause();
      else if (HMI_flag.abort_flag)
        DWIN_Print_Aborted();
      else
        DWIN_Print_Resume();
    }

    if (checkkey == PrintProcess) { // print process

      duration_t elapsed = print_job_timer.duration(); // print timer

      if (card.isPrinting() && !HMI_flag.percent_flag) {
        uint8_t percentDone = card.percentDone();
        if (_percent_done != percentDone) { // print percent
            _percent_done = percentDone;
            Draw_Print_ProgressBar();
        }

        // Estimate remaining time every 20 seconds
        static millis_t next_remain_time_update = 0;
        if (_percent_done > 1 && ELAPSED(ms, next_remain_time_update) && !HMI_flag.heat_flag && !HMI_flag.remain_flag) {
          _remain_time = (elapsed.value - dwin_heat_time) / (_percent_done * 0.01f) - (elapsed.value - dwin_heat_time);
          next_remain_time_update += DWIN_REMAIN_TIME_UPDATE_INTERVAL;
          Draw_Print_ProgressRemain();
        }
      }

      // Print time so far
      static uint16_t last_Printtime = 0;
      const uint16_t min = (elapsed.value % 3600) / 60;
      if (last_Printtime != min) { // 1 minute update
        last_Printtime = min;
        Draw_Print_ProgressElapsed();
      }

    }
    #if ENABLED(POWER_LOSS_RECOVERY)
      else if (DWIN_lcd_sd_status && recovery.dwin_flag) { // resume print before power off
        return Goto_PowerLossRecovery();
      }
    #endif // POWER_LOSS_RECOVERY
  }
  DWIN_UpdateLCD();
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void Popup_PowerLossRecovery() {
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 70, GET_TEXT_F(MSG_OUTAGE_RECOVERY));
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 147, F("It looks like the last"));
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 167, F("file was interrupted."));
    DWINUI::Draw_Button(BTN_Cancel,    26, 280);
    DWINUI::Draw_Button(BTN_Continue, 146, 280);
    SdFile *dir = nullptr;
    const char * const filename = card.diveToFile(true, dir, recovery.info.sd_filename);
    card.selectFileByName(filename);
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 207, card.longest_filename());
    Draw_Select_Highlight(HMI_flag.select_flag);
    DWIN_UpdateLCD();
  }

  void onClick_PowerLossRecovery() {
    if (HMI_flag.select_flag) {
      queue.inject(F("M1000C"));
      select_page.reset();
      return Goto_Main_Menu();
    }
    else {
      select_print.set(PRINT_SETUP);
      queue.inject(F("M1000"));
    }
  }

  void Goto_PowerLossRecovery() {
    recovery.dwin_flag = false;
    LCD_MESSAGE(MSG_CONTINUE_PRINT_JOB);
    Goto_Popup(Popup_PowerLossRecovery, onClick_PowerLossRecovery);
  }

#endif // POWER_LOSS_RECOVERY


void DWIN_HandleScreen() {
  switch (checkkey) {
    case MainMenu:        HMI_MainMenu(); break;
    case Menu:            HMI_Menu(); break;
    case SetInt:          HMI_SetDraw(); break;
    case SetFloat:        HMI_SetDraw(); break;
    case SetPInt:         HMI_SetPInt(); break;
    case SetPFloat:       HMI_SetPFloat(); break;
    case SetIntNoDraw:    HMI_SetNoDraw(); break;
    case PrintProcess:    HMI_Printing(); break;
    case Popup:           HMI_Popup(); break;
    case Leveling:        TERN_(ProUIex, HMI_WaitForUser();) break;
    #if HAS_LOCKSCREEN
      case Locked:        HMI_LockScreen(); break;
    #endif
    case PrintDone:
    TERN_(HAS_ESDIAG, case ESDiagProcess:)
    case WaitResponse:    HMI_WaitForUser(); break;
    case PlotProcess:     HMI_WaitForUser(); break;
    case Homing:
    case PidProcess:
    case NothingToDo:     break;
    default: break;
  }
}

bool IDisPopUp() {    // If ID is popup...
  return  (checkkey == NothingToDo)
       || (checkkey == WaitResponse)
       || (checkkey == Homing)
       || (checkkey == Leveling)
       || (checkkey == PidProcess)
       || (checkkey == PlotProcess)
       || TERN0(HAS_ESDIAG, (checkkey == ESDiagProcess))
       || (checkkey == Popup);
}

void HMI_SaveProcessID(const uint8_t id) {
  if (checkkey != id) {
    if (!IDisPopUp()) last_checkkey = checkkey; // if previous is not a popup
    if ((id == Popup)
         || TERN0(HAS_ESDIAG, (id == ESDiagProcess))
         || (id == PrintDone)
         || (id == Leveling)
         || (id == PlotProcess)
         || (id == WaitResponse)) wait_for_user = true;
    checkkey = id;
  }
}

void DWIN_HomingStart() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_HomingStart"));
  HMI_flag.home_flag = true;
  HMI_SaveProcessID(Homing);
  Title.ShowCaption(GET_TEXT_F(MSG_HOMING));
  DWIN_Show_Popup(ICON_BLTouch, GET_TEXT_F(MSG_HOMING), GET_TEXT_F(MSG_PLEASE_WAIT));
}

void DWIN_HomingDone() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_HomingDone"));
  HMI_flag.home_flag = false;
  #if DISABLED(HAS_BED_PROBE) && EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
    planner.synchronize();
    babystep.add_mm(Z_AXIS, HMI_data.ManualZOffset);
  #endif
  HMI_ReturnScreen();
}

void DWIN_LevelingStart() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_LevelingStart"));
  #if HAS_BED_PROBE
    HMI_SaveProcessID(Leveling);
    TERN_(ProUIex,HMI_flag.cancel_abl = 0);
    Title.ShowCaption(GET_TEXT_F(MSG_BED_LEVELING));
    DWIN_Show_Popup(ICON_AutoLeveling, GET_TEXT_F(MSG_BED_LEVELING), GET_TEXT_F(MSG_PLEASE_WAIT), TERN(ProUIex, BTN_Cancel, 0));
    #if BOTH(AUTO_BED_LEVELING_UBL, PREHEAT_BEFORE_LEVELING)
// Change to
      // #if ENABLED(PREHEAT_BEFORE_LEVELING)
      //   if (!abl.dryrun) probe.preheat_for_probing(LEVELING_NOZZLE_TEMP, LEVELING_BED_TEMP);
      // #endif
// and test.

      #if HAS_HOTEND
        if (thermalManager.degTargetHotend(0) < LEVELING_NOZZLE_TEMP)
          thermalManager.setTargetHotend(LEVELING_NOZZLE_TEMP, 0);
      #endif
      #if HAS_HEATED_BED
        if (thermalManager.degTargetBed() < HMI_data.BedLevT)
          thermalManager.setTargetBed(HMI_data.BedLevT);
      #endif
      TERN_(HAS_HOTEND, thermalManager.wait_for_hotend(0));
      TERN_(HAS_HEATED_BED, thermalManager.wait_for_bed_heating());
    #endif
  #elif ENABLED(MESH_BED_LEVELING)
    Draw_EditMesh_Menu();
  #endif
}

void DWIN_LevelingDone() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_LevelingDone"));
  #if HAS_MESH
    #if ProUIex && HAS_BED_PROBE
      if (ProEx.LevelingDone()) Goto_MeshViewer();
    #else
      Goto_MeshViewer();
    #endif
  #endif
}

#if HAS_MESH
  void DWIN_MeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval) {
    char msg[33] = "";
    char str_1[6] = "";
    sprintf_P(msg, PSTR(S_FMT " %i/%i Z=%s"), GET_TEXT(MSG_PROBING_POINT), cpos, tpos, dtostrf(zval, 1, 2, str_1));
    ui.set_status(msg);
  }
#endif

// PID/MPC process

#if HAS_PIDPLOT && ANY(HAS_PID_HEATING, MPCTEMP)
  celsius_t _maxtemp, _target;
  void DWIN_Draw_PID_MPC_Popup() {
    frame_rect_t gfrm = {30, 150, DWIN_WIDTH - 60, 160};
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    #if ENABLED(MPCTEMP)
      switch (HMI_value.pidresult) {
        case MPCTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[0];
          _target = 200;
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 100, GET_TEXT_F(MSG_MPC_AUTOTUNE));
          DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("MPC target:    Celsius"));
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 120, F("for Nozzle is running."));
          break;
        case PID_BED_START:
          _maxtemp = BED_MAXTEMP;
          _target = HMI_data.BedPidT;
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 100, GET_TEXT_F(MSG_PID_AUTOTUNE));
          DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("PID target:    Celsius"));
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 120, F("for BED is running."));
          break;
        default:
          return;
      }
    #else
      DWINUI::Draw_CenteredString(2, HMI_data.PopupTxt_Color, 70, GET_TEXT_F(MSG_PID_AUTOTUNE));
      DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("PID target:     Celsius"));
      switch (HMI_value.pidresult) {
        case PID_EXTR_START:
          _maxtemp = thermalManager.hotend_maxtemp[0];
          _target = HMI_data.HotendPidT;
          DWINUI::Draw_CenteredString(2, HMI_data.PopupTxt_Color, 92, F("for NOZZLE"));
          break;
        case PID_BED_START:
          _maxtemp = BED_MAXTEMP;
          _target = HMI_data.BedPidT;
          DWINUI::Draw_CenteredString(2, HMI_data.PopupTxt_Color, 92, F("for BED"));
          break;
        default:
          return;
      }
    #endif
    Plot.Draw(gfrm, _maxtemp, _target);
    DWINUI::Draw_Int(false, 2, HMI_data.StatusTxt_Color, HMI_data.PopupBg_color, 3, gfrm.x + 92, gfrm.y - DWINUI::fontHeight() - 6, _target);
  }
#endif

#if HAS_PID_HEATING
  void DWIN_PidTuning(tempcontrol_t result) {
    HMI_value.pidresult = result;
    switch (result) {
      case PID_BED_START:
        HMI_SaveProcessID(PidProcess);
        #if HAS_PIDPLOT
          DWIN_Draw_PID_MPC_Popup();
        #else
          DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for BED is running."));
        #endif
        break;
      case PID_EXTR_START:
        HMI_SaveProcessID(PidProcess);
        #if HAS_PIDPLOT
          DWIN_Draw_PID_MPC_Popup();
        #else
          DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for Nozzle is running."));
        #endif
        break;
      case PID_BAD_EXTRUDER_NUM:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_BAD_EXTRUDER_NUM));
        break;
      case PID_TUNING_TIMEOUT:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), GET_TEXT_F(MSG_PID_TIMEOUT));
        break;
      case PID_TEMP_TOO_HIGH:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_TEMP_TOO_HIGH));
        break;
      case PID_DONE:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
        break;
      default:
        checkkey = last_checkkey;
        break;
    }
  }
#endif

#if ENABLED(MPCTEMP)
  void DWIN_MPCTuning(tempcontrol_t result) {
    HMI_value.pidresult = result;
    switch (result) {
      case MPCTEMP_START:
        HMI_SaveProcessID(MPCProcess);
        #if HAS_PIDPLOT
          DWIN_Draw_PID_MPC_Popup();
        #else
          DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_MPC_AUTOTUNE), F("for Nozzle is running."));
        #endif
        break;
      case MPC_TEMP_ERROR:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), F(STR_MPC_TEMPERATURE_ERROR));
        ui.reset_alert_level();
        break;
      case MPC_INTERRUPTED:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), F(STR_MPC_AUTOTUNE STR_MPC_AUTOTUNE_INTERRUPTED));
        ui.reset_alert_level();
        break;
      case MPC_DONE:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_MPC_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
        ui.reset_alert_level();
        break;
      default:
        checkkey = last_checkkey;
        ui.reset_alert_level();
        break;
    }
  }
#endif

//Temperature (PID Tuning Graph) Plot During Printing

#if HAS_PIDPLOT && HAS_TEMP_SENSOR
  void DWIN_Draw_Plot_Nozzle() {
    HMI_SaveProcessID(PlotProcess);
    htemp = 1;
    frame_rect_t gfrm = {30, 135, DWIN_WIDTH - 60, 160};
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    DWINUI::Draw_CenteredString(3, HMI_data.PopupTxt_Color, 75, F("Nozzle Temperature"));
    DWIN_Draw_String(false, 2, HMI_data.PopupTxt_Color, HMI_data.PopupBg_color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("Target:     Celsius"));
    _maxtemp = thermalManager.hotend_maxtemp[0];
    _target = thermalManager.temp_hotend[0].target;
    Plot.Draw(gfrm, _maxtemp, _target);
    DWINUI::Draw_Int(false, 2, HMI_data.StatusTxt_Color, HMI_data.PopupBg_color, 3, gfrm.x + 80, gfrm.y - DWINUI::fontHeight() - 4, _target);
    DWINUI::Draw_Button(BTN_Continue, 86, 305);
    Draw_Select_Box(86, 305);
    DWIN_UpdateLCD();
  }
#endif

#if HAS_PIDPLOT && HAS_HEATED_BED
  void DWIN_Draw_Plot_Bed() {
    HMI_SaveProcessID(PlotProcess);
    htemp = 0;
    frame_rect_t gfrm = {30, 135, DWIN_WIDTH - 60, 160};
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    DWINUI::Draw_CenteredString(3, HMI_data.PopupTxt_Color, 75, F("Bed Temperature"));
    DWIN_Draw_String(false, 2, HMI_data.PopupTxt_Color, HMI_data.PopupBg_color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("Target:     Celsius"));
    _maxtemp = BED_MAXTEMP;
    _target = thermalManager.temp_bed.target;
    Plot.Draw(gfrm, _maxtemp, _target);
    DWINUI::Draw_Int(false, 2, HMI_data.StatusTxt_Color, HMI_data.PopupBg_color, 3, gfrm.x + 80, gfrm.y - DWINUI::fontHeight() - 4, _target);
    DWINUI::Draw_Button(BTN_Continue, 86, 305);
    Draw_Select_Box(86, 305);
    DWIN_UpdateLCD();
  }
#endif

// Started a Print Job
void DWIN_Print_Started() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_Print_Started: ", SD_Printing()));
  TERN_(HAS_GCODE_PREVIEW, if (Host_Printing()) Preview_Invalidate());
  _percent_done = 0;
  _remain_time = 0;
  HMI_flag.percent_flag = false;
  HMI_flag.remain_flag = false;
  HMI_flag.pause_flag = false;
  HMI_flag.abort_flag = false;
  select_print.reset();
  if (!HMI_flag.config_flag) Goto_PrintProcess();
}

// Pause a print job
void DWIN_Print_Pause() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_Print_Pause"));
  ICON_ResumeOrPause();
}

// Resume print job
void DWIN_Print_Resume() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_Print_Resume"));
  ICON_ResumeOrPause();
  LCD_MESSAGE(MSG_RESUME_PRINT);
}

// Ended print job
void DWIN_Print_Finished() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_Print_Finished"));
  TERN_(POWER_LOSS_RECOVERY, if (card.isPrinting()) recovery.cancel());
  HMI_flag.pause_flag = false;
  wait_for_heatup = false;
  planner.finish_and_disable();
  thermalManager.cooldown();
  if (!HMI_flag.config_flag) Goto_PrintDone(); else HMI_flag.config_flag = false;
}

// Print was aborted
void DWIN_Print_Aborted() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_Print_Aborted"));
  #if ProUIex
    char cmd[21] = "";
    sprintf_P(cmd, PSTR("G0 F3000 Y%i\nM84"), PRO_data.Park_point.y);
    queue.inject(cmd);
  #endif
  DWIN_Print_Finished();
}

// Progress and remaining time update
void DWIN_M73() {
  if (parser.seenval('P')) {
    _percent_done = parser.value_byte();
    HMI_flag.percent_flag = true;
  }
  if (parser.seenval('R')) {
    _remain_time = parser.value_ulong() * 60;
    HMI_flag.remain_flag = true;
  }
  if (checkkey == PrintProcess) {
    Draw_Print_ProgressBar();
    Draw_Print_ProgressRemain();
    Draw_Print_ProgressElapsed();
  }
}

#if HAS_FILAMENT_SENSOR
  // Filament Runout process
  void DWIN_FilamentRunout(const uint8_t extruder) { LCD_MESSAGE(MSG_RUNOUT_SENSOR); }
#endif

void DWIN_SetColorDefaults() {
  HMI_data.Background_Color = Def_Background_Color;
  HMI_data.Cursor_color     = Def_Cursor_color;
  HMI_data.TitleBg_color    = Def_TitleBg_color;
  HMI_data.TitleTxt_color   = Def_TitleTxt_color;
  HMI_data.Text_Color       = Def_Text_Color;
  HMI_data.Selected_Color   = Def_Selected_Color;
  HMI_data.SplitLine_Color  = Def_SplitLine_Color;
  HMI_data.Highlight_Color  = Def_Highlight_Color;
  HMI_data.StatusBg_Color   = Def_StatusBg_Color;
  HMI_data.StatusTxt_Color  = Def_StatusTxt_Color;
  HMI_data.PopupBg_color    = Def_PopupBg_color;
  HMI_data.PopupTxt_Color   = Def_PopupTxt_Color;
  HMI_data.AlertBg_Color    = Def_AlertBg_Color;
  HMI_data.AlertTxt_Color   = Def_AlertTxt_Color;
  HMI_data.PercentTxt_Color = Def_PercentTxt_Color;
  HMI_data.Barfill_Color    = Def_Barfill_Color;
  HMI_data.Indicator_Color  = Def_Indicator_Color;
  HMI_data.Coordinate_Color = Def_Coordinate_Color;
  HMI_data.Bottom_Color     = Def_Bottom_Color;
}

void DWIN_SetDataDefaults() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_SetDataDefaults"));
  DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.TitleBg_color);
  TERN_(PIDTEMP, HMI_data.HotendPidT = DEF_HOTENDPIDT);
  TERN_(PIDTEMPBED, HMI_data.BedPidT = DEF_BEDPIDT);
  TERN_(HAS_PID_HEATING, HMI_data.PidCycles = DEF_PIDCYCLES);
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    HMI_data.ExtMinT = EXTRUDE_MINTEMP;
    ApplyExtMinT();
  #endif
  #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    HMI_data.BedLevT = LEVELING_BED_TEMP;
  #endif
  TERN_(BAUD_RATE_GCODE, HMI_data.Baud250K = (BAUDRATE == 250000));
  HMI_data.FullManualTramming = false;
  HMI_data.MediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  HMI_data.SetLiveMove = false;
  #if BOTH(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    HMI_data.z_after_homing = DEF_Z_AFTER_HOMING;
  #endif
  #if DISABLED(HAS_BED_PROBE)
    HMI_data.ManualZOffset = 0;
  #endif
  #if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_default();
    ApplyLEDColor();
  #endif
  #if ProUIex
    PRO_data.x_bed_size = DEF_X_BED_SIZE;
    PRO_data.y_bed_size = DEF_Y_BED_SIZE;
    PRO_data.x_min_pos  = DEF_X_MIN_POS;
    PRO_data.y_min_pos  = DEF_Y_MIN_POS;
    PRO_data.x_max_pos  = DEF_X_MAX_POS;
    PRO_data.y_max_pos  = DEF_Y_MAX_POS;
    PRO_data.z_max_pos  = DEF_Z_MAX_POS;
    #if HAS_MESH
      PRO_data.grid_max_points = DEF_GRID_MAX_POINTS;
      PRO_data.mesh_min_x = DEF_MESH_MIN_X;
      PRO_data.mesh_max_x = DEF_MESH_MAX_Y;
      PRO_data.mesh_min_y = DEF_MESH_MIN_Y;
      PRO_data.mesh_max_y = DEF_MESH_MAX_X;
    #endif
    #if HAS_BED_PROBE
      PRO_data.zprobefeedslow = DEF_Z_PROBE_FEEDRATE_SLOW;
      PRO_data.multiple_probing = MULTIPLE_PROBING;
    #endif
    TERN_(HAS_EXTRUDERS, PRO_data.Invert_E0 = DEF_INVERT_E0_DIR);
    #if ENABLED(NOZZLE_PARK_FEATURE)
      PRO_data.Park_point = DEF_NOZZLE_PARK_POINT;
    #endif
    #if HAS_FILAMENT_SENSOR
      PRO_data.Runout_active_state = FIL_RUNOUT_STATE;
      PRO_data.FilamentMotionSensor = DEF_FIL_MOTION_SENSOR;
    #endif
    #if HAS_TOOLBAR
      const uint8_t _def[] = DEF_TBOPT;
      LOOP_L_N(i,TBMaxOpt) PRO_data.TBopt[i] = _def[i];
    #endif
    PRO_data.hotend_maxtemp = HEATER_0_MAXTEMP;
    ProEx.SetData();
  #endif
}

void DWIN_CopySettingsTo(char * const buff) {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_CopySettingsTo"));
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("HMI_data: ", sizeof(HMI_data_t)));
  memcpy(buff, &HMI_data, sizeof(HMI_data_t));
  #if ProUIex
    TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("PRO_data: ", sizeof(PRO_data_t)));
    memcpy(buff + sizeof(HMI_data_t), &PRO_data, sizeof(PRO_data_t));
  #endif
}

void DWIN_CopySettingsFrom(const char * const buff) {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_CopySettingsFrom"));
  memcpy(&HMI_data, buff, sizeof(HMI_data_t));
  TERN_(ProUIex, memcpy(&PRO_data, buff + sizeof(HMI_data_t), sizeof(PRO_data)));
  if (HMI_data.Text_Color == HMI_data.Background_Color) DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.TitleBg_color);
  TERN_(PREVENT_COLD_EXTRUSION, ApplyExtMinT());
  feedrate_percentage = 100;
  TERN_(BAUD_RATE_GCODE, if (HMI_data.Baud250K) SetBaud250K(); else SetBaud115K());
  #if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_color(
      (HMI_data.LED_Color >> 16) & 0xFF,
      (HMI_data.LED_Color >>  8) & 0xFF,
      (HMI_data.LED_Color >>  0) & 0xFF
      OPTARG(HAS_WHITE_LED, (HMI_data.LED_Color >> 24) & 0xFF)
    );
  #endif
  TERN_(ProUIex, ProEx.LoadSettings());
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("MarlinUI::init_lcd"));
  delay(750);   // wait to wakeup screen
  const bool hs = DWIN_Handshake(); UNUSED(hs);
  #if ENABLED(DEBUG_DWIN)
    SERIAL_ECHOPGM("DWIN_Handshake ");
    SERIAL_ECHOLNF(hs ? F("ok.") : F("error."));
  #endif
  DWIN_Frame_SetDir(1);
  Encoder_Configuration();
}

void DWIN_InitScreen() {
  TERN_(DEBUG_DWIN, SERIAL_ECHOLNPGM("DWIN_InitScreen"));
  HMI_Init();
  #if ProUIex
    DWIN_UpdateLCD();
    ProEx.Init();
    safe_delay(2000);
  #endif
  DWINUI::init();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.TitleBg_color);
  DWINUI::onTitleDraw = Draw_Title;
  InitMenu();
  checkkey = 255;
  hash_changed = true;
  last_E = 0;
  DWIN_DrawStatusLine();
  DWIN_Draw_Dashboard();
  Goto_Main_Menu();
}

void MarlinUI::update() {
  EachMomentUpdate();   // Status update
  HMI_SDCardUpdate();   // SD card update
  DWIN_HandleScreen();  // Rotary encoder update
}

void MarlinUI::refresh() { /* Nothing to see here */ }

#if HAS_LCD_BRIGHTNESS
  void MarlinUI::_set_brightness() { DWIN_LCD_Brightness(backlight ? brightness : 0); }
#endif

void MarlinUI::kill_screen(FSTR_P const lcd_error, FSTR_P const lcd_component) {
  DWIN_Draw_Popup(ICON_BLTouch, GET_TEXT_F(MSG_PRINTER_KILLED), lcd_error);
  DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 270, GET_TEXT_F(MSG_TURN_OFF));
  DWIN_UpdateLCD();
}

void DWIN_RebootScreen() {
  DWIN_Frame_Clear(Color_Bg_Black);
  DWIN_JPG_ShowAndCache(0);
  DWINUI::Draw_CenteredString(Color_White, 220, GET_TEXT_F(MSG_PLEASE_WAIT_REBOOT));
  DWIN_UpdateLCD();
  safe_delay(500);
}

void DWIN_RedrawDash() {
  hash_changed = true;
  DWIN_DrawStatusMessage();
  DWIN_Draw_Dashboard();
}

void DWIN_RedrawScreen() {
  Draw_Main_Area();
  DWIN_RedrawDash();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void DWIN_Popup_Pause(FSTR_P const fmsg, uint8_t button /*= 0*/) {
    HMI_SaveProcessID(button ? WaitResponse : NothingToDo);
    DWIN_Show_Popup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), fmsg, button);
  }

  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t extruder/*=active_extruder*/) {
    //if (mode == PAUSE_MODE_SAME) return;
    pause_mode = mode;
    switch (message) {
      case PAUSE_MESSAGE_PARKING:  DWIN_Popup_Pause(GET_TEXT_F(MSG_PAUSE_PRINT_PARKING));    break;                // M125
      case PAUSE_MESSAGE_CHANGING: DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INIT));   break;                // pause_print (M125, M600)
      case PAUSE_MESSAGE_WAITING:  DWIN_Popup_Pause(GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING), BTN_Continue); break;
      case PAUSE_MESSAGE_INSERT:   DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INSERT), BTN_Continue); break;
      case PAUSE_MESSAGE_LOAD:     DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_LOAD));   break;
      case PAUSE_MESSAGE_UNLOAD:   DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_UNLOAD)); break;                // Unload of pause and Unload of M702
      case PAUSE_MESSAGE_PURGE:
        #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
          DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_CONT_PURGE));
        #else
          DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE));
        #endif
        break;
      case PAUSE_MESSAGE_OPTION:   Goto_FilamentPurge(); break;
      case PAUSE_MESSAGE_RESUME:   DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT), BTN_Continue);   break;
      case PAUSE_MESSAGE_HEATING:  DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_STATUS:   HMI_ReturnScreen(); break;                                                      // Exit from Pause, Load and Unload
      default: break;
    }
  }

  void Draw_Popup_FilamentPurge() {
    DWIN_Draw_Popup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE_CONTINUE));
    DWINUI::Draw_Button(BTN_Purge, 26, 280);
    DWINUI::Draw_Button(BTN_Continue, 146, 280);
    Draw_Select_Highlight(true);
  }

  void onClick_FilamentPurge() {
    if (HMI_flag.select_flag)
      pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // "Purge More" button
    else {
      HMI_SaveProcessID(NothingToDo);
      pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;  // "Continue" button
    }
  }

  void Goto_FilamentPurge() {
    pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
    Goto_Popup(Draw_Popup_FilamentPurge, onClick_FilamentPurge);
  }

#endif // ADVANCED_PAUSE_FEATURE

#if HAS_MESH
  void DWIN_MeshViewer() {
    if (!leveling_is_valid())
      DWIN_Popup_Continue(ICON_BLTouch, GET_TEXT_F(MSG_MESH_VIEWER), GET_TEXT_F(MSG_NO_VALID_MESH));
    else {
      HMI_SaveProcessID(WaitResponse);
      MeshViewer.Draw();
    }
  }
#endif // HAS_MESH


#if HAS_LOCKSCREEN

  void DWIN_LockScreen() {
    if (checkkey != Locked) {
      lockScreen.rprocess = checkkey;
      checkkey = Locked;
      lockScreen.init();
    }
  }

  void DWIN_UnLockScreen() {
    if (checkkey == Locked) {
      checkkey = lockScreen.rprocess;
      Draw_Main_Area();
    }
  }

  void HMI_LockScreen() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    lockScreen.onEncoder(encoder_diffState);
    if (lockScreen.isUnlocked()) DWIN_UnLockScreen();
  }

#endif //  HAS_LOCKSCREEN

#if HAS_GCODE_PREVIEW

  void onClick_ConfirmToPrint() {
    Preview_Reset();
    DWIN_ResetStatusLine();
    if (HMI_flag.select_flag) {     // Confirm
      HMI_flag.heat_flag = true;
      Goto_Main_Menu();
      return card.openAndPrintFile(card.filename);
    }
    else HMI_ReturnScreen();
  }

  void Goto_ConfirmToPrint() {
    Goto_Popup(Preview_DrawFromSD, onClick_ConfirmToPrint);
  }

#else
  void Goto_ConfirmToPrint() {
    // Start choice and print SD file
    HMI_flag.heat_flag = true;
    HMI_flag.config_flag = false;
    card.openAndPrintFile(card.filename);
  }
#endif

#if HAS_ESDIAG
  void Draw_EndStopDiag() {
    HMI_SaveProcessID(ESDiagProcess);
    ESDiag.Draw();
  }
#endif

//=============================================================================
// NEW MENU SUBSYSTEM
//=============================================================================

// Tool functions

#if ENABLED(EEPROM_SETTINGS)
  void WriteEeprom() {
    DWIN_DrawStatusLine(GET_TEXT_F(MSG_STORE_EEPROM));
    DWIN_UpdateLCD();
    DONE_BUZZ(settings.save());
  }

  void ReadEeprom() {
    const bool success = settings.load();
    DWIN_RedrawScreen();
    DONE_BUZZ(success);
  }

  void ResetEeprom() {
    settings.reset();
    DWIN_RedrawScreen();
    DONE_BUZZ(true);
  }

  #if HAS_MESH
    void SaveMesh() { TERN(AUTO_BED_LEVELING_UBL, UBLSaveMesh(), WriteEeprom()); }
  #endif
#endif

// Reset Printer
void RebootPrinter() {
  wait_for_heatup = wait_for_user = false;    // Stop waiting for heating/user
  thermalManager.disable_all_heaters();
  planner.finish_and_disable();
  DWIN_RebootScreen();
  hal.reboot();
}

void Goto_Info_Menu() {
  Draw_Info_Menu();
  DWIN_UpdateLCD();
  HMI_SaveProcessID(WaitResponse);
}

void Goto_Move_Menu() {
  #if HAS_HOTEND
    gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
  #endif
  Draw_Move_Menu();
}

void DisableMotors() { queue.inject(F("M84")); }

void AutoLev() {   // Always reacquire the Z "home" position
  queue.inject(F(TERN(AUTO_BED_LEVELING_UBL, "G28Z\nG29P1", "G28XYO\nG28Z\nG29")));
}

void AutoHome() { queue.inject_P(G28_STR); }

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void HomeX() { queue.inject(F("G28X")); }
  void HomeY() { queue.inject(F("G28Y")); }
  void HomeZ() { queue.inject(F("G28Z")); }
  #if BOTH(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    void ApplyZAfterHoming() { HMI_data.z_after_homing = MenuData.Value; };
    void SetZAfterHoming() { SetIntOnClick(0, 20, HMI_data.z_after_homing, ApplyZAfterHoming); }
  #endif
#endif

#if HAS_HOME_OFFSET
  // Apply workspace offset, making the current position 0,0,0
  void SetHome() {
  queue.inject(F("G92X0Y0Z0"));
    DONE_BUZZ(true);
  }
#endif

#if HAS_ZOFFSET_ITEM

  void ApplyZOffset() { TERN_(EEPROM_SETTINGS, settings.save()); }
  void LiveZOffset() {
    #if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      const_float_t step_zoffset = round((MenuData.Value / 100.0f) * planner.settings.axis_steps_per_mm[Z_AXIS]) - babystep.accum;
      if (BABYSTEP_ALLOWED()) babystep.add_steps(Z_AXIS, step_zoffset);
      //SERIAL_ECHOLNF(F("BB Steps: "), step_zoffset);
    #endif
  }
  #if EITHER(HAS_BED_PROBE, BABYSTEPPING)
    void SetZOffset() {
      babystep.accum = round(planner.settings.axis_steps_per_mm[Z_AXIS] * BABY_Z_VAR);
      SetPFloatOnClick(Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX, 2, ApplyZOffset, LiveZOffset);
    }
  #endif

  void SetMoveZto0() {
    #if ENABLED(Z_SAFE_HOMING)
      char cmd[54], str_1[5], str_2[5];
      sprintf_P(cmd, PSTR("G28XYO\nG28Z\nG0X%sY%sF5000\nG0Z0F300\nM400"),
        dtostrf(Z_SAFE_HOMING_X_POINT, 1, 1, str_1),
        dtostrf(Z_SAFE_HOMING_Y_POINT, 1, 1, str_2)
      );
      gcode.process_subcommands_now(cmd);
    #else
      TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));
      gcode.process_subcommands_now(F("G28Z\nG0Z0F300\nM400"));
    #endif
    ui.reset_status();
    DONE_BUZZ(true);
  }

  #if !HAS_BED_PROBE
    void HomeZandDisable() {
      SetMoveZto0();
      DisableMotors();
    }
  #endif

#endif  // HAS_ZOFFSET_ITEM

#if HAS_PREHEAT
  #define _DoPreheat(N) void DoPreheat##N() { ui.preheat_all(N-1); }
  REPEAT_1(PREHEAT_COUNT, _DoPreheat)
#endif

void DoCoolDown() { thermalManager.cooldown(); }

void LiveMove() {
  *MenuData.P_Float = MenuData.Value / MINUNITMULT;
  if (!planner.is_full()) {
    planner.synchronize();
    planner.buffer_line(current_position, homing_feedrate(HMI_value.axis));
  }
}
void ApplyMoveE() {
  last_E = MenuData.Value / MINUNITMULT;
  if (!planner.is_full()) {
    planner.synchronize();
    planner.buffer_line(current_position, FEEDRATE_E);
  }
}

void SetMoveX() {
  if (!HMI_data.SetLiveMove) {
  HMI_value.axis = X_AXIS; SetPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, LiveMove); 
  }
  else {
  HMI_value.axis = X_AXIS; SetPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, planner.synchronize, LiveMove); 
  }
}
void SetMoveY() {
  if (!HMI_data.SetLiveMove) {
  HMI_value.axis = Y_AXIS; SetPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, LiveMove); 
  }
  else { 
  HMI_value.axis = Y_AXIS; SetPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, planner.synchronize, LiveMove); 
  }
}
void SetMoveZ() {
  if (!HMI_data.SetLiveMove) {
  HMI_value.axis = Z_AXIS; SetPFloatOnClick(Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, LiveMove, planner.synchronize);
  }
  else {
  HMI_value.axis = Z_AXIS; SetPFloatOnClick(Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, planner.synchronize, LiveMove); 
  }
}

#if HAS_HOTEND
  void SetMoveE() {
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      if (thermalManager.tooColdToExtrude(0))
        return DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT));
    #endif
    SetPFloatOnClick(last_E - (EXTRUDE_MAXLENGTH), last_E + (EXTRUDE_MAXLENGTH), UNITFDIGITS, ApplyMoveE);
  }
#endif

void SetPID(celsius_t t, heater_id_t h) {
  char cmd[53] = "";
  char str_1[5] = "", str_2[5] = "";
  sprintf_P(cmd, PSTR("G28OXY\nG0Z5F300\nG0X%sY%sF5000\nM84\nM400"),
    dtostrf(X_CENTER, 1, 1, str_1),
    dtostrf(Y_CENTER, 1, 1, str_2)
  );
  gcode.process_subcommands_now(cmd);
  thermalManager.PID_autotune(t, h, HMI_data.PidCycles, true);
}
#if ENABLED(PIDTEMP)
  void HotendPID() { SetPID(HMI_data.HotendPidT, H_E0); }
#endif
#if ENABLED(PIDTEMPBED)
  void BedPID() { SetPID(HMI_data.BedPidT, H_BED); }
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  void SetPwrLossr() {
    recovery.enable(!recovery.enabled);
    Show_Chkb_Line(CurrentMenu->line(), recovery.enabled);
  }
#endif

#if ENABLED(BAUD_RATE_GCODE)
  void SetBaudRate() {
    HMI_data.Baud250K = !HMI_data.Baud250K;
    if (HMI_data.Baud250K) SetBaud250K(); else SetBaud115K();
    Show_Chkb_Line(CurrentMenu->line(), HMI_data.Baud250K);
  }
  void SetBaud115K() { queue.inject(F("M575B115")); }
  void SetBaud250K() { queue.inject(F("M575B250")); }
#endif

#if HAS_LCD_BRIGHTNESS
  void ApplyBrightness() { ui.set_brightness(MenuData.Value); }
  void LiveBrightness() { DWIN_LCD_Brightness(MenuData.Value); }
  void SetBrightness() { SetIntOnClick(LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, ui.brightness, ApplyBrightness, LiveBrightness); }
  void TurnOffBacklight() { HMI_SaveProcessID(WaitResponse); ui.set_brightness(0); DWIN_RedrawScreen(); }
#endif

#if ENABLED(CASE_LIGHT_MENU)
  void SetCaseLight() {
    caselight.on = !caselight.on;
    caselight.update_enabled();
    Show_Chkb_Line(CurrentMenu->line(), caselight.on);
  }
  #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
    void LiveCaseLightBrightness() { caselight.brightness = MenuData.Value; caselight.update_brightness(); }
    void SetCaseLightBrightness() { SetIntOnClick(0, 255, caselight.brightness, nullptr, LiveCaseLightBrightness); }
  #endif
#endif

#if ENABLED(LED_CONTROL_MENU)
  #if !BOTH(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
    void SetLedStatus() {
      leds.toggle();
      Show_Chkb_Line(CurrentMenu->line(), leds.lights_on);
    }
  #endif
  #if HAS_COLOR_LEDS
    void ApplyLEDColor() { HMI_data.LED_Color = TERN0(HAS_WHITE_LED, (leds.color.w << 24)) | (leds.color.r << 16) | (leds.color.g << 8) | leds.color.b; }
    void LiveLEDColor(uint8_t *color) { *color = MenuData.Value; leds.update(); }
    void LiveLEDColorR() { LiveLEDColor(&leds.color.r); }
    void LiveLEDColorG() { LiveLEDColor(&leds.color.g); }
    void LiveLEDColorB() { LiveLEDColor(&leds.color.b); }
    void SetLEDColorR() { SetIntOnClick(0, 255, leds.color.r, ApplyLEDColor, LiveLEDColorR); }
    void SetLEDColorG() { SetIntOnClick(0, 255, leds.color.g, ApplyLEDColor, LiveLEDColorG); }
    void SetLEDColorB() { SetIntOnClick(0, 255, leds.color.b, ApplyLEDColor, LiveLEDColorB); }
    #if HAS_WHITE_LED
      void LiveLEDColorW() { LiveLEDColor(&leds.color.w); }
      void SetLEDColorW() { SetIntOnClick(0, 255, leds.color.w, ApplyLEDColor, LiveLEDColorW); }
    #endif
  #endif
#endif

#if ENABLED(SOUND_MENU_ITEM)
  void SetEnableSound() {
    ui.sound_on = !ui.sound_on;
    Show_Chkb_Line(CurrentMenu->line(), ui.sound_on);
  }
 //changed
  void SetEnableTick() {
    ui.no_tick = !ui.no_tick;
    Show_Chkb_Line(CurrentMenu->line(), ui.no_tick);
  }
#endif

#if ENABLED(USE_UBL_VIEWER) //changed
  void SetViewMesh() {
    BedLevelTools.view_mesh = !BedLevelTools.view_mesh;
    Show_Chkb_Line(CurrentMenu->line(), BedLevelTools.view_mesh);
  }
#endif

#if HAS_HOME_OFFSET
  void ApplyHomeOffset() { set_home_offset(HMI_value.axis, MenuData.Value / MINUNITMULT); }
  void SetHomeOffsetX() { HMI_value.axis = X_AXIS; SetPFloatOnClick(-50, 50, UNITFDIGITS, ApplyHomeOffset); }
  void SetHomeOffsetY() { HMI_value.axis = Y_AXIS; SetPFloatOnClick(-50, 50, UNITFDIGITS, ApplyHomeOffset); }
  void SetHomeOffsetZ() { HMI_value.axis = Z_AXIS; SetPFloatOnClick( -2,  2, UNITFDIGITS, ApplyHomeOffset); }
#endif

#if HAS_BED_PROBE
  void SetProbeOffsetX() { SetPFloatOnClick(-60, 60, UNITFDIGITS, TERN(ProUIex, ProEx.ApplyPhySet, nullptr)); }
  void SetProbeOffsetY() { SetPFloatOnClick(-60, 60, UNITFDIGITS, TERN(ProUIex, ProEx.ApplyPhySet, nullptr)); }
  void SetProbeOffsetZ() { SetPFloatOnClick(-10, 10, 2); }

  #if ProUIex
    void SetProbeZSpeed()  { SetPIntOnClick(60, Z_PROBE_FEEDRATE_FAST); }
    void ApplyProbeMultiple() { PRO_data.multiple_probing = (MenuData.Value > 0) ? MenuData.Value : 0; }
    void SetProbeMultiple()  { SetIntOnClick(0, 5, PRO_data.multiple_probing, ApplyProbeMultiple); }
  #endif

  void ProbeTest() {
    LCD_MESSAGE(MSG_M48_TEST);
    queue.inject(F("G28O\nM48 P5"));
  }
  void ProbeStow() { probe.stow(); }
  void ProbeDeploy() { probe.deploy(); }

  #ifdef BLTOUCH_HS_MODE
    void SetHSMode() {
      bltouch.high_speed_mode = !bltouch.high_speed_mode;
      Show_Chkb_Line(CurrentMenu->line(), bltouch.high_speed_mode);
    }
  #endif

#endif


  #if LCD_BACKLIGHT_TIMEOUT_MINS
  void SetTimer()   { SetPIntOnClick(ui.backlight_timeout_min, ui.backlight_timeout_max);
   }
  #endif

#if ProUIex && ENABLED(NOZZLE_PARK_FEATURE)
  void SetParkPosX()   { SetPIntOnClick(X_MIN_POS, X_MAX_POS); }
  void SetParkPosY()   { SetPIntOnClick(Y_MIN_POS, Y_MAX_POS); }
  void SetParkZRaise() { SetPIntOnClick(Z_MIN_POS, 50); }
#endif

#if HAS_FILAMENT_SENSOR
  void SetRunoutEnable() {
    runout.reset();
    runout.enabled = !runout.enabled;
    Show_Chkb_Line(CurrentMenu->line(), runout.enabled);
  }

  #if ProUIex
    void LiveRunoutActive() { ProEx.DrawRunoutActive(true); }
    void SetRunoutActive() {
      uint8_t val;
      val = PRO_data.FilamentMotionSensor ? 2 : PRO_data.Runout_active_state ? 1 : 0;
      SetOnClick(SetIntNoDraw, 0, 2, 0, val, ProEx.ApplyRunoutActive, LiveRunoutActive);
      ProEx.DrawRunoutActive(true);
    }
  #endif

  #if HAS_FILAMENT_RUNOUT_DISTANCE
    void ApplyRunoutDistance() { runout.set_runout_distance(MenuData.Value / MINUNITMULT); }
    void SetRunoutDistance() { SetFloatOnClick(0, 999, UNITFDIGITS, runout.runout_distance(), ApplyRunoutDistance); }
  #endif
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void SetFilLoad()   { SetPFloatOnClick(0, MAX_LOAD_UNLOAD, UNITFDIGITS); }
  void SetFilUnload() { SetPFloatOnClick(0, MAX_LOAD_UNLOAD, UNITFDIGITS); }
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  void ApplyExtMinT() { thermalManager.extrude_min_temp = HMI_data.ExtMinT; thermalManager.allow_cold_extrude = (HMI_data.ExtMinT == 0); }
  void SetExtMinT() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP, ApplyExtMinT); }
#endif

void RestoreDefaultsColors() {
  DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.TitleBg_color);
  DWIN_RedrawScreen();
}

void SelColor() {
  MenuData.P_Int = (int16_t*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
  HMI_value.Color[0] = GetRColor(*MenuData.P_Int);  // Red
  HMI_value.Color[1] = GetGColor(*MenuData.P_Int);  // Green
  HMI_value.Color[2] = GetBColor(*MenuData.P_Int);  // Blue
  Draw_GetColor_Menu();
}

void LiveRGBColor() {
    HMI_value.Color[CurrentMenu->line() - 2] = MenuData.Value;
    uint16_t color = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
    DWIN_Draw_Rectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
}
void SetRGBColor() {
  const uint8_t color = static_cast<MenuItemClass*>(CurrentMenu->SelectedItem())->icon;
  SetIntOnClick(0, (color == 1) ? 63 : 31, HMI_value.Color[color], nullptr, LiveRGBColor);
}

void DWIN_ApplyColor() {
  *MenuData.P_Int = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.TitleBg_color);
  Draw_SelectColors_Menu();
  hash_changed = true;
  LCD_MESSAGE(MSG_COLORS_APPLIED);
  DWIN_Draw_Dashboard();
}

void DWIN_ApplyColor(const int8_t element, const bool ldef /* = false*/) {
  const uint16_t color = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
  switch (element) {
    case  2: HMI_data.Background_Color = ldef ? Def_Background_Color : color; DWINUI::SetBackgroundColor(HMI_data.Background_Color); break;
    case  3: HMI_data.Cursor_color     = ldef ? Def_Cursor_color     : color; break;
    case  4: HMI_data.TitleBg_color    = ldef ? Def_TitleBg_color    : color; break;
    case  5: HMI_data.TitleTxt_color   = ldef ? Def_TitleTxt_color   : color; break;
    case  6: HMI_data.Text_Color       = ldef ? Def_Text_Color       : color; DWINUI::SetTextColor(HMI_data.Text_Color); break;
    case  7: HMI_data.Selected_Color   = ldef ? Def_Selected_Color   : color; break;
    case  8: HMI_data.SplitLine_Color  = ldef ? Def_SplitLine_Color  : color; break;
    case  9: HMI_data.Highlight_Color  = ldef ? Def_Highlight_Color  : color; break;
    case 10: HMI_data.StatusBg_Color   = ldef ? Def_StatusBg_Color   : color; break;
    case 11: HMI_data.StatusTxt_Color  = ldef ? Def_StatusTxt_Color  : color; break;
    case 12: HMI_data.PopupBg_color    = ldef ? Def_PopupBg_color    : color; break;
    case 13: HMI_data.PopupTxt_Color   = ldef ? Def_PopupTxt_Color   : color; break;
    case 14: HMI_data.AlertBg_Color    = ldef ? Def_AlertBg_Color    : color; break;
    case 15: HMI_data.AlertTxt_Color   = ldef ? Def_AlertTxt_Color   : color; break;
    case 16: HMI_data.PercentTxt_Color = ldef ? Def_PercentTxt_Color : color; break;
    case 17: HMI_data.Barfill_Color    = ldef ? Def_Barfill_Color    : color; break;
    case 18: HMI_data.Indicator_Color  = ldef ? Def_Indicator_Color  : color; break;
    case 19: HMI_data.Coordinate_Color = ldef ? Def_Coordinate_Color : color; break;
    case 20: HMI_data.Bottom_Color     = ldef ? Def_Bottom_Color     : color; break;
    default: break;
  }
}

void SetSpeed() { SetPIntOnClick(MIN_PRINT_SPEED, MAX_PRINT_SPEED); }

#if HAS_HOTEND
  void ApplyHotendTemp() { thermalManager.setTargetHotend(MenuData.Value, 0); }
  void SetHotendTemp() { SetIntOnClick(MIN_ETEMP, MAX_ETEMP, thermalManager.degTargetHotend(0), ApplyHotendTemp); }
#endif

#if HAS_HEATED_BED
  void ApplyBedTemp() { thermalManager.setTargetBed(MenuData.Value); }
  void SetBedTemp() { SetIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP, thermalManager.degTargetBed(), ApplyBedTemp); }
#endif

#if HAS_FAN
  void ApplyFanSpeed() { thermalManager.set_fan_speed(0, MenuData.Value); }
  void SetFanSpeed() { SetIntOnClick(0, 255, thermalManager.fan_speed[0], ApplyFanSpeed); }
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void ChangeFilament() {
    HMI_SaveProcessID(NothingToDo);
    queue.inject(F("M600 B2"));
  }

  #if ENABLED(NOZZLE_PARK_FEATURE)
    void ParkHead() {
      LCD_MESSAGE(MSG_FILAMENT_PARK_ENABLED);
      queue.inject(F("G28O\nG27"));
    }
  #endif

  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
    void UnloadFilament() {
      LCD_MESSAGE(MSG_FILAMENTUNLOAD);
      queue.inject(F("M702 Z20"));
    }

    void LoadFilament() {
      LCD_MESSAGE(MSG_FILAMENTLOAD);
      queue.inject(F("M701 Z20"));
    }
  #endif

#endif // ADVANCED_PAUSE_FEATURE

void ApplyFlow() { planner.refresh_e_factor(0); }
void SetFlow() { SetPIntOnClick(MIN_PRINT_FLOW, MAX_PRINT_FLOW, ApplyFlow); }

// Bed Tramming
TERN(HAS_BED_PROBE, float, void) Tram(uint8_t point) {
  char cmd[100] = "";
  #if HAS_BED_PROBE
    float corner_pos = ui.screw_pos;
    static bool inLev = false;
    float xpos = 0, ypos = 0, zval = 0, margin = 0;
    char str_1[6] = "", str_2[6] = "", str_3[6] = "";
    if (inLev) return NAN;
    margin = corner_pos;
  #else
    int16_t xpos = 0, ypos = 0;
    float margin = 25.4;
  #endif

  switch (point) {
    case 0:
      LCD_MESSAGE(MSG_LEVBED_FL);
      xpos = ypos = margin;
      break;
    case 1:
      LCD_MESSAGE(MSG_LEVBED_FR);
      xpos = X_BED_SIZE - margin; ypos = margin;
      break;
    case 2:
      LCD_MESSAGE(MSG_LEVBED_BR);
      xpos = X_BED_SIZE - margin; ypos = Y_BED_SIZE - margin;
      break;
    case 3:
      LCD_MESSAGE(MSG_LEVBED_BL);
      xpos = margin; ypos = Y_BED_SIZE - margin;
      break;
    case 4:
      LCD_MESSAGE(MSG_LEVBED_C);
      xpos = X_BED_SIZE / 2; ypos = Y_BED_SIZE / 2;
      break;
  }

  #if HAS_BED_PROBE

    if (HMI_data.FullManualTramming) {
      set_bed_leveling_enabled(false);
      sprintf_P(cmd, PSTR("G28O\nG90\nG0Z5F300\nG0X%sY%sF5000\nG0Z0F300"),
        dtostrf(xpos, 1, 1, str_1),
        dtostrf(ypos, 1, 1, str_2)
      );
      queue.inject(cmd);
    }
    else {
      LIMIT(xpos, MESH_MIN_X, MESH_MAX_X);
      LIMIT(ypos, MESH_MIN_Y, MESH_MAX_Y);
      probe.stow();
      gcode.process_subcommands_now(F("G28O"));
      inLev = true;
      zval = probe.probe_at_point(xpos, ypos, PROBE_PT_STOW);
      if (!isnan(zval)) {
        sprintf_P(cmd, PSTR("X:%s, Y:%s, Z:%s"),
          dtostrf(xpos, 1, 1, str_1),
          dtostrf(ypos, 1, 1, str_2),
          dtostrf(zval, 1, 2, str_3)
        );
        ui.set_status(cmd);
      }
      else LCD_MESSAGE(MSG_M48_OUT_OF_BOUNDS);
      inLev = false;
    }
    return zval;

  #else // !HAS_BED_PROBE

    sprintf_P(cmd, PSTR("M420S0\nG28O\nG90\nG0Z5F300\nG0X%iY%iF5000\nG0Z0F300"), xpos, ypos);
    queue.inject(cmd);

  #endif
}

void TramFL() { Tram(0); }
void TramFR() { Tram(1); }
void TramBR() { Tram(2); }
void TramBL() { Tram(3); }
void TramC () { Tram(4); }

#if HAS_BED_PROBE

  void Trammingwizard() {
    bed_mesh_t zval = {0};
    if (HMI_data.FullManualTramming) {
      LCD_MESSAGE_F("Disable manual tramming");
      return;
    }
    zval[0][0] = Tram(0);
    checkkey = NothingToDo;
    MeshViewer.DrawMesh(zval, 2, 2);
    zval[1][0] = Tram(1);
    MeshViewer.DrawMesh(zval, 2, 2);
    zval[1][1] = Tram(2);
    MeshViewer.DrawMesh(zval, 2, 2);
    zval[0][1] = Tram(3);
    MeshViewer.DrawMesh(zval, 2, 2);

    if (HMI_data.CalcAvg) {
    DWINUI::Draw_CenteredString(140, F("Calculating average"));
    DWINUI::Draw_CenteredString(160, F("and relative heights"));
    safe_delay(1000);
    float avg = 0.0f;
    LOOP_L_N(x, 2) LOOP_L_N(y, 2) avg += zval[x][y];
    avg /= 4.0f;
    LOOP_L_N(x, 2) LOOP_L_N(y, 2) zval[x][y] -= avg;
    MeshViewer.DrawMesh(zval, 2, 2);
    }
    ui.reset_status();

    if (ABS(MeshViewer.max - MeshViewer.min) < 0.05) {
      DWINUI::Draw_CenteredString(140,F("Corners leveled"));
      DWINUI::Draw_CenteredString(160,F("Tolerance achieved!"));
    }
    else {
      uint8_t p = 0;
      float max = 0;
      FSTR_P plabel;
      bool s = true;
      LOOP_L_N(x, 2) LOOP_L_N(y, 2) {
        const float d = ABS(zval[x][y]);
        if (max < d) {
          s = (zval[x][y] >= 0);
          max = d;
          p = x + 2 * y;
        }
      }
      switch (p) {
        case 0b00 : plabel = GET_TEXT_F(MSG_LEVBED_FL); break;
        case 0b01 : plabel = GET_TEXT_F(MSG_LEVBED_FR); break;
        case 0b10 : plabel = GET_TEXT_F(MSG_LEVBED_BL); break;
        case 0b11 : plabel = GET_TEXT_F(MSG_LEVBED_BR); break;
        default   : plabel = F(""); break;
      }
      DWINUI::Draw_CenteredString(120, F("Corners not leveled"));
      DWINUI::Draw_CenteredString(140, F("Knob adjustment required"));
      DWINUI::Draw_CenteredString(Color_Green, 160, s ? F("Lower") : F("Raise"));
      DWINUI::Draw_CenteredString(Color_Green, 180, plabel);
    }
    DWINUI::Draw_Button(BTN_Continue, 86, 305);
    Draw_Select_Box(86, 305);
    checkkey = Menu;
    HMI_SaveProcessID(WaitResponse);
  }

  void SetManualTramming() {
    HMI_data.FullManualTramming = !HMI_data.FullManualTramming;
    Show_Chkb_Line(CurrentMenu->line(), HMI_data.FullManualTramming);
  }

  void SetCalcAvg() {
    HMI_data.CalcAvg = !HMI_data.CalcAvg;
    Show_Chkb_Line(CurrentMenu->line(), HMI_data.CalcAvg);
  }  

#endif // HAS_BED_PROBE

#if ENABLED(MESH_BED_LEVELING)

  void ManualMeshStart() {
    LCD_MESSAGE(MSG_UBL_BUILD_MESH_MENU);
    gcode.process_subcommands_now(F("G28XYO\nG28Z\nM211S0\nG29S1"));
    #ifdef MANUAL_PROBE_START_Z
      const uint8_t line = CurrentMenu->line(MMeshMoveZItem->pos);
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 2, VALX - 2 * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(line), MANUAL_PROBE_START_Z);
    #endif
  }

  void LiveMeshMoveZ() {
    *MenuData.P_Float = MenuData.Value / POW(10, 2);
    if (!planner.is_full()) {
      planner.synchronize();
      planner.buffer_line(current_position, homing_feedrate(Z_AXIS));
    }
  }
  void SetMMeshMoveZ() { SetPFloatOnClick(-1, 1, 2, planner.synchronize, LiveMeshMoveZ); }

  void ManualMeshContinue() {
    gcode.process_subcommands_now(F("G29S2"));
    MMeshMoveZItem->redraw();
  }

  void ManualMeshSave() {
    LCD_MESSAGE(MSG_UBL_STORAGE_MESH_MENU);
    queue.inject(F("M211S1\nM500"));
  }

#endif // MESH_BED_LEVELING

#if HAS_PREHEAT
  #if HAS_HOTEND
    void SetPreheatEndTemp() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif
  #if HAS_HEATED_BED
    void SetPreheatBedTemp() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif
  #if HAS_FAN
    void SetPreheatFanSpeed() { SetPIntOnClick(0, 255); }
  #endif
#endif

void ApplyMaxSpeed() { planner.set_max_feedrate(HMI_value.axis, MenuData.Value / MINUNITMULT); }
void SetMaxSpeedX() { HMI_value.axis = X_AXIS, SetFloatOnClick(MIN_MAXFEEDSPEED, max_feedrate_edit_values[X_AXIS], UNITFDIGITS, planner.settings.max_feedrate_mm_s[X_AXIS], ApplyMaxSpeed); }
void SetMaxSpeedY() { HMI_value.axis = Y_AXIS, SetFloatOnClick(MIN_MAXFEEDSPEED, max_feedrate_edit_values[Y_AXIS], UNITFDIGITS, planner.settings.max_feedrate_mm_s[Y_AXIS], ApplyMaxSpeed); }
void SetMaxSpeedZ() { HMI_value.axis = Z_AXIS, SetFloatOnClick(MIN_MAXFEEDSPEED, max_feedrate_edit_values[Z_AXIS], UNITFDIGITS, planner.settings.max_feedrate_mm_s[Z_AXIS], ApplyMaxSpeed); }
#if HAS_HOTEND
  void SetMaxSpeedE() { HMI_value.axis = E_AXIS; SetFloatOnClick(MIN_MAXFEEDSPEED, max_feedrate_edit_values[E_AXIS], UNITFDIGITS, planner.settings.max_feedrate_mm_s[E_AXIS], ApplyMaxSpeed); }
#endif

void ApplyMaxAccel() { planner.set_max_acceleration(HMI_value.axis, MenuData.Value); }
void SetMaxAccelX() { HMI_value.axis = X_AXIS, SetIntOnClick(MIN_MAXACCELERATION, max_acceleration_edit_values[X_AXIS], planner.settings.max_acceleration_mm_per_s2[X_AXIS], ApplyMaxAccel); }
void SetMaxAccelY() { HMI_value.axis = Y_AXIS, SetIntOnClick(MIN_MAXACCELERATION, max_acceleration_edit_values[Y_AXIS], planner.settings.max_acceleration_mm_per_s2[Y_AXIS], ApplyMaxAccel); }
void SetMaxAccelZ() { HMI_value.axis = Z_AXIS, SetIntOnClick(MIN_MAXACCELERATION, max_acceleration_edit_values[Z_AXIS], planner.settings.max_acceleration_mm_per_s2[Z_AXIS], ApplyMaxAccel); }
#if HAS_HOTEND
  void SetMaxAccelE() { HMI_value.axis = E_AXIS; SetIntOnClick(MIN_MAXACCELERATION, max_acceleration_edit_values[E_AXIS], planner.settings.max_acceleration_mm_per_s2[E_AXIS], ApplyMaxAccel); }
#endif

#if HAS_CLASSIC_JERK
  void ApplyMaxJerk() { planner.set_max_jerk(HMI_value.axis, MenuData.Value / MINUNITMULT); }
  void SetMaxJerkX() { HMI_value.axis = X_AXIS, SetFloatOnClick(MIN_MAXJERK, max_jerk_edit_values[X_AXIS], UNITFDIGITS, planner.max_jerk[X_AXIS], ApplyMaxJerk); }
  void SetMaxJerkY() { HMI_value.axis = Y_AXIS, SetFloatOnClick(MIN_MAXJERK, max_jerk_edit_values[Y_AXIS], UNITFDIGITS, planner.max_jerk[Y_AXIS], ApplyMaxJerk); }
  void SetMaxJerkZ() { HMI_value.axis = Z_AXIS, SetFloatOnClick(MIN_MAXJERK, max_jerk_edit_values[Z_AXIS], UNITFDIGITS, planner.max_jerk[Z_AXIS], ApplyMaxJerk); }
  #if HAS_HOTEND
    void SetMaxJerkE() { HMI_value.axis = E_AXIS; SetFloatOnClick(MIN_MAXJERK, max_jerk_edit_values[E_AXIS], UNITFDIGITS, planner.max_jerk[E_AXIS], ApplyMaxJerk); }
  #endif
#elif HAS_JUNCTION_DEVIATION
  void ApplyJDmm() { TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk()); }
  void SetJDmm() { SetPFloatOnClick(MIN_JD_MM, MAX_JD_MM, 3, ApplyJDmm); }
#endif

#if ENABLED(LIN_ADVANCE)
  void SetLA_K() { SetPFloatOnClick(0, 10, 3); }
#endif

void SetStepsX() { HMI_value.axis = X_AXIS, SetPFloatOnClick( MIN_STEP, MAX_STEP, 2); }
void SetStepsY() { HMI_value.axis = Y_AXIS, SetPFloatOnClick( MIN_STEP, MAX_STEP, 2); }
void SetStepsZ() { HMI_value.axis = Z_AXIS, SetPFloatOnClick( MIN_STEP, MAX_STEP, 2); }
#if HAS_HOTEND
  void SetStepsE() { HMI_value.axis = E_AXIS; SetPFloatOnClick( MIN_STEP, MAX_STEP, 2); }
  void SetHotendPidT() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
#endif
#if HAS_HEATED_BED
  void SetBedPidT() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
#endif

#if ProUIex
  void SetBedSizeX() { HMI_value.axis = NO_AXIS_ENUM, SetPIntOnClick(X_BED_MIN, X_MAX_POS, ProEx.ApplyPhySet); }
  void SetBedSizeY() { HMI_value.axis = NO_AXIS_ENUM, SetPIntOnClick(Y_BED_MIN, Y_MAX_POS, ProEx.ApplyPhySet); }
  void SetMinPosX()  { HMI_value.axis = X_AXIS,       SetPIntOnClick(     -100,       100, ProEx.ApplyPhySet); }
  void SetMinPosY()  { HMI_value.axis = Y_AXIS,       SetPIntOnClick(     -100,       100, ProEx.ApplyPhySet); }
  void SetMaxPosX()  { HMI_value.axis = X_AXIS,       SetPIntOnClick(X_BED_MIN,       999, ProEx.ApplyPhySet); }
  void SetMaxPosY()  { HMI_value.axis = Y_AXIS,       SetPIntOnClick(Y_BED_MIN,       999, ProEx.ApplyPhySet); }
  void SetMaxPosZ()  { HMI_value.axis = Z_AXIS,       SetPIntOnClick(      100,       999, ProEx.ApplyPhySet); }
#endif

#if (HAS_HOTEND || HAS_HEATED_BED)
  void SetPidCycles() { SetPIntOnClick(3, 50); }
  void SetKp() { SetPFloatOnClick(0, 1000, 2); }
  void ApplyPIDi() {
    *MenuData.P_Float = scalePID_i(MenuData.Value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void ApplyPIDd() {
    *MenuData.P_Float = scalePID_d(MenuData.Value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void SetKi() {
    MenuData.P_Float = (float*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    const float value = unscalePID_i(*MenuData.P_Float);
    SetFloatOnClick(0, 1000, 2, value, ApplyPIDi);
  }
  void SetKd() {
    MenuData.P_Float = (float*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    const float value = unscalePID_d(*MenuData.P_Float);
    SetFloatOnClick(0, 1000, 2, value, ApplyPIDd);
  }
#endif

#if BOTH(ProUIex, HAS_EXTRUDERS)
  void SetInvertE0() {
    PRO_data.Invert_E0 = !PRO_data.Invert_E0;
    stepper.disable_e_steppers();
    current_position.e = 0;
    sync_plan_position_e();
    Show_Chkb_Line(CurrentMenu->line(), PRO_data.Invert_E0);
  }
#endif

#if ENABLED(FWRETRACT)
  void Return_FWRetract_Menu() { (PreviousMenu == FilamentMenu) ? Draw_FilamentMan_Menu() : Draw_Tune_Menu(); }
  void SetRetractLength() { SetPFloatOnClick( 0, 10, UNITFDIGITS); }
  void SetRetractSpeed() { SetPFloatOnClick( 1, 90, UNITFDIGITS); }
  void SetZRaise() { SetPFloatOnClick( 0, 2, 2); }
  void SetAddRecover() { SetPFloatOnClick(-5, 5, UNITFDIGITS); }
#endif

#if HAS_TOOLBAR
  void LiveTBSetupItem() {
    UpdateTBSetupItem(static_cast<MenuItemClass*>(CurrentMenu->SelectedItem()), MenuData.Value);
    DrawTBSetupItem(true, CurrentMenu->line());
  }
  void ApplyTBSetupItem() {
    DrawTBSetupItem(false, CurrentMenu->line());
    if (static_cast<MenuItemClass*>(CurrentMenu->SelectedItem())->icon) {
      uint8_t *Pint = (uint8_t *)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
      *Pint = MenuData.Value;
    }
  }
  void SetTBSetupItem() {
    const uint8_t val = *(uint8_t *)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    SetOnClick(SetIntNoDraw, 0, ToolBar.OptCount(), 0, val, ApplyTBSetupItem, LiveTBSetupItem);
    DrawTBSetupItem(true, CurrentMenu->line());
  }
  void onDrawTBSetupItem(MenuItemClass* menuitem, int8_t line) {
    uint8_t val = *(uint8_t *)static_cast<MenuItemPtrClass*>(menuitem)->value;
    UpdateTBSetupItem(menuitem, val);
    onDrawMenuItem(menuitem, line);
  }
#endif // HAS_TOOLBAR

// M593 - Acceleration items (Input Shaping)

#if ENABLED(SHAPING_MENU)


void ApplyFreq() { stepper.set_shaping_frequency(HMI_value.axis, *MenuData.P_Float ); }
void ApplyZeta() { stepper.set_shaping_damping_ratio(HMI_value.axis, *MenuData.P_Float ); }


void SetXFreq() { HMI_value.axis = X_AXIS, SetPFloatOnClick(1.0f, 200.0f, 2, ApplyFreq); }

void SetYFreq() { HMI_value.axis = Y_AXIS, SetPFloatOnClick(1.0f, 200.0f, 2, ApplyFreq); }

void SetXZeta() { HMI_value.axis = X_AXIS, SetPFloatOnClick(0.0f, 1.0f, 2, ApplyZeta); }

void SetYZeta() { HMI_value.axis = Y_AXIS, SetPFloatOnClick(0.0f, 1.0f, 2, ApplyZeta); }


  void Draw_InputShaping_Menu() {
    checkkey = Menu;
    if (SET_MENU(InputShapingMenu, MSG_INPUT_SHAPING, 8)) {
      BACK_ITEM(Draw_Prepare_Menu);
    // M593 F Frequency
    #if HAS_SHAPING_X
      static float xfreq = stepper.get_shaping_frequency(X_AXIS);
      EDIT_ITEM(ICON_MoveX, MSG_SHAPING_X_FREQ, onDrawPFloat2Menu, SetXFreq, &xfreq);
 
    #endif
    #if HAS_SHAPING_Y
      static float yfreq = stepper.get_shaping_frequency(Y_AXIS);
      EDIT_ITEM(ICON_MoveY, MSG_SHAPING_Y_FREQ, onDrawPFloat2Menu, SetYFreq, &yfreq);
    #endif
    // M593 D Damping ratio
    #if HAS_SHAPING_X
      static float xzeta = stepper.get_shaping_damping_ratio(X_AXIS);
      EDIT_ITEM(ICON_MoveX, MSG_SHAPING_X_ZETA, onDrawPFloat2Menu, SetXZeta, &xzeta);
 
    #endif
    #if HAS_SHAPING_Y
      static float yzeta = stepper.get_shaping_damping_ratio(Y_AXIS);
      EDIT_ITEM(ICON_MoveY, MSG_SHAPING_Y_ZETA, onDrawPFloat2Menu, SetYZeta, &yzeta);
    #endif
    }
  UpdateMenu(InputShapingMenu);
  }
#endif

// Special Menuitem Drawing functions =================================================

void onDrawSelColorItem(MenuItemClass* menuitem, int8_t line) {
  const uint16_t color = *(uint16_t*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  DWIN_Draw_Rectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  onDrawMenuItem(menuitem, line);
}

void onDrawGetColorItem(MenuItemClass* menuitem, int8_t line) {
  const uint8_t i = menuitem->icon;
  uint16_t color;
  switch (i) {
    case 0: color = RGB(31, 0, 0); break; // Red
    case 1: color = RGB(0, 63, 0); break; // Green
    case 2: color = RGB(0, 0, 31); break; // Blue
    default: color = 0; break;
  }
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  DWIN_Draw_Rectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  DWINUI::Draw_String(LBLX, MBASE(line) - 1, menuitem->caption);
  Draw_Menu_IntValue(HMI_data.Background_Color, line, 4, HMI_value.Color[i]);
  DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(line + 1), 240);
}

#if BOTH(HAS_FILAMENT_SENSOR, ProUIex)
  void onDrawRunoutActive(MenuItemClass* menuitem, int8_t line) {
    onDrawMenuItem(menuitem, line);
    if (PRO_data.FilamentMotionSensor)
      DWINUI::Draw_String(VALX - 8, MBASE(line), GET_TEXT_F(MSG_MOTION));
    else
      DWINUI::Draw_String(VALX + 8, MBASE(line), PRO_data.Runout_active_state ? GET_TEXT_F(MSG_HIGH) : GET_TEXT_F(MSG_LOW));
  }
#endif

void onDrawPIDi(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_i(*(float*)static_cast<MenuItemPtrClass*>(menuitem)->value)); }
void onDrawPIDd(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_d(*(float*)static_cast<MenuItemPtrClass*>(menuitem)->value)); }

#if BOTH(HAS_MESH, ProUIex)
  void onDrawMeshPoints(MenuItemClass* menuitem, int8_t line) {
    onDrawMenuItem(menuitem, line);
    ProEx.DrawMeshPoints(false, line, PRO_data.grid_max_points);
  }
#endif

void SetLiveMove() {
HMI_data.SetLiveMove = !HMI_data.SetLiveMove;
Show_Chkb_Line(CurrentMenu->line(), HMI_data.SetLiveMove);
}

//Print Stats Reset popup
void Popup_ResetStats() { DWIN_Popup_ConfirmCancel(ICON_Pause_0, F("Reset Print Count?")); }
void OnClick_ResetStats() { 
  if (HMI_flag.select_flag) PrintStatsClass::Reset();
  HMI_ReturnScreen(); 
  }
void PrintStatsReset() { Goto_Popup(Popup_ResetStats, OnClick_ResetStats); }

// Menu Creation and Drawing functions ======================================================

void Draw_Prepare_Menu() {
  checkkey = Menu;
  if (SET_MENU(PrepareMenu, MSG_PREPARE, 9 + PREHEAT_COUNT)) {
    BACK_ITEM(Goto_Main_Menu);
    MENU_ITEM(ICON_Axis, MSG_MOVE_AXIS, onDrawSubMenu, Goto_Move_Menu);
    #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
      MENU_ITEM(ICON_Homing, MSG_HOMING, onDrawSubMenu, Draw_Homing_Menu);
    #else
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
    #endif
    #if HAS_ZOFFSET_ITEM
      MENU_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, onDrawSubMenu, Draw_ZOffsetWiz_Menu);
    #endif
    MENU_ITEM(ICON_Tram, MSG_BED_TRAMMING, onDrawSubMenu, Draw_Tramming_Menu);
    MENU_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, onDrawMenuItem, DisableMotors);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENT_MAN, onDrawSubMenu, Draw_FilamentMan_Menu);
    #endif
    #if HAS_PREHEAT
      #define _ITEM_PREHEAT(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheat##N);
      REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT)
    #endif
    MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawMenuItem, DoCoolDown);
    #if ENABLED(SHAPING_MENU)
      MENU_ITEM(ICON_Language, MSG_INPUT_SHAPING, onDrawSubMenu, Draw_InputShaping_Menu);
    #endif
  }
  ui.reset_status(true);
  UpdateMenu(PrepareMenu);
}

void Draw_Tramming_Menu() {
  checkkey = Menu;
  if (SET_MENU(TrammingMenu, MSG_BED_TRAMMING, 9)) {
    BACK_ITEM(Draw_Prepare_Menu);
    #if HAS_BED_PROBE
      MENU_ITEM(ICON_ProbeSet, MSG_TRAMMING_WIZARD, onDrawMenuItem, Trammingwizard);
      EDIT_ITEM(ICON_Version, MSG_BED_TRAMMING_MANUAL, onDrawChkbMenu, SetManualTramming, &HMI_data.FullManualTramming);
      EDIT_ITEM_F(ICON_ResumeEEPROM, "Calculate Average", onDrawChkbMenu, SetCalcAvg, &HMI_data.CalcAvg);
    #else
      MENU_ITEM_F(ICON_MoveZ0, "Home Z and disable", onDrawMenuItem, HomeZandDisable);
    #endif
    MENU_ITEM(ICON_Axis, MSG_LEVBED_FL, onDrawMenuItem, TramFL);
    MENU_ITEM(ICON_Axis, MSG_LEVBED_FR, onDrawMenuItem, TramFR);
    MENU_ITEM(ICON_Axis, MSG_LEVBED_BR, onDrawMenuItem, TramBR);
    MENU_ITEM(ICON_Axis, MSG_LEVBED_BL, onDrawMenuItem, TramBL);
    MENU_ITEM(ICON_SetHome, MSG_LEVBED_C , onDrawMenuItem, TramC );
  }
  UpdateMenu(TrammingMenu);
}

void Draw_Control_Menu() {
  checkkey = Menu;
  if (SET_MENU(ControlMenu, MSG_CONTROL, 15)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
    #endif
      MENU_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawSubMenu, Draw_Temperature_Menu);
      MENU_ITEM(ICON_Motion, MSG_MOTION, onDrawSubMenu, Draw_Motion_Menu);
    #if HAS_LCD_BRIGHTNESS
      MENU_ITEM(ICON_Box, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Checkbox, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
      MENU_ITEM(ICON_Reboot, MSG_RESET_PRINTER, onDrawMenuItem, RebootPrinter);
    #if HAS_ESDIAG
      MENU_ITEM_F(ICON_ESDiag, "End-stops diag.", onDrawSubMenu, Draw_EndStopDiag);
    #endif
    #if ProUIex
      MENU_ITEM(ICON_PhySet, MSG_PHY_SET, onDrawSubMenu, Draw_PhySet_Menu);
    #endif
    #if HAS_HOME_OFFSET
      MENU_ITEM(ICON_HomeOffset, MSG_SET_HOME_OFFSETS, onDrawSubMenu, Draw_HomeOffset_Menu);
    #endif
      MENU_ITEM(ICON_Scolor, MSG_COLORS_SELECT, onDrawSubMenu, Draw_SelectColors_Menu);
    #if HAS_TOOLBAR
      MENU_ITEM(ICON_TBSetup, MSG_TOOLBAR_SETUP, onDrawSubMenu, Draw_TBSetup_Menu);
    #endif
    #if HAS_BED_PROBE || defined(MESH_BED_LEVELING)
      MENU_ITEM(ICON_Language, MSG_ADVANCED_SETTINGS, onDrawSubMenu, Draw_Advanced_Menu);
    #endif
    #if ENABLED(CASE_LIGHT_MENU)
      #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
        MENU_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawSubMenu, Draw_CaseLight_Menu);
      #else
        MENU_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
      #endif
    #endif
    #if ENABLED(LED_CONTROL_MENU)
      MENU_ITEM(ICON_LedControl, MSG_LED_CONTROL, onDrawSubMenu, Draw_LedControl_Menu);
    #endif
      MENU_ITEM(ICON_PrintStats, MSG_INFO_STATS_MENU, onDrawSubMenu, Goto_PrintStats);
      MENU_ITEM(ICON_Info, MSG_INFO_SCREEN, onDrawSubMenu, Goto_Info_Menu);
  }
  ui.reset_status(true);
  UpdateMenu(ControlMenu);
}

void Draw_Move_Menu() {
  checkkey = Menu;
  if (SET_MENU(MoveMenu, MSG_MOVE_AXIS, 6)) {
    BACK_ITEM(Draw_Prepare_Menu);
    EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, SetMoveX, &current_position.x);
    EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, SetMoveY, &current_position.y);
    EDIT_ITEM(ICON_MoveZ, MSG_MOVE_Z, onDrawPFloatMenu, SetMoveZ, &current_position.z);
    #if HAS_HOTEND
      EDIT_ITEM(ICON_Extruder, MSG_MOVE_E, onDrawPFloatMenu, SetMoveE, &current_position.e);
    #endif
    EDIT_ITEM_F(ICON_Version, "Live Adjustment", onDrawChkbMenu, SetLiveMove, &HMI_data.SetLiveMove);
  }
  UpdateMenu(MoveMenu);
  if (!all_axes_trusted()) LCD_MESSAGE_F("WARNING: Current position is unknown, Home axes.");
}

#if HAS_HOME_OFFSET
  void Draw_HomeOffset_Menu() {
    checkkey = Menu;
    if (SET_MENU(HomeOffMenu, MSG_SET_HOME_OFFSETS, 5)) {
      BACK_ITEM(Draw_Control_Menu);
      EDIT_ITEM(ICON_HomeOffsetX, MSG_HOME_OFFSET_X, onDrawPFloatMenu, SetHomeOffsetX, &home_offset[X_AXIS]);
      EDIT_ITEM(ICON_HomeOffsetY, MSG_HOME_OFFSET_Y, onDrawPFloatMenu, SetHomeOffsetY, &home_offset[Y_AXIS]);
      EDIT_ITEM(ICON_HomeOffsetZ, MSG_HOME_OFFSET_Z, onDrawPFloatMenu, SetHomeOffsetZ, &home_offset[Z_AXIS]);
      #if ProUIex
        #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_ParkPos, MSG_FILAMENT_PARK_ENABLED, onDrawSubMenu, Draw_ParkPos_Menu);
        #endif
      #endif  
    }
    UpdateMenu(HomeOffMenu);
  }
#endif

#if HAS_BED_PROBE
  void Draw_ProbeSet_Menu() {
    checkkey = Menu;
    if (SET_MENU(ProbeSetMenu, MSG_ZPROBE_SETTINGS, 11)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      EDIT_ITEM(ICON_ProbeOffsetX, MSG_ZPROBE_XOFFSET, onDrawPFloatMenu, SetProbeOffsetX, &probe.offset.x);
      EDIT_ITEM(ICON_ProbeOffsetY, MSG_ZPROBE_YOFFSET, onDrawPFloatMenu, SetProbeOffsetY, &probe.offset.y);
      EDIT_ITEM(ICON_ProbeOffsetZ, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetProbeOffsetZ, &probe.offset.z);
      #if ProUIex
        EDIT_ITEM(ICON_ProbeZSpeed, MSG_Z_FEED_RATE, onDrawPIntMenu, SetProbeZSpeed, &PRO_data.zprobefeedslow);
        EDIT_ITEM(ICON_Homing, MSG_ZPROBE_MULTIPLE, onDrawPInt8Menu, SetProbeMultiple, &PRO_data.multiple_probing);
      #endif
      #if ENABLED(BLTOUCH)
        MENU_ITEM(ICON_ProbeStow, MSG_MANUAL_STOW, onDrawMenuItem, ProbeStow);
        MENU_ITEM(ICON_ProbeDeploy, MSG_MANUAL_DEPLOY, onDrawMenuItem, ProbeDeploy);
        MENU_ITEM(ICON_BltouchReset, MSG_BLTOUCH_RESET, onDrawMenuItem, bltouch._reset);
        MENU_ITEM(ICON_ProbeTest, MSG_M48_TEST, onDrawMenuItem, ProbeTest);
        #ifdef BLTOUCH_HS_MODE
          EDIT_ITEM(ICON_HSMode, MSG_ENABLE_HS_MODE, onDrawChkbMenu, SetHSMode, &bltouch.high_speed_mode);
        #endif
      #endif
    }
    UpdateMenu(ProbeSetMenu);
  }
#endif

void Draw_FilSet_Menu() {
  checkkey = Menu;
  if (SET_MENU(FilSetMenu, MSG_FILAMENT_SET, 8)) {
    BACK_ITEM(Draw_FilamentMan_Menu);
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, SetRunoutEnable, &runout.enabled);
      #if ProUIex
        MENU_ITEM(ICON_Runout, MSG_RUNOUT_ACTIVE, onDrawRunoutActive, SetRunoutActive);
      #endif
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      EDIT_ITEM(ICON_FilLoad, MSG_FILAMENT_LOAD, onDrawPFloatMenu, SetFilLoad, &fc_settings[0].load_length);
      EDIT_ITEM(ICON_FilUnload, MSG_FILAMENT_UNLOAD, onDrawPFloatMenu, SetFilUnload, &fc_settings[0].unload_length);
    #endif
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_DISTANCE_MM, onDrawPFloatMenu, SetRunoutDistance, &runout.runout_distance());
    #endif
    #if BOTH(ProUIex, HAS_EXTRUDERS)
      EDIT_ITEM(ICON_InvertE0, MSG_INVERT_EXTRUDER, onDrawChkbMenu, SetInvertE0, &PRO_data.Invert_E0);
    #endif
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      EDIT_ITEM(ICON_ExtrudeMinT, MSG_EXTRUDER_MIN_TEMP, onDrawPIntMenu, SetExtMinT, &HMI_data.ExtMinT);
    #endif
  }
  UpdateMenu(FilSetMenu);
}

#if ProUIex
  #if ENABLED(NOZZLE_PARK_FEATURE)
    void Draw_ParkPos_Menu() {
      checkkey = Menu;
      if (SET_MENU(ParkPosMenu, MSG_FILAMENT_PARK_ENABLED, 4)) {
        BACK_ITEM(Draw_HomeOffset_Menu);
        EDIT_ITEM(ICON_ParkPosX, MSG_PARK_XPOSITION, onDrawPIntMenu, SetParkPosX, &PRO_data.Park_point.x);
        EDIT_ITEM(ICON_ParkPosY, MSG_PARK_YPOSITION, onDrawPIntMenu, SetParkPosY, &PRO_data.Park_point.y);
        EDIT_ITEM(ICON_ParkPosZ, MSG_PARK_ZRAISE, onDrawPIntMenu, SetParkZRaise, &PRO_data.Park_point.z);
      }
      UpdateMenu(ParkPosMenu);
    }
  #endif

  void Draw_PhySet_Menu() {
    checkkey = Menu;
    if (SET_MENU(PhySetMenu, MSG_PHY_SET, 8)) {
      BACK_ITEM(Draw_Control_Menu);
      EDIT_ITEM(ICON_BedSizeX, MSG_PHY_XBEDSIZE, onDrawPIntMenu, SetBedSizeX, &PRO_data.x_bed_size);
      EDIT_ITEM(ICON_BedSizeY, MSG_PHY_YBEDSIZE, onDrawPIntMenu, SetBedSizeY, &PRO_data.y_bed_size);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMINPOS, onDrawPIntMenu, SetMinPosX, &PRO_data.x_min_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMINPOS, onDrawPIntMenu, SetMinPosY, &PRO_data.y_min_pos);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMAXPOS, onDrawPIntMenu, SetMaxPosX, &PRO_data.x_max_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMAXPOS, onDrawPIntMenu, SetMaxPosY, &PRO_data.y_max_pos);
      EDIT_ITEM(ICON_MaxPosZ, MSG_PHY_ZMAXPOS, onDrawPIntMenu, SetMaxPosZ, &PRO_data.z_max_pos);
    }
    UpdateMenu(PhySetMenu);
  }
#endif

void Draw_SelectColors_Menu() {
  checkkey = Menu;
  if (SET_MENU(SelectColorMenu, MSG_COLORS_SELECT, 21)) {
    BACK_ITEM(Draw_Control_Menu);
    MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawMenuItem, RestoreDefaultsColors);
    EDIT_ITEM_F(0, "Screen Background", onDrawSelColorItem, SelColor, &HMI_data.Background_Color);
    EDIT_ITEM_F(0, "Cursor", onDrawSelColorItem, SelColor, &HMI_data.Cursor_color);
    EDIT_ITEM_F(0, "Title Background", onDrawSelColorItem, SelColor, &HMI_data.TitleBg_color);
    EDIT_ITEM_F(0, "Title Text", onDrawSelColorItem, SelColor, &HMI_data.TitleTxt_color);
    EDIT_ITEM_F(0, "Text", onDrawSelColorItem, SelColor, &HMI_data.Text_Color);
    EDIT_ITEM_F(0, "Selected", onDrawSelColorItem, SelColor, &HMI_data.Selected_Color);
    EDIT_ITEM_F(0, "Split Line", onDrawSelColorItem, SelColor, &HMI_data.SplitLine_Color);
    EDIT_ITEM_F(0, "Highlight", onDrawSelColorItem, SelColor, &HMI_data.Highlight_Color);
    EDIT_ITEM_F(0, "Status Background", onDrawSelColorItem, SelColor, &HMI_data.StatusBg_Color);
    EDIT_ITEM_F(0, "Status Text", onDrawSelColorItem, SelColor, &HMI_data.StatusTxt_Color);
    EDIT_ITEM_F(0, "Popup Background", onDrawSelColorItem, SelColor, &HMI_data.PopupBg_color);
    EDIT_ITEM_F(0, "Popup Text", onDrawSelColorItem, SelColor, &HMI_data.PopupTxt_Color);
    EDIT_ITEM_F(0, "Alert Background", onDrawSelColorItem, SelColor, &HMI_data.AlertBg_Color);
    EDIT_ITEM_F(0, "Alert Text", onDrawSelColorItem, SelColor, &HMI_data.AlertTxt_Color);
    EDIT_ITEM_F(0, "Percent Text", onDrawSelColorItem, SelColor, &HMI_data.PercentTxt_Color);
    EDIT_ITEM_F(0, "Bar Fill", onDrawSelColorItem, SelColor, &HMI_data.Barfill_Color);
    EDIT_ITEM_F(0, "Indicator value", onDrawSelColorItem, SelColor, &HMI_data.Indicator_Color);
    EDIT_ITEM_F(0, "Coordinate value", onDrawSelColorItem, SelColor, &HMI_data.Coordinate_Color);
    EDIT_ITEM_F(0, "Bottom Line", onDrawSelColorItem, SelColor, &HMI_data.Bottom_Color);
  }
  UpdateMenu(SelectColorMenu);
}

void Draw_GetColor_Menu() {
  checkkey = Menu;
  if (SET_MENU(GetColorMenu, MSG_COLORS_GET, 5)) {
    BACK_ITEM(DWIN_ApplyColor);
    MENU_ITEM(ICON_Cancel, MSG_BUTTON_CANCEL, onDrawMenuItem, Draw_SelectColors_Menu);
    MENU_ITEM(0, MSG_COLORS_RED, onDrawGetColorItem, SetRGBColor);
    MENU_ITEM(1, MSG_COLORS_GREEN, onDrawGetColorItem, SetRGBColor);
    MENU_ITEM(2, MSG_COLORS_BLUE, onDrawGetColorItem, SetRGBColor);
  }
  UpdateMenu(GetColorMenu);
  DWIN_Draw_Rectangle(1, *MenuData.P_Int, 20, 315, DWIN_WIDTH - 20, 335);
}

#if BOTH(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
  void Draw_CaseLight_Menu() {
    checkkey = Menu;
    if (SET_MENU(CaseLightMenu, MSG_CASE_LIGHT, 3)) {
      BACK_ITEM(Draw_Control_Menu);
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
      EDIT_ITEM(ICON_Brightness, MSG_CASE_LIGHT_BRIGHTNESS, onDrawPInt8Menu, SetCaseLightBrightness, &caselight.brightness);
    }
    UpdateMenu(CaseLightMenu);
  }
#endif

#if ENABLED(LED_CONTROL_MENU)
  void Draw_LedControl_Menu() {
    checkkey = Menu;
    if (SET_MENU(LedControlMenu, MSG_LED_CONTROL, 10)) {
      BACK_ITEM(Draw_Control_Menu);
      #if !BOTH(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
        EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, SetLedStatus, &leds.lights_on);
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
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_RED, onDrawPInt8Menu, SetLEDColorR, &leds.color.r);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_GREEN, onDrawPInt8Menu, SetLEDColorG, &leds.color.g);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_BLUE, onDrawPInt8Menu, SetLEDColorB, &leds.color.b);
          #if ENABLED(HAS_WHITE_LED)
            EDIT_ITEM(ICON_LedControl, MSG_COLORS_WHITE, onDrawPInt8Menu, SetLedColorW, &leds.color.w);
          #endif
        #endif
      #endif
    }
    UpdateMenu(LedControlMenu);
  }
#endif

void Draw_Tune_Menu() {
  checkkey = Menu;
  if (SET_MENU(TuneMenu, MSG_TUNE, 20)) {
    BACK_ITEM(Goto_PrintProcess);
    #if HAS_LCD_BRIGHTNESS
      MENU_ITEM(ICON_Box, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, SetSpeed, &feedrate_percentage);
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, SetFlow, &planner.flow_percentage[0]);
    #if HAS_HOTEND
      HotendTargetItem = EDIT_ITEM(ICON_HotendTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, SetHotendTemp, &thermalManager.temp_hotend[0].target);
    #endif
    #if HAS_HEATED_BED
      BedTargetItem = EDIT_ITEM(ICON_BedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      FanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, SetFanSpeed, &thermalManager.fan_speed[0]);
    #endif
    #if HAS_ZOFFSET_ITEM && EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
    EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
    #endif
    #if HAS_PIDPLOT
      MENU_ITEM_F(ICON_PIDNozzle, "Hotend Temp Graph", onDrawMenuItem, DWIN_Draw_Plot_Nozzle);
      MENU_ITEM_F(ICON_PIDBed, "Bed Temp Graph", onDrawMenuItem, DWIN_Draw_Plot_Bed);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, ChangeFilament);
    #endif
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, SetRunoutEnable, &runout.enabled);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, Draw_FWRetract_Menu);
    #endif
    #if HAS_JUNCTION_DEVIATION
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, SetJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(LIN_ADVANCE)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, SetLA_K, &planner.extruder_advance_K[0]);
    #endif
    #if LCD_BACKLIGHT_TIMEOUT_MINS
      EDIT_ITEM(ICON_Box, MSG_SCREEN_TIMEOUT, onDrawPIntMenu, SetTimer, &ui.backlight_timeout_minutes);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
    #endif
    #if ENABLED(CASE_LIGHT_MENU)
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
    #elif ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)
      EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, SetLedStatus, &leds.lights_on);
    #endif
  }
  UpdateMenu(TuneMenu);
}

void Draw_Motion_Menu() {
  checkkey = Menu;
  if (SET_MENU(MotionMenu, MSG_MOTION, 8)) {
    BACK_ITEM(Draw_Control_Menu);
    MENU_ITEM(ICON_MaxSpeed, MSG_SPEED, onDrawSubMenu, Draw_MaxSpeed_Menu);
    MENU_ITEM(ICON_MaxAccelerated, MSG_ACCELERATION, onDrawSubMenu, Draw_MaxAccel_Menu);
    #if HAS_CLASSIC_JERK
      MENU_ITEM(ICON_MaxJerk, MSG_JERK, onDrawSubMenu, Draw_MaxJerk_Menu);
    #elif HAS_JUNCTION_DEVIATION
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, SetJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(LIN_ADVANCE)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, SetLA_K, &planner.extruder_advance_K[0]);
    #endif
    MENU_ITEM(ICON_Step, MSG_STEPS_PER_MM, onDrawSubMenu, Draw_Steps_Menu);
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, SetFlow, &planner.flow_percentage[0]);
  }
  UpdateMenu(MotionMenu);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Draw_FilamentMan_Menu() {
    checkkey = Menu;
    if (SET_MENU(FilamentMenu, MSG_FILAMENT_MAN, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, ChangeFilament);
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        MENU_ITEM(ICON_FilUnload, MSG_FILAMENTUNLOAD, onDrawMenuItem, UnloadFilament);
        MENU_ITEM(ICON_FilLoad, MSG_FILAMENTLOAD, onDrawMenuItem, LoadFilament);
      #endif
      MENU_ITEM(ICON_FilSet, MSG_FILAMENT_SET, onDrawSubMenu, Draw_FilSet_Menu);
      #if ENABLED(FWRETRACT)
        MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, Draw_FWRetract_Menu);
      #endif
    }
    UpdateMenu(FilamentMenu);
  }
#endif

#if HAS_PREHEAT

  void Draw_Preheat_Menu(bool NotCurrent) {
    checkkey = Menu;
    if (NotCurrent) {
      BACK_ITEM(Draw_Temperature_Menu);
      #if HAS_HOTEND
        EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, SetPreheatEndTemp, &ui.material_preset[HMI_value.Select].hotend_temp);
      #endif
      #if HAS_HEATED_BED
        EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetPreheatBedTemp, &ui.material_preset[HMI_value.Select].bed_temp);
      #endif
      #if HAS_FAN
        EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPIntMenu, SetPreheatFanSpeed, &ui.material_preset[HMI_value.Select].fan_speed);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(PreheatMenu);
  }

  #define _Preheat_Menu(N) \
    void Draw_Preheat## N ##_Menu() { \
      HMI_value.Select = N-1; \
      Draw_Preheat_Menu(SET_MENU(PreheatMenu, MSG_PREHEAT_## N ##_SETTINGS, 5)); \
    }
  REPEAT_1(PREHEAT_COUNT, _Preheat_Menu)

#endif // HAS_PREHEAT

void Draw_Temperature_Menu() {
  checkkey = Menu;
  if (SET_MENU(TemperatureMenu, MSG_TEMPERATURE, 5 + PREHEAT_COUNT)) {
    BACK_ITEM(Draw_Control_Menu);
    #if HAS_HOTEND
      HotendTargetItem = EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, SetHotendTemp, &thermalManager.temp_hotend[0].target);
    #endif
    #if HAS_HEATED_BED
      BedTargetItem = EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      FanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, SetFanSpeed, &thermalManager.fan_speed[0]);
    #endif
    #if EITHER(PIDTEMPBED, PIDTEMP)
      MENU_ITEM_F(ICON_Temperature, "PID Settings", onDrawSubMenu, Draw_PID_Menu);
    #endif
    #if HAS_PREHEAT
      #define _ITEM_SETPREHEAT(N) MENU_ITEM(ICON_SetPreheat##N, MSG_PREHEAT_## N ##_SETTINGS, onDrawSubMenu, Draw_Preheat## N ##_Menu);
      REPEAT_1(PREHEAT_COUNT, _ITEM_SETPREHEAT)
    #endif
  }
  UpdateMenu(TemperatureMenu);
}

void Draw_MaxSpeed_Menu() {
  checkkey = Menu;
  if (SET_MENU(MaxSpeedMenu, MSG_MAX_SPEED, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    EDIT_ITEM(ICON_MaxSpeedX, MSG_VMAX_A, onDrawPFloatMenu, SetMaxSpeedX, &planner.settings.max_feedrate_mm_s[X_AXIS]);
    EDIT_ITEM(ICON_MaxSpeedY, MSG_VMAX_B, onDrawPFloatMenu, SetMaxSpeedY, &planner.settings.max_feedrate_mm_s[Y_AXIS]);
    EDIT_ITEM(ICON_MaxSpeedZ, MSG_VMAX_C, onDrawPFloatMenu, SetMaxSpeedZ, &planner.settings.max_feedrate_mm_s[Z_AXIS]);
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxSpeedE, MSG_VMAX_E, onDrawPFloatMenu, SetMaxSpeedE, &planner.settings.max_feedrate_mm_s[E_AXIS]);
    #endif
  }
  UpdateMenu(MaxSpeedMenu);
}

void Draw_MaxAccel_Menu() {
  checkkey = Menu;
  if (SET_MENU(MaxAccelMenu, MSG_AMAX_EN, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    EDIT_ITEM(ICON_MaxAccX, MSG_AMAX_A, onDrawPInt32Menu, SetMaxAccelX, &planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
    EDIT_ITEM(ICON_MaxAccY, MSG_AMAX_B, onDrawPInt32Menu, SetMaxAccelY, &planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    EDIT_ITEM(ICON_MaxAccZ, MSG_AMAX_C, onDrawPInt32Menu, SetMaxAccelZ, &planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxAccE, MSG_AMAX_E, onDrawPInt32Menu, SetMaxAccelE, &planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
    #endif
  }
  UpdateMenu(MaxAccelMenu);
}

#if HAS_CLASSIC_JERK
  void Draw_MaxJerk_Menu() {
    checkkey = Menu;
    if (SET_MENU(MaxJerkMenu, MSG_JERK, 5)) {
      BACK_ITEM(Draw_Motion_Menu);
      EDIT_ITEM(ICON_MaxSpeedJerkX, MSG_VA_JERK, onDrawPFloatMenu, SetMaxJerkX, &planner.max_jerk[X_AXIS]);
      EDIT_ITEM(ICON_MaxSpeedJerkY, MSG_VB_JERK, onDrawPFloatMenu, SetMaxJerkY, &planner.max_jerk[Y_AXIS]);
      EDIT_ITEM(ICON_MaxSpeedJerkZ, MSG_VC_JERK, onDrawPFloatMenu, SetMaxJerkZ, &planner.max_jerk[Z_AXIS]);
      #if HAS_HOTEND
        EDIT_ITEM(ICON_MaxSpeedJerkE, MSG_VE_JERK, onDrawPFloatMenu, SetMaxJerkE, &planner.max_jerk[E_AXIS]);
      #endif
    }
    UpdateMenu(MaxJerkMenu);
  }
#endif

void Draw_Steps_Menu() {
  checkkey = Menu;
  if (SET_MENU(StepsMenu, MSG_STEPS_PER_MM, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    EDIT_ITEM(ICON_StepX, MSG_A_STEPS, onDrawPFloat2Menu, SetStepsX, &planner.settings.axis_steps_per_mm[X_AXIS]);
    EDIT_ITEM(ICON_StepY, MSG_B_STEPS, onDrawPFloat2Menu, SetStepsY, &planner.settings.axis_steps_per_mm[Y_AXIS]);
    EDIT_ITEM(ICON_StepZ, MSG_C_STEPS, onDrawPFloat2Menu, SetStepsZ, &planner.settings.axis_steps_per_mm[Z_AXIS]);
    #if HAS_HOTEND
      EDIT_ITEM(ICON_StepE, MSG_E_STEPS, onDrawPFloat2Menu, SetStepsE, &planner.settings.axis_steps_per_mm[E_AXIS]);
    #endif
  }
  UpdateMenu(StepsMenu);
}

  void Draw_PID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(PIDMenu, "PID Settings", 3)) {
      BACK_ITEM(Draw_Temperature_Menu);
    #ifdef PIDTEMP
      MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID " Settings", onDrawSubMenu, Draw_HotendPID_Menu);
    #endif
    #ifdef PIDTEMPBED
      MENU_ITEM_F(ICON_PIDBed, STR_BED_PID " Settings", onDrawSubMenu, Draw_BedPID_Menu);
    #endif
    }
    UpdateMenu(PIDMenu);
  }

#if ENABLED(PIDTEMP)
  void Draw_HotendPID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(HotendPIDMenu, STR_HOTEND_PID " Settings", 8)) {
      BACK_ITEM(Draw_PID_Menu);
      MENU_ITEM_F(ICON_PIDNozzle, "Tune " STR_HOTEND_PID, onDrawMenuItem, HotendPID);
      EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, SetKp, &thermalManager.temp_hotend[0].pid.Kp);
      EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, SetKi, &thermalManager.temp_hotend[0].pid.Ki);
      EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, SetKd, &thermalManager.temp_hotend[0].pid.Kd);
      EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, SetHotendPidT, &HMI_data.HotendPidT);
      EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, SetPidCycles, &HMI_data.PidCycles);
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(HotendPIDMenu);
  }
#endif

#if ENABLED(MPCTEMP)

  void HotendMPC() { thermalManager.MPC_autotune(); }
  void SetHeaterPower() { SetPFloatOnClick(1, 200, 1); }
  void SetBlkHeatCapacity() { SetPFloatOnClick(0, 40, 2); }
  void SetSensorRespons() { SetPFloatOnClick(0, 1, 4); }
  void SetAmbientXfer() { SetPFloatOnClick(0, 1, 4); }
  #if ENABLED(MPC_INCLUDE_FAN)
    void onDrawFanAdj(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 4, thermalManager.temp_hotend[0].constants.ambient_xfer_coeff_fan0 + thermalManager.temp_hotend[0].constants.fan255_adjustment); }
    void ApplyFanAdj() { thermalManager.temp_hotend[0].constants.fan255_adjustment = MenuData.Value / POW(10, 4) - thermalManager.temp_hotend[0].constants.ambient_xfer_coeff_fan0; }
    void SetFanAdj() { SetFloatOnClick(0, 1, 4, thermalManager.temp_hotend[0].constants.ambient_xfer_coeff_fan0 + thermalManager.temp_hotend[0].constants.fan255_adjustment, ApplyFanAdj); }
  #endif

  void Draw_HotendMPC_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(HotendMPCMenu, STR_MPC_AUTOTUNE " Settings", 7)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      MENU_ITEM(ICON_MPCNozzle, MSG_MPC_AUTOTUNE, onDrawMenuItem, HotendMPC);
      EDIT_ITEM(ICON_MPCHeater, MSG_MPC_POWER, onDrawPFloatMenu, SetHeaterPower, &thermalManager.temp_hotend[0].constants.heater_power);
      EDIT_ITEM(ICON_MPCHeatCap, MSG_MPC_BLOCK_HEAT_CAPACITY, onDrawPFloat2Menu, SetBlkHeatCapacity, &thermalManager.temp_hotend[0].constants.block_heat_capacity);
      EDIT_ITEM(ICON_MPCValue, MSG_SENSOR_RESPONSIVENESS, onDrawPFloat4Menu, SetSensorRespons, &thermalManager.temp_hotend[0].constants.sensor_responsiveness);
      EDIT_ITEM(ICON_MPCValue, MSG_MPC_AMBIENT_XFER_COEFF, onDrawPFloat4Menu, SetAmbientXfer, &thermalManager.temp_hotend[0].constants.ambient_xfer_coeff_fan0);
      #if ENABLED(MPC_INCLUDE_FAN)
        EDIT_ITEM(ICON_MPCFan, MSG_MPC_AMBIENT_XFER_COEFF_FAN, onDrawFanAdj, SetFanAdj, &thermalManager.temp_hotend[0].constants.fan255_adjustment);
      #endif
    }
    UpdateMenu(HotendMPCMenu);
  }
#endif // MPCTEMP


#if ENABLED(PIDTEMPBED)
  void Draw_BedPID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(BedPIDMenu, STR_BED_PID " Settings", 8)) {
      BACK_ITEM(Draw_PID_Menu);
      MENU_ITEM_F(ICON_PIDBed, "Tune " STR_BED_PID, onDrawMenuItem, BedPID);
      EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, SetKp, &thermalManager.temp_bed.pid.Kp);
      EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, SetKi, &thermalManager.temp_bed.pid.Ki);
      EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, SetKd, &thermalManager.temp_bed.pid.Kd);
      EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, SetBedPidT, &HMI_data.BedPidT);
      EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, SetPidCycles, &HMI_data.PidCycles);
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(BedPIDMenu);
  }
#endif

#if HAS_ZOFFSET_ITEM
  void Draw_ZOffsetWiz_Menu() {
    checkkey = Menu;
    if (SET_MENU(ZOffsetWizMenu, MSG_PROBE_WIZARD, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      MENU_ITEM(ICON_SetHome, MSG_MOVE_NOZZLE_TO_BED, onDrawMenuItem, SetMoveZto0);
      EDIT_ITEM(ICON_Fade, MSG_XATC_UPDATE_Z_OFFSET, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
      MENU_ITEM_F(0,"For Best Results:\n", onDrawMenuItem);
      MENU_ITEM_F(ICON_More, "Make Nozzle Touch Bed", onDrawMenuItem);
    }
    UpdateMenu(ZOffsetWizMenu);
    if (!axis_is_trusted(Z_AXIS)) LCD_MESSAGE_F("WARNING: unknown Z position, Home Z axis...");
  }
#endif

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void Draw_Homing_Menu() {
    checkkey = Menu;
    if (SET_MENU(HomingMenu, MSG_HOMING, 7)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      MENU_ITEM(ICON_HomeX, MSG_AUTO_HOME_X, onDrawMenuItem, HomeX);
      MENU_ITEM(ICON_HomeY, MSG_AUTO_HOME_Y, onDrawMenuItem, HomeY);
      MENU_ITEM(ICON_HomeZ, MSG_AUTO_HOME_Z, onDrawMenuItem, HomeZ);
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_Park, MSG_FILAMENT_PARK_ENABLED, onDrawMenuItem, ParkHead);
      #endif
      #if ENABLED(MESH_BED_LEVELING)
        EDIT_ITEM(ICON_ZAfterHome, MSG_Z_AFTER_HOME, onDrawPInt8Menu, SetZAfterHoming, &HMI_data.z_after_homing);
      #endif
    }
    UpdateMenu(HomingMenu);
  }
#endif

#if ENABLED(FWRETRACT)
  void Draw_FWRetract_Menu() {
    checkkey = Menu;
    if (SET_MENU(FWRetractMenu, MSG_FWRETRACT, 6)) {
      BACK_ITEM(Return_FWRetract_Menu);
      EDIT_ITEM(ICON_FWRetLength, MSG_CONTROL_RETRACT, onDrawPFloatMenu, SetRetractLength, &fwretract.settings.retract_length);
      EDIT_ITEM(ICON_FWRetSpeed, MSG_SINGLENOZZLE_RETRACT_SPEED, onDrawPFloatMenu, SetRetractSpeed, &fwretract.settings.retract_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRetZRaise, MSG_CONTROL_RETRACT_ZHOP, onDrawPFloat2Menu, SetZRaise, &fwretract.settings.retract_zraise);
      EDIT_ITEM(ICON_FWRecSpeed, MSG_SINGLENOZZLE_UNRETRACT_SPEED, onDrawPFloatMenu, SetRetractSpeed, &fwretract.settings.retract_recover_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRecExtra, MSG_CONTROL_RETRACT_RECOVER, onDrawPFloatMenu, SetAddRecover, &fwretract.settings.retract_recover_extra);
    }
    UpdateMenu(FWRetractMenu);
  }
#endif

//=============================================================================
// Mesh Bed Leveling
//=============================================================================

#if HAS_MESH

  #if ProUIex
    void ApplyMeshPoints() { ProEx.ApplyMeshPoints(); ReDrawMenu(); }
    void LiveMeshPoints() { ProEx.DrawMeshPoints(true, CurrentMenu->line(), MenuData.Value); }
    void SetMeshPoints() {
      SetOnClick(SetIntNoDraw, GRID_MIN, GRID_LIMIT, 0, PRO_data.grid_max_points, ApplyMeshPoints, LiveMeshPoints);
      ProEx.DrawMeshPoints(true, CurrentMenu->line(), PRO_data.grid_max_points);
    }
    void SetMeshInset()  { SetPFloatOnClick(MIN_MESH_INSET, MAX_MESH_INSET, UNITFDIGITS, ProEx.ApplyMeshLimits); }
    void MaxMeshArea() {
      //#if HAS_BED_PROBE
      // PRO_data.mesh_min_x = _MAX(PROBING_MARGIN_LEFT, probe.offset.x);
      // PRO_data.mesh_max_x = _MIN(X_BED_SIZE - PROBING_MARGIN_RIGHT, X_MAX_POS + probe.offset.x);
      // PRO_data.mesh_min_y = _MAX(PROBING_MARGIN_FRONT, probe.offset.y);
      // PRO_data.mesh_max_y = _MIN(Y_BED_SIZE - PROBING_MARGIN_BACK, Y_MAX_POS + probe.offset.y);
      //#else
        PRO_data.mesh_min_x = PROBING_MARGIN_LEFT;
        PRO_data.mesh_max_x = X_BED_SIZE - PROBING_MARGIN_RIGHT;
        PRO_data.mesh_min_y = PROBING_MARGIN_FRONT;
        PRO_data.mesh_max_y = Y_BED_SIZE - PROBING_MARGIN_BACK;
      //#endif
      ProEx.ApplyMeshLimits();
      ReDrawMenu();
    }
    void CenterMeshArea() {
      float max = PRO_data.mesh_min_x;
      if (max < X_BED_SIZE - PRO_data.mesh_max_x) max = X_BED_SIZE - PRO_data.mesh_max_x;
      if (max < PRO_data.mesh_min_y) max = PRO_data.mesh_min_y;
      if (max < Y_BED_SIZE - PRO_data.mesh_max_y) max = Y_BED_SIZE - PRO_data.mesh_max_y;
      PRO_data.mesh_min_x = max;
      PRO_data.mesh_max_x = X_BED_SIZE - max;
      PRO_data.mesh_min_y = max;
      PRO_data.mesh_max_y = Y_BED_SIZE - max;
      ProEx.ApplyMeshLimits();
      ReDrawMenu();
    }
  #endif

  void ApplyMeshFadeHeight() { set_z_fade_height(planner.z_fade_height); }
  void SetMeshFadeHeight() { SetPFloatOnClick(0, 100, 1, ApplyMeshFadeHeight); }

  void SetMeshActive() {
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
    Show_Chkb_Line(CurrentMenu->line(), planner.leveling_active);
  }

  #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    void SetBedLevT() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  #if ENABLED(MESH_EDIT_MENU)
    bool AutoMovToMesh = false;
    #define Z_OFFSET_MIN -3
    #define Z_OFFSET_MAX  3

    void LiveEditMesh() { ((MenuItemPtrClass*)EditZValueItem)->value = &bedlevel.z_values[HMI_value.Select ? BedLevelTools.mesh_x : MenuData.Value][HMI_value.Select ? MenuData.Value : BedLevelTools.mesh_y]; EditZValueItem->redraw(); }
    void LiveEditMeshZ() { *MenuData.P_Float = MenuData.Value / POW(10, 3); if (AutoMovToMesh) BedLevelTools.MoveToZ(); }
    void ApplyEditMeshX() { BedLevelTools.mesh_x = MenuData.Value; if (AutoMovToMesh) BedLevelTools.MoveToXY(); }
    void ApplyEditMeshY() { BedLevelTools.mesh_y = MenuData.Value; if (AutoMovToMesh) BedLevelTools.MoveToXY(); }

    void SetEditMeshX() { HMI_value.Select = 0; SetIntOnClick(0, GRID_MAX_POINTS_X - 1, BedLevelTools.mesh_x, ApplyEditMeshX, LiveEditMesh); }
    void SetEditMeshY() { HMI_value.Select = 1; SetIntOnClick(0, GRID_MAX_POINTS_Y - 1, BedLevelTools.mesh_y, ApplyEditMeshY, LiveEditMesh); }
    void SetEditZValue() { SetPFloatOnClick(Z_OFFSET_MIN, Z_OFFSET_MAX, 3, nullptr, LiveEditMeshZ); if (AutoMovToMesh) BedLevelTools.MoveToXYZ(); }

    void SetAutoMovToMesh() {
      AutoMovToMesh = !AutoMovToMesh;
      Show_Chkb_Line(CurrentMenu->line(), AutoMovToMesh);
    }
  #endif
  
// Clear or Zero Bed Mesh Values
  void Popup_ZeroMesh() { DWIN_Popup_ConfirmCancel(ICON_Pause_0, F("Zero Current Mesh?")); }
  void OnClick_ZeroMesh() { 
    if (HMI_flag.select_flag) { 
    ZERO(bedlevel.z_values);
    DONE_BUZZ(true);
    }
    HMI_ReturnScreen();
  }
  void ZeroCurrentMesh() { Goto_Popup(Popup_ZeroMesh, OnClick_ZeroMesh); }

#endif // HAS_MESH

#if ENABLED(AUTO_BED_LEVELING_UBL)

  void ApplyUBLSlot() { bedlevel.storage_slot = MenuData.Value; }
  void SetUBLSlot() { SetIntOnClick(0, settings.calc_num_meshes() - 1, bedlevel.storage_slot, ApplyUBLSlot); }
  void onDrawUBLSlot(MenuItemClass* menuitem, int8_t line) {
    if (bedlevel.storage_slot < 0) bedlevel.storage_slot = 0;
    onDrawIntMenu(menuitem, line, bedlevel.storage_slot);
  }

  void ApplyUBLTiltGrid() { BedLevelTools.tilt_grid = MenuData.Value; }
  void SetUBLTiltGrid() { SetIntOnClick(1, 3, BedLevelTools.tilt_grid, ApplyUBLTiltGrid); }

  void UBLTiltMesh() {
    if (bedlevel.storage_slot < 0) bedlevel.storage_slot = 0;
    char buf[15];
    if (BedLevelTools.tilt_grid > 1) {
      sprintf_P(buf, PSTR("G28O\nG29 J%i"), BedLevelTools.tilt_grid);
      gcode.process_subcommands_now(buf);
    }
    else
      gcode.process_subcommands_now(F("G28O\nG29J"));
    LCD_MESSAGE(MSG_UBL_MESH_TILTED);
  }

  void UBLSmartFillMesh() {
    LOOP_L_N(x, TERN(ProUIex, GRID_LIMIT, GRID_MAX_POINTS_X)) bedlevel.smart_fill_mesh();
    LCD_MESSAGE(MSG_UBL_MESH_FILLED);
  }

  void UBLSaveMesh() {
    if (bedlevel.storage_slot < 0) bedlevel.storage_slot = 0;
    settings.store_mesh(bedlevel.storage_slot);
    ui.status_printf(0, GET_TEXT_F(MSG_MESH_SAVED), bedlevel.storage_slot);
    DONE_BUZZ(true);
  }

  void UBLLoadMesh() {
    if (bedlevel.storage_slot < 0) bedlevel.storage_slot = 0;
    settings.load_mesh(bedlevel.storage_slot);
    /* #if ENABLED(DWIN_LCD_PROUI)
      if (BedLevelTools.meshvalidate()) {
        ui.status_printf(0, GET_TEXT_F(MSG_MESH_LOADED), bedlevel.storage_slot);
        DONE_BUZZ(true);
      }
      else {
        bedlevel.invalidate();
        LCD_MESSAGE(MSG_UBL_MESH_INVALID);
        DONE_BUZZ(false);
      }
    #endif */
  }

#endif  // AUTO_BED_LEVELING_UBL

#if HAS_MESH
  void Draw_MeshSet_Menu() {
    checkkey = Menu;
    if (SET_MENU(MeshMenu, MSG_MESH_LEVELING, 7)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      EDIT_ITEM(ICON_Version, MSG_ACTIVATE_MESH, onDrawChkbMenu, SetMeshActive, &planner.leveling_active);
      #if ProUIex
        MENU_ITEM(ICON_UBLActive, MSG_MESH_POINTS, onDrawMeshPoints, SetMeshPoints);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_INSET, onDrawSubMenu, Draw_MeshInset_Menu);
      #endif
      #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
        EDIT_ITEM(ICON_Temperature, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedLevT, &HMI_data.BedLevT);
      #endif
      EDIT_ITEM(ICON_Fade, MSG_Z_FADE_HEIGHT, onDrawPFloatMenu, SetMeshFadeHeight, &planner.z_fade_height);
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        EDIT_ITEM(ICON_Tilt, MSG_UBL_TILTING_GRID, onDrawPInt8Menu, SetUBLTiltGrid, &BedLevelTools.tilt_grid);
      #endif
    }
    UpdateMenu(MeshMenu);
  }

  #if ENABLED(MESH_EDIT_MENU)
    void Draw_EditMesh_Menu() {
      if (!leveling_is_valid()) { LCD_MESSAGE(MSG_UBL_MESH_INVALID); return; }
      set_bed_leveling_enabled(false);
      checkkey = Menu;
      if (SET_MENU(EditMeshMenu, MSG_MESH_EDITOR, 6)) {
        BedLevelTools.mesh_x = BedLevelTools.mesh_y = 0;
        BACK_ITEM(Draw_AdvancedSettings_Menu);
        EDIT_ITEM(ICON_SetHome, MSG_PROBE_WIZARD_MOVING, onDrawChkbMenu, SetAutoMovToMesh, &AutoMovToMesh);
        EDIT_ITEM(ICON_HomeX, MSG_MESH_X, onDrawPInt8Menu, SetEditMeshX, &BedLevelTools.mesh_x);
        EDIT_ITEM(ICON_HomeY, MSG_MESH_Y, onDrawPInt8Menu, SetEditMeshY, &BedLevelTools.mesh_y);
        EditZValueItem = EDIT_ITEM(ICON_HomeZ, MSG_MESH_EDIT_Z, onDrawPFloat3Menu, SetEditZValue, &bedlevel.z_values[BedLevelTools.mesh_x][BedLevelTools.mesh_y]);
        TERN_(HAS_BED_PROBE, MENU_ITEM(ICON_HSMode, MSG_PROBE_WIZARD_PROBING, onDrawMenuItem, BedLevelTools.ProbeXY));
      }
      UpdateMenu(EditMeshMenu);
    }
  #endif

  #if ProUIex
    void Draw_MeshInset_Menu() {
      checkkey = Menu;
      if (SET_MENU(MeshInsetMenu, MSG_MESH_INSET, 7)) {
        BACK_ITEM(Draw_MeshSet_Menu);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_X, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_min_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_X, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_max_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_Y, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_min_y);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_Y, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_max_y);
        MENU_ITEM(ICON_Axis, MSG_MESH_AMAX, onDrawMenuItem, MaxMeshArea);
        MENU_ITEM(ICON_SetHome, MSG_MESH_CENTER, onDrawMenuItem, CenterMeshArea);
      }
      UpdateMenu(MeshInsetMenu);
    }
  #endif

#endif  // HAS_MESH


//=============================================================================
// ToolBar
//=============================================================================
#if HAS_TOOLBAR

  void Draw_TBSetup_Menu() {
    checkkey = Menu;
    if (SET_MENU(TBSetupMenu, MSG_TOOLBAR_SETUP, TBMaxOpt + 1)) {
      BACK_ITEM(Draw_Control_Menu);
      LOOP_L_N(i, TBMaxOpt) EDIT_ITEM_F(0, "", onDrawTBSetupItem, SetTBSetupItem, &PRO_data.TBopt[i]);
    }
    UpdateMenu(TBSetupMenu);
  }

  void Exit_ToolBar() {
    select_page.set(PAGE_ADVANCE);
    ICON_AdvSettings();
    checkkey = MainMenu;
    ToolBar.draw();
    DWIN_UpdateLCD();
  }

  void Goto_ToolBar() {
    checkkey = Menu;
    ToolBar.draw();
  }

#endif  // HAS_TOOLBAR

//=============================================================================
// More Host support
//=============================================================================

#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)

  void PopUp_HostShutDown() { DWIN_Popup_ConfirmCancel(ICON_Info_1, GET_TEXT_F(MSG_HOST_SHUTDOWN)); }
  void onClick_HostShutDown() {
    if (HMI_flag.select_flag) hostui.shutdown();
    HMI_ReturnScreen();
  }
  void HostShutDown() { Goto_Popup(PopUp_HostShutDown, onClick_HostShutDown); }

#endif

//=============================================================================

#if DEBUG_DWIN
  void DWIN_Debug(const char *msg) {
    SERIAL_ECHOLNPGM_P(msg);
    DWIN_Show_Popup(0, "Debug", msg, BTN_Continue);
    wait_for_user_response();
    Draw_Main_Area();
  }
#endif

#ifdef AUTO_BED_LEVELING_UBL
void Draw_AdvancedSettings_Menu() {
  checkkey = Menu;
  if (SET_MENU(AdvancedSettings, MSG_UBL_LEVELING, 14)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
    #endif
    #if HAS_MESH
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, Draw_ProbeSet_Menu);
      #endif
        MENU_ITEM(ICON_PrintSize, MSG_MESH_LEVELING, onDrawSubMenu, Draw_MeshSet_Menu);
        MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, AutoLev);
        MENU_ITEM(ICON_ProbeMargin, MSG_UBL_TILT_MESH, onDrawMenuItem, UBLTiltMesh);
      #if ENABLED(MESH_EDIT_MENU)
        MENU_ITEM(ICON_UBLActive, MSG_EDIT_MESH, onDrawSubMenu, Draw_EditMesh_Menu);
      #endif
        MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);  
      #if ENABLED(USE_UBL_VIEWER)
        EDIT_ITEM_F(ICON_PrintSize, "Change Mesh Viewer", onDrawChkbMenu, SetViewMesh, &BedLevelTools.view_mesh); 
      #endif
      EDIT_ITEM(ICON_ResumeEEPROM, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, SetUBLSlot, &bedlevel.storage_slot);
      MENU_ITEM(ICON_WriteEEPROM, MSG_UBL_SAVE_MESH, onDrawMenuItem, UBLSaveMesh);
      MENU_ITEM(ICON_ReadEEPROM, MSG_UBL_LOAD_MESH, onDrawMenuItem, UBLLoadMesh);
      MENU_ITEM(ICON_HSMode, MSG_UBL_SMART_FILLIN, onDrawMenuItem, UBLSmartFillMesh);
      MENU_ITEM_F(ICON_SetZOffset, "Zero Current Mesh", onDrawSubMenu, ZeroCurrentMesh);
    #endif 
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedSettings);
}

#elif defined(AUTO_BED_LEVELING_BILINEAR)
void Draw_AdvancedSettings_Menu() {
  checkkey = Menu;
  if (SET_MENU(AdvancedSettings, MSG_BILINEAR_LEVELING, 14)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
    #endif
    #if HAS_MESH
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, Draw_ProbeSet_Menu);
      #endif
        MENU_ITEM(ICON_PrintSize, MSG_MESH_LEVELING, onDrawSubMenu, Draw_MeshSet_Menu);
        MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);  
      #if ENABLED(USE_UBL_VIEWER)
        EDIT_ITEM_F(ICON_PrintSize, "Change Mesh Viewer", onDrawChkbMenu, SetViewMesh, &BedLevelTools.view_mesh); 
      #endif
      #if ENABLED(MESH_EDIT_MENU)
        MENU_ITEM(ICON_UBLActive, MSG_EDIT_MESH, onDrawSubMenu, Draw_EditMesh_Menu);
      #endif
        MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, AutoLev);
        EDIT_ITEM(ICON_ResumeEEPROM, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, SetUBLSlot, &bedlevel.storage_slot);
        MENU_ITEM(ICON_WriteEEPROM, MSG_UBL_SAVE_MESH, onDrawMenuItem, UBLSaveMesh);
        MENU_ITEM(ICON_ReadEEPROM, MSG_UBL_LOAD_MESH, onDrawMenuItem, UBLLoadMesh);
        MENU_ITEM(ICON_ProbeMargin, MSG_UBL_TILT_MESH, onDrawMenuItem, UBLTiltMesh);
        MENU_ITEM(ICON_HSMode, MSG_UBL_SMART_FILLIN, onDrawMenuItem, UBLSmartFillMesh);
        MENU_ITEM_F(ICON_SetZOffset, "Zero Current Mesh", onDrawSubMenu, ZeroCurrentMesh);
    #endif 
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedSettings);
}

#elif defined(MESH_BED_LEVELING)
void Draw_AdvancedSettings_Menu() {
  checkkey = Menu;
  if (SET_MENU(AdvancedSettings, MSG_UBL_MESH_LEVELING, 12)) {
    BACK_ITEM(Goto_Main_Menu);
    MENU_ITEM(ICON_ManualMesh, MSG_UBL_CONTINUE_MESH, onDrawMenuItem, ManualMeshStart);
    MMeshMoveZItem = EDIT_ITEM(ICON_Zoffset, MSG_MESH_EDIT_Z, onDrawPFloat2Menu, SetMMeshMoveZ, &current_position.z);
    MENU_ITEM(ICON_Axis, MSG_LEVEL_BED_NEXT_POINT, onDrawMenuItem, ManualMeshContinue);
    MENU_ITEM(ICON_PrintSize, MSG_MESH_LEVELING, onDrawSubMenu, Draw_MeshSet_Menu);
    MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);
    #if ENABLED(USE_UBL_VIEWER)
      EDIT_ITEM_F(ICON_PrintSize, "Change Mesh Viewer", onDrawChkbMenu, SetViewMesh, &BedLevelTools.view_mesh); 
    #endif
    #if ENABLED(MESH_EDIT_MENU)
      MENU_ITEM(ICON_UBLActive, MSG_EDIT_MESH, onDrawSubMenu, Draw_EditMesh_Menu);
    #endif
    MENU_ITEM(ICON_MeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, ManualMeshSave);
    EDIT_ITEM(ICON_ResumeEEPROM, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, SetUBLSlot, &bedlevel.storage_slot);
    MENU_ITEM(ICON_ReadEEPROM, MSG_UBL_LOAD_MESH, onDrawMenuItem, UBLLoadMesh);
    MENU_ITEM_F(ICON_SetZOffset, "Zero Current Mesh", onDrawSubMenu, ZeroCurrentMesh);
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedSettings);
}

#else
void Draw_AdvancedSettings_Menu() {
  checkkey = Menu;
  if (SET_MENU(AdvancedSettings, MSG_ADVANCED_SETTINGS, 22)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      MENU_ITEM(ICON_ReadEEPROM, MSG_LOAD_EEPROM, onDrawMenuItem, ReadEeprom);
      MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawMenuItem, ResetEeprom);
    #endif
    #if HAS_MESH
      MENU_ITEM(ICON_ProbeSet, MSG_MESH_LEVELING, onDrawSubMenu, Draw_MeshSet_Menu);
    #endif
    #if HAS_BED_PROBE
      MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, Draw_ProbeSet_Menu);
    #endif
    #if ProUIex
      MENU_ITEM(ICON_PhySet, MSG_PHY_SET, onDrawSubMenu, Draw_PhySet_Menu);
    #endif
    #if HAS_HOME_OFFSET
      MENU_ITEM(ICON_HomeOffset, MSG_SET_HOME_OFFSETS, onDrawSubMenu, Draw_HomeOffset_Menu);
    #endif
    #if HAS_TOOLBAR
      MENU_ITEM(ICON_TBSetup, MSG_TOOLBAR_SETUP, onDrawSubMenu, Draw_TBSetup_Menu);
    #endif
    #if HAS_ESDIAG
      MENU_ITEM_F(ICON_ESDiag, "End-stops diag.", onDrawSubMenu, Draw_EndStopDiag);
    #endif
      MENU_ITEM(ICON_Scolor, MSG_COLORS_SELECT, onDrawSubMenu, Draw_SelectColors_Menu);
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
    #if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
      MENU_ITEM(ICON_Host, MSG_HOST_SHUTDOWN, onDrawMenuItem, HostShutDown);
    #endif
    #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_SOUND, onDrawChkbMenu, SetEnableSound, &ui.sound_on); //changed
    #endif
      #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_TICK, onDrawChkbMenu, SetEnableTick, &ui.no_tick); //changed
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, SetPwrLossr, &recovery.enabled);
    #endif
    #if HAS_SD_EXTENDER
      EDIT_ITEM(ICON_File, MSG_MEDIA_UPDATE, onDrawChkbMenu, SetMediaAutoMount, &HMI_data.MediaAutoMount);
    #endif
    #if ENABLED(BAUD_RATE_GCODE)
      EDIT_ITEM_F(ICON_SetBaudRate, "250K baud", onDrawChkbMenu, SetBaudRate, &HMI_data.Baud250K);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(ICON_PrintStats, MSG_INFO_STATS_MENU, onDrawSubMenu, Goto_PrintStats);
      MENU_ITEM(ICON_PrintStatsReset, MSG_INFO_PRINT_COUNT_RESET, onDrawSubMenu, PrintStats.Reset);
    #endif
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedSettings);
}
#endif

void Draw_Advanced_Menu() { // Control --> Advaned Settings Menu
  checkkey = Menu;
  if (SET_MENU(AdvancedMenu, MSG_ADVANCED_SETTINGS, 11)) {
    BACK_ITEM(Draw_Control_Menu);
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
    #endif
    #if LCD_BACKLIGHT_TIMEOUT_MINS
      EDIT_ITEM(ICON_Box, MSG_SCREEN_TIMEOUT, onDrawPIntMenu, SetTimer, &ui.backlight_timeout_minutes); //added
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, SetPwrLossr, &recovery.enabled);
    #endif
      EDIT_ITEM_F(ICON_ProbeMargin, "Bed Screw Inset", onDrawPFloatMenu, SetRetractSpeed, &ui.screw_pos); //changed
    #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_TICK, onDrawChkbMenu, SetEnableTick, &ui.no_tick); //changed
      EDIT_ITEM(ICON_Sound, MSG_SOUND, onDrawChkbMenu, SetEnableSound, &ui.sound_on); //changed
    #endif
    #if ENABLED(BAUD_RATE_GCODE)
      EDIT_ITEM_F(ICON_SetBaudRate, "250K baud", onDrawChkbMenu, SetBaudRate, &HMI_data.Baud250K);
    #endif
    #if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
      MENU_ITEM(ICON_Host, MSG_HOST_SHUTDOWN, onDrawMenuItem, HostShutDown);
    #endif
    #if HAS_SD_EXTENDER
      EDIT_ITEM(ICON_File, MSG_MEDIA_UPDATE, onDrawChkbMenu, SetMediaAutoMount, &HMI_data.MediaAutoMount);
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_ReadEEPROM, MSG_LOAD_EEPROM, onDrawMenuItem, ReadEeprom);
      MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawMenuItem, ResetEeprom);
    #endif
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(ICON_PrintStatsReset, MSG_INFO_PRINT_COUNT_RESET, onDrawSubMenu, PrintStatsReset);
    #endif
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedMenu);
}

#endif // DWIN_LCD_PROUI
