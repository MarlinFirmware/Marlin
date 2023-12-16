# LeoTechLab SV06 v0.1 Firmware (15-12-2023)

**Follow me on YouTube: https://www.youtube.com/@LeoTechLab**

This is a stock Marlin fork with Sovol SV06 configurations based on the original Sovol firmware (https://github.com/Sovol3d/Sv06-Source-Code).

Part of the configuration was based on Michael Hill's work, here: https://github.com/hillsoftware/sv06

Currently this is tracking the **bugfix-2.1.x** branch.

# Highlights
- Up to date with branch **bugfix-2.1.x** (15-12-2023)
- **Automatic Bed Leveling (Bilinear)**:
    - 3x3, so that it is easy to manually override the mesh if required
    - Automatically activated after powering on and after homing (G28)
- **X-Twist Compensation**: Compensates for twisted X axis, using three manual measurements
- **Z-Probe Wizard**: Adjust the Z-Probe offset
- **Runout sensor**: LOW and HIGH polarity releases

# Installation
1. **Install the new firmware as usual**
    - put the .bin file inside a FAT32-formatted sdcard
    - put the sdcard in the printer and power it on
    - wait a couple of seconds until the new firmware appears on screen
    - just to be sure, I suggest removing the sdcard and deleting the firmware file
    - NOTE: if the printer does not update, try renaming the firmware file. The printer has a safety feature that stores the last flashed filename, so that it doesn't reflash it again accidentally.
2. **Reset EEPROM and restore defaults, save settings**
    - Configuration / Advanced / Initialize EEPROM
    - Configuration / Restore Defaults
    - Configuration / Save Settings
3. **Auto-home**
    - Motion / Homing / Auto Home
4. **Turn on runout sensor and power recovery, save settings**
    - Configuration / Runout Sensor ON
    - Configuration / Power Outage ON
5. **Auto Z-Align or manual Z align**
    - Motion / Auto Z-Align
6. **PID Autotune for extruder and bed, save settings**
    - Configuration / Advanced / Temperature / Autotune E1 PID: 200
    - Configuration / Advanced / Temperature / Autotune Bed PID: 60
7. **Z-Probe Wizard, save settings**
    - Motion / Bed Leveling / Z Probe Wizard
8. **ABL or input mesh manually via terminal, save settings (M500 for terminal)**
    - Motion / Bed Leveling / Level Bed
9. **Test print to check first layer** 
    - LeoCalibration.stl in the "calibration" folder

Done!

---

Disclaimer: **Use it at your own risk.**
