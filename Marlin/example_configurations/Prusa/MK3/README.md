# Prusa MK3 for Marlin 3D Printer Firmware

This folder contains sample configuration files to allow use of the Marlin 3D
printer firmware with the Prusa MK3 3D printer.

## Documentation and Resources

A number of individuals are contributing to this effort. The best resource
for questions or feedback is the #Marlin channel in the  ["602 Wasteland" Discord server](https://discord.gg/R848Xn). 
Please read the rules there before posting.

The team also contributes to the [PrusaOwners Wiki](https://github.com/PrusaOwners/prusaowners/wiki) 
which contains a wealth of information on the Prusa MK3 printer.

## Building the firmware for the Prusa i3 MK3

Install PlatformIO (e.g. as a Plugin to Microsoft Visual Sudio Code). Open the Marlin directory as a PlatformIO project. Edit the file "platformio.ini" in the main directory. 

    [platformio]
    ...
    env_default = rambo
    
    ...
    #
    # RAMBo
    #
    [env:rambo]
    ...
    monitor_speed     = 115200
   
Then copy the configuration files "Configuration.h" and "Configuration_adv.h" from the directory "example_configurations/Prusa/i3-MK3" to the "Marlin" directory overwriting the existing files there.

Build the firmware and upload to your EINSY board.

## Summary of Changes

This section contains a description of each change to the Marlin configuration
files, Configuration.h and Configuration_adv.h.  The intent is to provide a
reference to allow multiple developers to work on the project with a quick
ramp-up on what changes were needed.

### Configuration.h

The following tables shows all changes from Marlin 1.1.9 default values.

|Symbol                       |Value                    |Notes
|-----------------------------|-------------------------|---
|BAUDRATE                     |115200                   |Lower communication speed of printer.
|MOTHERBOARD BOARD_EINSY_RAMBO|+                        |Set controller board type
|CUSTOM_MACHINE_NAME          |"Prusa i3 MK3"           |Machine name displayed in LCD "Ready" message. Note: Do we want to change this?
|DEFAULT_NOMINAL_FILAMENT_DIA |1.75                     |Generally expected filament diameter.
|TEMP_SENSOR_0                |5                        |Hotend thermistor type (E3D)
|TEMP_SENSOR_BED              |1                        |Bed thermistor type
|HEATER_0_MAXTEMP             |305                      |Maximum allowed temperature for hotend.
|BED_MAXTEMP                  |125                      |Maximum allowed temperature for bed.
|DEFAULT_Kp                   |16.13                    |Hotend PID constant - Proportional              
|DEFAULT_Kd                   |56.23                    |Hotend PID constant - Integral
|DEFAULT_Ki                   |1.1625                   |Hotend PID constant - Derivative
|PIDTEMPBED                   |+                        |Enables PID control of Bed temperature
|DEFAULT_bedKp                |10.00                    |Bed PID constant - Proportional              
|DEFAULT_bedKd                |0.023                    |Bed PID constant - Integral
|DEFAULT_bedKi                |305.4                    |Bed PID constant - Derivative
|EXTRUDE_MINTEMP              |175                      |Minimum allowed hotend temperature for extrusion. Value was 170.
|ENDSTOPPULLUP_XMIN           |+                        |Remapped to X_DIAG_PIN in pins_EINSY_RAMBO.h. This effectively enables pull-up resistor on the X-axis TMC driver DIAG pin.
|ENDSTOPPULLUP_YMIN           |+                        |Remapped to Y_DIAG_PIN in pins_EINSY_RAMBO.h. This effectively enables pull-up resistor on the Y-axis TMC driver DIAG pin.
|X_MIN_ENDSTOP_INVERTING      |true                     |Inverts polarity of X endstop
|Y_MIN_ENDSTOP_INVERTING      |true                     |Inverts polarity of Y endstop
|X_DRIVER_TYPE                |TMC2130                  |Sets stepper motor driver type for axis.
|Y_DRIVER_TYPE                |TMC2130                  |Sets stepper motor driver type for axis.
|Z_DRIVER_TYPE                |TMC2130                  |Sets stepper motor driver type for axis.
|E0_DRIVER_TYPE               |TMC2130                  |Sets stepper motor driver type for axis.
|ENDSTOP_INTERRUPTS_FEATURE   |+                        |Enables hardware interrupts for end-stops (as opposed to polling)
|DEFAULT_AXIS_STEPS_PER_UNIT  |{ 100, 100, 3200/8, 280 }|Steps per mm for X,Y,Z,E axis.
|DEFAULT_MAX_FEEDRATE         |{ 200, 200, 12, 120 }    |Default max feedrate for X,Y,Z,E axis.
|DEFAULT_MAX_ACCELERATION     |{ 1000, 1000, 200, 5000 }|Default max acceleration for X,Y,Z,E axis.
|DEFAULT_ACCELERATION         |1250                     |Default acceleration for printing moves
|DEFAULT_RETRACT_ACCELERATION |1250                     |Default acceleration for retraction
|DEFAULT_TRAVEL_ACCELERATION  |1250                     |Default acceleration for non-printing moves
|DEFAULT_ZJERK                |0.4                      |Default jerk for Z moves
|DEFAULT_EJERK                |2.5                      |Default jerk for E moves
|S_CURVE_ACCELERATION         |+                        |Enables S-curve acceleration (NOTE: Let's disable for now)
|FIX_MOUNTED_PROBE            |+                        |Z-probe (PINDA2) is fixed and does not require automatic deployment
|X_PROBE_OFFSET_FROM_EXTRUDER |23                       |Z-probe X offset: -left  +right  [of the nozzle]
|Y_PROBE_OFFSET_FROM_EXTRUDER |5                        |Z-probe Y offset: -front +behind [the nozzle]
|Z_PROBE_OFFSET_FROM_EXTRUDER |-0.4                     |Z-probe Z offset: -below +above  [the nozzle]
|XY_PROBE_SPEED               |10000                    |X and Y axis travel speed (mm/m) between z-probes
|Z_PROBE_SPEED_SLOW           |(Z_PROBE_SPEED_FAST / 4) |Feedrate for the slow, more accurate movement during probing
|MULTIPLE_PROBING             |2                        |Number of probes at each point (doesn't Prusa do 3?)
|Z_CLEARANCE_DEPLOY_PROBE     |2                        |Z clearance between probing
|Z_CLEARANCE_BETWEEN_PROBES   |2                        |Z clearance between probing
|Z_CLEARANCE_MULTI_PROBE      |1                        |Z clearance between probing
|INVERT_X_DIR                 |true                     |Invert X stepper direction   
|INVERT_Y_DIR                 |false                    |Do not invert Y stepper direction    
|INVERT_Z_DIR                 |true                     |Invert Z stepper direction
|X_BED_SIZE                   |250                      |Print bed size (X)
|Y_BED_SIZE                   |210                      |Print bed size (Y)
|Y_MIN_POS                    |-4                       |Travel limits after homing
|Z_MIN_POS                    |0.15                     |Travel limits after homing
|X_MAX_POS                    |255                      |Travel limits after homing
|Y_MAX_POS                    |212.5                    |Travel limits after homing
|Z_MAX_POS                    |210                      |Travel limits after homing
|AUTO_BED_LEVELING_BILINEAR   |+                        |Bed leveling with bilinear interpolation
|GRID_MAX_POINTS_X            |5                        |5x5 grid for mesh bed leveling (GRID_MAX_POINTS_Y == GRID_MAX_POINTS_X)
|Z_SAFE_HOMING                |+                        |Only allows Z homing after X/Y homing and if motors have not timed out.
|HOMING_FEEDRATE_Z            |8*60                     |Homing speed for Z (8 mm/s, 480 mm/m)
|SKEW_CORRECTION_GCODE        |+                        |Enables user to set skew at runtime via M852
|EEPROM_SETTINGS              |+                        |Enables storing and retrieving settings to/from EEPROM with M500 and M501
|PREHEAT_1_TEMP_HOTEND        |215                      |Hotend temp for Preheat menu 1 (String set to PLA in Configuration_adv.h)
|PREHEAT_1_TEMP_BED           |60                       |Bed temp for Preheat menu 1 (String set to PLA in Configuration_adv.h)
|PREHEAT_2_TEMP_HOTEND        |255                      |Hotend temp for Preheat menu 2 (String set to ABS in Configuration_adv.h)
|PREHEAT_2_TEMP_BED           |100                      |Bed temp for Preheat menu 2 (String set to ABS in Configuration_adv.h)
|NOZZLE_PARK_FEATURE          |+                        |Enables nozzle park on idle or G27
|SDSUPPORT                    |+                        |Enable SD card support
|SD_CHECK_AND_RETRY           |+                        |Enables CRC checks and retries on SD card communication
|ENCODER_PULSES_PER_STEP      |4                        |Number of rotary encoder pulses to produce one step
|REVERSE_ENCODER_DIRECTION    |+                        |Reverses rotary encoder direction
|INDIVIDUAL_AXIS_HOMING_MENU  |+                        |Adds individual axis homing items to the menu
|SPEAKER                      |+                        |MK3 has a speaker that can produce tones
|REPRAP_DISCOUNT_SMART_CONTROLLER |+                    |Using a RepRapDiscount Smart LCD/Controller


#### Notes and Open Questions
* Einsy has an on-board thermistor, referred to as "Ambient" in Prusa firmware. How do we enable this?
* Should we disable S-curve acceleration for now? It seems still an experimental feaure.
* Is there any way to add more preheat settings w/o modifying code? If not, perhaps we should switch second menu item to PETG, since that is more common than ABS these days.
* NOZZLE_PARK_FEATURE - is this enabled in Prusa FW?


### Configuration_adv.h

The following tables shows all changes from Marlin (1.1.9) default values.

|Symbol                       |Value                    |Notes
|-----------------------------|-------------------------|---
|PID_EXTRUSION_SCALING        |+                        |Enabled, but Kc set to small value. Can be adjusted with M301 g-code
|DEFAULT_Kc                   |1                        |Default hotend PID value Kc (scale by delayed extruder speed)
|AUTOTEMP                     |-                        |(Disable) Feature that allows linear ramp between temperatures.
|FAN_KICKSTART_TIME           |800                      |Start fan at full speed for this time (in milliseconds) before reducing to PWM value.
|E0_AUTO_FAN_PIN              |8                        |Sets auto-extruder fan enable when extruder temp rises above configured value
|X_HOME_BUMP_MM               |0                        |Disable second homing bump on X?
|Y_HOME_BUMP_MM               |0                        |Disable second homing bump on Y?
|DEFAULT_STEPPER_DEACTIVE_TIME|60                       |Time in seconds after last move before steppers are automatically disabled
|HOME_AFTER_DEACTIVATE        |+                        |Requires rehoming after steppers are deactivated
|ADAPTIVE_STEP_SMOOTHING      |+                        |Increases resolution of multi-axis moves
|PWM_MOTOR_CURRENT            |{ 400, 750, 750 }        |*Need to validate*
|DIGIPOT_MOTOR_CURRENT        |{ 135,135,135,135,135 }  |*Need to validate*
|LCD_INFO_MENU                |+                        |Enable printer info page in LCD menu
|STATUS_MESSAGE_SCROLLING     |+                        |Enable status message scrolling for long messages
|LCD_TIMEOUT_TO_STATUS        |30000                    |Menu timeout (in ms)
|SDCARD_SORT_ALPHA            |+                        |Sort SD card files in alphabetical order
|SDSORT_LIMIT                 |100                      |Maximum number of sorted items for SD card file list (expensive...)
|BABYSTEPPING                 |+                        |Enables Z babystepping, which is the Marlin name for Live-Z
|BABYSTEP_ZPROBE_OFFSET       |+                        |Combines M851 and babystepping (makes it closer to Prusa's Live-Z)
|DOUBLECLICK_FOR_Z_BABYSTEPPING|+                       |Double-click on status screen goes to Z babystepping menu
|DOUBLECLICK_MAX_INTERVAL     |2000                     |Increases slightly the allowed delay to detect a double-click. This is the value that TH3D is using in their Marlin firmware.
|LIN_ADVANCE                  |+                        |Enable linear advance (woohoo!)
|MESH_MIN_X                   |35                       |X axis lower inset for mesh bed leveling
|MESH_MIN_Y                   |6                        |Y axis lower inset for mesh bed leveling
|MESH_MAX_X                   |238                      |X axis upper inset (as bed size minus upper inset)
|MESH_MAX_Y                   |202                      |Y axis upper inset (as bed size minus upper inset)
|MINIMUM_STEPPER_DIR_DELAY    |20                       |Set to value listed as appropriate for TMC2xxx drivers
|MINIMUM_STEPPER_PULSE        |0                        |Set to value listed as appropriate for TMC2xxx drivers
|MAXIMUM_STEPPER_RATE         |400000                   |Set to the maximum value listed as appropriate for TMC2xxx drivers
|ADVANCED_PAUSE_FEATURE       |+                        |Experimental feature for filament change support and for parking the nozzle when paused
|PARK_HEAD_ON_PAUSE           |+                        |Park the nozzle during pause and filament change.
|HOME_BEFORE_FILAMENT_CHANGE  |+                        |Ensure homing has been completed prior to parking for filament change
|FILAMENT_LOAD_UNLOAD_GCODES  |+                        |Add M701/M702 Load/Unload G-codes, plus Load/Unload in the LCD Prepare menu.

The following table lists the changes to TMC2130 motor driver settings (also in Configuration_adv.h)

|Symbol                       |Value                    |Notes
|-----------------------------|-------------------------|---
|R_SENSE                      |0.22                     |Current sense resistor value in ohms.
|HOLD_MULTIPLIER              |1.0                      |Scales down the holding current from run current
|X_CURRENT / X2_CURRENT       |282                      |X stepper driver current (holding and running) in RMS mA
|Y_CURRENT / Y2_CURRENT       |348                      |Y stepper driver current (holding and running) in RMS mA
|Z_CURRENT / Z2_CURRENT       |530                      |Z stepper driver current (holding and running) in RMS mA
|E0_CURRENT .. E4_CURRENT     |514                      |Extruder 0..4 stepper driver current (holding and running) in RMS mA
|E0_MICROSTEPS .. E4_MICROSTEPS|32                      |Extruder 0..4 stepper driver microsteps 
|STEALTHCHOP                  |-                        |Disable StealChop (ultra-quiet stepping mode). SpreadCycle will be used instead.
|MONITOR_DRIVER_STATUS        |+                        |Monitor Trinamic TMC2130 drivers for error conditions
|SENSORLESS_HOMING            |+                        |Enable sensorless homing via TMC stallGuard2 feature.
|X_HOMING_SENSITIVITY         |3                        |X homing sensitivity. Higher values make system less sensitive.
|Y_HOMING_SENSITIVITY         |3                        |Y homing sensitivity. Higher values make system less sensitive.
|Z_HOMING_SENSITIVITY         |3                        |Z homing sensitivity. Higher values make system less sensitive.
|TMC_Z_CALIBRATION            |+                        |Enables Z calibration that Prusa does (ram into top of Z axis, then home Z via PINDA)
|CALIBRATION_CURRENT          |348                      |Z steppers are set to this current during Z calibration
|TMC_ADV                      |(see Notes)              |This is a way of adding additional calls to tmc2130 class methods to set various custom values. See Configuration_adv.h for details (too numerous to list here)

