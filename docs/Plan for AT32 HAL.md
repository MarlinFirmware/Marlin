# Plan: New AT32 HAL for Marlin (Issues #24894 & #25952)

## Problem

Two feature requests ask for support of Artery Technology AT32F403-series ARM Cortex-M4 MCUs:

- **#24894**: Wanhao 0327001 v2.0 mainboard / AT32F403ARGT7
- **#25952**: EasyThreed K7 ET4000+ / AT32F403ARCT7

Neither MCU is currently supported in Marlin, and the existing STM32 HAL cannot be reused.

## Why a New HAL Is Required

The AT32F403 is **not register-compatible** with STM32F4 despite sharing the Cortex-M4 core:

| Component | STM32F4 | AT32F403 | Compatible? |
| --- | --- | --- | --- |
| Clock module | `RCC` | `CRM` | ❌ Different registers, bit fields, PLL ranges |
| GPIO alt-func | `AFR[0/1]` | `AFR1/AFR2` | ❌ Different naming/indices |
| Internal osc | 16 MHz HSI | 48 MHz HICK | ❌ Different frequency |
| PLL config | ST-specific | `CRM_PLL_OUTPUT_RANGE_*` | ❌ AT32-unique |
| Unique periph | — | ACC (auto clock cal) | ❌ No STM32 equivalent |
| Flash | Single bank | Dual bank (256+768 KB) | ⚠️ Different architecture |

Arduino cores exist (`WeActStudio/ArduinoCore-AT32F4`, `rizacelik/ArduinoCore-AT32F4`) but they wrap Artery's own Standard Peripheral Library — **not** stm32duino. No AT32 support exists in `Arduino_Core_STM32`.

## Approach

Create a new `HAL_AT32` directory modeled on the existing **HC32 HAL** (same pattern: Chinese vendor MCU with its own Arduino core). The Cortex-M4 core, SysTick, NVIC, and FPU are binary-compatible — only the peripheral abstraction layer differs.

## Implementation Steps

### Phase 1: HAL Skeleton (new files)

| # | Todo | Description |
| --- | --- | --- |
| 1 | **Creating HAL_AT32 directory structure** | Mirror HC32 HAL: `HAL.h`, `HAL.cpp`, `MarlinHAL.h/.cpp`, `MarlinSerial.h/.cpp`, `MinSerial.cpp`, `Servo.h/.cpp`, `fastio.h`, `pinsDebug.h`, `spi_pins.h`, `temp_soc.h`, `timers.h/.cpp`, `inc/` (Conditionals_*.h, SanityCheck.h), `eeprom/`, `sd/`, `u8g/` |
| 2 | **Adding AT32 detection to platforms.h** | Add `#elif defined(ARDUINO_ARCH_AT32)` (or similar) branch routing to `HAL/AT32/NAME` |
| 3 | **Writing HAL_AT32/HAL.h** | Core definitions: `CPU_32_BIT`, interrupt macros (`CRITICAL_SECTION_START/END`, `cli`/`sei`), `pin_t` type, ADC config, `MarlinHAL` class declaration, `flashFirmware()` |
| 4 | **Writing HAL_AT32/HAL.cpp** | HAL init, watchdog, reset source, `freeMemory()`, `idletask()`, `adc_result` variable |
| 5 | **Writing MarlinHAL.h/.cpp** | MarlinHAL class implementation: `init()`, `reboot()` (NVIC_SystemReset), ADC methods, PWM (`set_pwm_duty`, `set_pwm_frequency`) |
| 6 | **Writing timers.h/.cpp** | Timer-based PWM and SYSTICK millis counter |
| 7 | **Writing fastio.h** | Fast GPIO macros for AT32F403 |
| 8 | **Writing MarlinSerial.h/.cpp, MinSerial.cpp** | Serial port abstraction |
| 9 | **Writing Servo.h/.cpp** | Servo library integration |
| 10 | **Writing temp_soc.h** | SoC temperature sensor support |
| 11 | **Writing pinsDebug.h, spi_pins.h** | Pin debugging and SPI pin definitions |

### Phase 2: Board Support

