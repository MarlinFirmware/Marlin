### RepRap Discount Full Graphics Smart Controller

Many MKS products have the EXP1 & EXP2 LCD connectors rotated 180 degrees from the ones on the RepRap LCD controllers. So there are three options for connecting the RepRap Discount Full Graphic Smart Controller:
  1. Slice, file, or grind the key off one end of the cables and plug the cables in backwards.
  2. Carefully pry the plastic housings off the board or controller, rotate them 180 degrees, and press them back onto the board.
  3. Make custom cables with one end's connector rotated 180 degrees.

---

#### 3 DEC 2017

The current Marlin 2.0.x firmware can't access some of the pins on the EXP2 connector. A custom cable that gets three of the signals from a different connector is required to use the RepRap Discount Full Graphic Smart Controller. Use that cable to move the following connections:

From|To
--|--
`P0.8`|`J8-3`
`P0.7`|`J8-2`
`P0.5`|`J8-4`

If using pins other than the J8 pins listed above the `spi_pins.h` file will need to be modified.

An octopus cable like the [Adafruit 1199](https://www.adafruit.com/product/1199) 10-pin IDC Socket Rainbow Breakout Cable can simplify the construction of the custom cable. Just plug the Adafruit 1199 into one of the cables that came with the LCD and the individual pins into J8 and EXP2 as needed.

---

#### 27 APR 2018

For a functional `SD_DETECT_PIN`, also move the following pin:

From|To
--|--
`P0.27`|`J8-5`

If a different pin is used, modify the `pins_MKS_SBASE.h` file.
