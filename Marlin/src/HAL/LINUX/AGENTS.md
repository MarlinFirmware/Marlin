# AGENTS.md — LINUX HAL

Working notes and conventions for the **LINUX HAL** folder (`Marlin/src/HAL/LINUX/`).

## Build & Test Loop

The LINUX HAL is a **native host build** — there is no MCU and no Arduino framework. Marlin compiles as a regular C++ program (a console application) and runs on the host CPU. Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t linux_native -n1 -y          # host-native build, EEPROM enabled
buildroot/bin/mftest -t linux_native_test -n1 -y      # build + Unity unit tests
```

The `linux_native` target (`buildroot/tests/linux_native/config-01.ini`, `BOARD_SIMULATED`, `EEPROM_SETTINGS` on) is the primary smoke target and must be green before considering LINUX work done. The `linux_native_test` target extends it with the PlatformIO Unity test framework (`+<test>` sources, `-Werror`) and is the one to run when changing HAL code that has unit tests.

> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (native, not embedded)

This is a `platform = native` build (`ini/native.ini`, `[env:linux_native]`). There is **no Arduino core, no toolchain SDK, no vendored framework** — the only "platform" is the host's C++ compiler (`-std=gnu++17`), linked against `librt` / `libpthread`. The HAL selected by `__PLAT_LINUX__` (`HAL/platforms.h` maps `__PLAT_LINUX__` → `src/HAL/LINUX/`); it has no `ARDUINO_ARCH_*` define. `main()` lives in `main.cpp` (not Arduino's `setup()`/`loop()` shim) and spins up `std::thread`s for serial I/O and the simulation loop, then calls `setup()` / `loop()`.

Because it is a plain C++ program:

- `build_src_filter` adds `+<src/HAL/LINUX>` to the common filter; the rest of Marlin is compiled normally.
- `build_src_flags` adds `-IMarlin/src/HAL/LINUX/include` so the local `Arduino.h`, `serial.h`, and `pinmapping.h` are found ahead of any system headers.
- `-lrt -lpthread` are required link flags (timers/clock, threading).
- `HAS_LIBBSD` (macOS) provides `strlcpy`; on Linux the HAL falls back to a local `MarlinHAL::_strlcpy` (`HAL.h`/`HAL.cpp`), so no system lib is needed there.

### Gotchas that are not obvious from the source

1. **No real pins — GPIO is a software array.** `Gpio::pin_count = 255` (`hardware/Gpio.h`); pins are indices into an in-memory `pin_map[]` (`Gpio.cpp`), not physical lines. `fastio.h` macros (`READ_PIN`, `WRITE_PIN`, `SET_DIR_*`) all delegate to `Gpio::` static methods. `PIN_EXISTS`/`GET_PIN_MAP_INDEX` are pass-through identity ops; `PWM_PIN` and `INTERRUPT_PIN` always return `false`. Don't add hardware-specific pin logic here.

2. **Serial is virtual, via stdio.** `HalSerial` (`include/serial.h`) is a pair of `RingBuffer<uint8_t,128>` buffers. `main.cpp`'s `write_serial_thread` flushes the TX ring to `stdout` and `read_serial_thread` reads `stdin` (via `fgets`) into the RX ring. So G-code must be piped/entered on the process's stdin/stdout — there is no UART/USB device. `host_connected` starts `true`; `write()` busy-waits while the TX ring is full.

3. **EEPROM is a file on disk.** `eeprom.cpp` reads/writes `eeprom.dat` (4 KB, `MARLIN_EEPROM_SIZE = 0x1000`) in the current working directory via `fopen`. Settings persist across runs in that file, and a stale `eeprom.dat` in your CWD can mask config changes — delete it to get a fresh store. The buffer is the in-memory source of truth; `access_finish()` is what actually flushes to disk.

4. **No real ADC / PID feedback.** `MarlinHAL::adc_value()` synthesizes a 10-bit reading from the simulated GPIO bit value (`Gpio::get(pin) >> 2`). `main.cpp`'s `simulation_loop()` runs `Heater` (hotend/bed) and `LinearAxis` (X/Y/Z/E) objects that update pin state in software — this is the only "hardware" feedback loop, and it runs on its own thread alongside `loop()`.

5. **`main()` is excluded under `UNIT_TEST`.** `main.cpp` is wrapped in `#ifndef UNIT_TEST` (and `#ifdef __PLAT_LINUX__`). The `linux_native_test` env compiles the HAL as a library for Unity tests and does **not** link `main()` — so a test build "succeeds" without the console entry point. Don't add HAL runtime code that only runs from `main()` and expect it to execute under the test env.

