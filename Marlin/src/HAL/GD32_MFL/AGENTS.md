# AGENTS.md — GD32_MFL HAL

Working notes and conventions for the **GD32_MFL HAL** folder (`Marlin/src/HAL/GD32_MFL/`).

This HAL targets GigaDevice GD32F1/GD32F3 MCUs via the third-party **MFL Arduino Core** (`https://github.com/bnmguy/ArduinoCore_MFL`), not the older Maple/STM32duino cores. It currently supports the GD32F303RE (Creality V4.2.x) and GD32F103RC (Aquila v1.0.1) boards.

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t GD32F303RE_creality_mfl -n1 -y   # GD32F303RE, 512 KB flash
buildroot/bin/mftest -t GD32F103RC_aquila_mfl   -n1 -y   # GD32F103RC, 256 KB flash
```

The Aquila (`GD32F103RC`, 256 KB) build is the tighter flash constraint — if it fits there, the Creality (512 KB) build will too. Both use a 0x7000 bootloader offset, so the linked firmware image reserves space below 0x08007000.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, not vendored)

The MFL framework is **upstream**, pulled from a release ZIP (`platform = https://github.com/bmourit/platform-mfl/archive/refs/tags/V1.0.4.zip`) in `ini/gd32.ini`. It is **not** Marlin-vendored like AT32 — there is no `buildroot/share/PlatformIO/framework-arduino-*` to keep in sync, so the AT32 "mirror to installed package" warning does NOT apply. Marlin-specific glue lives entirely in this `HAL/GD32_MFL/` folder and in `ini/gd32.ini` / `buildroot/share/PlatformIO/` scripts (e.g. `offset_and_rename.py`).

Key `gd32_base` settings (`ini/gd32.ini`):

- `board_build.core = gd32`, C++23 (`-std=gnu++23`; `-std=gnu++11/14/17` stripped).
- `-DARDUINO_ARCH_MFL` (the compiler guard macro), `-DPLATFORM_M997_SUPPORT`, `-DTIMER_IRQ_PRIORITY=12`, `-DADC_RESOLUTION=12`, `-DCORE_DEBUG`.
- Per-board: `-DGD32F303RE` / `-DGD32F103RC`, `-DTIMER_TONE=2 -DSS_TIMER=3 -DTIMER_SERVO=4 -DTRANSFER_CLOCK_DIV=8`, `board_build.offset = 0x7000`, `board_upload.offset_address = 0x08007000`, `board_build.rename = firmware-{time}.bin`.

### Gotchas that are not obvious from the source

1. **Pins must be numeric `#define`s, not `enum`s.** `inc/SanityCheck.h` (lines 27–29) hard-errors `#ifndef PA0` with _"Your ARM platform pins are not defined as macros, only as enums!"_. The MFL core's `variant.h` / `pins_arduino.h` (included via `fastio.h` and `HAL.h`) supplies `PA0`, `PC6`, … as macros. **If that error appears, fix it at the pin-define level (add the missing `#define` to the variant), never by deleting the SanityCheck guard.** This is the same class of bug that bit AT32.

2. **0x7000 bootloader offset is mandatory.** Both boards link with `board_build.offset = 0x7000` and upload to `0x08007000`; the `offset_and_rename.py` extra script also renames the artifact to `firmware-{time}.bin`. Removing the offset / the script bricks the image (it would overwrite the bootloader). Keep them.

3. **EEPROM backend is per-board / fallback, not auto-picked like STM32.** `inc/Conditionals_post.h` falls back to `SDCARD_EEPROM_EMULATION` when no real or emulated EEPROM is selected, and maps `I2C_EEPROM`/`SPI_EEPROM` → `USE_SHARED_EEPROM`. `FLASH_EEPROM_LEVELING` is explicitly unsupported (SanityCheck error). The two test boards enable `SDSUPPORT` + `EEPROM_SETTINGS`, so the fallback SD emulation is what is exercised — make sure `SDSUPPORT` is on wherever `SDCARD_EEPROM_EMULATION` is used.

4. **Serial uses hardware USART, not USB-CDC.** `MarlinSerial` wraps the MFL `UsartSerial` (`MarlinSerial.h`); `SERIAL_INDEX_MIN/MAX` = 0..4. `EMERGENCY_PARSER` works via `register_emergency_callback()`. There is no USB-CDC path for Marlin — do not add one. `SERIAL_STATS_MAX_RX_QUEUED` and `SERIAL_STATS_DROPPED_RX` are explicitly unsupported (SanityCheck errors) — don't enable them.

5. **ADC defaults to 16× hardware oversampling.** The MFL framework oversamples by default; for `GD32F303RE` the HAL sets `HAL_ADC_FILTERED`. `HAL_ADC_RESOLUTION` is 12 (from `-DADC_RESOLUTION=12`). Software oversampling in Marlin is disabled to avoid double-counting.

6. **C++23 only.** `gd32_base` forces `-std=gnu++23` and unflags the older standards. Don't reintroduce `gnu++11/14/17` and don't use constructs that break under gnu++23.

7. **Timer assignment is fixed per board.** `TIMER_TONE=2`, `SS_TIMER=3`, `TIMER_SERVO=4`, `TRANSFER_CLOCK_DIV=8`, `TIMER_IRQ_PRIORITY=12` are passed in `ini/gd32.ini`. Changing them shifts which hardware timers are used for toning, stepper ISR, and servo — verify pin/timer conflicts before editing.

## Conventions

- All HAL source is guarded by `#ifdef ARDUINO_ARCH_MFL` (defined in `ini/gd32.ini`, NOT by the core).
- Keep HAL changes in `Marlin/src/HAL/GD32_MFL/` and board pin changes in `Marlin/src/pins/gd32f1/` (Aquila) and `Marlin/src/pins/gd32f3/` (Creality). The Creality/Aquila pins files `#include` the matching STM32F1 Creality pins and add `ALLOW_GD32F3` — they are thin overlays, not full pin lists.
- `MarlinSPI = SPIClass` (from the MFL `<SPI.h>`); no special SPI glue is needed.
- `M997` in-application firmware update is supported (`PLATFORM_M997_SUPPORT`, `flashFirmware()` in `HAL.cpp`).
- Pin mappings reuse the STM32F1 Creality layout; treat them as verified for those boards only and don't "fix" them to match guesses without a source.

## Related

- `README.md` in this folder — overview and supported boards.
- Wiki: [[hal-gd32-mfl]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) this HAL reuses.
- `ini/gd32.ini` — the env definitions and build flags for this family.
