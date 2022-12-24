/*********************
 * registers_ft800.h *
 *********************/

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
 * This header defines registers for the FTDI FT800 LCD Driver chip.        *
 ****************************************************************************/

/*******************************************************************************
 * FT810                                                                       *
 *                                                                             *
 * START    END ADDR   SIZE    NAME           DESCRIPTION                      *
 *                                                                             *
 * 0x000000 0x03FFFF   256 kB  RAM_G          Main Graphics RAM                *
 *                                                                             *
 * 0x0C0000 0x0C0003     4  B  ROM_CHIPID     [0:1] 0x800   Chip Id            *
 *                                            [1:2] 0x0100  Vers ID            *
 *                                                                             *
 * 0x0BB23C 0x0FFFFB   275 kB  ROM_FONT       Font table and bitmap            *
 *                                                                             *
 * 0x0FFFFC 0x0FFFFF     4  B  ROM_FONT_ADDR  Font table pointer address       *
 *                                                                             *
 * 0x100000 0x101FFF     8 kB  RAM_DL         Display List RAM                 *
 *                                                                             *
 * 0x102000 0x1023FF     1 kB  RAM_PAL        Palette RAM                      *
 *                                                                             *
 * 0x102400 0x10257F   380  B  *          Registers                        *
 *                                                                             *
 * 0x108000 0x108FFF     4 kB  RAM_CMD        Command Buffer                   *
 *                                                                             *
 *******************************************************************************/

#pragma once

namespace FTDI {
  struct ft800_memory_map {

    //         MEMORY LOCATIONS     FT800
    static constexpr uint32_t RAM_G          = 0x000000;   // Main Graphics RAM
    static constexpr uint32_t ROM_CHIPID     = 0x0C0000;   // Chip ID/Version ID
    static constexpr uint32_t ROM_FONT       = 0x0BB23C;   // Font ROM
    static constexpr uint32_t ROM_FONT_ADDR  = 0x0FFFFC;   // Font Table Pointer
    static constexpr uint32_t RAM_DL         = 0x100000;   // Display List RAM
    static constexpr uint32_t RAM_PAL        = 0x102000;   // Palette RAM
    static constexpr uint32_t RAM_REG        = 0x102400;   // Registers
    static constexpr uint32_t RAM_CMD        = 0x108000;   // Command Buffer

    static constexpr uint32_t RAM_G_SIZE     = 256*1024L;  // 256k
  };

  struct ft800_registers {
    // REGISTERS AND ADDRESSES    FT800

    //             REGISTER              ADDRESS       SIZE    RESET VALUE     TYPE     DESCRIPTION

