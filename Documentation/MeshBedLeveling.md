==============================================
Instructions for configuring Mesh Bed Leveling
==============================================

Background
----------

This mesh based method of leveling/compensating can compensate for an non-flat bed. There are various opinions about doing this. It was primarily written to compensate a RigidBot BIG bed (40x30cm) that was somewhat bent.

Currently there is no automatic way to probe the bed like the Auto Bed Leveling feature. This might soon be implemented though, stay tuned.

Theory
------

The bed is manually probed in a grid maner. During a print the Z axis compensation will be interpolated within each square using a bi-linear method. Because the grid squares can be tilting in different directions a printing move can be split on the borders of the grid squares. During fast travel moves one can sometimes notice a de-acceleration on these borders. 

Mesh point probing can either be carried out from the display, or by issuing `G29` commands.

The Z-endstop should be set slightly above the bed. An opto endstop is preferable but a switch with a metal arm that allow some travel though should also work.

Configuration
-------------

In `Configuration.h` there are two options that can be enabled.

`MESH_BED_LEVELING` will enable mesh bed leveling.<br/>
`MANUAL_BED_LEVELING` will add the menu option for bed leveling.

There are also some values that can be set.

Following four define the area to cover. Default 10mm from max bed size

`MESH_MIN_X`<br/>
`MESH_MAX_X`<br/>
`MESH_MIN_Y`<br/>
`MESH_MAX_Y`

Following two define the number of points to probe, total number will be these two multiplied. Default is 3x3 points. Don't probe more than 7x7 points (software limited)

`MESH_NUM_X_POINTS`<br/> 
`MESH_NUM_Y_POINTS`<br/>

The following will set the Z-endstop height during probing. When initiating a bed leveling probing, a homing will take place and the Z-endstop will be set to this height so lowering through the endstop can take place and the bed should be within this distance. Default is 4mm

`MESH_HOME_SEARCH_Z`

The probed points will also be saved in the EEPROM if it has been enables. Otherwise a new probe sequence needs to be made next time the printer has been turned on.

Probing the bed with the display
--------------------------------

If `MANUAL_BED_LEVELING` has been enabled then will a `Level bed` menu option be available in the `Prepare` menu.

When selecting this option the printer will first do a homing, and then travel to the first probe point. There it will wait. By turning the encoder on the display the hotend can now be lowered until it touches the bed. Using a paper to feel the distance when it gets close. Pressing the encoder/button will store this point and then travel to the next point. Repeating this until all points have been probed.

If the EEPROM has been enable it can be good to issue a `M500` to get these points saved.

Issuing a `G29` will return the state of the mesh leveling.

Probing the bed with G-codes
----------------------------

Probing the bed by G-codes follows the sequence much like doing it with the display.

`G29` or `G29 S0` will return the state bed leveling.

`G29 S1` will initiate the bed leveling, homing and traveling to the first point to probe.

Then use your preferred Printer controller program, i.e. Printrun, to lower the hotend until it touches the bed. Using a paper to feel the distance when it gets close.

`G29 S2` will store the point and travel to the next point until last point has been probed.

Note
----

Depending how firm feel you aim for on the paper you can use the `Z offset` option in Slic3r to compensate a slight height diff. (I like the paper loose so I needed to put `-0.05` in Slic3r)