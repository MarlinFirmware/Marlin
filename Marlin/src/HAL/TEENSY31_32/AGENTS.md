# AGENTS.md — TEENSY31_32 HAL

Working notes and conventions for the **TEENSY31_32 HAL** folder (`Marlin/src/HAL/TEENSY31_32/`). This HAL drives the PJRC Teensy 3.1 / 3.2 boards (NXP/Freescale MK20DX256, ARM Cortex-M4, 72 MHz, 256 KB flash). **Unlike AT32, the Teensy core is NOT vendored** — it is the upstream Teensyduino/Arduino core pulled in by the `teensy` PlatformIO platform.

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t teensy31 -n1 -y     # BOARD_TEENSY31_32, MK20DX256
```

`buildroot/tests/teensy31/` holds three configs (`config-01` default, `config-02` zero-endstops, `config-03` many-features). They all target the same board (`BOARD_TEENSY31_32`); `config-03` is the tightest constraint — the full-feature build that stresses the 256 KB flash. All three must be green before considering Teensy 3.1/3.2 work done.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e teensy31 -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, not vendored)

The Teensy Arduino core is the **upstream** Teensyduino package (`framework-arduinoteensy`, platform `teensy@~4.12.0` in `ini/teensy.ini`), installed at `~/.platformio/packages/framework-arduinoteensy*`. It is NOT vendored under `buildroot/share/PlatformIO/` (that is the AT32 exception). There is no "mirror to the installed package" step — changes to Marlin glue live only in this HAL folder, and the core comes straight from the PlatformIO registry.

- `ini/teensy.ini` `[env:teensy31]` extends `teensy_arm` with `board = teensy31` and adds `src/HAL/TEENSY31_32` to `build_src_filter`. When you add a new HAL source file it is picked up automatically by that filter.
- EEPROM backend: this HAL uses the **wired** EEPROM (`<avr/eeprom.h>`) — the MK20DX256 has 2 KB of real EEPROM. No per-board `SDCARD_EEPROM_EMULATION` / `I2C_EEPROM` / `FLASH_EEPROM` define is needed; `eeprom.cpp` builds under `USE_WIRED_EEPROM`.

### Gotchas that are not obvious from the source

1. **The compiler guard is `__MK20DX256__`, not `ARDUINO_ARCH_TEENSY`.** Every `.cpp` in this folder is wrapped in `#ifdef __MK20DX256__`, and `platforms.h` maps that macro to `HAL/TEENSY31_32`. The Teensy 3.5/3.6 and 4.0/4.1 HALs all share the `teensy_arm` platform/`teensy` toolchain, so a missing guard would compile your file for the wrong MCU. Always keep the `#ifdef __MK20DX256__` guard.

2. **EEPROM is wired `<avr/eeprom.h>`, not SD/flash emulation.** `eeprom.cpp` calls the AVR-style `eeprom_read_byte`/`eeprom_write_byte` against `REAL_EEPROM_ADDR(pos)` and is gated on `USE_WIRED_EEPROM`. Do NOT add `SDCARD_EEPROM_EMULATION` or any SPI/I2C backend — the silicon has real EEPROM.

3. **Bus-clock hard assumption in `timers.cpp`.** `HAL_timer_start()` derives FTM compare values from `FTM0_TIMER_RATE`/`FTM1_TIMER_RATE` presuming a **60 MHz bus clock**. The `teensy31` board JSON sets `F_CPU` accordingly; if that ever changes the step/temp timers run at the wrong rate. Also note `adc_init()` calls `NVIC_ENABLE_IRQ(IRQ_FTM1)` — the temperature timer (FTM1) must be enabled there for the shared temperature ISR to fire.

4. **SPI wraps the Teensyduino `SPI` library, not a custom driver.** `HAL_SPI.cpp` calls `SPI.begin()` / `SPI.transfer()` from upstream `<SPI.h>`; `MarlinSPI` is just `SPIClass`. `spiSendBlock()` still contains dead AVR `SPDR`/`SPSR` register code — that path is disabled; the active path uses `SPI.transfer`. Don't be misled into "fixing" it as a register driver.

5. **Servo `move()` calls `attach(0)` — the `0` is a placeholder, not pin 0.** `Servo.cpp` stores the real pin in `servoPin[servoIndex]` and `libServo::move()` re-attaches with `attach(0)`, which resolves to the stored entry. Do NOT change it to the actual pin number.

6. **ADC reads go through a fixed `pin2sc1a[]` lookup, not `analogRead`.** `adc_start()` indexes a hand-built table (`HAL.cpp`) that maps Marlin pin numbers to MK20DX256 SC1A ADC channels. The `analogInputToDigitalPin` macro in `HAL.h` only covers the first 12 A pins. Route analog sampling through `hal.adc_start()`/`adc_value()`, never `analogRead()`.

## Conventions

- All HAL source is guarded by `#ifdef __MK20DX256__` (plus `IS_TEENSY31_32` / `IS_TEENSY32` in `HAL.h`). The upstream framework also defines `ARDUINO_ARCH_TEENSY`, but the HAL does not rely on it for selection.
- Keep HAL changes in `Marlin/src/HAL/TEENSY31_32/` and board pin changes in `Marlin/src/pins/teensy3/pins_TEENSY31_32.h`. Core/PlatformIO integration is upstream — there is no vendored framework to edit here.
- `fastio.h` header comment says "Teensy 3.5 and 3.6" but is the 3.1/3.2 implementation; it uses Cortex-M bitbanding via the core's `CORE_PINxx_PORTSET`/`PORTCLEAR` symbols. Note `_IS_OUTPUT` is defined identically to `_IS_INPUT` — output direction is not actually distinguished; harmless today, but avoid adding output-direction checks that depend on it.
- Serial: `Serial0` aliases the core `Serial`; `USBSerial` wraps `SerialUSB`; multi-port `DefaultSerialX` objects forward to `SerialX`. `SERIAL_INDEX_MIN/MAX` is 0..3.

## Related

- Wiki: [[hal-teensy31-32]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) this HAL reuses.
- `Marlin/src/pins/teensy3/pins_TEENSY31_32.h` — board pin map.
- `ini/teensy.ini` — `[env:teensy31]` build definition.
