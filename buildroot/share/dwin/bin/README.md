# DWIN Icon Tools

Tools for processing `.ICO` files used by DWIN displays.

## Introduction

The DWIN LCDs that come with the Creality Ender-3 V2 and other 3D printers contain image and container files stored on them which are used to draw various the UI elements.

Standard `.JPG` files can be installed for things like the boot screen, and `.ICO` files can contain several images within a structured file format.

## DWIN Banks

Each JPEG and ICO file starts with a number that tells DWIN which bank it should be stored in. Each numbered bank is 32K in size, so a single JPEG or ICO file can be up to 32kB before it starts to overwrite the next bank. For example, if the file loaded into bank 0 is over 32K, the next file should be numbered "2" so it loads into bank 2. The limit to the total size of all files installed on the display is usually 512KB, corresponding to 16 banks. All JPEG images, ICO containers, and fonts must fit into this space, so you may need to apply a lot of JPEG compression.

## The Tools

This folder includes two useful scripts, one to convert JPEG images into ICO files and another to extract JPEGs from ICO files.

### splitIco.py

The `splitIco.py` script reads an ICO file and extracts the component JPEG images, saving them in a new folder. Each icon is named by its index in the ICO, also using symbol names from Marlin's `dwin.h` header file for the display.

### makeIco.py

The `makeIco.py` script reads all the images in a folder and combines them into a single `.ICO` file.

## Dependencies

These tools are written in Python 3 using the [Pillow image library](https://pillow.readthedocs.io/en/latest/index.html).

## Credits

Created by Brent Burton [[@b-pub](https://github.com/b-pub)]

## License

These tools are provided under the GPL 3 license. See the `LICENSE` file for details.

# Usage & Dependencies

These tools must be run from a terminal with access to an installed Python 3 and the Pillow image library.

Pillow is most easily installed with pip:

  python3 -m pip install pillow

## Examples

These tools process an `.ICO` file that you specify. The safest method is to create a folder and copy your `.ICO` file there. For example:

```
  $ mkdir hackicons
  $ cp 7.ICO hackicons
  $ cd hackicons
```

The following explanations will refer back to this layout.

### `splitIco.py` - Split the ICO archive

If you want to edit the individual icons stored in an ICO file (or add more images) you'll first need to extract all the images from the archive using `splitIco.py`.

#### Usage:
```
splitIco.py #.ICO foldername
```

#### Splitting .ICO FIle In Windows:
- Create `Split-ICO.bat` file in this folder with the following code:
  - `for /f %%f in ('dir *.ICO /B /O:-D') do splitico.py %%f %%f-icons`
- Paste `.ICO` file into this folder
- Run `Split-ICO.bat`
- A new folder should appear containing all icons

#### Example:

In this example we're extracting the constituent JPEG files from `7.ICO` and storing them in a folder named `icons-7`. As each file is extracted the script reports its index number, byte offset, size, dimensions, and filename:

```
$ cd buildroot/share/dwin
$ ./bin/splitIco.py 7.ICO icons-7
  Splitting 7.ICO into dir icons-7
  Splitting Entry Data...
  00: offset: 0x001000 len: 0x10a2 width: 130 height: 17
   Wrote 4258 bytes to icons-7/000-ICON_LOGO.jpg
  01: offset: 0x0020a2 len: 0x0eac width: 110 height: 100
  Wrote 3756 bytes to icons-7/001-ICON_Print_0.jpg
  02: offset: 0x002f4e len: 0x0eaa width: 110 height: 100
  Wrote 3754 bytes to icons-7/002-ICON_Print_1.jpg
  ...
  91: offset: 0x0345fc len: 0x0d89 width: 110 height: 100
  Wrote 3465 bytes to icons-7/091-ICON_Info_1.jpg
```

Once the individual JPEG files have been saved they can be edited using common graphics applications like Photoshop. JPEG files are inherently lossy and will usually contain ugly artifacts, so cleanup may be needed before they are re-exported. Keep the limits of bank size in mind when exporting images and try to find the best balance between compressed size and image quality.

### `makeIco.py` - Combine JPEGs into `ICO` archive

If you want to create an ICO file you'll need to use `makeIco.py`.

#### Usage:
```
makeIco.py foldername #.ICO
```

#### Making .ICO FIle In Windows:
- Create `Make-ICO.bat` file in this folder with the following code:
  - ```
    setlocal enabledelayedexpansion
    for /f %%f in ('dir *-icons /B /O:-D') do set f=%%f & makeico.py %%f !f:~0,-7!
    ```
- Paste folder containing all icons into this folder
- Run `Make-ICO.bat`
- A new `.ICO` file should appear

#### Example:

After editing images you'll create a new `7.ICO` archive with `makeIco.py` like so:

```
$ cd buildroot/share/dwin
$ ./bin/makeIco.py icons-7 7.ICO
  Making .ico file '7.ICO' from contents of 'icons-7'
  Scanning icon directory icons-7
  ...Scanned 16 icon files
  Scanning done. 16 icons included.
```
