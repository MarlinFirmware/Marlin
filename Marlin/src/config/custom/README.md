### Marlin 2.0.x - CR10 w/ E3D Titan Lite6

This fork of [Marlin Firmware](https://github.com/MarlinFirmware) was customized for use at the Caltech Library TechLab for the following printer and its modifications:

__Hictop CR-20 (Creality CR-10)__   

  + E3D Titan Extruder (Bowden setup)
  + E3D Lite6 Hotend w/ 0.5mm nozzle
  + Capricorn TL PTFE Bowden Tube
  + Buildtak FlexPlate System w/ PEI sheet

----------------------------------------------------------
#### 12.14.18
  - LCD Menu Tree revisions
    - menu_basic.cpp
    - menu_control.cpp
    - menu_filament.cpp
    - menu_main.cpp
    - menu_sdcard.cpp
    - language_en.h
  - Configuration.h
    - Advanced Pause length to 600
    - Preheat PLA/PETG to 205/240
  - Flashed to CR10B
    - 97.8% (used 127126 bytes from 130048 bytes)
----------------------------------------------------------
#### 12.04.18
  - Updated custom configs from 1.1.9 to 2.0.x
    - _Bootscreen.h
    - Configuration.h
    - Configuration_adv.h
    - language_en.h
    - README&#46;md
  - Flashed to CR10B using PIO on vscode [__env:melzi_optiboot__]

----------------------------------------------------------  
__Caltech Library TechLab__  
Web | [library.caltech.edu/techlab](https://www.library.caltech.edu/resources/techlab)  
Instagram | [@caltechlibrarytechlab](http://instagram.com/caltechlibrarytechlab/)

Last updated 12.14.18 by Aucoeur Ngo [[@aucoeurngo](https://github.com/aucoeurngo)]
