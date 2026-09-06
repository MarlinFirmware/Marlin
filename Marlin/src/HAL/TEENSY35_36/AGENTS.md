# AGENTS.md — TEENSY35_36 HAL

Working notes and conventions for the **TEENSY35_36 HAL** folder (`Marlin/src/HAL/TEENSY35_36/`).

Targets PJRC **Teensy 3.5** (MK64FX512, 512 KB flash) and **Teensy 3.6** (MK66FX1M0, 1 MB flash) — both ARM Cortex-M4. They share this one HAL folder; the physical chip is selected by the compiler macro `__MK64FX512__` / `__MK66FX1M0__` (set by the PlatformIO `teensy35` / `teensy36` board JSONs), not by a runtime check.

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t teensy35 -n1 -y          # Teensy 3.5 (MK64FX512)
buildroot/bin/mftest -t teensy35 -n8 -y          # ...dual-Z endstop stress config
```

All real test targets for this HAL live under `buildroot/tests/teensy35/` (`config-01` … `config-08`, all with `motherboard = BOARD_TEENSY35_36`). There is no `buildroot/tests/teensy36/` folder — the 3.6 build reuses the same HAL and board define, only differing by the PlatformIO `board = teensy36` (which flips `__MK66FX1M0__` on). To exercise the 3.6-only path, build env `teensy36` (`pio run -e teensy36`); otherwise the `teensy35` targets cover the shared code.

The tighter constraint is the **Teensy 3.5** (512 KB flash). If it fits there, the 3.6 (1 MB) will too.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

The Teensy Arduino core is the **upstream Teensyduino** package (`platform = teensy@~4.12.0` in `ini/teensy.ini`), provided by PlatformIO — it is NOT vendored under `buildroot/share/PlatformIO/` like the AT32 framework is. The `teensy_arm` env pulls in Teensyduino's `cores/teensy3/` (register headers like `kinetis.h`, `pins_teensy.h`, and the native `Serial1…`, `SerialUSB`, `SPI`, `Wire`, and `avr/eeprom.h` emulation). Marlin glue lives entirely in this HAL folder plus `Marlin/src/pins/teensy3/pins_TEENSY35_36.h`.

There is no installed-package-vs-repo mirror step (that warning applies only to the vendored AT32 framework). Build changes are just source edits under `Marlin/src/HAL/TEENSY35_36/` and `buildroot/` scripts.

### Gotchas that are not obvious from the source

1. **Chip is chosen by a compile-time macro, not runtime.** `HAL.h` does `#ifdef __MK66FX1M0__ … IS_TEENSY36 … #else IS_TEENSY35`, and every `.cpp` in the folder is wrapped in `#if defined(__MK64FX512__) || defined(__MK66FX1M0__)`. The 3.5 vs 3.6 distinction comes purely from the `board = teensy35` / `board = teensy36` PlatformIO JSON. Don't add runtime branching for the chip.

2. **No real EEPROM — the core emulates it in flash.** `eeprom.cpp` uses `USE_WIRED_EEPROM` and `#include <avr/eeprom.h>`; Teensyduino supplies an _emulated_ EEPROM (backed by program flash) since the Kinetis parts have no physical EEPROM. `MARLIN_EEPROM_SIZE` defaults to `E2END + 1`. The HAL adds `delay(2)` / `safe_delay(2)` between byte writes specifically to avoid tripping the watchdog during the slow flash-write cycles — keep that; it is load-bearing.

3. **Raw Kinetis register names come from the Teensyduino core.** `HAL.cpp` references `RCM_SRS0`, `WDOG_TOVALH`, `WDOG_STCTRLH`, `WDOG_REFRESH`, `ADC0_SC3`, `ADC1_SC3`, `IRQ_FTM1`, `NVIC_ENABLE_IRQ`, and `CORE_PIN…` (`fastio.h`) — all defined by Teensyduino's `cores/teensy3/`, not by Marlin. Do not redefine them; if a build can't find `RCM_SRS0` etc., the wrong core is selected, not a missing Marlin header.

4. **Dual-ADC with a physical-pin→SC1A lookup table.** `adc_start()` indexes a `pin2sc1a[]` array (in `HAL.cpp`) by _physical pin number_; digital-only pins map to `255` and yield `adc_select = -1`. Pin→ADC-channel wiring is fixed by the Teensy 3.5/3.6 silicon — match it against the Teensyduino `pins_teensy.h` numbering if it ever changes.

5. **Watchdog is the Kinetis `WDOG`, refreshed with a two-write sequence.** `watchdog_refresh()` writes `0xA602` then `0xB480` to `WDOG_REFRESH` (hardware-enforced order). The Teensy 3.x has no separate cortex `WDT`; don't port the STM32-style watchdog code here.

6. **Serial objects are native — no custom `Serial` global needed.** Unlike the AT32 HAL, Teensyduino already provides `Serial1`…`Serial3`, `SerialUSB`, and a bare `Serial`. `HAL.h`/`HAL.cpp` only forward-declare `MSerialN` via `ForwardSerial1Class<decltype(SerialN)>` and `USBSerial` (wrapping `SerialUSB`), with `SERIAL_INDEX_MIN 0` / `SERIAL_INDEX_MAX 3`. No `HardwareSerial& Serial = Serial1;` alias hack is required.

7. **SPI wraps the native `SPI` (SPIClass).** `MarlinSPI = SPIClass` (in `MarlinSPI.h`); `HAL_SPI.cpp` drives the Teensyduino `<SPI.h>` `SPI` object via `SPISettings`/`SPI.transfer`. `spiBegin()` configures the SD pins as GPIO. Note `spiSendBlock()` still contains leftover direct `SPDR` writes mixed with `SPI.transfer()` — functional but don't copy the pattern elsewhere.

8. **`reboot()` calls `_reboot_Teensyduino_()`** — a core-provided reset, not a hand-rolled `SCB->AIRCR` jump. Keep using the core helper.

## Conventions

- All HAL source is guarded by `#if defined(__MK64FX512__) || defined(__MK66FX1M0__)` (the chip macro, set by the PlatformIO board JSON — there is no `ARDUINO_ARCH_*` guard for this family).
- Keep HAL changes in `Marlin/src/HAL/TEENSY35_36/` and board pin changes in `Marlin/src/pins/teensy3/pins_TEENSY35_36.h`. Board selection (`BOARD_TEENSY35_36`) lives in `Marlin/src/core/boards.h`.
- The HAL folder has no `README.md` and no vendored framework — framework/platform integration changes are upstream Teensyduino behavior, not something patched in `buildroot/share/PlatformIO/`.

## Related

- Wiki: [[hal-teensy35-36]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`serial_ports.h`, `HAL_SPI.h`, `eeprom_api.h`) this HAL reuses.
- `Marlin/src/HAL/TEENSY31_32/` and `Marlin/src/HAL/TEENSY40_41/` — sibling Teensy HALs (different silicon, different macros).
