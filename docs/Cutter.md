### Introduction

With Marlin version 2.0.9.x or higher, Laser improvements were introduced that enhance inline functionality. Previously the inline feature option was not operational without enabling and recompiling the source. Also with inline enabled the base features are not functional. With v2.0.9.x new functionality is added which allows the standard and inline modes to be G-Code selectable and also compatible with each other. Additionally an experimental dynamic mode is also available. Spindle operational features are available with defines and recompiling.

### Architecture

Laser selectable feature capability is defined through 4 global mode flags within G-code ,laser/spindle, planner and stepper routines. The default mode maintains the standard laser function. G-Codes are received, processed and parsed to determine what mode to set through M3, M4 and M5 commands. When the inline mode parameter set is detected, laser power processing will be driven through the planner and stepper routines. Handling of the initial power values and settings are performed by G-Code parsing and the laser/spindle routines.

Inline power feeds from the block->inline_power variable into the planner's laser.power when in continuous power mode. Further power adjustment will be applied if the laser power trap feature is active otherwise laser.power is used as set in the stepper for the entire block. When laser power trap is active the power levels are step incremented during acceleration and step decremented during deceleration.

Two additional power sets are fed in the planner by features laser power sync and laser fan power sync. Both of these power sets are done with planner sync block bit flags. With laser power sync, when the bit flag is matched the global block laser.power value is updated from laser/spindle standard M3 S-Value power sets. For laser fan sync, power values are updated into the planner block->fan_speed[i] variable from fan G-Code S-Value sets.

With dynamic inline power mode, F-Value feedrate sets are processed with cutter.calc_dynamic_power() and fed into the planner laser.power value.

Irrespective of what laser power value source is used, the final laser output pin is always updated using the laser/spindle code. Specifically the apply_power(value) call is used to set the laser or spindle output. This call permits safe power control in the event that a sensor fault occurs.

Note: Spindle operation is not selectable with G-Codes at this time.

The following flow charts depict the flow control logic for spindle and laser operations in the code base.

#### Spindle Mode Logic:

                 ┌──────────┐  ┌───────────┐  ┌───────────┐
                 │M3 S-Value│  │Dir !same ?│  │Stepper    │
                 │Spindle   │  │stop & wait│  │processes  │
              ┌──┤Clockwise ├──┤ & start   ├──┤moves      │
    ┌──────┐  │  │          │  │spindle    │  │           │
    │G-Code│  │  └──────────┘  └───────────┘  └───────────┘
    │Send  ├──┤  ┌──────────┐  ┌───────────┐  ┌───────────┐
    └──────┘  │  │M4 S-Value│  │Dir !same ?│  │Stepper    │
              ├──┤Spindle   ├──┤stop & wait├──┤processes  │
              │  │Counter   │  │& start    │  │moves      │
              │  │Clockwise │  │spindle    │  │           │
              │  └──────────┘  └───────────┘  └───────────┘
              │  ┌──────────┐  ┌────────┐
              │  │M5        │  │Wait for│
              │  │Spindle   ├──┤move &  │
              └──┤Stop      │  │disable │
                 └──────────┘  └────────┘
                 ┌──────────┐  ┌──────────┐
     Sensors─────┤Fault     ├──┤Disable   │
                 └──────────┘  │power     │
                               └──────────┘

