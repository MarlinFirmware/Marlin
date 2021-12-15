# BigTreeTech SKR E3 Turbo in Ender 5 Pro

This board is larger than the other SKR Mini E3 and Creality boards and interferes with the power supply in the Ender 5 Pro electronics case.

I (lpla) did a simple adapter for the power supply that can use short M8 screws and bolts (glued to this adapter): https://www.thingiverse.com/thing:4676935

That way the PSU gives enough space for the board to be drop-in replace. So all components fit inside the electronics case and are hold properly.

Regarding wiring, electronics case fan should be connected to the FAN2 pins.

This folder contains configuration files for Ender 5 Pro using v2.0.X Marlin.

To compile, just use these config files and set `default_envs = LPC1769` in platformio.ini file.