| # | Todo | Description |
| --- | --- | --- |
| 12 | **Adding AT32 board definitions to boards.h** | Define `BOARD_WANHAO_0327001` and `BOARD_EASYTHREED_ET4000_PLUS` (or similar) with numeric IDs in the 8000+ range |
| 13 | **Creating AT32 pin files** | `Marlin/src/pins/at32f4/` with `pins_WANHAO_0327001.h` and `pins_EASYTHREED_ET4000_PLUS.h`, registered in `pins.h` |
| 14 | **Writing HAL_AT32/inc/SanityCheck.h** | Compile-time validation (core version, feature compatibility) |
| 15 | **Writing HAL_AT32/inc/Conditionals\_*.h** | Conditional defines specific to AT32 hardware capabilities |
| 16 | **Writing HAL_AT32/README.md** | Documentation on supported boards, Arduino core dependency, PlatformIO environment |

### Phase 3: Integration & Validation

| # | Todo | Description |
| --- | --- | --- |
| 17 | **Updating Marlin src/inc/SanityCheck.h** | Add `ARDUINO_ARCH_AT32` to feature-gate checks (e.g., serial stats, TFT support) |
| 18 | **Updating shared files** | Ensure `HAL/shared/servo.h`, `MarlinCore.cpp`, and any other shared files handle AT32 correctly |
| 19 | **Creating PlatformIO environment** | `platformio.ini` entries for AT32 builds (depend on `rizacelik/ArduinoCore-AT32F4` or similar) |
| 20 | **Testing build** | Verify firmware compiles for target boards with no errors |

## Dependencies

- Todo 2 (platforms.h) must complete before any HAL_AT32 files can be selected
- Todos 3–11 (HAL skeleton) are partially parallelizable but HAL.h must come first
- Todos 12–16 (board support) depend on HAL skeleton being in place
- Todo 20 (build test) depends on all prior phases

## Open Questions

1. **Arduino core to target**: ✅ Resolved — Using `rizacelik/ArduinoCore-AT32F4` v1.0.0
2. **Preprocessor define**: ✅ Resolved — Core does NOT define `ARDUINO_ARCH_AT32`; added `-DARDUINO_ARCH_AT32` to build flags
3. **Board IDs**: ✅ Resolved — 8000 (`WANHAO_0327001`) and 8001 (`EASYTHREED_ET4000_PLUS`)
4. **PlatformIO platform**: ✅ Resolved — Created local platform at `buildroot/share/PlatformIO/platform-at32/`
5. **Pin mappings**: ⚠️ Still open — Placeholder assignments; need schematic verification

---

## Klipper Research Findings (CrealityOfficial/K2_Series_Klipper)

No official Klipper board support exists for Wanhao 0327001 or EasyThreed ET4000+, but AT32F403A HAL files confirm all key hardware details:

### Clock

- HEXT 8 MHz → /2 → ×60 PLL → **240 MHz SYSCLK**
- AHB /1 (240 MHz), APB1 /2 (120 MHz), APB2 /2 (120 MHz)
- `F_CPU = system_core_clock` (set at runtime, not compile time)
- Timer clock max = APB1 × 2 = **240 MHz**

### GPIO (`src/at32/gpio.c`)

| AT32 register   | Function         | STM32 equivalent |
| --------------- | ---------------- | ---------------- |
| `gpio_x->scr`   | Set bits         | `GPIOx->BSRR`    |
| `gpio_x->clr`   | Clear bits       | `GPIOx->BRR`     |
| `gpio_x->odt`   | Output data      | `GPIOx->ODR`     |
| `gpio_x->idt`   | Input data       | `GPIOx->IDR`     |
| `gpio_x->cfglr` | Config pins 0–7  | `GPIOx->CRL`     |
| `gpio_x->cfghr` | Config pins 8–15 | `GPIOx->CRH`     |

### Timers (`src/at32/hard_pwm.c`)

| AT32 register              | Function              | STM32 equivalent    |
| -------------------------- | --------------------- | ------------------- |
| `tmr_x->div`               | Prescaler             | `TIMx->PSC`         |
| `tmr_x->pr`                | Auto-reload           | `TIMx->ARR`         |
| `tmr_x->cval`              | Counter value         | `TIMx->CNT`         |
| `tmr_x->c1dt`–`c4dt`       | Capture/Compare 1–4   | `TIMx->CCR1`–`CCR4` |
| `tmr_x->swevt_bit.ovfswtr` | Generate update event | `TIMx->EGR.UG`      |

### ADC (`src/at32/adc.c`)

- Clock: SYSCLK/6 = **40 MHz**
- Sample time: `ADC_SAMPLETIME_239_5` cycles
- 16 channels on ADC1

---

