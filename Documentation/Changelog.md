#  BQ Marlin v2.1.0
---
### Bugfixes:
* Fixed: Printing from SD stops if USB cable is disconnected (W2/H2).
* Fixed: Changing filament, inherited feedrate can produce sliding on extruder (W/H/HXL/W2/H2).
* Fixed serial printing from Cura 15.04 and possibly other host systems (W/H/HXL/W2/H2).
* Skip temperature animation on manual levelling when temperature is already above preheat value (W2/H2).
* Fixed: Printing position can be lost after a fast filament change (W2/H2).
* Auto-levelling option can now be build on 1st generation printers (W/H/HXL).
* Blower is now activated when a percent of speed (M106) is selected (always 100% of speed is used though) (W2/H2).
* Fixed serial printing entering in inactivity state if M800 is not used.

### Improvements:
* Reduce printing slowdown with very short segments due to screen updates (W2/H2).
* Added board serial number to M115 response over serial (W2/H2).
* Reduce one check on double homing (base detection) algorithm (W2).

### New features:
* Added an error screen when special (non supported) characters are used on file names or folders (W2/H2).
* Added a new statistics screen. Now printing time and successful/cancelled printings are stored on EEPROM (W2/H2).

### Translations:
* Italian, Russian and Portuguese fixes (W2/H2).
* Minor changes on all languages (W2/H2).

#  BQ Marlin v2.0.0
---
### Under the hood:
* Synced with upstream Marlin (git-9b50ec) (20150228) with additional fixes.
* Built with Arduino SDK 1.6.4.
* Built wit C++11 support.
* Dropped support for Arduino IDE. Use the provided Makefile instead, see README.md for detailed instructions.

### New hardware support:
* Support for additional Arduino pins, extending the SDK file, required by BQ-CNC and BQ-Zum Mega3D boards.
* Support for new printers Hephestos 2 and Witbox 2.
* Improved support for LCD display 12864U5.

### Bugfixes:
* Increased material extrusion on filament insertion, to ensure that the hotend is totally clean (W/H/HXL/W2/H2).
* Increased retraction of material on pause and filament change to avoid leaving drops of material on the piece (W/H/HXL/W2/H2).
* Fixed low extrusion after pausing and/or filament change (W/H/HXL/W2/H2).
* Fixed wrong retraction on M600 GCode (W/H/HXL/W2/H2).
* Fixed non-linear behaviour of thermistor for increased accuracy on temperature measurement (W/H/HXL/W2/H2).
* Ensure buffer is cleared after finishing or cancelling a printing (W/H/HXL/W2/H2).
* Fixed reading long filenames on SD card (W/H/HXL/W2/H2).
* Fixed crash when browsing up to 10 levels of subdirectories (W/H/HXL/W2/H2).
* Changed drivers pulse frequency to enhance real-time response and layer-shifting protection (W/H/HXL/W2/H2).
* Improved endstop logic (W/H/HXL/W2/H2).

### Changes:
* Removed last levelling point (confirmation point) (W/H/HXL).
* Changed some operational positions to improve usability and uniformity between printers (W/H/HXL).

### New features:
* New graphical interface for LCD display 12864U5 (W2/H2).
* Renewed auto-levelling feature with improved precision and accuracy (W2/H2).
* Sensor-assisted wizard for manual levelling (W2/H2).
* Support for new double drive gear (DDG) extruder (W2/H2).
* New real-time PID control for DDG extruder (W2/H2).
* Multilanguage support on a single build: English, Spanish, German, French, Portuguese, Italian, Swedish and Russian (W2/H2).
* Inactivity mode (W2/H2).
* Serial printing screen (W2/H2).
* Smart Light (W2).
* Automatic base detection (W2).
* Configurable chassis fan (W2).
