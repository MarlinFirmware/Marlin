### L64XX Stepper Driver

*Arduino-L6470* library revision 0.8.0 or above is required.

This software can be used with the L6470, L6474, L6480 and the powerSTEP01 (collectively referred to as "L64xx" from now on). Different drivers can be mixed within a system.

These devices use voltage PWMs to drive the stepper phases. On the L6474 the phase current is controlled by the `TVAL` register. On all the other drivers the phase current is indirectly controlled via the `KVAL_HOLD` register which scales the PWM duty cycle.

This software assumes that all drivers are in one SPI daisy chain.

### Hardware Setup

- MOSI from controller tied to SDI on the first device

- SDO of the first device is tied to SDI of the next device

- SDO of the last device is tied to MISO of the controller

- All devices share the same `SCK_PIN` and `SS_PIN` pins. The user must supply a macro to control the `RESET_PIN`(s).

- Each L6470 passes the data it saw on its SDI to its neighbor on the **NEXT** SPI cycle (8 bit delay).

- Each L6470 acts on the **last** SPI data it saw when the `SS_PIN` **goes high**.

The L6474 uses the standard STEP DIR interface. Phase currents are changed in response to step pulses. The direction is set by the DIR pin. Instead of an ENA pin, stepper power is controlled with SPI commands.

The other drivers operate in `STEP_CLOCK` mode. In this mode the Direction / Enable functions are done with SPI commands and the phase currents are changed in response to STEP pulses.

### Hardware / Software Interaction

Except for the L6474, powering up a stepper and setting the direction are done by the same command. You can't do one without the other.

**All** directions are set **every time** a new block is popped off the queue by the stepper ISR.

When setting direction, SPI transfers are minimized by using arrays and a specialized SPI method. *Arduino-L6470* library calls are not used. For N L64xx drivers, this results in N bytes transferred. If library calls were used then N<sup>2</sup> bytes would be sent.

### Power-up (Reset) Sequence

- Stepper objects are instantiated before the `setup()` entry point is reached.

- In `setup()` (before stepper drivers are initialized) the `L6470_init()` method is called to do the following:

  - If present, pulse the hardware reset pin.

  - Populate the `L6470_chain` array, which maps positions in the SPI stream to commands/data for L64XX stepper drivers.

  - Initialize the L64XX Software SPI pin states.

  - Initialize L64XX drivers. They may be reset later by a call to `L6470_init_to_defaults()`.

The steppers are **NOT** powered up (enabled) during this sequence.

### `L6470_chain` array

This array is used by all routines that transmit SPI data. For a chain with N devices, the array contains:

Index|Value
-----|-----
0|Number of drivers in chain
1|Axis index of the first device in the chain (closest to MOSI)
...|
N|Axis index of the last device chain (closest to MISO)

### Set Direction and Enable

The `DIR_WRITE` macros for the L64xx drivers are written so that the standard X, Y, Z and extruder logic used by the `set_directions()` routine is not altered. These macros write the correct forward/reverse command to the corresponding location in the array `L6470_dir_commands`. On the L6474 the array the command used just enables the stepper because direction is set by the DIR pin.

At the end of the `set_directions()` routine, the array `L6470_chain` is used to grab the corresponding direction/enable commands out of the array `L6470_dir_commands` and put them in the correct sequence in the array `L6470_buf`. Array `L6470_buf` is then passed to the **`void`** `L6470_Transfer` function which actually sends the data to the devices.

### Utilities, etc.

The **absolute position** registers should accurately reflect Marlin’s stepper position counts. They are set to zero during initialization. `G28` sets them to the Marlin counts for the corresponding axis after homing. NOTE: These registers are often the negative of the Marlin counts. This is because the Marlin counts reflect the logical direction while the registers reflect the stepper direction. The register contents are displayed via the `M114 D` command.

The `L6470_monitor` feature reads the status of each device every half second. It will report if there are any error conditions present or if communications has been lost/restored. The `KVAL_HOLD` value is reduced every 2 – 2.5 seconds if the thermal warning or thermal shutdown conditions are present.

**M122** displays the settings of most of the bits in the status register plus a couple of other items.

**M906** can be used to set the `KVAL_HOLD` register (`TVAL` on L6474) one driver at a time. If a setting is not included with the command then the contents of the registers that affect the phase current/voltage are displayed.

**M916, M917 & M918**

These utilities are used to tune the system. They can get you in the ballpark for acceptable jerk, acceleration, top speed and `KVAL_HOLD` settings (`TVAL` on L6474). In general they seem to provide an overly optimistic `KVAL_HOLD` (`TVAL`) setting because of the lag between setting `KVAL_HOLD` (`TVAL`) and the driver reaching final temperature. Enabling the `L6470_monitor` feature during prints will provide the **final useful setting**.

The amount of power needed to move the stepper without skipping steps increases as jerk, acceleration, top speed, and micro-steps increase. The power dissipated by the driver increases as the power to the stepper increases. The net result is a balancing act between jerk, acceleration, top speed, micro-steps, and power dissipated by the driver.

**M916** - Increases `KVAL_HOLD` (`TVAL`) while moving one axis until a thermal warning is generated. This routine is also useful for determining the approximate `KVAL_HOLD` (`TVAL`) where the stepper stops losing steps. The sound will get noticeably quieter as it stops losing steps.

**M917** - Find minimum current thresholds. This is accomplished by doing the following steps while moving an axis:

1. Decrease OCD current until overcurrent error.

2. Increase OCD until overcurrent error goes away.

3. Decrease stall threshold until stall error (not available on the L6474).

4. Increase stall until stall error goes away (not available on the L6474).

**M918** - Increase speed until error or max feedrate achieved.
