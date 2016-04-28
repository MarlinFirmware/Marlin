#Configurations for Deltaprintr (Kickstarter regular sized version)
-------------------------------------------------------------------

**Important note**

_For **all** supplied configuration files that are **not explicitely labeled**_
_as being for_
_[JohnSL's FSR sensor board](https://github.com/JohnSL/FSR_Endstop),_
_**make sure** to connect the_
_FSR sensors to the **T1** input, **not** to **TBED** as in the default_
_Deltaprintr setup!_

<img align="top" width=320 src="fsr_connection.png" />


##Existing configuration variants in subfolders

###default/

* Configuration files that should work out of the box with an
  unmodified Deltaprintr.

###e3dv6/

* Default configuration with `MANUAL_Z_HOME_POS` and PID values
  adapted to the E3Dv6 all metal hotend, which happens to be longer
  than the standard J-Head hotend.

* Double check `MANUAL_Z_HOME_POS` by slowly lowering the hotend. If
  the nozzle touches the bed before the LCD shows `0.00` (or less) for
  the Z axis, you have to substract the shown value from
  MANUAL_Z_HOME_POS and re-install the firmware.

* If possible, do a PID autotune to match the PID values to your
  specific hotend. [Instructions](http://reprap.org/wiki/PID_Tuning)

###e3dv6_pt100/

* E3Dv6 configuration set up for the PT100 high temperature sensor
  [Description](http://e3d-online.com/Kits-and-Multipacks/V6-PT100-Sensor-Kit)

* Double check `MANUAL_Z_HOME_POS` by slowly lowering the hotend. If
  the nozzle touches the bed before the LCD shows `0.00` (or less) for
  the Z axis, you have to substract the shown value from
  `MANUAL_Z_HOME_POS` and re-install the firmware.

* If possible, do a PID autotune to match the PID values to your
  specific hotend / temperature sensor combination.
  [Instructions](http://reprap.org/wiki/PID_Tuning)

###e3dv6_pt100+heatbed/

* Same as above plus the custom build heated bed and modified FSR
  standoffs
  [Information about and build instruction for customizations](http://www.tk-webart.de/wiki/doku.php?id=wiki:deltaprintr:deltaprintr_modifications)

* Double check `MANUAL_Z_HOME_POS` by slowly lowering the hotend. If
  the nozzle touches the bed before the LCD shows `0.00` (or less) for
  the Z axis, you have to substract the shown value from
  `MANUAL_Z_HOME_POS` and re-install the firmware.

* If possible, do a PID autotune to match the PID values to your
  specific hotend / bed / temperature sensor combination.
  [Instructions](http://reprap.org/wiki/PID_Tuning)

* **Please, be aware** that FSR bed leveling does not work reliably for this
  setup due to the weight of the heated bed.

###e3dv6_pt100+heatbed+fsrboard/

* Same as above, but with JohnSL's FSR sensor board connected to the
  ZMax endstop input
  [FSR board information](https://github.com/JohnSL/FSR_Endstop)
  [Information about and build instruction for customizations](http://www.tk-webart.de/wiki/doku.php?id=wiki:deltaprintr:deltaprintr_modifications)

* Again, double check `MANUAL_Z_HOME_POS` by slowly lowering the
  hotend. If the nozzle touches the bed before the LCD shows `0.00` (or
  less) for the Z axis, you have to substract the shown value from
  `MANUAL_Z_HOME_POS` and re-install the firmware.

* If possible, do a PID autotune to match the PID values to your
  specific hotend/bed / temperature sensor combination.
  [Instructions](http://reprap.org/wiki/PID_Tuning)
