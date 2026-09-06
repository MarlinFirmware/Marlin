# AGENTS.md — AVR HAL

Working notes and conventions for the **AVR HAL** folder (`Marlin/src/HAL/AVR/`). This HAL covers the legacy 8-bit AVR microcontrollers (ATmega1280/2560/644/1284, AT90USB family) used on classic boards (Arduino Mega, RAMBo, Sanguinololu, Melzi, MightyBoard). It is the tightest resource-constrained target in Marlin: 8-bit, no FPU, tiny RAM/flash.

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t mega2560 -n1 -y        # ATmega2560, 256 KB flash / 8 KB RAM — tightest constraint
buildroot/bin/mftest -t mega2560ext -n1 -y     # ATmega2560, extended pins 70-85
buildroot/bin/mftest -t mega1280 -n1 -y        # ATmega1280
buildroot/bin/mftest -t rambo -n1 -y           # RepRap RAMBo (ATmega2560)
buildroot/bin/mftest -t sanguino644p -n1 -y    # ATmega644p
buildroot/bin/mftest -t sanguino1284p -n1 -y   # ATmega1284p
buildroot/bin/mftest -t melzi_optiboot -n1 -y  # Melzi (ATmega1284p, optiboot)
```

All targets build from `ini/avr.ini` (`env:` sections). The ATmega2560 (8 KB RAM, 256 KB flash) is the tightest constraint — if it fits there, the 1284p/rambo builds will too, but watch **RAM**, not just flash. `melzi_optiboot` / `sanguino1284p_optimized` add size-saving flags (`-fno-split-wide-types -mcall-prologues`) because the 1284p is flash- starved.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

Unlike AT32, the AVR Arduino core is the **upstream** PlatformIO `atmelavr@~4.0.1` platform and the stock Arduino AVR core — it is NOT vendored under `buildroot/share/PlatformIO/`. There is no "mirror to installed package" requirement. Marlin-specific glue lives only in this HAL folder and in `buildroot/share/PlatformIO/` scripts (e.g. `generic_create_variant.py` for the `MARLIN_MEGA_EXTENDED` / `MARLIN_MEGA1281` variants). Most build flags are set in `ini/avr.ini`, not a framework build script.

### Gotchas that are not obvious from the source

1. **Harvard architecture — `PROGMEM` is mandatory for read-only data.** AVR has separate flash and RAM; any string or table left in `.data`/`.bss` silently eats the ~8 KB RAM. `HAL.h` defines `PGMSTR(NAM,STR)` and `pgm_read_ptr` shims; `pinsDebug.h`, `pinsDebug_plus_70.h`, and `u8g/u8g_com_HAL_AVR_sw_spi.cpp` all read via `pgm_read_byte` / `pgm_read_word`. **Never put a lookup table or string literal in plain RAM on AVR — mark it `PROGMEM` and read it back with the `pgm_read_*` macros.**

2. **No FPU — float math is emulated in software and is slow + large.** AVR has no native float unit. `HAL/AVR/math.h` provides hand-written assembly fixed-point helpers (`MultiU24X32toH16`, `MultiU8X8toH8`) used in the stepper hot path. **Avoid `float`/ `double` in ISRs and step-timing code; prefer the fixed-point helpers or integer math, or the build will bloat and stutter.**

3. **`MARLIN_EEPROM_SIZE` comes from `E2END`.** `eeprom.cpp` derives `MARLIN_EEPROM_SIZE` as `E2END + 1` (the framework defines `E2END` per-MCU — 4095 on ATmega2560). There is **no external EEPROM backend** (no `I2C_EEPROM`/`SPI_EEPROM`/ `SDCARD_EEPROM_EMULATION`) auto-selected; AVR uses the MCU's internal EEPROM only. Writes are byte-diffed to limit wear, with `delay(2)` to dodge the watchdog.

4. **`SERIAL_PORT` (and `_2`/`_3`) must be 0–3, and USB-CDC is not the default.** AVR Marlin uses hardware UART, not USB-CDC, except on the AT90USB family (native USB; `USBCON` defined → `HardwareSerial`, and `BOARD_NO_NATIVE_USB` is NOT defined). For non-USBCON parts `HAL.h` pulls in `MarlinSerial.h` and enforces `WITHIN(SERIAL_PORT, 0, 3)` with `#error`. **Don't raise `SERIAL_PORT` past 3.**

5. **Optimization flags live in `ini/avr.ini`, not a framework script.** Unlike AT32, there is no vendored `platformio-build-arduino.py`. Size-saving flags are in the env definitions: `common_avr8` adds `-Wl,--relax`; `tuned_1284p` / `mega_extended_optimized` add `-fno-tree-scev-cprop -fno-split-wide-types -mcall-prologues`. Flash overflow on a 1284p board means reaching for those flags, not editing source.

6. **`freeMemory()` is a real symbol, not a macro.** `HAL.h` declares `extern "C" int freeMemory();` (defined in `Marlin/src/HAL/shared/`); the AVR build's SRAM pressure makes it the go-to diagnostic. The watchdog is a no-op unless `USE_WATCHDOG` is set (`watchdog_init`/`watchdog_refresh` are `IF_DISABLED(USE_WATCHDOG, {})`), and reset source is cleared via `MCUSR = 0`.

## Conventions

- The compiler guard macro is `__AVR__` (defined by the compiler, NOT `ARDUINO_ARCH_AVR` — `HAL/platforms.h` keys on `__AVR__`). Do not add `ARDUINO_ARCH_AVR` guards.
- Keep HAL changes in `Marlin/src/HAL/AVR/`; per-board pin maps live in `Marlin/src/pins/rambo/`, `Marlin/src/pins/mega/`, `Marlin/src/pins/sanguino/`, `Marlin/src/pins/melzi/`, etc. Build/variant glue goes in `buildroot/share/PlatformIO/` and `ini/avr.ini`.
- Fast I/O macros are in `fastio.h` with per-MCU `fastio/fastio_*.h` includes (1280, 1281, 168, 644, AT90USB). Servo timing lives in `ServoTimers.h`.
- Treat RAM as the scarce resource: prefer `PROGMEM`, fixed-point math, and the tuned build flags on 1284p-class targets.

## Related

- Wiki: [[hal-avr]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs the AVR HAL reuses (`eeprom_api`, `HAL_SPI`, serial hook).
- `ini/avr.ini` — all AVR env definitions and size-saving flags.
