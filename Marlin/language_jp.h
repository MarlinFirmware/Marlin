/**
 * Japanese
 *
 * LCD Menu Messages
 * Please note these are limited to 17 characters!
 *
 */
 
#ifndef LANGUAGE_JP_H
#define LANGUAGE_JP_H

#define KANA

#ifndef REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
  #ifndef DISPLAY_CHARSET_HD44780_JAPAN
    #error "Kana requires DISPLAY_CHARSET_HD44780_JAPAN to be set in Configuration.h"
  #endif
  #include "language_jp1.h" // HD44780
#else
  #include "language_jp2.h" // DOGM
#endif

#endif // LANGUAGE_JP_H
