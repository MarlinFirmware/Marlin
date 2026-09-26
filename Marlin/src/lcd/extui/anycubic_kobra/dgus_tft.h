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
#pragma once

/**
 * lcd/extui/lib/dgus_tft.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "../../../inc/MarlinConfigPre.h"
#include "../ui_api.h"
#include "dgus_tft_defs.h"

#define DEVICE_NAME "Anycubic Kobra Max"
#define FIRMWARE_VER "Kobra Max V3.1"
#define BUILD_VOLUME "406*406*450 (mm)"
#define TECH_SUPPORT "https://www.anycubic.com"

/****************** PAGE INDEX***********************/
#define PAGE_OFFSET 0
#define PAGE_MAIN (121 + PAGE_OFFSET)
#define PAGE_FILE (122 + PAGE_OFFSET)
#define PAGE_STATUS1 (123 + PAGE_OFFSET)  // show resume
#define PAGE_STATUS2 (124 + PAGE_OFFSET)  // show pause
#define PAGE_ADJUST (125 + PAGE_OFFSET)
#define PAGE_KEYBOARD (6 + PAGE_OFFSET)
#define PAGE_TOOL (127 + PAGE_OFFSET)
#define PAGE_MOVE (128 + PAGE_OFFSET)
#define PAGE_TEMP (129 + PAGE_OFFSET)
#define PAGE_SPEED (130 + PAGE_OFFSET)
#define PAGE_ABOUT (133 + PAGE_OFFSET)
#define PAGE_RECORD (134 + PAGE_OFFSET)
#define PAGE_PREPARE (135 + PAGE_OFFSET)
#define PAGE_LEVELING_SETTINGS (136 + PAGE_OFFSET)
#define PAGE_Z_OFFSET (137 + PAGE_OFFSET)
#define PAGE_PREHEAT (138 + PAGE_OFFSET)
#define PAGE_FILAMENT (139 + PAGE_OFFSET)

#define PAGE_DONE (140 + PAGE_OFFSET)
#define PAGE_ABNORMAL (141 + PAGE_OFFSET)
#define PAGE_PRINT_FINISH (142 + PAGE_OFFSET)
#define PAGE_WAIT_STOP (143 + PAGE_OFFSET)
#define PAGE_FILAMENT_LACK (145 + PAGE_OFFSET)
#define PAGE_FORBIT (146 + PAGE_OFFSET)
#define PAGE_STOP_CONF (147 + PAGE_OFFSET)
#define PAGE_NO_SD (149 + PAGE_OFFSET)
#define PAGE_FILAMENT_HEAT (150 + PAGE_OFFSET)
#define PAGE_WAIT_PAUSE (152 + PAGE_OFFSET)

#define PAGE_LEVEL_ENSURE (153 + PAGE_OFFSET)
#define PAGE_LEVELING (154 + PAGE_OFFSET)

#define PAGE_PRINTERSTATS (213 + PAGE_OFFSET)

#define PAGE_AUTO_OFFSET (115 + PAGE_OFFSET)
#define PAGE_SYSTEM_AUDIO_ON (131 + PAGE_OFFSET)
#define PAGE_SYSTEM_AUDIO_OFF (170 + PAGE_OFFSET)

#define PAGE_OUTAGE_RECOVERY (173 + PAGE_OFFSET)
#define PAGE_PROBE_PREHEATING (175 + PAGE_OFFSET)

