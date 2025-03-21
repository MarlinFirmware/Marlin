/************
 * boards.h *
 ************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#define HAS_RESOLUTION (defined(TOUCH_UI_320x240) || defined(TOUCH_UI_480x272) || defined(TOUCH_UI_800x480))

#define IS_FT800 \
  constexpr uint16_t ftdi_chip = 800; \
  using namespace FTDI_FT800; \
  namespace DL { \
    using namespace FTDI_FT800_DL; \
  } \
  typedef ft800_memory_map ftdi_memory_map; \
  typedef ft800_registers  ftdi_registers;

#define IS_FT810 \
  constexpr uint16_t ftdi_chip = 810; \
  using namespace FTDI_FT810; \
  namespace DL { \
    using namespace FTDI_FT800_DL; \
    using namespace FTDI_FT810_DL; \
  } \
  typedef ft810_memory_map ftdi_memory_map; \
  typedef ft810_registers  ftdi_registers;

#ifdef LCD_FTDI_VM800B35A
  #if !HAS_RESOLUTION
    #define TOUCH_UI_320x240
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                800
  #endif
  namespace FTDI {
    IS_FT800
    constexpr bool Use_Crystal            = true;  // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = false; /* 1 = does use GPIO00 for amplifier control, 0 = not in use for Audio */
    constexpr bool GPIO_1_Audio_Shutdown  = true;  /* 1 = does use GPIO01 for amplifier control, 0 = not in use for Audio */
    constexpr uint8_t Swizzle             = 2;
    constexpr uint8_t CSpread             = 1;

    constexpr uint16_t touch_threshold    = 1200; /* touch-sensitivity */
  }

/**
 * Settings for the Haoyu Electronics, 4.3" Graphical LCD Touchscreen, 480x272, SPI, FT800 (FT800CB-HY43B)
 *                                 and   5" Graphical LCD Touchscreen, 480x272, SPI, FT800 (FT800CB-HY50B)
 *  http://www.hotmcu.com/43-graphical-lcd-touchscreen-480x272-spi-ft800-p-111.html?cPath=6_16
 *  http://www.hotmcu.com/5-graphical-lcd-touchscreen-480x272-spi-ft800-p-124.html?cPath=6_16
 * Datasheet:
 *  https://www.hantronix.com/files/data/1278363262430-3.pdf
 *  https://www.haoyuelectronics.com/Attachment/HY43-LCD/LCD%20DataSheet.pdf
 *  https://www.haoyuelectronics.com/Attachment/HY5-LCD-HD/KD50G21-40NT-A1.pdf
 */
#elif defined(LCD_HAOYU_FT800CB)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_480x272
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                800
  #endif
  namespace FTDI {
    IS_FT800
    constexpr bool Use_Crystal            = true; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = false;
    constexpr bool GPIO_1_Audio_Shutdown  = false;
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 1;
    constexpr uint16_t touch_threshold    = 2000; /* touch-sensitivity */
  }

/**
 * Settings for the Haoyu Electronics, 5" Graphical LCD Touchscreen, 800x480, SPI, FT810
 *  http://www.hotmcu.com/5-graphical-lcd-touchscreen-800x480-spi-ft810-p-286.html
 * Datasheet:
 *  https://www.haoyuelectronics.com/Attachment/HY5-LCD-HD/KD50G21-40NT-A1.pdf
 */
#elif defined(LCD_HAOYU_FT810CB)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_800x480
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                810
  #endif
  namespace FTDI {
    IS_FT810
    constexpr bool Use_Crystal            = true; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = false;
    constexpr bool GPIO_1_Audio_Shutdown  = false;
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 1;
    constexpr uint16_t touch_threshold    = 2000; /* touch-sensitivity */
  }

/**
 * Settings for the 4D Systems, 4.3" Embedded SPI Display 480x272, SPI, FT800 (4DLCD-FT843)
 *  https://4dsystems.com.au/4dlcd-ft843
 * Datasheet:
 *  https://4dsystems.com.au/mwdownloads/download/link/id/52/
 */
#elif defined(LCD_4DSYSTEMS_4DLCD_FT843)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_480x272
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                800
  #endif
  namespace FTDI {
    IS_FT800
    constexpr bool Use_Crystal            = true; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = false;
    constexpr bool GPIO_1_Audio_Shutdown  = true;
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 1;
    constexpr uint16_t touch_threshold    = 1200; /* touch-sensitivity */
  }

/**
 * Settings for the Aleph Objects Color LCD User Interface
 * Datasheet https://www.hantronix.com/files/data/s1501799605s500-gh7.pdf
 */
#elif defined(LCD_LULZBOT_CLCD_UI)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_800x480
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                810
  #endif
  namespace FTDI {
    IS_FT810
    constexpr bool Use_Crystal            = false; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = true;  // The AO CLCD uses GPIO0 to enable audio
    constexpr bool GPIO_1_Audio_Shutdown  = false;
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 0;
    constexpr uint16_t touch_threshold    = 2000; /* touch-sensitivity */
  }

