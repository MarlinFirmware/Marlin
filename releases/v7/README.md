# Marlin bugfix-2.1.x for Artillery Genius Pro — v7

Custom Marlin firmware for the **Artillery Genius Pro** (STM32F401RCT6, BOARD_ARTILLERY_RUBY) based on Marlin bugfix-2.1.x.

Built and tested on a physical Genius Pro unit. All configurations are hardware-verified.

---

## Flash the pre-built binary

If you just want to update your printer, use the pre-built binary — no compilation required.

**File:** `firmware-gpro-v7-0x08000000.bin`  
**Flash address:** `0x08000000`

### How to flash

1. Copy `firmware-gpro-v7-0x08000000.bin` to the root of a microSD card (FAT32, ≤32 GB)
2. Rename it to `firmware.bin` (some boards require this name)
3. Power off the printer, insert the SD card into the **mainboard SD slot** (not the TFT slot)
4. Power on — the LED on the board will blink while flashing (~10 seconds)
5. Power cycle once flashing is complete
6. Run `M502` followed by `M500` to reset and save EEPROM defaults
7. Re-run your bed leveling (`G29`) and save (`M500`)

> **Important:** after flashing, always do `M502` + `M500` before printing. Skipping this can cause unexpected behavior if old EEPROM data is incompatible.

---

## What's different from stock Marlin

See [FEATURE_COMPARISON.md](FEATURE_COMPARISON.md) for a full side-by-side table against stock Marlin, the gpro-mp reference, and the Marlin-for-artillery-genius-pro reference.

**Summary of key additions over stock:**

- **BLTouch** with correct dual-pin wiring for the Ruby board (Z_MIN_PROBE = PC2, Z_MIN = PA0)
- **Unified Bed Leveling (UBL)** with 3-point probing, Hilbert curve scan, and G26 mesh test
- **Input Shaping** (X + Y) with live tuning menu — reduces ringing/ghosting
- **Linear Advance** (K = 0.13 starting point for direct drive) — reduces bulging at corners
- **Full BTT TFT touchscreen support** — host action commands, prompts, status notifications, auto-report temperatures, auto-report position, M73 progress bar
- **Power loss recovery** — resume after power failure
- **M92** — set steps-per-unit at runtime
- **M211** — toggle software endstops at runtime
- **M600** — filament change mid-print
- **M486** — cancel individual objects mid-print
- **M43** — pin debug and toggle (diagnostic tool)
- **Arc support** (G2/G3) — smooth curves without extra host processing
- **Hardware watchdog** — resets printer if firmware hangs
- **Assisted tramming** (G35) — guided manual bed screw leveling
- **Segment-leveled moves** — UBL mesh compensation applied every 5 mm on long moves
- Hardware-tuned PID values for the Genius Pro hotend and bed

---

## Build from source

### Prerequisites

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- Python 3.x

### Steps

```bash
git clone https://github.com/persano/Marlin.git
cd Marlin
git checkout artillery-genius-pro
python -m platformio run -e Artillery_Ruby
```

The compiled binary will be at `.pio/build/Artillery_Ruby/firmware.bin`.

### Flash address

This board uses the standard STM32F401 layout:

| Region | Address | Size |
|--------|---------|------|
| Bootloader | `0x08000000` | 16 KB |
| EEPROM emulation | `0x08004000` | 16 KB |
| Firmware | `0x08008000` | 224 KB |

The binary produced by PlatformIO starts at `0x08000000` and includes the bootloader header.

---

## Post-flash calibration

1. `M502` — reset to firmware defaults
2. `M500` — save to EEPROM
3. Home all axes: `G28`
4. Run UBL mesh: `G29 P1` then `G29 P3` then `G29 S1` (save mesh to slot 1)
5. Enable leveling: `M420 S1`
6. Save: `M500`
7. Calibrate Linear Advance K-factor using the [Marlin K-factor calibration pattern](https://marlinfw.org/tools/lin_advance/k-factor.html)
8. Calibrate Input Shaping frequency with `M593` (use a ringing test print)

---

## Hardware

| Component | Part |
|-----------|------|
| MCU | STM32F401RCT6 @ 84 MHz |
| Board | Artillery Ruby |
| Extruder | Direct drive |
| Probe | BLTouch |
| Display | BTT TFT35 (or compatible) |

---

## Files in this release

| File | Description |
|------|-------------|
| `firmware-gpro-v7-0x08000000.bin` | Pre-built binary, flash at 0x08000000 |
| `FEATURE_COMPARISON.md` | Full feature table vs stock / gpro-mp / mfagp references |
| `MERGE_REPORT.md` | Detailed audit log of every configuration decision |
| `README.md` | This file |

---

## License

Marlin firmware is licensed under [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html). Configuration files and documentation in this branch follow the same license.
