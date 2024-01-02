# USB Files Source Documentation

## Source

The USB files in this project were sourced from the Atmel ASF (Advanced Software Framework). The framework provides a variety of examples, which were utilized in this project.

Atmel does not provide these files in a source repository, but they can be extracted from ASF, which can be downloaded from Atmel.

[Advanced Software Framework](https://www.microchip.com/en-us/tools-resources/develop/libraries/advanced-software-framework)

## Modifications

While the files are mostly unmodified except for minor cosmetic changes, some more significant changes have been required.

The change which prompted the addition of this file is below. Other changes may have been made prior to this.

1. uotghs_device_due.c was modified to resolve race conditions which could leave interrupts asserted when freezing the peripheral clock. This resulted in hangs and watchdog resets due to the ensuing interrupt storm.

## Version Information

The exact version of ASF used as the source is currently unknown. However, the copyright information in the files indicates they are from the year 2015.

## Upgrade Considerations

A comparison was made with the ASF 3.52.0 files released in 2022. No immediate benefits were identified that would justify an upgrade. It's important to note that the files in Marlin do not follow the same folder structure as the files in ASF, which complicates the process of comparing and consuming updated files.

When these files are updated, it is important that they are carefully compared to those in Marlin, to migrate forward any improvements made in the Marlin source code.

It would be best if the Marlin directory structure could be made to align with the ASF structure, or to at least document the source of each file to ease future updates.
