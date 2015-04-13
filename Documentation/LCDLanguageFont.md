# LCD Language Font System

We deal with a variety of different displays.
And we try to display a lot of different languages in different scripts on them.
This system is ought to solve some of the related problems.

## The Displays
We have two different technologies for the displays:

* Character based displays:
  Have a fixed set of symbols (charset - font) in their ROM.
  All of them have a similar but not identical symbol set at the positions 0 to 127 similar to US-ASCII.
  On the other hand symbols at places higher than 127 have mayor differences.
  Until now we know of (and support):
  * 1.) HD44780 and similar with Kana charset A00 https://www.sparkfun.com/datasheets/LCD/HD44780.pdf Page 17
      These are very common, but sadly not very useful when writing in European languages.
  * 2.) HD44780 and similar with Western charset A02 https://www.sparkfun.com/datasheets/LCD/HD44780.pdf Page 18
      These are rare, but fairly useful for European languages. Also a limited number of Cyrillic symbols is available.
  * 3.) HD44780 and similar with Cyrillic charset http://store.comet.bg/download-file.php?id=466 Page 14
      Some of our Russian friends use them.

  At all of them you can define 8 different symbols by yourself. In Marlin they are used for the Feedrate-, Thermometer-, ... symbols

* Full graphic displays:
  Where we have the full freedom to display whatever we want, when we can make a program for it.
  Currently we deal with 128x64 Pixel Displays and divide this area in about 5 Lines with about 22 columns.
  Therefore we need fonts with a bounding box of about 6x10.
  Until now we used a
  * 1.) Marlin-font similar to ISO10646-1 but with special Symbols at the end, what made 'ü' and 'ä' inaccessible, in the size 6x10.
  * 2.) Because these letters where to big for some locations on the info-screen we use a full ISO10646-1 font in the size of 6x9.(3200 byte)
  * 3.) When we define USE_BIG_EDIT_FONT we use an additional ISO10646-1 font with 9x18, eating up another 3120 bytes of progmem - but readable without glasses.

## The Languages
  For the moment Marlin wants to support a lot of languages:
  * en    English
  * pl    Polish
  * fr    French
  * de    German
  * es    Spanish
  * ru    Russian
  * it    Italian
  * pt    Portuguese
  * pt-br Portuguese (Brazil)
  * fi    Finnish
  * an    Aragonese
  * nl    Dutch
  * ca    Catalan
  * eu    Basque-Euskera

  and recently on [Thingiverse](http://www.thingiverse.com/) a new port to
  * jp    [Japanese](http://www.thingiverse.com/thing:664397)

 appeared.

## The Problem
  All of this languages, except the English, normally use extended symbol sets, not contained in US-ASCII.
  Even the English translation uses some Symbols not in US-ASCII. ( '\002' for Thermometer, STR_h3 for '³')
  And worse, in the code itself symbols are used, not taking in account, on what display they are written. [(This is true only for Displays with Japanese charset](https://github.com/MarlinFirmware/Marlin/blob/Development/Marlin/ultralcd_implementation_hitachi_HD44780.h#L218) on Western displays you'll see a '~' and on Cyrillic an 'arrow coming from top - pointing to left', what is quite the opposite of what the programmer wanted.)
  The Germans want to use "ÄäÖöÜüß" the Finnish at least "äö". Other European languages want to see their accents on their letters.
  For other scripts like Cyrillic, Japanese, Greek, Hebrew, ... you have to find totally different symbol sets.

  Until now the problems where ignored widely.
  The German translation used utf8 'ä' and 'ö' and did not care about showing garbage on ALL displays.
  The Russian translators new that their system only works on the Cyrillic character displays and relied on special LCD routines (LiquidCrystalRus.cpp) to handle UTF8 but missed to implement a proper strlen().
  The Japanese translator dealed with to scripts. He introduced a very special font for the full graphic displays and made use of the Japanese version of the character displays. Therefore he ended up with two pretty unreadable language.h files full of '\xxx' definitions.
  Other languages ether tried to avoid wording with their special symbols or ignored the problem at all and used the basic symbols without the accents, dots, ... whatever.

## The (partial) Solution
  On a 'perfect' system like Windows or Linux we'd dig out unifont.ttf and some code from the libraries and they'd do what we want. But we are on a embedded system with very limited resources. So we had to find ways to limit the used space (Alone unifont.ttf is about 12MB) and have to make some compromise.

### Aims:
  * 1.) Make the input for translators as convenient as possible. (Unicode UTF8)
  * 2.) Make the displays show the scripts as good as they can. (fonts, mapping tables)
  * 3.) Don't destroy the existing language files.
  * 3.) Don't loose to much speed
  * 4.) Don't loose to much memory.

