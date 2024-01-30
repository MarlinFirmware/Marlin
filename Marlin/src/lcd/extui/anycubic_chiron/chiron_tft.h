/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * lcd/extui/anycubic_chiron/chiron_tft.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "chiron_tft_defs.h"
#include "../../../inc/MarlinConfigPre.h"
#include "../ui_api.h"

#if NONE(CHIRON_TFT_STANDARD, CHIRON_TFT_NEW)
  #define AUTO_DETECT_CHIRON_TFT 1
#endif

namespace Anycubic {

class ChironTFT {
  #if AUTO_DETECT_CHIRON_TFT
    static panel_type_t panel_type;
  #else
    static constexpr panel_type_t panel_type = TERN(CHIRON_TFT_NEW, AC_panel_new, AC_panel_standard);
  #endif
  static last_error_t last_error;
  static printer_state_t  printer_state;
  static paused_state_t   pause_state;
  static heater_state_t   hotend_state;
  static heater_state_t   hotbed_state;
  static xy_uint8_t       selectedmeshpoint;
  static char             panel_command[MAX_CMND_LEN + 1];
  static uint8_t          command_len;
  static char             selectedfile[MAX_PATH_LEN + 1];
  static float            live_Zoffset;
  static file_menu_t      file_menu;
  public:
    static void startup();
    static void idleLoop();
    static void printerKilled(FSTR_P, FSTR_P);
    static void mediaEvent(media_event_t);
    static void timerEvent(timer_event_t);
    static void filamentRunout();
    static void confirmationRequest(const char * const);
    static void statusChange(const char * const);
    static void powerLossRecovery();
    static void printComplete();
    static void tftSend(FSTR_P const=nullptr);
    static void tftSendLn(FSTR_P const=nullptr);
  private:
    static void detectPanelType();
    static bool readTFTCommand();
    static int8_t findToken(char);
    static void checkHeaters();
    static void sendFileList(int8_t);
    static void selectFile();
    static void processPanelRequest();
    static void panelInfo(uint8_t);
    static void panelAction(uint8_t);
    static void panelProcess(uint8_t);
    static bool getLastError();
};

extern ChironTFT chiron;

} // Anycubic namespace
