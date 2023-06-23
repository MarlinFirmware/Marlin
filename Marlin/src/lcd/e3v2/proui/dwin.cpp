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

/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 3.25.3
 * Date: 2023/05/18
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "../../utf8.h"
#include "../../marlinui.h"
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

#if DISABLED(PROBE_MANUALLY) && ANY(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT)
  #define HAS_ONESTEP_LEVELING 1
#endif

#if HAS_MESH || (HAS_LEVELING && HAS_ZOFFSET_ITEM)
  #include "../../../feature/bedlevel/bedlevel.h"
  #include "bedlevel_tools.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../../feature/bltouch.h"
#endif

#if ENABLED(BABYSTEPPING)
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

#include "dwin.h"
#include "menus.h"
#include "dwin_popup.h"

#if HAS_GCODE_PREVIEW
  #include "gcode_preview.h"
#endif

#if HAS_ESDIAG
  #include "endstop_diag.h"
#endif

#if SHOW_TUNING_GRAPH
  #include "plot.h"
#endif

#if HAS_MESH
  #include "meshviewer.h"
#endif

#if HAS_LOCKSCREEN
  #include "lockscreen.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_DWIN)
#include "../../../core/debug_out.h"

#ifndef MACHINE_SIZE
  #define MACHINE_SIZE STRINGIFY(X_BED_SIZE) "x" STRINGIFY(Y_BED_SIZE) "x" STRINGIFY(Z_MAX_POS)
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

// Juntion deviation limits
#define MIN_JD_MM             0.001
#define MAX_JD_MM             TERN(LIN_ADVANCE, 0.3f, 0.5f)

#if HAS_TRINAMIC_CONFIG
  #define MIN_TMC_CURRENT 100
  #define MAX_TMC_CURRENT 3000
#endif

// Editable temperature limits
#define MIN_ETEMP  0
#define MAX_ETEMP  (thermalManager.hotend_maxtemp[0] - (HOTEND_OVERSHOOT))
#define MIN_BEDTEMP 0
#define MAX_BEDTEMP BED_MAX_TARGET

#define DWIN_VAR_UPDATE_INTERVAL         1024
#define DWIN_UPDATE_INTERVAL             1024

#if HAS_MESH && HAS_BED_PROBE
  #define BABY_Z_VAR probe.offset.z
#else
  float z_offset = 0;
  #define BABY_Z_VAR z_offset
#endif

// Structs
HMI_value_t HMI_value;
HMI_flag_t hmiFlag{0};
HMI_data_t HMI_data;

enum SelectItem : uint8_t {
  PAGE_PRINT = 0,
  PAGE_PREPARE,
  PAGE_CONTROL,
  PAGE_ADVANCE,
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

#if ENABLED(LCD_BED_TRAMMING)
  constexpr float bed_tramming_inset_lfbr[] = BED_TRAMMING_INSET_LFRB;
#endif

bool hash_changed = true; // Flag to know if message status was changed
bool blink = false;
uint8_t checkkey = 255, last_checkkey = MainMenu;

// New menu system pointers
MenuClass *FileMenu = nullptr;
MenuClass *PrepareMenu = nullptr;
#if ENABLED(LCD_BED_TRAMMING)
  MenuClass *TrammingMenu = nullptr;
#endif
MenuClass *MoveMenu = nullptr;
MenuClass *ControlMenu = nullptr;
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
#if HAS_PREHEAT
  MenuClass *PreheatMenu = nullptr;
  MenuClass *PreheatHotendMenu = nullptr;
#endif
MenuClass *TemperatureMenu = nullptr;
MenuClass *MaxSpeedMenu = nullptr;
MenuClass *MaxAccelMenu = nullptr;
#if HAS_CLASSIC_JERK
  MenuClass *MaxJerkMenu = nullptr;
#endif
MenuClass *StepsMenu = nullptr;
#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  MenuClass *HotendMPCMenu = nullptr;
#endif
#if ENABLED(PIDTEMP) && ANY(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  MenuClass *HotendPIDMenu = nullptr;
#endif
#if ENABLED(PIDTEMPBED) && ANY(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  MenuClass *BedPIDMenu = nullptr;
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  MenuClass *CaseLightMenu = nullptr;
#endif
#if ENABLED(LED_CONTROL_MENU)
  MenuClass *LedControlMenu = nullptr;
#endif
#if HAS_BED_PROBE
  MenuClass *ZOffsetWizMenu = nullptr;
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  MenuClass *HomingMenu = nullptr;
#endif
#if ENABLED(FWRETRACT)
  MenuClass *FWRetractMenu = nullptr;
#endif
#if HAS_MESH
  MenuClass *MeshMenu = nullptr;
  #if ENABLED(MESH_EDIT_MENU)
    MenuClass *EditMeshMenu = nullptr;
  #endif
#endif
#if ENABLED(SHAPING_MENU)
  MenuClass *InputShapingMenu = nullptr;
#endif
#if HAS_TRINAMIC_CONFIG
  MenuClass *TrinamicConfigMenu = nullptr;
#endif

// Updatable menuitems pointers
MenuItemClass *HotendTargetItem = nullptr;
MenuItemClass *BedTargetItem = nullptr;
MenuItemClass *FanSpeedItem = nullptr;
MenuItemClass *MMeshMoveZItem = nullptr;
MenuItemClass *EditZValueItem = nullptr;

bool Printing() { return printingIsActive() || printingIsPaused(); }
bool SD_Printing() { return Printing() && IS_SD_FILE_OPEN(); }
bool Host_Printing() { return Printing() && !IS_SD_FILE_OPEN(); }

#define DWIN_LANGUAGE_EEPROM_ADDRESS 0x01   // Between 0x01 and 0x63 (EEPROM_OFFSET-1)
                                            // BL24CXX::check() uses 0x00

inline bool HMI_IsChinese() { return hmiFlag.language == DWIN_CHINESE; }

void HMI_SetLanguageCache() {
  dwinJPGCacheTo1(HMI_IsChinese() ? Language_Chinese : Language_English);
}

void HMI_SetLanguage() {
  #if ALL(EEPROM_SETTINGS, IIC_BL24CXX_EEPROM)
    BL24CXX::read(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&hmiFlag.language, sizeof(hmiFlag.language));
  #endif
  HMI_SetLanguageCache();
}

void HMI_ToggleLanguage() {
  hmiFlag.language = HMI_IsChinese() ? DWIN_ENGLISH : DWIN_CHINESE;
  HMI_SetLanguageCache();
  #if ALL(EEPROM_SETTINGS, IIC_BL24CXX_EEPROM)
    BL24CXX::write(DWIN_LANGUAGE_EEPROM_ADDRESS, (uint8_t*)&hmiFlag.language, sizeof(hmiFlag.language));
  #endif
}

//-----------------------------------------------------------------------------
// Main Buttons
//-----------------------------------------------------------------------------

typedef struct { uint16_t x, y[2], w, h; } text_info_t;

void ICON_Button(const bool selected, const int iconid, const frame_rect_t &ico, const text_info_t (&txt), FSTR_P caption) {
  DWINUI::Draw_IconWB(iconid + selected, ico.x, ico.y);
  if (selected) DWINUI::Draw_Box(0, HMI_data.Highlight_Color, ico);
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, txt.x, txt.y[selected], txt.x + txt.w - 1, txt.y[selected] + txt.h - 1, ico.x + (ico.w - txt.w) / 2, (ico.y + ico.h - 25) - txt.h/2);
  }
  else {
    const uint16_t x = ico.x + (ico.w - strlen_P(FTOP(caption)) * DWINUI::fontWidth()) / 2,
                   y = (ico.y + ico.h - 20) - DWINUI::fontHeight() / 2;
    DWINUI::Draw_String(x, y, caption);
  }
}

//
// Main Menu: "Print"
//
void ICON_Print() {
  constexpr frame_rect_t ico = { 17, 110, 110, 100 };
  constexpr text_info_t txt = { 1, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_page.now == PAGE_PRINT, ICON_Print_0, ico, txt, GET_TEXT_F(MSG_BUTTON_PRINT));
}

//
// Main Menu: "Prepare"
//
void ICON_Prepare() {
  constexpr frame_rect_t ico = { 145, 110, 110, 100 };
  constexpr text_info_t txt = { 31, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_page.now == PAGE_PREPARE, ICON_Prepare_0, ico, txt, GET_TEXT_F(MSG_PREPARE));
}

//
// Main Menu: "Control"
//
void ICON_Control() {
  constexpr frame_rect_t ico = { 17, 226, 110, 100 };
  constexpr text_info_t txt = { 61, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_page.now == PAGE_CONTROL, ICON_Control_0, ico, txt, GET_TEXT_F(MSG_CONTROL));
}

//
// Main Menu: "Advanced Settings"
//
void ICON_AdvSettings() {
  constexpr frame_rect_t ico = { 145, 226, 110, 100 };
  constexpr text_info_t txt = { 91, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_page.now == PAGE_ADVANCE, ICON_Info_0, ico, txt, GET_TEXT_F(MSG_BUTTON_ADVANCED));
}

//
// Printing: "Tune"
//
void ICON_Tune() {
  constexpr frame_rect_t ico = { 8, 232, 80, 100 };
  constexpr text_info_t txt = { 121, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_print.now == PRINT_SETUP, ICON_Setup_0, ico, txt, GET_TEXT_F(MSG_TUNE));
}

//
// Printing: "Pause"
//
void ICON_Pause() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  constexpr text_info_t txt = { 181, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Pause_0, ico, txt, GET_TEXT_F(MSG_BUTTON_PAUSE));
}

//
// Printing: "Resume"
//
void ICON_Resume() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  constexpr text_info_t txt = { 1, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 15 };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Continue_0, ico, txt, GET_TEXT_F(MSG_BUTTON_RESUME));
}

//
// Printing: "Stop"
//
void ICON_Stop() {
  constexpr frame_rect_t ico = { 184, 232, 80, 100 };
  constexpr text_info_t txt = { 151, { 405, TERN(USE_STOCK_DWIN_SET, 446, 447) }, 27, 12 };
  ICON_Button(select_print.now == PRINT_STOP, ICON_Stop_0, ico, txt, GET_TEXT_F(MSG_BUTTON_STOP));
}

//
//PopUps
//
void Popup_window_PauseOrStop() {
  if (HMI_IsChinese()) {
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
         if (select_print.now == PRINT_PAUSE_RESUME) DWIN_Frame_AreaCopy(1, 237, 338, 269, 356, 98, 150);
    else if (select_print.now == PRINT_STOP) DWIN_Frame_AreaCopy(1, 221, 320, 253, 336, 98, 150);
    DWIN_Frame_AreaCopy(1, 220, 304, 264, 319, 130, 150);
    DWINUI::Draw_IconWB(ICON_Confirm_C, 26, 280);
    DWINUI::Draw_IconWB(ICON_Cancel_C, 146, 280);
    Draw_Select_Highlight(true);
    dwinUpdateLCD();
  }
  else
    DWIN_Popup_ConfirmCancel(ICON_BLTouch, select_print.now == PRINT_PAUSE_RESUME ? GET_TEXT_F(MSG_PAUSE_PRINT) : GET_TEXT_F(MSG_STOP_PRINT));
}

#if HAS_HOTEND

  void Popup_Window_ETempTooLow() {
    if (HMI_IsChinese()) {
      HMI_SaveProcessID(WaitResponse);
      DWINUI::ClearMainArea();
      Draw_Popup_Bkgd();
      DWINUI::Draw_Icon(ICON_TempTooLow, 102, 105);
      DWIN_Frame_AreaCopy(1, 103, 371, 136, 386,  69, 240);
      DWIN_Frame_AreaCopy(1, 170, 371, 270, 386, 102, 240);
      DWINUI::Draw_IconWB(ICON_Confirm_C, 86, 280);
      dwinUpdateLCD();
    }
    else
      DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT));
  }

#endif