### Actions:
  * a.) Declare the display hardware we use. (Configuration.h)
  * b.) Declare the language ore script we use. (Configuration.h)
  * c.) Declare the kind of input we use. Ether direct pointers to the font (\xxx) or UTF8 and the font to use on graphic displays. (language_xx.h)
  * d.) Declare the needed translations. (language_xx.h)
  * e.) Make strlen() work with UTF8. (ultralcd.cpp)
  * f.) Seperate the Marlin Symbols to their own font. (dogm_font_data_Marlin_symbols.h)
  * g.) Make the fontswitch function remember the last used font. (dogm_lcd_implementation.h)
  * h.) Make output functions that count the number of written chars and switch the font to Marlin symbols and back when needed. (dogm_lcd_implementation.h) (ultralcd_implementation_hitachi_HD44780.h)
  * i.) Make three fonts to simulate the HD44780 charsets on dogm-displays. With this fonts the translator can check how his translation will look on the character based displays.
  * j.) Make ISO fonts for Cyrillic and Katakana because they do not need a mapping table and are faster to deal with and have a better charset (less compromises) than the HD44780 fonts.
  * k.) Make mapping functions and tables to convert from UTF8 to the fonts and integrate in the new output functions. (utf_mapper.h)
  * l.) Delete the not needed any more 'LiquidCrystalRus.xxx' files and their calls in 'ultralcd_implementation_hitachi_HD44780.h'.
  * m.) Split 'dogm_font_data_Marlin.h' into separate fonts and delete. (+dogm_font_data_6x9_marlin.h , +dogm_font_data_Marlin_symbols.h, -dogm_font_data_Marlin.h)
  * n.) Do a bit of preprocessor magic to match displays - fonts and mappers in 'utf_mapper.h'.

