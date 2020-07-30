/*********************
 * registers_ft810.h *
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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

/****************************************************************************
 * This header defines registers for the FTDI FT810 LCD Driver chip.        *
 ****************************************************************************/

/*******************************************************************************
 * FT810                                                                       *
 *                                                                             *
 * START    END ADDR   SIZE    NAME           DESCRIPTION                      *
 *                                                                             *
 * 0x000000 0x0FFFFF  1024 kB  RAM_G          Main Graphics RAM (0 to 1048572) *
 *                                                                             *
 * 0x0C0000 0x0C0003     4  B  ROM_CHIPID     [0:1] 0x800   Chip Id            *
 *                                            [1:2] 0x0100  Vers ID            *
 *                                                                             *
 * 0x1E0000 0x2FFFFB  1152 kB  ROM_FONT       Font table and bitmap            *
 *                                                                             *
 * 0x201EE0 0x2029DC  2812  B  ROM_FONT_ROOT  ROM font table                   *
 *                                                                             *
 * 0x2FFFFC 0x2FFFFF     4  B  ROM_FONT_ADDR  Font table pointer address       *
 *                                                                             *
 * 0x300000 0x301FFF     8 kB  RAM_DL         Display List RAM                 *
 *                                                                             *
 * 0x302000 0x302FFF     4 kB  *          Registers                        *
 *                                                                             *
 * 0x308000 0x308FFF     4 kB  RAM_CMD        Command Buffer                   *
 *                                                                             *
 *******************************************************************************/

#pragma once

namespace FTDI {
  struct ft810_memory_map {
    //         MEMORY LOCATIONS     FT810
    static constexpr uint32_t RAM_G          = 0x000000;   // Main Graphics RAM
    static constexpr uint32_t ROM_CHIPID     = 0x0C0000;   // Chip ID/Version ID
    static constexpr uint32_t ROM_FONT       = 0x1E0000;   // Font ROM
    static constexpr uint32_t ROM_FONT_ADDR  = 0x2FFFFC;   // Font Table Pointer
    static constexpr uint32_t RAM_DL         = 0x300000;   // Display List RAM
    static constexpr uint32_t RAM_REG        = 0x302000;   // Registers
    static constexpr uint32_t RAM_CMD        = 0x308000;   // Command Buffer

    static constexpr uint32_t RAM_G_SIZE     = 1024*1024l; // 1024k
  };

  struct ft810_registers {
    // REGISTERS AND ADDRESSES    FT810

    //             REGISTER              ADDRESS       SIZE    RESET VALUE     TYPE     DESCRIPTION

