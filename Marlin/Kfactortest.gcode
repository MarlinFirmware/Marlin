G90
M82
M106 S0
M140 S70
M190 S70
M104 S220 T0
M109 S220 T0
G28 ;Homing
G29 L0 ; Load mesh
G29 A ;Ubl activation
G29 F10 ;Fade height
M207 F3000 S3 W60 Z0 ;Set FwRetract
M208 F1500 S0 W0 R1500 ;Set FwRetract recovery
M900 K0.06 ;Set K-Factor
M217 S60       ; Swap length
M217 B-1.5     ; Extra length
M217 E10     ; Prime length
M217 P276     ; Prime speed
M217 R3000   ; Retract speed
M217 U1500   ; UnRetract speed
M217 X467     ; Park X; Park Y
M217 Z2         ; Z Raise
M217 F255     ; Fan Speed
M217 G1      ; Fan time;
M217 V1        ; Enable first tool priming
M217 W1       ; Enable park & Z Raise
T0

G21 ; Millimeter units
G90 ; Absolute XYZ
M83 ; Relative E
M104 S205 ; Set nozzle temperature (no wait)
M190 S70 ; Set bed temperature (wait)
M109 S205 ; Wait for nozzle temp
M204 P500 ; Acceleration
G92 E0 ; Reset extruder distance
G1 X100 Y100 F7200 ; move to start
G1 Z0.3 F1200 ; Move to layer height
;
; print anchor frame
;
G1 X56 Y34.5 F7200 ; move to start
G1 X56 Y140.5 E6.9806 F1200 ; print line
G1 X56.48 Y140.5 F7200 ; move to start
G1 X56.48 Y34.5 E6.9806 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X136 Y34.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X136 Y140.5 E6.9806 F1200 ; print line
G1 X135.52 Y140.5 F7200 ; move to start
G1 X135.52 Y34.5 E6.9806 F1200 ; print line
G1 E-0 F1800 ; retract
;
; start the Test pattern
;
G4 P2000 ; Pause (dwell) for 2 seconds
G1 X56 Y37.5 F7200 ; move to start
M900 K0 ; set K-factor
M117 K0 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y37.5 E1.1974 F1200 ; print line
G1 X116 Y37.5 E2.3947 F4200 ; print line
G1 X136 Y37.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y42.5 F7200 ; move to start
M900 K0.1 ; set K-factor
M117 K0.1 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y42.5 E1.1974 F1200 ; print line
G1 X116 Y42.5 E2.3947 F4200 ; print line
G1 X136 Y42.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y47.5 F7200 ; move to start
M900 K0.2 ; set K-factor
M117 K0.2 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y47.5 E1.1974 F1200 ; print line
G1 X116 Y47.5 E2.3947 F4200 ; print line
G1 X136 Y47.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y52.5 F7200 ; move to start
M900 K0.3 ; set K-factor
M117 K0.3 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y52.5 E1.1974 F1200 ; print line
G1 X116 Y52.5 E2.3947 F4200 ; print line
G1 X136 Y52.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y57.5 F7200 ; move to start
M900 K0.4 ; set K-factor
M117 K0.4 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y57.5 E1.1974 F1200 ; print line
G1 X116 Y57.5 E2.3947 F4200 ; print line
G1 X136 Y57.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y62.5 F7200 ; move to start
M900 K0.5 ; set K-factor
M117 K0.5 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y62.5 E1.1974 F1200 ; print line
G1 X116 Y62.5 E2.3947 F4200 ; print line
G1 X136 Y62.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y67.5 F7200 ; move to start
M900 K0.6 ; set K-factor
M117 K0.6 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y67.5 E1.1974 F1200 ; print line
G1 X116 Y67.5 E2.3947 F4200 ; print line
G1 X136 Y67.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y72.5 F7200 ; move to start
M900 K0.7 ; set K-factor
M117 K0.7 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y72.5 E1.1974 F1200 ; print line
G1 X116 Y72.5 E2.3947 F4200 ; print line
G1 X136 Y72.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y77.5 F7200 ; move to start
M900 K0.8 ; set K-factor
M117 K0.8 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y77.5 E1.1974 F1200 ; print line
G1 X116 Y77.5 E2.3947 F4200 ; print line
G1 X136 Y77.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y82.5 F7200 ; move to start
M900 K0.9 ; set K-factor
M117 K0.9 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y82.5 E1.1974 F1200 ; print line
G1 X116 Y82.5 E2.3947 F4200 ; print line
G1 X136 Y82.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y87.5 F7200 ; move to start
M900 K1 ; set K-factor
M117 K1 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y87.5 E1.1974 F1200 ; print line
G1 X116 Y87.5 E2.3947 F4200 ; print line
G1 X136 Y87.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y92.5 F7200 ; move to start
M900 K1.1 ; set K-factor
M117 K1.1 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y92.5 E1.1974 F1200 ; print line
G1 X116 Y92.5 E2.3947 F4200 ; print line
G1 X136 Y92.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y97.5 F7200 ; move to start
M900 K1.2 ; set K-factor
M117 K1.2 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y97.5 E1.1974 F1200 ; print line
G1 X116 Y97.5 E2.3947 F4200 ; print line
G1 X136 Y97.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y102.5 F7200 ; move to start
M900 K1.3 ; set K-factor
M117 K1.3 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y102.5 E1.1974 F1200 ; print line
G1 X116 Y102.5 E2.3947 F4200 ; print line
G1 X136 Y102.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y107.5 F7200 ; move to start
M900 K1.4 ; set K-factor
M117 K1.4 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y107.5 E1.1974 F1200 ; print line
G1 X116 Y107.5 E2.3947 F4200 ; print line
G1 X136 Y107.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y112.5 F7200 ; move to start
M900 K1.5 ; set K-factor
M117 K1.5 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y112.5 E1.1974 F1200 ; print line
G1 X116 Y112.5 E2.3947 F4200 ; print line
G1 X136 Y112.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y117.5 F7200 ; move to start
M900 K1.6 ; set K-factor
M117 K1.6 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y117.5 E1.1974 F1200 ; print line
G1 X116 Y117.5 E2.3947 F4200 ; print line
G1 X136 Y117.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y122.5 F7200 ; move to start
M900 K1.7 ; set K-factor
M117 K1.7 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y122.5 E1.1974 F1200 ; print line
G1 X116 Y122.5 E2.3947 F4200 ; print line
G1 X136 Y122.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y127.5 F7200 ; move to start
M900 K1.8 ; set K-factor
M117 K1.8 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y127.5 E1.1974 F1200 ; print line
G1 X116 Y127.5 E2.3947 F4200 ; print line
G1 X136 Y127.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y132.5 F7200 ; move to start
M900 K1.9 ; set K-factor
M117 K1.9 ; 
G1 E0 F1800 ; un-retract
G1 X76 Y132.5 E1.1974 F1200 ; print line
G1 X116 Y132.5 E2.3947 F4200 ; print line
G1 X136 Y132.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X56 Y137.5 F7200 ; move to start
;
; Mark the test area for reference
M117 K0
M900 K0 ; Set K-factor 0
G1 X76 Y142.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X76 Y162.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 X116 Y142.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X116 Y162.5 E1.1974 F1200 ; print line
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
;
; print K-values
;
G1 X138 Y35.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X140 Y35.5 E0.1197 F1200 ; 0
G1 X140 Y37.5 E0.1197 F1200 ; 0
G1 X140 Y39.5 E0.1197 F1200 ; 0
G1 X138 Y39.5 E0.1197 F1200 ; 0
G1 X138 Y37.5 E0.1197 F1200 ; 0
G1 X138 Y35.5 E0.1197 F1200 ; 0
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y45.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X140 Y45.5 E0.1197 F1200 ; 0
G1 X140 Y47.5 E0.1197 F1200 ; 0
G1 X140 Y49.5 E0.1197 F1200 ; 0
G1 X138 Y49.5 E0.1197 F1200 ; 0
G1 X138 Y47.5 E0.1197 F1200 ; 0
G1 X138 Y45.5 E0.1197 F1200 ; 0
G1 E-0 F1800 ; retract
G1 X141 Y45.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X141 Y45.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X142 Y45.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X142 Y47.5 F7200 ; move to start
G1 X142 Y49.5 F7200 ; move to start
G1 X144 Y49.5 E0.1197 F1200 ; 2
G1 X144 Y47.5 E0.1197 F1200 ; 2
G1 X142 Y47.5 E0.1197 F1200 ; 2
G1 X142 Y45.5 E0.1197 F1200 ; 2
G1 X144 Y45.5 E0.1197 F1200 ; 2
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y55.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X140 Y55.5 E0.1197 F1200 ; 0
G1 X140 Y57.5 E0.1197 F1200 ; 0
G1 X140 Y59.5 E0.1197 F1200 ; 0
G1 X138 Y59.5 E0.1197 F1200 ; 0
G1 X138 Y57.5 E0.1197 F1200 ; 0
G1 X138 Y55.5 E0.1197 F1200 ; 0
G1 E-0 F1800 ; retract
G1 X141 Y55.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X141 Y55.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X142 Y55.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X142 Y57.5 F7200 ; move to start
G1 X142 Y59.5 F7200 ; move to start
G1 X142 Y57.5 E0.1197 F1200 ; 4
G1 X144 Y57.5 E0.1197 F1200 ; 4
G1 X144 Y59.5 F7200 ; move to start
G1 X144 Y57.5 E0.1197 F1200 ; 4
G1 X144 Y55.5 E0.1197 F1200 ; 4
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y65.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X140 Y65.5 E0.1197 F1200 ; 0
G1 X140 Y67.5 E0.1197 F1200 ; 0
G1 X140 Y69.5 E0.1197 F1200 ; 0
G1 X138 Y69.5 E0.1197 F1200 ; 0
G1 X138 Y67.5 E0.1197 F1200 ; 0
G1 X138 Y65.5 E0.1197 F1200 ; 0
G1 E-0 F1800 ; retract
G1 X141 Y65.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X141 Y65.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X142 Y65.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X142 Y67.5 F7200 ; move to start
G1 X144 Y67.5 E0.1197 F1200 ; 6
G1 X144 Y65.5 E0.1197 F1200 ; 6
G1 X142 Y65.5 E0.1197 F1200 ; 6
G1 X142 Y67.5 E0.1197 F1200 ; 6
G1 X142 Y69.5 E0.1197 F1200 ; 6
G1 X144 Y69.5 E0.1197 F1200 ; 6
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y75.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X140 Y75.5 E0.1197 F1200 ; 0
G1 X140 Y77.5 E0.1197 F1200 ; 0
G1 X140 Y79.5 E0.1197 F1200 ; 0
G1 X138 Y79.5 E0.1197 F1200 ; 0
G1 X138 Y77.5 E0.1197 F1200 ; 0
G1 X138 Y75.5 E0.1197 F1200 ; 0
G1 E-0 F1800 ; retract
G1 X141 Y75.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X141 Y75.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X142 Y75.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X142 Y77.5 F7200 ; move to start
G1 X144 Y77.5 E0.1197 F1200 ; 8
G1 X144 Y75.5 E0.1197 F1200 ; 8
G1 X142 Y75.5 E0.1197 F1200 ; 8
G1 X142 Y77.5 E0.1197 F1200 ; 8
G1 X142 Y79.5 E0.1197 F1200 ; 8
G1 X144 Y79.5 E0.1197 F1200 ; 8
G1 X144 Y77.5 E0.1197 F1200 ; 8
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y85.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X138 Y87.5 E0.1197 F1200 ; 1
G1 X138 Y89.5 E0.1197 F1200 ; 1
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y95.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X138 Y97.5 E0.1197 F1200 ; 1
G1 X138 Y99.5 E0.1197 F1200 ; 1
G1 E-0 F1800 ; retract
G1 X139 Y95.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X139 Y95.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X140 Y95.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X140 Y97.5 F7200 ; move to start
G1 X140 Y99.5 F7200 ; move to start
G1 X142 Y99.5 E0.1197 F1200 ; 2
G1 X142 Y97.5 E0.1197 F1200 ; 2
G1 X140 Y97.5 E0.1197 F1200 ; 2
G1 X140 Y95.5 E0.1197 F1200 ; 2
G1 X142 Y95.5 E0.1197 F1200 ; 2
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y105.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X138 Y107.5 E0.1197 F1200 ; 1
G1 X138 Y109.5 E0.1197 F1200 ; 1
G1 E-0 F1800 ; retract
G1 X139 Y105.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X139 Y105.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X140 Y105.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X140 Y107.5 F7200 ; move to start
G1 X140 Y109.5 F7200 ; move to start
G1 X140 Y107.5 E0.1197 F1200 ; 4
G1 X142 Y107.5 E0.1197 F1200 ; 4
G1 X142 Y109.5 F7200 ; move to start
G1 X142 Y107.5 E0.1197 F1200 ; 4
G1 X142 Y105.5 E0.1197 F1200 ; 4
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y115.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X138 Y117.5 E0.1197 F1200 ; 1
G1 X138 Y119.5 E0.1197 F1200 ; 1
G1 E-0 F1800 ; retract
G1 X139 Y115.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X139 Y115.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X140 Y115.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X140 Y117.5 F7200 ; move to start
G1 X142 Y117.5 E0.1197 F1200 ; 6
G1 X142 Y115.5 E0.1197 F1200 ; 6
G1 X140 Y115.5 E0.1197 F1200 ; 6
G1 X140 Y117.5 E0.1197 F1200 ; 6
G1 X140 Y119.5 E0.1197 F1200 ; 6
G1 X142 Y119.5 E0.1197 F1200 ; 6
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
G1 X138 Y125.5 F7200 ; move to start
G1 Z0.3 F1200 ; zHop
G1 E0 F1800 ; un-retract
G1 X138 Y127.5 E0.1197 F1200 ; 1
G1 X138 Y129.5 E0.1197 F1200 ; 1
G1 E-0 F1800 ; retract
G1 X139 Y125.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X139 Y125.9 E0.0239 F1200 ; dot
G1 E-0 F1800 ; retract
G1 X140 Y125.5 F7200 ; move to start
G1 E0 F1800 ; un-retract
G1 X140 Y127.5 F7200 ; move to start
G1 X142 Y127.5 E0.1197 F1200 ; 8
G1 X142 Y125.5 E0.1197 F1200 ; 8
G1 X140 Y125.5 E0.1197 F1200 ; 8
G1 X140 Y127.5 E0.1197 F1200 ; 8
G1 X140 Y129.5 E0.1197 F1200 ; 8
G1 X142 Y129.5 E0.1197 F1200 ; 8
G1 X142 Y127.5 E0.1197 F1200 ; 8
G1 E-0 F1800 ; retract
G1 Z0.4 F1200 ; zHop
;
; FINISH
;
M104 S0 ; Turn off hotend
M140 S0 ; Turn off bed
G1 Z30 X200 Y200 F7200 ; Move away from the print
M84 ; Disable motors
M501 ; Load settings from EEPROM
;