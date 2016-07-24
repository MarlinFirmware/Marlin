# Marlin 3D Printer Firmware
<img align="top" width=175 src="Documentation/Logo/Marlin%20Logo%20GitHub.png" />
 Additional documentation can be found in [The Marlin Wiki](https://github.com/MarlinFirmware/Marlin/wiki).

## UBL (Unified Bed Leveling) Development Branch -- Marlin 1.1.0-RC6 - 4 July 2016

__Not for production use – use with caution!__

This is a Development Branch of Marlin.   It is built on a High Resolution Mesh Bed Leveling System that was originally pioneered by Epatel.   It also incorporates the earlier concepts of 3-Point Bed Leveling and Grid Based Bed Leveling.   The System provides backward compatabily with the 3-Point and Grid Based Leveling by giving the user the ability to create a zero'ed mesh and then tilting it in the prescribed manner.   These two backward compatable modes of operation can be done with a G29 P0 T  or a G29 P0 G 3    (for a 3 x 3 grid) respectively.

## Usage of the UBL System

In order to realize the full benefits of the system, the user will need to choose a Mesh size and then populate it with accurate values.   The bulk of the values can be obtained automatically with a G29 P1 M command.   The unreachable (by the probe) areas can be populated by either performing Manual Probing with G29 P2 command or a G29 P3 command which fills an area of the Mesh with a specified value.   It is then anticipated that the user should utilize the new G26 Mesh Validation Command to see how well they have the Mesh defined for thier print bed.   With this information the user would likely move on to the Fine Tuning the Mesh with a set of G29 P4 commands.   The bulk of the UBL System was developed using a 10 x 10 Mesh.   The system can go up to 15 x 15 if needed.  

When bringing up the system, it is advisable to start with the Configuration.h file associated with the release and make as few  changes as possible.   The System was developed on a Prusa i3 machine with a 20x4 LCD display.  The closer you are to the development system, the less likely you are to have issues.   Once you have the System up and running correctly it should be OK to start making a few changes at a time to get the Configuration.h file more closely aligned with your preferred configuration.   There is a lot of new code in this package and by moving to your preferred configuration in steps it will help us give you good support.  

The full set of parameters for G29 and G26 are provided at the end of this page.

## Recent Changes
- Inital Release

## To Do Items
The UBL System is currently fully functional on Prusa i3 type machines with a 20x4 LCD Panel.   It is expected to be fully functional on all machine types with or without an LCD Panel prior to it being released as a 'Stable Release'.    

- Clean up the formatting of the Parameter Options at the bottom of this page.
- Deltas have not been checked out and verified for correct operation yet.   It should be noted that this code was originally designed and written to address the horrible quality issues I have with my Geeetech Delta printer.   That was my original intent but I got side tracked and ended up with the Cartesian versions running first.    I will get the Delta version going as soon as I get some spare time.  (I have to fix my Delta and put it back together again.  So it may take some time.)   The code was written to be compatible with Delta's and should not be very difficult to get running correctly.   If somebody with a Delta wants to give it a try, PLEASE DO!  And then report back any areas where things are not working correctly.
- LCD Panel Menu items should be added to facilitate the generation, loading, storing, tilting and editing of a Mesh.   Right now, these are all done using PronterFace.  The control should be expanded to allow operation purely from the LCD Panel.
- The UBL System was designed and coded with the expectation that some machines without LCD Panels will be using the system.   Right now, the LCD’s Encoder Wheel is used in a number of places to do fine adjustments.   That requirement can be relaxed and those adjustments should also be possible to be made through PronterFace or Repitier Host.   That (small amount of) extra code is not yet in place.
- Work with PronterFace and Repetier Host to allow Marlin to use non-proportional fonts.   The problem is when printing out Mesh Maps the use of spaces and minus signs really messes with the appearance of the Mesh.   So, Marlin has to compensate and print extra spaces and dashes in an attempt to even out and align the columns.   Everything is tuned for PronterFace, so Mesh Generation (and editing) is best done with it.   But feel free to use what ever you are comforatable using.


## Current Status: Testing

Please test this firmware and inform us if it misbehaves in any way. Volunteers are standing by!

## Credits

The current Marlin UBL team consists of:

 - Scott Lahteine [@thinkyhead] - English
 - Joao Brazio [@jbrazio] - Portuguese
 - Roxanne Neufeld [@Roxy-3D] - English

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

## G29 Parameters

    G29: Mesh Based Compensation System

     Parameters understood by this leveling system:

      A     Activate  	Activate the Unified Bed Leveling system.

      B #   Business  	Use the 'Business Card' mode of the Manual Probe subsystem.  This is invoked as
      		      		G29 P2 B   The mode of G29 P2 allows you to use a bussiness card or recipe card 
      		      		as a shim that the nozzle will pinch as it is lowered.   The idea is that you 
      		      		can easily feel the nozzle getting to the same height by the amount of resistance 
      		      		the business card exhibits to movement.   You should try to achieve the same amount 
      		      		of resistance on each probed point to facilitate accurate and repeatable measurements. 
      		      		You should be very careful not to drive the nozzle into the bussiness card with a 
      		      		lot of force as it is very possible to cause damage to your printer if your are 
      		      		careless.  If you use the B option with G29 P2 B you can leave the number parameter off 
      		      		on its first use to enable measurement of the business card thickness.  Subsequent usage 
      		      		of the B parameter can have the number previously measured supplied to the command.  
      		      		Incidently, you are much better off using something like a Spark Gap feeler gauge than 
      		      		something that compresses like a Business Card. 

     C     Continue	Continue, Card or Current Location.   This is not a primary command.  C is used to 
      			 	further refine the behaviour of several other commands.  Issuing a G29 P1 C will 
      		      		continue the generation of a partially constructed Mesh without invalidating what has 
      		      		been done.  Issuing a G29 P2 C will tell the Manual Probe subsystem to use the current 
      		      		location in its search for the closest unmeasured Mesh Point.  When used with a G29 Z C 
      		      		it indicates to use the current location instead of defaulting to the center of the print bed.

      D     Disable   	Disable the Unified Bed Leveling system.

      F #   Fade      	Fade the amount of Mesh Based Compensation over a specified height.  At the specified height, 
      		      		no correction is applied and natural printer kenimatics take over.  If no number is specified 
      		      		for the command, 10mm is assummed to be reasonable.

      G #   Grid      	Perform a Grid Based Leveling of the current Mesh using a grid with n points on
     		      		a side.

      H #   Height    	Specify the Height to raise the nozzle after each manual probe of the bed.  The
      		      		default is 5mm.  

      I #   Invalidate 	Invalidate specified number of Mesh Points.  The nozzle location is used unless
      		      		the X and Y parameter are used.   If no number is specified, only the closest Mesh 
      		      		point to the location is invalidated.  The M parameter is available as well to produce 
      		      		a map after the operation.  This command is useful to invalidate a portion of the 
      		      		Mesh so it can be adjusted using other tools in the Unified Bed Leveling System.  When 
      		      		attempting to invalidate an isolated bad point in the mesh, the M option will indicate 
      		      		where the nozzle is positioned in the Mesh with (#).  You can move the nozzle around on
      		      		the bed and use this feature to select the center of the area (or cell) you want to 
      		      		invalidate.

      K #   Kompare   	Kompare current Mesh with stored Mesh # replacing current Mesh with the result.  This
                      	command litterly performs a difference between two Mesh. 

      L     Load      	Load Mesh from the previously activated location in the EEPROM.

      L #   Load      	Load Mesh from the specified location in the EEPROM.  Set this location as activated
                      	for subsequent Load and Store operations.

      M     Map       	Display the Mesh Map Topology.  The parameter can be specified alone (ie. G29 M) or
      		      		in combination with many of the other commands.  The Mesh Map option works with 
      		      		all of the Phase commands (ie. G29 P4 R 5 X 50 Y100 C -.1 M) 
	
The P or Phase commands are used for the bulk of the work to setup a Mesh.  In general, your Mesh will
start off being initialized with a G29 P0 or a G29 P1.   Further refinement of the Mesh happens with
each additional Phase that processes it.

      P0    Phase 0   	Zero Mesh Data and turn off the Mesh Compensation System.  This reverts the
                      	Delta Printer to the same state it was in before the Delta Mesh Based Compensation
                      	System was turned on.  Setting the entire Mesh to Zero is a special case that allows
                      	a subsequent G or T leveling operation for backward compatability.

      P1    Phase 1   	Invalidate entire Mesh and continue with automatic generation of the Mesh data using
                      	the Z-Probe.   Depending upon the values of DELTA_PROBEABLE_RADIUS and
                      	DELTA_PRINTABLE_RADIUS some area of the bed will not have Mesh Data automatically
                      	generated.  This will be handled in Phase 2.  If the Phase 1 command is given the 
                      	C (Continue) parameter it does not invalidate the Mesh prior to automatically 
                      	probing needed locations.  This allows you to invalidate portions of the Mesh but still 
                      	use the automatic probing capabilities of the Unified Bed Leveling System.  An X and Y 
                      	parameter can be given to prioritize where the command should be trying to measure points. 
                      	If the X and Y parameters are not specified the current probe position is used.  Phase 1 
                      	allows you to specify the M (Map) parameter so you can watch the generation of the Mesh. 
                      	Phase 1 also watches for the LCD Panel's Encoder Switch being held in a depressed state. 
                      	It will suspend generation of the Mesh if it sees the user request that.  (This check is 
                      	only done between probe points.  You will need to press and hold the switch until the 
                      	Phase 1 command can detect it.)

      P2    Phase 2   	Probe areas of the Mesh that can not be automatically handled.  Phase 2 respects an H
      		      	parameter to control the height between Mesh points.  The default height for movement 
      		      	between Mesh points is 5mm.  A smaller number can be used to make this part of the  
      		      	calibration less time consuming.  You will be running the nozzle down until it just barely 
      		      	touches the glass.  You should have the nozzle clean with no plastic obstructing your view.  
      		      	Use caution and move slowly.  It is possible to damage your printer if you are careless. 
      		      	Note that this command will use the configuration #define SIZE_OF_LITTLE_RAISE if the 
      		      	nozzle is moving a distance of less than BIG_RAISE_NOT_NEEDED. 
      		      	
      		      	The H parameter can be set negative if your Mesh dips in a large area.  You can press 
      		      	and hold the LCD Panel's encoder wheel to terminate the current Phase 2 command.  You 
      		      	can then re-issue the G29 P 2 command with an H parameter that is more suitable for the 
      		      	area you are manually probing.   Note that the command tries to start you in a corner 
      		      	of the bed where movement will be predictable.  You can force the location to be used in 
      		      	the distance calculations by using the X and Y parameters.  You may find it is helpful to 
      		      	print out a Mesh Map (G29 M ) to understand where the mesh is invalidated and where 
      		      	the nozzle will need to move in order to complete the command.    The C parameter is 
      		      	available on the Phase 2 command also and indicates the search for points to measure should 
      		      	be done based on the current location of the nozzle. 
      		      	
      		      	A B parameter is also available for this command and described up above.  It places the 
      		      	manual probe subsystem into Business Card mode where the thickness of a business care is 
      		      	measured and then used to accurately set the nozzle height in all manual probing for the 
      		      	duration of the command.  (S for Shim mode would be a better parameter name, but S is needed 
      		      	for Save or Store of the Mesh to EEPROM)  A Business card can be used, but you will have 
      		      	better results if you use a flexible Shim that does not compress very much.  That makes it 
      		      	easier for you to get the nozzle to press with similar amounts of force against the shim so you 
      		      	can get accurate measurements.  As you are starting to touch the nozzle against the shim try
      		      	to get it to grasp the shim with the same force as when you measured the thickness of the
      		      	shim at the start of the command.
      		      	
      		      	Phase 2 allows the M (Map) parameter to be specified.  This allows the user to watch the progression
      		      	of the Mesh being built.

      P3    Phase 3   Fill the unpopulated regions of the Mesh with a fixed value.  The C parameter is used to
      		      specify the Constant value to fill all invalid areas of the Mesh.  If no C parameter is 
      		      specified, a value of 0.0 is assumed.  The R parameter can be given to specify the number 
      		      of points to set.  If the R parameter is specified the current nozzle position is used to 
      		      find the closest points to alter unless the X and Y parameter are used to specify the fill 
      		      location.

      P4    Phase 4   Fine tune the Mesh.  The Delta Mesh Compensation System assume the existance of
                      an LCD Panel.  It is possible to fine tune the mesh without the use of an LCD Panel.  
                      (More work and details on doing this later!)
                      The System will search for the closest Mesh Point to the nozzle.  It will move the
                      nozzle to this location.  The user can use the LCD Panel to carefully adjust the nozzle
                      so it is just barely touching the bed.  When the user clicks the control, the System
                      will lock in that height for that point in the Mesh Compensation System.

                      Phase 4 has several additional parameters that the user may find helpful.  Phase 4
                      can be started at a specific location by specifying an X and Y parameter.  Phase 4
                      can be requested to continue the adjustment of Mesh Points by using the R(epeat)
                      parameter.  If the Repetition count is not specified, it is assumed the user wishes
                      to adjust the entire matrix.  The nozzle is moved to the Mesh Point being edited. 
                      The command can be terminated early (or after the area of interest has been edited) by 
                      pressing and holding the encoder wheel until the system recognizes the exit request.
                      Phase 4's general form is G29 P4 [R # of points] [X position] [Y position]
                      
                      Phase 4 is intended to be used with the G26 Mesh Validation Command.   Using the 
                      information left on the printer's bed from the G26 command it is very straight forward 
                      and easy to fine tune the Mesh.   One concept that is important to remember and that 
                      will make using the Phase 4 command easy to use is this:  You are editing the Mesh Points.  
                      If you have too little clearance and not much plastic was extruded in an area, you want to 
                      LOWER the Mesh Point at the location.  If you did not get good adheasion, you want to 
                      RAISE the Mesh Point at that location.


      P5    Phase 5   Adjust Mesh to mean height.  The Mesh is analyzed and the mean height of it is
      		      calculated.   The Standard deviation of the points within the mesh are also 
      		      calculated.   With this information, the mesh is adjusted to have a mean of 0.0. 
      		      Probably, this operation should not be performed on a Mesh if more additions are 
      		      going to be made to it.  One of the primary reasons for doing this is to make the 
      		      Mesh Map easier to analyze and understand.   But it is also useful for the case where 
      		      your G28 homes the Z Axis at a place where the value is significantly different from 
      		      the bed's mean value.  (ie.  Most of your bed is more or less than your G28 value.)

      Q  #  Test      Load specified Test Pattern to assist in checking correct operation of system.  This 
      		      command is not anticipated to be of much value to the typical user.  It is intended 
      		      for developers to help them verify correct operation of the Unified Bed Leveling System.

      S     Store     Store the current Mesh in the Activated area of the EEPROM.

      S #   Store     Store the current Mesh at the specified location in EEPROM.  Activate this location
                      for subsequent Load and Store operations.

      T     3-Point   Perform a 3 Point Bed Leveling on the current Mesh

      W     What?     Display valuable data the Unified Bed Leveling System knows.

      X #             Specify X Location for this line of commands

      Y #             Specify Y Location for this line of commands

      Z #   Zero	Probes to set the Z Height of the nozzle.  The entire Mesh can be raised or lowered 
      				to conform with the measured difference.  With no addition number specified
      				Z_PROBE_OFFSET_FROM_EXTRUDER is used.  The default location to be probed is the 
      				center of the bed.  If a C parameter is also specified, the current location of the 
      				nozzle is used.  The X and Y parameters can be used to specify the desired location to be probed.  
      			
## G26 Parameters
~~~~
    G26: Mesh Validation Tool
    
G26 Is a Mesh Validation Tool intended to provide support for the Marlin Unified Bed Leveling System.
In order to fully utilize and benefit from the Marlin Unified Bed Leveling System an accurate Mesh must
be defined.  G29 is designed to allow the user to quickly validate the correctness of her Mesh.  It will
first heat the bed and nozzle. It will then print lines and circles along the Mesh Cell boundaries and
the intersections of those lines (respectively).

This action allows the user to immediately see where the Mesh is properly defined and where it needs to 
be edited.   The command will generate the Mesh lines closest to the nozzle's starting position.  Alternatively
the user can specify the X and Y position of interest with command parameters.  This allows the user to
focus on a particular area of the Mesh where attention is needed.


B #	Bed			Set the Bed Temperature.  If not specified, a default of 60 C. will be assumed.

C	Current		When searching for Mesh Intersection points to draw, use the current nozzle location 
				as the base for any distance comparison.  

D	Disable		Disable the Unified Bed Leveling System.  In the normal case the user is invoking this
 				command to see how well a Mesh as been adjusted to match a print surface.  In order to do
				this the Unified Bed Leveling System is turned on by the G26 command.   The D parameter
				alters the command's normal behaviour and disables the Unified Bed Leveling System even if
				it is on. 

H #	Hotend		Set the Nozzle Temperature.  If not specified, a default of 205 C. will be assumed.
 
F #  Filament	Used to specify the diameter of the filament being used.  If not specified
				1.75mm filament is assumed.  If you are not getting acceptable results by using the
				'correct' numbers, you can scale this number up or down a little bit to change the amount
				of filament that is being extruded during the printing of the various lines on the bed.

K    Keep-On	Keep the heaters turned on at the end of the command.

L #  Layer		Layer height.  (Height of nozzle above bed)  If not specified .20mm will be used.

N #  Nozzle		Used to control the size of nozzle diameter.  If not specified, a .4mm nozzle is assumed.

O #  Ooooze		How much your nozzle will Ooooze filament while getting in position to print.  This
				is over kill, but using this parameter will let you get the very first 'cicle' perfect 
				so you have a trophy to peel off of the bed and hang up to show how perfectly you have your
				Mesh calibrated.

P #  Prime		Prime the nozzle with specified length of filament.  If this parameter is not 
				given, no prime action will take place.   If the parameter specifies an amount, that much
				will be purged before continuing.   If no amount is specified the command will start
				purging filament until the user provides an LCD Click and then it will continue with
				printing the Mesh.  You can carefully remove the spent filament with a needle nose 
				pliers while holding the LCD Click wheel in a depressed state. 

R #  Random		Randomize the order that the circles are drawn on the bed.  The search for the closest 
				undrawn cicle is still done.  But the distance to the location for each circle has a 
				random number of the size specified added to it.  Specifying R50 will give an interesting
				deviation from the normal behaviour on a 10 x 10 Mesh.

X #	X coordinate	Specify the starting location of the drawing activity.

Y #	Y coordinate	Specify the starting location of the drawing activity.
~~~~

