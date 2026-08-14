# Axis / Endstop Repeatability Diagnostic for Marlin

> **Implementation status:** **DONE** for the MVP described in this document.
> `M49` is implemented and enabled in this firmware tree. Items explicitly
> marked **TODO** are not implemented or have not yet been validated on hardware.

## 1. Purpose

This document describes the implemented Marlin `M49` diagnostic G-code command
for measuring the **repeatability of an X, Y, or Z axis using its physical
microswitch endstop as an independent reference**.

The main motivation is to distinguish between:

- probe-related errors (for example BLTouch / clone behavior),
- axis motion errors,
- mechanical backlash,
- stiction,
- stepper / driver behavior,
- missed or non-reproducible microsteps,
- dual-motor synchronization issues,
- speed- or direction-dependent positioning errors,
- and endstop repeatability itself.

The concept is similar in spirit to Marlin's `M48 Z-Probe Repeatability Test`, but instead of measuring a bed probe repeatedly, this test exercises a selected machine axis and measures where its **mechanical endstop microswitch** triggers.

For the Z axis, the BLTouch must **not** be used as the measurement reference. The whole point is to use the original Z microswitch (or another dedicated physical Z endstop) so that the BLTouch is completely outside the measurement chain.

This makes the test especially valuable when trying to answer:

> Is the observed Z drift caused by the BLTouch, or by the Z motion system itself?

---

## 2. Core behavior - DONE

A single measurement sample proceeds as follows:

1. Establish a known reference using the selected axis endstop.
2. Move away from the endstop by a configurable safety margin.
3. Exercise the axis by repeatedly moving forward and backward over a configurable travel distance.
4. Return near the endstop.
5. Approach the physical endstop slowly.
6. Record the logical axis position at the exact moment the endstop triggers.
7. Repeat this complete sequence multiple times.
8. Report statistics.

Example result:

```text
Axis Repeatability Test: Z

Sample  1: +0.003
Sample  2: +0.004
Sample  3: +0.002
Sample  4: +0.004
Sample  5: +0.003

Mean:      +0.0032
Min:       +0.0020
Max:       +0.0040
Range:      0.0020
Std Dev:    0.0008
```

A monotonic sequence such as:

```text
Sample  1: +0.000
Sample  2: +0.025
Sample  3: +0.049
Sample  4: +0.074
Sample  5: +0.101
```

would be a strong indication of cumulative axis-position drift.

---

## 3. Implemented G-code - DONE

The command is registered as:

```text
M49
```

Syntax:

```text
M49 A<axis> D<distance> R<cycles> F<forward_speed> B<backward_speed>
    M<margin> S<switch_approach_speed> P<samples> V<verbosity>
```

Example:

```gcode
M49 AZ D4 R10 F240 B120 M2 S30 P10 V2
```

Meaning:

```text
A Z     Test the Z axis
D 4     Exercise travel = 4 mm
R 10    Perform 10 forward/backward exercise cycles per sample
F 240   Positive-direction speed = 240 mm/min
B 120   Negative-direction speed = 120 mm/min
M 2     Stay 2 mm away from the endstop before exercise / measurement
S 30    Final endstop approach speed = 30 mm/min
P 10    Collect 10 complete measurement samples
V 2     Verbose output
```

---

## 4. Parameter definitions - DONE

`A` is required. Numeric parameters are optional and default to
`D4 R10 F240 B120 M2 S30 P10 V1`. Implemented limits include `R0..1000`,
`P1..50`, and `V0..3`; distances must be positive and feedrates must not
exceed the configured maximum for the selected axis.

### `A` - Axis

Required.

```text
AX
AY
AZ
```

Only one axis may be tested at a time.

The axis must have a usable physical endstop input.

For Z, a BLTouch or other probe must not be silently substituted for a physical Z microswitch.

If no suitable physical endstop exists, the command fails safely.

Example:

```text
Error: Z physical endstop unavailable
```

