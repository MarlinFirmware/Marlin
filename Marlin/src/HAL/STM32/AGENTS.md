# AGENTS.md — STM32 HAL

Working notes and conventions for the **STM32 HAL** folder (`Marlin/src/HAL/STM32/`).

The STM32 HAL covers the **official STM32duino** ("generic STM32") core and serves the entire STM32 F0 / F1 / F4 / F7 / G0 / H7 family (and friends) from one directory. It is the primary 32-bit target for Marlin.

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t STM32F103RE          -n1 -y   # STM32F103RE, generic variant (F1)
buildroot/bin/mftest -t STM32F103RC_btt      -n1 -y   # SKR Mini E3 (F103RC, variant)
buildroot/bin/mftest -t STM32F401RC_creality -n1 -y   # Creality F401RC (F4)
buildroot/bin/mftest -t STM32F446VE_fysetc   -n1 -y   # FYSETC S6 / Spider (F446)
buildroot/bin/mftest -t STM32F407VE_black    -n1 -y   # "Black" F407VET6 (F4)
buildroot/bin/mftest -t NUCLEO_F767ZI        -n1 -y   # Nucleo F767ZI (F7)
buildroot/bin/mftest -t STM32G0B1RE_btt      -n1 -y   # BIGTREETECH G0B1RE (G0)
buildroot/bin/mftest -t STM32H743VI_btt      -n1 -y   # BIGTREETECH H743VI (H7)
```

Targets should be picked to span the families (F1 / F4 / F7 / G0 / H7) and both the `common_stm32` and `stm32_variant` code paths. The F103 generic (`STM32F103RE`) and an F4 board (`STM32F446VE_fysetc`) are the cheapest, highest-value smoke tests; the H7 build (`STM32H743VI_btt`) exercises the 32-bit-timer / FMC-TFT paths not seen elsewhere.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

### Maple legacy path is a SEPARATE HAL

`*_maple` boards (e.g. `chitu_f103`, `mks_robin_maple`, `jgaurora_a5s_a1_maple`, `STM32F103RC_btt_maple`, `STM32F103RC_fysetc_maple`) do **not** use this folder. They compile against the legacy **LibMaple** core via `ini/stm32f1-maple.ini`, which sets `board_build.core = maple` and `-DMAPLE_STM32F1`, and pull sources from `src/HAL/STM32F1/` (a distinct HAL). If a maple board fails, the fix usually lives in `HAL/STM32F1/`, not here. Treat maple as out-of-scope for changes to `HAL/STM32/`.

## Framework / Platform Integration (upstream, NOT vendored)

Unlike the AT32 HAL, the STM32 HAL uses the **upstream STM32duino framework**, pulled from the PlatformIO registry — it is **not** vendored under `buildroot/share/PlatformIO/`.

- Platform: `platform = ststm32@~12.1` (STM32 commons in `ini/stm32-common.ini`).
- Core / framework: `board_build.core = stm32` → the official `framework-arduinoststm32` (Arduino_Core_STM32) package installed by PlatformIO into `~/.platformio/packages/`.
- The **USB Flash Drive** mix-in (`[stm_flash_drive]`) is the one exception: it points `framework-arduinoststm32` at MarlinFirmware's fork archive (`Arduino_Core_STM32/archive/stm-flash-drive.zip`).

Consequences:

- Framework/toolchain fixes belong **upstream** (the installed PlatformIO package), not in the Marlin repo. There is no vendored copy to keep in sync — do not go looking for one.
- All STM32 `mftest` targets therefore require the `ststm32` platform + STM32duino core to already be installed in the PlatformIO cache. If a build fails with "unknown package / platform ststm32", run a normal `pio` install rather than editing repo files.
- `HAL_STM32` is the guard macro set by the build (`-DHAL_STM32` in `common_stm32`), not `ARDUINO_ARCH_STM32` alone. HAL sources guard on `#ifdef HAL_STM32`. (The maple path instead defines `ARDUINO_ARCH_STM32 -DMAPLE_STM32F1`.)

