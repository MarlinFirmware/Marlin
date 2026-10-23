# AGENTS.md — NATIVE_SIM HAL

Working notes and conventions for the **NATIVE_SIM HAL** folder (`Marlin/src/HAL/NATIVE_SIM/`).

## Build & Test Loop

NATIVE_SIM is a **native host simulator with a GUI** — there is no MCU, no Arduino framework, and no embedded toolchain. Marlin compiles as a regular C++ program (`platform = native`, `ini/native.ini`, `[simulator_common]`) that renders a printer UI on the host using SDL2 / SDL2_net / OpenGL / GLM. Always validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t simulator_linux_release -n1 -y   # Linux GUI simulator (primary)
buildroot/bin/mftest -t simulator_linux_debug   -n1 -y   # Linux GUI simulator, debug build
# On macOS:
buildroot/bin/mftest -t simulator_macos_release  -n1 -y
# On Windows (MSYS2):
buildroot/bin/mftest -t simulator_windows        -n1 -y
```

The `simulator_linux_release` target (`buildroot/tests/simulator_linux_release/config-01.ini` → `ini_use_config = example/Simulator`, `BOARD_SIMULATED`) is the canonical smoke target and must be green before considering NATIVE_SIM work done.

> `mftest` only **builds** the simulator — it does not launch it. The produced binary (`MarlinSimulator`, see below) needs a graphical session (X11 / macOS window server) to actually run, so a build that links clean here can still require a desktop to exercise the UI. Don't expect `mftest` to "run" the firmware.
>
> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` (removes only build artifacts) or just let `mftest` rebuild.

## Framework / Platform Integration (native, GUI — distinct from LINUX)

This is a `platform = native` build, but unlike the **LINUX** HAL (a headless console app that talks over stdin/stdout), NATIVE_SIM is the **graphical** simulator. The HAL is selected by `__PLAT_NATIVE_SIM__` (`HAL/platforms.h` maps `__PLAT_NATIVE_SIM__` → `src/HAL/NATIVE_SIM/`); it has no `ARDUINO_ARCH_*` define. There is **no vendored Arduino core, no MCU SDK** — the only "platform" is the host C++ compiler (`-std=gnu++17`).

The native GUI / Arduino-shim headers are **not** in this folder and **not** in `HAL/LINUX/include`. They are pulled in at build time from the `MarlinSimUI` library dependency (a pinned GitHub zip in `[simulator_common]` `lib_deps`, plus `Adafruit NeoPixel` and `LiquidCrystal` zips). `MarlinSimUI` supplies `Arduino.h`, `serial.h`, `pinmapping.h`, and the `Gpio` class — so `HAL.h`'s `#include "serial.h"` and `fastio.h`'s `#include <pinmapping.h>` resolve against _that_ library, not local or LINUX files. The `MarlinSimulator` program entry point (`main()`) also lives in `MarlinSimUI`, **not** in `src/HAL/NATIVE_SIM/` — there is no `main.cpp` here.

The build is wired by:

- `[simulator_common]` (`ini/native.ini`): `platform = native`, `-D__PLAT_NATIVE_SIM__ -DU8G_HAL_LINKS -DGLM_ENABLE_EXPERIMENTAL`, `build_src_filter = ... +<src/HAL/NATIVE_SIM>`, `lib_deps` (MarlinSimUI + others), and `pre:buildroot/share/PlatformIO/scripts/simulator.py`.
- `simulator.py`: renames the binary to `MarlinSimulator`, and on macOS **fails the build** if `/usr/bin/gcc` (Apple clang) is the compiler — it requires a real GCC from MacPorts/Homebrew (`gcc14`) plus Mesa/SDL2.

Required host libraries (from `native.ini` comments):

- Linux: `libsdl2-dev`, `libsdl2-net-dev`, `libglm-dev` (and `python3-venv`, `build-essential`).
- macOS: MacPorts or Homebrew `gcc@14`, `glm`, `mesa`, `sdl2`, `sdl2_net`.
- Windows: MSYS2 `mingw-w64-x86_64` toolchain + `glm`, `SDL2`, `SDL2_net`, `dlfcn`.

### Gotchas that are not obvious from the source

1. **No MCU, no real pins — GPIO is a software array.** `fastio.h` macros (`READ_PIN`, `WRITE_PIN`, `SET_DIR_*`, `SET_MODE`) all delegate to a `Gpio` class that operates on an in-memory `pin_map[256]` (`Gpio::pin_count = 255`, matching the LINUX HAL model). There are no physical lines; `PWM_PIN`/`INTERRUPT_PIN` effectively never fire. Do not add hardware-specific pin logic here — this is the same virtual-pin model as the LINUX HAL, just with a GUI on top.

2. **No real interrupts / ISRs.** `CRITICAL_SECTION_START/END` are empty, `isr_on/off` are no-ops, and `DELAY_CYCLES` is a software delay. Endstop/timer "hardware" events are driven by the simulator's own loop (MarlinSimUI), not by vector interrupts. Don't write ISR-dependent timing assumptions into HAL code.

