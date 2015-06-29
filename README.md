<<<<<<< HEAD
Current Status: Bug Fixing
===================

What bugs are we working on: https://github.com/ErikZalm/Marlin/milestones

Developer Notes
===================

- There are now 2 branches: The __development__ branch is where new features and code changes will be sorted out. This branch may have untested code in it, so please let us know if you find any bugs. When the __development__ branch has reached a state where it is stable, it will be moved to the __stable__ branch.

- We are doing a kind of cleanup in the list of Issues and Pull Requests, the aim is to get to a state where we can certify the code as stable. To get the code tested as widely as possible we require several volunteers with a wide variety of hardware configurations willing to test the firmware and help us to certify it as stable. If you want to help out testing go to this issue and let us know: https://github.com/ErikZalm/Marlin/issues/1209

- Before you submit any pull request, we ask that you _PLEASE_ test your code before submission, even if the change seems innocuous. When creating the pull request, please include the hardware you used for testing and a short synopsis of your testing procedure. Untested pull requests are less likely to be merged, as even slight changes create the risk of breaking the main branch.

- If you have a fix don't open an issue telling about it, but test the code and submit a pull request. Use the __development__ branch when you submit.

==========================
Marlin 3D Printer Firmware
==========================
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)
[![Travis Build Status](https://travis-ci.org/ErikZalm/Marlin.svg)](https://travis-ci.org/ErikZalm/Marlin)

Marlin has a GPL license because I believe in open development.
Please do not use this code in products (3D printers, CNC etc) that are closed source or are crippled by a patent.

[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/ErikZalm/Marlin&title=Marlin&language=&tags=github&category=software)

Quick Information
===================
This RepRap firmware is a mashup between <a href="https://github.com/kliment/Sprinter">Sprinter</a>, <a href="https://github.com/simen/grbl/tree">grbl</a> and many original parts.

Derived from Sprinter and Grbl by Erik van der Zalm.
Sprinters lead developers are Kliment and caru.
Grbls lead developer is Simen Svale Skogsrud. Sonney Jeon (Chamnit) improved some parts of grbl
A fork by bkubicek for the Ultimaker was merged, and further development was aided by him.
Some features have been added by:
Lampmaker, Bradley Feldman, and others...


Features:
=========

*   Interrupt based movement with real linear acceleration
*   High steprate
*   Look ahead (Keep the speed high when possible. High cornering speed)
*   Interrupt based temperature protection
*   preliminary support for Matthew Roberts advance algorithm
    For more info see: http://reprap.org/pipermail/reprap-dev/2011-May/003323.html
*   Full endstop support
*   SD Card support
*   SD Card folders (works in pronterface)
*   SD Card autostart support
*   LCD support (ideally 20x4)
*   LCD menu system for autonomous SD card printing, controlled by an click-encoder.
*   EEPROM storage of e.g. max-velocity, max-acceleration, and similar variables
*   many small but handy things originating from bkubicek's fork.
*   Arc support
*   Temperature oversampling
*   Dynamic Temperature setpointing aka "AutoTemp"
*   Support for QTMarlin, a very beta GUI for PID-tuning and velocity-acceleration testing. https://github.com/bkubicek/QTMarlin
*   Endstop trigger reporting to the host software.
*   Updated sdcardlib
*   Heater power reporting. Useful for PID monitoring.
*   PID tuning
*   CoreXY kinematics (www.corexy.com/theory.html)
*   Delta kinematics
*   SCARA kinematics
*   Dual X-carriage support for multiple extruder systems
*   Configurable serial port to support connection of wireless adaptors.
*   Automatic operation of extruder/cold-end cooling fans based on nozzle temperature
*   RC Servo Support, specify angle or duration for continuous rotation servos.
*   Bed Auto Leveling.
*   Support for a filament diameter sensor, which adjusts extrusion volume

The default baudrate is 250000. This baudrate has less jitter and hence errors than the usual 115200 baud, but is less supported by drivers and host-environments.


Differences and additions to the already good Sprinter firmware:
================================================================

Look-ahead:
-----------

Marlin has look-ahead. While sprinter has to break and re-accelerate at each corner,
lookahead will only decelerate and accelerate to a velocity,
so that the change in vectorial velocity magnitude is less than the xy_jerk_velocity.
This is only possible, if some future moves are already processed, hence the name.
It leads to less over-deposition at corners, especially at flat angles.

Arc support:
------------

Slic3r can find curves that, although broken into segments, were ment to describe an arc.
Marlin is able to print those arcs. The advantage is the firmware can choose the resolution,
and can perform the arc with nearly constant velocity, resulting in a nice finish.
Also, less serial communication is needed.

Temperature Oversampling:
-------------------------

To reduce noise and make the PID-differential term more useful, 16 ADC conversion results are averaged.

AutoTemp:
---------

If your gcode contains a wide spread of extruder velocities, or you realtime change the building speed, the temperature should be changed accordingly.
Usually, higher speed requires higher temperature.
This can now be performed by the AutoTemp function
By calling M109 S<mintemp> B<maxtemp> F<factor> you enter the autotemp mode.

You can leave it by calling M109 without any F.
If active, the maximal extruder stepper rate of all buffered moves will be calculated, and named "maxerate" [steps/sec].
The wanted temperature then will be set to t=tempmin+factor*maxerate, while being limited between tempmin and tempmax.
If the target temperature is set manually or by gcode to a value less then tempmin, it will be kept without change.
Ideally, your gcode can be completely free of temperature controls, apart from a M109 S T F in the start.gcode, and a M109 S0 in the end.gcode.

EEPROM:
-------

If you know your PID values, the acceleration and max-velocities of your unique machine, you can set them, and finally store them in the EEPROM.
After each reboot, it will magically load them from EEPROM, independent what your Configuration.h says.

LCD Menu:
---------

If your hardware supports it, you can build yourself a LCD-CardReader+Click+encoder combination. It will enable you to realtime tune temperatures,
accelerations, velocities, flow rates, select and print files from the SD card, preheat, disable the steppers, and do other fancy stuff.
One working hardware is documented here: http://www.thingiverse.com/thing:12663
Also, with just a 20x4 or 16x2 display, useful data is shown.

SD card folders:
----------------

If you have an SD card reader attached to your controller, also folders work now. Listing the files in pronterface will show "/path/subpath/file.g".
You can write to file in a subfolder by specifying a similar text using small letters in the path.
Also, backup copies of various operating systems are hidden, as well as files not ending with ".g".

SD card folders:
----------------

If you place a file auto[0-9].g into the root of the sd card, it will be automatically executed if you boot the printer. The same file will be executed by selecting "Autostart" from the menu.
First *0 will be performed, than *1 and so on. That way, you can heat up or even print automatically without user interaction.

Endstop trigger reporting:
--------------------------

If an endstop is hit while moving towards the endstop, the location at which the firmware thinks that the endstop was triggered is outputed on the serial port.
This is useful, because the user gets a warning message.
However, also tools like QTMarlin can use this for finding acceptable combinations of velocity+acceleration.

Coding paradigm:
----------------

Not relevant from a user side, but Marlin was split into thematic junks, and has tried to partially enforced private variables.
This is intended to make it clearer, what interacts which what, and leads to a higher level of modularization.
We think that this is a useful prestep for porting this firmware to e.g. an ARM platform in the future.
A lot of RAM (with enabled LCD ~2200 bytes) was saved by storing char []="some message" in Program memory.
In the serial communication, a #define based level of abstraction was enforced, so that it is clear that
some transfer is information (usually beginning with "echo:"), an error "error:", or just normal protocol,
necessary for backwards compatibility.

Interrupt based temperature measurements:
-----------------------------------------

An interrupt is used to manage ADC conversions, and enforce checking for critical temperatures.
This leads to less blocking in the heater management routine.
=======
# Marlin 3D Printer Firmware
<img align="right" src="Documentation/Logo/Marlin%20Logo%20GitHub.png" />
>>>>>>> MarlinFirmware/1.0.x

 Documentation has moved to [marlinfirmware.org](http://www.marlinfirmware.org).

## Release Branch

The Release branch contains the latest tagged version of Marlin (currently 1.0.2 – January 2015). It also includes a version 1.0.1 (December 2014). Any version of Marlin before 1.0.1 (when we started tagging versions) can be collectively referred to as Marlin 1.0.0.

For the most up-to-date Marlin code, please go to the [Development branch](https://github.com/MarlinFirmware/Marlin/tree/Development) – __Not for production use – use with caution!__ When reporting any issues, please check to see if they are resolved in the Development branch first. But let us know if issues exist in tagged releases so we may document and patch them.

## Current Status: In Development

Marlin development is being accelerated to catch up with a long list of issues. Check the Issues and Pull Requests links on the right to to see what we are currently working on.

[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)
[![Travis Build Status](https://travis-ci.org/MarlinFirmware/Marlin.svg)](https://travis-ci.org/MarlinFirmware/Marlin)

##### [RepRap.org Wiki Page](http://reprap.org/wiki/Marlin)

## Contact

__Google Hangout:__ <a href="https://plus.google.com/hangouts/_/gxn3wrea5gdhoo223yimsiforia" target="_blank">. Hangout</a> We have a hangout every 2 weeks. Search the issue list for "Hangout" and you will see what time and date the next event is going on.

## Credits

The current Marlin dev team consists of:

 - Scott Lahteine [@thinkyhead] - English
 - Andreas Hardtung [@AnHardt] - Deutsch, English
 - [@Wurstnase] - Deutsch, English
 - [@fmalpartida] - English, Spanish
 - [@CONSULitAS] - Deutsch, English
 - [@maverikou]
 - Chris Palmer [@nophead]
 - [@paclema]
 - [@epatel]
 - Erik van der Zalm [@ErikZalm]
 - David Braam [@daid]
 - Bernhard Kubicek [@bkubicek]

<<<<<<< HEAD
Filament Sensor
---------------
Supports the use of a real time filament diameter sensor that measures the diameter of the filament going into the extruder and then adjusts the extrusion rate to compensate for filament that does not match what is defined in the g-code.  The diameter can also be displayed on the LCD screen. This potentially eliminates the need to measure filament diameter when changing spools of filament. Gcode becomes independent of the filament diameter. Can also compensate for changing diameter.

For examples of these sensors, see: http://www.thingiverse.com/thing:454584, https://www.youmagine.com/designs/filament-diameter-sensor, http://diy3dprinting.blogspot.com/2014/01/diy-filament-diameter-sensor.html. Any sensor which produces a voltage equivalent to the diameter in mm (i.e. 1v = 1mm) can be used. This provides a very simple interface and may encourage more innovation in this area.

4 new Mcodes are defined to set relevant parameters: M404, M405, M406, M407 - see above.

 Implements a delay buffer to handle the transit delay between where the filament is measured and when it gets to the extruder.
=======
More features have been added by:
  - Lampmaker,
  - Bradley Feldman,
  - and others...

## License

Marlin is published under the [GPL license](/Documentation/COPYING.md) because We believe in open development.
Do not use this code in products (3D printers, CNC etc) that are closed source or are crippled by a patent.
>>>>>>> MarlinFirmware/1.0.x

[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=ErikZalm&url=https://github.com/MarlinFirmware/Marlin&title=Marlin&language=&tags=github&category=software)
