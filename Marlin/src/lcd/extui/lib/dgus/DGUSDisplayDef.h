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

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "DGUSVPVariable.h"

#include <stdint.h>

// This file defines the interaction between Marlin and the display firmware.

// information on which screen which VP is displayed
// As this is a sparse table, two arrays are needed:
// one to list the VPs of one screen and one to map screens to the lists.
// (Strictly this would not be necessary, but allows to only send data the display needs and reducing load on Marlin)
struct VPMapping {
  const uint8_t screen;
  const uint16_t *VPList;  // The list is null-terminated.
};

extern const struct VPMapping VPMap[];

// List of VPs handled by Marlin / The Display.
extern const struct DGUS_VP_Variable ListOfVP[];

#if ENABLED(DGUS_LCD_UI_MKS)
  extern uint16_t distanceMove;
  extern float distanceFilament;
  extern uint16_t FilamentSpeed;
  extern float ZOffset_distance;
  extern float mesh_adj_distance;
  extern float Z_distance;

  extern int16_t level_1_x_point;
  extern int16_t level_1_y_point;
  extern int16_t level_2_x_point;
  extern int16_t level_2_y_point;
  extern int16_t level_3_x_point;
  extern int16_t level_3_y_point;
  extern int16_t level_4_x_point;
  extern int16_t level_4_y_point;
  extern int16_t level_5_x_point;
  extern int16_t level_5_y_point;

  extern uint16_t tim_h;
  extern uint16_t tim_m;
  extern uint16_t tim_s;

  extern uint16_t x_park_pos;
  extern uint16_t y_park_pos;
  extern uint16_t z_park_pos;

  extern uint16_t min_ex_temp;

  extern float z_offset_add;

  extern uint16_t tmc_x_step;
  extern uint16_t tmc_y_step;
  extern uint16_t tmc_z_step;

  #if AXIS_HAS_STEALTHCHOP(X)
    extern uint16_t tmc_x_current;
  #endif

  #if AXIS_HAS_STEALTHCHOP(Y)
    extern uint16_t tmc_y_current;
  #endif

  #if AXIS_HAS_STEALTHCHOP(Z)
    extern uint16_t tmc_z_current;
  #endif

  #if AXIS_HAS_STEALTHCHOP(E0)
    extern uint16_t tmc_e0_current;
  #endif

  #if AXIS_HAS_STEALTHCHOP(E1)
    extern uint16_t tmc_e1_current;
  #endif

  typedef enum
  {
    EX_HEATING,
    EX_HEAT_STARUS,
    EX_CHANGING,
    EX_CHANGE_STATUS,
    EX_NONE,
  }EX_STATUS_DEF;

  typedef struct 
    {
        // uint8_t ex_change_flag:1;
        // uint8_t ex_heat_flag:1;
        uint8_t ex_load_unload_flag:1;  //0:unload  1:load
        EX_STATUS_DEF ex_status;
        uint32_t ex_tick_start;
        uint32_t ex_tick_end;       
        uint32_t ex_speed;
        uint32_t ex_length;
        uint32_t ex_need_time;
    }EX_FILAMENT_DEF;
  extern EX_FILAMENT_DEF ex_filament;


  typedef enum
  {
    UNRUNOUT_STATUS,
    RUNOUT_STATUS,
    RUNOUT_WAITTING_STATUS,
    RUNOUT_BEGIN_STATUS,
  }RUNOUT_MKS_STATUS_DEF;

  typedef struct 
  {
    
    RUNOUT_MKS_STATUS_DEF runout_status;
    uint8_t pin_status;
    uint8_t de_count;
    uint8_t de_times;

  }RUNOUT_MKS_DEF;
  extern RUNOUT_MKS_DEF runout_mks;

  typedef struct
  {
    uint8_t print_pause_start_flag:1;
    uint8_t runout_flag:1;
    bool blstatus;
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t z_pos;
  }NOZZLE_PARK_DEF;
  extern NOZZLE_PARK_DEF nozzle_park_mks;

#endif 

#include "../../../../inc/MarlinConfig.h"

#if ENABLED(DGUS_LCD_UI_ORIGIN)
  #include "origin/DGUSDisplayDef.h"
#elif ENABLED(DGUS_LCD_UI_MKS)
  #include "mks/DGUSDisplayDef.h"
#elif ENABLED(DGUS_LCD_UI_FYSETC)
  #include "fysetc/DGUSDisplayDef.h"
#elif ENABLED(DGUS_LCD_UI_HIPRECY)
  #include "hiprecy/DGUSDisplayDef.h"
#endif
