/**
 * Linker script for Marlin on the AT91SAM3U4E (4pi)
 *
 * Flash: 256KB at 0x00080000, as two 128KB planes
 *          bank 0 = 0x00080000..0x0009FFFF
 *          bank 1 = 0x000A0000..0x000BFFFF
 * SRAM:  48KB at 0x20000000
 *        (the part is quoted as 52KB, but 4KB of that is the NAND Flash
 *         Controller's buffer at a separate address and is not used here)
 *
 * The top 4KB of flash is carved out for EEPROM emulation, so the linker can
 * never place code where HAL/SAM3U/eeprom/eeprom_flash.cpp writes. That region
 * lies in bank 1, which lets the flash controller program it while code runs
 * from bank 0.
 *
 * RAM is laid out the conventional Cortex-M way: .data and .bss at the bottom,
 * the heap growing up from there, and the stack growing down from the top of
 * SRAM. MarlinHAL::freeMemory() reports the gap between the two.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

/* Size of the EEPROM emulation region, reserved at the top of flash.
   Must match MARLIN_EEPROM_SIZE and be a multiple of the 256-byte page. */
EEPROM_SIZE = DEFINED(EEPROM_SIZE) ? EEPROM_SIZE : 0x1000;

MEMORY
{
  rom    (rx)  : ORIGIN = 0x00080000, LENGTH = 0x00040000 - 0x1000
  eeprom (r)   : ORIGIN = 0x00080000 + 0x00040000 - 0x1000, LENGTH = 0x1000
  ram    (rwx) : ORIGIN = 0x20000000, LENGTH = 0x0000C000
}

/* Start of the EEPROM region, used by eeprom_flash.cpp */
_seeprom = ORIGIN(eeprom);
_eeeprom = ORIGIN(eeprom) + LENGTH(eeprom);

/* Minimum headroom the heap and stack must be able to claim */
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x1000;

ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))
        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(0x4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > rom

    /* .ARM.exidx is sorted, so has to go in its own output section. */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > rom
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);
    _etext = .;

    /* Initialized data, plus the routines that must execute from RAM.
       eeprom_flash.cpp puts its flash-programming routine in .ramfunc,
       because the flash cannot be read while it is being written. */
    .relocate : AT (_etext)
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*)
        *(.data .data.*)
        . = ALIGN(4);
        _erelocate = .;
    } > ram

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        _ezero = .;
    } > ram

    /* The heap starts here and grows up; _sbrk() hands out memory from it. */
    . = ALIGN(8);
    _end = .;
    PROVIDE(end = .);
    _sheap = .;

    /* The stack grows down from the top of SRAM. Reserve STACK_SIZE below it
       so the link fails loudly if .data + .bss leave no room, rather than
       silently letting the stack collide with the heap at runtime. */
    _estack = ORIGIN(ram) + LENGTH(ram);
    _sstack = _estack - STACK_SIZE;
    _eheap = _sstack;

    ASSERT(_sheap <= _sstack, "Not enough SRAM: .data + .bss leave no room for the stack.")

    _ram_end_ = ORIGIN(ram) + LENGTH(ram) - 1;
}
