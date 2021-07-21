/***************
 * constants.h *
 ***************/

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

/****************************************************************************
 * This header defines constants and commands for the FTDI FT810 LCD Driver *
 * chip.                                                                    *
 ****************************************************************************/

#pragma once

// OPTIONS

namespace FTDI {
  constexpr uint16_t OPT_3D           = 0x0000;
  constexpr uint16_t OPT_RGB565       = 0x0000;
  constexpr uint16_t OPT_MONO         = 0x0001;
  constexpr uint16_t OPT_NODL         = 0x0002;
  constexpr uint16_t OPT_FLAT         = 0x0100;
  constexpr uint16_t OPT_SIGNED       = 0x0100;
  constexpr uint16_t OPT_CENTERX      = 0x0200;
  constexpr uint16_t OPT_CENTERY      = 0x0400;
  constexpr uint16_t OPT_CENTER       = (OPT_CENTERX | OPT_CENTERY);
  constexpr uint16_t OPT_RIGHTX       = 0x0800;
  constexpr uint16_t OPT_NOBACK       = 0x1000;
  constexpr uint16_t OPT_NOTICKS      = 0x2000;
  constexpr uint16_t OPT_NOHM         = 0x4000;
  constexpr uint16_t OPT_NOPOINTER    = 0x4000;
  constexpr uint16_t OPT_NOSECS       = 0x8000;
  constexpr uint16_t OPT_NOHANDS      = (OPT_NOPOINTER | OPT_NOSECS);
}

namespace FTDI_FT810 {
  constexpr uint16_t OPT_NOTEAR      = 0x0004;
  constexpr uint16_t OPT_FULLSCREEN  = 0x0008;
  constexpr uint16_t OPT_MEDIAFIFO   = 0x0010;
  constexpr uint16_t OPT_SOUND       = 0x0020;
}

// GPIO Bits

namespace FTDI {
  constexpr uint8_t GPIO_GP0         = 1 << 0;
  constexpr uint8_t GPIO_GP1         = 1 << 1;
  constexpr uint8_t GPIO_DISP        = 1 << 7;
}

namespace FTDI_FT810 {
  constexpr uint16_t GPIOX_GP0       = 1 << 0;
  constexpr uint16_t GPIOX_GP1       = 1 << 1;
  constexpr uint16_t GPIOX_DISP      = 1 << 15;
}

// HOST COMMANDS

namespace FTDI {
  constexpr uint8_t ACTIVE  = 0x00;
  constexpr uint8_t STANDBY = 0x41;
  constexpr uint8_t SLEEP   = 0x42;
  constexpr uint8_t PWRDOWN = 0x50;
  constexpr uint8_t CLKEXT  = 0x44;
  constexpr uint8_t CLKINT  = 0x48;
  constexpr uint8_t CORESET = 0x68;
}

namespace FTDI_FT800 {
    constexpr uint8_t CLK48M  = 0x62;
    constexpr uint8_t CLK36M  = 0x61;
}

namespace FTDI_FT810 {
    constexpr uint8_t CLKSEL  = 0x61;
}

// DISPLAY LIST COMMANDS

namespace FTDI {
  constexpr uint8_t ARGB1555                           = 0;
  constexpr uint8_t L1                                 = 1;
  constexpr uint8_t L2                                 = 17;
  constexpr uint8_t L4                                 = 2;
  constexpr uint8_t L8                                 = 3;
  constexpr uint8_t RGB332                             = 4;
  constexpr uint8_t ARGB2                              = 5;
  constexpr uint8_t ARGB4                              = 6;
  constexpr uint8_t RGB565                             = 7;
  constexpr uint8_t PALETTED                           = 8;
  constexpr uint8_t TEXT8X8                            = 9;
  constexpr uint8_t TEXTVGA                            = 10;
  constexpr uint8_t BARGRAPH                           = 11;

  constexpr uint8_t ALPHA_FUNC_NEVER                   = 0;
  constexpr uint8_t ALPHA_FUNC_LESS                    = 1;
  constexpr uint8_t ALPHA_FUNC_LEQUAL                  = 2;
  constexpr uint8_t ALPHA_FUNC_GREATER                 = 3;
  constexpr uint8_t ALPHA_FUNC_GEQUAL                  = 4;
  constexpr uint8_t ALPHA_FUNC_EQUAL                   = 5;
  constexpr uint8_t ALPHA_FUNC_NOTEQUAL                = 6;
  constexpr uint8_t ALPHA_FUNC_ALWAYS                  = 7;

