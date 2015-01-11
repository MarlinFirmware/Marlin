

__Note for RAMPS users:__
----------------------

-By default, RAMPS have no power on servo bus (if you happen to have a multimeter, check the voltage on servo power pins).
-In order to get the servo working, you need to supply 5V to 5V pin.. You can do it using your power supply (if it has a 5V output) or jumping the "Vcc" from Arduino to the 5V RAMPS rail.
-These 2 pins are located just between the Reset Button and the yellow fuses... There are marks in the board showing 5V and VCC.. just connect them..
-If jumping the arduino Vcc do RAMPS 5V rail, take care to not use a power hungry servo, otherwise you will cause a blackout in the arduino board ;-)
