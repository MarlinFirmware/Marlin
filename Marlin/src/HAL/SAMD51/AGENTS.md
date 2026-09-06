# AGENTS.md — SAMD51 HAL

Working notes and conventions for the **SAMD51 HAL** folder (`Marlin/src/HAL/SAMD51/`).

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t SAMD51_grandcentral_m4 -n1 -y
```

There is exactly one SAMD51 test target: `SAMD51_grandcentral_m4`, which builds the Adafruit Grand Central M4 (`board = adafruit_grandcentral_m4`, `platform = atmelsam`) against `buildroot/tests/SAMD51_grandcentral_m4/config-01.ini` (board `BOARD_AGCM4_RAMPS_144`, TMC2209 + BLTouch + SD + EEPROM features). It must be green before considering SAMD51 work done. The Grand Central M4 has 1 MB flash / 256 KB RAM — not a tight flash constraint, but the build is the only real SAMD51 gate.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — it is unsafe and guarded. Use `pio run -e SAMD51_grandcentral_m4 -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

SAMD51 uses the **upstream Arduino SAMD core** (`atmelsam` platform + `framework-arduino-samd-*`), not a Marlin-vendored core like AT32. Do NOT mirror framework changes to a repo path — there is no `buildroot/share/PlatformIO/framework-*` for SAMD51; Marlin-specific glue lives only in `Marlin/src/HAL/SAMD51/` and the per-env build script.

- The `env:SAMD51_grandcentral_m4` section lives in `ini/samd51.ini`. It pulls in `Adafruit TinyUSB Library` and `SoftwareSerialM` as `lib_deps`, and adds `-DUSE_TINYUSB`.
- Marlin-specific build glue is in `buildroot/share/PlatformIO/scripts/SAMD51_grandcentral_m4.py`: it sets `-DSERIAL_BUFFER_SIZE` to `max(RX,TX,350)` from the resolved config. If you change how serial buffers are sized, update this script.
- EEPROM backend is **per-board, not auto-selected**. The HAL ships three backends that compile only when explicitly enabled: `eeprom/eeprom_wired.cpp` (`USE_WIRED_EEPROM`, needs `MARLIN_EEPROM_SIZE`), `eeprom/eeprom_qspi.cpp` (`QSPI_EEPROM`, via `QSPIFlash`), and `eeprom/eeprom_flash.cpp` (`FLASH_EEPROM_EMULATION`, SAMD51 NVM smart-EEPROM). Each SAMD51 board's pins file must `#define` the one it uses (`pins_RAMPS_144.h` / `pins_BRICOLEMON*.h` set these). Marlin does NOT auto-pick one.

### Gotchas that are not obvious from the source

1. **Native USB via TinyUSB, not Serial.** The Grand Central M4 exposes native USB CDC through Adafruit TinyUSB (`-DUSE_TINYUSB`). The USB serial is `Serial` (a `Serial_`, i.e. USB CDC), while hardware UARTs are `Serial1`/`Serial2`/… Marlin's port mapping in `MarlinSerial_AGCM4.h` aliases `MSerial0` → `Serial` (USB) and `MSerial1..4` → `Serial1..4`. `USB_SERIAL_PORT(...)` is `MSerial0`. If a board has no USB-CDC `Serial`, `USING_HW_SERIALUSB` is false and `MSerial0` is not defined — guard any USB-only code behind that macro (see `HAL.cpp:33`).

2. **Extra UARTs are defined by the HAL, not the framework.** The Adafruit core leaves `Serial2`–`Serial4` undefined to save SERCOM resources. `MarlinSerial_AGCM4.cpp` instantiates them on specific SERCOMs (`Serial2`→SERCOM4, `Serial3`→SERCOM1, `Serial4`→SERCOM5) and provides the matching `SERCOMx_y_Handler()` ISRs. If you add a hardware serial port, you must wire both the `UartT` instance and its four SERCOM interrupt handlers here, or it will silently not receive data.

3. **Pin representation is an integer Arduino index, not an enum.** `pin_t` is `int8_t` and `GET_PIN_MAP_PIN(index)`/`GET_PIN_MAP_INDEX(pin)` are identity, so `PIN_EXISTS(P)` works in the preprocessor (no AT32-style enum-pin trap). The SAMD port/pin is derived later via `SAMD51.h` macros (`PIN_TO_SAMD_PIN` → `DIO##P##_PIN` from the framework's `pins_arduino.h`). Don't reintroduce an enum-based pin scheme.

4. **ADC uses both ADC0 and ADC1 behind a DMA scan.** `HAL.cpp` builds an `ADCIndex` enum and a `PIN_TO_INPUTCTRL` list split into ADC0-then-ADC1 inputs, then drives conversion via `Adafruit_ZeroDMA` (`DMA_IS_REQUIRED` whenever any ADC is used). The `GET_TEMP_n_ADC()` / `GET_*_ADC()` macros must resolve to ADC unit 0 or 1; a pin mapped to the wrong ADC unit silently won't be sampled. Watchdog refresh must happen during long operations so the 4–8 s WDT (`WDT_CONFIG_PER_CYC*`) doesn't trip.

5. **Software SPI is unsupported.** `HAL_SPI.cpp` `#error`s on `SOFTWARE_SPI` / `FORCE_SOFT_SPI` — SAMD51 only does hardware SPI. SD SPI uses `SDCARD_SPI` when the SD is `ONBOARD` (QSPI), else the global `SPI`. `spiSendBlock()` uses DMA transfer. `MarlinSPI` is just `SPIClass` (`MarlinSPI.h`).

6. **SERCOM/SD/QSPI sharing.** The Grand Central M4 routes the SD card over QSPI (`sdcard_connection = LCD` maps to the onboard SPI/QSPI flash in the test config). Repurposing SERCOM or QSPI pins in a board's pins file can collide with the SD/QSPI backend — the board comments in `pins/samd/` warn to "be careful with the Sercom configurations" when enabling hardware serials.

## Conventions

- All HAL source is guarded by `#ifdef __SAMD51__` (the compiler macro for the SAM D51 family; `HAL/platforms.h` maps `__SAMD51__` → `HAL/SAMD51/`). Board-variant glue is further guarded by `#ifdef ADAFRUIT_GRAND_CENTRAL_M4`.
- Keep HAL changes in `Marlin/src/HAL/SAMD51/` and board pin changes in `Marlin/src/pins/samd/` (e.g. `pins_RAMPS_144.h`, `pins_BRICOLEMON*.h`). Build/env glue goes in `buildroot/share/PlatformIO/scripts/SAMD51_grandcentral_m4.py` and `ini/samd51.ini`.
- Because the framework is upstream, HAL fixes must be portable — don't assume a Marlin-patched core. Anything that needs a core change has to go through Adafruit's `framework-arduino-samd-adafruit` upstream.

## Related

- `README.md` — none in this folder yet.
- Wiki: [[hal-samd51]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the SAMD51 HAL reuses.
- `Marlin/src/pins/samd/` — SAMD51 board pin definitions.
