
<img height = 2000 width = 20% align = left  src = './Resources/Space.svg' >
<img height = 2000 width = 20% align = right src = './Resources/Space.svg' >

<div align = center>

# Config Embedding

*Injecting the build configuration into the firmware.*

<br>

Since version  [`2.0.9.3`]  , Marlin is able to automatically extract the  
config used to build the firmware and store it in the firmware binary.

</div>

<br>
<br>

<div align = left>

## Enabling

Uncomment the `CONFIGURATION_EMBEDDING`  
macro definition in [`Configuration_adv.h`]

<br>
<br>

## Packaging

*How and what data is packaged.*

At the start of the PlatformIO build process, all active  
options from the configuration are stored into a JSON  
file, this also includes some extra build information:

<kbd>  Version Info  </kbd>  
<kbd>  Git Revision  </kbd>  
<kbd>  Build Date  </kbd>

<br>

The JSON file is then compressed into a ZIP archive,  
serialized to a C array and stored in a C++ header.

<br>

`marlin_config.json`  🠖  `.pio/build/mc.zip`  🠖  `mc.h`

<br>

This header file is then included into the build itself.

<br>
<br>

## Extraction

To get the configuration out of a firmware binary,  
you'll need a non-write-protected SD card inserted  
into the printer while running the firmware.

<br>

1.  Send the following command to your printer to  
    have it write the  `mc.zip`  file to your SD card.

    ```gcode
    M503 C
    ```

    <br>

2.  Copy the zip archive to your computer, ideally  
    to the same folder as the Marlin repository.

    <br>
    
3.  Run the following series of commands to extract  
    and apply the configuration to your local setup.

    ```sh
    git checkout -f
    unzip mc.zip
    python buildroot/share/PlatformIO/scripts/mc-apply.py
    ```
    
    <br>

    This will attempt to update the configuration files  
    to match the settings used in the original build.

    It will also dump the git reference used to build  
    the code, that may be accessible if the firmware  
    was built from the code of the main repository.
    
    As a fallback the config also includes the following  
    variable which is unlikely to be modified by a fork.

    `STRING_DISTRIBUTION_DATE`
    
</div>

<br>


<!---------------------------------------------------------------->

[`Configuration_adv.h`]: https://github.com/MarlinFirmware/Marlin/blob/bugfix-2.1.x/Marlin/Configuration_adv.h
[`2.0.9.3`]: https://github.com/MarlinFirmware/Marlin/releases/tag/2.0.9.3
