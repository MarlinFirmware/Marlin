# AGENTS.md — HAL folder (Hardware Abstraction Layer)

Umbrella documentation for the entire `Marlin/src/HAL/` folder — the top-level HAL directory, **not** any single family subfolder. Per-family working notes live in each `Marlin/src/HAL/<FAMILY>/AGENTS.md` (e.g. `Marlin/src/HAL/STM32/AGENTS.md`). This file describes what the folder contains and how the HAL participates in Marlin's `inc/` include cascade and `ini` build-time source selection.

## What this folder contains

`Marlin/src/HAL/` is the root of Marlin's hardware abstraction layer. At the top level:

- **`HAL.h`** — the HAL dispatcher / entry point. It `#include "platforms.h"`, then pulls in the _active_ family's `HAL.h` via `#include HAL_PATH(.., HAL.h)` and declares the global `extern MarlinHAL hal;`. Defines a few cross-platform helpers (`HAL_ADC_RANGE`, `I2C_ADDRESS`, `S_FMT`, `PGMSTR`). See "Where the HAL sits in the inc/ include ordering" for how/when it is actually included into a translation unit.
- **`platforms.h`** — maps a compiler/board guard macro to a family folder. It defines the `HAL_PATH(PATH, NAME)` macro to stringize `PATH/HAL/<FAMILY>/NAME` for the currently-selected platform (see "HAL selection (platforms.h / guard macros)").
- **`shared/`** — cross-HAL APIs and helpers reused by every family: `eeprom_api.{h,cpp}`, `HAL_SPI.h`, `serial_ports.h`, `servo.{h,cpp}`, `servo_private.h`, `Marduino.h`, `math_32bit.h`, `progmem.h`, `Delay.{h,cpp}`, `MinSerial.{h,cpp}`, `fauxpins.h`, `HAL_ST7920.h`, `esp_wifi.{h,cpp}`, `eeprom_if*.{h,cpp}`, plus the `backtrace/` and `cpu_exception/` subfolders. Family HALs include these rather than reimplementing drivers.
- **`<FAMILY>/`** — one implementation folder per microcontroller family. Each contains that family's `HAL.h`/`HAL.cpp`, `MarlinHAL` subclass, timer/serial/SPI/fastio/servo code, an `inc/` subfolder (family-local `Conditionals*` and `SanityCheck.h`), and often an `eeprom/`, `u8g/`, and `tft/` subfolder.

The current family folders are:

```
AT32/  AVR/  DUE/  ESP32/  GD32_MFL/  HC32/  LINUX/  LPC1768/
NATIVE_SIM/  RP2040/  SAMD21/  SAMD51/  STM32/  STM32F1/
TEENSY31_32/  TEENSY35_36/  TEENSY40_41/
```

There is **no** `Marlin/src/HAL/inc/` subfolder at the HAL top level. Family-local `inc/` directories live _inside each family folder_ (e.g. `HAL/STM32/inc/SanityCheck.h`, `HAL/AT32/inc/Conditionals_adv.h`). The top-level shared include cascade lives in `Marlin/src/inc/`.

## HAL selection (platforms.h / guard macros)

`platforms.h` contains one `#if`/`#elif` chain keyed on compiler- or board-defined guard macros. The first matching branch defines `HAL_PATH(PATH, NAME)` to expand to `PATH/HAL/<FAMILY>/NAME`. The macros and their resolved folders (verified from `platforms.h`):

| Guard macro                       | Family folder                          |
| --------------------------------- | -------------------------------------- |
| `__AVR__`                         | `HAL/AVR`                              |
| `ARDUINO_ARCH_SAM`                | `HAL/DUE`                              |
| `__MK20DX256__`                   | `HAL/TEENSY31_32`                      |
| `__MK64FX512__` / `__MK66FX1M0__` | `HAL/TEENSY35_36`                      |
| `__IMXRT1062__`                   | `HAL/TEENSY40_41`                      |
| `TARGET_LPC1768`                  | `HAL/LPC1768`                          |
| `ARDUINO_ARCH_HC32`               | `HAL/HC32`                             |
| `ARDUINO_ARCH_MFL`                | `HAL/GD32_MFL`                         |
| `__STM32F1__` / `TARGET_STM32F1`  | `HAL/STM32F1`                          |
| `ARDUINO_ARCH_STM32`              | `HAL/STM32` (also `#define HAL_STM32`) |
| `ARDUINO_ARCH_ESP32`              | `HAL/ESP32`                            |
| `__PLAT_LINUX__`                  | `HAL/LINUX`                            |
| `__PLAT_NATIVE_SIM__`             | `HAL/NATIVE_SIM`                       |
| `__SAMD51__`                      | `HAL/SAMD51`                           |
| `__SAMD21__`                      | `HAL/SAMD21`                           |
| `__PLAT_RP2040__`                 | `HAL/RP2040`                           |
| `ARDUINO_ARCH_AT32`               | `HAL/AT32`                             |

