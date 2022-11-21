# Marlin for Labists ET4

_Note: The Labists ET4 is OEM'd by Anet, so the update process is very similar._

A debugging/programming probe (_e.g._, ST-Link, J-Link, or Black Magic Probe) is required to flash the the [OpenBLT bootloader for Anet ET4/ET5 series printers](https://github.com/davidtgbe/openblt/releases) due to incompatibility with the stock bootloader. Once OpenBLT is flashed, Marlin can be updated by SD card just like the original firmware.

## Flashing OpenBLT Bootloader with an ST-Link V2

1. Using three female to female Dupont jumper cables, connect `GND`, `SWDIO`, and `SWCLK` pins from the ST-Link to the Serial Wire Debug (SWD) header on the motherboard. Pay close attention that the cables on SWD header side match the order below using the `U` and `G` letters printed on the motherboard as a reference:

    ```
      SWD Header on ET Series motherboard
         ___
      U | o |  (not connected)
        |---|
        | o |  SWDIO
        |---|
        | o |  SWCLCK
        |---|
      G | o |  GND
         ---
    ```
    Photo reference:

    <img src="https://i.imgur.com/IBqE0i0.jpeg" width="50%">

    _`GND` is connected to `GND` (black wire), `SWDIO` to `SWDIO` (yellow wire), and `SWCLK` to `SWCLK` (green wire). Your ST-Link pinout may differ from the one pictured, so double-check pinout._
2. Plug in the ST-Link to your computer's USB port & power on your printer.
3. Download & install [STM32 ST-Link Utility](https://www.st.com/en/development-tools/stsw-link004.html).
4. Launch STM32 ST-Link Utility & click `Target` then `Connect`.
5. Back up the original motherboard firmware starting at address `0x08000000` with a size of `0x100000`:
    <img src="https://i.imgur.com/RGG4B6L.png" width="50%">
6. Download & extract [OpenBLT bootloader for Anet ET4/ET5 series printers](https://github.com/davidtgbe/openblt/releases).
7. Click on `Target` then `Program...`
8. Set the `Start Address` to `0x08000000`.
9. Under `File Path`, click `Browse` and select `openblt_et4.bin` extracted in Step 6.
10. Click `Start` to initiate the flashing process.
11. OpenBLT for Anet ET4/ET5 series printers is now installed on your motherboard.

## Compiling Marlin

1. Overwrite the default Marlin configs with the ones from this folder.
2. Compile with the `Anet_ET4_OpenBLT` PlatformIO environment.

## Installing Marlin

1. Once Marlin is compiled, copy `Marlin/.pio/build/Anet_ET4_OpenBLT/firmware.srec` to a blank SD card and insert it into your printer.
2. Power cycle your printer to start the update process.

## Acknowledgements
- [@davidtgbe](https://github.com/davidtgbe) for porting Marlin to the ET4/ET5 series.
- [Telegram Anet ET4 spanish group](https://t.me/anetet4esp), especially [@olidnon](https://github.com/olidnon), who lent his motherboard for testing.
- [@uwe](https://github.com/uwe) and [@mubes](https://github.com/mubes) from Black Magic Probe team, and to [@Ebiroll](https://github.com/Ebiroll) (BMP/ESP32).
- All contributors and testers.
