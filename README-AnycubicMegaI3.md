# Anycubic Mega I3 Marlin port with support for the orignal TFT

For the stock Mega i3 with Ultrabase you find a configuration in the example section. 

## Configuration

The following defines have been added:

ANYCUBIC_TFT_MODEL - Enable the display support
ANYCUBIC_FILAMENT_RUNOUT_SENSOR - Enable the Mega filament runout sensor
ANYCUBIC_TFT_DEBUG - Enable addition debugging messages on the terminal

## Differences to the stock firmware

Additional features:
- Subdirectory support ... you have to press the round arrow after selecting a directory
- Special menu in the SD file menu ... you have to press the round arrow after selecting a special menu item

Changed features:
- Leveling uses auto bed levelling... you can can check the positions afterwards

Missing features:
- Outage support
- Beeper
- Testprint after leveling
