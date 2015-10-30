# Implemented G Codes

## G Codes

*  G0  - Same as G1
*  G1  - Coordinated Movement of X Y Z E
*  G2  - Clockwise ARC
*  G3  - Counter-Clockwise ARC
*  G4  - Dwell S[seconds] or P[milliseconds]
*  G10 - retract filament according to settings of M207
*  G11 - retract recover filament according to settings of M208
*  G28 - Home all Axis
*  G29 - Detailed Z-Probe, probes the bed at 3 points.  You must be at the home position for this to work correctly.
*  G30 - Single Z Probe, probes bed at current XY location.
*  G90 - Use Absolute Coordinates
*  G91 - Use Relative Coordinates
*  G92 - Set current position to cordinates given

## M Codes
*  M17  - Enable/Power all stepper motors
*  M18  - Same as M84
*  M20  - List SD card
*  M21  - Init SD card
*  M22  - Release SD card
*  M23  - Select SD file (M23 filename.g)
*  M24  - Start/resume SD print
*  M25  - Pause SD print
*  M26  - Set SD position in bytes (M26 S12345)
*  M27  - Report SD print status
*  M28  - Start SD write (M28 filename.g)
*  M30  - Delete file from SD (M30 filename.g)
*  M31  - Output time since last M109 or SD card start to serial
*  M32  - Select file and start SD print (Can be used when printing from SD card)
*  M42  - Change pin status via gcode Use M42 Px Sy to set pin x to value y, when omitting Px the onboard led will be used.
*  M82  - Set E codes absolute (default)
*  M83  - Set E codes relative while in Absolute Coordinates (G90) mode
*  M84  - Disable steppers until next move
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
*  M120 - Disable endstops
*  M121 - Enable endstops
*  M140 - Set bed target temp
*  M190 - Sxxx Wait for bed current temp to reach target temp. Waits only when heating
*         Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
*  M200 - D[millimeters]- set filament diameter and set E axis units to cubic millimeters (use S0 to set back to millimeters).
*  M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
*  M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
*  M204 - Set default acceleration: S normal moves T filament only moves (M204 S3000 T7000) im mm/sec^2  also sets minimum segment time in ms (B20000) to prevent buffer underruns and M20 minimum feedrate
*  M205 - advanced settings:  minimum travel speed S=while printing T=travel only,  B=minimum segment time X= maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
*  M206 - set additional homing offset
*  M218 - set hotend offset (in mm): T[extruder_number] X[offset_on_X] Y[offset_on_Y]
*  M220 - S[factor in percent] - set speed factor override percentage
*  M221 - S[factor in percent] - set extrude factor override percentage
*  M226 - Wait until the specified pin reaches the state required
*  M240 - Trigger a camera to take a photograph
*  M250 - Set LCD contrast value
*  M280 - Position an RC Servo P[index] S[angle/microseconds], ommit S to report back current angle
*  M300 - Play beep sound S[frequency Hz] P[duration ms]
*  M301 - Set PID parameters P I and D
*  M302 - Allow cold extrudes
*  M303 - PID relay autotune S[temperature] sets the target temperature. (default target temperature = 150C)
*  M304 - Set bed PID parameters P I and D
*  M400 - Finish all moves
*  M500 - stores paramters in EEPROM
*  M501 - reads parameters from EEPROM (if you need reset them after you changed them temporarily).
*  M502 - reverts to the default "factory settings".  You still need to store them in EEPROM afterwards if you want to.
*  M503 - print the current settings (from memory not from EEPROM)
*  M600 - Pause for filament change X[pos] Y[pos] Z[relative lift] E[initial retract] L[later retract distance for removal]
*  M700 - Level Plate
*  M701 - Load Filament
*  M702 - Unload Filament
*  M703 - Get Board Type
*  M710 - Erase the EEPROM and reset the board
*  M800 - Fire start print procedure
*  M801 - Fire end print procedure
*  M907 - Set digital trimpot motor current using axis codes.
*  M908 - Control digital trimpot directly.
*  M928 - Start SD logging (M928 filename.g) - ended by M29
*  M999 - Restart after being stopped by error