### `D` - Exercise distance

Distance in millimeters traveled away from and back toward the reference region.

Example:

```text
D4
```

means the exercise motion spans 4 mm.

If the axis starts at the margin position:

```text
M = 2 mm
D = 4 mm
```

then a typical Z cycle may be:

```text
Z = 2 -> 6 -> 2
```

The implementation validates the requested move against software travel limits.

### `R` - Exercise repetitions

Number of forward/backward movement cycles performed **between two measurements**.

Example:

```text
R10
```

performs ten complete cycles.

This parameter is important because cumulative drift may depend on the number of direction reversals.

### `F` - Forward speed

Feedrate in mm/min for motion in the positive axis direction.

Example:

```text
F240
```

means:

```text
240 mm/min = 4 mm/s
```

### `B` - Backward speed

Feedrate in mm/min for motion in the negative axis direction.

Example:

```text
B120
```

means:

```text
120 mm/min = 2 mm/s
```

Using separate forward and backward speeds is important because some problems may be:

- direction dependent,
- speed dependent,
- caused by stiction,
- caused by driver behavior at low step rates,
- caused by different motor torque requirements,
- or caused by asymmetrical loading.

Useful test matrix:

```text
F240 B240
F120 B120
F240 B120
F120 B240
```

### `M` - Endstop margin

Distance from the physical endstop used as the starting / safe position before the exercise loop.

Example:

```text
M2
```

means:

```text
move 2 mm away from the triggered endstop before exercising the axis
```

This must be large enough to fully release the microswitch and clear switch hysteresis.

### `S` - Final switch approach speed

Feedrate in mm/min used for the final measurement approach toward the endstop.

Example:

```text
S30
```

means:

```text
30 mm/min = 0.5 mm/s
```

The final measurement must be done slowly to minimize dynamic effects and overshoot.

### `P` - Number of samples

Number of complete independent samples collected.

Example:

```text
P10
```

Each sample includes:

```text
margin
exercise loop
slow endstop approach
trigger-position measurement
```

### `V` - Verbosity

Implemented behavior:

```text
V0  Final statistics only
V1  One line per sample
V2  Detailed test progress
V3  Developer / diagnostic details
```

---

## 5. Implemented test sequence - DONE

A key design goal is to avoid contaminating each sample with Marlin's normal multi-stage homing routine.

Normal homing may contain:

- fast approach,
- backoff,
- slow bump,
- configurable bump distance,
- configurable bump divisor,
- special probe logic for Z,
- and other homing-specific behavior.

That would introduce variables which this diagnostic is supposed to isolate.

M49 uses the following sequence:

### Initialization

1. Validate parameters.
2. Validate physical endstop availability.
3. Ensure the selected axis can move safely.
4. Disable or bypass probe-based Z homing for the purpose of this command.
5. Perform one initial reference acquisition against the selected **physical microswitch**.
6. Establish a logical reference coordinate.

Then collect samples without repeating full normal homing.

---

## 6. Implemented sample algorithm - DONE

The implementation follows this algorithm:

```text
validate_parameters()

axis = selected_axis

ensure_physical_endstop_exists(axis)

save_relevant_machine_state()

disable_features_that_would_distort_test()

reference_axis_once_using_physical_endstop(axis)

for sample in 1..P:

    move_away_from_endstop(axis, M, slow_or_safe_speed)

    ensure_endstop_is_released()

    for cycle in 1..R:
        move_positive(axis, D, F)
        move_negative(axis, D, B)

    verify_axis_is_back_at_margin_command_position()

    arm_physical_endstop_detection(axis)

    move_toward_endstop(axis, S)

    stop_motion_at_trigger()

    trigger_position[sample] = capture_position_at_trigger()

    report_sample_if_requested()

restore_machine_state()

calculate_statistics()

report_results()
```

---

## 7. Trigger measurement - DONE

The useful value is not merely "the axis is now at zero."

