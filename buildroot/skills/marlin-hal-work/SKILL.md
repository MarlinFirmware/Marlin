---
name: marlin-hal-work
description: Use when adding a board to an existing HAL, extending a HAL, creating a new HAL, or debugging HAL-specific code. Includes the board checklist, comparing HALs, and verifying framework APIs.
---

# Marlin HAL and Board Work

Each HAL folder `Marlin/src/HAL/<FAMILY>/` has its own `AGENTS.md` with the build and test loop and the gotchas for that family. Read it first.

Three levels of work, from most to least common:

1. **Add a board** to an existing HAL.
2. **Extend a HAL** with an MCU variant.
3. **Create a new HAL** for a new architecture.

## Compare, don't generalize

Don't assume one HAL's behavior holds for another. Timers, ISR entry, serial buffering, and ADC differ a lot. To learn the contract, compare three representatives: a simple one (AVR), a mid one (LPC1768), and a complex one (STM32 or ESP32).

    ls Marlin/src/HAL/*/                      # a file in 10+ HALs is probably required
    head -80 Marlin/src/HAL/{AVR,LPC1768,STM32,RP2040}/timers.h
    head -50 Marlin/src/HAL/{AVR,LPC1768,STM32,RP2040}/fastio.h

`Marlin/src/HAL/shared/` holds what every HAL inherits (`Marduino.h`, `serial_ports.h`, `eeprom_api.h`, ...). `Marlin/src/HAL/platforms.h` is the single dispatch point: a wrong detection macro loads the wrong HAL, and the failure is confusing.

## Adding a board

| Step | File | Action |
| --- | --- | --- |
| Pins | `Marlin/src/pins/<arch>/pins_<BOARD>.h` | All `*_PIN` defines. Use a `_common.h` file when variants share a pinout |
| Board ID | `Marlin/src/core/boards.h` | `#define BOARD_<NAME> <ID>` in the right architecture range, highest ID + 1 |
| Routing | `Marlin/src/pins/pins.h` | `#elif MB(<NAME>)` in the same relative position as in `boards.h` |
| Environment | `ini/<arch>.ini` | Add an `[env:...]` that `extends` a similar one |
| Validation | `Marlin/src/pins/<arch>/env_validate.h` | Only if the family has a unique constraint |
| CI test | `buildroot/tests/<env>/` | Optional |

Do not edit `config.ini` for a board.

Conventions:

- Letters between `pins_` and `.h` are UPPERCASE. The `_common` suffix is lowercase. Prefix the manufacturer's name if the board name isn't distinctive.
- The `#define` order in `boards.h` must match the `#elif MB()` order in `pins.h`.
- Set `BOARD_INFO_NAME` (and optionally `DEFAULT_MACHINE_NAME`) in the board's pins file.
- Use `#ifndef` guards around pins that variants may override.
- The EEPROM backend is chosen per board: most boards define `FLASH_EEPROM_EMULATION` or `SDCARD_EEPROM_EMULATION` in the pins file. If none is enabled you get an `undefined reference to PersistentStore::*` at link.
- Link the schematic or source of the pin data in a comment, if there is one.

### Verifying pin and UART assignments

Check open sources before saying a pin can't be verified without hardware. Cross-check at least two:

1. Klipper configs (`Klipper3d/klipper/config/printer-<vendor>-<model>*.cfg`, `generic-<board>.cfg`). They list step/dir/enable/heater/fan pins, and the MCU serial pins in comments.
2. Vendor or community Marlin forks: `pins_*.h`, the variant's `variant.h` and `PeripheralPins.c`, and the ini env's `build_flags`.
3. Elimination: a UART has only a few alternate-function pin pairs. If the others are used by steppers or heaters, the one left is the answer.

STM32duino uses `PIN_SERIALn_RX/TX` if defined. Otherwise it takes the first matching entry in the variant's pin map. A board whose UART is on a later pair needs `-DPIN_SERIALn_RX=...` and `-DPIN_SERIALn_TX=...` in its env.

### Validate with mftest

    buildroot/bin/opt_set MOTHERBOARD BOARD_<NAME>
    buildroot/bin/mftest -a -n1 -y

Raw `pio run -e <env>` reads whatever `Configuration.h` was last generated, and can report a phantom `Build environment is incompatible with BOARD_...`. Trust `mftest` and don't chase a pio-only "incompatible" error. A passing build only shows the files are structurally sound. Say so in the PR if no one has tested the board.

## Creating a new HAL

The minimum set that must exist to compile:

- A branch in `platforms.h`
- `HAL.h` and `HAL.cpp` (the `MarlinHAL` class)
- `fastio.h`
- `timers.h` and `timers.cpp`
- `MarlinSerial.h` and `MarlinSerial.cpp`
- `inc/Conditionals-2-LCD.h` and `inc/SanityCheck.h`, plus the other files in `inc/`, which can be empty (`#pragma once`) but must exist

Use a recent HAL such as HC32 as the layout reference. Everything else fills in as features are enabled.

## Verify against the real framework

Code written without the vendor framework in view tends to invent names borrowed from another platform (for example `GPIO_TypeDef`, `BSRR`, `Pin_TypeDef`). The installed framework (`~/.platformio/packages/framework-*`) is ground truth: grep it before writing or "fixing" any peripheral symbol. A symbol that isn't there is a HAL bug, not a missing dependency.

## Pitfalls

- A HAL header missing `#include "../../inc/MarlinConfig.h"` makes shared files report `FSTR_P`, `PGM_P`, `millis_t`, or `pin_t` undefined. Fix the one include, not the cascade. Files in `HAL/<ARCH>/<sub>/` need `../../../inc/MarlinConfig.h`.
- A timer ISR macro with no vector or NVIC wiring compiles but never fires.
- `MF_TIMER_STEP`, `MF_TIMER_TEMP`, and `MF_TIMER_PWM` must not collide with each other or with platform-reserved timers.
- `CRITICAL_SECTION_START/END` must nest (save and restore the previous interrupt state). Don't use bare `__disable_irq()` / `__enable_irq()`.
- `pin_t` must match the platform's GPIO numbering.
- `SERIAL_PORT` numbering differs between HALs (AVR 0-3, STM32 and RP2040 -1/0/1).
- A serial adapter class must subclass the real core serial class. Forward-declaring it gives "incomplete type" errors.
- Some cores lack the bare `Serial` global or `spiBegin()`, which Marlin code calls.
- Vendored framework files may use CRLF. Some editors and tools turn an escape such as `"\r\n"` into a real control character, so use hex constants like `0x0D`.
- Don't remove or break the AVR HAL. It is the reference implementation.
- Some boards must keep the base AVR flash and RAM budget. Don't let the base build grow.