  constexpr uint8_t NEAREST                            = 0;
  constexpr uint8_t BILINEAR                           = 1;
  constexpr uint8_t BORDER                             = 0;
  constexpr uint8_t REPEAT                             = 1;

  constexpr uint8_t BLEND_FUNC_ZERO                    = 0;
  constexpr uint8_t BLEND_FUNC_ONE                     = 1;
  constexpr uint8_t BLEND_FUNC_SRC_ALPHA               = 2;
  constexpr uint8_t BLEND_FUNC_DST_ALPHA               = 3;
  constexpr uint8_t BLEND_FUNC_ONE_MINUS_SRC_ALPHA     = 4;
  constexpr uint8_t BLEND_FUNC_ONE_MINUS_DST_ALPHA     = 5;

  constexpr uint32_t COLOR_MASK_RED                    = 8;
  constexpr uint32_t COLOR_MASK_GRN                    = 4;
  constexpr uint32_t COLOR_MASK_BLU                    = 2;
  constexpr uint32_t COLOR_MASK_ALPHA                  = 1;

  constexpr uint8_t STENCIL_FUNC_NEVER                 = 0;
  constexpr uint8_t STENCIL_FUNC_LESS                  = 1;
  constexpr uint8_t STENCIL_FUNC_LEQUAL                = 2;
  constexpr uint8_t STENCIL_FUNC_GREATER               = 3;
  constexpr uint8_t STENCIL_FUNC_GEQUAL                = 4;
  constexpr uint8_t STENCIL_FUNC_EQUAL                 = 5;
  constexpr uint8_t STENCIL_FUNC_NOTEQUAL              = 6;
  constexpr uint8_t STENCIL_FUNC_ALWAYS                = 7;

  constexpr uint8_t STENCIL_OP_ZERO                    = 0;
  constexpr uint8_t STENCIL_OP_KEEP                    = 1;
  constexpr uint8_t STENCIL_OP_REPLACE                 = 2;
  constexpr uint8_t STENCIL_OP_INCR                    = 3;
  constexpr uint8_t STENCIL_OP_DECR                    = 4;
  constexpr uint8_t STENCIL_OP_INVERT                  = 5;

  typedef enum : uint32_t {
   BITMAPS                                             = 1,
   POINTS                                              = 2,
   LINES                                               = 3,
   LINE_STRIP                                          = 4,
   EDGE_STRIP_R                                        = 5,
   EDGE_STRIP_L                                        = 6,
   EDGE_STRIP_A                                        = 7,
   EDGE_STRIP_B                                        = 8,
   RECTS                                               = 9
  } begin_t;
}