#if HAS_HOTEND || HAS_HEATED_BED
  void dwinPopupTemperature(const bool toohigh) {
    HMI_SaveProcessID(WaitResponse);
    if (HMI_IsChinese()) {
      DWINUI::ClearMainArea();
      Draw_Popup_Bkgd();
      if (toohigh) {
        DWINUI::Draw_Icon(ICON_TempTooHigh, 102, 165);
        DWIN_Frame_AreaCopy(1, 103, 371, 237, 386, 52, 285);
        DWIN_Frame_AreaCopy(1, 151, 389, 185, 402, 187, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
      else {
        DWINUI::Draw_Icon(ICON_TempTooLow, 102, 165);
        DWIN_Frame_AreaCopy(1, 103, 371, 270, 386, 52, 285);
        DWIN_Frame_AreaCopy(1, 189, 389, 271, 402, 95, 310);
      }
    }
    else
      DWIN_Show_Popup(toohigh ? ICON_TempTooHigh : ICON_TempTooLow, F("Nozzle or Bed temperature"), toohigh ? F("is too high") : F("is too low"), BTN_Continue);
  }
#endif

//
// Draw status line
//
void DWIN_DrawStatusLine(const char *text) {
  dwinDrawRectangle(1, HMI_data.StatusBg_Color, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
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
      dwinDrawRectangle(1, HMI_data.StatusBg_Color, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
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
  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1,  0, 72,  63, 86,  41, 173);  // Printing Time
    DWIN_Frame_AreaCopy(1, 65, 72, 128, 86, 176, 173);  // Remain
  }
  else {
    DWINUI::Draw_String( 46, 173, GET_TEXT_F(MSG_INFO_PRINT_TIME));
    DWINUI::Draw_String(181, 173, GET_TEXT_F(MSG_REMAINING_TIME));
  }
}

void Draw_Print_ProgressBar() {
  const uint8_t _percent_done = ui.get_progress_percent();
  DWINUI::Draw_IconWB(ICON_Bar, 15, 93);
  dwinDrawRectangle(1, HMI_data.Barfill_Color, 16 + _percent_done * 240 / 100, 93, 256, 113);
  DWINUI::Draw_Int(HMI_data.PercentTxt_Color, HMI_data.Background_Color, 3, 117, 133, _percent_done);
  DWINUI::Draw_String(HMI_data.PercentTxt_Color, 142, 133, F("%"));
}

void Draw_Print_ProgressElapsed() {
  char buf[10];
  duration_t elapsed = print_job_timer.duration(); // Print timer
  sprintf_P(buf, PSTR("%02i:%02i "), (uint16_t)(elapsed.value / 3600), ((uint16_t)elapsed.value % 3600) / 60);
  DWINUI::Draw_String(HMI_data.Text_Color, HMI_data.Background_Color, 47, 192, buf);
}

#if ENABLED(SHOW_REMAINING_TIME)
  void Draw_Print_ProgressRemain() {
    const uint32_t _remain_time = ui.get_remaining_time();
    char buf[10];
    sprintf_P(buf, PSTR("%02i:%02i "), (uint16_t)(_remain_time / 3600), ((uint16_t)_remain_time % 3600) / 60);
    DWINUI::Draw_String(HMI_data.Text_Color, HMI_data.Background_Color, 181, 192, buf);
  }
#endif

void ICON_ResumeOrPause() {
  if (checkkey == PrintProcess) (print_job_timer.isPaused() || hmiFlag.pause_flag) ? ICON_Resume() : ICON_Pause();
}

// Update filename on print
void DWIN_Print_Header(const char *text = nullptr) {
  static char headertxt[31] = "";  // Print header text
  if (text) {
    const int8_t size = _MIN(30U, strlen_P(text));
    for (uint8_t i = 0; i < size; ++i) headertxt[i] = text[i];
    headertxt[size] = '\0';
  }
  if (checkkey == PrintProcess || checkkey == PrintDone) {
    dwinDrawRectangle(1, HMI_data.Background_Color, 0, 60, DWIN_WIDTH, 60+16);
    DWINUI::Draw_CenteredString(60, headertxt);
  }
}

void Draw_PrintProcess() {
  if (HMI_IsChinese())
    Title.FrameCopy(30, 1, 42, 14);                     // "Printing"
  else
    Title.ShowCaption(GET_TEXT_F(MSG_PRINTING));
  DWINUI::ClearMainArea();
  DWIN_Print_Header(nullptr);
  Draw_Print_Labels();
  DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
  DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  TERN_(SHOW_REMAINING_TIME, Draw_Print_ProgressRemain());
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
  dwinUpdateLCD();
}

void Draw_PrintDone() {
  TERN_(SET_PROGRESS_PERCENT, ui.set_progress_done());
  TERN_(SET_REMAINING_TIME, ui.reset_remaining_time());
  Title.ShowCaption(GET_TEXT_F(MSG_PRINT_DONE));
  DWINUI::ClearMainArea();
  DWIN_Print_Header(nullptr);
  #if HAS_GCODE_PREVIEW
    const bool haspreview = Preview_Valid();
    if (haspreview) {
      Preview_Show();
      DWINUI::Draw_Button(BTN_Continue, 86, 295);
    }
  #else
    constexpr bool haspreview = false;
  #endif

  if (!haspreview) {
    Draw_Print_ProgressBar();
    Draw_Print_Labels();
    DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
    DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
    Draw_Print_ProgressElapsed();
    TERN_(SHOW_REMAINING_TIME, Draw_Print_ProgressRemain());
    DWINUI::Draw_Button(BTN_Continue, 86, 273);
  }
}

void Goto_PrintDone() {
  wait_for_user = true;
  if (checkkey != PrintDone) {
    checkkey = PrintDone;
    Draw_PrintDone();
    dwinUpdateLCD();
  }
}

void Draw_Main_Menu() {
  DWINUI::ClearMainArea();
  if (HMI_IsChinese())
    Title.FrameCopy(2, 2, 26, 13);   // "Home" etc
  else
    Title.ShowCaption(MACHINE_NAME);
  DWINUI::Draw_Icon(ICON_LOGO, 71, 52);  // CREALITY logo
  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  ICON_AdvSettings();
}

void Goto_Main_Menu() {
  if (checkkey == MainMenu) return;
  checkkey = MainMenu;
  Draw_Main_Menu();
  dwinUpdateLCD();
}

// Draw X, Y, Z and blink if in an un-homed or un-trusted state
void _update_axis_value(const AxisEnum axis, const uint16_t x, const uint16_t y, const bool force) {
  const bool draw_qmark = axis_should_home(axis),
             draw_empty = NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING) && !draw_qmark && !axis_is_trusted(axis);

  // Check for a position change
  static xyz_pos_t oldpos = { -1, -1, -1 };

  const float p = (
    #if ALL(IS_FULL_CARTESIAN, SHOW_REAL_POS)
      planner.get_axis_position_mm(axis)
    #else
      current_position[axis]
    #endif
  );

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

void _draw_iconblink(bool &flag, const bool sensor, const uint8_t icon1, const uint8_t icon2, const uint16_t x, const uint16_t y) {
  #if DISABLED(NO_BLINK_IND)
    if (flag != sensor) {
      flag = sensor;
      if (!flag) {
        dwinDrawBox(1, HMI_data.Background_Color, x, y, 20, 20);
        DWINUI::Draw_Icon(icon1, x, y);
      }
    }
    if (flag) {
      dwinDrawBox(1, blink ? HMI_data.SplitLine_Color : HMI_data.Background_Color, x, y, 20, 20);
      DWINUI::Draw_Icon(icon2, x, y);
    }
  #else
    if (flag != sensor) {
      flag = sensor;
      dwinDrawBox(1, HMI_data.Background_Color, x, y, 20, 20);
      DWINUI::Draw_Icon(flag ? icon2 : icon1, x, y);
    }
  #endif
}

void _draw_ZOffsetIcon() {
  #if HAS_LEVELING
    static bool _leveling_active = false;
    _draw_iconblink(_leveling_active, planner.leveling_active, ICON_Zoffset, ICON_SetZOffset, 186, 416);
  #else
    DWINUI::Draw_Icon(ICON_Zoffset, 187, 416);
  #endif
}

void _draw_feedrate() {
  #if ENABLED(SHOW_SPEED_IND)
    int16_t _value;
    if (blink) {
      _value = feedrate_percentage;
      DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 4 * STAT_CHR_W + 2, 384, F(" %"));
    }
    else {
      _value = CEIL(feedrate_mm_s * feedrate_percentage / 100);
      dwinDrawBox(1, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, 20, 20);
    }
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, _value);
  #else
    static int16_t _feedrate = 100;
    if (_feedrate != feedrate_percentage) {
      _feedrate = feedrate_percentage;
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
    }
  #endif
}

void _draw_xyz_position(const bool force) {
  _update_axis_value(X_AXIS,  27, 459, force);
  _update_axis_value(Y_AXIS, 112, 459, force);
  _update_axis_value(Z_AXIS, 197, 459, force);
}

void update_variable() {
  _draw_xyz_position(false);
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
    TERN_(HAS_HOTEND, if (_new_hotend_target) HotendTargetItem->redraw());
    TERN_(HAS_HEATED_BED, if (_new_bed_target) BedTargetItem->redraw());
    TERN_(HAS_FAN, if (_new_fanspeed) FanSpeedItem->redraw());
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

  _draw_feedrate();

  #if HAS_FAN
    if (_new_fanspeed)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
    DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 204, 417, _offset);
  }

  _draw_ZOffsetIcon();
}

/**
 * Memory card and file management
 */

bool DWIN_lcd_sd_status = false;

#if ENABLED(MEDIASORT_MENU_ITEM)
  void SetMediaSort() {
    Toggle_Chkb_Line(HMI_data.MediaSort);
    card.setSortOn(HMI_data.MediaSort);
  }
#endif

void SetMediaAutoMount() { Toggle_Chkb_Line(HMI_data.MediaAutoMount); }

inline uint16_t nr_sd_menu_items() {
  return _MIN(card.get_num_items() + !card.flag.workDirIsRoot, MENU_MAX_ITEMS);
}

void make_name_without_ext(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
  size_t pos = strlen(src);  // Index of ending nul

  // For files, remove the extension
  // which may be .gcode, .gco, or .g
  if (!card.flag.filenameIsDir)
    while (pos && src[pos] != '.') pos--; // Find last '.' (stop at 0)

  if (!pos) pos = strlen(src);  // pos = 0 ('.' not found) restore pos

  size_t len = pos;   // nul or '.'
  if (len > maxlen) { // Keep the name short
    pos        = len = maxlen; // Move nul down
    dst[--pos] = '.'; // Insert dots
    dst[--pos] = '.';
    dst[--pos] = '.';
  }

  dst[len] = '\0';    // End it

  // Copy down to 0
  while (pos--) dst[pos] = src[pos];
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
    card.selectFileByIndexSorted(filenum);

    // Enter that folder!
    if (card.flag.filenameIsDir) return SDCard_Folder(card.filename);

    if (card.fileIsBinary())
      return DWIN_Popup_Confirm(ICON_Error, F("Please check filenames"), F("Only G-code can be printed"));
    else {
      DWIN_Print_Header(card.longest_filename()); // Save filename
      return Goto_ConfirmToPrint();
    }
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
    static int8_t shift_amt = 0, shift_len = 0;
    if (reset) {
      last_itemselected = 0;
      hasUpDir = !card.flag.workDirIsRoot; // Is a SubDir
      return;
    }
    const uint8_t selected = FileMenu->selected;
    if (last_itemselected != selected) {
      if (last_itemselected >= 1 + hasUpDir) FileMenu->Items()[last_itemselected]->redraw(true);
      last_itemselected = selected;
      if (selected >= 1 + hasUpDir) {
        const int8_t filenum = selected - 1 - hasUpDir; // Skip "Back" and ".."
        card.selectFileByIndexSorted(filenum);
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
    card.selectFileByIndexSorted(menuitem->pos - is_subdir - 1);
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
      for (uint8_t i = 0; i < nr_sd_menu_items(); ++i) {
        MenuItemAdd(onDrawFileName, onClickSDItem);
      }
    }
    UpdateMenu(FileMenu);
    TERN_(DASH_REDRAW, DWIN_RedrawDash());
  }
  else {
    if (SET_MENU(FileMenu, MSG_MEDIA_MENU, 1)) BACK_ITEM(Goto_Main_Menu);
    UpdateMenu(FileMenu);
    dwinDrawRectangle(1, HMI_data.AlertBg_Color, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::Draw_CenteredString(font12x24, HMI_data.AlertTxt_Color, MBASE(3), GET_TEXT_F(MSG_MEDIA_NOT_INSERTED));
  }
  TERN_(SCROLL_LONG_FILENAMES, FileMenuIdle(true));
}

//
// Watch for media mount / unmount
//
void HMI_SDCardUpdate() {
  if (hmiFlag.home_flag) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    ResetMenu(FileMenu);
    if (IsMenu(FileMenu)) {
      CurrentMenu = nullptr;
      Draw_Print_File_Menu();
    }
    if (!DWIN_lcd_sd_status && SD_Printing()) ui.abort_print();  // Media removed while printing
  }
}

/**
 * Dash board and indicators
 */

void DWIN_Draw_Dashboard() {

  dwinDrawRectangle(1, HMI_data.Background_Color, 0, STATUS_Y + 21, DWIN_WIDTH, DWIN_HEIGHT - 1);
  dwinDrawRectangle(1, HMI_data.SplitLine_Color, 0, 449, DWIN_WIDTH, 451);

  DWINUI::Draw_Icon(ICON_MaxSpeedX,  10, 456);
  DWINUI::Draw_Icon(ICON_MaxSpeedY,  95, 456);
  DWINUI::Draw_Icon(ICON_MaxSpeedZ, 180, 456);
  _draw_xyz_position(true);

  #if HAS_HOTEND
    DWINUI::Draw_Icon(ICON_HotendTemp, 10, 383);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, thermalManager.wholeDegHotend(0));
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(0));

    DWINUI::Draw_Icon(ICON_StepE, 112, 417);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWINUI::Draw_Icon(ICON_BedTemp, 10, 416);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, thermalManager.wholeDegBed());
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWINUI::Draw_Icon(ICON_Speed, 113, 383);
  DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  IF_DISABLED(SHOW_SPEED_IND, DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, F("%")));

  #if HAS_FAN
    DWINUI::Draw_Icon(ICON_FanSpeed, 187, 383);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWINUI::Draw_Icon(planner.leveling_active ? ICON_SetZOffset : ICON_Zoffset, 187, 416);
    DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 204, 417, BABY_Z_VAR);
  #endif
}

void Draw_Info_Menu() {
  DWINUI::ClearMainArea();
  if (HMI_IsChinese())
    Title.FrameCopy(30, 17, 28, 13);                        // "Info"
  else
    Title.ShowCaption(GET_TEXT_F(MSG_INFO_SCREEN));
  Draw_Menu_Line(0, ICON_Back, GET_TEXT_F(MSG_BACK), false, true);

  if (HMI_IsChinese()) {
    DWIN_Frame_AreaCopy(1, 197, 149, 252, 161, 108, 102);   // "Size"
    DWIN_Frame_AreaCopy(1,   1, 164,  56, 176, 108, 175);   // "Firmware Version"
    DWIN_Frame_AreaCopy(1,  58, 164, 113, 176, 105, 248);   // "Contact Details"
    DWINUI::Draw_CenteredString(268, F(CORP_WEBSITE));
  }
  else {
    DWINUI::Draw_CenteredString(102, F("Size"));
    DWINUI::Draw_CenteredString(175, F("Firmware version"));
    DWINUI::Draw_CenteredString(248, F("Build Datetime"));
    DWINUI::Draw_CenteredString(268, F(STRING_DISTRIBUTION_DATE));
  }
  DWINUI::Draw_CenteredString(122, F(MACHINE_SIZE));
  DWINUI::Draw_CenteredString(195, F(SHORT_BUILD_VERSION));

  for (uint8_t i = 0; i < 3; ++i) {
    DWINUI::Draw_Icon(ICON_PrintSize + i, ICOX, 99 + i * 73);
    dwinDrawHLine(HMI_data.SplitLine_Color, 16, MBASE(2) + i * 73, 240);
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
  dwinUpdateLCD();
}

// Pause or Stop popup
void onClick_PauseOrStop() {
  switch (select_print.now) {
    case PRINT_PAUSE_RESUME: if (hmiFlag.select_flag) ui.pause_print(); break; // Confirm pause
    case PRINT_STOP: if (hmiFlag.select_flag) ui.abort_print(); break; // Stop confirmed then abort print
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
        if (printingIsPaused()) {  // If printer is already in pause
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
  dwinUpdateLCD();
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
    case Popup:                  popupDraw(); break;
    #if HAS_LOCKSCREEN
      case Locked:               lockScreen.draw(); break;
    #endif
    case Menu:
    case SetInt:
    case SetPInt:
    case SetIntNoDraw:
    case SetFloat:
    case SetPFloat:              ReDrawMenu(true); break;
    default: break;
  }
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
      #endif
      default:
        HMI_ReturnScreen();
        break;
    }
  }
}

void HMI_Init() {
  #if ENABLED(SHOW_BOOTSCREEN)
    #ifndef BOOTSCREEN_TIMEOUT
      #define BOOTSCREEN_TIMEOUT 1100
    #endif
    DWINUI::Draw_Box(1, Color_Black, { 5, 220, DWIN_WIDTH - 5, DWINUI::fontHeight() });
    DWINUI::Draw_CenteredString(Color_White, 220, F("ProUI starting up "));
    for (uint16_t t = 15; t < 257; t += 11) {
      DWINUI::Draw_Icon(ICON_Bar, 15, 260);
      dwinDrawRectangle(1, HMI_data.Background_Color, t, 260, 257, 280);
      dwinUpdateLCD();
      safe_delay((BOOTSCREEN_TIMEOUT) / 22);
    }
  #endif
  HMI_SetLanguage();
}

