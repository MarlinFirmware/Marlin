# Marlin build — Ender 3 Pro (personal fork)

## Hardware — treat as fixed facts, do not change these defines
- Printer: Creality Ender 3 Pro (stock frame, Bowden, 235x235x250)
- Board: **Creality V4.2.2** → `#define MOTHERBOARD BOARD_CREALITY_V422`
- MCU: STM32F103RET6 (512K) → PlatformIO env `STM32F103RE_creality`
- Display: stock rotary-knob LCD → `#define CR10_STOCKDISPLAY`
- Probe: **CR Touch** on the dedicated 5-pin header (servo PB0 / probe PB1)
- Filament sensor: **BigTreeTech SFS v2.0**, dual mode
  - switch output → J1 (**PA4**)
  - motion/encoder output → Z endstop port (**PA7**)

## Critical invariants — never change without asking me first
- `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` **must stay disabled**. It is what frees
  PA7 for the filament motion sensor. Enabling it breaks the SFS.
- `FIL_MOTION1_PIN` must remain `PA7`.
- **Never** disable or weaken: `THERMAL_PROTECTION_HOTEND`,
  `THERMAL_PROTECTION_BED`, `THERMAL_PROTECTION_CHAMBER`, `USE_WATCHDOG`,
  or raise `HEATER_0_MAXTEMP` / `BED_MAXTEMP`. These are fire-safety features.
  If a build error appears to be fixed by touching one of these, stop and tell me
  instead — that is the wrong fix.
- Do not modify anything under `Marlin/src/` to make a build pass. Configuration
  changes only, unless I explicitly ask for a source patch.

## Build
```
pio run                                    # build default env
pio run -t clean                           # clean
```
Output: `.pio/build/STM32F103RE_creality/firmware-<date>-<time>.bin`
(auto-timestamped — required, the bootloader skips filenames it has seen).

I flash manually via SD card. Never attempt to upload/flash.

## Repo conventions
- This is a fork. `upstream` = MarlinFirmware/Marlin. Working branch:
  `ender3pro-crtouch-sfs`.
- Only `Marlin/Configuration.h` and `Marlin/Configuration_adv.h` should appear
  in my diffs against upstream. Flag anything else.
- Example configs come from the MarlinFirmware/Configurations repo,
  `config/examples/Creality/Ender-3 Pro/CrealityV422/` — **same branch** as this
  fork's upstream base. Mismatched branches cause misleading build errors.

## Working style
- Show me the diff for config changes before applying; I want to read every
  `#define` that moves.
- When a build fails, quote the actual compiler/sanity-check error before
  proposing a fix. Marlin's errors often point somewhere other than the cause.
- Prefer runtime G-code tuning over reflashing where possible
  (`M412 D<mm>` for runout distance, `M851` for probe offset, then `M500`).