namespace FTDI_FT800_DL {
  constexpr uint32_t ALPHA_FUNC                         = 0x09000000;
  constexpr uint32_t BEGIN                              = 0x1F000000;
  constexpr uint32_t BITMAP_HANDLE                      = 0x05000000;
  constexpr uint32_t BITMAP_LAYOUT                      = 0x07000000;
  constexpr uint32_t BITMAP_SIZE                        = 0x08000000;
  constexpr uint32_t BITMAP_SOURCE                      = 0x01000000;
  constexpr uint32_t BITMAP_TRANSFORM_A                 = 0x15000000;
  constexpr uint32_t BITMAP_TRANSFORM_B                 = 0x16000000;
  constexpr uint32_t BITMAP_TRANSFORM_C                 = 0x17000000;
  constexpr uint32_t BITMAP_TRANSFORM_D                 = 0x18000000;
  constexpr uint32_t BITMAP_TRANSFORM_E                 = 0x19000000;
  constexpr uint32_t BITMAP_TRANSFORM_F                 = 0x1A000000;
  constexpr uint32_t BLEND_FUNC                         = 0x0B000000;
  constexpr uint32_t CALL                               = 0x1D000000;
  constexpr uint32_t CELL                               = 0x06000000;
  constexpr uint32_t CLEAR                              = 0x26000000;
  constexpr uint32_t CLEAR_COLOR_BUFFER                 = 0x00000004;
  constexpr uint32_t CLEAR_STENCIL_BUFFER               = 0x00000002;
  constexpr uint32_t CLEAR_TAG_BUFFER                   = 0x00000001;
  constexpr uint32_t CLEAR_COLOR_A                      = 0x0F000000;
  constexpr uint32_t CLEAR_COLOR_RGB                    = 0x02000000;
  constexpr uint32_t CLEAR_STENCIL                      = 0x11000000;
  constexpr uint32_t CLEAR_TAG                          = 0x12000000;
  constexpr uint32_t COLOR_A                            = 0x10000000;
  constexpr uint32_t COLOR_MASK                         = 0x20000000;
  constexpr uint32_t COLOR_RGB                          = 0x04000000;
  constexpr uint32_t DL_DISPLAY                         = 0x00000000;
  constexpr uint32_t END                                = 0x21000000;
  constexpr uint32_t JUMP                               = 0x1E000000;
  constexpr uint32_t LINE_WIDTH                         = 0x0E000000;
  constexpr uint32_t MACRO                              = 0x25000000;
  constexpr uint32_t POINT_SIZE                         = 0x0D000000;
  constexpr uint32_t RESTORE_CONTEXT                    = 0x23000000;
  constexpr uint32_t RETURN                             = 0x24000000;
  constexpr uint32_t SAVE_CONTEXT                       = 0x22000000;
  constexpr uint32_t SCISSOR_SIZE                       = 0x1C000000;
  constexpr uint32_t SCISSOR_XY                         = 0x1B000000;
  constexpr uint32_t STENCIL_FUNC                       = 0x0A000000;
  constexpr uint32_t STENCIL_MASK                       = 0x13000000;
  constexpr uint32_t STENCIL_OP                         = 0x0C000000;
  constexpr uint32_t TAG                                = 0x03000000;
  constexpr uint32_t TAG_MASK                           = 0x14000000;
  constexpr uint32_t VERTEX2F                           = 0x40000000;
  constexpr uint32_t VERTEX2II                          = 0x80000000;
}

namespace FTDI_FT810_DL {
  constexpr uint32_t NOP                                = 0x25000000;
  constexpr uint32_t BITMAP_LAYOUT_H                    = 0x28000000;
  constexpr uint32_t BITMAP_SIZE_H                      = 0x29000000;
  constexpr uint32_t VERTEX_FORMAT                      = 0x27000000;
  constexpr uint32_t VERTEX_TRANSLATE_X                 = 0x2B000000;
  constexpr uint32_t VERTEX_TRANSLATE_Y                 = 0x2C000000;
}

// CO-PROCESSOR ENGINE COMMANDS
namespace FTDI {
  constexpr uint32_t CMD_DLSTART                        = 0xFFFFFF00;
  constexpr uint32_t CMD_SWAP                           = 0xFFFFFF01;
  constexpr uint32_t CMD_COLDSTART                      = 0xFFFFFF32;
  constexpr uint32_t CMD_INTERRUPT                      = 0xFFFFFF02;
  constexpr uint32_t CMD_APPEND                         = 0xFFFFFF1E;
  constexpr uint32_t CMD_REGREAD                        = 0xFFFFFF19;
  constexpr uint32_t CMD_MEMWRITE                       = 0xFFFFFF1A;
  constexpr uint32_t CMD_INFLATE                        = 0xFFFFFF22;
  constexpr uint32_t CMD_LOADIMAGE                      = 0xFFFFFF24;
  constexpr uint32_t CMD_MEMCRC                         = 0xFFFFFF18;
  constexpr uint32_t CMD_MEMZERO                        = 0xFFFFFF1C;
  constexpr uint32_t CMD_MEMSET                         = 0xFFFFFF1B;
  constexpr uint32_t CMD_MEMCPY                         = 0xFFFFFF1D;
  constexpr uint32_t CMD_BUTTON                         = 0xFFFFFF0D;
  constexpr uint32_t CMD_CLOCK                          = 0xFFFFFF14;
  constexpr uint32_t CMD_FGCOLOR                        = 0xFFFFFF0A;
  constexpr uint32_t CMD_BGCOLOR                        = 0xFFFFFF09;
  constexpr uint32_t CMD_GRADCOLOR                      = 0xFFFFFF34;
  constexpr uint32_t CMD_GAUGE                          = 0xFFFFFF13;
  constexpr uint32_t CMD_GRADIENT                       = 0xFFFFFF0B;
  constexpr uint32_t CMD_KEYS                           = 0xFFFFFF0E;
  constexpr uint32_t CMD_PROGRESS                       = 0xFFFFFF0F;
  constexpr uint32_t CMD_SCROLLBAR                      = 0xFFFFFF11;
  constexpr uint32_t CMD_SLIDER                         = 0xFFFFFF10;
  constexpr uint32_t CMD_DIAL                           = 0xFFFFFF2D;
  constexpr uint32_t CMD_TOGGLE                         = 0xFFFFFF12;
  constexpr uint32_t CMD_TEXT                           = 0xFFFFFF0C;
  constexpr uint32_t CMD_NUMBER                         = 0xFFFFFF2E;
  constexpr uint32_t CMD_LOADIDENTITY                   = 0xFFFFFF26;
  constexpr uint32_t CMD_SETMATRIX                      = 0xFFFFFF2A;
  constexpr uint32_t CMD_GETMATRIX                      = 0xFFFFFF33;
  constexpr uint32_t CMD_GETPTR                         = 0xFFFFFF23;
  constexpr uint32_t CMD_GETPROPS                       = 0xFFFFFF25;
  constexpr uint32_t CMD_SCALE                          = 0xFFFFFF28;
  constexpr uint32_t CMD_ROTATE                         = 0xFFFFFF29;
  constexpr uint32_t CMD_TRANSLATE                      = 0xFFFFFF27;
  constexpr uint32_t CMD_CALIBRATE                      = 0xFFFFFF15;
  constexpr uint32_t CMD_SPINNER                        = 0xFFFFFF16;
  constexpr uint32_t CMD_SCREENSAVER                    = 0xFFFFFF2F;
  constexpr uint32_t CMD_SKETCH                         = 0xFFFFFF30;
  constexpr uint32_t CMD_STOP                           = 0xFFFFFF17;
  constexpr uint32_t CMD_SETFONT                        = 0xFFFFFF2B;
  constexpr uint32_t CMD_TRACK                          = 0xFFFFFF2C;
  constexpr uint32_t CMD_SNAPSHOT                       = 0xFFFFFF1F;
  constexpr uint32_t CMD_LOGO                           = 0xFFFFFF31;
}

