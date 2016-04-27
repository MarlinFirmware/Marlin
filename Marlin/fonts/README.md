The fonts are created with Fony.exe (http://hukka.ncn.fi/?fony) because Fontforge didn't do what I want (probably lack of experience).

In Fony export the fonts to bdf-format. (Maybe another one can edit them with Fontforge.) Then run `make_fonts.bat` which calls `bdf2u8g.exe` with the parameters needed to produce the `.h` files. The `.h` files must be edited and moved:
- Replace `#include "u8g.h"` with `#include <utility/u8g.h>`,
- Replace `U8G_FONT_SECTION` with `U8G_SECTION`,
- Insert `.progmem.` right after the first quote `"`,
- Move the file to the main directory.

How to integrate a new font:
Currently we are limited to 256 symbols per font. We use a menu system with 5 lines, on a display with 64 pixel height. That means we have 12 pixels per line. So to have any space between the lines we can use no more than 10 pixel height for the symbols. For up to 11 pixels set TALL_FONT_CORRECTION 1 when loading the font.
To fit 22 Symbols on the 128 pixel wide screen, the symbols can't be wider than 5 pixel, for the first 128 symbols.
For the second half of the font we now support up to 11x11 pixel.

- Get `Fony.exe` from [hukka.ncn.fi](http://hukka.ncn.fi/?fony)
- Copy one of the existing `*.fon` files and use the copy for your work.
- Only change the pixels. Don't change width or height.
- Export as a `*.bdf` file
- Use `bdf2u8g.exe` to produce the `.h` file. Examples for the existing fonts are in `make_fonts.bat`.
- Edit the produced `.h` file to match our needs. Find hints in the `dogm_font_data_.h` files.
- Make a new entry in the font list in `dogm_lcd_implementation.h` before the `#else // fall-back` line:
```cpp
    #elif ENABLED(DISPLAY_CHARSET_NEWNAME)
      #include "dogm_font_data_yourfont.h"
      #define FONT_MENU_NAME YOURFONTNAME
    #else // fall-back
```
- Add your font to the list of permitted fonts in 'language_en.h'
```cpp
    ... || ENABLED(DISPLAY_CHARSET_YOUR_NEW_FONT) ... )
```