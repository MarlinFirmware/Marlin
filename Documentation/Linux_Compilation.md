# Development on Linux
## Compiling the Source Code
The compilation process has two stages:

1. Choose the target device.

  If the target device is Witbox, Hephestos or Hephestos XL, the firmware language can be set. For Witbox 2 and Hephestos 2, the firmware image is multilanguage and this option has no effect.

  ```
  make <target> [<language>]
  ```

  List of targets:

  * witbox
  * hephestos
  * hephestos_xl
  * witbox_2
  * hephestos_2

  List of languages:

  * DE: German
  * EN: English
  * ES: Spanish
  * FI: Finnish
  * FR: French
  * IT: Italian
  * NL: Dutch
  * PL: Polish
  * PT: Portuguese

2. Build the image for the selected target.

  ```
  make
  ```

## Flashing the Binary Image
For upload the generated image, connect your computer to the printer and type the next line:

```
  make upload
```