namespace FTDI_FT810 {
  constexpr uint32_t CMD_SETROTATE                    = 0xFFFFFF36;
  constexpr uint32_t CMD_SNAPSHOT2                    = 0xFFFFFF37;
  constexpr uint32_t CMD_SETBASE                      = 0xFFFFFF38;
  constexpr uint32_t CMD_MEDIAFIFO                    = 0xFFFFFF39;
  constexpr uint32_t CMD_PLAYVIDEO                    = 0xFFFFFF3A;
  constexpr uint32_t CMD_SETFONT2                     = 0xFFFFFF3B;
  constexpr uint32_t CMD_SETSCRATCH                   = 0xFFFFFF3C;
  constexpr uint32_t CMD_ROMFONT                      = 0xFFFFFF3F;
  constexpr uint32_t CMD_VIDEOSTART                   = 0xFFFFFF40;
  constexpr uint32_t CMD_VIDEOFRAME                   = 0xFFFFFF41;
  constexpr uint32_t CMD_SETBITMAP                    = 0xFFFFFF43;
}

namespace FTDI {
  enum effect_t : unsigned char {
    SILENCE                                         = 0x00,
    SQUARE_WAVE                                     = 0x01,
    SINE_WAVE                                       = 0x02,
    SAWTOOTH_WAVE                                   = 0x03,
    TRIANGLE_WAVE                                   = 0x04,
    BEEPING                                         = 0x05,
    ALARM                                           = 0x06,
    WARBLE                                          = 0x07,
    CAROUSEL                                        = 0x08,
    SHORT_PIPS_1                                    = 0x10,
    SHORT_PIPS_2                                    = 0x11,
    SHORT_PIPS_3                                    = 0x12,
    SHORT_PIPS_4                                    = 0x13,
    SHORT_PIPS_5                                    = 0x14,
    SHORT_PIPS_6                                    = 0x15,
    SHORT_PIPS_7                                    = 0x16,
    SHORT_PIPS_8                                    = 0x17,
    SHORT_PIPS_9                                    = 0x18,
    SHORT_PIPS_10                                   = 0x19,
    SHORT_PIPS_11                                   = 0x1A,
    SHORT_PIPS_12                                   = 0x1B,
    SHORT_PIPS_13                                   = 0x1C,
    SHORT_PIPS_14                                   = 0x1D,
    SHORT_PIPS_15                                   = 0x1E,
    SHORT_PIPS_16                                   = 0x1F,
    DTMF_POUND                                      = 0x23,
    DTMF_STAR                                       = 0x2C,
    DTMF_0                                          = 0x30,
    DTMF_1                                          = 0x31,
    DTMF_2                                          = 0x32,
    DTMF_3                                          = 0x33,
    DTMF_4                                          = 0x34,
    DTMF_5                                          = 0x35,
    DTMF_6                                          = 0x36,
    DTMF_7                                          = 0x37,
    DTMF_8                                          = 0x38,
    DTMF_9                                          = 0x39,
    HARP                                            = 0x40,
    XYLOPHONE                                       = 0x41,
    TUBA                                            = 0x42,
    GLOCKENSPIEL                                    = 0x43,
    ORGAN                                           = 0x44,
    TRUMPET                                         = 0x45,
    PIANO                                           = 0x46,
    CHIMES                                          = 0x47,
    MUSIC_BOX                                       = 0x48,
    BELL                                            = 0x49,
    CLICK                                           = 0x50,
    SWITCH                                          = 0x51,
    COWBELL                                         = 0x52,
    NOTCH                                           = 0x53,
    HIHAT                                           = 0x54,
    KICKDRUM                                        = 0x55,
    POP                                             = 0x56,
    CLACK                                           = 0x57,
    CHACK                                           = 0x58,
    MUTE                                            = 0x60,
    UNMUTE                                          = 0x61
  };

