Startup
=======================

After startup the MFU sends an

- MFU => 'started\n'


Home with Retraction
=======================
If there is no Filamentsensor in use or it is enabled and detects filament the Printer needs to heat up and then Home the MFU with Retraction
After Heating the Hotend to the needed Temperature (Prevent Cold Extrusion) Printer sends
- MFU <= 'H1\n'

After Retracting and Homing the MFU answers
- MFU => 'ok\n

Home without Retraction
=======================
If there is an filamentsensor enabled which does not detect an filament the Printer can directly send an Home Command to the MFU. Since there is no Filament loaded no Retraction or Heating is needed
- MFU <= 'H0\n'

After Retracting and Homing the MFU answers
- MFU => 'ok\n

Toolchange
=======================
- MFU <= 'T*Toolindex*\n'

Now the MFU unloads the Filament, switches the Tool and preloads the Filament to the Extrudergears
- MFU => 'ok\n'
- MFU <= 'E\n'

MFU now fully loads the Filament
- MFU => 'ok\n'

Load Filament
=======================
To Finally Load the Filament into the Extruder the Printer simply sends an 
- MFU <= 'L\n'

to which the MFU answers 
- MFU => 'ok\n'

Unload CurrentFilament
=======================
To Unload the Filament 
- MFU <= 'U\n'

to which the MFU answers after it finished 
- MFU => 'ok\n'


Load Configuration
=======================
To Load the storred Settings send this to the MFU
- MFU <= 'M01\n'

The MFU answers with 
- MFU => 'ok\n'

Save Configuration
=======================
To Save the current Settings send this to the MFU
- MFU <= 'M02\n'

The MFU answers with 
- MFU => 'ok\n'

Restore Default Configuration
=======================
To Restore the default Configuration (from the Configuration.h File of the MFU) send this to the MFU
- MFU <= 'M03\n'

The MFU answers with 
- MFU => 'ok\n'

Change Configuration
=======================
To change single Parameters of the Configuration you can use following Command
- MFU <= 'M04 *parameterType* *parameterValue*\n'

The MFU answers with
- MFU => 'ok\n'

*parameterType* is equal to one of the following
  A -> Gantry StepsMM
  B -> Gantry Acc
  C -> Gantry Offset
  D -> Extruder StepsMM
  E -> Extruder Acc
  F -> Feed Homing
  G -> Feed Gantry
  H -> Feed Preload
  I -> Feed Load
  J -> Distance Preload
  K -> Distance Load

*parameterValue* is the Value the new parameter should have. 

Log Config
=======================
Sending this command logs the Config to the Serial port
- MFU <= 'M05\n'

The MFU answers with the current Configuration
- MFU => 'Current Config...\n'