If the implementation simply resets the logical coordinate to zero whenever the switch triggers, all samples would trivially become zero.

The diagnostic captures the position the firmware **believed it had reached at
the trigger event**, before applying any homing coordinate reset.

Conceptually:

```text
commanded / internally accumulated position at physical switch trigger
```

This value reveals whether repeated motion cycles have caused the firmware's internal position to diverge from physical reality.

Example:

```text
Initial physical trigger reference: 0.000

After exercise:
switch triggers when firmware believes Z = +0.072
```

This indicates approximately 0.072 mm of accumulated position error relative to the physical switch.

This distinction is critical.

---

## 8. Endstop handling - DONE

During ordinary moves Marlin may not always treat endstops the same way it does during homing.

The implementation therefore takes explicit control of endstop monitoring.

The diagnostic:

1. deliberately enable the selected endstop for the final measurement move,
2. verify it is released before starting the approach,
3. stop motion immediately when the selected switch triggers,
4. capture the planner / stepper position associated with that event,
5. avoid resetting coordinates until after the diagnostic value is stored.

The implementation uses Marlin's internal endstop and planner APIs rather than
polling with `M119`.

Polling would be too slow and would make the measurement dependent on host latency.

---

## 9. Physical Z endstop requirement - DONE

For Z, the test must be explicitly independent of the bed probe.

Possible configurations include:

```text
Z physical microswitch + separate BLTouch
```

or a temporary diagnostic configuration where a Z microswitch is connected to a free usable endstop input.

The command rejects `AZ` when no physical Z endstop exists. It never substitutes
a BLTouch, inductive probe, capacitive probe, or other bed probe.

This implementation is specifically intended to separate:

```text
motion-system error
```

from:

```text
probe error
```

---

## 10. Why this test is useful

Suppose a BLTouch `M48` test shows:

```text
-0.002
+0.013
+0.035
+0.047
+0.059
+0.081
+0.098
+0.116
+0.133
+0.143
```

This is not random scatter. It is a largely monotonic drift.

Possible causes include:

- BLTouch repeatability problems,
- probe thermal drift,
- probe deployment behavior,
- Z-axis motion error,
- Z screw friction,
- backlash,
- stiction,
- missed steps,
- motor current limitations,
- speed-dependent positioning error,
- dual-Z synchronization problems,
- or some combination of these.

A physical-endstop axis test provides an independent reference.

If the Z microswitch test produces:

```text
0.000
0.001
-0.001
0.001
0.000
```

while `M48` drifts strongly, then the Z motion system is likely healthy and the probe chain becomes the main suspect.

If the Z microswitch test instead produces:

```text
0.000
0.020
0.041
0.063
0.084
```

then the same kind of drift exists without the BLTouch.

That would strongly implicate the Z motion system.

---

## 11. Backlash

### Definition

Backlash is mechanical lost motion caused by clearance between mating components.

For a lead screw and nut, one face of the screw thread carries the load while moving in one direction.

After reversing direction, the screw may rotate slightly before the opposite thread face takes up the load.

During that interval:

```text
motor moves
lead screw rotates
firmware counts steps
gantry may move little or not at all
```

That lost motion is backlash.

### Simplified example

Assume:

```text
mechanical backlash = 0.05 mm
```

When moving upward continuously, everything may be consistent.

After reversing downward, the first 0.05 mm of commanded movement may mostly be spent taking up clearance before the gantry fully follows the screw.

This produces a direction-change error.

### Backlash versus cumulative drift

Pure backlash usually behaves approximately like a repeatable offset after each direction reversal.

For example:

```text
Cycle 1 error: 0.050
Cycle 2 error: 0.051
Cycle 3 error: 0.049
Cycle 4 error: 0.050
```

A progressive ramp such as:

```text
0.000
0.018
0.036
0.055
0.073
0.091
```

is not characteristic of simple ideal backlash alone.

A cumulative ramp may instead suggest:

