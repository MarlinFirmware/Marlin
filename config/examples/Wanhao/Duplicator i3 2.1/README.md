Configuration for the Wanhao Duplicator i3 v2.1

[Made by Bot-In-a-Box Educational Robotics](https://botinabox.ca)
[See original configuration repo](https://github.com/BotInABoxER/marlin2-for-wanhao-i3)

Notes:
- YMMV, but the included thermistor table has been tested to work with a 100k bed thermistor with a 10K pull-up resistor
- The smaller Marlin logo is used to save memory
- You may have a 4.7K pull-up resistor, in which case the included thermistor table will be off
- The included thermistor table was generated with `buildroot/share/scripts/createTemperatureLookupMarlin.py --rp=10000`
- Includes an optional custom Wanhao logo bootscreen designed to fit the stock LCD
- Includes an optional custom Chippy from Bot-In-a-Box bootscreen designed to fit the stock LCD

Inspirations:
- https://www.thingiverse.com/thing:3378807 (Custom firmware 1.x by Nitrogen777)
- https://www.thingiverse.com/thing:3378807 (Custom firmware 2.x by Remotheman)

Instructions:
- Copy the `Configuration.h`, `_Bootscreen.h` (or `Chippy_Bootscreen.h`; rename it `_Bootscreen.h`), and `Configuration_adv.h` to the `Marlin/ directory`
- Copy the `thermistor_99.h` and `thermistors.h` files to the `Marlin/src/modules/thermistors` directory
- Flash the firmware onto your Melzi (https://www.fission3d.com/guides/flash-bootloader-and-install-firmware-with-raspberry-pi might help)
