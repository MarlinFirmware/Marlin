# Marlin 3D Printer Firmware for Delta QQS-Pro

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

__Not for production use. Use with caution!__

## Marlin 2.0.7 Branch "FLSUN"
 Update 2020-10-08

### Features of the Preset Configuration

  Features|Active|Value
  --------|------|-----
  Multi-Config Switch QQS-PRO|True|QQS_Config.h
  Module Wifi|True|Ready
  ZoffSet ED3v6|-|Ready|
  Extruder TITAN/BMG| Titan|Ready|
  UI Type|True|Color Marlin|
  TFT Color Selection|True|Classic,Color
  EEPROM|True|-
  Menu PID|True|-
  Arc Support|False|-
  Classic Jerk|True|10
  LinearAdvance|True|0
  Bezier curve acceleration|False|-
  Junction Deviation|False|0.013
  Unified Bed Leveling|True|53pts
  Filament sensor|True|Ready
  Recovery PowerFailed|True|-
  TMC 220x Soft Serial|UART|Ready
  TMC 2209 Hard Serial|UART|Ready
  Neopixel|-|Ready
  
## Marlin 2.0 Bugfix Branch Branch "FLSUN_QQS-PROv2"
 Update Marlin-BugFix 20200905

  ## Validate:

Firmware for QQS-Pro with A4988.

Firmware for QQS-Pro with TMC2208_Standalone

Firmware for QQS-Pro with TMC2208_UART

With activate parts:

[PID_EDIT_MENU]

[ENDSTOP_MENU]

[DELTA_CALIBRATION_MENU ]

[POWER_LOSS_RECOVERY]

[FILAMENT_RUNOUT_SENSOR]

No validate:
-TMC2209

  ## Exemple: 

8CWBL-Robin_mini.bin =>  (8)TMC2208 standalone - (C)UI Marlin - (W)Module Wifi - (B)Extruder BMG - (L)LinearAdvance  

Caption:

  ### /*------Drivers--------*/
(S) A4988

(8) TMC2208

(9) TMC2209

(U8) TMC2208_UART with no module ESP12.

(U8+S) TMC2208 (XYZ) + Choice for E0 (A4988)

(U8+8) TMC2208 (XYZ) + Choice for E0 (TMC220x) 

(U9+) TMC2209 (XYZ) + Choice for E0 (A4988,TMC220x)

  ### /*-------Options UI TFT--------*/
(F) UI STANDARD 

(C) UI MARLIN 

(I) UI MKS

  ### /*------Modules--------*/
(W) Module ESP8266/ESP12

(T) Extruder Titan

(B) Extruder BMG

(N) NeoPixel

  ### /*-------Others options in firmware----*/ 
(A) BED_LEVELING_BILINEAR

(U) BED_LEVELING_UBL

(L) Linear Advance (Possible Bug with BabyStep and TMC)

## For the Wifi Module (MKS_Wifi):
Put the firmware (MksWifi.bin) on the scard with the firmware FLSUN (Robin_mini.bin)
1) Flash original firmware + original mkswifi 
2) Flash original firmware + Custum mkswifi 
3) Inspect the Wifi Access Point and if you see: AP-FLSUN => Flash ok 
4) Flash Marlin 
to run the update of Mks_Wifi or You also can do by web page of the AP (192.168.4.1).

### Initial Configuration after the flash.
1. Open device web page on the AP connected device
Accept Captive portal redirect or
2 Open a web browser and navigate to http://192.168.4.1
3. Upload 2 files in the "zip' and configure the device to your choosing
4. I recommend changing to Station mode and connecting to your home/office Wifi instead of staying in AP mode
5 You may want to change the Baud rate
6. You can change to DHCP, or at the very least setup a Static IP you are familiar with.

More information: [ESP3Dv2.1](https://github.com/luc-github/ESP3D/wiki/Install-Instructions)

Once reconnected to the module's web page, you must load the two files "index.html.gz"
and "preferences.json" to have the user interface.

Enjoy....🙃
