
Language engine and tools
=========================

Supported hardware
------------------

Marlin supports HD44780 character LCD and 128x64 graph LCD via U8GLIB.
Because of the limitation of the HD44780, Marlin can only supports three charsets in those hardwares:
Japaness(kana_utf8), Russian/Cyrillic(ru), or Western(English or western languages)

For the graphic LCD modules, there's no limitation as before. User or language maintainer can easily
change the language translation strings if they save it as UTF-8 encoded. The only extra step is
regenerate the font data from a exist BDF font file.


How to support a new language?
------------------------------
1. prepare the translation source file

You need to add a language header file language_xx.h,
where xx is the language/country code, for example en is for English.
You may start with a existing language file by copying it to a new location:

```bash
cp language_zh_cn.h language_zh_tw.h
```

Then you can replace the strings in the macro _UxGT().

Make sure the file is saved as UTF-8 encode.


2. generate the font data file

This step is to gather the glyph appear in the language file from a 9pt bitmap font,
and save it in a language_data_xx.h file. So the font to convert should contains all
of the characters in your language file.

The default bitmap font is WQY's 9pt bitmap font. You may download from

http://wenq.org/daily/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz

or install in Debian/Ubuntu system:

```
sudo apt-get install xfonts-wqy
```


There's a script file in the source code folder, which will generate the font data for
all of the language translation files. You may edit the script to change the variable
LANG to the list of language you want to process. For example

```bash
LANG="zh_tw"
```

and then you run the script to get the font data:

```bash
./genallfont.sh
```

3. change the language settings

Now, to compile the firmware for your language, you need change the
language setting in file Configuration.h.

```cpp
#define LCD_LANGUAGE zh_tw
```

4. compile and update the firmware

Open the Marlin.ino in the IDE and compile the firmware.


Update the language translation
-------------------------------
If you change the language files, you need to run the script genallfont.sh
again to update the font data file.


Use a cool font
---------------
You may need to use a different font to support your own language,
because the default 9pt font is not complete.
(You may support them by adding missing glyph to the font. )

Suggestions for Maintainer
--------------------------
Even the tool and the language engine can be easily updated,
since it use the common bitmap font files and UTF-8 text,
the maintainer need to confirm that the fonts contains the glyph
in the language files.


Related resources
-----------------

Fonts
WQY: http://wenq.org/
WQY 9pt bitmap font: http://wenq.org/daily/wqy-bitmapfont-bdf-gb18030-nightly_build.tar.gz
unifont (16*8 or 16*16): http://unifoundry.com/unifont.html


The following are some documents related to the old version of language engine:

http://www.marlinfw.org/docs/development/fonts.html
https://github.com/MarlinFirmware/Marlin/wiki/LCD-Language

U8GLIB: https://github.com/olikraus/u8glib.git
UTF-8 for U8GLIB: https://github.com/yhfudev/u8glib-fontutf8.git
