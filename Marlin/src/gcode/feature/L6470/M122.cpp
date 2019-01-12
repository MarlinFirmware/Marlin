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

#include "../../../inc/MarlinConfig.h"


#if HAS_DRIVER(ST_L6470)

#include "../../gcode.h"
#include "../../../module/L6470/L6470_Marlin.h"
#include "../../../module/stepper_indirection.h"





void L6470_status_decode(uint16_t status, uint8_t axis) {
  char temp_buf[10];
  SERIAL_ECHOPAIR("AXIS: ", L6470_index_to_Axis[axis][0]);
  SERIAL_ECHO(L6470_index_to_Axis[axis][1]);
  sprintf(temp_buf,"  %4x   ", status);
  SERIAL_ECHO(temp_buf);
  print_bin(status);
  SERIAL_EOL();
  SERIAL_ECHOPAIR("...OUTPUT: ", status & STATUS_HIZ ? "OFF" : "ON ") ;
  SERIAL_ECHOPAIR("   BUSY: ", status & STATUS_BUSY ? "NO " : "YES ") ;
  SERIAL_ECHOPAIR("   DIR: ", (((status & STATUS_DIR) >> 4) ^ L6470_index_to_DIR[axis]) ? "FORWARD" : "REVERSE") ;
  SERIAL_ECHOPAIR("   Last Command: ", status & STATUS_WRONG_CMD ? "INVALID " : "VALID ") ;
  SERIAL_ECHOLNPAIR("   ", status & STATUS_NOTPERF_CMD ? "Not PERFORMED" : "COMPLETED    ") ;
  SERIAL_ECHOPAIR("...THERMAL: ", !(status & STATUS_TH_SD) ? "SHUTDOWN" : !(status & STATUS_TH_WRN) ? "WARNING " :"OK      ") ;
  SERIAL_ECHOPAIR("   OVERCURRENT: ", status & STATUS_OCD ? "NO " : "YES") ;
  SERIAL_ECHOPAIR("   STALL: ", (!(status & STATUS_STEP_LOSS_A) | !(status & STATUS_STEP_LOSS_B))  ? "YES" : "NO ") ;
  SERIAL_ECHOLNPAIR("   STEP-CLOCK MODE: ", status & STATUS_SCK_MOD  ? "YES" : "NO ") ;

}



/**
 * M122: Debug L6470 drivers
 */
void GcodeSuite::M122() {

  #define L6470_SAY_STATUS(Q)  L6470_status_decode(stepper##Q.getStatus(), Q)


//    if (parser.seen('S'))
//      tmc_set_report_status(parser.value_bool());
//    else



        #if AXIS_DRIVER_TYPE_X(ST_L6470)
          L6470_SAY_STATUS(X);
        #endif
        #if AXIS_DRIVER_TYPE_X2(ST_L6470)
          L6470_SAY_STATUS(X2);
        #endif
        #if AXIS_DRIVER_TYPE_Y(ST_L6470)
          L6470_SAY_STATUS(Y);
        #endif
        #if AXIS_DRIVER_TYPE_Y2(ST_L6470)
          L6470_SAY_STATUS(Y2);
        #endif
        #if AXIS_DRIVER_TYPE_Z(ST_L6470)
          L6470_SAY_STATUS(Z);
        #endif
        #if AXIS_DRIVER_TYPE_Z2(ST_L6470)
          L6470_SAY_STATUS(Z2);
        #endif
        #if AXIS_DRIVER_TYPE_Z3(ST_L6470)
          L6470_SAY_STATUS(Z3);
        #endif
        #if AXIS_DRIVER_TYPE_E0(ST_L6470)
          L6470_SAY_STATUS(E0);
        #endif
        #if AXIS_DRIVER_TYPE_E1(ST_L6470)
          L6470_SAY_STATUS(E1);
        #endif
        #if AXIS_DRIVER_TYPE_E2(ST_L6470)
          L6470_SAY_STATUS(E2);
        #endif
        #if AXIS_DRIVER_TYPE_E3(ST_L6470)
          L6470_SAY_STATUS(E3);
        #endif
        #if AXIS_DRIVER_TYPE_E4(ST_L6470)
          L6470_SAY_STATUS(E4);
        #endif
        #if AXIS_DRIVER_TYPE_E5(ST_L6470)
          L6470_SAY_STATUS(E5);
        #endif


}

#endif // HAS_DRIVER(ST_L6470)
