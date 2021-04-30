                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


https://www.thingiverse.com/thing:2494642
CR-10 Heavy Duty customisable modular e3D V6 mount Volcano 5015 ABL by chito is licensed under the Creative Commons - Attribution - Non-Commercial license.
http://creativecommons.org/licenses/by-nc/3.0/

# Summary

#CR-10 Heavy Duty Modular E3D V6 Mount
This is a mount for the V6 hot-end by E3D that is stronger and more rigid than some of the mounts that are out there. This mount also comes with an array of possible options for any extra functionality as attachments on the sides.

The minimum you will need to print out is the body and one of the faceplates. There are a variety of wings for different combinations, additions and customizations.

NOTE: To print a wing for the other side simply mirror the part in your slicer.

Current wing modules
- 5015 fan mount
- ABL (18mm thread). Offset X:41.5mm / Y:0
- 5015 fan & ABL (18mm thread). Do not use on left side. Offset X:63mm / Y:0
- 5015 fan & ABL alt (18mm thread). Offset X:63mm / Y:12mm

####Remix
Based on the V6 mount by graham01
https://www.thingiverse.com/thing:2471011
and the dual 5015 fan mod by RebelRousingProps
https://www.thingiverse.com/thing:2274499

I found some issues with the V6 mount file, also missing was support for dual 5015 fans and an ABL sensor so decided to make my own. I've also made my own library of parts.

If you would like to use a 40mm heatsink fan and don't want to use the integrated ones I've made. Then you can use this part https://www.thingiverse.com/thing:929326 which works great with this mount.

special shout out to Chris Ragucci who helped me test these parts and provide feedback.

####Update 1
- *Fixed the 5015 fan wings. correct measurements, centred with hot-end*
- *ABL sensor mount also centred, ie Y offset = 0*
- *Minor improvements to Mount body*

####Update 2
- *Optimised wings to not need as many supports (needs new body)*
- *Created new body with better mounting to x-carriage and updated wing rails*
- *NEW PART: 30mm fan mount (40mm version to come soon)*

####Update 3
- *Fixed a minor issue with the face plate*
- *Optimised the V6 30mm heatsink fan mount faceplate*
- *NEW PART: 40mm heatsink fan mount faceplate*

####Update 4
- *Removed logo (wouldnt print correctly)*

####Update 5
- *Updated 40mm fan mount - shouldn't blow air on to heat block (needs testing)*

####Update 6 (major update)
- *added holes back to the rear of the body for more compatibility*
- *resized holes for nuts, made them tighter as they were too loose and not being held correctly*
- *fixed clearance tolerance for heat sink, it now goes in tight without having to be forced in*
- *Added logo for e3D and V6 to face place to help with orientation during assembly. Fan mount version didn't require it but added for consistency*
- *NEW FILE: Added new STEP file of the wing base so people can create their own wings*

####Update 7 
- *NEW PART: new version of the 5015 fan mount with ABL. ABL mount has been moved forward 12mm. this is mainly because the origianl ver, when used on the left side, hits the gantry*

####Update 8
- *NEW PART: V6 Volcano mount (needs testing)*. fully compatible with all wing modules, covers and heatsink fan mounts

####Known issues
None

# Print Settings

Printer: CR-10
Rafts: No
Supports: Yes
Resolution: 0.2
Infill: 50

Notes: 
Most parts can be printed with the above settings. For orientation please look a the screen shot in the gallery.

The only part that needs support is the main body that attaches to the x-carriage.

# Post-Printing

## #Assembly

1. **Attach the wing of your choice to the body**
	- 1x M3 Nyloc nut
  	- 1x M3 12mm screw
  	
	The wings are adjustable in height (11mm total) and should never go past the tip of the hotend. Place the nut down the holes in the body and secure the wing using a screw from the side of the wing

	1a. **5015 wing - Attach the Fan**
	- 1x M3 20mm screw
	- 1x M3 Nyloc nut
	
	You can drop the nut through the top part of the mount and then slide into the nut shaped hole. lineup the fan and secure with the 20mm screw.
    
    1b. **ABL wing**
	Follow the instructions of your ABL. Take note of which side your sensor is, the offset settings are
	- 5015 ABL wing: X:63.00 mm Y:0
	- 5015 ABL wing alt: X:63.00 mm Y:12
	- ABL only wing: X:41.50 mm Y:0

2. **Attach the body to the x-carriage**
	- 2x M3 Nyloc nuts
	
	These need to be put in place before you mount the body to the x-carriage. Then use the existing screws to screw the body in place. NOTE The stock screws that come with the hotend and x-carriage might not be the best quality. It's optional to replace these. To do so you'll need:
	- 2x M3 6mm screws
	- 1x M3 20mm screw

3. **Attach the front plate**
	- 2x M3 30mm screws
	
	Insert the top part of the V6 heatsink and attach the front plate to the body with the screws. these should screw into the nuts you placed in the back of the body