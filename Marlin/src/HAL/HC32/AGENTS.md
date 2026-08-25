# AGENTS.md — HC32 HAL

Working notes and conventions for the **HC32 HAL** folder (`Marlin/src/HAL/HC32/`).

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t HC32F460C_aquila_101 -n1 -y   # Voxelab Aquila V1.0.x, 256 KB flash
```

`HC32F460C_aquila_101` is the only in-repo test target for HC32 (board `BOARD_AQUILA_V101`, 256 KB flash, `HC32F460C` base → `maximum_size = 262144`). It must be green before considering HC32 work done. The Aquila (256 KB) build is the tighter constraint — the 512 KB variants (`HC32F460E_*`) will fit if this does.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, not vendored)

Unlike AT32, the HC32 HAL uses **upstream (third-party) Arduino packages** pulled straight from GitHub by PlatformIO — there is **no vendored framework** in this repo (`buildroot/share/PlatformIO/` carries only `framework-arduino-at32f4` and `platform-at32`, nothing for HC32). `ini/hc32.ini` declares them as remote archives:

```ini
platform          = https://github.com/shadow578/platform-hc32f46x/archive/1.1.1.zip
platform_packages = framework-hc32f46x-ddl@https://github.com/shadow578/framework-hc32f46x-ddl/archive/2.2.3.zip
                    framework-arduino-hc32f46x@https://github.com/shadow578/framework-arduino-hc32f46x/archive/1.3.1.zip
```

Implications:

- **Framework fixes are NOT made in this repo.** To change the core (flags, serial, USB, DDL/DDL options) you edit `ini/hc32.ini` `build_flags` / `board_build.*` keys, or pin a new upstream core tag. There is no `buildroot/share/PlatformIO/framework-arduino-hc32f4x` to keep in sync with an installed package — the AT32 "mirror the two copies" rule does **not** apply here.
- The core version is pinned and **enforced at compile time**: `inc/SanityCheck.h` requires `ARDUINO_CORE_VERSION_INT >= 1.1.0` (for `app_config.h`) and gates `SERIAL_DMA`, `EMERGENCY_PARSER`, and `SoftwareSerial` on core 1.2.0 / 1.3.1. Bumping the core tag is a real compatibility event — check the `ARDUINO_CORE_VERSION_INT` checks before doing so.

### Gotchas that are not obvious from the source

1. **Pins are an `enum`, not `#define`.** The upstream core defines `PA0`, `PB1`, … as a C `enum`. The C preprocessor cannot see `enum` values in `#if`, so `PIN_EXISTS(PA0)` evaluated `PA0` as `0` → false pin-conflict errors. `inc/SanityCheck.h` now hard-errors if `PA0` is not `#define`d: _"Your ARM platform pins are not defined as macros, only as enums! Provide pins_arduino.h to define the pins."_ The fix is at the pin-define level (a `pins_arduino.h` mapping enum → macro). **Do not "fix" the symptom by editing SanityCheck — fix it at the pin-define level.**

