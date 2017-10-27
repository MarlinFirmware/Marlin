#!/bin/bash

# Test generator for K-Factor calibration
# Version 0.9 by Andre Ruiz (aka Token47), andre (dot) ruiz (at) gmail (dot) com

# To Do:
# put a list of the values used on the begining of the gcode as comments on simplify3d style

set -u

NOZZLE_TEMP="205" # C degrees
BED_TEMP="60" # C degrees
SLOW_SPEED="1200" # mm/min
FAST_SPEED="4200" # mm/min
MOVE_SPEED="7200" # mm/min
RETRACTION="1.000" # mm
START_X="30"
START_Y="30"
LAYER_HEIGHT="0.200" # mm 
EXTRUSION_FACTOR="0.037418" # calculated for 0.4mm nozzle, 0.2mm layer height and 1.75mm filament
EXTRUSION_MULT="1.1" # arbitraty multiplier, just for testing, should be 1.0 normally
for a in 5 20 40; do
		declare "EXTRUSION_${a}mm"="$(awk "BEGIN { 
				printf \"%f\", $EXTRUSION_FACTOR * ${a} * $EXTRUSION_MULT
		}")"
done

cat <<EOF
; K-FACTOR TEST
;
M190 S${BED_TEMP} ; set and wait for bed temp
M104 S${NOZZLE_TEMP} ; set nozzle temp and continue
G28 ; home all axys
G29 ; execute bed automatic leveling compensation
M109 S${NOZZLE_TEMP} ; block waiting for nozzle temp
G21 ; set units to millimeters
M204 S500 ; lower acceleration to 500mm/s2 during the test
M83 ; use relative distances for extrusion
G90 ; use absolute coordinates
;
; go to layer height and prime nozzle on a line to the left
G1 X0 Y$((START_Y+100)) F${MOVE_SPEED}
G1 Z${LAYER_HEIGHT} F${SLOW_SPEED}
G1 X0 Y$((START_Y+40)) E10 F${SLOW_SPEED} ; extrude some to start clean
G1 E-${RETRACTION}
G1 X${START_X} Y${START_Y} F${MOVE_SPEED} ; extrude some to start clean
;
; start the test (all values are relative coordinates)
;
G91 ; use relative coordinates
EOF

for K in 0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100; do
		cat << EOF
M900 K${K}
G1 E${RETRACTION}
G1 X20 Y0 E${EXTRUSION_20mm} F${SLOW_SPEED}
G1 X40 Y0 E${EXTRUSION_40mm} F${FAST_SPEED}
G1 X20 Y0 E${EXTRUSION_20mm} F${SLOW_SPEED}
G1 E-${RETRACTION}
G1 X-80 Y5 F${MOVE_SPEED}
EOF
		done

cat << EOF
;
; mark the test area for reference
;
G1 X20 Y0 F${MOVE_SPEED}
G1 E${RETRACTION}
G1 X0 Y20 E${EXTRUSION_20mm} F${SLOW_SPEED}
G1 E-${RETRACTION}
G1 X40 Y-20 F${MOVE_SPEED}
G1 E${RETRACTION}
G1 X0 Y20 E${EXTRUSION_20mm} F${SLOW_SPEED}
G1 E-${RETRACTION}
;
; finish
;
G4 ; wait
M104 S0 ; turn off hotend
M140 S0 ; turn off bed
G90 ; use absolute coordinates
G1 Z30 Y200 F${MOVE_SPEED}; move away from the print
M84 ; disable motors
M502 ; resets parameters from ROM (for those who do not have an EEPROM)
M501 ; resets parameters from EEPROM (preferably)
;
EOF

