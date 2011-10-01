#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// BASIC SETTINGS: select your board type, thermistor type, axis scaling, and endstop configuration

//// The following define selects which electronics board you have. Please choose the one that matches your setup
// Gen6 = 5, 
#define MOTHERBOARD 7

//// Thermistor settings:
// 1 is 100k thermistor
// 2 is 200k thermistor
// 3 is mendel-parts thermistor
#define THERMISTORHEATER 2
// Select one of these only to define how the nozzle temp is read.
//#define HEATER_USES_THERMISTOR
#define HEATER_USES_AD595
//#define HEATER_USES_MAX6675

// Select one of these only to define how the bed temp is read.
//#define BED_USES_THERMISTOR
//#define BED_USES_AD595

#define HEATER_CHECK_INTERVAL 50
#define BED_CHECK_INTERVAL 5000
#define BNUMTEMPS NUMTEMPS
#define bedtemptable temptable

//// Calibration variables
// X, Y, Z, E steps per unit - Metric Mendel / Orca with V9 extruder:
float axis_steps_per_unit[] = {79.87220447, 79.87220447,  200*8/3., 14}; 
// For E steps per unit = 67 for v9 with direct drive (needs finetuning) for other extruders this needs to be changed 
// Metric Prusa Mendel with Makergear geared stepper extruder:
//float axis_steps_per_unit[] = {80,80,3200/1.25,1380}; 

//// Endstop Settings
#define ENDSTOPPULLUPS // Comment this out (using // at the start of the line) to disable the endstop pullup resistors
// The pullups are needed if you directly connect a mechanical endswitch between the signal and ground pins.
const bool ENDSTOPS_INVERTING = true; // set to true to invert the logic of the endstops. 
// For optos H21LOB set to true, for Mendel-Parts newer optos TCST2103 set to false

// This determines the communication speed of the printer
#define BAUDRATE 115200

// Comment out (using // at the start of the line) to disable SD support:
//#define SDSUPPORT // Enable SD Card Support in Hardware Console
//#define FANCY_LCD  // Hardware Console with 20x4 multipage LCD
//#define SIMPLE_LCD  // 16x2 Simple LCD Display (Bootup, Temp Monitoring only)
#define ULTRA_LCD  //improved 3 button or clickencoder interface
const int dropsegments=2; //everything with this number of steps  will be ignored as move

//// ADVANCED SETTINGS - to tweak parameters

#include "thermistortables.h"

// For Inverting Stepper Enable Pins (Active Low) use 0, Non Inverting (Active High) use 1
#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0

// Disables axis when it's not being used.
#define DISABLE_X false
#define DISABLE_Y false
#define DISABLE_Z false
#define DISABLE_E false

// Inverting axis direction
#define INVERT_X_DIR true    // for Mendel set to false, for Orca set to true
#define INVERT_Y_DIR false   // for Mendel set to true, for Orca set to false
#define INVERT_Z_DIR true    // for Mendel set to false, for Orca set to true
#define INVERT_E_DIR false   // for direct drive extruder v9 set to true, for geared extruder set to false

//// ENDSTOP SETTINGS:
// Sets direction of endstops when homing; 1=MAX, -1=MIN
#define X_HOME_DIR -1
#define Y_HOME_DIR -1
#define Z_HOME_DIR -1

#define min_software_endstops false //If true, axis won't move to coordinates less than zero.
#define max_software_endstops true  //If true, axis won't move to coordinates greater than the defined lengths below.
#define X_MAX_LENGTH 210
#define Y_MAX_LENGTH 210
#define Z_MAX_LENGTH 210

//// MOVEMENT SETTINGS
#define NUM_AXIS 4 // The axis order in all axis related arrays is X, Y, Z, E
float max_feedrate[] = {190*60, 190*60, 10*60, 500000}; // set the max speeds
//note: on bernhards ultimaker 200 200 12 are working well.
float homing_feedrate[] = {70*60, 70*60, 12*60, 0};  // set the homing speeds
//the followint checks if an extrusion is existent in the move. if _not_, the speed of the move is set to the maximum speed. 
//!!!!!!Use only if you know that your printer works at the maximum declared speeds.
// works around the skeinforge cool-bug. There all moves are slowed to have a minimum layer time. However slow travel moves= ooze
#define TRAVELING_AT_MAXSPEED  
bool axis_relative_modes[] = {false, false, false, false};

//// Acceleration settings
// X, Y, Z, E maximum start speed for accelerated moves. E default values are good for skeinforge 40+, for older versions raise them a lot.
float acceleration = 4600;         // Normal acceleration mm/s^2  THIS IS THE DEFAULT ACCELERATION for all moves. M204 SXXXX
float retract_acceleration = 7000; //  mm/s^2   filament pull-pack and push-forward  while standing still in the other axis M204 TXXXX
float max_xy_jerk = 5.0*60; //speed than can be stopped at once, if i understand correctly.
float max_z_jerk = 1*60;
// X, Y, Z and E max acceleration in mm/s^2 for printing moves or retracts
long max_acceleration_units_per_sq_second[] = {9000,9000,150,10000}; // Use M201 to override by software
float minimumfeedrate=20;

// The watchdog waits for the watchperiod in milliseconds whenever an M104 or M109 increases the target temperature
// If the temperature has not increased at the end of that period, the target temperature is set to zero. It can be reset with another M104/M109
//#define WATCHPERIOD 5000 //5 seconds

//// The minimal temperature defines the temperature below which the heater will not be enabled
#define MINTEMP 5


// When temperature exceeds max temp, your heater will be switched off.
// This feature exists to protect your hotend from overheating accidentally, but *NOT* from thermistor short/failure!
// You should use MINTEMP for thermistor short/failure protection.
#define MAXTEMP 275


// minimum time in microseconds that a movement needs to take if the buffer is emptied.   Increase this number if you see blobs while printing high speed & high detail.  It will slowdown on the detailed stuff.
#define MIN_SEGMENT_TIME 60000


/// PID settings:
// Uncomment the following line to enable PID support.
//#define PIDTEMP
#ifdef PIDTEMP
//#define PID_DEBUG 1 // Sends debug data to the serial port. 
//#define PID_OPENLOOP 1 // Puts PID in open loop. M104 sets the output power in %
#define PID_MAX 156 // limits current to nozzle
#define PID_INTEGRAL_DRIVE_MAX 156.0
#define PID_dT 0.16
double Kp = 20.0;
double Ki = 1.5*PID_dT;
double Kd = 80/PID_dT;
#endif // PIDTEMP


// extruder advance constant (s2/mm3)
//
// advance (steps) = STEPS_PER_CUBIC_MM_E * EXTUDER_ADVANCE_K * cubic mm per second ^ 2
//
// hooke's law says:		force = k * distance
// bernoulli's priniciple says:	v ^ 2 / 2 + g . h + pressure / density = constant
// so: v ^ 2 is proportional to number of steps we advance the extruder
//#define ADVANCE

#ifdef ADVANCE
#define EXTRUDER_ADVANCE_K .3

#define D_FILAMENT 1.7
#define STEPS_MM_E 65
#define EXTRUTION_AREA (0.25 * D_FILAMENT * D_FILAMENT * 3.14159)
#define STEPS_PER_CUBIC_MM_E (axis_steps_per_unit[E_AXIS]/ EXTRUTION_AREA)

#endif // ADVANCE

#endif
