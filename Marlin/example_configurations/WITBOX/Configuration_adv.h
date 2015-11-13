#ifndef CONFIGURATION_ADV_H
#define CONFIGURATION_ADV_H

#include "Conditionals.h"

// @section temperature

//===========================================================================
//=============================Thermal Settings  ============================
//===========================================================================

#if ENABLED(BED_LIMIT_SWITCHING)
  #define BED_HYSTERESIS 2 //only disable heating if T>target+BED_HYSTERESIS and enable heating if T>target-BED_HYSTERESIS
#endif
#define BED_CHECK_INTERVAL 5000 //ms between checks in bang-bang control

/**
 * Thermal Protection parameters
 */
#if ENABLED(THERMAL_PROTECTION_HOTENDS)
  #define THERMAL_PROTECTION_PERIOD 40        // Seconds
  #define THERMAL_PROTECTION_HYSTERESIS 4     // Degrees Celsius

  /**
   * Whenever an M104 or M109 increases the target temperature the firmware will wait for the
   * WATCH_TEMP_PERIOD to expire, and if the temperature hasn't increased by WATCH_TEMP_INCREASE
   * degrees, the machine is halted, requiring a hard reset. This test restarts with any M104/M109,
   * but only if the current temperature is far enough below the target for a reliable test.
   */
  #define WATCH_TEMP_PERIOD 16                // Seconds
  #define WATCH_TEMP_INCREASE 4               // Degrees Celsius
#endif

#if ENABLED(THERMAL_PROTECTION_BED)
  #define THERMAL_PROTECTION_BED_PERIOD 20    // Seconds
  #define THERMAL_PROTECTION_BED_HYSTERESIS 2 // Degrees Celsius
#endif

/**
 * Automatic Temperature:
 * The hotend target temperature is calculated by all the buffered lines of gcode.
 * The maximum buffered steps/sec of the extruder motor is called "se".
 * Start autotemp mode with M109 S<mintemp> B<maxtemp> F<factor>
 * The target temperature is set to mintemp+factor*se[steps/sec] and is limited by
 * mintemp and maxtemp. Turn this off by excuting M109 without F*
 * Also, if the temperature is set to a value below mintemp, it will not be changed by autotemp.
 * On an Ultimaker, some initial testing worked with M109 S215 B260 F1 in the start.gcode
 */
#if ENABLED(PIDTEMP)
  // this adds an experimental additional term to the heating power, proportional to the extrusion speed.
  // if Kc is chosen well, the additional required power due to increased melting should be compensated.
  #define PID_ADD_EXTRUSION_RATE
  #if ENABLED(PID_ADD_EXTRUSION_RATE)
    #define DEFAULT_Kc (100) //heating power=Kc*(e_speed)
    #define LPQ_MAX_LEN 50
  #endif
#endif


//automatic temperature: The hot end target temperature is calculated by all the buffered lines of gcode.
//The maximum buffered steps/sec of the extruder motor are called "se".
//You enter the autotemp mode by a M109 S<mintemp> B<maxtemp> F<factor>
// the target temperature is set to mintemp+factor*se[steps/sec] and limited by mintemp and maxtemp
// you exit the value by any M109 without F*
// Also, if the temperature is set to a value <mintemp, it is not changed by autotemp.
// on an Ultimaker, some initial testing worked with M109 S215 B260 F1 in the start.gcode
#define AUTOTEMP
#if ENABLED(AUTOTEMP)
  #define AUTOTEMP_OLDWEIGHT 0.98
#endif

//Show Temperature ADC value
//The M105 command return, besides traditional information, the ADC value read from temperature sensors.
//#define SHOW_TEMP_ADC_VALUES

// @section extruder

//  extruder run-out prevention.
//if the machine is idle, and the temperature over MINTEMP, every couple of SECONDS some filament is extruded
//#define EXTRUDER_RUNOUT_PREVENT
#define EXTRUDER_RUNOUT_MINTEMP 190
#define EXTRUDER_RUNOUT_SECONDS 30.
#define EXTRUDER_RUNOUT_ESTEPS 14. //mm filament
#define EXTRUDER_RUNOUT_SPEED 1500.  //extrusion speed
#define EXTRUDER_RUNOUT_EXTRUDE 100

