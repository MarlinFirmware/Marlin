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
 * lcd/extui/sermoon_d1/sermoon_d1_rts.h
 *
 * Creality Sermoon D1 stock DWIN touchscreen (factory DWIN_SET).
 * Ported from Creality's Marlin 2.0.1 LCD_RTS (Sermoon D1 V1.1.16, screen DWIN 1.1.14).
 */

#include "../ui_api.h"

#define RTS_SERIAL LCD_SERIAL

#define RTS_FRAME_H1          0x5A
#define RTS_FRAME_H2          0xA5
#define RTS_CMD_WRITE_VAR     0x82
#define RTS_CMD_READ_VAR      0x83
#define RTS_RX_BUFFER_SIZE    32

#define RTS_UPDATE_INTERVAL   1000  // (ms) Status refresh
#define RTS_BOOT_STEP_MS        12  // (ms) Boot progress bar step
#define RTS_FILE_SLOTS          20  // File list entries on the screen
#define RTS_FILENAME_LEN        18  // Characters per filename field
#define RTS_LANGUAGES            9

// The screen shows a 5x5 mesh
#define RTS_MESH_SIZE            5

// Page change: 0x5A 0x01 + page number, written to the page register
#define RTS_PAGE_BASE         0x5A010000UL

//
// Variable (VP) addresses
//
#define VP_PAGE               0x0084

// Keys, one VP per group of pages
#define VP_KEY_MAIN           0x2001
#define VP_KEY_FILES          0x2002
#define VP_KEY_PRINT          0x2007
#define VP_KEY_TEMP           0x200C
#define VP_KEY_PREHEAT        0x200E
#define VP_KEY_MANUAL_TEMP    0x2010
#define VP_KEY_SETTINGS       0x2011
#define VP_KEY_LEVELING       0x2012
#define VP_KEY_FILAMENT       0x2015
#define VP_KEY_MOVE           0x2019
#define VP_KEY_LANGUAGE       0x201D
#define VP_KEY_POWER_LOSS     0x2025
#define VP_KEY_NO_FILAMENT    0x2026
#define VP_KEY_HOME           0x1046

// Values
#define VP_BOOT_PROGRESS      0x1000  // Also the print progress bar
#define VP_STATUS_ICON        0x1300
#define VP_NOZZLE_TARGET      0x1400
#define VP_NOZZLE_TEMP        0x1402
#define VP_BED_TARGET         0x1404
#define VP_BED_TEMP           0x1406
#define VP_PERCENTAGE         0x1408
#define VP_TIME_HOUR          0x140B
#define VP_TIME_MIN           0x140E
#define VP_FEEDRATE           0x1414
#define VP_FILAMENT_LEN       0x1418
#define VP_ZOFFSET            0x2100
#define VP_AXIS_X             0x2112
#define VP_AXIS_Y             0x2114
#define VP_AXIS_Z             0x2116
#define VP_FILAMENT_TEMP      0x2120  // Temperature needed to move filament

// Icons
#define VP_FILE_ICON          0x1200  // + file index, 1 = selected
#define VP_FAN_ICON           0x1220
#define VP_LEVELING_ICON      0x1221  // 2 = off, 3 = on
#define VP_MOTOR_ICON         0x1224
#define VP_LANGUAGE_ICON      0x1225  // + language - 1, 1 = selected
#define VP_PROBE_ICON         0x1120  // Probed points
#define VP_HOMING_ICON        0x1121  // Homing animation 1-9

// Text
#define VP_FILE_NAMES         0x1600  // + 0x14 per file
#define VP_PRINT_FILENAME     0x1790
#define VP_MACHINE_NAME       0x17B0
#define VP_HW_VERSION         0x17C4
#define VP_FW_VERSION         0x17D8
#define VP_SCREEN_VERSION     0x17EC
#define VP_PRINTER_SIZE       0x1800
#define VP_WEBSITE            0x1814
#define VP_ERROR_TEXT         0x1830
#define VP_MESH_VALUES        0x1840  // 25 words, zig-zag by row

#define FILENAME_WORDS        (RTS_FILENAME_LEN / 2 + 1)

