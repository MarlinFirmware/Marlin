# Auto First Layer Z Adjust (`AUTO_FIRST_LAYER_Z_ADJUST`)

The **Auto First Layer Z Adjust** feature automatically adjusts the first layer nozzle height when printing. It computes and applies a Z-offset correction dynamically, based on the difference between the actual first layer height specified in the sliced G-code and the printer's calibrated first layer height (the height for a perfect initial layer with the tuned Z offset).

---

## 1. Overview & Functionality

In 3D printing, bed leveling and Z-offset calibration are typically performed at a specific height or assume a standard first-layer height (e.g., `0.3 mm`). However, when you slice a 3D model with a different first layer height (e.g., `0.2 mm`), the nozzle can end up too close or too far from the build plate. This mismatch often requires manual tuning of the Z-offset (babystepping) to prevent first-layer failure.This is even more accurate if you print with various nozzle sizes and (or) a wide range of layer heights.

When `AUTO_FIRST_LAYER_Z_ADJUST` is enabled, Marlin intercepts and adjusts the initial first-layer height set by the G-code. Marlin computes the difference relative to the printer's pre-calibrated layer height using the following calculation:

`Delta = Z_Gcode - Z_Calibrated`

This delta is then applied directly to the movement of the first layer, ensuring that the nozzle prints at the correct distance from the bed without requiring you to re-slice the model or manually adjust babysteps.

### Key Implementation Details

1. **Safety Margin**:
   To prevent the nozzle from scraping the bed or over-squishing the first layer, a built-in safety margin of `0.05 mm` is enforced.
   - If the absolute difference is within the safety margin (absolute value of Delta is less than or equal to 0.05 mm), no correction is applied.
   - If the difference exceeds 0.05 mm, the correction is applied after incorporating the safety margin:
     `Applied Delta = Delta + 0.05 mm`
     *(For example, if Z_Gcode = 0.2 mm and Z_Calibrated = 0.3 mm, the raw Delta is -0.1 mm. With the safety margin applied, the final adjustment is -0.05 mm, moving the nozzle 0.05 mm closer to the bed instead of the full 0.1 mm.)*

2. **Slicer Integration**:
   Different slicers output the initial Z-height G-code commands differently. To handle this, Marlin tracks the slicer family:
   - **PrusaSlicer and clones**: The first Z-only movement command determines the first layer height.
   - **OrcaSlicer and clones**: These slicers output an initial Z-hop. When OrcaSlicer mode is selected, Marlin ignores the first Z-only command (treating it as the Z-hop) and reads the first-layer height from the second Z-only command.

---

## 2. G-Code Command: `M429`

Use `M429` to view the current status, toggle the feature, change the calibrated height, or specify the slicer format.

### Syntax
```gcode
M429 [S<bool>] [H<float>] [O<int>]
```

### Parameters
* **`S<bool>`**: Enable (`S1`) or disable (`S0`) the Adaptive First Layer Z Offset adjustment.
* **`H<float>`**: Set the calibrated first layer height (in mm). Accepts values in the range `0.0` to `1.0`.
* **`O<int>`**: Set the slicer type to ensure proper parser compatibility:
  * `0`: **PrusaSlicer** (and clones)
  * `1`: **OrcaSlicer** (and clones)

---

### Examples

* **Show Current Settings**:
  Run `M429` with no parameters:
  
  Response:

    ```
    Auto First Layer Z Adjust Activated
    Configured Layer Height 0.30 mm
    PrusaSlicer
    ```

* **Enable and Configure for OrcaSlicer at 0.3 mm**:
  ```
  M429 S1 H0.3 O1
  ```

* **Disable**:
  ```
  M429 S0
  ```

---

## 3. Configuration & EEPROM Settings

### Firmware Configuration (`Configuration.h`)
To use this feature, it must be enabled in your firmware configuration. Modify the following parameters in **Configuration.h**

```cpp
#define AUTO_FIRST_LAYER_Z_ADJUST
#if ENABLED(AUTO_FIRST_LAYER_Z_ADJUST)
  #define CALIBRATED_FIRST_LAYER_Z_HEIGHT 0.3f    // (mm) Pre-calibrated assumed first layer height
  //#define DEFAULT_SLICER UNKNOWN                // :[ 'UNKNOWN', 'ORCA', 'PRUSA' ]
#endif
```

### Saving Settings to EEPROM
Once configured via `M429`, the settings can be written to the printer's EEPROM so that they persist across power cycles:

* Save settings to EEPROM: `M500`
* Restore settings from EEPROM: `M501`

---

## 4. LCD Menu Controls
If your printer runs Marlin's UI, settings are accessible under the **Advanced Settings** menu:

* **Adapt. First Layer Z Offset**: Toggle the feature on/off.
* **Calibrated Layer Height**: Edit the `calibrated_first_layer_height` float value.
* **Slicer**: Choose between **Prusa** and **Orca**.

---
