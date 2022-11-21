## RAMPS with 5 Linear Axes (no extruders)

The axes are controlled with parameters `X`, `Y`, `Z`, `U`, and `V`.

All axes in this example are given the same steps/mm, acceleration, etc.

NOTE: Internal to Marlin the first three additional axes are called I, J, and K. However, the
G-code parameters for extra axes are assigned using AXIS4_NAME, AXIS5_NAME, AXIS6_NAME, etc.

In this example Marlin automatically maps the extra axes to the unused E0 and E1 stepper ports.
Endstop pins must be assigned manually, so this example assigns I_STOP_PIN and J_STOP_PIN to
pins 65 and 66 on the AUX-2 port.