void EachMomentUpdate() {
  static millis_t next_var_update_ms = 0, next_rts_update_ms = 0, next_status_update_ms = 0;
  const millis_t ms = millis();

  if (ELAPSED(ms, next_var_update_ms)) {
    next_var_update_ms = ms + DWIN_VAR_UPDATE_INTERVAL;
    blink = !blink;
    update_variable();
    #if HAS_ESDIAG
      if (checkkey == ESDiagProcess) ESDiag.Update();
    #endif
    #if SHOW_TUNING_GRAPH
      if (checkkey == PidProcess) {
        TERN_(PIDTEMP, if (HMI_value.tempcontrol == PIDTEMP_START) plot.Update(thermalManager.wholeDegHotend(0)));
        TERN_(PIDTEMPBED, if (HMI_value.tempcontrol == PIDTEMPBED_START) plot.Update(thermalManager.wholeDegBed()));
      }
      TERN_(MPCTEMP, if (checkkey == MPCProcess) plot.Update(thermalManager.wholeDegHotend(0)));
    #endif
  }

  #if HAS_STATUS_MESSAGE_TIMEOUT
    bool did_expire = ui.status_reset_callback && (*ui.status_reset_callback)();
    did_expire |= ui.status_message_expire_ms && ELAPSED(ms, ui.status_message_expire_ms);
    if (did_expire) ui.reset_status();
  #endif

  if (ELAPSED(ms, next_status_update_ms)) {
    next_status_update_ms = ms + 500;
    DWIN_DrawStatusMessage();
    #if ENABLED(SCROLL_LONG_FILENAMES)
      if (IsMenu(FileMenu)) FileMenuIdle();
    #endif
  }

  if (!PENDING(ms, next_rts_update_ms)) {
    next_rts_update_ms = ms + DWIN_UPDATE_INTERVAL;

    if ((Printing() != hmiFlag.printing_flag) && !hmiFlag.home_flag) {
      hmiFlag.printing_flag = Printing();
      if (hmiFlag.printing_flag)
        DWIN_Print_Started();
      else if (hmiFlag.abort_flag)
        DWIN_Print_Aborted();
      else
        DWIN_Print_Finished();
    }

    if ((printingIsPaused() != hmiFlag.pause_flag) && !hmiFlag.home_flag) {
      hmiFlag.pause_flag = printingIsPaused();
      if (hmiFlag.pause_flag)
        DWIN_Print_Pause();
      else if (hmiFlag.abort_flag)
        DWIN_Print_Aborted();
      else
        DWIN_Print_Resume();
    }

    if (checkkey == PrintProcess) { // Print process

      // Progress percent
      static uint8_t _percent_done = 255;
      if (_percent_done != ui.get_progress_percent()) {
        _percent_done = ui.get_progress_percent();
        Draw_Print_ProgressBar();
      }

      // Remaining time
      #if ENABLED(SHOW_REMAINING_TIME)
        static uint32_t _remain_time = 0;
        if (_remain_time != ui.get_remaining_time()) {
          _remain_time = ui.get_remaining_time();
          Draw_Print_ProgressRemain();
        }
      #endif

      // Elapsed print time
      static uint16_t _printtime = 0;
      const uint16_t min = (print_job_timer.duration() % 3600) / 60;
      if (_printtime != min) { // 1 minute update
        _printtime = min;
        Draw_Print_ProgressElapsed();
      }
    }
    #if ENABLED(POWER_LOSS_RECOVERY)
      else if (DWIN_lcd_sd_status && recovery.dwin_flag) { // Resume print before power off
        return Goto_PowerLossRecovery();
      }
    #endif

    dwinUpdateLCD();
  }
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void Popup_PowerLossRecovery() {
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    if (HMI_IsChinese()) {
      DWIN_Frame_AreaCopy(1, 160, 338, 235, 354, 98, 115);
      DWIN_Frame_AreaCopy(1, 103, 321, 271, 335, 52, 167);
      DWINUI::Draw_IconWB(ICON_Cancel_C,    26, 280);
      DWINUI::Draw_IconWB(ICON_Continue_C, 146, 280);
    }
    else {
      DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 70, GET_TEXT_F(MSG_OUTAGE_RECOVERY));
      DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 147, F("It looks like the last"));
      DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 167, F("file was interrupted."));
      DWINUI::Draw_Button(BTN_Cancel,    26, 280);
      DWINUI::Draw_Button(BTN_Continue, 146, 280);
    }
    MediaFile *dir = nullptr;
    const char * const filename = card.diveToFile(true, dir, recovery.info.sd_filename);
    card.selectFileByName(filename);
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 207, card.longest_filename());
    DWIN_Print_Header(card.longest_filename()); // Save filename
    Draw_Select_Highlight(hmiFlag.select_flag);
    dwinUpdateLCD();
  }

  void onClick_PowerLossRecovery() {
    if (hmiFlag.select_flag) {
      queue.inject(F("M1000C"));
      select_page.reset();
      return Goto_Main_Menu();
    }
    else {
      HMI_SaveProcessID(NothingToDo);
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
    case Leveling:        break;
    #if HAS_LOCKSCREEN
      case Locked:        HMI_LockScreen(); break;
    #endif
    case PrintDone:
    TERN_(HAS_ESDIAG, case ESDiagProcess:)
    case WaitResponse:    HMI_WaitForUser(); break;
    case Homing:
    case PidProcess:
    case NothingToDo:     break;
    default: break;
  }
}

bool IDisPopUp() {    // If ID is popup...
  switch (checkkey) {
    case NothingToDo:
    case WaitResponse:
    case Popup:
    case Homing:
    case Leveling:
    case PidProcess:
    TERN_(HAS_ESDIAG, case ESDiagProcess:)
      return true;
    default: break;
  }
  return false;
}

void HMI_SaveProcessID(const uint8_t id) {
  if (checkkey == id) return;
  if (!IDisPopUp()) last_checkkey = checkkey; // If previous is not a popup
  checkkey = id;
  switch (id) {
    case Popup:
    case WaitResponse:
    case PrintDone:
    case Leveling:
    TERN_(HAS_ESDIAG, case ESDiagProcess:)
      wait_for_user = true;
    default: break;
  }
}

void HMI_ReturnScreen() {
  checkkey = last_checkkey;
  wait_for_user = false;
  Draw_Main_Area();
}

void dwinHomingStart() {
  hmiFlag.home_flag = true;
  HMI_SaveProcessID(Homing);
  Title.ShowCaption(GET_TEXT_F(MSG_HOMING));
  DWIN_Show_Popup(ICON_BLTouch, GET_TEXT_F(MSG_HOMING), GET_TEXT_F(MSG_PLEASE_WAIT));
}

void dwinHomingDone() {
  hmiFlag.home_flag = false;
  if (last_checkkey == PrintDone)
    Goto_PrintDone();
  else
    HMI_ReturnScreen();
}

void DWIN_LevelingStart() {
  #if HAS_BED_PROBE
    HMI_SaveProcessID(Leveling);
    Title.ShowCaption(GET_TEXT_F(MSG_BED_LEVELING));
    DWIN_Show_Popup(ICON_AutoLeveling, GET_TEXT_F(MSG_BED_LEVELING), GET_TEXT_F(MSG_PLEASE_WAIT));
    #if ALL(AUTO_BED_LEVELING_UBL, PREHEAT_BEFORE_LEVELING)
      #if HAS_BED_PROBE
        if (!DEBUGGING(DRYRUN)) probe.preheat_for_probing(LEVELING_NOZZLE_TEMP, HMI_data.BedLevT);
      #else
        #if HAS_HOTEND
          if (!DEBUGGING(DRYRUN) && thermalManager.degTargetHotend(0) < LEVELING_NOZZLE_TEMP) {
            thermalManager.setTargetHotend(LEVELING_NOZZLE_TEMP, 0);
            thermalManager.wait_for_hotend(0);
          }
        #endif
        #if HAS_HEATED_BED
          if (!DEBUGGING(DRYRUN) && thermalManager.degTargetBed() < HMI_data.BedLevT) {
            thermalManager.setTargetBed(HMI_data.BedLevT);
            thermalManager.wait_for_bed_heating();
          }
        #endif
      #endif
    #endif
  #elif ENABLED(MESH_BED_LEVELING)
    Draw_ManualMesh_Menu();
  #endif
}

void DWIN_LevelingDone() {
  TERN_(HAS_MESH, Goto_MeshViewer(true));
}

#if HAS_MESH
  void DWIN_MeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval) {
    char str_1[6] = "";
    ui.status_printf(0, F(S_FMT " %i/%i Z=%s"), GET_TEXT_F(MSG_PROBING_POINT), cpos, tpos, dtostrf(zval, 1, 2, str_1));
  }
#endif

// PID/MPC process

#if SHOW_TUNING_GRAPH

  #include "plot.h"

  celsius_t _maxtemp, _target;
  void DWIN_Draw_PID_MPC_Popup() {
    constexpr frame_rect_t gfrm = { 40, 180, DWIN_WIDTH - 80, 120 };
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();

    switch (HMI_value.tempcontrol) {
      default: return;
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 100, GET_TEXT_F(MSG_MPC_AUTOTUNE));
          DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("MPC target:    Celsius"));
          break;
      #endif
      #if ANY(PIDTEMP, PIDTEMPBED)
        TERN_(PIDTEMP,    case PIDTEMP_START:)
        TERN_(PIDTEMPBED, case PIDTEMPBED_START:)
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 100, GET_TEXT_F(MSG_PID_AUTOTUNE));
          DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("PID target:    Celsius"));
          break;
      #endif
    }

    switch (HMI_value.tempcontrol) {
      default: break;
      #if ANY(PIDTEMP, MPC_AUTOTUNE)
        TERN_(PIDTEMP,      case PIDTEMP_START:)
        TERN_(MPC_AUTOTUNE, case MPCTEMP_START:)
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 120, F("for Nozzle is running."));
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 120, F("for BED is running."));
          break;
      #endif
    }

    switch (HMI_value.tempcontrol) {
      default: break;
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[0];
          _target = 200;
          break;
      #endif
      #if ENABLED(PIDTEMP)
        case PIDTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[0];
          _target = HMI_data.HotendPidT;
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
          _maxtemp = BED_MAXTEMP;
          _target = HMI_data.BedPidT;
          break;
      #endif
    }

    plot.Draw(gfrm, _maxtemp, _target);
    DWINUI::Draw_Int(HMI_data.PopupTxt_Color, 3, gfrm.x + 90, gfrm.y - DWINUI::fontHeight() - 4, _target);
  }

#endif // SHOW_TUNING_GRAPH

#if DWIN_PID_TUNE

  void DWIN_StartM303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp) {
    if (seenC) HMI_data.PidCycles = c;
    if (seenS) {
      switch (hid) {
        OPTCODE(PIDTEMP,    case 0 ... HOTENDS - 1: HMI_data.HotendPidT = temp; break)
        OPTCODE(PIDTEMPBED, case H_BED:             HMI_data.BedPidT = temp;    break)
        default: break;
      }
    }
  }

  void DWIN_PidTuning(tempcontrol_t result) {
    HMI_value.tempcontrol = result;
    switch (result) {
      #if ENABLED(PIDTEMP)
        case PIDTEMP_START:
          HMI_SaveProcessID(PidProcess);
          #if SHOW_TUNING_GRAPH
            DWIN_Draw_PID_MPC_Popup();
          #else
            DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for Nozzle is running."));
          #endif
          break;
        case PID_TEMP_TOO_HIGH:
          checkkey = last_checkkey;
          DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_TEMP_TOO_HIGH));
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
          HMI_SaveProcessID(PidProcess);
          #if SHOW_TUNING_GRAPH
            DWIN_Draw_PID_MPC_Popup();
          #else
            DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for BED is running."));
          #endif
          break;
      #endif
      case PID_BAD_HEATER_ID:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_PID_BAD_HEATER_ID));
        break;
      case PID_TUNING_TIMEOUT:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), GET_TEXT_F(MSG_PID_TIMEOUT));
        break;
      case AUTOTUNE_DONE:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
        break;
      default:
        checkkey = last_checkkey;
        break;
    }
  }

#endif // DWIN_PID_TUNE

#if ENABLED(MPC_AUTOTUNE)

  void DWIN_MPCTuning(tempcontrol_t result) {
    HMI_value.tempcontrol = result;
    switch (result) {
      case MPCTEMP_START:
        HMI_SaveProcessID(MPCProcess);
        #if SHOW_TUNING_GRAPH
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
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), F(STR_MPC_AUTOTUNE_INTERRUPTED));
        ui.reset_alert_level();
        break;
      case AUTOTUNE_DONE:
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

#endif // MPC_AUTOTUNE

// Started a Print Job
void DWIN_Print_Started() {
  TERN_(HAS_GCODE_PREVIEW, if (Host_Printing()) Preview_Invalidate());
  TERN_(SET_PROGRESS_PERCENT, ui.progress_reset());
  TERN_(SET_REMAINING_TIME, ui.reset_remaining_time());
  hmiFlag.pause_flag = false;
  hmiFlag.abort_flag = false;
  select_print.reset();
  Goto_PrintProcess();
}

// Pause a print job
void DWIN_Print_Pause() {
  ICON_ResumeOrPause();
}

// Resume print job
void DWIN_Print_Resume() {
  ICON_ResumeOrPause();
  LCD_MESSAGE(MSG_RESUME_PRINT);
}

// Ended print job
void DWIN_Print_Finished() {
  TERN_(POWER_LOSS_RECOVERY, if (card.isPrinting()) recovery.cancel());
  hmiFlag.abort_flag = false;
  hmiFlag.pause_flag = false;
  wait_for_heatup = false;
  planner.finish_and_disable();
  thermalManager.cooldown();
  Goto_PrintDone();
}

// Print was aborted
void DWIN_Print_Aborted() {
  DWIN_Print_Finished();
}

#if HAS_FILAMENT_SENSOR
  // Filament Runout process
  void DWIN_FilamentRunout(const uint8_t extruder) { LCD_MESSAGE(MSG_RUNOUT_SENSOR); }
#endif

void DWIN_SetColorDefaults() {
  HMI_data.Background_Color = Def_Background_Color;
  HMI_data.Cursor_Color     = Def_Cursor_Color;
  HMI_data.TitleBg_Color    = Def_TitleBg_Color;
  HMI_data.TitleTxt_Color   = Def_TitleTxt_Color;
  HMI_data.Text_Color       = Def_Text_Color;
  HMI_data.Selected_Color   = Def_Selected_Color;
  HMI_data.SplitLine_Color  = Def_SplitLine_Color;
  HMI_data.Highlight_Color  = Def_Highlight_Color;
  HMI_data.StatusBg_Color   = Def_StatusBg_Color;
  HMI_data.StatusTxt_Color  = Def_StatusTxt_Color;
  HMI_data.PopupBg_Color    = Def_PopupBg_Color;
  HMI_data.PopupTxt_Color   = Def_PopupTxt_Color;
  HMI_data.AlertBg_Color    = Def_AlertBg_Color;
  HMI_data.AlertTxt_Color   = Def_AlertTxt_Color;
  HMI_data.PercentTxt_Color = Def_PercentTxt_Color;
  HMI_data.Barfill_Color    = Def_Barfill_Color;
  HMI_data.Indicator_Color  = Def_Indicator_Color;
  HMI_data.Coordinate_Color = Def_Coordinate_Color;
}

void DWIN_SetDataDefaults() {
  DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  TERN_(PIDTEMP, HMI_data.HotendPidT = DEF_HOTENDPIDT);
  TERN_(PIDTEMPBED, HMI_data.BedPidT = DEF_BEDPIDT);
  TERN_(DWIN_PID_TUNE, HMI_data.PidCycles = DEF_PIDCYCLES);
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    HMI_data.ExtMinT = EXTRUDE_MINTEMP;
    ApplyExtMinT();
  #endif
  TERN_(PREHEAT_BEFORE_LEVELING, HMI_data.BedLevT = LEVELING_BED_TEMP);
  TERN_(BAUD_RATE_GCODE, SetBaud250K());
  #if ALL(LCD_BED_TRAMMING, HAS_BED_PROBE)
    HMI_data.FullManualTramming = DISABLED(BED_TRAMMING_USE_PROBE);
  #endif
  #if ENABLED(MEDIASORT_MENU_ITEM)
    HMI_data.MediaSort = true;
    card.setSortOn(true);
  #endif
  HMI_data.MediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  #if ALL(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    HMI_data.z_after_homing = DEF_Z_AFTER_HOMING;
  #endif
  #if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    TERN_(LED_COLOR_PRESETS, leds.set_default());
    ApplyLEDColor();
  #endif
  TERN_(ADAPTIVE_STEP_SMOOTHING, HMI_data.AdaptiveStepSmoothing = true);
  TERN_(HAS_GCODE_PREVIEW, HMI_data.EnablePreview = true);
}

void DWIN_CopySettingsTo(char * const buff) {
  memcpy(buff, &HMI_data, eeprom_data_size);
}

void DWIN_CopySettingsFrom(const char * const buff) {
  memcpy(&HMI_data, buff, sizeof(HMI_data_t));
  if (HMI_data.Text_Color == HMI_data.Background_Color) DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  TERN_(PREVENT_COLD_EXTRUSION, ApplyExtMinT());
  feedrate_percentage = 100;
  TERN_(BAUD_RATE_GCODE, HMI_SetBaudRate());
  #if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_color(
      HMI_data.Led_Color.r,
      HMI_data.Led_Color.g,
      HMI_data.Led_Color.b
      OPTARG(HAS_WHITE_LED, HMI_data.Led_Color.w)
    );
    leds.update();
  #endif
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {
  delay(750);   // Wait to wakeup screen
  const bool hs = dwinHandshake(); UNUSED(hs);
  dwinFrameSetDir(1);
  dwinJPGCacheTo1(Language_English);
  encoderConfiguration();
}

void dwinInitScreen() {
  DWIN_SetColorDefaults();
  HMI_Init();   // Draws boot screen
  DWINUI::init();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  DWINUI::onTitleDraw = Draw_Title;
  InitMenu();
  checkkey = 255;
  hash_changed = true;
  DWIN_DrawStatusLine();
  DWIN_Draw_Dashboard();
  Goto_Main_Menu();
}