- missed microsteps,
- mechanical settling,
- elastic deformation,
- stiction,
- coupler slip,
- one side of a dual-Z system progressively lagging,
- insufficient stepper torque,
- low-speed driver behavior,
- screw binding,
- or backlash interacting with another effect.

This diagnostic therefore **measures the effect without compensating for it**.

---

## 12. Backlash compensation handling - DONE

Marlin can optionally apply software backlash compensation.

M49 saves and temporarily disables backlash correction during the diagnostic,
then restores its previous state.

Otherwise the test would measure:

```text
mechanics + compensation algorithm
```

instead of:

```text
raw mechanical / motion-system behavior
```

Implemented behavior:

```text
save current backlash compensation state
temporarily disable compensation
run test
restore original state
```

**TODO:** An optional future `C` parameter could select whether compensation is
disabled or left unchanged:

```text
C0  compensation disabled during test
C1  leave compensation unchanged
```

The current implementation always uses diagnostic/raw mode, equivalent to:

```text
C0
```

---

## 13. Statistics - DONE / TODO

**DONE:** M49 calculates:

```text
Mean
Minimum
Maximum
Range
Standard deviation
```

**TODO:** The following advanced statistics are not implemented:

```text
Median
Linear drift slope per sample
R-squared of linear trend
Mean absolute deviation
```

The linear drift slope would be particularly useful for diagnosing cumulative motion error.

Example:

```text
Mean:               +0.067 mm
Min:                +0.002 mm
Max:                +0.133 mm
Range:               0.131 mm
Standard Deviation:  0.043 mm
Linear Drift:       +0.0143 mm/sample
R^2:                 0.992
```

A high `R^2` with significant slope strongly indicates systematic drift rather than random endstop noise.

The implemented MVP reports population standard deviation and range.

---

## 14. Output format - DONE

```text
M49 Axis-Endstop Repeatability Test

Axis: Z
Physical endstop: Z_MIN
Distance: 4.000 mm
Exercise cycles/sample: 10
Forward speed: 240 mm/min
Backward speed: 120 mm/min
Endstop margin: 2.000 mm
Measurement speed: 30 mm/min
Samples: 10

1 of 10: trigger position: +0.004
2 of 10: trigger position: +0.011
3 of 10: trigger position: +0.019
4 of 10: trigger position: +0.026
5 of 10: trigger position: +0.034
6 of 10: trigger position: +0.041
7 of 10: trigger position: +0.049
8 of 10: trigger position: +0.056
9 of 10: trigger position: +0.064
10 of 10: trigger position: +0.071

Finished!

Mean: +0.0375
Min: +0.0040
Max: +0.0710
Range: 0.0670
Standard Deviation: 0.0214
```

The output style deliberately resembles `M48`.

---

## 15. Safety checks - DONE

This command directly approaches physical travel limits and must be defensive.

M49 implements these checks:

- selected axis exists,
- selected physical endstop exists,
- requested distance is valid,
- requested margin is valid,
- speeds are positive and within configured limits,
- software travel limits will not be exceeded,
- endstop is initially functional,
- endstop releases after moving away,
- unexpected opposite-endstop trigger aborts the test,
- Marlin's existing emergency-stop and thermal-safety handling remains active,
- command is rejected while printing unless explicitly allowed.

If the endstop is already stuck triggered after moving away:

```text
Error: Z_MIN did not release
```

If the expected switch does not trigger within a maximum approach distance:

```text
Error: Z_MIN not triggered within safe travel
```

The axis must stop before a crash.

---

## 16. Abort handling - DONE / TODO

**DONE:** M49 aborts on:

```text
endstop stuck triggered
endstop never triggers
axis movement exceeds safe range
stopped firmware state
invalid parameter
unsupported axis
no physical endstop
```

Marlin's normal kill, emergency-stop, and thermal-fault paths remain active
during synchronized moves.

**TODO:** M49 has no dedicated driver-fault detection beyond Marlin's existing
global fault handling.

