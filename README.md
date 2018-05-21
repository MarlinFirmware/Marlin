# Marlin 3D Printer Firmware

[![Build Status](https://travis-ci.org/MarlinFirmware/Marlin.svg?branch=RCBugFix)](https://travis-ci.org/MarlinFirmware/Marlin)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/2224/badge.svg)](https://scan.coverity.com/projects/2224)

<img align="top" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

Additional documentation can be found at the [Marlin Home Page](http://marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Marlin 2.1 Bugfix Branch

__Not for production use. Use with caution!__

Marlin 2.1 is an attempt to place a Real Time Operating System (RTOS) under Marlin 2.0.   The goal is to support the same build environments and targets as the Marlin 2.0 branch.   The goal is also to exactly duplicate the Marlin 2.0 functionality.   This branch is not currently stable.  It is only suitable for use by developers interested in the RTOS effort.   The Marlin 2.1 branch will not be updated each time Marlin 2.0 is changed.  It will be rebased against the Marlin 2.0 branch on an "as needed" basis.

The changes to place an RTOS under the Marlin code base are very minor.

It is helpful to have a Max7219 8x8 LED matrix on the machine for debug purposes.  It is helpful because the various tasks that are running update the LED's so you can see what is happening.   I strongly encourage everybody to spend the $2.00 to get one of these devices:  https://www.ebay.com/itm/MAX7219-Dot-Matrix-8x8-Led-Display-Module-MCU-Control-For-Arduino-Innovation/222709799381?epid=1672606113&hash=item33da89d9d5:g:QmgAAOSwbw1aATcc


## Building Marlin 2.1

Currently, the Marlin 2.1 branch can only build code for the 8-bit AVR processors.  It is a priority to get the build process also supporting the 32-bit platforms.   

To build Marlin 2.1 for the 8-bit AVR processors, you'll need [Arduino IDE 1.9](https://www.arduino.cc/en/main/software)   These steps will get Arduino configured to build Marlin 2.1 with FreeRTOS under it (for the AVR processors):

1.  Install FreeRTOS for Arduino
       - Bring up Arduino
       - Sketch / Include_Library / Manage_Libraries
       - Set Type=Contributed
       - Set Topic=Timing
       - Scroll to FreeRTOS by Richard Barry.  Real Time Operating System implemented for AVR
       - Select version 10,0,0-10 and press Install
       - Modified FreeRTOS files are in a .ZIP file alongside the Marlin source files.  Copy supplied (and altered) FreeRTOS files to ???\Arduino\libraries\FreeRTOS\src  Specifically: (FreeRTOSConfig.h, Arduino_FreeRTOS.h, variantHooks.cpp).  These files can be found in an archive ../Marlin/src/FreeRTOS-changes.zip
       - Configure your Marlin files in the Marlin directory
       - Having a Max7219 LED Matrix is helpful...  Debug info is displayed on
         the device.

Changes:
  - The Marlin idle() function is its own task now.  It is slightly lower priority than the main Marlin task.  Instead of being called periodically, the main Marlin task can suspend itself and the former idle() task can continue its work from where it left off.  What this means is idle() no longer needs to run to completion and return to its caller.  Instead, it is allowed to run an appropriate amount of time, and it gets cut off so the main Marlin task can get other work done.   Admitably, this initial use of the RTOS is not very sophisticated.  But it does demonstrate that the surgery to activate the RTOS is in place and working.
  - A few changes have been made to the default FreeRTOS configuration.  And these configuration options exposed a bug in the Arduino version of FreeRTOS (in variantHooks.cpp).  And it was done with very minor changes to the Marlin code so it is a good first step.
  - An M101 RTOS Free Task Memory Status routine has been added.
  - The Max7219 runs as a task with a message queue.   Probably, this is a waste of memory, but it serves a good purpose right now just to show FreeRTOS is doing its thing.
   

To Do:
  - We need the Arduino_FreeRTOS.h, FreeRTOSConfig.h and variantHooks.cpp files to only exist in one place. We need the FreeRTOS library to be configured appropriately for Marlin and not with the default settings.

  - We need to figure out why the settings.load() function can not be called
    from setup() any more.   It seems to cause some random memory corruption.
    I've spent some time looking at this, and so far have not found the
    answer.   But...  with that said...  if settings.load() is moved to the
    top of the Marlin_main_loop() task, it is able to go through the entire
    routine (with no logic changes in configuration_store.cpp).   PLEASE!!!!
    If you can figure this out, please do!!!

  - Implement the previous concept of no_stepper_sleep for advanced pause in
    the Marlin idle routine. This used to be a parameter passed into idle()
    but this function is now run as a task.

              void idle(
            //#if ENABLED(ADVANCED_PAUSE_FEATURE)
            //  bool no_stepper_sleep/*=false*/
            //#endif ) 

  - Variables that are modified within the scope of idle() (via calls to things like  lcd_update(), host_keepalive(), manage_inactivity(), thermalManager.manage_heater(), print_job_timer.tick(), buzzer.tick() ) need to be checked for atomic access.   We can't have the new idle routine context
    switch in the middle of a variable update.   

  - The thermal code seems to be able to control the bed and hot end
    temperatures.   But I've seen a lot of Thermal Shutdown Kill's.  I have
    not searched for the root cause yet.   Instead, I've turned off Thermal
    Protection for the time being so I can use the time to make further
    progress on RTOS related topics.


## Current Functionality

Marlin 2.1 currently builds for the 8-bit AVR platforms using Arduino.   This needs to be expanded to include coverage for the 32-bit platforms and Platform-IO.

## High Priority Work Items

- Expand build environment to address 32-bit platforms.

- Alter build environment to handle inclusion (and configuration) of the FreeRTOS library gracefully.

## Submitting Patches

Proposed patches should be submitted as a Pull Request against the ([bugfix-2.1.x](https://github.com/MarlinFirmware/Marlin/tree/bugfix-2.1.x)) branch.

- This branch is for fixing bugs and integrating any new features for the duration of the Marlin 2.1.x life-cycle.
- Follow the [Coding Standards](http://marlinfw.org/docs/development/coding_standards.html) to gain points with the maintainers.
- Please submit your questions and concerns to the [Issue Queue](https://github.com/MarlinFirmware/Marlin/issues).

### [RepRap.org Wiki Page](http://reprap.org/wiki/Marlin)

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.


