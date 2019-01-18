#pragma once

#define BOARD_NAME "ADSK"

#include "pins_CNC_shield.h"

#define SDSS                52

#define SERVO0_PIN          61 // Analog pin 7, Digital pin 61
#define Z_MIN_PROBE_PIN     62 // Analog pin 8, Digital pin 62

/* RJ45 8 pins extruder connector

1 - GND (Please do not connect to the same GND as extruder heater to prevent ground offset voltage)
2 - thermistor
3 - SERVO PWM
4 - extruder heater
5 - FAN (print cooling)
6 - FAN (extruder cooling)
7 - Probe signal
8 - 5V

Standard ethernet pairs: 1&2, 3&6, 4&5, 7&8
Use CAT7 cable to have all pairs shielded

*/