## Translators handbook
  * a.) Check is there already is a language_xx.h file for your language (-> b.) or not (-> e.)
  * b.) Ether their is declared MAPPER_NON (-> c.) or an other mapper (-> d.)
  * c.) Symbols outside the normal ASCII-range (32-128) are written as "\xxx" and point directly into the font of the hardware you declared in 'Configuration.h'
      This is one of the three fonts of the character based Hitachi displays (DISPLAY_CHARSET_HD44780_JAPAN, DISPLAY_CHARSET_HD44780_WEST, DISPLAY_CHARSET_HD44780_CYRILIC).
      Even on the full graphic displays one of these will be used when SIMULATE_ROMFONT is defined.
      If you don't make use of the extended character set your file will look like 'language_en.h' and your language file will work on all the displays. 
      If you make intensive use, your file will look like 'language_kana.h' and your language file will only work on one of displays. (in this case DISPLAY_CHARSET_HD44780_JAPAN)
      Be careful with the characters 0x5c = '\', and 0x7b - 0x7f. "{|}"These are not the same on all variants.
      MAPPER_NON is the fastest an least memory consuming variant.
      If you want to make use of more than a view symbols outside standard ASCII or want to improve the portability to more different types of displays use UTF8 input. That means define an other mapper.
  * d.) With a mapper different to MAPPER_NON UTF8 input is used. Instead of "\xe1" (on a display with Japanese font) or STR_ae simply use "ä". When the string is read byte by byte , the "ä" will expand to "\0xc3\0xa4" or "Я" will expand to "0xd0\0xaf" or "ホ" will expand to "\0xe3\0x83\0x9b"
      To limit the used memory we can't use all the possibilities UTF8 gives at the same time. We define a subset matching to the language or script we use.
      * MAPPER_C2C3 correspondents good with west European languages the possible symbols are listed at (http://en.wikipedia.org/wiki/Latin-1_Supplement_(Unicode_block))
      * MAPPER_D0D1 correspondents well with the Cyrillic languages. See (http://en.wikipedia.org/wiki/Cyrillic_(Unicode_block))
      * MAPPER_E382E383 works with the Japanese Katakana script. See (http://en.wikipedia.org/wiki/Katakana_(Unicode_block))

      The mapper functions will only catch the 'lead in' described in the mappers name. If the input they get does not match they'll put out a '?' or garbage.
      The last byte in the sequence ether points directly into a matching ISO10646 font or via a mapper_table into one of the HD44780 fonts.
      The mapper_tables do their best to find a similar symbol in the HD44780_fonts. For example replacing small letters with the matching capital letters. But they may fail to find something matching and will output a '?'. There are combinations of language and display what simply have no corresponding symbols - like Cyrillic on a Japanese display or visa versa - than the compiler will throw an error.
      In short: Chose a Mapper working with the symbols you want to use. Use only symbols matching the mapper. On FULL graphic displays all will be fine, but check for daring replacements or question-marks in the output of character based displays by defining SIMULATE_ROMFONT and trying the different variants.
      If you get a lot of question-marks on the Hitachi based displays with your new translation, maybe creating an additional language file with the format 'language_xx_utf8.h' is the way to go.
      * MAPPER_NON is the fastest and least memory consuming variant.
      * Mappers together with a ISO10646_font are the second best choice regarding speed and memory consumption. Only a few more decisions are mad per character.
      * Mappers together with the HD44780_fonts use about additional 128 bytes for the mapping_table.
  * e.) Creating a new language file is not a big thing. Just make a new file with the format 'language_xx.h' or maybe 'language.xx.utf8.h', define a mapper and a font in there and translate some of the strings defined in language_en.h. You can drop the surrounding  #ifndef #endif. You don't have to translate all the stings - the missing one will be added by language_en.h - in English - of cause.
  * f.) If you cant find a matching mapper things will be a bit more complex. With the Hitachi based displays you will not have big chance to make something useful unless you have one with a matching charset. For a full graphic display - lets explain with - let's say Greece.
      Find a matching charset. (http://en.wikipedia.org/wiki/Greek_and_Coptic)
      Provide a font containing the symbols in the right size. Normal ASCII in the lower 127 places, the upper with your selection.
      Write a mapper catching, in this case, 0xcd to 0xcf and add it to 'utf_mapper.h'.
      In case of a ISO10646 font we have a MAPPER_ONE_TO_ONE and don't have to make a table.
  * g.) If you discover enough useful symbols in one of the HD44780 fonts you can provide a mapping table. For example HD44780_WEST contains 'alpha', 'beta', 'pi', 'Sigma', 'omega' 'My' - what is not enough to make USEFUL table - I think.
  * h.) If you want to integrate an entirely new variant of a Hitachi based display.
      Add it in 'Configuration.h'. Define mapper tables in 'utf_mapper.h'. Maybe you need a new mapper function. 

  The length of the strings is limited. '17 chars' was crude rule of thumb. Obviously 17 is to long for the 16x2 displays. A more exact rule would be max_strlen = Displaywidth - 2 - strlen(value to display behind). This is a bit complicated. So try and count is my rule of thumb. 

  On the 16x2 displays the strings are cut at the end to fit on the display. So it's a good idea to make them differ early. ('Somverylongoptionname x' -> 'x Somverylongoptionname')

  You'll find all translatable strings in 'language_en.h'. Please don't translate any strings from 'language.h', this may break the serial protocol.

## User Instructions
   Define your hardware and the wanted language in 'Configuration.h'.
   To find out what charset your hardware is, define language 'test' and compile. In the menu you will see two lines from the upper half of the charset.
   * DISPLAY_CHARSET_HD44780_JAPAN   locks like "バパヒビピフブプヘベペホボポマミ"
   * DISPLAY_CHARSET_HD44780_WESTERN locks like "ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
   * DISPLAY_CHARSET_HD44780_CYRILIC locks like "РСТУФХЦЧШЩЪЫЬЭЮЯ"

   If you get an error about missing mappers during compilation - lie about your displays hardware font to see at lest some garbage, or select an other language.
   
   English works on all hardware.