### Gotchas that are not obvious from the source

1. **`Serial` vs `MSerial1` — there is no bare `Serial`.** The Arduino STM32 core defines `Serial1`…`SerialN`, not a default `Serial`. Marlin's `MarlinSerial.h` wraps them as `MSerial1`…`MSerial10` (`Serial1Class<MarlinSerial>`) and the shared `serial_ports.h` maps `Serial` to `MSerial1` only when a port index 0 is selected. **Do not reference the bare `Serial` object in HAL code** expecting the Arduino default — use `MSerialN` or the configured `SERIAL_PORT` index. When `USBCON` is set, `SerialUSB` is wrapped as `MSerialUSB` and `USB_SERIAL_PORT(...)` resolves to it.

2. **USB-CDC vs hardware UART is a build-flag split, not a code split.** `common_stm32` builds with `-DUSBCON -DUSBD_USE_CDC` (USB CDC is the _default_ serial transport). Many boards `build_unflags` those and set `DISABLE_GENERIC_SERIALUSB` to fall back to a real UART (e.g. `rumba32`, `Anet_ET4`, the `lerdge_*` family) and enable `-DHAL_UART_MODULE_ENABLED`. So "is the port USB or UART?" is decided entirely by `build_flags`/`build_unflags`, not by which `Serial` object you touch. Watch for `-DUSE_USB_FS`, `-DUSBD_USE_CDC_MSC`, `-DUSBD_USE_CDC_COMPOSITE` variants too.

3. **EEPROM backend is per-board, not auto-selected.** Marlin does NOT auto-pick an EEPROM backend on STM32. Each board must `#define` one of `FLASH_EEPROM_EMULATION`, `SDCARD_EEPROM_EMULATION`, `IIC_EEPROM`, `SPI_EEPROM`, `SRAM_EEPROM`, etc. in its pins/Configuration files. The `eeprom/` subfolder ships ALL of these backends (`eeprom_flash.cpp`, `eeprom_sdcard.cpp`, `eeprom_if_iic.cpp`, `eeprom_bl24cxx.cpp`, `eeprom_sram.cpp`, …) but only the one selected is compiled. `FLASH_EEPROM_EMULATION` (via the core `EEPROM.h`) works only on parts with a single "sector" flash design; `FLASH_EEPROM_LEVELING` is hard-guarded to `STM32F4xx`/`STM32H7xx` only (see `inc/SanityCheck.h`). Do not assume flash-emulation works on an F0/F1/G0.

4. **Pins must be `#define` macros, or SanityCheck dies.** `inc/SanityCheck.h` opens with `#ifndef PA0 #error "Your ARM platform pins are not defined as macros, only as enums!"`. The STM32duino core represents pins as `PinName` enums, so each board/variant supplies a `pins_arduino.h` (or the generated variant) that re-exposes `PA0`, `PC6`, … as numeric `#define`s. If you add a board and hit that `#error`, the fix is at the pin-define level — **not** by relaxing SanityCheck. (`HAL.h` uses `typedef int32_t pin_t;` for parity with `platform/ststm32`.)

5. **FastIO needs `FastIO_init()` before use.** `fastio.h` routes `_WRITE`/`_READ`/`_TOGGLE` through `FastIOPortMap[]` and requires `FASTIO_INIT()` (`FastIO_init()`) to be called first; otherwise the port table is unpopulated. The F0/F1/L0/L4 branch uses `BRR` while F2/F4/F7/H7 use the `BSRR` high/low-bit form — a subtle per-family macro split.

6. **Timers are 16- or 32-bit per part.** `timers.h` defines `hal_timer_t uint32_t` but comments that STM32F0 pauses timers if `UINT32_MAX` is written to a 16-bit counter register; the 32-bit capability is queried at runtime via `IS_TIM_32B_COUNTER_INSTANCE`. Servo, stepper, and temperature timing all depend on `TIMER_SERIAL`/`TIMER_SERVO` being assigned a valid timer (e.g. `-DTIMER_SERIAL=TIM9`), and ISR priorities are pinned (`-DTIM_IRQ_PRIO=13`, USB host `USBH_IRQ_PRIO=3`). Changing a timer without rechecking the priority map can deadlock serial ISRs.

