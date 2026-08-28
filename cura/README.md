# Cura setup — Ender 3 Pro / Sprite Extruder Pro

For Cura 5.13, matched to the firmware on branch `ender3pro-crtouch-sfs`.

```
definitions/ender3pro_sprite.def.json            printer
extruders/ender3pro_sprite_extruder_0.def.json   hot end
materials/sprite_pro_pla.xml.fdm_material        filament
materials/sprite_pro_petg.xml.fdm_material       filament
materials/sprite_pro_tpu.xml.fdm_material        filament
Sprite_TPU.curaprofile                           print profile - REQUIRED
Sprite_PETG.curaprofile                          print profile - optional
```

Settings are layered, each level overriding the one below:

```
1. printer definition   the machine
2. extruder definition  the hot end
3. material             the filament        <- temps, fan, retraction
4. built-in quality     Cura's Low..Super   <- layer height
5. quality changes      these .curaprofile  <- only what 3 and 4 can't do
```

---

## Can you just use Cura's four built-in quality levels?

**Mostly yes.** With the materials installed, Low / Standard / Dynamic / Super
handle layer height and give sensible speeds for rigid filament. Per material:

| | Built-in quality enough? | Why |
|---|---|---|
| **PLA** | **Yes** — nothing else needed | Material carries 200/60, fan, retraction. Stock speeds suit the frame. |
| **PETG** | **Yes**, profile optional | Material carries 240/80. The profile only adds a longer minimum layer time and a tidier seam. |
| **TPU** | **No — profile required** | Speed. See below. |

### Why TPU needs a profile

**A material file cannot set speed.** The `.xml.fdm_material` schema covers
temperature, bed, fan, retraction and standby — there is no speed key.

Cura's built-in profiles print at roughly 50 mm/s. TPU at that rate buckles in
the extruder gear instead of feeding, because flexible filament acts as a spring
between the drive gear and the melt zone. It needs ~20 mm/s, and there is
nowhere to put that except a quality profile.

`Sprite_TPU.curaprofile` therefore carries speeds, brim, and a gyroid infill
that avoids the sharp direction changes TPU handles badly. **Use it.** Selecting
the TPU material without it will jam the extruder.

`Sprite_PETG.curaprofile` is genuinely optional — minimum layer time 8 s so
small layers get time to cool, sharpest-corner seam, and slightly reduced
speeds. Stock quality profiles work fine without it.

---

## Install

**1. Printer** — copy into Cura's config folder, then restart Cura:

```
definitions\ender3pro_sprite.def.json           ->  %APPDATA%\cura\5.13\definitions\
extruders\ender3pro_sprite_extruder_0.def.json  ->  %APPDATA%\cura\5.13\extruders\
```

Then **Add Printer → non-networked → Creality3D → Ender 3 Pro Sprite**.

**2. Materials** — copy in, then restart Cura:

```
materials\*.xml.fdm_material  ->  %APPDATA%\cura\5.13\materials\
```

They appear under brand **Sprite Pro** in the material dropdown.

**3. Profiles** — **Preferences → Configure Cura → Profiles → Import**, for
`Sprite_TPU.curaprofile` and, if you want it, `Sprite_PETG.curaprofile`.

### If a material or printer doesn't show up

Custom XML and JSON are the fussiest part of Cura and failures are silent —
nothing appears, no error dialog. `%APPDATA%\cura\5.13\cura.log` says why.

The reliable fallback for materials is to build them in the UI instead:
**Preferences → Materials → Generic PLA → Duplicate**, rename, and type the
values from the table below. That guarantees a valid GUID and correct printer
compatibility, which is where hand-written material files usually go wrong.

---

## What's in the materials

| | PLA | PETG | TPU |
|---|---|---|---|
| Print temperature | 200 °C | 240 °C | 225 °C |
| Bed temperature | 60 °C | 80 °C | 45 °C |
| Standby temperature | 175 °C | 190 °C | 180 °C |
| Print cooling | 100 % | 50 % | 40 % |
| **Retraction amount** | **1.0 mm** | **1.2 mm** | **0.5 mm** |
| **Retraction speed** | **40 mm/s** | **35 mm/s** | **25 mm/s** |
| Density | 1.24 | 1.27 | 1.21 |

**Retraction is the number that matters.** These are direct-drive values. The
old Bowden setup wanted 5–6 mm; the Sprite Pro wants about 1. Slicing with a
Bowden material will grind the filament flat and jam the extruder — the most
likely cause of a failed print after this conversion.

