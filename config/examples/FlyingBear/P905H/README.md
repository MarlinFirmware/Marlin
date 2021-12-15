# Flying Bear P905H configuration

This configuration is for a P905H with **a single extruder and inductive Z-sensor**. It's been thoroughly tested, and I've tried to make the printer work smoothly, so this configuration is more focused on print quality than speed.

- Use this firmware as a baseline for other P905 modifications.
- Find other notes in the config files by searching for "P905H".

## Build Instructions
  - Get Visual Studio Code and install the "Auto Build Marlin" extension.
  - Download the [Marlin source code](https://marlinfw.org/meta/download/).
  - Copy the two Configuration files from this folder into the 'Marlin' folder (replacing the existing files).
  - Open the 'MarlinFirmware' project folder in Visual Studio Code.
  - Click the "Auto Build Marlin" button and then the Upload button.
  - Don't forget to reset your EEPROM with `M502` and `M500` after flashing.
