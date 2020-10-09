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
      void Startup();
      void IdleLoop();
      void PrinterKilled(PGM_P,PGM_P);
      void MediaEvent(media_event_t);
      void TimerEvent(timer_event_t);
      void FilamentRunout();
      void ConfirmationRequest(const char * const );
      void StatusChange(const char * const );
      void PowerLossRecovery();

    private:
      void SendtoTFT(PGM_P);
      void SendtoTFTLN(PGM_P);
      bool ReadTFTCommand();
      int8_t Findcmndpos(const char *, char);
      void CheckHeaters();
      void SendFileList(int8_t);
      void SelectFile();
      void InjectCommandandWait(PGM_P);
      void ProcessPanelRequest();
      void PanelInfo(uint8_t);
      void PanelAction(uint8_t);
      void PanelProcess(uint8_t);
  };

  extern ChironTFT Chiron;

}
