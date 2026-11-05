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
 * lcd/extui/sermoon_v1/sermoon_v1_rts.h
 *
 * Creality Sermoon V1 / V1 Pro stock DWIN touchscreen (factory DWIN_SET, screen firmware 1.0.13).
 * Ported from Creality's Marlin 2.0.6.1 lcdAutoUI (Sermoon V1 V1.0.33).
 */

#include "../ui_api.h"

#define RTS_SERIAL LCD_SERIAL

#define RTS_FRAME_H1          0x5A
#define RTS_FRAME_H2          0xA5
#define RTS_CMD_WRITE_VAR     0x82
#define RTS_CMD_READ_VAR      0x83
#define RTS_RX_BUFFER_SIZE    32

#define RTS_UPDATE_INTERVAL   1000  // (ms) Status refresh
#define RTS_BOOT_STEP_MS        30  // (ms) Boot progress bar step
#define RTS_FILE_SLOTS          20  // File list entries on the screen
#define RTS_FILES_PER_PAGE       4
#define RTS_TEXT_LEN            32  // Characters per text field
#define RTS_LANGUAGES            9

// Page change: 0x5A 0x01 + page number, written to the page register
#define RTS_PAGE_BASE         0x5A010000UL

#define RTS_FEED_TEMP          240  // (°C) Filament load / unload temperature
#define RTS_LOAD_LENGTH        180  // (mm) Filament load
#define RTS_LOAD_SPEED           3  // (mm/s)
#define RTS_UNLOAD_PUSH         40  // (mm) Push before pulling the filament out
#define RTS_UNLOAD_PUSH_SPEED    5  // (mm/s)
#define RTS_UNLOAD_PULL         80  // (mm)
#define RTS_UNLOAD_PULL_SPEED   30  // (mm/s)
#define RTS_CALI_STEP        0.05f  // (mm) Bed calibration Z step
#define RTS_APP_PAUSE_LIFT       5  // (mm) Z lift when a Creality Cloud job pauses
#define RTS_NET_RESET_TIME      90  // (s) Network reset countdown

//
// Variable (VP) addresses
//
#define VP_PAGE               0x0084

