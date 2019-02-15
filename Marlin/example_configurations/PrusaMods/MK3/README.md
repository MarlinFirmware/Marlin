# Prusa MK3 for Marlin 3D Printer Firmware

This folder contains sample configuration files to allow use of the Marlin 3D
printer firmware with the Prusa MK3 3D printer.

## Documentation and Resources

A number of individuals are contributing to this effort. The best resource
for questions or feedback is the #Marlin channel in the  ["602 Wasteland" Discord server](https://discord.gg/R848Xn). 
Please read the rules there before posting.

The team also contributes to the [PrusaOwners Wiki](https://github.com/PrusaOwners/prusaowners/wiki) 
which contains a wealth of information on the Prusa MK3 printer.

## Building the firmware for the Prusa i3 MK3

Install PlatformIO (e.g. as a Plugin to Microsoft Visual Sudio Code). Open the Marlin directory as a PlatformIO project. Edit the file "platformio.ini" in the main directory. 

    [platformio]
    ...
    env_default = rambo
    
    ...
    #
    # RAMBo
    #
    [env:rambo]
    ...
    monitor_speed     = 115200
   
Then copy the configuration files "Configuration.h", "Configuration_adv.h" and " pins_EINSY_RAMBO.h" from the directory "example_configurations/Prusa/i3-MK3" to the "Marlin" directory overwriting the existing files there.

Build the firmware and upload to your EINSY board.

## Summary of Changes

This section contains a description of each change to the Marlin configuration
files, Configuration.h and Configuration_adv.h.  The intent is to provide a
reference to allow multiple developers to work on the project with a quick
ramp-up on what changes were needed.

### Configuration.h

TODO

### Configuration_adv.h

TODO

### pins_EINSY_RAMBO.h

Changed hotend fan to -1, so that automatic temperature control can be used.
Changed pin of part cooling fan to Prusa default
Added define for PINDA thermistor pin