On abort, print enough state for diagnosis.

Example:

```text
M49 aborted
Reason: Z_MIN did not trigger
Last commanded Z: -1.500
Exercise sample: 4 / 10
```

---

## 17. Interaction with homing - DONE

M49 performs a bounded initial reference acquisition using low-level
planner/endstop machinery and avoids normal probe-based Z homing.

For Z:

```text
physical Z microswitch only
```

The command does not call a generic `G28 Z`. Its internal helper explicitly
selects the physical endstop in the configured homing direction. The selected
axis must already be homed and trusted; otherwise M49 aborts before moving.

---

## 18. Why not implement this as a host-side script?

A host-side implementation could send movements and poll `M119`, but it would be inferior.

Problems include:

- serial latency,
- asynchronous planner execution,
- low polling resolution,
- inability to capture the exact trigger step,
- race conditions,
- dependence on host software,
- difficulty stopping exactly on the switch transition.

The feature belongs inside firmware because Marlin already has direct access to:

- stepper counters,
- planner state,
- endstop sampling,
- homing logic,
- configured feedrates,
- axis limits,
- and safety handling.

---

## 19. Available diagnostic modes - DONE

### 19.1 No-exercise baseline

```text
R0
```

Collect repeated switch measurements without exercise cycles.

This isolates endstop repeatability itself.

```text
R0  -> switch repeatability baseline
R10 -> switch + motion-cycle repeatability
```

### 19.2 Direction asymmetry tests

Run:

```text
F240 B240
F120 B120
F240 B120
F120 B240
```

Comparing results may expose speed/direction dependence.

### 19.3 Different travel distances

Example:

```text
D1
D2
D4
D10
D50
```

If drift scales with travel distance, this suggests a different mechanism than if drift scales only with the number of reversals.

### 19.4 Different reversal counts

Example:

```text
R1
R5
R10
R50
R100
```

This helps determine whether the error is proportional to reversal count.

### 19.5 Endstop-only repeatability

`R0` implements this mode by repeatedly performing:

```text
release switch
approach switch
measure trigger
```

with no large exercise motion.

This effectively measures microswitch repeatability and hysteresis independently.

---

## 20. Diagnostic interpretation examples

### Case A: Endstop baseline is stable; exercise test drifts

```text
R0:
Range = 0.003 mm

R20:
Range = 0.120 mm
monotonic slope
```

Likely issue:

```text
motion system
```

Possible suspects:

- mechanical settling,
- lost motion,
- stepper / driver behavior,
- dual-Z imbalance,
- screw binding,
- coupler slip,
- insufficient torque.

### Case B: Endstop baseline itself is noisy

```text
R0:
Range = 0.080 mm
```

Likely issue:

```text
endstop / switch mechanics / mounting / electrical noise
```

The exercise test cannot be trusted until the reference switch is improved.

### Case C: Physical-endstop Z test stable, BLTouch M48 unstable

Strong evidence for:

```text
probe subsystem problem
```

Possible suspects:

- BLTouch clone repeatability,
- probe pin friction,
- probe electronics,
- deployment behavior,
- probe mounting flex,
- temperature-related probe drift.

### Case D: Physical-endstop Z test and M48 show similar monotonic drift

Strong evidence for:

```text
Z motion-system problem
```

The BLTouch is probably reporting a real change rather than creating it.

---

## 21. Minimum viable implementation - DONE

The implemented MVP accepts:

```text
A  axis
D  travel distance
R  exercise cycles
F  positive speed
B  negative speed
M  endstop margin
S  measurement speed
P  samples
V  verbosity
```

Implemented statistics:

```text
Mean
Min
Max
Range
Standard deviation
```

Implemented axes:

```text
X
Y
Z
```

Implemented requirement for Z:

```text
physical microswitch required
no BLTouch fallback
```

Implemented behavior:

```text
one initial physical-endstop reference
then repeated:
    move to margin
    exercise R times
    slowly approach switch
    capture pre-reset trigger position
```

