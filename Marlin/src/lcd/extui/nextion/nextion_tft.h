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
#pragma once

/* ****************************************
 * lcd/extui/nextion/nextion_tft.h
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "nextion_tft_defs.h"
#include "../../../inc/MarlinConfigPre.h"
#include "../ui_api.h"

class NextionTFT {
  private:
    static uint8_t command_len;
    static char    nextion_command[MAX_CMND_LEN];
    static char    selectedfile[MAX_PATH_LEN];

  public:
    NextionTFT();
    static void Startup();
    static void IdleLoop();
    static void PrinterKilled(PGM_P, PGM_P);
    static void ConfirmationRequest(const char * const );
    static void StatusChange(const char * const );
    static void SendtoTFT(PGM_P);
    static void UpdateOnChange();
    static void PrintFinished();
    static void PanelInfo(uint8_t);

  private:
    static bool ReadTFTCommand();
    static void SendFileList(int8_t);
    static void SelectFile();
    static void ProcessPanelRequest();
    static void PanelAction(uint8_t);
    static void _format_time(char *, uint32_t);
};

extern NextionTFT nextion;
