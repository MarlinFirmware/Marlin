# AGENTS.md — DUE HAL

Working notes and conventions for the **DUE HAL** folder (`Marlin/src/HAL/DUE/`). Covers the Arduino Due and compatible boards built on the Atmel/Microchip **SAM3X8E** (ARM Cortex-M3).

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t DUE        -n1 -y   # RAMPS4DUE_EFB, hardware UART (Serial)
buildroot/bin/mftest -t DUE_USB    -n1 -y   # RAMPS4DUE over native USB CDC
buildroot/bin/mftest -t DUE_archim -n1 -y   # UltiMachine Archim 1/2 (SAM3X8E)
```

All three DUE-family envs live in `ini/due.ini` (`env:DUE`, `env:DUE_USB`, `env:DUE_archim`) and map to the PlatformIO `due`, `dueUSB`, and `marlin_archim` board JSONs. `DUE` (config-01 is the heaviest, with ABL/S-Curve/ExtUI/SD) is the broadest smoke test; Archim uses the upstream Arduino SAM variant shipped under `buildroot/share/PlatformIO/variants/` (see `board_build.variants_dir`).

> Verified in this environment (toolchain `toolchain-gccarmnoneeabi` + `framework-arduino-sam` installed): `mftest -t DUE_archim` **builds green** (~8.7 s, ~103 KB / 524 KB flash). `mftest -t DUE` **fails** with a pre-existing `SanityCheck.h` Serial-1 pin-conflict error (`Serial Port 1 pin D18 and/or D19 conflicts…`) in the RAMPS4DUE_EFB test config — not caused by this doc; investigate before relying on the plain `DUE` target.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

Unlike AT32, the DUE build uses the **upstream Arduino SAM core** (`framework-arduino-sam`, pulled by the `atmelsam` PlatformIO platform and `toolchain-gccarmnoneeabi`). It is NOT vendored under `buildroot/share/PlatformIO/`, so there is no installed-package ↔ repo mirror step. Marlin-specific glue lives entirely in this HAL folder, plus:

- `Marlin/src/HAL/DUE/upload_extra_script.py` — Archim-specific upload hook (`env:DUE_archim`).
- `buildroot/share/PlatformIO/variants/` — Archim board variant.

Do not "fix" the SAM core in `~/.platformio/packages/framework-arduino-sam/`; any Marlin change belongs in `Marlin/src/HAL/DUE/` (or `src/pins/...` for boards).

### Gotchas that are not obvious from the source

1. **Native USB is a real CDC stack, not a UART emulation.** When `SERIAL_PORT == -1` (the `DUE_USB` env uses it), Marlin routes serial over the SAM3X native USB (`MarlinSerialUSB`). The USB stack is vendored from **Atmel ASF (2015)** in `usb/` and MUST be initialized — `HAL.cpp` calls `usb_task_init()` in `init()` and `usb_task_idle()` in `idletask()`. If `idletask()` doesn't run, USB never enumerates. See `usb/README.md` for the ASF provenance and upgrade caveats.

2. **USB interrupt race fix lives in `usb/uotghs_device_due.c`.** A fix was applied there to resolve a race that left interrupts asserted when freezing the peripheral clock — causing hangs and watchdog resets from an interrupt storm. Preserve that fix if you touch the USB driver.

3. **EEPROM backend is per-board, not auto-selected.** Marlin does NOT auto-pick an EEPROM backend for DUE. `eeprom/eeprom_flash.cpp` implements `FLASH_EEPROM_EMULATION` (diff-based, wear-leveled over flash page groups — see the header comment) and `eeprom/eeprom_wired.cpp` implements I²C/SPI (`I2C_EEPROM` / `SPI_EEPROM`). Each DUE board's pins file must `#define` the backend it wants; nothing selects one by default.

4. **Watchdog is configured at boot, overriding the Arduino runtime.** `HAL.cpp`'s `watchdogSetup()` runs before `setup()` and either enables the WDT (4 s, or 8 s with `WATCHDOG_DURATION_8S`) or fully disables it. On SAM3X, late watchdog init after hardware reset is unreliable, so it must happen this early. `due.ini` passes `-DWATCHDOG_PIO_RESET` so a WDT fault also resets the PIO controller.

5. **Clock is 84 MHz from a 12 MHz crystal (PLL).** `F_CPU` is 84 MHz; the stepper and temperature timers (`timers.h`, prescaler 2, `TC2`/`TC4`/`TC6` handlers) are derived from it. Don't assume 16 MHz AVR-style timing — PWM frequencies and `analogWrite` scaling differ.

6. **`pin_t` is `int8_t` and ADC is 10-bit / 3.3 V.** `HAL.h` sets `HAL_ADC_VREF_MV 3300`, `HAL_ADC_RESOLUTION 10`, and `analogInputToDigitalPin(p) = p<12 ? p+54 : -1`. DUE analog pins are mapped above the digital range; `M43`/`M42` pin math depends on this.

## Conventions

- All HAL source is guarded by `#ifdef ARDUINO_ARCH_SAM` (defined by the upstream SAM core / `atmelsam` platform — no `ini` flag needed for it).
- Keep HAL changes in `Marlin/src/HAL/DUE/` and board pin changes in `Marlin/src/pins/ramps4due/`, `Marlin/src/pins/archim/`, etc.
- The `usb/` subtree is vendored ASF — treat as fragile; bring Marlin-local fixes forward when comparing against newer ASF.
- Don't pin or vendor the toolchain casually: `due.ini` leaves a commented `platform_packages = toolchain-gccarmnoneeabi@1.120301.0` because the default resolves to an older GCC 7.2.1.

## Related

- `usb/README.md` in this folder — ASF source provenance and USB upgrade notes.
- Wiki: [[hal-due]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the DUE HAL reuses.
