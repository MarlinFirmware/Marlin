# Artillery Genius Pro — Firmware Feature Comparison

**Firmware:** `firmware-gpro-merged-v7-0x08000000.bin`
**Base:** Marlin bugfix-2.1.x | **Board:** BOARD_ARTILLERY_RUBY (STM32F401RCT6)
**Date:** 2026-05-01

### Reference firmwares

| Label | Source |
|-------|--------|
| **Stock** | Marlin bugfix-2.1.x default configuration |
| **gpro-mp** | `custom fw - artillery genius pro/Marlin-bugfix-2.1.x.gpro-mp` |
| **mfagp** | `custom fw - artillery genius pro/Marlin-for-artillery-genius-pro-bugfix-2.1.x` |
| **Our v7** | `output/firmware-gpro-merged-v7-0x08000000.bin` |

---

## Hardware / Board

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| Board | generic | `BOARD_ARTILLERY_RUBY` | `BOARD_ARTILLERY_RUBY` | `BOARD_ARTILLERY_RUBY` |
| Dual serial (USB + TFT UART) | 1 port | 2 ports | 2 ports | 2 ports |
| TFT baud rate | — | 250000 | **115200** | 250000 |
| BLTouch | off | on | on | on |
| BLTouch dual-pin wiring | — | yes (Z_MIN_PROBE=PC2, Z_MIN=PA0) | yes (PC2) | yes — authoritative |
| Probe hit state | — | HIGH | HIGH | HIGH |
| Z endstop hit state | — | LOW (NC) | LOW (NC) | LOW (NC) |
| Calibrated steps/unit | generic | {80.121, 80.121, 402, 449.5} | same | same |
| Calibrated PID hotend | generic | hardware-tuned | generic defaults | hardware-tuned |
| Calibrated PID bed | generic | hardware-tuned | generic defaults | hardware-tuned |
| Controller fan auto-management | no | no | **yes** | no |
| NeoPixel RGB LED | no | no | **yes** | no |

---

## G-code Commands

| G-code | Purpose | Stock | gpro-mp | mfagp | Our v7 |
|--------|---------|-------|---------|-------|--------|
| M92 | Set steps-per-unit at runtime | no | **no** | yes | yes |
| M113 | Host keepalive interval | no | no | yes | yes |
| M114 D | Detailed position (planner state) | no | no | yes | yes |
| M115 | Firmware capabilities report | partial | yes | yes | yes |
| M43 | Pin debug / toggle | no | yes | **no** | yes |
| M154 | Auto-report position | no | no | **no** | yes |
| M155 auto on boot | Temps pushed to TFT immediately | no | no | no | **yes** |
| M211 | Software endstops toggle | yes | **no** | yes | yes |
| M290 | Babystepping | no | no | yes | yes |
| M303 | PID autotune | yes | yes | yes | yes |
| M486 | Cancel specific objects | no | no | yes | yes |
| M593 | Input shaping config | no | no | yes | yes |
| M600 | Filament change | no | partial | yes | yes |
| M701/M702 | Load/Unload filament | no | no | no | yes |
| M73 | Set print progress + remaining time | no | no | yes | yes |
| M810–M819 | G-code macros | no | no | **no** | yes |
| M876 | Host prompt response | no | no | yes | yes |
| M900 | Linear Advance K-factor | no | no | yes | yes |
| M48 | Probe repeatability test | no | no | yes | yes |

---

## Bed Leveling

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| Leveling method | bilinear | UBL | UBL | UBL |
| Probe repetitions | 1 | 3 + 1 extra | 3 + 1 extra | 3 + 1 extra |
| Restore leveling after G28 | no | yes | yes | yes |
| Segment-leveled moves | no | no | no | **yes** (5 mm) |
| Assisted tramming (G35) | no | yes | **no** | yes |

---

## Motion / Print Quality

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| S-curve acceleration | no | yes | yes | yes |
| Junction Deviation | no | yes | yes | yes |
| Linear Advance | no | no | yes (K unset) | yes (K=0.13) |
| Input Shaping X+Y | no | no | yes | yes |
| Input Shaping LCD menu | no | no | yes | yes |
| Adaptive step smoothing | no | no | yes | yes |
| Babystepping (always available) | no | no | yes | yes |
| Arc support (G2/G3) | no | no | yes | yes |
| AUTOTEMP | no | no | yes | yes |
| SLOWDOWN (buffer protection) | no | no | yes | yes |
| QUICK_HOME (simultaneous X+Y) | no | no | yes | yes |
| VALIDATE_HOMING_ENDSTOPS | no | no | yes | yes |
| Software endstops (M211) | yes | **no** | yes | yes |
| Stepper idle timeout | no | no | yes (120 s) | yes (120 s) |
| MULTISTEPPING_LIMIT | 128 | 16 | 16 | 16 |
| Z probe fast feedrate | — | 4×60 mm/min | **10×60 mm/min** | 4×60 mm/min |

---

