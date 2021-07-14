;Default theme file
;
C11 E2  R1 G2 B1     ;Screen Background
C11 E3  R29 G49 B15  ;Cursor
C11 E4  R2 G9 B5     ;Title Background
C11 E5  R31 G63 B31  ;Title Text
C11 E6  R31 G63 B31  ;Text
C11 E7  R6 G29 B27   ;Selected
C11 E8  R7 G19 B10   ;Split Line
C11 E9  R31 G63 B31  ;Highlight
C11 E10 R0 G20 B20   ;Status Background
C11 E11 R31 G56 B15  ;Status Text
C11 E12 R6 G15 B8    ;Popup Background
C11 E13 R26 G53 B26  ;Popup Text
C11 E14 R30 G0 B15   ;Alert Background
C11 E15 R31 G56 B15  ;Alert Text
C11 E16 R31 G49 B9   ;Percent Text
C11 E17 R2 G7 B4     ;Bar Fill
C11 E18 R31 G63 B31  ;Indicator value
C11 E19 R31 G63 B31  ;Coordinate value
;
M500 ;Save to EEPROM
M997 ;Reboot