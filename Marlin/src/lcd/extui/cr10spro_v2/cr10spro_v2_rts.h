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
 * lcd/extui/cr10spro_v2/cr10spro_v2_rts.h
 *
 * Creality CR-10S Pro V2 stock DWIN touchscreen (factory DWIN_SET).
 * Ported from Creality's Marlin 1.1.6 LCD_RTS (CR-10S Pro V2 V1.70.0 BL).
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
#define RTS_FILENAME_LEN        18  // Characters per filename field

// The screen shows a 5x5 mesh
#define RTS_MESH_SIZE            5

// Page change: 0x5A 0x01 + page number, written to the page register
#define RTS_PAGE_BASE         0x5A010000UL
#define RTS_START_SOUND       0x060480A0UL  // Play startup tune

// English status icons follow the Chinese ones
#define RTS_ICON_EN_OFFSET      12

//
// Variable (VP) addresses
//
#define VP_PAGE               0x0084
#define VP_SOUND              0x00A0
#define VP_BOOT_PROGRESS      0x1000
#define VP_PRINT_FILE         0x1002  // Main menu buttons
#define VP_ADJUST             0x1004
#define VP_FEEDRATE           0x1006
#define VP_STOP_PRINT         0x1008
#define VP_PAUSE_PRINT        0x100A
#define VP_RESUME_PRINT       0x100C
#define VP_PROGRESS_BAR       0x100E  // Two words: 0-50%, 50-100%
#define VP_TIME_HOUR          0x1010
#define VP_TIME_MIN           0x1012
#define VP_STATUS_ICON        0x1014
#define VP_ENERGY_ICON        0x1018  // VP_STATUS_ICON + 4
#define VP_PERCENTAGE         0x1016
#define VP_FAN_ICON           0x101E
#define VP_ENERGY_KEY_ICON    0x101F
#define VP_HEAT_TARGET        0x1020  // Temperature needed for filament change
#define VP_HEAT_PERCENT       0x1024
#define VP_ZOFFSET            0x1026
#define VP_TEMP_CONTROL       0x1030
#define VP_TEMP_SET           0x1032
#define VP_NOZZLE_TARGET      0x1034
#define VP_NOZZLE_TEMP        0x1036
#define VP_BED_TARGET         0x103A
#define VP_BED_TEMP           0x103C
#define VP_SETTING            0x103E
#define VP_RETURN_BACK        0x1040
#define VP_HOMING_ICON        0x1042
#define VP_BED_LEVEL          0x1044
#define VP_LEVELING_ICON      0x1045
#define VP_AUTOHOME           0x1046
#define VP_AXIS_X             0x1048
#define VP_AXIS_Y             0x104A
#define VP_AXIS_Z             0x104C
#define VP_FILAMENT_LEN1      0x1054
#define VP_FILAMENT           0x1056
#define VP_FILAMENT_LEN2      0x1058
#define VP_LANGUAGE           0x105C
#define VP_NO_FILAMENT        0x105E
#define VP_POWER_LOSS         0x105F
#define VP_MACHINE_NAME       0x1060
#define VP_FIRMWARE_VERSION   0x106A
#define VP_PRINTER_SIZE       0x1074
#define VP_WEBSITE            0x107E
#define VP_VOLUME             0x1088
#define VP_VOLUME_ICON        0x108A
#define VP_SOUND_ICON         0x108C
#define VP_AUTOLEVEL_ICON     0x108D
#define VP_REHEAT_ICON        0x108E
#define VP_MESH_VALUES        0x1100  // 25 words, zig-zag by row
#define VP_FILE_ICON          0x1200  // [0] also shows the motor state
#define VP_FILE_SELECT_ICON   0x1220
#define VP_PRINT_FILENAME     0x2000
#define VP_FILE_NAMES         0x200A  // 20 x 10 words
#define VP_FILE_PRINT         0x20D2
#define VP_FILE_CHOOSE        0x20D3
#define VP_CHOSEN_FILENAME    0x20D4
#define VP_FILE_COUNT         0x20DE
#define SP_FILE_NAMES         0x6003  // Text colour of each list entry (+16 per entry)

#define FILENAME_WORDS        (RTS_FILENAME_LEN / 2 + 1)