## Safety

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| Power loss recovery | no | yes | **no** | yes |
| Thermal protection hotend | yes | yes | yes | yes |
| Thermal protection bed | yes | yes | yes | yes |
| Hardware watchdog | no | no | yes | yes |
| Detect broken endstop | no | no | yes | yes |
| Cold extrusion prevention | yes | yes | yes | yes |
| Probe repeatability test (M48) | no | no | yes | yes |

---

## Serial / TFT Communication

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| BUFSIZE (command queue) | 4 | 32 | 32 | 32 |
| TX_BUFFER_SIZE | 0 | 128 | 128 | 128 |
| RX_BUFFER_SIZE | 128 | 1024 | 1024 | 1024 |
| XON/XOFF flow control | no | yes | **no** | yes |
| Serial overrun protection | no | no | yes | yes |
| Faster G-code parser | no | no | yes | yes |
| ADVANCED_OK (extended ack) | no | yes | yes | yes |
| NO_TIMEOUTS | no | yes | yes | yes |
| Emergency parser | no | no | yes | yes |
| Host action commands | no | no | yes | yes |
| Host prompt support | no | no | yes | yes |
| Host status notifications | no | no | yes | yes |
| Host keepalive (M113) | no | no | yes | yes |
| Busy-while-heating | no | no | yes | yes |
| Auto-report temperatures | yes | yes | yes | yes |
| Auto-report position (M154) | no | no | **no** | yes |
| M73 progress reporting | no | no | yes | yes |
| Startup auto-temp push (M155) | no | no | **no** | **yes** |
| Capabilities report (M115) | partial | yes | yes | yes |
| Extended capabilities report | no | no | yes | yes |
| M115 geometry report | no | no | **no** | yes |
| Fan change reporting | no | no | yes | yes |
| M114 detail mode | no | no | yes | yes |
| Debug flags G-code | no | no | **yes** | no |

---

## SD Card / Storage

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| SD support | yes | yes | yes | yes |
| Long filename support | no | no | yes | yes |
| Auto-report SD status (M27) | no | yes | yes | yes |
| SD block retry on error | no | no | no | **yes** |
| Most-recent files first | no | no | yes | yes |
| SD abort G-code (G28XY) | no | no | yes | yes |
| Cancel objects (M486) | no | no | yes | yes |
| G-code macros (M810–M819) | no | no | **no** | yes |

---

## Advanced Features

| Feature | Stock | gpro-mp | mfagp | Our v7 |
|---------|-------|---------|-------|--------|
| Advanced pause / M600 | no | partial | yes | yes |
| M701/M702 load/unload | no | no | no | **yes** |
| Nozzle park feature | no | no | yes | yes (via ADVANCED_PAUSE) |
| Print job timer | no | no | yes | yes |
| EEPROM settings (M500/M501) | yes | yes | yes | yes |

---

## Summary

### vs Stock Marlin 2.1.x

All three custom firmwares add: BLTouch + UBL, dual serial for TFT, hardware-tuned motion/PID, input shaping, linear advance, arc support, power loss recovery (except mfagp), host communication, cancel objects, and a large set of G-codes absent from a generic build.

### vs gpro-mp

Both mfagp and our v7 add over gpro-mp: M92, M211, input shaping, linear advance, arc support, cancel objects, full host action commands, hardware watchdog, serial overrun protection, faster parser, busy-while-heating, host keepalive, QUICK_HOME, VALIDATE_HOMING_ENDSTOPS, SD features, M73 progress, M48 probe test.

### vs mfagp (Marlin-for-artillery-genius-pro-bugfix-2.1.x)

**mfagp has, our v7 does not:**

| Feature | Why we don't have it |
|---------|---------------------|
| `USE_CONTROLLER_FAN` | Not needed — Ruby board manages the fan via `E0_AUTO_FAN_PIN` in board definition |
| `NEOPIXEL_LED` | No RGB LED hardware on Genius Pro |
| `DEBUG_FLAGS_GCODE` | Marlin emits a warning recommending it; can be added if space allows |

**Our v7 has, mfagp does not:**

| Feature | Effect |
|---------|--------|
| `STARTUP_COMMANDS "M155 S2"` | TFT gets live temps immediately on boot — mfagp relies on TFT to send M155 itself |
| `POWER_LOSS_RECOVERY` | Resume after power failure — mfagp has the block but it's commented out |
| `ASSISTED_TRAMMING` (G35) | Guided manual bed screw leveling — mfagp has block commented out |
| `GCODE_MACROS` (M810–M819) | Runtime macro storage — mfagp has block commented out |
| `AUTO_REPORT_POSITION` (M154) | Auto-push coordinates to TFT — mfagp has it commented out |
| `PINS_DEBUGGING` (M43) | Pin state reporting and toggle — mfagp has it commented out |
| `SERIAL_XON_XOFF` | Software flow control — mfagp has it commented out |
| `SEGMENT_LEVELED_MOVES` | UBL mesh compensation every 5 mm on long moves |
| `SD_CHECK_AND_RETRY` | Recovers silently from SD read errors |
| `M115_GEOMETRY_REPORT` | Includes geometry info in M115 response |
| Hardware-tuned PID values | gpro-mp calibrated Kp/Ki/Kd — mfagp uses Marlin defaults |
