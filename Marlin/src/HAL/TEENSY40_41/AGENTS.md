# AGENTS.md — TEENSY40_41 HAL

Working notes and conventions for the **TEENSY40_41 HAL** folder (`Marlin/src/HAL/TEENSY40_41/`).

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably). The build shortcut for these boards is `teensy41` / `t41` (and `teensy40` / `t40` for the bare 4.0 env). Only the **Teensy 4.1** board has committed test fixtures, so that is the primary green target:

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t teensy41 -n1 -y          # config-01 : default BOARD_TEENSY41
buildroot/bin/mftest -t teensy41 -n8 -y          # config-08 : dual Z + dual Z endstops
```

The 8 fixtures live in `buildroot/tests/teensy41/config-0[1-8].ini`, all `motherboard = BOARD_TEENSY41`. Run the whole set (or at least config-01 plus a feature-heavy one like config-08) before considering TEENSY40_41 work done.

To compile for the **Teensy 4.0** board specifically (no committed fixtures), build the env directly from a `MOTHERBOARD`:

```bash
buildroot/bin/mftest -t teensy40 -b            # -b builds the auto-detected env
# or: pio run -e teensy40
```

Both `teensy40` and `teensy41` envs pull in `src/HAL/TEENSY40_41/` via `ini/teensy.ini`; the only difference is `-DIS_TEENSY40` vs `-DIS_TEENSY41`.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

Unlike the AT32 / HC32 HALs, the Teensy Arduino core is **NOT vendored** in the Marlin repo. The build uses the upstream **Teensyduino** core shipped by PlatformIO's `teensy` platform (`platform = teensy@~4.12.0` in `ini/teensy.ini`). The entire HAL source is guarded by `#ifdef __IMXRT1062__` (the chip macro the upstream core defines), not by a Marlin-injected `ARDUINO_ARCH_*` define.

Practical consequences:

- There is **no vendored framework to keep in sync**. Framework/platform fixes are not made in `buildroot/share/PlatformIO/` for this HAL — they belong upstream in Teensyduino. If a Hal-level bug is really a core bug, document it and file it upstream; do not vendor a copy.
- The Teensy core provides `Wire.h`, `SPI.h`, `avr/eeprom.h` (EEPROM emulation), `Serial`, `Serial1`–`Serial8`, and `SerialUSB`. Marlin's `HAL.cpp` wires these into its `MSerialX` / `USBSerial` forwarders (see `HAL.h` serial-port block).
- Build flags come entirely from `ini/teensy.ini` + the HAL's `extra_flags`. There is no Marlin-side `platformio-build-*.py` to patch for size/optimization — the Teensy platform already builds `-O2` correctly.

### Gotchas that are not obvious from the source

1. **`__IMXRT1062__` is the real guard.** `HAL.h`/`HAL.cpp`/`eeprom.cpp` all guard on `__IMXRT1062__`, not `IS_TEENSY40_41`. `IS_TEENSY40_41` / `IS_TEENSY41` / `IS_TEENSY40` are _derived_ macros used for behavior selection inside the HAL (`HAL.h` sets `IS_TEENSY41` unless `IS_TEENSY40` is defined). Don't add `ARDUINO_ARCH_TEENSY` guards — they don't exist here.

2. **TFT displays are unsupported.** `inc/SanityCheck.h` hard-errors on `HAS_SPI_TFT || HAS_FSMC_TFT` ("TFT displays are not available for Teensy 4.0/4.1"). Don't try to enable an SPI/FSMC TFT on this HAL.

3. **`EMERGENCY_PARSER` is not implemented.** SanityCheck errors out if `EMERGENCY_PARSER` is enabled. Hosts must rely on the normal command queue, not the emergency-parser fast path.

4. **TMC220x software serial is unsupported.** `HAS_TMC_SW_SERIAL` errors out. TMC drivers must use hardware UART or SPI (the `MarlinSPI.h` / `HAL_SPI.cpp` path).

5. **`POSTMORTEM_DEBUGGING` and `SERIAL_STATS_*` are unsupported.** Both error out in SanityCheck. Don't enable them for Teensy 4.x boards.