class RTS {
  public:
    enum page_t : uint8_t {
      PAGE_BOOT          =  0,
      PAGE_MAIN          =  1,
      PAGE_FILES         =  2,  // to 6, four files each
      PAGE_PRINT_HEAT    =  7,
      PAGE_PRINTING      =  8,
      PAGE_PAUSED        =  9,
      PAGE_PRINT_DONE    = 10,
      PAGE_ADJUST        = 11,
      PAGE_TEMP_FAN_ON   = 12,
      PAGE_TEMP_FAN_OFF  = 13,
      PAGE_SETTINGS      = 17,
      PAGE_LEVELING      = 18,
      PAGE_AUTOLEVEL     = 19,
      PAGE_TRAMMING      = 20,
      PAGE_FILAMENT      = 21,
      PAGE_FIL_COLD      = 22,
      PAGE_FIL_HEATING   = 23,
      PAGE_FIL_STOP_HEAT = 24,
      PAGE_MOVE          = 25,  // + 0/1/2 for 0.1/1/10mm
      PAGE_HOMING        = 28,
      PAGE_LANGUAGE      = 29,
      PAGE_ABOUT         = 30,
      PAGE_STOP_CONFIRM  = 31,
      PAGE_PAUSE_CONFIRM = 32,
      PAGE_RECOVERY      = 37,
      PAGE_NO_FILAMENT   = 38,
      PAGE_FIRST_LANG    = 40,
      PAGE_WAIT          = 43,
      PAGE_ERROR         = 44
    };

    // Status line messages, each drawn in all nine languages
    enum StatusMsg : uint8_t {
      MSG_NO_CARD = 3, MSG_READY, MSG_HEATING, MSG_HEAT_DONE, MSG_PRINTING,
      MSG_PRINT_DONE, MSG_COOLING, MSG_COOL_DONE, MSG_STOPPED
    };

    // Persistent screen settings, kept in the ExtUI EEPROM block
    struct settings_t {
      uint8_t version;
      uint8_t language;   // 1-9, 0 = ask at boot
    };
    static settings_t settings;

    static void onStartup();
    static void onIdle();

    static void gotoPage(const uint8_t page);
    static void setStatus(const StatusMsg msg);
    static void writeWord(const uint16_t vp, const uint16_t value);
    static void writeLong(const uint16_t vp, const uint32_t value);
    static void writeText(const uint16_t vp, const char *str, const uint8_t maxlen=RTS_FILENAME_LEN);
    static void clearWords(const uint16_t vp, const uint8_t words);

    static void sendLanguage();
    static void sendZOffset();
    static void sendMesh();
    static void sendMeshPoint(const int8_t x, const int8_t y, const float z);
    static void sendPosition();
    static void refreshFileList();
    static void clearFileList();
    static void clearPrintInfo();
    static void showError(FSTR_P const msg);

    static void printStarted();
    static void printPaused();
    static void printStopped();
    static void printFinished();
    static void homingStarted();
    static void homingFinished();
    static void levelingFinished();
    static void meshPointProbed(const int8_t x, const int8_t y, const float z);
    static void powerLossDetected();
    static void mediaInserted();
    static void mediaRemoved();
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      static void pauseMessage(const PauseMessage message);
    #endif
    static void filamentRunout();

  private:
    static void receive();
    static void handle(const uint16_t vp, const uint16_t value);
    static void update();
    static void bootAnimation();
    static void checkHeatDone();

    static void showFilename(const uint16_t vp, const char *name);
    static void showTempPage();
    static void showPrintPage();
    static bool filamentMissing();
    static void saveSettings();
    static void setZOffset(const float z);
    static void moveToTrammingPoint(const float x, const float y);
    static void moveFilament(const float mm);
    static void startSelectedFile();
    static void resumeJob();
    static void selectLanguage(const uint8_t lang);

    static void onMainKey(const uint16_t value);
    static void onFilesKey(const uint16_t value);
    static void onPrintKey(const uint16_t value);
    static void onTempKey(const uint16_t value);
    static void onManualTempKey(const uint16_t value);
    static void onSettingsKey(const uint16_t value);
    static void onLevelingKey(const uint16_t value);
    static void onFilamentKey(const uint16_t value);
    static void onMoveKey(const uint16_t value);
    static void onLanguageKey(const uint16_t value);
    static void onPowerLossKey(const uint16_t value);
    static void onNoFilamentKey(const uint16_t value);
    static void onAxis(const uint16_t vp, const uint16_t value);
};

extern RTS rts;
