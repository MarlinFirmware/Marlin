## Creality Ender-5 Pro with BTT SKR Mini E3

The configuration was made on a Phaetus Dragon hotend and a BMG clone extruder, running the Leon-Me Gen 5 cooling shroud with dual 5015s.

### Changes:

#### Configuration.h

- Set `SERIAL_PORT` to `2`
- Set `SERIAL_PORT_2` to `-1`
- Set `MOTHERBOARD` to `BOARD_BTT_SKR_MINI_E3_V2_0`
- Enabled `PIDTEMPBED` and set default values
- Set `EXTRUDE_MAXLENGTH` to `600` to allow BMG extruder load/unload
- Set `[XYZE]_DRIVER_TYPE` to `TMC2209`
- Enabled `CLASSIC_JERK` and set default values
- Disabled `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN`
- Enabled `USE_PROBE_FOR_Z_HOMING`
- Set `Z_MIN_PROBE_PIN` to `PC14`
- Enabled `BLTOUCH`
- Set `PROBING_MARGIN` to `8`
- Set `XY_PROBE_FEEDRATE` and `Z_PROBE_FEEDRATE_FAST` to faster values
- Set `MULTIPLE_PROBING` to 2
- Set `INVERT_[XYZE]_DIR` to `true`
- Set `X_BED_SIZE` to `230` to regain bed size
- Set `Y_BED_SIZE` to `225` to regain bed size
- Enabled `SOFT_ENDSTOPS_MENU_ITEM`
- Enabled `AUTO_BED_LEVELING_BILINEAR`
- Enabled `RESTORE_LEVELING_AFTER_G28`
- Enabled `PREHEAT_BEFORE_LEVELING` and set default values
- Enabled `G26_MESH_VALIDATION`
- Set `GRID_MAX_POINTS_X` to `9`
- Enabled `EXTRAPOLATE_BEYOND_GRID`
- Enabled `MESH_EDIT_GFX_OVERLAY`, set `MESH_INSET` to `10` and `GRID_MAX_POINTS_X` to `9` (for UBL)
- Enabled `LCD_BED_LEVELING`
- Enabled `MESH_EDIT_MENU`
- Enabled `LEVEL_BED_CORNERS`
- Enabled `Z_SAFE_HOMING`
- Set `HOMING_FEEDRATE_MM_M` to faster values
- Enabled `NOZZLE_PARK_FEATURE`
- Disabled `SPEAKER` to work around fan stuck at 100% issue
- Enabled `FAN_SOFT_PWM` for my dual 5015 setup

#### Configuration_adv.h

- Enabled `USE_CONTROLLER_FAN`
- Enabled `CONTROLLER_FAN_EDITABLE`
- Set `BLTOUCH_DELAY` to `500`
- Enabled `PROBE_OFFSET_WIZARD`
- Enabled `BROWSE_MEDIA_ON_INSERT`
- Enabled `LONG_FILENAME_HOST_SUPPORT`
- Set `SDCARD_CONNECTION` to `ONBOARD`
- Enabled `BABYSTEP_ZPROBE_OFFSET` and `BABYSTEP_ZPROBE_GFX_OVERLAY`
- Enabled `LIN_ADVANCE` and set default value
- Enabled `ARC_P_CIRCLES`
- Enabled `ADVANCED_PAUSE_FEATURE`
- Set `FILAMENT_CHANGE_UNLOAD_LENGTH` to `500`
- Enabled `ADVANCED_PAUSE_CONTINUOUS_PURGE`
- Set `ADVANCED_PAUSE_PURGE_LENGTH` to `600`
- Enabled `PARK_HEAD_ON_PAUSE`
- Set all `SLAVE_ADDRESS` to SKR values
- Set `[XY]_STALL_SENSITIVITY` to `50`
- Enabled `IMPROVE_HOMING_RELIABILITY`
