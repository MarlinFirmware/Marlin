; ### Marlin K-Factor Calibration Pattern ###
; -------------------------------------------
;
; Printer: Flash
; Filament: PLA
; Created: Mon Oct 28 2019 14:30:44 GMT+0100 (heure normale d’Europe centrale)
;
; Settings Printer:
; Filament Diameter = 1.75 mm
; Nozzle Diameter = 0.4 mm
; Nozzle Temperature = 210 °C
; Bed Temperature = 50 °C
; Retraction Distance = 4 mm
; Layer Height = 0.2 mm
; Z-axis Offset = 0 mm
;
; Settings Print Bed:
; Bed Shape = Rect
; Bed Size X = 240 mm
; Bed Size Y = 240 mm
; Origin Bed Center = false
;
; Settings Speed:
; Slow Printing Speed = 2400 mm/min
; Fast Printing Speed = 4800 mm/min
; Movement Speed = 7200 mm/min
; Retract Speed = 3000 mm/min
; Printing Acceleration = 5000 mm/s^2
; Jerk X-axis =  firmware default
; Jerk Y-axis =  firmware default
; Jerk Z-axis =  firmware default
; Jerk Extruder =  firmware default
;
; Settings Pattern:
; Linear Advance Version = 1.5
; Starting Value Factor = 0
; Ending Value Factor = 1
; Factor Stepping = 0.2
; Test Line Spacing = 5 mm
; Test Line Length Slow = 20 mm
; Test Line Length Fast = 40 mm
; Print Pattern = Standard
; Print Frame = false
; Number Lines = true
; Print Size X = 104.65180361560903 mm
; Print Size Y = 104.65180361560903 mm
; Print Rotation = 45 degree
;
; Settings Advance:
; Nozzle / Line Ratio = 1.2
; Bed leveling = G29
; Use FWRETRACT = false
; Extrusion Multiplier = 1
; Prime Nozzle = true
; Prime Extrusion Multiplier = 2.5
; Prime Speed = 1800
; Dwell Time = 1 s
;
; prepare printing
;
M104 S210 ; set nozzle temperature but do not wait
M190 S50 ; set bed temperature and wait
M109 S210 ; block waiting for nozzle temp
; <================ ici pour le Z
G28 ; home all axes with heated bed
G29; Activate bed leveling compensation
G21 ; set units to millimeters
M204 P5000 ; set acceleration
G90 ; use absolute coordinates
M83 ; use relative distances for extrusion
G92 E0 ; reset extruder distance
G1 X120 Y120 F7200 ; move to start
G1 Z0.2 F2400 ; move to layer height
;
; prime nozzle
;
G1 X67.6741 Y136.9706 F7200 ; move to start
G1 X103.0294 Y172.3259 E4.989 F1800 ; print line
G1 X103.5386 Y171.8168 F7200 ; move to start
G1 X68.1832 Y136.4614 E4.989 F1800 ; print line
G1 E-4 F3000 ; retract
;
; start the Test pattern
;
G4 P1000 ; Pause (dwell) for 2 seconds
G1 X74.7452 Y129.8995 F7200 ; move to start
M900 K0 ; set K-factor
M117 K0 ; 
G1 E4 F3000 ; un-retract
G1 X88.8873 Y115.7574 E0.7982 F2400 ; print line
G1 X117.1716 Y87.4731 E1.5965 F4800 ; print line
G1 X131.3137 Y73.331 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 X78.2807 Y133.435 F7200 ; move to start
M900 K0.2 ; set K-factor
M117 K0.2 ; 
G1 E4 F3000 ; un-retract
G1 X92.4228 Y119.2929 E0.7982 F2400 ; print line
G1 X120.7071 Y91.0086 E1.5965 F4800 ; print line
G1 X134.8492 Y76.8665 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 X81.8162 Y136.9706 F7200 ; move to start
M900 K0.4 ; set K-factor
M117 K0.4 ; 
G1 E4 F3000 ; un-retract
G1 X95.9584 Y122.8284 E0.7982 F2400 ; print line
G1 X124.2426 Y94.5442 E1.5965 F4800 ; print line
G1 X138.3848 Y80.402 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 X85.3518 Y140.5061 F7200 ; move to start
M900 K0.6 ; set K-factor
M117 K0.6 ; 
G1 E4 F3000 ; un-retract
G1 X99.4939 Y126.364 E0.7982 F2400 ; print line
G1 X127.7782 Y98.0797 E1.5965 F4800 ; print line
G1 X141.9203 Y83.9376 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 X88.8873 Y144.0416 F7200 ; move to start
M900 K0.8 ; set K-factor
M117 K0.8 ; 
G1 E4 F3000 ; un-retract
G1 X103.0294 Y129.8995 E0.7982 F2400 ; print line
G1 X131.3137 Y101.6152 E1.5965 F4800 ; print line
G1 X145.4558 Y87.4731 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 X92.4228 Y147.5772 F7200 ; move to start
M900 K1 ; set K-factor
M117 K1 ; 
G1 E4 F3000 ; un-retract
G1 X106.565 Y133.435 E0.7982 F2400 ; print line
G1 X134.8492 Y105.1508 E1.5965 F4800 ; print line
G1 X148.9914 Y91.0086 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
;
; mark the test area for reference
M117 K0 ;
M900 K0 ; set K-factor 0
G1 X110.1005 Y136.9706 F7200 ; move to start
G1 E4 F3000 ; un-retract
G1 X124.2426 Y151.1127 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 X138.3848 Y108.6863 F7200 ; move to start
G1 E4 F3000 ; un-retract
G1 X152.5269 Y122.8284 E0.7982 F2400 ; print line
G1 E-4 F3000 ; retract
G1 Z0.3 F2400 ; zHop
;
; print K-values
;
G1 X131.3137 Y70.5025 F7200 ; move to start
G1 Z0.2 F2400 ; zHop
G1 E4 F3000 ; un-retract
G1 X132.7279 Y69.0883 E0.0798 F2400 ; 0
G1 X134.1421 Y70.5025 E0.0798 F2400 ; 0
G1 X135.5563 Y71.9167 E0.0798 F2400 ; 0
G1 X134.1421 Y73.331 E0.0798 F2400 ; 0
G1 X132.7279 Y71.9167 E0.0798 F2400 ; 0
G1 X131.3137 Y70.5025 E0.0798 F2400 ; 0
G1 E-4 F3000 ; retract
G1 Z0.3 F2400 ; zHop
G1 X138.3848 Y77.5736 F7200 ; move to start
G1 Z0.2 F2400 ; zHop
G1 E4 F3000 ; un-retract
G1 X139.799 Y76.1594 E0.0798 F2400 ; 0
G1 X141.2132 Y77.5736 E0.0798 F2400 ; 0
G1 X142.6274 Y78.9878 E0.0798 F2400 ; 0
G1 X141.2132 Y80.402 E0.0798 F2400 ; 0
G1 X139.799 Y78.9878 E0.0798 F2400 ; 0
G1 X138.3848 Y77.5736 E0.0798 F2400 ; 0
G1 E-4 F3000 ; retract
G1 X140.5061 Y75.4523 F7200 ; move to start
G1 E4 F3000 ; un-retract
G1 X140.7889 Y75.7351 E0.016 F2400 ; dot
G1 E-4 F3000 ; retract
G1 X141.2132 Y74.7452 F7200 ; move to start
G1 E4 F3000 ; un-retract
G1 X142.6274 Y76.1594 F7200 ; move to start
G1 X144.0416 Y77.5736 F7200 ; move to start
G1 X142.6274 Y76.1594 E0.0798 F2400 ; 4
G1 X144.0416 Y74.7452 E0.0798 F2400 ; 4
G1 X145.4558 Y76.1594 F7200 ; move to start
G1 X144.0416 Y74.7452 E0.0798 F2400 ; 4
G1 X142.6274 Y73.331 E0.0798 F2400 ; 4
G1 E-4 F3000 ; retract
G1 Z0.3 F2400 ; zHop
G1 X145.4558 Y84.6447 F7200 ; move to start
G1 Z0.2 F2400 ; zHop
G1 E4 F3000 ; un-retract
G1 X146.8701 Y83.2304 E0.0798 F2400 ; 0
G1 X148.2843 Y84.6447 E0.0798 F2400 ; 0
G1 X149.6985 Y86.0589 E0.0798 F2400 ; 0
G1 X148.2843 Y87.4731 E0.0798 F2400 ; 0
G1 X146.8701 Y86.0589 E0.0798 F2400 ; 0
G1 X145.4558 Y84.6447 E0.0798 F2400 ; 0
G1 E-4 F3000 ; retract
G1 X147.5772 Y82.5233 F7200 ; move to start
G1 E4 F3000 ; un-retract
G1 X147.86 Y82.8062 E0.016 F2400 ; dot
G1 E-4 F3000 ; retract
G1 X148.2843 Y81.8162 F7200 ; move to start
G1 E4 F3000 ; un-retract
G1 X149.6985 Y83.2304 F7200 ; move to start
G1 X151.1127 Y81.8162 E0.0798 F2400 ; 8
G1 X149.6985 Y80.402 E0.0798 F2400 ; 8
G1 X148.2843 Y81.8162 E0.0798 F2400 ; 8
G1 X149.6985 Y83.2304 E0.0798 F2400 ; 8
G1 X151.1127 Y84.6447 E0.0798 F2400 ; 8
G1 X152.5269 Y83.2304 E0.0798 F2400 ; 8
G1 X151.1127 Y81.8162 E0.0798 F2400 ; 8
G1 E-4 F3000 ; retract
G1 Z0.3 F2400 ; zHop
;
; finish
;
M104 S0 ; turn off hotend
M140 S0 ; turn off bed
G1 Z30 X240 Y240 F7200 ; move away from the print
M84 ; disable motors
M502 ; resets parameters from ROM
M501 ; resets parameters from EEPROM
;