// Keys, one VP per button
#define VP_KEY_MAIN_FILES     0x2004
#define VP_KEY_MAIN_MODE      0x2005
#define VP_KEY_MAIN_SETTINGS  0x2006
#define VP_KEY_MAIN_INFO      0x2007
#define VP_KEY_RETURN         0x2008  // Value = which page's return button
#define VP_KEY_FILE_SELECT    0x2009  // Value = file 0-19
#define VP_KEY_FILE_NEXT      0x200A
#define VP_KEY_FILE_PREV      0x200B
#define VP_KEY_PRINT_ADJUST   0x200C
#define VP_KEY_PRINT_PAUSE    0x200D
#define VP_KEY_PRINT_STOP     0x200E
#define VP_KEY_PRINT_DONE     0x200F
#define VP_KEY_BOX_FAN        0x2010
#define VP_KEY_BOX_LED        0x2011
#define VP_KEY_WIFI_LED       0x2012
#define VP_KEY_STOP_YES       0x2014
#define VP_KEY_STOP_NO        0x2015
#define VP_KEY_PAUSE_RESUME   0x2016
#define VP_KEY_PAUSE_STOP     0x2017
#define VP_KEY_PAUSE_FILAMENT 0x2018
#define VP_KEY_MODE_PLA       0x201B
#define VP_KEY_MODE_ABS       0x201C
#define VP_KEY_MODE_COOL      0x201E
#define VP_KEY_MODE_FILAMENT  0x201F
#define VP_KEY_LOAD           0x2021
#define VP_KEY_UNLOAD         0x2022
#define VP_KEY_UNLOAD_DONE    0x2023
#define VP_KEY_CALIBRATE      0x202A
#define VP_KEY_MOVE           0x202B
#define VP_KEY_CALI_UP        0x202C
#define VP_KEY_CALI_DOWN      0x202D
#define VP_KEY_CALI_NEXT      0x202E
#define VP_KEY_STEP_1MM       0x202F
#define VP_KEY_STEP_01MM      0x2030
#define VP_KEY_X_MINUS        0x2031
#define VP_KEY_X_PLUS         0x2032
#define VP_KEY_Y_PLUS         0x2033
#define VP_KEY_Y_MINUS        0x2034
#define VP_KEY_Z_MINUS        0x2035
#define VP_KEY_Z_PLUS         0x2036
#define VP_KEY_HOME           0x2037
#define VP_KEY_STEP_10MM      0x2038
#define VP_KEY_INFO_LANGUAGE  0x2039
#define VP_KEY_INFO_RESET     0x203A
#define VP_KEY_NET_RESET      0x203B
#define VP_KEY_NET_RESET_YES  0x203C
#define VP_KEY_NET_RESET_NO   0x203E
#define VP_KEY_LANGUAGE       0x2040  // Value = language 0-8
#define VP_KEY_RESET_YES      0x2041
#define VP_KEY_RESET_NO       0x2042
#define VP_KEY_RECOVER_YES    0x2044
#define VP_KEY_RECOVER_NO     0x2045
#define VP_KEY_START_PRINT    0x2046
#define VP_KEY_RUNOUT_LOAD    0x2047
#define VP_KEY_RUNOUT_STOP    0x2048
#define VP_KEY_LOADED_RESUME  0x2049
#define VP_KEY_LOADED_STOP    0x204A
#define VP_KEY_COOL_YES       0x204C
#define VP_KEY_COOL_NO        0x204D
#define VP_KEY_FEEDRATE       0x2050  // Keypad
#define VP_KEY_NOZZLE_TARGET  0x2054  // Keypad
#define VP_KEY_BED_TARGET     0x2058  // Keypad
#define VP_KEY_WIFI_RESET_OK  0x205C
#define VP_KEY_CALI_YES       0x205D
#define VP_KEY_CALI_NO        0x205E
#define VP_KEY_LOAD_START     0x2002  // Page 5, the screen changes to page 6 itself
#define VP_KEY_LOAD_DONE      0x2003

// Icons
#define VP_BOOT_PROGRESS      0x1000
#define VP_LANGUAGE           0x100A  // Language 0-8, on every page
#define VP_FILE_ICON          0x100E  // + file index, 1 = selected
#define VP_PROGRESS_ICON      0x1024
#define VP_BOX_FAN_ICON       0x102E
#define VP_BOX_LED_ICON       0x1030
#define VP_WIFI_LED_ICON      0x1032
#define VP_WIFI_LINK_ICON     0x1033  // Main page corner: WiFi on and board linked
#define VP_PLA_ICON           0x1042
#define VP_ABS_ICON           0x1044
#define VP_CALI_POINT_ICON    0x1065
#define VP_HEATING_NOZZLE     0x1080  // Animation, 1 = run
#define VP_HEATING_BED        0x1082  // Animation
#define VP_LOADING            0x1084  // Animation
#define VP_UNLOAD_RETURN      0x1086  // 1 = show the return key on page 22
#define VP_LANGUAGE_ICON      0x1087  // + language, 1 = selected

// Values
#define VP_NOZZLE_TEMP        0x3000
#define VP_NOZZLE_TARGET      0x3002
#define VP_BED_TEMP           0x3004
#define VP_BED_TARGET         0x3006
#define VP_TOTAL_TIME         0x3008  // 32-bit, hours x 100
#define VP_PERCENTAGE         0x300D
#define VP_TIME_HOUR          0x300F
#define VP_TIME_MIN           0x3011
#define VP_CALI_ZOFFSET       0x3017  // x 100
#define VP_CALI_ZHEIGHT       0x301B  // x 100
#define VP_AXIS_X             0x301F  // x 10, also a keypad
#define VP_AXIS_Y             0x3023
#define VP_AXIS_Z             0x3027
#define VP_FEEDRATE           0x302B
#define VP_NET_RESET_TIME     0x3039  // Countdown in seconds

