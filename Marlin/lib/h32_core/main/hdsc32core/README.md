# About Memory Regions

## Stock

in the stock configuration, .text and .data sections were located in a single memory region `FLASH`:

```
FLASH containing .text and .data:
- start : 0x0000C420
- length: 0x34000 (208K)
- end   : 0x00040420
```

## New

to ensure that data is not executed, the .text and .data sections are located into separate memory regions `FLASH_RX` (read-execute) and `FLASH_R` (read). 

```
FLASH_RX containing .text:
- start : 0x0000C420
- length: (0x34000 (208K) - 0x7800 (30K)) = 0x2C800 (178K)
- end   : 0x00038C20
```
```
FLASH_R containing .data:
- start : (0x00040420 - 0x7800) = 0x00038C20
- length: 0x7800 (30K)
- end   : 0x00040420
```

> NOTE: in ld, 1K == 1024

> NOTE: I'm assuming that 30K are enough for the data sections. 
> if more is needed, the memory regions must be adjusted accordingly (shrink `FLASH_RX` and increase `FLASH_R`)



### FLASH_RX

- `.text`
- `.rodata`
- `.ARM.extab`
- `.ARM.exidx`


### FLASH_R

- `.preinit_array`
- `.init_array`
- `.fini_array`
- `.data`
- `.ram_ret_data`


### Sections Overview

```
/usr/share/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-readelf -lWS ./build/firmware.elf
There are 30 section headers, starting at offset 0xb54e58:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .vectors          PROGBITS        0000c000 00c000 000280 00   A  0   0  4
  [ 2] .text             PROGBITS        0000c420 00c420 0228e0 00  AX  0   0 16
  [ 3] .rodata           PROGBITS        0002ed00 02ed00 002864 00   A  0   0  8
  [ 4] .ARM.extab        PROGBITS        00031564 031564 000894 00   A  0   0  4
  [ 5] .ARM.exidx        ARM_EXIDX       00031df8 031df8 0013a0 00  AL  2   0  4
  [ 6] .init_array       INIT_ARRAY      00038c20 038c20 000028 04  WA  0   0  4
  [ 7] .fini_array       FINI_ARRAY      00038c48 038c48 000004 04  WA  0   0  4
  [ 8] .data             PROGBITS        1fff8000 048000 0004d4 00  WA  0   0  4
  [ 9] .ram_ret_data     PROGBITS        200f0000 0484d4 000000 00   W  0   0  1
  [10] .bss              NOBITS          1fff84d4 0484d4 00219c 00  WA  0   0  4
  [11] .ram_ret_bss      PROGBITS        200f0000 0484d4 000000 00   W  0   0  1
  [12] .heap             PROGBITS        1fffa670 0484d8 000c00 00      0   0  8
  [13] .stack_dummy      PROGBITS        1fffa670 0490d8 000400 00      0   0  8
  [14] .ARM.attributes   ARM_ATTRIBUTES  00000000 0494d8 00002a 00      0   0  1
  [15] .comment          PROGBITS        00000000 049502 0000b5 01  MS  0   0  1
  [16] .debug_info       PROGBITS        00000000 0495b7 8350ac 00      0   0  1
  [17] .debug_abbrev     PROGBITS        00000000 87e663 04091c 00      0   0  1
  [18] .debug_loclists   PROGBITS        00000000 8bef7f 02086f 00      0   0  1
  [19] .debug_aranges    PROGBITS        00000000 8df7f0 003db8 00      0   0  8
  [20] .debug_rnglists   PROGBITS        00000000 8e35a8 0065a3 00      0   0  1
  [21] .debug_line       PROGBITS        00000000 8e9b4b 065b68 00      0   0  1
  [22] .debug_str        PROGBITS        00000000 94f6b3 1a1a41 01  MS  0   0  1
  [23] .debug_frame      PROGBITS        00000000 af10f4 00db68 00      0   0  4
  [24] .debug_line_str   PROGBITS        00000000 afec5c 00025e 01  MS  0   0  1
  [25] .debug_ranges     PROGBITS        00000000 afeeba 000708 00      0   0  1
  [26] .debug_macro      PROGBITS        00000000 aff5c2 031546 00      0   0  1
  [27] .symtab           SYMTAB          00000000 b30b08 018530 10     28 4592  4
  [28] .strtab           STRTAB          00000000 b49038 00bcd3 00      0   0  1
  [29] .shstrtab         STRTAB          00000000 b54d0b 00014a 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), y (purecode), p (processor specific)

Elf file type is EXEC (Executable file)
Entry point 0x23acd
There are 6 program headers, starting at offset 52

Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  EXIDX          0x031df8 0x00031df8 0x00031df8 0x013a0 0x013a0 R   0x4
  LOAD           0x000000 0x00000000 0x00000000 0x0c280 0x0c280 R   0x10000
  LOAD           0x00c420 0x0000c420 0x0000c420 0x26d78 0x26d78 R E 0x10000
  LOAD           0x038c20 0x00038c20 0x00038c20 0x0002c 0x0002c RW  0x10000
  LOAD           0x048000 0x1fff8000 0x00038c4c 0x004d4 0x004d4 RW  0x10000
  LOAD           0x0084d4 0x1fff84d4 0x1fff84d4 0x00000 0x0219c RW  0x10000

 Section to Segment mapping:
  Segment Sections...
   00     .ARM.exidx
   01     .vectors
   02     .text .rodata .ARM.extab .ARM.exidx
   03     .init_array .fini_array
   04     .data
   05     .bss
```