---

## 22. Implementation structure - DONE

The command is implemented in `Marlin/src/gcode/calibrate/M49.cpp` as:

```cpp
GcodeSuite::M49()
```

It:

1. parse parameters,
2. validate axis and physical endstop,
3. store validated settings in bounded local variables,
4. save relevant runtime state,
5. perform initial reference,
6. run sample loop,
7. accumulate trigger-offset statistics online,
8. restore runtime state,
9. compute statistics,
10. print results.

Settings are held in bounded local variables. Statistics are accumulated
online with Welford's algorithm, so no dynamic allocation or sample array is
needed.

---

## 23. Trigger-position capture - DONE

The command captures a position representing:

```text
where the firmware thought the axis was
when the physical microswitch actually triggered
```

M49 reads `planner.triggered_position_mm(axis)` **before any coordinate reset**.

Do not derive the value from a post-homing `current_position`, because it may already have been forced to the configured home coordinate.

---

## 24. Planner synchronization - DONE

M49 synchronizes planner / stepper execution before checking switch release,
capturing a sample, or continuing after a move.

Otherwise the reported logical position may not correspond to completed physical motion.

The implementation uses:

```cpp
planner.synchronize();
```

---

## 25. Feature guards - DONE

The feature is compile-time guarded by:

```cpp
#define AXIS_ENDSTOP_REPEATABILITY_TEST
```

The dispatcher and source file use this guard:

```cpp
#if ENABLED(AXIS_ENDSTOP_REPEATABILITY_TEST)
  // M49 implementation and dispatch
#endif
```

The implementation resolves the available endstop using Marlin's configured
endstop macros, including:

```text
USE_X_MIN / USE_X_MAX
USE_Y_MIN / USE_Y_MAX
USE_Z_MIN / USE_Z_MAX
```

The command determines which physical endstop corresponds to the selected
homing direction.

For example:

```text
X_HOME_DIR < 0 -> X_MIN
X_HOME_DIR > 0 -> X_MAX
```

and similarly for Y / Z.

For Z, it explicitly requires a **real physical endstop input**, not a
virtual/probe substitution.

---

## 26. Endstop release verification - DONE

After moving away by the margin:

```text
switch must be OPEN / not triggered
```

If not, abort.

This protects against:

- stuck switches,
- incorrect wiring,
- insufficient margin,
- inverted endstop logic,
- mechanical obstruction.

---

## 27. Reproducible output - DONE

At verbosity `V1` or higher, the output includes all test parameters needed for
meaningful comparisons.

Example header:

```text
Axis Repeatability Test
Axis: Z
Endstop: Z_MIN
Distance: 4.000
Cycles: 10
Forward: 240
Backward: 120
Margin: 2.000
Approach: 30
Samples: 20
Backlash compensation: OFF
```

This allows test results to be copied into bug reports and compared later.

---

## 28. Hardware experiment plan - TODO

### Experiment 1 - Baseline switch repeatability

```gcode
M49 AZ D4 R0 F240 B120 M2 S30 P20 V1
```

Purpose: measure Z microswitch repeatability without exercise cycles.

### Experiment 2 - Symmetric speed

```gcode
M49 AZ D4 R10 F240 B240 M2 S30 P20 V1
```

Purpose: determine whether equal forward/backward speeds remain stable.

### Experiment 3 - Fast up, slow down

```gcode
M49 AZ D4 R10 F240 B120 M2 S30 P20 V1
```

Purpose: simulate a motion pattern similar to fast/slow probing behavior.

### Experiment 4 - Slow up, fast down

```gcode
M49 AZ D4 R10 F120 B240 M2 S30 P20 V1
```

Purpose: test whether the error follows direction or speed.

### Experiment 5 - Reversal-count scaling

