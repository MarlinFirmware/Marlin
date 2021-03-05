# dwin-ico-tools
Tools to process DWIN LCD display .ICO files.

## What

DWIN LCD displays use a number of image and container files to
skin the UI elements on the display. These displays are used on
Creality's Ender 3 v2 and other 3d printers, and the
configuration files to support these displays is included in the
[Marlin firmware](https://github.com/MarlinFirmware/Marlin).

One file they use is "9.ICO", which is a structured file
containing the icons. This project contains two tools to help
developers manipulate these ICO files.

## The Tools

dwin-ico-tools consists of two tools: splitIco and makeIco.

### splitIco.py

"splitIco.py" reads an ICO file, and extracts the component icon
images, saving each into a new directory. Each icon is named by
its index in the ICO, and is named using symbol names from the
Marlin dwin.h header file for this display.

### makeIco.py

"makeIco.py" is the inverse: it reads the images from a
directory, and combines them to create a valid .ICO file.

## Dependencies

The dwin-ico-tools are written in Python 3, and use the
[Pillow image library](https://pillow.readthedocs.io/en/latest/index.html).

## Versioning

This is a new project and I don't forsee having a strict
versioning in the tools.

That said, the tools correspond the the DWIN image and
header files included in Marlin 2.0.7, as of 1-oct-2020.

## Credits

These tools were created by:
* Brent Burton [[@b-pub](https://github.com/b-pub)]

## License

dwin-ico-tools is published under the GPL 3 license. See
the LICENSE file for details.
