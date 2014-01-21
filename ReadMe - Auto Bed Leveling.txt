ReadMe - Auto Bed Leveling (ABL)

Date:
2014-01-31

Summary:
A few notes on configuring the Auto Bed Leveling feature of Marlin for use with
a modified Solidoodle.


Detailed Description:
Auto Bed Leveling is a feature that compensates for a printing surface that is
not perfectly level, however the surface does need to be perfectly flat. This 
feature utilizes the Z-axis endstop to probe 3 points (or more, see below) on 
the printing surface. Typically, the endstop is mounted to a servo that 
positions the endstop below the hot end during leveling, then retracts the 
endstop during printing.


Auto Bed Leveling Parameters in Configuration.h file:

1. Uncomment #define ENALBLE_AUTO_BED_LEVELING

2. Define the probe locations, where:
	left/front is [x-min,y-min]
	left/back is [x-min, y-max]
	right/front is [x-max, y-min]

3. Define probe offset from hot end

4. Use the Z_RAISE_* values to ensure the ABL does not crash into anything nor
    scrape along any surface during Homing and ABL

5. Z Safe Homing is required if the ABL probe cannot Z home at X=0, Y=0

6. Use Accurate Bed Leveling to probe a grid of points, as opposed to just 3