## Arduino Core API Summary (`rizacelik/ArduinoCore-AT32F4` v1.0.0)

Downloaded and inspected the actual core zip. Key facts for HAL implementation:

### Header chain

- `Arduino.h` → `libcore/mcu_core.h` → includes: `adc.h`, `delay.h`, `exti.h`, `gpio.h`, `pwm.h`, `timer.h`, `wdg.h`
- `libcore/mcu_type.h` defines `F_CPU = system_core_clock`, TIM1=TMR1 … TIM15=TMR15 aliases

### GPIO

- Pin enum: `PA0`–`PD15` (plus PE/PF/PG if defined)
- `PIN_MAP[pin]` → `PinInfo_TypeDef { GPIOx, TIMx, ADCx, GPIO_Pin_x, TimerChannel, ADC_Channel }`
- `IS_PIN(p)`, `IS_ADC_PIN(p)`, `IS_PWM_PIN(p)` macros
- `digitalWrite_HIGH(pin)`, `digitalWrite_LOW(pin)`, `digitalRead_FAST(pin)`, `togglePin(pin)` → fast access via PIN_MAP
- `portInputRegister(Port)` → `&(Port->idt)`; `portOutputRegister(Port)` → `&(Port->odt)`

### Arduino API (`libcore/Arduino.c`)

- `void pinMode(uint8_t pin, PinMode_TypeDef mode)` — modes: `INPUT`, `INPUT_PULLUP`, `INPUT_PULLDOWN`, `INPUT_ANALOG`, `INPUT_ANALOG_DMA`, `OUTPUT`, `OUTPUT_OPEN_DRAIN`, `OUTPUT_AF_OD`, `OUTPUT_AF_PP`, `PWM`
- `void digitalWrite(uint8_t pin, uint8_t value)`
- `uint8_t digitalRead(uint8_t pin)`
- `void analogWrite(uint8_t pin, uint16_t value)` — calls `PWM_Write(pin, value)`
- `uint16_t analogRead(uint8_t pin)` — calls `ADCx_GetValue(...)`, returns 0–4095 (12-bit)
- `uint16_t analogRead_DMA(uint8_t pin)`
- **No `analogReadResolution()`, `analogReadAsync()`, `analogWriteFrequency()`, or `analogWriteScaled()`**

### PWM (`libcore/pwm.c`)

- `uint8_t PWM_Init(uint8_t Pin, uint32_t Resolution, uint32_t Frequency)` — resolution default 1000, frequency default 10000 Hz
- `void PWM_Write(uint8_t Pin, uint32_t Value)` — write duty 0..Resolution
- `analogWrite(pin, value)` calls `PWM_Write(pin, value)` — no scaling, no frequency change

### Timers (`libcore/timer.c`)

- `void Timer_SetInterrupt(tmr_type* TIMx, uint32_t time_us, Timer_CallbackFunction_t fn)`
- `bool Timer_SetInterruptFreqUpdate(tmr_type* TIMx, uint32_t Freq)`
- `void Timer_SetInterruptBase(TIMx, Period, Prescaler, fn, PreemptPrio, SubPrio)`
- `void Timer_SetCounter(tmr_type* TIMx, uint32_t Counter)` — sets `TIMx->cval`
- `void Timer_SetPrescaler(tmr_type* TIMx, uint16_t Prescaler)` — sets `TIMx->div`
- `void Timer_SetReload(tmr_type* TIMx, uint16_t Reload)` — sets `TIMx->pr`
- `void Timer_SetCompare(tmr_type* TIMx, uint8_t ch, uint32_t val)`
- `uint32_t Timer_GetClockMax(tmr_type* TIMx)` — returns `apb1_freq * 2` (= 240 MHz)
- `void Timer_GenerateUpdate(tmr_type* TIMx)` — sets `swevt_bit.ovfswtr = TRUE`
- IRQ names: `TMR2_GLOBAL_IRQHandler`, `TMR3_GLOBAL_IRQHandler`, etc.
- **No HC32-style `Timer0` class** — direct `tmr_type*` pointer API

### Watchdog (`libcore/wdg.h`)

- `uint32_t WDG_Init(uint32_t timeout_ms)` — initialize IWDG
- `void WDG_ReloadCounter(void)` — kick the watchdog
- **No `IWatchdog` class or `WDT.begin()`/`WDT.reload()`**

### Serial (`libcore/HardwareSerial.h`)

