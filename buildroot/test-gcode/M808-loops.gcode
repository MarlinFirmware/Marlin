;
; M808 Repeat Marker Test
;

M808 L3 ; Marker at 54

M118 Outer Loop
M300 S220 P100

M808 L5 ; Marker at 111

M118 Inner Loop
M300 S110 P100

M808
M808
