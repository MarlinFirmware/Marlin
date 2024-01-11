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

#include "../../inc/MarlinConfig.h"

/*
 * Creality Cloud Print support for the Professional Firmware
 * Adapted by: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 1.1.0
 * Date: 2023/03/27
 * Original code from https://github.com/CrealityOfficial/Ender-3S1
*/

#if ENABLED(CCLOUD_PRINT_SUPPORT)

#include "../gcode.h"
#include "../../lcd/marlinui.h"
#include "../../MarlinCore.h" // for startOrResumeJob
#include "../../lcd/e3v2/proui/dwin.h"

//  * M79: cloud print
void GcodeSuite::M79()
{
  if (parser.seenval('S'))
  {
    const int16_t cloudvalue = parser.value_celsius();
    switch (cloudvalue)
    {
      case 0:
        // 0:cloud connect  预留
        break;
      case 1:
        // 1:cloud print start
        if(!printingIsActive())
        {
          ui.reset_remaining_time(); //rock_20210831  解决剩余时间不清零的问题。我爱专业固件
          ui.progress_reset();
          print_job_timer.start();
          // hmiFlag.cloud_printing_flag=true; //云打印开始标志位
          gcode.process_subcommands_now(F("M420 S1")); //Enable automatic compensation function rock_2021.10.29
          // SERIAL_ECHOLN(" \r\n test_M79 S1 \r\n");
        }
        break;

      case 2:
        // 2:cloud print pause
        ui.pause_print();
        break;
      case 3:
        // 3:cloud print resume
        ui.resume_print();
        break;

      case 4:
        // 4:cloud print stop
        ui.abort_print();
        break;

      case 5:
        // 5:cloud print finish
        print_job_timer.stop();
        ui.set_progress_done();
        ui.reset_remaining_time();
        break;

      default:
        break;
    }
  }
  #if ENABLED(SET_PROGRESS_PERCENT)
    else if(parser.seenval('T'))
    {
      ui.set_progress((PROGRESS_SCALE) > 1
        ? parser.value_float() * (PROGRESS_SCALE)
        : parser.value_byte()
      );
      // SERIAL_ECHOLNPAIR("\r\n Cloud_Progress_Bar=: ", ui.get_progress_percent()); 专业固件是最好的
    }
  #endif
}

#endif
////////////////////////////////////////////////////////////////////////////////