#define PAGE_HOMING (189 + PAGE_OFFSET)
#define PAGE_ABNORMAL_BED_HEATER (190 + PAGE_OFFSET)
#define PAGE_ABNORMAL_BED_NTC (191 + PAGE_OFFSET)
#define PAGE_ABNORMAL_HOTEND_HEATER (192 + PAGE_OFFSET)
#define PAGE_ABNORMAL_HOTEND_NTC (193 + PAGE_OFFSET)
#define PAGE_ABNORMAL_ENDSTOP (194 + PAGE_OFFSET)
#define PAGE_ABNORMAL_X_ENDSTOP (194 + PAGE_OFFSET)
#define PAGE_ABNORMAL_Y_ENDSTOP (195 + PAGE_OFFSET)
#define PAGE_ABNORMAL_Z_ENDSTOP (196 + PAGE_OFFSET)
#define PAGE_ABNORMAL_ZL_ENDSTOP (197 + PAGE_OFFSET)
#define PAGE_ABNORMAL_ZR_ENDSTOP (198 + PAGE_OFFSET)
#define PAGE_ABNORMAL_LEVELING_SENSOR (199 + PAGE_OFFSET)
#define PAGE_LEVELING_FAILED (200 + PAGE_OFFSET)

#define PAGE_PROBE_PRECHECK (204 + PAGE_OFFSET)
#define PAGE_PROBE_PRECHECK_OK (205 + PAGE_OFFSET)
#define PAGE_PROBE_PRECHECK_FAILED (206 + PAGE_OFFSET)

#define PAGE_TOOL_CASELIGHT (209 + PAGE_OFFSET)

#define PAGE_PRINTING_SETTING (212 + PAGE_OFFSET)

/****************** PopUps **************************/
#define POPUP_T0_ERROR 10
#define POPUP_FILAMENT_LACK 15
#define POPUP_STOP_WAIT 16
#define POPUP_FILAMENT_LACKING 23
#define POPUP_PRINT_FINISH 24
#define POPUP_LEVELING_DONE 25
#define POPUP_NONE 100

/****************** Lcd control **************************/
#define REG_LCD_READY 0x0014

/****************** TXT **************************/

// MIAN PAGE TXT
#define TXT_MAIN_BED 0x2000
#define TXT_MAIN_HOTEND 0x2030
#define TXT_MAIN_MESSAGE 0x2060

// FILE TXT

#define TXT_FILE_0 (0x2000 + 3 * 0x30)
#define TXT_DESCRIPT_0 0x5000  // DESCRIPT ADDRESS
#define TXT_FILE_1 (0x2000 + 4 * 0x30)
#define TXT_DESCRIPT_1 0x5030
#define TXT_FILE_2 (0x2000 + 5 * 0x30)
#define TXT_DESCRIPT_2 0x5060
#define TXT_FILE_3 (0x2000 + 6 * 0x30)
#define TXT_DESCRIPT_3 0x5090
#define TXT_FILE_4 (0x2000 + 7 * 0x30)
#define TXT_DESCRIPT_4 0x50C0

// PRINT TXT
#define TXT_PRINT_NAME 0x2000 + 8 * 0x30
#define TXT_PRINT_SPEED 0x2000 + 9 * 0x30
#define TXT_PRINT_TIME 0x2000 + 10 * 0x30
#define TXT_PRINT_PROGRESS 0x2000 + 11 * 0x30
#define TXT_PRINT_COMMENT 0x2000 + 12 * 0x30  // MEL_MOD malebuffy

// PRINT ADJUST TXT

#define TXT_ADJUST_HOTEND (0x2000 + 14 * 0x30)
#define TXT_ADJUST_BED (0x2000 + 15 * 0x30)
#define TXT_ADJUST_SPEED (0x2000 + 16 * 0x30)

// TEMP SET TXT

#define TXT_BED_NOW (0x2000 + 17 * 0x30)
#define TXT_BED_TARGET (0x2000 + 18 * 0x30)
#define TXT_HOTNED_NOW (0x2000 + 19 * 0x30)
#define TXT_HOTEND_TARGET (0x2000 + 20 * 0x30)

// SPEED SET TXT
#define TXT_FAN_SPEED_NOW (0x2000 + 21 * 0x30)
#define TXT_FAN_SPEED_TARGET (0x2000 + 22 * 0x30)
#define TXT_PRINT_SPEED_NOW (0x2000 + 23 * 0x30)
#define TXT_PRINT_SPEED_TARGET (0x2000 + 24 * 0x30)

// ABOUT TXT
#define TXT_ABOUT (0x2000 + 25 * 0x30)

