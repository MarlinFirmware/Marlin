# AGENTS.md — STM32F1 HAL

Working notes and conventions for the **STM32F1 HAL** folder (`Marlin/src/HAL/STM32F1/`).

This HAL targets STM32F103 (and GD32F103 / N32G455 clones) boards using the **LibMaple-based** "Roger Clark" Arduino_STM32 core (`board_build.core = maple`), NOT the unified STM32duino core that the rest of the STM32 family uses.

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t STM32F103RC_btt_maple -n1 -y      # SKR Mini E3 1.0, 256K
buildroot/bin/mftest -t mks_robin_maple -n1 -y            # MKS Robin, FSMC UI
buildroot/bin/mftest -t jgaurora_a5s_a1_maple -n1 -y      # JGAurora A5S/A1, ZE
buildroot/bin/mftest -t STM32F103RC_meeb_maple -n1 -y     # MEEB_3DP, 512K, USB
buildroot/bin/mftest -t mks_robin_nano_v1v2_maple -n1 -y  # Robin Nano, FSMC
buildroot/bin/mftest -t STM32F103VE_longer_maple -n1 -y   # Alfawise U20, VE
buildroot/bin/mftest -t mks_robin_pro_maple -n1 -y
buildroot/bin/mftest -t mks_robin_lite_maple -n1 -y
buildroot/bin/mftest -t STM32F103RC_fysetc_maple -n1 -y
buildroot/bin/mftest -t STM32F103RC_btt_USB_maple -n1 -y  # USB Composite
buildroot/bin/mftest -t STM32F103VE_ZM3E4V2_USB_maple -n1 -y
```

> **Only the `_maple` envs in `ini/stm32f1-maple.ini` compile this HAL** (`build_src_filter` includes `src/HAL/STM32F1`, `-DMAPLE_STM32F1`). The non-maple envs in `ini/stm32f1.ini` (`STM32F103RE`, `mks_robin`, `STM32F103RE_creality`, …) extend `common_stm32` and build the **unified** `src/HAL/STM32` (`-DHAL_STM32`) — they do NOT exercise this folder.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, not vendored)

The STM32F1 (LibMaple) core is pulled from the **upstream** `ststm32@~15.4.1` PlatformIO platform (`board_build.core = maple`) — it is **not** vendored by Marlin, so there is no "mirror to installed package" step (that warning is AT32-specific only). Marlin-specific glue lives in:

- `Marlin/src/HAL/STM32F1/` — the HAL itself.
- `buildroot/share/PlatformIO/scripts/STM32F1_build_flags.py` — emits base C/CXX/LINK flags (`-Os`, `-mcpu=cortex-m3`, `-ffreestanding`, `--specs=nano.specs`, `-IMarlin/src/HAL/STM32F1`, `-DTARGET_STM32F1 -DARDUINO_ARCH_STM32 -DPLATFORM_M997_SUPPORT`).
- `buildroot/share/PlatformIO/scripts/{fix_framework_weakness,stm32_serialbuffer,custom_board,offset_and_rename,STM32F1_create_variant}.py` — run as `pre:`/`custom_marlin` extras (per-board offsets, variant creation for Chitu/Meeb).

Toolchain is pinned: `toolchain-gccarmnoneeabi@1.120301.0` (else GCC 7.2.1) and `tool-stm32duino`. `lib_ignore = SPI, FreeRTOS701, FreeRTOS821`; `SoftwareSerialM` is added.

### Gotchas that are not obvious from the source

1. **Two different "STM32F1" paths.** `ini/stm32f1.ini` targets (e.g. `STM32F103RE`, `mks_robin`) silently use the unified `HAL/STM32`, while only `ini/stm32f1-maple.ini` targets use this `HAL/STM32F1`. Changing one does not test the other — build a `_maple` target to exercise this folder.

2. **Pins are numeric `#define`s, not an `enum`.** `fastio.h` defines `PA0 0x00` … `PG15 0x6F` and macros use the libmaple `PIN_MAP[IO]` table (e.g. `READ`/`WRITE`). Unlike the AT32 `enum` quirk, `PIN_EXISTS` works in the preprocessor here. BUT the **CB/C8 parts** only expose `PC13/PC14/PC15` (guarded at `fastio.h:97`), while larger parts expose `PC0‑PC15` — pin files must match the die.