Two things deliberately left out of the material files, because the schema's
support for them is unreliable: **flow** (PETG usually wants ~95 %, TPU ~105 %)
and per-machine overrides. Set flow in Cura's print settings if you need it.

## What's in the TPU profile

| Setting | Value |
|---|---|
| Print speed | 20 mm/s |
| Outer wall | 15 mm/s |
| First layer | 12 mm/s |
| Travel | 80 mm/s |
| Min travel before retract | 3.0 mm |
| Infill | 15 % gyroid |
| Adhesion | Brim, 8 mm |

---

## Support

Support is a **geometry** decision, not a material one — you turn it on when a
model has overhangs, whatever it's printed in. So neither profile enables it.
What they do carry is *how support behaves once you tick the box*, because the
right gaps differ enormously by filament.

| | Removal | Verdict |
|---|---|---|
| **PLA** | Snaps off cleanly | Use freely — Cura's defaults are fine, no profile needed |
| **PETG** | **Welds to itself** | Avoid where you can; the profile widens the gaps |
| **TPU** | Stretches instead of breaking | Avoid entirely; the profile is damage limitation |

| Setting | PLA (stock) | PETG | TPU |
|---|---|---|---|
| Overhang angle | 45° | 55° | 60° |
| Z distance | 0.2 mm | **0.3 mm** | **0.4 mm** |
| XY distance | 0.7 mm | 0.8 mm | 1.0 mm |
| Interface | On | **Off** | **Off** |

Turning the interface **off** for PETG looks wrong but isn't: a dense interface
layer is precisely what fuses to the part. A wider Z gap plus no interface
gives a support you can actually remove, at some cost to the overhang surface.

Raising the overhang angle (55°/60°) also means less support gets generated in
the first place — worth it when removal is the painful part.

Second-order effect: PETG runs **50 % fan** against PLA's 100 %, so it bridges
and overhangs less well. The same model may need support in PETG where PLA
manages unaided. Orient to avoid overhangs first; chamfer rather than overhang
where you control the design.

---

## Build plate adhesion

Mostly a **geometry** decision too — small footprints and tall thin parts need
a brim regardless of material. But each filament has a characteristic failure:

| | Problem | Setting |
|---|---|---|
| **PLA** | None to speak of at 60 °C | Skirt |
| **PETG** | **Sticks too well** | Skirt — and never a raft |
| **TPU** | Small parts lift | **Brim, 8 mm** |

**PETG's problem is removal, not adhesion.** At 80 °C on PEI it bonds hard
enough to tear the coating off the sheet if the first layer is over-squished.
The fix is *less* first-layer squish, not more adhesion:

- If parts are hard to remove, raise the Z offset slightly — babystep `+0.02`
  and `M500` — rather than adding brims or glue
- Let the bed cool fully before removing; PETG releases as it contracts
- Glue stick on PEI acts as a **release agent** for PETG, not an adhesive
- **Never use a raft with PETG.** It welds and you'll destroy the part getting
  it off

**TPU** gets an 8 mm brim because flexible parts are usually small with little
bed contact, and a brim you can peel is cheap insurance.

Deliberately not set anywhere: `layer_height` and `layer_height_0`. Overriding
those would defeat the point of using Cura's four built-in quality levels.

---

## Machine settings, if you'd rather type them

The printer definition sets all of this. Only needed if the JSON is rejected —
**Settings → Printer → Manage Printers → Machine Settings**.

The dialog has two tabs and the split is meaningful. **Printer** prepares the
*machine*; **Extruder 1** prepares the *material*. Cura emits the extruder
scripts when the extruder is activated, which on a single-extruder printer
happens once per print, so both pairs run.

### Tab 1 — "Printer"

| Setting | Value |
|---|---|
| X (Width) | **225** — X travel reduced; the head cable fouls the upright at full travel |
| Y (Depth) | 235 |
| Z (Height) | 250 |
| Build plate shape | Rectangular |
| Origin at centre | No |
| Heated bed | Yes |
| G-code flavour | Marlin |

**Start G-code** — machine preparation and heating, no extrusion:

```gcode
G90 ; absolute positioning
M82 ; absolute extrusion
G92 E0
M117 {material_type} heating bed
M140 S{material_bed_temperature_layer_0} ; start bed, don't wait
M104 S{material_standby_temperature} ; idle nozzle - no ooze while probing
M190 S{material_bed_temperature_layer_0} ; wait for bed
M109 S{material_standby_temperature} ; wait for idle nozzle
M117 {material_type} homing
G28
M420 S1 Z10 ; load stored mesh, fade out over 10mm
M117 {material_type} heating nozzle
M109 S{material_print_temperature_layer_0} ; full temp, last moment
G92 E0
G1 Z2.0 F3000
```