void MarlinUI::update() {
  HMI_SDCardUpdate();   // SD card update
  EachMomentUpdate();   // Status update
  DWIN_HandleScreen();  // Rotary encoder update
}

void MarlinUI::refresh() { /* Nothing to see here */ }

#if HAS_LCD_BRIGHTNESS
  void MarlinUI::_set_brightness() { dwinLCDBrightness(backlight ? brightness : 0); }
#endif

void MarlinUI::kill_screen(FSTR_P const lcd_error, FSTR_P const lcd_component) {
  DWIN_Draw_Popup(ICON_BLTouch, GET_TEXT_F(MSG_PRINTER_KILLED), lcd_error);
  DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 270, GET_TEXT_F(MSG_TURN_OFF));
  dwinUpdateLCD();
}

void DWIN_RebootScreen() {
  dwinFrameClear(Color_Bg_Black);
  dwinJPGShowAndCache(0);
  DWINUI::Draw_CenteredString(Color_White, 220, GET_TEXT_F(MSG_PLEASE_WAIT_REBOOT));
  dwinUpdateLCD();
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
  void DWIN_Popup_Pause(FSTR_P const fmsg, uint8_t button/*=0*/) {
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
    if (hmiFlag.select_flag)
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
#endif

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

#endif // HAS_LOCKSCREEN

#if HAS_GCODE_PREVIEW

  void SetPreview() { Toggle_Chkb_Line(HMI_data.EnablePreview); }

  void onClick_ConfirmToPrint() {
    DWIN_ResetStatusLine();
    if (hmiFlag.select_flag) {     // Confirm
      Goto_Main_Menu();
      return card.openAndPrintFile(card.filename);
    }
    else
      HMI_ReturnScreen();
  }

#endif // HAS_GCODE_PREVIEW

void Goto_ConfirmToPrint() {
  #if HAS_GCODE_PREVIEW
    if (HMI_data.EnablePreview) return Goto_Popup(Preview_DrawFromSD, onClick_ConfirmToPrint);
  #endif
  card.openAndPrintFile(card.filename); // Direct print SD file
}

#if HAS_ESDIAG
  void Draw_EndStopDiag() {
    HMI_SaveProcessID(ESDiagProcess);
    ESDiag.Draw();
  }
#endif

//=============================================================================
// MENU SUBSYSTEM
//=============================================================================

// Tool functions

#if ENABLED(EEPROM_SETTINGS)

  void WriteEeprom() {
    DWIN_DrawStatusLine(GET_TEXT_F(MSG_STORE_EEPROM));
    dwinUpdateLCD();
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
    void SaveMesh() { TERN(AUTO_BED_LEVELING_UBL, UBLMeshSave(), WriteEeprom()); }
  #endif

#endif // EEPROM_SETTINGS

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
  dwinUpdateLCD();
  HMI_SaveProcessID(WaitResponse);
}

void DisableMotors() { queue.inject(F("M84")); }

void AutoLev() {   // Always reacquire the Z "home" position
  queue.inject(F(TERN(AUTO_BED_LEVELING_UBL, "G29P1", "G29")));
}

void AutoHome() { queue.inject_P(G28_STR); }

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void HomeX() { queue.inject(F("G28X")); }
  void HomeY() { queue.inject(F("G28Y")); }
  void HomeZ() { queue.inject(F("G28Z")); }
  #if ALL(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    void ApplyZAfterHoming() { HMI_data.z_after_homing = MenuData.Value; };
    void SetZAfterHoming() { SetIntOnClick(0, 20, HMI_data.z_after_homing, ApplyZAfterHoming); }
  #endif
#endif

#if HAS_ZOFFSET_ITEM

  void ApplyZOffset() { TERN_(EEPROM_SETTINGS, settings.save()); }
  void LiveZOffset() {
    #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      const_float_t step_zoffset = round((MenuData.Value / 100.0f) * planner.settings.axis_steps_per_mm[Z_AXIS]) - babystep.accum;
      if (BABYSTEP_ALLOWED()) babystep.add_steps(Z_AXIS, step_zoffset);
    #endif
  }
  void SetZOffset() {
    #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      babystep.accum = round(planner.settings.axis_steps_per_mm[Z_AXIS] * BABY_Z_VAR);
    #endif
    SetPFloatOnClick(Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX, 2, ApplyZOffset, LiveZOffset);
  }

#endif // HAS_ZOFFSET_ITEM

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

#if DISABLED(HAS_BED_PROBE)
  void HomeZandDisable() {
    SetMoveZto0();
    DisableMotors();
  }
#endif

#if HAS_PREHEAT
  #define _DoPreheat(N) void DoPreheat##N() { ui.preheat_all(N-1); }\
                        void DoPreheatHotend##N() { ui.preheat_hotend(N-1); }
  REPEAT_1(PREHEAT_COUNT, _DoPreheat)
#endif

void DoCoolDown() { thermalManager.cooldown(); }

void SetLanguage() {
  HMI_ToggleLanguage();
  CurrentMenu = nullptr;  // Invalidate menu to full redraw
  Draw_Prepare_Menu();
}

bool EnableLiveMove = false;
void SetLiveMove() { Toggle_Chkb_Line(EnableLiveMove); }
void AxisMove(AxisEnum axis) {
  #if HAS_HOTEND
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(0)) {
      gcode.process_subcommands_now(F("G92E0"));  // Reset extruder position
      return DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT));
    }
  #endif
  planner.synchronize();
  if (!planner.is_full()) planner.buffer_line(current_position, manual_feedrate_mm_s[axis]);
}
void LiveMove() {
  if (!EnableLiveMove) return;
  *MenuData.P_Float = MenuData.Value / MINUNITMULT;
  AxisMove(HMI_value.axis);
}
void ApplyMove() {
  if (EnableLiveMove) return;
  AxisMove(HMI_value.axis);
}

void SetMoveX() { HMI_value.axis = X_AXIS; SetPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
void SetMoveY() { HMI_value.axis = Y_AXIS; SetPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
void SetMoveZ() { HMI_value.axis = Z_AXIS; SetPFloatOnClick(Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }

#if HAS_HOTEND
  void SetMoveE() {
    const float e_min = current_position.e - (EXTRUDE_MAXLENGTH),
                e_max = current_position.e + (EXTRUDE_MAXLENGTH);
    HMI_value.axis = E_AXIS; SetPFloatOnClick(e_min, e_max, UNITFDIGITS, ApplyMove, LiveMove);
  }
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  void SetPwrLossr() {
    Toggle_Chkb_Line(recovery.enabled);
    recovery.changed();
  }
#endif

#if ENABLED(BAUD_RATE_GCODE)
  void HMI_SetBaudRate() { HMI_data.Baud115K ? SetBaud115K() : SetBaud250K(); }
  void SetBaudRate() {
    HMI_data.Baud115K ^= true;
    HMI_SetBaudRate();
    Draw_Chkb_Line(CurrentMenu->line(), HMI_data.Baud115K);
    dwinUpdateLCD();
  }
  void SetBaud115K() { queue.inject(F("M575 P0 B115200")); HMI_data.Baud115K = true; }
  void SetBaud250K() { queue.inject(F("M575 P0 B250000")); HMI_data.Baud115K = false; }
#endif

#if HAS_LCD_BRIGHTNESS
  void ApplyBrightness() { ui.set_brightness(MenuData.Value); }
  void LiveBrightness() { dwinLCDBrightness(MenuData.Value); }
  void SetBrightness() { SetIntOnClick(LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, ui.brightness, ApplyBrightness, LiveBrightness); }
  void TurnOffBacklight() { HMI_SaveProcessID(WaitResponse); ui.set_brightness(0); DWIN_RedrawScreen(); }
#endif

#if ENABLED(CASE_LIGHT_MENU)
  void SetCaseLight() {
    Toggle_Chkb_Line(caselight.on);
    caselight.update_enabled();
  }
  #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
    void LiveCaseLightBrightness() { caselight.brightness = MenuData.Value; caselight.update_brightness(); }
    void SetCaseLightBrightness() { SetIntOnClick(0, 255, caselight.brightness, nullptr, LiveCaseLightBrightness); }
  #endif
#endif

#if ENABLED(LED_CONTROL_MENU)
  #if !ALL(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
    void SetLedStatus() {
      leds.toggle();
      Show_Chkb_Line(leds.lights_on);
    }
  #endif
  #if HAS_COLOR_LEDS
    void ApplyLEDColor() {
      HMI_data.Led_Color = LEDColor( {leds.color.r, leds.color.g, leds.color.b OPTARG(HAS_WHITE_LED, HMI_data.Led_Color.w) } );
    }
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
    Toggle_Chkb_Line(ui.sound_on);
  }
#endif

#if HAS_HOME_OFFSET
  void ApplyHomeOffset() { set_home_offset(HMI_value.axis, MenuData.Value / MINUNITMULT); }
  void SetHomeOffsetX() { HMI_value.axis = X_AXIS; SetPFloatOnClick(-50, 50, UNITFDIGITS, ApplyHomeOffset); }
  void SetHomeOffsetY() { HMI_value.axis = Y_AXIS; SetPFloatOnClick(-50, 50, UNITFDIGITS, ApplyHomeOffset); }
  void SetHomeOffsetZ() { HMI_value.axis = Z_AXIS; SetPFloatOnClick( -2,  2, UNITFDIGITS, ApplyHomeOffset); }
#endif

#if HAS_BED_PROBE
  void SetProbeOffsetX() { SetPFloatOnClick(-60, 60, UNITFDIGITS); }
  void SetProbeOffsetY() { SetPFloatOnClick(-60, 60, UNITFDIGITS); }
  void SetProbeOffsetZ() { SetPFloatOnClick(-10, 10, 2); }
  void ProbeTest() {
    LCD_MESSAGE(MSG_M48_TEST);
    queue.inject(F("G28O\nM48 P10"));
  }
  void ProbeStow() { probe.stow(); }
  void ProbeDeploy() { probe.deploy(); }

  #if HAS_BLTOUCH_HS_MODE
    void SetHSMode() { Toggle_Chkb_Line(bltouch.high_speed_mode); }
  #endif

#endif

#if HAS_FILAMENT_SENSOR
  void SetRunoutEnable() {
    runout.reset();
    Toggle_Chkb_Line(runout.enabled);
  }
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

void SetFlow() { SetPIntOnClick(MIN_PRINT_FLOW, MAX_PRINT_FLOW, []{ planner.refresh_e_factor(0); }); }

// Bed Tramming

#if ENABLED(LCD_BED_TRAMMING)

  void TramXY(const uint8_t point, float &x, float &y) {
    switch (point) {
      case 0:
        LCD_MESSAGE(MSG_TRAM_FL);
        x = bed_tramming_inset_lfbr[0];
        y = bed_tramming_inset_lfbr[1];
        break;
      case 1:
        LCD_MESSAGE(MSG_TRAM_FR);
        x = X_BED_SIZE - bed_tramming_inset_lfbr[2];
        y = bed_tramming_inset_lfbr[1];
        break;
      case 2:
        LCD_MESSAGE(MSG_TRAM_BR);
        x = X_BED_SIZE - bed_tramming_inset_lfbr[2];
        y = Y_BED_SIZE - bed_tramming_inset_lfbr[3];
        break;
      case 3:
        LCD_MESSAGE(MSG_TRAM_BL);
        x = bed_tramming_inset_lfbr[0];
        y = Y_BED_SIZE - bed_tramming_inset_lfbr[3];
        break;
      #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
        case 4:
          LCD_MESSAGE(MSG_TRAM_C);
          x = X_CENTER; y = Y_CENTER;
          break;
      #endif
    }
  }

  #if HAS_BED_PROBE

    float Tram(const uint8_t point) {
      char cmd[100] = "";
      static bool inLev = false;
      float xpos = 0, ypos = 0, zval = 0;
      char str_1[6] = "", str_2[6] = "", str_3[6] = "";
      if (inLev) return NAN;

      TramXY(point, xpos, ypos);

      if (HMI_data.FullManualTramming) {
        sprintf_P(cmd, PSTR("M420S0\nG28O\nG90\nG0Z5F300\nG0X%sY%sF5000\nG0Z0F300"),
          dtostrf(xpos, 1, 1, str_1),
          dtostrf(ypos, 1, 1, str_2)
        );
        queue.inject(cmd);
      }
      else {
        // AUTO_BED_LEVELING_BILINEAR does not define MESH_INSET
        #ifndef MESH_MIN_X
          #define MESH_MIN_X (_MAX(X_MIN_BED + PROBING_MARGIN, X_MIN_POS))
        #endif
        #ifndef MESH_MIN_Y
          #define MESH_MIN_Y (_MAX(Y_MIN_BED + PROBING_MARGIN, Y_MIN_POS))
        #endif
        #ifndef MESH_MAX_X
          #define MESH_MAX_X (_MIN(X_MAX_BED - (PROBING_MARGIN), X_MAX_POS))
        #endif
        #ifndef MESH_MAX_Y
          #define MESH_MAX_Y (_MIN(Y_MAX_BED - (PROBING_MARGIN), Y_MAX_POS))
        #endif

        LIMIT(xpos, MESH_MIN_X, MESH_MAX_X);
        LIMIT(ypos, MESH_MIN_Y, MESH_MAX_Y);
        probe.stow();
        gcode.process_subcommands_now(F("M420S0\nG28O"));
        inLev = true;
        zval = probe.probe_at_point(xpos, ypos, PROBE_PT_STOW);
        if (isnan(zval))
          LCD_MESSAGE(MSG_ZPROBE_OUT);
        else {
          sprintf_P(cmd, PSTR("X:%s, Y:%s, Z:%s"),
            dtostrf(xpos, 1, 1, str_1),
            dtostrf(ypos, 1, 1, str_2),
            dtostrf(zval, 1, 2, str_3)
          );
          ui.set_status(cmd);
        }
        inLev = false;
      }
      return zval;
    }

  #else

    void Tram(const uint8_t point) {
      float xpos = 0, ypos = 0;
      TramXY(point, xpos, ypos);

      char cmd[100] = "", str_1[6] = "", str_2[6] = "";
      sprintf_P(cmd, PSTR("M420S0\nG28O\nG90\nG0Z5F300\nG0X%sY%sF5000\nG0Z0F300"), dtostrf(xpos, 1, 1, str_1), dtostrf(ypos, 1, 1, str_2));
      queue.inject(cmd);
    }

  #endif

  inline void TramFL() { Tram(0); }
  inline void TramFR() { Tram(1); }
  inline void TramBR() { Tram(2); }
  inline void TramBL() { Tram(3); }
  #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
    inline void TramC() { Tram(4); }
  #endif

  #if HAS_BED_PROBE && HAS_MESH

    void Trammingwizard() {
      if (HMI_data.FullManualTramming) {
        LCD_MESSAGE_F("Disable manual tramming");
        return;
      }
      bed_mesh_t zval = {0};
      zval[0][0] = TramFL();
      checkkey = NothingToDo;
      MeshViewer.DrawMesh(zval, 2, 2);
      zval[1][0] = TramFR();
      MeshViewer.DrawMesh(zval, 2, 2);
      zval[1][1] = TramBR();
      MeshViewer.DrawMesh(zval, 2, 2);
      zval[0][1] = TramBL();
      MeshViewer.DrawMesh(zval, 2, 2);

      DWINUI::Draw_CenteredString(140, F("Calculating average"));
      DWINUI::Draw_CenteredString(160, F("and relative heights"));
      safe_delay(1000);
      float avg = 0.0f;
      for (uint8_t x = 0; x < 2; ++x) for (uint8_t y = 0; y < 2; ++y) avg += zval[x][y];
      avg /= 4.0f;
      for (uint8_t x = 0; x < 2; ++x) for (uint8_t y = 0; y < 2; ++y) zval[x][y] -= avg;
      MeshViewer.DrawMesh(zval, 2, 2);
      ui.reset_status();

      #ifndef BED_TRAMMING_PROBE_TOLERANCE
        #define BED_TRAMMING_PROBE_TOLERANCE 0.05
      #endif

      if (ABS(MeshViewer.max - MeshViewer.min) < BED_TRAMMING_PROBE_TOLERANCE) {
        DWINUI::Draw_CenteredString(140, F("Corners leveled"));
        DWINUI::Draw_CenteredString(160, F("Tolerance achieved!"));
      }
      else {
        uint8_t p = 0;
        float max = 0;
        FSTR_P plabel;
        bool s = true;
        for (uint8_t x = 0; x < 2; ++x) for (uint8_t y = 0; y < 2; ++y) {
          const float d = ABS(zval[x][y]);
          if (max < d) {
            s = (zval[x][y] >= 0);
            max = d;
            p = x + 2 * y;
          }
        }
        switch (p) {
          case 0b00 : plabel = GET_TEXT_F(MSG_TRAM_FL); break;
          case 0b01 : plabel = GET_TEXT_F(MSG_TRAM_FR); break;
          case 0b10 : plabel = GET_TEXT_F(MSG_TRAM_BL); break;
          case 0b11 : plabel = GET_TEXT_F(MSG_TRAM_BR); break;
          default   : plabel = F(""); break;
        }
        DWINUI::Draw_CenteredString(120, F("Corners not leveled"));
        DWINUI::Draw_CenteredString(140, F("Knob adjustment required"));
        DWINUI::Draw_CenteredString(Color_Green, 160, s ? F("Lower") : F("Raise"));
        DWINUI::Draw_CenteredString(Color_Green, 180, plabel);
      }
      DWINUI::Draw_Button(BTN_Continue, 86, 305);
      checkkey = Menu;
      HMI_SaveProcessID(WaitResponse);
    }

    void SetManualTramming() {
      Toggle_Chkb_Line(HMI_data.FullManualTramming);
    }

  #endif // HAS_BED_PROBE && HAS_MESH

#endif // LCD_BED_TRAMMING

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
      planner.buffer_line(current_position, manual_feedrate_mm_s[Z_AXIS]);
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
#if HAS_X_AXIS
  void SetMaxSpeedX() { HMI_value.axis = X_AXIS, SetFloatOnClick(min_feedrate_edit_values.x, max_feedrate_edit_values.x, UNITFDIGITS, planner.settings.max_feedrate_mm_s[X_AXIS], ApplyMaxSpeed); }
#endif
#if HAS_Y_AXIS
  void SetMaxSpeedY() { HMI_value.axis = Y_AXIS, SetFloatOnClick(min_feedrate_edit_values.y, max_feedrate_edit_values.y, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Y_AXIS], ApplyMaxSpeed); }
#endif
#if HAS_Z_AXIS
  void SetMaxSpeedZ() { HMI_value.axis = Z_AXIS, SetFloatOnClick(min_feedrate_edit_values.z, max_feedrate_edit_values.z, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Z_AXIS], ApplyMaxSpeed); }
