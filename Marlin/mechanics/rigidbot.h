// Predefined Mechanics for kits/pre-assembled printers
//
// Invent-A-Part RigidBot
#define PREDEFINED_MECHANICS

#define ENDSTOPPULLUPS
const bool X_MIN_ENDSTOP_INVERTING = true;
const bool Y_MIN_ENDSTOP_INVERTING = true;
const bool Z_MIN_ENDSTOP_INVERTING = true;
const bool X_MAX_ENDSTOP_INVERTING = true;
const bool Y_MAX_ENDSTOP_INVERTING = true;
const bool Z_MAX_ENDSTOP_INVERTING = true;

#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0

#define DISABLE_X false
#define DISABLE_Y false
#define DISABLE_Z false
#define DISABLE_E false
#define DISABLE_INACTIVE_EXTRUDER true

#define INVERT_X_DIR true
#define INVERT_Y_DIR false
#define INVERT_Z_DIR false
#define INVERT_E0_DIR true
#define INVERT_E1_DIR true
#define INVERT_E2_DIR false

#define X_HOME_DIR -1
#define Y_HOME_DIR -1
#define Z_HOME_DIR -1

#define min_software_endstops true
#define max_software_endstops true

#if EXTRUDERS == 1
  #define X_DUAL_REDUCTION 0
#elif EXTRUDERS == 2
  #define X_DUAL_REDUCTION 57
#else
  #define X_DUAL_REDUCTION 350 // Prevent head crashes if EXTRUDERS > 2
#endif

#if RIGIDBOT_SIZE == 1 // 10x10 Regular
  #define X_MAX_POS (254 - X_DUAL_REDUCTION)
  #define Y_MAX_POS 248
  #define Z_MAX_POS 254
#elif RIGIDBOT_SIZE == 2 // 12x16 Big
  #define X_MAX_POS (406 - X_DUAL_REDUCTION)
  #define Y_MAX_POS 304
  #define Z_MAX_POS 254
#endif
#define X_MIN_POS 0
#define Y_MIN_POS 0
#define Z_MIN_POS 0

#define MANUAL_X_HOME_POS 0
#define MANUAL_Y_HOME_POS 0
#define MANUAL_Z_HOME_POS 0

#define NUM_AXIS 4
#define HOMING_FEEDRATE {50*60, 50*60, 15*60, 0}

#define DEFAULT_AXIS_STEPS_PER_UNIT   {44.3090,22.1545,1600,53.5}  // default steps per unit for Ultimaker
#define DEFAULT_MAX_FEEDRATE          {500, 500, 4, 25}    // (mm/sec)
#define DEFAULT_MAX_ACCELERATION      {800,600,100,10000}    // X, Y, Z, E maximum start speed for accelerated moves. E default values are good for Skeinforge 40+, for older versions raise them a lot.

#define DEFAULT_ACCELERATION          600    // X, Y, Z and E max acceleration in mm/s^2 for printing moves
#define DEFAULT_RETRACT_ACCELERATION  1000   // X, Y, Z and E max acceleration in mm/s^2 for retracts

#define DEFAULT_XYJERK                5.0    // (mm/sec)
#define DEFAULT_ZJERK                 0.4     // (mm/sec)
#define DEFAULT_EJERK                 5.0    // (mm/sec)