6. **EEPROM uses the Teensyduino emulated EEPROM, not SD-card emulation.** `eeprom.cpp` is guarded by `USE_WIRED_EEPROM` and includes the upstream `<avr/eeprom.h>` (Teensyduino's flash-backed emulation). It does **not** use `SDCARD_EEPROM_EMULATION`; per-board `pins_teensy*` files do not need to define an SD-backed store. `MARLIN_EEPROM_SIZE` defaults to `E2END + 1` from the core.

7. **`PSTR` is redefined to defeat PROGMEM.** `HAL.h` does `#undef PSTR` / `#define PSTR(str) ({static const char *data = (str); &data[0];})` because the IMXRT1062 has linear address space and the AVR `PSTR`/progmem model does not apply. String literals in this HAL behave like normal RAM pointers — do not assume `__flash`/`PROGMEM` semantics.

8. **`sq()` is redefined.** `HAL.h` `#undef sq` / `#define sq(x) ((x)*(x))` to match the Teensy core's math macros and avoid collisions with the Arduino `sq`.

9. **ADC pins map to a different index range.** `analogInputToDigitalPin(p)` returns `p + 54` for `p < 12` (Teensy's analog pins are numbered 54+), else `-1`. Pin numbers here are the Teensy digital pin numbers, not the AVR-style `A0`→`0` scheme.

10. **FastIO goes through `digitalRead`/`digitalWrite`, not direct-port manipulation.** `fastio.h` implements `READ`/`WRITE`/`SET_*` as calls to the Teensy GPIO functions (no AVR-style port registers). `is_output(pin)` is a real helper in `HAL.cpp` used by `_GET_MODE`/`IS_INPUT`/`IS_OUTPUT`. PWM capability is `digitalPinHasPWM(P)`.

11. **PWM frequency is set per-pin via the Teensy pulse API.** `set_pwm_frequency()` wraps `analogWriteFrequency()`; `set_pwm_duty()` wraps `analogWrite()`. The SanityCheck emits a `#warning` if `FAST_PWM_FAN` is left at the 1000 Hz default (suggesting a higher, quieter frequency) and another `#warning` that `SPINDLE_LASER_USE_PWM` is untested. See https://www.pjrc.com/teensy/td_pulse.html.

12. **Serial port indices are 0–8, with USB and Ethernet aliases.** `HAL.h` defines `SERIAL_INDEX_MIN 0` / `SERIAL_INDEX_MAX 8`, `Serial0` = `Serial`, `USBSerial` (forwarding `SerialUSB`), and `ETH_SERIAL_PORT` = `ethernet.telnetClient`. `BAUD_RATE_GCODE` is rejected when `SERIAL_PORT_2 == -2` (the Ethernet serial port).

13. **No `FRAME` / variant linker script surgery needed.** The Teensy platform supplies the correct flash/RAM linker script for both 4.0 (1 MB flash / 512 KB RAM) and 4.1 (1 MB / 8 MB PSRAM variants). There is no per-board linker-script assignment to maintain in the Marlin repo.

## Conventions

- All HAL source is guarded by `#ifdef __IMXRT1062__`. Select 4.0-vs-4.1 behavior with `IS_TEENSY40` / `IS_TEENSY41` (set from `extra_flags` in `ini/teensy.ini`).
- Keep HAL implementation in `Marlin/src/HAL/TEENSY40_41/` and board pin files in `Marlin/src/pins/teensy40_41/` (and any shared `pins_teensy*` helpers).
- Because the Arduino core is upstream (not vendored), do NOT mirror framework changes into `buildroot/share/PlatformIO/` for this HAL — there is nothing to mirror.
- Don't paper over SanityCheck errors with config edits; the errors document real unsupported features (TFT, emergency parser, TMC SW serial, postmortem, serial stats).

## Related

- `ini/teensy.ini` — env definitions for `teensy31` / `teensy35` / `teensy36` / `teensy40` / `teensy41`.
- `buildroot/tests/teensy41/*.ini` — the committed test fixtures for this HAL.
- Wiki: [[hal-teensy40-41]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers, serial ports) the TEENSY40_41 HAL reuses.
