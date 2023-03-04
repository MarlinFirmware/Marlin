/* calculate continious memory regions for FLASH */
FLASH_ISR_ORIGIN = FLASH_START;
FLASH_ISR_LENGTH = 1024;
FLASH_ICG_ORIGIN = FLASH_ISR_ORIGIN + FLASH_ISR_LENGTH;
FLASH_ICG_LENGTH = 32;
FLASH_MAIN_ORIGIN = FLASH_ICG_ORIGIN + FLASH_ICG_LENGTH;
FLASH_MAIN_LENGTH = FLASH_SIZE - FLASH_MAIN_ORIGIN;

MEMORY
{
    FLASH_ISR   (rx): ORIGIN = FLASH_ISR_ORIGIN, LENGTH = FLASH_ISR_LENGTH
    FLASH_ICG   (rx): ORIGIN = FLASH_ICG_ORIGIN, LENGTH = FLASH_ICG_LENGTH
    FLASH       (rx): ORIGIN = FLASH_MAIN_ORIGIN, LENGTH = FLASH_MAIN_LENGTH
    RAM        (rwx): ORIGIN = RAM_START, LENGTH = RAM_SIZE
    RAM_RET    (rwx): ORIGIN = RAM_RET_START, LENGTH = RAM_RET_START
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
    } >FLASH

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH
    __exidx_end = .;

    .preinit_array :
    {
        /* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH

    .init_array :
    {
        /* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH

    .fini_array :
    {
        /* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

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
    } >RAM AT> FLASH

    __ram_ret_data_start = LOADADDR(.ram_ret_data);
    .ram_ret_data :
    {
        . = ALIGN(4);
        __data_start_ram_ret__ = .;
        *(.ram_ret_data)
        *(.ram_ret_data*)
        . = ALIGN(4);
        __data_end_ram_ret__ = .;
    } >RAM_RET AT> FLASH

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
