; Ender 3 Pro - restore saved settings to EEPROM
; Load in Pronterface and press Print
M117 Restoring settings...
G21                                     ; Units: mm
M200 S0 D1.75                           ; Volumetric extrusion off, 1.75mm filament
M92 X80.00 Y80.00 Z400.00 E424.90       ; Steps per unit
M203 X500.00 Y500.00 Z10.00 E60.00      ; Max feedrates
M201 X500.00 Y500.00 Z100.00 E5000.00   ; Max acceleration
M204 P500.00 R1000.00 T500.00           ; Print/retract/travel acceleration
M205 B20000.00 S0.00 T0.00 J0.08        ; Advanced motion / junction deviation
; M206 X0.00 Y0.00 Z0.00                  ; Home offsets (set in Configuration.h via MIN_POS), in config instead
M420 S1 Z10.00                          ; Bed leveling on, fade height 10mm
M145 S0 H190.00 B50.00 F255             ; Preheat PLA
M145 S1 H240.00 B70.00 F255             ; Preheat ABS
M301 P14.11 I1.13 D43.94                ; Hotend PID
M851 X-27.00 Y-40.00 Z-3.3           ; Probe offsets
M500                                    ; Save to EEPROM
M503                                    ; Report settings to verify
M117 Settings restored