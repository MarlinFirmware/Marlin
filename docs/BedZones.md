> **⚠ Temporary Simulator Dependency**: The directory `TMP_MarlinSimUI/` in the workspace root contains a locally modified copy of [MarlinSimUI](https://github.com/p3p/MarlinSimUI) with zone heater components added to `virtual_printer.cpp`. This is a **temporary testing shim** — it is referenced by `ini/native.ini` via `symlink://` so the simulator build picks it up instead of the upstream archive. Once the BED_ZONES feature is upstreamed or MarlinSimUI is updated to support zone heaters natively, `TMP_MarlinSimUI/` should be deleted and `ini/native.ini` reverted to the upstream URL.

---

# Bed Heating Zones — Feature Requirements

## Overview

Add a **BED_ZONES** feature that divides the print bed into independently controlled heating zones. Each zone has its own heater output pin and thermistor input pin. Zones are selected via bitmask (for groups) or individually via index. The host G-code interface is the primary target for the initial implementation.

---

## 1. Configuration Block — `Configuration_adv.h`

### 1.1 Feature Enable

```cpp
//#define BED_ZONES
```

Guarded by `HAS_HEATED_BED`.

---

### 1.2 Zone Count

```cpp
#if ENABLED(BED_ZONES)
  #define BED_ZONES_COUNT  4   // Number of zones: 1–16
```

- Maximum supported value: **16** (fits in a `uint16_t` bitmask).
- A count of 1 is valid (single zone, back-compat with standard bed heater).
- **Must be an explicit integer literal** — it is used in preprocessor `#if` guards (`#if BED_ZONES_COUNT > 8` etc.), so deriving it from `ARRAY_SIZE(BED_ZONE_HEATER_PINS)` is not feasible at the preprocessor level. This is the same pattern Marlin uses for `EXTRUDERS` and `NUM_AXES`.
- `SanityCheck.h` will `static_assert` that the pin arrays have exactly `BED_ZONES_COUNT` entries, catching any count/pin-list mismatch at compile time.

---

### 1.3 Zone Pin Definitions

Each zone entry specifies:
| Field | Type | Default for Zone 0 |
|---|---|---|
| Heater output pin | `pin_t` | `HEATER_BED_PIN` |
| Thermistor input pin | `pin_t` | `TEMP_BED_PIN` |

**Zone 0 always maps to the board's standard bed heater/thermistor.** Additional zones require explicit pin assignments.

Example block (4-zone configuration):

```cpp
  #define BED_ZONE_HEATER_PINS  { HEATER_BED_PIN, 46, 47, 48 }
  #define BED_ZONE_SENSOR_PINS  { TEMP_BED_PIN,   14, 15, 12 }
```

- Arrays must have exactly `BED_ZONES_COUNT` entries — enforced by `static_assert(ARRAY_SIZE(_bed_zone_heater_pins) == BED_ZONES_COUNT, ...)` in SanityCheck.
- Duplicate heater pins across zones is an error (validate at compile time where possible).

---

### 1.4 Zone Masks

Numbered bitmask presets for common groupings. Up to **16 presets** are supported. Each mask is a `uint16_t` bitmask where bit *n* corresponds to zone *n*.

```cpp
  // Example: 4-zone bed, two presets:
  //   P0 — all four zones (0b00001111 = 15)
  //   P1 — front two zones only (0b00000011 = 3)
  #define BED_ZONE_MASK_COUNT  2
  #define BED_ZONE_MASKS       { 0b00001111, 0b00000011 }
```

- Preset 0 (`P0`) is the power-on default and the `M502` reset value.
- `BED_ZONE_MASKS` entries are `uint16_t` literals.
- Presets are selected by index via `M142 P<n>`. If `BED_ZONE_MASK_COUNT` is 0, no presets are defined; use `M142 K<bitmask>` or `M142 A<index>` directly.

```cpp
#endif // BED_ZONES
```

---

## 2. Internal Representation

### 2.1 Temperature Arrays

All per-bed temperature state that is currently scalar becomes arrays of length `BED_ZONES_COUNT` when `BED_ZONES` is enabled:

| Current scalar | New array |
|---|---|
| `temp_bed.celsius` | `temp_bed[n].celsius` |
| `temp_bed.target` | `temp_bed[n].target` |
| `temp_bed.soft_pwm_amount` | `temp_bed[n].soft_pwm_amount` |

The existing `temp_bed` scalar accessor is preserved as a macro/inline aliasing zone 0 so that non-zone code continues to compile unchanged.

### 2.2 Zone Selection State

```cpp
uint16_t bed_zone_mask;  // Active zone bitmask, default = all zones enabled
```

Stored in RAM and persisted to EEPROM. Saved by `M500`, restored by `M501`, reset by `M502`. The active mask is part of the printer's working state (like feed rate overrides or PID values) and must survive power cycles.

### 2.3 Active Zone Mask at Runtime

The mask is applied whenever a set-temperature command is issued. Each bit selects which zones receive the new target temperature. Zones not in the mask retain their current target.

---

## 3. G-Code Interface

### 3.1 M140 / M190 — Set Bed Temperature

Existing parameters are preserved. New optional parameters:

| Parameter | Description |
|---|---|
| `K<mask>` | Unsigned integer bitmask selecting zones. Overrides current mask for this command only. Not a permanent change — subsequent commands still use the active mask. |
| `A<index>` | Zero-based area (zone) index (0–15). Selects exactly one zone (`mask = 1 << index`). Mutually exclusive with `K`. |

If neither `K` nor `A` is given, the **current active mask** is used (all zones by default).

**Examples**

```gcode
M140 S60           ; Heat all active-mask zones to 60°C and return
M190 R40           ; Heat all active-mask zones to 40°C and wait
M140 S80 A2        ; Heat area 2 only to 80°C and return
M190 S60 K5        ; Heat areas 0 and 2 (0b0101) to 60°C and wait
M140 S0  K15       ; Turn off all 4 areas (0b1111)
M140 S60 K65535    ; Turn off all 16 areas immediately
```

Wait condition for M190: satisfied when **all selected zones** are at target within `TEMP_BED_HYSTERESIS`.

### 3.2 M142 (new) — Set Active Bed Zone Mask

Persistently sets the active zone mask used by subsequent M140/M190 commands when no `K`/`I` is given.

```
M142 K<mask>       ; Set active area mask (raw bitmask)
M142 A<index>      ; Set active mask to single area (1 << index)
M142 P<preset>     ; Select a numbered preset by index (0-based into BED_ZONE_MASKS)
M142               ; Report current active mask and matching preset index [P<n>] if any
```

> M141 is already used for chamber temperature (M141/M191 pair). M142 is free and sits naturally next to M140 (bed set) and M141 (chamber set), making its purpose clear by proximity. Axis letters (X Y Z U V W) and E are intentionally avoided on all bed-zone parameters. `A` stands for **Area**.

### 3.3 M105 — Report Temperatures

When `BED_ZONES` is enabled, M105 reports each zone's temperature and power level:

```
ok T:210 /210 B0:60 /60 B1:59 /60 B2:61 /60 B3:60 /60 @:127 B0@:64 B1@:64 B2@:64 B3@:64
```

- Temperature format: `B<n>:<actual> /<target>` for each zone (all zones, regardless of active mask).
- Power format: `B<n>@:<pwm>` for each zone.
- Implemented in `print_heater_states()` in `temperature.cpp`.

### 3.4 M303 — PID Autotune

Add `A<index>` parameter to target a specific bed area (zone):

```
M303 E-1 A2 S60   ; Autotune area 2
```

Without `A`, autotune applies to area 0 (current behaviour for `E-1`).

### 3.5 M304 — Set Bed PID

Add `A<index>` parameter to select the zone (`A` = Area, consistent with all other bed-zone G-codes; `P`/`I`/`D` are the PID gain letters and `X`/`Y`/`Z`/`E`/`U`/`V`/`W` are axis letters):

```
M304 A2 P1.2 I0.08 D18.6
```

`M304` with no parameters reports PID values for all zones (when `BED_ZONES` is enabled), one line per zone: `M304 A<n> P... I... D...`.

---

## 4. Areas of Existing Code Requiring Audit / Changes

### 4.1 `temperature.cpp` / `temperature.h`

- [x] Expand `temp_bed` from scalar `TempInfo` to array `temp_bed[BED_ZONES_COUNT]`.
- [x] `setTargetBed(celsius, mask)` — apply target to all zones in mask.
- [x] `degBed()` — returns zone-0 value for back-compat; add `degBedZone(n)`.
- [x] `wholeDegBed()` — same aliasing strategy.
- [x] `isHeatingBed()` — true if any zone in mask is heating.
- [x] `isCoolingBed()` — true if any zone in mask is cooling.
- [x] `wait_for_bed()` / `wait_for_bed_heating()` — wait on all active-mask zones. Residency timer requires all active-mask zones within `TEMP_BED_WINDOW`; resets if any drift beyond `TEMP_BED_HYSTERESIS`.
- [x] Bang-bang control loop: iterate zones.
- [x] PID compute loop: iterate zones (shared PID coefficients for now — see §7.2).
- [x] `soft_pwm` output: zone 0 drives the shared `soft_pwm_bed` counter; zone heater pins use `extDigitalWrite` (required for runtime-variable pin numbers on AVR).
- [x] Pin initialisation (`init()`) — iterate and configure all zone heater pins via `pinMode` + `extDigitalWrite`.
- [x] `MAX_BED_POWER` — applied per-zone (shared value; per-zone override is future).
- [x] Scalar `temp_bed` accesses in ISR, ADC sampling, `readings_ready`, `disable_all_heaters`, and `getHeaterPower` all guarded with `#if HAS_BED_ZONES` / `#else`.
- [x] Zone 1+ sensor reads — each zone gets its own `PrepareTemp_BED_ZONEn` / `MeasureTemp_BED_ZONEn` ISR state pair driven from `BED_ZONE_SENSOR_PINS`. Celsius converted per-zone in `updateTemperaturesFromRawValues`.
- [x] `BED_ZONE_SENSOR_TYPES` — zones 1–15 each get an individual `BED_ZONE_SENSOR_TYPE_n` define (defaults to `TEMP_SENSOR_BED`). `ANY_THERMISTOR_IS` in `Conditionals-4-adv.h` is extended to check each. `Conditionals-5-post.h` sets defaults for all 15 slots. `thermistors.h` defines `TEMPTABLE_BED_ZONEn` / `TEMPTABLE_BED_ZONEn_LEN` for each zone. `temperature.cpp` builds `zone_ttbl_map[]` + `zone_ttbllen_map[]` and dispatches per zone using the same pointer-to-array-of-unknown-bound cast as the hotend path. `analog_to_celsius_bed` gains an optional `zone` parameter (default 0).
- [x] **Per-zone ISR PWM output** — zones 1+ have a dedicated `soft_pwm_bed_zone[BED_ZONES_COUNT - 1]` array of `SoftPWM` counters. Each counter drives its zone heater pin via `extDigitalWrite` in both the standard and `SLOW_PWM_HEATERS` ISR paths. Zone 0 continues to use `WRITE_HEATER_BED` via the shared `soft_pwm_bed` counter.
- [x] **Per-zone thermal runaway** — `tr_state_machine_bed[BED_ZONES_COUNT]` is a separate array (independent of `tr_state_machine[RUNAWAY_IND_BED]`) giving each zone its own `THERMAL_PROTECTION_BED` state machine. A locked-on zone with a disconnected thermistor will trigger runaway independently.
- [x] **Per-zone heating watchdog** — `watch_bed_zone[BED_ZONES_COUNT]` replaces the scalar `watch_bed` when `HAS_BED_ZONES`. Each zone has its own `WATCH_BED` "heating failed" watchdog. The non-zones `WATCH_BED` block in `manage_heated_bed` is guarded with `!HAS_BED_ZONES` to prevent the scalar `watch_bed` being referenced when only the array exists.

### 4.2 `M140_M190.cpp`

- [x] Parse `K` (mask) and `I` (index) parameters.
- [x] Call `setTargetBed(temp, mask)` instead of `setTargetBed(temp)`.
- [x] Update wait condition to check all masked zones.
- [x] `BED_ANNEALING_GCODE` cooling loop must iterate masked zones.

### 4.3 `settings.cpp` (EEPROM)

- [x] Store `bed_zone_mask` in EEPROM (new versioned slot). Saved by `M500`, restored by `M501`, reset by `M502` to the compile-time default defined in `Configuration_adv.h` (`BED_ZONE_MASKS[0]`, or all-zones if `BED_ZONE_MASK_COUNT` is 0).
- [x] Mask presets themselves (`BED_ZONE_MASKS` array) are compile-time constants — only the **active mask selection** is stored in EEPROM.
- [x] `M142` with no parameters reports current active mask (decimal + binary) and matching preset index `[P<n>]` if any.

### 4.4 `probe.cpp`

- [x] `PROBING_BED_TEMP` / `WAIT_FOR_BED_HEATER` / `LEVELING_BED_TEMP` / `PREHEAT_BEFORE_PROBING`: since zone geometry is not stored (open §7.3) there is no way to know which zone is under the probe — all zones must be at temperature. `setTargetBed(bedPreheat)` passes no mask and defaults to `0xFFFF` (all zones) ✅. `wholeDegBed()` and `degTargetBed()` are already zone-aware (return hottest active zone) ✅. `wait_for_bed_heating()` calls zone-aware `isHeatingBed()` + `wait_for_bed()` ✅. No code changes required.

### 4.5 `marlinui.cpp` / Preheat Presets (M145)

- [x] `material_preset[].bed_temp` is scalar — sets all zones to same temp (acceptable initially).
- [x] `LCD apply preheat` path calls `setTargetBed()` — passes `bed_zone_mask` so only active zones are heated.

### 4.10 LCD Menus (`menu_temperature.cpp`, `menu_tune.cpp`, `menu_advanced.cpp`)

- [x] `menu_temperature.cpp` / `menu_tune.cpp` — **Temperature** and **Tune** menus: when `HAS_BED_ZONES`, the single `Bed` item is replaced by a per-zone loop showing `Bed 0` … `Bed N-1` (using `MSG_BED_N`). Editing a zone calls `setTargetBed(celsius, 1U << zone)` — active-mask independent, intentional. Without `HAS_BED_ZONES`, falls back to original single `Bed` item.
- [x] `language_en.h` — added `MSG_BED_N = _UxGT("Bed ~")` (tilde replaced by zone index at render time, same pattern as `MSG_NOZZLE_N`).
- [x] `menu_advanced.cpp` — **Advanced Settings > Temperature** menu:
  - `PID_EDIT_MENU` + `HAS_BED_ZONES`: per-zone P/I/D edit items via `REPEAT(BED_ZONES_COUNT, _BED_ZONE_PID_ITEMS)`. Apply callbacks use `MenuItemBase::itemIndex` to write the correct `temp_bed[z].pid`.
  - `PID_AUTOTUNE_MENU` + `HAS_BED_ZONES`: per-zone autotune-temp edit + trigger item via `REPEAT(BED_ZONES_COUNT, _BED_ZONE_AUTOTUNE_ITEM)`. Trigger injects `M303 U1 E-1 A<z> S<temp>`.
  - Without `HAS_BED_ZONES`, falls back to original single-bed PID edit/autotune items.
  - `autotune_temp_bed_zone[BED_ZONES_COUNT]` replaces scalar `autotune_temp_bed` when zones enabled.

### 4.6 `controllerfan.cpp`

- [x] `CONTROLLER_FAN_BED_HEATING`: uses `thermalManager.isHeatingBed()` when `HAS_BED_ZONES` (zone-aware), falls back to `temp_bed.soft_pwm_amount > 0` otherwise.

### 4.7 `G76_M871.cpp` (Temperature Probe Calibration)

- [x] `setTargetBed()` calls pass no mask → the zone-aware overload defaults to `mask=0xFFFF`, which heats all zones. Since zone geometry is not stored (open §7.3), there is no way to know which zone is under `PTC_PROBE_POS`; heating the whole bed is the only correct behaviour. `wholeDegBed()` returns zone-0 back-compat — adequate for calibration monitoring. No code changes required.

### 4.8 `M360.cpp` (Host Config Report)

- [x] ~~Reports `BedZones:<count>` when `HAS_BED_ZONES` is enabled.~~ M360 is the legacy Repetier Host config report — do not modify it. Zone count is already advertised via M115 `Cap:BED_ZONES:<count>`.

### 4.9 Conditionals / Feature Guards

- [x] Add `HAS_BED_ZONES` macro (true when `BED_ZONES` enabled and `BED_ZONES_COUNT > 1`).
- [x] `Conditionals-5-post.h` — `HAS_BED_ZONES` definition + error if `BED_ZONES` without `HAS_HEATED_BED`.
- [x] `SanityCheck.h` — validate zone count 1–16, validate array lengths via `static_assert`, check pin conflicts.
- [x] `features.ini` — `HAS_BED_ZONES = build_src_filter=+<src/gcode/temp/M142.cpp>`.
- [x] `BED_ZONE_SENSOR_TYPES` — add Conditionals logic to inspect each entry of `BED_ZONE_SENSOR_TYPES` and ensure the corresponding `TEMPTABLE_n` is included, mirroring how `TEMP_SENSOR_0` through `TEMP_SENSOR_BED` trigger thermistor table includes. Without this, a zone using a thermistor type that no other heater uses will compile but produce garbage temperature readings.

---

## 5. LCD Display — 128×64 (HD44780 / U8g2)

### 5.1 Status Screen

The standard status screen shows `B:60/60` for bed temperature. With zones active, show the **hottest active zone** temperature in the same `B:` field — no zone indicator, no extra characters:

```
B:62/60
```

This gives the most safety-relevant reading at a glance without using any extra display space. The target shown is the target of whichever zone is hottest.

A dedicated **Bed Areas** info screen (accessible from the main menu) shows all zone temperatures individually — see §5.2.

This behaviour is the same for both 128×64 (U8g2) and HD44780 displays.

### 5.2 Bed Areas Detail Screen

A dedicated read-only info screen lists all zone temperatures sequentially, one zone per line:

```
Bed Areas  [P0]
A0: 62/60
A1: 61/60
A2: 60/60
A3: 62/60
```

- Zones are listed in order 0, 1, 2, … with no special layout.
- Inactive areas (not in active mask) are shown as `A<n>: --`.
- Header line shows `[P<n>]` if the active mask matches a numbered preset.
- Target temperature shown alongside actual (`actual/target`).
- Scrolling is supported if the zone list overflows the screen.

### 5.3 Menu Items

- **Control > Bed Temperature** — ✅ Implemented. Shows `Bed 0` … `Bed N-1` (one edit item per zone). Each item sets that zone's target independently of the active mask.
- **Tune > Bed** — ✅ Same per-zone items as above, accessible during a print.
- **Advanced Settings > Temperature > PID** — ✅ Implemented. Per-zone P/I/D edit items and per-zone PID autotune items when `PID_EDIT_MENU` / `PID_AUTOTUNE_MENU` and `HAS_BED_ZONES` are enabled.
- **Control > Bed Areas** (new, §5.2 detail screen) — ⬜ Todo:
  - Select Active Mask (lists numbered presets P0…Pn from `BED_ZONE_MASKS`)
  - Area Status (read-only detail screen — §5.2)

### 5.4 HD44780 (16×2 / 20×4) Consideration

Same rule as §5.1: the `B:` field on the status line shows the hottest active zone temperature with no extra characters. The full per-zone detail screen from §5.2 is accessible via the menu.

---

## 6. Implementation Phases

| Phase | Scope | Status | Goal |
|---|---|---|---|
| **1 — Config & Skeleton** | `Configuration_adv.h`, `SanityCheck.h`, `Conditionals` | ✅ Done | Feature compiles, zone count/pins validated |
| **2 — Temperature Core** | `temperature.cpp/h` | ✅ Done | Multi-zone PWM output, per-zone temp read, back-compat accessors |
| **3 — G-Code** | `M140_M190.cpp`, new `M142.cpp`, `M105`, `features.ini` | ✅ Done (M303/M304 ✅ Done) | G-code control, zone reporting, per-zone PID autotune + set, clean mega2560 build |
| **4 — Subsystem Audit** | `probe.cpp`, `settings.cpp`, `controllerfan.cpp`, `G76`, thermal safety | ✅ Done | No regressions in existing features; zone mask EEPROM; per-zone runaway + watchdog; preheat ✅; probe/G76 ✅ (all-zones by default — correct since no zone geometry is stored) |
| **5 — LCD** | `marlinui_HD44780.cpp`, `marlinui.cpp`, menu screens | 🟡 Partial | Status screen (§5.1/§5.4) and §5.2 detail screen still todo; Temperature/Tune/PID menus ✅ done |

---

## 7. Open Questions / Decisions Required

1. **M-code for mask selection** — resolved: **M142**. M141 is already chamber temperature; M142 is free and adjacent to M140/M141, making its role self-evident.
2. **Per-zone PID vs shared PID** — resolved for Phase 3: per-zone PID coefficients stored at runtime in `temp_bed[z].pid` and persisted to EEPROM (`bedZonePIDs[BED_ZONES_COUNT]`). `M303 E-1 A<area>` autotunes a specific area; `M304 A<area> P I D` sets PID for a specific area (`A` = Area, consistent across all bed-zone G-codes). EEPROM version bumped to V91.
3. **Zone layout metadata** — Should `BED_ZONES` store row/column geometry for the LCD grid, or just a flat list of zones? (open)
4. **Mixed-mode probing** — ✅ Resolved. Since zone geometry is not stored (§7.3), all zones must be at temperature before probing. `probe.cpp` already heats all zones via the `setTargetBed` default mask and uses zone-aware `wholeDegBed()` / `wait_for_bed_heating()`. No code changes required.
5. **Thermal runaway per zone** — ✅ Resolved. Each zone has its own `tr_state_machine_bed[z]` instance (separate from the shared `tr_state_machine[RUNAWAY_IND_BED]`). Per-zone heating watchdog uses `watch_bed_zone[z]`. MAXTEMP and MINTEMP are checked per-zone in the manage loop. All safety paths are independent — a locked-on zone with a disconnected sensor triggers runaway for that zone alone.
6. **`BED_ZONES_COUNT = 1` behaviour** — resolved: always uses the array path (simplifies code; `HAS_BED_ZONES` is only true for count > 1, so the scalar path is used for count = 1). This means `BED_ZONES` with count = 1 compiles through the array path but behaves identically to the non-zone build at runtime.
7. **Mask preset selection via G-code** — resolved: `M142 P<preset_index>` (0-based integer index into `BED_ZONE_MASKS`). No string names — presets are referenced by number only. Integer index is consistent with other Marlin preset selectors.
8. **Per-zone ADC reads** — ✅ Resolved. Each zone has its own `PrepareTemp_BED_ZONEn` / `MeasureTemp_BED_ZONEn` state pair in the ISR. Celsius is converted per-zone using the zone's thermistor dispatch table.
9. **`LOOP_L_N` / `extDigitalWrite` in headers** — Resolved: `LOOP_L_N` is not available at header-parse time; all zone loops in `temperature.h` and `M142.cpp` use plain `for` loops. Zone heater pins use `extDigitalWrite` / `pinMode` (not `OUT_WRITE` / `WRITE`) since AVR token-pasting macros require compile-time constant pin numbers.

---

## 8. Non-Goals (Initial Implementation)

- ~~Per-zone PID coefficients in EEPROM~~ — ✅ Done (`bedZonePIDs[BED_ZONES_COUNT]` in `settings.cpp`; saved by M500, restored by M501, reset by M502).
- Zone mask persistence across preset reorders (EEPROM stores the raw bitmask value, not the preset index).
- Graphical UI for non-64px displays other than HD44780 basics.
- Automatic zone sequencing or scheduling.
- Zone-aware mesh bed leveling compensation.