7. **ADC resolution is forced to 12-bit.** `HAL.h` sets `HAL_ADC_RESOLUTION 12` (or `ADC_RESOLUTION` if defined; `common_stm32` passes `-DADC_RESOLUTION=12`), and `MarlinHAL::adc_init()` calls `analogReadResolution(HAL_ADC_RESOLUTION)`. Don't hardcode a 10-bit assumption in sensor code.

8. **SPI is wrapped, not the raw core class.** `MarlinSPI.h` wraps the STM32duino `SPIClass` (via `utility/spi_com.h`) and exposes the `SPIClass`/`SPISettings` globals libraries expect (`HAL/shared/HAL_SPI.h`). `spi_pins.h` selects AF pins. Touching SPI for TFT/LCD or card readers goes through this wrapper, not a bare `SPI.begin()`.

9. **Servo uses `libServo` over the core `Servo`.** `Servo.h` defines `libServo` as a subclass of the STM32duino `Servo`, with `pause_all_servos()`/`resume_all_servos()` (referenced from `HAL.h` as `PAUSE_SERVO_OUTPUT`/`RESUME_SERVO_OUTPUT`) and an interrupt-priority setter. Use `libServo`, not the raw core `Servo`, in HAL-adjacent code.

10. **TFT backends are conditionally compiled in.** `common_stm32` excludes `src/HAL/STM32/tft` by default and re-adds only the pieces a board needs via `custom_marlin.*` switches (`HAS_LTDC_TFT`, `HAS_FSMC_TFT`, `HAS_SPI_TFT`, `HAS_TFT_XPT2046`, `TFT_TOUCH_DEVICE_GT911`). FSMC needs `-DHAL_SRAM_MODULE_ENABLED`. Don't add a `#include` of a tft cpp expecting it to always be linked — it may be filtered out of the build.

11. **`M997` firmware flash / reboot is supported.** `HAL.h` defines `PLATFORM_M997_SUPPORT` and `flashFirmware()`; STM32 can apply firmware updates in place (bootloader-dependent). The maple F1 path is the one notable case that uses a different core entirely (see above).

## Conventions

- All HAL source is guarded by `#ifdef HAL_STM32` (set by `common_stm32`'s `-DHAL_STM32`). Do not key STM32-specific code on `ARDUINO_ARCH_STM32` alone, since the maple legacy path also defines that.
- Keep HAL changes in `Marlin/src/HAL/STM32/` and board pin changes in `Marlin/src/pins/stm32f1/` (and the other `pins/` families). Variant `pins_arduino.h` files for `stm32_variant` boards are auto-generated by `buildroot/share/PlatformIO/scripts/generic_create_variant.py` — edit the source pins file, not the generated variant.
- PlatformIO integration lives in `ini/stm32-common.ini` (`[common_stm32]`, `[stm32_variant]`, `[stm_flash_drive]`) and the per-family `ini/stm32f*.ini`; framework behavior is upstream (STM32duino), not vendored — there is no repo copy to mirror.
- When changing serial/USB behavior, remember the upstream-framework + `build_flags` split (gotcha #2): the fix is usually a flag in the board env or a variant define, not HAL C++.
- Pin mappings must remain as numeric `#define`s (gotcha #4). Mark any placeholder pins as such and don't "fix" them to guesses without a source.

## Related

- `README.md` in this folder — overview and supported STM32 families.
- Wiki: [[hal-stm32]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, `serial_ports.h`, `HAL_SPI.h`) the STM32 HAL reuses.
- `Marlin/src/HAL/STM32F1/` — the **legacy LibMaple** HAL for `*_maple` boards (separate core, out of scope for this folder).
- `Marlin/src/pins/stm32f1/` and sibling `pins/` families — board pin tables.