- `HardwareSerial(usart_type* usart, uint8_t* buffer, uint16_t size)`
- `void begin(uint32_t baud, SERIAL_Config_t config = SERIAL_8N1)`
- `int available()`, `int read()`, `void write(uint8_t c)`, `void write(const uint8_t*, uint16_t len)`, `void print(const char*)`
- Globals: `Serial1`–`Serial5`, `Serial7` (no `Serial6`)
- **Not derived from Arduino `Stream` class** — missing `println`, `flush`, `peek`, etc.

### Clock (`libcore/at32f403a_407_clock.c`)

- `system_core_clock` is a global `uint32_t` updated by clock init
- Clock tree: HEXT 8MHz → HEXTDIV /2 = 4MHz → PLL ×60 = 240MHz → SYSCLK 240MHz → AHB /1 → APB1 /2 → APB2 /2

### No Servo library

- The core includes no `Servo` class. Servo support must be implemented directly via PWM (PWM_Init / PWM_Write).

### No panic/postmortem support

- No `<drivers/panic/panic.h>` — MinSerial must use raw USART register writes instead.

---

## Current Status

**Phases 1–2: HAL Skeleton + Board Support** — ✅ All files created and committed

**Phase 3: PlatformIO Integration** — ✅ Framework in place (needs build test)

- `ini/at32.ini` updated to use local platform at `buildroot/share/PlatformIO/platform-at32/`
- `buildroot/share/PlatformIO/platform-at32/platform.json` — platform descriptor
- `buildroot/share/PlatformIO/platform-at32/builder/frameworks/arduino.py` — build script
- `buildroot/share/PlatformIO/boards/at32f403argt7.json` — Wanhao board
- `buildroot/share/PlatformIO/boards/at32f403arct7.json` — EasyThreed board
- `Marlin/src/HAL/AT32/app_config.h` — force-included app config
- `Marlin/src/inc/SanityCheck.h` — AT32 added to SERIAL_DMA gate
- `Marlin/src/HAL/shared/servo.h` — AT32/Servo.h included

**Phase 4: HAL Correctness** — ✅ All HC32→AT32 API corrections applied. All files rewritten/fixed in this session.

---

## Phase 5: PlatformIO Build — 🔧 IN PROGRESS

### Root cause of SCons path-doubling (IDENTIFIED & FIXED)

**Problem:** When `platform = buildroot/share/PlatformIO/platform-at32` (bare relative path) is used in `ini/at32.ini`, PlatformIO's `PlatformFactory.new()` detects `os.path.isdir()` is True and loads the platform **directly** from the relative path — bypassing the package manager. This makes `p.get_dir()` return a relative path (`buildroot/share/PlatformIO/platform-at32`). SCons then resolves `GetFrameworkScript("arduino")` relative to the current SCons source dir (the `builder/` subdir), doubling the path:

```
buildroot/share/PlatformIO/platform-at32/builder/
  + buildroot/share/PlatformIO/platform-at32/builder/frameworks/arduino.py
  = DOUBLED ERROR
```

**Fix applied:** Changed `ini/at32.ini` to use `file://buildroot/share/PlatformIO/platform-at32`. With the `file://` prefix, `os.path.isdir()` returns False, PlatformIO goes through the package manager, finds the installed package at `~/.platformio/platforms/at32/` (absolute path), and `p.get_dir()` returns the absolute path. No more doubling.

Verified: `PlatformPackageManager().get_package("file://buildroot/share/PlatformIO/platform-at32")` returns `PackageItem <path=/Users/thinkyhead/.platformio/platforms/at32>`.

Also synced: `rsync -a buildroot/share/PlatformIO/platform-at32/ ~/.platformio/platforms/at32/`

### Next Steps

1. **Run build test** — `buildroot/bin/mftest -t WANHAO_0327001 -n1 -y` (or `pio run -e WANHAO_0327001`)
   - Expect compile errors on first pass — iterate
2. **Fix any compile errors** — likely areas:
   - `timers.h`: `tmr_type*` needs `at32f403a_407.h` in scope (should come via core CPPPATH)
   - `map()` availability in HAL (use `constexpr` or stdlib `map`-equivalent)
   - Missing `INPUT_ANALOG` constant (check AT32 core `libcore/gpio.h`)
   - `NVIC_SetPriorityGrouping` availability
3. **Run EASYTHREED_ET4000_PLUS** after WANHAO_0327001 passes
4. **Commit** once both build cleanly
