Startup sequence
================

When initialized, MMU sends

- MMU => 'start\n'

We follow with

- MMU <= 'S1\n'
- MMU => 'ok*Firmware version*\n'
- MMU <= 'S2\n'
- MMU => 'ok*Build number*\n'

#if (12V_mode)

- MMU <= 'M1\n'
- MMU => 'ok\n'

#endif

- MMU <= 'P0\n'
- MMU => '*FINDA status*\n'

Now we are sure MMU is available and ready. If there was a timeout or other communication problem somewhere, printer will be killed.

- *Firmware version* is an integer value, but we don't care about it
- *Build number* is an integer value and has to be >=126, or =>132 if 12V mode is enabled
- *FINDA status* is 1 if the filament is loaded to the extruder, 0 otherwise


*Build number* is checked against the required value, if it does not match, printer is halted.



Toolchange
==========

- MMU <= 'T*Filament index*\n'

MMU sends

- MMU => 'ok\n'

as soon as the filament is fed down to the extruder. We follow with

- MMU <= 'C0\n'

MMU will feed a few more millimeters of filament for the extruder gears to grab.
When done, the MMU sends

- MMU => 'ok\n'

We don't wait for a response here but immediately continue with the next gcode which should
be one or more extruder moves to feed the filament into the hotend.


FINDA status
============

- MMU <= 'P0\n'
- MMU => '*FINDA status*\n'

*FINDA status* is 1 if the is filament loaded to the extruder, 0 otherwise. This could be used as filament runout sensor if probed regularly.



Load filament
=============

- MMU <= 'L*Filament index*\n'

MMU will feed filament down to the extruder, when done

- MMU => 'ok\n'


Unload filament
=============

- MMU <= 'U0\n'

MMU will retract current filament from the extruder, when done

- MMU => 'ok\n'



Eject filament
==============

- MMU <= 'E*Filament index*\n'
- MMU => 'ok\n'