#endif
#if HAS_HOTEND
  void SetMaxSpeedE() { HMI_value.axis = E_AXIS; SetFloatOnClick(min_feedrate_edit_values.e, max_feedrate_edit_values.e, UNITFDIGITS, planner.settings.max_feedrate_mm_s[E_AXIS], ApplyMaxSpeed); }
#endif

void ApplyMaxAccel() { planner.set_max_acceleration(HMI_value.axis, MenuData.Value); }
#if HAS_X_AXIS
  void SetMaxAccelX() { HMI_value.axis = X_AXIS, SetIntOnClick(min_acceleration_edit_values.x, max_acceleration_edit_values.x, planner.settings.max_acceleration_mm_per_s2[X_AXIS], ApplyMaxAccel); }
#endif
#if HAS_Y_AXIS
  void SetMaxAccelY() { HMI_value.axis = Y_AXIS, SetIntOnClick(min_acceleration_edit_values.y, max_acceleration_edit_values.y, planner.settings.max_acceleration_mm_per_s2[Y_AXIS], ApplyMaxAccel); }
#endif
#if HAS_Z_AXIS
  void SetMaxAccelZ() { HMI_value.axis = Z_AXIS, SetIntOnClick(min_acceleration_edit_values.z, max_acceleration_edit_values.z, planner.settings.max_acceleration_mm_per_s2[Z_AXIS], ApplyMaxAccel); }
#endif
#if HAS_HOTEND
  void SetMaxAccelE() { HMI_value.axis = E_AXIS; SetIntOnClick(min_acceleration_edit_values.e, max_acceleration_edit_values.e, planner.settings.max_acceleration_mm_per_s2[E_AXIS], ApplyMaxAccel); }
#endif

#if HAS_CLASSIC_JERK
  void ApplyMaxJerk() { planner.set_max_jerk(HMI_value.axis, MenuData.Value / MINUNITMULT); }
  #if HAS_X_AXIS
    void SetMaxJerkX() { HMI_value.axis = X_AXIS, SetFloatOnClick(min_jerk_edit_values.x, max_jerk_edit_values.x, UNITFDIGITS, planner.max_jerk.x, ApplyMaxJerk); }
  #endif
  #if HAS_Y_AXIS
    void SetMaxJerkY() { HMI_value.axis = Y_AXIS, SetFloatOnClick(min_jerk_edit_values.y, max_jerk_edit_values.y, UNITFDIGITS, planner.max_jerk.y, ApplyMaxJerk); }
  #endif
  #if HAS_Z_AXIS
    void SetMaxJerkZ() { HMI_value.axis = Z_AXIS, SetFloatOnClick(min_jerk_edit_values.z, max_jerk_edit_values.z, UNITFDIGITS, planner.max_jerk.z, ApplyMaxJerk); }
  #endif
  #if HAS_HOTEND
    void SetMaxJerkE() { HMI_value.axis = E_AXIS; SetFloatOnClick(min_jerk_edit_values.e, max_jerk_edit_values.e, UNITFDIGITS, planner.max_jerk.e, ApplyMaxJerk); }
  #endif
#elif HAS_JUNCTION_DEVIATION
  void ApplyJDmm() { TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk()); }
  void SetJDmm() { SetPFloatOnClick(MIN_JD_MM, MAX_JD_MM, 3, ApplyJDmm); }
#endif

#if ENABLED(LIN_ADVANCE)
  void SetLA_K() { SetPFloatOnClick(0, 10, 3); }
#endif

#if HAS_X_AXIS
  void SetStepsX() { HMI_value.axis = X_AXIS, SetPFloatOnClick( min_steps_edit_values.x, max_steps_edit_values.x, UNITFDIGITS); }
#endif
#if HAS_Y_AXIS
  void SetStepsY() { HMI_value.axis = Y_AXIS, SetPFloatOnClick( min_steps_edit_values.y, max_steps_edit_values.y, UNITFDIGITS); }
#endif
#if HAS_Z_AXIS
  void SetStepsZ() { HMI_value.axis = Z_AXIS, SetPFloatOnClick( min_steps_edit_values.z, max_steps_edit_values.z, UNITFDIGITS); }
#endif
#if HAS_HOTEND
  void SetStepsE() { HMI_value.axis = E_AXIS; SetPFloatOnClick( min_steps_edit_values.e, max_steps_edit_values.e, UNITFDIGITS); }
#endif
#if ENABLED(FWRETRACT)
  void Return_FWRetract_Menu() { (PreviousMenu == FilSetMenu) ? Draw_FilSet_Menu() : Draw_Tune_Menu(); }
  void SetRetractLength() { SetPFloatOnClick( 0, 10, UNITFDIGITS); }
  void SetRetractSpeed()  { SetPFloatOnClick( 1, 90, UNITFDIGITS); }
  void SetZRaise()        { SetPFloatOnClick( 0, 2, 2); }
  void SetRecoverSpeed()  { SetPFloatOnClick( 1, 90, UNITFDIGITS); }
  void SetAddRecover()    { SetPFloatOnClick(-5, 5, UNITFDIGITS); }
#endif

// Special Menuitem Drawing functions =================================================

void onDrawBack(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 129, 72, 156, 84);
  onDrawMenuItem(menuitem, line);
}

void onDrawTempSubMenu(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1,  57, 104,  84, 116);
  onDrawSubMenu(menuitem, line);
}

void onDrawMotionSubMenu(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1,  87, 104, 114, 116);
  onDrawSubMenu(menuitem, line);
}

#if ENABLED(EEPROM_SETTINGS)
  void onDrawWriteEeprom(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 117, 104, 172, 116);
    onDrawMenuItem(menuitem, line);
  }

  void onDrawReadEeprom(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 174, 103, 229, 116);
    onDrawMenuItem(menuitem, line);
  }

  void onDrawResetEeprom(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1,   1, 118,  56, 131);
    onDrawMenuItem(menuitem, line);
  }
#endif

void onDrawInfoSubMenu(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 231, 104, 258, 116);
  onDrawSubMenu(menuitem, line);
}

void onDrawMoveX(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 58, 118, 106, 132);
  onDrawPFloatMenu(menuitem, line);
}

void onDrawMoveY(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 109, 118, 157, 132);
  onDrawPFloatMenu(menuitem, line);
}

void onDrawMoveZ(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 160, 118, 209, 132);
  onDrawPFloatMenu(menuitem, line);
}

#if HAS_HOTEND
  void onDrawMoveE(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 212, 118, 253, 131);
    onDrawPFloatMenu(menuitem, line);
  }
#endif

void onDrawMoveSubMenu(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 159, 70, 200, 84);
  onDrawSubMenu(menuitem, line);
}

void onDrawDisableMotors(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 204, 70, 259, 82);
  onDrawMenuItem(menuitem, line);
}

void onDrawAutoHome(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 0, 89, 41, 101);
  onDrawMenuItem(menuitem, line);
}

#if HAS_ZOFFSET_ITEM
  #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
    void onDrawZOffset(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 174, 164, 223, 177);
      onDrawPFloat2Menu(menuitem, line);
    }
  #endif
#endif

#if HAS_HOTEND
  void onDrawPreheat1(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 100, 89, 151, 101);
    onDrawMenuItem(menuitem, line);
  }
  #if PREHEAT_COUNT > 1
    void onDrawPreheat2(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 180, 89, 233, 100);
      onDrawMenuItem(menuitem, line);
    }
  #endif
#endif

#if HAS_PREHEAT
  void onDrawCooldown(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 1, 104,  56, 117);
    onDrawMenuItem(menuitem, line);
  }
#endif

void onDrawLanguage(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 239, 134, 266, 146);
  onDrawMenuItem(menuitem, line);
  DWINUI::Draw_String(VALX, MBASE(line), HMI_IsChinese() ? F("CN") : F("EN"));
}

void onDrawSelColorItem(MenuItemClass* menuitem, int8_t line) {
  const uint16_t color = *(uint16_t*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  dwinDrawRectangle(0, HMI_data.Highlight_Color, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  dwinDrawRectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
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
  dwinDrawRectangle(0, HMI_data.Highlight_Color, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  dwinDrawRectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  DWINUI::Draw_String(LBLX, MBASE(line) - 1, menuitem->caption);
  Draw_Menu_IntValue(HMI_data.Background_Color, line, 4, HMI_value.Color[i]);
  dwinDrawHLine(HMI_data.SplitLine_Color, 16, MYPOS(line + 1), 240);
}

void onDrawSpeedItem(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 116, 164, 171, 176);
  onDrawPIntMenu(menuitem, line);
}

#if HAS_HOTEND
  void onDrawHotendTemp(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 1, 134, 56, 146);
    onDrawPIntMenu(menuitem, line);
  }
#endif

#if HAS_HEATED_BED
  void onDrawBedTemp(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 58, 134, 113, 146);
    onDrawPIntMenu(menuitem, line);
  }
#endif

#if HAS_FAN
  void onDrawFanSpeed(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 115, 134, 170, 146);
    onDrawPInt8Menu(menuitem, line);
  }
#endif

void onDrawSteps(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) menuitem->SetFrame(1, 153, 148, 194, 161);
  onDrawSubMenu(menuitem, line);
}

#if ENABLED(MESH_BED_LEVELING)
  void onDrawMMeshMoveZ(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) menuitem->SetFrame(1, 160, 118, 209, 132);
    onDrawPFloat2Menu(menuitem, line);
  }
#endif

#if HAS_PREHEAT
  #if HAS_HOTEND
    void onDrawSetPreheatHotend(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 1, 134, 56, 146);
      onDrawPIntMenu(menuitem, line);
    }
  #endif
  #if HAS_HEATED_BED
    void onDrawSetPreheatBed(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 58, 134, 113, 146);
      onDrawPIntMenu(menuitem, line);
    }
  #endif
  #if HAS_FAN
    void onDrawSetPreheatFan(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) menuitem->SetFrame(1, 115, 134, 170, 146);
      onDrawPIntMenu(menuitem, line);
    }
  #endif
#endif // HAS_PREHEAT

void onDrawSpeed(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese())
    menuitem->SetFrame(1, 173, 133, 228, 147);
  onDrawSubMenu(menuitem, line);
}

#if HAS_X_AXIS
  void onDrawMaxSpeedX(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 228, 147);
      DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 58, MBASE(line));   // X
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

#if HAS_Y_AXIS
  void onDrawMaxSpeedY(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 228, 147);
      DWIN_Frame_AreaCopy(1, 1, 150, 7, 160, LBLX + 58, MBASE(line));       // Y
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

#if HAS_Z_AXIS
  void onDrawMaxSpeedZ(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 228, 147);
      DWIN_Frame_AreaCopy(1, 9, 150, 16, 160, LBLX + 58, MBASE(line) + 3);  // Z
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

#if HAS_HOTEND
  void onDrawMaxSpeedE(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 228, 147);
      DWIN_Frame_AreaCopy(1, 18, 150, 25, 160, LBLX + 58, MBASE(line));     // E
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

void onDrawAcc(MenuItemClass* menuitem, int8_t line) {
  if (HMI_IsChinese()) {
    menuitem->SetFrame(1, 173, 133, 200, 147);
    DWIN_Frame_AreaCopy(1, 28, 149, 69, 161, LBLX + 27, MBASE(line) + 1);   // ...Acceleration
  }
  onDrawSubMenu(menuitem, line);
}

#if HAS_X_AXIS
  void onDrawMaxAccelX(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 200, 147);
      DWIN_Frame_AreaCopy(1, 28,  149,  69, 161, LBLX + 27, MBASE(line));
      DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 71, MBASE(line));   // X
    }
    onDrawPInt32Menu(menuitem, line);
  }
#endif

#if HAS_Y_AXIS
  void onDrawMaxAccelY(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 200, 147);
      DWIN_Frame_AreaCopy(1, 28, 149,  69, 161, LBLX + 27, MBASE(line));
      DWIN_Frame_AreaCopy(1,  1, 150,   7, 160, LBLX + 71, MBASE(line));    // Y
    }
    onDrawPInt32Menu(menuitem, line);
  }
#endif

#if HAS_Z_AXIS
  void onDrawMaxAccelZ(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 200, 147);
      DWIN_Frame_AreaCopy(1, 28, 149,  69, 161, LBLX + 27, MBASE(line));
      DWIN_Frame_AreaCopy(1,  9, 150,  16, 160, LBLX + 71, MBASE(line));    // Z
    }
    onDrawPInt32Menu(menuitem, line);
  }
#endif

#if HAS_HOTEND
  void onDrawMaxAccelE(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 200, 147);
      DWIN_Frame_AreaCopy(1, 28, 149,  69, 161, LBLX + 27, MBASE(line));
      DWIN_Frame_AreaCopy(1, 18, 150,  25, 160, LBLX + 71, MBASE(line));    // E
    }
    onDrawPInt32Menu(menuitem, line);
  }
#endif

