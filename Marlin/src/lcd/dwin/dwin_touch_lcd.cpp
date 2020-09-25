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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(DWIN_CREALITY_TOUCHLCD)

#include "../../inc/MarlinConfig.h"

#include "dwin_touch_lcd.h"
#include "cr6/touch_lcd.h"

#include <libmaple/usart.h>

#include "../../../Version.h"

#include "../../MarlinCore.h"
#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/settings.h"
#include "../../module/stepper.h"
#include "../../module/printcounter.h"
#include "../../feature/babystep.h"
#include "../../feature/powerloss.h"
#include "../../gcode/gcode.h"

#include "../../feature/bedlevel/abl/abl.h"

#include "../../libs/duration_t.h"

void DWINTouch_inactivity_callback() {
  waitway = 0;

  rtscheck.change_page(DWINTouchPage::ERR_FATAL_UNSPECIFIED);

  rtscheck.RTS_SndData(Error_201, ABNORMAL_TEXT_VP);
  errorway = 1;
}

void DWINTouch_refresh() {
  RTSUpdate();
}

void DWINTouch_init() {
  rtscheck.RTS_Init();

  #ifdef FIX_MOUNTED_PROBE
    OUT_WRITE(COM_PIN, 1);
    SET_INPUT(OPTO_SWITCH_PIN);
    OUT_WRITE(LED_CONTROL_PIN, 0);
  #endif
}

void DWINTouch_notify_filament_runout() {
  rtscheck.RTS_FilamentRunout();
}

void DWINTouch_filament_loaded() {
  rtscheck.RTS_FilamentLoaded();
}

void DWINTouch_bedlevel_update_callback(uint8_t count) {
  rtscheck.RTS_SndData(count, AUTO_BED_LEVEL_TITLE_VP);

  rtscheck.change_page(DWINTouchPage::LEVELING);

  rtscheck.RTS_SndData(AUTO_BED_LEVEL_PREHEAT, AUTO_BED_PREHEAT_HEAD_DATA_VP);
  rtscheck.RTS_SndData(AUTO_BED_LEVEL_PREHEAT, HEAD_SET_TEMP_VP);
  rtscheck.RTS_SndData(probe.offset.z * 100, AUTO_BED_LEVEL_ZOFFSET_VP);

  rtscheck.RTS_SndData(feedrate_percentage, PRINT_SPEED_RATE_VP);
  rtscheck.RTS_SndData(thermalManager.temp_hotend[0].target, HEAD_SET_TEMP_VP);
  rtscheck.RTS_SndData(thermalManager.temp_bed.target, BED_SET_TEMP_VP);
}

void DWINTouch_bedlevel_finish_callback() {
  if(3 == waitway)
  {
     waitway = 0;
  }
}

void DWINTouch_autohome_callback() {
  home_flag = true;

  if(waitway == 4 || waitway == 6 || waitway == 7)
  {
    rtscheck.change_page(DWINTouchPage::AUTOHOME_IN_PROGRESS);
  }
}

void DWINTouch_autohome_update_callback() {
  if(rtscheck.has_fatal_error() == false)
    {
      if(waitway == 6)
      {
        
        do_blocking_move_to_z(0);
        rtscheck.change_page(DWINTouchPage::MENU_ZOFFSET_LEVELING);

        waitway = 0;
      }
      else if(waitway == 4)
      {
       
        do_blocking_move_to_z(0);

        switch (AxisUnitMode) {
          case 1:
            rtscheck.change_page(DWINTouchPage::MOVE_10MM);
            break;

          case 2:
            rtscheck.change_page(DWINTouchPage::MOVE_1MM);
            break;

          case 3:
            rtscheck.change_page(DWINTouchPage::MOVE_01MM);
            break;
        }

        waitway = 0;
      }
      else if(waitway == 7)
      {
      
        rtscheck.change_page(DWINTouchPage::MAIN_MENU);
        
        rtscheck.RTS_SndData(2, MOTOR_FREE_ICON_VP); 
        rtscheck.RTS_SndData(0, PRINT_PROCESS_TITLE_VP);
        rtscheck.RTS_SndData(0, PRINT_PROCESS_VP);
        delay(2);
        for(int j = 0;j < 10;j ++)
        {
          // clean screen.
          rtscheck.RTS_SndData(0, CONTINUE_PRINT_FILE_TEXT_VP + j);
        }
        waitway = 0;
      }

      rtscheck.RTS_SndData(10*current_position[X_AXIS], AXIS_X_COORD_VP);
      rtscheck.RTS_SndData(10*current_position[Y_AXIS], AXIS_Y_COORD_VP);
      rtscheck.RTS_SndData(10*current_position[Z_AXIS], AXIS_Z_COORD_VP);

      if(finish_home) finish_home = false;

      if(StartPrint_flag) 
      {
        StartPrint_flag = 0;
        recovery.info.current_position.z = 0;
      }
      errorway = errornum = 0;
    }
}

void DWINTouch_autohome_complete_callback() {
  home_flag = false;
}

bool DWINTouch_autohome_is_lcd_ready() {
  return finish_home == false && waitway != 7;
}

void DWINTouch_print_completed_callback() {
  print_finish = 1;

  rtscheck.RTS_SndData(100, PRINT_PROCESS_VP);
  delay(1);
  rtscheck.RTS_SndData(100 ,PRINT_PROCESS_TITLE_VP);
 
  rtscheck.change_page(DWINTouchPage::PRINT_FINISHED);

}

void DWINTouch_error_home_failed() {
  waitway = 0;

  rtscheck.change_page(DWINTouchPage::ERR_FATAL_UNSPECIFIED);
  rtscheck.RTS_SndData(Error_202, ABNORMAL_TEXT_VP);
  errorway = 2;
}

void DWINTouch_error_probe_failed() {
  waitway = 0;

  rtscheck.change_page(DWINTouchPage::ERR_FATAL_UNSPECIFIED);
  rtscheck.RTS_SndData(Error_203, ABNORMAL_TEXT_VP);
  errorway = 3;
}

void DWINTouch_temperature_refresh() {
  rtscheck.change_page(DWINTouchPage::ERR_HEATING_FAILED);
}

void DWINTouch_error_max_temp() {
  rtscheck.change_page(DWINTouchPage::ERR_THERMISTOR);
}

void DWINTouch_error_min_temp() {
  rtscheck.change_page(DWINTouchPage::ERR_THERMISTOR);
}

void DWINTouch_error_runaway_temp() {
  rtscheck.change_page(DWINTouchPage::ERR_THERMAL_RUNAWAY);
}

void DWINTouch_heating_callback() {
  if(heat_flag && printingIsActive())
  {
    rtscheck.change_page(DWINTouchPage::PRINT_PROGRESS_RUNNING);
  }
  
  heat_flag = 0;
}

void DWINTouch_user_confirm_required() {
  rtscheck.change_page(DWINTouchPage::DIALOG_POWER_FAILURE);
}

#endif