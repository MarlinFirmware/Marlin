# USB Files Source Documentation

## Source

We sourced the USB files in Marlin from the Atmel ASF (Advanced Software Framework). The framework provides a variety of examples which were utilized in this project.

Atmel doesn't provide these files in a source repository but they can be extracted from ASF, which can be downloaded from Atmel.

[Advanced Software Framework](https://www.microchip.com/en-us/tools-resources/develop/libraries/advanced-software-framework)

## Modifications

The files are mostly unmodified except for minor cosmetic changes but some more significant changes were needed.

The changes that prompted the addition of this README file are listed below. Other changes may have been made prior to this.

1. Modified `uotghs_device_due.c` to resolve race conditions that could leave interrupts asserted when freezing the peripheral clock, resulting in hangs and watchdog resets due to the ensuing interrupt storm.

## Version Information

We don't know the exact version of ASF used as the source. However, the copyright information in the files indicates they are from 2015.

## Upgrade Considerations

We looked at the ASF 3.52.0 files released in 2022 but saw no immediate benefits to justify an upgrade. It's important to note that the files in Marlin don't follow the same folder structure as the files in ASF, which complicates the process of comparing and applying updated files.

When these files are updated it's important to carefully compare them to Marlin's versions so any improvements in the Marlin sources are brought forward.

It would be best to make Marlin's directory structure align with ASF or at least document the source of each file to ease future updates.
