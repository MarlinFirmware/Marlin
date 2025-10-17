# Startup sequence

When initialized, MMU sends

- MMU => 'start\n'

We follow with

- MMU <= 'S1\n'
- MMU => 'ok<_Firmware version_>\n'
- MMU <= 'S2\n'
- MMU => 'ok<_Build number_>\n'

#if (12V_mode)

- MMU <= 'M1\n'
- MMU => 'ok\n'

#endif

- MMU <= 'P0\n'
- MMU => '<_FINDA status_>\n'

Now we are sure MMU is available and ready. If there was a timeout or other communication problem somewhere, printer will be killed.

- <_Firmware version_> is an integer value, but we don't care about it.
- <_Build number_> is an integer value and has to be >=126, or =>132 if 12V mode is enabled.
- <_FINDA status_> is 1 if the filament is loaded to the extruder, 0 otherwise.

<_Build number_> is checked against the required value, if it does not match, printer is halted.

# Toolchange

- MMU <= 'T<_Filament index_>\n'

MMU sends

- MMU => 'ok\n'

as soon as the filament is fed down to the extruder. We follow with:

- MMU <= 'C0\n'

MMU will feed a few more millimeters of filament for the extruder gears to grab.
When done, the MMU sends

- MMU => 'ok\n'

We don't wait for a response here but immediately continue with the next G-code which should
be one or more extruder moves to feed the filament into the hotend.

# FINDA status

- MMU <= 'P0\n'
- MMU => '<_FINDA status_>\n'

_FINDA status_ is 1 if the is filament loaded to the extruder, 0 otherwise. This could be used as filament runout sensor if probed regularly.

# Load filament

- MMU <= 'L<_Filament index_>\n'

MMU will feed filament down to the extruder, when done:

- MMU => 'ok\n'

# Unload filament

- MMU <= 'U0\n'

MMU will retract current filament from the extruder, when done:

- MMU => 'ok\n'

# Eject filament

- MMU <= 'E<_Filament index_>\n'
- MMU => 'ok\n'
