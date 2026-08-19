# Building Marlin for an Ender 3 Pro (Creality 4.2.2) — CR Touch, BTT SFS 2.0, Sprite Extruder Pro

**Target setup:** Ender 3 Pro · Creality V4.2.2 board · stock rotary-knob LCD · CR Touch on the 5-pin header · BigTreeTech SFS v2.0 in dual switch+motion mode · Creality Sprite Extruder Pro (direct drive) · Windows

This describes the configuration actually committed on branch `ender3pro-crtouch-sfs`, not a generic recipe. Where a value is a starting point rather than a verified fact, it says so.

---

## 1. Install the tools

1. **Visual Studio Code** — https://code.visualstudio.com/
2. **PlatformIO IDE extension** — `Ctrl+Shift+X`, search `PlatformIO IDE`, Install. First launch downloads a Python toolchain in the background; wait for the ant-head icon in the sidebar before doing anything else.
3. **Auto Build Marlin extension** (optional) — gives a one-click build button and a config editor.
4. **CH340 USB driver** — only needed to talk to the printer over USB. SD-card flashing does not need it.

`pio` is not on the Windows PATH. The executable lives at:

```
%USERPROFILE%\.platformio\penv\Scripts\pio.exe
```

## 2. Get the source

This repo is a fork of MarlinFirmware/Marlin. The working branch is `ender3pro-crtouch-sfs`, based on `bugfix-2.1.x`.

```
git fetch upstream
git switch ender3pro-crtouch-sfs
```

If you ever need to re-derive the config from scratch, the example set comes from the **MarlinFirmware/Configurations** repo, on the **same branch** as this fork's upstream base:

```
config/examples/Creality/Ender-3 Pro/CrealityV422/
```

Mismatched branches between Marlin and Configurations cause misleading build errors. Both files stamp a `CONFIGURATION_H_VERSION`; they must match.

## 3. Files that make up the config

Four files differ from upstream — not two:

| File | Why |
|---|---|
| `Marlin/Configuration.h` | Main config |
| `Marlin/Configuration_adv.h` | Advanced config |
| `Marlin/_Bootscreen.h` | **Required** — `SHOW_CUSTOM_BOOTSCREEN` is enabled |
| `Marlin/_Statusscreen.h` | **Required** — `CUSTOM_STATUS_SCREEN_IMAGE` is enabled |

The build **fails with a missing include** if the two screen headers are absent. Either copy them from the example set or disable those two defines in `Configuration_adv.h`.

## 4. Build

Open the folder containing `platformio.ini` — the repo root, **not** the inner `Marlin\` folder.

`default_envs` in `platformio.ini` is deliberately left at `mega2560` so the upstream diff stays limited to config files. Specify the environment explicitly:

```
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run -e STM32F103RE_creality
```

Clean build:

```
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run -e STM32F103RE_creality -t clean
```

Output:

```
.pio\build\STM32F103RE_creality\firmware-YYYYMMDD-HHMMSS.bin
```

Marlin auto-timestamps the filename, which is required — the bootloader skips filenames it has already flashed.

The 4.2.2 is essentially always an STM32F103RET6 (512 KB). If the board refuses to boot, it may be an RC variant — switch to `STM32F103RC_creality`.

## 5. Key configuration decisions

### Probe — CR Touch

```cpp
#define BLTOUCH                                    // CR Touch speaks the BLTouch protocol
#define USE_PROBE_FOR_Z_HOMING
//#define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN       // MUST stay disabled
#define NOZZLE_TO_PROBE_OFFSET { -31.8, -40.5, 0 } // Sprite Pro bracket; Z is a placeholder
#define AUTO_BED_LEVELING_BILINEAR
#define GRID_MAX_POINTS_X 5
#define MULTIPLE_PROBING 2
#define PREHEAT_BEFORE_LEVELING                    // 120 C nozzle / 50 C bed
#define Z_MIN_PROBE_REPEATABILITY_TEST             // enables M48
#define Z_SAFE_HOMING
#define RESTORE_LEVELING_AFTER_G28
```

**Both** `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` being off **and** `USE_PROBE_FOR_Z_HOMING` being on are what free **PA7** for the filament motion sensor. `Conditionals-5-post.h` gates the Z-MIN endstop on:

```cpp
#if _USE_STOP(Z,,MIN,...) && (DISABLED(USE_PROBE_FOR_Z_HOMING) || ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN))
  #define USE_Z_MIN 1