// @section temperature

//These defines help to calibrate the AD595 sensor in case you get wrong temperature measurements.
//The measured temperature is defined as "actualTemp = (measuredTemp * TEMP_SENSOR_AD595_GAIN) + TEMP_SENSOR_AD595_OFFSET"
#define TEMP_SENSOR_AD595_OFFSET 0.0
#define TEMP_SENSOR_AD595_GAIN   1.0

//This is for controlling a fan to cool down the stepper drivers
//it will turn on when any driver is enabled
//and turn off after the set amount of seconds from last driver being disabled again
#define CONTROLLERFAN_PIN -1 //Pin used for the fan to cool controller (-1 to disable)
#define CONTROLLERFAN_SECS 60 //How many seconds, after all motors were disabled, the fan should run
#define CONTROLLERFAN_SPEED 255  // == full speed

// When first starting the main fan, run it at full speed for the
// given number of milliseconds.  This gets the fan spinning reliably
// before setting a PWM value. (Does not work with software PWM for fan on Sanguinololu)
//#define FAN_KICKSTART_TIME 100

// This defines the minimal speed for the main fan, run in PWM mode
// to enable uncomment and set minimal PWM speed for reliable running (1-255)
// if fan speed is [1 - (FAN_MIN_PWM-1)] it is set to FAN_MIN_PWM
//#define FAN_MIN_PWM 50

// @section extruder

// Extruder cooling fans
// Configure fan pin outputs to automatically turn on/off when the associated
// extruder temperature is above/below EXTRUDER_AUTO_FAN_TEMPERATURE.
// Multiple extruders can be assigned to the same pin in which case
// the fan will turn on when any selected extruder is above the threshold.
#define EXTRUDER_0_AUTO_FAN_PIN -1
#define EXTRUDER_1_AUTO_FAN_PIN -1
#define EXTRUDER_2_AUTO_FAN_PIN -1
#define EXTRUDER_3_AUTO_FAN_PIN -1
#define EXTRUDER_AUTO_FAN_TEMPERATURE 50
#define EXTRUDER_AUTO_FAN_SPEED   255  // == full speed


//===========================================================================
//=============================Mechanical Settings===========================
//===========================================================================

// @section homing

//#define ENDSTOPS_ONLY_FOR_HOMING // If defined the endstops will only be used for homing

// @section extras

//#define Z_LATE_ENABLE // Enable Z the last moment. Needed if your Z driver overheats.

// A single Z stepper driver is usually used to drive 2 stepper motors.
// Uncomment this define to utilize a separate stepper driver for each Z axis motor.
// Only a few motherboards support this, like RAMPS, which have dual extruder support (the 2nd, often unused, extruder driver is used
// to control the 2nd Z axis stepper motor). The pins are currently only defined for a RAMPS motherboards.
// On a RAMPS (or other 5 driver) motherboard, using this feature will limit you to using 1 extruder.
//#define Z_DUAL_STEPPER_DRIVERS

#if ENABLED(Z_DUAL_STEPPER_DRIVERS)

  // Z_DUAL_ENDSTOPS is a feature to enable the use of 2 endstops for both Z steppers - Let's call them Z stepper and Z2 stepper.
  // That way the machine is capable to align the bed during home, since both Z steppers are homed.
  // There is also an implementation of M666 (software endstops adjustment) to this feature.
  // After Z homing, this adjustment is applied to just one of the steppers in order to align the bed.
  // One just need to home the Z axis and measure the distance difference between both Z axis and apply the math: Z adjust = Z - Z2.
  // If the Z stepper axis is closer to the bed, the measure Z > Z2 (yes, it is.. think about it) and the Z adjust would be positive.
  // Play a little bit with small adjustments (0.5mm) and check the behaviour.
  // The M119 (endstops report) will start reporting the Z2 Endstop as well.

  //#define Z_DUAL_ENDSTOPS

  #if ENABLED(Z_DUAL_ENDSTOPS)
    #define Z2_MAX_PIN 36                     //Endstop used for Z2 axis. In this case I'm using XMAX in a Rumba Board (pin 36)
    const bool Z2_MAX_ENDSTOP_INVERTING = false;
    #define DISABLE_XMAX_ENDSTOP              //Better to disable the XMAX to avoid conflict. Just rename "XMAX_ENDSTOP" by the endstop you are using for Z2 axis.
  #endif