3. **Serial is virtual over the host process.** `HAL.h` declares `MSerialT serial_stream_0..3` (`SERIAL_INDEX_MIN/MAX` 0..3) and `serial.h` (from MarlinSimUI) defines `MSerialT` as `Serial1Class<HalSerial>` — ring-buffer-backed virtual ports. G-code is exchanged with the `MarlinSimulator` process's stdin/stdout, not a UART/USB device. There is no `Serial` USB-CDC object.

4. **Arduino/serial/pinmapping/Gpio headers come from MarlinSimUI, not this folder.** A HAL change that _looks_ like a missing local header (e.g. `serial.h`, `pinmapping.h`, `Gpio.h`) is actually supplied by the downloaded library dependency. If a build fails with "header not found", suspect a `lib_deps` fetch failure or a `simulator.py`/GCC mismatch — not a missing file in `src/HAL/NATIVE_SIM/`.

5. **The GUI entry point is external.** There is no `main.cpp` in this HAL. `MarlinSimUI` provides `main()` and the SDL2 event/render loop; `simulator.py` sets `PROGNAME = "MarlinSimulator"`. HAL code here runs _inside_ that host GUI program — don't add process entry points or assume `setup()`/`loop()` are the only callers.

6. **macOS requires real GCC, not Apple clang.** `simulator.py` hard-fails when `/usr/bin/gcc` (which is clang) is on PATH; you must `port install gcc14 ...` or `brew install gcc@14 ...` and symlink `gcc`/`g++` to `gcc-14`/`g++-14`. Linux/Windows use their respective native GCC/MinGW toolchains. A "can't build on macOS" symptom is almost always this GCC-vs-clang issue, not a HAL bug.

7. **`NO_COMPILE_TIME_PWM` is set in `fastio.h`.** PWM duty is computed at runtime via `MarlinHAL::set_pwm_duty()` (`map(v, 0, v_size, 0, UINT16_MAX)` → `analogWrite`), so there are no compile-time PWM pin tables. Don't add `PWM`/compile-time duty logic.

8. **Display layer is software-SPI u8g, not hardware.** `u8g/u8g_com_sw_spi.cpp`, `u8g_com_st7920_sw_spi.cpp`, and the `LCD_*` routines implement bit-banged SPI that the simulator renders into its GUI window. The `build_flags` carry `-DU8G_HAL_LINKS`; the drawing ultimately goes through U8glib-HAL in the MarlinSimUI host. Treat these as rendering shims, not real bus drivers.

9. **No local `eeprom.cpp` / `main.cpp`.** Unlike the LINUX HAL (which has its own `eeprom.cpp` writing `eeprom.dat`), NATIVE_SIM has no EEPROM file in this folder — persistence uses the shared `HAL/shared` EEPROM API mediated by the simulator host. A "missing eeprom" symptom points at the shared API or MarlinSimUI, not a missing `src/HAL/NATIVE_SIM` file.

10. **`freeMemory()` is a stub.** Per `HAL.h`, `MarlinHAL::freeMemory()` and `::freeMemory()` are placeholders (no meaningful host heap figure). Don't key any logic off the returned value.

## Conventions

- All HAL source is guarded by `#ifdef __PLAT_NATIVE_SIM__` (not `ARDUINO_ARCH_*`). Every `.cpp` wraps its body in this guard (see `HAL.cpp`, `pinsDebug.cpp`, `Servo.cpp`).
- Keep HAL changes in `Marlin/src/HAL/NATIVE_SIM/` and its `u8g/`, `tft/`, `inc/` subfolders. Board pin layout is `Marlin/src/pins/native/pins_RAMPS_NATIVE.h` (`BOARD_SIMULATED`, a RAMPS-like map) — not in this folder.
- This is the **GUI** simulator; the **LINUX** HAL (`src/HAL/LINUX/`, `linux_native` env) is the separate headless console build. Do not conflate the two. Both share the same virtual-`Gpio` pin model, so a pin/simulation fix may need to be mirrored in the LINUX HAL too.
- External dependencies (`MarlinSimUI`, `Adafruit NeoPixel`, `LiquidCrystal`) are pinned git-zip `lib_deps`; if you change a HAL API they call, bump/verify the library version and re-test the build.
- When you change HAL code, run `simulator_linux_release` (and the platform you develop on) and verify green before committing.

## Related

- `Marlin/src/HAL/AT32/AGENTS.md` — the precedent for this file's structure.
- `Marlin/src/HAL/LINUX/AGENTS.md` — the sibling headless console simulator (shared virtual-`Gpio` model, distinct build).
- Wiki: [[hal-native-sim]], [[marlin-src-hal]], [[hal-linux]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers, `Marduino.h`) reused by this HAL.
- `ini/native.ini` — `[simulator_common]` / `simulator_linux*` / `simulator_macos*` / `simulator_windows` env definitions.
- `buildroot/tests/simulator_linux_release/config-01.ini` — the `BOARD_SIMULATED`
  - `example/Simulator` test config.
- `buildroot/share/PlatformIO/scripts/simulator.py` — pre-build script (binary rename, macOS GCC/OpenGL checks).
