# Prusa MK3 for Marlin 3D Printer Firmware

This folder contains sample configuration files to allow use of the Marlin 3D
printer firmware with the Prusa MK3 3D printer.

## Documentation and Resources

A number of individuals are contributing to this effort. The best resource
for questions or feedback is the #Marlin channel in the  ["602 Wasteland" Discord server](https://discord.gg/R848Xn). 
Please read the rules there before posting.

The firmware itself is maintained on the [PrusaOwners/Marlin repository on Github](https://github.com/PrusaOwners/Marlin)

The team also contributes to the [PrusaOwners Wiki](https://github.com/PrusaOwners/prusaowners/wiki) 
which contains a wealth of information on the Prusa MK3 printer.

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

### Features Missing Vs Prusa Firmware

No all features present in the Prusa firmware have been implemented. Some are planned for future releases, but realistically some will never be added. In the table below, the main missing features are listed, along with the likelihood that this feature will be ported to this firmware.

|Feature                           |Planned?|Comments
|----------------------------------|--------|--------
|Crash Detection                   |Yes     | 
|Power Panic                       |No      |
|Filament Sensor V1                |Maybe   |Note: Support for the new filament sensor is present, but not enabled pending testing.
|Wizard-based XYZ Calibration      |No      |Marlin supports this feature by printing calibration targets, measuring, and using G-Code to calibrate.
|Belt tension estimator            |No      |This feature is extremely unreliable in the Prusa firmware (basically a placebo)
|PINDA temperature compensation    |Yes     |Pending validation that this feature actually works and is stable for any significant time.
|Selftest                          |Maybe   |Minus bogus belt tension feature
|Fan check (tachometer)            |Yes     |

### Additional Features over Prusa

This section lists some of the benefits over Prusa

|Feature                           |Comments
|----------------------------------|--------
|Linear Advance 1.5                |Improves print quality, less taxing on microcontroller
|S-Curve acceleration              |Improves print quality
|Junction Deviation                |Improves print quality
|Adaptive Step Smoothing           |Improves print quality
|Automatic Bed Leveling (Bilinear) |Prusa supports only a 3x3 grid. Our Marlin builds are configured for a 5x5 grid, which gives a good compromise between results and leveling time. Larger meshes are supported via a simple configuration change.
|Improved PINDA g-code support     |M850 has been moved to M199, and now supports a timeout, explicit cooldown and warmup, smoothed temperature measurements, and an LCD status display.
|Better hotend temp control        |Marlin more consistently controls the hotend temperature, and even better there are no mysterious 10C dips!
|More control via G-Code           |This firmware supports all of the Marlin G-Codes except for a few that are hardware dependent
|Better documentation              |Marlin has [reference guide](http://marlinfw.org/meta/gcode/) for all gcodes and a large and active community of users. 
|Increased modification support    |Marlin is designed to work on many different printers, with many different variations of hardware. For example, alternative Z probes such as BLTOUCH are already supported.

## Installation

The installation process consists of:

1. Build or download the firmware. Note: at this time, we're not yet making pre-built firmwares available. However, after we've had more time for testing by our "power users", we will make these available.
1. Clear EEPROM
1. Set your Live-Z / Probe Offset / Babystep value
1. Reset or measure and update your linear advance K-Factor
1. Update your slicer start-up g-code
1. (Optional) Measure and calibrate skew correction

Each step is covered in detail in the sections below.

### Building the firmware for the Prusa i3 MK3

Install PlatformIO, [standalone](https://platformio.org/) or as a [Plugin to Microsoft Visual Sudio Code](https://platformio.org/install/ide?install=vscode). Open the Marlin directory as a PlatformIO project. Edit the file "platformio.ini" in the main directory. 

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
** It is very important that you do not forget to copy these two files FROM the "example_configurations/Prusa/i3-MK3" and INTO the "Marlin" directory before you build. If you do not do that, you
will get an incorrect firmware and risk damaging your machine.

Build the firmware and upload to your printer. [The Marlin site has some instructions](http://marlinfw.org/docs/basics/install_platformio.html) if you need helo with this.

### Installing a pre-built firmware

When the pre-built firmwares are available, you may install the provided .hex file using whatever tool you use to update the printer currently (for example, the Octoprint Firmware Updater plug-in works fine).

### Clear EEPROM 

**IMPORTANT: After uploading the firmware to your printer, you should immediately clear the EEPROM It is important to do this when coming from the Prusa firmware, because the layout of the EEPROM storage is different for Prusa, and this can create some strange, potentially even damaging behavior.**

#### Clear the EEPROM via the menus

1. Click the wheel to open the top-level menu, the select the "Control" sub-menu followed by Restore Failsafe"
1. Select "Store Settings" from the "Control" menu

Note: After setting your Z-offset (see below), you probably want to perform another "Store Settings" to save it.

#### Clear the EEPROM via g-code

Send the printer the following g-codes: ```M502``` to load firmware defaults, then ```M500``` to safe these defaults to EEPROM. Marlin also recommends you then issue an ```M501``` to load the values back from EEPROM.

### Calibrate Z Offset

The Marlin term for "Live-Z" is "Probe Z Offset". I have found that the value I was using with the Prusa firmware worked perfectly in Marlin. To set this, you have two options:

1. Set via the M851 g-code. For example, to set the value to -0.725, you would send this g-code command to the printer: ```M851 Z-0.725``` . After doing this you should issue an ```M500``` to store the setting in EEPROM.
1. To set the value via the printer menus, click the wheel to open the top-level menu, then select "Control" to open the sub-menu, then select "Motion", then "Probe Z Offset". Use the wheel to select your value (remember, 
the value will be negative). Click the wheel when done. I recommend that you immediately save this new value in EEPROM so that it will be restored if the printer is reset or power cycled. You can do that by selecting 
"Store Settings" from the control menu.

If you don't have a good Live-Z value from the Prusa firmware, or want to remeasure, you should go through the usual process to do this (i.e. a test print). To adjust the value during a print, double-click the control wheel. 
This should open the Babystep menu, which acts exactly like Prusa's Live-Z adjustment. 

### Reset or Measure and Recalibrate Linear Advance K-Factor

Marlin offers the newer 1.5 version of [Linear
Advance](http://marlinfw.org/docs/features/lin_advance.html). This version
offers improved print quality as well as being less taxing on the printer's
microcontroller so that it is less likely to cause print issues because the
microcontroller is overloaded with work. However, Linear Advance 1.5 uses a
different range of K values. For the Linear Advance 1.5 offered by this Marlin
firmware, values range friom 0 to 2.0. For the old Linear Advance, values
generally ranged from 30 to 130. Therefore, if you are using Linear Advance, you will
need to update your K-factor values. Note that the filament slicer profiles included
in Prusa's Slic3r PE enable Linear Advance and set the K-factor in 
```Filament Settings / Custom G-code / Start G-code```. Look for a line like this:

    M900 K{if printer_notes=~/.*PRINTER_HAS_BOWDEN.*/}200{else}40{endif}; Filament gcode

You will want to change the "40" to be the appropriate number you measure via using [Marlin's
Linear Advance Calibration Pattern](http://marlinfw.org/tools/lin_advance/k-factor.html)

If you just want to get printing quickly, just set the value to zero (set the
"40" in the example above to "0"). This will disable linear advance, and you
can come back and tweak it later. I do highly recommend you use it, however,
as it really helps with oozing and stringing.

Also note that it's possible to update this line in the Slic3r Configuration
to have distinct values for printers depending on some printer attribute
defined in the Printer Settings, Notes configuration section (this is how the ```PRINTER_HAS_BOWDEN```
trick works in the Prusa-provided settings.

### Update your start-up g-code

At this point, you're almost ready to print. One last thing you need to do is
make some small changes to the g-code that your Slicer places in the start of
the file. A few of the g-code commands that Prusa uses have changed. In particular, you will need to:

1. Replace the G80 and G81 (mesh bed level, display mesh results) with a G29. Note that G29 both measures and displays the mesh results. 
1. If you are using M860 to manage the PINDA (Z) probe temperature, you will need to use M199 instead.
1. If your start-up g-code has an dependency on the mesh level leaving the probe at X=0,
you will need to add a line to your g-code after the mesh level to move to this position. Note that
the exact position the prusa firmware leaves the probe seems to have changed over time, so if you need to do any post-leveling 
movement in your start-up g-code, for example to print a prime line, I recommend you just put a full X/Y/Z move in rather than
assuming some position. For example, the start-up g-code I was using with Prusa firmware included these lines:

    G1 Y-3.0 F1000.0 ; go outside print area
    G1 X60.0 E9.0  F500.0 ; prime line
    G1 X100.0 E12.5  F500.0 ; prime line

I changed the first line to:

    G1 X0 Y-3.0 Z0.25 F1000.0 ; go outside print area

(TODO: Is there a way to print out the mesh without remeasuring?)

### Skew Compensation

Prusa has an XYZ calibration wizard that in theory calculates print bed/frame
skew and compensates for it somewhat. While Marlin does not have an automated
tool to configure this, it does have a way to perform skew compensation.  To
configure it, you will need to print some calibration targets, perform some
measurements, use a calculator or spreadsheet to compute the skew factor, and
then tell the printer about the skew factor values via G-code. This [video
from Chris Riley](https://www.youtube.com/watch?v=YfAb5IaHDSo) will walk you
through the print and measure steps. In Chris' video, however, he updates the
values directly in the Marlin  firmware configuration file, and then rebuilds
it. You should feel free to do this, but you will need to remember to do it
again if you later get an updated firmware from our site. So as an
alternative, I recommend setting the value via g-code M852.

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
|MOTHERBOARD                  |BOARD_EINSY_RAMBO        |Set controller board type
|CUSTOM_MACHINE_NAME          |"Prusa i3 MK3"           |Machine name displayed in LCD "Ready" message.
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
|Z_PROBE_OFFSET_FROM_EXTRUDER |0                        |Z-probe Z offset. Set to zero because we are using babystep z-offset, and this most closely matches the prusa behavior.
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
|PINDA_TEMP_SMOOTHING         |+                        |New option that filters noise in PINDA temperature measurements
|PINDA_TEMP_SMOOTHING_DIV_LOG2|6                        |Controls the running average used to filter noise in PINDA temperature measurements


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
|X_HOME_BUMP_MM               |0                        |Disable second homing bump on X?
|Y_HOME_BUMP_MM               |0                        |Disable second homing bump on Y?
|DEFAULT_STEPPER_DEACTIVE_TIME|60                       |Time in seconds after last move before steppers are automatically disabled
|HOME_AFTER_DEACTIVATE        |+                        |Requires rehoming after steppers are deactivated
|JUNCTION_DEVIATION           |+                        |Enable feature that provides smoother direction and speed transitions
|ADAPTIVE_STEP_SMOOTHING      |+                        |Increases resolution of multi-axis moves
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

