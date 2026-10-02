# AGENTS.md — ESP32 HAL

Working notes and conventions for the **ESP32 HAL** folder (`Marlin/src/HAL/ESP32/`). This HAL runs Marlin on Espressif ESP32 (Xtensa) boards, using I2S-DMA stepper pulse generation, an optional serial I/O expander, and optional WiFi/OTA/Web support.

## Build & Test Loop

Validate with `mftest` (it regenerates `Marlin/Configuration.h` per target, which raw `pio run -e` does not do reliably):

```bash
cd "$(git rev-parse --show-toplevel)"
buildroot/bin/mftest -t esp32 -n1 -y        # BOARD_ESPRESSIF_ESP32, WIFISUPPORT+WEBSUPPORT+OTA
buildroot/bin/mftest -t esp32 -n2 -y        # BOARD_ESPRESSIF_ESP32, TMC HW serial (Serial1)
buildroot/bin/mftest -t mks_tinybee -n1 -y  # BOARD_MKS_TINYBEE, ESP3D_WIFISUPPORT
```

Both `esp32` test targets exercise the base `env:esp32` build; `mks_tinybee` exercises the `ESP3D_WIFISUPPORT` path. Additional `ini/esp32.ini` envs with no test folder — `FYSETC_E4`, `PANDA`, `godi_esp32` — can be built directly with `pio run -e <env> -y` to check their partition-table / EXIO variants.

> Builds are **not verified in this environment** (the `espressif32` PlatformIO platform may not be installed here). Run the `mftest` commands above to validate; do not claim green without running them.
>
> Do NOT use `rm -rf .pio/build/...` to force a rebuild — the cross-profile write guard blocks it and it is unsafe. Use `pio run -e <env> -t clean` or just let `mftest` rebuild.

## Framework / Platform Integration (upstream, NOT vendored)

ESP32 uses the **upstream `espressif32` PlatformIO platform** (`espressif32@2.1.0`, toolchain `toolchain-xtensa-esp32s3`) plus the upstream Arduino-ESP32 / ESP-IDF libraries. There is **no Marlin-vendored core** (AT32 is the exception). All Marlin-specific glue lives in:

- `Marlin/src/HAL/ESP32/` — HAL implementation, I2S stepper, WiFi, OTA, SPIFFS, serial.
- `Marlin/src/pins/esp32/` — board pin tables (each pins file declares its `env:`).
- `ini/esp32.ini` — env defs, partition CSV selection, build flags.

Because the framework is upstream, there is **no "mirror to installed package" step** — just edit the repo files and rebuild.

### Gotchas that are not obvious from the source

1. **Marlin runs on the Arduino "app" core, not alone.** The I2S stepper task is pinned to `CONFIG_ARDUINO_RUNNING_CORE` via `xTaskCreatePinnedToCore(...)` in `i2s.cpp:343`. The chip is dual-core; do not assume single-core timing and avoid long `portENTER_CRITICAL` sections that would stall the other core.

2. **`DELAY_CYCLES` can pause prints for ~17.9 s.** The cycle-delay loop in `HAL.h` reads the `ccount` cycle counter and is vulnerable to overflow if a long FreeRTOS preemption/ISR lands mid-delay (it re-loops until `ccount` passes `stop`). This has caused real, apparently-random print stalls. The code already guards the overflow case — preserve that logic; do not "simplify" it.

3. **`MYSERIAL1` is `flushableSerial`, not the USB `Serial`.** `HAL.h` defines `MYSERIAL1 flushableSerial` (`FlushableHardwareSerial.h`). On ESP32 the bare `Serial` is USB/JTAG CDC; Marlin's G-code link is a hardware UART (Serial1 by default). Do not confuse the two. TMC HW-serial remaps Serial1/Serial2 to user pins in `HAL.cpp init_board()`.

4. **ADC2 is unusable while WiFi is active.** `HAL.cpp adc_init()` only configures ADC1 channels (`get_channel()` maps pins 32–39); ADC2 is shared with the WiFi radio and is intentionally left unconfigured (see comment at `HAL.cpp:253`). Keep thermistor pins on ADC1, or readings will fail when WiFi is up.

5. **Partition table is per-board and matters.** `env:esp32`/`mks_tinybee` use the platform default CSV; `FYSETC_E4` uses `default_16MB.csv`; `PANDA` and `godi_esp32` use the custom `Marlin/src/HAL/ESP32/esp32.csv` (otadata + two OTA app slots of 0x180000 + a SPIFFS partition). `WEBSUPPORT` needs the SPIFFS partition — if you change flash layout, keep the board's partition CSV in sync or OTA/Web won't fit.

6. **Two mutually-exclusive stepper backends.** `I2S_STEPPER_STREAM` (DMA-driven I2S GPIO expander, the default) and `USE_ESP32_EXIO` (serial I/O expander, used by PANDA). `i2s.cpp` is compiled only when `!USE_ESP32_EXIO`. Do not enable both; expander-pin PWM (pin > 127) is routed through the I2S path in `HAL.cpp` `set_pwm_duty`.

7. **EEPROM auto-selects the Arduino `EEPROM` class.** `eeprom.cpp` backs `PersistentStore` with the ESP32 `EEPROM` NVS emulation (`MARLIN_EEPROM_SIZE` = 4 KB, `EEPROM.begin()` / `EEPROM.end()`). No per-board `SDCARD_EEPROM_EMULATION` / `I2C_EEPROM` define is required (unlike AT32).

8. **Hardware-PWM pins are GPIO 0–33 only.** `MAX_PWM_IOPIN = 33` in `HAL.h`; `get_pwm_channel()` (HAL.cpp) rejects pins ≥ 34 (input-only) and caps at 15 LEDC channels. Pins 34–39 are input-only on ESP32 — never assign them to heater/fan PWM.

9. **Watchdog is split and partly stubbed.** `USE_WATCHDOG` refreshes the ESP task WDT via `esp_task_wdt_reset()`, but `MarlinHAL::watchdog_init()` is an explicit TODO stub. Separately, `USE_ESP32_TASK_WDT` inits a 10 s task WDT in `init_board()`. Don't rely on `watchdog_init()` doing setup.

10. **WiFi builds are size-sensitive.** `WIFISUPPORT` pulls ESPAsyncWebServer + Wi-Fi; `ESP3D_WIFISUPPORT` pulls esp3dlib. `ini/esp32.ini` sets `-DCORE_DEBUG_LEVEL=0` to keep the image small, and `mks_tinybee` adds `build_src_flags = -O3`. If a WiFi-enabled config overflows, check the partition CSV and debug level before touching HAL code.

## Conventions

- All HAL source is guarded by `#ifdef ARDUINO_ARCH_ESP32`.
- Keep HAL changes in `Marlin/src/HAL/ESP32/` and board pin changes in `Marlin/src/pins/esp32/`. Build/partition/env glue goes in `ini/esp32.ini`.
- The framework is upstream — edit repo files and rebuild; there is no vendored package to mirror.
- Don't enable both `I2S_STEPPER_STREAM` and `USE_ESP32_EXIO`; don't put thermistor pins on ADC2 when WiFi is in use; don't assign heater/fan pins ≥ 34.

## Related

- No `README.md` in this folder.
- Wiki: [[hal-esp32]], [[marlin-src-hal]].
- `Marlin/src/HAL/shared/` — shared HAL APIs (`eeprom_api`, SPI helpers) the ESP32 HAL reuses.
- `ini/esp32.ini` — env/partition/flag definitions for all ESP32 boards.