#endif // Z_DUAL_STEPPER_DRIVERS

// Same again but for Y Axis.
//#define Y_DUAL_STEPPER_DRIVERS

#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  // Define if the two Y drives need to rotate in opposite directions
  #define INVERT_Y2_VS_Y_DIR true
#endif

// Enable this for dual x-carriage printers.
// A dual x-carriage design has the advantage that the inactive extruder can be parked which
// prevents hot-end ooze contaminating the print. It also reduces the weight of each x-carriage
// allowing faster printing speeds.
//#define DUAL_X_CARRIAGE
#if ENABLED(DUAL_X_CARRIAGE)
  // Configuration for second X-carriage
  // Note: the first x-carriage is defined as the x-carriage which homes to the minimum endstop;
  // the second x-carriage always homes to the maximum endstop.
  #define X2_MIN_POS 80     // set minimum to ensure second x-carriage doesn't hit the parked first X-carriage
  #define X2_MAX_POS 353    // set maximum to the distance between toolheads when both heads are homed
  #define X2_HOME_DIR 1     // the second X-carriage always homes to the maximum endstop position
  #define X2_HOME_POS X2_MAX_POS // default home position is the maximum carriage position
      // However: In this mode the EXTRUDER_OFFSET_X value for the second extruder provides a software
      // override for X2_HOME_POS. This also allow recalibration of the distance between the two endstops
      // without modifying the firmware (through the "M218 T1 X???" command).
      // Remember: you should set the second extruder x-offset to 0 in your slicer.

  // Pins for second x-carriage stepper driver (defined here to avoid further complicating pins.h)
  #define X2_ENABLE_PIN 29
  #define X2_STEP_PIN 25
  #define X2_DIR_PIN 23

  // There are a few selectable movement modes for dual x-carriages using M605 S<mode>
  //    Mode 0: Full control. The slicer has full control over both x-carriages and can achieve optimal travel results
  //                           as long as it supports dual x-carriages. (M605 S0)
  //    Mode 1: Auto-park mode. The firmware will automatically park and unpark the x-carriages on tool changes so
  //                           that additional slicer support is not required. (M605 S1)
  //    Mode 2: Duplication mode. The firmware will transparently make the second x-carriage and extruder copy all
  //                           actions of the first x-carriage. This allows the printer to print 2 arbitrary items at
  //                           once. (2nd extruder x offset and temp offset are set using: M605 S2 [Xnnn] [Rmmm])

  // This is the default power-up mode which can be later using M605.
  #define DEFAULT_DUAL_X_CARRIAGE_MODE 0

  // Default settings in "Auto-park Mode"
  #define TOOLCHANGE_PARK_ZLIFT   0.2      // the distance to raise Z axis when parking an extruder
  #define TOOLCHANGE_UNPARK_ZLIFT 1        // the distance to raise Z axis when unparking an extruder

  // Default x offset in duplication mode (typically set to half print bed width)
  #define DEFAULT_DUPLICATION_X_OFFSET 100

#endif //DUAL_X_CARRIAGE

// @section homing

//homing hits the endstop, then retracts by this distance, before it tries to slowly bump again:
#define X_HOME_BUMP_MM 5
#define Y_HOME_BUMP_MM 5
#define Z_HOME_BUMP_MM 2
#define HOMING_BUMP_DIVISOR {2, 2, 4}  // Re-Bump Speed Divisor (Divides the Homing Feedrate)
//#define QUICK_HOME  //if this is defined, if both x and y are to be homed, a diagonal move will be performed initially.

