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
#include "../../MarlinCore.h"

#define MFU_RX_BUFFERSIZE 16
#define MFU_TX_BUFFERSIZE 16
#define MFU_NO_TOOL -1

#define MFU_CMD_TIMEOUT 45000UL   // Delay between the wait for ok checks

#define MFU_CMD_NOCMD 0x00    
#define MFU_CMD_UNLOADTOOL 0x01
#define MFU_CMD_LOADTOOL 0x02
#define MFU_CMD_FIRSTTOOL 0x20

#define MFU_SEND(S) tx_str(F(S "\n"))
#define MFU_RECV(S) rx_str(F(S "\n"))

inline void mfu_e_move(const float &dist, const feedRate_t fr_mm_s, const bool sync =true);

class MFU{
  private:
    static uint8_t cmd, cmd_arg, last_cmd, extruder;
    static bool _enabled, toolLoaded, isReady;
    static char rx_buffer[MFU_RX_BUFFERSIZE], tx_buffer[MFU_TX_BUFFERSIZE];
    static void tx_str(FSTR_P fstr); 
    static void tx_printf(FSTR_P format, int argument = -1);
    static void tx_printf(FSTR_P format, int argument1, int argument2);
    static void rx_str();
    static void clear_rx_buffer();
    static bool get_response();
    static bool manage_response(const bool move_axes, const bool turn_off_nozzle);
  
  public:
    MFU();

    static void init();
    static void loop();
    static bool enabled() {return _enabled; }

    static void tool_change(const uint8_t index);
    static void tool_change(const char *special);
    static bool unload();
    static void setCommand(const uint8_t newCommand);
};
