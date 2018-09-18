/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "../../inc/MarlinConfig.h"

struct E_Step;

class MMU2 {

  public:
    MMU2();

    void init();
    void reset();
    void mmuLoop();
    void toolChange(uint8_t index);
    void toolChange(const char* special);
    uint8_t getCurrentTool();
    void setFilamentType(uint8_t index, uint8_t type);
#if HAS_LCD_MENU && ENABLED(MMU2_MENUS)
    bool unload();
    void loadFilament(uint8_t);
    void loadAll();
    bool loadFilamentToNozzle(uint8_t index);
    bool ejectFilament(uint8_t index, bool recover);
#endif

  private:
    bool rx_str_P(const char* str);
    void tx_str_P(const char* str);
    void tx_printf_P(const char* format, int argument);
    void tx_printf_P(const char* format, int argument1, int argument2);
    void clear_rx_buffer();

    bool rx_ok();
    bool rx_start();
    void checkVersion();
    
    void command(uint8_t cmd);
    bool getResponse(void);
    void manageResponse(bool move_axes, bool turn_off_nozzle);

#if HAS_LCD_MENU && ENABLED(MMU2_MENUS)        
    void loadToNozzle();
    void filamentRamming();

    void executeExtruderSequence(const E_Step * sequence, int steps);
#endif
  
    void filamentRunout();
  
    bool enabled = false;
    bool ready = false;
    bool mmu_print_saved = false;
    uint8_t cmd = 0;
    uint8_t cmd_arg;
    uint8_t last_cmd = 0;
    int8_t state = 0;
    uint8_t extruder;
    volatile int8_t finda = 1;
    volatile bool findaRunoutValid = false;
    int16_t version = -1;
    int16_t buildnr = -1;
    uint32_t last_request = 0;
    uint32_t last_response = 0;
    char rx_buffer[16];
    char tx_buffer[16];

};

extern MMU2 mmu2;