// When G28 is called, this option will make Y home before X
//#define HOME_Y_BEFORE_X

// @section machine

#define AXIS_RELATIVE_MODES {false, false, false, false}

// @section machine

//By default pololu step drivers require an active high signal. However, some high power drivers require an active low signal as step.
#define INVERT_X_STEP_PIN false
#define INVERT_Y_STEP_PIN false
#define INVERT_Z_STEP_PIN false
#define INVERT_E_STEP_PIN false

// Default stepper release if idle. Set to 0 to deactivate.
#define DEFAULT_STEPPER_DEACTIVE_TIME 60

#define DEFAULT_MINIMUMFEEDRATE       0.0     // minimum feedrate
#define DEFAULT_MINTRAVELFEEDRATE     0.0

// @section lcd

#if ENABLED(ULTIPANEL)
  #define MANUAL_FEEDRATE {120*60, 120*60, 18*60, 60}  // Feedrates for manual moves along X, Y, Z, E from panel
  #define ULTIPANEL_FEEDMULTIPLY  // Comment to disable setting feedrate multiplier via encoder
#endif

// @section extras

// minimum time in microseconds that a movement needs to take if the buffer is emptied.
#define DEFAULT_MINSEGMENTTIME        20000

// If defined the movements slow down when the look ahead buffer is only half full
#define SLOWDOWN

// Frequency limit
// See nophead's blog for more info
// Not working O
//#define XY_FREQUENCY_LIMIT  15

// Minimum planner junction speed. Sets the default minimum speed the planner plans for at the end
// of the buffer and all stops. This should not be much greater than zero and should only be changed
// if unwanted behavior is observed on a user's machine when running at very slow speeds.
#define MINIMUM_PLANNER_SPEED 0.05// (mm/sec)

// Microstep setting (Only functional when stepper driver microstep pins are connected to MCU.
#define MICROSTEP_MODES {16,16,16,16,16} // [1,2,4,8,16]

// Motor Current setting (Only functional when motor driver current ref pins are connected to a digital trimpot on supported boards)
#define DIGIPOT_MOTOR_CURRENT {135,135,135,135,135} // Values 0-255 (RAMBO 135 = ~0.75A, 185 = ~1A)

// uncomment to enable an I2C based DIGIPOT like on the Azteeg X3 Pro
//#define DIGIPOT_I2C
// Number of channels available for I2C digipot, For Azteeg X3 Pro we have 8
#define DIGIPOT_I2C_NUM_CHANNELS 8
// actual motor currents in Amps, need as many here as DIGIPOT_I2C_NUM_CHANNELS
#define DIGIPOT_I2C_MOTOR_CURRENTS {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}

//===========================================================================
//=============================Additional Features===========================
//===========================================================================

#define ENCODER_RATE_MULTIPLIER         // If defined, certain menu edit operations automatically multiply the steps when the encoder is moved quickly
#define ENCODER_10X_STEPS_PER_SEC 75    // If the encoder steps per sec exceeds this value, multiply steps moved x10 to quickly advance the value
#define ENCODER_100X_STEPS_PER_SEC 160  // If the encoder steps per sec exceeds this value, multiply steps moved x100 to really quickly advance the value

//#define CHDK 4        //Pin for triggering CHDK to take a picture see how to use it here http://captain-slow.dk/2014/03/09/3d-printing-timelapses/
#define CHDK_DELAY 50 //How long in ms the pin should stay HIGH before going LOW again

// @section lcd

