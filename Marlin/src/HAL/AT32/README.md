# AT32F403 HAL for Marlin

This HAL provides support for Artery Technology **AT32F403-series** ARM Cortex-M4 microcontrollers. It was added to satisfy feature requests #24894 (Wanhao 0327001 v2.0 / AT32F403ARGT7) and #25952 (EasyThreed K7 ET4000+ / AT32F403ARCT7).

The HAL is modeled on the [[hal-hc32|HC32 HAL]] pattern: a Chinese-vendor Cortex-M4 MCU with its own Arduino core (not stm32duino), so only the Cortex-M4 core / SysTick / NVIC / FPU are binary-compatible with STM32 — the entire peripheral layer differs.

## Supported Boards

| Board | MCU | Flash | PlatformIO env | Pins file |
| --- | --- | --- | --- | --- |
| `BOARD_WANHAO_0327001` (8000) | AT32F403ARGT7 | 256 KB | `WANHAO_0327001` | `src/pins/at32f4/pins_WANHAO_0327001.h` |
| `BOARD_EASYTHREED_ET4000_PLUS` (8001) | AT32F403ARCT7 | 1 MB | `EASYTHREED_ET4000_PLUS` | `src/pins/at32f4/pins_EASYTHREED_ET4000_PLUS.h` |

**Build status:** both targets compile and link cleanly via `mftest` (see [[at32-build-notes]] and `AGENTS.md` in this folder).

## Why a New HAL (not STM32 reuse)

The AT32F403 is **not register-compatible** with STM32F4 despite sharing the Cortex-M4 core:

| Component | STM32F4 | AT32F403 | Compatible? |
| --- | --- | --- | --- |
| Clock module | `RCC` | `CRM` | no — different registers/bits/PLL ranges |
| GPIO alt-func | `AFR[0/1]` | `AFR1/AFR2` | no — different naming/indices |
| Internal osc | 16 MHz HSI | 48 MHz HICK | no — different frequency |
| PLL config | ST-specific | `CRM_PLL_OUTPUT_RANGE_*` | no — AT32-unique |
| Unique periph | — | ACC (auto clock cal) | no — no STM32 equivalent |
| Flash | Single bank | Dual bank (256+768 KB) | differs |

Arduino cores (`rizacelik/ArduinoCore-AT32F4`, derived from `WeActStudio/ArduinoCore-AT32F4`) wrap Artery's own Standard Peripheral Library, not stm32duino, so there is no AT32 support in `Arduino_Core_STM32`.

## Arduino Core Dependency

Uses `rizacelik/ArduinoCore-AT32F4` v1.0.0 (based on WeActStudio's core). Key core facts driving the HAL:

- Header chain `Arduino.h` → `libcore/mcu_core.h` → `adc.h`, `delay.h`, `exti.h`, `gpio.h`, `pwm.h`, `timer.h`, `wdg.h`.
- `PIN_MAP[pin]` → `{ GPIOx, TIMx, ADCx, GPIO_Pin_x, TimerChannel, ADC_Channel }`; `IS_PIN` / `IS_ADC_PIN` / `IS_PWM_PIN`.
- `F_CPU = system_core_clock` (set at runtime) — build forces `-DF_CPU=240000000L` for `constexpr` math.
- **The core defines pin names (`PC6`, `PC7`, …) as a C `enum`, not `#define`.** The C preprocessor cannot see `enum` values in `#if`, so Marlin's `PIN_EXISTS(PC6)` evaluated `PC6` as `0` and produced false pin-conflict errors. The fix lives in `HAL/AT32/pins_arduino.h` (and the `marlin_at32f403` variant's `pins_arduino.h`): they provide `#define PC6 PC6` (etc.) so the preprocessor sees real numeric values.
- The core's `HardwareSerial` is **NOT** derived from Arduino `Stream` (no `flush()` / `end()` / `peek()`); `AT32SerialAdapter` wraps it. The core also does not define a bare `Serial` global (only `Serial1`–`Serial5`), so `HardwareSerial.cpp` provides `HardwareSerial& Serial = Serial1;` plus a `println(const char*)` for Marlin feature code.
- No core `Servo` class, no panic/postmortem — `MinSerial` uses raw USART writes.
- USB-CDC is **not** used by Marlin (it uses hardware UART). All USB-CDC code in the core is guarded behind `#ifdef AT32_USB_CDC` (undefined for Marlin).

## PlatformIO Integration

A local PlatformIO platform is scaffolded at `buildroot/share/PlatformIO/platform-at32/` (referenced via `file://` in `ini/at32.ini` to avoid SCons path-doubling). Board JSONs: `at32f403argt7.json` (Wanhao, 256 KB), `at32f403arct7.json` (EasyThreed, 1 MB). Variant: `marlin_at32f403` (generated at build time by `generic_create_variant.py`). Build flags add `-DARDUINO_ARCH_AT32`, `-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16`.

Build/test commands:

```bash
cd $MARLIN_REPO
buildroot/bin/mftest -t WANHAO_0327001 -n1 -y
buildroot/bin/mftest -t EASYTHREED_ET4000_PLUS -n1 -y
# or directly:
pio run -e WANHAO_0327001
pio run -e EASYTHREED_ET4000_PLUS
```

> Note: `pio run -e <env>` resolves the board from the generated `Marlin/Configuration.h`. If you switch envs with raw `pio`, an outdated generated `Configuration.h` can cause a spurious "incompatible with BOARD_…" preflight error. `mftest` regenerates the config correctly per target, so prefer `mftest` for HAL validation.

The vendored framework (`buildroot/share/PlatformIO/framework-arduino-at32f4/`) and platform carry the integration fixes needed to build Marlin (optimization flags, hard-float link flags, peripheral-driver sources, clock config, 256 KB linker script, USB-CDC guards). See `AGENTS.md` in this folder for the full list and the integration gotchas.

## Implementation Status

Fully compiling for both boards. Key subsystems:

- Core HAL (`HAL.h` / `MarlinHAL`), timers (TMR2 stepper / TMR4 temp @ 240 MHz), fast GPIO, serial (`AT32SerialAdapter`), servo (via `PWM_Init`/`PWM_Write`), PWM, ADC, endstop interrupts, SPI, SoC temp sensor, M43 pin debugging.
- EEPROM: `SDCARD_EEPROM_EMULATION` is enabled per-board in the pins files; the `PersistentStore` SD backend is implemented in `eeprom/eeprom_sdcard.cpp`.
- SD card: supported via the hardware-SPI path (`HAL_SPI.cpp`).
- u8g display support: software-SPI (`u8g/u8g_com_HAL_AT32_sw_spi.cpp`) — needs validation.

## Open / Placeholder Items

- Pin mappings in `src/pins/at32f4/pins_*.h` are **placeholders** — awaiting schematic verification.
- `get_reset_source()` is simplified (always power-on); `idletask()` is a no-op.
- u8g software-SPI display path needs runtime validation on real hardware.

## References

- AT32F403 datasheet: https://www.arterytek.com/en/product/AT32F403.html
- ArduinoCore-AT32F4 (rizacelik): https://github.com/rizacelik/ArduinoCore-AT32F4
- Issue #24894: Wanhao 0327001 support
- Issue #25952: EasyThreed ET4000+ support
- Wiki: [[hal-at32]], [[at32-build-notes]]

## License

This HAL follows the same license as the rest of Marlin (GPLv3).