#if HAS_CLASSIC_JERK

  void onDrawJerk(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 173, 133, 200, 147);
      DWIN_Frame_AreaCopy(1, 1, 180, 28, 192, LBLX + 27, MBASE(line) + 1);  // ...
      DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 54, MBASE(line));   // ...Jerk
    }
    onDrawSubMenu(menuitem, line);
  }

  #if HAS_X_AXIS
    void onDrawMaxJerkX(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) {
        menuitem->SetFrame(1, 173, 133, 200, 147);
        DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(line));
        DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(line));
        DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 83, MBASE(line));
      }
      onDrawPFloatMenu(menuitem, line);
    }
  #endif

  #if HAS_Y_AXIS
    void onDrawMaxJerkY(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) {
        menuitem->SetFrame(1, 173, 133, 200, 147);
        DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(line));
        DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(line));
        DWIN_Frame_AreaCopy(1,   1, 150,   7, 160, LBLX + 83, MBASE(line));
      }
      onDrawPFloatMenu(menuitem, line);
    }
  #endif

  #if HAS_Z_AXIS
    void onDrawMaxJerkZ(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) {
        menuitem->SetFrame(1, 173, 133, 200, 147);
        DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(line));
        DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(line));
        DWIN_Frame_AreaCopy(1,   9, 150,  16, 160, LBLX + 83, MBASE(line));
      }
      onDrawPFloatMenu(menuitem, line);
    }
  #endif

  #if HAS_HOTEND

    void onDrawMaxJerkE(MenuItemClass* menuitem, int8_t line) {
      if (HMI_IsChinese()) {
        menuitem->SetFrame(1, 173, 133, 200, 147);
        DWIN_Frame_AreaCopy(1,   1, 180,  28, 192, LBLX + 27, MBASE(line));
        DWIN_Frame_AreaCopy(1, 202, 133, 228, 147, LBLX + 53, MBASE(line));
        DWIN_Frame_AreaCopy(1,  18, 150,  25, 160, LBLX + 83, MBASE(line));
      }
      onDrawPFloatMenu(menuitem, line);
    }

  #endif

#endif // HAS_CLASSIC_JERK

#if HAS_X_AXIS
  void onDrawStepsX(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 153, 148, 194, 161);
      DWIN_Frame_AreaCopy(1, 229, 133, 236, 147, LBLX + 44, MBASE(line));      // X
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

#if HAS_Y_AXIS
  void onDrawStepsY(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 153, 148, 194, 161);
      DWIN_Frame_AreaCopy(1,   1, 150,   7, 160, LBLX + 44, MBASE(line));      // Y
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

#if HAS_Z_AXIS
  void onDrawStepsZ(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 153, 148, 194, 161);
      DWIN_Frame_AreaCopy(1,   9, 150,  16, 160, LBLX + 44, MBASE(line));      // Z
    }
    onDrawPFloatMenu(menuitem, line);
  }
#endif

#if HAS_HOTEND

  void onDrawStepsE(MenuItemClass* menuitem, int8_t line) {
    if (HMI_IsChinese()) {
      menuitem->SetFrame(1, 153, 148, 194, 161);
      DWIN_Frame_AreaCopy(1,  18, 150,  25, 160, LBLX + 44, MBASE(line));    // E
    }
    onDrawPFloatMenu(menuitem, line);
  }

#endif

#if HAS_ONESTEP_LEVELING
  void onDrawManualTramming(MenuItemClass* menuitem, int8_t line) { onDrawChkbMenu(menuitem, line, HMI_data.FullManualTramming); }
#endif

// Menu Creation and Drawing functions ======================================================

frame_rect_t selrect(frame_rect_t) {
  return HMI_IsChinese() ? frame_rect_t({ 133, 1, 28, 13 }) : frame_rect_t({ 0 });
}

void Draw_Prepare_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(PrepareMenu, selrect({133, 1, 28, 13}), MSG_PREPARE, 10 + PREHEAT_COUNT)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENT_MAN, onDrawSubMenu, Draw_FilamentMan_Menu);
    #endif
    MENU_ITEM(ICON_Axis, MSG_MOVE_AXIS, onDrawMoveSubMenu, Draw_Move_Menu);
    #if ENABLED(LCD_BED_TRAMMING)
      MENU_ITEM(ICON_Tram, MSG_BED_TRAMMING, onDrawSubMenu, Draw_Tramming_Menu);
    #endif
    MENU_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, onDrawDisableMotors, DisableMotors);
    #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
      MENU_ITEM(ICON_Homing, MSG_HOMING, onDrawSubMenu, Draw_Homing_Menu);
    #else
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawAutoHome, AutoHome);
    #endif
    #if ENABLED(MESH_BED_LEVELING)
      MENU_ITEM(ICON_ManualMesh, MSG_MANUAL_MESH, onDrawSubMenu, Draw_ManualMesh_Menu);
    #elif HAS_BED_PROBE
      MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, AutoLev);
    #endif
    #if HAS_ZOFFSET_ITEM
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, onDrawSubMenu, Draw_ZOffsetWiz_Menu);
      #elif ENABLED(BABYSTEPPING)
        EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
      #endif
    #endif
    #if HAS_PREHEAT
      #define _ITEM_PREHEAT(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheat##N);
      REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT)
    #endif
    MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawCooldown, DoCoolDown);
    MENU_ITEM(ICON_Language, MSG_UI_LANGUAGE, onDrawLanguage, SetLanguage);
  }
  ui.reset_status(true);
  UpdateMenu(PrepareMenu);
}

#if ENABLED(LCD_BED_TRAMMING)

  void Draw_Tramming_Menu() {
    checkkey = Menu;
    if (SET_MENU(TrammingMenu, MSG_BED_TRAMMING, 8)) {
      BACK_ITEM(Draw_Prepare_Menu);
      #if HAS_BED_PROBE && HAS_MESH
        MENU_ITEM(ICON_ProbeSet, MSG_TRAMMING_WIZARD, onDrawMenuItem, Trammingwizard);
        EDIT_ITEM(ICON_ProbeSet, MSG_BED_TRAMMING_MANUAL, onDrawChkbMenu, SetManualTramming, &HMI_data.FullManualTramming);
      #elif !HAS_BED_PROBE && HAS_ZOFFSET_ITEM
        MENU_ITEM_F(ICON_MoveZ0, "Home Z and disable", onDrawMenuItem, HomeZandDisable);
      #endif
      MENU_ITEM(ICON_Axis, MSG_TRAM_FL, onDrawMenuItem, TramFL);
      MENU_ITEM(ICON_Axis, MSG_TRAM_FR, onDrawMenuItem, TramFR);
      MENU_ITEM(ICON_Axis, MSG_TRAM_BR, onDrawMenuItem, TramBR);
      MENU_ITEM(ICON_Axis, MSG_TRAM_BL, onDrawMenuItem, TramBL);
      #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
        MENU_ITEM(ICON_Axis, MSG_TRAM_C, onDrawMenuItem, TramC);
      #endif
    }
    UpdateMenu(TrammingMenu);
  }

#endif // LCD_BED_TRAMMING

void Draw_Control_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(ControlMenu, selrect({103, 1, 28, 14}), MSG_CONTROL, 10)) {
    BACK_ITEM(Goto_Main_Menu);
    MENU_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawTempSubMenu, Draw_Temperature_Menu);
    MENU_ITEM(ICON_Motion, MSG_MOTION, onDrawMotionSubMenu, Draw_Motion_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawWriteEeprom, WriteEeprom);
      MENU_ITEM(ICON_ReadEEPROM, MSG_LOAD_EEPROM, onDrawReadEeprom, ReadEeprom);
      MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawResetEeprom, ResetEeprom);
    #endif
    MENU_ITEM(ICON_Reboot, MSG_RESET_PRINTER, onDrawMenuItem, RebootPrinter);
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
    MENU_ITEM(ICON_Info, MSG_INFO_SCREEN, onDrawInfoSubMenu, Goto_Info_Menu);
  }
  ui.reset_status(true);
  UpdateMenu(ControlMenu);
}

void Draw_AdvancedSettings_Menu() {
  checkkey = Menu;
  if (SET_MENU(AdvancedSettings, MSG_ADVANCED_SETTINGS, 22)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
    #endif
    #if HAS_MESH
      MENU_ITEM(ICON_ProbeSet, MSG_MESH_LEVELING, onDrawSubMenu, Draw_MeshSet_Menu);
    #endif
    #if HAS_BED_PROBE
      MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, Draw_ProbeSet_Menu);
    #endif
    MENU_ITEM(ICON_FilSet, MSG_FILAMENT_SET, onDrawSubMenu, Draw_FilSet_Menu);
    #if ENABLED(PIDTEMP) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID " Settings", onDrawSubMenu, Draw_HotendPID_Menu);
    #endif
    #if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
      MENU_ITEM_F(ICON_MPCNozzle, "MPC Settings", onDrawSubMenu, Draw_HotendMPC_Menu);
    #endif
    #if ENABLED(PIDTEMPBED) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM_F(ICON_PIDBed, STR_BED_PID " Settings", onDrawSubMenu, Draw_BedPID_Menu);
    #endif
    #if HAS_TRINAMIC_CONFIG
      MENU_ITEM(ICON_TMCSet, MSG_TMC_DRIVERS, onDrawSubMenu, Draw_TrinamicConfig_menu);
    #endif
    #if HAS_ESDIAG
      MENU_ITEM_F(ICON_ESDiag, "End-stops diag.", onDrawSubMenu, Draw_EndStopDiag);
    #endif
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(ICON_PrintStats, MSG_INFO_STATS_MENU, onDrawSubMenu, Goto_PrintStats);
      MENU_ITEM(ICON_PrintStatsReset, MSG_INFO_PRINT_COUNT_RESET, onDrawSubMenu, PrintStatsReset);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
    #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_SOUND_ENABLE, onDrawChkbMenu, SetEnableSound, &ui.sound_on);
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, SetPwrLossr, &recovery.enabled);
    #endif
    #if HAS_GCODE_PREVIEW
      EDIT_ITEM(ICON_File, MSG_HAS_PREVIEW, onDrawChkbMenu, SetPreview, &HMI_data.EnablePreview);
    #endif
    #if ENABLED(MEDIASORT_MENU_ITEM)
      EDIT_ITEM(ICON_File, MSG_MEDIA_SORT, onDrawChkbMenu, SetMediaSort, &HMI_data.MediaSort);
    #endif
    EDIT_ITEM(ICON_File, MSG_MEDIA_UPDATE, onDrawChkbMenu, SetMediaAutoMount, &HMI_data.MediaAutoMount);
    #if ENABLED(BAUD_RATE_GCODE)
      EDIT_ITEM_F(ICON_SetBaudRate, "115K baud", onDrawChkbMenu, SetBaudRate, &HMI_data.Baud115K);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    #if HAS_CUSTOM_COLORS
      MENU_ITEM(ICON_Scolor, MSG_COLORS_SELECT, onDrawSubMenu, Draw_SelectColors_Menu);
    #endif
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedSettings);
}

void Draw_Move_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(MoveMenu, selrect({192, 1, 42, 14}), MSG_MOVE_AXIS, 6)) {
    BACK_ITEM(Draw_Prepare_Menu);
    EDIT_ITEM(ICON_Axis, MSG_LIVE_MOVE, onDrawChkbMenu, SetLiveMove, &EnableLiveMove);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawMoveX, SetMoveX, &current_position.x);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawMoveY, SetMoveY, &current_position.y);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MoveZ, MSG_MOVE_Z, onDrawMoveZ, SetMoveZ, &current_position.z);
    #endif
    #if HAS_HOTEND
      gcode.process_subcommands_now(F("G92E0"));  // Reset extruder position
      EDIT_ITEM(ICON_Extruder, MSG_MOVE_E, onDrawMoveE, SetMoveE, &current_position.e);
    #endif
  }
  UpdateMenu(MoveMenu);
  if (!all_axes_trusted()) LCD_MESSAGE_F("WARNING: Current position unknown. Home axes.");
}

#if HAS_HOME_OFFSET

  void Draw_HomeOffset_Menu() {
    checkkey = Menu;
    if (SET_MENU(HomeOffMenu, MSG_SET_HOME_OFFSETS, 4)) {
      BACK_ITEM(Draw_PhySet_Menu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_HomeOffsetX, MSG_HOME_OFFSET_X, onDrawPFloatMenu, SetHomeOffsetX, &home_offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_HomeOffsetY, MSG_HOME_OFFSET_Y, onDrawPFloatMenu, SetHomeOffsetY, &home_offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_HomeOffsetZ, MSG_HOME_OFFSET_Z, onDrawPFloatMenu, SetHomeOffsetZ, &home_offset.z);
      #endif
    }
    UpdateMenu(HomeOffMenu);
  }

#endif // HAS_HOME_OFFSET

#if HAS_BED_PROBE

  void Draw_ProbeSet_Menu() {
    checkkey = Menu;
    if (SET_MENU(ProbeSetMenu, MSG_ZPROBE_SETTINGS, 9)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_ProbeOffsetX, MSG_ZPROBE_XOFFSET, onDrawPFloatMenu, SetProbeOffsetX, &probe.offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_ProbeOffsetY, MSG_ZPROBE_YOFFSET, onDrawPFloatMenu, SetProbeOffsetY, &probe.offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_ProbeOffsetZ, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetProbeOffsetZ, &probe.offset.z);
      #endif
      #if ENABLED(BLTOUCH)
        MENU_ITEM(ICON_ProbeStow, MSG_MANUAL_STOW, onDrawMenuItem, ProbeStow);
        MENU_ITEM(ICON_ProbeDeploy, MSG_MANUAL_DEPLOY, onDrawMenuItem, ProbeDeploy);
        MENU_ITEM(ICON_BltouchReset, MSG_BLTOUCH_RESET, onDrawMenuItem, bltouch._reset);
        #if HAS_BLTOUCH_HS_MODE
          EDIT_ITEM(ICON_HSMode, MSG_ENABLE_HS_MODE, onDrawChkbMenu, SetHSMode, &bltouch.high_speed_mode);
        #endif
      #endif
      MENU_ITEM(ICON_ProbeTest, MSG_M48_TEST, onDrawMenuItem, ProbeTest);
    }
    UpdateMenu(ProbeSetMenu);
  }

#endif // HAS_BED_PROBE

void Draw_FilSet_Menu() {
  checkkey = Menu;
  if (SET_MENU(FilSetMenu, MSG_FILAMENT_SET, 9)) {
    BACK_ITEM(Draw_AdvancedSettings_Menu);
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, SetRunoutEnable, &runout.enabled);
    #endif
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_DISTANCE_MM, onDrawPFloatMenu, SetRunoutDistance, &runout.runout_distance());
    #endif
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      EDIT_ITEM(ICON_ExtrudeMinT, MSG_EXTRUDER_MIN_TEMP, onDrawPIntMenu, SetExtMinT, &HMI_data.ExtMinT);
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      EDIT_ITEM(ICON_FilLoad, MSG_FILAMENT_LOAD, onDrawPFloatMenu, SetFilLoad, &fc_settings[0].load_length);
      EDIT_ITEM(ICON_FilUnload, MSG_FILAMENT_UNLOAD, onDrawPFloatMenu, SetFilUnload, &fc_settings[0].unload_length);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, Draw_FWRetract_Menu);
    #endif
  }
  UpdateMenu(FilSetMenu);
}

#if ALL(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)

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
      #if !ALL(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
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
            EDIT_ITEM(ICON_LedControl, MSG_COLORS_WHITE, onDrawPInt8Menu, SetLEDColorW, &leds.color.w);
          #endif
        #endif
      #endif
    }
    UpdateMenu(LedControlMenu);
  }

#endif // LED_CONTROL_MENU