    static constexpr uint32_t ID                = 0x102400;  //    8    0x7C               r     Identification Register, Always 0x7C
    static constexpr uint32_t FRAMES            = 0x102404;  //   32    0x00000000         r     Frame Counter, Since Reset
    static constexpr uint32_t CLOCK             = 0x102408;  //   32    0x00000000         r     Clock cycles, Since Reset
    static constexpr uint32_t FREQUENCY         = 0x10240C;  //   28    0x03938700       r/w     Main Clock Frequency
    static constexpr uint32_t RENDERMODE        = 0x102410;  //    1    0x00             r/w     Rendering Mode: 0 = normal, 1 = single-line
    static constexpr uint32_t SNAPY             = 0x102414;  //   11    0x0000           r/w     Scan Line Select for RENDERMODE 1
    static constexpr uint32_t SNAPSHOT          = 0x102418;  //    1    -                  r     Trigger for RENDERMODE 1
    static constexpr uint32_t CPURESET          = 0x10241C;  //    3    0x02             r/w     RESET Bit2 Audio - Bit1 Touch - Bit0 Graphics
    static constexpr uint32_t TAP_CRC           = 0x102420;  //   32    -                  r     Live Video Tap
    static constexpr uint32_t TAP_MASK          = 0x102424;  //   32    0xFFFFFFFF       r/w     Live Video Tap Mask
    static constexpr uint32_t HCYCLE            = 0x102428;  //   12    0x224            r/w     Horizontal Total Cycle Count
    static constexpr uint32_t HOFFSET           = 0x10242C;  //   12    0x02B            r/w     Horizontal Display Start Offset
    static constexpr uint32_t HSIZE             = 0x102430;  //   12    0x1E0            r/w     Horizontal Display Pixel Count
    static constexpr uint32_t HSYNC0            = 0x102434;  //   12    0x000            r/w     Horizontal Sync Fall Offset
    static constexpr uint32_t HSYNC1            = 0x102438;  //   12    0x029            r/w     Horizontal Sync Rise Offset
    static constexpr uint32_t VCYCLE            = 0x10243C;  //   12    0x124            r/w     Vertical Total Cycle Count
    static constexpr uint32_t VOFFSET           = 0x102440;  //   12    0x00C            r/w     Vertical Display Start Offset
    static constexpr uint32_t VSIZE             = 0x102444;  //   12    0x110            r/w     Vertical Display Line Count
    static constexpr uint32_t VSYNC0            = 0x102448;  //   10    0x000            r/w     Vertical Sync Fall Offset
    static constexpr uint32_t VSYNC1            = 0x10244C;  //   10    0x00A            r/w     Vertical Sync Rise Offset
    static constexpr uint32_t DLSWAP            = 0x102450;  //    2    0x00             r/w     Display List Swap Control
    static constexpr uint32_t ROTATE            = 0x102454;  //    3    0x00             r/w     Screen 90,180, 270 degree rotate
    static constexpr uint32_t OUTBITS           = 0x102458;  //    9    0x1B6            r/w     Output Resolution, 3x3x3 Bits
    static constexpr uint32_t DITHER            = 0x10245C;  //    1    0x01             r/w     Output Dither Enable
    static constexpr uint32_t SWIZZLE           = 0x102460;  //    4    0x00             r/w     Output RGB Swizzle, Pin Change for PCB Routing
    static constexpr uint32_t CSPREAD           = 0x102464;  //    1    0x01             r/w     Output Clock Spreading Enable
    static constexpr uint32_t PCLK_POL          = 0x102468;  //    1    0x00             r/w     PCLK Polarity: 0 = Rising Edge, 1 = Falling Edge
    static constexpr uint32_t PCLK              = 0x10246C;  //    8    0x00             r/w     PCLK Frequency Divider, 0 = Disable Clock
    static constexpr uint32_t TAG_X             = 0x102470;  //   11    0x000            r/w     Tag Query X Coordinate
    static constexpr uint32_t TAG_Y             = 0x102474;  //   11    0x000            r/w     Tag Query Y Coordinate
    static constexpr uint32_t TAG               = 0x102478;  //    8    0x00               r     Tag Query Result
    static constexpr uint32_t VOL_PB            = 0x10247C;  //    8    0xFF             r/w     Audio Playback Volume
    static constexpr uint32_t VOL_SOUND         = 0x102480;  //    8    0xFF             r/w     Audio Synthesizer Volume
    static constexpr uint32_t SOUND             = 0x102484;  //   16    0x0000           r/w     Audio Sound Effect Select
    static constexpr uint32_t PLAY              = 0x102488;  //    1    0x00             r/w     Audio Start Effect Playback
    static constexpr uint32_t GPIO_DIR          = 0x10248C;  //    8    0x80             r/w     GPIO Pin Direction: 0 = Input , 1 = Output
    static constexpr uint32_t GPIO              = 0x102490;  //    8    0x00             r/w     GPIO Pin Values for 0, 1, 7 Drive Strength 2, 3, 4, 5, 6
    static constexpr uint32_t INT_FLAGS         = 0x102498;  //    8    0x00               r     Interrupt Flags, Clear by Reading
    static constexpr uint32_t INT_EN            = 0x10249C;  //    1    0x00             r/w     Global Interrupt Enable
    static constexpr uint32_t INT_MASK          = 0x1024A0;  //    8    0xFF             r/w     Interrupt Enable Mask
    static constexpr uint32_t PLAYBACK_START    = 0x1024A4;  //   20    0x00000          r/w     Audio Playback RAM Start Address
    static constexpr uint32_t PLAYBACK_LENGTH   = 0x1024A8;  //   20    0x00000          r/w     Audio Playback Sample Length (Bytes)
    static constexpr uint32_t PLAYBACK_READPTR  = 0x1024AC;  //   20    -                  r     Audio Playback Read Pointer
    static constexpr uint32_t PLAYBACK_FREQ     = 0x1024B0;  //   16    0x1F40           r/w     Audio Playback Frequency (Hz)
    static constexpr uint32_t PLAYBACK_FORMAT   = 0x1024B4;  //    2    0x00             r/w     Audio Playback Format
    static constexpr uint32_t PLAYBACK_LOOP     = 0x1024B8;  //    1    0x00             r/w     Audio Playback Loop Enable
    static constexpr uint32_t PLAYBACK_PLAY     = 0x1024BC;  //    1    0x00               r     Audio Start Playback
    static constexpr uint32_t PWM_HZ            = 0x1024C0;  //   14    0x00FA           r/w     Backlight PWM Frequency (Hz)
    static constexpr uint32_t PWM_DUTY          = 0x1024C4;  //    8    0x80             r/w     Backlight PWM Duty Cycle: 0 = 0%, 128 = 100%
    static constexpr uint32_t MACRO_0           = 0x1024C8;  //   32    0x00000000       r/w     Display List Macro Command 0
    static constexpr uint32_t MACRO_1           = 0x1024CC;  //   32    0x00000000       r/w     Display List Macro Command 1
    static constexpr uint32_t CMD_READ          = 0x1024E4;  //   12    0x000            r/w     Command Buffer Read Pointer
    static constexpr uint32_t CMD_WRITE         = 0x1024E8;  //   12    0x000            r/w     Command Buffer Write Pointer
    static constexpr uint32_t CMD_DL            = 0x1024EC;  //   13    0x0000           r/w     Command Display List Offset
    static constexpr uint32_t TOUCH_MODE        = 0x1024F0;  //    2    0x03             r/w     Touch-Screen Sampling Mode
    static constexpr uint32_t TOUCH_ADC_MODE    = 0x1024F4;  //    1    0x01             r/w     Select Single Ended or Differential Sampling
    static constexpr uint32_t TOUCH_CHARGE      = 0x1024F8;  //   16    0x1770           r/w     Touch Screen Charge Time, n x 6 Clocks
    static constexpr uint32_t TOUCH_SETTLE      = 0x1024FC;  //    4    0x03             r/w     Touch-Screen Settle Time, n x 6 Clocks
    static constexpr uint32_t TOUCH_OVERSAMPLE  = 0x102500;  //    4    0x07             r/w     Touch-Screen Oversample Factor
    static constexpr uint32_t TOUCH_RZTHRESH    = 0x102504;  //   16    0xFFFF           r/w     Touch-Screen Resistance Threshold
    static constexpr uint32_t TOUCH_RAW_XY      = 0x102508;  //   32    -                  r     Touch-Screen Raw (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_RZ          = 0x10250C;  //   16    -                  r     Touch-Screen Resistance
    static constexpr uint32_t TOUCH_SCREEN_XY   = 0x102510;  //   32    -                  r     Touch-Screen Screen (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_TAG_XY      = 0x102514;  //   32    -                  r     Touch-Screen Tag 0 Lookup (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_TAG         = 0x102518;  //    8    -                  r     Touch-Screen Tag 0 Result
    static constexpr uint32_t TOUCH_TRANSFORM_A = 0x10251C;  //   32    0x00010000       r/w     Touch-Screen Transform Coefficient A (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_B = 0x102520;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient B (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_C = 0x102524;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient C (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_D = 0x102528;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient D (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_E = 0x10252C;  //   32    0x00010000       r/w     Touch-Screen Transform Coefficient E (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_F = 0x102530;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient F (s15.16)
  //               Reserved Addresses      0x102434 - 0x102470
    static constexpr uint32_t TOUCH_DIRECT_XY   = 0x102574;  //   32    -                  r     Touch-Screen Direct Conversions XY (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_DIRECT_Z1Z2 = 0x102578;  //   32    -                  r     Touch-Screen Direct Conversions Z (z1-MSB16; z2-LSB16)
    static constexpr uint32_t TRACKER           = 0x109000;  //   32    0x00000000       r/w     Track Register (Track Value MSB16; Tag Value - LSB8)
  };
}
