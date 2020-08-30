# Marlin 3D Printer Firmware

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

<img align="right" width=175 src="buildroot/share/pixmaps/logo/marlin-250.png" />

* Update Marlin-BugFix 20200829

Validate:
Firmware for QQS-Pro with A4988.
Firmware for QQS-Pro with TMC2208_Standalone

No validate:
-TMC2209
-TMC220x UART

# Exemple: 8CWBL-Robin_mini.bin =>  TMC2208 standalone/UI Marlin/Module Wifi/Extruder BMG/LinearAdvance  

Caption:
/*------Drivers--------*/
(S) A4988
(8) TMC2208
(9) TMC2209
(U8) Remove module ESP12    

/*    Modules       */
(W)Module ESP8266/ESP12
(T) Extruder Titan
(B) Extruder BMG

/*-------OPTIONS--------*/
  Choice UI TFT 
(F) UI STANDARD 
(C) UI MARLIN 
(I) UI MKS

(B) BED_LEVELING_BILINEAR
(U) BED_LEVELING_UBL

//Many options for Modules: 
(L) Linear Advance (Possible Bug with BabyStep and TMC)  