void Draw_Tune_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(TuneMenu, selrect({73, 2, 28, 12}), MSG_TUNE, 17)) {
    BACK_ITEM(Goto_PrintProcess);
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawSpeedItem, SetSpeed, &feedrate_percentage);
    #if HAS_HOTEND
      HotendTargetItem = EDIT_ITEM(ICON_HotendTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawHotendTemp, SetHotendTemp, &thermalManager.temp_hotend[0].target);
    #endif
    #if HAS_HEATED_BED
      BedTargetItem = EDIT_ITEM(ICON_BedTemp, MSG_UBL_SET_TEMP_BED, onDrawBedTemp, SetBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      FanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawFanSpeed, SetFanSpeed, &thermalManager.fan_speed[0]);
    #endif
    #if ALL(HAS_ZOFFSET_ITEM, HAS_BED_PROBE, BABYSTEP_ZPROBE_OFFSET, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawZOffset, SetZOffset, &BABY_Z_VAR);
    #elif ALL(HAS_ZOFFSET_ITEM, MESH_BED_LEVELING, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
    #endif
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, SetFlow, &planner.flow_percentage[0]);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, ChangeFilament);
    #endif
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, SetRunoutEnable, &runout.enabled);
    #endif
    #if ENABLED(PLR_TUNE_ITEM)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, SetPwrLossr, &recovery.enabled);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, Draw_FWRetract_Menu);
    #endif
    #if ENABLED(JD_TUNE_ITEM)
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, SetJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(ADVK_TUNE_ITEM)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, SetLA_K, &planner.extruder_advance_K[0]);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    #if ENABLED(CASE_LIGHT_MENU)
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
    #elif ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)
      EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, SetLedStatus, &leds.lights_on);
    #endif
  }
  UpdateMenu(TuneMenu);
}

#if ENABLED(ADAPTIVE_STEP_SMOOTHING)
  void SetAdaptiveStepSmoothing() {
    Toggle_Chkb_Line(HMI_data.AdaptiveStepSmoothing);
  }
#endif

#if ENABLED(SHAPING_MENU)
  void ApplyShapingFreq() { stepper.set_shaping_frequency(HMI_value.axis, MenuData.Value / 100); }
  void ApplyShapingZeta() { stepper.set_shaping_damping_ratio(HMI_value.axis, MenuData.Value / 100); }

  #if ENABLED(INPUT_SHAPING_X)
    void onDrawShapingXFreq(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_frequency(X_AXIS)); }
    void onDrawShapingXZeta(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_damping_ratio(X_AXIS)); }
    void SetShapingXFreq() { HMI_value.axis = X_AXIS; SetFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(X_AXIS), ApplyShapingFreq); }
    void SetShapingXZeta() { HMI_value.axis = X_AXIS; SetFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(X_AXIS), ApplyShapingZeta); }
  #endif

  #if ENABLED(INPUT_SHAPING_Y)
    void onDrawShapingYFreq(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_frequency(Y_AXIS)); }
    void onDrawShapingYZeta(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_damping_ratio(Y_AXIS)); }
    void SetShapingYFreq() { HMI_value.axis = Y_AXIS; SetFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(Y_AXIS), ApplyShapingFreq); }
    void SetShapingYZeta() { HMI_value.axis = Y_AXIS; SetFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(Y_AXIS), ApplyShapingZeta); }
  #endif

  void Draw_InputShaping_menu() {
    checkkey = Menu;
    if (SET_MENU(InputShapingMenu, MSG_INPUT_SHAPING, 5)) {
      BACK_ITEM(Draw_Motion_Menu);
      #if ENABLED(INPUT_SHAPING_X)
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_FREQ, onDrawShapingXFreq, SetShapingXFreq);
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_ZETA, onDrawShapingXZeta, SetShapingXZeta);
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_FREQ, onDrawShapingYFreq, SetShapingYFreq);
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_ZETA, onDrawShapingYZeta, SetShapingYZeta);
      #endif
    }
    UpdateMenu(InputShapingMenu);
  }
#endif

#if HAS_TRINAMIC_CONFIG
  #if AXIS_IS_TMC(X)
    void SetXTMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperX.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Y)
    void SetYTMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperY.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Z)
    void SetZTMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperZ.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(E0)
    void SetETMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperE0.refresh_stepper_current(); }); }
  #endif

  void Draw_TrinamicConfig_menu() {
    checkkey = Menu;
    if (SET_MENU(TrinamicConfigMenu, MSG_TMC_DRIVERS, 5)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if AXIS_IS_TMC(X)
        EDIT_ITEM(ICON_TMCXSet, MSG_TMC_ACURRENT, onDrawPIntMenu, SetXTMCCurrent, &stepperX.val_mA);
      #endif
      #if AXIS_IS_TMC(Y)
        EDIT_ITEM(ICON_TMCYSet, MSG_TMC_BCURRENT, onDrawPIntMenu, SetYTMCCurrent, &stepperY.val_mA);
      #endif
      #if AXIS_IS_TMC(Z)
        EDIT_ITEM(ICON_TMCZSet, MSG_TMC_CCURRENT, onDrawPIntMenu, SetZTMCCurrent, &stepperZ.val_mA);
      #endif
      #if AXIS_IS_TMC(E0)
        EDIT_ITEM(ICON_TMCESet, MSG_TMC_ECURRENT, onDrawPIntMenu, SetETMCCurrent, &stepperE0.val_mA);
      #endif
    }
    UpdateMenu(TrinamicConfigMenu);
  }
#endif

void Draw_Motion_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(MotionMenu, selrect({1, 16, 28, 13}), MSG_MOTION, 10)) {
    BACK_ITEM(Draw_Control_Menu);
    MENU_ITEM(ICON_MaxSpeed, MSG_SPEED, onDrawSpeed, Draw_MaxSpeed_Menu);
    MENU_ITEM(ICON_MaxAccelerated, MSG_ACCELERATION, onDrawAcc, Draw_MaxAccel_Menu);
    #if HAS_CLASSIC_JERK
      MENU_ITEM(ICON_MaxJerk, MSG_JERK, onDrawJerk, Draw_MaxJerk_Menu);
    #elif HAS_JUNCTION_DEVIATION
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, SetJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(LIN_ADVANCE)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, SetLA_K, &planner.extruder_advance_K[0]);
    #endif
    #if ENABLED(SHAPING_MENU)
      MENU_ITEM(ICON_InputShaping, MSG_INPUT_SHAPING, onDrawSubMenu, Draw_InputShaping_menu);
    #endif
    #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
      EDIT_ITEM(ICON_UBLActive, MSG_STEP_SMOOTHING, onDrawChkbMenu, SetAdaptiveStepSmoothing, &HMI_data.AdaptiveStepSmoothing);
    #endif
    MENU_ITEM(ICON_Step, MSG_STEPS_PER_MM, onDrawSteps, Draw_Steps_Menu);
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, SetFlow, &planner.flow_percentage[0]);
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, SetSpeed, &feedrate_percentage);
  }
  UpdateMenu(MotionMenu);
}

#if HAS_PREHEAT
  void Draw_PreheatHotend_Menu() {
    checkkey = Menu;
    if (SET_MENU(PreheatHotendMenu, MSG_PREHEAT_HOTEND, 1 + PREHEAT_COUNT)) {
      BACK_ITEM(Draw_FilamentMan_Menu);
      #define _ITEM_PREHEAT_HE(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheatHotend##N);
      REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT_HE)
    }
    UpdateMenu(PreheatHotendMenu);
  }
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void Draw_FilamentMan_Menu() {
    checkkey = Menu;
    if (SET_MENU(FilamentMenu, MSG_FILAMENT_MAN, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_Park, MSG_FILAMENT_PARK_ENABLED, onDrawMenuItem, ParkHead);
      #endif
      #if HAS_PREHEAT
        MENU_ITEM(ICON_SetEndTemp, MSG_PREHEAT_HOTEND, onDrawSubMenu, Draw_PreheatHotend_Menu);
      #endif
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, ChangeFilament);
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        MENU_ITEM(ICON_FilUnload, MSG_FILAMENTUNLOAD, onDrawMenuItem, UnloadFilament);
        MENU_ITEM(ICON_FilLoad, MSG_FILAMENTLOAD, onDrawMenuItem, LoadFilament);
      #endif
    }
    UpdateMenu(FilamentMenu);
  }

#endif

#if ENABLED(MESH_BED_LEVELING)

  void Draw_ManualMesh_Menu() {
    checkkey = Menu;
    if (SET_MENU(ManualMesh, MSG_UBL_MANUAL_MESH, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_ManualMesh, MSG_LEVEL_BED, onDrawMenuItem, ManualMeshStart);
      MMeshMoveZItem = EDIT_ITEM(ICON_Zoffset, MSG_MOVE_Z, onDrawMMeshMoveZ, SetMMeshMoveZ, &current_position.z);
      MENU_ITEM(ICON_Axis, MSG_UBL_CONTINUE_MESH, onDrawMenuItem, ManualMeshContinue);
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);
      MENU_ITEM(ICON_MeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, ManualMeshSave);
    }
    UpdateMenu(ManualMesh);
  }

#endif // MESH_BED_LEVELING

#if HAS_PREHEAT

  void Draw_Preheat_Menu(bool NotCurrent) {
    checkkey = Menu;
    if (NotCurrent) {
      BACK_ITEM(Draw_Temperature_Menu);
      #if HAS_HOTEND
        EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawSetPreheatHotend, SetPreheatEndTemp, &ui.material_preset[HMI_value.Select].hotend_temp);
      #endif
      #if HAS_HEATED_BED
        EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawSetPreheatBed, SetPreheatBedTemp, &ui.material_preset[HMI_value.Select].bed_temp);
      #endif
      #if HAS_FAN
        EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawSetPreheatFan, SetPreheatFanSpeed, &ui.material_preset[HMI_value.Select].fan_speed);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawWriteEeprom, WriteEeprom);
      #endif
    }
    UpdateMenu(PreheatMenu);
  }

  #define _Preheat_Menu(N) \
    void Draw_Preheat## N ##_Menu() { \
      HMI_value.Select = (N) - 1; \
      Draw_Preheat_Menu(SET_MENU(PreheatMenu, MSG_PREHEAT_## N ##_SETTINGS, 5)); \
    }
  REPEAT_1(PREHEAT_COUNT, _Preheat_Menu)

#endif // HAS_PREHEAT

void Draw_Temperature_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(TemperatureMenu, selrect({236, 2, 28, 12}), MSG_TEMPERATURE, 4 + PREHEAT_COUNT)) {
    BACK_ITEM(Draw_Control_Menu);
    #if HAS_HOTEND
      HotendTargetItem = EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawHotendTemp, SetHotendTemp, &thermalManager.temp_hotend[0].target);
    #endif
    #if HAS_HEATED_BED
      BedTargetItem = EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawBedTemp, SetBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      FanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawFanSpeed, SetFanSpeed, &thermalManager.fan_speed[0]);
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
  if (SET_MENU_R(MaxSpeedMenu, selrect({1, 16, 28, 13}), MSG_MAX_SPEED, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxSpeedX, MSG_VMAX_A, onDrawMaxSpeedX, SetMaxSpeedX, &planner.settings.max_feedrate_mm_s[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxSpeedY, MSG_VMAX_B, onDrawMaxSpeedY, SetMaxSpeedY, &planner.settings.max_feedrate_mm_s[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxSpeedZ, MSG_VMAX_C, onDrawMaxSpeedZ, SetMaxSpeedZ, &planner.settings.max_feedrate_mm_s[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxSpeedE, MSG_VMAX_E, onDrawMaxSpeedE, SetMaxSpeedE, &planner.settings.max_feedrate_mm_s[E_AXIS]);
    #endif
  }
  UpdateMenu(MaxSpeedMenu);
}

void Draw_MaxAccel_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(MaxAccelMenu, selrect({1, 16, 28, 13}), MSG_AMAX_EN, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxAccX, MSG_AMAX_A, onDrawMaxAccelX, SetMaxAccelX, &planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxAccY, MSG_AMAX_B, onDrawMaxAccelY, SetMaxAccelY, &planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxAccZ, MSG_AMAX_C, onDrawMaxAccelZ, SetMaxAccelZ, &planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxAccE, MSG_AMAX_E, onDrawMaxAccelE, SetMaxAccelE, &planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
    #endif
  }
  UpdateMenu(MaxAccelMenu);
}

#if HAS_CLASSIC_JERK

  void Draw_MaxJerk_Menu() {
    checkkey = Menu;
    if (SET_MENU_R(MaxJerkMenu, selrect({1, 16, 28, 13}), MSG_JERK, 5)) {
      BACK_ITEM(Draw_Motion_Menu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkX, MSG_VA_JERK, onDrawMaxJerkX, SetMaxJerkX, &planner.max_jerk.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkY, MSG_VB_JERK, onDrawMaxJerkY, SetMaxJerkY, &planner.max_jerk.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkZ, MSG_VC_JERK, onDrawMaxJerkZ, SetMaxJerkZ, &planner.max_jerk.z);
      #endif
      #if HAS_HOTEND
        EDIT_ITEM(ICON_MaxSpeedJerkE, MSG_VE_JERK, onDrawMaxJerkE, SetMaxJerkE, &planner.max_jerk.e);
      #endif
    }
    UpdateMenu(MaxJerkMenu);
  }

#endif // HAS_CLASSIC_JERK

void Draw_Steps_Menu() {
  checkkey = Menu;
  if (SET_MENU_R(StepsMenu, selrect({1, 16, 28, 13}), MSG_STEPS_PER_MM, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_StepX, MSG_A_STEPS, onDrawStepsX, SetStepsX, &planner.settings.axis_steps_per_mm[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_StepY, MSG_B_STEPS, onDrawStepsY, SetStepsY, &planner.settings.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_StepZ, MSG_C_STEPS, onDrawStepsZ, SetStepsZ, &planner.settings.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_StepE, MSG_E_STEPS, onDrawStepsE, SetStepsE, &planner.settings.axis_steps_per_mm[E_AXIS]);
    #endif
  }
  UpdateMenu(StepsMenu);
}

//=============================================================================
// UI editable custom colors
//=============================================================================

#if HAS_CUSTOM_COLORS

  void RestoreDefaultColors() {
    DWIN_SetColorDefaults();
    DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
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
    dwinDrawRectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
  }
  void SetRGBColor() {
    const uint8_t color = static_cast<MenuItemClass*>(CurrentMenu->SelectedItem())->icon;
    SetIntOnClick(0, (color == 1) ? 63 : 31, HMI_value.Color[color], nullptr, LiveRGBColor);
  }

  void DWIN_ApplyColor() {
    *MenuData.P_Int = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
    DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
    Draw_SelectColors_Menu();
    hash_changed = true;
    LCD_MESSAGE(MSG_COLORS_APPLIED);
    DWIN_Draw_Dashboard();
  }

  void Draw_SelectColors_Menu() {
    checkkey = Menu;
    if (SET_MENU(SelectColorMenu, MSG_COLORS_SELECT, 20)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      MENU_ITEM(ICON_StockConfiguration, MSG_RESTORE_DEFAULTS, onDrawMenuItem, RestoreDefaultColors);
      EDIT_ITEM_F(0, "Screen Background", onDrawSelColorItem, SelColor, &HMI_data.Background_Color);
      EDIT_ITEM_F(0, "Cursor", onDrawSelColorItem, SelColor, &HMI_data.Cursor_Color);
      EDIT_ITEM_F(0, "Title Background", onDrawSelColorItem, SelColor, &HMI_data.TitleBg_Color);
      EDIT_ITEM_F(0, "Title Text", onDrawSelColorItem, SelColor, &HMI_data.TitleTxt_Color);
      EDIT_ITEM_F(0, "Text", onDrawSelColorItem, SelColor, &HMI_data.Text_Color);
      EDIT_ITEM_F(0, "Selected", onDrawSelColorItem, SelColor, &HMI_data.Selected_Color);
      EDIT_ITEM_F(0, "Split Line", onDrawSelColorItem, SelColor, &HMI_data.SplitLine_Color);
      EDIT_ITEM_F(0, "Highlight", onDrawSelColorItem, SelColor, &HMI_data.Highlight_Color);
      EDIT_ITEM_F(0, "Status Background", onDrawSelColorItem, SelColor, &HMI_data.StatusBg_Color);
      EDIT_ITEM_F(0, "Status Text", onDrawSelColorItem, SelColor, &HMI_data.StatusTxt_Color);
      EDIT_ITEM_F(0, "Popup Background", onDrawSelColorItem, SelColor, &HMI_data.PopupBg_Color);
      EDIT_ITEM_F(0, "Popup Text", onDrawSelColorItem, SelColor, &HMI_data.PopupTxt_Color);
      EDIT_ITEM_F(0, "Alert Background", onDrawSelColorItem, SelColor, &HMI_data.AlertBg_Color);
      EDIT_ITEM_F(0, "Alert Text", onDrawSelColorItem, SelColor, &HMI_data.AlertTxt_Color);
      EDIT_ITEM_F(0, "Percent Text", onDrawSelColorItem, SelColor, &HMI_data.PercentTxt_Color);
      EDIT_ITEM_F(0, "Bar Fill", onDrawSelColorItem, SelColor, &HMI_data.Barfill_Color);
      EDIT_ITEM_F(0, "Indicator value", onDrawSelColorItem, SelColor, &HMI_data.Indicator_Color);
      EDIT_ITEM_F(0, "Coordinate value", onDrawSelColorItem, SelColor, &HMI_data.Coordinate_Color);
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
    dwinDrawRectangle(1, *MenuData.P_Int, 20, 315, DWIN_WIDTH - 20, 335);
  }

#endif // HAS_CUSTOM_COLORS

//=============================================================================
// Nozzle and Bed PID/MPC
//=============================================================================

#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)

  #if ENABLED(MPC_EDIT_MENU)
    void SetHeaterPower() { SetPFloatOnClick(1, 200, 1); }
    void SetBlkHeatCapacity() { SetPFloatOnClick(0, 40, 2); }
    void SetSensorResponse() { SetPFloatOnClick(0, 1, 4); }
    void SetAmbientXfer() { SetPFloatOnClick(0, 1, 4); }
    #if ENABLED(MPC_INCLUDE_FAN)
      void onDrawFanAdj(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 4, thermalManager.temp_hotend[0].fanCoefficient()); }
      void ApplyFanAdj() { thermalManager.temp_hotend[0].applyFanAdjustment(MenuData.Value / POW(10, 4)); }
      void SetFanAdj() { SetFloatOnClick(0, 1, 4, thermalManager.temp_hotend[0].fanCoefficient(), ApplyFanAdj); }
    #endif
  #endif

  void Draw_HotendMPC_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(HotendMPCMenu, "MPC Settings", 7)) {
      MPC_t &mpc = thermalManager.temp_hotend[0].mpc;
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if ENABLED(MPC_AUTOTUNE_MENU)
        MENU_ITEM(ICON_MPCNozzle, MSG_MPC_AUTOTUNE, onDrawMenuItem, []{ thermalManager.MPC_autotune(active_extruder, Temperature::MPCTuningType::AUTO); });
      #endif
      #if ENABLED(MPC_EDIT_MENU)
        EDIT_ITEM(ICON_MPCHeater, MSG_MPC_POWER, onDrawPFloatMenu, SetHeaterPower, &mpc.heater_power);
        EDIT_ITEM(ICON_MPCHeatCap, MSG_MPC_BLOCK_HEAT_CAPACITY, onDrawPFloat2Menu, SetBlkHeatCapacity, &mpc.block_heat_capacity);
        EDIT_ITEM(ICON_MPCValue, MSG_SENSOR_RESPONSIVENESS, onDrawPFloat4Menu, SetSensorResponse, &mpc.sensor_responsiveness);
        EDIT_ITEM(ICON_MPCValue, MSG_MPC_AMBIENT_XFER_COEFF, onDrawPFloat4Menu, SetAmbientXfer, &mpc.ambient_xfer_coeff_fan0);
        #if ENABLED(MPC_INCLUDE_FAN)
          EDIT_ITEM(ICON_MPCFan, MSG_MPC_AMBIENT_XFER_COEFF_FAN, onDrawFanAdj, SetFanAdj, &mpc.fan255_adjustment);
        #endif
      #endif
    }
    UpdateMenu(HotendMPCMenu);
  }

