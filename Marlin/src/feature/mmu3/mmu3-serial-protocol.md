MMU3 Messages
=============

Starting with the version 2.0.19 of the MMU firmware, the requests and responds
have a trailing section that contains the CRC8 of the original message. The
general structure is as follows:

```shell
Requests (what Marlin requests):
MMU2:>{RequestMsgCode}{Value}*{CRC8}\n

Responses (what MMU responds with):
MMU2:<{RequestMsgCode}{Value} {ResponseMsgParamCode}{paramValue}*{CRC8}\n
```

An example to that would be:

```shell
MMU2:>S0*c6\n
MMU2:<S0 A3*22\n

MMU2:>S1*ad\n
MMU2:<S1 A0*34\n

MMU2:>S2*10\n
MMU2:<S2 A2*65\n
```

All combined we have a response of v3.0.2 as the firmware version.

Startup sequence
================

When initialized, MMU waits for requests. Marlin repeatedly sends `S0` commands
until it gets an answer:

```shell
MMU2:>S0*c6\n
MMU2:>S0*c6\n
MMU2:>S0*c6\n
MMU2:>S0*c6\n
MMU2:>S0*c6\n
MMU2:>S0*c6\n
MMU2:>S0*c6\n
...
```

When the communication is in place MMU follows with:

```shell
MMU2:<S0 A3*22\n
```

Then Marlin continues to get the rest of the MMU firmware version.

```shell
MMU2:>S1*ad\n
MMU2:<S1 A0*34\n
MMU2:>S2*10\n
MMU2:<S2 A2*65\n
```

Setting the mode to SpreadCycle (M0) or StealthChop (M1)

```shell
MMU2:>M1*{CRC8};
MMU2:<---nothing---
```

#endif

```shell
MMU2:>P0
MMU2:<P0 A{FINDA status}*{CRC8}\n
```

Now we are sure MMU is available and ready. If there was a timeout or other
communication problem somewhere, printer will not be killed this would be
stupid, the MMU feature will be disabled.

- *Firmware version* is an integer value, and we care about it. As there is no
  otherway of knowing which protocol to use.
- *FINDA status* is 1 if the filament is loaded to the extruder, 0 otherwise.

*Firmware version* is checked against the required value, if it does not match,
printer will not be halted, only the MMU feature will be disabled.


Toolchange
==========

```shell
MMU2:>T{Filament index}*{CRC8}\n
MMU2:<Q0*ea\n
```

MMU sends

```shell
MMU2:<T{filament index}*P{ProgressCode}{CRC8}\n
```

Which in normal operation would be as follows, let's say that we requested MMU
to load `T0``:

```shell
MMU2:>T0*{CRC8}\n

MMU2:>Q0*{CRC8}\n
MMU2:<T0*P5{CRC8}\n  # P5 => FeedingToFinda

MMU2:>Q0*{CRC8}\n
MMU2:<T0*P7{CRC8}\n  # P7 => FeedingToNozzle
```

as soon as the filament is fed down to the extruder. We follow with:

```shell
MMU2:>C0*{CRC8}\n
```

MMU will feed a few more millimeters of filament for the extruder gears to
grab. When done, the MMU sends

```shell
MMU2:>Q0*{CRC8}\n
MMU2:<T0*P9{CRC8}\n  # P9 => FinishingMoves
```

After the `T0*P9` response we immediately continue with the next G-code which
should be one or more extruder moves to feed the filament into the hotend.


FINDA status
============

```shell
MMU2:>P0*{CRC8}\n
```

If the filament is loaded to the extruder, FINDA status is 1 and MMU responds
with:

```shell
MMU2:<P0 A1*{CRC8}\n
```

otherwise:

```shell
MMU2:<P0 A0*7b\n
```

This could be used as filament runout sensor if probed regularly.


Load filament
=============

To load a filament to the MMU itself, we run:

```shell
MMU2:>L{Filament index}*{CRC8}\n
MMU2:<L{Filament index} A1*{CRC8}\n
```

and immediately after that we query the status:

```shell
MMU2:>Q0*{CRC8}\n
```

MMU will respond with status messages:

```shell
MMU2:<L0*P5{CRC8}\n
```

MMU will load the filament and when done:

```shell
MMU2:>Q0*{CRC8}\n
MMU2:<L0*P9{CRC8}\n
```

Unload filament
=============

- MMU <= 'U0\n'

MMU will retract current filament from the extruder, when done

- MMU => 'ok\n'



Eject filament
==============

- MMU <= 'E*Filament index*\n'
- MMU => 'ok\n'
