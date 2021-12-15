## Eryone Thinker V2

 - BLTouch probe
 - Filament runout Sensor

## How to use the config

#### Check the hardware of your printer

1. If you're using the stock printer there's need to modify the config.
2. If you're using the Filament Runout Sensor:
  - Enable this line (Remove the "`//`" prefix)<br/>
    `//#define THINKERV2_FL  // Enable for an installed Filament runout sensor`
3. You are using the Bltouch:
  - Enable this line (Remove the "`//`" prefix)<br/>
    `//#define THINKERV2_BLTOUCH  // Enable for an installed BLTOUCH`
4. You are using the direct drive extruder:
  - Enable this line (Remove the "`//`" prefix)<br/>
    `//#define THINKERV2_Direct // Enable for Direct drive extruder system`
5. Can I use all of the mods? The answer is yes. You can use all of them or just some of them.
6. If your printer's PSU has a reset problem you can disable the `PIDTEMPBED` feature:
  - Disable this line (Prefix with "`//`")<br/>
    `#define PIDTEMPBED  // If your PSU has a reset problem then try to disable it to improve this problem`

## Have fun :)