```gcode
M49 AZ D4 R1  F240 B120 M2 S30 P20 V1
M49 AZ D4 R5  F240 B120 M2 S30 P20 V1
M49 AZ D4 R10 F240 B120 M2 S30 P20 V1
M49 AZ D4 R50 F240 B120 M2 S30 P20 V1
```

Purpose: determine whether drift is proportional to direction reversals.

### Experiment 6 - Travel-distance scaling

```gcode
M49 AZ D1  R10 F240 B120 M2 S30 P20 V1
M49 AZ D2  R10 F240 B120 M2 S30 P20 V1
M49 AZ D4  R10 F240 B120 M2 S30 P20 V1
M49 AZ D10 R10 F240 B120 M2 S30 P20 V1
```

Purpose: determine whether drift depends on distance or mainly on reversals.

---

## 29. Dual-Z systems - TODO

On machines with two Z motors, especially two motors connected in parallel to one driver, this test may expose problems that are difficult to feel manually.

Possible behavior:

- one motor produces slightly less torque,
- one screw has more friction,
- one side settles after repeated reversals,
- the gantry twists microscopically,
- one motor loses microsteps without an obvious audible skip.

A physical microswitch gives a fixed mechanical reference that can reveal accumulated position error even when the movement feels normal by hand.

M49 currently rejects axes configured with multiple endstops. Independent
left/right Z-endstop testing remains **TODO**.

---

## 30. Diagnostic-only behavior - DONE

This command is diagnostic.

It does not:

- modify steps/mm,
- change endstop offsets,
- alter backlash settings,
- automatically tune motor current,
- rewrite home offsets,
- modify probe offsets.

Its job is only to measure and report.

Automatic correction could hide the original fault and make diagnosis harder.

---

## 31. Command summary - DONE

Documentation title:

```text
M49 - Axis Endstop Repeatability Test
```

Short description:

> Exercise a selected axis repeatedly, then measure its return-to-reference accuracy using the physical axis endstop. Intended for diagnosing mechanical, stepper, driver, backlash, and cumulative positioning errors independently of bed probes.

Warning:

> For Z diagnostics intended to isolate a bed probe, this command requires a physical Z endstop. It must not silently substitute a BLTouch or other probe.

---

## 32. Implementation checklist

1. **DONE** - Selected and registered `M49`.
2. **DONE** - Followed `M48` conventions for parsing, statistics, output, and sampling.
3. **DONE** - Identified the homing/endstop APIs for arming, trigger capture, and coordinate reset.
4. **DONE** - Selects the real Z microswitch without falling back to the probe.
5. **DONE** - Added the `AXIS_ENDSTOP_REPEATABILITY_TEST` compile-time flag.
6. **DONE** - Implemented parsing and validation for `A`, `D`, `R`, `F`, `B`, `M`, `S`, `P`, and `V`.
7. **DONE** - Added bounded initial physical-reference acquisition.
8. **DONE** - Added endstop-release verification.
9. **DONE** - Added the exercise movement loop.
10. **DONE** - Added the slow measured approach.
11. **DONE** - Captures the trigger position before coordinate reset.
12. **DONE** - Accumulates statistics online without dynamic allocation.
13. **DONE** - Computes mean, minimum, maximum, range, and population standard deviation.
14. **DONE** - Added verbosity levels `V0..V3`.
15. **DONE** - Saves and restores relevant endstop, leveling, backlash, and feedrate-scaling state.
16. **DONE** - Temporarily disables backlash correction during the test.
17. **DONE** - Validates axis travel, margin, overrun, and feedrates.
18. **DONE** - Aborts on stuck, missing, or unexpected endstop triggers and stopped firmware state.
19. **DONE** - Registered conditional source inclusion in `ini/features.ini`.
20. **DONE** - Verified a complete `STM32F103RE_creality` PlatformIO build.
21. **TODO** - Validate motion and abort behavior on physical X or Y hardware.
22. **TODO** - Validate Z using the physical microswitch and confirm that BLTouch is excluded.
23. **TODO** - Compare `R0`, speed asymmetry, reversal counts, and travel distances on hardware.
24. **TODO** - Compare physical Z-endstop results against `M48` probe results.

