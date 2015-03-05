Filament Sensor
---------------
Supports the use of a real time filament diameter sensor that measures the diameter of the filament going into the extruder and then adjusts the extrusion rate to compensate for filament that does not match what is defined in the g-code.  The diameter can also be displayed on the LCD screen. This potentially eliminates the need to measure filament diameter when changing spools of filament. Gcode becomes independent of the filament diameter. Can also compensate for changing diameter.

For examples of these sensors, see: http://www.thingiverse.com/thing:454584, https://www.youmagine.com/designs/filament-diameter-sensor, http://diy3dprinting.blogspot.com/2014/01/diy-filament-diameter-sensor.html. Any sensor which produces a voltage equivalent to the diameter in mm (i.e. 1v = 1mm) can be used. This provides a very simple interface and may encourage more innovation in this area.

4 new Mcodes are defined to set relevant parameters: M404, M405, M406, M407 - see above.

 Implements a delay buffer to handle the transit delay between where the filament is measured and when it gets to the extruder.