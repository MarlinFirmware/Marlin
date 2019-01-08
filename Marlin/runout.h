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

/**
 * runout.h - Runout sensor support
 */

#ifndef _RUNOUT_H_
#define _RUNOUT_H_

#include "temperature.h"
#include "planner.h"
#include "cardreader.h"
#include "printcounter.h"
#include "stepper.h"
#include "Marlin.h"

#include "types.h"
#include "language.h"
#include "configuration_store.h"
#include "duration_t.h"

#include "MarlinConfig.h"

#if ENABLED(CREALITY_DWIN)
 #include "Creality_DWIN.h"
#endif
#define FIL_RUNOUT_THRESHOLD 5

class FilamentRunoutSensor {
  public:
    FilamentRunoutSensor() {}

    static void setup();

    FORCE_INLINE static void reset() { runout_count = 0; filament_ran_out = false; }

    FORCE_INLINE static void run() {
      if ((IS_SD_PRINTING || print_job_timer.isRunning()) && check() && !filament_ran_out) {
        filament_ran_out = true;
        SERIAL_ECHOPAIR("\n ***Runout.h",(int)filament_ran_out);
        #if ENABLED(CREALITY_DWIN)
          char pause_str_Z[16];
				current_position[Z_AXIS] += 5;
				pause_z = current_position[Z_AXIS];
				waitway = 5;		//reject to receive cmd and jump to the corresponding page
	 			card.pauseSDPrint();
				print_job_timer.pause();
	 			temphot=thermalManager.degTargetHotend(0); //thermalManager.target_temperature[0];
	 			tempbed=thermalManager.degTargetBed();//thermalManager.target_temperature_bed;
				//thermalManager.setTargetHotend(0, 0);
				thermalManager.setTargetBed(0);
			//	enqueue_and_echo_commands_P(PSTR("G1 X10 Y10"));

	 			PrintStatue[1] = 1;	// for returning the corresponding page
	 	//		FilementStatus[0] = 2;  // no filements during printing
				FilementStatus[1] = 0;
				PrinterStatusKey[1] = 4;
				
				TPShowStatus = false;
				Update_Time_Value = 0;
				memset(commandbuf,0,sizeof(commandbuf));
				dtostrf(pause_z, 3, 2, pause_str_Z);
				sprintf(commandbuf,"G0 X10 Y10  Z%s",pause_str_Z);
	   			//strncpy_P(commandbuf, PSTR("G28 X0 Y0"), sizeof(commandbuf) - 1);
				injected_commands_P =commandbuf;// PSTR("G28 X0 Y0");//commandbuf;

				enqueue_and_echo_commands_P(PSTR("G0 X10 Y10 F3000"));
        #else
          enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
        #endif
        planner.synchronize();
      }
    }
  private:
    static bool filament_ran_out;
    static uint8_t runout_count;

    FORCE_INLINE static bool check() {
      #if NUM_RUNOUT_SENSORS < 2
        // A single sensor applying to all extruders
        const bool is_out = READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING;
      #else
        // Read the sensor for the active extruder
        bool is_out;
        switch (active_extruder) {
          case 0: is_out = READ(FIL_RUNOUT_PIN) == FIL_RUNOUT_INVERTING; break;
          case 1: is_out = READ(FIL_RUNOUT2_PIN) == FIL_RUNOUT_INVERTING; break;
          #if NUM_RUNOUT_SENSORS > 2
            case 2: is_out = READ(FIL_RUNOUT3_PIN) == FIL_RUNOUT_INVERTING; break;
            #if NUM_RUNOUT_SENSORS > 3
              case 3: is_out = READ(FIL_RUNOUT4_PIN) == FIL_RUNOUT_INVERTING; break;
              #if NUM_RUNOUT_SENSORS > 4
                case 4: is_out = READ(FIL_RUNOUT5_PIN) == FIL_RUNOUT_INVERTING; break;
              #endif
            #endif
          #endif
        }
      #endif
      return (is_out ? ++runout_count : (runout_count = 0)) > FIL_RUNOUT_THRESHOLD;
    }
};

extern FilamentRunoutSensor runout;

#endif // _RUNOUT_H_