If none match, `platforms.h` emits `#error "Unsupported Platform!"`. The guard macros are supplied by the Arduino core / toolchain / board JSON for the active environment, so the _compiler_ (not the build script) decides which family `HAL_PATH` points at.

> Note: `HAL_PATH` is a stringizing macro, so every "include the family file" line in the cascade uses `HAL_PATH(.., <name>)` rather than a literal path. The `..` argument is the relative path back to `Marlin/src/` from inside `Marlin/src/HAL/`.

## Build-time inclusion (ini build_src_filter)

The PlatformIO `default_src_filter` (defined in `platformio.ini`'s `[common]` section) starts from `+<src/*>` and then **excludes the whole HAL tree**:

```
default_src_filter = +<src/*> -<src/config> -<src/tests>
  ...
  ; Marlin HAL
  -<src/HAL>
  +<src/HAL/shared>
  -<src/HAL/shared/backtrace>
  -<src/HAL/shared/cpu_exception>
  -<src/HAL/shared/eeprom_if_i2c.cpp>
  -<src/HAL/shared/eeprom_if_spi.cpp>
  ...
```

So by default _no_ family folder is compiled (only `HAL/shared/` and a few named shared files survive). Each family's board environment re-enables exactly one family folder via its `build_src_filter`, e.g.:

- `ini/at32.ini` (`[AT32F403_base]`): `build_src_filter = ${common.default_src_filter} +<src/HAL/AT32>`
- `ini/avr.ini`: `+<src/HAL/AVR>`
- `ini/raspberrypi.ini`: `+<src/HAL/RP2040>`
- `ini/esp32.ini`: `+<src/HAL/ESP32>`
- `ini/lpc176x.ini`: `+<src/HAL/LPC1768> -<src/HAL/LPC1768/tft> +<src/HAL/shared/backtrace>`
- `ini/stm32f1-maple.ini`: `+<src/HAL/STM32F1> -<src/HAL/STM32F1/tft>`

Some environments additionally enable `src/HAL/shared/backtrace` or prune per-board subtrees (e.g. `-<src/HAL/LPC1768/tft>`, re-added selectively through `custom_marlin.*` switches). The net effect: at build time, only the _one active_ family folder plus the shared helpers are compiled — even though all families ship in the source tree.

## Layering: HAL vs pins vs boards

Three layers cooperate to bind a binary to a specific board:

1. **`Marlin/src/core/boards.h`** — selects the board. The active `MOTHERBOARD` define (from `Configuration.h` / `Config.h`) maps to a board header under `Marlin/src/pins/<family>/`, which in turn supplies `pins_arduino.h` (or equivalent) pin macros and the family's `HAL_PATH`-relevant defines.
2. **`Marlin/src/pins/<family>/`** — board pin maps (logical pin → MCU pin) for one family. These are data only; they do not implement behavior.
3. **`Marlin/src/HAL/<FAMILY>/`** — the _implementation_: timers, serial, SPI, fastio, EEPROM backends, `MarlinHAL` subclass, etc. This is the only layer that contains executable HAL logic.

The compiler guard macro in `platforms.h` (section above) decides which `HAL/<FAMILY>` directory the `HAL_PATH` macro resolves to; `core/boards.h` + `pins/` decide which pin table and which `MOTHERBOARD` apply; and `ini/*.ini` `build_src_filter` decides which family's `.cpp` files are actually passed to the compiler. They must agree for a build to be coherent.

## Where the HAL sits in the inc/ include ordering

Every Marlin translation unit includes `Marlin/src/inc/MarlinConfig.h` (or, for dependency-only scans, `MarlinConfigPre.h`). The cascade below is traced from the real files (`MarlinConfig.h`, `MarlinConfigPre.h`, `MarlinConfigPre-5-post.h`, `MarlinConfigPre-6-type.h`, `Conditionals-*`).

### Net ordered cascade

```
MarlinConfig.h
└─ Conditionals-6-type.h
   └─ MarlinConfigPre-6-type.h
      ├─ Conditionals-5-post.h
      │  ├─ MarlinConfigPre.h
      │  │  ├─ Conditionals-1-axes.h
      │  │  │  └─ MarlinConfigPre-1-axes.h
      │  │  │     ├─ Config.h
      │  │  │     ├─ macros.h
      │  │  │     ├─ core/boards.h            ← board selection
      │  │  │     ├─ Configuration.h (if not Config.h)
      │  │  │     ├─ HAL/platforms.h          ← defines HAL_PATH macro + family
      │  │  │     └─ Version.h
      │  │  ├─ Conditionals-2-LCD.h
      │  │  ├─ Conditionals-3-etc.h
      │  │  ├─ Conditionals-4-adv.h
      │  │  │  └─ MarlinConfigPre-4-adv.h
      │  │  │     ├─ Conditionals-3-etc.h (re-included)
      │  │  │     ├─ drivers.h
      │  │  │     └─ Configuration_adv.h
      │  │  └─ HAL_PATH(.., inc/Conditionals_adv.h)   ← FAMILY conditionals
      │  └─ (rest of Conditionals-5-post.h)
      │  ├─ HAL_PATH(.., inc/Conditionals_post.h)     ← FAMILY post-conditionals
      │  └─ core/types.h
      └─ MarlinConfigPre-5-post.h
         ├─ ../HAL/HAL.h          ← HAL DISPATCHER (pulls family HAL.h via HAL_PATH;
         │                            declares `extern MarlinHAL hal;`)
         ├─ ../pins/pins.h        ← board pin table
         ├─ HAL_PATH(.., timers.h)
         └─ HAL_PATH(.., spi_pins.h)
   (type-level conditionals defined in Conditionals-6-type.h)
MarlinConfig.h (resumes)
├─ HAL_PATH(.., inc/Conditionals_type.h)   ← FAMILY type conditionals
├─ Changes.h
├─ core/language.h, core/utility.h, core/mstring.h, core/serial.h, core/endianness.h
├─ MarlinCore.h
├─ SanityCheck.h                          ← TOP-LEVEL src/inc/SanityCheck.h
└─ HAL_PATH(.., inc/SanityCheck.h)        ← FAMILY sanity checks
```

Key points:

- **`HAL/platforms.h` is pulled in early** — inside `MarlinConfigPre-1-axes.h`, before any Conditionals run — so the `HAL_PATH` macro and the selected family are defined before anything else needs them.
- **`HAL.h` (the dispatcher) is included mid-cascade**, from `Marlin/src/inc/MarlinConfigPre-5-post.h` via `#include "../HAL/HAL.h"` (or `HAL/shared/fauxpins.h` when building with `__MARLIN_DEPS__`). This is the point where the family's own `HAL.h` (via `HAL_PATH(.., HAL.h)`) and the `MarlinHAL hal` instance become visible.
- **`pins.h` and the family `timers.h` / `spi_pins.h` are included immediately after `HAL.h`** in the same file, so pin and timer configuration are available before any module source compiles.
- **Family-local `inc/` files are included through `HAL_PATH(.., inc/<NAME>.h)`, never by literal path.** The top-level cascade includes the family's `inc/Conditionals_adv.h` (from `MarlinConfigPre.h`), `inc/Conditionals_post.h` (from `MarlinConfigPre-6-type.h`), and `inc/Conditionals_type.h` plus `inc/SanityCheck.h` (both from `MarlinConfig.h`). Each family provides whichever of these files it needs; missing ones simply aren't compiled.

### How the family SanityCheck is reached

`Marlin/src/inc/MarlinConfig.h` ends the cascade with (inside `#ifndef __MARLIN_DEPS__`):

```cpp
#include "SanityCheck.h"                 // Marlin/src/inc/SanityCheck.h (top-level)
#include HAL_PATH(.., inc/SanityCheck.h) // e.g. Marlin/src/HAL/STM32/inc/SanityCheck.h
```

So the _family_ sanity checks run **last**, after the top-level `SanityCheck.h` and after all `Conditionals-*`. The family file guards itself with `#pragma once` and family-specific `#ifdef` checks (e.g. the STM32/AT32 files open with `#ifndef PA0 #error "Your ARM platform pins are not defined as macros, only as enums!"`, forcing the board pin file to re-expose pins as numeric `#define`s). The top-level `src/inc/SanityCheck.h` may also reference HAL-specific macros and, where it does, point the reader to `HAL/<FAMILY>/inc/SanityCheck.h` for the platform-specific rule.

## Conventions

- The HAL top level is deliberately thin: `HAL.h` + `platforms.h` + `shared/` + the family folders. Do not add cross-family logic at the top level — put shared code in `HAL/shared/` and family-specific code in `HAL/<FAMILY>/`.
- Edit `Marlin/src/inc/*` (Conditionals/SanityCheck) only for _cross-family_ rules. Platform-specific checks belong in `HAL/<FAMILY>/inc/`.
- Keep board pin tables in `Marlin/src/pins/<family>/`; keep HAL behavior in `Marlin/src/HAL/<FAMILY>/`.
- When adding a new family, you must (a) add a branch to `platforms.h`, (b) create a `HAL/<FAMILY>/` tree with its own `HAL.h`/`MarlinHAL` and `inc/` files, and (c) add a `build_src_filter = ${common.default_src_filter} +<src/HAL/<FAMILY>>` to the family's `ini/*.ini` environment.

## Related

- Per-family docs: `Marlin/src/HAL/<FAMILY>/AGENTS.md` (e.g. `STM32/`, `AT32/`, `AVR/`, `ESP32/`, `RP2040/`, `LPC1768/`, `LINUX/`, `NATIVE_SIM/`, …).
- `Marlin/src/inc/MarlinConfig.h`, `MarlinConfigPre.h` — the include cascade.
- `Marlin/src/core/boards.h` — board selection; `Marlin/src/pins/` — board pin maps.
- `Marlin/src/HAL/shared/` — shared HAL APIs every family reuses.
- `platformio.ini` (`[common]` `default_src_filter`) and `ini/*.ini` — build-time family selection via `build_src_filter`.
- Wiki: [[marlin-src-hal]], [[hal-selection]], [[marlin-config-cascade]].
