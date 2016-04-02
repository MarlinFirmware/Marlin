# Configuration for Kossel k800 XL
This example configuration ist for a Kossel XL with a printable bed diameter of 280mm and a height of 385mm. It also has the auto bed leveling probe (with a endstop switch) and the heat bed activated. 

## Configuration
You might have/want to edit at least the following settings in Configuration.h:
* <code>MANUAL_Z_HOME_POS<code> The hight of your printing space available, auto bed leveling makes this not as important as before
* <code>DELTA_PRINTABLE_RADIUS</code> The printable radius
* <code>DEFAULT_AXIS_STEPS_PER_UNIT</code> [http://zennmaster.com/makingstuff/reprap-101-calibrating-your-extruder-part-1-e-steps](The steps for the extruder to optimize the amount of filament flow)

### Fine tuning
* Increase <code>DELTA_RADIUS</code> when the model is convex (bulge in the middle)
* Increase <code>DELTA_DIAGONAL_ROD</code> when the model is larger then expected

### [http://reprap.org/wiki/PID_Tuning](PID Tuning)
* <code>DEFAULT_Kp</code> (PID tuning for the hotend)
* <code>DEFAULT_Ki</code> (PID tuning for the hotend)
* <code>DEFAULT_Kd</code> (PID tuning for the hotend)

### PSU Options
* The power supply is configured to 2 (to use a relay to switch 12V on and off)
* It is configured to be off by default
