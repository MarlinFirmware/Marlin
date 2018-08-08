# Configuration for Kossel k800 XL
This example configuration is for a Kossel XL with a printable bed diameter of 280mm and a height of 380mm. It also has the auto bed leveling probe (with an endstop switch) and the heated bed activated.

## Configuration
You might need (or want) to edit at least the following settings in `Configuration.h`:
* `MANUAL_Z_HOME_POS` - The available height of your printing space. Auto Bed Leveling makes it less important to have the exact value.
* `DELTA_PRINTABLE_RADIUS` - The printable radius is how far from the center the nozzle can reach.
* `DEFAULT_AXIS_STEPS_PER_UNIT` - Steps-per-millimeter for the delta steppers, and for the extruder [to optimize the amount of filament flow](http://zennmaster.com/makingstuff/reprap-101-calibrating-your-extruder-part-1-e-steps).

### Fine tuning
* Increase `DELTA_RADIUS` if the model comes out convex (with a bulge in the middle)
* Increase `DELTA_DIAGONAL_ROD` if the model comes out larger than expected

### [http://reprap.org/wiki/PID_Tuning](PID Tuning)
* `DEFAULT_Kp` - The proportional term
* `DEFAULT_Ki` - The integral term
* `DEFAULT_Kd` - The derivative term

### PSU Options
* The power supply is configured to 2 (to use a relay to switch 12V on and off)
* It is configured to be off by default
