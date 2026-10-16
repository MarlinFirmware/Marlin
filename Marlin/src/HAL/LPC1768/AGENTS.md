# AGENTS.md — LPC1768 HAL

Working notes and conventions for the **LPC1768 HAL** folder (`Marlin/src/HAL/LPC1768/`). Covers the NXP LPC1768/LPC1769 family (ARM Cortex-M3) used on ReARM, Smoothieboard, BTT SKR 1.3/1.4, MKS SBASE, etc.

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t LPC1768 -n1 -y        # env LPC1768 — ReARM / RAMPS_14_RE_ARM boards
buildroot/bin/mftest -t LPC1769 -n1 -y        # env LPC1769 — Smoothieboard, etc.
```

Both targets should be green before considering LPC1768 work done. `LPC1768` is the primary/most-used env (both MCUs share 512 KB flash / 64 KB RAM; LPC1769 just clocks higher). Pick the individual test with `-n` (e.g. `-n1` … `-n5`); `LPC1768/config-01.ini` is ReARM EFB + VIKI2 + SD + NeoPixel + dual stepper.

> Do NOT use `rm -rf .pio` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream community core, not vendored)

The LPC176x support is **not** a Marlin-vendored framework. `ini/lpc176x.ini` pulls an upstream **community** Arduino core from GitHub:

- `platform  = https://github.com/p3p/pio-nxplpc-arduino-lpc176x/archive/0.1.3.zip`
- `framework-arduino-lpc176x@https://github.com/p3p/pio-framework-arduino-lpc176x/archive/ab41696b64.zip`
- `toolchain-gccarmnoneeabi@1.100301.220327`

These land in `~/.platformio/packages/…` as normal PlatformIO packages — there is no copy under `buildroot/share/PlatformIO/`, so the AT32 "mirror to installed package" warning does NOT apply here. Marlin-specific glue lives entirely in this HAL folder plus `Marlin/src/pins/lpc1768/`. Build flags of note: `-std=gnu++20 -Wno-volatile -DU8G_HAL_LINKS -DPLATFORM_M997_SUPPORT`, and `build_src_filter` excludes `src/HAL/LPC1768/tft` by default (selected per-board via `custom_marlin.HAS_SPI_TFT`).

### Gotchas that are not obvious from the source

1. **Guard macro is `TARGET_LPC1768`, not `ARDUINO_ARCH_*`.** It is defined by the platform/board JSON, not by the core. Every file in this HAL is wrapped in `#ifdef TARGET_LPC1768`, and `HAL/platforms.h` selects `HAL/LPC1768` on that macro. `LPC1769` builds compile the _same_ folder (the MCU difference is handled inside the core, e.g. `isLPC1769()` in `HAL.cpp`).

2. **The default `Serial` is USB-CDC, not a UART.** `MarlinSerial.h` defines `USB_SERIAL_PORT(...) USBSerial` where `USBSerial` is a `ForwardSerial1Class<UsbSerial>` over the USB CDC stack (`usb_serial.cpp`, `cdcuser.h`). Hardware UARTs `MSerial0..3` (UART0–UART3) are defined separately in `MarlinSerial.cpp` behind `USING_HW_SERIALx`. Boards that want UART-as-primary must route a hardware serial to `Serial`.

3. **`HardwareSerial` interface mismatch → `ForwardSerial1Class` wrapper.** The core's `HardwareSerial` does not implement Arduino's `Serial` overloads, so `MarlinSerial.h` wraps `MarlinSerial` in a `ForwardSerial1Class<MarlinSerial>` (`MSerialT`) and warns in comments. Because of that, `SERIAL_RUNTIME_HOOK` is explicitly unsupported here — `MarlinSerial.h` has `#error "SERIAL_RUNTIME_HOOK is not yet supported for LPC176x."`

4. **EEPROM backend is per-board, never auto-selected.** Three backends ship in `eeprom/`, each gated by a define the board/pins file must set: `eeprom_wired.cpp` (`USE_WIRED_EEPROM`), `eeprom_sdcard.cpp` (`SDCARD_EEPROM_EMULATION`), `eeprom_flash.cpp` (`FLASH_EEPROM_EMULATION`). Most boards use `SDCARD_EEPROM_EMULATION` (e.g. `pins_BTT_SKR_V1_4.h` selects it over `I2C_EEPROM`). Marlin will not pick one for you — forgetting the define yields no settings storage.

5. **Flash EEPROM uses IAP on sector 29 with wear-leveling.** `eeprom_flash.cpp` reserves a 32 KB sector and writes 4 KB "slots" in turn, erasing the whole sector only when all slots are used. It holds a `ram_eeprom[]` image and disables IRQs around the IAP calls (`lpc17xx_iap.h` `CopyRAM2Flash`/`EraseSector`). Don't touch the IAP critical sections.

6. **Hardware-SPI pins can't be bit-banged while the SSP is active.** `HAL_SPI.cpp` documents that SCK/MOSI/MISO cannot be set with `WRITE`/`digitalWrite` once the LPC17xx SSP module is on; if the LCD and SD share those pins you MUST use `SOFTWARE_SPI`. The global `SPI` object is instantiated in `HAL_SPI.cpp` keyed off `SD_MISO_PIN` matching `BOARD_SPI1_MISO_PIN`/`BOARD_SPI2_MISO_PIN`.

7. **Firmware update (M997) works via USB reconnect + NVIC reset.** `HAL.cpp` `flashFirmware()` does `USB_Connect(false)`, delays, then `hal.reboot()` (`NVIC_SystemReset()`). `PLATFORM_M997_SUPPORT` is forced on in `HAL.h`. Upload itself is mass-storage copy: `upload_extra_script.py` auto-finds the `REARM` volume or a `FIRMWARE.CUR` file and sets `upload_port` — manual copy of `.pio/build/<env>/firmware.bin` also works.

8. **All pins are PWM-capable; ADC is median-filtered.** `fastio.h` sets `PWM_PIN(P) true` and `HAL.h` defines `HAL_CAN_SET_PWM_FREQ`. ADC uses a median filter (`ADC_MEDIAN_FILTER_SIZE 23`) + low-pass (`ADC_LOWPASS_K_VALUE 2`) and is already filtered in-HAL (`HAL_ADC_FILTERED`), so don't re-filter upstream.

9. **Onboard-SD pins are sensitive.** `HAL.h` declares `HAL_SENSITIVE_PINS P0_06, P0_07, P0_08, P0_09` (the MCU's onboard SD bus) — M42/M43 must not let these be toggled.

## Conventions

- All HAL source is guarded by `#ifdef TARGET_LPC1768` (set by the platform, not the core).
- Keep HAL changes in `Marlin/src/HAL/LPC1768/`; board pin changes in `Marlin/src/pins/lpc1768/`. Build-root integration (`ini/lpc176x.ini`, `upload_extra_script.py`) is the only place for build/upload tweaks.
- The core is an upstream p3p package, not a Marlin-vendored one — no `buildroot/share/PlatformIO` mirror step is needed. Fixes to the core itself must be upstreamed to p3p's repos, not patched in-tree.
- Don't "fix" missing `Serial`/EEPROM behavior in `SanityCheck` — solve it at the pin/ backend level in the board's pins file.

## Related

- `Marlin/src/pins/lpc1768/` — board pin definitions (ReARM, Smoothieboard, SKR, SBASE…).
- Wiki: [[hal-lpc1768]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, `HAL_SPI`, `serial_ports`) reused here.