#if ENABLED(SDSUPPORT)

  // Some RAMPS and other boards don't detect when an SD card is inserted. You can work
  // around this by connecting a push button or single throw switch to the pin defined
  // as SD_DETECT_PIN in your board's pins definitions.
  // This setting should be disabled unless you are using a push button, pulling the pin to ground.
  // Note: This is always disabled for ULTIPANEL (except ELB_FULL_GRAPHIC_CONTROLLER).
  #define SD_DETECT_INVERTED

  #define SD_FINISHED_STEPPERRELEASE true  //if sd support and the file is finished: disable steppers?
  #define SD_FINISHED_RELEASECOMMAND "M84 X Y Z E" // You might want to keep the z enabled so your bed stays in place.

  #define SDCARD_RATHERRECENTFIRST  //reverse file order of sd card menu display. Its sorted practically after the file system block order.
  // if a file is deleted, it frees a block. hence, the order is not purely chronological. To still have auto0.g accessible, there is again the option to do that.
  // using:
  //#define MENU_ADDAUTOSTART

  // Show a progress bar on HD44780 LCDs for SD printing
  //#define LCD_PROGRESS_BAR

  #if ENABLED(LCD_PROGRESS_BAR)
    // Amount of time (ms) to show the bar
    #define PROGRESS_BAR_BAR_TIME 2000
    // Amount of time (ms) to show the status message
    #define PROGRESS_BAR_MSG_TIME 3000
    // Amount of time (ms) to retain the status message (0=forever)
    #define PROGRESS_MSG_EXPIRE   0
    // Enable this to show messages for MSG_TIME then hide them
    //#define PROGRESS_MSG_ONCE
  #endif

  // This allows hosts to request long names for files and folders with M33
  //#define LONG_FILENAME_HOST_SUPPORT

  // This option allows you to abort SD printing when any endstop is triggered.
  // This feature must be enabled with "M540 S1" or from the LCD menu.
  // To have any effect, endstops must be enabled during SD printing.
  // With ENDSTOPS_ONLY_FOR_HOMING you must send "M120" to enable endstops.
  //#define ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED

#endif // SDSUPPORT

// for dogm lcd displays you can choose some additional fonts:
#if ENABLED(DOGLCD)
  // save 3120 bytes of PROGMEM by commenting out #define USE_BIG_EDIT_FONT
  // we don't have a big font for Cyrillic, Kana
  //#define USE_BIG_EDIT_FONT

  // If you have spare 2300Byte of progmem and want to use a
  // smaller font on the Info-screen uncomment the next line.
  //#define USE_SMALL_INFOFONT
#endif // DOGLCD


// @section more

// The hardware watchdog should reset the microcontroller disabling all outputs, in case the firmware gets stuck and doesn't do temperature regulation.
//#define USE_WATCHDOG

#if ENABLED(USE_WATCHDOG)
// If you have a watchdog reboot in an ArduinoMega2560 then the device will hang forever, as a watchdog reset will leave the watchdog on.
// The "WATCHDOG_RESET_MANUAL" goes around this by not using the hardware reset.
//  However, THIS FEATURE IS UNSAFE!, as it will only work if interrupts are disabled. And the code could hang in an interrupt routine with interrupts disabled.
//#define WATCHDOG_RESET_MANUAL
#endif

// @section lcd

// Babystepping enables the user to control the axis in tiny amounts, independently from the normal printing process
// it can e.g. be used to change z-positions in the print startup phase in real-time
// does not respect endstops!
//#define BABYSTEPPING
#if ENABLED(BABYSTEPPING)
  #define BABYSTEP_XY  //not only z, but also XY in the menu. more clutter, more functions
  #define BABYSTEP_INVERT_Z false  //true for inverse movements in Z
  #define BABYSTEP_MULTIPLICATOR 1 //faster movements
#endif

// @section extruder

// extruder advance constant (s2/mm3)
//
// advance (steps) = STEPS_PER_CUBIC_MM_E * EXTRUDER_ADVANCE_K * cubic mm per second ^ 2
//
// Hooke's law says:    force = k * distance
// Bernoulli's principle says:  v ^ 2 / 2 + g . h + pressure / density = constant
// so: v ^ 2 is proportional to number of steps we advance the extruder
//#define ADVANCE

#if ENABLED(ADVANCE)
  #define EXTRUDER_ADVANCE_K .0
  #define D_FILAMENT 1.75
  #define STEPS_MM_E 100.47095761381482