// RECORT TXT
#define TXT_RECORT_0 (0x2000 + 26 * 0x30)
#define TXT_RECORT_1 (0x2000 + 27 * 0x30)
#define TXT_RECORT_2 (0x2000 + 28 * 0x30)
#define TXT_RECORT_3 (0x2000 + 29 * 0x30)
#define TXT_RECORT_4 (0x2000 + 30 * 0x30)
#define TXT_RECORT_5 (0x2000 + 31 * 0x30)

// ADVANCE LEVEL  TXT
#define TXT_LEVEL_OFFSET (0x2000 + 32 * 0x30)

// FILAMENT TXT
#define TXT_FILAMENT_TEMP (0x2000 + 33 * 0x30)

#define TXT_FINISH_TIME (0x2000 + 34 * 0x30)
#define TXT_VERSION (0x2000 + 35 * 0x30)
#define TXT_PREHEAT_HOTEND (0x2000 + 36 * 0x30)
#define TXT_PREHEAT_BED (0x2000 + 37 * 0x30)

#define TXT_PREHEAT_HOTEND_INPUT 0x3000
#define TXT_PREHEAT_BED_INPUT 0x3002

#define TXT_OUTAGE_RECOVERY_PROGRESS 0x2210
#define TXT_OUTAGE_RECOVERY_FILE 0x2180

#define ADDRESS_SYSTEM_AUDIO 0x0080

#define ADDRESS_MOVE_DISTANCE 0x4300
#define ADDRESS_SYSTEM_LED_STATUS 0x4500
#define ADDRESS_PRINT_SETTING_LED_STATUS 0x4550

#define TXT_ABOUT_DEVICE_NAME 0x2750
#define TXT_ABOUT_FW_VERSION 0x2690
#define TXT_ABOUT_PRINT_VOLUMN 0x2770
#define TXT_ABOUT_TECH_SUPPORT 0x2790

#define TXT_STATS_PRINTS_TOTAL 0x2690
#define TXT_STATS_PRINTS_FINISHED 0x2750
#define TXT_STATS_PRINTS_FAILED 0x2770
#define TXT_STATS_PRINTS_TIME 0x2790
#define TXT_STATS_PRINTS_LONGEST 0x2810
#define TXT_STATS_PRINTS_FILAMENT 0x2830

/*********************** KEY VALUE **************************/
#define KEY_ADDRESS 0x1000

// MAIN PAGE KEY

#define KEY_MAIN_TO_FILE 1
#define KEY_MAIN_TO_TOOL 2
#define KEY_MAIN_TO_PREPARE 3
#define KEY_MAIN_TO_SYSTEM 4

// FILE PAGE KEY

#define KEY_FILE_TO_MAIN 1
#define KEY_PRINT 6
#define KEY_RESUME 5
#define KEY_PgUp 2
#define KEY_pgDn 3
#define KEY_FLASH 4
#define KEY_FILE0 7
#define KEY_FILE1 8
#define KEY_FILE2 9
#define KEY_FILE3 10
#define KEY_FILE4 11

#define KEY_CONTINUE 2
#define KEY_PAUSE 2
#define KEY_STOP 3
#define KEY_TO_ADJUST 4
#define KEY_ADJUST_TO_PRINT 1
#define KEY_ADJUST_ENSURE 7
#define KEY_CHECK_DOOR 2
#define KEY_DONE_OFF 3

// TOOL PAGE KEY

#define KEY_TOOL_TO_MAIN 1
#define KEY_TOOL_TO_MOVE 2
#define KEY_TOOL_TO_TEMP 3
#define KEY_TOOL_TO_SPEED 4
#define KEY_TOOL_LIGHT 5

#define KEY_MOVE_TO_TOLL 1  // move page
#define KEY_MOVE_X 2
#define KEY_01 3
#define KEY_MOVE_NX 4
#define KEY_HOME_X 5
#define KEY_MOVE_Y 6
#define KEY_1 7
#define KEY_MOVE_NY 8
#define KEY_HOME_Y 9
#define KEY_MOVE_Z 10
#define KEY_10 11
#define KEY_MOVE_NZ 12
#define KEY_HOME_Z 13
#define KEY_SPEED_LOW 14
#define KEY_SPEED_MIDDLE 15
#define KEY_SPEED_HIGHT 16
#define KEY_HOME_ALL 17

