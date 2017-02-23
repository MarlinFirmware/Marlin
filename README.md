**Marlin firmware for the Leapfrog Creatr**
===

*Based on Marlin 1.1.0 - RCBugFix (22 feb 2017)*<br>
The stock Leapfrog Creatr use a proprietary board which it completly screwed and of course, replacing it would cost 150€ alone and not even hackable (soldered stepper driver, not much expansion port and no community around it).

Complete part list are:
- RAMPS 1.4 ***(modified for 24v)***.
- Reprapdiscount LCD 20x04.
- Arduino Mega.
- 4x TMC2100 stepper driver (X, Y and Dual E).
- 1x DRV8825 stepper driver (Solely for the Z axis: torque freak).
- BLTouch sensor (Z probe and auto-leveling).

All the rest of the hardware are for the moment stock from the factory to ensure a smooth and easy transition from stock to custom.

---
**THE RAMPS MUST BE MODIFIED TO SUPPORT 24V**


I can't stress that enough. The Leapfrog Creatr use a 24V power supply which will fry or burst into flame immediatly if the proper modification on the RAMPS is not made. Search on internet but a rough explaination is:
- Check if all capacitors are rated above 24V (mine are 35V), if not, you're screwed.
- Check if the polyfuse are rated above 24V, if not, remove them and put an adequate fuse or bridge the pins.
- Remove the D1 diode to cut the 24V to flow into the Arduino.
- Supply an external power to the Arduino (for exemple 5V from the USB).

**I can't be held responsible for any damage or injure that can happen resulting on modifying your printer.**<br>
The wiring is pretty straight forward like every RAMPS on the planet. Might add a picture later.

---
**BLTouch sensor**
---
The stock RC8 BugFix does support the BLTouch OoB (using #define BL_TOUCH) but it's somehow broken with my sensor (knockoff: Deployed is triggered). So it has been manually added like every regular sensor (Servo position {10,90}) etc.

The sensor is connected to Servo pin 0 (closest to the reset button).

|Commands|Code
|:--|:--
|Pin down (deployed)|`M280 P0 S10`
|Pin up (parking)|`M280 P0 S90`
|Self test|`M280 P0 S120`
|Reset alarm|`M280 P0 S160`

The firmware is set to do a bilinear leveling with a grid of 4x4 and safe homing in the center of the bed.<br>
**DON'T `G28 (Home)` AFTER `G29 (Autoleveling)`. It will just remove the correction from the printer**

If your sensor is a perfect replica of the ANTlabs sensor, you might change to the builtin define and revert the behavior of the endstop.

---
**Scriptings**
---


The scripts below are pulled from my Simplify3D profil. Just adapt to your liking or software (temperature hooker differ in Cura for exemple).

Starting script:
```
M104 S[extruder0_temperature] ;Set hotend to temp
M280 P0 S160                  ;Reset BLTouch sensor
M280 P0 S120                  ;BLTouch selftest mode
G4 S5                         ;Wait 5 seconds
M280 P0 S160                  ;Reset BLTouch sensor
G28                           ;Homing
G29                           ;Autoleveling
G1 F2400 X0 Y0                ;Move to Origin XY
M109 S[extruder0_temperature] ;Wait hotend to reach temp
G90                           ;Absolute positioning
M83                           ;Extruder in relative mode
G1 F300 Z0.2                  ;Nozzle to starting position
G1 F300 E5                    ;Priming the hotend
G1 F300 E15 X20               ;Purge the hotend
M82                           ;Extruder in absolute mode
G92 E0                        ;Reset extruder position
```

Ending script:
```
G91	                          ;Relative
G1 F600 Z50                   ;Move Z down by 50mm
G90                           ;Absolute
G1 F1200 Y250 X100            ;Move head toward center backend
G92 E0                        ;Reset extruder position
G1 E-5                        ;Reduce filament pressure
M106 S0                       ;Turn off fan
M104 S0                       ;Turn off extruder
M140 S0                       ;Turn off heated bed
G92 E0                        ;Reset extruder position
M84                           ;Turn steppers off
```

---
**Futur plan**
---
- Replace the Z axis DRV8825 to DM422 stepper driver.
- Replace the LCD 2004 for a 12864 full graphics.
- Dual E3D V6 or Chimera/Cyclops.
- Bowden conversion.
- Full enclosure with HEPA filtering.

---
**Random informations**
---
- `THERMAL_PROTECTION_*_PERIOD` have been increased to `40`.
- `WATCH_*_TEMP_PERIOD`have been increased to `60`.
- `BED PID` for the hotend is customized (run `M303 E0 S200 C8` to get yours lines 326-328 in configuration.h).
