## HC32F460 HAL

HAL for the HC32F460 MCU.

## Adding support for a new board

This HAL is designed to be generic enough to run on any HC32F460-based board.
To add support for a new board, you'll have to do a few things:

1. Add [the "normal" things](https://marlinfw.org/docs/development/boards.html#adding-a-new-board) Marlin requires (pins, Configuration.h, ...)
2. Determine the flash size your board uses
   - look at the chip soldered to your board and use the naming key in `hc32.ini`
   - extend `common_HC32F460C_variant` for 256K flash, or `common_HC32F460E_variant` for 512K flash
3. Determine the app start address of your board (see [below](#finding-the-app-start-address))
   - set `board_build.ld_args.flash_start` to the app start address once you've found it
   - if your board doesn't use a bootloader, you're out of luck for now.

### Finding the app start address

If your board boots using a bootloader, you'll have to find the address the bootloader jumps to after it's done. This can be done in multiple ways:

#### 1. using log messages

if you're lucky, the bootloader may print the app start address during boot.
to check for this, monitor the serial port when you power on the board, and look for a message like "Jumping to 0xC000" or "GotoApp->addr=0xC000".
The line will be printed before Marlin's "start" message.

Example:

```
[...]
version 1.2
sdio init success!
Disk init
Tips ------ None Firmware file
GotoApp->addr=0xC000

start
[...]
```

#### 2. using published source code

if the vendor has published the source code for Marlin, you can look at the bootloader source code and find the address there.
you should start your search with the following steps:

1. find the code that sets the vector table offset

the vector table offset is usually set using a line like this:

```c
SCB->VTOR = ((uint32_t) APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk);
```

just searching for `SCB->VTOR` should yield some results. From there, you just need to look at the value that's assigned to it. In the example it's `APP_START_ADDRESS`.

> [!NOTE]
> some vendors publish incomplete source code. But they sometimes leave version control related files in the repo, which can contain previous version of files that were removed.
> so be sure to include files in folders like `.git` or `.svn` in your search.

> [!NOTE]
> example is based on [Voxelab-64/Aquila_X2](https://github.com/Voxelab-64/Aquila_X2/blob/d1f23adf96920996b979bc31023d1dce236d05db/firmware/Sources/.svn/pristine/ec/ec82bcb480b511906bc3e6658450e3a803ab9813.svn-base#L96), who've actually included deleted files in their repo.

2. using a linker script

if the repository contains a linker script, look at the memory regions, specifically a region named `FLASH` or similar.
the ORIGIN of that region will be the app start address.

Example:

```ld
MEMORY
{
    FLASH       (rx): ORIGIN = 0x0000C000, LENGTH = 512K
    OTP         (rx): ORIGIN = 0x03000C00, LENGTH = 1020
    RAM        (rwx): ORIGIN = 0x1FFF8000, LENGTH = 188K
    RET_RAM    (rwx): ORIGIN = 0x200F0000, LENGTH = 4K
}
```

> [!NOTE]
> example is based on [Voxelab-64/Aquila_X2](https://github.com/Voxelab-64/Aquila_X2/blob/d1f23adf96920996b979bc31023d1dce236d05db/firmware/Sources/main/hdsc32core/hc32f46x_flash.ld#L55)

# Documentation on the HC32F460

due to uncertain licensing, documentation for the HC32F460 is only available on request.
documentation includes the following:

- datasheet, user manual, reference manual
- application notes for the DDL
- DDL source code
- IDE support packages (Keil, IAR, ...) including .svd files
- programming software
- emulator / debugger drivers
- development board documentation and schematics
- errata documents
- (limited) sales information
- full Voxelab firmware source code
- documents in chinese or english (machine translated)

contact me on discord (@shadow578) if you need it.

# Dependencies

This HAL depends on the following projects:

- [shadow578/platform-hc32f46x](https://github.com/shadow578/platform-hc32f46x) (platformio platform for HC32F46x)
- [shadow578/framework-arduino-hc32f46x](https://github.com/shadow578/framework-arduino-hc32f46x) (Arduino framework for HC32F46x)
- [shadow578/framework-hc32f46x-ddl](https://github.com/shadow578/framework-hc32f46x-ddl) (HC32F46x DDL framework)

# Credits

This HAL wouldn't be possible without the following projects:

- [Voxelab-64/Aquila_X2](https://github.com/Voxelab-64/Aquila_X2) (original implementation)
- [alexqzd/Marlin-H32](https://github.com/alexqzd/Marlin-H32) (misc. fixes to the original implementation)
- [kgoveas/Arduino-Core-Template](https://github.com/kgoveas/Arduino-Core-Template) (template for Arduino headers)
- [stm32duino/Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32) (misc. Arduino functions)