#define KEY_TEMP_TO_TOOL 1  // Temperature setting page
#define KEY_BED_ADD 2
#define KEY_BED_DEC 3
#define KEY_HOTEND_ADD 4
#define KEY_HOTEND_DEC 5
#define KEY_COOL 6
#define KEY_TEMP_ENSURE 7

#define KEY_SPEED_TO_TOOL 1  // speed setting page
#define KEY_FAN_SPEED_ADD 2
#define KEY_FAN_SPEED_DEC 3
#define KEY_PRINT_SPEED_ADD 4
#define KEY_PRINT_SPEED_DEC 5
#define KEY_SPEED_ENSURE 6

#define KEY_PREPARE_TO_MAIN 1  // PREPARE PAGE TO MAIN
#define KEY_PREPARE_TO_PreLEVE 2
#define KEY_PreLEVE_TO_PREPARE 1
#define KEY_PreLEVE_TO_LEVELING 2
#define KEY_PreLEVE_TO_ADVANCE 3
#define KEY_ADVANCE_TO_PreLEVE 1
#define KEY_LEVEL_ADD 3
#define KEY_LEVEL_DEC 2
#define KEY_LEVEL_ENSURE 4

#define KEY_PREPARE_TO_PREHEAT 3
#define KEY_PREHEAT_TO_PREPARE 1
#define KEY_PREHEAT_PLA 2
#define KEY_PREHEAT_ABS 3

#define KEY_PREPARE_TO_FILAMENT 4
#define KEY_FILAMENT_TO_PREPARE 1
#define KEY_RETREAT 3
#define KEY_FORWARD 2
#define KEY_FILAMENT_STOP 4

// SYSTEM PAGE KEY

#define KEY_SYS_TO_MAIN 1
#define KEY_LANGUAGE 2
#define KEY_SYS_TO_WIFI 3
#define KEY_WIFI_TO_SYS 1

#define KEY_BEEP 4
#define KEY_SYS_TO_ABOUT 5
#define KEY_ABOUT_TO_SYS 1

#define KEY_SYS_TO_RECORD 6
#define KEY_RECORD_TO_SYS 1
#define KEY_RECORD_PaUp 2
#define KEY_RECORD_PaDn 3
#define KEY_RECORD_FLASH 4

#define COLOR_RED 0xf800
#define COLOR_BLUE 0x0210
#define COLOR_WHITE 0xffff

#define DATA_BUF_SIZE 128

namespace ExtUI {
  enum language_t : uint8_t { ENG };
  enum audio_t : uint8_t { ON, OFF };
}  // namespace ExtUI

namespace Anycubic {

  typedef struct _lcd_info_t {
    ExtUI::language_t language;
    ExtUI::audio_t audio;
  } lcd_info_t;

  class DgusTFT {
   public:
    DgusTFT();

    static lcd_info_t lcd_info;
    static lcd_info_t lcd_info_back;
    static uint32_t page_index_now;

    static void Startup();

    static void IdleLoop();

    static void PrinterKilled(FSTR_P const error, FSTR_P const component);

    static void SetDescriptColor(const uint16_t color,
                                 const uint8_t index = lcd_txtbox_index);

    static void MediaEvent(media_event_t);

    static void TimerEvent(timer_event_t);

    static void FilamentRunout();

    static void ConfirmationRequest(const char *const);

    static void StatusChange(const char *const);

    static void PowerLoss();

    static void PowerLossRecovery();

    static void HomingStart();

    static void HomingComplete();

    static void LevelingStart();

    static void LevelingDone();

    static void MeshUpdate(const int8_t xpos, const int8_t ypos,
                           const ExtUI::probe_state_t state);

    typedef void (*p_fun)(void);

