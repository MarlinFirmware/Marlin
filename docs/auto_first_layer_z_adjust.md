# Auto First Layer Z Adjust (`AUTO_FIRST_LAYER_Z_ADJUST`)

The **Auto First Layer Z Adjust** feature adjusts the first layer nozzle height automatically. It applies a dynamic Z-offset correction based on the difference between the sliced first layer height and the printer's calibrated first layer height. This calibrated height corresponds to the perfect initial layer obtained with the tuned Z offset.

---

## 1. Overview & Functionality

In 3D printing, bed leveling and Z-offset calibration are usually done at a specific height or assume a standard first-layer height (e.g., `0.3 mm`). When you slice a model with a different first layer height (e.g., `0.2 mm`), the nozzle may end up too close or too far from the bed. This mismatch often requires manual Z-offset tuning (baystepping).This is even more accurate if you print with various nozzle sizes and (or) a wide range of layer heights.

When `AUTO_FIRST_LAYER_Z_ADJUST` is enabled, Marlin adjusts the initial first-layer height set by the G-code. Marlin computes the difference relative to the calibrated height using:

`Delta = Z_Gcode - Z_Calibrated`

This delta is applied to the first layer movement so the nozzle prints at the correct distance without manual babystepping or re-slicing the model.

### Key Implementation Details

1. **Safety Margin**

   To prevent the nozzle from scraping the bed or over-squishing the first layer, a built-in safety margin of `0.05 mm` is enforced.

   - If the absolute value of Delta is less than or equal to `0.05 mm`, no correction is applied.
   - If Delta exceeds `0.05 mm`, Marlin applies the correction with the safety margin.

   `Applied Delta = Delta + 0.05 mm`.

   _(Example: If `Z_Gcode = 0.2 mm` and `Z_Calibrated = 0.3 mm`, Delta is `-0.1 mm`. With the margin, the adjustment becomes `-0.05 mm`, moving the nozzle slightly closer instead of the full `0.1 mm`.)_

2. **Slicer Integration**

   Different slicers output the initial Z-height G-code commands differently. To handle this, the slicer type must be set to help Marlin correctly detect the first layer height:
   - **PrusaSlicer**: The first Z-only move is the first layer height.
   - **OrcaSlicer**: The first Z-only move is a Z-hop. The second Z-only move is the first layer height.

---

## 2. G-Code Command: `M429`

Use `M429` to view the current status, toggle the feature, change the calibrated height, or set the slicer family.

### Syntax

```gcode
M429 [S<bool>] [H<float>] [O<int>]
```

### Parameters

- **`S<bool>`**: Enable (`S1`) or disable (`S0`) the Adaptive First Layer Z Offset adjustment.
- **`H<float>`**: Set the calibrated first layer height (in mm).Accepts values in the range `0.0` to `1.0`.
- **`O<int>`**: Set the slicer type to ensure proper parser compatibility:
  - `0`: **PrusaSlicer** (and clones)
  - `1`: **OrcaSlicer** (and clones)

---

### Examples

- **Show Current Settings**:

```gcode
     M429

     Response:
     Auto First Layer Z Adjust Activated
     Configured Layer Height 0.30 mm
     PrusaSlicer
```

- **Enable and Configure for OrcaSlicer at 0.3 mm**:

```gcode
     M429 S1 H0.3 O1
```

- **Disable**:

```gcode
     M429 S0
```

---

## 3. Configuration & EEPROM Settings

### Firmware Configuration (`Configuration.h`)

To use this feature, it must be enabled in your firmware configuration. Uncomment the following parameters in **Configuration.h**, set your calibrated first layer height and define your slicer

```c++
//#define AUTO_FIRST_LAYER_Z_ADJUST
#if ENABLED(AUTO_FIRST_LAYER_Z_ADJUST)
  #define CALIBRATED_FIRST_LAYER_Z_HEIGHT 0.3f    // (mm) Pre-calibrated assumed first layer height
  //#define DEFAULT_SLICER UNKNOWN                // :[ 'UNKNOWN', 'ORCA', 'PRUSA' ]
#endif
```

### Saving Settings to EEPROM

The settings can be written to the printer's EEPROM so that they persist across power cycles:

- Save settings to EEPROM: `M500`
- Restore settings from EEPROM: `M501`

---

## 4. LCD Menu Controls

If your printer runs Marlin's UI, settings are accessible under the **Advanced Settings** menu:

- **Adapt. First Layer Z Offset**: Toggle the feature on/off.
- **Calibrated Layer Height**: Edit the `calibrated_first_layer_height` float value.
- **Slicer**: Choose between **Prusa** and **Orca**.

---
