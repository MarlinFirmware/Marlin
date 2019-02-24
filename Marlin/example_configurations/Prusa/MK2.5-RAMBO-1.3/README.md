# Prusa MK3 for Marlin 3D Printer Firmware

This folder contains sample configuration files to allow use of the Marlin 3D
printer firmware with the Prusa MK2.5 3D printer.

***NOTE: At this time, this firmware has NOT BEEN TESTED on an actual printer. The authors do not have access to an MK2.5 to test. Use at your own risk.***

## Documentation and Resources

A number of individuals are contributing to this effort. The best resource
for questions or feedback is the #Marlin channel in the  ["602 Wasteland" Discord server](https://discord.gg/R848Xn). 
Please read the rules there before posting.

The firmware itself is maintained on the [PrusaOwners/Marlin repository on Github](https://github.com/PrusaOwners/Marlin)

The team also contributes to the [PrusaOwners Wiki](https://github.com/PrusaOwners/prusaowners/wiki) 
which contains a wealth of information on the Prusa MK3 printer.

## Why would I use this firmware?

An obvious question is: why would someone want to use this firmware over the
Prusa-provided firmware. Prusa's firmware itself is a "fork" of Marlin. At
some point years ago, someone at Prusa made a copy of the Marlin firmware, and
then Prusa proceeded to make changes to it for their purposes. Unfortunately,
as happens often with forks of open source projects, the two projects have continued to
diverge. This means that Prusa has not been able to take advantage of thew
new Marlin features, for example, Linear Advance 1.5.

So the first reason to choose this firmware is if you want to take advantage
of the newer features in recent releases of Marlin, which in all likelihood
Prusa will never support.

The second reason you might want this firmware is because we are prioritizing
stability over new features. While no-one writes bug-free code, our goal is to
maintain a stable core feature set and above all to listen to the community
and prioritize bug fixes over new features.

The third reason is that our intent is to keep this repo in sync with Marlin.
We will work hard to get any changes integrated back to Marlin, in the spirit
of open source, and also to ensure future Marlin fixes and features can be
readily integrated.

### Additional Features over Prusa

This section lists some of the benefits over Prusa

* Linear Advance 1.5
* S-Curve acceleration
* Junction Deviation
* Improved PINDA g-code support supports timeout, explicit cooldown and warmup, smoothed temperature measurements, serial console output integrated with other temperatures, and LCD status display.
* More consistent control of hotend temperature. No mysterious 10C dips!
* More control of the printer via G-Code. This firmware supports ALL of the Marlin G-Codes, except for a few that are hardware dependent.

TODO - lots to document here

## Project Goals

The team started this project with the following goals in mind:

* Create a Prusa compatible firmware based on the most recent stable release of Marlin, supporting all the features of Marlin that have become available since Prusa forked from that.
* Maintain a stable release without bugs in the core feature-set.
* Prioritize the core extensions needed to make a usable MK3 firmware. So for example, we focused on getting the PINDA probe working fully, but have a lower priority adding features like Power Panic.
* Provide a good base firmware for those wishing to develop a firmware for Prusa clones or spin-offs. The Marlin firmware is designed to be easily customizable for use across a huge range of printers. We want to provide a firmware that users of variants of the MK3 can easily tweak to support their specific needs.
* Listen to community feedback, and prefer high-priority bug fixes over adding new features.
* Where possible, work to merge code changes back into the Marlin repo. This means minimizing changes to Marlin to those that are needed to support actual features. It also means following the [Marlin Coding Standards](http://marlinfw.org/docs/development/coding_standards.html)

The following are not goals for this project:

* To create an exact clone of Prusa's firmware. For example, the LCD status screen has the Marlin layout, not Prusa's. The menus are Marlin menus, not Prusas. Changes to both those things are possible, but they make it harder to re-integrate our changes back into Marlin.
* To implement all the consumer-friendly features in the Prusa firmware. Prusa has some "wizards" for printer set-up. There are equivalent ways to achieve the same goals in Marlin, but they aren't simple "select and click" style options in the printer menus. At the same time, the results using the Marlin equivalent approach are usually better (bed skew compensation is a good example).

## Building the firmware for the Prusa i3 MK2.5

**NOTE: This firmware currently only supports MK2.5 that have the newer miniRambo 1.3a board. The older 1.0a board is not yet supported.** Adding support for this board should be extremely simple, but at this time we do not have a way to test it.

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
   
Then copy the configuration files "Configuration.h" and "Configuration_adv.h" from the directory "example_configurations/Prusa/i3-MK2.5-" to the "Marlin" directory overwriting the existing files there.

Build the firmware and upload to your printer.

**IMPORTANT: After uploading the firmware to your printer, you should immediately clear the EEPROM using the printer menus or the M502 followed by the M500 commands.** It is important to do this when coming from the Prusa firmware, because the layout of the EEPROM storage is different for Prusa, and this can create some strange, potentially even damaging behavior. 

## Summary of Changes

This section contains a description of each change to the Marlin configuration
files, Configuration.h and Configuration_adv.h.  The intent is to provide a
reference to allow multiple developers to work on the project with a quick
ramp-up on what changes were needed.

### Feature Changes

#### PINDA Probe

* Reading of the PINDA thermistor was added to thermalManager (thermistor.h, thermistor.cpp)
* A new g-code command M199 (M860 was taken already) was added (Marlin_main.cpp) to allow waiting for the PINDA to cool down or heat up
* M199 supports a timeout value
* M199 supports (optional) explicit control over whether cool-down or warm-up is desired
* LCD status display of PINDA temperature and target during M199 warm-up or cool-down
* M199 serial output includes full temperature output ala M105. This means that Octoprint will continue to update it's temperature graph during PINDA waiting
* PINDA temperature, which is notoriously noisy, is now filtered via a running average. This provides more predictable and consistent behavior, as well as making it easier to track progress in the serial console and LCD.

### Configuration.h

The following tables shows all changes from Marlin 1.1.9 default values.

|Symbol                       |Value                    |Notes
|-----------------------------|-------------------------|---
|BAUDRATE                     |115200                   |Lower communication speed of printer.
|MOTHERBOARD                  |BOARD_MINIRAMBO          |Set controller board type. In theory, you can set this to BOAR_MINIRAMBO_10A to support the older miniRambo 1.0a board.
|CUSTOM_MACHINE_NAME          |"Prusa i3 MK2.5"         |Machine name displayed in LCD "Ready" message.
|DEFAULT_NOMINAL_FILAMENT_DIA |1.75                     |Generally expected filament diameter.
|TEMP_SENSOR_0                |5                        |Hotend thermistor type (E3D)
|TEMP_SENSOR_BED              |1                        |Bed thermistor type
|PINDA_THERMISTOR             |+                        |Enable newly added support for the PINDA (Z-probe) thermistor
|TEMP_PINDA_PIN               |1                        |PINDA thermistor is on Analog input 1 (A1)
|TEMP_SENSOR_PINDA            |+                        |Leave this enabled
|PINDA_TEMP_SMOOTHING         |+                        |Enables smoothing of noise in PINDA temperature readings via a running average
|PINDA_TEMP_SMOOTHING_DIV_LOG2|6                        |PINDA temperature smoothing running average uses 1 part current reading with 2^6-1 (63) parts the previous average.
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
|X_DRIVER_TYPE                |A4988                    |Sets stepper motor driver type for axis.
|Y_DRIVER_TYPE                |A4988                    |Sets stepper motor driver type for axis.
|Z_DRIVER_TYPE                |A4988                    |Sets stepper motor driver type for axis.
|E0_DRIVER_TYPE               |A4988                    |Sets stepper motor driver type for axis.
|ENDSTOP_INTERRUPTS_FEATURE   |+                        |Enables hardware interrupts for end-stops (as opposed to polling)
|DEFAULT_AXIS_STEPS_PER_UNIT  |{ 100, 100, 3200/8, 133 }|Steps per mm for X,Y,Z,E axis.
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
|Z_PROBE_OFFSET_FROM_EXTRUDER |0                        |Z-probe Z offset. Set to zero because we are using babystep z-offset, and this most closely matches the prusa behavior.
|XY_PROBE_SPEED               |10000                    |X and Y axis travel speed (mm/m) between z-probes
|Z_PROBE_SPEED_SLOW           |(Z_PROBE_SPEED_FAST / 4) |Feedrate for the slow, more accurate movement during probing
|MULTIPLE_PROBING             |2                        |Number of probes at each point (doesn't Prusa do 3?)
|Z_CLEARANCE_DEPLOY_PROBE     |2                        |Z clearance between probing
|Z_CLEARANCE_BETWEEN_PROBES   |2                        |Z clearance between probing
|Z_CLEARANCE_MULTI_PROBE      |1                        |Z clearance between probing
|INVERT_Y_DIR                 |false                    |Do not invert Y stepper direction    
|INVERT_E0_DIR                |true                     |Invert E0 stepper direction    
|X_BED_SIZE                   |250                      |Print bed size (X)
|Y_BED_SIZE                   |210                      |Print bed size (Y)
|Y_MIN_POS                    |-4                       |Travel limits after homing
|Z_MIN_POS                    |0.15                     |Travel limits after homing
|Z_MAX_POS                    |210                      |Travel limits after homing. Note Z-axis is 10mm higher than actual value for Z calibration purposes.
|AUTO_BED_LEVELING_BILINEAR   |+                        |Bed leveling with bilinear interpolation
|GRID_MAX_POINTS_X            |5                        |5x5 grid for mesh bed leveling (GRID_MAX_POINTS_Y == GRID_MAX_POINTS_X)
|LEFT_PROBE_BED_POSITION      |35                       |Set area for probing of bed during mesh leveling
|RIGHT_PROBE_BED_POSITION     |238                      |Set area for probing of bed during mesh leveling
|FRONT_PROBE_BED_POSITION     |6                        |Set area for probing of bed during mesh leveling
|BACK_PROBE_BED_POSITION      |202                      |Set area for probing of bed during mesh leveling
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


### Configuration_adv.h

The following tables shows all changes from Marlin (1.1.9) default values.

|Symbol                       |Value                    |Notes
|-----------------------------|-------------------------|---
|PID_EXTRUSION_SCALING        |+                        |Enabled, but Kc set to small value. Can be adjusted with M301 g-code
|DEFAULT_Kc                   |1                        |Default hotend PID value Kc (scale by delayed extruder speed)
|AUTOTEMP                     |-                        |(Disable) Feature that allows linear ramp between temperatures.
|FAN_KICKSTART_TIME           |800                      |Start fan at full speed for this time (in milliseconds) before reducing to PWM value.
|E0_AUTO_FAN_PIN              |8                        |Sets auto-extruder fan pin and enables when extruder temp rises above configured value
|FAN_PIN                      |6                        |Overrides print-fan pin in board file (Prusa swaps fan connectors)
|FAN1_PIN                     |-1                       |Fan pins are defined by E0_AUTO_FAN_PIN and FAN_PIN, so we disable this one.
|DEFAULT_STEPPER_DEACTIVE_TIME|60                       |Time in seconds after last move before steppers are automatically disabled
|HOME_AFTER_DEACTIVATE        |+                        |Requires rehoming after steppers are deactivated
|JUNCTION_DEVIATION           |+                        |Enable feature that provides smoother direction and speed transitions
|ADAPTIVE_STEP_SMOOTHING      |+                        |Increases resolution of multi-axis moves
|PWM_MOTOR_CURRENT            |{ 540, 830, 500 }        |Motor currents, in milliamps, for MK2.5 (from Prusa firmware)
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
|MESH_MIN_X                   |35                       |Mesh probing area for unified bed leveling (UBL is disabled, but provided in case someone wants to use this)
|MESH_MIN_Y                   |6                        |Mesh probing area for unified bed leveling (UBL is disabled, but provided in case someone wants to use this)
|MESH_MAX_X                   |238                      |Mesh probing area for unified bed leveling (UBL is disabled, but provided in case someone wants to use this)
|MESH_MAX_Y                   |202                      |Mesh probing area for unified bed leveling (UBL is disabled, but provided in case someone wants to use this)
|MINIMUM_STEPPER_DIR_DELAY    |200                      |From A4982 datasheet
|MINIMUM_STEPPER_PULSE        |1                        |From A4982 datasheet
|MAXIMUM_STEPPER_RATE         |500000                   |From A4982 datasheet
|ADVANCED_PAUSE_FEATURE       |+                        |Experimental feature for filament change support and for parking the nozzle when paused
|PARK_HEAD_ON_PAUSE           |+                        |Park the nozzle during pause and filament change.
|HOME_BEFORE_FILAMENT_CHANGE  |+                        |Ensure homing has been completed prior to parking for filament change
|FILAMENT_LOAD_UNLOAD_GCODES  |+                        |Add M701/M702 Load/Unload G-codes, plus Load/Unload in the LCD Prepare menu.
