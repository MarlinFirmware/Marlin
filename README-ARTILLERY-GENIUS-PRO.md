# Marlin bugfix-2.1.x — Artillery Genius Pro

This branch (`artillery-genius-pro`) contains a fully configured and hardware-verified Marlin build for the **Artillery Genius Pro** 3D printer.

- **Board:** Artillery Ruby (STM32F401RCT6)
- **Base:** Marlin bugfix-2.1.x
- **Probe:** BLTouch (dual-pin wiring)
- **Leveling:** Unified Bed Leveling (UBL)

## Quick start — flash the pre-built binary

Go to [`releases/v7/`](releases/v7/) for the pre-built `.bin` file, flashing instructions, and full feature documentation.

## Key features over stock Marlin

- BLTouch with correct Ruby board wiring
- Input Shaping (X + Y) with LCD tuning menu
- Linear Advance (K = 0.13 for direct drive)
- Full BTT TFT touchscreen support (host actions, auto-reports, M73 progress)
- M92, M211, M600, M486, M43, G35 and more
- Power loss recovery
- Hardware watchdog
- Hardware-tuned PID values

See [`releases/v7/FEATURE_COMPARISON.md`](releases/v7/FEATURE_COMPARISON.md) for the full comparison against stock and other Artillery reference firmwares.

## Build

```bash
git checkout artillery-genius-pro
python -m platformio run -e Artillery_Ruby
```

## Configuration files

- `Marlin/Configuration.h`
- `Marlin/Configuration_adv.h`
