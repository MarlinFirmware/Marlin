# 3D Chameleon - WIP

The 3D Chameleon is an "up to the extruder" filament changing device that turns any single extruder printer into a multi-color master. The Chameleon does the bulk of the filament changing labor, loading the filament as far as the extruder gears, at which point the printer takes over. The Chameleon is perfectly suited for lightweight direct drive extruders and Bowden printers with reliable feeding mechanisms. Combine with a dry box for the ultimate in filament changing powers.

Different versions of Chameleon may require different moves, but we'll just aim for Chameleon MK4 firmware and encourage Chameleon users to update.

See https://www.youtube.com/playlist?list=PLB_0YGFjbOnbGTlFWEU46AW1Y504RGmI0

### Basic operation example:

- PULSE x 7, RELEASE: HOME and SELECT E0

- PULSE x 2, RELEASE: SELECT E1
- HOLD : LOAD E1 (~1in/second)
- RELEASE : OPEN

- PULSE x 4, RELEASE: SELECT E1
- HOLD : UNLOAD E1 (~1in/second)
- RELEASE : SELECT E3
- HOLD : LOAD E3 (~1in/second)
- RELEASE: OPEN

### G-code Notes:

The [Chameleon MK4 G-code generator](https://www.3dchameleon.com/mk3-mode-3-gcode-generator) gives the Prusa Slicer Tool-change output below for [ X-Axis, 222mm, 4s, +86mm ]:

```gcode
; Auto generated 3D Chameleon Mk4 Tool Change GCode for PrusaSlicer
; - 3D Chameleon Mk4 Tool T{next_extruder} -

; begin switch from extruder {previous_extruder} to extruder #{next_extruder}
M117 Unloading Tool T[previous_extruder]
M83 ; turn relative distances for the extruder

; unload extruder {previous_extruder}
G92 E0

; go to holding position to unload the stock extruder
G0 X219 F2000  ; <<----- EDIT THIS LINE TO SET THE INITIAL LOCATION OF THE BUTTON

; if this is the first load, then don't unload
G91 ; move to relative mode
M83

; load extruder #{next_extruder}
M117 Loading Tool T{next_extruder}

{if previous_extruder>-1}

; back out the filament for cutting
G0 E-20 F500 ; retract a bit, adjust this to tune waste

; <<<< insert mechanical cutter code here! >>>>

;<<< Start Of Tip Shaping- Not Needed If You Have A 3DClippy Filament Cutter! >>>

M109 R180    ; cool down to prevent swelling
M302 S0      ; enable cold extrusion
M106 S255
G0 E20 F1500
G0 E-5 F500
M109 R165    ; cool down to prevent swelling
G0 E5 F1500
G0 E-1 F500
M109 R155    ; cool down to prevent swelling
G0 E1 F1500
G0 E-25 F500
M109 R150; cool down to prevent swelling
G0 E24 F1500 ; last tip dip with cold tip
G0 E-24      ; last tip dip with cold tip
M109 R180    ; ok... go back up in temp so we can move the extruder
G0 E-80 F500 ; back out of the extruder
G92 E0
M104 S[temperature]

;<<< End Of Tip Shaping >>>

; we skipped to here if it was extruder -1
{endif}

{if previous_extruder>-1}

; retract it back out of the extruder
G92 E0
G0 E-86 F2000
G92 E0

G0 E-30 F2000 ; <<<---- if mechanical cutter, change to a positive value of at least 5mm
G92 E0

; ok - out of extruder - press button to remove it all the way
G91 ; move to relative mode
M82

; we skipped to here if it was extruder -1
{endif}

; press the button to select the new extruder
G0 X3 F2000

{if next_extruder==0}
G4 P500     ; dwell for .5 seconds - adjust this to match your machines single pulse time
{endif}
{if next_extruder==1}
G4 P1000    ; dwell for 1.0 seconds - adjust this to match your machines two pulse time
{endif}
{if next_extruder==2}
G4 P1500    ; dwell for 1.5 seconds - adjust this to match your machines three pulse time
{endif}
{if next_extruder==3}
G4 P2000    ; dwell for 2.0 seconds - adjust this to match your machines four pulse time
{endif}
G0 X-3

G4 P2000    ; wait for cutter to work

; ok command selected - back out filament if needed after cut

{if previous_extruder>-1}

; UNLOAD TO SPLITTER (25mm/s)

G0 X3 F2000
G4 P2500
G0 X-3
G4 P400

{endif}

; RELOAD TO SPLITTER (100ms per mm?)

G0 X3 F2000
G4 P2500
G0 E25 F1500  ; <<<--- adjust this E value to tune extruder loading
G0 X-3
G4 P400

G92 E0
G90 ; move back to absolute mode
M83 ; but make sure the extruder stays in relative

; reload filament all the way into extruder

G92 E0
G0 E61 F2000 ; <<<<<-----------  EDIT THIS DISTANCE TO FINE TUNE THE FILAMENT LOADING TO STOP BLOBBING OR UNDEREXTRUSION
G92 E0

M83       ; turn relative distances for the extruder
M302 S180 ; prevent cold extrusion

M117 3D Chameleon Tool T{next_extruder}
```

- Setup requires XYZ position of switch, switch axis, switch direction, and pulse duration.
- An inch is ~25mm
- 7 pulses (e.g., hold 3.5 seconds) to "home" the 3D Chameleon.
- TODO: MarlinUI editable button press distance (default 3mm). Allow other end button mounting.

### Color Change Sequence:

- Move (X) to "initial free position" (button - 3)

- Changing from some other color? (oldtool>-1)
  - Slow Retract (8mm/s) -20mm of filament (for cut) (-20)
  - Mechanical Cutter (optional)

  - Cool down to 180C
  - Allow cold E moves
  - Turn cooling fan to high

  - Fast (25mm/s) extrude +20, Slow (8mm/s) retract -5 ( -5)
  - Then 165C - Fast extrude +5, Slow retract -1 ( -1)
  - Then 155C - Fast extrude +1, Slow retract -25 (-25)
  - Then 150C - Fast extrude +24, Fast retract -24 (-25)

  - Heat up to 180C
  - Slow retract -80 (eject from extruder) (-105)
  - Turn heat back up to Target Temperature (no wait)
  - Speedy (33mm/s) retract -86 extruder-to-hotend (-191)
  - Speedy retract -30 (-221) (for mechanical cutter use forward move >= +5)

- Select the New Color
  - Speedy Move (X) +3mm to "pressed position"
  - Dwell for PULSE \* TOOL = SELECT COLOR
  - Speedy Move (X) -3mm to "free position"
  - Dwell 2000 waiting for CUTTER

- UNLOAD Old Color (if oldtool>-1)
  - Speedy Move (X) to "pressed position" (UNLOAD OLD COLOR)
  - Dwell for 4s (~100mm)
  - Speedy Move (X) to "free position" (OPEN)
  - Dwell 400ms to settle

- LOAD New Color
  - Speedy Move (X) to "pressed position" (LOAD NEW COLOR)
  - Dwell for 4s (~100mm)
  - LOAD Extruder 25mm at 25mm/s (around ~1s). Configure extruder to catch the filament just before the Chameleon releases it.
  - Speedy Move (X) to "free position" (OPEN)
  - Dwell 400ms to settle

  - Speedy Move E +61mm (86mm - 25mm) to load to extruder

### Configurable Settings

- Initial E Retract Distance (for optional cut).
- BUTTON Standby Position { 211, 0, 0 } and Press Distance { +3, 0, 0 }.
- Speedy 2000 (33), Fast 1500 (25), and Slow 500 (8) Feedrates.
- Tipping Moves, e.g., { 180, -20, +20, -5, 165, +5, -1, 155, +1, -25, 150, +24, -24 }
  - Split them up?
    - Tipping Temps, e.g., { 180, 165, 155, 150 }
    - Hot Moves, { -20, +20, -5 } (20 == Initial Retract)
    - Warm Moves { +5, -1 }
    - Cool Moves { +1, -25 }
    - Cold Moves { +24, -24 }

### Wired Switch

The 3D Chameleon can also be wired to a GPIO pin with a transistor for direct digital control with significantly shortened pulses.