/**
 * FYSETC Color LCD
 *  https://www.aliexpress.com/item/4000627651757.html
 * Product information:
 *  https://github.com/FYSETC/TFT81050
 */
#elif defined(LCD_FYSETC_TFT81050)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_800x480
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL 810
  #endif
  namespace FTDI {
    IS_FT810
    constexpr bool Use_Crystal            = false; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = true;  // The AO CLCD uses GPIO0 to enable audio
    constexpr bool GPIO_1_Audio_Shutdown  = false;
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 0;
    constexpr uint16_t touch_threshold    = 2000; /* touch-sensitivity */
  }

/**
 * Settings for EVE3-50G - Matrix Orbital 5.0" 800x480, BT815
 * https://www.matrixorbital.com/ftdi-eve/eve-bt815-bt816/eve3-50g
 * use for example with: https://github.com/RudolphRiedel/EVE_display-adapter
 */
#elif defined(LCD_EVE3_50G)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_800x480
    #define TOUCH_UI_800x480_GENERIC    // use more common timing parameters as the original set
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                810
  #endif
  namespace FTDI {
    IS_FT810
    constexpr bool Use_Crystal            = true; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = false;
    constexpr bool GPIO_1_Audio_Shutdown  = false;
    #define USE_GT911 // this display uses an alternative touch-controller and we need to tell the init function to switch
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 1;
    constexpr uint8_t Pclkpol             = 1;
    constexpr uint16_t touch_threshold    = 1200; /* touch-sensitivity */

    constexpr uint32_t default_transform_a  =  0x000109E4;
    constexpr uint32_t default_transform_b  =  0x000007A6;
    constexpr uint32_t default_transform_c  =  0xFFEC1EBA;
    constexpr uint32_t default_transform_d  =  0x0000072C;
    constexpr uint32_t default_transform_e  =  0x0001096A;
    constexpr uint32_t default_transform_f  =  0xFFF469CF;
  }

/**
 * Settings for EVE2-50G - Matrix Orbital 5.0" 800x480, FT813
 * https://www.matrixorbital.com/ftdi-eve/eve-bt815-bt816/eve3-50g
 * use for example with: https://github.com/RudolphRiedel/EVE_display-adapter
 */
#elif defined(LCD_EVE2_50G)
  #if !HAS_RESOLUTION
    #define TOUCH_UI_800x480
    #define TOUCH_UI_800x480_GENERIC    // use more common timing parameters as the original set
  #endif
  #ifndef FTDI_API_LEVEL
    #define FTDI_API_LEVEL                810
  #endif
  namespace FTDI {
    IS_FT810
    constexpr bool Use_Crystal            = false; // 0 = use internal oscillator, 1 = module has a crystal populated
    constexpr bool GPIO_0_Audio_Enable    = false;
    constexpr bool GPIO_1_Audio_Shutdown  = false;
    #define PATCH_GT911 // this display uses an alternative touch-controller and we need to tell the init function to patch the FT813 for it
    constexpr uint8_t Pclkpol             = 1;
    constexpr uint8_t Swizzle             = 0;
    constexpr uint8_t CSpread             = 1;
    constexpr uint16_t touch_threshold    = 1200; /* touch-sensitivity */

    constexpr uint32_t default_transform_a  =  0x000109E4;
    constexpr uint32_t default_transform_b  =  0x000007A6;
    constexpr uint32_t default_transform_c  =  0xFFEC1EBA;
    constexpr uint32_t default_transform_d  =  0x0000072C;
    constexpr uint32_t default_transform_e  =  0x0001096A;
    constexpr uint32_t default_transform_f  =  0xFFF469CF;
  }

#else

  #error "Unknown or no TOUCH_UI_FTDI_EVE board specified. To add a new board, modify this file."

#endif

