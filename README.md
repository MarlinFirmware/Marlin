# Community firmware for the Creality CR-6 3D printer

**This branch is for the Creality CR-6 SE with stock v4.5.2 motherboard and the stock display.**

_For other configurations for the Creality CR-6 printer (like BigTreeTech SKR board and optional BTT TFT v3.0 display - please check the [branches and development section](#development-and-compile-it-yourself) section below._

## Downloads

Please find official releases in the [Releases section](https://github.com/CR6Community/Marlin/releases). Take the release which belongs to the particular touch screen firmware you are going to flash. Please read the release notes *carefully* - it contains all the instructions you need.

Ensure you take the right assets: the `firmware[suffix].bin`. You should not download the `Source code` archive if you are downloading with the purpose of directly flashing your printer.

*Support for the [BTT SKR board](https://damsteen.nl/blog/2020/11/25/how-to-btt-skr-cr6-installation) is experimental.*

### Development and compile-it-yourself

There are several branches in this project for your convenience. You can directly build them if you have the Platform.io plugin installed in Visual Studio code:

- **[`extui`](https://github.com/CR6Community/Marlin/tree/extui)** - main source branch for CR-6 SE community firmware development; rewritten from scratch; paired with the [`extui`](https://github.com/CR6Community/CR-6-Touchscreen/tree/extui) branch of the CR-6 touch screen repository. This branch is used if you run the *stock* CR-6 mainboard (v4.5.2 `BOARD_CREALITY_V452` or v4.5.3 `BOARD_CREALITY_V453`).
- **[`extui-cr6-max`](https://github.com/CR6Community/Marlin/tree/extui-cr6-max)** - The same as `extui` but for the CR-6 MAX
- **[`extui-btt-skr-stock-tft`](https://github.com/CR6Community/Marlin/tree/extui-btt-skr-stock-tft)** - The same as `extui` but for the Big Tree Tech SKR-CR6 motherboard _and_ when you use the stock Creality CR-6 display
- **[`extui-btt-skr-tft3`](https://github.com/CR6Community/Marlin/tree/extui-btt-skr-tft3)** - Up-to-date Marlin for the CR-6 with the Big Tree Tech SKR-CR6 motherboard _and_ when you use the Big Tree Tech TFT

Legacy branches:

- **[`creality-cr6-merge-attempt`](https://github.com/CR6Community/Marlin/tree/creality-cr6-merge-attempt)** - initial branch based on Creality v1.0.3.7 firmware source code release and upgraded until the community firmware 3 release. All new releases are released from the `extui` branch.

Original source code tracking:

- **[`cr6-creality-changes`](https://github.com/CR6Community/Marlin/tree/cr6-creality-changes)** - tracks the changes from the Creality source code dump against Marlin upstream. As of now we have the Creality v1.0.3.7 firmware on this branch, based on Marlin pre-2.0.

    - **[`v1.0.3.7`](https://github.com/CR6Community/Marlin/tree/official-fw/v1.0.3.7)**
    - **[`v1.0.4.1`](https://github.com/CR6Community/Marlin/tree/official-fw/v1.0.4.1)**

- **[`cr6-btt-dump`](https://github.com/CR6Community/Marlin/tree/cr6-btt-dump) - tracks the changes from the [Big Tree Tech SKR board firmware](https://github.com/bigtreetech/BIGTREETECH-SKR-CR6/tree/master/firmware/BTT-SKR-CR6)** source code (which does not have any git history). It appears the for the moment BTT source code is based on the Creality v1.0.3.7 source code release.

## Purpose of this community firmware

This fork of Marlin is meant for:

- Providing up to date and stable Marlin for the CR-6 SE native and [BTT SKR CR6](https://damsteen.nl/blog/2020/11/25/how-to-btt-skr-cr6-installation) motherboard
- [Expanding the features](https://github.com/CR6Community/CR-6-touchscreen) of the limited Creality CR-6 stock touch screen

Once upstream Marlin supports the strain gauge, [currently being whipped into shape in this PR @Sebazzz has submitted](https://github.com/MarlinFirmware/Marlin/pull/19958), the future of this project will probably be:

- Still expanding the features of the touch screen and merge upstream
- Continuously update this fork to the latest Marlin stable versions
- Provide builds for the CR-6 and SKR boards for the less technically inclined

## Community firmware support & communities

Get in touch with the developers! We [have our own Discord server](https://discord.gg/RKrxYy3Q9N).

The following CR-6 communities exist:

- [Facebook independend CR-6 community](https://www.facebook.com/groups/cr6community)
- [Reddit /r/CR6](https://www.reddit.com/r/CR6/)

Communities hosted by Creality:

- [Official CR-6 user group](https://www.facebook.com/groups/CR6SECR6MAX)
- [Official Creality user group](https://www.facebook.com/groups/creality3dofficial)

Other communities:

- [Reddit /r/3dprinting](https://www.reddit.com/r/3dprinting/)

### General Marlin support

For general Marlin support, please check:

- [Marlin Documentation](http://marlinfw.org) - Official Marlin documentation
- [Marlin Discord](https://discord.gg/n5NJ59y) - Discuss issues with Marlin users and developers
- Facebook Group ["Marlin Firmware"](https://www.facebook.com/groups/1049718498464482/)
- RepRap.org [Marlin Forum](http://forums.reprap.org/list.php?415)
- [Tom's 3D Forums](https://forum.toms3d.org/)
- Facebook Group ["Marlin Firmware for 3D Printers"](https://www.facebook.com/groups/3Dtechtalk/)
- [Marlin Configuration](https://www.youtube.com/results?search_query=marlin+configuration) on YouTube


## Reporting issues

- Submit **bug fixes** as pull requests to the current active default branch (`extui`)
- Follow the [coding standards](https://marlinfw.org/docs/development/coding_standards.html)
- Please submit your questions and concerns in the [issue tracker](https://github.com/MarlinFirmware/Marlin/issues)

## Credits

The current core CR-6 Community firmware dev team consists of:

 - Sebastiaan Dammann [[@Sebazzz](https://github.com/Sebazzz)] - Netherlands &nbsp; ([Donate](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=B7XURY2X39GS6&item_name=CR-6+development&currency_code=EUR) | [Website](https://damsteen.nl))
 - Juan Rodriguez [[@Nushio](https://github.com/Nushio)] - Mexico
 - Romain [[@grobux](https://github.com/grobux)] - France ([Donate](https://www.paypal.com/donate?hosted_button_id=CP2SAW4W9RBT4))
 - Nick Acker [[@nickacker](https://github.com/nickacker)] - USA
 - And more...

We stand on the shoulders of giants. Don't forget to send your love [upstream too](https://github.com/MarlinFirmware/Marlin)!

## License

Marlin and the Creality CR-6 Community Firmware is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.

While we can't prevent the use of this code in products (3D printers, CNC, etc.) that are closed source or crippled by a patent, we would prefer that you choose another firmware or, better yet, make your own.