#endif

// @section extras

// Arc interpretation settings:
#define MM_PER_ARC_SEGMENT 1
#define N_ARC_CORRECTION 25

const unsigned int dropsegments=5; //everything with less than this number of steps will be ignored as move and joined with the next movement

// @section temperature

// Control heater 0 and heater 1 in parallel.
//#define HEATERS_PARALLEL

//===========================================================================
//================================= Buffers =================================
//===========================================================================

// @section hidden

// The number of linear motions that can be in the plan at any give time.
// THE BLOCK_BUFFER_SIZE NEEDS TO BE A POWER OF 2, i.g. 8,16,32 because shifts and ors are used to do the ring-buffering.
#if ENABLED(SDSUPPORT)
  #define BLOCK_BUFFER_SIZE 16   // SD,LCD,Buttons take more memory, block buffer needs to be smaller
#else
  #define BLOCK_BUFFER_SIZE 16 // maximize block buffer
#endif

// @section more

//The ASCII buffer for receiving from the serial:
#define MAX_CMD_SIZE 96
#define BUFSIZE 4

// Bad Serial-connections can miss a received command by sending an 'ok'
// Therefore some clients abort after 30 seconds in a timeout.
// Some other clients start sending commands while receiving a 'wait'.
// This "wait" is only sent when the buffer is empty. 1 second is a good value here.
//#define NO_TIMEOUTS 1000 // Milliseconds

// Some clients will have this feature soon. This could make the NO_TIMEOUTS unnecessary.
//#define ADVANCED_OK

// @section fwretract

// Firmware based and LCD controlled retract
// M207 and M208 can be used to define parameters for the retraction.
// The retraction can be called by the slicer using G10 and G11
// until then, intended retractions can be detected by moves that only extrude and the direction.
// the moves are than replaced by the firmware controlled ones.

//#define FWRETRACT  //ONLY PARTIALLY TESTED
#if ENABLED(FWRETRACT)
  #define MIN_RETRACT 0.1                //minimum extruded mm to accept a automatic gcode retraction attempt
  #define RETRACT_LENGTH 3               //default retract length (positive mm)
  #define RETRACT_LENGTH_SWAP 13         //default swap retract length (positive mm), for extruder change
  #define RETRACT_FEEDRATE 80*60            //default feedrate for retracting (mm/s)
  #define RETRACT_ZLIFT 0                //default retract Z-lift
  #define RETRACT_RECOVER_LENGTH 0       //default additional recover length (mm, added to retract length when recovering)
  //#define RETRACT_RECOVER_LENGTH_SWAP 0  //default additional swap recover length (mm, added to retract length when recovering from extruder change)
  #define RETRACT_RECOVER_FEEDRATE 8*60     //default feedrate for recovering from retraction (mm/s)
#endif

// Add support for experimental filament exchange support M600; requires display
#if ENABLED(ULTIPANEL)
  //#define FILAMENTCHANGEENABLE
  #if ENABLED(FILAMENTCHANGEENABLE)
    #define FILAMENTCHANGE_XPOS 3
    #define FILAMENTCHANGE_YPOS 3
    #define FILAMENTCHANGE_ZADD 10
    #define FILAMENTCHANGE_FIRSTRETRACT -2
    #define FILAMENTCHANGE_FINALRETRACT -100
  #endif
#endif

/******************************************************************************\
 * enable this section if you have TMC26X motor drivers.
 * you need to import the TMC26XStepper library into the arduino IDE for this
 ******************************************************************************/

// @section tmc

//#define HAVE_TMCDRIVER
#if ENABLED(HAVE_TMCDRIVER)

//#define X_IS_TMC
  #define X_MAX_CURRENT 1000  //in mA
  #define X_SENSE_RESISTOR 91 //in mOhms
  #define X_MICROSTEPS 16     //number of microsteps

//#define X2_IS_TMC
  #define X2_MAX_CURRENT 1000  //in mA
  #define X2_SENSE_RESISTOR 91 //in mOhms
  #define X2_MICROSTEPS 16     //number of microsteps