    static constexpr uint32_t ID                = 0x302000;  //    8    0x7C               r     Identification Register, Always 0x7C
    static constexpr uint32_t FRAMES            = 0x302004;  //   32    0x00000000         r     Frame Counter, Since Reset
    static constexpr uint32_t CLOCK             = 0x302008;  //   32    0x00000000         r     Clock cycles, Since Reset
    static constexpr uint32_t FREQUENCY         = 0x30200C;  //   28    0x03938700       r/w     Main Clock Frequency
    static constexpr uint32_t RENDERMODE        = 0x302010;  //    1    0x00             r/w     Rendering Mode: 0 = normal, 1 = single-line
    static constexpr uint32_t SNAPY             = 0x302014;  //   11    0x0000           r/w     Scan Line Select for RENDERMODE 1
    static constexpr uint32_t SNAPSHOT          = 0x302018;  //    1    -                  r     Trigger for RENDERMODE 1
    static constexpr uint32_t SNAPFORMAT        = 0x30201C;  //    6    0x20             r/w     Pixel Format for Scanline Readout
    static constexpr uint32_t CPURESET          = 0x302020;  //    3    0x02             r/w     RESET Bit2 Audio - Bit1 Touch - Bit0 Graphics
    static constexpr uint32_t TAP_CRC           = 0x302024;  //   32    -                  r     Live Video Tap
    static constexpr uint32_t TAP_MASK          = 0x302028;  //   32    0xFFFFFFFF       r/w     Live Video Tap Mask
    static constexpr uint32_t HCYCLE            = 0x30202C;  //   12    0x224            r/w     Horizontal Total Cycle Count
    static constexpr uint32_t HOFFSET           = 0x302030;  //   12    0x02B            r/w     Horizontal Display Start Offset
    static constexpr uint32_t HSIZE             = 0x302034;  //   12    0x1E0            r/w     Horizontal Display Pixel Count
    static constexpr uint32_t HSYNC0            = 0x302038;  //   12    0x000            r/w     Horizontal Sync Fall Offset
    static constexpr uint32_t HSYNC1            = 0x30203C;  //   12    0x029            r/w     Horizontal Sync Rise Offset
    static constexpr uint32_t VCYCLE            = 0x302040;  //   12    0x124            r/w     Vertical Total Cycle Count
    static constexpr uint32_t VOFFSET           = 0x302044;  //   12    0x00C            r/w     Vertical Display Start Offset
    static constexpr uint32_t VSIZE             = 0x302048;  //   12    0x110            r/w     Vertical Display Line Count
    static constexpr uint32_t VSYNC0            = 0x30204C;  //   10    0x000            r/w     Vertical Sync Fall Offset
    static constexpr uint32_t VSYNC1            = 0x302050;  //   10    0x00A            r/w     Vertical Sync Rise Offset
    static constexpr uint32_t DLSWAP            = 0x302054;  //    2    0x00             r/w     Display List Swap Control
    static constexpr uint32_t ROTATE            = 0x302058;  //    3    0x00             r/w     Screen 90,180, 270 degree rotate
    static constexpr uint32_t OUTBITS           = 0x30205C;  //    9    0x1B6            r/w     Output Resolution, 3x3x3 Bits
    static constexpr uint32_t DITHER            = 0x302060;  //    1    0x01             r/w     Output Dither Enable
    static constexpr uint32_t SWIZZLE           = 0x302064;  //    4    0x00             r/w     Output RGB Swizzle, Pin Change for PCB Routing
    static constexpr uint32_t CSPREAD           = 0x302068;  //    1    0x01             r/w     Output Clock Spreading Enable
    static constexpr uint32_t PCLK_POL          = 0x30206C;  //    1    0x00             r/w     PCLK Polarity: 0 = Rising Edge, 1 = Falling Edge
    static constexpr uint32_t PCLK              = 0x302070;  //    8    0x00             r/w     PCLK Frequency Divider, 0 = Disable Clock
    static constexpr uint32_t TAG_X             = 0x302074;  //   11    0x000            r/w     Tag Query X Coordinate
    static constexpr uint32_t TAG_Y             = 0x302078;  //   11    0x000            r/w     Tag Query Y Coordinate
    static constexpr uint32_t TAG               = 0x30207C;  //    8    0x00               r     Tag Query Result
    static constexpr uint32_t VOL_PB            = 0x302080;  //    8    0xFF             r/w     Audio Playback Volume
    static constexpr uint32_t VOL_SOUND         = 0x302084;  //    8    0xFF             r/w     Audio Synthesizer Volume
    static constexpr uint32_t SOUND             = 0x302088;  //   16    0x0000           r/w     Audio Sound Effect Select
    static constexpr uint32_t PLAY              = 0x30208C;  //    1    0x00             r/w     Audio Start Effect Playback
    static constexpr uint32_t GPIO_DIR          = 0x302090;  //    8    0x80             r/w     GPIO Pin Direction: 0 = Input , 1 = Output
    static constexpr uint32_t GPIO              = 0x302094;  //    8    0x00             r/w     GPIO Pin Values for 0, 1, 7 Drive Strength 2, 3, 4, 5, 6
    static constexpr uint32_t GPIOX_DIR         = 0x302098;  //   16    0x8000           r/w     Extended GPIO Pin Direction
    static constexpr uint32_t GPIOX             = 0x30209C;  //   16    0x0080           r/w     Extended GPIO Pin Values
    //             Reserved Addr           0x3020A0
    //             Reserved Addr           0x3020A4
    static constexpr uint32_t INT_FLAGS         = 0x3020A8;  //    8    0x00               r     Interrupt Flags, Clear by Reading
    static constexpr uint32_t INT_EN            = 0x3020AC;  //    1    0x00             r/w     Global Interrupt Enable
    static constexpr uint32_t INT_MASK          = 0x3020B0;  //    8    0xFF             r/w     Interrupt Enable Mask
    static constexpr uint32_t PLAYBACK_START    = 0x3020B4;  //   20    0x00000          r/w     Audio Playback RAM Start Address
    static constexpr uint32_t PLAYBACK_LENGTH   = 0x3020B8;  //   20    0x00000          r/w     Audio Playback Sample Length (Bytes)
    static constexpr uint32_t PLAYBACK_READPTR  = 0x3020BC;  //   20    -                  r     Audio Playback Read Pointer
    static constexpr uint32_t PLAYBACK_FREQ     = 0x3020C0;  //   16    0x1F40           r/w     Audio Playback Frequency (Hz)
    static constexpr uint32_t PLAYBACK_FORMAT   = 0x3020C4;  //    2    0x00             r/w     Audio Playback Format
    static constexpr uint32_t PLAYBACK_LOOP     = 0x3020C8;  //    1    0x00             r/w     Audio Playback Loop Enable
    static constexpr uint32_t PLAYBACK_PLAY     = 0x3020CC;  //    1    0x00               r     Audio Start Playback
    static constexpr uint32_t PWM_HZ            = 0x3020D0;  //   14    0x00FA           r/w     Backlight PWM Frequency (Hz)
    static constexpr uint32_t PWM_DUTY          = 0x3020D4;  //    8    0x80             r/w     Backlight PWM Duty Cycle: 0 = 0%, 128 = 100%
    static constexpr uint32_t MACRO_0           = 0x3020D8;  //   32    0x00000000       r/w     Display List Macro Command 0
    static constexpr uint32_t MACRO_1           = 0x3020DC;  //   32    0x00000000       r/w     Display List Macro Command 1
    //             Reserved Addr           0x3020E0
    //             Reserved Addr           0x3020E4
    //             Reserved Addr           0x3020E8
    //             Reserved Addr           0x3020EC
    //             Reserved Addr           0x3020F0
    //             Reserved Addr           0x3020F4
    static constexpr uint32_t CMD_READ          = 0x3020F8;  //   12    0x000            r/w     Command Buffer Read Pointer
    static constexpr uint32_t CMD_WRITE         = 0x3020FC;  //   12    0x000            r/w     Command Buffer Write Pointer
    static constexpr uint32_t CMD_DL            = 0x302100;  //   13    0x0000           r/w     Command Display List Offset
    static constexpr uint32_t TOUCH_MODE        = 0x302104;  //    2    0x03             r/w     Touch-Screen Sampling Mode
    static constexpr uint32_t TOUCH_ADC_MODE    = 0x302108;  //    1    0x01             r/w     Select Single Ended or Differential Sampling
    static constexpr uint32_t TOUCH_CHARGE      = 0x30210C;  //   16    0x1770           r/w     Touch Screen Charge Time, n x 6 Clocks
    static constexpr uint32_t TOUCH_SETTLE      = 0x302110;  //    4    0x03             r/w     Touch-Screen Settle Time, n x 6 Clocks
    static constexpr uint32_t TOUCH_OVERSAMPLE  = 0x302114;  //    4    0x07             r/w     Touch-Screen Oversample Factor
    static constexpr uint32_t TOUCH_RZTHRESH    = 0x302118;  //   16    0xFFFF           r/w     Touch-Screen Resistance Threshold
    static constexpr uint32_t TOUCH_RAW_XY      = 0x30211C;  //   32    -                  r     Touch-Screen Raw (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_RZ          = 0x302120;  //   16    -                  r     Touch-Screen Resistance
    static constexpr uint32_t TOUCH_SCREEN_XY   = 0x302124;  //   32    -                  r     Touch-Screen Screen (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_TAG_XY      = 0x302128;  //   32    -                  r     Touch-Screen Tag 0 Lookup (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_TAG         = 0x30212C;  //    8    -                  r     Touch-Screen Tag 0 Result
    static constexpr uint32_t TOUCH_TAG1_XY     = 0x302130;  //   32    -                  r     Touch-Screen Tag 1 Lookup
    static constexpr uint32_t TOUCH_TAG1        = 0x302134;  //    8    -                  r     Touch-Screen Tag 1 Result
    static constexpr uint32_t TOUCH_TAG2_XY     = 0x302138;  //   32    -                  r     Touch-Screen Tag 2 Lookup
    static constexpr uint32_t TOUCH_TAG2        = 0x30213C;  //    8    -                  r     Touch-Screen Tag 2 Result
    static constexpr uint32_t TOUCH_TAG3_XY     = 0x302140;  //   32    -                  r     Touch-Screen Tag 3 Lookup
    static constexpr uint32_t TOUCH_TAG3        = 0x302144;  //    8    -                  r     Touch-Screen Tag 3 Result
    static constexpr uint32_t TOUCH_TAG4_XY     = 0x302148;  //   32    -                  r     Touch-Screen Tag 4 Lookup
    static constexpr uint32_t TOUCH_TAG4        = 0x30214C;  //    8    -                  r     Touch-Screen Tag 4 Result
    static constexpr uint32_t TOUCH_TRANSFORM_A = 0x302150;  //   32    0x00010000       r/w     Touch-Screen Transform Coefficient A (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_B = 0x302154;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient B (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_C = 0x302158;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient C (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_D = 0x30215C;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient D (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_E = 0x302160;  //   32    0x00010000       r/w     Touch-Screen Transform Coefficient E (s15.16)
    static constexpr uint32_t TOUCH_TRANSFORM_F = 0x302164;  //   32    0x00000000       r/w     Touch-Screen Transform Coefficient F (s15.16)
    static constexpr uint32_t TOUCH_CONFIG      = 0x302168;  //   16    0x8381           r/w     Touch Configuration
    static constexpr uint32_t CTOUCH_TOUCH4_X   = 0x30216C;  //   16    -                  r     Extended Mode Touch Screen
    //             Reserved Addresses      0x302170
    static constexpr uint32_t BIST_EN           = 0x302174;  //    1    0                r/w     BIST Memory Mapping Enable
    //             Reserved Addr           0x302178
    //             Reserved Addr           0x30217C
    static constexpr uint32_t TRIM              = 0x302180;  //    8    0                r/w     Internal Clock Trimming
    static constexpr uint32_t ANA_COMP          = 0x302184;  //    8    0                r/w     Analog Control Register
    static constexpr uint32_t SPI_WIDTH         = 0x302188;  //    3    0                r/w     QSPI Bus Width Setting
    static constexpr uint32_t TOUCH_DIRECT_XY   = 0x30218C;  //   32    -                  r     Touch-Screen Direct Conversions XY (x-MSB16; y-LSB16)
    static constexpr uint32_t TOUCH_DIRECT_Z1Z2 = 0x302190;  //   32    -                  r     Touch-Screen Direct Conversions Z (z1-MSB16; z2-LSB16)
    //             Reserved Addresses      0x302194 - 0x302560
    static constexpr uint32_t DATESTAMP         = 0x320564;  //  128    -                  r     Stamp Date Code
    static constexpr uint32_t CMDB_SPACE        = 0x302574;  //   12    0xFFC            r/w     Command DL Space Available
    static constexpr uint32_t CMDB_WRITE        = 0x302578;  //   32    0                  w     Command DL Write

    static constexpr uint32_t TRACKER           = 0x309000;  //   32    0x00000000       r/w     Track Register (Track Value MSB16; Tag Value - LSB8)
    static constexpr uint32_t TRACKER_1         = 0x309004;  //   32    0x00000000       r/w     Track Register (Track Value MSB16; Tag Value - LSB8)
    static constexpr uint32_t TRACKER_2         = 0x309008;  //   32    0x00000000       r/w     Track Register (Track Value MSB16; Tag Value - LSB8)
    static constexpr uint32_t TRACKER_3         = 0x30900C;  //   32    0x00000000       r/w     Track Register (Track Value MSB16; Tag Value - LSB8)
    static constexpr uint32_t TRACKER_4         = 0x309010;  //   32    0x00000000       r/w     Track Register (Track Value MSB16; Tag Value - LSB8)

    static constexpr uint32_t MEDIAFIFO_READ    = 0x309014;  //   32    0x00000000       r/w     Media FIFO read pointer
    static constexpr uint32_t MEDIAFIFO_WRITE   = 0x309018;  //   32    0x00000000       r/w     Media FIFO write pointer
  };
}