6. **Threads + `yield()` model timing, not interrupts.** There are no real ISRs; `CRITICAL_SECTION_START/END` are empty, `isr_on/off` are no-ops, and `DELAY_CYCLES` is `Clock::delayCycles()`. The step/sim timing is driven by `Clock` (set to `F_CPU` in `main()`) and `HAL_timer_init()`, plus per-thread `std::this_thread::yield()`. Real-time behavior is approximate and host-load dependent.

7. **`freeMemory()` always returns 0.** Unlike embedded HALs, the LINUX HAL reports no meaningful free-SRAM figure (`HAL.cpp` returns `0`; `MarlinHAL::freeMemory()` wraps it). Don't key any logic off the returned heap value.

8. **`NUM_ANALOG_INPUTS` is fixed at 16.** `include/pinmapping.h` hard-codes 16 analog inputs mapped to the top of the 255-entry digital space (`analog_offset`). The analog index ↔ digital pin conversions are constexpr arithmetic, not a board-specific table.

9. **SD / media is file-based, not SPI.** There is no SD card controller; `spi_pins.h` still defines default `SD_SCK_PIN`/`SD_MISO_PIN`/`SD_MOSI_PIN` (50/51/52) and a `SOFTWARE_SPI` fallback, but actual media acts on the host filesystem. Treat SD operations as ordinary file I/O in the host environment.

10. **Optional GPIO/position CSV logging.** `#define GPIO_LOGGING` in `main.cpp` (off by default) attaches an `IOLoggerCSV` and writes `all_gpio_log.csv` / `axis_position_log.csv` into the CWD. It is a debugging aid only and adds filesystem side effects; leave it disabled unless actively tracing pin/axis activity.

## Conventions

- All HAL source is guarded by `#ifdef __PLAT_LINUX__` (not `ARDUINO_ARCH_*`). The licensing/skeleton header wraps every `.cpp` in this guard.
- Keep HAL changes in `Marlin/src/HAL/LINUX/` and its `hardware/`, `include/`, `inc/`, `u8g/` subfolders. There are no board pin files here — `BOARD_SIMULATED` is the only board and the pin map is the fixed software array.
- The build is host-native: no flash/linker-script constraints, no MCU family macros, no Arduino `Serial` object — `MYSERIAL1` is the `HalSerial`-backed `usb_serial` (`HAL.h`/`HAL.cpp`).
- `simulator_linux_*` envs in `ini/native.ini` are a _different_ target: they build the graphical `NATIVE_SIM` HAL (`src/HAL/NATIVE_SIM`, SDL2/GLM UI), **not** this LINUX HAL. Do not conflate the two — LINUX is the headless console build, NATIVE_SIM is the GUI simulator.
- When you change HAL code, run `linux_native` (and `linux_native_test` if unit tests are affected) and verify green before committing.

## Related

- `Marlin/src/HAL/NATIVE_SIM/` — the sibling GUI simulator HAL (SDL2/GLM), built via `simulator_linux_*` envs (distinct from this LINUX HAL).
- Wiki: [[hal-linux]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers, `Marduino.h`) that this HAL reuses.
- `ini/native.ini` — `[env:linux_native]` / `[env:linux_native_test]` definitions.
- `buildroot/tests/linux_native/config-01.ini` — the `BOARD_SIMULATED` + EEPROM test config.