2. **`app_config.h` is injected into EVERY core source.** `board_build.app_config = Marlin/src/HAL/HC32/app_config.h` is force-included by the Arduino build script for all translation units. It re-configures the core/DDL from `Configuration.h`/`Configuration_adv.h` (`MARLIN_DEV_MODE` → `__DEBUG`/`__CORE_DEBUG`, `POSTMORTEM_DEBUGGING` → `CORE_DISABLE_FAULT_HANDLER`, `SERIAL_DMA` → `USART_RX_DMA_SUPPORT`). It also **`#define DISABLE_SERIAL_GLOBALS 1`** (the core's `Serial1`/`Serial2`/`Serial` globals are suppressed — Marlin defines its own `MarlinSerial`/`Serial` alias) and force-enables the custom `PANIC_ENABLE` panic handler (in `MinSerial.cpp`). Changing Marlin serial/panic behavior without touching `app_config.h` will silently mismatch the core.

3. **Hardware UART, not USB-CDC.** `MarlinSerial` derives from the core's `Usart` class (`drivers/usart/Usart.h`) — Marlin talks over hardware USART only. There is no USB-CDC serial class here. `SERIAL_INDEX_MIN=1`, `SERIAL_INDEX_MAX=4` (ports 1–4). The core's `SERIAL_STATS_MAX_RX_QUEUED` / `SERIAL_STATS_DROPPED_RX` are explicitly error-unsupported.

4. **Emergency parser hooks the USART RX ISR.** `HAL.h` declares `extern "C" void usart_rx_irq_hook(uint8_t ch, uint8_t usart);` (behind `ENABLED(EMERGENCY_PARSER)`). `SERIAL_DMA` + `EMERGENCY_PARSER` together require core `>= 1.2.0` (that release added `core_hook_usart_rx_irq`), else SanityCheck errors. Enable both with an older core at your peril.

5. **EEPROM backend is per-board, not auto-selected.** Marlin does NOT auto-pick an `EEPROM_*_EMULATION` backend. Three implementations ship in `eeprom/`: `eeprom_wired.cpp` (I2C/SPI wired), `eeprom_if_iic.cpp` (generic I2C), `eeprom_bl24cxx.cpp` (BL24Cxx I2C EEPROM), and `eeprom_sdcard.cpp` (`SDCARD_EEPROM_EMULATION`, requires `SDSUPPORT`). The board pins file must `#define` the backend it wants; the Aquila test uses SD-card EEPROM (`sdsupport = on`, `eeprom_settings = on`). `SDCARD_EEPROM_EMULATION` without `HAS_MEDIA` is an error.

6. **`FAST_PWM_FAN` is unsupported.** `inc/SanityCheck.h` hard-errors on `FAST_PWM_FAN` — there is no hardware PWM-fan implementation yet. Don't enable it; use software PWM fans.

7. **`NEOPIXEL_LED` (Adafruit) is unsupported** except on `MKS_MINI_12864_V3`; any other config is an error. `TEMP_SENSOR_SOC` requires `TEMP_SOC_PIN` defined and not a real GPIO pin. These are Marlin feature gaps specific to this HAL.

8. **No compile-time PWM check (`NO_COMPILE_TIME_PWM`).** `fastio.h` defines `NO_COMPILE_TIME_PWM` because PWM capability is discovered at runtime via the TimerA assignment table (`timera_get_assignment`), not via a pin macro. `PWM_PIN(IO)` calls `isAnalogWritePin(IO)` at runtime. Don't assume `analogWrite`-eligible pins are known at preprocessor time.

9. **Flash size / offset is board-specific and lives in `ini/hc32.ini`.** The Aquila uses `board_upload.offset_address = 0xC000` (bootloader start, also `secondary` boot mode to save ~1.4 KB by dropping the ICG); Ender-2-Pro uses `0x8000`; OpenHC32Boot builds use `0x4000`. `HC32F460C` = 256 KB (`maximum_size = 262144`), `HC32F460E` = 512 KB. The linker script / flash layout is supplied by the upstream platform, not vendored here.

10. **`-fno-signed-char` is required for meatpack.** `board_build.flags.common` forces `-fno-signed-char`; removing it breaks meatpack. The build also adds `-fno-threadsafe-statics`, `-fno-exceptions`, `-fno-rtti` (no C++ runtime features used by Marlin) and `-g3` (debug symbols only — does not affect binary size).

11. **DDL driver middleware is selected via `board_build.ddl.*` / `mw.*` keys.** The `ots`, `sdioc`, `wdt`, `timer0`, `timera` DDL modules and the `sd_card` middleware are pulled in through `ini/hc32.ini`. `sdio.cpp` (SD card over SDIO) depends on `ddl.sdioc` + `mw.sd_card`; dropping those breaks SD support. **Don't assume a peripheral driver exists unless its DDL/MW flag is enabled.**

## Conventions

- All HAL source is guarded by `#ifdef ARDUINO_ARCH_HC32` (added by `ini/hc32.ini` `-DARDUINO_ARCH_HC32`; `inc/SanityCheck.h` sanity-checks the env via `NOT_TARGET`).
- Keep HAL changes in `Marlin/src/HAL/HC32/` and board pin changes in `Marlin/src/pins/hc32f4/`. Build/platform integration changes (core version, DDL/MW flags, flash offsets) go in `ini/hc32.ini` — there is no vendored framework to edit.
- Pin mappings are board-specific; see `Marlin/src/pins/hc32f4/pins_AQUILA_101.h` and `pins_CREALITY_ENDER2P_V24S4.h`. Mark any pin as a placeholder if it is not verified against a schematic.
- When you change the upstream core version (the `*@https://...archive/X.Y.Z.zip` tags in `ini/hc32.ini`), re-check every `ARDUINO_CORE_VERSION_INT` gate in `inc/SanityCheck.h` (1.1.0 for `app_config.h`, 1.2.0 for `SERIAL_DMA`+`EMERGENCY_PARSER`, 1.3.1 for `SoftwareSerial`) and rebuild the test target.

## Related

- `README.md` in this folder — overview, "adding a board" guide, dependencies, documentation notes.
- `app_config.h` in this folder — the auto-included core/DDL config bridge.
- Wiki: [[hal-hc32]], [[marlin-src-hal]].
- `Marlin/src/pins/hc32f4/` — board pin files.
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the HC32 HAL reuses.
- `Marlin/src/HAL/AT32/AGENTS.md` — the younger sibling HAL, modeled on this one.