#endif
```

With the probe homing Z and the endstop-pin option off, no Z-MIN endstop is instantiated on PA7 (`Z_STOP_PIN`). Enabling either condition reclaims the pin and breaks the SFS.

The probe offset comes from the Ender-3 S1 Pro example config — the factory Sprite Pro + CR Touch machine. The CR Touch bolts directly to the Sprite with no adapter shift, so the geometry transfers.

**Consequence:** the probe cannot reach the whole bed. Probeable area is roughly **X 32–235, Y 40–235**. `EXTRAPOLATE_BEYOND_GRID` extends the mesh over the unprobed strips. This is expected, not a fault.

### Filament sensor — SFS 2.0 dual mode

Switch output → J1 (**PA4**). Motion/encoder output → Z endstop port (**PA7**).

```cpp
#define FILAMENT_RUNOUT_SENSOR
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #define FIL_RUNOUT_ENABLED_DEFAULT true
  #define NUM_RUNOUT_SENSORS   1
  #define FIL_RUNOUT_STATE     LOW
  #define FIL_RUNOUT_PULLUP
  #define FILAMENT_RUNOUT_SCRIPT "M600"
  #define FILAMENT_RUNOUT_DISTANCE_MM 7
  #ifdef FILAMENT_RUNOUT_DISTANCE_MM
    #define FILAMENT_MOTION_SENSOR
    #if ENABLED(FILAMENT_MOTION_SENSOR)
      #define FILAMENT_SWITCH_AND_MOTION
      #if ENABLED(FILAMENT_SWITCH_AND_MOTION)
        #define FILAMENT_MOTION_DISTANCE_MM 7.0   // <-- this is the jam threshold
        #define NUM_MOTION_SENSORS 1
        #define FIL_MOTION1_PIN PA7
      #endif
    #endif
  #endif
