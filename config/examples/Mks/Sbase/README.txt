The MKS products (all?) have the EXP1 & EXP2 LCD connectors rotated 180 degrees from the ones on the RepRap LCD controllers.

In order to attach the RepRap Discount Full Graphic Smart Controller you'll need to do something like one of the following for both EXP1 & EXP2:
 a. On one end only, shave the keying plug off the cables and plug the cables in backwards.
 b. On one end only, carefully pry the housings off the board, rotate them 180 degrees and press them back onto the pins.
 c. Make custom cables where one connector is rotated 180 degrees.

     MKS:  1  2  3  4  5  6  7  8  9 10
  RepRap: 10  9  8  7  6  5  4  3  2  1


/////////////////////////////////////////////////////////////////////

3 DEC 2017

The current Marlin 2.0.x firmware cannot properly access some of the pins on the EXP2 connector.  In order to use the RepRap Discount Full Graphic Smart Controller you'll need a custom cable that gets three of the signals from a different connector. In that cable move the pin/wire that:

    used to go to P0.8 to J8-3
    "     "  "  " P0.7 to J8-2
    "     "  "  " P0.5 to J8-4

If pins different than the J8 ones above are used then the spi_pins.h file will need to be modified.

An octopus cable something like the Adafruit 1199 will simply the construction of the custom cable.  Just plug the Adafruit 1199 into one of the cables that came with the LCD and the individual pins into J8 and EXP2 as needed.

Adafruit 10-pin IDC Socket Rainbow Breakout Cable [1199]
  https://www.adafruit.com/product/1199


/////////////////////////////////////////////////////////////////////

27 APR 2018

If you also want a functional SD_DETECT_PIN then you'll need to also move the following pin:

    used to go to P0.27 to J8-5

if you decide to use a different pin then the pins_MKS_SBASE.h file will need to be modified.
