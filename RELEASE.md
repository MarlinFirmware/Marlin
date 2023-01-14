Created directly from the Marlin-Makaira baseline.
Set up preheat presets for 210, 215. 220 and 230 C plus a cooling "preheat".
TODO menu timeout increased
For RECREATOR MK5, connected heat block fan to heat bed terminals and assign in to FAN_PIN in pins_CREALTY_V4.h; 
     assigned part cooling fan to FAN1_PIN so it can be controlled from the Marlin menu.

Regression Test
---------------
- Four preheat settings, including Cooling?
- Does not HALT after stopping print from SD Card?
- Preheat Cooling activates fan on heat block?
- FAN 2 in menu activates filament cooling fan?