---

## 33. Primary diagnostic question

The implemented feature is intended to answer this question:

```text
Does repeated Z motion create a measurable accumulated position error
when the reference sensor is a physical microswitch rather than the BLTouch?
```

If yes:

```text
investigate Z mechanics / motors / driver / screws / couplers / current / speed behavior
```

If no, while `M48` still drifts:

```text
investigate BLTouch / probe subsystem
```

That is the central purpose of M49.

---

## 34. Implementation status (2026-08-11)

The first firmware implementation is present in this repository and enabled
for the configured `STM32F103RE_creality` target.

### DONE

- `M49` is registered in the G-code dispatcher and built conditionally with
  `AXIS_ENDSTOP_REPEATABILITY_TEST`. PlatformIO includes the implementation
  through `ini/features.ini`.
- The compact selectors `AX`, `AY`, and `AZ` are supported, together with all
  MVP parameters. Defaults are `D4 R10 F240 B120 M2 S30 P10 V1`.
  Accepted ranges are `R0..1000`, `P1..50`, and `V0..3`.
- X, Y, and Z use the physical endstop in their configured homing direction.
  For this printer, Z uses the separate `Z_MIN` input (`PA7`); BLTouch is on
  `PB1`. Probe endstop monitoring is explicitly disabled during the test.
- The selected axis must already be homed and trusted. If not, M49 aborts
  immediately. M49 then acquires one initial physical-switch reference,
  establishes the home coordinate, and runs the requested samples without
  normal multi-stage homing between samples.
- Each sample moves to the margin, performs the exercise cycles with separate
  positive/negative feedrates, approaches the switch at `S`, and captures
  `planner.triggered_position_mm()` before any coordinate reset.
- Mean, minimum, maximum, range, and population standard deviation are
  reported. `R0` provides the endstop-only baseline.
- Bed leveling and backlash correction are temporarily disabled. Feedrate
  scaling, leveling, backlash correction, probe monitoring, and global endstop
  state are restored afterward.
- The command is rejected during active or paused print jobs. It validates the
  selected switch, homed state, speeds, margin, exercise travel, switch release,
  expected trigger, unexpected endstop hits, and stopped firmware state.
- A missing trigger can travel at most
  `AXIS_ENDSTOP_REPEATABILITY_MAX_OVERRUN` beyond the configured home position;
  this is currently configured as `0.5 mm`. After a successful test the axis is
  left released at the requested margin.
- The complete `STM32F103RE_creality` PlatformIO build succeeds: 9,072 bytes
  RAM and 192,752 bytes flash in the verified build.

### TODO and deliberate limitations

- **Deliberate limitation:** M49 does not perform a blind full-axis initial
  homing move. Requiring an already trusted axis bounds the physical-switch
  search around the configured home coordinate.
- **Deliberate limitation:** Delta/SCARA kinematics, sensorless homing, and
  dual/multiple endstops on the selected axis are explicitly rejected.
- **TODO:** Add separate left/right dual-Z analysis if required.
- **TODO:** Add the optional `C` compensation-control parameter. Raw mode is
  currently always used.
- **TODO:** Add median, drift slope, R-squared, and mean absolute deviation.
- **TODO:** Add a dedicated LCD menu, translated UI text, and automatic
  driver-fault detection if required. Standard Marlin kill, thermal-safety,
  and emergency handling remain active during synchronized moves.
- **TODO:** Add an automated motion-test fixture if a safe hardware test rig
  becomes available.
- **TODO:** Validate behavior on the physical printer. Compilation is already
  verified. Begin with a low-stress baseline after homing, for example:

  ```gcode
  G28 Z
  M49 AZ D1 R0 F60 B60 M2 S30 P5 V2
  ```

  Confirm switch release, direction, and clearance before increasing distance,
  speed, repetitions, or sample count.
