# P2A/P2B/P2C VFD protocol

The manual can be quite confusing at times about how the RS485
protocol should be implemented. First off, wiring:

Use a MAX-485 and wire it like this:

- GND = GND of Arduino
- A = RS+485
- B = RS-485
- VCC = 5V of Arduino
- DI = TX (can be any D pin, 3.3V or 5V)
- DE = Wire to RE and some pin (can be any D pin, 3.3V or 5V)
- RE 
- RO = RX (can be any D pin, 3.3V or 5V)

If you're not sure if your board can handle the RX signal (which 
can be a bit higher than 3.3V), you should add a voltage level 
converter (or a simple voltage divider). The normal Arduino's handle
the RX signal just fine in my test setup, but the DUE needed a 
voltage level converter.

## VFD settings

**ALWAYS** read the manual for VFD's! This is imperative to 
get motor speed etc. all correct. Also, you need to set 
certain settings to get RS485 working correctly, most notably:

- F0.02 = 7 (use rs485)
- F0.04 = 2 (use rs485)
- F0.09 = 4 (use rs485)
- F9.00 = 4 (19200 baud)
- F9.01 = 0 (this is 8,N,1 parity, for SoftwareSerial) or if you use 
  HardwareSerial (like me) pick whatever suits you. Note the DUE doesn't
  support SoftwareSerial.
- F9.02 = 1 (address)
- F9.05 = 0 (non-std modbus, 1 = std modbus, 2 = ascii)
- F9.07 = 0 (write ops responded)

Note that 19200,8N1 is more than enough for anything you want 
to throw at a VFD. High baud rates will just get you more 
errors -- BUT you need a high baud rate to keep Marlin happy.
It's a bit of a trade-off...

Also note 8N1. If you're using SoftwareSerial, you have no options
and have to use 8N1. If you use Hardware serial (like on a DUE), 
you should set 8E1 (which is the VFD default).

## CRC

The CRC check should be implemented like in the document. They 
often mix up Big Endian and Little Endian unfortunately. The way
it should be ordered is best described by an example:

	01:  Address of device, usually 1
	06:  03 = read, 06 = write, 07 = command
	20:  Byte 1 of 0x2000 (set command)
	00:  Byte 2 of 0x2000
	00:  Byte 1 of 0x0002 (rev run)
	02:  Byte 2 of 0x0002
	03:  crc_value & 0xFF
	CB:  (crc_value >> 8) & 0xFF

For reference, I'll just write this packet down as follows:
`01.06.2000.0002`

# Commands

Most commands simply work with an ID (which they call address)
to make it confusing, and the number of return values you would
like to have. For example:

       Send: 01 03 3000 0004
       // Command 0x3000, number of results = 4
       Recv: 01 03 0008 0002 0000 0000 0000 D285 
                   #bytes #1   #2   #3   #4 CRC

If you want to query the current [running] status, that's 
command 0x3000, and the status is 1 byte, so you might as 
well add `0001` as parameter. There are exceptions here,
obviously when writing data to the EEPROM or the speed. 

I hereby list the most important command sequences, and how
they work:

## Initialization & status

Get current status: `01 03 3000 0001`. The receive data 
contains 1 value, namely the status. This is an enum with
the following values:

- 03 = idle
- 01 = forward running
- 02 = reverse running

Example:

    Send: 01 03 30 00 00 01 8B 0A 
    Recv: 01 03 00 02 00 02 65 CB 


Get max RPM (b0.05): `01 03 B005 0002`

Example:

    Max RPM: b0.05
    Send: 01 03 B0 05 00 02 F2 CA 
    Recv: 01 03 00 04 5D C0 03 F6 D0 21 
                      -- -- = 24000


Get current RPM (d0.12): `01 03 700C 0002`

Note that current RPM doesn't translate 1:1 to a percentage.
So, when setting 10% of the max RPM on a 24.000 rpm spindle, 
we would get:

    Send: 01 03 700C 0002 1EC8 
    Recv: 01 03 0004 095D 0000 D149 
                     ---- 2397 RPM (~ 10%)

## Running

Forward run: `01 06 2000 0001`
Backward run: `01 06 2000 0002`
Stop: `01 06 2000 0006`

Set speed: `01 06 1000 xxxx`
where xxxx is the speed in 1/100 percent of the max. So,
that means a value of 10000 is the max speed (`27 10`)
and a value of 00000 is the min speed (`00 00`).

Note that `01 06` will return the original command, so 
for example:

    Send: 01 06 20 00 00 01 43 CA 
    Recv: 01 06 20 00 00 01 43 CA 

# Implementation details

If sending and receiving collides, the checksum will be 
corrupted. And even if it was received by the VFD, it might 
take time to execute. For example, if we have a FWD run, and 
we want to move to a REV run, this takes time. So, it's 
*ALWAYS* a good idea to send a command, and check if the 
value matches the live values.

For example: if we set 1000 RPM, we want to check if the 
current running speed is 1000 RPM - and until this happens,
we *definitely* want to wait and do nothing. Same for FWD,
REV, STOP, etc.

## Timing and receive data

Timing and receive data is *VERY* tricky. There are buffers 
everywhere, and they need to be flushed and awaited.... 

The timeout of the VFD is *not* 5 ms. It it more like 5 ms 
*iff* the response is immediate. Things get interesting if 
the response is *not* immediate. Apparently, the VFD just 
starts sending stuff, and hopes it can produce the next 
character in time (which is cannot apparently). It can take
up to 20 ms till this little dance is solved.

If it isn't working for you, the problem is probably timing
issues in RS485. You should try a lower baud rate, or start 
with 1200 baud and work your way up...

Another thing you can try (if you have the option) is using 
another TX/RX port. For the DUE, Serial2 appears to work 
better, for some weird reason...