  enum note_t : unsigned char {
    END_SONG                                        = 0xFF,
    REST                                            = 0x00,

    NOTE_C1                                         = 0x18, // 24
    NOTE_C1S                                        = 0x19,
    NOTE_D1                                         = 0x1A,
    NOTE_D1S                                        = 0x1B,
    NOTE_E1                                         = 0x1C,
    NOTE_F1                                         = 0x1D,
    NOTE_F1S                                        = 0x1E,
    NOTE_G1                                         = 0x1F,
    NOTE_G1S                                        = 0x20,
    NOTE_A1                                         = 0x21,
    NOTE_A1S                                        = 0x22,
    NOTE_B1                                         = 0x23,

    NOTE_C2                                         = 0x24,  //36
    NOTE_C2S                                        = 0x25,
    NOTE_D2                                         = 0x26,
    NOTE_D2S                                        = 0x27,
    NOTE_E2                                         = 0x28,
    NOTE_F2                                         = 0x29,
    NOTE_F2S                                        = 0x2A,
    NOTE_G2                                         = 0x2B,
    NOTE_G2S                                        = 0x2C,
    NOTE_A2                                         = 0x2D,
    NOTE_A2S                                        = 0x2E,
    NOTE_B2                                         = 0x2F,

    NOTE_C3                                         = 0x30,
    NOTE_C3S                                        = 0x31,
    NOTE_D3                                         = 0x32,
    NOTE_D3S                                        = 0x33,
    NOTE_E3                                         = 0x34,
    NOTE_F3                                         = 0x35,
    NOTE_F3S                                        = 0x36,
    NOTE_G3                                         = 0x37,
    NOTE_G3S                                        = 0x38,
    NOTE_A3                                         = 0x39,
    NOTE_A3S                                        = 0x3A,
    NOTE_B3                                         = 0x3B,

    NOTE_C4                                         = 0x3C,
    NOTE_C4S                                        = 0x3D,
    NOTE_D4                                         = 0x3E,
    NOTE_D4S                                        = 0x3F,
    NOTE_E4                                         = 0x40,
    NOTE_F4                                         = 0x41,
    NOTE_F4S                                        = 0x42,
    NOTE_G4                                         = 0x43,
    NOTE_G4S                                        = 0x44,
    NOTE_A4                                         = 0x45,
    NOTE_A4S                                        = 0x46,
    NOTE_B4                                         = 0x47,

    NOTE_C5                                         = 0x48,
    NOTE_C5S                                        = 0x49,
    NOTE_D5                                         = 0x4A,
    NOTE_D5S                                        = 0x4B,
    NOTE_E5                                         = 0x4C,
    NOTE_F5                                         = 0x4D,
    NOTE_F5S                                        = 0x4E,
    NOTE_G5                                         = 0x4F,
    NOTE_G5S                                        = 0x50,
    NOTE_A5                                         = 0x51,
    NOTE_A5S                                        = 0x52,
    NOTE_B5                                         = 0x53,
  };
}
