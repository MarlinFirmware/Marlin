## Implemented G Codes

### Movement G and M Codes
```
G0   -> G1
G1   - Coordinated Movement X Y Z E
G2   - CW ARC
G3   - CCW ARC
G4   - Dwell S[seconds] or P[milliseconds]
G92  - Set the "current position" to coordinates: X<xpos> Y<ypos> Z<zpos> E<epos>
M0   - Wait for user, with optional prompt message (requires LCD controller)
       M0 Click When Ready ; show "Click When Ready" until the button is pressed.
M1   - Same as M0
M400 - Finish all moves
M999 - Restart after being stopped by error
```
### SD Card M Codes
```
M20  - List SD card
M21  - Init SD card
M22  - Release SD card
M23  - Select SD file (M23 filename.g)
M24  - Start/resume SD print
M25  - Pause SD print
M26  - Set SD position in bytes (M26 S12345)
M27  - Report SD print status
M28  - Start SD write (M28 filename.g)
M29  - Stop SD write
M30  - Delete file from SD (M30 filename.g)
M31  - Output time since last M109 or SD card start to serial
M32  - Select file and start SD print (Can be used _while_ printing from SD card files):
       syntax "M32 /path/filename#", or "M32 S<startpos bytes> !filename#"
       Call gcode file : "M32 P !filename#" and return to caller file after finishing (similar to #include).
       The '#' is necessary when calling from within sd files, as it stops buffer prereading
M540 - Enable/Disable "Stop SD Print on Endstop Hit" (req. ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
       M540 S<0|1>
M928 - Start SD logging (M928 filename.g) - ended by M29
```
### Hardware Control
```
M42  - Change pin status via gcode: P<pin> S<value>. If P is omitted the onboard LED pin will be used.
       M42 P9 S1 ; set PIN 9 to 1. 
M80  - Turn on Power Supply
M81  - Turn off Power Supply
M84  - Disable steppers until next move,
       or use S<seconds> to specify an inactivity timeout, after which the steppers will be disabled.  S0 to disable the timeout.
M85  - Set inactivity shutdown timer with parameter S<seconds>. Disable with "M85" or "M85 S0".
M112 - Emergency stop. Requires hardware reset!!
M226 - Wait for a pin to be in some state: P<pin number> S<pin state>
```
### Temperature M Codes
```
M104 - Set extruder target temp
M105 - Read current temp
M106 - Fan on
M107 - Fan off
M109 - Sxxx Wait for extruder current temp to reach target temp. Waits only when heating
       Rxxx Wait for extruder current temp to reach target temp. Waits when heating and cooling
       IF AUTOTEMP is enabled, S<mintemp> B<maxtemp> F<factor>. Exit autotemp by any M109 without F
M140 - Set bed target temp
M190 - Sxxx Wait for bed current temp to reach target temp. Waits only when heating
       Rxxx Wait for bed current temp to reach target temp. Waits when heating and cooling
M301 - Set PID parameters P I and D
M302 - Allow cold extrudes, or set the minimum extrude S<temperature>.
M303 - PID relay autotune S<temperature> sets the target temperature. (default target temperature = 150C)
M304 - Set bed PID parameters P I and D
```
### Message M Codes
```
M114 - Output current position to serial port
M115 - Capabilities string
M117 - Display message text on the LCD
M119 - Output Endstop status to serial port
```
### Endstops M Codes
```
M120 - Enable endstop detection
M121 - Disable endstop detection
```
### Special Features M Codes
```
M126 - Solenoid Air Valve Open (BariCUDA support by jmil)
M127 - Solenoid Air Valve Closed (BariCUDA vent to atmospheric pressure by jmil)
M128 - EtoP Open (BariCUDA EtoP = electricity to air pressure transducer by jmil)
M129 - EtoP Closed (BariCUDA EtoP = electricity to air pressure transducer by jmil)
M150 - Set BlinkM Color: R<red> U<green> B<blue> via I2C. Range: 0-255
M240 - Trigger a camera to take a photograph. (Add to your per-layer GCode.)
M250 - Set LCD contrast: C<0-63>
M280 - Set servo position absolute. P<servo index> S<angle or microseconds>
M300 - Play beep sound S<frequency Hz> P<duration ms>
M380 - Activate solenoid on active extruder
M381 - Disable all solenoids
M600 - Pause for filament change X<xpos> Y<ypos> Z<lift> E<initial retract> L<later retract for removal>
```
### Units and Measures G and M Codes
```
G90  - Use Absolute Coordinates
G91  - Use Relative Coordinates
M82  - Set E codes absolute (default)
M83  - Set E codes relative while in Absolute Coordinates (G90) mode
M92  - Set axis_steps_per_unit - same syntax as G92
M200 - Set filament diameter and set E axis units to mm^3 (use S0 to set back to mm).: D<mm>
M201 - Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
M202 - Set max acceleration in units/s^2 for travel moves (M202 X1000 Y1000) Unused in Marlin!!
M203 - Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in mm/sec
M204 - Set default acceleration: P for Printing moves, R for Retract only (no X, Y, Z) moves and T for Travel (non printing) moves (ex. M204 P800 T3000 R9000) in mm/sec^2
M205 - Advanced settings: minimum travel speed S=while printing T=travel only, B=minimum segment time X=maximum xy jerk, Z=maximum Z jerk, E=maximum E jerk
M206 - Set additional homing offset
M218 - Set hotend offset (in mm): T<extruder_number> X<offset_on_X> Y<offset_on_Y>
M220 - Set speed factor override percentage: S<factor in percent>
M221 - Set extrude factor override percentage: S<factor in percent>
```
### Firmware Retraction G and M Codes
```
G10  - Retract filament according to settings of M207
G11  - Retract recover filament according to settings of M208
M207 - Set retract length S[positive mm] F[feedrate mm/min] Z[additional zlift/hop], stays in mm regardless of M200 setting
M208 - Set recover=unretract length S[positive mm surplus to the M207 S*] F[feedrate mm/s]
M209 - S<1=true/0=false> enable automatic retract detect if the slicer did not support G10/11: every normal extrude-only move will be classified as retract depending on the direction
```
### Z Probe G and M Codes
```
G28  - Home all Axis
G29  - Detailed Z-Probe, probes the bed at 3 or more points. The printer must be homed with G28 before G29.
G30  - Single Z Probe, probes bed at current XY location.
G31  - Dock Z Probe sled (if enabled)
G32  - Undock Z Probe sled (if enabled)
M48  - Measure Z_Probe repeatability: P<points> X<xpos> Y<ypos> V<verbosity> E=engage L<legs of travel>
       As with G29, the E flag causes the probe to stow after each probe.
M401 - Lower Z-probe (if present)
M402 - Raise Z-probe (if present)
```
### Filament Diameter M Codes
```
M404 - Set (or display) Nominal Filament Diameter in mm: [N<diameter>] (e.g., 3mm or 1.75mm)
M405 - Turn on Filament Sensor extrusion control: [D<distance>] to set distance (in cm) from sensor to extruder
M406 - Turn off Filament Sensor extrusion control
M407 - Display measured Filament Diameter
```
### EEPROM Settings M Codes
```
M500 - Store parameters in EEPROM
M501 - Read parameters from EEPROM (if you need reset them after you changed them temporarily).
M502 - Revert to the default "factory settings". You still need to store them in EEPROM afterwards if you want to.
M503 - Print the current settings (from memory not from EEPROM). Use S0 to leave off headings.
```
### Delta M Codes
```
M665 - Set Delta configurations: L<diagonal rod> R<delta radius> S<segments/s>
M666 - Set Delta endstop adjustment: X<x-adjustment> Y<y-adjustment> Z<z-adjustment>
M605 - Set dual x-carriage movement mode: S<mode> [ X<duplication x-offset> R<duplication temp offset> ]
```
### Stepper Driver M Codes
```
M17  - Enable/Power all stepper motors
M18  - Disable all stepper motors. (same as M84)
M907 - Set digital trimpot motor current using axis codes.
M908 - Control digital trimpot directly.
M350 - Set microstepping mode.
M351 - Toggle MS1 MS2 pins directly.
```
### SCARA M-Codes
__May change to suit future G-code standards__
```
M360 - SCARA calibration: Move to cal-position ThetaA (0 deg calibration)
M361 - SCARA calibration: Move to cal-position ThetaB (90 deg calibration - steps per degree)
M362 - SCARA calibration: Move to cal-position PsiA (0 deg calibration)
M363 - SCARA calibration: Move to cal-position PsiB (90 deg calibration - steps per degree)
M364 - SCARA calibration: Move to cal-position PSIC (90 deg to Theta calibration position)
M365 - SCARA calibration: Scaling factor, X, Y, Z axis
```

## Comments

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
