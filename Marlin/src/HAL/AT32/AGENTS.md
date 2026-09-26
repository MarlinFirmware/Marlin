# AGENTS.md — AT32 HAL

Working notes and conventions for the **AT32 HAL** folder (`Marlin/src/HAL/AT32/`). This file is the precedent for per-HAL `AGENTS.md` files: it captures the build/test loop, the framework-integration gotchas that are NOT obvious from the source, and the conventions a maintainer or agent must follow when touching this HAL.

## Build & Test Loop

Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t WANHAO_0327001 -n1 -y          # AT32F403ARGT7, 256 KB flash
buildroot/bin/mftest -t EASYTHREED_ET4000_PLUS -n1 -y  # AT32F403ARCT7, 1 MB flash
```

Both targets must be green before considering AT32 work done. The Wanhao (256 KB) build is the tighter constraint — if it fits there, EasyThreed (1 MB) will too.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (vendored, not the upstream core)

The AT32 Arduino core is **vendored** under `buildroot/share/PlatformIO/framework-arduino-at32f4/` and the platform under `buildroot/share/PlatformIO/platform-at32/`. These carry Marlin-specific fixes. If you change the build (flags, link, USB, serial), change them in BOTH:

- `~/.platformio/packages/framework-arduino-at32f4/` (what the build actually uses), and
- `buildroot/share/PlatformIO/framework-arduino-at32f4/` (what ships in the repo / PR).

A framework change in the installed package alone will NOT be in the PR. Keep the two in sync.

### Gotchas that are not obvious from the source

1. **Pins are an `enum`, not `#define`.** The core's `libcore/gpio.h` defines `PC6`, `PC7`, … as a C `enum`. The C preprocessor cannot see `enum` values in `#if`, so `PIN_EXISTS(PC6)` evaluated `PC6` as `0` → false pin-conflict errors (e.g. `PART_COOLING_FAN1_PIN == PART_COOLING_FAN0_PIN`). The fix: `HAL/AT32/pins_arduino.h` (and the variant `variants/marlin_at32f403/pins_arduino.h`) define `#define PC6 PC6` etc. **Do not "fix" the symptom by editing SanityCheck — fix it at the pin-define level.**

2. **Optimization flags live in the framework build script, not `platform.json`.** PlatformIO does not auto-apply a custom `platform.json` `build.flags` to a custom framework `platformio-build-arduino.py`. Without `-Os -DNDEBUG` appended to `CFLAGS`/`CXXFLAGS` in that script, Marlin compiles to ~2 MB and overflows flash. Hard-float `LINKFLAGS` (`-mfloat-abi=hard -mfpu=fpv4-sp-d16`) are also required there or the link step rejects the hard-float objects.

3. **`FrameLib` is an empty stub.** The shipped `FrameLib.a` has no symbols. The real peripheral driver code is in `system/Drivers/Firmware/src/` and is compiled as `BuildSources`. Do not add `-lFrameLib` back.

4. **Device header family macro.** `system/Drivers/Firmware/src/at32f403a_407.h` ships with `//#define AT32F403Axx` commented out. It must be uncommented or peripheral base addresses are undefined. Board JSONs also pass `-DAT32F403Axx` in `extra_flags`.

5. **USB-CDC is guarded out.** Marlin uses hardware UART, not USB-CDC. All USB-CDC code in `mcu_core.c` / `usb_vcp_wrapper.cpp` / `usb_vcp_wrapper.h` is behind `#ifdef AT32_USB_CDC` (undefined for Marlin). The core also declared `extern VcpSerial Serial;` which conflicts with Marlin's `Serial`; guarding it out is what lets the `HardwareSerial& Serial = Serial1;` alias stand.

6. **The bare `Serial` global + `println`.** The core only defines `Serial1`–`Serial5` / `Serial7`. Marlin feature code in the core (`DShot.cpp`, `UbxGpsParser.cpp`) references `Serial.println(...)`. `HardwareSerial.h` declares `extern HardwareSerial& Serial;`, `HardwareSerial.cpp` defines `HardwareSerial& Serial = Serial1;` and a `println(const char*)` that emits CRLF via `write((uint8_t)0x0D)` / `write((uint8_t)0x0A)`. **The core files are CRLF and string-literal `\r\n` escapes get mangled by some edit layers — use the hex byte writes instead.**

7. **Flash size / linker script.** The Wanhao (ARGT7) has 256 KB flash; the framework only ships a 1 MB linker script. `AT32F403AxR_FLASH.ld` (256 KB) is assigned to the Wanhao board JSON; EasyThreed uses the 1 MB `AT32F403AxG_FLASH.ld`.

8. **EEPROM backend is per-board, not auto-selected.** Marlin does NOT auto-pick `SDCARD_EEPROM_EMULATION`. Each AT32 board's pins file must `#define SDCARD_EEPROM_EMULATION` (both Wanhao and EasyThreed do). `PersistentStore` is implemented in `eeprom/eeprom_sdcard.cpp` (ported from the STM32 HAL).

9. **`pinMode` overload.** The core declares `pinMode(pin, PinMode_TypeDef)` strictly; `M43.cpp` calls `pinMode(pin, bool)`. `HAL/AT32/fastio.h` adds a `pinMode(uint8_t, int)` overload.

10. **`SPI` global.** The core leaves the global `SPI` commented out; `HAL_SPI.cpp` instantiates `SPIClass SPI(SPI_CLASS_1_SPI);` and provides `spiBegin()`.

## Conventions

- All HAL source is guarded by `#ifdef ARDUINO_ARCH_AT32` (the core does not define it; `ini/at32.ini` adds `-DARDUINO_ARCH_AT32`).
- Keep HAL changes in `Marlin/src/HAL/AT32/` and board pin changes in `Marlin/src/pins/at32f4/`. Framework/platform integration changes go in `buildroot/share/PlatformIO/` (and must be mirrored to the installed package).
- When you change the vendored framework, update BOTH the repo copy and `~/.platformio/packages/...`, and verify the build before committing.
- Pin mappings in `pins_at32f4/*.h` are placeholders pending schematic verification — mark them as such and do not "fix" them to match guesses without a source.

## Related

- `README.md` in this folder — overview and supported boards.
- Wiki: [[hal-at32]], [[at32-build-notes]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the AT32 HAL reuses.
