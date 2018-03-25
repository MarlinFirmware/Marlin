## Language engine and tools

### Supported hardware

Marlin supports HD44780 character LCD and 128x64 graphical LCD via U8GLIB.
Because of the limitation of HD44780 hardware, Marlin can only support three
character sets for that hardware:
Japanese (kana_utf8), Russian/Cyrillic (ru), or Western (Roman characters)

For the graphical LCD modules, there's far less limitation. Users and language
maintainers can easily change the language translation strings if they save
with UTF-8 encoding. The only extra step is to regenerate the font data from
an existing BDF font file.


### How to support a new language?

1. prepare the translation source file

You need to add a language header file language_xx.h,
where xx is the language/country code, for example en is for English.
You may start with a existing language file by copying it to a new location:

```bash
cp language_zh_CN.h language_zh_TW.h
```

Then you can replace the strings in the macro _UxGT().

Make sure the file is saved as UTF-8 encoded.


2. Generate the font data file

This step is to gather the glyph appearing in the language file from a 9pt bitmap font,
and save the data in a language_data_xx.h file. So the font used for conversion should
contain all the characters in your language file.

The default bitmap font is WQY's 9pt bitmap font. You may download from

http://wenq.org/daily/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz

or install in Debian/Ubuntu system:

```
sudo apt-get install xfonts-wqy
```

You need also compile the bdf2u8g which convert the BDF font file to U8glib supported data structures.
Before you compile bdf2u8g, it would better to patch the source file bdf2u8g.c.

```
cd marlin-git/buildroot/share/fonts/
wget https://github.com/olikraus/u8glib/archive/master.zip
unzip master.zip
cd u8glib-master/
patch -p0 < ../u8glib-bdf2u8g.patch
make -C tools/font/bdf2u8g/
cd -
ln -s u8glib-master/tools/font/bdf2u8g/bdf2u8g
```

The 'genallfont.sh' script will generate the font data for all of the
language translation files. You may edit the script to change the variable
LANGS to the list of languages you want to process. For example:

```bash
LANGS="zh_TW"
```

and then run the script to generate the font data (`language_data_xx.h`):

```bash
cd marlin-git/Marlin/
../buildroot/share/fonts/genallfont.sh
```

3. Change the language settings

To compile the firmware for your language, you need choose a language
in `Configuration.h`. For Chinese (Taiwan) you would use:

```cpp
#define LCD_LANGUAGE zh_TW
```

4. Compile and Upload the firmware

Open `Marlin.ino` in your IDE and compile the firmware. If it's compiled succeeds, upload it to your Arduino hardware.


### Update the language translation

If you change the language files, you need to run the script `genallfont.sh`
again to update the font data file.


### Use a cool font

You may need to use a different font to support your own language, because the
default 9pt font is not complete. (You may also support them by adding the missing
glyphs to the font.)

After you've prepared your font, specify the font file path as an argument to
`genallfont.sh`, so that the font used for your language is your new font. For
example, your new font file name is `newfont.bdf`, then run the following command:

```bash
cd Marlin/
../buildroot/share/fonts/genallfont.sh ./newfont.bdf
```

### Suggestions for Maintainers

Even the tool and the language engine can be easily updated,
since it use the common bitmap font files and UTF-8 text,
the maintainer need to confirm that the fonts contains the glyph
in the language files.

The font file currently used to generate the font data is the file ISO10646-0-3.bdf,
which is combined with the Marlin's ISO10646-1 fonts and WQY 9pt bitmap font.

### Related resources

#### Fonts

- [WQY](http://wenq.org/)
- [WQY 9pt bitmap font](http://wenq.org/daily/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz)
- [unifont (16x8 or 16x16)](http://unifoundry.com/unifont.html)

Documents related to the old version of the language engine:

- [Marlin Fonts Documentation](http://www.marlinfw.org/docs/development/fonts.html)
- [Marlin LCD Language](https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language)
- [U8GLIB](https://github.com/olikraus/u8glib.git)
- [UTF-8 for U8GLIB](https://github.com/yhfudev/u8glib-fontutf8.git)
- [Standalone test project for the Marlin UTF-8 language engine](https://github.com/yhfudev/marlin-fontutf8.git)
