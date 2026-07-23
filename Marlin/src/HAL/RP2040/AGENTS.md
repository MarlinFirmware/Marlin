# AGENTS.md — RP2040 HAL

Working notes and conventions for the **RP2040 HAL** folder (`Marlin/src/HAL/RP2040/`).

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t SKR_Pico -n1 -y          # BTT SKR Pico, BOARD_BTT_SKR_PICO
```

`SKR_Pico` is the only RP2040 test folder under `buildroot/tests/` and is the canonical RP2040 target. The `ini/raspberrypi.ini` file also defines the `RP2040` and `RP2040-alt` environments (board `pico`), but they have no dedicated test folder, so `SKR_Pico` is what CI and maintainers build for this HAL.

> Build not verified in this environment — the Arduino-Pico / Raspberry Pi Pico SDK toolchain is not installed here. Run `mftest -t SKR_Pico -n1 -y` locally to confirm green. Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e SKR_Pico -t clean` (build artifacts only) or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, not vendored)

The RP2040 Arduino core is **upstream**, NOT Marlin-vendored. It is the [Arduino-Pico core](https://arduino-pico.readthedocs.io/) (earlephilhower), which wraps the Raspberry Pi **Pico SDK** (`pico-sdk`) and TinyUSB. `ini/raspberrypi.ini` selects it:

- `env:RP2040` → `platform = raspberrypi`, `framework = arduino` (default `earlephilhower` core, board `pico`).
- `env:RP2040-alt` → `platform = https://github.com/maxgerhardt/platform-raspberrypi.git` with `board_build.core = earlephilhower`.

Because the framework is upstream, there is **no mirror-to-installed-package step** like AT32 requires. Marlin-specific glue lives entirely in `Marlin/src/HAL/RP2040/` (plus `buildroot/share/PlatformIO/` scripts common to all HALs). The compile guard is `-D__PLAT_RP2040__` (set in `raspberrypi.ini` `build_flags`), not `ARDUINO_ARCH_RP2040`. All HAL sources are guarded by `#ifdef __PLAT_RP2040__`.

### Gotchas that are not obvious from the source

1. **The MCU is dual-core and Marlin splits work across cores.** `MarlinHAL::adc_init()` launches `core1_adc_task()` on Core 1 via `multicore_launch_core1()` (`HAL.cpp`); it runs a continuous ADC scan loop forever. Marlin's `Temperature` ISR and main loop run on Core 0. Do not block or spin Core 1, and keep shared ADC state (`adc_values[]`, `*_heartbeat`) `volatile`. The watchdog refresh (`watchdog_refresh`) cross-checks both cores' heartbeats — if Core 1 stalls >2 s, Core 0 stops refreshing the WDT and lets the chip reset (`HAL.cpp`).

2. **`Serial` is USB-CDC, not a UART.** In the Arduino-Pico core, `Serial`/`SerialUSB` is the USB CDC ACM port and `Serial1`/`Serial2` are hardware UART0/UART1. Marlin's `MarlinSerial.h` wraps these as `MSerial0`/`MSerial1` (UART0/UART1) and `MSerial2` (USB). `SERIAL_PORT`/`SERIAL_PORT_2`/`LCD_SERIAL_PORT` are clamped to `-1..1` (`HAL.h`). USB-CDC emergency parsing requires `USBD_USE_CDC` + `EMERGENCY_PARSER` (`HAL.cpp`); otherwise Marlin talks over a real UART or the USB serial.

3. **EEPROM backend is per-board, not auto-selected.** The HAL ships two backends: `eeprom/eeprom_flash.cpp` (`FLASH_EEPROM_EMULATION`) and `eeprom/eeprom_wired.cpp` (`USE_WIRED_EEPROM`). Each RP2040 board's pins file must `#define` one of them; Marlin does not pick it automatically. The flash backend writes the **last flash sector** via `FLASH_TARGET_OFFSET = PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE` and **assumes 2 MB flash** — change that if a board has different flash size (`eeprom_flash.cpp`). Writes disable interrupts around `flash_range_erase`/ `flash_range_program`.

4. **USB SD-host drive (MSC) is TinyUSB-based and off by default on RP2040.** `raspberrypi.ini` compiles with `-DNO_SD_HOST_DRIVE`; `HAS_SD_HOST_DRIVE` / `msc_sd.cpp` (TinyUSB `tud_msc_*_cb` callbacks) is only active if you drop that flag (see the commented `#custom_marlin.HAS_SD_HOST_DRIVE = tinyusb` line). The USB connect/disconnect toggle in `HAL.cpp` (`USB_CONNECT_PIN`) exists only when that pin is defined.

5. **`freeMemory()` relies on a linker symbol, not a stack probe.** It computes `(char*)&__StackLimit - (char*)_sbrk(0)` using the Pico linker-provided `__StackLimit` (`HAL.cpp`). Don't "fix" it with a local-variable subtraction as on single-stack chips — that is wrong on this core.

6. **`flashFirmware()` (M997) reboots into the bootloader via the watchdog.** `flashFirmware()` → `hal.reboot()` → `watchdog_reboot(0,0,1)` (`HAL.cpp`). The `raspberrypi.ini` build flags include `-DPLATFORM_M997_SUPPORT` (also forced on in `HAL.h`). A UF2/picotool upload picks up the reset.

7. **SPI uses the core's `<SPI.h>` directly; SOFTWARE_SPI path also exists.** `HAL_SPI.cpp` includes `<SPI.h>` and calls `spiBegin()`; a software-SPI fallback is compiled when `SOFTWARE_SPI` is enabled. There is no custom `SPI` global shim needed (unlike AT32). `spi_pins.h` pins live in `HAL/RP2040/`.

## Conventions

- All HAL source is guarded by `#ifdef __PLAT_RP2040__` (defined by `ini/raspberrypi.ini`, not by the core).
- Keep HAL changes in `Marlin/src/HAL/RP2040/` and board pin changes in `Marlin/src/pins/rp2040/`. Build/wrapper glue for the upstream core goes in `ini/raspberrypi.ini` + `buildroot/share/PlatformIO/` (no vendored framework to mirror).
- The framework is upstream Arduino-Pico (Pico SDK + TinyUSB): do not vendor or patch it in-repo for Marlin; if a core fix is needed, report upstream or keep it in the HAL.
- ADC is continuous on Core 1; temperature updates are driven from Core 0's `Temperature` ISR reading `adc_values[]`. Treat the ADC plumbing as a dual-core contract.

## Related

- `README.md` in this folder — overview and supported boards.
- Wiki: [[hal-rp2040]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the RP2040 HAL reuses.
- Upstream: <https://arduino-pico.readthedocs.io/en/latest/platformio.html>.