#endif // MPC_EDIT_MENU || MPC_AUTOTUNE_MENU

#if DWIN_PID_TUNE
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
  void SetPidCycles() { SetPIntOnClick(3, 50); }
#endif

#if ALL(HAS_PID_HEATING, PID_EDIT_MENU)

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
  void onDrawPIDi(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_i(*(float*)static_cast<MenuItemPtrClass*>(menuitem)->value)); }
  void onDrawPIDd(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_d(*(float*)static_cast<MenuItemPtrClass*>(menuitem)->value)); }

#endif // HAS_PID_HEATING && PID_EDIT_MENU

#if ENABLED(PIDTEMP) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void HotendPID() { SetPID(HMI_data.HotendPidT, H_E0); }
    void SetHotendPidT() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif

  void Draw_HotendPID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(HotendPIDMenu, STR_HOTEND_PID " Settings", 8)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID, onDrawMenuItem, HotendPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, SetHotendPidT, &HMI_data.HotendPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, SetPidCycles, &HMI_data.PidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, SetKp, &thermalManager.temp_hotend[0].pid.Kp);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, SetKi, &thermalManager.temp_hotend[0].pid.Ki);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, SetKd, &thermalManager.temp_hotend[0].pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(HotendPIDMenu);
  }

#endif // PIDTEMP && (PID_AUTOTUNE_MENU || PID_EDIT_MENU)

#if ENABLED(PIDTEMPBED) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void BedPID() { SetPID(HMI_data.BedPidT, H_BED); }
    void SetBedPidT() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  void Draw_BedPID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(BedPIDMenu, STR_BED_PID " Settings", 8)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM_F(ICON_PIDBed, STR_BED_PID, onDrawMenuItem,BedPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, SetBedPidT, &HMI_data.BedPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, SetPidCycles, &HMI_data.PidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, SetKp, &thermalManager.temp_bed.pid.Kp);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, SetKi, &thermalManager.temp_bed.pid.Ki);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, SetKd, &thermalManager.temp_bed.pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(BedPIDMenu);
  }

#endif // PIDTEMPBED && (PID_AUTOTUNE_MENU || PID_EDIT_MENU)

//=============================================================================

#if HAS_BED_PROBE

  void Draw_ZOffsetWiz_Menu() {
    checkkey = Menu;
    if (SET_MENU(ZOffsetWizMenu, MSG_PROBE_WIZARD, 4)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      MENU_ITEM_F(ICON_MoveZ0, "Move Z to Home", onDrawMenuItem, SetMoveZto0);
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
    }
    UpdateMenu(ZOffsetWizMenu);
    if (!axis_is_trusted(Z_AXIS)) LCD_MESSAGE_F("WARNING: Z position unknown, move Z to home");
  }

#endif

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)

  void Draw_Homing_Menu() {
    checkkey = Menu;
    if (SET_MENU(HomingMenu, MSG_HOMING, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      #if HAS_X_AXIS
        MENU_ITEM(ICON_HomeX, MSG_AUTO_HOME_X, onDrawMenuItem, HomeX);
      #endif
      #if HAS_Y_AXIS
        MENU_ITEM(ICON_HomeY, MSG_AUTO_HOME_Y, onDrawMenuItem, HomeY);
      #endif
      #if HAS_Z_AXIS
        MENU_ITEM(ICON_HomeZ, MSG_AUTO_HOME_Z, onDrawMenuItem, HomeZ);
      #endif
      #if ENABLED(MESH_BED_LEVELING)
        EDIT_ITEM(ICON_ZAfterHome, MSG_Z_AFTER_HOME, onDrawPInt8Menu, SetZAfterHoming, &HMI_data.z_after_homing);
      #endif
    }
    UpdateMenu(HomingMenu);
  }

#endif // INDIVIDUAL_AXIS_HOMING_SUBMENU

#if ENABLED(FWRETRACT)

  void Draw_FWRetract_Menu() {
    checkkey = Menu;
    if (SET_MENU(FWRetractMenu, MSG_FWRETRACT, 6)) {
      BACK_ITEM(Return_FWRetract_Menu);
      EDIT_ITEM(ICON_FWRetLength, MSG_CONTROL_RETRACT, onDrawPFloatMenu, SetRetractLength, &fwretract.settings.retract_length);
      EDIT_ITEM(ICON_FWRetSpeed, MSG_SINGLENOZZLE_RETRACT_SPEED, onDrawPFloatMenu, SetRetractSpeed, &fwretract.settings.retract_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRetZRaise, MSG_CONTROL_RETRACT_ZHOP, onDrawPFloat2Menu, SetZRaise, &fwretract.settings.retract_zraise);
      EDIT_ITEM(ICON_FWRecSpeed, MSG_SINGLENOZZLE_UNRETRACT_SPEED, onDrawPFloatMenu, SetRecoverSpeed, &fwretract.settings.retract_recover_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRecExtra, MSG_CONTROL_RETRACT_RECOVER, onDrawPFloatMenu, SetAddRecover, &fwretract.settings.retract_recover_extra);
    }
    UpdateMenu(FWRetractMenu);
  }

#endif

//=============================================================================
// Mesh Bed Leveling
//=============================================================================

#if HAS_MESH

  void ApplyMeshFadeHeight() { set_z_fade_height(planner.z_fade_height); }
  void SetMeshFadeHeight() { SetPFloatOnClick(0, 100, 1, ApplyMeshFadeHeight); }

  void SetMeshActive() {
    set_bed_leveling_enabled(!planner.leveling_active);
    Draw_Chkb_Line(CurrentMenu->line(), planner.leveling_active);
    dwinUpdateLCD();
  }

  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    void SetBedLevT() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  #if ENABLED(MESH_EDIT_MENU)
    #define Z_OFFSET_MIN -3
    #define Z_OFFSET_MAX  3

    void LiveEditMesh() { ((MenuItemPtrClass*)EditZValueItem)->value = &bedlevel.z_values[HMI_value.Select ? bedLevelTools.mesh_x : MenuData.Value][HMI_value.Select ? MenuData.Value : bedLevelTools.mesh_y]; EditZValueItem->redraw(); }
    void ApplyEditMeshX() { bedLevelTools.mesh_x = MenuData.Value; }
    void ApplyEditMeshY() { bedLevelTools.mesh_y = MenuData.Value; }
    void ResetMesh() { bedLevelTools.mesh_reset(); LCD_MESSAGE(MSG_MESH_RESET); }
    void SetEditMeshX() { HMI_value.Select = 0; SetIntOnClick(0, GRID_MAX_POINTS_X - 1, bedLevelTools.mesh_x, ApplyEditMeshX, LiveEditMesh); }
    void SetEditMeshY() { HMI_value.Select = 1; SetIntOnClick(0, GRID_MAX_POINTS_Y - 1, bedLevelTools.mesh_y, ApplyEditMeshY, LiveEditMesh); }
    void SetEditZValue() { SetPFloatOnClick(Z_OFFSET_MIN, Z_OFFSET_MAX, 3); }
  #endif

#endif // HAS_MESH

#if ENABLED(AUTO_BED_LEVELING_UBL)

  void ApplyUBLSlot() { bedlevel.storage_slot = MenuData.Value; }
  void SetUBLSlot() { SetIntOnClick(0, settings.calc_num_meshes() - 1, bedlevel.storage_slot, ApplyUBLSlot); }
  void onDrawUBLSlot(MenuItemClass* menuitem, int8_t line) {
    NOLESS(bedlevel.storage_slot, 0);
    onDrawIntMenu(menuitem, line, bedlevel.storage_slot);
  }

  void ApplyUBLTiltGrid() { bedLevelTools.tilt_grid = MenuData.Value; }
  void SetUBLTiltGrid() { SetIntOnClick(1, 3, bedLevelTools.tilt_grid, ApplyUBLTiltGrid); }

  void UBLMeshTilt() {
    NOLESS(bedlevel.storage_slot, 0);
    char buf[9];
    if (bedLevelTools.tilt_grid > 1) {
      sprintf_P(buf, PSTR("G29J%i"), bedLevelTools.tilt_grid);
      gcode.process_subcommands_now(buf);
    }
    else
      gcode.process_subcommands_now(F("G29J"));
    LCD_MESSAGE(MSG_UBL_MESH_TILTED);
  }

  void UBLSmartFillMesh() {
    for (uint8_t x = 0; x < GRID_MAX_POINTS_Y; ++x) bedlevel.smart_fill_mesh();
    LCD_MESSAGE(MSG_UBL_MESH_FILLED);
  }

  void UBLMeshSave() {
    NOLESS(bedlevel.storage_slot, 0);
    settings.store_mesh(bedlevel.storage_slot);
    ui.status_printf(0, GET_TEXT_F(MSG_MESH_SAVED), bedlevel.storage_slot);
    DONE_BUZZ(true);
  }

  void UBLMeshLoad() {
    NOLESS(bedlevel.storage_slot, 0);
    settings.load_mesh(bedlevel.storage_slot);
  }

#endif // AUTO_BED_LEVELING_UBL

#if HAS_MESH

  void Draw_MeshSet_Menu() {
    checkkey = Menu;
    if (SET_MENU(MeshMenu, MSG_MESH_LEVELING, 14)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if ENABLED(PREHEAT_BEFORE_LEVELING)
        EDIT_ITEM(ICON_Temperature, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedLevT, &HMI_data.BedLevT);
      #endif
      EDIT_ITEM(ICON_SetZOffset, MSG_Z_FADE_HEIGHT, onDrawPFloatMenu, SetMeshFadeHeight, &planner.z_fade_height);
      EDIT_ITEM(ICON_UBLActive, MSG_ACTIVATE_MESH, onDrawChkbMenu, SetMeshActive, &planner.leveling_active);
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, AutoLev);
      #endif
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        EDIT_ITEM(ICON_UBLSlot, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, SetUBLSlot, &bedlevel.storage_slot);
        MENU_ITEM(ICON_UBLMeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, UBLMeshSave);
        MENU_ITEM(ICON_UBLMeshLoad, MSG_UBL_LOAD_MESH, onDrawMenuItem, UBLMeshLoad);
        EDIT_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILTING_GRID, onDrawPInt8Menu, SetUBLTiltGrid, &bedLevelTools.tilt_grid);
        MENU_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILT_MESH, onDrawMenuItem, UBLMeshTilt);
        MENU_ITEM(ICON_UBLSmartFill, MSG_UBL_SMART_FILLIN, onDrawMenuItem, UBLSmartFillMesh);
      #endif
      #if ENABLED(MESH_EDIT_MENU)
        MENU_ITEM(ICON_MeshReset, MSG_MESH_RESET, onDrawMenuItem, ResetMesh);
        MENU_ITEM(ICON_MeshEdit, MSG_EDIT_MESH, onDrawSubMenu, Draw_EditMesh_Menu);
      #endif
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);
    }
    UpdateMenu(MeshMenu);
  }

  #if ENABLED(MESH_EDIT_MENU)
    void Draw_EditMesh_Menu() {
      if (!leveling_is_valid()) { LCD_MESSAGE(MSG_UBL_MESH_INVALID); return; }
      set_bed_leveling_enabled(false);
      checkkey = Menu;
      if (SET_MENU(EditMeshMenu, MSG_EDIT_MESH, 4)) {
        bedLevelTools.mesh_x = bedLevelTools.mesh_y = 0;
        BACK_ITEM(Draw_MeshSet_Menu);
        EDIT_ITEM(ICON_MeshEditX, MSG_MESH_X, onDrawPInt8Menu, SetEditMeshX, &bedLevelTools.mesh_x);
        EDIT_ITEM(ICON_MeshEditY, MSG_MESH_Y, onDrawPInt8Menu, SetEditMeshY, &bedLevelTools.mesh_y);
        EditZValueItem = EDIT_ITEM(ICON_MeshEditZ, MSG_MESH_EDIT_Z, onDrawPFloat2Menu, SetEditZValue, &bedlevel.z_values[bedLevelTools.mesh_x][bedLevelTools.mesh_y]);
      }
      UpdateMenu(EditMeshMenu);
    }
  #endif

#endif // HAS_MESH

#endif // DWIN_LCD_PROUI
