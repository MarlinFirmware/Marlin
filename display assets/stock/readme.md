# Stock display assets, boot images and icons

<img  src="preview1.jpg"  height="400" />
  
## How to install
  
[**Click to Download DWIN_SET**](https://downgit.github.io/#/home?url=https://github.com/mriscoc/Ender3V2S1/tree/Ender3V2S1-Released/display%20assets/stock/DWIN_SET) (for DWIN screen units)  
[**Click to Download private**](https://downgit.github.io/#/home?url=https://github.com/mriscoc/Ender3V2S1/tree/Ender3V2S1-Released/display%20assets/stock/private) (for DACAI screen units)  
  
1. Get an µSD card of 8GB or less.
1. Format the µSD card MBR, FAT32 and with a 4 KB sector size
1. Copy the `DWIN_SET`, for Ender3V2 or `private` for Ender3S1 folder in the Root of SD
1. Turn off your printer
1. Disconnect and dissasembly the screen unit
1. Install the µSD card into the slot of the screen unit
1. Reconnect the screen to the printer
1. Turn on the printer and wait for the display to change color from blue to
  orange
1. Verify that the screen assets were updated
1. Turn off the printer and remove the µSD card from the screen unit
1. Reassembly the screen unit  
  
>For the Ender3V2, if you want to change only the icons, then you can left only the **9.ICO** file
inside of the DWIN_SET folder.  

If you wasn't able to update the display, verify the format of the µSD Card
(MBR, FAT32 and allocation unit of 4096 bytes) and the CRC of the files.  
  

<img src="../DWIN_SET-folder.jpg"  height="350" />
  
Be sure to leave **only** the `DWIN_SET` or `private` folder at the root of the card.  
  

## Credits
- [**Creality**](https://www.creality.com/download)  

## DWIN_SET CRC  
|File                        | SHA-256
|----------------------------|-----------------------
|9.ICO                       | CDFBAFBFD5F4C4D502BB55FB71740E672FB6112B0275C70BC4F3CB648EB9BBC0
|T5UIC1.CFG                  | E1C573639BFA2B3A06C2FA7AD3CAB483653DD3DC383217FF653FAB3145458095
|T5UIC1_V20_4??_191022.BIN | F8F9A3075AE5516328044ACB79CA522753133B66F1ECBD108E7B5DB2F3FF2FE5

  
<br>
  
---

### Updating printer firmware
For update your printer firmware and get the latest version go here: <https://github.com/mriscoc/Ender3V2S1/releases/latest>  


