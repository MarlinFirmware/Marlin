# AGENTS.md — AT32 HAL

Working notes and conventions for the **AT32 HAL** folder (`Marlin/src/HAL/AT32/`). This file is the precedent for per-HAL `AGENTS.md` files: it captures the build/test loop, the framework-integration gotchas that are NOT obvious from the source, and the conventions a maintainer or agent must follow when touching this HAL.

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t WANHAO_0327001 -n1 -y          # AT32F403ARGT7, 1 MB flash
buildroot/bin/mftest -t EASYTHREED_ET4000_PLUS -n1 -y  # AT32F403ARCT7, 256 KB flash
```

Both targets must be green before considering AT32 work done. The 256 KB EasyThreed builds are the tighter constraint — if they fit, the Wanhao (1 MB) build will too.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration

The AT32 Arduino core is **not** in this repo. PlatformIO downloads it from the [MarlinFirmware/ArduinoCore-AT32F4](https://github.com/MarlinFirmware/ArduinoCore-AT32F4) fork (tag `marlin-1.0.0.3`, branch `marlin`), which carries the Marlin-specific fixes and the `marlin_at32f403` variant. The platform is [MarlinFirmware/platform-at32](https://github.com/MarlinFirmware/platform-at32) (tag `1.0.0` in `ini/at32.ini`); its `platform.json` names the framework archive and its `boards/` holds the board JSONs.

- Framework changes (core, variant, linker scripts, build script) are made in the fork, then pinned by updating the framework URL in platform-at32's `platform.json`, tagging platform-at32, and updating the platform URL in `ini/at32.ini`.
- Editing `~/.platformio/packages/framework-arduino-at32f4/` only changes your local build. It is overwritten on reinstall and never reaches a PR.
- Board JSONs live in platform-at32's `boards/`, not in Marlin.

### Gotchas that are not obvious from the source

1. **Pins are an `enum`, not `#define`.** The core's `libcore/gpio.h` defines `PC6`, `PC7`, … as a C `enum`. The C preprocessor cannot see `enum` values in `#if`, so `PIN_EXISTS(PC6)` evaluated `PC6` as `0` → false pin-conflict errors (e.g. `PART_COOLING_FAN1_PIN == PART_COOLING_FAN0_PIN`). The fix: the framework variant's `variants/marlin_at32f403/pins_arduino.h` (included by `HAL.h` as `<pins_arduino.h>`) defines numeric macros (`#define PC6 38`, …) in enum order, checked by a `static_assert` on `PIN_MAX`. The AT32F403A has ports A–E only. **Do not "fix" the symptom by editing SanityCheck — fix it at the pin-define level.**

2. **Optimization flags live in the framework build script, not `platform.json`.** PlatformIO does not auto-apply a custom `platform.json` `build.flags` to a custom framework `platformio-build-arduino.py`. Without `-Os -DNDEBUG` appended to `CFLAGS`/`CXXFLAGS` in that script, Marlin compiles to ~2 MB and overflows flash. Hard-float `LINKFLAGS` (`-mfloat-abi=hard -mfpu=fpv4-sp-d16`) are also required there or the link step rejects the hard-float objects.

3. **`FrameLib` is an empty stub.** The shipped `FrameLib.a` has no symbols. The real peripheral driver code is in `system/Drivers/Firmware/src/` and is compiled as `BuildSources`. Do not add `-lFrameLib` back.

4. **Device header family macro.** `system/Drivers/Firmware/src/at32f403a_407.h` ships with `//#define AT32F403Axx` commented out. It must be uncommented or peripheral base addresses are undefined. Board JSONs also pass `-DAT32F403Axx` in `extra_flags`.

5. **USB-CDC is guarded out.** Marlin uses hardware UART, not USB-CDC. All USB-CDC code in `mcu_core.c` / `usb_vcp_wrapper.cpp` / `usb_vcp_wrapper.h` is behind `#ifdef AT32_USB_CDC` (undefined for Marlin). The core also declared `extern VcpSerial Serial;` which conflicts with Marlin's `Serial`; guarding it out is what lets the `HardwareSerial& Serial = Serial1;` alias stand.

6. **The bare `Serial` global + `println`.** The core only defines `Serial1`–`Serial5` / `Serial7`. Marlin feature code in the core (`DShot.cpp`, `UbxGpsParser.cpp`) references `Serial.println(...)`. `HardwareSerial.h` declares `extern HardwareSerial& Serial;`, `HardwareSerial.cpp` defines `HardwareSerial& Serial = Serial1;` and a `println(const char*)` that emits CRLF via `write((uint8_t)0x0D)` / `write((uint8_t)0x0A)`. **The core files are CRLF and string-literal `\r\n` escapes get mangled by some edit layers — use the hex byte writes instead.**

7. **Flash size / linker script.** The ARCT7 (EasyThreed ET4000+) has 256 KB flash and uses `AT32F403AxR_FLASH.ld`; the ARGT7 (Wanhao) has 1 MB and uses `AT32F403AxG_FLASH.ld`; the AT32F407RCT7 (K7 Rev. B) uses `AT32F407xC_FLASH.ld`. These live in the framework variant and are named by the board JSONs.

8. **EEPROM backend is per-board, not auto-selected.** Marlin does NOT auto-pick `SDCARD_EEPROM_EMULATION`. Each AT32 board's pins file must `#define SDCARD_EEPROM_EMULATION` (both Wanhao and EasyThreed do). `PersistentStore` is implemented in `eeprom/eeprom_sdcard.cpp` (ported from the STM32 HAL).

9. **Pin modes come from the GPIO registers.** The core doesn't track pin modes, so `fastio.h` reads each pin's 4-bit config from CFGLR/CFGHR (`_GET_CFG`) for `IS_INPUT`/`IS_OUTPUT`/`IS_ANALOG`/`IS_MUX`. M43 (`pinsDebug.h`) uses these plus the timer registers (`ctrl1.tmren`, `cctrl` channel enable, `cNdt`/`pr`) to report PWM. The core's `analogRead()` takes a pin, so `PinsDebug::analog_index()` returns the pin and `print_analog()` shows the ADC channel.

10. **`pinMode` overload.** The core declares `pinMode(pin, PinMode_TypeDef)` strictly; `M43.cpp` calls `pinMode(pin, bool)`. `HAL/AT32/fastio.h` adds a `pinMode(uint8_t, int)` overload.

11. **`SPI` global.** The core leaves the global `SPI` commented out; `HAL_SPI.cpp` instantiates `SPIClass SPI(SPI_CLASS_1_SPI);` and provides `spiBegin()`.

## Conventions

- All HAL source is guarded by `#ifdef ARDUINO_ARCH_AT32` (the core does not define it; `ini/at32.ini` adds `-DARDUINO_ARCH_AT32`).
- Keep HAL changes in `Marlin/src/HAL/AT32/` and board pin changes in `Marlin/src/pins/at32f4/`. Platform and board-JSON changes go in MarlinFirmware/platform-at32; framework changes go in the ArduinoCore-AT32F4 fork.
- After changing the framework fork, tag it, update platform-at32 and tag that, then update `ini/at32.ini` and verify the build before committing.
- Pin mappings in `pins_at32f4/*.h` are placeholders pending schematic verification — mark them as such and do not "fix" them to match guesses without a source.

## Related

- `README.md` in this folder — overview and supported boards.
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the AT32 HAL reuses.