#endif
```

**The two distances are different things and are easy to confuse:**

| Define | What it does | Tune live with |
|---|---|---|
| `FILAMENT_RUNOUT_DISTANCE_MM` | How far to keep printing after the **switch** reports filament gone | `M412 D<mm>` |
| `FILAMENT_MOTION_DISTANCE_MM` | Missing encoder motion that counts as a **jam** | `M412 L<mm>` |

`FILAMENT_RUNOUT_DISTANCE_MM` must be defined for the motion block to exist at all, but it is *not* what fires `M600` on a jam.

`FIL_MOTION1_STATE` and `FIL_MOTION1_PULLUP` are left undefined on purpose — they inherit `FIL_RUNOUT_STATE` and `FIL_RUNOUT_PULLUP` automatically (`Conditionals-3-etc.h`).

### Hotend — Sprite Extruder Pro

```cpp
#define TEMP_SENSOR_0 1          // confirmed: S1 Pro ships type 1 at 300 C
#define HEATER_0_MAXTEMP 300     // -> 285 C settable
#define DEFAULT_KP  14.49
#define DEFAULT_KI   0.96
#define DEFAULT_KD  54.59
```

Marlin caps the settable target at `MAXTEMP - HOTEND_OVERSHOOT` (15), so `300` yields **285 °C settable**. It cannot go much higher regardless: `temperature.cpp` asserts `MAXTEMP <= table_max - 15`, and thermistor table `1` ends at 320 °C, making `305` the hard ceiling. `300` puts the thermal cutoff exactly at the hotend's rating.

The PID constants come from the Ender-3 S1 Pro config. They are a **starting point, not a specification** — the non-Pro S1 carries two different `KP` values (22.20 and 19.54) for the same printer on different board revisions, which shows these are one machine's `M303` output. Retune on your own hardware.

### Extruder and motion — Sprite Extruder Pro

```cpp
#define DEFAULT_AXIS_STEPS_PER_UNIT { 80, 80, 400, 424.9 }
#define DEFAULT_MAX_FEEDRATE        { 500, 500, 5, 50 }
#define DEFAULT_MAX_ACCELERATION    { 500, 500, 100, 2500 }
#define DEFAULT_XJERK  5.0
#define DEFAULT_YJERK  5.0
#define INVERT_E0_DIR false        // VERIFY — see below
```

X/Y max acceleration stays at the Ender 3 Pro's `500`. The S1's `1000` belongs to the S1's frame, not to the Sprite.

`INVERT_E0_DIR false` matches every Sprite-equipped factory config, but those are Creality boards with Creality harnesses. **It is not runtime-settable**, so getting it wrong costs a reflash. Check it first (step 7).

### M600 and pause

```cpp
#define NOZZLE_PARK_FEATURE       // in Configuration.h, not Configuration_adv.h
#define ADVANCED_PAUSE_FEATURE
#define PARK_HEAD_ON_PAUSE
#define EMERGENCY_PARSER
#define FILAMENT_CHANGE_UNLOAD_LENGTH    30   // direct drive: no tube to clear
#define FILAMENT_CHANGE_FAST_LOAD_LENGTH 30
```

`ADVANCED_PAUSE_FEATURE` hard-requires `NOZZLE_PARK_FEATURE`. `EMERGENCY_PARSER` makes `M112` and `M108` act immediately instead of queueing.

### Preheat presets

PLA 185/45 · ABS 240/70 · **PETG 235/80** · **TPU 225/45**

`PREHEAT_COUNT` derives automatically from the highest `PREHEAT_n_LABEL` defined — up to 10, no separate count to maintain.

## 6. Flash

1. Format a small SD card (8 GB or less) as **FAT32**, **4096-byte** allocation unit.
2. Copy the single `.bin` to the **root**. Nothing else.
3. Power **off**, insert card, power **on**.
4. Wait ~10–30 s. Screen stays blank or shows a bar, then boots.
5. The bootloader renames the file to `.CUR` — that is how you know it flashed.

If nothing happens: filename was reused, card format is wrong, or the card is too large.

## 7. First boot — in this order

```
M502    ; load hardcoded defaults
M500    ; write to EEPROM
```

The EEPROM layout changed, so old stored values are garbage.

**Then, before anything else, check extruder direction.** Heat the hotend to 200 °C, extrude 10 mm slowly, and confirm filament moves *into* the hotend. If it backs out, `INVERT_E0_DIR` is wrong and needs flipping and reflashing.

Then, in order:

```
M48                        ; probe repeatability — confirms the CR Touch is healthy
M106 S255                  ; fan on, as it will be while printing
M303 E0 S220 C8 U1         ; hotend PID autotune, ~10-15 min
M107                       ; fan off
M500
M303 E-1 S60 C8 U1         ; bed PID (optional)
M500
```

`M303` parameters: `E0` = hotend (`E-1` = bed), `S220` = tune at this temperature, `C8` = 8 cycles (min 3, default 5), `U1` = apply the result. **`U1` only writes to RAM** — without `M500` the result is lost on power-off. Tune at the temperature you actually print at, with the part-cooling fan in a representative state.

Finally:

```
M851 Z-…    ; or use the Probe Offset Wizard in the LCD menu
M500
G29         ; fresh bed mesh
M500
```

`BABYSTEP_ZPROBE_OFFSET` is enabled, so live Z adjustments during a print feed straight into the probe offset — double-click the status screen, adjust, then `M500` to keep it.

## 8. Verify the filament sensor

Two separate tests, exercising two different pins.

**Runout (PA4, switch):** with the printer idle, pull filament out of the SFS. `M119` reports the filament sensor triggered.

**Motion (PA7, encoder):** start a print, then pinch the filament so it stops feeding while the extruder keeps pulling. After ~7 mm of commanded extrusion with no encoder movement, `M600` fires and parks the nozzle.

Tune live rather than reflashing:

```
M412 S1      ; enable runout sensor
M412 L10     ; increase JAM threshold if you get false triggers
M412 D10     ; distance to keep printing after a SWITCH runout
M500
```

## Troubleshooting

| Symptom | Likely cause |
|---|---|
| Missing `_Bootscreen.h` / `_Statusscreen.h` | Only the two config files were copied; see step 3 |
| Build errors about missing defines | Marlin and Configurations from different branches |
| Builds the wrong board | `default_envs` is `mega2560`; pass `-e STM32F103RE_creality` |
| `pio` not recognised | Not on PATH — use `%USERPROFILE%\.platformio\penv\Scripts\pio.exe` |
| Blank screen after flash | Wrong env (try `STM32F103RC_creality`), or wrong display define |
| Won't flash at all | SD too large, not FAT32/4096, or filename reused |
| Filament backs out when extruding | `INVERT_E0_DIR` wrong — requires a reflash |
| Motion sensor never triggers | PA7 claimed by the Z endstop — check `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` is off *and* `USE_PROBE_FOR_Z_HOMING` is on |
| Constant false jam triggers | Raise `M412 L<mm>` (not `D`) |
| `G29` skips the left/front edges | Expected — probe can't reach X<32, Y<40 |
| Hotend overshoots or oscillates | PID is the S1 Pro's, not yours — run `M303` |

## Sources

- Marlin firmware — https://github.com/MarlinFirmware/Marlin
- Marlin configurations — https://github.com/MarlinFirmware/Configurations
- `pins_CREALITY_V4.h` — `Marlin/src/pins/stm32f1/pins_CREALITY_V4.h` (SERVO0 PB0, Z_MIN_PROBE PB1, FIL_RUNOUT PA4, Z_STOP PA7)
- Ender-3 S1 Pro example config — source of the Sprite Pro probe offset, E steps, PID and maxtemp
- BigTreeTech SFS V2.0 manual — https://3d.nice-cdn.com/upload/file/SFS_V2.0_User_Manual_20230210.pdf
- M412 Filament Runout — https://marlinfw.org/docs/gcode/M412.html