Three things this does deliberately:

**`M420 S1 Z10` instead of `G29`.** Loads the mesh already stored in EEPROM
rather than re-probing every print — much faster, and `Z10` fades the correction
out over the first 10 mm so tall prints aren't distorted by bed shape they've
long since left behind. Re-probe with `G29` only after moving the bed or
changing the probe.

**The nozzle idles at `{material_standby_temperature}` while homing** so it
doesn't ooze onto the bed during probing, then goes to full temperature at the
last moment. The material files set standby to 175/190/180.

**Placeholders, not fixed numbers.** Since the materials now carry the
temperatures, hardcoding `S240` would print PLA and TPU at PETG's temperature.

> **Watch for Cura's own preheat.** Cura normally prepends its own
> `M140`/`M190`/`M104`/`M109` at full temperature *before* your start script.
> When that happens the nozzle reaches print temperature and waits there before
> homing, which defeats the idle-temperature trick and costs a heat-cool-heat
> cycle. Slice something and check the first 20 lines: if you see a bare
> `M109 S240` above `G90`, Cura is still adding its own and the placeholders
> above aren't suppressing it.

**End G-code**:

```gcode
M104 S0
M140 S0
M107
G91
G1 Z10 F3000           ; lift away from the part
G90
G27                    ; park (NOZZLE_PARK_FEATURE)
M84
M117 {material_type} complete
```

### Tab 2 — "Extruder 1"

| Setting | Value |
|---|---|
| Nozzle size | 0.4 |
| Compatible material diameter | 1.75 |
| Nozzle offset X / Y | 0 / 0 |

**Extruder Start G-code** — material priming. Two passes up and back, which
purges more reliably than a single line and leaves a tidier start:

```gcode
M117 {material_type} priming
G92 E0
G1 X5.1 Y20 Z0.3 F5000 ; prime line 1 start
G1 X5.1 Y200 Z0.3 F1500 E15
G1 X5.4 Y200 Z0.3 F5000
G1 X5.4 Y20 Z0.3 F1500 E30 ; prime line 2, back down
G92 E0
G1 Z2.0 F3000
G1 X10 Y20 Z0.3 F5000 ; step off the prime
M117 {material_type} printing
```

### Display messages

Every `M117` names the material, so the LCD tells you what's loaded at each
stage — useful when a print starts unattended and you can't remember which
spool is on:

```
PETG heating bed  →  PETG homing  →  PETG heating nozzle
                  →  PETG priming →  PETG printing  →  PETG complete
```

`{material_type}` renders as `PLA`, `PETG` or `TPU`. The longest message is
19 characters, inside the stock display's 20 columns, so nothing truncates —
and `STATUS_MESSAGE_SCROLLING` is enabled in the firmware if you ever lengthen
them.

This runs **after** the printer start G-code, so the nozzle is already at full
temperature by the time it primes. That ordering matters — priming a cold
nozzle would skip steps and grind. If you'd rather keep the whole sequence in
one place, appending this to the Printer tab's start G-code works identically.

**Extruder End G-code**:

```gcode
G1 E-3 F1800           ; short retract, direct drive
```

Raise the `E15` prime to around `E18` for PETG and TPU if the first few
centimetres come out starved — TPU especially takes a while to build pressure.

---

## Why the speeds are modest

The firmware caps acceleration, so higher slicer speeds are unreachable:

| Firmware limit | Value |
|---|---|
| `DEFAULT_ACCELERATION` (print) | 500 mm/s² |
| `DEFAULT_TRAVEL_ACCELERATION` | 1000 mm/s² |
| `DEFAULT_XJERK` / `YJERK` | 5.0 |
| `DEFAULT_MAX_FEEDRATE` E | 50 mm/s |

At 500 mm/s² it takes ~2.5 mm of travel to reach 50 mm/s. Asking for 150 mm/s
means most moves never get there while cornering quality suffers. To print
faster, raise the firmware acceleration first — the slicer is not the
bottleneck. The printer definition carries these limits so Cura's time
estimates are realistic.

---

## Still unverified on the machine

Starting points, not measured results:

- **E-steps** are `424.9` from the Ender-3 S1 Pro config, never checked against
  100 mm of actual extrusion. Every flow figure moves with them.
- **Retraction distances** are typical Sprite Pro values. Print a stringing test.
- **Temperatures** are conventional, not tuned to your particular filament.
