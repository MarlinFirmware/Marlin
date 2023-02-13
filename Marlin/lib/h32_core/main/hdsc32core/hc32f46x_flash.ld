/*****************************************************************************/
/*  File        hc32f46x_flash.ld                                            */
/*  Abstract    Linker script for HC32F46x Device with                       */
/*              xxxKByte FLASH, 192KByte RAM                                 */ 
/*****************************************************************************/

/* Use contiguous memory regions for simple. */
/* seperate .text (FLASH_RX) from .data (FLASH_R) sections to prevent execution of data */
MEMORY
{
    FLASH_ISR   (rx): ORIGIN = 0x0000C000, LENGTH = 1024
    FLASH_ICG   (rx): ORIGIN = 0x0000C400, LENGTH = 32
    FLASH_RX    (rx): ORIGIN = 0x0000C420, LENGTH = 178K
    FLASH_R      (r): ORIGIN = 0x00038C20, LENGTH = 30K
    RAM        (rwx): ORIGIN = 0x1FFF8000, LENGTH = 188K
    RAM_RET    (rwx): ORIGIN = 0x200F0000, LENGTH = 4K
}

ENTRY(Reset_Handler)

SECTIONS
{
    .vectors :
    {
        KEEP(*(.vectors))
    } >FLASH_ISR

    .icg_sec :
    {
        KEEP(*(.icg_sec))
    } >FLASH_ICG

    .text :
    {
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)

        KEEP(*(.init))
        KEEP(*(.fini))
    } >FLASH_RX

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH_RX

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH_RX

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH_RX
    __exidx_end = .;

    .preinit_array :
    {
        /* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH_R

    .init_array :
    {
        /* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH_R

    .fini_array :
    {
        /* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH_R

    /*__etext = ALIGN(4);
    .data : AT (__etext)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM*/

    __etext = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM AT> FLASH_R

    __ram_ret_data_start = LOADADDR(.ram_ret_data);
    .ram_ret_data :
    {
        . = ALIGN(4);
        __data_start_ram_ret__ = .;
        *(.ram_ret_data)
        *(.ram_ret_data*)
        . = ALIGN(4);
        __data_end_ram_ret__ = .;
    } >RAM_RET AT> FLASH_R

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } >RAM

    .ram_ret_bss :
    {
        . = ALIGN(4);
        __bss_start_ram_ret__ = .;
        *(.ram_ret_bss)
        *(.ram_ret_bss*)
        . = ALIGN(4);
        __bss_end_ram_ret__ = .;
    } >RAM_RET

    .heap (COPY) :
    {
        __end__ = .;
        PROVIDE(end = .);
        *(.heap*)
        __HeapLimit = .;
    } >RAM

    .stack_dummy (COPY) :
    {
        *(.stack*)
    } >RAM

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(_stack = __StackTop);

    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
