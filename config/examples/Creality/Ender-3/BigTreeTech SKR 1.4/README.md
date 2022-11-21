## SKR 1.4 Board

These files configure Marlin for an Ender 3 with `BOARD_BTT_SKR_V1_4` board installed with silent stepper drivers and a BLTouch probe. The probe is configured to use the dedicated BLTouch port on the board, so you can remove the Z endstop entirely.

Features included:

 - BLTouch
 - TMC2208 Stepper Drivers
 - Advanced Pause for Filament Change (`M600`)
 - Nozzle Parking

### Nozzle to Probe Offsets

You'll need to to adjust the `NOZZLE_TO_PROBE_OFFSET` values according to your probe's mounting location. This example is based on the Ender 3 BLTouch mount posted at https://www.thingiverse.com/thing:3584158
