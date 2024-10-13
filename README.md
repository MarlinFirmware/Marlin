# SUNLU S9 with MKS Robin Nano V3

Marlin Firmware configured for SUNLU S9 with MKS Robin Nano V3 motherboard.

Based on [Marlin](https://github.com/MarlinFirmware/Marlin) sources.


### Commit - Minimal installation.

Configured as 'SUNLU S9+'

'MKS Robin Nano V3' Minimum Configuration

Stepper drivers A4988

\+ LIN_ADVANCE


### Commit - MKS Lcd

Fixed observed bugs in MKS.

At the end of a program, the elapsed time is displayed (from version 2.0.9.2)


### Commit - Direct drive

'BMG Dual Drive' extruder with 'Ender-3 CR10' head

Calibrated by M303 E0 S250 C8 U for 'Ender-3 CR10' head

3 materials in Preheat Constants. (on MKS_TS35_V2_0 only 2 work)