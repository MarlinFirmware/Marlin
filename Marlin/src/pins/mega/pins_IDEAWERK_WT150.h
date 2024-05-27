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
 * ideawerk pin assignments
 * Comes with an ATmega1280
 */

//#define ALLOW_MEGA1280
//#include "env_validate.h"

#define BOARD_INFO_NAME "Ideawerk"

//
// Limit Switches
//
#define X_MIN_PIN                            49 //pl0
#define Y_MIN_PIN                            47 //pl2 
#define Z_MAX_PIN                            42 //pl7 

//
// Steppers
//
#define X_STEP_PIN                            55 //pf1 
#define X_DIR_PIN                             54 //pf0
#define X_ENABLE_PIN                          56 //pf2 

#define Y_STEP_PIN                            59 //pf5 
#define Y_DIR_PIN                             58 //pf4 
#define Y_ENABLE_PIN                          60 //pf6 

#define Z_STEP_PIN                            63 //pk1 
#define Z_DIR_PIN                             62 //pk0
#define Z_ENABLE_PIN                          64 //pk2 

#define E0_STEP_PIN                           25 //pa3 
#define E0_DIR_PIN                            24 //pa2 
#define E0_ENABLE_PIN                         26 //pa4 

//
// Temperature Sensor. Located in printhead 
//

#define TEMP_0_CS_PIN                          3 //pe5 uses MAX6675 themocouple adc 



//
// Heaters / Fans
//
#define HEATER_0_PIN                          6 //ph3


#ifndef FAN_PIN
  #define FAN_PIN                             7 //ph4 
#endif


//
// Misc. Functions
//
#define SDSS                                  53
//#define SD_DETECT_PIN                         15

// Beeper 

# define BEEPER_PIN                           4 //pg5 

// Following functions are not included in stock but the board includes headers for these features. 


// More endstops 

#define X_MAX_PIN                             48 //PL1

#define Y_MAX_PIN                             46 //PL3. Board appears to use part of this header for a botch wire. 

#define Z_MIN_PIN                             43 //PL6

// Additional heaters/fans 

#define HEATER_1_PIN                          11 //PB5

#define FAN_1_PIN                             12 //PB6

#define FAN_2_PIN                             44 //PL5. Board refers to this as an "extra" fan

// Second temperature sensor

# define TEMP_1_CS_PIN                        79 //PE6

// Bed heater

#define HEATER_BED_PIN                        45 //PL4