# AGENTS.md — SAMD21 HAL

Working notes and conventions for the **SAMD21 HAL** folder (`Marlin/src/HAL/SAMD21/`).

## Build & Test Loop

One real test target exists for this family. Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t SAMD21_minitronics20 -n1 -y   # ReprapWorld Minitronics 2.0, SAMD21J18
```

`SAMD21_minitronics20` is the only SAMD21 env (`ini/samd21.ini`, `buildroot/tests/SAMD21_minitronics20/config-01.ini`, `motherboard = BOARD_MINITRONICS20`). It is the tightest (and only) constraint for SAMD21 — get it green and the family is covered. (The separate `SAMD51_grandcentral_m4` target is the SAMD51 family, not SAMD21 — do not confuse the two.)

The toolchain (`atmelsam` platform + upstream Arduino SAMD framework) is installed in this environment, so the build can actually be run here.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

Unlike AT32, the SAMD21 Arduino core is the **upstream** PlatformIO `atmelsam` platform (`framework-arduino-samd-*`, e.g. `framework-arduino-samd-reprap`/`...-adafruit`) — there is no Marlin-vendored copy under `buildroot/share/PlatformIO/`. Marlin-specific glue lives in:

- `Marlin/src/HAL/SAMD21/` — the HAL itself (serial, SPI, ADC, timers, servo, EEPROM backends, u8g LCD drivers).
- `Marlin/src/pins/samd/` — board pin files (`pins_MINITRONICS20.h`, `pins_BRICOLEMON_V1_0.h`, `pins_BRICOLEMON_LITE_V1_0.h`, `pins_RAMPS_144.h`).
- `buildroot/share/PlatformIO/scripts/SAMD21_minitronics20.py` — per-env extra script (only adjusts `BUILD_FLAGS` serial buffer sizing).

Because the framework is upstream, there is **no mirror-to-installed-package warning** (that applies only to the vendored AT32 core).

### Gotchas that are not obvious from the source

1. **Compiler guard is `__SAMD21__`, set by build flags, not the core.** `HAL.h` / `HAL.cpp` / `HAL_SPI.cpp` / `eeprom_*.cpp` all key on `#ifdef __SAMD21__`, but the upstream Arduino SAMD core defines `ARDUINO_ARCH_SAMD`, not `__SAMD21__`. `ini/samd21.ini` passes `-D__SAMD21__`. **If you add a new SAMD21 board env, you MUST include `-D__SAMD21__` in its `build_flags` or nothing in this HAL compiles.**

2. **Primary serial is native USB-CDC, not a UART.** `HAL.h` defines `DefaultSerial1 = ForwardSerial1Class<SerialUSB>` as `MSerialUSB` and `#define USB_SERIAL_PORT(...) MSerialUSB`. Build flags add `-DUSBCON -DUSBD_USE_CDC` so `SerialUSB` exists. `Serial1`/`Serial2` map to hardware UARTs (`MSerial1`/`MSerial2` via SERCOM). The test config sets `serial_port = -1` for the UART and relies on USB — a board without native USB must re-point the serial port.

3. **Software SPI is a hard `#error`.** `HAL_SPI.cpp` rejects `SOFTWARE_SPI` / `FORCE_SOFT_SPI` ("Software SPI not supported for SAMD21. Use Hardware SPI."). `MarlinSPI` is just `SPIClass` (`MarlinSPI.h`), i.e. the upstream SAMD SERCOM SPI driver. `spiSendBlock()` _claims_ DMA in a comment but actually calls blocking `SPI.transfer()` — fine, just don't expect DMA offload on the SD path.

4. **EEPROM backend is per-board, not auto-selected.** `eeprom/` provides `I2C_EEPROM`, `FLASH_EEPROM_EMULATION`, and `QSPI_EEPROM` (the latter via `eeprom_qspi.cpp`/`QSPIFlash.cpp`), but `USE_WIRED_EEPROM` is an explicit `#error` ("Not implemented on SAMD21"). Each `pins/samd/*.h` must pick one: `BRICOLEMON_*` and `RAMPS_144` default to `I2C_EEPROM`; `MINITRONICS20` leaves `FLASH_EEPROM_EMULATION` commented out (with `MARLIN_EEPROM_SIZE 500U`). `QSPI_EEPROM` is coded but not selected by any current board.

5. **`WDT` macro collides with the CMSIS struct field.** In `HAL.cpp`, `get_reset_source()` wraps its body in `#pragma push_macro("WDT")` / `#undef WDT` / `#pragma pop_macro("WDT")` because `REG_WDT_CTRL` resolves to `.bit.WDT` and the preprocessor wrongly substitutes the `WDT` define. **Do not "fix" this by renaming the field — keep the macro push/pop.**

6. **ADC is free-running INPUTSCAN with remapped sensor order.** `adc_init()` configures the 12-bit ADC as 16-bit averaging (32 samples, DIV2 gain), free-running, scanning 3 contiguous AINs (`HAL_ADC_AIN_NUM_SENSORS = 3`). The SAMD21 ADC only scans contiguous AINs, so `ADC_Handler()` reads `RESULT` by `INPUTOFFSET` and `adc_start()` remaps the pin's AIN back to the array index (`HAL.cpp`). `HAL_ADC_FILTERED 1` means Marlin's own oversampling is disabled — the HAL already filters.

7. **48 MHz Cortex-M0+, no FPU.** SAMD21 tops out at 48 MHz with no hardware float unit; keep math in integer/fixed-point where hot. `freeMemory()` is a `_sbrk`/heap-end sbrk hack (`HAL.cpp`), not a real stack-check.

## Conventions

- All HAL source is guarded by `#ifdef __SAMD21__` — ensure new files and new board envs set `-D__SAMD21__` (see gotcha 1).
- Keep HAL changes in `Marlin/src/HAL/SAMD21/` and board pin changes in `Marlin/src/pins/samd/`. Build glue goes in `buildroot/share/PlatformIO/scripts/` (per-env) or `ini/samd21.ini`.
- Native USB CDC is the intended host link; treat `SerialUSB`/`MSerialUSB` as the primary port unless a board explicitly moves to a UART.

## Related

- `Marlin/src/pins/samd/` — board pin files (no `README.md` in the HAL folder).
- Wiki: [[hal-samd21]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the SAMD21 HAL reuses.