3. **Flash size / linker script are per-board.** `HAL.h` defaults `STM32_FLASH_SIZE` to 256, but 512 for `MCU_STM32F103RE/VE/ZE`. Each env passes its own `board_build.ldscript` (`creality256k.ld`, `creality.ld`, `crealityPro.ld`, `mks_robin.ld`, `mks_robin_mini.ld`, `fly_mini.ld`, `jgaurora_a5s_a1.ld`, `ZONESTAR_ZM3E_*.ld`, …). Mismatched `.ld` → silent overflow or wrong vector table.

4. **EEPROM backend is auto-fallback, not auto-selected.** `inc/Conditionals_post.h` defines `SDCARD_EEPROM_EMULATION` only when `USE_FALLBACK_EEPROM` (no I2C/SPI/flash EEPROM). `eeprom/` carries `eeprom_sdcard`, `eeprom_wired`, `eeprom_if_iic`, `eeprom_flash`, `eeprom_bl24cxx` — a board pins file must pick one. `SDCARD_EEPROM_EMULATION` requires `SDSUPPORT` (else `SanityCheck.h` warns and undefs it).

5. **N32 (Nationstar) clone layer.** `HAL_N32.h/.cpp` re-implements ADC/GPIO/DMA register maps for the N32G455 (board `N32G455RE_voxelab_maple`, `-DVOXELAB_N32`). It is not a generic N32 port — don't assume other N32 parts build.

6. **USB vs hardware UART.** Default is hardware USART (`HardwareSerial`). USB serial is opt-in: `MarlinSerial.h` only defines `USBSerial`/`UsbSerial` under `#ifdef SERIAL_USB`, and the Zonestar/Meeb/BTT-USB targets add `USBComposite for STM32F1` + `-DUSE_USB_COMPOSITE` / `-DSERIAL_USB`. Don't enable USB flags on a UART-only board.

7. **Timer assignments vary per board.** `TIMER_SERVO`, `TIMER_TONE`, `SS_TIMER`, `TEMP_TIMER_CHAN` are set in each env (e.g. `TIMER_SERVO=TIM5`, `SS_TIMER=4`). Two Chitu envs expose `chitu_v5_gpio_init` / `-DCHITU_V5_Z_MIN_BUGFIX` for a known GPIO-init bug (use when G28/G29 always fail on Chitu V5).

8. **GD32F103 / N32 reuse this HAL.** `GD32F103RC_voxelab_maple`, `N32G455RE_voxelab_maple`, and `STM32_XL_DENSITY` Z-parts (`jgaurora_a5s_a1_maple`, `chitu_f103_maple`) all build `src/HAL/STM32F1`, so a HAL change can affect clone boards too.

9. **`SPI` global.** The maple core leaves `SPI` out; `HAL_SPI.cpp` instantiates `SPIClass SPI(...)` and provides `spiBegin()` (mirrors the AT32 pattern).

## Conventions

- Source is selected by the `MAPLE_STM32F1` build flag (with `ARDUINO_ARCH_STM32`); some files guard on `BOARD_NR_GPIO_PINS` ("Only in MAPLE_STM32F1", see `pinsDebug.h`).
- Keep HAL changes in `Marlin/src/HAL/STM32F1/` and board pin changes in `Marlin/src/pins/stm32f1/` (and the various `pins/` clones). Build/glue changes go in `buildroot/share/PlatformIO/scripts/` and `ini/stm32f1-maple.ini`.
- Build flags live in `STM32F1_build_flags.py` (a `pre:` extra), **not** in `platform.json`.
- Pin tables in `fastio.h` are MCU-die-specific (CB/C8 vs others) — don't generalize.

## Related

- `README.md` in this folder — overview and supported boards.
- Wiki: [[hal-stm32f1]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) this HAL reuses.
- `Marlin/src/HAL/STM32/` — the unified STM32duino HAL that the non-maple F1 targets use.
