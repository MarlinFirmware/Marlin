# Creality Ender-5 Leadscrew Changes

In mid-to-late 2019, Creality upgraded the base Ender-5 leadscrew to the same one used on the Ender-5 Pro which prevents the bed from dropping once power is cut.

If your machine shipped with the upgraded leadscrew (or if your prints are 1/2 as tall as they should be), change your Z steps from `400` to `800` in `Configuration.h`:

Before:
`DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 93 }`

After:
`DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 800, 93 }`

Or by sending `M92 Z800` followed by `M500` after updating your firmware.
