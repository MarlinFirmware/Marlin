# Example Configuration for BQ [Hephestos 2](http://www.bq.com/uk/hephestos-2)
This configuration file is based on the original configuration file shipped with the heavily modified Marlin fork by BQ. The original firmware and configuration file can be found at [BQ Github repository](https://github.com/bq/Marlin).

NOTE: The look and feel of the Hephestos 2 while navigating the LCD menu will change by using the original Marlin firmware.

## Changelog
 * 2016/03/01 - Initial release
 * 2016/03/21 - Activated 4-point auto leveling by default
                Updated miscellaneous z-probe values
 * 2016/06/21 - Disabled hot bed related options
                Activated software endstops
                SD printing now disables the heater when finished
 * 2016/07/13 - Update the `DEFAULT_AXIS_STEPS_PER_UNIT` for the Z axis
                Increased the `DEFAULT_XYJERK`