//#define Y_IS_TMC
  #define Y_MAX_CURRENT 1000  //in mA
  #define Y_SENSE_RESISTOR 91 //in mOhms
  #define Y_MICROSTEPS 16     //number of microsteps

//#define Y2_IS_TMC
  #define Y2_MAX_CURRENT 1000  //in mA
  #define Y2_SENSE_RESISTOR 91 //in mOhms
  #define Y2_MICROSTEPS 16     //number of microsteps

//#define Z_IS_TMC
  #define Z_MAX_CURRENT 1000  //in mA
  #define Z_SENSE_RESISTOR 91 //in mOhms
  #define Z_MICROSTEPS 16     //number of microsteps

//#define Z2_IS_TMC
  #define Z2_MAX_CURRENT 1000  //in mA
  #define Z2_SENSE_RESISTOR 91 //in mOhms
  #define Z2_MICROSTEPS 16     //number of microsteps

//#define E0_IS_TMC
  #define E0_MAX_CURRENT 1000  //in mA
  #define E0_SENSE_RESISTOR 91 //in mOhms
  #define E0_MICROSTEPS 16     //number of microsteps

//#define E1_IS_TMC
  #define E1_MAX_CURRENT 1000  //in mA
  #define E1_SENSE_RESISTOR 91 //in mOhms
  #define E1_MICROSTEPS 16     //number of microsteps

//#define E2_IS_TMC
  #define E2_MAX_CURRENT 1000  //in mA
  #define E2_SENSE_RESISTOR 91 //in mOhms
  #define E2_MICROSTEPS 16     //number of microsteps

//#define E3_IS_TMC
  #define E3_MAX_CURRENT 1000  //in mA
  #define E3_SENSE_RESISTOR 91 //in mOhms
  #define E3_MICROSTEPS 16     //number of microsteps

#endif

/******************************************************************************\
 * enable this section if you have L6470  motor drivers.
 * you need to import the L6470 library into the arduino IDE for this
 ******************************************************************************/

// @section l6470

//#define HAVE_L6470DRIVER
#if ENABLED(HAVE_L6470DRIVER)

//#define X_IS_L6470
  #define X_MICROSTEPS 16     //number of microsteps
  #define X_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define X_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define X_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define X2_IS_L6470
  #define X2_MICROSTEPS 16     //number of microsteps
  #define X2_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define X2_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define X2_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define Y_IS_L6470
  #define Y_MICROSTEPS 16     //number of microsteps
  #define Y_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define Y_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define Y_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define Y2_IS_L6470
  #define Y2_MICROSTEPS 16     //number of microsteps
  #define Y2_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define Y2_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define Y2_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define Z_IS_L6470
  #define Z_MICROSTEPS 16     //number of microsteps
  #define Z_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define Z_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define Z_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define Z2_IS_L6470
  #define Z2_MICROSTEPS 16     //number of microsteps
  #define Z2_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define Z2_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define Z2_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define E0_IS_L6470
  #define E0_MICROSTEPS 16     //number of microsteps
  #define E0_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define E0_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define E0_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define E1_IS_L6470
  #define E1_MICROSTEPS 16     //number of microsteps
  #define E1_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define E1_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define E1_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define E2_IS_L6470
  #define E2_MICROSTEPS 16     //number of microsteps
  #define E2_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define E2_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define E2_STALLCURRENT 1500 //current in mA where the driver will detect a stall

//#define E3_IS_L6470
  #define E3_MICROSTEPS 16     //number of microsteps
  #define E3_K_VAL 50          // 0 - 255, Higher values, are higher power. Be carefull not to go too high
  #define E3_OVERCURRENT 2000  //maxc current in mA. If the current goes over this value, the driver will switch off
  #define E3_STALLCURRENT 1500 //current in mA where the driver will detect a stall

#endif

#include "Conditionals.h"
#include "SanityCheck.h"

#endif //CONFIGURATION_ADV_H
