# Implemented G Codes

## G Codes

*  G0  -> G1
*  G1  - Coordinated Movement X Y Z E
*  G2  - CW ARC
*  G3  - CCW ARC
*  G4  - Dwell S[seconds] or P[milliseconds]
*  G10 - retract filament according to settings of M207
*  G11 - retract recover filament according to settings of M208
*  G28 - Home all Axis
*  G29 - Detailed Z-Probe, probes the bed at 3 points.  You must be at the home position for this to work correctly.
*  G30 - Single Z Probe, probes bed at current XY location.
*  G31 - Dock Z Probe sled (if enabled)
*  G32 - Undock Z Probe sled (if enabled)
*  G90 - Use Absolute Coordinates
*  G91 - Use Relative Coordinates
*  G92 - Set current position to cordinates given

## M Codes
*  M0   - Unconditional stop - Wait for user to press a button on the LCD (Only if ULTRA_LCD is enabled)
*  M1   - Same as M0
*  M17  - Enable/Power all stepper motors
*  M18  - Disable all stepper motors; same as M84
*  M20  - List SD card
*  M21  - Init SD card
*  M22  - Release SD card
*  M23  - Select SD file (M23 filename.g)
*  M24  - Start/resume SD print
*  M25  - Pause SD print
*  M26  - Set SD position in bytes (M26 S12345)
*  M27  - Report SD print status
*  M28  - Start SD write (M28 filename.g)
*  M29  - Stop SD write
*  M30  - Delete file from SD (M30 filename.g)
*  M31  - Output time since last M109 or SD card start to serial
*  M32  - Select file and start SD print (Can be used when printing from SD card)
*  M42  - Change pin status via gcode Use M42 Px Sy to set pin x to value y, when omitting Px the onboard led will be used.
*  M80  - Turn on Power Supply
*  M81  - Turn off Power Supply
*  M82  - Set E codes absolute (default)
*  M83  - Set E codes relative while in Absolute Coordinates (G90) mode
*  M84  - Disable steppers until next move, or use S[seconds] to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
*  M85  - Set inactivity shutdown timer with parameter S[seconds]. To disable set zero (default)
*  M92  - Set axis_steps_per_unit - same syntax as G92
*  M104 - Set extruder target temp
*  M105 - Read current temp
*  M106 - Fan on
*  M107 - Fan off
*  M109 - Sxxx Wait for extruder current temp to reach target temp. Waits only when heating
*         Rxxx Wait for extruder current temp to reach target temp. Waits when heating and cooling
*  M112 - Emergency stop
*  M114 - Output current position to serial port
*  M115 - Capabilities string
*  M117 - display message
*  M119 - Output Endstop status to serial port
*  M126 - Solenoid Air Valve Open (BariCUDA support by jmil)
*  M127 - Solenoid Air Valve Closed (BariCUDA vent to atmospheric pressure by jmil)
*  M128 - EtoP Open (BariCUDA EtoP = electricity to air pressure transducer by jmil)
*  M129 - EtoP Closed (BariCUDA EtoP = electricity to air pressure transducer by jmil)
*  M140 - Set bed target temp
*  M190 - Sxxx Wait for bed current temp to reach target temp. Waits only when heating
*         Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
*  M200 - D[millimeters]- set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
*  M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
*  M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in Marlin!!
*  M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
*  M204 - Set default acceleration: P for Printing moves, R for Retract only (no X, Y, Z) moves and T for Travel (non printing) moves (ex. M204 P800 T3000 R9000) in mm/sec^2
*  M205 - advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
*  M206 - set additional homing offset
*  M207 - set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop], stays in mm regardless of M200 setting
*  M208 - set recover=unretract length S[positive mm surplus to the M207 S*] F[feedrate mm/min]
*  M209 - S[1=true/0=false] enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction.
*  M218 - set hotend offset (in mm): T[extruder_number] X[offset_on_X] Y[offset_on_Y]
*  M220 - S[factor in percent] - set speed factor override percentage
*  M221 - S[factor in percent] - set extrude factor override percentage
*  M240 - Trigger a camera to take a photograph
*  M280 - Position an RC Servo P[index] S[angle/microseconds], ommit S to report back current angle
*  M300 - Play beep sound S[frequency Hz] P[duration ms]
*  M301 - Set PID parameters P I and D
*  M302 - Allow cold extrudes
*  M303 - PID relay autotune S[temperature] sets the target temperature. (default target temperature = 150C)
*  M304 - Set bed PID parameters P I and D
*  M350 - Set microstepping mode.
*  M351 - Toggle MS1 MS2 pins directly.
*  M400 - Finish all moves
*  M401 - Lower z-probe if present
*  M402 - Raise z-probe if present
*  M404 - N[dia in mm] Enter the nominal filament width (3mm, 1.75mm) or will display nominal filament width without parameters
*  M405 - Turn on Filament Sensor extrusion control.  Optional D[delay in cm] to set delay in centimeters between sensor and extruder
*  M406 - Turn off Filament Sensor extrusion control
*  M407 - Displays measured filament diameter
*  M500 - stores paramters in EEPROM
*  M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).
*  M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
*  M503 - print the current settings (from memory not from EEPROM)
*  M540 - Use S[0|1] to enable or disable the stop SD card print on endstop hit (requires ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
*  M600 - Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
*  M907 - Set digital trimpot motor current using axis codes.
*  M908 - Control digital trimpot directly.
*  M928 - Start SD logging (M928 filename.g) - ended by M29
*  M999 - Restart after being stopped by error

# Comments

Comments start at a `;` (semicolon) and end with the end of the line:

    N3 T0*57 ; This is a comment
    N4 G92 E0*67
    ; So is this
    N5 G28*22

(example taken from the [RepRap wiki](http://reprap.org/wiki/Gcode#Comments))

If you need to use a literal `;` somewhere (for example within `M117`), you can escape semicolons with a `\`
(backslash):

     M117 Hello \;)

`\` can also be used to escape `\` itself, if you need a literal `\` in front of a `;`:

    M117 backslash: \\;and a comment

Please note that hosts should strip any comments before sending GCODE to the printer in order to save bandwidth.