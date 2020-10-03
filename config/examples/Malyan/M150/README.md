# Configuration for Malyan M150 HobbyKing printer

Config without automatic bed level sensor, or in other words, "as stock"

## To install:

1. Install [Arduino](https://www.arduino.cc/en/Main/Software)

1. Install U8glib
    * `Sketch` -> `Include Library` -> `Manage Libraries...`
    * Search for and install `U8glib` by oliver

1. Install Sanguino
    * `File` -> `Preferences`
    * Add
    `https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json`
    to `Additional Boards Manager URLs`

1. Modify Sanguino `boards.txt`
    * Close Arduino
    * Locate Arduino15 folder
        - `C:\Users\<username>\AppData\Local\Arduino15` for Windows
        - `~/.arduino15` for Linux

    * Locate `boards.txt` in `packages/Sanguino/hardware/avr/1.0.2`
    (version number may change)
    * Add the following to the end of `boards.txt`
    (note that it is the same as sanguino.menu.cpu.atmega1284p but with
    a different name and upload speed)

            ## Malyan M150 W/ ATmega1284P 16MHz
            sanguino.menu.cpu.malyan_m150=Malyan M150
            sanguino.menu.cpu.malyan_m150.upload.maximum_size=130048
            sanguino.menu.cpu.malyan_m150.upload.maximum_data_size=16384
            sanguino.menu.cpu.malyan_m150.upload.speed=57600
            sanguino.menu.cpu.malyan_m150.bootloader.file=optiboot/optiboot_atmega1284p.hex
            sanguino.menu.cpu.malyan_m150.build.mcu=atmega1284p
            sanguino.menu.cpu.malyan_m150.build.f_cpu=16000000L

1. Configure Marlin
    * Copy `_Bootscreen.h`, `Configuration.h`, and `Configuration_adv.h`
    from `config/examples/Malyan/M150` to `Marlin`
    (overwrite files)
    * Read `Configuration.h` and make any necessary changes

1. Flash Marlin
    * Turn on printer while pressing scroll wheel button
    * Plug printer in to computer with USB cable
    * Open `Marlin/Marlin.ino` with Arduino
    * Configure Arduino
        - `Tools` -> `Board` -> `Sanguino`
        - `Tools` -> `Processor` -> `Malyan M150`
        - `Tools` -> `Port` -> Select your port

    * `Sketch` -> `Upload` or click arrow in top right corner
