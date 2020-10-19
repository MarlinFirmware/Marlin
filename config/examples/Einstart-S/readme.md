## Einstart S Machine Notes

The Einstart S is a commercial machine that ships with a proprietary firmware and "licensed" slicer program which requires the original SD card or license file (.lic/.reg) from support in order to function. The impetus for the port was that I acquired the machine on the weekend when tech support was closed.

## Specifications:

 - Powder coated, formed steel enclosure
 - Machined Aluminum internal axis support frame
 - Genuine matched HIWIN rails on all axis
 - Genuine Minebea NEMA17 motors (200 steps/rev)
 - THC7128 3A Stepper Drivers (configured for 16 microsteps, loud)
 - 160x160 magnetically coupled non-heated bed
 - PLA-only, Direct Drive extruder
 - SH1106 OLED Display w/ Cardinal navigation
 - 9533 I2C PWM RGB LED Chassis illumination
 - Onboard SD-Card Support

## Initial modifications

### Bootloader/Marlin installation

This machine probably doesn't ship with the arduino boot loader, so unless you reflow a 2560 on there to replace the atmega1280 like I did (to preserve the original firmware), you'll need to use ISP to do an initial firmware load and/or bootloader flash!

It's not that bad though, you can just buy an ISP programmer with the appropriate header, or build an ISP programmer using the Arduino ISP sketch (wiring is easier if you use the "old style wiring" which utilizes  the following pinout: (see the comments in the ArduinoISP sketch).

I used an arduino Nano and a header I had lying around, but you could do the same with a nano and some dupont headers cut and soldered to the board, and taped/glued into a header.

Triple check your pinouts, it shouldn't fry if connected backwards but if your pinout is wrong you're going to struggle for sure.

#### ArduinoISP Sketch pinout for 10pin ISP header using "old style wiring":

See the comments in the ArduinoISP sketch under File:Examples:ArduinoISP

1 MOSI  D11
2 5v    Vcc
3 N/C
4 GND
5 RES   D10
6 N/C
7 SCK   D13
8 N/C
9 MISO  D12
10 GND  GND

#### Einstart Motherboard ISP Header Pinout

The 10 pin ISP Header is located to the right of the ATMEGA chip, just above the long LCD header.

#### ISP Diagram

<--- Face of chassis

MOSI ( 1 ) ( 2 ) Vcc
 N/C ( 3 ) ( 4 ) GND
 RST ( 5 ) ( 6 ) GND
 SCK ( 7 ) ( 8 ) GND
MISO ( 9 ) (10 ) GND

Component Plugs --->

### Motherboard Diagram

 |1234|1234|1234|1234|12|12|12|12|12|12|12|12|12|12|123|12|1234|
 ---------------------------------------------------------------
| XMOT YMOT ZMOT EMOT EF HB HE PF ZS YS XS PS BT ET PT1 TC VBRG |
|     =====     ======                                       [DC|
|======    ======              ____     ISP          IC1__   ___|
|                             |1280|   :::::           |  | |USB|
|                             |____|   1               |__| |___|
|                         BUTTON                            |SD |
|                         GDLRUO  LCD                       |___|
|________________________[......][....................]_________|

Legend:

XMOT    X-Motor
YMOT    Y-Motor
ZMOT    Z-Motor
EMOT    E-Motor
EF      Extruder Fan
HB      Heated Bed  (PWM)
HE      Hot End     (PWM)
PF      Part Fan    (PWM)
ZS      Z-Endstop   (Max)
YS      Y-Endstop   (Min)
XS      X-Endstop   (Min)
PS      SPST Switch to enable DC 24v
BT      Bed Thermistor (header not populated)
ET      Extruder Thermistor
PT1     Pt100 Thermocouple (not populated)
TC      Thermocouple (not populated)
VBRG    LED Output from PCA5933
BUTTONS
G       GND  (buttons are pulled low on activation)
D       DOWN
L       LEFT
R       RIGHT
U       UP
O       OK   (MIDDLE)

## Uploading Marlin firmware to a stock Einstart S

### Summary:

Make, buy or steal a 10 pin ISP programmer and select the appropriate programmer option via Tools:Programmer in Arduino IDE, use the Sketch:Upload Using Programmer command or Shift-Ctrl-U (vs Ctrl-U to upload via serial).

### Detailed procedure:

You can either load Marlin directly via ISP programmer (easiest, but requires opening the chassis every time you want to change/update firmware)  or, just program the bootloader which will enable later uploads via USB. HOWEVER, to actually upload the sketch you will need one of the following:

 - ISP programming header (ArduinoISP sketch, pinout below)
 - OR
 - Install bootloader on atmega1280 to enable native USB uploads (also requires ISP).
 - AND
 - Connect button between RESET and GND on ISP header, release post compile just before upload starts.
 - OR
 - Install permanent modification described below, requires no button press during upload.

### Permanent modification to enable USB uploads via Arduino IDE

As a permanent modification to enable direct USB upload from the arduino IDE, you would need to flash the boot loader using ISP first; but then you can solder a wire or 100n capacaitor between the DTR line on the FTDI to the reset line, which is conveniently located in several places on the board.

#### FTDI Chip Diagram (IC1)

           ______
   TXD  --|1   28|--OSCO
   DTR  --|2   27|--OSCI
   RTS  --|3   26|--TEST
   VCCIO--|4   25|--AGND
   RXD  --|5   24|--N/C
   RI   --|6   23|--CBUS0
   GND  --|7   22|--CBUS1
   N/C  --|8   21|--GND
   DSR  --|9   20|--Vcc
   DCD  --|10  19|--RES
   CTS  --|11  18|--GND
   CBUS4--|12  17|--3V3
   CBUS2--|13  16|--USBDM
   CBUS3--|14  15|--USBDP
          |______|

#### Arduino Reset Circuit

   DTR-----||----GND----/\/\/\/----Vcc
          100nf           10k

With this modification you no longer need the reset button and you can program directly from the Arduino IDE.

### Power notes during flashing.

You do NOT need power connected when flashing via ISP.
You DO need power when flashing via USB Serial but you MUST time the release of the reset line to allow the upload unless you've performed the permanent modification.