// Text, 32 characters each
#define VP_FILE_NAMES         0x4020  // + 0x20 per file
#define VP_PRINT_FILENAME     0x42B0
#define VP_ERROR_TEXT         0x42D0
#define VP_RECOVERY_FILENAME  0x42F0
#define VP_FW_VERSION         0x4330
#define VP_WIFI_MAC           0x4370
#define VP_PRINTER_SIZE       0x4390
#define VP_MACHINE_NAME       0x43B0

#define TEXT_WORDS            (RTS_TEXT_LEN / 2)

class RTS {
  public:
    enum page_t : uint8_t {
      PAGE_BOOT          =  0,
      PAGE_LOAD          =  5,  // Insert filament
      PAGE_LOADING       =  6,
      PAGE_MAIN          =  8,
      PAGE_FILES         =  9,  // to 13, four files each
      PAGE_PRINTING      = 14,
      PAGE_PRINT_DONE    = 15,
      PAGE_ADJUST        = 16,
      PAGE_STOP_CONFIRM  = 17,
      PAGE_PAUSED        = 18,
      PAGE_MODE          = 20,
      PAGE_FILAMENT      = 21,
      PAGE_UNLOAD_HEAT   = 22,
      PAGE_UNLOAD_PULL   = 23,
      PAGE_SETTINGS      = 25,
      PAGE_BUSY          = 26,
      PAGE_CALIBRATE     = 27,
      PAGE_MOVE          = 28,  // + 0/1/2 for 10/1/0.1mm
      PAGE_INFO          = 31,
      PAGE_LANGUAGE      = 32,
      PAGE_RESET_CONFIRM = 33,
      PAGE_ERROR         = 34,
      PAGE_RECOVERY      = 36,
      PAGE_NO_FILAMENT   = 39,
      PAGE_LOADED        = 40,
      PAGE_COOL_CONFIRM  = 42,
      PAGE_HEATING       = 43,
      PAGE_WIFI_RESET    = 44,
      PAGE_CALI_CONFIRM  = 45,
      PAGE_NET_CONFIRM   = 47,
      PAGE_NET_RESET     = 48
    };

    // Persistent screen settings, kept in the ExtUI EEPROM block
    struct settings_t {
      uint8_t version;
      uint8_t language;   // 0-8
      bool    wifi_led;
    };
    static settings_t settings;

    static void onStartup();
    static void onIdle();

    static void gotoPage(const uint8_t page);
    static void writeWord(const uint16_t vp, const uint16_t value);
    static void writeLong(const uint16_t vp, const uint32_t value);
    static void writeText(const uint16_t vp, const char *str);
    static void clearWords(const uint16_t vp, const uint8_t words);

    static void sendLanguage();
    static void sendPosition();
    static void refreshFileList();
    static void clearFileList();
    static void clearPrintInfo();
    static void showError(FSTR_P const msg);

    static void printStarted();
    static void printPaused();
    static void printStopped();
    static void printFinished();
    static void homingFinished();
    static void powerLossDetected();
    static void mediaInserted();
    static void mediaRemoved();
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      static void pauseMessage(const PauseMessage message);
    #endif
    static void filamentRunout();

    // Creality Cloud WiFi board, via M79 and M72
    static void wifiCommand(const char code, const char *arg);
    static void cloudJobName(const char *name);
    static bool recoveryPrompt();

  private:
    static void receive();
    static void handle(const uint16_t vp, const uint16_t value);
    static void update();
    static void bootAnimation();

    static void showTextField(const uint16_t vp, const char *str);
    static void showPrintPage();
    static void showFilamentReturn();
    static bool filamentMissing();
    static void saveSettings();
    static void startSelectedFile();
    static void resumeJob();
    static void selectFile(const uint8_t n);
    static void jog(const AxisEnum axis, const int8_t dir);
    static void startCalibration();
    static void calibrationNext();
    static void endCalibration();
    static void startLoad();
    static void startUnload();
    static void finishFilament();
    static void onReturn(const uint16_t value);
    static void appStart();
    static void appStop();
};

extern RTS rts;