    static void page6_handle(void) {}

    static void page121_handle(void);

    static void page122_handle(void);

    static void Page123_124_Return();

    static void Page123_124_Stop();

    static void Page123_124_Settings(uint16_t &feedrate_last);

    static void Page123_124_End(millis_t &flash_time, uint8_t &progress_last,
                                uint16_t &feedrate_last);

    static void page123_handle(void);

    static void page124_handle(void);

    static void page125_SaveAndBack(bool z_change);
    static void page125_handle(void);

    static void page126_handle(void);

    static void page127_handle(void);  // tool
    static void page128_handle(void);

    static void page129_handle(void);

    static void page130_handle(void);  // fan and print speed
    static void page131_handle(void);  // system

    static void page133_handle(void);

    static void page135_handle(void);

    static void page136_handle(void);

    static void page137_handle(void);

    static void page138_handle(void);

    static void page139_handle(void);

    static void page140_handle(void);

    static void page141_handle(void);

    static void page142_handle(void);

    static void page143_handle(void);

    static void page144_handle(void);

    static void page145_handle(void);

    static void page146_handle(void);

    static void page147_handle(void);

    static void page148_handle(void);

    static void page149_handle(void);

    static void page150_handle(void);

    static void page151_handle(void);

    static void page152_handle(void);

    static void page153_handle(void);

    static void page170_handle(void);
    static void page175_176_handle(void);

    static void page199_to_200_handle(void);

    static void page204_handle(void);

    static void page205_handle(void);

    static void page206_handle(void);

    static void page207_209_handle(void);

    static void page211_212_handle(void);

    static void page213_handle(void);  // MEL_MOD printer stats page (213)
    static void pop_up_manager(void);

    static void printerStatsToTFT(void);  // MEL_MOD

    static void SendtoTFT(PGM_P);

    static void SendtoTFTLN(PGM_P);

    static bool ReadTFTCommand();

    static int8_t Findcmndpos(const uint8_t *, uint8_t);

    static void CheckHeaters();

    static void SendFileList(int8_t);

    static void SelectFile();

    static void ProcessPanelRequest();

    static void PanelInfo(uint8_t);

    static void PanelAction(uint8_t);

    static void PanelProcess(uint8_t);

    static void SendValueToTFT(uint32_t value, uint32_t address);

    static void RequestValueFromTFT(uint32_t address);

    static void SendTxtToTFT(const char *pdata, uint32_t address);

    static void SendColorToTFT(uint32_t color, uint32_t address);

    static void SendReadNumOfTxtToTFT(uint8_t number, uint32_t address);

    static void ChangePageOfTFT(uint32_t page_index, bool no_send = false);

    static void FakeChangePageOfTFT(uint32_t page_index);

    static void LcdAudioSet(ExtUI::audio_t audio);

   private:
    static printer_state_t printer_state;
    static paused_state_t pause_state;
    static heater_state_t hotend_state;
    static heater_state_t hotbed_state;
    static xy_uint8_t selectedmeshpoint;
    static char panel_command[MAX_CMND_LEN];
    static uint8_t command_len;
    static char selectedfile[MAX_PATH_LEN];
    static float live_Zoffset;
    static file_menu_t file_menu;
    static bool data_received;
    static uint8_t data_buf[DATA_BUF_SIZE];
    static uint8_t data_index;
    static uint32_t page_index_last;
    static uint32_t page_index_last_2;
    static uint8_t message_index;
    static uint8_t pop_up_index;
    static uint32_t key_index;
    static uint32_t key_value;
    static uint16_t filenumber;
    static uint16_t filepage;
    static uint8_t lcd_txtbox_index;
    static uint8_t lcd_txtbox_page;
    static uint16_t change_color_index;
    static uint8_t TFTpausingFlag;
    static uint8_t TFTStatusFlag;
    static uint8_t TFTresumingflag;
    static uint8_t ready;
    static int16_t home_owner_page;
    static bool is_auto_leveling;
  };

  extern DgusTFT Dgus;

}  // namespace Anycubic