/* this data is used to patch FT813 displays that use a GT911 as a touch-controller */
#ifdef PATCH_GT911
  constexpr PROGMEM unsigned char GT911_data[] = {
    26,255,255,255,32,32,48,0,4,0,0,0,2,0,0,0,
    34,255,255,255,0,176,48,0,120,218,237,84,221,111,84,69,20,63,51,179,93,160,148,101,111,76,5,44,141,123,111,161,11,219,154,16,9,16,17,229,156,75,26,11,13,21,227,3,16,252,184,179,
    45,219,143,45,41,125,144,72,67,100,150,71,189,113,18,36,17,165,100,165,198,16,32,17,149,196,240,128,161,16,164,38,54,240,0,209,72,130,15,38,125,48,66,82,30,76,19,31,172,103,46,
    139,24,255,4,227,157,204,156,51,115,102,206,231,239,220,5,170,94,129,137,75,194,216,98,94,103,117,115,121,76,131,177,125,89,125,82,123,60,243,58,142,242,204,185,243,188,118,156,
    227,155,203,238,238,195,251,205,229,71,92,28,169,190,184,84,143,113,137,53,244,103,181,237,87,253,113,137,233,48,12,198,165,181,104,139,25,84,253,155,114,74,191,0,54,138,163,
    12,62,131,207,129,23,217,34,91,31,128,65,246,163,175,213,8,147,213,107,35,203,94,108,3,111,40,171,83,24,15,165,177,222,116,97,23,188,140,206,150,42,102,181,87,78,86,182,170,134,
    215,241,121,26,243,252,2,76,115,217,139,222,206,173,136,132,81,61,35,185,39,113,23,46,199,76,178,54,151,183,224,0,40,189,28,149,182,58,131,79,152,30,76,34,98,234,162,216,133,141,
    102,39,170,40,192,101,53,201,146,191,37,77,44,177,209,74,211,5,206,187,5,6,216,47,53,96,123,22,50,103,251,192,84,17,74,227,185,56,106,51,91,161,96,182,163,48,171,141,139,65,152,
    66,66,11,102,43,158,75,36,80,147,184,147,139,112,17,235,216,103,111,239,245,92,10,175,194,40,44,58,125,5,59,112,50,103,245,4,78,192,5,156,194,51,60,191,134,75,110,173,237,46,192,
    121,156,192,115,184,218,120,67,63,115,46,11,102,10,97,232,50,235,114,182,148,118,178,41,188,12,135,77,202,124,12,96,238,35,161,234,189,129,23,249,212,139,230,25,53,48,205,52,93,
    163,117,53,154,170,81,85,163,178,70,69,66,167,241,14,46,241,1,226,136,152,179,197,59,184,148,254,49,132,48,15,176,137,192,76,131,196,105,104,162,86,81,160,165,255,26,173,162,137,
    86,145,210,183,192,55,175,194,211,60,91,120,230,184,174,27,41,131,155,40,224,29,87,179,232,16,55,55,7,165,147,81,23,165,49,101,54,224,75,180,81,108,18,29,226,69,225,110,175,224,
    42,212,25,47,130,193,110,234,192,215,252,56,74,162,24,46,251,174,54,106,68,245,14,9,155,160,22,120,207,104,240,29,90,178,140,28,24,220,47,166,112,61,251,208,192,111,56,239,238,
    93,255,251,62,99,32,193,75,61,190,235,123,229,110,218,194,85,79,225,59,98,20,238,227,235,220,11,221,149,25,180,116,194,159,111,96,192,24,213,59,139,179,156,215,69,230,19,24,35,
    135,117,206,171,206,162,67,129,234,61,235,11,104,103,84,64,223,167,254,40,163,101,92,84,43,150,46,249,219,205,7,116,11,91,104,61,57,75,223,8,48,25,28,119,252,222,113,49,86,249,
    74,180,211,156,181,61,215,168,157,7,251,199,150,242,250,91,58,132,94,121,7,53,151,139,98,6,165,153,69,214,32,110,211,100,101,31,89,45,81,98,23,205,205,197,209,109,186,198,35,
    141,191,249,25,60,132,223,153,251,98,20,239,146,139,20,217,250,41,250,137,58,177,90,57,79,51,108,233,20,253,194,187,49,222,205,114,141,96,48,175,219,107,54,111,138,22,154,103,
    108,79,58,252,179,178,79,164,195,2,153,36,39,170,199,201,167,197,85,106,8,59,177,81,46,56,2,230,75,114,17,55,112,188,65,208,137,77,114,10,115,55,58,208,197,173,122,87,6,140,
    110,42,208,124,163,70,108,241,104,18,245,98,214,187,134,53,42,221,22,182,133,211,116,148,177,194,209,192,85,90,199,58,55,203,2,229,19,137,187,161,228,154,112,203,145,125,244,
    188,220,118,228,41,201,181,41,195,144,215,183,51,80,250,21,217,16,217,200,235,109,227,188,122,218,142,60,170,224,112,240,184,130,229,224,113,5,223,148,163,80,165,183,130,187,
    132,116,64,238,161,85,220,115,139,205,98,227,244,29,102,125,7,37,243,123,223,11,26,92,63,243,116,61,191,138,123,244,160,84,186,74,31,5,174,247,119,135,199,248,253,135,242,97,
    102,145,190,144,14,85,238,221,231,193,158,48,205,25,120,248,15,220,29,158,9,70,185,30,103,229,33,254,23,237,160,172,62,193,90,222,224,232,14,200,56,90,104,142,227,120,110,6,
    21,211,203,65,150,99,151,220,247,87,164,50,159,49,239,234,58,142,0,109,108,123,18,79,227,36,100,248,222,205,96,127,120,26,171,228,69,63,36,17,252,200,17,116,242,187,227,88,143,
    247,2,75,191,6,130,59,188,11,55,240,31,243,122,152,226,183,207,154,73,188,39,219,43,105,222,87,41,143,141,140,175,73,112,184,252,61,184,16,90,250,35,168,82,119,176,57,116,94,
    200,150,22,190,179,44,104,12,235,84,149,102,252,89,154,193,99,228,106,242,125,248,64,194,255,223,127,242,83,11,255,2,70,214,226,128,0,0
  };
#endif // PATCH_GT911
