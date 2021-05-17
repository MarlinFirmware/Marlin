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
 * lcd/extui/lib/chiron_tft.h
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "chiron_tft_defs.h"
#include "../../../../inc/MarlinConfigPre.h"
#include "../../ui_api.h"

namespace Anycubic {

  class ChironTFT {
    private:
      static printer_state_t  printer_state;
      static paused_state_t   pause_state;
      static heater_state_t   hotend_state;
      static heater_state_t   hotbed_state;
      static xy_uint8_t       selectedmeshpoint;
      static char             panel_command[MAX_CMND_LEN];
      static uint8_t          command_len;
      static char             selectedfile[MAX_PATH_LEN];
      static float            live_Zoffset;
      static file_menu_t      file_menu;

    public:
      ChironTFT();
      static void Startup();
      static void IdleLoop();
      static void PrinterKilled(PGM_P,PGM_P);
      static void MediaEvent(media_event_t);
      static void TimerEvent(timer_event_t);
      static void FilamentRunout();
      static void ConfirmationRequest(const char * const );
      static void StatusChange(const char * const );
      static void PowerLossRecovery();

    private:
      static void SendtoTFT(PGM_P);
      static void SendtoTFTLN(PGM_P);
      static bool ReadTFTCommand();
      static int8_t Findcmndpos(const char *, char);
      static void CheckHeaters();
      static void SendFileList(int8_t);
      static void SelectFile();
      static void InjectCommandandWait(PGM_P);
      static void ProcessPanelRequest();
      static void PanelInfo(uint8_t);
      static void PanelAction(uint8_t);
      static void PanelProcess(uint8_t);
  };

  extern ChironTFT Chiron;

} // Anycubic