class RTS {
  public:
    // Screen pages, Chinese and English
    struct page_t { uint8_t cn, en; };
    static constexpr page_t PAGE_MAIN         = {  1, 45 },
                            PAGE_FILES        = {  2, 46 },
                            PAGE_REHEATING    = {  7, 82 },
                            PAGE_REHEAT_DONE  = {  8, 83 },
                            PAGE_PRINT_DONE   = {  9, 51 },
                            PAGE_PRINT_HEAT   = { 10, 52 },
                            PAGE_PRINTING     = { 11, 53 },
                            PAGE_PAUSED       = { 12, 54 },
                            PAGE_TEMP_FAN_ON  = { 15, 57 },
                            PAGE_TEMP_FAN_OFF = { 16, 58 },
                            PAGE_TOOLS        = { 21, 63 },
                            PAGE_LEVELING     = { 22, 64 },
                            PAGE_FILAMENT     = { 23, 65 },
                            PAGE_FIL_COLD     = { 24, 66 },
                            PAGE_FIL_HEATING  = { 26, 68 },
                            PAGE_TRAMMING     = { 28, 84 },
                            PAGE_MOVE         = { 29, 71 },  // + 0/1/2 for 10/1/0.1mm
                            PAGE_HOMING       = { 32, 74 },
                            PAGE_RECOVERY     = { 36, 76 },
                            PAGE_NO_FILAMENT  = { 38, 78 },
                            PAGE_AUTOLEVEL    = { 43, 85 },
                            PAGE_RUNOUT_COLD  = { 44, 81 },
                            PAGE_WAIT         = { 86, 87 },
                            PAGE_ERR_RUNAWAY  = { 88, 88 },
                            PAGE_ERR_HEATING  = { 89, 89 },
                            PAGE_ERR_TEMP     = { 90, 90 };

    // Status icons, Chinese numbering
    enum StatusIcon : uint8_t {
      ICON_READY = 0, ICON_HEATING = 1, ICON_PRINTING = 2, ICON_PAUSED = 4,
      ICON_NO_CARD = 6, ICON_HEAT_DONE = 7, ICON_COOLING = 8, ICON_COOL_DONE = 9
    };

    // Persistent screen settings, kept in the ExtUI EEPROM block
    struct settings_t {
      uint8_t version;
      bool    chinese;
      uint8_t volume;
      bool    energy_saving;  // Turn off the bed after the first layers
      bool    leveling;       // Bed leveling on at startup (not saved by Marlin for bilinear)
    };
    static settings_t settings;

    static void onStartup();
    static void onIdle();

    static void gotoPage(const page_t p, const uint8_t offset=0);
    static void setStatusIcon(const StatusIcon icon);
    static void writeWord(const uint16_t vp, const uint16_t value);
    static void writeLong(const uint16_t vp, const uint32_t value);
    static void writeText(const uint16_t vp, const char *str);
    static void clearWords(const uint16_t vp, const uint8_t words);

    static void sendLanguage();
    static void sendVolume();
    static void sendZOffset();
    static void sendMesh();
    static void sendMeshPoint(const int8_t x, const int8_t y, const float z);
    static void sendPosition();

    static void refreshFileList();
    static void clearFileList();
    static void clearPrintInfo();

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

    static void onPrintFileKey(const uint16_t value);
    static void onAdjustKey(const uint16_t value);
    static void onPrintChoice(const uint16_t vp, const uint16_t value);
    static void onTempControl(const uint16_t value);
    static void onTempSet(const uint16_t vp, const uint16_t value);
    static void onSetting(const uint16_t value);
    static void onReturnBack(const uint16_t value);
    static void onBedLevel(const uint16_t value);
    static void onAxis(const uint16_t vp, const uint16_t value);
    static void onFilament(const uint16_t vp, const uint16_t value);
    static void onNoFilament(const uint16_t value);
    static void onPowerLoss(const uint16_t value);
    static void onVolume(const uint16_t value);
    static void onFileChoose(const uint16_t value);
    static void onFilePrint(const uint16_t value);

    static void showTempPage();
    static void showPrintPage();
    static void showFilename(const uint16_t vp, const char *name);
    static bool filamentMissing();
    static void startSelectedFile();
    static void resumeJob();
    static void setZOffset(const float z);
    static void moveToTrammingPoint(const float x, const float y);
    static void extrudeFilament(const float mm);
    static void saveSettings();
};

extern RTS rts;
