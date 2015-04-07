===============================================
Instructions for configuring Bed Auto Leveling
===============================================
There are two options for this feature. You may choose to use a servo mounted on the X carriage or you may use a sled that mounts on the X axis and can be docked when not in use.
See the section for each option below for specifics about installation and configuration. Also included are instructions that apply to both options.

Instructions for Both Options
-----------------------------

Uncomment the "ENABLE_AUTO_BED_LEVELING" define (commented by default)

The following options define the probing positions. These are good starting values.
I recommend to keep a better clearance from borders in the first run and then make the probes as close as possible to borders:

* \#define LEFT_PROBE_BED_POSITION 30
* \#define RIGHT_PROBE_BED_POSITION 140
* \#define BACK_PROBE_BED_POSITION 140
* \#define FRONT_PROBE_BED_POSITION 30

A few more options:

* \#define XY_TRAVEL_SPEED 6000

X and Y axis travel speed between probes, in mm/min.
Bear in mind that really fast moves may render step skipping. 6000 mm/min (100mm/s) is a good value.

* \#define Z_RAISE_BEFORE_PROBING 10
* \#define Z_RAISE_BETWEEN_PROBINGS 10

The Z axis is lifted when traveling to the first probe point by Z_RAISE_BEFORE_PROBING value
and then lifted when traveling from first to second and second to third point by Z_RAISE_BETWEEN_PROBINGS.
All values are in mm as usual.

Servo Option Notes
------------------
You will probably need a swivel Z-MIN endstop in the extruder. A rc servo do a great job.
Check the system working here: http://www.youtube.com/watch?v=3IKMeOYz-1Q (Enable English subtitles)
Teasing ;-) video: http://www.youtube.com/watch?v=x8eqSQNAyro

In order to get the servo working, you need to enable:

* \#define NUM_SERVOS 1 // Servo index starts with 0 for M280 command

* \#define SERVO_ENDSTOPS {-1, -1, 0} // Servo index for X, Y, Z. Disable with -1

* \#define SERVO_ENDSTOP_ANGLES {0,0, 0,0, 165,60} // X,Y,Z Axis Extend and Retract angles

The first define tells firmware how many servos you have.
The second tells what axis this servo will be attached to. In the example above, we have a servo in Z axis.
The third one tells the angle in 2 situations: Probing (165º) and resting (60º). Check this with command M280 P0 S{angle} (example: M280 P0 S60 moves the servo to 60º)

Next you need to define the Z endstop (probe) offset from hotend.
My preferred method:

* a) Make a small mark in the bed with a marker/felt-tip pen.
* b) Place the hotend tip as *exactly* as possible on the mark, touching the bed. Raise the hotend 0.1mm (a regular paper thickness) and zero all axis (G92 X0 Y0 Z0);
* d) Raise the hotend 10mm (or more) for probe clearance, lower the Z probe (Z-Endstop) with M401 and place it just on that mark by moving X, Y and Z;
* e) Lower the Z in 0.1mm steps, with the probe always touching the mark (it may be necessary to adjust X and Y as well) until you hear the "click" meaning the mechanical endstop was trigged. You can confirm with M119;
* f) Now you have the probe in the same place as your hotend tip was before. Perform a M114 and write down the values, for example: X:24.3 Y:-31.4 Z:5.1;
* g) You can raise the z probe with M402 command;
* h) Fill the defines bellow multiplying the values by "-1" (just change the signal)


* \#define X_PROBE_OFFSET_FROM_EXTRUDER -24.3
* \#define Y_PROBE_OFFSET_FROM_EXTRUDER 31.4
* \#define Z_PROBE_OFFSET_FROM_EXTRUDER -5.1

Sled Option Notes
-----------------
The sled option uses an electromagnet to attach and detach to/from the X carriage. See http://www.thingiverse.com/thing:396692 for more details on how to print and install this feature. It uses the same connections as the servo option.

To use the sled option, you must define two additional things in Configuration.h:

* \#define Z_PROBE_SLED
* \#define SLED_DOCKING_OFFSET 5

Uncomment the Z_PROBE_SLED to define to enable the sled (commented out by default).

Uncomment the SLED_DOCKING_OFFSET to set the extra distance the X axis must travel to dock the sled. This value can be found by moving the X axis to its maximum position then measure the distance to the right X end and subtract the width of the sled (23mm if you printed the sled from Thingiverse).

Next you need to define the Z endstop (probe) offset from hotend.
My preferred method:

* a) Home the X and Y axes.
* b) Move the X axis to about the center of the print bed. Make a mark on the print bed.
* c) Move the Y axis to the maximum position. Make another mark.
* d) Home the X axis and use a straight edge to make a line between the two points.
* e) Repeat (b)-(d) reversing the X and Y. When you are done you will have two lines on the print bed. We will use these to measure the offset for the Z probe endstop.
* f) Move the nozzle so that it is positioned on the center point of the two lines. You can use fine movement of 0.1mm to get it as close as possible. Note the position of X and Y.
* g) Zero the Z axis with the G92 Z0 command.
* h) Raise the Z axis about 20mmm.
* i) Use the G32 command to retrieve the sled.
* j) Now more the X and Y axis to the position recorded in (f).
* k) Lower the Z axis in 0.1mm steps until you hear the "click" meaning the mechanical endstop was trigged. You can confirm with the M119 command. Note the position of the Z axis.
* l) Make a mark on the print bed where the endstop lever has touched the print bed. Raise the Z-axis about 30mm to give yourself some room.
* m) Now measure the distance from the center point to the endstop impact site along the X and Y axis using the lines drawn previously.
* n) Fill in the values below. If the endstop mark is in front of the line running left-to-right, use positive values. If it is behind, use negative values. For the Z axis use the value from (k) and subtract 0.1mm.

For example, suppose you measured the endstop position and it was 20mm to the right of the line running front-to-back, 10mm toward the front of the line running left-to-right, and the value from (k) was 2.85. The values for the defines would be:

* \#define X_PROBE_OFFSET_FROM_EXTRUDER 20
* \#define Y_PROBE_OFFSET_FROM_EXTRUDER 10
* \#define Z_PROBE_OFFSET_FROM_EXTRUDER 2.75

That's it.. enjoy never having to calibrate your Z endstop neither leveling your bed by hand anymore ;-)