#### Laser Mode Logic:

                 ┌──────────┐  ┌─────────────┐  ┌───────────┐
                 │M3,M4,M5 I│  │Set power    │  │Stepper    │
              ┌──┤Standard  ├──┤Immediately &├──┤processes  │
              │  │Default   │  │wait for move│  │moves      │
              │  │          │  │completion   │  │           │
              │  └──────────┘  └─────────────┘  └───────────┘
              │  ┌──────────┐  ┌───────────┐  ┌───────────┐  ┌────────────┐  ┌────────────┐  ┌────────────┐  ┌───────────┐
    ┌──────┐  │  │M3 I      │  │G0,G1,G2,G4│  │Planner    │  │Planner     │  │Planner fan │  │Planner     │  │Stepper    │
    │G-Code│  │  │Continuous│  │M3 receive │  │sets block │  │sync power ?│  │sync power ?│  │trap power ?│  │uses block │
    │Send  ├──┼──┤Inline    ├──┤power from ├──┤power using├──┤process M3  ├──┤process fan ├──┤adjusts for ├──┤values to  │
    └──────┘  │  │          │  │S-Value    │  │Gx S-Value │  │power inline│  │power inline│  │accel/decel │  │apply power│
              │  └──────────┘  └───────────┘  └───────────┘  └────────────┘  └────────────┘  └────────────┘  └───────────┘
              │  ┌──────────┐  ┌───────────┐  ┌────────────────┐  ┌───────────┐
              │  │M4 I      │  │Gx F-Value │  │Planner         │  │Stepper    │
              │  │Dynamic   │  │set power  │  │Calc & set block│  │uses block │
              └──┤Inline    ├──┤or use     ├──┤block power     ├──┤values to  │
                 │          │  │default    │  │using F-Value   │  │apply power│
                 └──────────┘  └───────────┘  └────────────────┘  └───────────┘
                 ┌──────────┐  ┌──────────┐
     Sensors─────┤Fault     ├──┤Disable   │
                 └──────────┘  │Power     │
                               └──────────┘

<!-- https://asciiflow.com/#/ -->

### Continuous Inline Trap Power Calculations

When LASER_FEATURE and LASER_POWER_TRAP are defined, planner calculations are performed and applied to the incoming laser power S-Value. The power will be factored and distributed across trapezoid acceleration and deceleration movements.

When the laser.power > 0

We set a minimum power if defined in SPEED_POWER_MIN it's fed into the planner block as laser_power_floor.

A reduced entry laser power factor is based on the entry step rate to cruise step rate ratio for acceleration.

    block entry laser power = laser power * ( entry step rate / cruise step rate )

The initial power will be set to no less than the laser_power_floor or the initial power calculation.

The reduced final power factor is based on the final step rate to cruise step rate ratio for deceleration.

    block exit laser power = laser power * ( exit step rate / cruise step rate )

Once the entry and exit power values are determined, the values are divided into step increments to be applied in the stepper.

    trap step power incr_decr = ( cruize power - entry_exit ) / accel_decel_steps

The trap steps are incremented or decremented during each accel or decel step until the block is complete.
Step power is either cumulatively added or subtracted during trapeziod ramp progressions.

#### Planner Code:

   ```
   if (block->laser.power > 0) {
      NOLESS(block->laser.power, laser_power_floor);
      block->laser.trap_ramp_active_pwr = (block->laser.power - laser_power_floor) * (initial_rate / float(block->nominal_rate)) + laser_power_floor;
      block->laser.trap_ramp_entry_incr = (block->laser.power - block->laser.trap_ramp_active_pwr) / accelerate_steps;
      float laser_pwr = block->laser.power * (final_rate / float(block->nominal_rate));
      NOLESS(laser_pwr, laser_power_floor);
      block->laser.trap_ramp_exit_decr = (block->laser.power - laser_pwr) / decelerate_steps;
   ```

#### Stepper Code:

   ```
   if (current_block->laser.trap_ramp_entry_incr > 0) {
      cutter.apply_power(current_block->laser.trap_ramp_active_pwr);
      current_block->laser.trap_ramp_active_pwr += current_block->laser.trap_ramp_entry_incr;
   ```

   ```
   if (current_block->laser.trap_ramp_exit_decr > 0) {
      current_block->laser.trap_ramp_active_pwr -= current_block->laser.trap_ramp_exit_decr;
      cutter.apply_power(current_block->laser.trap_ramp_active_pwr);
   ```

### Dynamic Inline Calculations

Dynamic mode will calculate laser power based on the F-Value feedrate. The method uses bit shifting to set a power level from 0 to 255. It's simple and fast and we can use a scaler to shift the laser power output to center on a given power level.

#### Spindle/Laser Code:

```
    // Dynamic mode rate calculation
    static inline uint8_t calc_dynamic_power() {
      if (feedrate_mm_m > 65535) return 255;         // Too fast, go always on
      uint16_t rate = uint16_t(feedrate_mm_m);       // 16 bits from the G-code parser float input
      rate >>= 8;                                    // Take the G-code input e.g. F40000 and shift off the lower bits to get an OCR value from 1-255
      return uint8_t(rate);
    }
```
