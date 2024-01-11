# Display firmware, boot image and icons compilations

Some features, such as G-code preview, need updated firmware to work on some DACAI and TJC displays. Custom icon assets such as Giadej icon set are only supported by DWIN and DACAI displays.

If you have the stock icon set, in principle you **do not need to update the display to use this firmware**, only some TJC display units need a [firmware update](https://github.com/mriscoc/Ender3V2S1/blob/Ender3V2S1-Released/display%20assets/tjc_update.zip?raw=true) to boot properly. Creality currently provides several types of display units with its Ender-3 V2/S1/Neo printers:

**DWIN display**, the original display unit, it supports all firmware functions.  
[![Ender3v2-DWIN](https://user-images.githubusercontent.com/2745567/156829365-a58a3afc-77e3-40b9-9e16-5edfe3073de8.jpg)](https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/displays/DWIN.jpg)

**DACAI display**, some versions need [updated firmware to enable G-code preview](https://github.com/mriscoc/Ender3V2S1/blob/Ender3V2S1-Released/display%20assets/dacai_update.zip?raw=true)  
[![Ender3S1-DACAI](https://user-images.githubusercontent.com/2745567/156829472-2c38a4ab-bdde-4c21-b78f-a30692c96500.jpg)](https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/displays/DACAI.jpg)
  
**SYNWIT (VIEWE) display**, initially unsupported due to incompatibilities with QR-codes (removed in latest firmware version) some users [report problems](https://github.com/mriscoc/Ender3V2S1/issues/323) with this screen, but now seems that it is working without issues.   
[![VIEWE](https://user-images.githubusercontent.com/2745567/163235004-1d3f1ed4-e149-4ca8-ae60-438df5f0b70a.png)](https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/displays/SYNWIT1.jpg)
[![SYNWIT2R](https://user-images.githubusercontent.com/2745567/209407402-25053f01-6a5d-4c76-90c8-da5aec43100c.png)](https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/displays/SYNWIT2.jpg)

**TJC display**, currently this display needs a [special compile](https://github.com/mriscoc/Special_Configurations/releases/tag/tjc) to show leveling numeric data in the mesh viewer and some displays need [updated firmware](https://github.com/mriscoc/Ender3V2S1/blob/Ender3V2S1-Released/display%20assets/tjc_update.zip?raw=true) to enable G-code preview and boot properly, 
more info: [TJC issues](https://github.com/mriscoc/Ender3V2S1/issues/542).  
[![TJC](https://user-images.githubusercontent.com/2745567/206931166-24185525-e377-472e-9bed-37a39aab24fb.jpg)](https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/displays/TJC.jpg)

The `DWIN_SET` display firmware / icon assets only apply to the DWIN display, for the DACAI screens, you should use the `private` display firmware / icon assets.

# Display firmware, boot image and icons compilations
The display assets compilations in this page are compatible with [Professional firmware](https://github.com/mriscoc/Ender3V2S1/releases/latest). Each firmware can have its own compatible icon set.

## How to install
1. Get an µSD card of 8GB or less.
1. Format the µSD card MBR, FAT32 and with a 4 KB sector size
1. Copy the `DWIN_SET`(DWIN), `private`(DACAI) or `TJC_SET` (TJC) folder in the Root of SD
1. Turn off your printer
1. Disconnect and disassembly the screen unit
1. Install the µSD card into the slot of the screen unit
1. Reconnect the screen to the printer
1. Turn on the printer and wait for the display to change color from blue to
  orange
1. Verify that the screen assets were updated
1. Turn off the printer and remove the µSD card from the screen unit
1. Reassembly the screen unit  
  
If you weren't able to update the display, verify the format of the µSD Card
(MBR, FAT32 and allocation unit of 4096 bytes) and the CRC of the files.
  
<img src="https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/DWIN_SET-folder.jpg" height="350" />  

Be sure to leave **only** the `DWIN_SET` or `private` folder at the root of the card.  

### DACAI firmware update
In order to use all the features of this firmware with DACAI screens it has to be updated.
If you can't see preview images with this [g-code test file](https://github.com/mriscoc/Ender3V2S1/raw/Ender3V2S1-Released/slicer%20scripts/cura/SimpleCuraTest.gcode) update your [DACAI screen firmware](dacai_update.zip).
<br>
  
---

## [Go to Stock display firmware](https://github.com/mriscoc/Ender3V2S1/tree/Ender3V2S1-Released/display%20assets/stock)  

<a href=https://github.com/mriscoc/Ender3V2S1/tree/Ender3V2S1-Released/display%20assets/stock><img src="https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/stock/preview1.jpg" height="400" /></a>  
  
<br>
  
---

## [Go to Giadej compilation](https://github.com/mriscoc/Ender3V2S1/tree/Ender3V2S1-Released/display%20assets/Giadej%20compilation)

<a href=https://github.com/mriscoc/Ender3V2S1/tree/Ender3V2S1-Released/display%20assets/Giadej%20compilation><img src="https://raw.githubusercontent.com/mriscoc/Ender3V2S1/Ender3V2S1-Released/display%20assets/Giadej%20compilation/preview1.jpg"  height="400" /></a>  
  
<br>
  
---

### Updating printer firmware
For update your printer firmware and get the latest version go here: <https://github.com/mriscoc/Ender3V2S1/releases/latest>  
