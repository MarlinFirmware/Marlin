# Ender 3 Pro with SKR 1.4 Turbo + TMC2209

Configurations for Ender 3 Pro with the following options enabled:

  - Motherboard: BigTreeTech SKR 1.4 Turbo
  - Drivers: TMC2209 (sensorless homing enabled)
  - BLTouch Probe plugged into in the PROBE port - ABL Bilinear 5x5
  - Automatic Filament Load / Unload
    - Measure the full filament path up to the nozzle and update `FILAMENT_CHANGE_UNLOAD_LENGTH` and `FILAMENT_CHANGE_FAST_LOAD_LENGTH`.
  - Linear Advance Extrusion (Do a calibration for your correct values)
  - TFT 35V3 Controller with two options:
    - EXP3 + TFT cables using `CR10_STOCKDISPLAY`, or
    - EXP1/2 with `REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER`
  - Extruder Auto Fan at 50C (Fan connected to Pin P2_04 HE1)

I may have some other options enabled that I don't remember so be careful to check everything that corresponds to your machine.
